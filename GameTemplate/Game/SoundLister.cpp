#include "stdafx.h"
#include "SoundLister.h"


namespace nsApp
{
	namespace nsSound
	{
		bool SoundLister::Start()
		{
			/* ‚»‚ê‚¼‚ê‚ğ“Ç‚İ‚ŞB*/
			m_bgmList.Init();
			m_seList.Init();

			return true;
		}
	}
}