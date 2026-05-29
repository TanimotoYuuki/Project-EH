#include "stdafx.h"
#include "MagicFactory.h"
#include "Src/Actor/Magic/Parameter/MagicParameterTable.h"
#include "Src/Actor/Magic/MagicProjectotile.h"

namespace nsApp
{
	void MagicFactory::CreateMagicObject(nsActor::MagicType type, const Vector3& basePosition, const Vector3& forward, nsActor::ICharacter* target)
	{
		/* テーブルから魔法の種類の情報を取りだす。*/
		const auto& magicParameter = MagicParameterTable::GetParameter(type);

		/* 方向ベクトルを正規化する。*/
		Vector3 localForwardPosition = forward;
		localForwardPosition.Normalize();

		/* 生成座標を正規化し、補正する。*/
		Vector3 localSpawnPosition = basePosition;
		localSpawnPosition.y += magicParameter.spawnOffsetY;
		localSpawnPosition += localForwardPosition * magicParameter.forwardOffset;

		/* 魔法オブジェクトクラスを生成し、初期化する。*/
		auto* createMagic = NewGO<nsActor::MagicProjectotile>(0, "MagicObject");
		if (createMagic)
		{
			/* 初期化。*/
			createMagic->Initialize(type, localSpawnPosition, localForwardPosition, magicParameter);

			/* 追従目標が設定されているならば、設定する。*/
			if (target)
				createMagic->SetTarget(target);
		}
	}
}