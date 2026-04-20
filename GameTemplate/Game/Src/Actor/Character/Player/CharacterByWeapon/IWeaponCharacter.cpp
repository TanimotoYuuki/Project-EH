#include "stdafx.h"
#include "IWeaponCharacter.h"
#include "Src/Sound/SoundLister.h"

namespace nsApp
{
	namespace nsActor
	{
		bool IWeaponCharacter::Start()
		{
			if (!Player::Start())
				return false;

			return true;
		}


		void IWeaponCharacter::CharacterWeaponAnimation(AttackType attack)
		{
			/* アニメーションの番号を取得する。*/
			m_animationIndex = m_playerAnimation.GetAttackAnimationIndex(attack);

			/* アニメーションを再生する。*/ 
			m_model.PlayAnimation(m_animationIndex, 0.05f);
		}
	}
}