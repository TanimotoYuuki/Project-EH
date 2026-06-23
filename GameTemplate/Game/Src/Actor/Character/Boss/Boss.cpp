#include "stdafx.h"
#include "Boss.h"

#include "Src/Actor/Character/Boss/State/BossIdleState.h"
#include "Src/Actor/Character/Boss/State/BossMoveState.h"
#include "Src/Actor/Character/Boss/Strategy/BossAttackState.h"
#include "Src/Actor/Character/Boss/State/BossDamageState.h"
#include "Src/Actor/Character/Boss/Strategy/BossRoarState.h"
#include "Src/Actor/Character/Boss/State/BossDethState.h"
#include "Src/Actor/Character/Boss/Status/BossManager.h"
#include "Src/Direction/GameStartDirection.h"

namespace
{
	/* 初期配置・回転 */
	const Vector3 START_POSITION{ 200.0, 50.0f, 0.0f };				//! ボス初期配置座標。
	const float   ROT_SPEED = 5.0f;									//! ターゲット方向への回転速度（大きいほど素早く向く）。

	/* ステータス初期値（TSV 未使用部分のフォールバック） */
	static constexpr float BASE_ATTACK_DAMAGE = 50.0f;				//! 通常攻撃ダメージ。
	static constexpr float BASE_CRITICAL_DAMAGE = 80.0f;			//! 会心攻撃ダメージ。
	static constexpr float BASE_CRITICAL_RATE = 0.1f;				//! 会心発生率。
}


namespace nsApp
{
	namespace nsActor
	{
		/* ボスタイプごとのモデル表示オフセット */
		const std::unordered_map<CharacterModelType, Vector3> BOSS_OFFSETS =
		{
			{ CharacterModelType::GrayDragon,   Vector3(0.0f, 0.0f, 0.0f) },
			{ CharacterModelType::TutorialBoss, Vector3(0.0f, 0.0f, 0.0f) },
			{ CharacterModelType::RedDragon,    Vector3(0.0f, 0.0f, 0.0f) },
			{ CharacterModelType::GreenDragon,  Vector3(0.0f, 0.0f, 0.0f) },
		};

		/* ボスタイプごとのモデルスケール */
		const std::unordered_map<CharacterModelType, float> BOSS_SCALES =
		{
			{ CharacterModelType::GrayDragon,   0.2f  },
			{ CharacterModelType::TutorialBoss, 0.2f  },
			{ CharacterModelType::RedDragon,    0.17f },
			{ CharacterModelType::GreenDragon,  0.2f  },
		};

		/* ボスタイプを文字列に変換（アニメーション・モデル読み込み用） */
		static const char* BossTypeToString(CharacterModelType bossType)
		{
			switch (bossType)
			{
			case CharacterModelType::GrayDragon:   return "GrayDragon";
			case CharacterModelType::GreenDragon:  return "GreenDragon";
			case CharacterModelType::RedDragon:    return "RedDragon";
			case CharacterModelType::TutorialBoss: return "TutorialBoss";
			default:                               return "TutorialBoss";
			}
		}


		/* 特定ステート時に加算するモデル浮きオフセット */
		const std::unordered_map<BossStateID, Vector3> STATE_OFFSETS =
		{
			{ BossStateID::enDamage, Vector3(0.0f, 5.0f, 0.0f) },
		};


		void Boss::InitStatus()
		{
			/* TSV から HP を算出して設定 */
			const auto& params = nsAI::BossTypeManager::GetBossTypeParameters(m_bossType);
			int bossHP = static_cast<int>(params.m_baseHP * params.m_Multiplier);

			m_characterStatus.hp.maxHP = bossHP;
			m_characterStatus.hp.currentHP = bossHP;
			m_prevHP = bossHP;

			/* 攻撃ステータス初期化 */
			m_characterStatus.attack.normalDamage = BASE_ATTACK_DAMAGE;
			m_characterStatus.attack.criticalDamage = BASE_CRITICAL_DAMAGE;
			m_characterStatus.attack.criticalRate = BASE_CRITICAL_RATE;
			m_hitStopFlame = 0;
		}


