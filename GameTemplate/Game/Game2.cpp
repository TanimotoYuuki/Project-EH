#include "stdafx.h"
#include <time.h>
#include "Game2.h"
#include "Src/Fade/Fade.h"

#include "Src/Actor/Stage/LoadStageData.h"
#include "Src/UI/Character/CharacterHP.h"
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
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Boss/Status/BossPhaseEventController.h"
#include "Src/Actor/Stage/BackGround.h"
#include "Src/Actor/Character/Player/Component/PlayerGenerator.h"
#include "Src/Actor/Character/Player/InputSystem/PlayerControlerHub.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"

#include "Src/Actor/Character/Common/Damage/DamageIndicatorPool.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include "Src/UI/ReboneGage/ReboneGageManager.h"
#include "Src/UI/Commentary/CommentaryUIManager.h"
#include "Src/Parameter/ParameterSystem.h"
#include <Windows.h>
#include "Src/UI/GuardGage/GuardGageUIManager.h"
#include "graphics/GraphicsEngine.h"

namespace
{
	bool IsTriggerKey(int keyCode)
	{
		static SHORT prevKeyState[256] = {0};

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
		Game2::~Game2()
		{
			/* [最重要] GPUが現在の描画命令を全て終えるのを同期して待つ。 */
			if (g_graphicsEngine != nullptr)
			{
				g_graphicsEngine->WaitDraw();
			}

			/* --- 1. UIやマネージャー系のクリア --- */
			if (m_reboneGaugeUIManager != nullptr)
			{
				m_reboneGaugeUIManager->ClearPlayers();
				delete m_reboneGaugeUIManager;
				m_reboneGaugeUIManager = nullptr;
			}

			if (m_guardGaugeUIManager != nullptr)
			{
				m_guardGaugeUIManager->ClearPlayers();
				delete m_guardGaugeUIManager;
				m_guardGaugeUIManager = nullptr;
			}

			if (m_soundLister != nullptr)
			{
				m_soundLister->GetBGMList().StopBGM();
				m_soundLister->GetSEList().Clear();
				DeleteGO(m_soundLister);
				m_soundLister = nullptr;
			}

			/* --- 2. 依存関係の強い【カメラ】をキャラクターより「先」に安全に削除 --- */
			// キャラクター(Player/Boss)のメモリが消える前にカメラを消すことで、カメラ破棄時のヌルポ参照を防ぐ
			if (m_camera != nullptr)
			{
				m_camera = nullptr; // 必ずnullptrにする
			}

			/* --- 3. ステージデータ及び背景の削除 --- */
			nsApp::nsStage::LoadStageData::GetInstance().ChangeStage(nsApp::nsStage::StageID::Invalid);

			if (m_backGround != nullptr)
			{
				DeleteGO(m_backGround);
				m_backGround = nullptr;
			}

			/* --- 4. 各演出・UI・システムインスタンスの削除（安全のためnullptrチェックと初期化を徹底） --- */
			if (m_gameClearDirection != nullptr)
			{
				DeleteGO(m_gameClearDirection);
				m_gameClearDirection = nullptr;
			}
			if (m_gameTimeUpDirection != nullptr)
			{
				DeleteGO(m_gameTimeUpDirection);
				m_gameTimeUpDirection = nullptr;
			}
			if (m_gameOverDirection != nullptr)
			{
				DeleteGO(m_gameOverDirection);
				m_gameOverDirection = nullptr;
			}
			if (m_gameEndSelect != nullptr)
			{
				DeleteGO(m_gameEndSelect);
				m_gameEndSelect = nullptr;
			}
			if (m_commentaryUIManager != nullptr)
			{
				DeleteGO(m_commentaryUIManager);
				m_commentaryUIManager = nullptr;
			}
			if (m_pause != nullptr)
			{
				DeleteGO(m_pause);
				m_pause = nullptr;
			}
			if (m_gameStartDirection != nullptr)
			{
				DeleteGO(m_gameStartDirection);
				m_gameStartDirection = nullptr;
			}
			if (m_characterHP != nullptr)
			{
				DeleteGO(m_characterHP);
				m_characterHP = nullptr;
			}
			if (m_gameTimeLimit != nullptr)
			{
				DeleteGO(m_gameTimeLimit);
				m_gameTimeLimit = nullptr;
			}

			/* --- 5. キャラクター(Player/NPC)のクリーンアップと削除 --- */
			for (auto *player : m_players)
			{
				if (player == nullptr)
					continue;

				/* Player自身のステートをIdleへ。 */
				player->ForceChangeToIdleState();

				/* NPCのステートもIdleへ。 */
				if (player->GetBrain() != nullptr)
				{
					player->GetBrain()->ChangeState(new nsState::NPCIdleState());
				}
			}

			for (auto *player : m_players)
			{
				if (player != nullptr)
				{
					if (player->GetBrain() != nullptr)
					{
						// 先に参照を切る。
						player->GetBrain()->SetVirtualInputAdapter(nullptr);
					}
					DeleteGO(player);
				}
			}
			m_players.clear();
			m_player = nullptr;

			/* --- 6. ボスの削除 --- */
			if (m_boss != nullptr)
			{
				DeleteGO(m_boss);
				m_boss = nullptr;
			}

			/* --- 7. その他のプール・コントローラー類の削除 --- */
			DamageProcessor::SetDamageIndicatorPool(nullptr);
			if (m_damageIndicatorPool != nullptr)
			{
				DeleteGO(m_damageIndicatorPool);
				m_damageIndicatorPool = nullptr;
			}

			delete m_generator;
			m_generator = nullptr;
			delete m_playerHub;
			m_playerHub = nullptr;
			delete m_bossPhaseController;
			m_bossPhaseController = nullptr;
		}

		bool Game2::Start()
		{
			/* ゲーム画面は非アクティブな状態から開始する。 */
			m_isGameActive = false;

			return true;
		}

		void Game2::Update()
		{
			if (!m_isGameActive)
				return;

			/* ゲーム開始演出の更新。 */
			if (m_gameStartDirection != nullptr)
			{
				/* ゲーム開始演出終了時、インスタンスを削除。 */
				if (m_gameStartDirection->IsDirectionFinished())
				{
					DeleteGO(m_gameStartDirection);
					m_gameStartDirection = nullptr;
					m_characterHP->Activate();
					m_gameTimeLimit->Activate();
					return;
				}

				/* フェードイン終了でゲーム開始演出再生。 */
				if (nsApp::nsFade::Fade::GetInstance()->IsEnd())
					m_gameStartDirection->Activate();
			}
			else
			{
				/* ポーズ画面を表示していないとき。 */
				if (!m_pause->IsActive())
				{
					/*タイムアップ演出がすでに生成されている場合はすべてスキップする。*/
					if (m_gameTimeUpDirection != nullptr)
					{
						if (m_gameEndSelect == nullptr)
						{
							if (m_gameTimeUpDirection->IsDirectionFinished())
							{
								m_gameTimeUpDirection->Deactivate();
								m_gameEndSelect = NewGO<GameEndSelect>(2, "gameEndSelect");
							}
						}

						nsApp::nsStage::LoadStageData::GetInstance().Update();

						if (m_reboneGaugeUIManager != nullptr)
						{
							m_reboneGaugeUIManager->Update();
						}
						if (m_guardGaugeUIManager != nullptr)
						{
							m_guardGaugeUIManager->Update();
						}
						return;
					}

					/* 演出が出ていないときのみ入力を受け付ける。 */
					if (m_gameClearDirection == nullptr &&
						m_gameOverDirection == nullptr)
					{
						/* Selectボタンでポーズ画面表示。 */
						if (g_pad[0]->IsTrigger(enButtonSelect))
						{
							m_pause->Activate();
							m_pause->EnableDrawingUI();
							return;
						}

						/* ボスフェーズイベントコントローラーの更新。 */
						if (m_bossPhaseController != nullptr)
							m_bossPhaseController->Update();

						/* イベント中かどうかを判定する。 */
						bool isEventActive =
							m_bossPhaseController != nullptr &&
							m_bossPhaseController->IsEventActive();

						/* ゲーム開始演出終了時に有効化。 */
						if (m_gameStartDirection == nullptr)
						{
							m_characterHP->Activate();

							/* イベント中はタイムリミットを開始しない。 */
							if (!isEventActive)
								m_gameTimeLimit->Activate();
						}

						/* ボスフェーズイベント中はプレイヤー/NPCの移動・更新を停止。 */
						if (m_playerHub != nullptr && !isEventActive)
							m_playerHub->Update();
					}

					/* ゲームクリア演出。 */
					/* TODO: ボスのHPが0になったら演出を流すようにする。 */
					if (m_gameClearDirection == nullptr)
					{
						if (m_boss->GetCurrentHP() <= 0)
						{
							m_gameClearDirection = NewGO<GameClearDirection>(2, "gameClearDirection");
							m_characterHP->Deactivate();
							m_gameTimeLimit->Deactivate();

							/*クリアした瞬間にカメラを安全な状態に切り替える。*/
							if (m_camera != nullptr)
							{
								m_camera->ChangeToLoading();
							}
						}
					}

					/* 時間切れ演出。 */
					if (m_gameTimeUpDirection == nullptr)
					{
						if (m_gameTimeLimit->IsTimeUp())
						{
							m_gameTimeUpDirection = NewGO<GameTimeUpDirection>(2, "gameTimeUpDirection");
							m_characterHP->Deactivate();
							m_gameTimeLimit->Deactivate();

							/*タイムアップした瞬間に、カメラを安全な状態にする。*/
							if (m_camera != nullptr)
							{
								m_camera->ChangeToLoading();
							}

							/*タイムアップした瞬間にNPCのBrainとVirtualInputの接続を切る。*/
							for (auto *player : m_players)
							{
								if (player != nullptr)
								{
									if (player->GetBrain() != nullptr)
									{
										player->GetBrain()->SetVirtualInputAdapter(nullptr);
										player->Deactivate();
									}
								}
							}
							if (m_boss != nullptr)
							{
								m_boss->Deactivate();
							}
							return; /*タイムアップを検知したフレームはここで抜ける。*/
						}
					}

					/*ゲームオーバー演出。*/
					if (m_gameOverDirection == nullptr)
					{
						if (m_characterHP->IsAllNoneCharacterHP())
						{
							m_gameOverDirection = NewGO<GameOverDirection>(2, "gameOverDirection");
							m_characterHP->Deactivate();
							m_gameTimeLimit->Deactivate();
						}
					}
					else
					{
						if (m_gameEndSelect == nullptr)
						{
							if (m_gameOverDirection->IsDirectionFinished())
							{
								m_gameOverDirection->Deactivate();
								m_gameEndSelect = NewGO<GameEndSelect>(2, "gameEndSelect");

								/*ゲームオーバー演出が終了し、選択画面が出た時点でカメラを安全にする。*/
								if (m_camera != nullptr)
								{
									m_camera->ChangeToLoading();
								}
							}
						}
					}
				}
				/* ポーズ画面表示しているとき。 */
				else
				{
					m_characterHP->Deactivate();
					m_gameTimeLimit->Deactivate();
				}
			}

			/* 現在のステージの更新を行う。 */
			nsApp::nsStage::LoadStageData::GetInstance().Update();

			if (m_reboneGaugeUIManager != nullptr)
				m_reboneGaugeUIManager->Update();

			if (m_guardGaugeUIManager != nullptr)
				m_guardGaugeUIManager->Update();
		}

		void Game2::Render(RenderContext &rc)
		{
			if (!m_isGameActive)
				return;

			/* 現在のステージを描画する。 */
			nsApp::nsStage::LoadStageData::GetInstance().Draw(rc);

			if (m_reboneGaugeUIManager != nullptr)
				m_reboneGaugeUIManager->Render(rc);

			if (m_guardGaugeUIManager != nullptr)
				m_guardGaugeUIManager->Render(rc);
		}

		void Game2::ApplyBuildResult(const InGameBuildResult &result)
		{
			/* ビルド結果を反映。 */
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
			m_guardGaugeUIManager = result.guardGaugeUIManager;
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

			for (auto *player : m_players)
			{
				if (player != nullptr)
					player->Activate();
			}

			/* PlayerHubをGame2側で生成・初期化。 */
			if (m_playerHub == nullptr)
			{
				m_playerHub = new PlayerControlerHub();
				m_playerHub->Initialize(m_players, m_partyData);
			}

			m_bossPhaseController = new BossPhaseEventController();
			m_bossPhaseController->Initialize(
				m_boss,
				m_camera,
				m_commentaryUIManager,
				m_gameTimeLimit,
				m_playerHub,
				m_players);

			/* Bossのターゲットを最終保証する。 */
			if (m_boss != nullptr && m_player != nullptr)
				m_boss->SetTarget(m_player);

			/* ボスに全プレイヤーを登録。 */
			if (m_boss != nullptr && !m_players.empty())
			{
				std::vector<nsActor::ICharacter *> allTargets(m_players.begin(), m_players.end());
				m_boss->SetAllTargets(allTargets);
			}

			/* CharacterHPにプレイヤーを登録。 */
			if (m_characterHP != nullptr)
				m_characterHP->SetPlayers(m_players);

			if (m_boss != nullptr)
				m_boss->Activate();

			/* フェードを切り替える。 */
			nsFade::Fade::GetInstance()->ChangeFadeType(nsFade::Fade::enFadeType_FadeIn);
		}
	}
}