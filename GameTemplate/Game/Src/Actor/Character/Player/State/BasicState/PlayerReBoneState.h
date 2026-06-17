#pragma once
/**
 * @file   PlayerReBoneState.h
 * @brief  �v���C���[�̃��{�[����ԃN���X
 * @author YamaguchiHayato
 * @date   202/03/24
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerReBoneState : public IState<nsActor::Actor>
		{
		public:
			/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
			PlayerReBoneState() = default;
			virtual ~PlayerReBoneState() = default;


		public:
			/* ���C�t�T�C�N���B*/
			void Enter() override;
			void Update() override;
			void Exit() override;
			bool RequestID(uint8_t& id) override { return false; };


		public:
			/**
			 * @brief ������Ώۂ�ݒ肷��B
			 * @param target ������Ώۂ̃v���C���[
			 */
			inline void SetTargetCharacter(nsActor::Player* target)
			{
				m_targetPlayer = target;
			}


		private:
			/**
			 * @brief �~���Ώۂ���������B
			 */
			void ResolveTargetCharacter();

			/**
			 * @brief �~���ΏۂƂ��ėL�������肷��B
			 * @return �L���ȋ~���ΏۂȂ�true�B
			 */
			bool IsValidTargetCharacter() const;

			/**
			 * @brief �~���i�s���J�n����B
			 */
			void BeginHelpProgress();

			/**
			 * @brief �~��������̏����B
			 */
			void CompleteHelp();

			/**
			 * @brief �ҋ@��Ԃ֖߂�B
			 */
			void ChangeIdleState();


		private:
			nsActor::Player* m_player = nullptr;       //! �v���C���[�̃|�C���^�B
			nsActor::Player* m_targetPlayer = nullptr; //! ������Ώۂ̃v���C���[�̃|�C���^�B


		private:
			bool m_isHelpStarted = false;              //! �~���i�s���J�n�������B
			bool m_isHelpCompleted = false;            //! �~�������ς݂��B
			bool m_shouldReturnIdle = false;           //! ����Update�őҋ@��Ԃ֖߂����B
		};
	}
}