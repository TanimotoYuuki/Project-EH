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
		/* リストの数だけプレイアブルキャラを生成する。*/
		for (const auto& data : spawnDataList)
		{
			/* */
			if (m_characterFactory.count(data.weaponType) > 0)
				m_spawnPlayer = m_characterFactory[data.weaponType](data.playerName);

			/* 生成に成功した場合。*/
			if (m_spawnPlayer != nullptr)
				m_spawnPlayer->InitializeSpawnData(data);
		}
	}
}