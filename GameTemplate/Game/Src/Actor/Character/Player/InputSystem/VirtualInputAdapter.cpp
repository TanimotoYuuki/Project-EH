#include "stdafx.h"
#include "IInputDevice.h"
#include "VirtualInputAdapter.h"

namespace
{
	const auto INPUT_VOLUME = 0.0f; //! 入力量。
}

namespace nsApp
{
	void VirtualInputAdapter::BeginFlame()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		/* 現在の入力情報を前フレーム入力として一括辞退。*/
		m_previousButtons = m_currentButtons;

		for (auto it = m_buttonHoldFrames.begin(); it != m_buttonHoldFrames.end();)
		{
			/* ボタンの保持フレーム数が0より大きい場合は、保持フレーム数をデクリメントし、現在の入力状態をtrueに設定。*/
			if (it->second > 0)
			{
				it->second--;
				m_currentButtons[it->first] = true;
				++it;
			}

			else
			{
				m_currentButtons[it->first] = false;
				it = m_buttonHoldFrames.erase(it);
			}
		}
	}

	void VirtualInputAdapter::Reset()
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		/* 入力情報の初期化。*/
		/* 現在の入力状態。*/
		m_currentButtons.clear();
		/* 前フレームの入力状態。*/
		m_previousButtons.clear();
		/* 入力の保持フレーム数。*/
		m_buttonHoldFrames.clear();
		/* スティックの値。*/
		/* X軸。*/
		m_stickX = INPUT_VOLUME;
		/* Y軸。*/
		m_stickY = INPUT_VOLUME;
	}

	bool VirtualInputAdapter::IsTrigger(nsK2EngineLow::EnButton button) const
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		/* 現在の入力状態を取得。*/
		bool current = false;
		auto itCurrent = m_currentButtons.find(static_cast<int>(button));
		if (itCurrent != m_currentButtons.end())
		{
			current = itCurrent->second;
		}

		bool previos = false;
		auto itPrevious = m_previousButtons.find(static_cast<int>(button));
		if (itPrevious != m_previousButtons.end())
		{
			previos = itPrevious->second;
		}

		/* トリガー入力は、現在の入力が押されていて、前フレームの入力が押されていない場合にtrueを返す。*/
		return current && !previos;
	}
}