#include "stdafx.h"
#include "ICharacter.h"

namespace
{
	const auto HP_0 = 0;
}

namespace nsApp
{
	namespace nsActor
	{
		void ICharacter::Update()
		{
			/* ヒットストップタイマーを減らす。*/
			if (m_hitStopFlame > 0)
				m_hitStopFlame--;

			/* Actorクラスの更新。*/
			Actor::Update();
		}


		void ICharacter::Render(RenderContext& rc)
		{
			/* キャラクターを描画する。*/
			m_model.DrawCharacterModel(rc);
		}


		void ICharacter::ApplyDamage(int damage)
		{
			/* HPを参照してダメージを与える。*/
			m_characterStatus.hp.currentHP -= damage;

			/* HPが0以下になった場合は0にする。*/
			if(m_characterStatus.hp.currentHP < HP_0)
				m_characterStatus.hp.currentHP = HP_0;

			/* 最大HPを参照する。*/
			if(m_characterStatus.hp.currentHP > m_characterStatus.hp.maxHP)
				m_characterStatus.hp.currentHP = m_characterStatus.hp.maxHP;
		}
	}
}