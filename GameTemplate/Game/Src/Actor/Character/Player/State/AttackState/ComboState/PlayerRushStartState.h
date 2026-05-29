#pragma once
/*
* @file   PlayerRushStartState.h
* @brief  プレイヤーの連続攻撃開始状態を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/17
*/

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"
#include "Src/Actor/Magic/MagicProjectotile.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerRushStartState  :public PlayerAttackBaseState
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerRushStartState() = default;
			virtual ~PlayerRushStartState() = default;


		protected:
			void PlayAttackAnimation() override;
			void OnEnterAttack() override;
			bool OnUpdateAttack() override;
			void OnExitAttack() override;
			bool OnRequestAttackID(uint8_t& id) override;
			void OnAttackTick() override;


		private:
			/* 前進する処理。*/
			void MoveForward();

			/* ミサイルを召喚。*/
			void SummonMissile();

			/* キーフレームの数に応じてボーンの名前を変更する処理。*/ 
			void AdjustBoneNameByKeyFrameNumber();

			/* 銃の左右での撃ち分け処理。*/
			void FireRushBullet(const wchar_t* boneName);


		private:
			int m_loopCount = 0;                  //! 連続攻撃のループ回数を管理するカウンター。

			float m_forwardSpeed = 0.0f;          //! 連続攻撃をする際、前方への移動速度。

			bool m_isSummoned = false;            //! ミサイルを射出したかどうかを管理するフラグ。
			bool m_isButtonReleased = false;      //! 攻撃ボタンが離されたかどうかを管理するフラグ。
			bool m_isReleased = false;			  //! 

			Vector3 m_moveVector = Vector3::Zero; //! 前進する際の移動ベクトル。
			Vector3 m_spawnPos = Vector3::Zero;   //! ミサイルを召喚する位置。
			Vector3 m_subWeaponSpawnPos;		  //! サブ武器の生成位置。
		};
	}
}
