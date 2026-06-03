#pragma once

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Common/ICharacter.h"
#include "Src/Actor/Character/Common/WeaponHitDetection.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include "Src/StateMachine/StateMachine.h"
#include "BossAnimation.h"
#include "BossAIConfig.h"

namespace nsApp
{
	namespace nsActor
	{
		enum BossStateID : uint8_t
		{
			enIdle,
			enMove,
			enRoar,
			enAttack,
			enDamage,
			enDeath,
		};

		using StateType = nsState::IState<nsActor::Actor>;

		class Boss : public ICharacter
		{
		public:
			bool Start() override;
			void Update() override;
			void Render(RenderContext &rc) override;

			/*アニメーションを再生。*/
			void PlayAnimation(BossAnimationID id)
			{
				m_model.PlayAnimation((int)id, 0.2f);
			}

			/*アニメーションが終わったか。*/
			inline bool IsAnimationEnd()
			{
				return !m_model.IsPlayAnimation();
			}

			/*セッター。*/

			/*座標を設定。*/
			inline void SetPosition(const Vector3 &pos)
			{
				m_position = pos;
			}

			/*回転を設定。*/
			inline void SetRotation(const Quaternion &rot)
			{
				m_rotation = rot;
				m_model.SettRotation(m_rotation);
			}

			/*ターゲットを設定できるようにする。*/
			void SetTarget(ICharacter *target)
			{
				m_target = target;
			}

			/*ボスタイプを設定。*/
			inline void SetBossType(CharacterModelType type)
			{
				m_bossType = type;
			}

			void SetModelOffset(const Vector3 &offset)
			{
				m_modelDynamicOffset = offset;
			}

			/*Y位置ロック。*/
			inline void LockYPosition(float yPos)
			{
				m_isYLocked = true;
				m_lockedYPosition = yPos;
			}

			inline void UnlockYPosition()
			{
				m_isYLocked = false;
			}

			/*ゲッター。*/

			/*前方向を取得。*/
			inline Vector3 GetForward() const
			{
				return m_forward;
			}

			/*前方向を設定。*/
			inline void SetForward(const Vector3 &forward)
			{
				if (forward.LengthSq() > 0.0f)
				{
					m_forward = forward;
				}
			}

			/*座標を取得。*/
			inline Vector3 &GetPosition() override
			{
				return m_position;
			}

			CharacterController &GetController()
			{
				return m_BossController;
			}

			/*ターゲットとの距離を取得。*/
			inline float GetDistanceToTarget() const
			{
				if (!m_target)
					return 10000.0;
				return (m_target->GetPosition() - m_position).Length();
			}

			/*ターゲットへのポインタを取得。*/
			ICharacter *GetTarget() const
			{
				return m_target;
			}

			/*現在のステートを取得。*/
			uint8_t GetCurrentStateID() const
			{
				return m_currentStateID;
			}

			/*攻撃。*/
			void AttackBite();
			void AttackTail();
			void ShotFireBall();

			/*ダメージを受けているか。*/
			bool IsDamage();

			/*前フレームのHP値をリセット。*/
			void ResetPrevHP()
			{
				m_prevHP = m_characterStatus.hp.currentHP;
			}

			/*AI支援メソッド。*/
			/*前回の攻撃タイプを記録。*/
			void RecordLastAttackType(uint8_t attackType)
			{
				m_lastAttackType = attackType;
			}

			/*前回の攻撃タイプを取得。*/
			uint8_t GetLastAttackType() const
			{
				return m_lastAttackType;
			}

			/*現在のHP率を取得 (0.0f - 1.0f)。*/
			float GetHPRatio() const
			{
				if (m_characterStatus.hp.maxHP <= 0)
					return 1.0f;
				return static_cast<float>(m_characterStatus.hp.currentHP) /
					   static_cast<float>(m_characterStatus.hp.maxHP);
			}

			/*Roarクールダウンを更新。*/
			void UpdateRoarCooldown(float deltaTime)
			{
				if (m_roarCooldownTimer > 0.0f)
				{
					m_roarCooldownTimer -= deltaTime;
				}
			}

			/*Roarが使用可能か。*/
			bool CanUseRoar() const
			{
				return m_roarCooldownTimer <= 0.0f;
			}

			/*Roarを使用。*/
			void UseRoar()
			{
				m_roarCooldownTimer = nsAI::BossAIConfig::ROAR_COOLDOWN;
				m_roarUsageCount++;
			}

			/*Roar使用回数を取得。*/
			uint8_t GetRoarUsageCount() const
			{
				return m_roarUsageCount;
			}

			/*ボスタイプを取得。*/
			CharacterModelType GetBossType() const
			{
				return m_bossType;
			}

			/*ダメージを受ける。*/
			inline void ApplyDamage(int damage)
			{
				ICharacter::ApplyDamage(damage);
			}

			/*攻撃判定へのアクセッサ。*/
			WeaponHitDetection &GetBiteHit()
			{
				return m_BiteHit;
			}

			WeaponHitDetection &GetTailHit()
			{
				return m_TailHit;
			}

			WeaponHitDetection &GetFireHit()
			{
				return m_FireHit;
			}

		private:
			/*ステートの登録。*/
			void RegisterState();

			/*ステータス初期化。*/
			void InitStatus();

			/*回転の更新。*/
			void UpdateRotation(float deltaTime);

			void OnDamageEvent(int damage);

			std::unique_ptr<BossAnimation> m_BossAnimation;
			CharacterController m_BossController;

			/*プレイヤーへのポインタ。*/
			ICharacter *m_target = nullptr;

			CharacterModelType m_bossType = CharacterModelType::TutorialBoss;

			/*ヒット判定。*/
			WeaponHitDetection m_BiteHit;
			WeaponHitDetection m_TailHit;
			WeaponHitDetection m_FireHit;

			Vector3 m_position = Vector3::Zero;
			Vector3 m_attackPosition = Vector3::Zero;
			Vector3 m_forward = Vector3::Zero;
			Vector3 m_modelOffset = Vector3::Zero;

			Quaternion m_rotation = Quaternion::Identity;

			int m_prevHP = 0; /*HPの変化を検出するための前のHP。*/
			float m_dir = 1.0f;
			Vector3 m_modelDynamicOffset = Vector3::Zero;

			bool m_isYLocked = false;		// Y位置をロックしているか
			float m_lockedYPosition = 0.0f; // ロック時のY位置

			uint8_t m_lastAttackType = 0xFF;	/*前回の攻撃タイプ (0=Bite, 1=Tail, 2=Fire)*/
			float m_roarCooldownTimer = 0.0f;	/*Roarのクールダウン時間*/
			uint8_t m_roarUsageCount = 0;		/*Roar使用回数*/
			float m_lastRoarHPThreshold = 1.0f; /*最後にRoarを使った時のHP率*/

			int m_accumulatedDamage = 0;		/*累積ダメージ。*/
			float m_damageResetTimer = 0.0f;	/*累計ダメージリセット用タイマー。*/
			float m_flinchCooldownTimer = 0.0f; /*連続怯み防止用のクールダウンタイマー。*/

			/*ステート。*/
			std::unordered_map<BossStateID, std::function<StateType *()>> m_stateFactory;
			uint8_t m_currentStateID = 0;
		};
	}
}