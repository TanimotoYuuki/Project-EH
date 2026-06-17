#include "stdafx.h"
#include "BossFireCombatSystem.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include <random>

namespace
{
	/* Rain Drop 最大数。m_rainDrops[] の要素数と同期すること。*/
	static constexpr int MAX_RAIN_DROPS = 16;

	/* 火球飛翔 */
	static constexpr float FIREBALL_SPEED = 85.0f;              //! 火球速度。
	static constexpr float FIREBALL_LIFE = 3.0f;                //! 火球寿命（秒）。

	/* 口位置オフセット */
	static constexpr float MOUTH_FORWARD = 110.0f;              //! 口先方向オフセット。
	static constexpr float MOUTH_HEIGHT = 90.0f;                //! 口高さオフセット。
	static constexpr float AIM_HEIGHT_OFFSET = 30.0f;           //! 照準Y補正（胸〜上半身）。

	/* ステージ地面・着弾炎柱 */
	static constexpr float STAGE_GROUND_Y = 50.0f;              //! キャラ足元Y。
	static constexpr float LANDING_EFFECT_Y_OFFSET = -15.0f;    //! 炎柱表示Y補正。
	static constexpr float LANDING_EFFECT_DURATION = 3.0f;      //! 火球着弾後の炎柱持続（秒）。
	static constexpr float LANDING_EFFECT_SPAWN_INTERVAL = 0.8f;  //! 炎柱エフェクト再生成間隔（秒）。
	static constexpr float RAIN_LANDING_DURATION = 2.5f;        //! Rain着弾後の炎柱持続（秒）。

	/* 飛翔中火球エフェクト（当たり判定と分離） */
	static constexpr float FIREBALL_EFFECT_Y_OFFSET = 0.0f;     //! 上下補正。
	static constexpr float FIREBALL_EFFECT_Z_OFFSET = -10.0f;   //! 手前→奥補正。
	static constexpr float FIREBALL_EFFECT_SCALE = 6.0f;        //! 火球エフェクトスケール。
	static constexpr float RAIN_EFFECT_SCALE = 5.0f;            //! Rainエフェクトスケール。
	static constexpr float LANDING_PILLAR_SCALE = 5.0f;         //! 着弾炎柱スケール。

	/* Burst */
	static constexpr float BURST_SHOT_INTERVAL = 0.35f;         //! 3連射間隔（秒）。
	static constexpr int   BURST_SHOT_COUNT = 3;                //! 連射数。

	/* Rain */
	static constexpr float RAIN_DURATION = 4.0f;                //! 降り注ぎ時間（秒）。
	static constexpr float RAIN_SPAWN_INTERVAL = 0.75f;         //! Drop生成間隔（秒）。
	static constexpr float RAIN_SPAWN_HEIGHT = 260.0f;          //! 生成高さオフセット。
	static constexpr float RAIN_FALL_SPEED = 120.0f;            //! 降下速度。

	/* 火球ヒット判定半径 */
	static constexpr float FIRE_HIT_RADIUS = 15.0f;             //! 飛翔中火球の当たり半径。
	static constexpr float LANDING_HIT_RADIUS = 35.0f;          //! 着弾炎柱の当たり半径。
	static constexpr float RAIN_HIT_RADIUS = 18.0f;             //! Rain Drop の当たり半径。

	/* 照準・方向 */
	static constexpr float MIN_DIRECTION_LENGTH_SQ = 0.01f;     //! 方向ベクトルが無効とみなす長さの2乗。
	static constexpr float DEFAULT_AIM_DIR_X = -1.0f;           //! 方向未設定時のフォールバック X。
	static constexpr float DEFAULT_AIM_DIR_Y = 0.0f;            //! 方向未設定時のフォールバック Y。
	static constexpr float DEFAULT_AIM_DIR_Z = 0.0f;            //! 方向未設定時のフォールバック Z。

	/* 火炎パターン抽選（Straight / Burst / Rain） */
	static constexpr int FIRE_PATTERN_ROLL_MIN = 0;             //! パターン抽選の下限。
	static constexpr int FIRE_PATTERN_ROLL_MAX = 2;             //! パターン抽選の上限。

	/* Rain スポーン位置 */
	static constexpr int RAIN_SPAWN_X_OFFSET_MIN = -120;      //! ターゲット基準 X オフセット最小。
	static constexpr int RAIN_SPAWN_X_OFFSET_MAX = 120;       //! ターゲット基準 X オフセット最大。

