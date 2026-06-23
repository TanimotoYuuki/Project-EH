#include "stdafx.h"
#include "NPCLocomotonMind.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/NPC/Navigation/SimpleAvoidPathfinder.h"
#include "Src/Actor/Character/NPC/Social/NPCSocialContext.h"
#include "Src/Actor/Character/Player/Player.h"  
#include <cstdlib>

namespace
{
	const auto REFERENCE_VALUE_HP = 0; //! HP 比較用。

	/**
	 * @brief min〜max の乱数（回避反応遅延用）。
	 * @param minValue 最小値。
	 * @param maxValue 最大値。
	 */
	inline int RandomRange(int minValue, int maxValue)
	{
		if (minValue >= maxValue)
			return minValue;

		return minValue + (rand() % (maxValue - minValue + 1));
	}


	inline float GetScaledDangerCost(const nsApp::NPCMindContext& ctx)
	{
		if (ctx.outer == nullptr || ctx.profile == nullptr || ctx.threats == nullptr)
			return 0.0f;
		const float raw = ctx.threats->GetTotalDangerCost(ctx.outer->GetPosition());
		return raw * ctx.profile->threatMarginScale;
	}
}

namespace nsApp
{
	nsActor::ICharacter* NPCLocomotionMind::SearchTarget()
	{
		/* 目標を探索する。*/
		m_bossTarget = FindGO<nsActor::Boss>("boss");
		if (m_bossTarget == nullptr)
			return nullptr;

		/* Boss の HP が 0 なら戦闘終了。*/
		if (m_bossTarget->GetCharacterStatus().hp.currentHP <= REFERENCE_VALUE_HP)
			return nullptr;

		return m_bossTarget;
	}


	bool NPCLocomotionMind::IsDangerous(const NPCMindContext& ctx) const
	{
		/* 早期リターン。*/
		if (ctx.outer == nullptr || ctx.profile == nullptr || ctx.threats == nullptr)
			return false;

		/* 危険度が閾値を超えているか。*/
		const float cost = GetScaledDangerCost(ctx);
		return cost > ctx.profile->evadeThreshold;
	}


	bool NPCLocomotionMind::ShouldEvade(NPCMindContext& ctx)
	{
		/* 早期リターン。*/
		if (ctx.outer == nullptr || ctx.profile == nullptr || ctx.threats == nullptr)
			return false;

		/* 危険度を取得。*/
		const float cost = GetScaledDangerCost(ctx);
		const float enterThreshold = ctx.profile->evadeThreshold;

		/* 退出閾値が未設定なら、テーブル読み込みと同じ 65% を使う。*/
		float exitThreshold = ctx.profile->evadeExitThreshold;
		if (exitThreshold <= 0.0f)
			exitThreshold = enterThreshold * 0.65f;

		/* 回避中：退出閾値を下回るまで継続する。*/
		if (m_isEvading)
		{
			/* 最低継続タイマーを消化。*/
			if (m_evadeMinTimer > 0)
				--m_evadeMinTimer;

			/* 危険度が退出閾値以下で、最低継続タイマーも消化していれば回避終了。*/
			if (cost <= exitThreshold && m_evadeMinTimer <= 0)
			{
				m_isEvading = false;
				m_evadeReactionTimer = 0;
				m_evadeDelayConsumed = false;
				return false;
			}
			return true;
		}

		/* 未回避：進入閾値以下なら回避しない。*/
		if (cost <= enterThreshold)
		{
			m_evadeReactionTimer = 0;
			m_evadeDelayConsumed = false;
			return false;
		}

		/* 反応遅延中はまだ回避しない。*/
		if (m_evadeReactionTimer > 0)
		{
			--m_evadeReactionTimer;
			return false;
		}

		/* 遅延消化済みなら回避開始。*/
		if (m_evadeDelayConsumed)
		{
			m_isEvading = true;
			m_evadeMinTimer = 18;
			return true;
		}

		/* 初回検知：遅延をセットして次フレーム以降に回避。*/
		m_evadeDelayConsumed = true;
		m_evadeReactionTimer = CalcEvadeReactionDelay(*ctx.profile);
		return false;
	}


	nsNPC::AvoidPathResult NPCLocomotionMind::GetEvadeDirection(const NPCMindContext& ctx) const
	{
		/* 早期リターン。*/
		if (ctx.outer == nullptr || ctx.threats == nullptr)
			return nsNPC::AvoidPathResult();

		/* 回避方向を計算する。*/
		nsNPC::NPCSocialContext socialContext;
		socialContext.self = ctx.outer;

		/* パーティメンバーを避ける必要があるか。*/
		return nsNPC::SimpleAvoidPathfinder::Compute(ctx.outer->GetPosition(), ctx.threats->GetActiveZones(), ctx.partyIndex, socialContext);
	}


	int NPCLocomotionMind::CalcEvadeReactionDelay(const NPCBehaviorProfile& profile) const
	{
		/* 最小値と最大値が同じならその値を返す。*/
		return RandomRange(profile.evadeReactionDelayMin, profile.evadeReactionDelayMax);
	}
}