#pragma once

/**
 * @file   CommentaryUI.h
 * @brief  実況UIのクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/28 最終更新日。
 */

#include <array>
#include "Src/UI/Commentary/CommentaryMessage.h"

namespace nsApp
{
	namespace nsUI
	{
		class CommentaryUI
		{
		public:
			/**
			 * @brief 初期化。
			 */
			void Init();

			/**
			 * @brief 描画。
			 * @param rc 描画コンテキスト。
			 * @param messages 表示する実況メッセージ一覧。
			 */
			void Render(RenderContext& rc, const std::vector<CommentaryMessage>& messages);


		private:
			static constexpr int MAX_MESSAGE_COUNT = 3;				   //! 最大表示メッセージ数。

			SpriteRender m_frameSprite;
			std::array<FontRender, MAX_MESSAGE_COUNT> m_fontRender;    //! コメントを描画するためのフォント。
		};
	}
}