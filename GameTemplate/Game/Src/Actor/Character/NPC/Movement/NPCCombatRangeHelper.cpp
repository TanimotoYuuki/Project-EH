#include "stdafx.h"
#include "NPCCombatRangeHelper.h"

namespace
{
	const float RETREAT_HYSTERESIS = 40.0f;
	const float RETREAT_STICK_DEAD_ZONE = 5.0f;
	const float RETREAT_RANGE_MARGIN = 50.0f;
	const float RANGED_ATTACK_RANGE_MARGIN = 30.0f;
	const float RANGED_ATTACK_RANGE = 250.0f;
	const float CHASE_TRANSITION_DISTANCE = 300.0f;
	const float MELEE_ATTACK_RANGE_SWORD = 95.0f;
	const float MELEE_ATTACK_RANGE_HAMMER = 110.0f;
}

namespace nsApp
{
	namespace nsNPC
	{
		float CalcRetreatStickX(const Vector3& myPos, const Vector3& targetPos)
		{
			/* ターゲットから見て自分がどちら側にいるかを計算する。ターゲットの正面を0、右を正、左を負とする。*/
			const float deltaX = myPos.x - targetPos.x;

			/* ある程度近い場合はスティックを倒さない。*/
			if (fabsf(deltaX) < RETREAT_STICK_DEAD_ZONE)
				return 0.0f;

			/* ターゲットの右側にいる場合はスティックを右に倒し、左側にいる場合はスティックを左に倒す。*/
			return (deltaX > 0.0f) ? 1.0f : -1.0f;
		}


		float GetMeleeMaxAttackRange(WeaponType weapon)
		{
			/* 武器の種類に応じた近接攻撃の最大射程を返す。*/
			if (weapon == WeaponType::Hammer)
				return MELEE_ATTACK_RANGE_HAMMER;

			/* デフォルトは剣の射程を返す。*/
			return MELEE_ATTACK_RANGE_SWORD;
		}


		bool CanBeginAttack(float distance, WeaponType weapon, const NPCBehaviorProfile& profile)
		{
			/* 遠距離武器を使用している場合は、距離が攻撃範囲内であるかどうかを判定する。*/
			if (IsRangedWeapon(profile,RANGED_ATTACK_RANGE))
			{
				/* 遠距離攻撃の開始条件は、距離が後退ライン以上で、かつ攻撃範囲の上限以下であること。*/
				const float retreatLine = GetRetreatDistance(profile, RETREAT_RANGE_MARGIN);
				return distance >= retreatLine && distance <= profile.preferredAttackRange + RANGED_ATTACK_RANGE_MARGIN;
			}

			/* 近接武器を使用している場合は、距離が近接攻撃の最大射程以下であるかどうかを判定する。*/
			return distance <= GetMeleeMaxAttackRange(weapon);
		}


		Vector3 CalcRetreatDirection(const Vector3& myPos, const Vector3& targetPos)
		{
			/* ターゲットから自分へのベクトルを計算し、y成分を0にして水平面上のベクトルにする。*/
			Vector3 away = myPos - targetPos;
			away.y = 0.0f;

			/* ベクトルの長さが一定以下の場合は、ターゲットに近すぎるとみなしてスティックを倒さない。*/
			if (away.LengthSq() <= RETREAT_STICK_DEAD_ZONE * RETREAT_STICK_DEAD_ZONE)
				return Vector3::Zero;

			/* ベクトルを正規化して、ターゲットから後退する方向の単位ベクトルを返す。*/
			away.Normalize();
			return away;
		}


		bool ShouldRetreatWithHysteresis(float distance, const NPCBehaviorProfile& profile, bool& isRetreating)
		{
			/* 遠距離武器を使用していない場合は、後退しない。*/
			if (!IsRangedWeapon(profile, RANGED_ATTACK_RANGE))
				return false;

			/* ヒステリシスを使用して、後退開始距離と停止距離を設定する。
			   これにより、NPCが距離の境界で頻繁に後退と停止を繰り返すのを防ぐ。*/
			const float enterDistance = GetRangedRetreatEnterDistance(profile);
			const float exitDistance = GetRangedRetreatExitDistance(profile);

			/* すでに後退している場合は、距離が停止距離以上になるまで後退を続ける。*/
			if (isRetreating)
			{
				/* 後退中は退出距離に達するまで継続する。*/
				if (distance >= exitDistance)
				{
					isRetreating = false;
					return false;
				}

				return true;
			}

			/* まだ後退していなければ、進入距離を下回ったら後退開始。*/
			if (distance < enterDistance)
			{
				isRetreating = true;
				return true;
			}

			return false;
		}


		bool ShouldApproachWithHysteresis(float distance, const NPCBehaviorProfile& profile, bool& isApproaching)
		{
			/* 遠距離武器を使用していない場合は、接近しない。*/
			if (!IsRangedWeapon(profile, RANGED_ATTACK_RANGE))
				return false;

			/* ヒステリシスを使用して、接近開始距離と停止距離を設定する。
			   これにより、NPCが距離の境界で頻繁に接近と停止を繰り返すのを防ぐ。*/
			const float enterDistance = GetRangedChaseEnterDistance(profile);
			const float exitDistance = GetRangedChaseExitDistance(profile);

			/* すでに接近している場合は、距離が停止距離以下になるまで接近を続ける。*/
			if (isApproaching)
			{
				/* 接近中は停止距離に達するまで継続する。*/
				if (distance <= exitDistance)
				{
					isApproaching = false;
					return false;
				}

				return true;
			}

			/* まだ接近していなければ、開始距離を超えたら接近開始。*/
			if (distance > enterDistance)
			{
				isApproaching = true;
				return true;
			}

			return false;
		}


		bool IsRangedWeaponByProfile(const NPCBehaviorProfile& profile)
		{
			/* 遠距離武器かどうかを判定する。遠距離武器の定義は、攻撃範囲が一定以上であること。*/
			return IsRangedWeapon(profile, RANGED_ATTACK_RANGE);
		}


		float GetRetreatDistanceByProfile(const NPCBehaviorProfile& profile)
		{
			/* 遠距離武器の後退距離を計算する。後退距離は、NPCの好む攻撃範囲にマージンを加えた値である。*/
			return GetRetreatDistance(profile, RETREAT_RANGE_MARGIN);
		}


		float GetRangedAttackMaxDistanceByProfile(const NPCBehaviorProfile& profile)
		{
			/* 遠距離武器の最大攻撃距離を計算する。最大攻撃距離は、NPCの好む攻撃範囲にマージンを加えた値である。*/
			return profile.preferredAttackRange + RANGED_ATTACK_RANGE_MARGIN;
		}
	}
}