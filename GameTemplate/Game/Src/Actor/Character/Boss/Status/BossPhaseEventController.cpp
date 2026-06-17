#include "stdafx.h"
#include "BossPhaseEventController.h"

#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Boss/AI/BossAIConfig.h"
#include "GameTimeLimit.h"
#include "Src/Camera/Camera.h"
#include "Src/UI/Commentary/CommentaryUIManager.h"
#include "Src/Actor/Character/Player/InputSystem/PlayerControlerHub.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	void BossPhaseEventController::Initialize(nsActor::Boss* boss, Camera* camera, nsUI::CommentaryUIManager* commentary, nsGame::GameTimeLimit* timeLimit, PlayerControlerHub* playerHub, const std::vector<nsActor::Player*>& players)
	{
		m_boss = boss;
		m_camera = camera;
		m_commentary = commentary;
		m_timeLimit = timeLimit;
		m_playerHub = playerHub;
		m_players = players;
	}


	void BossPhaseEventController::Update()
	{
		if (m_boss == nullptr)
			return;

		/* 復帰待ち中はタイマーを進め、一定時間後に起き上がらせる。*/
		if (m_waitingForRecovery)
		{
			m_recoveryTimer += g_gameTime->GetFrameDeltaTime();

			if (m_recoveryTimer >= RECOVERY_DELAY)
			{
				m_waitingForRecovery = false;

				for (auto* player : m_players)
				{
					if (player != nullptr)
						player->ForceGetUp();
				}
			}
			return;
		}

		/* イベント中はタイマーを進め、演出を順番に発火する。*/
		if (m_isEventActive)
		{
			m_eventTimer += g_gameTime->GetFrameDeltaTime();

			/* t >= SHAKE_DELAY : カメラシェイク開始。*/
			if (!m_shakeStarted && m_eventTimer >= SHAKE_DELAY)
			{
				m_shakeStarted = true;
				if (m_camera != nullptr)
					m_camera->StartShake(SHAKE_DURATION, SHAKE_INTENSITY);
			}

			/* t >= BLOWAWAY_DELAY : プレイヤー吹っ飛び。*/
			if (!m_blowAwayStarted && m_eventTimer >= BLOWAWAY_DELAY)
			{
				m_blowAwayStarted = true;

				if (m_commentary != nullptr)
					m_commentary->ForceShowMessage(L"ガオー！！");

				for (auto* player : m_players)
				{
					if (player != nullptr)
						player->ForceBlowAway(BLOWAWAY_VELOCITY);
				}
			}

			/* ボスが咆哮ステートを抜けたらイベント終了。*/
			if (m_boss->GetCurrentStateID() != nsActor::BossStateID::enRoar)
				EndEvent();

			return;
		}

		/* イベントが発生していないときはフェーズ遷移をチェックする。*/
		CheckPhaseTransitions();
	}


	void BossPhaseEventController::CheckPhaseTransitions()
	{
		/* 50%：咆哮イベントを発火する。*/
		if (!m_phase1Notified && m_boss->IsPhase1EventTriggered())
		{
			m_phase1Notified = true;
			TriggerPhase1Event();
		}

		/* 25%：コメント表示（ステータス強化はBoss側で完了済み）。*/
		if (!m_phase2Notified &&
			m_boss->GetHPRatio() <= nsAI::BossAIConfig::PHASE2_HP_THRESHOLD)
		{
			m_phase2Notified = true;
			ApplyPhase2Effect();
		}
	}


	void BossPhaseEventController::TriggerPhase1Event()
	{
		m_isEventActive = true;
		m_eventTimer = 0.0f;
		m_shakeStarted = false;
		m_blowAwayStarted = false;

		/* t=0 : 吹っ飛びの直前まで残るように長めに表示する。*/
		if (m_commentary != nullptr)
			m_commentary->ForceShowMessage(L"ボスのようすが！！", BLOWAWAY_DELAY + 0.3f);

		/* ボスを強制的に咆哮ステートへ遷移させる。*/
		m_boss->ForceRoar();

		/* タイマーを停止する。*/
		if (m_timeLimit != nullptr)
			m_timeLimit->Deactivate();

		/* 怒りオーラエフェクトを開始する。*/
		m_boss->StartRageEffect();
	}


	void BossPhaseEventController::ApplyPhase2Effect()
	{
		if (m_commentary != nullptr)
			m_commentary->AddMessage(L"ボスがおこってる！", nsUI::CommentaryMessagePriority::Critical);
	}


	void BossPhaseEventController::EndEvent()
	{
		m_isEventActive = false;
		m_waitingForRecovery = true;
		m_recoveryTimer = 0.0f;

		/* タイマーを再開する（プレイヤーはまだダウン中だがゲームは再開）。*/
		if (m_timeLimit != nullptr)
			m_timeLimit->Activate();
	}
}