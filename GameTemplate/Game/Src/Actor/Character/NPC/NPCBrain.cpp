#include "stdafx.h"
#include "NPCBrain.h"
#include "Boss.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/PlayerInput.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"

#include "Src/Utilty/ResourceUtility.h"
#include "Src/Actor/Character/NPC/Component/NPCActionParameterTable.h"

namespace
{
	const auto REFERENCE_VALUE_HP = 0;				//! HPの比較値。
	const auto REFERENCE_VALUE_ATTACK_INTERVAL = 0; //! 攻撃インターバルの比較値。
}

namespace nsApp
{
	NPCBrain::~NPCBrain()
	{
		/* NPCステートマシーンを削除する。*/
		delete m_npcStateMachine;
		m_npcStateMachine = nullptr;
	}


	void NPCBrain::Init(nsActor::Player* outer)
	{
		/* 仮想入力アダプタの生成。*/
		m_outer = outer;

		/* TSV導入前と同じ救助探索範囲に戻す。*/
		m_helpSearchRange = 800.0f;

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

		/* 自身がDethステートなら思考を止める。*/
		if (m_outer->IsDeath() || m_outer->GetCharacterStatus().hp.currentHP <= 0)
		{
			m_virtualInputAdapter->Reset();
			m_helpTarget = nullptr;
			return;
		}

		/* 攻撃インターバルを更新。*/
		UpdateAttackInterval();

		/* ターゲットを探す。*/
		m_helpTarget = SearchHelpTarget();

		/* ターゲットが居ない場合は何もしない。*/
		if (m_npcStateMachine != nullptr)
			m_npcStateMachine->Update();
	}


	nsActor::ICharacter* NPCBrain::SearchTarget()
	{
    	/* 目標を探索する。*/
		m_bossTarget = FindGO<nsActor::Boss>("boss");

		/* 見つからなかった場合。*/
		if (m_bossTarget == nullptr)
			return nullptr;

		/* BossのHPが0になった場合。*/
		if(m_bossTarget->GetCharacterStatus().hp.currentHP <= REFERENCE_VALUE_HP)
			return nullptr;

		return m_bossTarget;
	}


	void NPCBrain::UpdateAttackInterval()
	{
		/* 攻撃インターバルが0以下の場合は何もしない。*/
		if (m_attackIntervalTimer <= REFERENCE_VALUE_ATTACK_INTERVAL)
			return;

		/* 攻撃インターバルを減算。*/
		m_attackIntervalTimer--;

		/* 攻撃インターバルが0以下になった場合は0に補正。*/
		if(m_attackIntervalTimer < REFERENCE_VALUE_ATTACK_INTERVAL)
			m_attackIntervalTimer = REFERENCE_VALUE_ATTACK_INTERVAL;
	}


	nsActor::Player* NPCBrain::SearchHelpTarget() const
	{
		/* 早期リターン。*/
		if (m_outer == nullptr)
			return nullptr;

		/* 目標を探索する。*/
		auto* target = nsActor::ResourceUtility::SearchNearestDownCharacter(m_outer, m_helpSearchRange);

		/* 救助対象が有効かどうかをチェック。*/
		return target;
	}
}