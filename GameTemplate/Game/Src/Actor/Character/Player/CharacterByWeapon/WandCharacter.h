#pragma once
/**
* @file   WandCharacter.h
* @brief  WandPlayerを管理するクラス。
* @author YamaguchiHayato。
* @date   2026/04/20。
*/

#include "Src/Actor/Character/Player/CharacterByWeapon/IWeaponCharacter.h"

enum class WandMagicMode : uint8_t
{
	enAttackMagic, //! 通常攻撃魔法。
	enHeelMagic,   //! ヒール魔法。
	enNone, 	   //! 魔法なし。
};

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


		/* セッター。*/
		public:
			/* 現在の魔法をセットする。*/
			inline void SetWandMagicMode(WandMagicMode mode)
			{
				m_currentMagicMode = mode;
			}



		/* ゲッター。*/
		public:
			/* 現在の魔法を取得する。*/
			inline WandMagicMode GetWandMagicMode() const
			{
				return m_currentMagicMode;
			}


		private:
			Quaternion m_angle = Quaternion::Identity;                //! モデルの角度。

			WandMagicMode m_currentMagicMode = WandMagicMode::enNone; //! 現在の魔法モード。
		};
	}
}
