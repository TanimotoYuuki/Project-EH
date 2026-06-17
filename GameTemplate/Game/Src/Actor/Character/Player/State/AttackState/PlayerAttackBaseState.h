#pragma once
/**
 * @file   PlayerAttackBaseState.h
 * @brief  �v���C���[�̍U����Ԃ̊��N���X�B
 * @author Yamaguchi Hayato
 * @date   2026/05/14 �ŏI�X�V���B
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include "Src/Actor/Magic/Factory/MagicFactory.h"
#include "Src/Actor/Character/Player/Component/ComboRouteTable.h"
#include "Src/Actor/Character/NPC/Component/NPCCombatHelper.h"
#include "Src/Actor/Character/Status/AttackParameterTable.h"

 /** @def
  * �v���C���[�̑J�ڏ�Ԃ� PLAYER_STATE_ID�Ƃ������O�Œ�`����}�N���B
  */
#define PLAYER_STATE_ID nsActor::PlayerStateID


namespace nsApp
{
	namespace nsState
	{
		class PlayerAttackBaseState : public IState<nsActor::Actor> 
		{
		public:
			/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
			PlayerAttackBaseState() = default;
			virtual ~PlayerAttackBaseState() = default;


		public:
			/* ���C�t�T�C�N���B*/
			/* �����������B*/
			void Enter() override final;
			/* ���t���[���X�V���鏈���B*/
			void Update() override final;
			/* �`�揈���B*/
			void Exit() override final;
			/* �J�ڗv���B*/
			bool RequestID(uint8_t& id) override
			{
				return OnRequestAttackID(id);
			}


			/* TemplateMethod�B*/
		protected:
			/**
			 * @brief �U���A�j���[�V�����̍Đ���U���^�C�v�̐ݒ�B
			 */
			virtual void PlayAttackAnimation() {};

			/*
			 * @brief �X�e�[�g�J�n���ɍs���ŗL�̏����B
			 */
			virtual void OnEnterAttack() {};

			/**
			 * @brief Enter���̋��ʂ̏����������B
			 */
			void OnCommonInitializeToEnter()
			{
				/* �����蔻��B*/
				m_isHit = false;
				/* ���͗\��B*/
				m_inputRequests.clear();
				/* �U���^�C�}�[�B*/
				SetAttackTimer(0);
				/* �A�ŃJ�E���g�B*/
				m_rushCount = 0;
				/* �_���[�W���B*/
				m_finalDamage = 0;
			}

			/**
			 * @brief ���t���[���X�V���ɍs���ŗL�̏����B
			 */
			virtual bool OnUpdateAttack() { return false; };

			/**
			 * @brief �X�e�[�g�I�����ɍs���ŗL�̏����B
			 */
			virtual void OnExitAttack() {};

			/**
			 * @brief ���̑��̏������s�����߂̊֐��B
			 * @detail �����蔻��̗L��/�e��/���@�̐����^�C�~���O�𐧌�B
			 */
			virtual void OnAttackTick() {};

			/**
			 * @brief �U���̑J�ڗv�����s�����߂̊֐��B
			 */
			virtual bool OnRequestAttackID(uint8_t& id)
			{
				return false;
			}


			/* �Z�b�^�[�B*/
		public:
			/**
			 * @brief �U���̎��Ԃ��Z�b�g�B
			 * @param timer �U���̎��Ԃ��Ǘ�����^�C�}�[�̒l�B
			 */
			inline void SetAttackTimer(int timer)
			{
				m_attackTimer = timer;
			}


			/**
			 * @brief ��������e�ۂ�ʒm����B
			 * @param request �e�ۂ̐����ɕK�v�ȏ����i�[����\���́B
			 * @param type �ʒm����e�ۂ̎�ށB
			 * @detail ���ꂼ��̍U���֐��ł̋��ʍ��̏������֐����B
			 */
			void ConstructAndTransmitBulletRequest(BulletType type)
			{
				/* �e�ۂ̎�ނ��i�[����B*/
				BulletFireRequest request;

				/* �ʒm����e�ۂ̎�ނ�ݒ�B*/
				request.kind = type;

				/* �e�ۂ𐶐�������W�B*/
				request.basePosition = m_spawnPosition;

				/* �e�ۂ̔��˕����B*/
				request.direction = m_forwardDirection;

				/* GunShooter�N���X�ɔ��ˏ������˗��B*/
				m_player->GetGunShooter().Fire(request);
			}

			/**
			 * @brief ���@�̐���
			 * @param type �������閂�@�̎�ށB
			 * @param target ���@�̃^�[�Q�b�g(�f�t�H���g�l�ŏ������ς�)�B
			 * @detail �v���C���[�̍��W�ƌ����������擾���ăZ�b�g����o�[�W�����B
			 */
			void ConstructAndTransmitMagicRequest(nsActor::MagicType type, nsActor::ICharacter* target = nullptr)
			{
				m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();
				m_forwardDirection = m_player->GetForwardVector();

				MagicFactory::CreateMagicObject(
					type,
					m_spawnPosition,
					m_forwardDirection,
					target,
					&m_player->GetEffectList()
				);
			}


