#pragma once

/**
 * @file   NPCSupportMind.h
 * @brief  NPC の後頭部。救助・回復・ガード（協力行動）を担当する。
 * @author Yamaguchi Hayato。
 * @date   2026/06/22
 */

#include "Src/Actor/Character/NPC/Brain/NPCMindContext.h"

namespace nsApp
{
	namespace nsActor {
		class Player;
	}

	/**
	 * @class NPCSupportMind
	 * @brief パーティ支援思考専用。ボス攻撃・通常移動は持たない。
	 */
	class NPCSupportMind
	{
	public:
		/**
		 * @brief 救助探索範囲をセットする。
		 */
		void SetHelpSearchRange(float range);

		/**
		 * @brief 最寄りのダウン味方を探索する。
		 */
		nsActor::Player* SearchHelpTarget(const NPCMindContext& ctx) const;

		/**
		 * @brief 対象が救助が必要なダウン状態か。
		 */
		bool IsDownedAllyNeedingHelp(const NPCMindContext& ctx, nsActor::Player* target) const;

		/**
		 * @brief この NPC が救助役か（HelpPriority + パーティ番号）。
		 */
		bool ShouldRespondToHelp(const NPCMindContext& ctx) const;

		/**
		 * @brief 自分 HP が低く、自己回復すべきか（Wand 専用）。
		 */
		bool ShouldHealSelf(const NPCMindContext& ctx) const;

		/**
		 * @brief 回復が必要な味方のうち、最も HP が低い者。
		 */
		nsActor::Player* FindAllyNeedingHeal(const NPCMindContext& ctx) const;

		/**
		 * @brief 味方回復すべきか（救助中は false）。
		 */
		bool ShouldHealAlly(const NPCMindContext& ctx) const;

		/**
		 * @brief ガードすべきか（Phase3 で実装。現状はスタブ）。
		 */
		bool ShouldGuard(const NPCMindContext& ctx) const;

		/**
		 * @brief 回復が必要な味方の人数（Wand 専用）。
		 */
		int CountAlliesNeedingHeal(const NPCMindContext& ctx) const;

		/**
		 * @brief 回復を最優先すべきか（複数負傷・危急など）。
		 */
		bool ShouldPrioritizeHeal(const NPCMindContext& ctx) const;


	private:
		float m_helpSearchRange = 800.0f; //! 救助探索半径。
	};
}