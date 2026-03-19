#include "stdafx.h"
#include "Game.h"

#include "Src/Actor/Stage/LoadStageData.h"
#include "Src/Camera/Camera.h"
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