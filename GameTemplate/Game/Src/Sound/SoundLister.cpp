#include "stdafx.h"
#include "SoundLister.h"


namespace nsApp
{
	namespace nsSound
	{
		bool SoundLister::Start()
		{
			/* BGMとSEの音源データをロード。 */
			InitSound();

			return true;
		}


		void SoundLister::InitSound()
		{
			if (m_isInitialized)
				return;

			/* BGMとSEを初期化。*/
			m_bgmList.Init();
			m_seList.Init();

			/* フラグをセット。*/
			m_isInitialized = true;
		}
	}
}