	/* ステージ座標 */
	static constexpr float STAGE_CENTER_Z = 0.0f;               //! ステージ中央 Z（着弾判定用）。
	static constexpr float LANDING_POS_UNSET_Y = 0.0f;          //! 着弾位置未設定とみなす Y 閾値。

	/* Rain Drop 初速（Y のみ降下） */
	static constexpr float RAIN_VELOCITY_X = 0.0f;              //! Rain Drop 水平速度 X。
	static constexpr float RAIN_VELOCITY_Z = 0.0f;              //! Rain Drop 水平速度 Z。
}

namespace nsApp
{
	namespace nsActor
	{
		void BossFireCombatSystem::Init()
		{
			m_FireHit.Init(FIRE_HIT_RADIUS);
			m_LandingHit.Init(LANDING_HIT_RADIUS);
			m_RainHit.Init(RAIN_HIT_RADIUS);
		}

		void BossFireCombatSystem::Update(Boss* boss)
		{
			if (boss == nullptr)
				return;

			UpdateBurst(boss, g_gameTime->GetFrameDeltaTime());

			if (m_fireballActive)
			{
				m_fireballLifeTime -= g_gameTime->GetFrameDeltaTime();
				m_fireballPos += m_fireballVelocity * g_gameTime->GetFrameDeltaTime();

				if (m_fireballEmitter != nullptr)
					m_fireballEmitter->SetPosition(GetFireballEffectPosition(m_fireballPos));

				m_FireHit.Update(m_fireballPos);

				for (auto* target : boss->GetAllTargets())
				{
					if (target == nullptr || target->IsDead())
						continue;

					if (m_FireHit.IsHit(target))
					{
						DamageProcessor::ApplyDamageToTarget(
							target, static_cast<int>(boss->GetCharacterStatus().attack.normalDamage));
						m_landingPos = GetLandingEffectPosition(target->GetPosition());
						DeleteFireball(boss);
						break;
					}
				}

				if (m_fireballActive && m_fireballLifeTime <= 0.0f)
					DeleteFireball(boss);
			}

			UpdateRain(boss);
			UpdateRainDrops(boss);
			UpdateLandingEffect(boss);
		}

		void BossFireCombatSystem::ShotFireBall(Boss* boss)
		{
			if (boss == nullptr)
				return;

			if (boss->GetForward().LengthSq() < MIN_DIRECTION_LENGTH_SQ)
				boss->SetForward(GetDefaultForward());

			if (boss->GetBossType() == CharacterModelType::TutorialBoss)
			{
				m_currentFirePattern = FirePattern::Straight;
				m_burstRemaining = 0;
				CreateFireball(boss);
				return;
			}

			static std::mt19937 rng(std::random_device{}());
			std::uniform_int_distribution<int> dist(FIRE_PATTERN_ROLL_MIN, FIRE_PATTERN_ROLL_MAX);

			switch (static_cast<FirePattern>(dist(rng)))
			{
			case FirePattern::Burst:
				m_burstRemaining = BURST_SHOT_COUNT;
				m_burstTimer = 0.0f;
				break;

			case FirePattern::Rain:
				StartRain();
				break;

			case FirePattern::Straight:
			default:
				m_currentFirePattern = FirePattern::Straight;
				m_burstRemaining = 0;
				CreateFireball(boss);
				break;
			}
		}

		void BossFireCombatSystem::UpdateBurst(Boss* boss, float deltaTime)
		{
			if (m_fireballActive || m_burstRemaining <= 0)
				return;

			m_burstTimer -= deltaTime;
			if (m_burstTimer <= 0.0f)
			{
				m_currentFirePattern = FirePattern::Straight;
				CreateFireball(boss);
				m_burstRemaining--;
				m_burstTimer = BURST_SHOT_INTERVAL;
			}
		}

