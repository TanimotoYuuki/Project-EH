#pragma once

namespace nsApp
{
	class ICameraStrategy
	{
	public:
		/* コンストラクタとデストラクタ。*/
		ICameraStrategy() = default;
		virtual ~ICameraStrategy() = default;


	public:
		/**
		 * @brief 更新処理。
		 */
		virtual void Update() = 0;
	};
}
