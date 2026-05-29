#pragma once
/**
 * @file  IPoolable.h
 * @brief プールを付与するオブジェクトを管理するインタフェース。
 * @author Yamaguchi Hayato
 * @date  2026/05/22
 */

namespace nsApp
{
	class IPoolable
	{
	public:
		/* デストラクタ。*/
		virtual ~IPoolable() = default;


	public:
		/**
		 * @brief オブジェクトを使用可能な状態にする。
		 */
		virtual void OnAcquire() = 0;

		/**
		 * @brief オブジェクトを使用不可な状態にする。
		 */
		virtual void OnRelease() = 0;

		/**
		 * @brief オブジェクトを使用中か判断する。
		 * @return 使用中ならtrue、そうでなければfalse。
		 */
		virtual bool IsActive() const = 0;    

		/**
		 * @brief 非アクティブ状態にする。
		 */
		virtual void Deactivate() = 0;    
	};
}