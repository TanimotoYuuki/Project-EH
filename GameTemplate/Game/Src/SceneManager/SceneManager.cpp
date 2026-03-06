#include "stdafx.h"
#include "SceneManager.h"
#include "Title.h"
#include "Game.h"

namespace nsApp
{
	namespace nsTitle
	{
		//タイトルシーン。
		TitleScene::~TitleScene()
		{
			DeleteGO(m_title);
		}

		bool TitleScene::Start()
		{
			m_title = NewGO<Title>(0, "title");
			return true;
		}

		void TitleScene::Update()
		{

		}
	}

	namespace nsStageSelect
	{
		//ステージ選択シーン。
		StageSelectScene::~StageSelectScene()
		{

		}

		bool StageSelectScene::Start()
		{
			return true;
		}

		void StageSelectScene::Update()
		{

		}
	}

	namespace nsGame
	{
		//インゲームシーン。
		InGameScene::~InGameScene()
		{
			DeleteGO(m_game);
		}

		bool InGameScene::Start()
		{
			m_game = NewGO<Game>(0, "game");
			return true;
		}

		void InGameScene::Update()
		{

		}
	}

	namespace nsResult
	{
		//リザルトシーン。
		ResultScene::~ResultScene()
		{

		}

		bool ResultScene::Start()
		{
			return true;
		}

		void ResultScene::Update()
		{

		}
	}

	namespace nsScene
	{
		SceneManager* SceneManager::m_instance = nullptr;//シングルトンインスタンスの初期化。

		//開始処理。
		bool SceneManager::Start()
		{
			m_instance->GetInstance()->ChangeScene(EnSceneID::enSceneID_Title);//シーンの切り替え(デフォルトはタイトルシーン)。
			return true;
		}

		//更新処理
		void SceneManager::Update()
		{
			if (m_changeSceneID == enSceneID_None)return;//切り替えるシーンがなければ処理しない。
			if (m_changeSceneID == m_currentSceneID)return;//切り替えるシーンが現在のシーンと同じなら処理しない。

			//シーン用のインスタンスに現在進行中のシーンがあれば破棄する。
			if (m_scene)
			{
				DeleteGO(m_scene);
				m_scene = nullptr;
			}

			//新しくシーンを生成する処理。
			switch (m_changeSceneID)
			{
			case IScene::enSceneID_Title://タイトルシーン。
				m_currentSceneID = enSceneID_Title;
				m_scene = NewGO<nsTitle::TitleScene>(0, "titleScene");
				break;
			case IScene::enSceneID_StageSelect://ステージ選択シーン。
				m_currentSceneID = enSceneID_StageSelect;
				m_scene = NewGO<nsStageSelect::StageSelectScene>(0, "stageSelectScene");
				break;
			case IScene::enSceneID_InGame://インゲームシーン。
				m_currentSceneID = enSceneID_InGame;
				m_scene = NewGO<nsGame::InGameScene>(0, "inGameScene");
				break;
			case IScene::enSceneID_Result://リザルトシーン。
				m_currentSceneID = enSceneID_Result;
				m_scene = NewGO<nsResult::ResultScene>(0, "resultScene");
				break;
			default:
				break;
			}
		}
	}
}