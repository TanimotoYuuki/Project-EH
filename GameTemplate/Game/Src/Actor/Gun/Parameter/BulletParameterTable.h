#pragma once
/*＊
 * @file   BulletParameterTable.h
 * @brief  各弾丸のパラメータを管理する。
 * @author Yamaguchi Hayato
 * @date   2026/05/05
 */

#include "Src/Actor/Gun/Register/BulletModelRegister.h"
#include "Src/Actor/Gun/Parameter/BulletParameter.h"

namespace nsApp
{
	class BulletParameterTable
	{
	public:
		static const BulletParameter& GetParameter(BulletType kindBullet);


	private:
		static const std::unordered_map<BulletType, BulletParameter> m_bulletParameterTable; //! 弾の種類と性能データの対応表。
	};
}
