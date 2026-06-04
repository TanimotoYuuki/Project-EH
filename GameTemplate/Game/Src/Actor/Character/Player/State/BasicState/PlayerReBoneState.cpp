#include "stdafx.h"
#include "PlayerReBoneState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/UI/Commentary/CommentaryUIManager.h"

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

			/* 救助行動を実況UIへ通知する。*/
			auto* commentary = FindGO<nsUI::CommentaryUIManager>("CommentaryUIManager");
			if (commentary != nullptr)
				commentary->AddActionMessage(m_player->GetCurrentWeapon(), L"Help");
		}


		void PlayerReBoneState::Update()
		{
			/* 救助開始前、または救助対象が無効になった場合は待機ステートに戻す。*/
			if (m_shouldReturnIdle)
			{
				ChangeIdleState();
				return;
			}

			/* 救助開始前は対象の有効性を確認しない。*/
			if (m_player == nullptr || m_targetPlayer == nullptr)
			{
				ChangeIdleState();
				return;
			}

			/* 救助開始後は対象の有効性を常に確認する。*/
			if (!IsValidTargetCharacter())
			{
				ChangeIdleState();
				return;
			}

			/* 救助開始後は救助進行を更新する。*/
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
			/* 救助中は入力を止める。*/
			if (m_player != nullptr)
				m_player->SetInputEnable(true);

			/* 救助が完了していない場合は、救助をキャンセルする。*/
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
			/* 対象が存在しない、または自分自身を対象にしている場合は無効。*/
			if (m_player == nullptr || m_targetPlayer == nullptr)
				return false;

			/* 自分自身を対象にすることはできない。*/
			if (m_targetPlayer == m_player)
				return false;

			/* 対象が死亡している、またはHPが0以下の場合は無効。*/
			return m_targetPlayer->IsDeath() ||
				m_targetPlayer->GetCharacterStatus().hp.currentHP <= 0;
		}


		void PlayerReBoneState::BeginHelpProgress()
		{
			/* 救助開始前、または救助対象が無効になった場合は救助を開始しない。*/
			if (m_player == nullptr || m_targetPlayer == nullptr || m_isHelpStarted)
				return;

			/* 救助開始。*/
			m_targetPlayer->GetRescueStatusManager().BeginHelp(m_player->GetCurrentWeapon());
			m_isHelpStarted = true;
		}


		void PlayerReBoneState::CompleteHelp()
		{
			if (m_targetPlayer != nullptr)
			{
				/* 救助完了。*/
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
			/* 救助開始前、または救助対象が無効になった場合は待機ステートに戻す。*/
			if (m_stateMachine != nullptr)
				m_stateMachine->ChangeState(new PlayerIdleState());
		}
	}
}
