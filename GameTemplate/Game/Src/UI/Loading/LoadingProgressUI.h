#pragma once

namespace nsApp
{
	namespace nsUI
	{
		class LoadingProgressUI
		{
		public:
			/* コンストラクタとデストラクタ。*/
			LoadingProgressUI() = default;
			virtual ~LoadingProgressUI() = default;


		public:
			/**
			 * @brief 初期化処理。
			 * @details ローディングの進行度を表示するためのUIを初期化する。
			 */
			void Initialize();

			/**
			 * @brief 更新処理。
			 * @param progress ローディングの進行度を表す0.0fから1.0fの値。
			 */
			void Update(float progress);

			/**
			 * @brief 描画処理。
			 */
			void Render(RenderContext& rc);


		private:
			/**
			 * @brief 値を0.0fから1.0fの範囲にクランプする関数。
			 * @param value クランプする値。
			 * @return クランプされた値。
			 */
			float Clamp01(float value) const;

		};
	}
}
