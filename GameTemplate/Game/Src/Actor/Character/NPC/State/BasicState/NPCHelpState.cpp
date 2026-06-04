#include "stdafx.h"
#include "NPCHelpState.h"

#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCChaseState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Utilty/ResourceUtility.h"
#include "Src/Sound/SoundLister.h"

namespace
{
	const auto INPUT_STICK_VALUE_X = 1.0f; //! 救助対象に近づくためのスティック入力のX値。
	const auto INPUT_STICK_VALUE_Y = 1.0f; //! 救助対象に近づくためのスティック入力のY値。
											   
	const auto INPUT_HOLD_FRAME = 3;	   //! 救助入力を押し続けるフレーム数。
	const auto VOLUME = 1.0f;			   //! 救助SEの音量。
	const auto LIFE_TIME = 2.0f;		   //! 救助SEの寿命。
}

namespace nsApp
{
	namespace nsState
	{
		NPCHelpState::NPCHelpState(nsActor::Player* helpTarget) : m_helpTarget(helpTarget){}


		void NPCHelpState::Enter()
		{
			/* キャスト。*/
			m_brain = static_cast<NPCBrain*>(m_owner);
			if (m_brain == nullptr)
				return;

			/* Brainから必要なコンポーネントを取得する。*/
			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			/* 入力情報をリセットする。*/
			if (m_vInput != nullptr)
				m_vInput->Reset();

			/* 救助要求はまだ行っていない。*/
			m_hasRequestedHelp = false;

			/* コンストラクタで渡された救助対象を使う。*/ 
			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();
		}


		void NPCHelpState::Update()
		{
			/* 救助対象が有効でない場合、入力をリセットして追いかけ状態に遷移する。*/
			if (m_brain == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* 毎フレームタグ検索で上書きしない。*/ 
			/* ターゲットが未設定のときだけ、Brainが見つけた救助対象を使う。*/ 
			if (m_helpTarget == nullptr)
				m_helpTarget = m_brain->GetHelpTarget();

			/* 救助対象が有効でない場合、入力をリセットして追いかけ状態に遷移する。*/
			if (!IsValidHelpTarget())
			{
				/* 入力情報をリセットする。*/
				m_vInput->Reset();

				/* 追いかけ状態に遷移する。*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCChaseState());

				return;
			}

			/* 救助対象との距離を計算する。*/
			m_difference = m_helpTarget->GetPosition() - m_body->GetPosition();
			m_difference.y = 0.0f;

			/* 距離を計算する。*/
			m_distance = m_difference.Length();

			/* 一定の距離以上離れた場合、救助対象に近づく。*/
			if (m_distance > m_helpRange)
			{
				/* 近づく。*/
				MoveToHelpTarget();
				return;
			}

			/* 救助対象に近づいている場合、Help入力を実行する。*/
			ExecuteHelp();
		}


		void NPCHelpState::Exit()
		{
			/* 入力情報をリセットする。*/
			if (m_vInput != nullptr)
				m_vInput->Reset();

			/* 救助対象の状態をリセットする。*/
			if (m_helpTarget != nullptr && m_helpTarget != m_body)
				m_helpTarget->GetRescueStatusManager().SetBeingHelped(false);

			/* SEを停止する。*/
			StopHelpSE();

			m_brain = nullptr;
			m_body = nullptr;
			m_vInput = nullptr;
			m_helpTarget = nullptr;
			m_difference = Vector3::Zero;
			m_distance = 0.0f;
		}


		void NPCHelpState::MoveToHelpTarget()
		{
			/* 救助対象が有効でない場合、入力をリセットして追いかけ状態に遷移する。*/
			if (m_vInput == nullptr)
				return;

			/* 一定の距離以上離れた場合、救助対象に近づく。*/
			if (m_distance <= 0.001f)
			{
				/* 救助対象に非常に近い場合、ニュートラルなスティック入力を設定する。*/
				m_vInput->SetLStick(INPUT_STICK_VALUE_X, INPUT_STICK_VALUE_Y);
				m_vInput->SetButton(enButtonLB1, false);
				return;
			}

			/* 救助対象に近づくためのスティック入力を設定する。*/
			m_difference.Normalize();
			m_vInput->SetLStick(m_difference.x, m_difference.z);
			m_vInput->SetButton(enButtonLB1, true);
		}


		void NPCHelpState::ExecuteHelp()
		{
			/* 救助対象が有効でない場合、入力をリセットして追いかけ状態に遷移する。*/
			if (m_helpTarget == nullptr || m_body == nullptr || m_vInput == nullptr)
				return;

			/* 入力情報をリセットする。*/
			m_vInput->SetLStick(INPUT_STICK_VALUE_X, INPUT_STICK_VALUE_Y);

			/* SEの再生。*/
			StartHelpSE();

			/* 走り解除。*/
			m_vInput->SetButton(enButtonLB1, false);

			/* 救助要求は一度だけ行う。*/ 
			if (!m_helpTarget->GetRescueStatusManager().IsBeingHelped())
			{
				/* Help入力。*/
				m_vInput->RequestButton(enButtonY, INPUT_HOLD_FRAME);
			}
		}


		bool NPCHelpState::IsValidHelpTarget() const
		{
			/* 救助対象が有効でない場合、入力をリセットして追いかけ状態に遷移する。*/
			if (m_helpTarget == nullptr)
				return false;

			/* 救助対象が自分自身の場合は、死亡状態かHP0以下の状態を有効な救助対象とする。*/
			if (m_helpTarget == m_body)
				return false;

			return m_helpTarget->IsDeath() || m_helpTarget->GetCharacterStatus().hp.currentHP <= 0;
		}


		void NPCHelpState::StartHelpSE()
		{
			/* すでにSEが再生されている場合は、再度再生しない。*/
			if (m_helpSE != nullptr)
				return;
			
			/* 音源管理クラスを探索する。*/
			auto* se = FindGO<nsSound::SoundLister>("SoundManager");

			/* 音源管理クラスが見つからない場合は処理を中断する。*/
			if (se == nullptr)
				return;

			/* SEを再生する。*/
			m_helpSE = se->GetSEList().PlaySE(nsSound::SE_ID::Rescue, VOLUME, true, LIFE_TIME);
		}


		void NPCHelpState::StopHelpSE()
		{
			/* SEが再生されていない場合は、処理を中断する。*/
			if (m_helpSE == nullptr)
				return;

			/* 音源管理クラスを探索する。*/
			auto* se = FindGO<nsSound::SoundLister>("SoundManager");

			/* 音源管理クラスが見つからない場合は処理を中断する。*/
			if (se == nullptr)
				return;

			if (se == nullptr)
			{
				m_helpSE = nullptr;
				return;
			}

			/* SEを停止する。*/
			se->GetSEList().StopSE(m_helpSE);
		}
	}
}

