#pragma once

/**
 * @file   Player.h
 * @brief  プレイヤーステートマシーンを更新するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/03/11
 */

#include "Src/Actor/Character/Player/PlayerInput.h"
#include "Src/Actor/Character/Common/ICharacter.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include "Src/Actor/Character/Common/WeaponHitDetection.h"
#include "Src/Actor/Character/Player/Component/PlayerSpawnData.h"
#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Gun/Shooter/GunShooter.h"
#include "Src/Effect/EffectList.h"
#include "Src/Effect/EffectListener.h"

#include "Src/Actor/Character/Player/Component/Damage/PlayerDamageInvincibilitySystem.h"
#include "Src/Actor/Character/NPC/Component/RescueStatusLister.h"
#include "Src/Actor/Character/Player/Component/Guard/PlayerGuardSystem.h"

namespace nsApp
{

	namespace nsEffect
	{
		class EffectList;
	}

	namespace nsActor
	{
		/*
		 * @enum PlayerStateID。
		 *プレイヤーの状態を管理する列挙型。
		 */
		enum class PlayerStateID : uint8_t
		{
			/* 基本動作。*/
			enIdle,	 //! 待機状態。
			enWalk,	 //! 歩行状態。
			enRun,	 //! 走行状態。
			enJump,	 //! ジャンプ状態。
			enHit,	 //! 被弾状態。
			enDeath, //! 死亡状態。
			enGuard, //! ガード状態。
			enHelp,	 //! 助け中。
			enGetUp, //! 起き上がり状態。

			/* 攻撃状態。*/
			enNormalAttack, //! 攻撃状態。
			enHeavyAttack,	//! 重攻撃状態。
			enCharging,		//! チャージ状態。
			enChargeAttack, //! チャージ攻撃状態。
			enMagicAttack,	//! 魔法攻撃状態。
			enHeelMagic,	//! 回復魔法状態。
			enAirAttack,	//! 空中攻撃状態。
			enComboAttack,	//! コンボ攻撃状態。
			enRushStart,	//! 連続攻撃状態。
			enRushEnd,		//! 連続攻撃のループ状態。
			enSlashUp,		//! 斬り上げ状態。
			enPushForward,	//! 突き進む攻撃状態。
		};

		class Player : public ICharacter
		{
		public:
			/* コンストラクタとデストラクタ。*/
			Player() = default;
			virtual ~Player();

		public:
			/*
			 * @def PlayerGeneratorにてコールする。
			 * @brief PlayerGeneratorクラスからデータを受け取る処理。
			 * @param data: 生成時に必要な構造体のデータを取得する。
			 */
			virtual void InitializeSpawnData(const PlayerSpawnData &data)
			{
				/* 座標をセット。*/
				m_currentPosition = data.spawnPosition;
				m_characterController.SetPosition(m_currentPosition);

				if (m_brain == nullptr)
				{
					m_brain = std::make_unique<NPCBrain>();
					m_brain->Init(this);
				}
			}

		public:
			/* ライフサイクル。*/
			/* 初期化処理。*/
			bool Start() override;
			/* 毎フレーム更新する処理。*/
			void Update() override;
			/* 描画処理。*/
			void Render(RenderContext &rc) override;

		private:
			/* 攻撃力の初期化処理。*/
			void InitAttackStatus();

			/* すり抜け計算。*/
			void ComputeSlipThrough();

			/**
			 * @brief 戦闘エリア制限を考慮して移動量を制限する処理。
			 * @param moveVector 移動したい量のベクトル。
			 * @param frameTime フレーム時間。
			 * @return 制限された移動量のベクトル。
			 */
			Vector3 ClampBattleAreaMoveVector(const Vector3 &moveVector, float frameTime) const;

		public:
			/**
			 * @brief 基本動作用アニメーションを再生。
			 * @param state 再生する基本動作の列挙型。
			 */
			void PlayBasicAnimation(CharacterBasicAnimationList state);

			/**
			 * @brief 攻撃用アニメーションを再生。
			 * @param attack 再生する攻撃の列挙型。
			 */
			void PlayWeaponAnimation(AttackType attack);

			/* 起き上がり状態。*/
			void ReceiveHelp();

