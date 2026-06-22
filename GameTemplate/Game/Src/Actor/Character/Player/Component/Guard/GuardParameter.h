#pragma once

/*
 * @file   GuardParameter.h
 * @brief  ガードパラメータを定義するヘッダーファイル。
 * @author Yamaguchi Hayato
 * @date   2024/06/21: 初版作成
 */


namespace nsApp
{
	/**
	 * @struct GuardParameter
	 * @brief ガードパラメータを表す構造体。
	 */
	struct GuardParameter
	{
		float maxDuration = 2.0f;        //! ガードの最大持続時間。
		float damageReduction = 0.5f;    //! ガード時のダメージ軽減率（0.0f〜1.0f）。
		bool isBlockKnockback = true;    //! ガード時にノックバックをブロックするかどうか。
	};
}