#include "stdafx.h"
#include "NPCMovementController.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

namespace
{
	const float MOVE_DEAD_ZONE = 0.001f; //! 正規化前の安全閾値。
}

namespace nsApp
{
	void NPCMovementController::Apply(VirtualInputAdapter* vInput, const NPCMovementIntent& intent)
	{
		/* 早期リターン。*/
		if (vInput == nullptr)
			return;

		/* 移動入力がない場合は停止する。*/
		if (intent.stop || intent.direction.LengthSq() <= MOVE_DEAD_ZONE)
		{
			/* 移動入力をクリアして停止する。*/
			Stop(vInput);
			return;
		}

		/* 移動入力を正規化してセットする。*/
		Vector3 dir = intent.direction;
		dir.y = 0.0f;

		/* 移動入力がデッドゾーン内の場合は停止する。*/
		if (fabsf(dir.x) <= MOVE_DEAD_ZONE)
		{
			Stop(vInput);
			return;
		}

		/* 正規化。*/
		dir.Normalize();

		/* 移動入力をセットする。*/
		const float signX = (dir.x > 0.0f) ? 1.0f : -1.0f;
		vInput->SetLStick(dir.x, dir.z);
		vInput->SetButton(enButtonLB1, intent.useRun);
	}


	void NPCMovementController::Stop(VirtualInputAdapter* vInput)
	{
		/* 早期リターン。*/
		if (vInput == nullptr)
			return;

		/* 移動入力をクリアする。*/
		vInput->SetLStick(0.0f, 0.0f);
		vInput->SetButton(enButtonLB1, false);
	}


	NPCMovementIntent NPCMovementController::MakeMoveIntent(const Vector3& direction, bool useRun)
	{
		/* 移動意図を作成する。*/
		NPCMovementIntent intent{};
		intent.direction = direction;
		intent.useRun = useRun;
		intent.stop = false;
		return intent;
	}
}