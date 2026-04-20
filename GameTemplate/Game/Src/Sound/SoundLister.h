#pragma once
/**
 * @file   SoundLister.h
 * @brief  BGM/SEを管理するクラス。
 * @author YamaguchiHayato
 * @date   2026/03/21
*/

#include "BGMList.h"
#include "SEList.h"


namespace nsApp
{
	namespace nsSound
	{

		class BGMList;
		class SEList;

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
			inline BGMList& GetBGMList()
			{
				return m_bgmList;
			}

			/* SEのリストを取得する。*/
			inline SEList& GetSEList()
			{
				return m_seList;
			}


		private:
			nsSound::BGMList m_bgmList; //! BGM担当。
			nsSound::SEList m_seList;   //! SE担当。
		};
	}
}
