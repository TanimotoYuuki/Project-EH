#pragma once
#include <Src/Actor/Character/Player/Component/PlayerSpawnData.h>

/**
 * @file   Game2.h
 * @brief  ゲームメイン管理クラス
 * @author All
 * @date   2026/05/08
 */

#include "Src/Parameter/ParameterSystem.h"
#include "Src/Build/InGameBuildHelper.h"
#include "Src/Actor/Character/Boss/Status/BossPhaseEventController.h"

namespace nsApp
{
	class Camera;
	class GameEndSelect;
	class PlayerGenerator;
	class PlayerControlerHub;
	class DamageIndicatorPool;
	class ParameterSystem;

	namespace nsSound { 
		class SoundLister; 
	}

	namespace nsStage  {
		class BackGround; 
	}

	namespace nsActor {
		class Player; 
		class Boss; 
	}

	namespace nsUI {
		class ReboneGaugeUIManager;
		class CommentaryUIManager;
		class GuardGaugeUIManager;
	}

	namespace nsGame
	{
		class CharacterHP;
		class GameTimeLimit;
		class GameStartDirection;
		class GameClearDirection;
		class GameTimeUpDirection;
		class GameOverDirection;
		class Pause;


		/**
		 * @class Game2
		 * @brief  ゲーム内各オブジェクトの参照を保持・管理するクラス。
		 */
		class Game2 : public IGameObject
		{
		public:
			/* コンストラクタとデストラクタ。 */
			Game2() = default;
			~Game2();


		public:
			/**
			 * @brief  初期化・開始処理。
			 * @return 正常に初期化された場合 true。
			 */
			bool Start();

			/**
			 * @brief  更新処理。
			 */
			void Update();

			/**
			 * @brief  描画処理。
			 * @param rc 描画コンテキスト。
			 */
			void Render(RenderContext& rc);


		public:
			/**
			 * @brief  補助ビルドクラスで生成した結果を Game2 に反映する。
			 * @param result InGameビルド結果。
			 */
			void ApplyBuildResult(const InGameBuildResult& result);

			/**
			 * @brief  ゲーム本編をアクティブにする。
			 */
			void ActivateGame();


		public:
			/**
			 * @brief  ゲームクリア演出のインスタンスを返す。
			 * @return ゲームクリア演出のインスタンス。
			 */
			inline GameClearDirection* GetGameClearDirectionInstance() const
			{
				return m_gameClearDirection;
			}

			/**
			 * @brief  時間切れ演出のインスタンスを返す。
			 * @return 時間切れ演出のインスタンス。
			 */
			inline GameTimeUpDirection* GetGameTimeUpDirectionInstance() const
			{
				return m_gameTimeUpDirection;
			}

			/**
			 * @brief  ゲームオーバー演出のインスタンスを返す。
			 * @return ゲームオーバー演出のインスタンス。
			 */
			inline GameOverDirection* GetGameOverDirectionInstance() const
			{
				return m_gameOverDirection;
			}

			/**
			 * @brief  ゲーム終了時の選択画面のインスタンスを返す。
			 * @return 選択画面のインスタンス。
			 */
			inline GameEndSelect* GetGameEndSelectInstance() const
			{
				return m_gameEndSelect;
			}

			/**
			 * @brief  ポーズ画面のインスタンスを返す。
			 * @return ポーズ画面のインスタンス。
			 */
			inline Pause* GetPauseInstance() const
			{
				return m_pause;
			}

			/**
			 * @brief  ボスの種類を設定する。
			 * @param bossType ボスの種類。
			 */
			inline void SetBossType(int bossType)
			{
				m_bossType = bossType;
			}

			/**
			 * @brief  ボスの種類を返す。
			 * @return ボスの種類。
			 */
			inline int GetBossType() const
			{
				return m_bossType + 5;
			}

			/**
			 * @brief  キャラクターの役割（ロール）を設定する。
			 * @param playerIndex プレイヤーのインデックス。
			 * @param role        役割（ロール）。
			 */
			inline void SetCharacterRole(int playerIndex, int role)
			{
				m_characterRole[playerIndex] = role;
			}

