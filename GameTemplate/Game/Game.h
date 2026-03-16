#pragma once

#include "Level3DRender/LevelRender.h"

class Player;


namespace nsApp
{
	class Camera;

	namespace nsGame
	{
		class Game : public IGameObject
		{
		public:
			Game() = default;
			~Game();


		public:
			bool Start();
			void Update();
			void Render(RenderContext& rc)override;


		private:
			nsApp::Camera* m_camera = nullptr;
		};
	}
}

