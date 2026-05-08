#pragma once
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/Common/IState.h"

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsState
	{
		class BossDethState :public IState<nsActor::Actor>
		{
		public:
			void Enter() override;
			void Update() override;
			void Exit() override;
			bool RequestID(uint8_t& id) override;

		private:
			nsActor::Boss* m_boss = nullptr;
		};
	}
}