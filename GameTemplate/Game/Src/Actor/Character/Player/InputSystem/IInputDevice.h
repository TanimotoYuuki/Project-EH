#pragma once
/**
 * @file    IInputDevice.h
 * @brief   入力デバイスのインターフェースクラス。
 * @author  Yamaguchi Hayato。
 * @date    2026/05/12
 * @details プレイヤーの入力を抽象化するためのインターフェースクラス。
 */

namespace nsApp
{
	class IInputDevice
	{
	public:
		/* デストラクタ。*/
		virtual ~IInputDevice() = default;


	public:
		/**
		 * @brief フレーム開始時の更新処理。
		 */
		virtual void BeginFlame() {};

		/**
		 * @brief 入力状態のリセット。
		 */
		virtual void Reset() = 0;

		/**
		 * @brief 指定したボタンのPress入力判定があるかチェックする。
		 * @param button コントローラーの列挙型をセットする。
		 * @return 指定したボタンのPress入力判定があるかどうか。
		 */
		virtual bool IsPress(nsK2EngineLow::EnButton button) const = 0;

		/**
		 * @brief 指定したボタンのTrigger入力判定があるかチェックする。
		 * @param button コントローラーの列挙型をセットする。
		 * @return 指定したボタンのTrigger入力判定があるかどうか。
		 */
		virtual bool IsTrigger(nsK2EngineLow::EnButton button) const = 0;

		/**
		 * @brief  左スティックのX軸の値を取得する。
		 * @return 左スティックのX軸の値。
		 *         0.0fを中心に、-1.0fから1.0fの範囲で値が変化する。
		 *	       -1.0fは左方向、1.0fは右方向を表す。
		 */
		virtual float GetLStickX() const = 0;

		/**
		 * @brief 左スティックのY軸の値を取得する。
		 * @return 左スティックのY軸の値。
		 */
		virtual float GetLStickY() const = 0;
		
	};
}