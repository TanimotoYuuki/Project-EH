#include "stdafx.h"
#include "Camera.h"

namespace
{
	const Vector3 CAMERA_POSITION = Vector3(0.0f, 140.0f, 300.0f);
	const Vector3 CAMERA_TARGET = Vector3(0.0f, 115.0f, 0.0f);
}

namespace nsApp
{
	bool Camera::Start()
	{
		g_camera3D->SetPosition(CAMERA_POSITION);
		g_camera3D->SetTarget(CAMERA_TARGET);

		return true;
	}
}