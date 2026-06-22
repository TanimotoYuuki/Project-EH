#pragma once

/**
 * @file   LoadingDestination.h
 * @brief  ローディング完了後の遷移先。
 * @author Yamaguchi Hayato
 * @date   2026/06/20: 新規作成。
 */

namespace nsApp
{
	namespace nsScene
	{
		/**
		 * @enum EnLoadingDestination
		 * @brief ローディング完了後の遷移先を管理する列挙型。
		 */
		enum EnLoadingDestination : uint8_t
		{
			toSelect,  //! タイトル → クエスト選択。
			toInGame,  //! 出撃確定 → インゲーム。
			Num,
		};
	}
}