			/**
			 * @brief NPC が救助対象へ蘇生を開始する（VirtualInput の Y 経由は使わない）。
			 * @param target 救助対象。
			 * @return 救助開始できた、または既に救助中なら true。
			 */
			bool TryBeginHelpToTarget(nsActor::Player *target);

			/**
			 * @brief NPC が回復魔法を開始する（VirtualInput の RT 経由は使わない）。
			 * @return 回復開始できた、または既に詠唱中なら true。
			 */
			bool TryBeginHeelMagic();

			/**
			 * @brief 回復魔法を詠唱中か。
			 * @return 詠唱中なら true。
			 */
			inline bool IsCastingHeal() const
			{
				return m_playerStateID == PlayerStateID::enHeelMagic;
			}

			/* 助ける対象のキャラクターを探索する。*/
			nsActor::Player *SearchCharacter();

			/* 現在扱う武器のSEを止める処理。*/
			void StopWeaponSE()
			{
				if (m_currentWeaponSE != nullptr)
				{
					/* SEの再生を止める。*/
					m_currentWeaponSE->Stop();

					/* リモコンを破棄する。*/
					m_currentWeaponSE = nullptr;
				}
			}

			/**
			 * @brief サブウェポンの表示。
			 * @param type 表示するサブウェポンの種類。
			 */
			inline void LoadSubWeapon(CharacterModelType type)
			{
				/* サブウェポンモデルを読み込む。*/
				m_model.LoadSubWeaponModel(type);
			}

			/* サブウェポンの非表示。*/
			inline void ResetSubWeapon()
			{
				m_model.ResetSubWeaponModel();
			}

			/**
			 * @brief 戦闘エリア制限を考慮してプレイヤーを移動させる。
			 * @param moveDelta 今回フレームで移動したい量。
			 */
			void MoveWithBattleClamp(const Vector3 &moveDelta, float deltaTime);

			/**
			 * @brief 死亡判定。
			 */
			void CheckDeth();

			/**
			 * @brief ボスの攻撃を受けてノックバックする。
			 * @param knockbackVelocity 吹っ飛びの初速。
			 * @param dirX             吹っ飛ぶX方向（+1.0fで右、-1.0fで左）。
			 */
			void ForceBlowAway(float knockbackVelocity, float dirX = -1.0f);

			/**
			 * @brief 外部から強制的に起き上がり状態へ遷移させる。
			 */
			void ForceGetUp();

			/**
			 * @brief KnockBack から Idle に戻ったときの状態IDを同期する。
			 */
			void NotifyReturnedFromKnockBack();

			/**
			 * @brief 外部から強制的に待機ステートへ遷移させる。
			 * @note ゲーム終了時など、吐き前にほかプレイヤーへの参照を安全に解除するために使用。
			 */
			void ForceChangeToIdleState();

			/* セッター。*/
		public:
			/**
			 * @brief 座標を設定。
			 * @param position 設定する座標。
			 */
			inline void SetPosition(const Vector3 &position)
			{
				m_currentPosition = position;
			}

			/**
			 * @brief 入力判定の切り替えを設定。
			 * @param isEnable 入力を有効にするかどうか。
			 */
			inline void SetInputEnable(bool isEnable)
			{
				m_playerInput.SetInputEnable(isEnable);
			}

			/**
			 * @brief 攻撃ダメージ補正率を設定する。
			 * @param damageRate 攻撃ダメージ補正率。
			 */
			inline void SetAttackDamageRate(float damageRate)
			{
				/* ダメージ補正率は0.0f未満にならないようにする。*/
				if (damageRate < 0.0f)
					damageRate = 0.0f;

				/* キャラクターステータスの攻撃力にダメージ補正率を掛ける。*/
				m_attackDamageRate = damageRate;
			}

			/**
			 * @brief 入力時間を設定。
			 * @param timer 入力時間。
			 */
			inline void SetWaitInputTimer(int timer)
			{
				m_inputWaitTimer = timer;
			}

			/**
			 * @brief 角度を設定。
			 * @param angle 設定する角度。
			 */
			inline void SetAngle(float angle)
			{
				m_angle = Quaternion::Identity;
				m_angle.SetRotationDegY(angle);
				m_model.SettRotation(m_angle);
			}

