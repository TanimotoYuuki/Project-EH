#pragma once
/**
 * @file   SoundLister.h
 * @brief  BGM/SEを管理するクラス。
 * @author YamaguchiHayato
 * @date   2026/03/21
*/


namespace nsApp
{
	namespace nsSound
	{
		class SoundLister : public IGameObject
		{
		public:
			/* コンストラクタとデストラクタ。*/
			SoundLister() = default;
			~SoundLister() = default;


		public:
			bool Start();


		/* ゲッター。*/
		public:
			/* BGMのリストを取得する。*/
			const std::vector<std::string>& GetBGMs() const
			{
				return m_bgmList;
			}

			/* SEのリストを取得する。*/
			const std::vector<std::string>& GetSEs() const
			{
				return m_seList;
			}


		private:
			/* BGMのリスト。*/
			std::vector<std::string> m_bgmList;
			/* SEのリスト。*/
			std::vector<std::string> m_seList;
		};
	}
}
