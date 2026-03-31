#include "k2EngineLowPreCompile.h"
#include "GameSoundEngine.h"
namespace
{
	const int BGM_NO_PLAYING = -1; //BGMが再生していない
}

namespace nsK2EngineLow
{
	//初期化
	void GameSoundEngine::Init()
	{
		//BGM
		// タイトル画面BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_Title, "Assets/sound/bgm/title.wav");
		m_isPlayingSound[GameSoundList_BGM_Title] = false;
		m_sound[GameSoundList_BGM_Title] = nullptr;

		// ステージ選択画面BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_StageSelect, "Assets/sound/bgm/selectStage.wav");
		m_isPlayingSound[GameSoundList_BGM_StageSelect] = false;
		m_sound[GameSoundList_BGM_StageSelect] = nullptr;

		// ステージ1BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_Stage1, "Assets/sound/bgm/stage1.wav");
		m_isPlayingSound[GameSoundList_BGM_Stage1] = false;
		m_sound[GameSoundList_BGM_Stage1] = nullptr;

		// ステージ2BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_Stage2, "Assets/sound/bgm/stage2.wav");
		m_isPlayingSound[GameSoundList_BGM_Stage2] = false;
		m_sound[GameSoundList_BGM_Stage2] = nullptr;

		// BOSSBGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_BOSS, "Assets/sound/bgm/boss.wav");
		m_isPlayingSound[GameSoundList_BGM_BOSS] = false;
		m_sound[GameSoundList_BGM_BOSS] = nullptr;

		// ボスにダメージを与える音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_BossHit, "Assets/sound/se/bossHit.wav");
		m_isPlayingSound[GameSoundList_SE_BossHit] = false;
		m_sound[GameSoundList_SE_BossHit] = nullptr;

		// リザルト画面BGM
		g_soundEngine->ResistWaveFileBank(GameSoundList_BGM_Result, "Assets/sound/bgm/result.wav");
		m_isPlayingSound[GameSoundList_BGM_Result] = false;
		m_sound[GameSoundList_BGM_Result] = nullptr;

		// EndRollBGM。
		g_soundEngine->ResistWaveFileBank(GameSoudList_BGM_EndRoll, "Assets/sound/bgm/endRoll.wav");
		m_isPlayingSound[GameSoudList_BGM_EndRoll] = false;
		m_sound[GameSoudList_BGM_EndRoll] = nullptr;

		//SE
		//0. 決定音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_SelectScreen_Decision, "Assets/sound/se/decision.wav");
		m_isPlayingSound[GameSoundList_SE_SelectScreen_Decision] = false;
		m_sound[GameSoundList_SE_SelectScreen_Decision] = nullptr;

		// カーソル移動音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_CursorMove, "Assets/sound/se/CursorMove.wav");
		m_isPlayingSound[GameSoundList_SE_CursorMove] = false;
		m_sound[GameSoundList_SE_CursorMove] = nullptr;

		// 決定音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_CursorMove, "Assets/sound/se/CursorMove.wav");
		m_isPlayingSound[GameSoundList_SE_CursorMove] = false;
		m_sound[GameSoundList_SE_CursorMove] = nullptr;

		// ボタン音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Button, "Assets/sound/se/Button.wav");
		m_isPlayingSound[GameSoundList_SE_Button] = false;
		m_sound[GameSoundList_SE_Button] = nullptr;

		// 落下音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Fall, "Assets/sound/se/fall.wav");
		m_isPlayingSound[GameSoundList_SE_Fall] = false;
		m_sound[GameSoundList_SE_Fall] = nullptr;

		// 回転音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Rotation, "Assets/sound/se/rotation.wav");
		m_isPlayingSound[GameSoundList_SE_Rotation] = false;
		m_sound[GameSoundList_SE_Rotation] = nullptr;

		// ボスのジャンプ音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_BossJump, "Assets/sound/se/BossJump.wav");
		m_isPlayingSound[GameSoundList_SE_BossJump] = false;
		m_sound[GameSoundList_SE_BossJump] = nullptr;

		// 着地音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Land, "Assets/sound/se/Landing.wav");
		m_isPlayingSound[GameSoundList_SE_Land] = false;
		m_sound[GameSoundList_SE_Land] = nullptr;

		// 足場ブロックを生成する音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_SpawnBlock, "Assets/sound/se/SpawnBlock.wav");
		m_isPlayingSound[GameSoundList_SE_SpawnBlock] = false;
		m_sound[GameSoundList_SE_SpawnBlock] = nullptr;

		// 槍攻撃の音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Spear, "Assets/sound/se/Spear.wav");
		m_isPlayingSound[GameSoundList_SE_Spear] = false;
		m_sound[GameSoundList_SE_Spear] = nullptr;

		// 咆哮の音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Roar, "Assets/sound/se/Roar.wav");
		m_isPlayingSound[GameSoundList_SE_Roar] = false;
		m_sound[GameSoundList_SE_Roar] = nullptr;

		// ボスの攻撃UIを出している間鳴らす音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Warning, "Assets/sound/se/Warning.wav");
		m_isPlayingSound[GameSoundList_SE_Warning] = false;
		m_sound[GameSoundList_SE_Warning] = nullptr;

		// ボスにダメージを与える音。
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_BossHit, "Assets/sound/se/bossHit.wav");
		m_isPlayingSound[GameSoundList_SE_BossHit] = false;
		m_sound[GameSoundList_SE_BossHit] = nullptr;


