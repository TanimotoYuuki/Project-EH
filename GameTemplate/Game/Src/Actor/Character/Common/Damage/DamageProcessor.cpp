#include "stdafx.h"
#include "DamageProcessor.h"
#include "DamageIndicatorPool.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace
	{
		const auto DAMAGE_TEXT_OFFSET_Y = 120.0f; //! ダメージテキストのY軸オフセット。

		/**
		 * @brief 最終的なダメージを計算する関数。
	     * ガード状態のプレイヤーに対しては、ガードダメージ減少率を考慮してダメージを減少させる。
		 * ガードしていない場合は、基本ダメージをそのまま返す。
		 *
		 * @param target ダメージを受けるキャラクター。
		 * @param baseDamage 基本ダメージ。
		 * @return 最終的なダメージ値。ガードしている場合は減少したダメージ、そうでない場合は基本ダメージを返す。
		 * ガードしているプレイヤーに対しては、ガードダメージ減少率を考慮してダメージを減少させる。
		 * ガードしていない場合は、基本ダメージをそのまま返す。
		 *
		 * @param target ダメージを受けるキャラクター。
		 * @param baseDamage 基本ダメージ。
		 * @return 最終的なダメージ値。ガードしている場合は減少したダメージ、そうでない場合は基本ダメージを返す。
		 */
		int CalculateFinalDamage(nsActor::ICharacter* target, int baseDamage)
		{
			/* ターゲットが存在しない場合や基本ダメージが0以下の場合は、ダメージを適用しない。*/
			if (target == nullptr || baseDamage <= 0)
				return 0;

			/* ターゲットがプレイヤーで、かつガードしている場合は、ガードダメージ減少率を考慮してダメージを減少させる。*/
			auto* player = dynamic_cast<nsActor::Player*>(target);
			if (player == nullptr || !player->IsGuarding())
				return baseDamage;

			/* ガードシステム経由でダメージを軽減する。*/
			return player->GetGuardSystem().CalculateReducedDamage(baseDamage);
		}
	}

	/* 静的メンバ変数の定義。*/
	DamageIndicatorPool* DamageProcessor::m_damageIndicatorPool = nullptr;


	bool DamageProcessor::ApplyDamage(const DamageRequest& request)
	{
		/* ダメージリクエストが有効でない場合は、ダメージを適用せずにfalseを返す。*/
		if (!IsValidRequest(request))
			return false;
		/* プレイヤーが被ダメ無敵ならダメージを適用しない。*/
		if (auto* player = dynamic_cast<nsActor::Player*>(request.target))
		{
			if (player->GetDamageInvincibilitySystem().BlocksDamage())
				return false;
		}

		/* 最終的なダメージを計算する。*/
		const int finalDamage = CalculateFinalDamage(request.target, request.damageAmount);

		/* 最終的なダメージが0以下の場合は、ダメージを適用せずにfalseを返す。*/
		if (finalDamage <= 0)
			return false;

		/* ターゲットにダメージを適用する。*/
		request.target->ApplyDamage(finalDamage);
		/* ダメージインジケーターを表示する。*/
		SpawnDamageIndicator(finalDamage, request.hitPosition);
		return true;
	}


	DamageRequest DamageProcessor::BuildTargetDamageRequest(nsActor::ICharacter* targetCharacter, int damageAmount)
	{
		DamageRequest request; //! ダメージリクエストの構築。

		/* ターゲットキャラクターとダメージ量を設定する。*/
		request.target = targetCharacter;
		request.damageAmount = damageAmount;


		/* ターゲットキャラクターが存在する場合は、ヒット位置を設定する。*/
		if (targetCharacter != nullptr)
		{
			/* ターゲットキャラクターの位置を取得して、ダメージテキストのY軸オフセットを加算してヒット位置を設定する。*/
			request.hitPosition = targetCharacter->GetPosition();

			/* ダメージテキストがキャラクターの頭上に表示されるように、Y軸にオフセットを加算する。*/
			request.hitPosition.y += DAMAGE_TEXT_OFFSET_Y;
		}

		return request;
	}


	bool DamageProcessor::IsValidRequest(const DamageRequest& request)
	{
		/* ターゲットが存在しない場合やダメージ量が0以下の場合は、リクエストが無効であると判断する。*/
		if (request.target == nullptr)
			return false;

		/* ダメージ量が0以下の場合は、リクエストが無効であると判断する。*/
		if (request.damageAmount <= 0)
			return false;

		return true;
	}


	void DamageProcessor::SpawnDamageIndicator(int damageValue, const Vector3& position)
	{
		/* ダメージインジケーターのプールが存在しない場合は、ダメージテキストを表示せずに処理を終了する。*/
		if (m_damageIndicatorPool == nullptr)
			return;

		/* ダメージインジケーターのプールを使用して、ダメージテキストを表示する。*/
		m_damageIndicatorPool->SpawnDamageText(damageValue, position);
	}
}