#include "stdafx.h"
#include "PlayerGenerator.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/HammerCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/SwordCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/WandCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/TwinGunCharacter.h"

namespace nsApp
{
	PlayerGenerator::PlayerGenerator()
	{
		/* 生成対象を登録する。*/
		/* Sword。*/
		m_characterFactory[WeaponType::GreatSword] = [](const char* name) { return NewGO<nsActor::SwordCharacter>(0, name); };

        /* Hammer。*/
		m_characterFactory[WeaponType::Hammer] = [](const char* name) { return NewGO<nsActor::HammerCharacter>(0, name); };

		/* Wand。*/
		m_characterFactory[WeaponType::Wand] = [](const char* name) { return NewGO<nsActor::WandCharacter>(0, name); };

		/* TwinGun。*/
		m_characterFactory[WeaponType::TwinGun] = [](const char* name) { return NewGO<nsActor::TwinGunCharacter>(0, name); };
	}


	nsActor::Player* PlayerGenerator::SpawnPlayer(const PlayerSpawnData& spawnData)
	{
		/* 生成用変数。*/
		nsActor::Player* player = nullptr;

		/* 武器の種類に対応したキャラクターを生成する。*/
		if (m_characterFactory.count(spawnData.weaponType) > 0)
			player = m_characterFactory[spawnData.weaponType](spawnData.playerName);

		/* プレイヤーのnullチェック。*/
		if (player != nullptr)
			player->InitializeSpawnData(spawnData);

		return player;
	}


	std::vector<nsActor::Player*> PlayerGenerator::SpawnPlayers(const std::vector<PlayerSpawnData>& spawnDataList)
	{
		/* 生成用変数。*/
		std::vector<nsActor::Player*> spawnedPlayers;

		/* 生成データのリストをループして、プレイヤーを生成する。*/
		for (const auto& data : spawnDataList)
		{
			/* プレイヤーを生成する。*/
			nsActor::Player* player = SpawnPlayer(data);

			/* プレイヤーのnullチェック。*/
			if (player != nullptr)
				/* 生成したプレイヤーをリストに追加する。*/
				spawnedPlayers.push_back(player);
		}

		return spawnedPlayers;
	}
}