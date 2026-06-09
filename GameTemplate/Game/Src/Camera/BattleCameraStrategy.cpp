#include "stdafx.h"
#include "BattleCameraStrategy.h"

namespace
{
	const Vector3 BATTLE_CAMERA_POSITION = Vector3(0.0f, 140.0f, 300.0f);
	const Vector3 BATTLE_CAMERA_TARGET = Vector3(0.0f, 115.0f, 0.0f);
}

namespace nsApp
{
	void BattleCameraStrategy::Update()
	{
		/* カメラが存在しないのなら処理をしない。*/
		if (g_camera3D == nullptr)
			return;

		/* カメラの位置とターゲットを設定する。*/
		g_camera3D->SetPosition(BATTLE_CAMERA_POSITION);
		g_camera3D->SetTarget(BATTLE_CAMERA_TARGET);
	}
}