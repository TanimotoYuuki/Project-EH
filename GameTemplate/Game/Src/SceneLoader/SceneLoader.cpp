#include "stdafx.h"
#include "SceneLoader.h"
#include "Title.h"
#include "Game.h"

namespace nsApp
{
	namespace nsTitle
	{
		/*タイトルシーン。*/
		/*デストラクタ。*/
		TitleScene::~TitleScene()
		{
			DeleteGO(m_title);
		}

		/*開始処理。*/
		bool TitleScene::Start()
		{
			m_title = NewGO<Title>(0, "title");
			return true;
		}
		
		/*更新処理。*/
		void TitleScene::Update()
		{

		}
	}

	namespace nsSelect
	{
		/*クエスト選択シーン。*/
		/*デストラクタ。*/
		SelectScene::~SelectScene()
		{

		}

		/*開始処理。*/
		bool SelectScene::Start()
		{

			return true;
		}

		/*更新処理。*/
		void SelectScene::Update()
		{

		}
	}

	namespace nsGame
	{
		/*インゲームシーン。*/
		/*デストラクタ。*/
		InGameScene::~InGameScene()
		{
			DeleteGO(m_game);
		}

		/*開始処理。*/
		bool InGameScene::Start()
		{
			m_game = NewGO<Game>(0, "game");
			return true;
		}
		
		/*更新処理。*/
		void InGameScene::Update()
		{

		}
	}

	namespace nsResult
	{
		/*リザルトシーン。*/
		/*デストラクタ、*/
		ResultScene::~ResultScene()
		{

		}

		/*開始処理。*/
		bool ResultScene::Start()
		{
			return true;
		}

		/*更新処理。*/
		void ResultScene::Update()
		{

		}
	}

	namespace nsScene
	{
		SceneLoader* SceneLoader::m_instance = nullptr;/*シングルトンインスタンスの初期化。*/

		/*開始処理。*/
		bool SceneLoader::Start()
		{
			m_instance->GetInstance()->ChangeScene(IScene::EnSceneID::enSceneID_Title);/*シーンの切り替え(デフォルトはタイトルシーン)。*/
			return true;
		}

		/*更新処理*/
		void SceneLoader::Update()
		{
			if (m_changeSceneID == IScene::enSceneID_None)return;/*切り替えるシーンがなければ処理しない。*/
			if (m_changeSceneID == m_currentSceneID)return;/*切り替えるシーンが現在のシーンと同じなら処理しない。*/

			/*シーン用のインスタンスに現在進行中のシーンがあれば破棄する。*/
			if (m_currentScene)
			{
				DeleteGO(m_currentScene);
				m_currentScene = nullptr;
			}

			/*新しくシーンを生成する処理。*/
			switch (m_changeSceneID)
			{
			case IScene::enSceneID_Title:/*タイトルシーン。*/
				m_currentSceneID = IScene::enSceneID_Title;
				m_currentScene = NewGO<nsTitle::TitleScene>(0, "titleScene");
				break;
			case IScene::enSceneID_Select:/*クエスト選択シーン。*/
				m_currentSceneID = IScene::enSceneID_Select;
				m_currentScene = NewGO<nsSelect::SelectScene>(0, "selectScene");
				break;
			case IScene::enSceneID_InGame:/*インゲームシーン。*/
				m_currentSceneID = IScene::enSceneID_InGame;
				m_currentScene = NewGO<nsGame::InGameScene>(0, "inGameScene");
				break;
			case IScene::enSceneID_Result:/*リザルトシーン。*/
				m_currentSceneID = IScene::enSceneID_Result;
				m_currentScene = NewGO<nsResult::ResultScene>(0, "resultScene");
				break;
			default:
				break;
			}
		}
	}
}