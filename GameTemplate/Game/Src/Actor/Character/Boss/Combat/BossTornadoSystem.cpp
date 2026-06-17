#include "stdafx.h"
#include "BossTornadoSystem.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include <random>
#include <cmath>


namespace
{
	/* 竜巻。m_tornadoes[] の要素数 4 と同期すること */
	static constexpr int   MAX_TORNADOES = 4;					//! 同時に存在可能な竜巻の最大数。
	static constexpr float STAGE_GROUND_Y = 50.0f;				//! ステージ地面の Y 座標。
	static constexpr float TORNADO_SPEED = 62.0f;				//! 竜巻の移動速度。
	static constexpr float TORNADO_BASE_SCALE = 5.0f;			//! 竜巻の基本スケール。エフェクトの大きさに影響。
	static constexpr float TORNADO_EFFECT_Y_OFFSET = 0.0f;	    //! 竜巻エフェクトの Y オフセット。竜巻の論理座標からエフェクト表示位置への高さ調整。
	static constexpr float TORNADO_EFFECT_Z_OFFSET = -15.0f;	//! 竜巻エフェクトの Z オフセット。竜巻の論理座標からエフェクト表示位置への前後調整。
	static constexpr float TORNADO_STAGE_MIN_X = -110.0f;		//! 竜巻の移動範囲最小 X。ステージ左端からのマージンを考慮。
	static constexpr float TORNADO_STAGE_MAX_X = 110.0f;		//! 竜巻の移動範囲最大 X。ステージ右端からのマージンを考慮。
	static constexpr float TORNADO_STAGE_EXIT_MARGIN = 30.0f;   //! 竜巻がステージ外に出たと判定するマージン。ステージ端からこの距離だけさらに出たら消滅扱い。
	static constexpr float TORNADO_PUSH_LIFE = 4.5f;			//! 竜巻の押し出しパターンの寿命。
	static constexpr float TORNADO_PASS_LIFE = 4.0f;			//! 竜巻の通過パターンの寿命。
	static constexpr float TORNADO_WANDER_LIFE = 5.0f;			//! 竜巻の徘徊パターンの寿命。
	static constexpr float TORNADO_WANDER_RANGE = 55.0f;		//! 竜巻の徘徊パターンの振れ幅（中心からの最大距離）。
	static constexpr float TORNADO_WANDER_FREQ = 2.8f;			//! 竜巻の徘徊パターンの振れの速さ（大きいほど速く振れる）。
	static constexpr float TORNADO_DAMAGE_INTERVAL = 0.45f;		//! 竜巻がダメージを与える間隔。
	static constexpr float TORNADO_DAMAGE_RATE = 0.65f;			//! 竜巻のダメージ倍率。近接攻撃の何割ダメージを与えるか。
	static constexpr float TORNADO_WANDER_SPAWN_RANGE = 70.0f;	//! 竜巻の徘徊パターンのスポーン範囲。プレイヤーからこの距離以内にスポーンする。
	static constexpr float TORNADO_SPAWN_EFFECT_SCALE = 2.0f;   //! 竜巻スポーンエフェクトのスケール。エフェクトの大きさに影響。
	static constexpr float TORNADO_HIT_RADIUS = 38.0f;
}


namespace nsApp
{
	namespace nsActor
	{
		void BossTornadoSystem::Init()
		{
			/* ヒット判定の初期化 */
			m_TornadoHit.Init(TORNADO_HIT_RADIUS);
		}


		Vector3 BossTornadoSystem::GetTornadoEffectPosition(const Vector3& logicPos) const
		{
			/* 竜巻の論理座標からエフェクト表示位置へのオフセットを適用 */
			Vector3 pos = logicPos;
			pos.y += TORNADO_EFFECT_Y_OFFSET;
			pos.z = TORNADO_EFFECT_Z_OFFSET;
			return pos;
		}


