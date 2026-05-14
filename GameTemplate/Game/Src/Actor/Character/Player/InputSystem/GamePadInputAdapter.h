#pragma once



#include "IInputDevice.h"

namespace nsApp
{
	class GamePadInputAdapter : public IInputDevice
	{
	public:
		/* コンストラクタ。*/
		GamePadInputAdapter(int padIndex) : m_padIndex(padIndex) {}


	public:
		/* 
		 * @brief 親クラスからのオーバーライド処理。
		 * 処理の詳細はIInputDeviceクラスの同名関数の説明を参照。
		 */

		/* 
		 * @brief リセット処理。
		 */
		void Reset() override {};

		/**
		 * @brief Press入力判定画あるか検知。
		 */
		inline bool IsPress(nsK2EngineLow::EnButton button) const override
		{
			return g_pad[m_padIndex]->IsPress(button);
		}

		/**
		 * @brief Trigger入力判定画あるか検知。
		 */
		inline bool IsTrigger(nsK2EngineLow::EnButton button) const override
		{
			return g_pad[m_padIndex]->IsTrigger(button);
		}

		/**
		 * @brief 左スティックのX軸の値を取得。
		 */
		inline float GetLStickX() const override
		{
			return g_pad[m_padIndex]->GetLStickXF();
		}

		/**
		 * @brief 左スティックのY軸の値を取得。
		 */
		inline float GetLStickY() const override
		{
			return g_pad[m_padIndex]->GetLStickYF();
		}


	private:
		int m_padIndex = 0;	//! gamepadの要素数。
	};
}
