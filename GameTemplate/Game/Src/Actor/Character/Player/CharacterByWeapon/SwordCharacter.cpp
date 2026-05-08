#include "stdafx.h"
#include "SwordCharacter.h"

#include "Src/Actor/Character/Player/State/AttackState/PlayerNormalAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerAirAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargingState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerPushState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushStartState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerSlashUpState.h"


namespace nsApp
{
	namespace nsActor
	{
		bool SwordCharacter::Start()
		{
			/* 武器をセットする。*/
			m_currentWeapon = WeaponType::GreatSword;

			/* モデルをセットする。*/
			m_modelType = nsApp::CharacterModelType::Player_3P;

		    /* ロードを行う。*/
			if (!IWeaponCharacter::Start())
				return false;

			/* キャラモデルの大きさをセットする。*/
			m_model.SetCharacterScale(Vector3::One * 0.5f);

			/* 武器のモデルとスケールをセット。*/
			m_model.LoadWeaponModel(CharacterModelType::Weapon_GreatSword);
			m_model.SetWeaponScale(Vector3::One * 0.3f);
			m_model.SetWeaponOffset(Vector3::One);

			return true;
		}


		void SwordCharacter::RegisterState()
		{
			/* 親クラス経由で基本動作ステートを登録。*/
			Player::RegisterState();

			/* 通常攻撃状態。*/
			m_stateFactory[PlayerStateID::enNormalAttack] = []() { return new nsState::PlayerNormalAttackState(); };

			/* チャージ攻撃状態。*/
			m_stateFactory[PlayerStateID::enChargeAttack] = []() { return new nsState::PlayerChargeAttackState(); };

			/* チャージ中状態。*/
			m_stateFactory[PlayerStateID::enCharging] = []() { return new nsState::PlayerChargingState(); };

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