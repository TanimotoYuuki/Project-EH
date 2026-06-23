#include "stdafx.h"
#include "NPCSupportMind.h"
#include "Src/Utilty/ResourceUtility.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Status/NPCStatusParameterTable.h"
#include "Src/Actor/Character/NPC/NPCBrain.h"

namespace
{
	const float SELF_HEAL_HP_RATE = 0.65f;   //! 自分 HP 55% 以下で自己回復。
	const float ALLY_HEAL_HP_RATE = 0.70f;   //! 味方 HP 60% 以下で味方回復。
	const float HEAL_SEARCH_RANGE = 500.0f; //! 味方回復の検索範囲（X距離）。
	const float HEAL_CRITICAL_HP_RATE = 0.35f; //! この以下は最優先回復。
	const int   HEAL_MULTI_INJURED_COUNT = 2;  //! この人数以上で回復優先度アップ。
}

namespace nsApp
{
	void NPCSupportMind::SetHelpSearchRange(float range)
	{
		/* 救助探索半径を更新する。*/
		m_helpSearchRange = range;
	}


	nsActor::Player* NPCSupportMind::SearchHelpTarget(const NPCMindContext& ctx) const
	{
		/* 早期リターン。*/
		if (ctx.outer == nullptr)
			return nullptr;

		/* 範囲内で最も近いダウン味方を探索する。*/
		return nsActor::ResourceUtility::SearchNearestDownCharacter(ctx.outer, m_helpSearchRange);
	}


	bool NPCSupportMind::IsDownedAllyNeedingHelp(const NPCMindContext& ctx, nsActor::Player* target) const
	{
		/* 早期リターン。*/
		if (ctx.outer == nullptr || target == nullptr)
			return false;

		/* 自分自身は救助対象にしない。*/
		if (target == ctx.outer)
			return false;

		/* 死亡または HP0 なら救助が必要。*/
		return target->IsDeath() || target->GetCharacterStatus().hp.currentHP <= 0;
	}


	bool NPCSupportMind::ShouldRespondToHelp(const NPCMindContext& ctx) const
	{
		/* 早期リターン。*/
		if (ctx.outer == nullptr || ctx.helpTarget == nullptr)
			return false;

		/* 救助対象が本当にダウンしているか確認。*/
		if (!IsDownedAllyNeedingHelp(ctx, ctx.helpTarget))
			return false;

		/* 自分の救助優先度を取得（TSV: 数値が小さいほど優先）。*/
		const int myPriority =
			NPCStatusParameterTable::GetParameter(ctx.outer->GetCurrentWeapon()).helpPriority;

		const char* playerNames[] = { "player1", "player2", "player3", "player4" };

		/* 生存中の他プレイヤーと優先度を比較する。*/
		for (const char* name : playerNames)
		{
			auto* other = FindGO<nsActor::Player>(name);
			if (other == nullptr || other == ctx.outer)
				continue;

			/* 操作キャラは救助役争いから除外する。*/
			if (!other->IsNpcControlled())
				continue;

			/* ダウン中は救助役の競合対象にしない。*/
			if (other->IsDeath() || other->GetCharacterStatus().hp.currentHP <= 0)
				continue;

			if (other->GetBrain() == nullptr)
				continue;

			const int otherPriority =
				NPCStatusParameterTable::GetParameter(other->GetCurrentWeapon()).helpPriority;

			/* より優先度の高い NPC がいれば自分は行かない（Hammer=1 が最優先）。*/
			if (otherPriority < myPriority)
				return false;

			/* 同優先度ならパーティ番号が小さい方が行く。*/
			if (otherPriority == myPriority && other->GetBrain()->GetPartyIndex() < ctx.partyIndex)
				return false;
		}

		return true;
	}


	bool NPCSupportMind::ShouldHealSelf(const NPCMindContext& ctx) const
	{
		/* 早期リターン。*/
		if (ctx.outer == nullptr)
			return false;

		/* 杖以外は自己回復しない。*/
		if (ctx.outer->GetCurrentWeapon() != WeaponType::Wand)
			return false;

		const auto& hp = ctx.outer->GetCharacterStatus().hp;
		if (hp.maxHP <= 0)
			return false;

		/* HP 割合が閾値以下なら自己回復。*/
		return static_cast<float>(hp.currentHP) / static_cast<float>(hp.maxHP) <= SELF_HEAL_HP_RATE;
	}


