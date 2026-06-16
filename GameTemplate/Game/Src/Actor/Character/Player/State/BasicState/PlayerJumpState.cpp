#include "stdafx.h"
#include "PlayerJumpState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerAirAttackState.h"
#include "Src/Actor/Character/Player/Component/StateTransitionDiagram.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerSlashUpState.h"

namespace
{
		const auto MOVE_FRAME_SPEED = 1.0 / 60.0f; /* �O�i����t���[�����B*/
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerJumpState::Enter()
		{
			/* �v���C���[�̃|�C���^���擾����B */
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* �W�����v�A�j���[�V�������Đ�����B */
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Jump);

			/* ���n���Ă���ꍇ�A�W�����v���̏��������߂�B*/
			if (m_player->GetCharacterController().IsOnGround())
				m_jumpVelocity = m_player->GetJumpPower();
		}


		void PlayerJumpState::Update()
		{
			/* ���t���[���A�ړ����x���[���Ƀ��Z�b�g����B */
			m_moveSpeed = Vector3::Zero;
			/* ���͏����擾����B */
			const auto& inputClass = m_player->GetInputClass();

			/* �󒆂ł̃{�^���A�N�V�����ōU����ԂɑJ�ځB*/
			if (inputClass.IsAirAttack())
			{
				auto airAttack = new PlayerAirAttackState();
				airAttack->SetFallVelocity(m_jumpVelocity);
				m_stateMachine->ChangeState(airAttack);
				return;
			}


			/* �X�e�B�b�N���͂�����΁A�󒆂�X�EZ���̈ړ��ʂ��v�Z����B */
			if (inputClass.IsMove())
			{
				m_moveSpeed.x = inputClass.GetMoveVector().x * m_player->GetAirMoveSpeed();
				m_moveSpeed.z = inputClass.GetMoveVector().y * m_player->GetAirMoveSpeed();
			}

			/* �󒆂ŃX�e�B�b�N�� + A���������a��グ��Ԃ�*/
			if (inputClass.IsSlashUp())
			{
				m_stateMachine->ChangeState(new PlayerSlashUpState());
				return;
			}

			/* �d�͌v�Z�i���t���[���Œ�̗͂ŉ��ֈ�������j�B */
			m_jumpVelocity -= m_player->GetGravity();

			/* �����X�s�[�h�̌��E��ݒ�i���蔲���Ȃǂ̃o�O�h�~�j�B */
			if (m_jumpVelocity < m_player->GetMaxFallVelocity())
				m_jumpVelocity = m_player->GetMaxFallVelocity();

			/* �v�Z����Y���̑��x���ړ����x�x�N�g���ɐݒ肷��B */
			m_moveSpeed.y = m_jumpVelocity;

			/* �L�����N�^�[�R���g���[���[���g���Ĉړ����������s����B */
			m_player->GetCharacterController().Execute(m_moveSpeed, MOVE_FRAME_SPEED);

			/* �v�Z���ʂ̈��S�ȍ��W���v���C���[�{�̂ɐݒ肷��B */
			m_player->SetPosition(m_player->GetCharacterController().GetPosition());

			/* ���n���ȂǂɃL�����R�����v�Z����Y���̑��x��ϐ��ɔ��f����B */
			m_jumpVelocity = m_moveSpeed.y;
		}


		bool PlayerJumpState::RequestID(uint8_t& id)
		{
			/* ���n������Idle�ɖ߂�B*/
			if (m_player->GetCharacterController().IsOnGround())
			{
				id = static_cast<uint8_t>(nsActor::PlayerStateID::enIdle);
				return true;
			}
			return false;
		}
	}
}