			/**
			 * @brief 前方向ベクトルを設定。
			 * @param forward 設定する前方向ベクトル。
			 */
			inline void SetForwardVector(const Vector3 &forward)
			{
				m_forwardVector = forward;
			}

			/**
			 * @brief ステートから武器の角度をいじれるように設定。
			 * @param rotDeg 回転軸の角度。
			 */
			inline void SetWeaponRotationAngle(const Vector3 &rotDeg, float angle)
			{
				m_weaponAngle = Quaternion::Identity;
				m_weaponAngle.SetRotationDeg(rotDeg, angle);
				m_model.SetWeaponAngle(m_weaponAngle);
			}

			/**
			 * @brief 下速度を設定。
			 * @param velocity 設定する下速度。
			 */
			inline void SetFallVelocity(float velocity)
			{
				m_fallVelocity = velocity;
			}

			/**
			 * @brief チャージレベルを設定する。
			 * @param chargeLevel 設定するチャージレベル。
			 */
			inline void SetChargeLevel(int chargeLevel)
			{
				m_chargeLevel = chargeLevel;
			}

			/**
			 * @brief モデル全体を傾きを設定をする。
			 * @param offset 設定する傾きのクォータニオン。
			 */
			inline void SetPostureOffset(const Quaternion &offset)
			{
				m_postureOffset = offset;
			}

			/**
			 * @brief 武器の回転をクォータニオンで設定する。
			 * @param rot 設定する回転のクォータニオン。
			 */
			inline void SetWeaponRotationByQuaternion(const Quaternion &rot)
			{
				m_weaponAngle = rot;
				m_model.SetWeaponAngle(m_weaponAngle);
			}

			/**
			 * @brief モデルの大きさを設定する。
			 * @param scale 設定する大きさ。
			 */
			inline void SetScale(const Vector3 &scale)
			{
				m_model.SetCharacterScale(scale);
				m_model.SetWeaponScale(scale);
			}

			/**
			 * @brief ローディングプレビュー用の座標と角度を設定する。
			 * @param position 設定する座標。
			 * @param angleY   設定するY軸の角度。
			 * @param scale    設定する大きさ。
			 */
			void SetLoadingPreviewTransform(const Vector3 &position, float angleY, const Vector3 &scale);

			/**
			 * @brief ローディングプレビューの有効化。
			 */
			inline void EnableLoadingPreview()
			{
				m_isLoadingPreview = true;
				m_isLoadingPreviewRunStarted = false;
			}

			/**
			 * @brief ローディングプレビューの無効化。
			 */
			void DisableLoadingPreview()
			{
				m_isLoadingPreview = false;
				m_isLoadingPreviewRunStarted = false;
			}

			/**
			 * @brief NPC 操作かどうかを設定する。
			 */
			inline void SetNpcControlled(bool isNpcControlled)
			{
				m_isNpcControlled = isNpcControlled;
			}
			/**
			 * @brief NPC 操作かどうか。
			 */
			inline bool IsNpcControlled() const
			{
				return m_isNpcControlled;
			}

			/* ゲッター。*/
		public:
			/* アニメーションが再生終了しているかどうか。*/
			inline bool IsPlayAnimation()
			{
				/* ModelRender経由でコール。*/
				return m_model.IsPlayAnimation();
			}

			/* 入力判定クラスを取得。*/
			inline PlayerInput &GetInputClass()
			{
				return m_playerInput;
			}

			/* キャラコンを取得。*/
			inline CharacterController &GetCharacterController()
			{
				return m_characterController;
			}

			/* 前方向ベクトルを取得。*/
			inline const Vector3 &GetForwardVector()
			{
				return m_forwardVector;
			}

			/* 落下速度を取得。*/
			inline float GetFallVelocity() const
			{
				return m_fallVelocity;
			}

			/* 座標を取得(親クラス経由で)。*/
			virtual inline Vector3 &GetPosition() override
			{
				return m_currentPosition;
			}

			/* 武器の当たり判定を取得。*/
			inline WeaponHitDetection &GetWeaponHitDetection()
			{
				return m_weaponHitDetection;
			}

			/* エフェクトリストを取得。*/
			inline nsEffect::EffectList &GetEffectList()
			{
				return m_effectList;
			}

			/* 現在の武器を取得する。*/
			inline WeaponType GetCurrentWeapon() const
			{
				return m_currentWeapon;
			}

