#include "stdafx.h"
#include "SEList.h"

namespace nsApp
{
	namespace nsSound
	{
		void SEList::Init()
		{
			/* ファイルパスを登録。*/
			/* ソード。*/
			StorageGreatSwordSE();
		}


		void SEList::StorageGreatSwordSE()
		{
			/* ソードのSEファイルパスを登録。*/
			/* 通常攻撃。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::NormalAttack, GetSEFilePath("GreatSword_NormalAttack").c_str());

			/* 連続攻撃。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::RushAttack, GetSEFilePath("GreatSword_RushAttack").c_str());
		}


		void SEList::PlaySE(SE_ID id, float volume)
		{
			volume = 1.0f;

			/* 音源クラス(ライブラリ)の生成。*/
			auto seSource = NewGO<nsK2EngineLow::SoundSource>(0, "SE");

			/* データを代入。*/
			seSource->Init(id);
			/* 音量をセット。*/
			seSource->SetVolume(volume);
			/* 再生。*/
			seSource->Play(false);

		}
	}
}