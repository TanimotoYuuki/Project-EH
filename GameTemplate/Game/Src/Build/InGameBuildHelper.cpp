#include "stdafx.h"
#include "InGameBuildHelper.h"

#include <time.h>
#include "Src/Actor/Character/Boss/Status/BossParameterTable.h"
#include "Src/Actor/Stage/LoadStageData.h"
#include "Src/Actor/Stage/BackGround.h"
#include "Src/Camera/Camera.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Actor/Character/Common/Damage/DamageIndicatorPool.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include "Src/Actor/Character/Player/Component/PlayerGenerator.h"
#include "Src/Actor/Character/Player/InputSystem/PlayerControlerHub.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/UI/Character/CharacterHP.h"
#include "GameTimeLimit.h"
#include "Src/Direction/GameStartDirection.h"
#include "Src/Scene/InGame/Pause.h"
#include "Src/UI/ReboneGage/ReboneGageManager.h"
#include "Src/UI/Commentary/CommentaryUIManager.h"
#include "Src/UI/GuardGage/GuardGageUIManager.h"

namespace
{
	const auto INIT_CHARACTER_POSITION_Y = 50.0f;
	const auto INIT_CHARACTER_POSITION_Z = 0.0f;
	const auto INIT_CHARACTER_POSITION_PLAYER1 = Vector3(80.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
	const auto INIT_CHARACTER_POSITION_PLAYER2 = Vector3(-80.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
	const auto INIT_CHARACTER_POSITION_PLAYER3 = Vector3(-50.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
	const auto INIT_CHARACTER_POSITION_PLAYER4 = Vector3(50.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
}

namespace nsApp
{
	using namespace nsStage;

	void InGameBuildHelper::Initialize(const InGameBuildRequest &request)
	{
		/* 生成に必要な情報を保存する。*/
		m_request = request;
		m_result = InGameBuildResult();

		/* 生成に必要なデータを初期化する。*/
		m_partyData.clear();
		m_players.clear();
		m_buildFunctions.clear();

		/* 生成の進行状況を初期化する。*/
		m_currentBuildIndex = 0;
		m_isFinished = false;

		/* 生成関数の初期化。*/
		InitializeBuildFunctions();
	}

	void InGameBuildHelper::InitializeBuildFunctions()
	{
		/* 生成関数の初期化。*/
		m_buildFunctions.clear();

		/* 生成関数の追加。*/
		m_buildFunctions.push_back([this]()
								   { BuildRandom(); });
		m_buildFunctions.push_back([this]()
								   { BuildSound(); });
		m_buildFunctions.push_back([this]()
								   { BuildStage(); });
		m_buildFunctions.push_back([this]()
								   { BuildBackGround(); });
		m_buildFunctions.push_back([this]()
								   { BuildCamera(); });
		m_buildFunctions.push_back([this]()
								   { BuildDamagePool(); });
		m_buildFunctions.push_back([this]()
								   { BuildCommentaryUI(); });
		m_buildFunctions.push_back([this]()
								   { BuildPlayerGenerator(); });
		m_buildFunctions.push_back([this]()
								   { CreatePartyData(); });
		m_buildFunctions.push_back([this]()
								   { SpawnPlayer(0); });
		m_buildFunctions.push_back([this]()
								   { SpawnPlayer(1); });
		m_buildFunctions.push_back([this]()
								   { SpawnPlayer(2); });
		m_buildFunctions.push_back([this]()
								   { SpawnPlayer(3); });
		m_buildFunctions.push_back([this]()
								   { RegisterPlayersToReboneGauge(); });
		m_buildFunctions.push_back([this]()
								   { BuildGuardGaugeUI(); });
		m_buildFunctions.push_back([this]()
								   { RegisterPlayersToGuardGauge(); });
		m_buildFunctions.push_back([this]()
								   { BuildPlayerHub(); });
		m_buildFunctions.push_back([this]()
								   { BuildBoss(); });
		m_buildFunctions.push_back([this]()
								   { BuildCharacterHP(); });
		m_buildFunctions.push_back([this]()
								   { BuildGameTimeLimit(); });
		m_buildFunctions.push_back([this]()
								   { BuildGameStartDirection(); });
		m_buildFunctions.push_back([this]()
								   { BuildPause(); });
		m_buildFunctions.push_back([this]()
								   { FinishBuild(); });
	}

	void InGameBuildHelper::ExecuteNextBuildFunction()
	{
		/* 生成が完了している場合は何もしない。*/
		if (m_isFinished)
			return;

		/* 次の生成関数が存在しない場合は生成完了とする。*/
		if (m_currentBuildIndex >= static_cast<int>(m_buildFunctions.size()))
		{
			m_isFinished = true;
			return;
		}

		/* 次の生成関数を実行する。*/
		m_buildFunctions[m_currentBuildIndex]();

		/* 次の生成関数に進む。*/
		++m_currentBuildIndex;
	}

	void InGameBuildHelper::BuildRandom()
	{
		/* 乱数の初期化。*/
		srand(static_cast<unsigned int>(time(nullptr)));
	}

	void InGameBuildHelper::BuildSound()
	{
		/* サウンド管理クラスの生成。*/
		m_result.soundLister = FindGO<nsSound::SoundLister>("SoundManager");

		/* サウンド管理クラスが存在しない場合は生成する。*/
		if (m_result.soundLister == nullptr)
			m_result.soundLister = NewGO<nsSound::SoundLister>(0, "SoundManager");

		/* サウンド管理クラスの初期化。*/
		m_result.soundLister->InitSound();

		/* BGMの再生。*/
		m_result.soundLister->GetBGMList().StopBGM();

		/* ステージ1のBGMを再生する。*/
		m_result.soundLister->GetBGMList().PlayBGM(nsSound::BGM_ID::Stage1, 1.0f);
	}

	void InGameBuildHelper::BuildStage()
	{
		/* ステージ1に切り替える。*/
		LoadStageData::GetInstance().ChangeStage(StageID::stage1);
	}

	void InGameBuildHelper::BuildBackGround()
	{
		/* 背景の生成。*/
		m_result.backGround = NewGO<BackGround>(0, "BackGround");
	}

	void InGameBuildHelper::BuildCamera()
	{
		m_result.camera = FindGO<Camera>("camera");

		if (m_result.camera == nullptr)
			m_result.camera = NewGO<Camera>(0, "camera");
	}

	void InGameBuildHelper::BuildDamagePool()
	{
		/* ダメージインジケーターのプールの生成。*/
		m_result.damageIndicatorPool = NewGO<DamageIndicatorPool>(0, "damagePool");

		/* ダメージインジケーターのプールをDamageProcessorに設定する。*/
		DamageProcessor::SetDamageIndicatorPool(m_result.damageIndicatorPool);
	}

	void InGameBuildHelper::BuildCommentaryUI()
	{
		/* リボーンゲージUIの生成。*/
		m_result.reboneGaugeUIManager = new nsUI::ReboneGaugeUIManager();

		/* リボーンゲージUIの初期化。*/
		m_result.reboneGaugeUIManager->Init();

		/* 実況UIの生成。*/
		m_result.commentaryUIManager = NewGO<nsUI::CommentaryUIManager>(0, "CommentaryUIManager");

		/* ロード中は描画しない。*/
		if (m_result.commentaryUIManager != nullptr)
			m_result.commentaryUIManager->Deactivate();
	}

	void InGameBuildHelper::BuildPlayerGenerator()
	{
		/* PlayerGeneratorの生成。*/
		m_result.generator = new PlayerGenerator();
	}

	void InGameBuildHelper::CreatePartyData()
	{
		/* パーティーデータの初期化。*/
		m_partyData.clear();

		/* コントローラータイプの設定。*/
		for (int i = 0; i < 4; i++)
		{
			/* プレイヤーが操作しているかどうかでコントローラータイプを設定する。*/
			if (m_request.isPlayerControle[i])
				m_controllerType[i] = static_cast<ControllerType>(i);

			/* プレイヤーが操作していない場合はNPCに設定する。*/
			else
				m_controllerType[i] = ControllerType::NPC;
		}

		/* パーティーデータの作成。*/
		m_partyData =
			{
				{"player1",
				 static_cast<WeaponType>(m_request.characterRole[0]),
				 m_controllerType[0],
				 INIT_CHARACTER_POSITION_PLAYER1},
				{"player2",
				 static_cast<WeaponType>(m_request.characterRole[1]),
				 m_controllerType[1],
				 INIT_CHARACTER_POSITION_PLAYER2},
				{"player3",
				 static_cast<WeaponType>(m_request.characterRole[2]),
				 m_controllerType[2],
				 INIT_CHARACTER_POSITION_PLAYER3},
				{"player4",
				 static_cast<WeaponType>(m_request.characterRole[3]),
				 m_controllerType[3],
				 INIT_CHARACTER_POSITION_PLAYER4}};
	}

	void InGameBuildHelper::SpawnPlayer(int playerIndex)
	{
		/* PlayerGeneratorが初期化されていない場合は生成しない。*/
		if (m_result.generator == nullptr)
			return;

		/* プレイヤーの生成に必要なデータが存在するか。*/
		if (playerIndex < 0 || playerIndex >= static_cast<int>(m_partyData.size()))
			return;

		/* プレイヤーの生成。*/
		nsActor::Player *player = m_result.generator->SpawnPlayer(m_partyData[playerIndex]);

		/* プレイヤーの生成に失敗した。*/
		if (player == nullptr)
			return;

		/* プレイヤーの登録。*/
		m_players.push_back(player);

		/* 代表プレイヤーを登録する。*/
		if (m_result.player == nullptr)
			m_result.player = player;

		/* ロード中は描画しない。*/
		player->Deactivate();
	}

	void InGameBuildHelper::RegisterPlayersToReboneGauge()
	{
		/* リボーンゲージUIが生成されていない場合は登録しない。*/
		if (m_result.reboneGaugeUIManager == nullptr)
			return;

		/* 生成済みのプレイヤーをリボーンゲージUIに登録する。*/
		for (auto *player : m_players)
			m_result.reboneGaugeUIManager->RegisterPlayer(player);
	}

	void InGameBuildHelper::BuildGuardGaugeUI()
	{
		/* ガードゲージUIの生成。*/
		m_result.guardGaugeUIManager = new nsUI::GuardGaugeUIManager();

		/* ガードゲージUIの初期化。*/
		if (m_result.guardGaugeUIManager != nullptr)
			m_result.guardGaugeUIManager->Init();
	}

	void InGameBuildHelper::RegisterPlayersToGuardGauge()
	{
		/* ガードゲージUIが生成されていない場合は登録しない。*/
		if (m_result.guardGaugeUIManager == nullptr)
			return;

		/* 生成済みのプレイヤーをガードゲージUIに登録する。*/
		for (auto *player : m_players)
			m_result.guardGaugeUIManager->RegisterPlayer(player);
	}

	void InGameBuildHelper::BuildPlayerHub()
	{
		/* PlayerHubの生成。*/
		m_result.playerHub = new PlayerControlerHub();

		/* PlayerHubの初期化。*/
		m_result.playerHub->Initialize(m_players, m_partyData);
	}

	void InGameBuildHelper::BuildBoss()
	{
		nsAI::BossParameterTable::LoadTSVFile("Assets/Parameter/BossType.tsv");

		/* ボスの生成。*/
		m_result.boss = NewGO<nsActor::Boss>(0, "boss");

		/* ボスの設定。*/
		if (m_result.boss == nullptr)
			return;

		/* ボスタイプの設定。*/
		m_result.boss->SetBossType(static_cast<CharacterModelType>(m_request.bossType + 4));

		/* ターゲットの設定。*/
		m_result.boss->SetTarget(m_result.player);

		/* ロード中は描画しない。*/
		m_result.boss->Deactivate();
	}

	void InGameBuildHelper::BuildCharacterHP()
	{
		/* キャラクターHPの生成。*/
		m_result.characterHP = NewGO<nsGame::CharacterHP>(0, "characterHP");

		/* キャラクターHPの設定。*/
		if (m_result.characterHP == nullptr)
			return;

		/* キャラクターごとの役割を設定。*/
		for (int i = 0; i < nsGame::CharacterHP::EnCharacter::enCharacter_Num; i++)
			m_result.characterHP->SetCharacterRole(i, m_request.characterRole[i]);

		/* キャラクターHPの非アクティブ化。*/
		m_result.characterHP->Deactivate();
	}

	void InGameBuildHelper::BuildGameTimeLimit()
	{
		/* ゲーム制限時間の生成。*/
		m_result.gameTimeLimit = NewGO<nsGame::GameTimeLimit>(0, "gameTimeLimit");

		/* ゲーム制限時間の設定。*/
		if (m_result.gameTimeLimit == nullptr)
			return;

		/* ゲーム制限時間の時間を設定。*/
		m_result.gameTimeLimit->SetTimeLimit(180);

		/* ゲーム制限時間の非アクティブ化。*/
		m_result.gameTimeLimit->Deactivate();
	}

	void InGameBuildHelper::BuildGameStartDirection()
	{
		/* ゲーム開始演出の生成。*/
		m_result.gameStartDirection =
			NewGO<nsGame::GameStartDirection>(2, "gameStartDirection");

		/* 非アクティブ化する。*/
		if (m_result.gameStartDirection != nullptr)
			m_result.gameStartDirection->Deactivate();
	}

	void InGameBuildHelper::BuildPause()
	{
		/* ポーズ画面の生成。*/
		m_result.pause = NewGO<nsGame::Pause>(0, "pause");

		/* 非アクティブ化する。*/
		if (m_result.pause != nullptr)
			m_result.pause->Deactivate();
	}

	void InGameBuildHelper::FinishBuild()
	{
		/* 生成済みプレイヤーを結果に渡す。*/
		m_result.players = m_players;

		/* 生成に使用したパーティーデータを渡す。*/
		m_result.partyData = m_partyData;

		/* 生成完了。*/
		m_isFinished = true;
	}

	float InGameBuildHelper::GetProgress() const
	{
		/* 生成関数が存在しない場合は進捗率を返す。*/
		if (m_buildFunctions.empty())
		{
			/* 生成が完了している場合は進捗率を1.0fにする。*/
			if (m_isFinished)
				return 1.0f;

			/* 生成が完了していない場合は進捗率を0.0fにする。*/
			return 0.0f;
		}

		/* 進捗率を計算する。*/
		float progress = static_cast<float>(m_currentBuildIndex) / static_cast<float>(m_buildFunctions.size());

		/* 進捗率を0.0f〜1.0fの範囲に収める。*/
		if (progress < 0.0f)
			return 0.0f;
		if (progress > 1.0f)
			return 1.0f;

		/* 進捗率を返す。*/
		return progress;
	}
}