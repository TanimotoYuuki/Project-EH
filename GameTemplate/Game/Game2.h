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
	class PlayerControlerHub;
	class DamageIndicatorPool;

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

	namespace nsUI {
		class ReboneGaugeUIManager;
		class CommentaryUIManager;
	}

	namespace nsGame
	{
		class CharacterHP;
		class GameTimeLimit;
		class GameStartDirection;
		class GameClearDirection;
		class GameTimeUpDirection;
		class GameOverDirection;

		class Game2 : public IGameObject
		{
		public:
			/* コンストラクタとデストラクタ。*/
			Game2() = default;
			~Game2();


		public:
			/* サイクル。*/
			bool Start();
			void Update();
			void Render(RenderContext& rc);


		private:
			/* プレイアブルキャラを生成する。*/
			void SpawnPlayCharacter();

			/**
			 * @brief 音源クラスの初期設定に関する関数群を呼び出す。
			 */
			void SettingSound();

			/**
			 * @brief 実況UIの初期設定に関する関数群を呼び出す。
			 */
			void SettingCommentaryUI();


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
			nsUI::ReboneGaugeUIManager* m_reboneGaugeUIManager = nullptr;
			nsUI::CommentaryUIManager* m_commentaryUIManager = nullptr;
			nsApp::Camera* m_camera = nullptr;
			nsActor::Player* m_player = nullptr;
			nsActor::Boss* m_boss = nullptr;
			DamageIndicatorPool* m_damageIndicatorPool = nullptr;
			CharacterHP* m_characterHP = nullptr;
			GameTimeLimit* m_gameTimeLimit = nullptr;
			GameStartDirection* m_gameStartDirection = nullptr;
			GameClearDirection* m_gameClearDirection = nullptr;
			GameTimeUpDirection* m_gameTimeUpDirection = nullptr;
			GameOverDirection* m_gameOverDirection = nullptr;
			GameEndSelect* m_gameEndSelect = nullptr;


			////////////////////////////////////////////////////////
			//Player生成: メンバ選択クラスに移動をするなら移動して
			////////////////////////////////////////////////////////
			PlayerGenerator* m_generator = nullptr;
			PlayerControlerHub* m_playerHub = nullptr;


			/**
			 * @brief デバッグ用の回復テスト関数。
			 */
			void DebugUpdateHealTest();

			/**
			 * @brief デバッグ用のダメージテスト関数。
			 */
			void DebugDamageParty();

			/**
		     * @brief デバッグ用のHP表示関数。
			 */
			void DebugPrintPartyHP();

		};
	}
}

