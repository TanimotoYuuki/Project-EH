#pragma once
/*
 * @file   PlayerSpawnData.h
 * @brief  プレイヤー生成システム。
 * @author Yamaguchi Hayato。
 * @date   2026/04/24。
 */

#include "Src/Actor/Character/Common/CharacterAnimation.h"


namespace nsApp
{
	/**
	 * @enum   ControllerType。
	 * @brief  操作できるコントローラーを定義。
	 * @note   プレイアブルキャラによってコントローラーが違う。
	 */
	enum class ControllerType
	{
		Player_1P = 0, //! padIndex 0。
		Player_2P = 1, //! padIndex 1。
		Player_3P = 2, //! padIndex 2。
		Player_4P = 3, //! padIndex 3。
		NPC = -1	   //! NPCはコントローラーを持たない。
	};

	/**
	 * @struct PlayerSpawnData。
	 * @brief  プレイヤー生成に必要なデータをまとめた構造体。
	 */
	struct PlayerSpawnData
	{
		const char* playerName;        //! "〇〇"などの検索用の名前。
		WeaponType weaponType;         //! 武器の種類。
		ControllerType controllerType; //! コントローラーの種類。
		Vector3 spawnPosition;         //! 生成位置。
	};
}
