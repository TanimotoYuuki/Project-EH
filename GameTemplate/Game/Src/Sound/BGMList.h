#pragma once
/**
 * @file   BGMList
 * @brief  BGMを管理するクラス。
 * @author YamaguchiHayato。
 * @date   2026/03/21
*/

namespace nsApp
{
	namespace nsSound
	{
		/* BGMのリスト。*/
		enum BGM_ID : uint8_t
		{
			Title,
			Tutorial,
			Select,
			Stage1,
			Stage2,
			Stage3,
			Result,
			None
		};

		class BGMList
		{
		public:
			/* コンストラクタとデストラクタ。*/
			BGMList() = default;
			virtual ~BGMList();


		public:
			/* 
			 * @brief 初期化処理。
			 */
			void Init();

			/* 
			 * @brief BGMの再生。
			 * @param id BGMの種類。
			 * @param volume BGMの大きさ。
			 */
			void PlayBGM(BGM_ID id, float volume);

			/* 
			 * @brief BGMの停止。
			 */
			void StopBGM();

		public:
			/**
			* @brief 音量の割合を考慮した音量の計算処理。
			* @param bgmRate BGMの音量の割合。
			* @param masterRate マスター音量の割合。
			*/
			void CalcVolume(int bgmRate, int masterRate);

		public:
			/* 
			 * @brief ファイルパスを格納。
			 * @param bgmName BGMの名前。
			 */
			inline const std::string GetBGMFilePath(std::string bgmName)
			{
				std::string filePath = "Assets/sound/BGM/" + bgmName + ".wav";
				return filePath;
			}


		private:
			static nsK2EngineLow::SoundSource* m_bgmSource; //! BGMを鳴らすスピーカー。
			float m_baseVolume = 0.0f;/*BGMの基本音量。*/

		private:
			static BGM_ID m_currentBGM	;                   //! 現在流れているBGMのIDを記録する変数。
		};
	}
}
