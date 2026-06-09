#include "stdafx.h"
#include <time.h>
#include "Game2.h"
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
#include "Src/Sound/SoundLister.h"
#include "Boss.h"
#include "Src/Actor/Stage/BackGround.h"
#include "Src/Actor/Character/Player/Component/PlayerGenerator.h"
#include "Src/Actor/Character/Player/InputSystem/PlayerControlerHub.h"

#include "Src/Actor/Character/Common/Damage/DamageIndicatorPool.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include "Src/UI/ReboneGage/ReboneGageManager.h"
#include "Src/UI/Commentary/CommentaryUIManager.h"
#include "Src/Parameter/ParameterSystem.h"
#include <Windows.h>


namespace
{
	bool IsTriggerKey(int keyCode)
	{
		static SHORT prevKeyState[256] = { 0 };

		SHORT currentKeyState = GetAsyncKeyState(keyCode);
		bool isCurrentDown = (currentKeyState & 0x8000) != 0;
		bool isPrevDown = (prevKeyState[keyCode] & 0x8000) != 0;

		prevKeyState[keyCode] = currentKeyState;

		return isCurrentDown && !isPrevDown;
	}
}

namespace nsApp
{
	namespace nsGame
	{

		void Game2::DebugUpdateHealTest()
		{
			/* 1キーで味方全員にデバッグダメージを与える。*/
			if (IsTriggerKey('1'))
			{
				OutputDebugStringA("[DEBUG] 1 DamageParty\n");
				DebugDamageParty();
			}

			/* 2キーで味方全員のHPを表示する。*/
			if (IsTriggerKey('2'))
			{
				OutputDebugStringA("[DEBUG] 2 PrintPartyHP\n");
				DebugPrintPartyHP();
			}
		}

		void Game2::DebugDamageParty()
		{
			constexpr int DEBUG_DAMAGE = 300;

			const char* playerNames[] =
			{
				"player1",
				"player2",
				"player3",
				"player4"
			};

			for (const char* name : playerNames)
			{
				auto player = FindGO<nsActor::Player>(name);

				if (player == nullptr)
				{
					char debugText[256];
					sprintf_s(
						debugText,
						"[DEBUG DAMAGE] %s not found\n",
						name
					);

					OutputDebugStringA(debugText);
					continue;
				}

				player->ApplyDamage(DEBUG_DAMAGE);

				const auto& hp = player->GetCharacterStatus().hp;

				char debugText[256];
				sprintf_s(
					debugText,
					"[DEBUG DAMAGE] %s HP: %d / %d\n",
					name,
					hp.currentHP,
					hp.maxHP
				);

				OutputDebugStringA(debugText);
			}
		}

		void Game2::DebugPrintPartyHP()
		{
			const char* playerNames[] =
			{
				"player1",
				"player2",
				"player3",
				"player4"
			};

			for (const char* name : playerNames)
			{
				auto player = FindGO<nsActor::Player>(name);

				if (player == nullptr)
				{
					char debugText[256];
					sprintf_s(
						debugText,
						"[DEBUG HP] %s not found\n",
						name
					);

					OutputDebugStringA(debugText);
					continue;
				}

				const auto& hp = player->GetCharacterStatus().hp;

				char debugText[256];
				sprintf_s(
					debugText,
					"[DEBUG HP] %s HP: %d / %d\n",
					name,
					hp.currentHP,
					hp.maxHP
				);

				OutputDebugStringA(debugText);
			}
		}


		Game2::~Game2()
		{
			if (m_reboneGaugeUIManager != nullptr)
			{
				m_reboneGaugeUIManager->ClearPlayers();
				delete m_reboneGaugeUIManager;
				m_reboneGaugeUIManager = nullptr;
			}

			if (m_soundLister != nullptr)
			{
				m_soundLister->GetBGMList().StopBGM();
				m_soundLister = nullptr;
			}

			nsApp::nsStage::LoadStageData::GetInstance().ChangeStage(nsApp::nsStage::StageID::Invalid);
			DeleteGO(m_backGround);
			DeleteGO(m_camera);

			DeleteGO(m_gameClearDirection);
			DeleteGO(m_gameTimeUpDirection);
			DeleteGO(m_gameOverDirection);
			DeleteGO(m_gameEndSelect);
			DeleteGO(m_commentaryUIManager);
			DeleteGO(m_pause);

			m_commentaryUIManager = nullptr;
			m_player = nullptr;

			DamageProcessor::SetDamageIndicatorPool(nullptr);
			DeleteGO(m_damageIndicatorPool);
			m_damageIndicatorPool = nullptr;

			m_players.clear();

			delete m_generator;
			delete m_playerHub;
		}


