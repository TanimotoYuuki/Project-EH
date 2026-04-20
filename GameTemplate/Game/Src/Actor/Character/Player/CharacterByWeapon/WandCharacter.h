#pragma once
/**
* @file   WandCharacter.h
* @brief  WandPlayerを管理するクラス。
* @author YamaguchiHayato。
* @date   2026/04/20。
*/

#include "Src/Actor/Character/Player/CharacterByWeapon/IWeaponCharacter.h"

namespace nsApp
{
	namespace nsActor
	{
		class WandCharacter : public IWeaponCharacter
		{
		public:
			WandCharacter() = default;
			virtual ~WandCharacter() = default;


		public:
			/**
			* @brief 初期化。
			*/
			bool Start() override;


		protected:
			/**
			 * @brief ステートを登録する。
			 */
			void RegisterState() override;


		private:
			Quaternion m_angle = Quaternion::Identity; //! モデルの角度。
		};
	}
}