		bool Boss::Start()
		{
			/* ステートマシン生成 */
			if (!m_stateMachine)
				m_stateMachine = new nsState::StateMachine<Actor>(this);

			/* アニメーション・モデル読み込み */
			m_BossAnimation = std::make_unique<BossAnimation>();
			m_BossAnimation->Init(BossTypeToString(m_bossType));
			m_model.LoadCharacterModel(m_bossType, m_BossAnimation->GetAnimationClips(), (int)BossAnimationID::Max);

			/* ボスタイプ別オフセット適用 */
			auto it = BOSS_OFFSETS.find(m_bossType);
			if (it != BOSS_OFFSETS.end())
				m_modelOffset = it->second;

			/* 初期配置・向き設定 */
			m_position = START_POSITION;
			m_BossController.SetPosition(m_position);
			m_model.SetPosition(m_position);

			m_forward = Vector3::Left;
			m_rotation.SetRotationYFromDirectionXZ(m_forward);

			/* コントローラ・スケール初期化 */
			m_BossController.Init(40.0, 20.0f, m_position);
			m_BossController.SetPosition(m_position);
			m_model.SetPosition(m_position);

			auto scaleIt = BOSS_SCALES.find(m_bossType);
			float scale = (scaleIt != BOSS_SCALES.end()) ? scaleIt->second : 0.2f;
			m_model.SetCharacterScale(Vector3::One * scale);

			/* 戦闘 System 初期化（近接・竜巻・火球） */
			m_damageHandler.Init();
			m_phaseController.Init();
			m_meleeCombat.Init();
			m_tornadoSystem.Init();
			m_fireCombat.Init();

			/* ステート登録・待機ステートで開始 */
			RegisterState();
			m_stateMachine->ChangeState(m_stateFactory[enIdle]());

			InitStatus();
			m_prevHP = m_characterStatus.hp.currentHP;

			return true;
		}


		void Boss::Update()
		{
			ICharacter::Update();

			/* ヒットストップ中は以降の更新を行わない */
			if (IsHitStop())
				return;

			/* 開始演出中は戦闘だけ止め、モデルは毎フレーム更新する。*/
			if (auto* startDir = FindGO<nsGame::GameStartDirection>("gameStartDirection"))
			{
				if (!startDir->IsDirectionFinished())
				{
					m_position.z = 0.0f;
					m_model.SetPosition(m_position + m_modelOffset);
					m_model.SettRotation(m_rotation);
					m_model.Update();
					return;
				}
			}

			/* 主ターゲット更新・ロアクールダウン更新 */
			UpdatePrimaryTarget();
			UpdateRoarCooldown(g_gameTime->GetFrameDeltaTime());

			/* 硬直関連タイマー更新 */
			m_damageHandler.UpdateTimers(g_gameTime->GetFrameDeltaTime());

			/* HP 減少を検知したら被ダメ処理とフェーズ判定 */
			if (m_characterStatus.hp.currentHP < m_prevHP)
			{
				int damage = m_prevHP - m_characterStatus.hp.currentHP;
				m_damageHandler.HandleDamageEvent(this, damage);
				m_phaseController.CheckAndApplyPhase(this);
			}

			/* ステートマシン更新・近接ヒット判定 */
			m_stateMachine->Update();
			CheckHitAndDamagePlayers();

			/* 火球・竜巻 System 更新 */
			m_fireCombat.Update(this);
			m_tornadoSystem.Update(this);

			/* 次ステート ID を決定（死亡優先、次にステートからの遷移要求） */
			uint8_t nextID = m_currentStateID;

			if (m_characterStatus.hp.currentHP <= 0)
				nextID = BossStateID::enDeath;
			else
			{
				uint8_t reqID = 0;
				if (m_stateMachine->GetCurrentState()->RequestID(reqID))
					nextID = reqID;
			}

			/* ステートが変わったら切り替え */
			if (nextID != m_currentStateID)
			{
				auto next = static_cast<BossStateID>(nextID);
				if (m_stateFactory.count(next) > 0)
				{
					m_currentStateID = next;
					m_stateMachine->ChangeState(m_stateFactory[next]());
				}
			}

			/* 怒りエフェクト追従・ターゲット方向への回転 */
			UpdateRageEffect();
			UpdateRotation(g_gameTime->GetFrameDeltaTime());

			/* 物理コントローラと論理座標を同期 */
			if (m_isYLocked)
			{
				m_position = m_BossController.GetPosition();
				m_position.y = m_lockedYPosition;
				m_BossController.SetPosition(m_position);
			}
			else
				m_position = m_BossController.GetPosition();

			/* 2D ステージ用に Z を固定し、モデルへ座標・回転を反映 */
			m_position.z = 0.0f;
			m_model.SetPosition(m_position);
			m_model.SettRotation(m_rotation);

			/* 固定オフセット + 動的オフセット + ステート別オフセットを合成してモデル位置を決定 */
			Vector3 finalOffset = m_modelOffset + m_modelDynamicOffset;
			auto offsetIt = STATE_OFFSETS.find((BossStateID)m_currentStateID);
			if (offsetIt != STATE_OFFSETS.end())
				finalOffset += offsetIt->second;

			m_model.SetPosition(m_position + finalOffset);

			/* 次フレームの被ダメ判定用に HP を記録 */
			m_prevHP = m_characterStatus.hp.currentHP;
			m_model.Update();
		}


