#include "stdafx.h"
#include "NPCSocialEvaluator.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
	const float ALLY_AVOID_DISTANCE = 80.0f;   //! この距離より近いとペナルティ。
	const float ALLY_PENALTY_WEIGHT = 0.02f;   //! 1 単位近いごとの cost 加算。
	const float SEPARATION_RADIUS = 80.0f;     //! この距離より近い味方から離れる。
	const float MOVE_DEAD_ZONE = 0.001f;     //! ベクトル長がこれ以下なら無視する。
}

namespace nsApp
{
	namespace nsNPC
	{
		float NPCSocialEvaluator::CalcPositionPenalty(const Vector3& samplePosition, const NPCSocialContext& context)
		{
			const char* playerNames[] = { "player1", "player2", "player3", "player4" };
			float penalty = 0.0f;

			for (const char* name : playerNames)
			{
				auto* other = FindGO<nsActor::Player>(name);
				if (other == nullptr || other == context.self)
					continue;

				if (other->IsDeath() || other->GetCharacterStatus().hp.currentHP <= 0)
					continue;

				Vector3 otherPos = other->GetPosition();
				otherPos.y = 0.0f;

				const float deltaX = fabsf(samplePosition.x - otherPos.x);
				if (deltaX >= ALLY_AVOID_DISTANCE)
					continue;

				penalty += (ALLY_AVOID_DISTANCE - deltaX) * ALLY_PENALTY_WEIGHT;
			}

			return penalty;
		}


		Vector3 NPCSocialEvaluator::CalcSeparationVector(const Vector3& myPosition, const NPCSocialContext& context)
		{
			const char* playerNames[] = { "player1", "player2", "player3", "player4" };
			Vector3 separation = Vector3::Zero;

			Vector3 myPos = myPosition;
			myPos.y = 0.0f;

			for (const char* name : playerNames)
			{
				auto* other = FindGO<nsActor::Player>(name);
				if (other == nullptr || other == context.self)
					continue;

				if (other->IsDeath() || other->GetCharacterStatus().hp.currentHP <= 0)
					continue;

				Vector3 otherPos = other->GetPosition();
				otherPos.y = 0.0f;

				Vector3 away = myPos - otherPos;
				const float distSq = away.LengthSq();
				if (distSq >= SEPARATION_RADIUS * SEPARATION_RADIUS)
					continue;
				if (distSq <= MOVE_DEAD_ZONE)
					continue;

				const float dist = sqrtf(distSq);
				away /= dist;

				const float strength = (SEPARATION_RADIUS - dist) / SEPARATION_RADIUS;
				separation += away * strength;
			}

			separation.y = 0.0f;
			if (separation.LengthSq() <= MOVE_DEAD_ZONE)
				return Vector3::Zero;

			separation.Normalize();
			return separation;
		}
	}
}