#include "stdafx.h"
#include "NPCEvadeState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCHelpState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/Movement/NPCMovementController.h"

namespace
{
	const auto EVADE_DIRECTION_LOCK_FRAMES = 25; //! 回避方向を維持するフレーム数。
}

namespace nsApp
{
	namespace nsState
	{
		void NPCEvadeState::Enter()
		{
			/* m_owner を NPCBrain にキャストする。*/
			m_brain = static_cast<NPCBrain *>(m_owner);
			if (m_brain == nullptr)
				return;

			/* NPCBrain から Player クラスのポインタを取得する。*/
			m_body = m_brain->GetBody();

			/* NPCBrain から VirtualInputAdapter のポインタを取得する。*/
			m_vInput = m_brain->GetVirtualInputAdapter();

			/* 回避状態に入ったら方向ロックをリセットする。*/
			ResetLockedDirection();

			/* 回避状態に入ったら移動入力をクリアする。*/
			ClearMoveInput();
		}

		void NPCEvadeState::Update()
		{
			/* 早期リターン。*/
			if (m_brain == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* 優先度 Help > Evade > Chase。*/
			auto *helpTarget = m_brain->GetHelpTarget();
			if (m_brain->ShouldRespondToHelp())
			{
				/* 助ける対象が死亡している場合は、助ける対象をクリアする。*/
				ClearMoveInput();

				/* 助ける対象が死亡している場合は、NPCHelpState へ遷移する。*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCHelpState(helpTarget));

				return;
			}

			/* 危険が解消されたら追跡へ戻る。*/
			if (!m_brain->ShouldEvade())
			{
				/* 危険が解消されたら移動入力をクリアする。*/
				ClearMoveInput();

				/* 危険が解消されたら NPCChaseState へ遷移する。*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCChaseState());
				return;
			}

			/* 危険がある場合は回避方向へ移動する。*/
			MoveEvadeDirection();
		}

		void NPCEvadeState::Exit()
		{
			/*破棄処理等でBrain側の参照が切られている可能性があるため、使用前に取得する。*/
			if (m_brain != nullptr)
			{
				m_vInput = m_brain->GetVirtualInputAdapter();
			}

			/* 回避状態を抜けるときは方向ロックをリセットする。*/
			ResetLockedDirection();

			/* ブレインとコンポーネントがない場合は移動入力をクリアできないため、ここで終了する。*/
			if (m_brain != nullptr && m_vInput != nullptr)
			{
				ClearMoveInput();
			}
			m_brain = nullptr;
			m_body = nullptr;
			m_vInput = nullptr;
		}

		void NPCEvadeState::ClearMoveInput()
		{
			/* 移動入力をクリアする。*/
			NPCMovementController::Stop(m_vInput);
		}

		void NPCEvadeState::ResetLockedDirection()
		{
			/* ロック方向とタイマーを初期化する。*/
			m_lockedDirection = Vector3::Zero;
			m_directionLockTimer = 0;
			m_hasLockedDirection = false;
		}

		void NPCEvadeState::MoveEvadeDirection()
		{
			Vector3 moveDirection = Vector3::Zero;

			/* ロック中は同じ方向を維持する。*/
			if (m_hasLockedDirection && m_directionLockTimer > 0)
			{
				moveDirection = m_lockedDirection;
				--m_directionLockTimer;
			}
			else
			{
				/* Brain から回避方向を取得する。*/
				const nsNPC::AvoidPathResult result = m_brain->GetEvadeDirection();
				if (!result.hasValidDirection)
				{
					ResetLockedDirection();
					ClearMoveInput();
					return;
				}

				/* 新しい方向をロックする。*/
				moveDirection = result.direction;
				m_lockedDirection = moveDirection;
				m_hasLockedDirection = true;
				m_directionLockTimer = EVADE_DIRECTION_LOCK_FRAMES;
			}

			/* 回避方向へ移動する。*/
			const bool useRun = m_brain->GetBehaviorProfile().preferRunInEvade;
			const NPCMovementIntent intent = NPCMovementController::MakeMoveIntent(moveDirection, useRun);
			NPCMovementController::Apply(m_vInput, intent);
		}
	}
}