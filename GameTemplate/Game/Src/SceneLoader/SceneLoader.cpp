#include "stdafx.h"
#include "SceneLoader.h"
#include "system/system.h"
#include "Src/Fade/Fade.h"
#include "Src/Scene/TitleScene/Title.h"
#include "Src/Scene/TitleScene/TitleSelect.h"
#include "Src/Scene/TitleScene/TitleBackGround.h"
#include "Src/Scene/SelectScene/Select.h"
#include "Src/Scene/SelectScene/QuestSelect.h"
#include "Src/Scene/SelectScene/RoleSelect.h"
#include "Src/Scene/SelectScene/MemberSelect.h"
#include "Src/Select/ConfirmationSelect.h"
#include "Src/UI/GameFlow/Option.h"
#include "Src/UI/GameFlow/HowToPlay.h"
#include "Game2.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Direction/GameClearDirection.h"
#include "Src/Select/GameEndSelect.h"
#include "Src/Scene/InGame/Pause.h"
#include "Src/Scene/ResultScene/Result.h"
#include "Src/Camera/Camera.h"

namespace nsApp
{
	namespace nsTitle
	{
		TitleScene::~TitleScene()
		{
			DeleteGO(m_titleBackGround);
			DeleteGO(m_title);
			DeleteGO(m_titleSelect);
			DeleteGO(m_option);
		}


		bool TitleScene::Start()
		{
			m_titleBackGround = NewGO<TitleBackGround>(0, "titleBackGround");
			m_title = NewGO<Title>(0, "title");
			m_titleSelect = NewGO<TitleSelect>(0, "titleSelect");
			m_option = NewGO<nsOption::Option>(0, "option");

			for (int i = 0; i < nsOption::Option::EnGaugeUI::enGaugeUI_Num; i++)
				m_option->SetVolumeRate((nsOption::Option::EnGaugeUI)i, GetVolumeRate(i));


			return true;
		}


		void TitleScene::Update()
		{
			/*選択が終わったら。*/
			if (m_title->DidSelect())
			{
				m_titleSelect->Activate();

				if (m_titleSelect->DidSelect())
				{
					/*選択画面でゲームスタートを選択していたら。*/
					if (m_titleSelect->GetCurrentSelect() == nsApp::nsTitle::TitleSelect::EnSelect::enSelect_Start)
					{
						/* BGNの停止処理。*/
						if (m_title->GetBGMInstance() != nullptr)
							m_title->StopBGM();

						for (int i = 0; i < nsApp::nsOption::Option::EnGaugeUI::enGaugeUI_Num; i++)
							SetVolumeRate(i, m_option->GetVolumeRate((nsApp::nsOption::Option::EnGaugeUI)i));

						/*フェード処理が終わったら選択シーンに遷移する。*/
						if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
						{
							auto* loader = nsApp::nsScene::SceneLoader::GetInstance();
							loader->SetLoadingDestination(nsScene::toSelect);
							loader->ChangeScene(IScene::enSceneID_Loading);
						}
					}
					/*選択画面で設定を選択していたら。*/
					else if (m_titleSelect->GetCurrentSelect() == nsApp::nsTitle::TitleSelect::EnSelect::enSelect_Setting)
					{
						/*選択画面で選択できている状態。*/
						if (m_option->DidSelect())
						{
							m_titleSelect->DisableSelect();
							m_option->DisableSelect();
							m_option->DisableDirection();
							return;
						}

						m_option->Activate();
						m_option->EnableDrawingUI();
					}
					/*選択画面でゲーム終了を選択していたら。*/
					else if (m_titleSelect->GetCurrentSelect() == nsApp::nsTitle::TitleSelect::EnSelect::enSelect_ExitGame)
					{
						/*ゲームを終了。*/
						g_gameLoop.m_isLoop = false;
					}
					return;
				}

				if (m_title->IsSlideUpDirection())
				{
					if (m_title->IsEndSlideUpDirection())
					{
						m_titleSelect->DisableSlideUpDirection();
						m_title->DisableSlideUpDirection();
					}
					else
					{
						m_titleSelect->EnableSlideUpDirection();
					}
					return;
				}
				else
				{
					m_titleSelect->ResetSlideUpUIAnimation();
					m_title->ResetSlideUpUIAnimation();

					m_option->Deactivate();
					m_option->DisableDrawingUI();
					m_option->ResetSelect();
				}

				if (m_titleSelect->IsBackScene())
				{
					m_title->Activate();
					m_title->EnableSlideDownDirection();
					m_title->DisableSelect();
					m_title->ResetPressAButtonMulColor();
					return;
				}

				m_title->Deactivate();
			}
			else
			{
				m_title->Activate();

				if (m_titleSelect->IsSlideDownDirection())
				{
					if (m_titleSelect->IsEndSlideDownDirection())
					{
						m_title->DisableSlideDownDirection();
						m_titleSelect->DisableSlideDownDirection();
					}
					else
					{
						m_title->EnableSlideDownDirection();
					}
					return;
				}
				else
				{
					m_title->ResetSlideDownUIAnimation();
					m_titleSelect->ResetSlideDownUIAnimation();
					m_titleSelect->DisableBackScene();

					m_option->Deactivate();
					m_option->DisableDrawingUI();
				}

				m_titleSelect->Deactivate();
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
			DeleteGO(m_howToPlay);
			DeleteGO(m_option);
		}

		/*開始処理。*/
		bool SelectScene::Start()
		{
			m_select = NewGO<Select>(0, "select");
			m_howToPlay = NewGO<nsApp::nsHowToPlay::HowToPlay>(0, "howToPlay");
			m_option = NewGO<nsApp::nsOption::Option>(1, "option");

			for (int i = 0; i < nsApp::nsOption::Option::EnGaugeUI::enGaugeUI_Num; i++)
				m_option->SetVolumeRate((nsApp::nsOption::Option::EnGaugeUI)i, GetVolumeRate(i));

			/* 操作方法・設定は、開くまで非表示にする。*/
			m_howToPlay->Deactivate();
			m_option->Deactivate();

			if (g_camera3D != nullptr)
			{
				g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
				g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });
			}