		//1. 決定音(画面遷移用)
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_SelectScreen_Decision_ScreenTransition,"Assets/sound/se/decision_gamestart.wav");
		m_isPlayingSound[GameSoundList_SE_SelectScreen_Decision_ScreenTransition] = false;
		m_sound[GameSoundList_SE_SelectScreen_Decision_ScreenTransition] = nullptr;

		//2. 選択音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_SelectScreen_Select, "Assets/sound/se/select.wav");
		m_isPlayingSound[GameSoundList_SE_SelectScreen_Select] = false;
		m_sound[GameSoundList_SE_SelectScreen_Select] = nullptr;

		//3. キャンセル音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_SelectScreen_Cancel, "Assets/sound/se/cancel.wav");
		m_isPlayingSound[GameSoundList_SE_SelectScreen_Cancel] = false;
		m_sound[GameSoundList_SE_SelectScreen_Cancel] = nullptr;

		//4. プレイヤーの足音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Player_FootStep, "Assets/sound/se/footstep.wav");
		m_isPlayingSound[GameSoundList_SE_Player_FootStep] = false;

		//5. プレイヤーのジャンプ音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Player_Jump, "Assets/sound/se/jump.wav");
		m_isPlayingSound[GameSoundList_SE_Player_Jump] = false;
		m_sound[GameSoundList_SE_Player_Jump] = nullptr;

		//5. カメラアクション音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Player_Flip, "Assets/sound/se/flip.wav");
		m_isPlayingSound[GameSoundList_SE_Player_Flip] = false;
		m_sound[GameSoundList_SE_Player_Flip] = nullptr;


		//7. プレイヤーが敵を踏んだ時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Player_StepOnEnemy, "Assets/sound/se/stepon.wav");
		m_isPlayingSound[GameSoundList_SE_Player_StepOnEnemy] = false;
		m_sound[GameSoundList_SE_Player_StepOnEnemy] = nullptr;

		//8. ブロックを叩いた時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Object_BlockTouch, "Assets/sound/se/blocktouch.wav");
		m_isPlayingSound[GameSoundList_SE_Object_BlockTouch] = false;
		m_sound[GameSoundList_SE_Object_BlockTouch] = nullptr;
		
		//11. リザルト画面で取得したアイテムを表示した時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_Result_ViewItemGet, "Assets/sound/se/viewresult.wav");
		m_isPlayingSound[GameSoundList_SE_Result_ViewItemGet] = false;
		m_sound[GameSoundList_SE_Result_ViewItemGet] = nullptr;


		//16. ステージクリア時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_System_StageClear, "Assets/sound/se/stageclear.wav");
		m_isPlayingSound[GameSoundList_SE_System_StageClear] = false;
		m_sound[GameSoundList_SE_System_StageClear] = nullptr;

		//17. ゲームオーバー時の音
		g_soundEngine->ResistWaveFileBank(GameSoundList_SE_System_GameOver, "Assets/sound/se/gameover.wav");
		m_isPlayingSound[GameSoundList_SE_System_GameOver] = false;
		m_sound[GameSoundList_SE_System_GameOver] = nullptr;
	}

	//BGMを再生
	void GameSoundEngine::PlayBGM(GameSoundList gameSoundList, float soundVolume)
	{
		// すでにその曲が流れていたら何もしない（連打防止）
		if (m_nowBGMPlaying == gameSoundList && m_isPlayingSound[gameSoundList])
		{
			return;
		}

		// ▼▼▼ 前の曲を止める処理を追加 ▼▼▼
		if (m_nowBGMPlaying != -1) // 何か再生中なら
		{
			StopSound((GameSoundList)m_nowBGMPlaying); // 止める

			// メモリリーク防止のため削除しておく（エンジンの仕様によりますが念のため）
			if (m_sound[m_nowBGMPlaying] != nullptr) {
				DeleteGO(m_sound[m_nowBGMPlaying]);
				m_sound[m_nowBGMPlaying] = nullptr;
			}
			m_isPlayingSound[m_nowBGMPlaying] = false;
		}

		// 新しい曲を再生
		m_sound[gameSoundList] = NewGO<SoundSource>(0);
		m_sound[gameSoundList]->Init(gameSoundList);
		m_sound[gameSoundList]->Play(true); // ループ再生
		m_sound[gameSoundList]->SetVolume(soundVolume);

		m_isPlayingSound[gameSoundList] = true;
		m_nowBGMPlaying = gameSoundList; // 現在再生中のIDを更新	}
	}


	//SEを再生
	void GameSoundEngine::PlaySE(GameSoundList gameSoundList, float soundVolume)
	{
		m_sound[gameSoundList] = NewGO<SoundSource>(0);
		m_sound[gameSoundList]->Init(gameSoundList);
		m_sound[gameSoundList]->Play(false);
		m_sound[gameSoundList]->SetVolume(soundVolume);
	}

	//更新処理
	void GameSoundEngine::Update()
	{
		//BGMを再生しているときに処理する
		if (m_nowBGMPlaying != BGM_NO_PLAYING)
		{
			//今再生しているサウンドインスタンスが削除したら
			if (m_sound[m_nowBGMPlaying]->IsDead())
			{
				//サウンドが再生しているかを判定するフラグをfalseにする
				m_isPlayingSound[m_nowBGMPlaying] = false;
			}
		}
	}
}