		void BossTornadoSystem::SpawnFromTail(Boss* boss, const Vector3& tailHitPos)
		{
			/* 尻尾攻撃位置から竜巻を生成。Boss::AttackTail() から呼ばれる。 */
			(void)tailHitPos;

			/* ボスタイプとターゲットの有無をチェック。これらが満たされない場合は竜巻を生成しない。 */
			if (boss->GetBossType() != CharacterModelType::GreenDragon)
				return;

			/* ターゲットの取得と生存チェック。ターゲットが存在しないか死んでいる場合は竜巻を生成しない。 */
			ICharacter* target = boss->GetTarget();
			if (target == nullptr || target->IsDead())
				return;

			/* 竜巻の移動パターンをランダムに選択。これにより、同じ攻撃でも毎回異なる動きをするため、プレイヤーは常に注意を払う必要がある。 */
			static std::mt19937 rng(std::random_device{}());
			std::uniform_int_distribution<int> patternDist(0, 2);
			const TornadoMoveType pattern = static_cast<TornadoMoveType>(patternDist(rng));

			/* 空きスロット検索 */
			auto findSlot = [&]() -> BossTornado*
				{
					for (int i = 0; i < MAX_TORNADOES; i++)
					{
						if (!m_tornadoes[i].active)
							return &m_tornadoes[i];
					}
					return nullptr;
				};

			/* スロット初期化 */
			auto initSlot = [&](BossTornado* slot, const Vector3& logicPos, const Vector3& velocity, TornadoMoveType type, float life, float wanderAnchorX = 0.0f)
				{
					slot->active = true;
					slot->moveType = type;
					slot->pos = logicPos;
					slot->pos.y = STAGE_GROUND_Y;
					slot->pos.z = 0.0f;
					slot->velocity = velocity;
					slot->lifeTimer = life;
					slot->wanderAnchorX = wanderAnchorX;
					slot->wanderPhase = 0.0f;
					slot->damageTimer = 0.0f;
					slot->effectRespawnTimer = 0.0f;
					slot->baseScale = TORNADO_BASE_SCALE;
					slot->emitter = NewGO<nsK2EngineLow::EffectEmitter>(0, "bossTornado");
					slot->emitter->Init(nsEffect::Effect_ID::ShockWave);
					slot->emitter->SetPosition(GetTornadoEffectPosition(slot->pos));
					slot->emitter->SetScale(Vector3::One * TORNADO_SPAWN_EFFECT_SCALE);
					slot->emitter->Play();
				};

			const float centerZ = 0.0f;

			switch (pattern)
			{
			case TornadoMoveType::DualPush:
			{
				/* 左右から同時に押し出し */
				BossTornado* leftSlot = findSlot();
				BossTornado* rightSlot = findSlot();

				/* 左右両方のスロットが必要なため、どちらかが見つからない場合は生成を中止。これにより、竜巻が片方だけ出現することを防ぎ、攻撃の見た目と挙動の一貫性を保つことができる。 */
				if (leftSlot == nullptr || rightSlot == nullptr)
					return;

				Vector3 leftSpawn(TORNADO_STAGE_MIN_X, STAGE_GROUND_Y, centerZ);
				Vector3 rightSpawn(TORNADO_STAGE_MAX_X, STAGE_GROUND_Y, centerZ);
				initSlot(leftSlot, leftSpawn, Vector3(TORNADO_SPEED, 0.0f, 0.0f), TornadoMoveType::DualPush, TORNADO_PUSH_LIFE);
				initSlot(rightSlot, rightSpawn, Vector3(-TORNADO_SPEED, 0.0f, 0.0f), TornadoMoveType::DualPush, TORNADO_PUSH_LIFE);
				break;
			}

			case TornadoMoveType::SidePass:
			{
				/* 片側から通過 */
				BossTornado* slot = findSlot();
				if (slot == nullptr)
					return;

				std::uniform_int_distribution<int> sideDist(0, 1);
				const float sideSign = (sideDist(rng) == 0) ? -1.0f : 1.0f;
				Vector3 spawnPos;
				Vector3 velocity;

				if (sideSign < 0.0f)
				{
					/* 竜巻のスポーン位置をステージの左右端に設定。移動速度は、スポーンした側から反対側へ向かうように設定。これにより、竜巻はステージの片側から出現して反対側へ通過する動きをするようになる。 */
					spawnPos = Vector3(TORNADO_STAGE_MIN_X, STAGE_GROUND_Y, centerZ);
					velocity = Vector3(TORNADO_SPEED, 0.0f, 0.0f);
				}
				else
				{
					/* 竜巻のスポーン位置をステージの左右端に設定。移動速度は、スポーンした側から反対側へ向かうように設定。これにより、竜巻はステージの片側から出現して反対側へ通過���る動きをするようになる。 */
					spawnPos = Vector3(TORNADO_STAGE_MAX_X, STAGE_GROUND_Y, centerZ);
					velocity = Vector3(-TORNADO_SPEED, 0.0f, 0.0f);
				}

				/* スロットを初期化して竜巻をスポーン。移動速度は左右どちらかから中心に向かうように設定。寿命は TORNADO_PASS_LIFE に設定。これにより、竜巻はステージの片側から出現して反対側へ通過する動きをするようになる。 */
				initSlot(slot, spawnPos, velocity, TornadoMoveType::SidePass, TORNADO_PASS_LIFE);
				break;
			}

			case TornadoMoveType::StageWander:
			default:
			{
				/* ターゲット周辺を sin 波で徘徊 */
				BossTornado* slot = findSlot();
				if (slot == nullptr)
					return;

				/* ターゲットの X 座標を中心に、TORNADO_WANDER_SPAWN_RANGE の範囲内でランダムにスポーン位置を決定。これにより、竜巻は常にプレイヤーの近くに出現するが、毎回同じ位置から出るわけではないため、プレイヤーは常に注意を払う必要がある。 */
				const float centerX = target->GetPosition().x;
				/* 竜巻のスポーン位置がステージ内に収まるように、中心 X を基準にスポーン範囲を調整。これにより、竜巻がステージ外に出現することを防ぎ、常にプレイヤーが対処できる位置から出るようになる。 */
				std::uniform_real_distribution<float> xDist( centerX - TORNADO_WANDER_SPAWN_RANGE, centerX + TORNADO_WANDER_SPAWN_RANGE);
				/* スポーン位置の X 座標をランダムに決定し、Y 座標は地面の高さ、Z 座標は 0 に固定してスポーン位置を作成。これにより、竜巻は常に地面に沿って移動し、プレイヤーが予測しやすい動きをするようになる。 */
				const float anchorX = xDist(rng);
				/* スポーン位置がステージ内に収まるように、X 座標をステージの移動範囲内にクランプ。これにより、竜巻がステージ外に出現することを防ぎ、常にプレイヤーが対処できる位置から出るようになる。 */
				Vector3 spawnPos(anchorX, STAGE_GROUND_Y, centerZ);
				/* スロットを初期化して竜巻をスポーン。移動速度はゼロで、徘徊パターンとして設定。寿命は TORNADO_WANDER_LIFE に設定。wanderAnchorX はスポーン位置の X 座標に設定して、徘徊の中心点とする。これにより、竜巻はスポーン位置を中心に左右に振れながら移動するようになる。 */
				initSlot(slot, spawnPos, Vector3::Zero, TornadoMoveType::StageWander, TORNADO_WANDER_LIFE, anchorX);
				break;
			}
			}
		}


