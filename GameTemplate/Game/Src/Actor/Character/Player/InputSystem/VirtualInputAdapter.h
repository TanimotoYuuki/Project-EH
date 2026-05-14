#pragma once
/**
 * @file    VirtualInputAdapter.h 
 * @brief   仮想入力アダプタークラス。
 * @author  Yamaguchi Hayato。
 * @detail  プレイヤーの入力を抽象化するための仮想入力アダプタークラス。
 */

#include "IInputDevice.h"
#include <unordered_map>

namespace nsApp
{
	class VirtualInputAdapter : public IInputDevice
	{
	public:
		/*
		 * @brief 親クラスからのオーバーライド処理。
		 */

		/**
		 * @brief 前フレームの入力を保存する。
		 */
		void BeginFlame() override;

		/**
		 * @brief 入力状態のリセット。
		 */
		void Reset() override;

		/**
		 * @brief Press入力判定画あるか検知。
		 */
		inline bool IsPress(nsK2EngineLow::EnButton button) const override
		{
			/* 現在の入力状態を取得。*/
			auto inputIterator = m_currentButtons.find(static_cast<int>(button));
			return (inputIterator != m_currentButtons.end()) ? inputIterator->second : false;
		}

		/**
		 * @brief Trigger入力判定画あるか検知。
		 */
		bool IsTrigger(nsK2EngineLow::EnButton button) const override;

		/**
		 * @brief 左スティックのX軸の値を取得。
		 */
		inline float GetLStickX() const override
		{
			return m_stickX;
		}

		/**
		 * @brief 左スティックのY軸の値を取得。
		 */
		inline float GetLStickY() const override
		{
			return m_stickY;
		}


	public:
		/* 
		 * @brief NPCBrainクラスへの判断を書き込むためのメソッド。
		 * @param button 判定するボタン。
		 * @param isPress ボタンが押されているかどうかの状態。
		 */
		inline void SetButton(nsK2EngineLow::EnButton button, bool isPress)
		{
			/* ボタンの状態を更新。*/
			m_currentButtons[static_cast<int>(button)] = isPress;
		}

		/**
		 * @brief 左スティックの値を設定するためのメソッド。
		 * @param stickX X軸の値。
		 * @param stickY Y軸の値。
		 */
		inline void SetLStick(float stickX, float stickY)
		{
			m_stickX = stickX;
			m_stickY = stickY;
		}


	private:
		std::unordered_map<int, bool> m_currentButtons;		//! 現在のボタンの状態を保存するマップ。
		std::unordered_map<int, bool> m_previousButtons;	//! 前フレームのボタンの状態を保存するマップ。

		float m_stickX = 0.0f;								//! 現在の左スティックのX軸の値。
		float m_stickY = 0.0f;								//! 現在の左スティックのY軸の値。
	};
}