			/**
			 * @brief 攻撃ダメージ補正率を取得する。
			 * @return 攻撃ダメージ補正率。
			 */
			inline float GetAttackDamageRate() const
			{
				return m_attackDamageRate;
			}

			/* プレイヤーの移動速度を取得する。*/
			inline float GetWalkSpeed() const
			{
				return m_walkSpeed;
			}

			/* プレイヤーの走る速度を取得する。*/
			inline float GetRunSpeed() const
			{
				return m_runSpeed;
			}

			/* プレイヤーのジャンプ力を取得する。*/
			inline float GetJumpPower() const
			{
				return m_jumpPower;
			}

			/* プレイヤーの空中での移動速度を取得する。*/
			inline float GetAirMoveSpeed() const
			{
				return m_airMoveSpeed;
			}

			/* プレイヤーの重力を取得する。*/
			inline float GetGravity() const
			{
				return m_gravity;
			}

			/* プレイヤーの最大落下速度を取得する。*/
			inline float GetMaxFallVelocity() const
			{
				return m_maxFallVelocity;
			}

			/* エフェクトの大きさを取得する。*/
			inline int GetEffectScale()
			{
				return m_chargeLevel;
			}

			/*
			 * @broef 指定したボーンの位置を取得。
			 * @param boneName 取得したいボーンの名前。
			 */
			inline Vector3 GetBonePosition(const wchar_t *boneName)
			{
				m_subWeaponHandMatrix = m_model.GetWorldMatrix(boneName);
				return Vector3(m_subWeaponHandMatrix.m[3][0], m_subWeaponHandMatrix.m[3][1], m_subWeaponHandMatrix.m[3][2]);
			}

			/**
			 * @brief GunShooterクラスを取得する。
			 */
			inline GunShooter &GetGunShooter()
			{
				return m_gunShooter;
			}

			/**
			 * @brief NPCの思考クラスを取得する。
			 */
			inline NPCBrain *GetBrain()
			{
				return m_brain.get();
			}

			/**
			 * @brief 死亡判定。HPが0以下、もしくは死亡フラグが立っている場合はtrueを返す。
			 * @return 死亡している場合はtrue、そうでない場合はfalse。
			 */
			inline bool IsDeath() const
			{
				return m_isDown;
			}

			/**
		　　 * @brief ガード中かどうか。
		 　　*/
			inline bool IsGuarding() const
			{
				return m_playerStateID == PlayerStateID::enGuard;
			}

			/**
			 * @brief プレイヤーモデル種別（ガードエフェクトの色分け用）。
			 */
			inline CharacterModelType GetModelType() const
			{
				return m_modelType;
			}

			/**
			 * @brief 救助状態管理クラスを取得する。
			 * @return 救助状態管理クラスの参照。
			 */
			inline RescueStatusLister &GetRescueStatusManager()
			{
				return m_rescueStatusManager;
			}

			/**
			 * @brief 救助状態管理クラスを取得する。
			 * @return 救助状態管理クラス。
			 */
			inline const RescueStatusLister &GetRescueStatusManager() const
			{
				return m_rescueStatusManager;
			}

			/**
			 * @brief ガードシステムを取得する。
			 * @return ガードシステムの参照。
			 */
			inline PlayerGuardSystem &GetGuardSystem()
			{
				return m_guardSystem;
			}

			/**
			 * @brief ガードシステムを取得する（const）。
			 * @return ガードシステムのconst参照。
			 */
			inline const PlayerGuardSystem &GetGuardSystem() const
			{
				return m_guardSystem;
			}

			/**
			 * @brief ダメージ無敵システムを取得する。
			 * @return ダメージ無敵システムの参照。
			 */
			inline PlayerDamageInvincibilitySystem &GetDamageInvincibilitySystem()
			{
				return m_damageInvincibilitySystem;
			}

			/**
			 * @brief ダメージ無敵システムを取得する（const）。
			 * @return ダメージ無敵システムのconst参照。
			 */
			inline const PlayerDamageInvincibilitySystem &GetDamageInvincibilitySystem() const
			{
				return m_damageInvincibilitySystem;
			}

			/**
			 * @brief KnockBack 硬直中か。
			 */
			inline bool IsInKnockBackState() const
			{
				return m_currentStateID == static_cast<uint8_t>(PlayerStateID::enHit);
			}

