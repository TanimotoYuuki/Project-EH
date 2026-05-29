#pragma once
/**
 * @file   PlayerGenerator.h
 * @brief  Player生成システム。
 * @author Yamaguchi Hayato。
 * @date   2026/04/24。
 */

#include "Src/Actor/Character/Player/Component/PlayerSpawnData.h"
#include "Src/Actor/Character/Player/Player.h"

#include <vector>
#include <functional>
#include <unordered_map>

namespace nsApp
{

	namespace nsActor {
		class Player;
	}

	/**
	 * @def Playerのポインタを返すfunctionKey。
	 */
	using PLAYER_FUNCTION = std::function<nsActor::Player* (const char*)>;


	class PlayerGenerator
	{
	public:
		/* コンストラクタとデストラクタ。*/
		PlayerGenerator();
		virtual ~PlayerGenerator() = default;


	public:
		/**
		 * @brief 渡されたマップを元にPlayerを生成する。
		 * @param spawnDataList Player生成に必要なデータのリスト。
		 */
		std::vector<nsActor::Player*> SpawnPlayers(const std::vector<PlayerSpawnData>& spawnDataList);


	private:
		nsActor::Player* m_spawnPlayer = nullptr;


	private:
		std::unordered_map<WeaponType, PLAYER_FUNCTION> m_characterFactory; //! 武器の種類とPlayerクラスのfunctionを保持するマップ。
	};

}
