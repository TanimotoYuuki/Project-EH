#pragma once
/**
 * @file   MagicParameterTable.h
 * @brief  魔法のパラメータテーブルを管理するクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/05/12
 */

#include "MagicParameter.h"
#include "Src/Actor/Magic/MagicProjectotile.h"

namespace nsApp
{
	class MagicParameterTable
	{
	/* ゲッター。*/
	public:
		/**
		 * @brief 魔法の種類に応じたパラメータを取得。
		 * @param type 魔法の種類。
		 * @return 魔法のパラメータ。
		 */
		static const MagicParameter& GetParameter(nsActor::MagicType type);

	    /**
		 * @brief マジックオブジェクトのファイルパスを取得。
		 * @param modelName モデル名。
		 * @return マジックオブジェクトのファイルパス。
		 */
		static inline const std::string SearchMissileModelPath(const std::string& modelName)
		{
			return "Assets/modelData/Character/Weapon/" + modelName + ".tkm";
		}


	private:
		/**
		 * @brief 角度計算を行う。
		 * @param angleX X軸の角度。
		 * @param angleY Y軸の角度。
		 * @param angleZ Z軸の角度。
		 * @return 最終的な角度の計算結果。
		 */
		Quaternion MakeAngle(float angleX, float angleY, float angleZ);


	private:
		static const std::unordered_map<nsActor::MagicType, MagicParameter> m_magicTable; //! 魔法の種類とそのパラメータを紐づけるテーブル。
	};
}
