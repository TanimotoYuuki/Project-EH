#pragma once
#include <Src/Actor/Character/Player/Component/PlayerSpawnData.h>

/**
 * @file  Game2.h
 * @brief �f�[�^�����N���X�B
 * @author All
 * @date  2026/05/08
 */

#include "Src/Parameter/ParameterSystem.h"
#include "Src/Build/InGameBuildHelper.h"
#include "Src/Actor/Character/Boss/Status/BossPhaseEventController.h"

namespace nsApp
{
	class Camera;
	class GameEndSelect;
	class PlayerGenerator;
	class PlayerControlerHub;
	class DamageIndicatorPool;
	class ParameterSystem;

	namespace nsSound { class SoundLister; }
	namespace nsStage { class BackGround; }
	namespace nsActor { class Player; class Boss; }

	namespace nsUI
	{
		class ReboneGaugeUIManager;
		class CommentaryUIManager;
	}

	namespace nsGame
	{
		class CharacterHP;
		class GameTimeLimit;
		class GameStartDirection;
		class GameClearDirection;
		class GameTimeUpDirection;
		class GameOverDirection;
		class Pause;


		/**
		 * @class Game2
		 * @brief �C���Q�[���̊e�I�u�W�F�N�g�̎Q�Ƃ�ێ��E�Ǘ�����f�[�^�����N���X�B
		 */
		class Game2 : public IGameObject
		{
		public:
			/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
			Game2() = default;
			~Game2();


		public:
			/**
			 * @brief �J�n�����B
			 * @return ����ɏ��������ꂽ�ꍇ true�B
			 */
			bool Start();

			/**
			 * @brief �X�V�����B
			 */
			void Update();

			/**
			 * @brief �`�揈���B
			 * @param rc �`��R���e�L�X�g�B
			 */
			void Render(RenderContext& rc);


		public:
			/**
			 * @brief �⏕�N���X�Ő����������ʂ�Game2�ɔ��f����B
			 * @param result InGame�������ʁB
			 */
			void ApplyBuildResult(const InGameBuildResult& result);

			/**
			 * @brief �Q�[���{�̂�����������B
			 */
			void ActivateGame();


		public:
			/**
			 * @brief �Q�[���N���A���o�̃C���X�^���X��Ԃ��B
			 * @return �Q�[���N���A���o�̃C���X�^���X�B
			 */
			inline GameClearDirection* GetGameClearDirectionInstance() const
			{
				return m_gameClearDirection;
			}

			/**
			 * @brief ���Ԑ؂ꉉ�o�̃C���X�^���X��Ԃ��B
			 * @return ���Ԑ؂ꉉ�o�̃C���X�^���X�B
			 */
			inline GameTimeUpDirection* GetGameTimeUpDirectionInstance() const
			{
				return m_gameTimeUpDirection;
			}

			/**
			 * @brief �Q�[���I�[�o�[���o�̃C���X�^���X��Ԃ��B
			 * @return �Q�[���I�[�o�[���o�̃C���X�^���X�B
			 */
			inline GameOverDirection* GetGameOverDirectionInstance() const
			{
				return m_gameOverDirection;
			}

			/**
			 * @brief �Q�[���I����ɑI�����ʂ̃C���X�^���X��Ԃ��B
			 * @return �I�����ʂ̃C���X�^���X�B
			 */
			inline GameEndSelect* GetGameEndSelectInstance() const
			{
				return m_gameEndSelect;
			}

			/**
			 * @brief �|�[�Y�̃C���X�^���X��Ԃ��B
			 * @return �|�[�Y�̃C���X�^���X�B
			 */
			inline Pause* GetPauseInstance() const
			{
				return m_pause;
			}

			/**
			 * @brief �{�X�̎�ނ�ݒ肷��B
			 * @param bossType �{�X�̎�ށB
			 */
			inline void SetBossType(int bossType)
			{
				m_bossType = bossType;
			}

			/**
			 * @brief �{�X�̎�ނ�Ԃ��B
			 * @return �{�X�̎�ށB
			 */
			inline int GetBossType() const
			{
				return m_bossType + 5;
			}

			/**
			 * @brief �L�����N�^�[�̖�����ݒ肷��B
			 * @param playerIndex �v���C���[�̃C���f�b�N�X�B
			 * @param role        �����B
			 */
			inline void SetCharacterRole(int playerIndex, int role)
			{
				m_characterRole[playerIndex] = role;
			}

