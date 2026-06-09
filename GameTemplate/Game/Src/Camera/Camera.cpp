#include "stdafx.h"
#include "Camera.h"

#include "Src/Camera/ICameraStrategy.h"

namespace nsApp
{
	bool Camera::Start()
	{
		/* 初期状態は戦闘用カメラにしておく。*/
		if (m_strategy == nullptr)
			ChangeToBattle();

		return true;
	}


	void Camera::Update()
	{
		/* 戦闘用カメラもロード用カメラも、両方ともUpdate()を呼び出す必要がある。 */
		if (m_strategy == nullptr)
			return;

		/* 戦闘用カメラもロード用カメラも、両方ともUpdate()を呼び出す必要がある。 */
		m_strategy->Update();
	}


	void Camera::ChangeToLoading()
	{
		ChangeStrategy(std::make_unique<LoadingCameraStrategy>());
	}


	void Camera::ChangeToBattle()
	{
		ChangeStrategy(std::make_unique<BattleCameraStrategy>());
	}


	void Camera::ChangeStrategy(std::unique_ptr<ICameraStrategy> strategy)
	{
		/* 新しいStrategyに切り替える。*/
		m_strategy = std::move(strategy);

		/* 切り替えた瞬間に即反映する。*/
		if (m_strategy != nullptr)
			m_strategy->Update();
	}
}