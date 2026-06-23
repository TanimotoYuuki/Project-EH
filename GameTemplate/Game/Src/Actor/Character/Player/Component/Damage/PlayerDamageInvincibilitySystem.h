#pragma once

/**
 * @file   PlayerDamageInvincibilitySystem.h
 * @brief  プレイヤーの被ダメ無敵を管理するクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/06/22
 */

#include "DamageInvincibilityParameter.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	/**
	 * @class PlayerDamageInvincibilitySystem
	 * @brief 被ダメ無敵タイマーの開始・更新・判定を担当する。
	 */
	class PlayerDamageInvincibilitySystem
	{
	public:
		/**
		 * @brief パラメータを初期化する。
		 * @param weaponType 武器種（将来 TSV 参照用。現状は共通値）。
		 */
		void Initialize(WeaponType weaponType);

		/**
		 * @brief 毎フレーム、無敵タイマーを減算する。
		 * @param deltaTime 経過時間（秒）。
		 */
		void Update(float deltaTime);

		/**
		 * @brief KnockBack 起き上がり後の無敵を開始する。
		 */
		void BeginAfterKnockBack();

		/**
		 * @brief 任意時間の被ダメ無敵を開始する。
		 * @param durationSec 無敵時間（秒）。
		 */
		void Begin(float durationSec);

		/**
		 * @brief 被ダメ無敵を即終了する。
		 */
		void End();

		/**
		 * @brief 被ダメ無敵中か。
		 */
		bool IsActive() const;

		/**
		 * @brief ダメージ適用をブロックすべきか。
		 */
		bool BlocksDamage() const;

		/**
		 * @brief ノックバックをブロックすべきか。
		 */
		bool BlocksKnockback() const;

		/**
		 * @brief 残り無敵時間（秒）。
		 */
		float GetRemainingTime() const;

	private:
		DamageInvincibilityParameter m_param{};
		float m_remainingTime = 0.0f;
	};
}