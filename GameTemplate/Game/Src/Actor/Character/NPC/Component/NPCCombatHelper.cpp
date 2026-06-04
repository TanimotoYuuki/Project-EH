#include "stdafx.h"
#include "NPCCombatHelper.h"

namespace
{
	const auto OUT_DIR_Y = 0.0f;        //! Y軸の差分を無視するための定数。
	const auto ZERO_THRESHOLD = 0.001f; //! 距離が近すぎるとみなすための閾値。
	const auto MAX_CLIPPING = 90.0f;    //! クリッピング防止のための最大角度。
	const auto MIN_CLIPPING = 0.0f;     //! クリッピング防止のための最小角度。
}

namespace nsApp
{
	float NPCCombatHelper::ComputeDistance(const Vector3& myPos, const Vector3& targetPos, Vector3& outDiff)
	{
		/* 距離の計算。*/
		outDiff = targetPos - myPos;
		outDiff.y = OUT_DIR_Y;

		/* 計算した距離を流さベクトルとして置き換える。*/
		float dist = outDiff.Length();

		/* 正規化。*/
		if (dist > ZERO_THRESHOLD)
			outDiff.Normalize();
		/* 距離が近すぎる場合は、ベクトルをゼロにする。*/
		else
			outDiff = Vector3::Zero;

		return dist;
	}


	void NPCCombatHelper::PreventClipping(nsActor::Player* body, nsActor::ICharacter* target, float currentDistance, float limitDistance)
	{
		/* クリッピング防止の処理。*/
		if (body == nullptr || target == nullptr)
			return;

		/* クリッピング防止の処理。*/
		if (currentDistance >= limitDistance || currentDistance <= 0.001f)
			return;

		/* クリッピング防止の処理。*/
		Vector3 pushDir = body->GetPosition() - target->GetPosition();
		pushDir.y = 0.0f;

		/* 距離が近すぎる場合は、ベクトルをゼロにする。*/
		if (pushDir.LengthSq() <= ZERO_THRESHOLD)
			return;
		/* ベクトルを正規化。*/
		pushDir.Normalize();

		/* クリッピング防止の処理。*/
		Vector3 nextPos = body->GetPosition() + pushDir * (limitDistance - currentDistance);
		/* キャラクターコントローラーとキャラクターポジションの両方を更新。*/
		body->GetCharacterController().SetPosition(nextPos);
		/* キャラクターポジションの更新。*/
		body->SetPosition(nextPos);
	}


	void NPCCombatHelper::UpdateFacing(nsActor::Player* body, const Vector3& diff, bool isAttacking)
	{
		/* 向きの更新の処理。*/
		if (body == nullptr)
			return;

		/* 攻撃中の場合は、向きの更新がより迅速に行われる。*/
		if (isAttacking)
		{
			body->SetAngle(diff.x > MIN_CLIPPING ? MAX_CLIPPING : -MAX_CLIPPING);
			body->SetForwardVector(diff.x > MIN_CLIPPING ? Vector3::Right : Vector3::Left);
		}
	}
}
