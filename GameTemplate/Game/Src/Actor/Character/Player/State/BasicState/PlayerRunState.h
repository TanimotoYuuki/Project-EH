#pragma once
/**
* @file   PlayerRunState.h
* @brief  Player��Run��Ԃ��Ǘ�����N���X�B
* @author Yamaguchi Hayato
* @date   2026/03/15
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsActor {
		class Player;
	}


	namespace nsState
	{
		class PlayerRunState : public IState<nsActor::Actor>
		{ 
		public:
			/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
			PlayerRunState() = default;
			virtual ~PlayerRunState() = default;


		public:
			/* ���C�t�T�C�N���B*/
			void Enter() override;
			void Update() override;
			void Exit() override {};
			bool RequestID(uint8_t& id) override;


		/* �Z�b�^�[�B*/
		public:
			/**
			 * @brief ���鑬�x��ݒ�B
			 * @param speed ���鑬�x�B
			 */
			inline void SetRunSpeed(float speed) 
			{
				m_runSpeed = speed;
			}

			/**
			 * @brief ���݂̍��W��ݒ�B
			 * @param position ���݂̍��W�B
			 */
			inline void SetPosition(const Vector3& position) 
			{
				m_currentPosition = position;
			}

			/**
			 * @brief �ړ�������ݒ�B
			 * @param direction �ړ������̃x�N�g���B
			 */
			inline void SetMoveDirection(const Vector3& direction) 
			{
				m_moveDirection = direction;
			}
			

		private:	
			nsActor::Player* m_player = nullptr;		//! Player�ւ̃|�C���^�B


		private:
			float m_runSpeed;                           //! ���鑬�x�B

			Vector3 m_moveVector = Vector3::Zero;       //! ����ۂ̈ړ��x�N�g���B
			Vector3 m_currentPosition = Vector3::Zero;  //! ���݂̍��W�B
			Vector3 m_moveDirection = Vector3::Zero;    //! �ړ��x�N�g���B
		};
	}
}
