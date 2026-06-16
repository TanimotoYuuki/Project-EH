#pragma once
/**
* @file   WeaponHitDetection.h
* @brief  ����̓����蔻����Ǘ�����N���X�B
* @author Yamaguchi Hayato
* @date   2026/05/29
*/

#include "Src/Actor/Character/Common/ICharacter.h"

namespace nsApp
{
	class WeaponHitDetection
	{
	public:
		/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
		WeaponHitDetection() = default;
		virtual ~WeaponHitDetection() = default;


	public:
		/** 
		 * @brief�����蔻��̏����������B
		 * @param radius ����̓����蔻��̔��a�B
		 **/
		void Init(float radius);

		/**
		 * @brief�����蔻��̍X�V�����B
		 * @param weaponPosition ����̌��݂̍��W�B
		 */
		inline void Update(const Vector3& weaponPosition)
		{
			/* ���W��ێ�����B*/
			m_position = weaponPosition;
		}

		/**
		 * @brief�`�揈���B
		 * @param rc �����_�����O�R���e�L�X�g�B
		 */
		void Render(RenderContext& rc);


	public:
		/**
		 * @brief �����蔻����I���B
		 */
		inline void Enable()
		{
			m_isActive = true;
		}

		/**
		 * @brief �����蔻����I�t�B
		 */
		inline void Disable()
		{
			m_isActive = false;
			m_hitTargets.clear();
		}


	/* �Z�b�^�[�B*/
	public:
		/**
		 * @brief �����蔻��̔��a��ݒ�B
		 * @param radius �����蔻��̔��a�B
		 */
		inline void SetRadius(float radius)
		{
			m_radius = radius;
		}


	/* �Q�b�^�[�B*/
	public:
        /**
		 * @brief �w�肵���^�[�Q�b�g�������蔻��Ƀq�b�g���Ă��邩�ǂ����𔻒�B
		 * @param target ����Ώۂ̃L�����N�^�[�B
		 * @return �^�[�Q�b�g�������蔻��Ƀq�b�g���Ă���ꍇ��true�A�����łȂ��ꍇ��false�B
         */
        bool IsHit(nsActor::ICharacter* target);

		/**
		 * @brief �����蔻��̒��S���W���擾�B
		 * @return �����蔻��̒��S���W�B
		 */
		inline const Vector3& GetPosition()
		{
			return m_position;
		}


	private:
		std::vector<nsActor::ICharacter*> m_hitTargets; //! ���������Ώۂ̃��X�g�B


	private:
		Vector3 m_position = Vector3::Zero;             //! �����蔻��̒��S���W�B
		Vector3 m_targetPosition = Vector3::Zero;       //! �^�[�Q�b�g�̍��W�B
		Vector3 m_diffVector = Vector3::Zero;           //! ����ƃ^�[�Q�b�g�̋����x�N�g���B

		float m_radius = 0.0f;                          //! �����蔻��̔��a�B
		float m_diff = 0.0f;                            //! ����ƃ^�[�Q�b�g�̋����B

		bool m_isActive = false;                        //! �����蔻�肪�L�����ǂ����B
	};
}

