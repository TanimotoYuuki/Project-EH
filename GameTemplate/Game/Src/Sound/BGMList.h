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
			/* 初期化処理。*/
			void Init();
			/* BGMの再生。*/
			void PlayBGM(BGM_ID id, float volume);
			/* BGMの停止。*/
			void StopBGM();


		public:
			/* ファイルパスを格納。*/
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
