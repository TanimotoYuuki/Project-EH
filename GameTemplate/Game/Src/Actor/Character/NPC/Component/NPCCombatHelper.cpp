#include "stdafx.h"
#include "NPCCombatHelper.h"

namespace nsApp
{
	float NPCCombatHelper::ComputeDistance(const Vector3& myPos, const Vector3& targetPos, Vector3& outDiff)
	{
		/* 距離の計算。*/
		outDiff = targetPos - myPos;
		outDiff.y = 0.0f;  //! Y軸の差分は無視する。

		/* 距離の長さを計算。*/
		float dist = outDiff.Length();

		/* 正規化。*/
		if(dist > 0.001f)
			outDiff.Normalize();

		return dist;
	}


	void NPCCombatHelper::PreventClipping(nsActor::Player* body, nsActor::ICharacter* target, float currentDistance, float limitDistance)
	{
		if (currentDistance >= limitDistance || currentDistance <= 0.0f) 
			return;

		/* ターゲットとの位置の差分を元にした押し返す方向を計算。*/
		Vector3 pushDir = body->GetPosition() - target->GetPosition();
		pushDir.y = 0.0f;
		pushDir.Normalize();

		/* 押し返す距離を計算。*/
		Vector3 nextPos = body->GetPosition() + pushDir * (limitDistance - currentDistance);
		body->GetCharacterController().SetPosition(nextPos);
		body->SetPosition(nextPos);
	}


	void NPCCombatHelper::UpdateFacing(nsActor::Player* body, const Vector3& diff, bool isAttacking)
	{
		/* ターゲットの方を向く処理。*/
		if (isAttacking) {
			body->SetAngle(diff.x > 0.0f ? 90.0f : -90.0f);
			body->SetForwardVector(diff.x > 0.0f ? Vector3::Right : Vector3::Left);
		}
	}
}