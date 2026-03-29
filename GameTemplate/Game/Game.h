#pragma once
/**
* @file Game.h
* @brief データの統合クラス。
* @author All
* @date 2026/03/23
*/

#include "Level3DRender/LevelRender.h"

class Player;


namespace nsApp
{
	class Camera;

	

	namespace nsActor {
		class Player;
		class Sandbag;
	}

	namespace nsGame
	{
		class Game : public IGameObject
		{
		public:
			/* コンストラクタとデストラクタ。*/
			Game() = default;
			~Game();


		public:
			/* サイクル。*/
			bool Start();
			void Update();
			void Render(RenderContext& rc)override;


		private:
			nsApp::Camera* m_camera = nullptr;     //! カメラ。
			nsActor::Player* m_player = nullptr;   //! プレイヤー。
			nsActor::Player* m_player2 = nullptr;  //! プレイヤー2。
			nsActor::Sandbag* m_sandbag = nullptr; //! サンドバッグ。
		};
	}
}

