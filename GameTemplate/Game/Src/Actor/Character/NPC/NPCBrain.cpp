#include "stdafx.h"

#include "NPCBrain.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/NPC/Component/NPCBehaviorProfileTable.h"

namespace nsApp
{
	NPCBrain::~NPCBrain()
	{
		/*ステートマシンを破棄する前に依存ポインタを切る。*/
		m_outer = nullptr;
		m_virtualInputAdapter = nullptr;

		if (m_npcStateMachine != nullptr)
		{
			/* NPC ステートマシンを解放する。*/
			delete m_npcStateMachine;
			m_npcStateMachine = nullptr;
		}
	}

	void NPCBrain::Init(nsActor::Player *outer)
	{
		/* NPC 本体を保持する。*/
		m_outer = outer;

		/* 救助探索半径を SupportMind に設定（TSV 導入前と同じ 800）。*/
		m_supportMind.SetHelpSearchRange(800.0f);

		/* ステートマシンを生成し Idle から開始する。*/
		m_npcStateMachine = new nsState::StateMachine<NPCBrain>(this);
		m_npcStateMachine->ChangeState(new nsState::NPCIdleState());

		/* Threat Provider を登録（Init で 1 回だけ）。*/
		m_threatCollector.RegisterProvider(&m_bossMeleeThreatProvider);
		m_threatCollector.RegisterProvider(&m_bossFireThreatProvider);

		/* 武器別の行動プロファイルを読み込む。*/
		if (m_outer != nullptr)
			m_profile = NPCBehaviorProfileTable::GetParameter(m_outer->GetCurrentWeapon());

		/* Mind 共有コンテキストを初期化する。*/
		RefreshMindContext();
	}

	void NPCBrain::SetPartyIndex(int partyIndex)
	{
		/* 負数は 0 に補正する。*/
		if (partyIndex < 0)
			partyIndex = 0;

		m_partyIndex = partyIndex;

		/* 救助役の同優先度分担に使う値を Mind へ反映する。*/
		RefreshMindContext();
	}

	bool NPCBrain::ShouldGuard() const
	{
		return m_supportMind.ShouldGuard(BuildMindContext());
	}

	void NPCBrain::RefreshMindContext()
	{
		m_mindContext.outer = m_outer;
		m_mindContext.profile = &m_profile;
		m_mindContext.threats = &m_threatCollector;
		m_mindContext.partyIndex = m_partyIndex;
		m_mindContext.helpTarget = m_helpTarget;
	}

	NPCMindContext NPCBrain::BuildMindContext() const
	{
		/* const メソッドから Mind を呼ぶためのスナップショット。*/
		NPCMindContext ctx;
		ctx.outer = m_outer;
		ctx.profile = &m_profile;
		ctx.threats = const_cast<nsNPC::ThreatCollector *>(&m_threatCollector);
		ctx.partyIndex = m_partyIndex;
		ctx.helpTarget = m_helpTarget;
		return ctx;
	}

	void NPCBrain::Update()
	{
		/* 未初期化なら何もしない。*/
		if (m_outer == nullptr || m_virtualInputAdapter == nullptr)
			return;

		/* 死亡中は入力をリセットして思考を止める。*/
		if (m_outer->IsDeath() || m_outer->GetCharacterStatus().hp.currentHP <= 0)
		{
			m_virtualInputAdapter->Reset();
			m_helpTarget = nullptr;
			return;
		}
		/* KnockBack 中・被ダメ無敵中は NPC 思考を止める。*/ /* ← ここから追加 */
		if (m_outer->IsInKnockBackState() || m_outer->GetDamageInvincibilitySystem().IsActive())
		{
			m_virtualInputAdapter->Reset();
			return;
		}

		/* 左脳：攻撃インターバルを進める。*/
		m_combatMind.UpdateAttackInterval();

		/* 危険ゾーンを再収集する。*/
		m_threatCollector.Collect();

		/* Mind 用コンテキストを最新化する。*/
		RefreshMindContext();

		/* 後頭部：今フレームの救助対象を決める。*/
		m_helpTarget = m_supportMind.SearchHelpTarget(m_mindContext);
		m_mindContext.helpTarget = m_helpTarget;

		/* ステートマシンを更新（State は Brain API をそのまま呼ぶ）。*/
		if (m_npcStateMachine != nullptr)
			m_npcStateMachine->Update();
	}

	/* --- CombatMind へ委譲 --- */

	void NPCBrain::SetAttackInterval(int intervalFrame)
	{
		float aggression = 1.0f;
		if (m_profile.aggression > 0.0f)
			aggression = m_profile.aggression;

		m_combatMind.SetAttackInterval(intervalFrame, aggression);
	}

	void NPCBrain::StartAttackInterval()
	{
		m_combatMind.StartAttackInterval();
	}

	bool NPCBrain::CanAttack() const
	{
		return m_combatMind.CanAttack();
	}

	void NPCBrain::UpdateAttackInterval()
	{
		m_combatMind.UpdateAttackInterval();
	}

	bool NPCBrain::IsBossAttackWindow() const
	{
		return m_combatMind.IsBossAttackWindow();
	}

	/* --- LocomotionMind へ委譲 --- */

	nsActor::ICharacter *NPCBrain::SearchTarget()
	{
		return m_locomotionMind.SearchTarget();
	}

	bool NPCBrain::ShouldEvade()
	{
		/* 回避判定はタイマーを更新するため、毎回コンテキストを最新化する。*/
		RefreshMindContext();
		return m_locomotionMind.ShouldEvade(m_mindContext);
	}

	nsNPC::AvoidPathResult NPCBrain::GetEvadeDirection() const
	{
		return m_locomotionMind.GetEvadeDirection(BuildMindContext());
	}

	/* --- SupportMind へ委譲 --- */

	nsActor::Player *NPCBrain::SearchHelpTarget() const
	{
		return m_supportMind.SearchHelpTarget(BuildMindContext());
	}

	bool NPCBrain::IsDownedAllyNeedingHelp(nsActor::Player *target) const
	{
		return m_supportMind.IsDownedAllyNeedingHelp(BuildMindContext(), target);
	}

	bool NPCBrain::ShouldRespondToHelp() const
	{
		return m_supportMind.ShouldRespondToHelp(BuildMindContext());
	}

	bool NPCBrain::ShouldHealSelf() const
	{
		return m_supportMind.ShouldHealSelf(BuildMindContext());
	}

	bool NPCBrain::ShouldHealAlly() const
	{
		return m_supportMind.ShouldHealAlly(BuildMindContext());
	}

	nsActor::Player *NPCBrain::FindAllyNeedingHeal() const
	{
		return m_supportMind.FindAllyNeedingHeal(BuildMindContext());
	}
}