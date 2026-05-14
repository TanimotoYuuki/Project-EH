#include "stdafx.h"
#include "NPCBrain.h"
#include "Boss.h"

#include "Src/Actor/Character/Player/PlayerInput.h"
#include "Src/Actor/Character/Player/Player.h"

#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

namespace nsApp
{
	void NPCBrain::Init(nsActor::Player* outer)
	{
		m_outer = outer;
		/* NPC用のステートマシーンの生成。*/
		m_npcStateMachine = new nsState::StateMachine<NPCBrain>(this);

		/* NPC用ステートクラスを生成。*/
		m_npcStateMachine->ChangeState(new nsState::NPCIdleState());
	}


	void NPCBrain::Update()
	{
		/* 早期リターン。*/
		if (m_outer == nullptr || m_virtualInputAdapter == nullptr)
			return;

		/* フレーム開始の処理。*/
		m_virtualInputAdapter->BeginFlame();

		/* ターゲットを探す。*/
		m_helpTarget = m_outer->SearchCharacter();

		/* ターゲットが居ない場合は何もしない。*/
		if (m_npcStateMachine != nullptr)
			m_npcStateMachine->Update();
	}


	nsActor::ICharacter* NPCBrain::SearchTarget()
	{
		/* 目標を探索する。*/
		auto target  = FindGO<nsActor::Boss>("boss");

		/* 見つからなかった場合。*/
		if (target == nullptr || reinterpret_cast<uintptr_t>(target) == 0xFFFFFFFFFFFFFFFF)
			return nullptr;

		return target;
	}
}