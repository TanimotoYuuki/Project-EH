#pragma once

/**
 * @file   LoadingTextUI.h
 * @brief  ローディング画面でのテキストを管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/06/09
 */

namespace nsApp
{
	namespace nsUI
	{
		class LoadingTextUI
		{
		public:
			/* コンストラクタとデストラクタ。*/
			LoadingTextUI() = default;
			virtual ~LoadingTextUI() = default;


		public:
			/* 初期化処理。*/
			void Init();

			/* 更新処理。*/
			void Update(float elapsedTime);

			/* 描画処理。*/
			void Render(RenderContext& rc);


		private:
			FontRender m_loadingText; //! ローディング画面でのテキスト。

			float m_timer = 0.0f;     //! テキストの点滅やアニメーションのためのタイマー。

			int m_dot = 0;            //! ローディングテキストの末尾に表示するドットの数を管理する変数。
		};
	}
}