		bool Game2::Start()
		{
			/* ゲーム生成処理を実行する。*/
			m_isGameActive = false;

			return true;
		}


		void Game2::Update()
		{
			if (!m_isGameActive)
				return;

			DebugUpdateHealTest();

			static int debugFrame = 0;
			++debugFrame;

			if (m_playerHub != nullptr)
			{
				if (debugFrame % 60 == 0)
				{
					OutputDebugStringA("[Game2] PlayerHub Update\n");
				}

				m_playerHub->Update();
			}
			else
			{
				if (debugFrame % 60 == 0)
				{
					OutputDebugStringA("[Game2] PlayerHub is nullptr in Update\n");
				}
			}

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

				/*フェードが終わっていたらゲーム開始演出を再生する。*/
				if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
					m_gameStartDirection->Activate();
			}

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

			if (m_reboneGaugeUIManager != nullptr)
				m_reboneGaugeUIManager->Update();
		}


		void Game2::Render(RenderContext& rc)
		{
			if (!m_isGameActive)
				return;

			/* 現在のステージを描画する。*/
			nsApp::nsStage::LoadStageData::GetInstance().Draw(rc);

			if (m_reboneGaugeUIManager != nullptr)
				m_reboneGaugeUIManager->Render(rc);
		}



		void Game2::ApplyBuildResult(const InGameBuildResult& result)
		{
			/* 生成結果を適用する。*/
			m_soundLister = result.soundLister;
			m_backGround = result.backGround;
			m_reboneGaugeUIManager = result.reboneGaugeUIManager;
			m_commentaryUIManager = result.commentaryUIManager;
			m_camera = result.camera;
			m_player = result.player;
			m_players = result.players;
			m_partyData = result.partyData;
			m_boss = result.boss;
			m_damageIndicatorPool = result.damageIndicatorPool;
			m_characterHP = result.characterHP;
			m_gameTimeLimit = result.gameTimeLimit;
			m_gameStartDirection = result.gameStartDirection;
			m_pause = result.pause;
			m_generator = result.generator;
		}


		void Game2::ActivateGame()
		{
			m_isGameActive = true;

			if (m_backGround != nullptr)
				m_backGround->Activate();

			if (m_camera != nullptr)
				m_camera->Activate();

			if (m_damageIndicatorPool != nullptr)
				m_damageIndicatorPool->Activate();

			if (m_commentaryUIManager != nullptr)
				m_commentaryUIManager->Activate();

			for (auto* player : m_players)
			{
				if (player != nullptr)
					player->Activate();
			}

			/* PlayerHubはGame2側で生成・初期化する。*/
			if (m_playerHub == nullptr)
			{
				char debugText[256];
				sprintf_s(
					debugText,
					"[Game2] ActivateGame players=%d partyData=%d player=%p boss=%p\n",
					static_cast<int>(m_players.size()),
					static_cast<int>(m_partyData.size()),
					m_player,
					m_boss
				);
				OutputDebugStringA(debugText);


				m_playerHub = new PlayerControlerHub();
				m_playerHub->Initialize(m_players, m_partyData);
			}

			/* Bossのターゲットを最終保証する。*/
			if (m_boss != nullptr && m_player != nullptr)
				m_boss->SetTarget(m_player);

			if (m_boss != nullptr)
				m_boss->Activate();

			/* フェードを切り替える。*/
			nsFade::Fade::GetInstance()->ChangeFadeType(nsFade::Fade::enFadeType_FadeIn);
		}	
	}
}