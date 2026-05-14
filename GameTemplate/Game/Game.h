#pragma once

/**
* @file Game.h
* @brief データの統合クラス。
* @author All
* @date 2026/05/08
*/


namespace nsApp
{
	class Camera;
	class GameEndSelect;
	class PlayerGenerator;

	namespace nsSound {
		class SoundLister;
	}

	namespace nsStage {
		class BackGround;
	}

	namespace nsActor {
		class Player;
		class Boss;
	}

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
			/* コンストラクタとデストラクタ。*/
			Game() = default;
			~Game();
			

		public:
			/* サイクル。*/
			bool Start();
			void Update();
			void Render(RenderContext& rc);


		private:
			/* プレイアブルキャラを生成する。*/
			void SpawnPlayCharacter();


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
			nsApp::nsSound::SoundLister* m_soundLister = nullptr; //! サウンドリスター。
			nsApp::nsStage::BackGround* m_backGround = nullptr; //! 背景。
			nsApp::Camera* m_camera = nullptr;
			nsActor::Player* m_player = nullptr;
			nsActor::Boss* m_boss = nullptr;
			CharacterHP* m_characterHP = nullptr;
			GameTimeLimit* m_gameTimeLimit = nullptr;
			GameStartDirection* m_gameStartDirection = nullptr;
			GameClearDirection* m_gameClearDirection = nullptr;
			GameTimeUpDirection* m_gameTimeUpDirection = nullptr;
			GameOverDirection* m_gameOverDirection = nullptr;
			GameEndSelect* m_gameEndSelect = nullptr;
			PlayerGenerator* m_generator = nullptr;
		};
	}
}

