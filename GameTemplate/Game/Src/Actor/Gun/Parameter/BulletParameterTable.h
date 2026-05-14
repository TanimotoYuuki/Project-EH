#pragma once
/**
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
		/**
		 * @brief 弾丸の種類に応じたパラメータを取得する。
		 * @param kindBullet 弾丸の種類。
		 * @return 弾丸のパラメータ。
		 */
		static const BulletParameter& GetParameter(BulletType kindBullet);

		/**
		 * @brief 弾丸のモデルファイルのパスを取得する。
		 * @param name モデル名。
		 * @return モデルファイルのパス。
		 */
		static inline std::string GetModelPath(const std::string& name)
		{
			return "Assets/modelData/Character/Weapon/" + name + ".tkm";
		}


	private:
		static const std::unordered_map<BulletType, BulletParameter> m_bulletParameterTable; //! 弾の種類と性能データの対応表。
	};
}
