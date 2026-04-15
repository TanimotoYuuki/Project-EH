#pragma once
/**
* @file   IWeaponCharacter.h
* @brief  Playerクラスを継承する親クラス。
* @brief  このクラスを継承してSword/Hammer等の実際のプレイアブルキャラクタークラスにステートの情報と共通処理を渡していきます。
* @author YamaguchiHayato。
* @date   2026/04/02
*/

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Common/WeaponHitDetection.h"

namespace nsApp
{
	namespace nsActor
	{
		class IWeaponCharacter : public Player
		{
		public:
			IWeaponCharacter() = default;
			virtual ~IWeaponCharacter() = default;


		public:
			/**
			 * 初期化。
			 */
			virtual bool Start() override ;


		public:
			/**
			 * 武器用のアニメーションを再生する。
			 */
			void CharacterWeaponAnimation(AttackType attack);


		/* ゲッター。*/
		public:
			/* 武器の当たり判定を取得。*/
			WeaponHitDetection& GetWeaponHitDetection()
			{
				return m_weaponHitDetection;
			}


		protected:	
			int m_animationIndex = 0;                      //! 再生するアニメーションのインデックス
		};
	}
}

