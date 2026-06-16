#pragma once
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerGuardState : public IState<nsActor::Actor>
		{
		public:
			/* �R���X�g���N�^�ƃf�X�g���N�^�B*/
			PlayerGuardState() = default;
			~PlayerGuardState() = default;


		public:
			/* ���C�t�T�C�N���B*/
			void Enter() override;
			void Update() override;
			void Exit() override {};
			bool RequestID(uint8_t& id)override { return false; };


		private:
			nsActor::Player* m_player = nullptr; //! �v���C���[�ւ̃|�C���^�B
		};
	}
}
