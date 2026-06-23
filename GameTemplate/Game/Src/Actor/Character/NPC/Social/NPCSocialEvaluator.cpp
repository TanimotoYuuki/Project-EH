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
			/* プレイヤー全員に対して、サンプル位置が近いほどペナルティを加算する。*/
			const char* playerNames[] = { "player1", "player2", "player3", "player4" };
			float penalty = 0.0f;

			/* サンプル位置の y 座標を無視するために 0 にする。*/
			for (const char* name : playerNames)
			{
				/* プレイヤーが存在しない、または自分自身ならスキップする。*/
				auto* other = FindGO<nsActor::Player>(name);
				if (other == nullptr || other == context.self)
					continue;

				/* 死亡している、または HP が 0 以下ならスキップする。*/
				if (other->IsDeath() || other->GetCharacterStatus().hp.currentHP <= 0)
					continue;

				/* サンプル位置とプレイヤーの x 座標の差を計算する。*/
				Vector3 otherPos = other->GetPosition();
				otherPos.y = 0.0f;

				/* サンプル位置とプレイヤーの x 座標の差が ALLY_AVOID_DISTANCE 以上ならスキップする。*/
				const float deltaX = fabsf(samplePosition.x - otherPos.x);
				if (deltaX >= ALLY_AVOID_DISTANCE)
					continue;

				/* サンプル位置とプレイヤーの x 座標の差が ALLY_AVOID_DISTANCE 未満ならペナルティを加算する。*/
				penalty += (ALLY_AVOID_DISTANCE - deltaX) * ALLY_PENALTY_WEIGHT;
			}

			return penalty;
		}


		Vector3 NPCSocialEvaluator::CalcSeparationVector(const Vector3& myPosition, const NPCSocialContext& context)
		{
			/* プレイヤー全員に対して、近いほど離れる方向を計算する。*/
			const char* playerNames[] = { "player1", "player2", "player3", "player4" };
			Vector3 separation = Vector3::Zero;

			/* 自分の位置の y 座標を無視するために 0 にする。*/
			Vector3 myPos = myPosition;
			myPos.y = 0.0f;

			/* プレイヤー全員に対して、近いほど離れる方向を加算する。*/
			for (const char* name : playerNames)
			{
				/* プレイヤーが存在しない、または自分自身ならスキップする。*/
				auto* other = FindGO<nsActor::Player>(name);
				if (other == nullptr || other == context.self)
					continue;

				/* 死亡している、または HP が 0 以下ならスキップする。*/
				if (other->IsDeath() || other->GetCharacterStatus().hp.currentHP <= 0)
					continue;

				/* プレイヤーの位置の y 座標を無視するために 0 にする。*/
				Vector3 otherPos = other->GetPosition();
				otherPos.y = 0.0f;

				/* 自分の位置とプレイヤーの位置の距離を計算する。*/
				Vector3 away = myPos - otherPos;
				const float distSq = away.LengthSq();

				/* SEPARATION_RADIUS 以上ならスキップする。*/
				if (distSq >= SEPARATION_RADIUS * SEPARATION_RADIUS)
					continue;
				if (distSq <= MOVE_DEAD_ZONE)
					continue;

				/* 離れる方向を正規化する。*/
				const float dist = sqrtf(distSq);
				away /= dist;

				/* SEPARATION_RADIUS からの距離に応じて強さを計算する。*/
				const float strength = (SEPARATION_RADIUS - dist) / SEPARATION_RADIUS;
				separation += away * strength;
			}

			/* y 座標を無視するために 0 にする。*/
			separation.y = 0.0f;
			if (separation.LengthSq() <= MOVE_DEAD_ZONE)
				return Vector3::Zero;

			/* 正規化して返す。*/
			separation.Normalize();
			return separation;
		}
	}
}