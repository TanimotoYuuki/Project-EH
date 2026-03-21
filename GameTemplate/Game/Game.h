#pragma once

#include "Level3DRender/LevelRender.h"

class Player;


namespace nsApp
{
	class Camera;
	namespace nsStage	{
		class BackGround;
	}

	namespace nsActor {
		class Player;
		class Sandbag;
	}

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
			nsStage::BackGround* m_backGround = nullptr;
			nsActor::Player* m_player = nullptr;
			nsActor::Sandbag* m_sandbag = nullptr;
		};
	}
}

