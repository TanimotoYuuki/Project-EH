#include "stdafx.h"
#include "SoundLister.h"


namespace nsApp
{
	namespace nsSound
	{
		bool SoundLister::Start()
		{
			/* BGMとSEの音源データを一気に読み込む！ */
			m_bgmList.Init();
			m_seList.Init();

			/* 最初から鳴らしたいBGMがあれば、ここで呼ぶのもアリです */
			m_bgmList.PlayBGM(BGM_ID::Title, 1.0f);

			return true;
		}
	}
}