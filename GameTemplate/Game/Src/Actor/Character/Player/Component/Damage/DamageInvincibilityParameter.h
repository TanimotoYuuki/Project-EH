#pragma once

/**
 * @file   DamageInvincibilityParameter.h
 * @brief  被ダメ無敵のパラメータ。
 * @author Yamaguchi Hayato。
 * @date   2026/06/22
 */

namespace nsApp
{
	/**
	 * @struct DamageInvincibilityParameter
	 * @brief 被ダメ無敵の設定値。
	 */
	struct DamageInvincibilityParameter
	{
		float afterKnockBackDuration = 2.0f; //! KnockBack 起き上がり後の無敵時間（秒）。
	};
}