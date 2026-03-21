#include "stdafx.h"
#include "Boss.h"

#include "stdafx.h"
#include "Boss.h"


namespace nsApp
{
	namespace nsActor
	{
		bool Boss::Start()
		{
			/*モデルの読み込み。*/
			/*m_model.LoadCharacterModel(CharacterModelType::GrayDragon); */

			/*ステータス。*/
			m_characterStatus.hp.maxHP = 500;
			m_characterStatus.hp.currentHP = m_characterStatus.hp.maxHP;
			m_characterStatus.attack.normalDamage = 30.0f;
			m_characterStatus.moveSpeed = 3.0f;

			m_model.SetPosition(Vector3::Zero);

			return true;

		}

		void Boss::Update()
		{
			ICharacter::Update();
		}
	}
}