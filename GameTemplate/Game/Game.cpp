#include "stdafx.h"
#include "Game.h"
#include "Src/Fade/Fade.h"

#include "Src/Actor/Stage/LoadStageData.h"

#include "CharacterHP.h"
#include "GameTimeLimit.h"
#include "GameStartDirection.h"
#include "GameClearDirection.h"
#include "GameTimeUpDirection.h"
#include "GameOverDirection.h"
#include "GameEndSelect.h"

#include "Src/Camera/Camera.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Sound/SoundLister.h"
#include "Boss.h"
#include "Src/Actor/Stage/BackGround.h"
#include "Src/Actor/Character/Player/Component/PlayerGenerator.h"

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
	namespace nsGame
	{
		Game::~Game()
		{
			nsApp::nsStage::LoadStageData::GetInstance().ChangeStage(nsApp::nsStage::StageID::Invalid);
			DeleteGO(m_soundLister);
			DeleteGO(m_backGround);
			DeleteGO(m_camera);
			DeleteGO(m_player);
		  DeleteGO(m_boss);
			DeleteGO(m_gameClearDirection);
			DeleteGO(m_gameTimeUpDirection);
			DeleteGO(m_gameOverDirection);
			DeleteGO(m_gameEndSelect);

			delete m_generator;
		}


		bool Game::Start()
		{
			/* 音源の生成。*/
			m_soundLister = NewGO<nsSound::SoundLister>(0, "SoundManager");
			m_soundLister->GetBGMList().Init();
			m_soundLister->GetSEList().Init();

			/* 初期ステージのセット。*/
			/* @TODO ステージ選択画面からこの処理を呼ぶようにする。*/
			nsApp::nsStage::LoadStageData::GetInstance().ChangeStage(nsApp::nsStage::StageID::stage1);

			m_backGround = NewGO<nsStage::BackGround>(0, "BackGround");
			/* カメラを生成。*/
			m_camera = NewGO<Camera>(0, "camera");

			PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

			m_player = NewGO<nsActor::Player>(0, "player");

			/*ボスを作成。*/
			m_boss = NewGO<nsActor::Boss>(0, "boss");
			/*ボスの種類を設定。*/
			m_boss->SetBossType(CharacterModelType::TutorialBoss);
			/*ボスにプレイヤーをターゲットとして教える。*/
			m_boss->SetTarget(m_player);

			/*m_sandbag = NewGO<nsActor::Sandbag>(0, "Sandbag"); */
			m_characterHP = NewGO<CharacterHP>(0, "characterHP");
			m_characterHP->Deactivate();

			m_gameTimeLimit = NewGO<GameTimeLimit>(0, "gameTimeLimit");
			m_gameTimeLimit->SetTimeLimit(180);
			m_gameTimeLimit->Deactivate();

			m_gameStartDirection = NewGO<GameStartDirection>(2, "gameStartDirection");
			m_gameStartDirection->Deactivate();

			/*フェードインに切り替える。*/
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeIn);

			/* プレイアブルキャラを生成する。*/
			SpawnPlayCharacter();
			return true;
		}


		void Game::Update()
		{
			if (m_gameStartDirection != nullptr)
			{
				if (m_gameStartDirection->IsDirectionFinished())
				{
					DeleteGO(m_gameStartDirection);
					m_gameStartDirection = nullptr;
					m_characterHP->Activate();
					m_gameTimeLimit->Activate();
					return;
				}

				if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
				{
					m_gameStartDirection->Activate();
				}
			}

			/*ゲームクリア演出。*/
			/*現在は左を入力することで演出を流すようにしている。*/
			/*TODO:今後はボスのHPが0になったら演出を流すようにする。*/
			if (m_gameClearDirection == nullptr)
			{
				if (g_pad[0]->IsTrigger(enButtonLeft))
				{
					m_gameClearDirection = NewGO<GameClearDirection>(2, "gameClearDirection");
					m_characterHP->Deactivate();
					m_gameTimeLimit->Deactivate();
				}
			}

			/*時間切れ演出。*/
			if (m_gameTimeUpDirection == nullptr)
			{
				if (m_gameTimeLimit->IsTimeUp())
				{
					m_gameTimeUpDirection = NewGO<GameTimeUpDirection>(2, "gameTimeUpDirection");
					m_characterHP->Deactivate();
					m_gameTimeLimit->Deactivate();
				}
			}
			else
			{
				if (m_gameEndSelect == nullptr)
				{
					if (m_gameTimeUpDirection->IsDirectionFinished())
					{
						m_gameTimeUpDirection->Deactivate();
						m_gameEndSelect = NewGO<GameEndSelect>(2, "gameEndSelect");
					}
				}
			}

			/* 現在のステージの更新を行う。*/
			nsApp::nsStage::LoadStageData::GetInstance().Update();
		}


		void Game::Render(RenderContext& rc)
		{
			/* 現在のステージを描画する。*/
			nsApp::nsStage::LoadStageData::GetInstance().Draw(rc);
		}


		void Game::SpawnPlayCharacter()
		{
			/* 生成システムクラスを生成する。*/
			m_generator = new PlayerGenerator();

			/* PlayerGeneratorを用い、プレイアブルキャラを作成する。*/
			std::vector<PlayerSpawnData> partyData =
			{
				//{"player1", WeaponType::GreatSword, ControllerType::NPC,INIT_CHARACTER_POSITION_PLAYER1},
				//{"player2", WeaponType::Hammer, ControllerType::NPC,INIT_CHARACTER_POSITION_PLAYER2},
				//{"player3", WeaponType::Wand, ControllerType::Player_1P, INIT_CHARACTER_POSITION_PLAYER3},
				{"player4", WeaponType::TwinGun, ControllerType::Player_1P, INIT_CHARACTER_POSITION_PLAYER4}
			};

			/* 作成したリストをセットする。*/
			m_generator->SpawnPlayers(partyData);
		}
	}
}