			auto* camera = FindGO<Camera>("camera");
			if (camera != nullptr)
				DeleteGO(camera);

			return true;
		}


		/*更新処理。*/
		void SelectScene::Update()
		{
			/*インスタンスがnullptrの場合は取得する。*/
			if (m_questSelect == nullptr)/*クエスト選択。*/
			{
				m_questSelect = m_select->GetQuestSelectInstance();
				return;
			}

			if (m_memberSelect == nullptr)/*メンバー選択。*/
			{
				m_memberSelect = m_select->GetMemberSelectInstance();
				return;
			}

			if (m_roleSelect == nullptr)/*役割選択。*/
			{
				m_roleSelect = m_select->GetRoleSelectInstance();
				return;
			}

			if (m_confirmationSelect == nullptr)/*確認選択。*/
			{
				m_confirmationSelect = m_select->GetConfirmationSelectInstance();
				return;
			}

			/*確認選択画面でYesを選択できている状態。*/
			if (m_confirmationSelect->GetCurrentSelect() == nsApp::nsSelect::ConfirmationSelect::EnConfirmationSelectTextUI::enConfirmationSelectTextUI_Yes &&
				m_confirmationSelect->DidSelect())
			{
				m_memberSelect->DisableDirection();
				m_confirmationSelect->DisableDrawingUI();

				/*メンバー選択画面が表示していたら。*/
				if (m_memberSelect->IsActive())
				{
					SetBossType(m_questSelect->GetCurrentSelect());
					for (int i = 0; i < nsApp::nsSelect::MemberSelect::EnCharacterFrameUI::enCharacterFrameUI_Num; i++)
					{
						SetPlayerControle(i, m_memberSelect->IsPlayerControle(i));
						SetCharacterRole(i, m_memberSelect->GetCurrentRole((nsApp::nsSelect::MemberSelect::EnCharacterFrameUI)i));
					}
					for (int j = 0; j < nsApp::nsOption::Option::EnGaugeUI::enGaugeUI_Num; j++)
						SetVolumeRate(j, m_option->GetVolumeRate((nsApp::nsOption::Option::EnGaugeUI)j));


					auto* loader = nsScene::SceneLoader::GetInstance();
					loader->SetLoadingDestination(nsScene::toInGame);
					loader->ChangeScene(IScene::enSceneID_Loading);
					return;
				}

				/*クエスト選択画面が表示されていたら。*/
				else
				{
					/*ゲームを終了。*/
					g_gameLoop.m_isLoop = false;
				}
				return;
			}

			/*クエスト選択で選択できていたらメンバー選択画面が表示されている。*/
			if (m_questSelect->DidSelect())
			{
				if (m_questSelect->IsEndSelectEndDirectionUIAnimation(m_questSelect->GetCurrentSelect()))
				{
					m_memberSelect->Activate();
					m_memberSelect->EnableDrawingButtonAndTextUI();
				}

				/*ゲーム設定ボタン(Selectボタン)を押したら設定画面を表示する。*/
				if (m_memberSelect->IsPushGameOptionButton())
				{
					/*選択画面で選択できている状態。*/
					if (m_option->DidSelect())
					{
						m_memberSelect->DisablePushGameOptionButton();
						m_option->DisableSelect();
						m_option->DisableDirection();
						return;
					}

					m_option->Activate();
					m_option->EnableDrawingUI();
					return;
				}
				/*操作方法ボタン(Yボタン)を押したら操作方法画面を表示する。*/
				else if (m_memberSelect->IsPushHowToPlayButton())
				{
					/*前の画面に戻る状態。*/
					if (m_howToPlay->IsBackSelect())
					{
						m_howToPlay->DisableBackSelect();
						m_memberSelect->Activate();
						m_memberSelect->DisablePushHowToPlayButton();
						return;
					}

					m_howToPlay->Activate();
					m_memberSelect->Deactivate();
					m_memberSelect->AllCharacterModelDeactivate();
					return;
				}

				/*選択できたら確認画面か役割選択画面を表示する。*/
				if (m_memberSelect->DidSelect())
				{
					/*メンバー選択画面で出撃を選択していたら確認画面を表示する。*/
					if (m_memberSelect->GetCurrentSelect() == MemberSelect::EnSelect::enSelect_Deploy)
					{
						/*確認選択画面でNoを選択できている状態。*/
						if (m_confirmationSelect->GetCurrentSelect() == nsApp::nsSelect::ConfirmationSelect::EnConfirmationSelectTextUI::enConfirmationSelectTextUI_No &&
							m_confirmationSelect->DidSelect())
						{
							m_memberSelect->DisableSelect();
							m_memberSelect->DisableDirection();
							m_memberSelect->ResetSelectStartDirectionUIAnimation();
							m_memberSelect->ResetSelectEndDirectionUIAnimation();
							m_memberSelect->EnableDrawingButtonAndTextUI();
							m_confirmationSelect->DisableSelect();
							m_confirmationSelect->DisableDirection();
							return;
						}

						/*選択したときの演出UIアニメーションが終わっていたら確認選択画面を表示する。*/
						if (m_memberSelect->IsEndSelectEndDirectionUIAnimation(
							nsApp::nsSelect::MemberSelect::EnSelectDirectionUIAnimationSprite::enSelectDirectionUIAnimationSprite_DeployTextUI
						)
							)
						{
							m_memberSelect->DisableDrawingButtonAndTextUI();
							m_confirmationSelect->Activate();
							m_confirmationSelect->EnableDrawingUI();
							m_confirmationSelect->ChangeConfirmationUI(nsApp::nsSelect::ConfirmationSelect::EnConfirmationUI::enConfirmationUI_GameStart);
						}
					}
					/*メンバー確認画面で出撃以外を選択していたら役割選択画面を表示する。*/
					else
					{
						/*役割選択画面で選択または戻るができている状態。*/
						if (m_roleSelect->DidSelect() || m_roleSelect->IsBackSelect())
						{
							if (m_roleSelect->DidSelect())
							{
								int characterFrameUI = m_memberSelect->GetCurrentSelect();
								m_memberSelect->ChangeRoleIconUI((MemberSelect::EnCharacterFrameUI)characterFrameUI, m_roleSelect->GetCurrentRole());
								m_roleSelect->DisableSelectDirection();
							}

							m_memberSelect->DisableSelect();
							m_memberSelect->DisableDirection();
							m_memberSelect->ResetSelectStartDirectionUIAnimation();
							m_memberSelect->ResetSelectEndDirectionUIAnimation();
							m_memberSelect->EnableDrawingButtonAndTextUI();
							m_roleSelect->DiableSelect();
							m_roleSelect->DisableBackSelect();
						}

						if (m_memberSelect->IsPlayerControle(m_memberSelect->GetCurrentSelect()))
						{
							/*選択したときの演出UIアニメーションが終わっていないときは役割選択画面を表示しない。*/
							if (!m_memberSelect->IsEndSelectEndDirectionUIAnimation(m_memberSelect->GetCurrentSelect() + 1))
								return;
						}
						else
						{
							/*選択したときの演出UIアニメーションが終わっていないときは役割選択画面を表示しない。*/
							if (!m_memberSelect->IsEndSelectEndDirectionUIAnimation(m_memberSelect->GetCurrentSelect() + 4))
								return;
						}

						m_roleSelect->Activate();
						m_roleSelect->EnableDrawingUI();
						m_roleSelect->SetReferencePosition(m_memberSelect->GetCurrentSelectCharacterFrameUIPosition());
					}
					return;
				}

				/*戻るを選択したらクエスト選択画面の演出を流す。*/
				if (m_memberSelect->IsBackSelect())
				{
					m_questSelect->Activate();
					m_questSelect->EnableDirection();
					m_questSelect->DisableSelect();
					return;
				}

				m_roleSelect->Deactivate();
				m_roleSelect->DisableDrawingUI();

				/*選択しているキャラクター枠UIにある役割UIに応じた役割選択画面のUIを表示するための設定。*/
				int currentSelectCharacterFrame = m_memberSelect->GetCurrentSelect();
				if (currentSelectCharacterFrame != nsApp::nsSelect::MemberSelect::EnSelect::enSelect_Deploy)
					m_roleSelect->ChangeDisplayRoleUI(m_memberSelect->GetCurrentRole((nsApp::nsSelect::MemberSelect::EnCharacterFrameUI)currentSelectCharacterFrame));

				m_confirmationSelect->Deactivate();
				m_confirmationSelect->DisableDrawingUI();

				m_option->Deactivate();
				m_option->DisableDrawingUI();
				m_option->ResetSelect();

				m_howToPlay->Deactivate();

				/*クエスト選択画面の演出中のとき。*/
				if (m_questSelect->IsDirection())
				{
					/*UIをスライドさせるアニメーションが終了していたら。*/
					if (m_memberSelect->IsEndSlideLeftUIAnimation())
					{
						m_questSelect->ChangeSlide(nsApp::nsSelect::QuestSelect::enSlide_Right);
						m_questSelect->DisableDirection();
						m_questSelect->ResetSlideLeftUIAnimation();
						m_questSelect->ResetSelectStartDirectionUIAnimation();
						m_questSelect->ResetSelectEndDirectionUIAnimation();
						m_memberSelect->DisableDirection();
						m_memberSelect->ResetSlideLeftUIAnimation();
						return;
					}

					m_questSelect->Activate();
					m_questSelect->EnableDrawingButtonAndTextUI();
					m_questSelect->EnableDirection();
					m_questSelect->ResetAlphaUIAnimation();

					/*クエスト選択画面で選択したときの演出UIアニメーションが終わっていたらメンバー選択画面での演出を流す。*/
					if (m_questSelect->IsEndSelectEndDirectionUIAnimation(m_questSelect->GetCurrentSelect()))
					{
						m_questSelect->DisableDrawingButtonAndTextUI();
						m_memberSelect->EnableDirection();
					}
				}
				/*クエスト選択画面の演出中ではないとき。*/
				else
				{
					m_questSelect->Deactivate();
					m_questSelect->DisableDirection();
					m_memberSelect->ChangeSlide(nsApp::nsSelect::MemberSelect::enSlide_Right);
				}
			}
			/*クエスト選択で選択できていなければクエスト選択画面が表示されている。*/
			else if (!m_questSelect->DidSelect())
			{
				m_questSelect->Activate();
				m_questSelect->EnableDrawingButtonAndTextUI();

				/*ゲーム終了ボタン(Bボタン)を押していたら確認画面を表示する。*/
				if (m_questSelect->IsPushGameEndButton())
				{
					/*確認選択画面でNOを選択できている状態。*/
					if (m_confirmationSelect->GetCurrentSelect() == nsApp::nsSelect::ConfirmationSelect::EnConfirmationSelectTextUI::enConfirmationSelectTextUI_No &&
						m_confirmationSelect->DidSelect())
					{
						m_questSelect->DisablePushGameEndButton();
						m_confirmationSelect->DisableSelect();
						m_confirmationSelect->DisableDirection();
						return;
					}

					m_questSelect->DisableDrawingButtonAndTextUI();
					m_confirmationSelect->Activate();
					m_confirmationSelect->EnableDrawingUI();
					m_confirmationSelect->ChangeConfirmationUI(nsApp::nsSelect::ConfirmationSelect::EnConfirmationUI::enConfirmationUI_GameEnd);
					return;
				}

				m_roleSelect->Deactivate();
				m_roleSelect->DisableDrawingUI();

				m_confirmationSelect->Deactivate();
				m_confirmationSelect->DisableDrawingUI();

				m_option->Deactivate();
				m_option->DisableDrawingUI();

				m_howToPlay->Deactivate();

				/*メンバー選択画面の演出中のとき。*/
				if (m_memberSelect->IsDirection())
				{
					/*UIをスライドさせるアニメーションが終了していたら。*/
					if (m_questSelect->IsEndSlideRightUIAnimation())
					{
						m_memberSelect->ChangeSlide(nsApp::nsSelect::MemberSelect::enSlide_Left);
						m_memberSelect->DisableDirection();
						m_memberSelect->ResetSlideRightUIAnimation();
						m_questSelect->DisableDirection();
						m_questSelect->ResetSlideRightUIAnimation();
						return;
					}

					m_memberSelect->Activate();
					m_memberSelect->DisableDrawingButtonAndTextUI();
					m_memberSelect->EnableDirection();
					m_memberSelect->ResetAlphaUIAnimation();
					m_questSelect->EnableDirection();
				}
				/*メンバー選択画面の演出中ではないとき。*/
				else
				{
					m_memberSelect->Deactivate();
					m_memberSelect->DisableDirection();
					m_memberSelect->DisableBackSelect();
					m_memberSelect->ResetSelect();
					m_memberSelect->AllCharacterModelDeactivate();
					m_questSelect->ChangeSlide(nsApp::nsSelect::QuestSelect::enSlide_Left);
				}
			}

			/* 選択シーン中（ボス選択・メンバー選択）select.wav */
			auto* sound = FindGO<nsSound::SoundLister>("SoundManager");
			if (sound == nullptr)
			{
				sound = NewGO<nsSound::SoundLister>(0, "SoundManager");
				sound->InitSound();
			}
			if (!m_confirmationSelect->IsActive()
				&& !m_howToPlay->IsActive()
				&& !m_option->IsActive())
			{
				sound->GetBGMList().PlayBGM(nsSound::BGM_ID::Select, 1.0f);
			}
			else
			{
				sound->GetBGMList().StopBGM();
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
			m_game2 = FindGO<Game2>("game");

			if (m_game2 == nullptr)
			{
				OutputDebugStringA("[InGameScene] Game2 not found.\n");
				return false;
			}

			m_game2->ActivateGame();

			m_howToPlay = NewGO<nsApp::nsHowToPlay::HowToPlay>(0, "howToPlay");
			m_howToPlay->Deactivate();

			return true;
		}


		/*更新処理。*/
		void InGameScene::Update()
		{
			auto soundListers = FindGOs<nsSound::SoundLister>("SoundManager");
			for (const auto& soundLister : soundListers)
			{
				soundLister->SetBGMVolumeRate(GetVolumeRate(0));
				soundLister->SetSEVolumeRate(GetVolumeRate(1));
				soundLister->SetMasterVolumeRate(GetVolumeRate(2));
			}

			/*ポーズ用のインスタンスがnullptrの場合は取得する。*/
			if (m_pause == nullptr)
				m_pause = m_game2->GetPauseInstance();

			else
			{
				/*ポーズ画面が表示している状態。*/
				if (m_pause->IsActive())
				{
					/*確認選択用のインスタンスがnullptrの場合は取得する。*/
					if (m_confirmationSelect == nullptr)
					{
						m_confirmationSelect = m_pause->GetConfirmationSelectInstance();
						return;
					}

					/*確認選択画面でYesを選択できている状態。*/
					if (m_confirmationSelect->GetCurrentSelect() == nsApp::nsSelect::ConfirmationSelect::EnConfirmationSelectTextUI::enConfirmationSelectTextUI_Yes &&
						m_confirmationSelect->DidSelect())
					{
						m_confirmationSelect->DisableDrawingUI();

						/*ポーズ画面でゲームをやり直すを選択していたら。*/
						if (m_pause->GetCurrentSelect() == nsGame::Pause::EnSelect::enSelect_RestartTheGame)
						{
							/*フェード処理が終わったらローディングシーンに遷移する。*/
							if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
							{
								auto* loader = nsApp::nsScene::SceneLoader::GetInstance();
								loader->SetLoadingDestination(nsScene::toInGame);
								loader->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_Loading);
							}
						}
						/*ポーズ画面でクエスト選択に戻るを選択していたら。*/
						else if (m_pause->GetCurrentSelect() == nsGame::Pause::EnSelect::enSelect_ReturnToQuestSelect)
						{
							/*フェード処理が終わったら選択シーンに遷移する。*/
							if (nsFade::Fade::GetInstance()->IsEnd())
								nsScene::SceneLoader::GetInstance()->ChangeScene(IScene::EnSceneID::enSceneID_Select);
						}
						return;
					}

					/*ポーズ画面で選択できていなければ以下の処理しない。*/
					if (!m_pause->DidSelect())
					{
						m_confirmationSelect->Deactivate(); 
						return; 
					}

					/*ポーズ画面で「ゲームを続ける」と「操作方法」以外を選択できている状態。*/
					if (m_pause->GetCurrentSelect() != nsGame::Pause::EnSelect::enSelect_ReturnToGame && m_pause->GetCurrentSelect() != nsGame::Pause::EnSelect::enSelect_HowToPlay && m_pause->DidSelect())
					{
						/*確認画面でNoを選択できている状態。*/
						if (m_confirmationSelect->GetCurrentSelect() == nsSelect::ConfirmationSelect::EnConfirmationSelectTextUI::enConfirmationSelectTextUI_No &&
							m_confirmationSelect->DidSelect())
						{
							m_pause->DisableSelect();
							m_pause->DisableDirection();
							m_confirmationSelect->DisableSelect();
							m_confirmationSelect->DisableDirection();
							return;
						}

						m_confirmationSelect->Activate();
						m_confirmationSelect->EnableDrawingUI();

						/*ポーズ画面でゲームをやり直すを選択できている状態。*/
						if (m_pause->GetCurrentSelect() == nsGame::Pause::EnSelect::enSelect_RestartTheGame)
							m_confirmationSelect->ChangeConfirmationUI(nsSelect::ConfirmationSelect::EnConfirmationUI::enConfirmationUI_RestartTheGame);

						/*ポーズ画面でクエスト選択に戻るを選択できている状態。*/
						else if (m_pause->GetCurrentSelect() == nsGame::Pause::EnSelect::enSelect_ReturnToQuestSelect)
							m_confirmationSelect->ChangeConfirmationUI(nsSelect::ConfirmationSelect::EnConfirmationUI::enConfirmationUI_ReturnToQuestSelect);

						return;
					}

					/*ポーズ画面で操作方法を選択できている状態。*/
					if (m_pause->GetCurrentSelect() == nsGame::Pause::EnSelect::enSelect_HowToPlay)
					{
						/*前の画面に戻る状態。*/
						if (m_howToPlay->IsBackSelect())
						{
							m_howToPlay->DisableBackSelect();
							m_howToPlay->Deactivate();
							m_pause->EnableDrawingUI();
							m_pause->DisableSelect();
							m_pause->DisableDirection();
							return;
						}

						if (m_howToPlay->IsActive())
							m_pause->DisableDrawingUI();

						m_howToPlay->Activate();
						return;
					}

					/*ポーズ画面でゲームを続けるを選択できている状態。*/
					if (m_pause->GetCurrentSelect() == nsApp::nsGame::Pause::EnSelect::enSelect_ReturnToGame)
					{
						m_pause->Deactivate();
						m_pause->DisableDrawingUI();
						m_pause->DisableSelect();
						m_pause->DisableDirection();
					}
				}
			}

			/*インゲーム上でゲームクリア演出用のインスタンスがnullptrではないとき。*/
			if (m_game2->GetGameClearDirectionInstance() != nullptr)
			{
				/*ゲームクリア演出用のインスタンスがnullptrの場合は取得する。*/
				if (m_gameClearDirection == nullptr)
				{
					m_gameClearDirection = m_game2->GetGameClearDirectionInstance();
					return;
				}

				/*ゲームクリア演出が終わっているとき。*/
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
				/*ゲーム終了選択用のインスタンスがnullptrの場合は取得する。*/
				if (m_gameEndSelect == nullptr)
				{
					m_gameEndSelect = m_game2->GetGameEndSelectInstance();
					return;
				}

				/*ゲーム終了選択画面で選択できていなければ以下の処理しない。*/
				if (!m_gameEndSelect->DidSelect()) { return; }

				m_gameEndSelect->Deactivate();

				/*ゲーム終了選択画面でリトライを選択できている状態。*/
				if (m_gameEndSelect->GetCurrentSelect() == nsApp::GameEndSelect::enSelect_Retry)
				{
					/*フェード処理が終わったらローディングシーンに遷移する。*/
					if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
					{
						auto* loader = nsApp::nsScene::SceneLoader::GetInstance();
						loader->SetLoadingDestination(nsScene::toInGame);
						loader->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_Loading);
					}
				}
				/*ゲーム終了選択画面でクエスト選択に戻るを選択できている状態。*/
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
			m_result->SetBossType(GetBossType());
			return true;
		}