		void BossFireCombatSystem::CreateFireball(Boss* boss)
		{
			m_fireballActive = true;
			m_fireballPos = GetMouthPosition(boss);

			if (boss->GetTarget() && !boss->GetTarget()->IsDead())
			{
				Vector3 aimPos = boss->GetTarget()->GetPosition();
				aimPos.y += AIM_HEIGHT_OFFSET;
				m_fireballVelocity = ComputeAimDirection(boss, m_fireballPos, aimPos) * FIREBALL_SPEED;
			}
			else
			{
				m_fireballVelocity = ComputeAimDirection(boss, m_fireballPos, m_fireballPos) * FIREBALL_SPEED;
			}

			m_fireballLifeTime = FIREBALL_LIFE;

			m_fireballEmitter = NewGO<nsK2EngineLow::EffectEmitter>(0, "fireball");
			m_fireballEmitter->Init(nsEffect::Effect_ID::FireBall);
			m_fireballEmitter->SetPosition(GetFireballEffectPosition(m_fireballPos));
			m_fireballEmitter->SetScale(Vector3::One * FIREBALL_EFFECT_SCALE);
			m_fireballEmitter->Play();

			m_FireHit.Enable();
			m_FireHit.Update(m_fireballPos);
		}

		void BossFireCombatSystem::DeleteFireball(Boss* boss)
		{
			(void)boss;

			if (m_landingPos.y <= LANDING_POS_UNSET_Y)
				m_landingPos = GetLandingEffectPosition(m_fireballPos);

			m_landingEffectTimer = LANDING_EFFECT_DURATION;
			m_landingSpawnInterval = 0.0f;

			if (m_fireballEmitter != nullptr)
			{
				m_fireballEmitter->Stop();
				m_fireballEmitter = nullptr;
			}

			m_fireballActive = false;
			m_FireHit.Disable();
		}

		void BossFireCombatSystem::StartRain()
		{
			m_rainActive = true;
			m_rainTimer = RAIN_DURATION;
			m_rainSpawnTimer = 0.0f;
		}

		void BossFireCombatSystem::UpdateRain(Boss* boss)
		{
			if (!m_rainActive)
				return;

			m_rainTimer -= g_gameTime->GetFrameDeltaTime();
			m_rainSpawnTimer -= g_gameTime->GetFrameDeltaTime();

			if (m_rainSpawnTimer <= 0.0f)
			{
				SpawnRainDrop(boss);
				m_rainSpawnTimer = RAIN_SPAWN_INTERVAL;
			}

			if (m_rainTimer <= 0.0f)
				m_rainActive = false;
		}

		void BossFireCombatSystem::SpawnRainDrop(Boss* boss)
		{
			if (!boss->GetTarget() || boss->GetTarget()->IsDead())
				return;

			for (int i = 0; i < MAX_RAIN_DROPS; i++)
			{
				if (m_rainDrops[i].active)
					continue;

				static std::mt19937 rng(std::random_device{}());
				std::uniform_int_distribution<int> xDist(RAIN_SPAWN_X_OFFSET_MIN, RAIN_SPAWN_X_OFFSET_MAX);

				m_rainDrops[i].active = true;
				m_rainDrops[i].pos = boss->GetTarget()->GetPosition();
				m_rainDrops[i].pos.x += static_cast<float>(xDist(rng));
				m_rainDrops[i].pos.y += RAIN_SPAWN_HEIGHT;
				m_rainDrops[i].velocity = Vector3(RAIN_VELOCITY_X, -RAIN_FALL_SPEED, RAIN_VELOCITY_Z);

				m_rainDrops[i].emitter = NewGO<nsK2EngineLow::EffectEmitter>(0, "rainFireball");
				m_rainDrops[i].emitter->Init(nsEffect::Effect_ID::FireBall);
				m_rainDrops[i].emitter->SetPosition(GetFireballEffectPosition(m_rainDrops[i].pos));
				m_rainDrops[i].emitter->SetScale(Vector3::One * RAIN_EFFECT_SCALE);
				m_rainDrops[i].emitter->Play();
				return;
			}
		}

		void BossFireCombatSystem::LandRainDrop(RainDrop& drop)
		{
			m_landingPos = GetLandingEffectPosition(drop.pos);
			m_landingEffectTimer = RAIN_LANDING_DURATION;
			m_landingSpawnInterval = 0.0f;

			if (drop.emitter != nullptr)
			{
				drop.emitter->Stop();
				drop.emitter = nullptr;
			}

			drop.active = false;
		}

