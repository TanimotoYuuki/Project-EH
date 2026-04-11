#include "stdafx.h"
#include "HammerCharacter.h"


#include "Src/Actor/Character/Player/State/AttackState/PlayerNormalAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargingState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerAirAttackState.h"

#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

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

		//	m_model.SetWeaponAttackBone(L"mixamorig:RightHand");

			/* 武器のスケールと位置の調整。*/
			m_model.SetWeaponScale(Vector3::One);
			m_model.SetWeaponOffset(Vector3(0.0f, 15.0f, 0.0f));
			
			Quaternion agnle;
			agnle.SetRotationDegY(90.0f);
			m_model.SetWeaponAngle(agnle);

			return true;
		}


		// 基本動作は親に登録してもらう
		void HammerCharacter::RegisterState()
		{
			Player::RegisterState();


			m_stateFactory[PlayerStateID::enIdle] = []() { return new nsState::PlayerIdleState(); };

			///* 通常攻撃状態。*/ 
			//m_stateFactory[PlayerStateID::enNormalAttack] = []() { return new nsState::PlayerNormalAttackState(); };

			///* チャージ中状態。*/
			//m_stateFactory[PlayerStateID::enCharging] = []() { return new nsState::PlayerChargingState(); };

			///* チャージ攻撃状態。*/
			//m_stateFactory[PlayerStateID::enChargeAttack] = []() { return new nsState::PlayerChargeAttackState(); };

			///* 空中攻撃状態。*/
			//m_stateFactory[PlayerStateID::enAirAttack] = []() { return new nsState::PlayerAirAttackState(); };
		}
	}
}