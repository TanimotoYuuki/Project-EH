#include "stdafx.h"
#include "BGMList.h"

namespace nsApp
{
	namespace nsSound
	{
		BGMList::~BGMList()
		{
			/* 画面が切り替わる時などに、確実に音を止めてスピーカーを消す */
			StopBGM();
		}

		void BGMList::Init()
		{
			/* BGMのファイルパスを登録。*/
			//g_soundEngine->ResistWaveFileBank(BGM_ID::Title, GetBGMFilePath("title").c_str());
			//g_soundEngine->ResistWaveFileBank(BGM_ID::Stage1, GetBGMFilePath("stage1").c_str());
			//g_soundEngine->ResistWaveFileBank(BGM_ID::Stage2, GetBGMFilePath("stage2").c_str());
			//g_soundEngine->ResistWaveFileBank(BGM_ID::Result, GetBGMFilePath("result").c_str());
		}

		void BGMList::PlayBGM(BGM_ID id, float volume)
		{
			/* 同じ曲がすでに流れていたら、最初から再生し直さないようにする（連打バグ防止） */
			if (m_currentBGM == id && m_bgmSource != nullptr)
				return;

			/* 前の曲が流れていたら、しっかり止めてスピーカーを壊す */
			StopBGM();

			/* 新しい曲のスピーカーを作って鳴らす */
			m_bgmSource = NewGO<SoundSource>(0, "BGM_Source");
			m_bgmSource->Init(id);

			/* 音量をセット */
			m_bgmSource->SetVolume(volume);

			/* 再生する！（true を渡すとループ再生になります！） */
			m_bgmSource->Play(true);

			/* 今流れている曲のIDをメモしておく */
			m_currentBGM = id;
		}

		void BGMList::StopBGM()
		{
			/* スピーカーが存在していたら、音を止めてから消去する */
			if (m_bgmSource != nullptr)
			{
				m_bgmSource->Stop();
				DeleteGO(m_bgmSource);
				m_bgmSource = nullptr;
			}
			/* 記録を None（何も鳴っていない状態）にリセット */
			m_currentBGM = BGM_ID::None;
		}
	}
}