		private:
			nsK2EngineLow::EffectEmitter *m_chargeEffect = nullptr;	 //! チャージエフェクトのリモコン
			nsK2EngineLow::SoundSource *m_currentWeaponSE = nullptr; //! 現在の武器のSEのリモコン
			std::unique_ptr<NPCBrain> m_brain = nullptr;			 //! NPCの親クラスのポインタ。

		protected:
			CharacterAnimation m_playerAnimation;						 //! プレイヤーのアニメーション。
			PlayerInput m_playerInput;									 //! プレイヤーの入力を管理するクラス。
			CharacterModelType m_modelType;								 //! プレイヤーのモデルの種類。
			WeaponHitDetection m_weaponHitDetection;					 //! 武器の当たり判定を管理するクラス。
			WeaponType m_currentWeapon = WeaponType::None;				 //! 現在の武器。@TODO 武器の種類を増やす際に要調整。
			float m_attackDamageRate = 1.0f;							 //! 攻撃ダメージ補正率。
			RescueStatusLister m_rescueStatusManager;					 //! 救助状態管理クラス
			PlayerDamageInvincibilitySystem m_damageInvincibilitySystem; //! ダメージ無敵システム。

			/* ステート生成。*/
		protected:
			std::unordered_map<PlayerStateID, std::function<nsState::IState<nsActor::Actor> *()>> m_stateFactory; //! ステートの種類を格納。
			uint8_t m_currentStateID = 0;																		  //! 現在のステートID。

			/**
			 * @brief ステートを登録する関数。
			 */
			virtual void RegisterState();

		private:
			nsApp::nsEffect::EffectList m_effectList; //! エフェクト管理クラス
			PlayerStateID m_playerStateID;			  //! プレイヤーの状態ID。
			GunShooter m_gunShooter;				  //! 銃の発射処理を管理するクラス。
			EffectListener m_effectListener;		  //! エフェクトの再生に合わせてSEを鳴らすために使用するクラス
			PlayerGuardSystem m_guardSystem;		  //! ガードシステム。

		private:
			CharacterController m_characterController; //! プレイヤーのキャラコン。

			Quaternion m_angle = Quaternion::Identity;		 //! プレイヤーの回転角。
			Quaternion m_weaponAngle = Quaternion::Identity; //! 武器の回転角。
			Quaternion m_postureOffset = Quaternion::Identity;

			Vector3 m_currentPosition = Vector3::Zero; //! プレイヤーの現在位置。
			Vector3 m_forwardVector = Vector3::Zero;   //! プレイヤーの前方向ベクトル。
			Vector3 m_getPosition = Vector3::Zero;	   //! プレイヤーの座標を取得。。

			Matrix m_subWeaponHandMatrix; //! サブ武器を装備させるときの左手のボーンの行列を管理する変数。

			int animIndex = 0;		  //! アニメーションのインデックスを管理する変数。
			int m_inputWaitTimer = 0; //! 入力を受け付けるまでの時間。
			int m_chargeLevel = 1;	  //! チャージレベル。

			float m_walkSpeed = 50.0f;			//! プレイヤーの歩く速度。
			float m_runSpeed = 120.0f;			//! プレイヤーの走る速度。
			float m_jumpPower = 500.0f;			//! プレイヤーのジャンプ力。
			float m_airMoveSpeed = 120.0f;		//! プレイヤーの空中での移動速度。
			float m_gravity = 30.0f;			//! プレイヤーの重力。
			float m_maxFallVelocity = -1200.0f; //! プレイヤーの最大落下速度。
			float m_fallVelocity = 0.0f;		//! 落下速度。

			bool m_isIgnorePlayerSet = false;		   //! プレイヤーのセットを無視するかどうかのフラグ。
			bool m_isDown = false;					   //! プレイヤー専用のダウン状態。IGameObject側の死亡/削除フラグとは分ける。
			bool m_isLoadingPreview = false;		   //! ローディング画面用表示モードかどうか。
			bool m_isLoadingPreviewRunStarted = false; //! ローディング用Runアニメーションを開始したか。
			bool m_isNpcControlled = false;			   //! true なら Brain が動く。
		};
	}
}