		void Boss::UpdatePrimaryTarget()
		{
			if (m_allTargets.empty())
				return;

			ICharacter* nearest = nullptr;
			float bestDistSq = FLT_MAX;

			/* 生存中の最寄りターゲットを探索 */
			for (auto* target : m_allTargets)
			{
				if (target == nullptr || target->IsDead())
					continue;

				const float distSq = (target->GetPosition() - m_position).LengthSq();
				if (distSq < bestDistSq)
				{
					bestDistSq = distSq;
					nearest = target;
				}
			}

			if (nearest != nullptr)
				m_target = nearest;
		}


		void Boss::UpdateRotation(float deltaTime)
		{
			/* 回転固定でなければターゲット方向へ前ベクトルを更新 */
			if (!m_isRotationLocked)
			{
				if (!m_target || m_target->IsDead())
					return;

				Vector3 toTarget = m_target->GetPosition() - m_position;
				toTarget.y = 0.0f;

				if (toTarget.LengthSq() < FLT_EPSILON)
					return;

				toTarget.Normalize();
				m_forward = toTarget;
			}

			/* 前ベクトルから Y 軸回転を求め、補間で滑らかに向きを合わせる */
			Quaternion targetRot;
			targetRot.SetRotationYFromDirectionXZ(m_forward);

			float rate = ROT_SPEED * deltaTime;
			if (rate >= 1.0f)
				m_rotation = targetRot;
			else
			{
				Quaternion currentRot = m_rotation;
				m_rotation.Slerp(rate, currentRot, targetRot);
			}
		}


		void Boss::AttackBite()
		{
			/* 噛みつき判定の配置を MeleeCombatSystem へ委譲 */
			m_meleeCombat.AttackBite(this);
		}


		void Boss::AttackTail()
		{
			/* 尻尾判定の配置を MeleeCombatSystem へ委譲 */
			Vector3 attackPos = m_meleeCombat.AttackTail(this);

			/* GreenDragon は尻尾攻撃と同時に竜巻を生成 */
			if (m_bossType == CharacterModelType::GreenDragon)
				m_tornadoSystem.SpawnFromTail(this, attackPos);
		}


		void Boss::ShotFireBall()
		{
			/* 火球発射を FireCombatSystem へ委譲 */
			m_fireCombat.ShotFireBall(this);
		}