		/*更新処理。*/
		void ResultScene::Update()
		{
			auto soundListers = FindGOs<nsSound::SoundLister>("SoundManager");
			for (const auto& soundLister : soundListers)
			{
				soundLister->SetBGMVolumeRate(GetVolumeRate(0));
				soundLister->SetSEVolumeRate(GetVolumeRate(1));
				soundLister->SetMasterVolumeRate(GetVolumeRate(2));
			}

			/*リザルト画面で選択できていなければ処理しない。*/
			if (!m_result->DidSelect()) { return; }

			/*ゲーム終了選択用のインスタンスでnullptrの場合は取得する。*/
			if (m_gameEndSelect == nullptr)
			{
				m_gameEndSelect = m_result->GetGameEndSelectInstance();
				return;
			}

			/*ゲーム終了選択画面で選択できていなければ処理しない。*/
			if (!m_gameEndSelect->DidSelect()) { return; }

			m_gameEndSelect->Deactivate();

			/*ゲーム終了選択画面でリトライを選択できている状態。*/
			if (m_gameEndSelect->GetCurrentSelect() == nsApp::GameEndSelect::enSelect_Retry)
			{
				/*フェード処理が終わったらローディングシーンに遷移する。*/
				if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
				{
					auto* loader = nsApp::nsScene::SceneLoader::GetInstance();
					loader->SetLoadingDestination(nsScene::toInGame);
					loader->ChangeScene(nsApp::IScene::EnSceneID::enSceneID_Loading);
				}
			}
			/*ゲーム終了選択画面でクエスト選択に戻るを選択できている状態。*/
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


	namespace nsLoading
	{
		using namespace nsFade;

		bool LoadingScene::Start()
		{
			/* フラグをセット。*/
			m_isChangesScene = false;

			/* ローディング中はフェードを解除する。*/
			Fade::GetInstance()->ChangeFadeType(Fade::enFadeType_FadeIn);

			/* InGame生成に必要な情報を作成する。*/
			m_request.bossType = GetBossType();

			/* メンバーの数をセットする。*/
			for (int i = 0; i < nsApp::nsSelect::MemberSelect::EnCharacterFrameUI::enCharacterFrameUI_Num; i++)
			{
				/* キャラクターの役割をセットする。*/
				m_request.characterRole[i] = GetCharacterRole(i);
				/* プレイヤーの操作キャラクターかどうかをセットする。*/
				m_request.isPlayerControle[i] = GetPlayerControle(i);
			}

			/* ローディング処理を初期化する。*/
			m_loadingSceneController.Initialize(m_request, nsScene::SceneLoader::GetInstance()->GetLoadingDestination());

			return true;
		}


		void LoadingScene::Update()
		{
			/* ローディング処理を更新する。*/
			m_loadingSceneController.Update();

			/* ローディング処理に失敗した場合。*/
			if (m_loadingSceneController.IsFailed())
			{
				if (!m_isChangesScene)
				{
					/* ローディングに失敗したときはタイトルシーンに遷移する。*/
					m_isChangesScene = true;

					/* エラーメッセージを出力する。*/
					m_errorMessage = m_loadingSceneController.GetErrorMessage();
					OutputDebugStringA(m_errorMessage.c_str());
					OutputDebugStringA("\n");
				}
				return;
			}


			/* ローディングに成功した場合。*/
			if (m_loadingSceneController.IsCompleted())
			{
				if (!m_isChangesScene)
				{
					/* ローディングに成功したときはインゲームシーンに遷移する。*/
					m_isChangesScene = true;

					/* クラスを取得。*/
					auto* loader = nsScene::SceneLoader::GetInstance();

					if (loader->GetLoadingDestination() == toSelect)
						/* Selectへ遷移。*/
						loader->ChangeScene(IScene::enSceneID_Select);

					else
						/* InGameへ遷移。*/
						loader->ChangeScene(IScene::enSceneID_InGame);
				}
				return;
			}
		}


		void LoadingScene::Render(RenderContext& rc)
		{
			/* 描画。*/
			m_loadingSceneController.Render(rc);
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
			if (m_changeSceneID == IScene::enSceneID_None)
				return;/*切り替えるシーンがなければ処理しない。*/

			/*シーン用のインスタンスに現在進行中のシーンがあれば破棄する。*/
			if (m_currentScene)
			{
				if (m_currentSceneID == IScene::enSceneID_Title || m_currentSceneID == IScene::enSceneID_Select)
				{
					for (int i = 0; i < nsApp::nsOption::Option::EnGaugeUI::enGaugeUI_Num; i++)
						m_volumeRate[i] = m_currentScene->GetVolumeRate((nsApp::nsOption::Option::EnGaugeUI)i);
				}

				if (m_currentSceneID == IScene::enSceneID_Select || m_currentSceneID == IScene::enSceneID_Loading || m_currentSceneID == IScene::enSceneID_InGame)
					m_bossType = m_currentScene->GetBossType();


				/* タイトルシーン、もしくは選択シーンのときは音量の割合を保存する。*/
				if (m_currentSceneID == IScene::enSceneID_Select ||
					m_currentSceneID == IScene::enSceneID_Loading ||
					m_currentSceneID == IScene::enSceneID_InGame)
				{
					/* 選択シーン、ローディングシーン、インゲームシーンのときはキャラクター情報を保存する。*/
					for (int i = 0; i < nsApp::nsSelect::MemberSelect::EnCharacterFrameUI::enCharacterFrameUI_Num; i++)
					{
						m_isPlayerControle[i] = m_currentScene->GetPlayerControle(i);
						m_characterRole[i] = m_currentScene->GetCharacterRole(i);
					}
				}

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


			case IScene::enSceneID_Loading:
			{
				/* IDをLoadingに指定。*/
				m_currentSceneID = IScene::enSceneID_Loading;
				m_currentScene = NewGO<nsLoading::LoadingScene>(100, "asyncLoading");

				/* Bossの種類をセットする。*/
				m_currentScene->SetBossType(m_bossType);

				for (int i = 0; i < nsApp::nsSelect::MemberSelect::EnCharacterFrameUI::enCharacterFrameUI_Num; i++)
				{
					m_currentScene->SetPlayerControle(i, m_isPlayerControle[i]);
					m_currentScene->SetCharacterRole(i, m_characterRole[i]);
				}
				break;
			}

			case IScene::enSceneID_InGame:/*インゲームシーン。*/
				m_currentSceneID = IScene::enSceneID_InGame;
				m_currentScene = NewGO<nsGame::InGameScene>(0, "inGameScene");
				m_currentScene->SetBossType(m_bossType);
				for (int i = 0; i < nsApp::nsSelect::MemberSelect::EnCharacterFrameUI::enCharacterFrameUI_Num; i++)
				{
					m_currentScene->SetPlayerControle(i, m_isPlayerControle[i]);
					m_currentScene->SetCharacterRole(i, m_characterRole[i]);
				}
				break;
			case IScene::enSceneID_Result:/*リザルトシーン。*/
				m_currentSceneID = IScene::enSceneID_Result;
				m_currentScene = NewGO<nsResult::ResultScene>(0, "resultScene");
				m_currentScene->SetBossType(m_bossType);
				break;
			default:
				break;
			}

			for (int j = 0; j < nsApp::nsOption::Option::EnGaugeUI::enGaugeUI_Num; j++)
			{
				/*音量の割合の設定。*/
				m_currentScene->SetVolumeRate(j, m_volumeRate[j]);
			}

			/*ボスの種類をデフォルトに戻す。*/
			m_bossType = 0;
			/*キャラクターの役割をデフォルトに戻す。*/
			for (int i = 0; i < nsApp::nsSelect::MemberSelect::EnCharacterFrameUI::enCharacterFrameUI_Num; i++)
				m_characterRole[i] = 0;

			/*音量の割合をデフォルトに戻す。*/
			for (int j = 0; j < nsApp::nsOption::Option::EnGaugeUI::enGaugeUI_Num; j++)
				m_volumeRate[j] = 100;

			/*プレイヤーが操作するキャラクターをデフォルトに戻す。*/
			for (int k = 0; k < nsApp::nsSelect::MemberSelect::EnCharacterFrameUI::enCharacterFrameUI_Num; k++)
			{

				if (k == 0)
					m_isPlayerControle[k] = true;
				else
					m_isPlayerControle[k] = false;

			}
			/*シーンを切り替えたらIDをデフォルトに戻す。*/
			m_changeSceneID = IScene::enSceneID_None;
		}
	}
}