		void BossTornadoSystem::Update(Boss* boss)
		{
			if (boss->GetBossType() != CharacterModelType::GreenDragon)
				return;

			float dt = g_gameTime->GetFrameDeltaTime();
			const int baseDamage = static_cast<int>(boss->GetCharacterStatus().attack.normalDamage);
			const int tickDamage = static_cast<int>(baseDamage * TORNADO_DAMAGE_RATE);

			for (int i = 0; i < MAX_TORNADOES; i++)
			{
				BossTornado& t = m_tornadoes[i];
				if (!t.active)
					continue;

				t.lifeTimer -= dt;
				t.damageTimer -= dt;

				/* 移動パターン別更新 */
				switch (t.moveType)
				{
				case TornadoMoveType::DualPush:
				case TornadoMoveType::SidePass:
					t.pos += t.velocity * dt;
					t.pos.y = STAGE_GROUND_Y;
					t.pos.z = 0.0f;
					break;

				case TornadoMoveType::StageWander:
					t.wanderPhase += dt;
					t.pos.x = t.wanderAnchorX + std::sin(t.wanderPhase * TORNADO_WANDER_FREQ) * TORNADO_WANDER_RANGE;
					t.pos.y = STAGE_GROUND_Y;
					t.pos.z = 0.0f;
					break;
				}

				/* エフェクト追従・再生成 */
				if (t.emitter == nullptr)
				{
					t.emitter = NewGO<nsK2EngineLow::EffectEmitter>(0, "bossTornado");
					t.emitter->Init(nsEffect::Effect_ID::ShockWave);
					t.emitter->SetPosition(GetTornadoEffectPosition(t.pos));
					t.emitter->SetRotation(Quaternion::Identity);
					t.emitter->SetScale(Vector3::One * TORNADO_BASE_SCALE);
					t.emitter->Play();
				}
				else
				{
					t.emitter->SetPosition(GetTornadoEffectPosition(t.pos));
					if (!t.emitter->IsPlay())
					{
						t.emitter->Stop();
						t.emitter = nullptr;
					}
				}

				/* ヒット判定更新 */
				m_TornadoHit.Disable();
				m_TornadoHit.Enable();
				m_TornadoHit.Update(t.pos);

				/* ダメージ間隔ごとに命中判定 */
				if (t.damageTimer <= 0.0f)
				{
					for (auto* target : boss->GetAllTargets())
					{
						if (target == nullptr || target->IsDead())
							continue;

						if (m_TornadoHit.IsHit(target))
						{
							DamageProcessor::ApplyDamageToTarget(target, tickDamage);
							t.damageTimer = TORNADO_DAMAGE_INTERVAL;
							break;
						}
					}
				}

				/* 寿命切れ・画面外で消滅 */
				const bool outOfStage = t.pos.x < (TORNADO_STAGE_MIN_X - TORNADO_STAGE_EXIT_MARGIN) || t.pos.x >(TORNADO_STAGE_MAX_X + TORNADO_STAGE_EXIT_MARGIN);

				if (t.lifeTimer <= 0.0f || ((t.moveType == TornadoMoveType::DualPush || t.moveType == TornadoMoveType::SidePass) && outOfStage))
				{
					if (t.emitter != nullptr)
					{
						t.emitter->Stop();
						t.emitter = nullptr;
					}
					t.active = false;
				}
			}
		}
	}
}