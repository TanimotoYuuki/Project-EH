#pragma once
/**
 * @file   PlayerDethState.h
 * @brief  �v���C���[�̎��S��Ԃ��Ǘ�����N���X�B
 * @author Yamaguchi Hayato�B
 * @date   2026/06/03:�ŏI�X�V��
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerDethState : public IState<nsActor::Actor>
		{
		public:
			/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
			PlayerDethState() = default;

			explicit PlayerDethState(const Vector3& desathPos)
				: m_dethPosition(desathPos)
				, m_hasDeathPosition(true) {}

			virtual ~PlayerDethState() = default;


		public:
		    /* ���C�t�T�C�N���B*/
			void Enter() override;
			void Update() override {};
			void Exit() override {};
			bool RequestID(uint8_t& id) override { return false; };


		private:
			nsActor::Player* m_player = nullptr;	   //! �v���C���[�̎��Ԃւ̃|�C���^�B


		private:
			Vector3 m_fallSpeed;					   //! �������x�B
			Vector3 m_dethPosition = Vector3::Zero;    //! ���S�����ʒu�B

			bool m_hasDeathPosition = false;		   //! ���S�ʒu���ݒ肳�ꂽ���ǂ����̃t���O�B

		};
	}
}
