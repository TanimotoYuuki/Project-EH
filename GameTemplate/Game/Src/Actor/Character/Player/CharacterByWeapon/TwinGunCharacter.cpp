#include "stdafx.h"
#include "TwinGunCharacter.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerWalkState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerJumpState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerRunState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerGuardState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerHitState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerGetUpState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerDethState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerReBoneState.h"

#include "Src/Actor/Character/Player/State/AttackState/PlayerAirAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargingState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerNormalAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushStartState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerPushState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerHeavyAttackState.h"

namespace
{
	const Vector3 GUN_OFFSET = Vector3(0.0f, 10.0f, 0.0f);    //! 銃と腕のオフセット。

	/* X,Y,Zそれぞれの成分の角度を設定する。*/
	const auto GUN_ANGLE_X = 0.0f;                            //! 銃のX軸の角度。
	const auto GUN_ANGLE_Y = 0.0f;                            //! 銃のY軸の角度。
	const auto GUN_ANGLE_Z = -90.0f;                          //! 銃のZ軸の角度。
}

namespace nsApp
{
	namespace nsActor
	{
		bool TwinGunCharacter::Start()
		{
			/* 武器をセット。*/
			m_currentWeapon = WeaponType::TwinGun;

			/* モデルの種類をセット。*/
			m_modelType = CharacterModelType::Player_1P;

			/* */
			if (!IWeaponCharacter::Start())
				return false;

			/* 装備する武器をセットする。*/
			m_model.LoadWeaponModel(CharacterModelType::Weapon_TwinGun);

			/* 武器のスケールと位置の調整。*/
			/* スケール。*/
			m_model.SetWeaponScale(Vector3::One * 2.0f);

			/* 大きさ。*/
			m_model.SetWeaponOffset(GUN_OFFSET);

			/* 武器の角度を設定。*/
			m_angle = MakeGunAngle(GUN_ANGLE_X, GUN_ANGLE_Y, GUN_ANGLE_Z);
			m_model.SetWeaponAngle(m_angle);

			return true;
		}


		void TwinGunCharacter::RegisterState()
		{
			/* 親クラス経由で基本動作ステートの更新。*/
			Player::RegisterState();

			/* 各ステートを登録する。*/
			/* 待機状態。*/
			m_stateFactory[PlayerStateID::enIdle] = []() { return new nsState::PlayerIdleState(); };

			/* 歩き状態。*/
			m_stateFactory[PlayerStateID::enWalk] = []() { return new nsState::PlayerWalkState(); };

			/* 走り状態。*/
			m_stateFactory[PlayerStateID::enRun] = []() { return new nsState::PlayerRunState(); };

			/* ジャンプ状態。*/
			m_stateFactory[PlayerStateID::enJump] = []() { return new nsState::PlayerJumpState(); };

			/* ガード状態。*/
			m_stateFactory[PlayerStateID::enGuard] = []() { return new nsState::PlayerGuardState(); };

			/* 被弾状態。*/
			m_stateFactory[PlayerStateID::enHit] = []() { return new nsState::PlayerHitState(); };

			/* 死亡状態。*/
			m_stateFactory[PlayerStateID::enDeath] = []() { return new nsState::PlayerDethState(); };

			/* 助ける状態。*/
			m_stateFactory[PlayerStateID::enHelp] = []() { return new nsState::PlayerReBoneState(); };

			/* 起き上がる状態。*/
			m_stateFactory[PlayerStateID::enGetUp] = []() { return new nsState::PlayerGetUpState(); };

			/* ガード状態。*/
			m_stateFactory[PlayerStateID::enGuard] = []() { return new nsState::PlayerGuardState(); };

			/* 攻撃状態。*/
			m_stateFactory[PlayerStateID::enNormalAttack] = []() { return new nsState::PlayerNormalAttackState(); };

			/* チャージ状態。*/
			m_stateFactory[PlayerStateID::enCharging] = []() { return new nsState::PlayerChargingState(); };

			/* チャージ攻撃状態。*/
			m_stateFactory[PlayerStateID::enChargeAttack] = []() { return new nsState::PlayerChargeAttackState (); };

			/* 乱射開始状態。*/
			m_stateFactory[PlayerStateID::enRushStart] = []() { return new nsState::PlayerRushStartState(); };

			/* 乱射終了状態。*/
			m_stateFactory[PlayerStateID::enRushEnd] = []() { return new nsState::PlayerRushEndState(); };

            /* ダッシュ攻撃状態。*/
			m_stateFactory[PlayerStateID::enPushForward] = []() { return new nsState::PlayerPushState(); };

            /* 空中攻撃。*/
			m_stateFactory[PlayerStateID::enAirAttack] = []() { return new nsState::PlayerAirAttackState(); };

            /* 重攻撃。*/
			m_stateFactory[PlayerStateID::enHeavyAttack] = []() { return new nsState::PlayerHeavyAttackState(); };
		}
	}
}

