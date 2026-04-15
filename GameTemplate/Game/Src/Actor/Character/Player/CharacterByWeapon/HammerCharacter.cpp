#include "stdafx.h"
#include "HammerCharacter.h"


#include "Src/Actor/Character/Player/State/AttackState/PlayerNormalAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargingState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerAirAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerPushState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerHeavyAttackState.h"

#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerWalkState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerJumpState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerRunState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerHitState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerDethState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerGuardState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerReBoneState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerGetUpState.h"


namespace
{
	const Vector3 HAMMER_OFFSET = Vector3(0.0f, 5.0f, 0.0f);     //! ハンマーと腕のオフセット。
	const auto HAMMER_ANGLE_Y = 90.0f;                           //! ハンマーの初期角度。
}

namespace nsApp
{
	namespace nsActor
	{
		bool HammerCharacter::Start()
		{
			/* 武器をセット。*/
			m_currentWeapon = WeaponType::Hammer;

			/* モデルの種類をセット。*/
			m_modelType = CharacterModelType::Player_2P;

			/* 親クラスの初期化。
			*  @brief 必要なモデルの判定を行いPlayeクラスでモデルが読み込まれる。
			*/
			if (!IWeaponCharacter::Start())
				return false;

			/* 装備する武器の種類をセットする。*/
			m_model.LoadWeaponModel(CharacterModelType::Weapon_Hammer);

			/* 武器のスケールと位置の調整。*/
			/* スケール。*/
			m_model.SetWeaponScale(Vector3::One);
			/* 大きさ。*/
			m_model.SetWeaponOffset(HAMMER_OFFSET);
			/* 武器の角度を設定。*/
			m_agnle.SetRotationDegY(HAMMER_ANGLE_Y);
			m_model.SetWeaponAngle(m_agnle);

			return true;
		}


		void HammerCharacter::RegisterState()
		{
			Player::RegisterState();

			/* 基本動作ステート。*/
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


			/* 攻撃ステート。*/
			/* 通常攻撃状態。*/ 
			m_stateFactory[PlayerStateID::enNormalAttack] = []() { return new nsState::PlayerNormalAttackState(); };

			/* チャージ中状態。*/
			m_stateFactory[PlayerStateID::enCharging] = []() { return new nsState::PlayerChargingState(); };

			/* チャージ攻撃状態。*/
			m_stateFactory[PlayerStateID::enChargeAttack] = []() { return new nsState::PlayerChargeAttackState(); };

			/* 空中攻撃状態。*/
			m_stateFactory[PlayerStateID::enAirAttack] = []() { return new nsState::PlayerAirAttackState(); };

			/* 横回転攻撃。*/
			m_stateFactory[PlayerStateID::enPushForward] = []() { return new nsState::PlayerPushState(); };

			/* 重い攻撃。*/
			m_stateFactory[PlayerStateID::enHeavyAttack] = []() { return new nsState::PlayerHeavyAttackState(); };
		}
	}
}