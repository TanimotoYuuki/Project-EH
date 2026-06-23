#pragma once

/**
 * @file   Boss.h
 * @brief  ボスキャラクターを管理する Facade クラス。戦闘処理は各 System に委譲する。
 * @date   2026/06/16
 */

#include "Src/Effect/EffectList.h"
#include "Src/StateMachine/StateMachine.h"

#include "BossAnimation.h"
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Common/ICharacter.h"
#include "Src/Actor/Character/Common/WeaponHitDetection.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"

#include "Src/Actor/Character/Boss/AI/BossAIConfig.h"
#include "Src/Actor/Character/Boss/Combat/BossPhaseController.h"
#include "Src/Actor/Character/Boss/Combat/BossDamageHandler.h"
#include "Src/Actor/Character/Boss/Combat/BossFireCombatSystem.h"
#include "Src/Actor/Character/Boss/Combat/BossMeleeCombatSystem.h"
#include "Src/Actor/Character/Boss/Combat/BossTornadoSystem.h"

namespace nsApp
{
	namespace nsActor
	{
		/**
		 * @enum BossStateID
		 * @brief ボスステートマシンの状態 ID。
		 */
		enum BossStateID : uint8_t
		{
			enIdle,	  //! 待機。
			enMove,	  //! 移動。
			enRoar,	  //! 咆哮。
			enAttack, //! 攻撃。
			enDamage, //! 被ダメージ。
			enDeath,  //! 死亡。
		};

		using StateType = nsState::IState<nsActor::Actor>;

		/**
		 * @class Boss
		 * @brief ボス本体。ICharacter としての公開 API を保ち、戦闘処理は各 System に委譲する。
		 */
		class Boss : public ICharacter
		{
		public:
			/* コンストラクタとデストラクタ。*/
			Boss() = default;
			virtual ~Boss() override = default;

		public:
			/* ライフサイクル。*/
			bool Start() override;
			void Update() override;
			void Render(RenderContext &rc) override;

		public:
			/* 攻撃・戦闘（System 委譲入口）。*/

			/**
			 * @brief 噛みつき攻撃。
			 */
			void AttackBite();

			/**
			 * @brief 尻尾攻撃。
			 */
			void AttackTail();

			/**
			 * @brief 火炎攻撃。
			 */
			void ShotFireBall();

			/**
			 * @brief Playerにあたったか。　
			 */
			void CheckHitAndDamagePlayers();

			/**
			 * @brief 硬直により被ダメステートへ遷移する（DamageHandler から呼ぶ）。
			 */
			inline void EnterFlinchDamageState()
			{
				/* 被ダメ硬直ステートへ遷移 */
				m_currentStateID = BossStateID::enDamage;
				m_stateMachine->ChangeState(m_stateFactory[BossStateID::enDamage]());
			}

			/**
			 * @brief 攻撃ステータスを倍率で乗算する（PhaseController から呼ぶ）。
			 * @param multiplier 攻撃力倍率。
			 */
			inline void MultiplyAttackStats(float multiplier)
			{
				/* 通常・会心ダメージを同倍率で強化 */
				m_characterStatus.attack.normalDamage *= multiplier;
				m_characterStatus.attack.criticalDamage *= multiplier;
			}

		public:
			/**
			 * @brief 座標をセット。
			 * @param pos　座標。
			 */
			inline void SetPosition(const Vector3 &pos)
			{
				m_position = pos;
			}

			/**
			 * @brief 回転軸をセット。
			 * @param rot　回転軸。
			 */
			inline void SetRotation(const Quaternion &rot)
			{
				m_rotation = rot;
				m_model.SettRotation(m_rotation);
			}

			/**
			 * @brief 前ベクトルを設定。
			 * @param forward 前ベクトル。
			 */
			inline void SetForward(const Vector3 &forward)
			{
				if (forward.LengthSq() > 0.0f)
					m_forward = forward;
			}

			/**
			 * @brief モデルごとのオフセットを設定。
			 * @param offset　オフセット。
			 */
			inline void SetModelOffset(const Vector3 &offset)
			{
				m_modelDynamicOffset = offset;
			}

