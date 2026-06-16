#include "stdafx.h"
#include "BattleCameraStrategy.h"
#include <cmath>

namespace
{
	const Vector3 BATTLE_CAMERA_POSITION = Vector3(0.0f, 140.0f, 300.0f); //! �o�g���J�����̊�{�ʒu�B
	const Vector3 BATTLE_CAMERA_TARGET = Vector3(0.0f, 115.0f, 0.0f);   //! �o�g���J�����̊�{�����_�B
}

namespace nsApp
{
	void BattleCameraStrategy::Update()
	{
		if (g_camera3D == nullptr)
			return;

		Vector3 pos = BATTLE_CAMERA_POSITION;
		Vector3 target = BATTLE_CAMERA_TARGET;

		/* �V�F�C�N���̓T�C���g�I�t�Z�b�g�����Z����B*/
		if (m_shakeDuration > 0.0f)
		{
			float dt = g_gameTime->GetFrameDeltaTime();
			m_shakeDuration -= dt;
			m_shakeTimer += dt;

			float offsetX = std::sinf(m_shakeTimer * SHAKE_FREQUENCY_X) * m_shakeIntensity;
			float offsetY = std::cosf(m_shakeTimer * SHAKE_FREQUENCY_Y) * m_shakeIntensity * SHAKE_Y_RATIO;

			pos.x += offsetX;
			pos.y += offsetY;
			target.x += offsetX * TARGET_DAMP_RATIO;
		}

		g_camera3D->SetPosition(pos);
		g_camera3D->SetTarget(target);
	}


	void BattleCameraStrategy::StartShake(float duration, float intensity)
	{
		m_shakeDuration = duration;
		m_shakeIntensity = intensity;
		m_shakeTimer = 0.0f;
	}
}