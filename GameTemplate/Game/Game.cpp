#include "stdafx.h"
#include "Game.h"

#include "Src/Actor/Stage/LoadStageData.h"
#include "Src/Camera/Camera.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Stage/BackGround.h"
#include "Src/Debug/Sandbag.h"

namespace nsApp
{
	namespace nsGame
	{
		Game::~Game()
		{
			DeleteGO(m_camera);
			DeleteGO(m_backGround);
			DeleteGO(m_player);
			DeleteGO(m_sandbag);
		}


		bool Game::Start()
		{
			/* 初期ステージのセット。*/
			/* @TODO ステージ選択画面からこの処理を呼ぶようにする。*/
			nsApp::nsStage::LoadStageData::GetInstance().ChangeStage(nsApp::nsStage::StageID::stage1);

			/* カメラを生成。*/
			m_camera = NewGO<Camera>(0, "camera");

			PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

		//	m_backGround = NewGO<nsStage::BackGround>(0, "backGround");
			m_player = NewGO<nsActor::Player>(0, "player");
			m_sandbag = NewGO<nsActor::Sandbag>(0, "Sandbag");
			return true;
		}


		void Game::Update()
		{
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