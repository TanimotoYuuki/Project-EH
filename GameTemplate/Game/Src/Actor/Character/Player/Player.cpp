#include "stdafx.h"
#include "Player.h"

/* 基本動作ステート。*/
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerWalkState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerJumpState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerRunState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerHitState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerDethState.h"

/* 攻撃ステート。*/
#include "Src/Actor/Character/Player/State/AttackState/PlayerNormalAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerAirAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushStartState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerSlashUpState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerPushState.h"

#include "Src/Actor/Character/Common/WeaponHitDetection.h"

namespace
{
	const auto ANGLE_Y = 90.0f; /* プレイヤーの初期角度。*/
	const Vector3 POS = Vector3(0.0f, 50.0f, 0.0f);
}

namespace nsApp
{
	namespace nsActor
	{
		bool Player::Start()
		{
			/* アニメーションとモデルを準備する。*/
			/* アニメーションクラスの初期化処理をコール。*/
			m_playerAnimation.Initialize();
			/* 今の武器をセットする。*/
			m_playerAnimation.LoadAnimation(m_currentWeapon);

			/* アニメーションとモデルをセットする。*/
			m_model.LoadCharacterModel
			(
				CharacterModelType::Player_3P,
				m_playerAnimation.GetAnimatiocClip(),
				m_playerAnimation.GetAnimationClips()
			);

			/* 武器の種類をセットする。*/
			m_model.LoadWeaponModel(CharacterModelType::Weapon_GreatSword);

			/* キャラスケールをセットする。*/
			m_model.SetCharacterScale(Vector3::One * 0.5f);

			/* 武器の調整。*/
			m_model.SetWeaponScale(Vector3::One * 0.3f);
			m_model.SetWeaponOffset(Vector3::Zero);

			m_model.SetPosition(POS);

			m_angle.AddRotationDegY(ANGLE_Y);
			m_model.SettRotation(m_angle);

			m_currentPosition = POS;
			m_characterController.Init(5.0f, 8.0f, POS);

			/* ステータスを初期化。*/
			InitAttackStatus();

			/* ステートを生成する。*/
			RegisterState();
			m_stateMachine->ChangeState(m_stateFactory[PlayerStateID::enIdle]());

			SetWaitInputTimer(10);

			return true;
		}


		void Player::Update()
		{
			/* ICharacterクラスの更新処理をコール。*/
			ICharacter::Update();

			/* ヒットストップ状態なら*/
			if (IsHitStop())
				return;

			/* ゲーム開始直後数フレームは入力を受け付けない*/
			/*@全体共有: その硬直はボス戦の開始演出でカバーする*/
			if (m_inputWaitTimer > 0)
			{
				m_inputWaitTimer--;
				m_playerInput.SetInputEnable(false);
			}
			else
				m_playerInput.SetInputEnable(true);


			/* モデルの更新より先に入力判定を更新する。*/
			m_playerInput.Update();

			/* ステートマシーンを更新する。*/
			m_stateMachine->Update();

			/* リクエストを受け取って必要なステートをコール。*/
			if (m_stateMachine->GetCurrentState()->RequestID(m_currentStateID))
			{
				/* ステートの種類をキャストする。*/
				m_playerStateID = static_cast<PlayerStateID>(m_currentStateID);

				/* 登録されているステートならChangeStateに情報を渡す。*/
				if (m_stateFactory.count(m_playerStateID) > 0)
					m_stateMachine->ChangeState(m_stateFactory[m_playerStateID]());
			}

			/* モデルの座標を更新する。*/
			m_model.SetPosition(m_currentPosition);

			/* モデルを更新する。*/
			m_model.Update();

			/* モデルの更新が終わった後に剣の当たり判定をテーブルに渡す。*/
			m_weaponHitDetection.Update(m_model.GetWeaponPosition());
		}


		void Player::Render(RenderContext& rc)
		{
			/* 描画。*/
			ICharacter::Render(rc);
		}


		void Player::InitAttackStatus()
		{
			/* 基本ダメージ数の初期化。*/
			m_characterStatus.attack.normalDamage = 100.0f;	

			/* クリティカル率の初期化。*/ 
			m_characterStatus.attack.criticalRate = 0.1f;

			/* クリティカルダメージの初期化。*/
			m_characterStatus.attack.criticalDamage = 2.0f;
		}


		void Player::PlayBasicAnimation(CharacterBasicAnimationList state)
		{
			int animIndex = m_playerAnimation.GetBasicAnimationIndex(state);
			m_model.PlayAnimation(animIndex, 0.2f);
		}


		void Player::RegisterState()
		{
			/* 待機状態。*/
			m_stateFactory[PlayerStateID::enIdle] = []() { return new nsState::PlayerIdleState(); };

			/* 歩行状態。*/
			m_stateFactory[PlayerStateID::enWalk] = []() { return new nsState::PlayerWalkState(); };

			/* ジャンプ状態。*/
			m_stateFactory[PlayerStateID::enJump] = []() { return new nsState::PlayerJumpState(); };

			/* 走り状態。*/
			m_stateFactory[PlayerStateID::enRun] = []() { return new nsState::PlayerRunState(); };

			/* ダメージ状態。*/
			m_stateFactory[PlayerStateID::enHit] = []() { return new nsState::PlayerHitState(); };

			/* 死亡状態。*/
			m_stateFactory[PlayerStateID::enDeath] = []() { return new nsState::PlayerDethState(); };

			/* 通常攻撃状態。*/
			m_stateFactory[PlayerStateID::enNormalAttack] = []() { return new nsState::PlayerNormalAttackState(); };

			/* チャージ攻撃状態。*/
			m_stateFactory[PlayerStateID::enChargeAttack] = []() { return new nsState::PlayerChargeAttackState(); };

			/* 空中攻撃状態。*/
			m_stateFactory[PlayerStateID::enAirAttack] = []() { return new nsState::PlayerAirAttackState(); };

            /* 連続攻撃開始状態。*/
			m_stateFactory[PlayerStateID::enRushStart] = []() { return new nsState::PlayerRushStartState(); };

			/* 連続攻撃ループ状態。*/ 
			m_stateFactory[PlayerStateID::enRushEnd] = []() { return new nsState::PlayerRushEndState(); };

			/* 斬り上げ状態。*/
			m_stateFactory[PlayerStateID::enSlashUp] = []() { return new nsState::PlayerSlashUpState(); };

			/* 突き進む状態。*/
			m_stateFactory[PlayerStateID::enPushForward] = []() { return new nsState::PlayerPushState(); };
		}
	}
}
