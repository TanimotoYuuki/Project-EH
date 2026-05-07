#pragma once
/**
 * @file    TwinGunCharacter.h。
 * @brief   TwinGunPlayerを管理を管轄するクラス。
 * @author  Yamaguchi Hayato。
 * @date    2026/04/24
 */

#include "Src/Actor/Character/Player/CharacterByWeapon/IWeaponCharacter.h"

namespace nsApp
{
	namespace nsActor
	{
		class TwinGunCharacter : public IWeaponCharacter
		{
		public:
			/* コンストラクタとデストラクタ。*/
			TwinGunCharacter() = default;
			virtual ~TwinGunCharacter() = default;


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
