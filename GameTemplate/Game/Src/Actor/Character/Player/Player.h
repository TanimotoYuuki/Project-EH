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

namespace nsApp
{
	namespace nsEffect {
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
			enIdle,           //! 待機状態。
			enWalk,           //! 歩行状態。
			enRun,            //! 走行状態。
			enJump,           //! ジャンプ状態。
			enHit,            //! 被弾状態。
			enDeath,          //! 死亡状態。
			enGuard,          //! ガード状態。
			enHelp,           //! 助け中。
			enGetUp,          //! 起き上がり状態。

			/* 攻撃状態。*/
			enNormalAttack,   //! 攻撃状態。
			enHeavyAttack,	  //! 重攻撃状態。
			enCharging,       //! チャージ状態。
			enChargeAttack,   //! チャージ攻撃状態。
			enMagicAttack,	  //! 魔法攻撃状態。
			enHeelMagic,	  //! 回復魔法状態。
			enAirAttack,	  //! 空中攻撃状態。
			enComboAttack,	  //! コンボ攻撃状態。
			enRushStart,	  //! 連続攻撃状態。
			enRushEnd,		  //! 連続攻撃のループ状態。
			enSlashUp,        //! 斬り上げ状態。
			enPushForward,    //! 突き進む攻撃状態。
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
			inline virtual void InitializeSpawnData(const PlayerSpawnData& data)
			{
				/* 座標をセット。*/
				m_currentPosition = data.spawnPosition;
				m_characterController.SetPosition(m_currentPosition);

				if (m_brain == nullptr)
				{
					m_brain = new NPCBrain();
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
			void Render(RenderContext& rc) override;


		private:
			/* 攻撃力の初期化処理。*/
			void InitAttackStatus();

			/* ダミーモデルの初期化。*/
			void InitDummyModel();

			/* すり抜け計算。*/
			void ComputeSlipThrough();


		public:
			/* 
			 * @brief 基本動作用アニメーションを再生。
			 * @param state 再生する基本動作の列挙型。
			 */
			void PlayBasicAnimation(CharacterBasicAnimationList state);

			/* 
			 * @brief 攻撃用アニメーションを再生。
			 * @param attack 再生する攻撃の列挙型。
			 */
			void PlayWeaponAnimation(AttackType attack);

			/* 起き上がり状態。*/
			void ReceiveHelp();

			/* 助ける対象のキャラクターを探索する。*/
			nsActor::Player* SearchCharacter();

			/* 現在扱う武器のSEを止める処理。*/
		    void StopWeaponSE()
			{
				if (m_currentWeaponSE != nullptr)
				{
					/* SEの再生を止める。*/
					m_currentWeaponSE->Stop();

					/* 破棄。*/
					DeleteGO(m_currentWeaponSE);

					/* リモコンを破棄する。*/ 
					m_currentWeaponSE = nullptr;
				}
			}

			/* 
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


		/* セッター。*/
		public:
			/* 
			 * @brief 座標を設定。
			 * @param position 設定する座標。
			 */
			inline void SetPosition(const Vector3& position)
			{
				m_currentPosition = position;
			}

			/* 
			 * @brief 入力判定の切り替えを設定。
			 * @param isEnable 入力を有効にするかどうか。
			 */
			inline void SetInputEnable(bool isEnable)
			{
				m_playerInput.SetInputEnable(isEnable);
			}

			/* 
			 * @brief 入力時間を設定。
			 * @param timer 入力時間。
			 */
			inline void SetWaitInputTimer(int timer)
			{
				m_inputWaitTimer = timer;
			}

			/* 
			 * @brief 角度を設定。
			 * @param angle 設定する角度。
			 */
			inline void SetAngle(float angle)
			{
				m_angle = Quaternion::Identity;
				m_angle.SetRotationDegY(angle);
				m_model.SettRotation(m_angle);
			}

			/*
			 * @brief 前方向ベクトルを設定。
			 * @param forward 設定する前方向ベクトル。
			 */
			inline void SetForwardVector(const Vector3& forward)
			{
				m_forwardVector = forward;
			}

			/* 
			 * @brief ステートから武器の角度をいじれるように設定。
			 * @param rotDeg 回転軸の角度。
			 */
			inline void SetWeaponRotationAngle(const Vector3& rotDeg, float angle)
			{
				m_weaponAngle = Quaternion::Identity;
				m_weaponAngle.SetRotationDeg(rotDeg, angle);
				m_model.SetWeaponAngle(m_weaponAngle);
			}

			/* 
			 * @brief 下速度を設定。
			 * @param velocity 設定する下速度。
			 */
			inline void SetFallVelocity(float velocity)
			{
				m_fallVelocity = velocity;
			}

			/* 
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
			inline void SetPostureOffset(const Quaternion& offset)
			{
				m_postureOffset = offset;
			}

			/**
			 * @brief 武器の回転をクォータニオンで設定する。
			 * @param rot 設定する回転のクォータニオン。
			 */
			inline void SetWeaponRotationByQuaternion(const Quaternion& rot)
			{
				m_weaponAngle = rot;
				m_model.SetWeaponAngle(m_weaponAngle);
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
			inline  PlayerInput& GetInputClass() 
			{
				return m_playerInput;
			}

			/* キャラコンを取得。*/
			inline CharacterController& GetCharacterController()
			{
				return m_characterController;
			}

			/* 前方向ベクトルを取得。*/
			inline const Vector3& GetForwardVector()
			{
				return m_forwardVector;
			}

			/* 落下速度を取得。*/
			inline float GetFallVelocity() const
			{
				return m_fallVelocity;
			}

			/* 座標を取得(親クラス経由で)。*/
			virtual inline Vector3& GetPosition() override
			{
				return m_currentPosition;
			}

			/* 武器の当たり判定を取得。*/
			inline WeaponHitDetection& GetWeaponHitDetection()
			{
				return m_weaponHitDetection;
			}

			/* エフェクトリストを取得。*/
			inline nsEffect::EffectList& GetEffectList()
			{
				return m_effectList;
			}

			/* 現在の武器を取得する。*/
			inline WeaponType GetCurrentWeapon() const
			{
				return m_currentWeapon;
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
			inline Vector3 GetBonePosition(const wchar_t* boneName)
			{
				m_subWeaponHandMatrix = m_model.GetWorldMatrix(boneName);
				return Vector3(m_subWeaponHandMatrix.m[3][0], m_subWeaponHandMatrix.m[3][1], m_subWeaponHandMatrix.m[3][2]);
			}

			/**
			 * @brief GunShooterクラスを取得する。
			 */
			inline GunShooter& GetGunShooter()
			{
				return m_gunShooter;
			}

			/**
			 * @brief NPCの思考クラスを取得する。
			 */
			inline NPCBrain* GetBrain()
			{
				return m_brain;
			}


		private:
			nsK2EngineLow::EffectEmitter* m_chargeEffect = nullptr;                                                //! チャージエフェクトのリモコン       
			nsK2EngineLow::SoundSource* m_currentWeaponSE = nullptr;                                               //! 現在の武器のSEのリモコン
			NPCBrain* m_brain = nullptr;																		   //! NPCの親クラスのポインタ。


		protected:
			CharacterAnimation m_playerAnimation;                                                                  //! プレイヤーのアニメーション。
			PlayerInput m_playerInput;                                                                             //! プレイヤーの入力を管理するクラス。
			CharacterModelType m_modelType;                                                                        //! プレイヤーのモデルの種類。
			WeaponHitDetection m_weaponHitDetection;                                                               //! 武器の当たり判定を管理するクラス。
			WeaponType m_currentWeapon = WeaponType::None;                                                         //! 現在の武器。@TODO 武器の種類を増やす際に要調整。


		/* ステート生成。*/
		protected:
			std::unordered_map<PlayerStateID, std::function<nsState::IState<nsActor::Actor>* ()>> m_stateFactory;  //! ステートの種類を格納。
			uint8_t m_currentStateID = 0;                                                                          //! 現在のステートID。


			/* 必要なステートを登録。*/
			virtual void RegisterState();


		private:
			nsApp::nsEffect::EffectList m_effectList;                                                              //! エフェクト管理クラス
			PlayerStateID m_playerStateID;                                                                         //! プレイヤーの状態ID。
			GunShooter m_gunShooter;                                                                               //! 銃の発射処理を管理するクラス。
			EffectListener m_effectListener;


		private:
			CharacterController m_characterController;                                                             //! プレイヤーのキャラコン。

			Quaternion m_angle = Quaternion::Identity ;                                                            //! プレイヤーの回転角。
			Quaternion m_weaponAngle = Quaternion::Identity;                                                       //! 武器の回転角。
			Quaternion m_postureOffset = Quaternion::Identity;

			Vector3 m_currentPosition = Vector3::Zero;                                                             //! プレイヤーの現在位置。
			Vector3 m_forwardVector = Vector3::Zero;                                                               //! プレイヤーの前方向ベクトル。

			Matrix m_subWeaponHandMatrix;                                                                          //! サブ武器を装備させるときの左手のボーンの行列を管理する変数。


			int animIndex = 0;
			int m_inputWaitTimer;
			int m_chargeLevel = 1;                                                                                 //! チャージレベル。

			float m_fallVelocity = 0.0f;                                                                           //! 落下速度。

			bool m_isIgnorePlayerSet = false;
		};
	}
}
