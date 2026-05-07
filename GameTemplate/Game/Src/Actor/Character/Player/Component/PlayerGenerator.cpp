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
		/* �v���C�A�u���L�����N���X��o�^�B*/
		/* Sword�B*/
		m_characterFactory[WeaponType::GreatSword] = [](const char* name) { return NewGO<nsActor::SwordCharacter>(0, name); };

        /* Hammer�B*/
		m_characterFactory[WeaponType::Hammer] = [](const char* name) { return NewGO<nsActor::HammerCharacter>(0, name); };

		/* Wand�B*/
		m_characterFactory[WeaponType::Wand] = [](const char* name) { return NewGO<nsActor::WandCharacter>(0, name); };

		/* @TODO: �e�L����������ł�����A�o�^����B*/
	}


	void PlayerGenerator::SpawnPlayers(const std::vector<PlayerSpawnData>& spawnDataList)
	{
		/* */
		std::vector<nsActor::Player*> spawnedPlayers;

		/* ���X�g�̐������v���C�A�u���L�����𐶐�����B*/
		for (const auto& data : spawnDataList)
		{
			/* ��������L�����𐔂��A����̎�ނƐ�������Z�b�g�B*/
			if (m_characterFactory.count(data.weaponType) > 0)
				m_spawnPlayer = m_characterFactory[data.weaponType](data.playerName);

			/* �����ɐ��������ꍇ�B*/
			if (m_spawnPlayer != nullptr)
			{
				/* */
				m_spawnPlayer->InitializeSpawnData(data);

				/* ���������v���C���[����X�g�ɕۑ�����B*/
				spawnedPlayers.push_back(m_spawnPlayer);
			}
		}

	}
}