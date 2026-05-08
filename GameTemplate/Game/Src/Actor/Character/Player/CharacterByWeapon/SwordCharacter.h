#pragma once
/**
* @file   SwordCharacter.h
* @brief  SwordPlayerを管理を管轄するクラス。
* @author YamaguchiHayato。
* @date   2026\04/02
*/

#include "Src/Actor/Character/Player/Player.h"
#include "IWeaponCharacter.h"

namespace nsApp
{
	namespace nsActor
	{
		class SwordCharacter : public IWeaponCharacter
		{
		public:
			SwordCharacter() = default;
			virtual ~SwordCharacter() = default;


		public:
			/**
			* 初期化。
			*/
			bool Start() override;


		protected:
			void RegisterState() override;
		};
	}
}


