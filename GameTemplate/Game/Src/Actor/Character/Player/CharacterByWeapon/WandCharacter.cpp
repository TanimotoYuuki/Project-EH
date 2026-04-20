#include "stdafx.h"
#include "WandCharacter.h"

#include "Src/Actor/Character/Player/State/AttackState/PlayerNormalAttackState.h"

namespace
{
	const auto WEAPON_OFFSET = Vector3(0.0f, -20.0f, 0.0f); //! 武器と腕モデル間のオフセット。
	const auto WEAPON_ANGLE_Y = 0.0f;						//! 武器のY軸の角度。
}

namespace nsApp
{
	namespace nsActor
	{
		bool WandCharacter::Start()
		{
			/* 武器をセットする。*/
			m_currentWeapon = WeaponType::Wand;

			/* モデルの種類をセットする。*/
			m_modelType = CharacterModelType::Player_4P;

			/* 親クラスを更新する。*/
			if(!IWeaponCharacter::Start())
				return false;

			/* 武器モデルをロードする。*/
			m_model.LoadWeaponModel(CharacterModelType::Weapon_Wand);

			/* 武器の大きさを設定する。*/
			m_model.SetWeaponScale(Vector3::One * 0.25f);

			/* 武器とキャラモデルの腕の差を設定。*/
			m_model.SetWeaponOffset(WEAPON_OFFSET);

			/* 武器の角度を設定する。*/
			m_angle.SetRotationDegY(WEAPON_ANGLE_Y);
			m_model.SetWeaponAngle(m_angle);

			return true;
		}


		void WandCharacter::RegisterState()
		{
			/* 基本動作ステートはPlayerクラス経由で登録。*/
			Player::RegisterState();

			/* 通常攻撃ステート。*/
			m_stateFactory[PlayerStateID::enNormalAttack] = []() { return new nsState::PlayerNormalAttackState(); };
		}
	}
}