#include "stdafx.h"
#include "ICharacter.h"

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
			m_model.DrawCharacterModel(rc);
		}
	}
}