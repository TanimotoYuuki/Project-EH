#include "stdafx.h"
#include "Camera.h"

namespace nsApp
{
	bool Camera::Start()
	{
		/* ƒJƒƒ‰‚ð—pˆÓ‚·‚éB*/
		g_camera3D->SetPosition(Vector3(0.0f, 150.0f, 180.0f));
		g_camera3D->SetTarget(Vector3(0.0f, 60.0f, 0.0f));

		Quaternion cameraAngle;
		cameraAngle.SetRotationDegX(45.0f);
   //	g_camera3D->RotateOriginTarget(cameraAngle);
		return true;
	}
}