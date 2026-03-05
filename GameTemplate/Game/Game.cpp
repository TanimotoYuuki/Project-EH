#include "stdafx.h"
#include "Game.h"

#include "Src/Actor/Stage/LoadStageData.h"

namespace nsApp
{
	namespace nsGame
	{
		bool Game::Start()
		{
			/* 初期ステージのセット。*/
			/* @TODO ステージ選択画面からこの処理を呼ぶようにする。*/
			nsApp::nsStage::LoadStageData::GetInstance().ChangeStage(nsApp::nsStage::StageID::stage1);

			/* @TODO カメラクラスを用意する。*/
			g_camera3D->SetPosition(Vector3(0.0f, 75.0f, 125.0f));
			g_camera3D->SetTarget(Vector3::Zero);

			PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
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