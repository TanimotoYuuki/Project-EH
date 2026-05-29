#pragma once
/**
 * @file   SEList.h
 * @brief  SEのみを管理するクラス。
 * @author YamaguchiHayato。
 * @date   2026/05/04
 */

#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	namespace nsSound
	{  
		/*
		 * @struct SEInfo。
		 * @brief 再生するSEの情報を管理する。
	     */
		struct SEInfo
		{
			nsK2EngineLow::SoundSource* source = nullptr;	//! SEのサウンドエンジン。
			float lifeTime = 0.0f;						    //! SEの寿命。
			float currentTime = 0.0f;					    //! SEの現在の経過時間。
			bool isLoop = false;					        //! SEをループ再生するかどうかのフラグ。
		};


		/* SEのIDリスト。*/
		enum SE_ID : uint8_t
		{
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
			Rescue,				 //! 救助中のSE。

		};


		class SEList
		{
		public:
			/* コンストラクタとデストラクタ。*/
			SEList() = default;
			virtual ~SEList();


		public:
			/* 初期化処理。*/
			void Init();

			/*
			 * @brief 更新処理。
			 * @param deltaTime: 前フレームからの経過時間。
			 */
			void Update(float deltaTime);

			/*
			 * @brief SEのキャッシュを解放する。
			 */
			void Clear();


			/* 
			 * @brief SEの再生。
			 * @param id: SEの識別子。
			 * @param volume: SEの音量。
			 * @param flag: ループ再生するかどうかのフラグ。
			 * @param lifeTime  単発SEの寿命。
			 */ 
			nsK2EngineLow::SoundSource* PlaySE(SE_ID id, float volume, bool flag, float lifeTime);

			/**
			 * @brief 指定したSEを停止して削除する。
			 * @param soundSource 停止したいSoundSource。
			 */
			void StopSE(nsK2EngineLow::SoundSource*& soundSource);

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
			std::vector<SEInfo> m_playingSEs;													 //! 再生中のSEの情報を管理するベクター。

			bool m_isLoop = false;																 //! ループ再生するかどうかのフラグ。
		};
	}
}