			/**
			 * @brief ���@�̐���
			 * @param type �������閂�@�̎�ށB
			 * @param customPos ���@�̐����ʒu���w�肷�邽�߂̈����B
			 * @param customDir ���@�̔��˕������w�肷�邽�߂̈����B
			 * @param target ���@�̃^�[�Q�b�g(�f�t�H���g�l�ŏ������ς�)�B
			 * @detail �v���C���[�̍��W�ƌ������g�p�����A�����Ŏw�肵�����W�ƌ����Ŗ��@�𐶐�����o�[�W�����B
			 */
			inline void ConstructAndTransmitMagicRequest(nsActor::MagicType type, const Vector3& customPos, const Vector3& customDir, nsActor::ICharacter* target = nullptr)
			{
				m_spawnPosition = customPos;
				m_forwardDirection = customDir;

				MagicFactory::CreateMagicObject(
					type,
					customPos,
					customDir,
					target,
					&m_player->GetEffectList()
				);
			}

		 	/**
			 * @brief ���݂̍U���^�C�v���Z�b�g����B
			 * @param attackKind �Z�b�g����U���^�C�v�B
		 	 */
		 	inline void SetCurrentAttackType(AttackType attackKind)
			{
				m_currentAttackType = attackKind;
			}


		/* �Q�b�^�[�B*/
		public:
			/**
			 * @brief ���݂̍U���^�C�v�ɑΉ�����U���p�����[�^�[���擾����B
			 * @return ���݂̍U���^�C�v�ɑΉ�����U���p�����[�^�[�B
			 */
			inline const AttackParameter& GetCurrentAttackParameter() const
			{
				return AttackParameterTable::GetAttackParameter(m_currentAttackType);
			}


		protected:
			/**
			 *  �R���{���L�����m�F���鏈���B
			 * @param currentStateID ���݂̏��ID�B
			 */
			bool CheckCombo(PLAYER_STATE_ID currentStateID, uint8_t& id);

			/**
			 * @brief �_���[�W�e�L�X�g�̕\���ƃ_���[�W�̌v�Z���s�������B
			 * @param target �_���[�W��^����Ώۂ̃L�����N�^�[�B
			 */
			void ApplyDamageToText(nsActor::ICharacter* target);

			/**
			 * @brief �_���[�W�̌v�Z���s�������B
			 */
			int CalculateFinalDamage() const;

			/**
			 * @brief �_���[�W���N�G�X�g�̍\�z���s�������B
			 * @param target �_���[�W��^����Ώۂ̃L�����N�^�[�B
			 * @param damageAmount �_���[�W�̗ʁB
			 * @return �_���[�W���N�G�X�g�\���́B
			 */
			DamageRequest BuildDamageRequest(nsActor::ICharacter* target, int damageAmount) const;

			/**
			 * @brief �U���̏I�����ɋ��ʂ��čs�������B
			 */
			virtual bool UseCommonEndTransition() const
			{
				return true;
			}


		protected:
			/**
			 * @brief CommentaryUI�ɍU�����e��ʒm����B
			 */
			void NotifyAttackCommentary();

			/**
			 * @brief ���݂̍U���^�C�v�������p�e�L�X�g�ɕϊ�����B
			 * @return �����p�̍U�����B
			 */
			std::wstring GetCommentaryActionName() const;


		protected:
			nsActor::Player* m_player = nullptr;                      //! �v���C���[�̃|�C���^�B
			nsActor::Boss* m_boss = nullptr;						  //! �{�X�̃|�C���^�B


		protected:
			int m_attackTimer = 0;                                    //! �U���̎��ԊǗ��p�^�C�}�[�B
			int m_rushCount = 0;                                      //! �A���U���̉񐔊Ǘ��p�J�E���^�[�B
			int m_finalDamage = 0;                                    //! �ŏI�I�ȃ_���[�W�ʂ��Ǘ�����ϐ��B

			bool m_isInputMatch;									  //! ���͂��R���{���[�g�̏����ɍ��v���Ă��邩�B
			bool m_isGrounded;										  //! �n��ɂ��邩�ǂ����B
			bool m_isHit;											  //! �U�����q�b�g�������ǂ����B

			std::unordered_map<ComboInputType, bool> m_inputRequests; //! ���̓^�C�v�ƃt���O��R�Â��B

			AttackType m_currentAttackType = AttackType::None;        //! ���݂̍U���^�C�v�B
			DamageRequest m_damageRequest;						      //! �_���[�W�̌v�Z�ɕK�v�ȏ����i�[����\���́B	

			Vector3 m_forwardDirection = Vector3::Zero;				  //! �v���C���[�̌����Ă���������擾���邽�߂̕ϐ��B
			Vector3 m_spawnPosition = Vector3::Zero;				  //! �e�ۂ̔��ˈʒu���Ǘ�����ϐ��B
		};
	}
}
