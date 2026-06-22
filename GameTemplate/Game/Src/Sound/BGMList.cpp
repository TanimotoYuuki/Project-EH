#include "stdafx.h"
#include "BGMList.h"

namespace nsApp
{
	namespace nsSound
	{
		nsK2EngineLow::SoundSource *BGMList::m_bgmSource = nullptr;
		BGM_ID BGMList::m_currentBGM = BGM_ID::None;

		BGMList::~BGMList()
		{
		}

		void BGMList::Init()
		{
			/* BGMのファイルパスを登録。*/
			/* タイトルBGM。*/
			g_soundEngine->ResistWaveFileBank(BGM_ID::Title, GetBGMFilePath("title").c_str());
			/* ステージ1BGM。*/
			g_soundEngine->ResistWaveFileBank(BGM_ID::Stage1, GetBGMFilePath("stage").c_str());
			/* SelectBGM。*/
			g_soundEngine->ResistWaveFileBank(BGM_ID::Select, GetBGMFilePath("select").c_str());
		}

		void BGMList::PlayBGM(BGM_ID id, float volume)
		{
			/* 同じ曲がすでに流れていたら、最初から再生し直さないようにする（連打バグ防止） */
			if (m_currentBGM == id && m_bgmSource != nullptr)
				return;

			/* BGMを停止。*/
			StopBGM();

			/* 新しい曲のスピーカーを作って鳴らす */
			m_bgmSource = NewGO<SoundSource>(0, "BGM_Source");
			m_bgmSource->Init(id);

			/* 音量をセット */
			m_bgmSource->SetVolume(volume);

			/* 基本音量をセット。*/
			m_baseVolume = volume;

			/* 再生。*/
			m_bgmSource->Play(true);

			/* 今流れているBGMを保存。 */
			m_currentBGM = id;
		}


		void BGMList::StopBGM()
		{
			/* スピーカーが存在していたら、音を止めてから消去する */
			if (m_bgmSource != nullptr)
			{
				/* BGMを停止する。*/
				m_bgmSource->Stop();
				/* BGMクラスを削除。*/
				DeleteGO(m_bgmSource);
				/* 再初期化。*/
				m_bgmSource = nullptr;
			}
			/* 音源をリセット。*/
			m_currentBGM = BGM_ID::None;
		}

		void BGMList::CalcVolume(int bgmRate, int masterRate)
		{
			int bgmVolumeRate = bgmRate;
			float bgmVolume = bgmVolumeRate / 100.0f;

			int masterVolumeRate = masterRate;
			float masterVolume = masterVolumeRate / 100.0f;

			float finalVolume = m_baseVolume * bgmVolume * masterVolume;

			m_bgmSource->SetVolume(finalVolume);
		}
	}
}