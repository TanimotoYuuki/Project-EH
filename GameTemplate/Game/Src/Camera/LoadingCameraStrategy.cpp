#include "stdafx.h"
#include "LoadingCameraStrategy.h"

namespace
{
	/* ロード中だけ使う固定カメラ。*/
	const Vector3 LOADING_CAMERA_POSITION = Vector3(0.0f, 60.0f, 350.0f);
	const Vector3 LOADING_CAMERA_TARGET = Vector3(0.0f, 60.0f, 0.0f);
}

namespace nsApp
{
	void LoadingCameraStrategy::Update()
	{
		/* カメラが生成されていないなら処理をしない。*/
		if (g_camera3D == nullptr)
			return;

		/* ロード中は固定カメラ。*/ 
		g_camera3D->SetPosition(LOADING_CAMERA_POSITION);
		g_camera3D->SetTarget(LOADING_CAMERA_TARGET);
	}
}