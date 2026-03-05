#pragma once

#include "Level3DRender/LevelRender.h"

class Player;


namespace nsApp
{
	namespace nsGame
	{
		class Game : public IGameObject
		{
		public:
			Game() = default;
			~Game() = default;


		public:
			bool Start();
			void Update();
			void Render(RenderContext& rc)override;
		};
	}
}

