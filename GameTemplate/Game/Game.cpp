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
#include "Src/Debug/Sandbag.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Actor/Stage/BackGround.h"

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
			DeleteGO(m_player2);
			DeleteGO(m_sandbag);
			DeleteGO(m_gameClearDirection);
			DeleteGO(m_gameTimeUpDirection);
			DeleteGO(m_gameOverDirection);
			DeleteGO(m_gameEndSelect);
		}


		bool Game::Start()
		{
			/* 初期ステージのセット。*/
			/* @TODO ステージ選択画面からこの処理を呼ぶようにする。*/
			nsApp::nsStage::LoadStageData::GetInstance().ChangeStage(nsApp::nsStage::StageID::stage1);

			m_soundLister = NewGO<nsSound::SoundLister>(0, "SoundManager");
			m_backGround = NewGO<nsStage::BackGround>(0, "BackGround");
			/* カメラを生成。*/
			m_camera = NewGO<Camera>(0, "camera");

			PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

			m_characterHP = NewGO<CharacterHP>(0, "characterHP");
			m_characterHP->Deactivate();

			m_gameTimeLimit = NewGO<GameTimeLimit>(0, "gameTimeLimit");
			m_gameTimeLimit->SetTimeLimit(180);
			m_gameTimeLimit->Deactivate();

			m_gameStartDirection = NewGO<GameStartDirection>(2, "gameStartDirection");
			m_gameStartDirection->Deactivate();

			/*フェードインに切り替える。*/
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeIn);

			m_player = NewGO<nsActor::Player>(0, "player");
			m_player2 = NewGO<nsActor::Player>(0, "player2");

			m_sandbag = NewGO<nsActor::Sandbag>(0, "Sandbag");

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

			/*ゲームオーバー演出。*/
			/*現在は左を入力することで演出を流すようにしている。*/
			/*TODO:今後はキャラクター全員のHPが0になったら演出を流すようにする。*/
			//if (m_gameOverDirection == nullptr)
			//{
			//	if (g_pad[0]->IsTrigger(enButtonLeft))
			//	{
			//		m_gameOverDirection = NewGO<GameOverDirection>(2, "gameOverDirection");
			//		m_characterHP->Deactivate();
			//		m_gameTimeLimit->Deactivate();
			//	}
			//}
			//else
			//{
			//	if (m_gameEndSelect == nullptr)
			//	{
			//		if (m_gameOverDirection->IsDirectionFinished())
			//		{
			//			m_gameOverDirection->Deactivate();
			//			m_gameEndSelect = NewGO<GameEndSelect>(2, "gameEndSelect");
			//		}
			//	}
			//}

			/* 現在のステージの更新を行う。*/
			nsApp::nsStage::LoadStageData::GetInstance().Update();
		}


		void Game::Render(RenderContext& rc)
		{
			/* 現在のステージを描画する。*/
			nsApp::nsStage::LoadStageData::GetInstance().Draw(rc);
		}
	}

}