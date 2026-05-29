#pragma once
/*
 * @file    DamageProcessor.h
 * @brief   DamageRequestを元にダメージ数を描画する。
 * @author  Yamaguchi Hayato
 * @date    2026/05/20
 * @details ダメージフォントの描画とダメージの適応を一括で行う。
 */

#include "DamageRequest.h"

namespace nsApp
{
	class DamageIndicatorPool;

	class DamageProcessor final
	{
	public:
		/* コンストラクタとデストラクタ。*/
		DamageProcessor() = delete;
		~DamageProcessor() = delete;


	public:
		/**
		 * @brief ダメージ表示用プールを設定する。
		 * @param damageIndicatorPool ダメージ表示用プール。
		 */
		static void SetDamageIndicatorPool(DamageIndicatorPool* damageIndicatorPool);

		/**
		 * @brief ダメージ数を描画する。
		 * @param request ダメージフォントを描画する際に必要な構造体。
		 * @return 描画に成功できたかどうかのフラグ。
		 */
		static bool ApplyDamage(const DamageRequest& request);

		/**
		 * @brief ダメージリクエストを構築する。
		 * @param targetCharacter ダメージを受けるキャラクター。
		 * @param damageAmount ダメージ量。
		 * @return 構築されたダメージリクエスト。
		 */
		static DamageRequest BuildTargetDamageRequest(nsActor::ICharacter* targetCharacter, int damageAmount);

		/**
		 * @brief ダメージを適応する。
		 * @param targetCharacter ダメージを受けるキャラクター。
		 * @param damageAmount ダメージ量。
		 * @param textOffsetY ダメージテキストのY軸オフセット。デフォルトは120.0f。
		 * @return ダメージ適用に成功したらtrue。
		 */
		inline static bool ApplyDamageToTarget(nsActor::ICharacter* targetCharacter, int damageAmount, float textOffsetY = 120.0f)
		{
			const DamageRequest request = BuildTargetDamageRequest(targetCharacter, damageAmount);
			return ApplyDamage(request);
		}


	private:
		/**
		 * @brief リクエストが有効か判定する。
		 * @param request ダメージ処理に必要な情報。
		 * @return 有効な場合true。
		 */
		static bool IsValidRequest(const DamageRequest& request);

		/**
		 * @brief ダメージフォントを生成する。
		 * @param damageValue 表示するダメージ量。
		 * @param position 表示位置。
		 */
		static void SpawnDamageIndicator(int damageValue, const Vector3& position);


	private:
		static DamageIndicatorPool* m_damageIndicatorPool; //! ダメージ表示用プール。
	};
}