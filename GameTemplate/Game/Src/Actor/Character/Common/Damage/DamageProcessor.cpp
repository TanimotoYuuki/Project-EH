#include "stdafx.h"
#include "DamageProcessor.h"
#include "DamageIndicatorPool.h"

namespace
{
	const auto DAMAGE_TEXT_OFFSET_Y = 120.0f; //! ダメージテキストのY軸オフセット。
}


namespace nsApp
{
	DamageIndicatorPool* DamageProcessor::m_damageIndicatorPool = nullptr;


	void DamageProcessor::SetDamageIndicatorPool(DamageIndicatorPool* damageIndicatorPool)
	{
		m_damageIndicatorPool = damageIndicatorPool;
	}


	bool DamageProcessor::ApplyDamage(const DamageRequest& request)
	{
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
		DamageRequest request;
		request.target = targetCharacter;
		request.damageAmount = damageAmount;

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
		if (m_damageIndicatorPool == nullptr)
			return;

		m_damageIndicatorPool->SpawnDamageText(damageValue, position);
	}
}