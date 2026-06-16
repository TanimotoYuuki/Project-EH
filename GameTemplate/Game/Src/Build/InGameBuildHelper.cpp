#include "stdafx.h"
#include "InGameBuildHelper.h"

#include <time.h>
#include "Src/Actor/Character/Boss/Status/BossParameterTable.h"
#include "Src/Actor/Stage/LoadStageData.h"
#include "Src/Actor/Stage/BackGround.h"
#include "Src/Camera/Camera.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Actor/Character/Common/Damage/DamageIndicatorPool.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include "Src/Actor/Character/Player/Component/PlayerGenerator.h"
#include "Src/Actor/Character/Player/InputSystem/PlayerControlerHub.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "CharacterHP.h"
#include "GameTimeLimit.h"
#include "Src/Direction/GameStartDirection.h"
#include "Src/Scene/InGame/Pause.h"
#include "Src/UI/ReboneGage/ReboneGageManager.h"
#include "Src/UI/Commentary/CommentaryUIManager.h"

namespace
{
	const auto INIT_CHARACTER_POSITION_Y = 50.0f;
	const auto INIT_CHARACTER_POSITION_Z = 0.0f;
	const auto INIT_CHARACTER_POSITION_PLAYER1 = Vector3(80.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
	const auto INIT_CHARACTER_POSITION_PLAYER2 = Vector3(-80.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
	const auto INIT_CHARACTER_POSITION_PLAYER3 = Vector3(-50.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
	const auto INIT_CHARACTER_POSITION_PLAYER4 = Vector3(50.0f, INIT_CHARACTER_POSITION_Y, INIT_CHARACTER_POSITION_Z);
}


namespace nsApp
{
	using namespace nsStage;


	void InGameBuildHelper::Initialize(const InGameBuildRequest& request)
	{
		/* �����ɕK�v�ȏ���ۑ�����B*/
		m_request = request;
		m_result = InGameBuildResult();

		/* �����ɕK�v�ȃf�[�^������������B*/
		m_partyData.clear();
		m_players.clear();
		m_buildFunctions.clear();

		/* �����̐i�s�󋵂�����������B*/
		m_currentBuildIndex = 0;
		m_isFinished = false;

		/* �����֐��̏������B*/
		InitializeBuildFunctions();
	}


	void InGameBuildHelper::InitializeBuildFunctions()
	{
		/* �����֐��̏������B*/
		m_buildFunctions.clear();

		/* �����֐��̒ǉ��B*/
		m_buildFunctions.push_back([this]() { BuildRandom(); });
		m_buildFunctions.push_back([this]() { BuildSound(); });
		m_buildFunctions.push_back([this]() { BuildStage(); });
		m_buildFunctions.push_back([this]() { BuildBackGround(); });
		m_buildFunctions.push_back([this]() { BuildCamera(); });
		m_buildFunctions.push_back([this]() { BuildDamagePool(); });
		m_buildFunctions.push_back([this]() { BuildCommentaryUI(); });
		m_buildFunctions.push_back([this]() { BuildPlayerGenerator(); });
		m_buildFunctions.push_back([this]() { CreatePartyData(); });
		m_buildFunctions.push_back([this]() { SpawnPlayer(0); });
		m_buildFunctions.push_back([this]() { SpawnPlayer(1); });
		m_buildFunctions.push_back([this]() { SpawnPlayer(2); });
		m_buildFunctions.push_back([this]() { SpawnPlayer(3); });
		m_buildFunctions.push_back([this]() { RegisterPlayersToReboneGauge(); });
		m_buildFunctions.push_back([this]() { BuildBoss(); });
		m_buildFunctions.push_back([this]() { BuildCharacterHP(); });
		m_buildFunctions.push_back([this]() { BuildGameTimeLimit(); });
		m_buildFunctions.push_back([this]() { BuildGameStartDirection(); });
		m_buildFunctions.push_back([this]() { BuildPause(); });
		m_buildFunctions.push_back([this]() { FinishBuild(); });
	}


	void InGameBuildHelper::ExecuteNextBuildFunction()
	{
		/* �������������Ă���ꍇ�͉������Ȃ��B*/
		if (m_isFinished)
			return;

		/* ���̐����֐������݂��Ȃ��ꍇ�͐��������Ƃ���B*/
		if (m_currentBuildIndex >= static_cast<int>(m_buildFunctions.size()))
		{
			m_isFinished = true;
			return;
		}

		char text[128];
		sprintf_s(
			text,
			"[InGameBuildHelper] Execute index = %d\n",
			m_currentBuildIndex
		);
		OutputDebugStringA(text);

		/* ���̐����֐������s����B*/
		m_buildFunctions[m_currentBuildIndex]();

		sprintf_s(
			text,
			"[InGameBuildHelper] Finished index = %d\n",
			m_currentBuildIndex
		);
		OutputDebugStringA(text);

		/* ���̐����֐��ɐi�ށB*/
		++m_currentBuildIndex;
	}


	void InGameBuildHelper::BuildRandom()
	{
		/* �����̏������B*/
		srand(static_cast<unsigned int>(time(nullptr)));
	}


	void InGameBuildHelper::BuildSound()
	{
		/* �����Ǘ��N���X�̐����B*/
		m_result.soundLister = FindGO<nsSound::SoundLister>("SoundManager");

		/* �����Ǘ��N���X�����݂��Ȃ��ꍇ�͐�������B*/
		if (m_result.soundLister == nullptr)
			m_result.soundLister = NewGO<nsSound::SoundLister>(0, "SoundManager");

		/* �����Ǘ��N���X�̏������B*/
		m_result.soundLister->InitSound();

		/* BGM�̍Đ��B*/
		m_result.soundLister->GetBGMList().StopBGM();

		/* �X�e�[�W1��BGM���Đ�����B*/
		m_result.soundLister->GetBGMList().PlayBGM(nsSound::BGM_ID::Stage1, 1.0f);
	}


	void InGameBuildHelper::BuildStage()
	{
		/* �X�e�[�W1�ɐ؂�ւ���B*/
		LoadStageData::GetInstance().ChangeStage(StageID::stage1);
	}


	void InGameBuildHelper::BuildBackGround()
	{
		/* �w�i�̐����B*/
		m_result.backGround = NewGO<BackGround>(0, "BackGround");
	}


	void InGameBuildHelper::BuildCamera()
	{
		m_result.camera = FindGO<Camera>("camera");

		if (m_result.camera == nullptr)
			m_result.camera = NewGO<Camera>(0, "camera");
	}


	void InGameBuildHelper::BuildDamagePool()
	{
		/* �_���[�W�C���W�P�[�^�[�̃v�[���̐����B*/
		m_result.damageIndicatorPool = NewGO<DamageIndicatorPool>(0, "damagePool");

		/* �_���[�W�C���W�P�[�^�[�̃v�[����DamageProcessor�ɐݒ肷��B*/
		DamageProcessor::SetDamageIndicatorPool(m_result.damageIndicatorPool);
	}


	void InGameBuildHelper::BuildCommentaryUI()
	{
		/* ���{�[���Q�[�WUI�̐����B*/
		m_result.reboneGaugeUIManager = new nsUI::ReboneGaugeUIManager();

		/* ���{�[���Q�[�WUI�̏������B*/
		m_result.reboneGaugeUIManager->Init();

		/* ����UI�̐����B*/
		m_result.commentaryUIManager = NewGO<nsUI::CommentaryUIManager>(0, "CommentaryUIManager");

		/* ���[�h���͕`�悵�Ȃ��B*/
		if (m_result.commentaryUIManager != nullptr)
			m_result.commentaryUIManager->Deactivate();
	}


	void InGameBuildHelper::BuildPlayerGenerator()
	{
		/* PlayerGenerator�̐����B*/
		m_result.generator = new PlayerGenerator();
	}


	void InGameBuildHelper::CreatePartyData()
	{
		/* �p�[�e�B�[�f�[�^�̏������B*/
		m_partyData.clear();

		/* �R���g���[���[�^�C�v�̐ݒ�B*/
		for (int i = 0; i < 4; i++)
		{
			/* �v���C���[�����삵�Ă��邩�ǂ����ŃR���g���[���[�^�C�v��ݒ肷��B*/
			if (m_request.isPlayerControle[i])
				m_controllerType[i] = static_cast<ControllerType>(i);

			/* �v���C���[�����삵�Ă��Ȃ��ꍇ��NPC�ɐݒ肷��B*/
			else
				m_controllerType[i] = ControllerType::NPC;
		}

		/* �p�[�e�B�[�f�[�^�̍쐬�B*/
		m_partyData =
		{
			{
				"player1",
				static_cast<WeaponType>(m_request.characterRole[0]),
				m_controllerType[0],
				INIT_CHARACTER_POSITION_PLAYER1
			},
			{
				"player2",
				static_cast<WeaponType>(m_request.characterRole[1]),
				m_controllerType[1],
				INIT_CHARACTER_POSITION_PLAYER2
			},
			{
				"player3",
				static_cast<WeaponType>(m_request.characterRole[2]),
				m_controllerType[2],
				INIT_CHARACTER_POSITION_PLAYER3
			},
			{
				"player4",
				static_cast<WeaponType>(m_request.characterRole[3]),
				m_controllerType[3],
				INIT_CHARACTER_POSITION_PLAYER4
			}
		};
	}


	void InGameBuildHelper::SpawnPlayer(int playerIndex)
	{
		/* PlayerGenerator����������Ă��Ȃ��ꍇ�͐������Ȃ��B*/
		if (m_result.generator == nullptr)
			return;

		/* �v���C���[�̐����ɕK�v�ȃf�[�^�����݂��邩�B*/
		if (playerIndex < 0 || playerIndex >= static_cast<int>(m_partyData.size()))
			return;

		/* �v���C���[�̐����B*/
		nsActor::Player* player = m_result.generator->SpawnPlayer(m_partyData[playerIndex]);

		/* �v���C���[�̐����ɐ����������B*/
		if (player == nullptr)
			return;

		/* �v���C���[�̓o�^�B*/
		m_players.push_back(player);

		/* ��\�v���C���[��o�^����B*/
		if (m_result.player == nullptr)
			m_result.player = player;

		/* ���[�h���͕`�悵�Ȃ��B*/
		player->Deactivate();
	}


	void InGameBuildHelper::RegisterPlayersToReboneGauge()
	{
		/* ���{�[���Q�[�WUI����������Ă��Ȃ��ꍇ�͓o�^���Ȃ��B*/
		if (m_result.reboneGaugeUIManager == nullptr)
			return;

		/* �����ς݂̃v���C���[�����{�[���Q�[�WUI�ɓo�^����B*/
		for (auto* player : m_players)
			m_result.reboneGaugeUIManager->RegisterPlayer(player);
	}


	void InGameBuildHelper::BuildPlayerHub()
	{
		/* PlayerHub�̐����B*/
		m_result.playerHub = new PlayerControlerHub();

		/* PlayerHub�̏������B*/
		m_result.playerHub->Initialize(m_players, m_partyData);
	}


	void InGameBuildHelper::BuildBoss()
	{
		nsAI::BossParameterTable::LoadTSVFile("Assets/Parameter/BossType.tsv");

		/* �{�X�̐����B*/
		m_result.boss = NewGO<nsActor::Boss>(0, "boss");

		/* �{�X�̐ݒ�B*/
		if (m_result.boss == nullptr)
			return;

		/* �{�X�^�C�v�̐ݒ�B*/
		m_result.boss->SetBossType(static_cast<CharacterModelType>(m_request.bossType + 4));

		/* �^�[�Q�b�g�̐ݒ�B*/
		m_result.boss->SetTarget(m_result.player);

		/* ���[�h���͕`�悵�Ȃ��B*/
		m_result.boss->Deactivate();

	}


	void InGameBuildHelper::BuildCharacterHP()
	{
		/* �L�����N�^�[HP�̐����B*/
		m_result.characterHP = NewGO<nsGame::CharacterHP>(0, "characterHP");

		/* �L�����N�^�[HP�̐ݒ�B*/
		if (m_result.characterHP == nullptr)
			return;

		/* �L�����N�^�[���Ƃ̖�����ݒ�B*/
		for (int i = 0; i < nsGame::CharacterHP::EnCharacter::enCharacter_Num; i++)
			m_result.characterHP->SetCharacterRole(i, m_request.characterRole[i]);

		/* �L�����N�^�[HP�̔�A�N�e�B�u���B*/
		m_result.characterHP->Deactivate();
	}


	void InGameBuildHelper::BuildGameTimeLimit()
	{
		/* �Q�[�����Ԑ����̐����B*/
		m_result.gameTimeLimit = NewGO<nsGame::GameTimeLimit>(0, "gameTimeLimit");

		/* �Q�[�����Ԑ����̐ݒ�B*/
		if (m_result.gameTimeLimit == nullptr)
			return;

		/* �Q�[�����Ԑ����̎��Ԃ�ݒ�B*/
		m_result.gameTimeLimit->SetTimeLimit(180);

		/* �Q�[�����Ԑ����̔�A�N�e�B�u���B*/
		m_result.gameTimeLimit->Deactivate();
	}


	void InGameBuildHelper::BuildGameStartDirection()
	{
		/* �Q�[���J�n���o�̐����B*/
		m_result.gameStartDirection =
			NewGO<nsGame::GameStartDirection>(2, "gameStartDirection");

		/* ��A�N�e�B�u������B*/
		if (m_result.gameStartDirection != nullptr)
			m_result.gameStartDirection->Deactivate();
	}


	void InGameBuildHelper::BuildPause()
	{
		/* �|�[�Y��ʂ̐����B*/
		m_result.pause = NewGO<nsGame::Pause>(0, "pause");

		/* ��A�N�e�B�u������B*/
		if (m_result.pause != nullptr)
			m_result.pause->Deactivate();
	}


	void InGameBuildHelper::FinishBuild()
	{
		/* �����ς݃v���C���[�����ʂɓn���B*/
		m_result.players = m_players;

		/* �����Ɏg�p�����p�[�e�B�[�f�[�^��n���B*/
		m_result.partyData = m_partyData;

		/* ���������B*/
		m_isFinished = true;
	}


	float InGameBuildHelper::GetProgress() const 
	{
		if (m_buildFunctions.empty())
		{
			if (m_isFinished)
				return 1.0f;

			return 0.0f;
		}

		float progress = static_cast<float>(m_currentBuildIndex) / static_cast<float>(m_buildFunctions.size());

		if (progress < 0.0f)
			return 0.0f;

		if (progress > 1.0f)
			return 1.0f;

		return progress;
	}
}