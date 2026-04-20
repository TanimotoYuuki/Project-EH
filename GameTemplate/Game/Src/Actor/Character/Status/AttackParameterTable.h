#pragma once
/**
* @file   AttackParameterTable.h
* @brief  技ごとのダメージ数/クリティカルダメージ数/クリティカル率を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/19
*/

#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	/** 
	 * @struct AttackParameter
	 * @brief 技ごとのダメージ数/クリティカルダメージ数/クリティカル率を管理する構造体。
	 */
	struct AttackParameter
	{
		float damageMultiplier; //! ダメージ倍率。
		float criticalRatel;    //! クリティカル率。
	};

	class AttackParameterTable
	{
	public:
		/*
		* @brief AttackTypeを受け取り、その技のパラメーターを返す処理。
		*/
		static const AttackParameter& GetAttackParameter(AttackType type);


	private:
		/* テーブル本体。*/
		static const std::unordered_map<AttackType, AttackParameter> m_table; //! AttackTypeごとのAttackParameterを管理するマップ。
	};
}

