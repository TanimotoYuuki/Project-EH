#pragma once
/**
 * @file   PlayerJumpState.h
 * @brief  Player��Jump��Ԃ��Ǘ�����N���X�B
 * @author Yamaguchi Hayato
 * @date   2026/03/15
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		/**
		 * @brief �v���C���[�̃W�����v��Ԃ���������X�e�[�g�N���X
		 */
		class PlayerJumpState : public IState<nsActor::Actor>
		{
		public:
			/**
			 * @brief �R���X�g���N�^
			 */
			PlayerJumpState() = default;

			/**
			 * @brief �f�X�g���N�^
			 */
			virtual ~PlayerJumpState() = default;

		public:
			/**
			 * @brief �X�e�[�g�ɓ��������̏���������
			 */
			void Enter() override;

			/**
			 * @brief ���t���[���̍X�V����
			 */
			void Update() override;

			/**
			 * @brief �X�e�[�g�𔲂��鎞�̏I������
			 */
			void Exit() override {};

			/**
			 * @brief ���̃X�e�[�g�ւ̑J�ڏ����𔻒肷��
			 * @param[out] id ���̃X�e�[�gID
			 * @retval true �J�ڃ��N�G�X�g����
			 * @retval false �J�ڃ��N�G�X�g�Ȃ�
			 */
			bool RequestID(uint8_t& id) override;


		/* �Z�b�^�[�B*/
		public:
			/**
			 * @brief �W�����v���̈ړ����x��ݒ肷��B
			 * @param velocity �ړ����x�B
			 */
			inline void SetJumpVelocity(float velocity) 
			{
				m_jumpVelocity = velocity; 
			}


		private:
			nsActor::Player* m_player = nullptr;    //!�v���C���[�̎��̂ւ̃|�C���^�B


		private:
			Vector3 m_moveSpeed = Vector3::Zero;    //! �W�����v���̈ړ����x�B

			float m_jumpVelocity = 0.0f;            //! �W�����v�̏����B 
		};
	}
}