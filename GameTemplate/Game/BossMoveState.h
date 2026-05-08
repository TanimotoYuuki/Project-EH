#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/Common/IState.h"
#include"Boss.h"

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsState
	{
		class BossMoveState :public IState<nsActor::Actor>
		{
		public:
			void Enter() override;
			void Update() override;
			void Exit() override;
			bool RequestID(uint8_t& id) override;

		private:
			nsActor::Boss* m_boss = nullptr;
			float m_timer = 0.0f;

		};
	}
}
