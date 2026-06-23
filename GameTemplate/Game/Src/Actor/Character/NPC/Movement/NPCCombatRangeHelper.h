#pragma once

#include "Src/Actor/Character/NPC/Component/NPCBehaviorProfile.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	namespace nsNPC
	{
		/**
		 * @enum class NPCChaseRangeMode
		 * @brief NPCの追跡範囲モードを表す列挙型。
		 */
		enum class NPCChaseRangeMode : uint8_t
		{
			Hold,		 //! 追跡範囲を維持するモード。
			Approaching, //! 追跡範囲に近づくモード。
			Retreating   //! 追跡範囲から離れるモード。
		};

		/**
		 * @brief 理想距離からこれだけ近いと後退する距離を計算する関数。
		 * @param profile NPCの行動プロファイル。
		 * @return 理想距離からこれだけ近いと後退する距離。
		 */
		inline float GetRetreatDistance(const NPCBehaviorProfile& profile, float range)
		{
			return profile.preferredAttackRange - range;
		}

		/**
		 * @brief NPCが遠距離武器を使用しているかどうかを判定する関数。
		 * @param profile NPCの行動プロファイル。
		 * @return 遠距離武器を使用している場合はtrue、そうでない場合はfalse。
		 */
		inline bool IsRangedWeapon(const NPCBehaviorProfile& profile, float attackRange)
		{
			return profile.preferredAttackRange >= attackRange;
		}

		/**
		 * @brief NPCが後退すべきかどうかを判定する関数。
		 * @param distance NPCとターゲットの距離。
		 * @param profile NPCの行動プロファイル。
		 * @return 後退すべき場合はtrue、そうでない場合はfalse。
		 */
		inline bool ShouldRetreat(float distance, const NPCBehaviorProfile& profile)
		{
			return IsRangedWeapon(profile, distance) && distance < GetRetreatDistance(profile, distance);
		}

		/**
		 * @brief NPCが攻撃帯にいるかどうかを判定する関数。
		 * @param distance NPCとターゲットの距離。
		 * @param profile NPCの行動プロファイル。
		 * @return 
		 */
		inline bool IsInAttackBand(float distance, const NPCBehaviorProfile& profile)
		{
			return distance <= profile.preferredAttackRange;
		}

		/**
		 * @brief NPCが追跡帯にいるかどうかを判定する関数。
		 * @param myPos NPCの位置。
		 * @param targetPos ターゲットの位置。
		 * @return 追跡帯にいる場合はtrue、そうでない場合はfalse。
		 */
		float CalcRetreatStickX(const Vector3& myPos, const Vector3& targetPos);

		/**
		 * @brief NPCの武器に応じた近接攻撃の最大射程を取得する関数。
		 * @param weapon NPCの武器の種類。
		 * @return NPCの武器に応じた近接攻撃の最大射程。
		 */
		float GetMeleeMaxAttackRange(WeaponType weapon);

		/**
		 * @brief NPCが攻撃を開始できるかどうかを判定する関数。
		 * @param distance NPCとターゲットの距離。
		 * @param weapon NPCの武器の種類。
		 * @param profile NPCの行動プロファイル。
		 * @return 攻撃を開始できる場合はtrue、そうでない場合はfalse。
		 */
		bool CanBeginAttack(float distance, WeaponType weapon, const NPCBehaviorProfile& profile);

		/**
		 * @brief NPCがターゲットから後退する方向を計算する関数。
		 * @param myPos NPCの位置。
		 * @param targetPos ターゲットの位置。
		 * @return NPCがターゲットから後退する方向の単位ベクトル。距離が近すぎる場合はゼロベクトル。
		 */
		Vector3 CalcRetreatDirection(const Vector3& myPos, const Vector3& targetPos);

		/**
		 * @brief 遠距離の接近開始距離を返す。
		 * @param profile NPCの行動プロファイル。
		 * @return 接近開始距離。
		 */
		inline float GetRangedChaseEnterDistance(const NPCBehaviorProfile& profile)
		{
			return profile.preferredAttackRange + profile.rangedChaseEnterOffset;
		}

		/**
		 * @brief 遠距離の接近停止距離を返す。
		 * @param profile NPCの行動プロファイル。
		 * @return 接近停止距離。
		 */
		inline float GetRangedChaseExitDistance(const NPCBehaviorProfile& profile)
		{
			return profile.preferredAttackRange - profile.rangedChaseExitOffset;
		}

		/**
		 * @brief 遠距離の後退開始距離を返す。
		 * @param profile NPCの行動プロファイル。
		 * @return 後退開始距離。
		 */
		inline float GetRangedRetreatEnterDistance(const NPCBehaviorProfile& profile)
		{
			return profile.preferredAttackRange - profile.rangedRetreatEnterOffset;
		}

		/**
		 * @brief 遠距離の後退停止距離を返す。
		 * @param profile NPCの行動プロファイル。
		 * @return 後退停止距離。
		 */
		inline float GetRangedRetreatExitDistance(const NPCBehaviorProfile& profile)
		{
			return profile.preferredAttackRange - profile.rangedRetreatExitOffset;
		}

		/**
		 * @brief ヒステリシス付きで後退すべきか判定する。
		 * @param distance NPCとターゲットの距離。
		 * @param profile NPCの行動プロファイル。
		 * @param isRetreating 後退中フラグ（入出力）。
		 * @return 後退すべきなら true。
		 */
		bool ShouldRetreatWithHysteresis(float distance, const NPCBehaviorProfile& profile, bool& isRetreating);

		/**
		 * @brief ヒステリシス付きで接近すべきか判定する。
		 * @param distance NPCとターゲットの距離。
		 * @param profile NPCの行動プロファイル。
		 * @param isApproaching 接近中フラグ（入出力）。
		 * @return 接近すべきなら true。
		 */
		bool ShouldApproachWithHysteresis(float distance, const NPCBehaviorProfile& profile, bool& isApproaching);


		bool IsRangedWeaponByProfile(const NPCBehaviorProfile& profile);
		float GetRetreatDistanceByProfile(const NPCBehaviorProfile& profile);
		float GetRangedAttackMaxDistanceByProfile(const NPCBehaviorProfile& profile);
	}
}