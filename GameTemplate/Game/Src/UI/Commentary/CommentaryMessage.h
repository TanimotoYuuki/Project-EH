#pragma once

#include <string>

namespace nsApp
{
	namespace nsUI
	{
		/**
		 * @brief 実況UIに表示するメッセージ。
		 */
		struct CommentaryMessage
		{
			std::wstring text;         //! 表示する文章。
			float displayTime = 2.5f;  //! 表示時間。
			float timer = 0.0f;        //! 経過時間。
		};
	}
}