			/**
			 * @brief Y座標を監視。
			 * @param yPos Y軸。
			 */
			inline void LockYPosition(float yPos)
			{
				m_isYLocked = true;
				m_lockedYPosition = yPos;
			}

			/**
			 * @brief Y軸を固定。
			 */
			inline void UnlockYPosition()
			{
				m_isYLocked = false;
			}

			/**
			 * @brief 回転軸を固定。
			 * @param lock　固定軸。
			 */
			inline void LockRotation(bool lock)
			{
				m_isRotationLocked = lock;
			}

			/**
			 * @brief アニメーションを再生。
			 * @param id　AnimationID。
			 */
			inline void PlayAnimation(BossAnimationID id)
			{
				m_model.PlayAnimation((int)id, 0.2f);
			}

		public:
			/**
			 * @brief ターゲットを設定。
			 * @param target 設定ターゲット。
			 */
			inline void SetTarget(ICharacter *target)
			{
				m_target = target;
			}

			/**
			 * @brief 攻撃目標を更新。
			 */
			void UpdatePrimaryTarget();

			/**
			 * @brief ターゲットを全員に設定。
			 * @param targets 複数のターゲット。
			 */
			inline void SetAllTargets(const std::vector<nsActor::ICharacter *> &targets)
			{
				m_allTargets = targets;
			}

			/**
			 * @brief ターゲットを取得。
			 * @return
			 */
			inline const std::vector<nsActor::ICharacter *> &GetAllTargets() const
			{
				return m_allTargets;
			}

		public:
			/**
			 * @brief ボスタイプを設定。
			 * @param type キャラクターモデル種別。
			 */
			inline void SetBossType(CharacterModelType type)
			{
				m_bossType = type;
			}

			/**
			 * @brief ダメージを適用する（ICharacter 委譲）。
			 * @param damage 与えるダメージ量。
			 */
			inline void ApplyDamage(int damage)
			{
				ICharacter::ApplyDamage(damage);
			}

			/**
			 * @brief 前フレーム HP を現在 HP に合わせてリセット。
			 */
			inline void ResetPrevHP()
			{
				m_prevHP = m_characterStatus.hp.currentHP;
			}

			/**
			 * @brief HP が減少したかどうかを判定し、m_prevHP を更新する。
			 * @return true なら被ダメージあり。
			 */
			bool IsDamage();

			/**
			 * @brief フェーズ補正後の待機時間を取得。
			 * @return 待機時間（秒）。
			 */
			float GetEffectiveIdleTime() const;

			/**
			 * @brief 現在 HP 比率を取得。
			 * @return 0.0〜1.0 の HP 比率。
			 */
			float GetHPRatio() const;


		public:
			/* ロア・フェーズ。*/
			/**
			 * @brief 咆哮ステートへ強制遷移する。
			 */
			void ForceRoar();

			/**
			 * @brief ロアクールダウンタイマーを更新。
			 * @param deltaTime 経過時間（秒）。
			 */
			void UpdateRoarCooldown(float deltaTime);

			/**
			 * @brief ロアを使用し、クールダウンと使用回数を更新。
			 */
			void UseRoar();

			/**
			 * @brief ロアが使用可能か判定。
			 * @return true なら使用可能。
			 */
			inline bool CanUseRoar() const
			{
				return m_roarCooldownTimer <= 0.0f;
			}

			/**
			 * @brief フェーズ1イベントが発火済みか。
			 * @return true なら発火済み。
			 */
			inline bool IsPhase1EventTriggered() const
			{
				return m_phaseController.IsPhase1Triggered();
			}

			/**
			 * @brief 直前の攻撃種別を記録。
			 * @param attackType 攻撃種別 ID。
			 */
			inline void RecordLastAttackType(uint8_t attackType)
			{
				m_lastAttackType = attackType;
			}

		public:
			/* エフェクト。*/
			/**
			 * @brief 怒りエフェクトを開始。
			 */
			void StartRageEffect();

