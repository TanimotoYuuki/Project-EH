#pragma once

/**
 * @file   InGameBuildHelper.h
 * @brief  InGame�����⏕�N���X�B
 * @author Yamaguchi Hayato�B
 * @date   2026/06/05
 * @detail Game2�ŕK�v�Ȑ���������1�X�e�b�v�����s����B
 */

#include <vector>
#include <functional>

#include "Src/Actor/Character/Player/Component/PlayerSpawnData.h"

namespace nsApp
{
	class Camera;
	class PlayerGenerator;
	class PlayerControlerHub;
	class DamageIndicatorPool;

	namespace nsSound {
		class SoundLister;
	}

	namespace nsStage {
		class BackGround;
	}

	namespace nsActor {
		class Player;
		class Boss;
	}

	namespace nsUI {
		class ReboneGaugeUIManager;
		class CommentaryUIManager;
	}

	namespace nsGame {
		class CharacterHP;
		class GameTimeLimit;
		class GameStartDirection;
		class Pause;
	}


	/**
	 * @struct InGameBuildRequest
	 * @brief InGame�����ɕK�v�ȃ��N�G�X�g���B
	 */
	struct InGameBuildRequest
	{
		int bossType = 0;
		int characterRole[4] = { 0 };
		bool isPlayerControle[4] = { true, false, false, false };
	};


	/**
	 * @struct InGameBuildResult
	 * @brief InGame�������ʁB
	 */
	struct InGameBuildResult
	{
		nsSound::SoundLister* soundLister = nullptr;
		nsStage::BackGround* backGround = nullptr;
		nsUI::ReboneGaugeUIManager* reboneGaugeUIManager = nullptr;
		nsUI::CommentaryUIManager* commentaryUIManager = nullptr;
		Camera* camera = nullptr;
		nsActor::Player* player = nullptr;
		std::vector<nsActor::Player*> players;
		std::vector<PlayerSpawnData> partyData;    
		nsActor::Boss* boss = nullptr;
		DamageIndicatorPool* damageIndicatorPool = nullptr;
		nsGame::CharacterHP* characterHP = nullptr;
		nsGame::GameTimeLimit* gameTimeLimit = nullptr;
		nsGame::GameStartDirection* gameStartDirection = nullptr;
		nsGame::Pause* pause = nullptr;
		PlayerGenerator* generator = nullptr;
		PlayerControlerHub* playerHub = nullptr;
	};


	/**
	 * @brief InGame�����⏕�N���X�B
	 */
	class InGameBuildHelper
	{
	public:
		InGameBuildHelper() = default;
		virtual ~InGameBuildHelper() = default;


	public:
		/**
		 * @brief �����������B
		 * @param request �����ɕK�v�ȏ��B
		 */
		void Initialize(const InGameBuildRequest& request);

		/**
		 * @brief �X�V�����B
		 */
		void Update()
		{
			/* �������������Ă���ꍇ�͉������Ȃ��B*/
			ExecuteNextBuildFunction();
		}


		/* �Q�b�^�[�B*/
	public:
		/**
		 * @brief �������������Ă��邩�B
		 * @return �������������Ă���ꍇ��true�B
		 */
		inline bool IsFinished() const
		{
			return m_isFinished;
		}

		/**
		 * @brief �������ʂ��擾����B
		 * @return �������ʁB
		 */
		inline const InGameBuildResult& GetResult() const
		{
			return m_result;
		}

		/**
		 * @brief �i�������擾����B
		 */
		float GetProgress() const;


	private:
		using BuildFunction = std::function<void()>;


	private:
		/**
		 * @brief �����֐���o�^����B
		 */
		void InitializeBuildFunctions();

		/**
		 * @brief ���̐����֐���1���s����B
		 */
		void ExecuteNextBuildFunction();

		/**
		 * @brief �����̐����B
		 */
		void BuildRandom();

		/**
		 * @brief �����̐����B
		 */
		void BuildSound();

		/**
		 * @brief �X�e�[�W�̐����B
		 */
		void BuildStage();

		/**
		 * @brief �w�i�̐����B
		 */
		void BuildBackGround();

		/**
		 * @brief �J�����̐����B
		 */
		void BuildCamera();

		/**
		 * @brief �_���[�W�C���W�P�[�^�[�̃v�[���̐����B
		 */
		void BuildDamagePool();

		/**
		 * @brief ����UI�̐����B
		 */
		void BuildCommentaryUI();

		/**
		 * @brief PlayerGenerator�̐����B
		 */
		void BuildPlayerGenerator();

		/**
		 * @brief �v���C���[�̐����ɕK�v�ȃf�[�^�̍쐬�B
		 */
		void CreatePartyData();

		/**
		 * @brief �v���C���[�̐����B
		 * @param playerIndex �v���C���[�̃C���f�b�N�X�B
		 */
		void SpawnPlayer(int playerIndex);

		/**
		 * @brief �v���C���[�����{�[���Q�[�W�ɓo�^����B
		 */
		void RegisterPlayersToReboneGauge();

		/**
		 * @brief PlayerHub�̐����B
		 */
		void BuildPlayerHub();

		/**
		 * @brief Boss�̐����B
		 */
		void BuildBoss();

		/**
		 * @brief CharacterHP�̐����B
		 */
		void BuildCharacterHP();

		/**
		 * @brief �Q�[�����Ԑ����̐����B
		 */
		void BuildGameTimeLimit();

		/**
		 * @brief �Q�[���J�n���o�̐����B
		 */
		void BuildGameStartDirection();

		/**
		 * @brief �|�[�Y�̐����B
		 */
		void BuildPause();

		/**
		 * @brief �������������B
		 */
		void FinishBuild();


	private:
		InGameBuildRequest m_request;//! �����ɕK�v�ȏ��B
		InGameBuildResult m_result;  //! �������ʁB

		std::vector<BuildFunction> m_buildFunctions; //! �����֐��̃��X�g�B
		std::vector<PlayerSpawnData> m_partyData;    //! �v���C���[�̐����ɕK�v�ȃf�[�^�̃��X�g�B
		std::vector<nsActor::Player*> m_players;     //! �������ꂽ�v���C���[�̃��X�g�B

		ControllerType m_controllerType[4] = { ControllerType::Player_1P }; //! �L�����N�^�[���Ƃ̃R���g���[���[�̎�ށB

		bool m_isFinished = false; //! �������������Ă��邩�B 
			
		int m_currentBuildIndex = 0; //! ���ݎ��s���鐶���֐��̃C���f�b�N�X�B
	};
}