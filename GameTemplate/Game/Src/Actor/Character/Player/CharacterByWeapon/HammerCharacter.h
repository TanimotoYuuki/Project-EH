#pragma once
/**
* @file   HammerCharacter.h
* @brief  HammerPlayerを管理を管轄するクラス。
* @author YamaguchiHayato。
* @date   2026/04/02
*/

#include "Src/Actor/Character/Player/CharacterByWeapon/IWeaponCharacter.h"

namespace nsApp
{
	namespace nsActor
	{
		class HammerCharacter : public IWeaponCharacter
		{
		public:
			/* コンストラクタとデストラクタ。*/
			HammerCharacter() = default;
			virtual ~HammerCharacter() = default;


		public:
			/**
			 * 初期化。
			 */
			bool Start() override;


		protected:
			void RegisterState() override;


		private:
			Quaternion m_agnle = Quaternion::Identity; //! モデルの角度。

		};
	}
}

