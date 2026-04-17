#pragma once
/**
* @file   SEList.h
* @brief  SEのみを管理するクラス。
* @author YamaguchiHayato。
* @date   2026/03/21
*/

#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	namespace nsSound
	{
		/* SEのIDリスト。*/
		enum SE_ID : uint8_t
		{
			/*
			 * SEの種類を記載していく。
			 * @TODO: 必要になった際にSEを足していく。
			 */

			/* Sword。*/
			NormalAttack_Sword,  //! 通常攻撃のSE。
			RushAttack_Sword,    //! 連続攻撃のSE。


			/* Hammer*/
			NormalAttack_Hammer, //! 通常攻撃のSE。
			HeavyAttack_Hammer,  //! 重攻撃のSE。
			AirAttack_Hammer,    //! 空中攻撃のSE。
			ChargeAttack_Hammer, //! 貯め攻撃のSE。
			DashAttack_Hammer,   //! ダッシュ攻撃のSE。

			/* その他。*/
			Charge,              //! チャージ中のSE。

		};


		class SEList
		{
		public:
			/* コンストラクタとデストラクタ。*/
			SEList() = default;
			~SEList() = default;


		public:
			/* 初期化処理。*/
			void Init();

			/* SEの再生。*/ 
			nsK2EngineLow::SoundSource* PlaySE(SE_ID id, float volume, bool flag);

			/* @fun 
			 * @brief 武器と攻撃タイプからSEを判別。
			 * @param wepon  武器の種類。
			 * @param attack 攻撃の種類。
			 */
			nsK2EngineLow::SoundSource* PlayAttackSE(WeaponType wepon, AttackType attack);


		private:
			/* 攻撃の種類ごとのSEを管理するテーブル。*/
			/* ソード。*/
			void RegisterSwordSEBank();


			/* ハンマー。*/
			void RegisterHammerSEBank();


		public:
			/* ファイルパスを格納する。*/
			inline const std::string GetSEFilePath(std::string seName)
			{
				std::string filePath = "Assets/sound/SE/" + seName + ".wav";
				return filePath;
			}


		private:
			/* SEの種類ごとに格納関数を作る。*/
			/* ソード。*/
			void StorageGreatSwordSE();

			/* ハンマー。*/
			void StorageHammerSE();

			/* その他の効果音。*/
			void StorageOtherSE();


		private:
			std::unordered_map<WeaponType, std::unordered_map<AttackType, SE_ID>> m_attackSEmap; //! 武器と攻撃タイプからSE_IDを紐づけるマップ。

			bool m_isLoop = false;																 //! ループ再生するかどうかのフラグ。
		};

	}
}
