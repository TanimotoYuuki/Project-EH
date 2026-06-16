#include "stdafx.h"
#include "WeaponHitDetection.h"

namespace
{
	const auto HIT_RADIUS = 30.0f; //! ����̓����蔻��̔��a�B
}


namespace nsApp
{
	void WeaponHitDetection::Init(float radius)
	{
		/* �����蔻����Z�b�g����B*/
		m_radius = radius;
		/* �������蔻����Z�b�g����B*/
		SetRadius(m_radius);
	}


	bool WeaponHitDetection::IsHit(nsActor::ICharacter* target)
	{
		/* ���킪�L���łȂ��A�������͓G�����Ȃ��ꍇ�͓�����Ȃ��B*/
		if(!m_isActive || !target)
			return false;

		/* ���łɎa�������X�g�̓G�����邩�`�F�b�N�B*/
		for (auto hitTarget : m_hitTargets)
		{
			if (hitTarget == target)
				return false;
		}

		/* �G�̍��W�ƕ���̍��W��}��B*/
		m_targetPosition = target->GetPosition();
		m_diffVector = m_position - m_targetPosition;

		/* �������v��B*/
		m_diff = m_diffVector.Length();

		if (m_diff < (m_radius + 30.0f))
		{
			/* ���������G�����X�g�ɓo�^�B*/
			m_hitTargets.push_back(target);
			return true;
		}

		return false;
	}
}