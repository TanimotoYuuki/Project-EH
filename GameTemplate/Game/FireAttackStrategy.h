#pragma once
#include "IBossAttackStrategy.h"

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsAI
	{
		class FireAttackStrategy :public IBossAttackStrategy
		{
		public:
			void Enter(nsActor::Boss* boss) override;
			void Update(nsActor::Boss* boss) override;
			bool IsEnd() override;

		private:
			float m_timer = 0.0f;
			bool m_isShot = false;
		};
	}
}