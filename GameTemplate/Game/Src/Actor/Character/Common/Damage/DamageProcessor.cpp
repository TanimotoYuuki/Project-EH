#include "stdafx.h"
#include "DamageProcessor.h"
#include "DamageIndicatorPool.h"

namespace
{
	const auto DAMAGE_TEXT_OFFSET_Y = 120.0f; //! ダメージテキストのY軸オフセット。
}


namespace nsApp
{
	/* static変数を初期化。*/
	DamageIndicatorPool* DamageProcessor::m_damageIndicatorPool = nullptr;


	bool DamageProcessor::ApplyDamage(const DamageRequest& request)
	{
		/* リクエストの内容が有効でない場合、処理を行わない。*/
		if (!IsValidRequest(request))
			return false;

		/* ダメージを付与。*/	
		request.target->ApplyDamage(request.damageAmount);

		/* ダメージフォントの表示を依頼。*/
		SpawnDamageIndicator(request.damageAmount, request.hitPosition);

		return true;
	}


	DamageRequest DamageProcessor::BuildTargetDamageRequest(nsActor::ICharacter* targetCharacter, int damageAmount)
	{
		/* ダメージリクエストを構築。*/
		DamageRequest request;
		request.target = targetCharacter;
		request.damageAmount = damageAmount;

		/* ダメージテキストの表示位置を設定。*/
		if (targetCharacter != nullptr)
		{
			request.hitPosition = targetCharacter->GetPosition();
			request.hitPosition.y += DAMAGE_TEXT_OFFSET_Y;
		}

		return request;
	}


	bool DamageProcessor::IsValidRequest(const DamageRequest& request)
	{
		/* 目標が存在しない場合、処理を行わない。*/
		if (request.target == nullptr)
			return false;

		/* ダメージ数が0の場合、処理を行わない。*/
		if (request.damageAmount <= 0)
			return false;

		return true;
	}

	void DamageProcessor::SpawnDamageIndicator(int damageValue, const Vector3& position)
	{
		/* ダメージ表示用プールが設定されていない場合、処理を行わない。*/
		if (m_damageIndicatorPool == nullptr)
			return;

		/* ダメージ表示用プールにダメージテキストの生成を依頼。*/
		m_damageIndicatorPool->SpawnDamageText(damageValue, position);
	}
}