	nsActor::Player* NPCSupportMind::FindAllyNeedingHeal(const NPCMindContext& ctx) const
	{
		/* 早期リターン。*/
		if (ctx.outer == nullptr)
			return nullptr;

		const char* playerNames[] = { "player1", "player2", "player3", "player4" };
		nsActor::Player* bestTarget = nullptr;
		float lowestRate = 1.0f;

		/* 探索範囲内で最も HP が低い味方を選ぶ。*/
		for (const char* name : playerNames)
		{
			auto* other = FindGO<nsActor::Player>(name);
			if (other == nullptr || other == ctx.outer)
				continue;

			/* ダウン・死亡は回復対象外（救助が別途担当）。*/
			if (other->IsDeath() || other->GetCharacterStatus().hp.currentHP <= 0)
				continue;

			const auto& hp = other->GetCharacterStatus().hp;
			if (hp.maxHP <= 0)
				continue;

			const float rate = static_cast<float>(hp.currentHP) / static_cast<float>(hp.maxHP);
			/* 閾値より HP が高い味方はスキップ。*/
			if (rate > ALLY_HEAL_HP_RATE)
				continue;

			/* 水平距離で範囲外ならスキップ。*/
			Vector3 diff = other->GetPosition() - ctx.outer->GetPosition();
			diff.y = 0.0f;
			if (diff.Length() > HEAL_SEARCH_RANGE)
				continue;

			/* より HP が低い味方を優先。*/
			if (rate < lowestRate)
			{
				lowestRate = rate;
				bestTarget = other;
			}
		}

		return bestTarget;
	}


	bool NPCSupportMind::ShouldHealAlly(const NPCMindContext& ctx) const
	{
		/* 早期リターン。*/
		if (ctx.outer == nullptr)
			return false;
		/* 杖以外は味方回復しない。*/
		if (ctx.outer->GetCurrentWeapon() != WeaponType::Wand)
			return false;
		/* ダウン救助の方が優先。*/
		if (ctx.helpTarget != nullptr && ShouldRespondToHelp(ctx))
			return false;
		/* 回復対象が1人でもいれば true。*/
		return FindAllyNeedingHeal(ctx) != nullptr;
	}


	bool NPCSupportMind::ShouldGuard(const NPCMindContext& ctx) const
	{
		if (ctx.outer == nullptr || ctx.profile == nullptr || ctx.threats == nullptr)
			return false;

		const float cost = ctx.threats->GetTotalDangerCost(ctx.outer->GetPosition());
		const float enter = ctx.profile->evadeThreshold;

		/* 軽〜中程度の危険はガード。本格回避は Evade へ。*/
		return cost > enter * 0.35f && cost < enter * 0.85f;
	}


	int NPCSupportMind::CountAlliesNeedingHeal(const NPCMindContext& ctx) const
	{
		if (ctx.outer == nullptr)
			return 0;

		const char* playerNames[] = { "player1", "player2", "player3", "player4" };
		int count = 0;

		for (const char* name : playerNames)
		{
			auto* other = FindGO<nsActor::Player>(name);
			if (other == nullptr || other == ctx.outer)
				continue;
			if (other->IsDeath() || other->GetCharacterStatus().hp.currentHP <= 0)
				continue;

			const auto& hp = other->GetCharacterStatus().hp;
			if (hp.maxHP <= 0)
				continue;

			const float rate = static_cast<float>(hp.currentHP) / static_cast<float>(hp.maxHP);
			if (rate > ALLY_HEAL_HP_RATE)
				continue;

			Vector3 diff = other->GetPosition() - ctx.outer->GetPosition();
			diff.y = 0.0f;
			if (fabsf(diff.x) > HEAL_SEARCH_RANGE)
				continue;

			++count;
		}

		return count;
	}


	bool NPCSupportMind::ShouldPrioritizeHeal(const NPCMindContext& ctx) const
	{
		if (ctx.outer == nullptr || ctx.outer->GetCurrentWeapon() != WeaponType::Wand)
			return false;
		if (ShouldHealSelf(ctx))
			return true;
		const int injuredCount = CountAlliesNeedingHeal(ctx);
		if (injuredCount >= HEAL_MULTI_INJURED_COUNT)
			return true;
		auto* target = FindAllyNeedingHeal(ctx);
		if (target == nullptr)
			return false;
		const auto& hp = target->GetCharacterStatus().hp;
		if (hp.maxHP <= 0)
			return false;
		const float rate = static_cast<float>(hp.currentHP) / static_cast<float>(hp.maxHP);
		return rate <= HEAL_CRITICAL_HP_RATE;
	}
}