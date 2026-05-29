#include "stdafx.h"
#include "SoundLister.h"


namespace nsApp
{
	namespace nsSound
	{
		bool SoundLister::Start()
		{
			/* BGMとSEの音源データをロード。 */
			/* BGM。*/
			m_bgmList.Init();

			/* SE。*/
			m_seList.Init();

			return true;
		}

		void SoundLister::Update()
		{
			/* BGMとSEの音量の割合を考慮した計算処理。*/
			m_bgmList.CalcVolume(m_bgmVolumeRate, m_masterVolumeRate);
			m_seList.CalcVolume(m_seVolumeRate, m_masterVolumeRate);
		}
	}
}