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


	std::vector<nsActor::Player*> PlayerGenerator::SpawnPlayers(const std::vector<PlayerSpawnData>& spawnDataList)
	{
		/* 生成用変数。*/
		std::vector<nsActor::Player*> spawnedPlayers;

		/* 生成データのリストをループして、プレイヤーを生成する。*/
		for (const auto& data : spawnDataList)
		{
			/* 武器の数を調べる。*/
			if (m_characterFactory.count(data.weaponType) > 0)
				m_spawnPlayer = m_characterFactory[data.weaponType](data.playerName);

			/* プレイヤーのnullチェック。*/
			if (m_spawnPlayer != nullptr)
			{
				/* 生成したプレイヤーにスポーンデータを渡す。*/
				m_spawnPlayer->InitializeSpawnData(data);

				/* 生成したプレイヤーをリストに追加する。*/
				spawnedPlayers.push_back(m_spawnPlayer);
			}
		}

		return spawnedPlayers;
	}
}