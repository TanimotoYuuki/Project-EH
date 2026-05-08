#pragma once

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsAI
	{
		class IBossAttackStrategy
		{
		public:
			virtual ~IBossAttackStrategy() = default;

			virtual void Enter(nsActor::Boss* boss) = 0;
			virtual void Update(nsActor::Boss* boss) = 0;
			virtual bool IsEnd() = 0;
		};
	}
}