			/**
			 * @brief  キャラクターの役割（ロール）を返す。
			 * @param playerIndex プレイヤーのインデックス。
			 * @return キャラクターの役割（ロール）。
			 */
			inline int GetCharacterRole(int playerIndex) const
			{
				return m_characterRole[playerIndex];
			}

			/**
			 * @brief  プレイヤーが操作されているかどうかを設定する。
			 * @param playerIndex プレイヤーのインデックス。
			 * @param isControle  操作されているかどうか。
			 */
			inline void SetPlayerControle(int playerIndex, bool isControle)
			{
				m_isPlayerControle[playerIndex] = isControle;
			}

			/**
			 * @brief  プレイヤーが操作されているかどうかを返す。
			 * @param playerIndex プレイヤーのインデックス。
			 * @return プレイヤーが操作されているかどうか。
			 */
			inline bool IsPlayerControle(int playerIndex) const
			{
				return m_isPlayerControle[playerIndex];
			}


		private:
			/* ポインタ（サウンド・背景）。 */
			nsApp::nsSound::SoundLister* m_soundLister = nullptr; //! サウンドリスナー。
			nsApp::nsStage::BackGround* m_backGround = nullptr; //! 背景。

			/* ポインタ（UI）。 */
			nsUI::ReboneGaugeUIManager* m_reboneGaugeUIManager = nullptr; //! リボーンゲージUIマネージャー。
			nsUI::CommentaryUIManager* m_commentaryUIManager = nullptr;   //! 実況・解説UIマネージャー。
			nsUI::GuardGaugeUIManager* m_guardGaugeUIManager = nullptr;   //! ガードゲージUIマネージャー。


			/* ポインタ（カメラ・キャラクター）。 */
			nsApp::Camera* m_camera = nullptr; //! カメラ。
			nsActor::Player* m_player = nullptr; //! 操作プレイヤー。
			nsActor::Boss* m_boss = nullptr; //! ボス。

			/* ポインタ（システム）。 */
			DamageIndicatorPool* m_damageIndicatorPool = nullptr; //! ダメージインジケータープール。
			CharacterHP* m_characterHP = nullptr; //! キャラクターHP管理。
			GameTimeLimit* m_gameTimeLimit = nullptr; //! ゲーム制限時間。
			GameStartDirection* m_gameStartDirection = nullptr; //! ゲーム開始演出。
			GameClearDirection* m_gameClearDirection = nullptr; //! ゲームクリア演出。
			GameTimeUpDirection* m_gameTimeUpDirection = nullptr; //! 時間切れ演出。
			GameOverDirection* m_gameOverDirection = nullptr; //! ゲームオーバー演出。
			GameEndSelect* m_gameEndSelect = nullptr; //! ゲーム終了時選択。
			Pause* m_pause = nullptr; //! ポーズ。
			PlayerGenerator* m_generator = nullptr; //! プレイヤー生成。
			PlayerControlerHub* m_playerHub = nullptr; //! プレイヤーコントローラーハブ。
			BossPhaseEventController* m_bossPhaseController = nullptr; //! ボスフェーズイベントコントローラー。

			/* オブジェクト型。 */
			ParameterSystem              m_parameterSystem;                //! パラメータシステム。

			/* コンテナ型。 */
			std::vector<nsActor::Player*> m_players;   //! プレイヤーリスト。
			std::vector<PlayerSpawnData>  m_partyData; //! パーティデータリスト。


		private:
			/* int型。 */
			int m_bossType = 0;    //! ボスの種類。
			int m_characterRole[4] = { 0 }; //! キャラクターごとの役割（ロール）。

			/* bool型。 */
			bool m_isPlayerControle[4] = { true }; //! プレイヤーが操作されているかどうか。
			bool m_isGameActive = false;     //! ゲーム本編がアクティブになっているか。
		};
	}
}