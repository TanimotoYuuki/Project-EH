#include "stdafx.h"
#include "Camera.h"

#include "Src/Camera/ICameraStrategy.h"
#include "Src/Camera/BattleCameraStrategy.h"  // �� .cpp ���ŃC���N���[�h

namespace nsApp
{
	bool Camera::Start()
	{
		/* ������Ԃ͐퓬�p�J�����ɂ��Ă����B*/
		if (m_strategy == nullptr)
			ChangeToBattle();

		return true;
	}


	void Camera::Update()
	{
		if (m_strategy == nullptr)
			return;

		m_strategy->Update();
	}


	void Camera::ChangeToLoading()
	{
		/* �퓬�J�����̐��|�C���^���N���A���Ă���Strategy��؂�ւ���B*/
		m_battleCameraStrategy = nullptr;
		ChangeStrategy(std::make_unique<LoadingCameraStrategy>());
	}


	void Camera::ChangeToBattle()
	{
		/* ���|�C���^��ێ����Ă���unique_ptr�̏��L�����ڏ�����B*/
		auto strategy = std::make_unique<BattleCameraStrategy>();
		m_battleCameraStrategy = strategy.get();
		ChangeStrategy(std::move(strategy));
	}


	void Camera::StartShake(float duration, float intensity)
	{
		if (m_battleCameraStrategy != nullptr)
			m_battleCameraStrategy->StartShake(duration, intensity);
	}


	void Camera::ChangeStrategy(std::unique_ptr<ICameraStrategy> strategy)
	{
		m_strategy = std::move(strategy);

		/* �؂�ւ����u�Ԃɑ������f����B*/
		if (m_strategy != nullptr)
			m_strategy->Update();
	}
}