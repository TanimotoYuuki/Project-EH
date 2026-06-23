#include "stdafx.h"
#include "SimpleAvoidPathfinder.h"
#include "Src/Actor/Character/NPC/Social/NPCSocialEvaluator.h"
#include "Src/Actor/Character/Boss/Boss.h"

namespace
{
	const auto SAMPLE_DISTANCE = 80.0f;        //! 各方向の先をどれだけ進めて評価するか。
	const auto MOVE_DEAD_ZONE = 0.001f;        //! 移動方向の長さがこの値以下なら無効とみなす。
	const auto COST_TIE_EPSILON = 0.0001f;     //! 同コスト判定の誤差。
	const auto COST_STABILITY_EPSILON = 0.02f; //! この範囲なら「ほぼ同じ危険度」とみなす。
	const auto DIRECTION_SIMILARITY = 0.85f;   //! 前回方向とこれ以上似ていれば維持する。
	const auto MAX_PARTY_SLOTS = 4;            //! 前回方向を保持するパーティ枠数。

	Vector3 s_lastDirection[MAX_PARTY_SLOTS];  //! パーティごとの前回回避方向。
	bool s_hasLastDirection[MAX_PARTY_SLOTS] = {}; //! 前回方向が有効か。
}

namespace nsApp
{
	namespace nsNPC
	{
		AvoidPathResult SimpleAvoidPathfinder::Compute(const Vector3& currentPosition, const std::vector<IThreatZone*>& zones, int partyIndex, const NPCSocialContext& socialContext)
		{
			AvoidPathResult result;
			if (zones.empty())
				return result;

			const int partySlot = (partyIndex < 0) ? 0 : ((partyIndex >= MAX_PARTY_SLOTS) ? (MAX_PARTY_SLOTS - 1) : partyIndex);

			auto CalcSampleCost = [&](const Vector3& dir) -> float
				{
					Vector3 samplePos = currentPosition + dir * SAMPLE_DISTANCE;
					samplePos.y = 0.0f;

					float cost = 0.0f;
					for (IThreatZone* zone : zones)
					{
						if (zone == nullptr || !zone->IsActive())
							continue;
						cost += zone->GetDangerCost(samplePos);
					}
					cost += NPCSocialEvaluator::CalcPositionPenalty(samplePos, socialContext);
					return cost;
				};

			const Vector3 leftDir(-1.0f, 0.0f, 0.0f);
			const Vector3 rightDir(1.0f, 0.0f, 0.0f);

			const float leftCost = CalcSampleCost(leftDir);
			const float rightCost = CalcSampleCost(rightDir);

			Vector3 awayDir = Vector3::Zero;
			float awayCost = FLT_MAX;
			bool hasAway = false;

			auto* boss = FindGO<nsActor::Boss>("boss");
			if (boss != nullptr)
			{
				awayDir = currentPosition - boss->GetPosition();
				awayDir.y = 0.0f;

				if (awayDir.LengthSq() > MOVE_DEAD_ZONE)
				{
					awayDir.Normalize();
					awayCost = CalcSampleCost(awayDir);
					hasAway = true;
				}
			}

			/* 最低コストを求める。*/
			float bestCost = leftCost;
			if (rightCost < bestCost)
				bestCost = rightCost;
			if (hasAway && awayCost < bestCost)
				bestCost = awayCost;

			auto IsNearBestCost = [&](float cost) -> bool
				{
					return cost <= bestCost + COST_STABILITY_EPSILON;
				};

			Vector3 bestDirection = Vector3::Zero;

			/* ほぼ同コストなら、ボスから離れる方向を優先する。*/
			if (hasAway && IsNearBestCost(awayCost))
				bestDirection = awayDir;

			/* 前回方向に近い候補があれば維持する。*/
			if (bestDirection.LengthSq() <= MOVE_DEAD_ZONE && s_hasLastDirection[partySlot])
			{
				const Vector3 lastDir = s_lastDirection[partySlot];

				auto TryPreferSimilar = [&](const Vector3& candidateDir, float candidateCost) -> void
					{
						if (!IsNearBestCost(candidateCost))
							return;

						Vector3 normalizedCandidate = candidateDir;
						normalizedCandidate.Normalize();

						const float dot = lastDir.x * normalizedCandidate.x + lastDir.z * normalizedCandidate.z;
						if (dot >= DIRECTION_SIMILARITY)
							bestDirection = normalizedCandidate;
					};

				TryPreferSimilar(leftDir, leftCost);
				TryPreferSimilar(rightDir, rightCost);
				if (hasAway)
					TryPreferSimilar(awayDir, awayCost);
			}

			/* まだ決まらなければ最低コストの候補を選ぶ。*/
			if (bestDirection.LengthSq() <= MOVE_DEAD_ZONE)
			{
				if (IsNearBestCost(leftCost))
					bestDirection = leftDir;
				else if (IsNearBestCost(rightCost))
					bestDirection = rightDir;
				else if (hasAway && IsNearBestCost(awayCost))
					bestDirection = awayDir;
			}

			/* 最終手段：パーティ番号で左右を固定する。*/
			if (bestDirection.LengthSq() <= MOVE_DEAD_ZONE)
				bestDirection = (partyIndex % 2 == 0) ? leftDir : rightDir;

			if (bestDirection.LengthSq() <= MOVE_DEAD_ZONE)
				return result;

			bestDirection.Normalize();
			s_lastDirection[partySlot] = bestDirection;
			s_hasLastDirection[partySlot] = true;

			result.direction = bestDirection;
			result.hasValidDirection = true;
			return result;
		}
	}
}