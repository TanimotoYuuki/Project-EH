#pragma once

/**
 * @file   NPCLocomotionMind.h
 * @brief  NPC の右脳。ターゲット探索・回避判定・回避方向を担当する。
 * @author Yamaguchi Hayato。
 * @date   2026/06/22
 */

#include "Src/Actor/Character/Common/ICharacter.h"
#include "Src/Actor/Character/NPC/Brain/NPCMindContext.h"
#include "Src/Actor/Character/NPC/Navigation/AvoidPathResult.h"

namespace nsApp
{
	namespace nsActor {
		class Boss;
	}

	/**
	 * @class NPCLocomotionMind
	 * @brief 移動・回避思考専用。攻撃・救助は持たない。
	 */
	class NPCLocomotionMind
	{
	public:
		/**
		 * @brief 戦闘ターゲット（ボス）を探索する。
		 * @return 有効なボス。いなければ nullptr。
		 */
		nsActor::ICharacter* SearchTarget();

		/**
		 * @brief 現在位置が危険か（Evade ステートへ遷移すべきか）。
		 * @note  反応遅延タイマーを内部で更新するため non-const。
		 */
		bool ShouldEvade(NPCMindContext& ctx);

		/**
		 * @brief 回避移動方向（SimpleAvoidPathfinder へ委譲）。
		 */
		nsNPC::AvoidPathResult GetEvadeDirection(const NPCMindContext& ctx) const;

		/**
		 * @brief 致命圏か（タイマーを消費せず参照のみ）。回復可否の判定用。
		 */
		bool IsDangerous(const NPCMindContext& ctx) const;


	private:
		/**
		 * @brief 回避反応遅延タイマーを更新し、現在の遅延時間を返す。
		 * @param profile NPC の行動プロファイル。遅延時間の範囲をここから取得する。
		 * @return 現在の回避反応遅延時間。0 なら回避可能。正の値ならまだ待ち時間が残っている。
		 */
		int CalcEvadeReactionDelay(const NPCBehaviorProfile& profile) const;


	private:
		nsActor::Boss* m_bossTarget = nullptr;  //! キャッシュしたボス参照。

		int m_evadeReactionTimer = 0;			//! 回避反応待ち（0 で判定可能）。
		int m_evadeMinTimer = 0;				//! 回避最低継続フレーム。

		bool m_evadeDelayConsumed = false;		//! 回避待ちを1回処理したか。
		bool m_isEvading = false;				//! 回避中フラグ（退出ヒステリシス用）。
	};
}