		void BossFireCombatSystem::UpdateRainDrops(Boss* boss)
		{
			for (int i = 0; i < MAX_RAIN_DROPS; i++)
			{
				if (!m_rainDrops[i].active)
					continue;

				m_rainDrops[i].pos += m_rainDrops[i].velocity * g_gameTime->GetFrameDeltaTime();

				if (m_rainDrops[i].emitter != nullptr)
					m_rainDrops[i].emitter->SetPosition(GetFireballEffectPosition(m_rainDrops[i].pos));

				m_RainHit.Disable();
				m_RainHit.Enable();
				m_RainHit.Update(m_rainDrops[i].pos);

				if (m_rainDrops[i].pos.y <= STAGE_GROUND_Y)
				{
					LandRainDrop(m_rainDrops[i]);
					continue;
				}

				for (auto* target : boss->GetAllTargets())
				{
					if (target == nullptr || target->IsDead())
						continue;

					if (m_RainHit.IsHit(target))
					{
						DamageProcessor::ApplyDamageToTarget(
							target, static_cast<int>(boss->GetCharacterStatus().attack.normalDamage));
						LandRainDrop(m_rainDrops[i]);
						break;
					}
				}
			}
		}

		void BossFireCombatSystem::UpdateLandingEffect(Boss* boss)
		{
			if (m_landingEffectTimer <= 0.0f)
			{
				m_LandingHit.Disable();
				return;
			}

			m_landingEffectTimer -= g_gameTime->GetFrameDeltaTime();
			m_landingSpawnInterval -= g_gameTime->GetFrameDeltaTime();

			if (m_landingSpawnInterval <= 0.0f)
			{
				auto* pillar = NewGO<nsK2EngineLow::EffectEmitter>(0, "firePillar");
				pillar->Init(nsEffect::Effect_ID::BossFireAttack);
				pillar->SetPosition(m_landingPos);
				pillar->SetScale(Vector3::One * LANDING_PILLAR_SCALE);
				pillar->Play();

				m_LandingHit.Disable();
				m_LandingHit.Enable();
				m_LandingHit.Update(GetLandingHitPosition());

				m_landingSpawnInterval = LANDING_EFFECT_SPAWN_INTERVAL;
			}

			m_LandingHit.Update(GetLandingHitPosition());
			CheckLandingHitDamage(boss);
		}

		void BossFireCombatSystem::CheckLandingHitDamage(Boss* boss)
		{
			if (m_landingEffectTimer <= 0.0f)
				return;

			for (auto* target : boss->GetAllTargets())
			{
				if (target == nullptr || target->IsDead())
					continue;

				if (m_LandingHit.IsHit(target))
					DamageProcessor::ApplyDamageToTarget(
						target, static_cast<int>(boss->GetCharacterStatus().attack.normalDamage));
			}
		}

		Vector3 BossFireCombatSystem::GetMouthPosition(Boss* boss) const
		{
			return boss->GetPosition()
				+ boss->GetForward() * MOUTH_FORWARD
				+ Vector3(0.0f, MOUTH_HEIGHT, 0.0f);
		}

		Vector3 BossFireCombatSystem::GetFireballEffectPosition(const Vector3& logicPos) const
		{
			return Vector3(
				logicPos.x,
				logicPos.y + FIREBALL_EFFECT_Y_OFFSET,
				FIREBALL_EFFECT_Z_OFFSET);
		}

		Vector3 BossFireCombatSystem::GetLandingEffectPosition(const Vector3& hitPos) const
		{
			return Vector3(
				hitPos.x,
				STAGE_GROUND_Y + LANDING_EFFECT_Y_OFFSET,
				STAGE_CENTER_Z);
		}

		Vector3 BossFireCombatSystem::GetLandingHitPosition() const
		{
			return Vector3(m_landingPos.x, STAGE_GROUND_Y, STAGE_CENTER_Z);
		}

		Vector3 BossFireCombatSystem::GetDefaultForward() const
		{
			return Vector3(DEFAULT_AIM_DIR_X, DEFAULT_AIM_DIR_Y, DEFAULT_AIM_DIR_Z);
		}

		Vector3 BossFireCombatSystem::ComputeAimDirection(
			Boss* boss, const Vector3& fromPos, const Vector3& toPos) const
		{
			Vector3 dir = toPos - fromPos;

			if (dir.LengthSq() < MIN_DIRECTION_LENGTH_SQ)
			{
				dir = boss->GetForward();
				if (dir.LengthSq() < MIN_DIRECTION_LENGTH_SQ)
					dir = GetDefaultForward();
			}

			dir.Normalize();
			return dir;
		}
	}
}