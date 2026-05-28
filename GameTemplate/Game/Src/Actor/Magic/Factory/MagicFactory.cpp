#include "stdafx.h"
#include "MagicFactory.h"
#include "Src/Actor/Magic/Parameter/MagicParameterTable.h"
#include "Src/Actor/Magic/MagicProjectotile.h"

namespace
{
	const char* GetMagicObjectName(nsApp::nsActor::MagicType type)
	{
		using nsApp::nsActor::MagicType;

		switch (type)
		{
		case MagicType::enNormalMagic:
			return "MagicObject_Normal";

		case MagicType::enRushMagic:
			return "MagicObject_Rush";

		case MagicType::enAirMagic:
			return "MagicObject_Air";

		case MagicType::enChargeMagic:
			return "MagicObject_Charge";

		default:
			return "MagicObject_Normal";
		}
	}
}

namespace nsApp
{
	void MagicFactory::CreateMagicObject(MAGICTYPE type,const Vector3& basePosition,const Vector3& forward,CHARACTER* target,EFFECT* effectList)
	{
		/* テーブルから魔法の種類の情報を取りだす。*/
		const auto& magicParameter = MagicParameterTable::GetParameter(type);

		/* 方向ベクトルを正規化する。*/
		Vector3 localForwardPosition = forward;
		localForwardPosition.Normalize();

		/* 生成座標を補正する。*/
		Vector3 localSpawnPosition = basePosition;
		localSpawnPosition.y += magicParameter.spawnOffsetY;
		localSpawnPosition += localForwardPosition * magicParameter.forwardOffset;

		/* 未使用の魔法弾を探す。*/
		const char* objectName = GetMagicObjectName(type);

		nsActor::MagicProjectotile* createMagic = nullptr;

		const auto& magics = FindGOs<nsActor::MagicProjectotile>(objectName);
		for (auto* pooledMagic : magics)
		{
			if (pooledMagic != nullptr && !pooledMagic->IsInUse())
			{
				createMagic = pooledMagic;
				break;
			}
		}

		if (createMagic == nullptr)
			createMagic = NewGO<nsActor::MagicProjectotile>(0, objectName);

		if (createMagic)
		{
			createMagic->Initialize(type, localSpawnPosition, localForwardPosition, magicParameter);
			createMagic->SetEffectList(effectList);

			if (target)
				createMagic->SetTarget(target);
		}
	}
}