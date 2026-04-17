#pragma once
/**
* @file   SEList.h
* @brief  SEのみを管理するクラス。
* @author YamaguchiHayato。
* @date   2026/03/21
*/


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
			void PlaySE(SE_ID id, float volume);


		public:
			/* ファイルパスを格納する。*/
			inline const std::string GetSEFilePath(std::string seName)
			{
				std::string filePath = "Assets/sound/SE/" + seName + ".wav";
				return filePath;
			}


		private:
			/* SEの種類ごとに格納関数を作る。*/
			void StorageGreatSwordSE();
		};

	}
}
