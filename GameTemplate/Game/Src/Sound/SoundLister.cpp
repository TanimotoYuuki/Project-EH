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

		void SoundLister::Update()
		{
			/*SEの再生時間を管理して、寿命が来たら片す。*/
			m_seList.Update(g_gameTime->GetFrameDeltaTime());
			/* BGMとSEの音量の割合を考慮した計算処理。*/
			// m_bgmList.CalcVolume(m_bgmVolumeRate, m_masterVolumeRate);
			// m_seList.CalcVolume(m_seVolumeRate, m_masterVolumeRate);
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