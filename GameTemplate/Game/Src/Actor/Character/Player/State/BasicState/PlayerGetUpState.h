#pragma once
/**
 * @file   PlayerGetUpState.h
 * @brief  �v���C���[�̋N���オ���Ԃ��Ǘ�����N���X�B
 * @author Yamaguchi Hayato
 * @date   2026/06/03: �ŏI�X�V���B
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerGetUpState : public IState<nsActor::Actor>
		{
		public:
			/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
			PlayerGetUpState() = default;
			virtual ~PlayerGetUpState() = default;


		public:
			/* ���C�t�T�C�N���B*/
			void Enter() override;
			void Update() override;
			void Exit() override {};
			bool RequestID(uint8_t& id) override { return false; };


		private:
			nsActor::Player* m_player = nullptr; //! �v���C���[�̃|�C���^�B
		};
	}
}

