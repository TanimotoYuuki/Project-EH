#include "stdafx.h"
#include "PlayerGenerator.h"

#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/HammerCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/SwordCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/WandCharacter.h"

namespace nsApp
{
	PlayerGenerator::PlayerGenerator()
	{
		/* プレイアブルキャラクラスを登録。*/
		/* Sword。*/
		m_characterFactory[WeaponType::GreatSword] = [](const char* name) { return NewGO<nsActor::SwordCharacter>(0, name); };

        /* Hammer。*/
		m_characterFactory[WeaponType::Hammer] = [](const char* name) { return NewGO<nsActor::HammerCharacter>(0, name); };

		/* Wand。*/
		m_characterFactory[WeaponType::Wand] = [](const char* name) { return NewGO<nsActor::WandCharacter>(0, name); };

		/* @TODO: 銃キャラも実装でき次第、登録する。*/
	}


	void PlayerGenerator::SpawnPlayers(const std::vector<PlayerSpawnData>& spawnDataList)
	{
		/* */
		std::vector<nsActor::Player*> spawnedPlayers;

		/* リストの数だけプレイアブルキャラを生成する。*/
		for (const auto& data : spawnDataList)
		{
			/* 生成するキャラを数え、武器の種類と生成名をセット。*/
			if (m_characterFactory.count(data.weaponType) > 0)
				m_spawnPlayer = m_characterFactory[data.weaponType](data.playerName);

			/* 生成に成功した場合。*/
			if (m_spawnPlayer != nullptr)
			{
				/* */
				m_spawnPlayer->InitializeSpawnData(data);

				/* 生成したプレイヤーをリストに保存する。*/
				spawnedPlayers.push_back(m_spawnPlayer);
			}
		}

	}
}