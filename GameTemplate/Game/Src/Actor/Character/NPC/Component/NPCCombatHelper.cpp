#include "stdafx.h"
#include "NPCCombatHelper.h"

namespace nsApp
{
	float NPCCombatHelper::ComputeDistance(const Vector3& myPos, const Vector3& targetPos, Vector3& outDiff)
	{
		outDiff = targetPos - myPos;
		outDiff.y = 0.0f;

		float dist = outDiff.Length();

		if (dist > 0.001f)
			outDiff.Normalize();
		else
			outDiff = Vector3::Zero;

		return dist;
	}

	void NPCCombatHelper::PreventClipping(nsActor::Player* body, nsActor::ICharacter* target, float currentDistance, float limitDistance)
	{
		if (body == nullptr || target == nullptr)
			return;

		if (currentDistance >= limitDistance || currentDistance <= 0.001f)
			return;

		Vector3 pushDir = body->GetPosition() - target->GetPosition();
		pushDir.y = 0.0f;

		if (pushDir.LengthSq() <= 0.001f)
			return;

		pushDir.Normalize();

		Vector3 nextPos = body->GetPosition() + pushDir * (limitDistance - currentDistance);
		body->GetCharacterController().SetPosition(nextPos);
		body->SetPosition(nextPos);
	}

	void NPCCombatHelper::UpdateFacing(nsActor::Player* body, const Vector3& diff, bool isAttacking)
	{
		if (body == nullptr)
			return;

		if (isAttacking)
		{
			body->SetAngle(diff.x > 0.0f ? 90.0f : -90.0f);
			body->SetForwardVector(diff.x > 0.0f ? Vector3::Right : Vector3::Left);
		}
	}
}
