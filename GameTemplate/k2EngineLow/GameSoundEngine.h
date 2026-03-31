#pragma once
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

//ゲームサウンドリスト
enum GameSoundList
{
	// BGM
	GameSoundList_BGM_Title,									//タイトル画面BGM
	GameSoundList_BGM_StageSelect,							    //ステージセレクト画面BGM
	GameSoundList_BGM_Stage1,									//ステージ1BGM
	GameSoundList_BGM_Stage2,									//ステージ2BGM
	GameSoundList_BGM_BOSS,									    //BOSS戦
	GameSoundList_BGM_Result,									//リザルト画面BGM
	GameSoudList_BGM_EndRoll,									//エンドロールBGM

	// SE
	GameSoundList_SE_CursorMove,						        // カーソル移動音
    GameSoundList_SE_Button,                                    // ボタン音
	GameSoundList_SE_SelectScreen_Decision,						// 決定音
	GameSoundList_SE_Fall,						                // 落下音。
	GameSoundList_SE_Rotation,					                // 回転音。
	GameSoundList_SE_BossHit,				                    // ボスにダメージを与える音。
	GameSoundList_SE_StarRotation,				                // スター回転音。
	GameSoundList_SE_BossJump,				                    // ボスのジャンプ音。
	GameSoundList_SE_Land,                                      // 着地音。
	GameSoundList_SE_SpawnBlock,				                // 足場ブロックを生成する音。
	GameSoundList_SE_Spear,				                        // 槍攻撃の音。
	GameSoundList_SE_Roar,				                        // 咆哮の音。
	GameSoundList_SE_Warning,				                    // ボスの攻撃UIを出している間鳴らす音。


	GameSoundList_SE_SelectScreen_Decision_ScreenTransition,	//決定音(画面遷移用)
	GameSoundList_SE_SelectScreen_Select,						//選択音
	GameSoundList_SE_SelectScreen_Cancel,						//キャンセル音
	GameSoundList_SE_Player_FootStep,							//プレイヤーの足音
	GameSoundList_SE_Player_Jump,								//プレイヤーのジャンプ音
	GameSoundList_SE_Player_Flip,								//プレイヤーのジャンプ音
	GameSoundList_SE_Player_Miss,								//プレイヤーがミスした時の音
	GameSoundList_SE_Player_StepOnEnemy,						//プレイヤーが敵を踏んだ時の音
	GameSoundList_SE_Object_BlockTouch,							//ブロックを叩いた時の音
	GameSoundList_SE_Object_ItemGet,							//アイテムを取得した時の音
	GameSoundList_SE_Object_PassingCheckPoint,					//中間地点を通過した時の音
	GameSoundList_SE_Result_ViewItemGet,						//リザルト画面で取得したアイテムを表示した時の音
	GameSoundList_SE_Result_CRank,								//Cランク用の音
	GameSoundList_SE_Result_BRank,								//Bランク用の音
	GameSoundList_SE_Result_ARank,								//Aランク用の音
	GameSoundList_SE_Result_SRank,								//Sランク用の音
	GameSoundList_SE_System_StageClear,							//ステージクリア時の音
	GameSoundList_SE_System_GameOver,							//ゲームオーバー時の音
	GameSoundList_Num											//ゲームサウンドリスト数
};

/// <summary>
/// サウンドエンジン
/// </summary>
namespace nsK2EngineLow
{
	class GameSoundEngine
	{
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// BGMを再生
		/// </summary>
		/// <param name="soundList">ゲームサウンドリスト</param>
		/// <param name="soundVolume">サウンドの音量</param>
		void PlayBGM(GameSoundList gameSoundList, float soundVolume);

		/// <summary>
		/// SEを再生
		/// </summary>
		/// <param name="soundList">ゲームサウンドリスト</param>
		/// <param name="soundVolume">サウンドの音量</param>
		void PlaySE(GameSoundList gameSoundList, float soundVolume);

		/// <summary>
		/// サウンドの音量の設定
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		/// <param name="soundVolume">サウンドの音量</param>
		void SetVolume(GameSoundList gameSoundList, float soundVolume)
		{
			m_sound[gameSoundList]->SetVolume(soundVolume);
		}

		/// <summary>
		/// サウンドの音量を取得
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		/// <returns>サウンドの音量</returns>
		float GetVolume(GameSoundList gameSoundList)
		{
			return m_sound[gameSoundList]->GetVolume();
		}

		/// <summary>
		/// 再生中？
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		/// <returns>trueなら再生中</returns>
		bool IsPlayingSound(GameSoundList gameSoundList)
		{
			return m_isPlayingSound[gameSoundList];
		}

		/// <summary>
		/// サウンドの停止
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		void StopSound(GameSoundList gameSoundList)
		{
			m_sound[gameSoundList]->Stop();
		}

		/// <summary>
		/// サウンドの一時停止
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		void PauseSound(GameSoundList gameSoundList)
		{
			m_sound[gameSoundList]->Pause();
		}

		/// <summary>
		/// サウンド用インスタンスを取得
		/// </summary>
		/// <param name="gameSoundList">ゲームサウンドリスト</param>
		/// <returns>サウンド用インスタンス</returns>
		SoundSource* GetSoundInstance(GameSoundList gameSoundList)
		{
			return m_sound[gameSoundList];
		}
	private:
		int			 m_nowBGMPlaying = -1;					//今何のBGMが再生されているのか？
		bool		 m_isPlayingSound[GameSoundList_Num];	//サウンドが再生されているか？
		SoundSource* m_sound[GameSoundList_Num];			//サウンド再生用インスタンス
	};
}