		bool Boss::IsDamage()
		{
			/* 前フレーム HP と比較し、被ダメ有無を返す（比較後 m_prevHP を更新） */
			bool isDamage = m_characterStatus.hp.currentHP < m_prevHP;
			m_prevHP = m_characterStatus.hp.currentHP;
			return isDamage;
		}


		void Boss::Render(RenderContext& rc)
		{
			ICharacter::Render(rc);
		}


		void Boss::RegisterState()
		{
			/* 全 BossStateID に対応するステート生成関数を登録 */
			m_stateFactory[BossStateID::enIdle] = []() { return new nsState::BossIdleState(); };
			m_stateFactory[BossStateID::enMove] = []() { return new nsState::BossMoveState(); };
			m_stateFactory[BossStateID::enAttack] = []() { return new nsState::BossAttackState(); };
			m_stateFactory[BossStateID::enRoar] = []() { return new nsState::BossRoarState(); };
			m_stateFactory[BossStateID::enDamage] = []() { return new nsState::BossDamageState(); };
			m_stateFactory[BossStateID::enDeath] = []() { return new nsState::BossDethState(); };
		}


		float Boss::GetHPRatio() const
		{
			/* maxHP が 0 以下のときは割合計算を避ける */
			if (m_characterStatus.hp.maxHP <= 0)
				return 1.0f;

			return static_cast<float>(m_characterStatus.hp.currentHP) / static_cast<float>(m_characterStatus.hp.maxHP);
		}


		void Boss::UpdateRoarCooldown(float deltaTime)
		{
			/* ロアクールダウンタイマーを減算 */
			if (m_roarCooldownTimer > 0.0f)
				m_roarCooldownTimer -= deltaTime;
		}


		void Boss::UseRoar()
		{
			/* ボスタイプ別クールダウンをセットし、使用回数と発動時 HP を記録 */
			const auto& params = nsAI::BossTypeManager::GetBossTypeParameters(m_bossType);
			m_roarCooldownTimer = params.m_roarCooldown;
			m_roarUsageCount++;
			m_lastRoarHPThreshold = GetHPRatio();
		}


		void Boss::ForceRoar()
		{
			/* 死亡中は咆哮ステートへ遷移しない */
			if (m_currentStateID == BossStateID::enDeath)
				return;

			m_currentStateID = BossStateID::enRoar;
			m_stateMachine->ChangeState(m_stateFactory[BossStateID::enRoar]());
		}


		float Boss::GetEffectiveIdleTime() const
		{
			/* TSV の待機時間にフェーズ補正倍率を掛け合わせる */
			const auto& params = nsAI::BossTypeManager::GetBossTypeParameters(m_bossType);
			return params.m_idleTime * m_phaseController.GetIdleTimeMultiplier();
		}


		void Boss::StartRageEffect()
		{
			/* 怒りエフェクトを生成して再生（TODO: m_rageEffectEmitter への代入が必要） */
			auto* emitter = NewGO<nsK2EngineLow::EffectEmitter>(0, "bossRageEffect");
			emitter->Init(nsEffect::Effect_ID::Charge);
			emitter->SetPosition(m_position);
			emitter->SetScale(Vector3::One * 3.0f);
			emitter->Play();
			m_rageEffectEmitter = nullptr;
		}


		void Boss::UpdateRageEffect()
		{
			/* 怒りエフェクトが存在する場合、ボス位置へ追従 */
			if (m_rageEffectEmitter != nullptr)
				m_rageEffectEmitter->SetPosition(m_position);
		}


		void Boss::StopRageEffect()
		{
			if (m_rageEffectEmitter == nullptr)
				return;

			/* 怒りエフェクトを停止・破棄 */
			DeleteGO(m_rageEffectEmitter);
			m_rageEffectEmitter = nullptr;
		}


		void Boss::CheckHitAndDamagePlayers()
		{
			/* 近接ヒット判定とダメージ適用を MeleeCombatSystem へ委譲 */
			m_meleeCombat.CheckHitAndDamagePlayers(this);
		}
	}
}