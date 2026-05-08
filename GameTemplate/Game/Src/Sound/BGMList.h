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
			nsK2EngineLow::SoundSource* m_bgmSource = nullptr;//! BGMのサウンドソースを管理するポインタ。


		private:
			BGM_ID m_currentBGM = BGM_ID::None;               //! 現在流れているBGMのIDを記録する変数。
		};
	}
}
