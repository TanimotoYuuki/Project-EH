#include "stdafx.h"
#include "SceneLoader.h"
#include "Src/Fade/Fade.h"
#include "Title.h"
#include "Select.h"
#include "QuestSelect.h"
#include "MemberSelect.h"
#include "ConfirmationSelect.h"
#include "Game.h"
#include "Game2.h"
#include "GameClearDirection.h"
#include "GameEndSelect.h"
#include "Result.h"

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
			/*選択が終わったら。*/
			if (m_title->DidSelect())
			{
				/*フェード処理が終わったら選択シーンに遷移する。*/
				if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
				{
					nsApp::nsScene::SceneLoader::GetInstance()->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_Select);
				}
			}
		}
	}

	namespace nsSelect
	{
		/*選択シーン。*/
		/*デストラクタ。*/
		SelectScene::~SelectScene()
		{
			DeleteGO(m_select);
		}

		/*開始処理。*/
		bool SelectScene::Start()
		{
			m_select = NewGO<Select>(0, "select");
			return true;
		}

		/*更新処理。*/
		void SelectScene::Update()
		{
			if (m_questSelect == nullptr)
			{
				m_questSelect = m_select->GetQuestSelectInstance();
				return;
			}

			if (m_memberSelect == nullptr)
			{
				m_memberSelect = m_select->GetMemberSelectInstance();
				return;
			}

			if (m_confirmationSelect == nullptr)
			{
				m_confirmationSelect = m_select->GetConfirmationSelectInstance();
				return;
			}

			if (m_confirmationSelect->GetCurrentSelect() == nsApp::nsSelect::ConfirmationSelect::EnConfirmationSelectTextUI::enConfirmationSelectTextUI_Yes &&
				m_confirmationSelect->DidSelect())
			{
				m_confirmationSelect->DisableDrawingUI();
				/*フェード処理が終わったらインゲームシーンに遷移する。*/
				if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
				{
					nsApp::nsScene::SceneLoader::GetInstance()->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_InGame);
				}
				return;
			}

			if (m_questSelect->DidSelect())
			{
				m_memberSelect->Activate();

				if (m_memberSelect->DidSelect())
				{
					if (m_confirmationSelect->GetCurrentSelect() == nsApp::nsSelect::ConfirmationSelect::EnConfirmationSelectTextUI::enConfirmationSelectTextUI_No &&
						m_confirmationSelect->DidSelect())
					{
						m_memberSelect->DisableSelect();
						m_confirmationSelect->DisableSelect();
						return;
					}

					m_confirmationSelect->Activate();
					m_confirmationSelect->EnableDrawingUI();
					return;
				}

				/*前の選択に戻るか？*/
				if (m_memberSelect->IsBackSelect())
				{
					m_questSelect->Activate();
					m_questSelect->EnableDirection();
					m_questSelect->DisableSelect();
					return;
				}

				m_confirmationSelect->Deactivate();
				m_confirmationSelect->DisableDrawingUI();

				if (m_questSelect->IsDirection())
				{
					if (m_memberSelect->IsEndSlideUIAnimation(
						nsApp::nsSelect::MemberSelect::EnSlide::enSlide_Left,
						nsApp::nsSelect::MemberSelect::EnSlideUIAnimationSprite::enSlideUIAnimationSprite_FourCharacterFrameUI)
						)
					{
						m_questSelect->ChangeSlide(nsApp::nsSelect::QuestSelect::enSlide_Right);
						m_questSelect->DisableDirection();
						m_questSelect->ResetSlideUIAnimation(nsApp::nsSelect::QuestSelect::EnSlide::enSlide_Left);
						m_memberSelect->DisableDirection();
						m_memberSelect->ResetSlideUIAnimation(nsApp::nsSelect::MemberSelect::EnSlide::enSlide_Left);
						return;
					}

					m_questSelect->Activate();
					m_questSelect->EnableDirection();
					m_questSelect->ResetAlphaUIAnimation();
					m_memberSelect->EnableDirection();
				}
				else
				{
					m_questSelect->Deactivate();
					m_questSelect->DisableDirection();
					m_memberSelect->ChangeSlide(nsApp::nsSelect::MemberSelect::enSlide_Right);
				}
			}
			else if (!m_questSelect->DidSelect())
			{
				m_questSelect->Activate();

				m_confirmationSelect->Deactivate();
				m_confirmationSelect->DisableDrawingUI();

				if (m_memberSelect->IsDirection())
				{
					if (m_questSelect->IsEndSlideUIAnimation(
						nsApp::nsSelect::QuestSelect::EnSlide::enSlide_Right,
						nsApp::nsSelect::QuestSelect::EnSlideUIAnimationSprite::enSlideUIAnimationSprite_TargetTextUI)
						)
					{
						m_memberSelect->ChangeSlide(nsApp::nsSelect::MemberSelect::enSlide_Left);
						m_memberSelect->DisableDirection();
						m_memberSelect->ResetSlideUIAnimation(nsApp::nsSelect::MemberSelect::EnSlide::enSlide_Right);
						m_questSelect->DisableDirection();
						m_questSelect->ResetSlideUIAnimation(nsApp::nsSelect::QuestSelect::EnSlide::enSlide_Right);
						return;
					}

					m_memberSelect->Activate();
					m_memberSelect->EnableDirection();
					m_memberSelect->ResetAlphaUIAnimation();
					m_questSelect->EnableDirection();
				}
				else
				{
					m_memberSelect->Deactivate();
					m_memberSelect->DisableDirection();
					m_memberSelect->DisableBackSelect();
					m_memberSelect->ResetSelect();
					m_questSelect->ChangeSlide(nsApp::nsSelect::QuestSelect::enSlide_Left);
				}
			}
		}
	}

	namespace nsGame
	{
		/*インゲームシーン。*/
		/*デストラクタ。*/
		InGameScene::~InGameScene()
		{
			DeleteGO(m_game2);
		}

		/*開始処理。*/
		bool InGameScene::Start()
		{
			m_game2 = NewGO<Game2>(0, "game");
			return true;
		}
		
		/*更新処理。*/
		void InGameScene::Update()
		{
			if (m_game2->GetGameClearDirectionInstance() != nullptr)
			{
				if (m_gameClearDirection == nullptr)
				{
					m_gameClearDirection = m_game2->GetGameClearDirectionInstance();
				}

				if (m_gameClearDirection->IsDirectionFinished())
				{
					m_gameClearDirection->Deactivate();
					/*フェード処理が終わったらリザルトシーンに遷移する。*/
					if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
					{
						nsApp::nsScene::SceneLoader::GetInstance()->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_Result);
					}
				}
			}

			/*ゲームオーバー、もしくはタイムアップのどちらかの状態になったら。*/
			if (m_game2->GetGameTimeUpDirectionInstance() != nullptr ||
				m_game2->GetGameOverDirectionInstance() != nullptr)
			{
				if (m_gameEndSelect == nullptr)
				{
					m_gameEndSelect = m_game2->GetGameEndSelectInstance();
					return;
				}

				if (!m_gameEndSelect->DidSelect()) { return; }

				m_gameEndSelect->Deactivate();
				if (m_gameEndSelect->GetCurrentSelect() == nsApp::GameEndSelect::enSelect_Retry)
				{
					/*フェード処理が終わったらインゲームシーンに遷移する。*/
					if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
					{
						nsApp::nsScene::SceneLoader::GetInstance()->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_InGame);
					}
				}
				else if (m_gameEndSelect->GetCurrentSelect() == nsApp::GameEndSelect::enSelect_QuestSelect)
				{
					/*フェード処理が終わったら選択シーンに遷移する。*/
					if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
					{
						nsApp::nsScene::SceneLoader::GetInstance()->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_Select);
					}
				}
			}
		}
	}

	namespace nsResult
	{
		/*リザルトシーン。*/
		/*デストラクタ、*/
		ResultScene::~ResultScene()
		{
			DeleteGO(m_result);
		}

		/*開始処理。*/
		bool ResultScene::Start()
		{
			m_result = NewGO<Result>(0, "result");
			return true;
		}

		/*更新処理。*/
		void ResultScene::Update()
		{
			if (!m_result->DidSelect()) { return; }

			if (m_gameEndSelect == nullptr)
			{
				m_gameEndSelect = m_result->GetGameEndSelectInstance();
				return;
			}

			if (!m_gameEndSelect->DidSelect()) { return; }

			m_gameEndSelect->Deactivate();
			if (m_gameEndSelect->GetCurrentSelect() == nsApp::GameEndSelect::enSelect_Retry)
			{
				/*フェード処理が終わったらインゲームシーンに遷移する。*/
				if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
				{
					nsApp::nsScene::SceneLoader::GetInstance()->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_InGame);
				}
			}
			else if (m_gameEndSelect->GetCurrentSelect() == nsApp::GameEndSelect::enSelect_QuestSelect)
			{
				/*フェード処理が終わったら選択シーンに遷移する。*/
				if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
				{
					nsApp::nsScene::SceneLoader::GetInstance()->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_Select);
				}
			}
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
			m_changeSceneID = IScene::enSceneID_None;
		}
	}
}