			/**
			 * @brief 怒りエフェクトの位置をボスに追従させる。
			 */
			void UpdateRageEffect();

			/**
			 * @brief 怒りエフェクトを停止・破棄。
			 */
			void StopRageEffect();


		/* Getter。*/
		public:
			/**
			 * @brief ボス座標を取得。
			 * @return 座標参照。
			 */
			inline Vector3 &GetPosition() override
			{
				return m_position;
			}

			/**
			 * @brief 前方向ベクトルを取得。
			 * @return 正規化前方向。
			 */
			inline Vector3 GetForward() const
			{
				return m_forward;
			}

			/**
			 * @brief キャラクターコントローラを取得。
			 * @return コントローラ参照。
			 */
			inline CharacterController &GetController()
			{
				return m_BossController;
			}

			/**
			 * @brief 主ターゲットを取得。
			 * @return ターゲット。未設定なら nullptr。
			 */
			inline ICharacter *GetTarget() const
			{
				return m_target;
			}

			/**
			 * @brief 噛みつきヒット判定を取得（MeleeCombatSystem 委譲）。
			 * @return ヒット判定参照。
			 */
			inline WeaponHitDetection &GetBiteHit()
			{
				return m_meleeCombat.GetBiteHit();
			}

			/**
			 * @brief 尻尾ヒット判定を取得（MeleeCombatSystem 委譲）。
			 * @return ヒット判定参照。
			 */
			inline WeaponHitDetection &GetTailHit()
			{
				return m_meleeCombat.GetTailHit();
			}

			/**
			 * @brief 火球ヒット判定を取得（FireCombatSystem 委譲）。
			 * @return ヒット判定参照。
			 */
			inline WeaponHitDetection &GetFireHit()
			{
				return m_fireCombat.GetFireHit();
			}

			/**
			 * @brief 着弾炎柱ヒット判定を取得（FireCombatSystem 委譲）。
			 */
			inline WeaponHitDetection& GetFireLandingHit()
			{
				return m_fireCombat.GetLandingHit();
			}

			/**
			 * @brief Rain 関連（FireCombatSystem 委譲）。
			 */
			inline int GetFireRainDropCount() const
			{
				return m_fireCombat.GetRainDropCount();
			}

			/**
			 * @brief Rain 滴の状態を取得。
			 * @param index 滴のインデックス（0 〜 GetFireRainDropCount() - 1）。
			 * @return true なら有効な滴、false なら無効な滴またはインデックスエラー。
			 * @note インデックスエラーも false を返すため、呼び出し元はインデックスの範囲を事前に確認することが望ましい。
			 */
			inline bool IsFireRainDropActive(int index) const
			{
				return m_fireCombat.IsRainDropActive(index);
			}

			/**
			 * @brief Rain 滴の位置を取得。
			 * @param index 滴のインデックス（0 〜 GetFireRainDropCount() - 1）。呼び出し元はこの範囲内であることを確認すること。
			 * @return 滴の位置。インデックスエラーの場合は Vector3::Zero を返す。
			 */
			inline Vector3 GetFireRainDropPosition(int index) const
			{
				return m_fireCombat.GetRainDropPosition(index);
			}

			/**
			 * @brief Rain 滴のヒット判定半径を取得。
			 * @return ヒット判定半径。
			 */
			inline float GetFireRainHitRadius() const
			{
				return m_fireCombat.GetRainHitRadius();
			}

			/**
			 * @brief 主ターゲットまでの距離を取得。
			 * @return 距離。ターゲットなしは 10000.0f。
			 */
			inline float GetDistanceToTarget() const
			{
				if (!m_target)
					return 10000.0f;
				return (m_target->GetPosition() - m_position).Length();
			}

			/**
			 * @brief フェーズ補正後の攻撃力倍率を取得。
			 * @return ダメージ倍率。
			 */
			inline float GetDamageMultiplier() const
			{
				return m_damageMultiplier;
			}

