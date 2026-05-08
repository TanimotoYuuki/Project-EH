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
	}
}