#pragma once

#include <deque>
#include <string>
#include <vector>
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include "Src/UI/Commentary/CommentaryMessage.h"
#include "Src/UI/Commentary/CommentaryUI.h"

namespace nsApp
{
	namespace nsUI
	{
		enum class CommentaryActionType
		{
			NormalAttack,
			HeavyAttack,
			ChargeAttack,
			JumpAttack,
			Help,
			Revive,
			Down,
		};

		enum class CommentaryMessagePriority
		{
			Low,
			Normal,
			High,
			Critical,
		};


		/**
		 * @brief �R�����gUI���Ǘ�����N���X�B
		 */
		class CommentaryUIManager : public IGameObject
		{
		private:
			/**
			 * @brief �\���҂��L���[�̃��b�Z�[�W�B
			 */
			struct QueuedCommentaryMessage
			{
				CommentaryMessage         message;  //! ���b�Z�[�W�{�́B
				CommentaryMessagePriority priority; //! ���b�Z�[�W�D��x�B
			};

		public:
			/**
			 * @brief �J�n�����B
			 * @return �������ɐ��������ꍇtrue�B
			 */
			bool Start() override;

			/**
			 * @brief �X�V�����B
			 */
			void Update() override;

			/**
			 * @brief �`�揈���B
			 * @param rc �`��R���e�L�X�g�B
			 */
			void Render(RenderContext& rc) override;


		public:
			/**
			 * @brief �R�����g��ǉ�����B
			 * @param text �ǉ�����R�����g�B
			 */
			void AddMessage(const std::wstring& text);

			/**
			 * @brief �D��x�t���ŃR�����g��ǉ�����B
			 * @param text     �ǉ�����R�����g�B
			 * @param priority ���b�Z�[�W�D��x�B
			 */
			void AddMessage(const std::wstring& text, CommentaryMessagePriority priority);

			/**
			 * @brief ����ƍs��������R�����g��ǉ�����B
			 * @param weapon     ����̎�ށB
			 * @param actionName �s�����B
			 */
			void AddActionMessage(WeaponType weapon, const std::wstring& actionName);

			/**
			 * @brief ����ƍs���^�C�v����R�����g��ǉ�����B
			 * @param weapon     ����̎�ށB
			 * @param actionType �s���̎�ށB
			 */
			void AddActionMessage(WeaponType weapon, CommentaryActionType actionType);

			/**
			 * @brief �L���[��\�����𖳎����đ����ɋ����\������B
			 * @param text        �\������R�����g�B
			 * @param displayTime �\�����ԁi�b�j�B�ȗ����̓f�t�H���g�l��2�{�B
			 */
			void ForceShowMessage(const std::wstring& text, float displayTime = MESSAGE_DISPLAY_TIME * 2.0f);


		private:
			/**
			 * @brief �\�����Ԃ𒴂������b�Z�[�W���폜����B
			 */
			void RemoveExpiredMessages();

			/**
			 * @brief �\�����̃��b�Z�[�W���Ȃ���΁A�ҋ@�񂩂玟�̃��b�Z�[�W��\������B
			 */
			void StartNextMessageIfNeeded();

			/**
			 * @brief ���b�Z�[�W��ǉ����Ă悢�����肷��B
			 * @param text     ���ׂ�R�����g�B
			 * @param priority ���b�Z�[�W�D��x�B
			 * @retval true  �ǉ����Ă悢�B
			 * @retval false �ǉ����Ȃ��B
			 */
			bool CanAcceptMessage(const std::wstring& text, CommentaryMessagePriority priority) const;

			/**
			 * @brief ���O�̃��b�Z�[�W�Ɠ������e�����ׂ�B
			 * @param text ���ׂ�R�����g�B
			 * @retval true  ���O�Ɠ������e�B
			 * @retval false ���O�ƈقȂ���e�B
			 */
			bool IsSameAsLatestMessage(const std::wstring& text) const;

			/**
			 * @brief �ҋ@�񂪋l�܂肷���Ȃ��悤�ɐ�������B
			 * @param priority �ǉ��\�胁�b�Z�[�W�̗D��x�B
			 */
			void TrimWaitingMessages(CommentaryMessagePriority priority);

			/**
			 * @brief �ҋ@�񂩂�Â���D��x���b�Z�[�W��1���폜����B
			 * @retval true  �폜�����B
			 * @retval false �폜�Ώۂ��Ȃ��B
			 */
			bool RemoveOldestLowPriorityWaitingMessage();

			/**
			 * @brief ���햼���擾����B
			 * @param weapon ����̎�ށB
			 * @return �\���p�̕��햼�B
			 */
			std::wstring GetWeaponName(WeaponType weapon) const;

			/**
			 * @brief �s���^�C�v����\���p�̍s�������擾����B
			 * @param actionType �s���̎�ށB
			 * @return �\���p�̍s�����B
			 */
			std::wstring GetActionName(CommentaryActionType actionType) const;

			/**
			 * @brief �s���^�C�v����D��x���擾����B
			 * @param actionType �s���̎�ށB
			 * @return ���b�Z�[�W�D��x�B
			 */
			CommentaryMessagePriority GetPriorityFromActionType(CommentaryActionType actionType) const;

			/**
			 * @brief �s��������D��x���擾����B
			 * @param actionName �s�����B
			 * @return ���b�Z�[�W�D��x�B
			 */
			CommentaryMessagePriority GetPriorityFromActionName(const std::wstring& actionName) const;


		private:
			static const size_t MAX_QUEUE_COUNT;      //! �ő�ҋ@���b�Z�[�W���B
			static const float  MESSAGE_DISPLAY_TIME; //! 1���b�Z�[�W�̕\�����ԁB
			static const float  SAME_MESSAGE_COOLDOWN;   //! ���ꃁ�b�Z�[�W�Ԋu���ԁB
			static const float  LOW_PRIORITY_COOLDOWN;   //! ��D��x���b�Z�[�W�Ԋu���ԁB

			std::vector<CommentaryMessage>          m_messages;        //! �\�����̎��̃��b�Z�[�W�B���0���܂���1���ɂ���B
			std::deque<QueuedCommentaryMessage>     m_waitingMessages; //! �\���҂��̎��̃��b�Z�[�W�B

			CommentaryUI m_commentaryUI; //! �R�����gUI�`��N���X�B

			std::wstring m_latestAcceptedText;       //! �Ō�Ɏ󗝂����e�L�X�g�B
			float        m_sameMessageCooldownTimer; //! ���ꃁ�b�Z�[�W�Ԋu�p�^�C�}�[�B
			float        m_lowPriorityCooldownTimer; //! ��D��x���b�Z�[�W�Ԋu�p�^�C�}�[�B
		};
	}
}