			/**
			 * @brief 現在ステート ID を取得。
			 * @return BossStateID 値。
			 */
			inline uint8_t GetCurrentStateID() const
			{
				return m_currentStateID;
			}

			/**
			 * @brief ロア使用回数を取得。
			 * @return 使用回数。
			 */
			inline uint8_t GetRoarUsageCount() const
			{
				return m_roarUsageCount;
			}

			/**
			 * @brief 直前の攻撃種別を取得。
			 * @return 攻撃種別 ID。
			 */
			inline uint8_t GetLastAttackType() const
			{
				return m_lastAttackType;
			}

			/**
			 * @brief ボスタイプを取得。
			 * @return キャラクターモデル種別。
			 */
			inline CharacterModelType GetBossType() const
			{
				return m_bossType;
			}

			/**
			 * @brief 再生中アニメーションが終了したか。
			 * @return true なら終了。
			 */
			inline bool IsAnimationEnd()
			{
				return !m_model.IsPlayAnimation();
			}


		private:
			/* 内部初期化・更新。*/
			/**
			 * @brief ステートファクトリを登録。
			 */
			void RegisterState();

			/**
			 * @brief TSV からステータスを初期化。
			 */
			void InitStatus();

			/**
			 * @brief ターゲット方向へ回転を更新。
			 * @param deltaTime 経過時間（秒）。
			 */
			void UpdateRotation(float deltaTime);


		private:
			std::vector<nsActor::ICharacter *> m_allTargets;				  //! 攻撃対象一覧。
			ICharacter *m_target = nullptr;									  //! 主ターゲット（最寄り）。
			CharacterModelType m_bossType = CharacterModelType::TutorialBoss; //! ボス種別。
			nsK2EngineLow::EffectEmitter *m_rageEffectEmitter = nullptr;	  //! 怒りエフェクト。

		private:
			BossPhaseController m_phaseController;							  //! HP フェーズ強化。
			BossDamageHandler m_damageHandler;								  //! 被ダメージ累積・硬直。
			BossFireCombatSystem m_fireCombat;								  //! 火球・Burst・Rain・着弾炎柱。
			BossMeleeCombatSystem m_meleeCombat;							  //! 噛みつき・尻尾。
			BossTornadoSystem m_tornadoSystem;								  //! 竜巻（GreenDragon）。

			std::unique_ptr<BossAnimation> m_BossAnimation;					  //! アニメーション管理。
			CharacterController m_BossController;							  //! 物理移動。

			Vector3 m_position = Vector3::Zero;								  //! 論理座標。
			Vector3 m_forward = Vector3::Zero;								  //! 前方向。
			Vector3 m_modelOffset = Vector3::Zero;							  //! ボスタイプ別固定オフセット。
			Vector3 m_modelDynamicOffset = Vector3::Zero;					  //! ステート等の動的オフセット。

			Quaternion m_rotation = Quaternion::Identity;					  //! 回転。

			int m_prevHP = 0;												  //! 前フレーム HP。
			int m_accumulatedDamage = 0;									  //! 硬直判定用累積ダメージ。

			float m_damageMultiplier = 1.0f;								  //! 攻撃力倍率（未使用なら 1.0）。
			float m_roarCooldownTimer = 0.0f;								  //! ロアクールダウン残り。
			float m_lastRoarHPThreshold = 1.0f;								  //! 前回ロア発動時 HP 閾値。
			float m_dir = 1.0f;												  //! 向き補助（左右）。
			float m_lockedYPosition = 0.0f;									  //! Y 固定時の高さ。

			bool m_isYLocked = false;										  //! Y 座標固定フラグ。
			bool m_isRotationLocked = false;								  //! 回転固定フラグ。

			uint8_t m_roarUsageCount = 0;									  //! ロア使用回数。
			uint8_t m_lastAttackType = 0xFF;								  //! 直前攻撃種別。
			uint8_t m_currentStateID = 0;									  //! 現在ステート ID。

			std::unordered_map<BossStateID, std::function<StateType *()>> m_stateFactory; //! ステート生成表。
		};
	}
}