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
	const auto INIT_CHARACTER_POSITION_Y = 50.0f;
	const auto INIT_CHARACTER_POSITION_Z = 0.0f;

	const auto INIT_CHARACTER_POSITION_PLAYER1 = Vector3(80.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
	const auto INIT_CHARACTER_POSITION_PLAYER2 = Vector3(-80.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
	const auto INIT_CHARACTER_POSITION_PLAYER3 = Vector3(-50.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
	const auto INIT_CHARACTER_POSITION_PLAYER4 = Vector3(50.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
}

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

		
			delete m_generator;
			delete m_playerHub;
		}


		bool Game2::Start()
		{
			/* 乱数の初期化。*/
			srand(static_cast<unsigned int>(time(nullptr)));

			/* 音源クラスの設定項目。*/
			SettingSound();

			/* 初期ステージのセット。*/
			/* @TODO ステージ選択画面からこの処理を呼ぶようにする。*/
			nsApp::nsStage::LoadStageData::GetInstance().ChangeStage(nsApp::nsStage::StageID::stage1);

			
			m_backGround = NewGO<nsStage::BackGround>(0, "BackGround");
			/* カメラを生成。*/
			m_camera = NewGO<Camera>(0, "camera");


			/* ダメージプールを生成。*/
			m_damageIndicatorPool = NewGO<DamageIndicatorPool>(0, "damagePool");
			DamageProcessor::SetDamageIndicatorPool(m_damageIndicatorPool);

			/* Player達を生成。*/
			SpawnPlayCharacter();

			///*ボスを作成。*/
			m_boss = NewGO<nsActor::Boss>(0, "boss");
			/*ボスの種類を設定。*/
			m_boss->SetBossType((CharacterModelType)GetBossType());

			if(m_boss != nullptr)
				/*ボスにプレイヤーをターゲットとして教える。*/
				m_boss->SetTarget(m_player);


			m_characterHP = NewGO<CharacterHP>(0, "characterHP");
			for (int i = 0; i < CharacterHP::EnCharacter::enCharacter_Num; i++)
			{
				m_characterHP->SetCharacterRole(i, GetCharacterRole(i));
			}
			m_characterHP->Deactivate();

			m_gameTimeLimit = NewGO<GameTimeLimit>(0, "gameTimeLimit");
			m_gameTimeLimit->SetTimeLimit(180);
			m_gameTimeLimit->Deactivate();

			m_gameStartDirection = NewGO<GameStartDirection>(2, "gameStartDirection");
			m_gameStartDirection->Deactivate();

			m_pause = NewGO<Pause>(0, "pause");
			m_pause->Deactivate();

			/*フェードインに切り替える。*/
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeIn);

			/* コメントの生成。*/
			SettingCommentaryUI();
			return true;
		}


		void Game2::Update()
		{
			DebugUpdateHealTest();

			if(m_playerHub)
				m_playerHub->Update();
			
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
				{
					m_gameStartDirection->Activate();
				}
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

			if(m_reboneGaugeUIManager != nullptr)
				m_reboneGaugeUIManager->Update();
		}


		void Game2::Render(RenderContext &rc)
		{
			/* 現在のステージを描画する。*/
			nsApp::nsStage::LoadStageData::GetInstance().Draw(rc);

			if(m_reboneGaugeUIManager != nullptr)
				m_reboneGaugeUIManager->Render(rc);
		}


		void Game2::SpawnPlayCharacter()
		{
			/* 生成システムクラスを生成する。*/
			m_generator = new PlayerGenerator();

			for (int i = 0; i < 4; i++)
			{
				if (m_isPlayerControle[i])
				{
					m_controllerType[i] = (ControllerType)i;
				}
				else
				{
					m_controllerType[i] = ControllerType::NPC;
				}
			}

			/* PlayerGeneratorを用い、プレイアブルキャラを作成する。*/
			std::vector<PlayerSpawnData> partyData =
			{
				{"player1", (WeaponType)GetCharacterRole(0), m_controllerType[0], INIT_CHARACTER_POSITION_PLAYER1},
				{"player2", (WeaponType)GetCharacterRole(1), m_controllerType[1], INIT_CHARACTER_POSITION_PLAYER2},
				{"player3", (WeaponType)GetCharacterRole(2), m_controllerType[2], INIT_CHARACTER_POSITION_PLAYER3},
				{"player4", (WeaponType)GetCharacterRole(3), m_controllerType[3], INIT_CHARACTER_POSITION_PLAYER4}
			};

			/* 作成したリストをセットする。*/
			auto players = m_generator->SpawnPlayers(partyData);
			if (!players.empty())
				m_player = players[0];

			/* ReboneUIを登録。*/
			if (m_reboneGaugeUIManager != nullptr)
			{
				for (auto* player : players)
					m_reboneGaugeUIManager->RegisterPlayer(player);
			}

			/* Hubを生成する。*/
			m_playerHub = new PlayerControlerHub();
			m_playerHub->Initialize(players, partyData);
		}


		void Game2::SettingSound()
		{
			/* 音源クラスを生成する。*/
			m_soundLister = FindGO<nsSound::SoundLister>("SoundManager");

			if (m_soundLister == nullptr)
				m_soundLister = NewGO<nsSound::SoundLister>(0, "SoundManager");

			/* 初期化。*/
			m_soundLister->InitSound();

			/* BGMを再生。*/ 
			m_soundLister->GetBGMList().StopBGM();

			/* ステージBGMを再生する。*/
			m_soundLister->GetBGMList().PlayBGM(nsSound::BGM_ID::Stage1, 1.0f);
		}


		void Game2::SettingCommentaryUI()
		{
			/* UIを生成。*/
			m_reboneGaugeUIManager = new nsUI::ReboneGaugeUIManager();
			m_reboneGaugeUIManager->Init();

			/* 実況UIを生成。*/
			m_commentaryUIManager = NewGO<nsUI::CommentaryUIManager>(0, "CommentaryUIManager");
		}
	}
}