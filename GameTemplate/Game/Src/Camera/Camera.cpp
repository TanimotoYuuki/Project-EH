#include "stdafx.h"
#include "Camera.h"

namespace
{
	const Vector3 CAMERA_POSITION = Vector3(0.0f, 150.0f, 180.0f);
	const Vector3 CAMERA_TARGET = Vector3(0.0f, 60.0f, 0.0f);
	const auto CAMERA_ANGLE = 45.0f;
}

namespace nsApp
{
	bool Camera::Start()
	{
		/* ƒJƒƒ‰‚ð—pˆÓ‚·‚éB*/
		g_camera3D->SetPosition(CAMERA_POSITION);
		g_camera3D->SetTarget(CAMERA_TARGET);

		Quaternion cameraAngle;
		cameraAngle.SetRotationDegX(CAMERA_ANGLE);
		return true;
	}
}