#include "stdafx.h"
#include "ICharacter.h"

namespace nsApp
{
	namespace nsActor
	{
		void ICharacter::Update()
		{
			/* Actorクラスの更新。*/
			Actor::Update();
		}


		void ICharacter::Draw(RenderContext& rc)
		{
			m_model.DrawCharacterModel(rc);
		}
	}
}