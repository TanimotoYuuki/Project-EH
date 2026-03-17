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

namespace nsApp
{
	namespace nsActor
	{
		/* プレイヤーの状態を管理する列挙型。*/
		enum class PlayerStateID : uint8_t
		{
			/* 基本動作。*/
			enIdle,           /* 待機状態。*/
			enWalk,           /* 歩行状態。*/
			enRun,            /* 走行状態。*/
			enJump,           /* ジャンプ状態。*/
			enHit,            /* 被弾状態。*/
			enDeath,          /* 死亡状態。*/

			/* 攻撃状態。*/
			enNormalAttack,   /* 攻撃状態。*/
			enChargeAttack,   /* チャージ攻撃状態。*/
			enAirAttack,	  /* 空中攻撃状態。*/
			enComboAttack,	  /* コンボ攻撃状態 1段目。*/
			enComboLink,	  /* コンボ攻撃状態 2段目。*/
			enComboFinish,	  /* コンボ攻撃状態 3段目。*/
			enRushStart,	  /* 連続攻撃状態。*/
			enRushEnd,		  /* 連続攻撃のループ状態。*/
		};


		class Player : public ICharacter
		{
		public:
		    /* コンストラクタとデストラクタ。*/
			Player() = default;
			virtual ~Player() = default;


		public:
			/* ライフサイクル。*/
			/* 初期化処理。*/
			bool Start() override;
			/* 毎フレーム更新する処理。*/
			void Update() override;
			/* 描画処理。*/
			void Render(RenderContext& rc) override;


		public:
			/* 基本動作用アニメーションを再生。*/
			void PlayBasicAnimation(CharacterBasicAnimationList state);

			/* 攻撃用アニメーションを再生。*/
			inline void PlayWeaponAnimation(AttackType attack)
			{
				/* 攻撃アニメーションの数を取得。*/
				animIndex = m_playerAnimation.GetAttackAnimationIndex(attack);
				/* 攻撃アニメーションはボタンを押した瞬間に切り替わってほしいため補完割合を低めに設定。*/
				m_model.PlayAnimation(animIndex, 0.05f);
			}


		/* セッター。*/
		public:
			/* 座標を設定。*/
			inline void SetPosition(const Vector3& position)
			{
				m_currentPosition = position;
			}

			/* 入力判定の切り替えを設定。*/
			inline void SetInputEnable(bool isEnable)
			{
				m_playerInput.SetInputEnable(isEnable);
			}

			/* 入力時間を設定。*/
			inline void SetWaitInputTimer(int timer)
			{
				m_inputWaitTimer = timer;
			}

			/* 回転軸を制御。*/
			inline void SetAngle(float angle)
			{
				m_angle = Quaternion::Identity;
				m_angle.SetRotationDegY(angle);
				m_model.SettRotation(m_angle);
			}

			/* 前方向ベクトルを設定。*/
			inline void SetForwardVector(const Vector3& forward)
			{
				m_forwardVector = forward;
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
			inline const PlayerInput GetInputClass() const
			{
				return m_playerInput;
			}

			/* 座標を取得。*/
			inline Vector3& GetPosition()
			{
				return m_currentPosition;
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


		private:
			CharacterAnimation m_playerAnimation;                                                                  /* プレイヤーのアニメーション。*/
			CharacterController m_characterController;                                                             /* プレイヤーのキャラコン。*/

			WeaponType m_currentWeapon = WeaponType::GreatSword;                                                   /* 現在の武器。@TODO 武器の種類を増やす際に要調整。*/

			PlayerInput m_playerInput;                                                                             /* プレイヤーの入力を管理するクラス。*/
			PlayerStateID m_playerStateID;                                                                         /* プレイヤーの状態ID。*/

			Quaternion m_angle = Quaternion::Identity ;                                                            /* プレイヤーの回転角。*/

			Vector3 m_currentPosition = Vector3::Zero;                                                             /* プレイヤーの現在位置。*/
			Vector3 m_forwardVector = Vector3::Zero;                                                               /* プレイヤーの前方向ベクトル。*/

			int animIndex = 0;
			int m_inputWaitTimer;


		/* ステート生成。*/
		private:
			std::unordered_map<PlayerStateID, std::function<nsState::IState<nsActor::Actor>* ()>> m_stateFactory;  /* ステートの種類を格納。*/
			uint8_t m_currentStateID = 0;                                                                          /* 現在のステートID。*/

			/* 必要なステートを登録。*/
			void RegisterState();
		};
	}
}
