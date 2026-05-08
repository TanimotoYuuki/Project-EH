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
			PlayerGuardState() = default;
			~PlayerGuardState() = default;


		public:
			void Enter() override;
			void Update() override;
			void Exit() override;
			bool RequestID(uint8_t& id)override;


		private:
			nsActor::Player* m_player = nullptr;
		};
	}
}
