#include "stdafx.h"
#include "Game.h"
#include "Src/Fade/Fade.h"

#include "Src/Actor/Stage/LoadStageData.h"
#include "Camera.h"
#include "CharacterHP.h"
#include "GameTimeLimit.h"
#include "GameStartDirection.h"
#include "GameClearDirection.h"
#include "GameTimeUpDirection.h"
#include "GameOverDirection.h"
#include "GameEndSelect.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsGame
	{
		Game::~Game()
		{
			DeleteGO(m_camera);
		}


		bool Game::Start()
		{
			/* 初期ステージのセット。*/
			/* @TODO ステージ選択画面からこの処理を呼ぶようにする。*/
			nsApp::nsStage::LoadStageData::GetInstance().ChangeStage(nsApp::nsStage::StageID::stage1);

			/* カメラを生成。*/
			m_camera = NewGO<Camera>(0, "camera");

			PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

			NewGO<nsActor::Player>(0, "player");

			m_characterHP = NewGO<CharacterHP>(0, "characterHP");
			m_characterHP->Deactivate();

			m_gameTimeLimit = NewGO<GameTimeLimit>(0, "gameTimeLimit");
			m_gameTimeLimit->SetTimeLimit(180);
			m_gameTimeLimit->Deactivate();

			m_gameStartDirection = NewGO<GameStartDirection>(2, "gameStartDirection");
			m_gameStartDirection->Deactivate();

			/*フェードインに切り替える。*/
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeIn);
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
			//if (m_gameClearDirection == nullptr)
			//{
			//	if (g_pad[0]->IsTrigger(enButtonLeft))
			//	{
			//		m_gameClearDirection = NewGO<GameClearDirection>(2, "gameClearDirection");
			//	}
			//}

			/*時間切れ演出。*/
			//if (m_gameTimeUpDirection == nullptr)
			//{
			//	if (m_gameTimeLimit->IsTimeUp())
			//	{
			//		m_gameTimeUpDirection = NewGO<GameTimeUpDirection>(2, "gameTimeUpDirection");
			//	}
			//}
			//else
			//{
			//	if (m_gameEndSelect == nullptr)
			//	{
			//		if (m_gameTimeUpDirection->IsDirectionFinished())
			//		{
			//			m_gameTimeUpDirection->Deactivate();
			//			m_gameEndSelect = NewGO<GameEndSelect>(2, "gameEndSelect");
			//		}
			//	}
			//}

			///*ゲームオーバー演出。*/
			//if (m_gameOverDirection == nullptr)
			//{
			//	if (g_pad[0]->IsTrigger(enButtonLeft))
			//	{
			//		m_gameOverDirection = NewGO<GameOverDirection>(2, "gameOverDirection");
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