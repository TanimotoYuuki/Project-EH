#pragma once

#include "Src/Actor/Character/ICharacter.h" 

namespace nsApp
{
	namespace nsActor
	{
		class Boss : public ICharacter
		{
		public:
			Boss() = default;
			~Boss() = default;

			bool Start() override;
			void Update() override;
		};
	}
}
