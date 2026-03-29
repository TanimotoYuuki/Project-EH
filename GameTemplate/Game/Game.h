#pragma once

#include "Level3DRender/LevelRender.h"

class Player;


namespace nsApp
{
	class Camera;
	class GameEndSelect;

	namespace nsGame
	{
		class CharacterHP;
		class GameTimeLimit;
		class GameStartDirection;
		class GameClearDirection;
		class GameTimeUpDirection;
		class GameOverDirection;
		class Game : public IGameObject
		{
		public:
			Game() = default;
			~Game();


		public:
			bool Start();
			void Update();
			void Render(RenderContext& rc)override;

		public:
			
			/**
			* @brief ゲームクリア演出のインスタンスの取得。
			* @return ゲームクリア演出のインスタンス。
			*/
			inline GameClearDirection* GetGameClearDirectionInstance() const
			{
				return m_gameClearDirection;
			}

			/**
			* @brief 時間切れ演出のインスタンスの取得。
			* @return 時間切れ演出のインスタンス。
			*/
			inline GameTimeUpDirection* GetGameTimeUpDirectionInstance() const
			{
				return m_gameTimeUpDirection;
			}

			/**
			* @brief ゲームオーバー演出のインスタンスの取得。
			* @return ゲームオーバー演出のインスタンス。
			*/
			inline GameOverDirection* GetGameOverDirectionInstance() const
			{
				return m_gameOverDirection;
			}

			/**
			* @brief ゲームが終了した時に選択する画面のインスタンスの取得。
			* @return ゲームが終了した時に選択する画面のインスタンス。
			*/
			inline GameEndSelect* GetGameEndSelectInstance() const
			{
				return m_gameEndSelect;
			}

		private:
			nsApp::Camera* m_camera = nullptr;
			CharacterHP* m_characterHP = nullptr;
			GameTimeLimit* m_gameTimeLimit = nullptr;
			GameStartDirection* m_gameStartDirection = nullptr;
			GameClearDirection* m_gameClearDirection = nullptr;
			GameTimeUpDirection* m_gameTimeUpDirection = nullptr;
			GameOverDirection* m_gameOverDirection = nullptr;
			GameEndSelect* m_gameEndSelect = nullptr;
		};
	}
}

