#include "stdafx.h"
#include "Game.h"
#include "Src/Fade/Fade.h"

#include "Src/Actor/Stage/LoadStageData.h"

#include "CharacterHP.h"
#include "GameTimeLimit.h"
#include "Src/Direction/GameStartDirection.h"
#include "Src/Direction/GameClearDirection.h"
#include "Src/Direction/GameTimeUpDirection.h"
#include "Src/Direction/GameOverDirection.h"
#include "Src/Select/GameEndSelect.h"
#include "Src/Scene/InGame/Pause.h"
#include "Src/Camera/Camera.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Debug/Sandbag.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Actor/Stage/BackGround.h"

#include "Src/Actor/Character/Player/CharacterByWeapon/SwordCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/HammerCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/WandCharacter.h"

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
			DeleteGO(m_pause);
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

			m_player = NewGO<nsActor::WandCharacter>(0, "player4");

			m_pause = NewGO<Pause>(0, "pause");
			m_pause->Deactivate();

			//////////////////////////////////////////////////////////////////////
			//// ダミーモデル用。////////////////////////////////////////////////
			//m_player2 = NewGO<nsActor::HammerCharacter>(0, "player2");
			//m_player2->SetPosition(Vector3(-150.0f, 50.0f, 0.0f));
			//m_player2->SetInputEnable(false);

			/*フェードインに切り替える。*/
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeIn);

			m_sandbag = NewGO<nsActor::Sandbag>(0, "Sandbag");

			return true;
		}


		void Game::Update()
		{
			/*ゲーム開始用のインスタンスがnullptrではなければ。*/
			if (m_gameStartDirection != nullptr)
			{
				/*ゲーム開始処理が終わっていたらインスタンスを削除する。*/
				if (m_gameStartDirection->IsDirectionFinished())
				{
					DeleteGO(m_gameStartDirection);
					m_gameStartDirection = nullptr;
					m_characterHP->Activate();
					m_gameTimeLimit->Activate();
					return;
				}

				/*フェードがおわっていたらゲーム開始演出を再生する。*/
				if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
				{
					m_gameStartDirection->Activate();
				}
			}
			/*ゲーム開始用のインスタンスがnullptrの場合。*/
			else
			{
				/*ポーズ画面が表示していないとき。*/
				if (!m_pause->IsActive())
				{
					/*演出が流れていなければ処理する*/
					if (m_gameClearDirection == nullptr &&/*ゲームクリア演出*/
						m_gameOverDirection == nullptr &&/*ゲームオーバー演出*/
						m_gameTimeUpDirection == nullptr/*時間切れ演出*/
						)
					{
						/*Selectボタンを押すとポーズ画面を表示する。*/
						if (g_pad[0]->IsTrigger(enButtonSelect))
						{
							m_pause->Activate();
							m_pause->EnableDrawingUI();
							return;
						}

						/*ゲーム開始演出が終了しているときに処理する。*/
						if (m_gameStartDirection == nullptr)
						{
							m_characterHP->Activate();
							m_gameTimeLimit->Activate();
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
				}
				/*ポーズ画面が表示しているとき。*/
				else
				{
					m_characterHP->Deactivate();
					m_gameTimeLimit->Deactivate();
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
	}

}