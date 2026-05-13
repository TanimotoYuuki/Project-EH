#include "stdafx.h"
#include "IInputDevice.h"
#include "VirtualInputAdapter.h"

namespace
{
	const auto INPUT_VOLUME = 0.0f;  //! 入力量。
}

namespace nsApp
{
	void VirtualInputAdapter::BeginFlame()
	{
		/* 現在の入力情報を前フレーム入力として一括辞退。*/
		m_previousButtons = m_currentButtons;
	}


	void VirtualInputAdapter::Reset()
	{
		/* 入力情報の初期化。*/
		/* 現在の入力状態。*/
		m_currentButtons.clear();  
		/* 前フレームの入力状態。*/
		m_previousButtons.clear();

		/* スティックの値。*/
		/* X軸。*/
		m_stickX = INPUT_VOLUME;
		/* Y軸。*/
		m_stickY = INPUT_VOLUME;
	}


	bool VirtualInputAdapter::IsTrigger(nsK2EngineLow::EnButton button) const
	{
		/* 現在の入力状態を取得。*/
		bool current = IsPress(button);
		/* 前フレームの入力状態を取得。*/
		auto inputIterator = m_previousButtons.find(static_cast<int>(button));
		/* 前フレームの入力状態が存在する場合はその値を、存在しない場合はfalseを使用。*/
		bool previos = (inputIterator != m_previousButtons.end()) ? inputIterator->second : false;

		/* トリガー入力は、現在の入力が押されていて、前フレームの入力が押されていない場合にtrueを返す。*/
		return current && !previos;
	}
}