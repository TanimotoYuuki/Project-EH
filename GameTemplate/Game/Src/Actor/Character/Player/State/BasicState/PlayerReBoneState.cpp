#include "stdafx.h"
#include "PlayerReBoneState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/UI/Commentary/CommentaryUIManager.h"

namespace
{
	const auto MOVE_DEAD_ZONE = 0.001f;
	const auto MOVE_FRAME_TIME = 1.0f / 60.0f;
}

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
			m_difference = Vector3::Zero;
			m_distance = 0.0f;

			if (m_player == nullptr)
				return;

			/* 蘇生対象を解決する（外部指定 → Brain → 周囲探索の順）。*/
			ResolveTargetCharacter();

			/* 蘇生対象がいなければ、次の Update で待機へ戻す。*/
			if (!IsValidTargetCharacter())
			{
				m_shouldReturnIdle = true;
				return;
			}

			/* 近づく間は操作可能のままにする（Y 押下直後から走れる）。*/
			m_player->SetInputEnable(true);
		}


		void PlayerReBoneState::Update()
		{
			/* Enter で対象が見つからなかった場合の後始末。*/
			if (m_shouldReturnIdle)
			{
				ChangeIdleState();
				return;
			}

			/* 参照切れなら待機へ戻す。*/
			if (m_player == nullptr || m_targetPlayer == nullptr)
			{
				ChangeIdleState();
				return;
			}

			/* 対象が蘇生不能になったら中断する。*/
			if (!IsValidTargetCharacter())
			{
				ChangeIdleState();
				return;
			}

			/* 対象までの水平距離を更新する。*/
			UpdateDistanceToTarget();

			/* 探索距離より遠いなら蘇生をやめる。*/
			if (m_distance > m_searchRange)
			{
				ChangeIdleState();
				return;
			}

			/* 蘇生開始距離より遠い間は、死体へ走って近づく。*/
			if (m_distance > m_helpRange)
			{
				MoveTowardTarget();
				return;
			}

			/* 十分近づいたら初めて蘇生を開始する。*/
			if (!m_isHelpStarted)
			{
				/* 蘇生中は操作を止める。*/
				m_player->SetInputEnable(false);

				/* 蘇生アニメーションを再生する。*/
				m_player->PlayBasicAnimation(CharacterBasicAnimationList::Help);

				/* 実際に蘇生を始めたタイミングで UI に通知する。*/
				if (auto* commentary = FindGO<nsUI::CommentaryUIManager>("CommentaryUIManager"))
					commentary->AddActionMessage(m_player->GetCurrentWeapon(), L"Help");

				/* 蘇生ゲージの進行を開始する。*/
				BeginHelpProgress();
			}

			auto& rescueStatus = m_targetPlayer->GetRescueStatusManager();

			/* 蘇生ゲージが満タンになったら完了処理へ。*/
			if (rescueStatus.UpdateHelpProgress())
			{
				CompleteHelp();
				return;
			}

			/* アニメが切れたら蘇生アニメをループ再生する。*/
			if (!m_player->IsPlayAnimation())
				m_player->PlayBasicAnimation(CharacterBasicAnimationList::Help);
		}


		void PlayerReBoneState::Exit()
		{
			/* 入力を元に戻す。*/
			if (m_player != nullptr)
				m_player->SetInputEnable(true);

			/* 蘇生完了前に抜けた場合は、対象側の蘇生進行をキャンセルする。*/
			if (!m_isHelpCompleted && m_targetPlayer != nullptr && IsValidTargetCharacter())
				m_targetPlayer->GetRescueStatusManager().CancelHelp();

			m_player = nullptr;
			m_targetPlayer = nullptr;
			m_isHelpStarted = false;
			m_isHelpCompleted = false;
			m_shouldReturnIdle = false;
			m_difference = Vector3::Zero;
			m_distance = 0.0f;
		}


		void PlayerReBoneState::ResolveTargetCharacter()
		{
			if (m_player == nullptr)
				return;

			/* NPC など外部から SetTargetCharacter 済みなら上書きしない。*/
			if (m_targetPlayer != nullptr)
				return;

			/* NPC 蘇生は Brain が決めた対象を優先する。*/
			if (m_player->GetBrain() != nullptr)
				m_targetPlayer = m_player->GetBrain()->GetHelpTarget();

			/* プレイヤー操作の Y 蘇生は周囲探索で対象を決める。*/
			if (m_targetPlayer == nullptr)
				m_targetPlayer = m_player->SearchCharacter();
		}


		bool PlayerReBoneState::IsValidTargetCharacter() const
		{
			if (m_player == nullptr || m_targetPlayer == nullptr)
				return false;

			/* 自分自身は蘇生対象にできない。*/
			if (m_targetPlayer == m_player)
				return false;

			/* ダウン中（死亡フラグ or HP0）だけ有効。*/
			return m_targetPlayer->IsDeath() ||
				m_targetPlayer->GetCharacterStatus().hp.currentHP <= 0;
		}


		void PlayerReBoneState::BeginHelpProgress()
		{
			if (m_player == nullptr || m_targetPlayer == nullptr || m_isHelpStarted)
				return;

			/* 救助者の武器種で必要時間を決めて蘇生開始。*/
			m_targetPlayer->GetRescueStatusManager().BeginHelp(m_player->GetCurrentWeapon());
			m_isHelpStarted = true;
		}


		void PlayerReBoneState::CompleteHelp()
		{
			if (m_targetPlayer != nullptr)
			{
				m_isHelpCompleted = true;

				/* 対象を蘇生して HP を満タンに戻す。*/
				m_targetPlayer->ReceiveHelp();
				m_targetPlayer->GetRescueStatusManager().ResetRescueStatus();
			}

			ChangeIdleState();
		}


		void PlayerReBoneState::ChangeIdleState()
		{
			if (m_stateMachine != nullptr)
				m_stateMachine->ChangeState(new PlayerIdleState());
		}


		void PlayerReBoneState::UpdateDistanceToTarget()
		{
			if (m_player == nullptr || m_targetPlayer == nullptr)
			{
				m_difference = Vector3::Zero;
				m_distance = 0.0f;
				return;
			}

			/* Y 成分を無視して水平距離だけを見る。*/
			m_difference = m_targetPlayer->GetPosition() - m_player->GetPosition();
			m_difference.y = 0.0f;
			m_distance = m_difference.Length();
		}


		bool PlayerReBoneState::MoveTowardTarget()
		{
			if (m_player == nullptr)
				return false;

			/* ほぼ同位置なら移動しない。*/
			if (m_distance <= MOVE_DEAD_ZONE || m_difference.LengthSq() <= MOVE_DEAD_ZONE)
				return false;

			Vector3 dir = m_difference;
			dir.Normalize();

			/* 走行速度で死体方向へ移動する。*/
			m_player->MoveWithBattleClamp(dir * m_player->GetRunSpeed(), MOVE_FRAME_TIME);
			return true;
		}
	}
}