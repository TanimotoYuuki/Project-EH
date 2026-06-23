#include "stdafx.h"
#include "NPCHelpState.h"

#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/NPC/Movement/NPCMovementController.h"
#include "Src/Utilty/ResourceUtility.h"
#include "Src/Sound/SoundLister.h"

namespace
{
	const auto MOVE_DEAD_ZONE = 0.001f;  //! 移動停止とみなす距離。
	const auto INPUT_HOLD_FRAME = 3;     //! 救助入力を押し続けるフレーム数。
	const auto VOLUME = 1.0f;            //! 救助 SE の音量。
	const auto LIFE_TIME = 2.0f;         //! 救助 SE の寿命。
}

namespace nsApp
{
	namespace nsState
	{
		NPCHelpState::NPCHelpState(nsActor::Player* helpTarget) : m_helpTarget(helpTarget)
		{
		}


		void NPCHelpState::Enter()
		{
			/* ブレインとコンポーネントの取得。*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			if (m_brain == nullptr)
				return;

			/* ブレインから必要なコンポーネントを取得。*/
			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			/* ブレインとコンポーネントがない場合はこの状態を維持できないため、ここで終了する。*/
			if (m_vInput != nullptr)
				m_vInput->Reset();

			/* タグで上書きしない。ターゲット未設定時だけ Brain から取得。*/
			m_hasRequestedHelp = false;
			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();
		}


		void NPCHelpState::Update()
		{
			if (m_brain == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* 毎フレームタグで上書きしない。ターゲット未設定時だけ Brain から取得。*/
			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();

			/* 救助対象が有効でない場合は待機ステートへ遷移。*/
			if (!IsValidHelpTarget())
			{
				if (m_vInput != nullptr)
					m_vInput->Reset();

				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCChaseState());

				return;
			}

			/* 救助対象が死亡している場合は救助行動を実行。*/
			m_difference = m_helpTarget->GetPosition() - m_body->GetPosition();
			m_difference.y = 0.0f;
			m_distance = m_difference.Length();

			/* 救助対象が範囲外にいる場合は接近する。*/
			if (m_distance > m_helpRange)
			{
				/* 救助対象が範囲外にいる場合は接近する。*/
				MoveToHelpTarget();
				return;
			}

			/* 救助対象が範囲内に入ったら救助行動を実行。*/
			ExecuteHelp();
		}


		void NPCHelpState::Exit()
		{
			/* 救助対象が有効でない場合は待機ステートへ遷移。*/
			if (m_vInput != nullptr)
				m_vInput->Reset();

			/* 救助対象の救助状態をリセット。*/
			if (m_helpTarget != nullptr && m_helpTarget != m_body)
				m_helpTarget->GetRescueStatusManager().SetBeingHelped(false);

			/* 救助 SE を停止。*/
			StopHelpSE();

			/* リセット。*/
			m_brain = nullptr;
			m_body = nullptr;
			m_vInput = nullptr;
			m_helpTarget = nullptr;
			m_difference = Vector3::Zero;
			m_distance = 0.0f;
		}


		void NPCHelpState::MoveToHelpTarget()
		{
			/* ブレインとコンポーネントがない場合は移動入力を入れられないため、ここで終了する。*/
			if (m_vInput == nullptr)
				return;

			/* 救助対象が近すぎる場合は移動入力をクリアして停止する。*/
			if (m_distance <= MOVE_DEAD_ZONE)
			{
				NPCMovementController::Stop(m_vInput);
				return;
			}

			/* 危険時はガードしながら接近。*/
			if (m_brain != nullptr && m_brain->ShouldGuard())
				m_vInput->RequestButton(enButtonLB2, 3);

			/* 救助対象へ走って近づく。*/
			const NPCMovementIntent intent = NPCMovementController::MakeMoveIntent(m_difference, true);
			NPCMovementController::Apply(m_vInput, intent);
		}


		void NPCHelpState::ExecuteHelp()
		{
			/* ブレインとコンポーネントがない場合は救助行動を実行できないため、ここで終了する。*/
			if (m_helpTarget == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* 救助中は移動を止める。*/
			NPCMovementController::Stop(m_vInput);

			/* 救助時のSEを再生し始める。*/
			StartHelpSE();

			/* 既に誰かが救助中なら上書きしない。*/
			if (m_helpTarget->GetRescueStatusManager().IsBeingHelped())
				return;

			/* 救助中もガードを維持（LB2 長押し）。*/
			m_vInput->RequestButton(enButtonLB2, INPUT_HOLD_FRAME);

			/* Player 側を直接救助ステートへ（攻撃中でも開始できる）。*/
			m_body->TryBeginHelpToTarget(m_helpTarget);
		}


		bool NPCHelpState::IsValidHelpTarget() const
		{
			/* 救助対象が有効でない場合は false を返す。*/
			if (m_helpTarget == nullptr)
				return false;

			/* 自分自身を救助対象にすることはないため、false を返す。*/
			if (m_helpTarget == m_body)
				return false;

			/* 救助対象が死亡している場合は true を返す。*/
			return m_helpTarget->IsDeath() || m_helpTarget->GetCharacterStatus().hp.currentHP <= 0;
		}


		void NPCHelpState::StartHelpSE()
		{
			/* 既に救助 SE が再生中なら何もしない。*/
			if (m_helpSE != nullptr)
				return;

			/* 音源クラスを取得する。*/
			auto* se = FindGO<nsSound::SoundLister>("SoundManager");
			if (se == nullptr)
				return;

			/* 救助 SE を再生する。*/
			m_helpSE = se->GetSEList().PlaySE(nsSound::SE_ID::Rescue, VOLUME, true, LIFE_TIME);
		}


		void NPCHelpState::StopHelpSE()
		{
			if (m_helpSE == nullptr)
				return;

			/* 音源クラスを取得する。*/
			auto* se = FindGO<nsSound::SoundLister>("SoundManager");
			if (se == nullptr)
			{
				m_helpSE = nullptr;
				return;
			}

			/* 停止。*/
			se->GetSEList().StopSE(m_helpSE);
		}
	}
}