			/**
			 * @brief �L�����N�^�[�̖�����Ԃ��B
			 * @param playerIndex �v���C���[�̃C���f�b�N�X�B
			 * @return �L�����N�^�[�̖����B
			 */
			inline int GetCharacterRole(int playerIndex) const
			{
				return m_characterRole[playerIndex];
			}

			/**
			 * @brief �v���C���[�����삵�Ă��邩�ǂ�����ݒ肷��B
			 * @param playerIndex �v���C���[�̃C���f�b�N�X�B
			 * @param isControle  ���삵�Ă��邩�B
			 */
			inline void SetPlayerControle(int playerIndex, bool isControle)
			{
				m_isPlayerControle[playerIndex] = isControle;
			}

			/**
			 * @brief �v���C���[�����삵�Ă��邩�ǂ�����Ԃ��B
			 * @param playerIndex �v���C���[�̃C���f�b�N�X�B
			 * @return �v���C���[�����삵�Ă��邩�ǂ����B
			 */
			inline bool IsPlayerControle(int playerIndex) const
			{
				return m_isPlayerControle[playerIndex];
			}


		private:
			/* �|�C���^�^�i�T�E���h�E�w�i�j�B*/
			nsApp::nsSound::SoundLister* m_soundLister = nullptr; //! �T�E���h���X�^�[�B
			nsApp::nsStage::BackGround* m_backGround = nullptr; //! �w�i�B

			/* �|�C���^�^�iUI�j�B*/
			nsUI::ReboneGaugeUIManager* m_reboneGaugeUIManager = nullptr; //! ���{�[���Q�[�WUI�}�l�[�W���[�B
			nsUI::CommentaryUIManager* m_commentaryUIManager = nullptr; //! �R�����^���[UI�}�l�[�W���[�B

			/* �|�C���^�^�i�J�����E�L�����N�^�[�j�B*/
			nsApp::Camera* m_camera = nullptr; //! �J�����B
			nsActor::Player* m_player = nullptr; //! ��\�v���C���[�B
			nsActor::Boss* m_boss = nullptr; //! �{�X�B

			/* �|�C���^�^�i�V�X�e���j�B*/
			DamageIndicatorPool* m_damageIndicatorPool = nullptr; //! �_���[�W�C���W�P�[�^�[�v�[���B
			CharacterHP* m_characterHP = nullptr; //! �L�����N�^�[HP�Ǘ��B
			GameTimeLimit* m_gameTimeLimit = nullptr; //! �Q�[���������ԁB
			GameStartDirection* m_gameStartDirection = nullptr; //! �Q�[���J�n���o�B
			GameClearDirection* m_gameClearDirection = nullptr; //! �Q�[���N���A���o�B
			GameTimeUpDirection* m_gameTimeUpDirection = nullptr; //! ���Ԑ؂ꉉ�o�B
			GameOverDirection* m_gameOverDirection = nullptr; //! �Q�[���I�[�o�[���o�B
			GameEndSelect* m_gameEndSelect = nullptr; //! �Q�[���I����I���B
			Pause* m_pause = nullptr; //! �|�[�Y�B
			PlayerGenerator* m_generator = nullptr; //! �v���C���[������B
			PlayerControlerHub* m_playerHub = nullptr; //! �v���C���[�R���g���[���[�n�u�B
			BossPhaseEventController* m_bossPhaseController = nullptr; //! �{�X�t�F�[�Y�C�x���g�R���g���[���[�B

			/* �I�u�W�F�N�g�^�B*/
			ParameterSystem              m_parameterSystem;                //! �p�����[�^�[�V�X�e���B

			/* �R���e�i�^�B*/
			std::vector<nsActor::Player*> m_players;   //! �v���C���[���X�g�B
			std::vector<PlayerSpawnData>  m_partyData; //! �p�[�e�B�f�[�^���X�g�B


		private:
			/* int �^�B*/
			int m_bossType = 0;    //! �{�X�̎�ށB
			int m_characterRole[4] = { 0 }; //! �L�����N�^�[���Ƃ̖����B

			/* bool �^�B*/
			bool m_isPlayerControle[4] = { true }; //! �v���C���[�����삵�Ă��邩�ǂ����B
			bool m_isGameActive = false;     //! �Q�[���{�̂����������Ă��邩�B
		};
	}
}