#include "stdafx.h"
#include "PlayerReBoneState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerReBoneState::Enter()
		{
			m_player = static_cast<nsActor::Player*>(m_owner);
			m_isHelpStarted = false;
			m_isHelpCompleted = false;
			m_shouldReturnIdle = false;

			if (m_player == nullptr)
				return;

			/* 救助対象を解決する。*/
			ResolveTargetCharacter();

			/* 救助対象がいない場合、次のフレームで戻す。*/
			if (!IsValidTargetCharacter())
			{
				m_shouldReturnIdle = true;
				return;
			}

			/* 救助進行を開始する。*/
			BeginHelpProgress();

			/* 救助中は入力を止める。*/
			m_player->SetInputEnable(false);

			/* 救助アニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Help);
		}


		void PlayerReBoneState::Update()
		{
			if (m_shouldReturnIdle)
			{
				ChangeIdleState();
				return;
			}

			if (m_player == nullptr || m_targetPlayer == nullptr)
			{
				ChangeIdleState();
				return;
			}

			if (!IsValidTargetCharacter())
			{
				ChangeIdleState();
				return;
			}

			auto& rescueStatus = m_targetPlayer->GetRescueStatusManager();

			/* 救助進行はRescueStatusManagerに任せる。*/
			if (rescueStatus.UpdateHelpProgress())
			{
				CompleteHelp();
				return;
			}

			/* 必要救助時間がアニメーションより長い場合は、見た目だけ再生し直す。*/
			if (!m_player->IsPlayAnimation())
				m_player->PlayBasicAnimation(CharacterBasicAnimationList::Help);
		}


		void PlayerReBoneState::Exit()
		{
			if (m_player != nullptr)
				m_player->SetInputEnable(true);

			if (!m_isHelpCompleted && m_targetPlayer != nullptr && IsValidTargetCharacter())
				m_targetPlayer->GetRescueStatusManager().CancelHelp();

			m_player = nullptr;
			m_targetPlayer = nullptr;
			m_isHelpStarted = false;
			m_isHelpCompleted = false;
			m_shouldReturnIdle = false;
		}


		void PlayerReBoneState::ResolveTargetCharacter()
		{
			if (m_player == nullptr)
				return;

			/* SetTargetCharacterで外から指定済みなら、それを最優先する。*/
			if (m_targetPlayer != nullptr)
				return;

			/* NPC救助の場合は、NPCHelpState/NPCBrainが決めた対象を優先する。*/
			if (m_player->GetBrain() != nullptr)
				m_targetPlayer = m_player->GetBrain()->GetHelpTarget();

			/* プレイヤー操作の救助、またはNPC側に対象が無い場合は周囲検索する。*/
			if (m_targetPlayer == nullptr)
				m_targetPlayer = m_player->SearchCharacter();
		}


		bool PlayerReBoneState::IsValidTargetCharacter() const
		{
			if (m_player == nullptr || m_targetPlayer == nullptr)
				return false;

			if (m_targetPlayer == m_player)
				return false;

			return m_targetPlayer->IsDeath() ||
				m_targetPlayer->GetCharacterStatus().hp.currentHP <= 0;
		}


		void PlayerReBoneState::BeginHelpProgress()
		{
			if (m_player == nullptr || m_targetPlayer == nullptr || m_isHelpStarted)
				return;

			m_targetPlayer->GetRescueStatusManager().BeginHelp(m_player->GetCurrentWeapon());
			m_isHelpStarted = true;
		}


		void PlayerReBoneState::CompleteHelp()
		{
			if (m_targetPlayer != nullptr)
			{
				m_isHelpCompleted = true;

				/* 救助対象を復活させる。*/
				m_targetPlayer->ReceiveHelp();

				m_targetPlayer->GetRescueStatusManager().ResetRescueStatus();
			}

			/* 待機ステートに戻す。*/
			ChangeIdleState();
		}


		void PlayerReBoneState::ChangeIdleState()
		{
			if (m_stateMachine != nullptr)
				m_stateMachine->ChangeState(new PlayerIdleState());
		}
	}
}
