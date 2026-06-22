#pragma once

/**
 * @file   InGameBuildHelper.h
 * @brief  InGame生成処理を管理するクラスのヘッダファイル。
 * @author Yamaguchi Hayato
 * @date   2026/06/05
 * @detail Game2で必要な生成処理を1ステップずつ実行する。
 */

#include <vector>
#include <functional>

#include "Src/Actor/Character/Player/Component/PlayerSpawnData.h"

namespace nsApp
{
	class Camera;
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
		class GuardGaugeUIManager;
	}

	namespace nsGame {
		class CharacterHP;
		class GameTimeLimit;
		class GameStartDirection;
		class Pause;
	}


	/**
	 * @struct InGameBuildRequest
	 * @brief InGame生成に必要なリクエスト情報。
	 */
	struct InGameBuildRequest
	{
		int bossType = 0;
		int characterRole[4] = { 0 };
		bool isPlayerControle[4] = { true, false, false, false };
	};


	/**
	 * @struct InGameBuildResult
	 * @brief InGame生成結果。
	 */
	struct InGameBuildResult
	{
		nsSound::SoundLister* soundLister = nullptr;
		nsStage::BackGround* backGround = nullptr;
		nsUI::ReboneGaugeUIManager* reboneGaugeUIManager = nullptr;
		nsUI::CommentaryUIManager* commentaryUIManager = nullptr;
		nsUI::GuardGaugeUIManager* guardGaugeUIManager = nullptr;
		Camera* camera = nullptr;
		nsActor::Player* player = nullptr;
		std::vector<nsActor::Player*> players;
		std::vector<PlayerSpawnData> partyData;
		nsActor::Boss* boss = nullptr;
		DamageIndicatorPool* damageIndicatorPool = nullptr;
		nsGame::CharacterHP* characterHP = nullptr;
		nsGame::GameTimeLimit* gameTimeLimit = nullptr;
		nsGame::GameStartDirection* gameStartDirection = nullptr;
		nsGame::Pause* pause = nullptr;
		PlayerGenerator* generator = nullptr;
		PlayerControlerHub* playerHub = nullptr;
	};


	/**
	 * @class InGameBuildHelper
	 * @brief InGame生成補助クラス。
	 */
	class InGameBuildHelper
	{
	public:
		InGameBuildHelper() = default;
		virtual ~InGameBuildHelper() = default;


	public:
		/**
		 * @brief 初期化処理。
		 * @param request 生成に必要な情報。
		 */
		void Initialize(const InGameBuildRequest& request);

		/**
		 * @brief 更新処理。
		 */
		void Update()
		{
			/* 生成が完了している場合は何もしない。*/
			ExecuteNextBuildFunction();
		}


		/* ゲッター。*/
	public:
		/**
		 * @brief 生成が完了しているか。
		 * @return 生成が完了している場合は true。
		 */
		inline bool IsFinished() const
		{
			return m_isFinished;
		}

		/**
		 * @brief 生成結果を取得する。
		 * @return 生成結果。
		 */
		inline const InGameBuildResult& GetResult() const
		{
			return m_result;
		}

		/**
		 * @brief 進捗率を取得する。
		 * @return 0.0f〜1.0f の進捗率。
		 */
		float GetProgress() const;


	private:
		using BuildFunction = std::function<void()>;


	private:
		/**
		 * @brief 生成関数を登録する。
		 */
		void InitializeBuildFunctions();

		/**
		 * @brief 次の生成関数を1つ実行する。
		 */
		void ExecuteNextBuildFunction();

		/**
		 * @brief 乱数の生成。
		 */
		void BuildRandom();

		/**
		 * @brief サウンドの生成。
		 */
		void BuildSound();

		/**
		 * @brief ステージの生成。
		 */
		void BuildStage();

		/**
		 * @brief 背景の生成。
		 */
		void BuildBackGround();

		/**
		 * @brief カメラの生成。
		 */
		void BuildCamera();

		/**
		 * @brief ダメージインジケーターのプールの生成。
		 */
		void BuildDamagePool();

		/**
		 * @brief 実況UIの生成。
		 */
		void BuildCommentaryUI();

		/**
		 * @brief PlayerGeneratorの生成。
		 */
		void BuildPlayerGenerator();

		/**
		 * @brief プレイヤーの生成に必要なデータの作成。
		 */
		void CreatePartyData();

		/**
		 * @brief プレイヤーの生成。
		 * @param playerIndex プレイヤーのインデックス。
		 */
		void SpawnPlayer(int playerIndex);

		/**
		 * @brief プレイヤーをリボーンゲージに登録する。
		 */
		void RegisterPlayersToReboneGauge();

		/**
		 * @brief ガードゲージUIの生成。
		 */
		void BuildGuardGaugeUI();

		/**
		 * @brief プレイヤーをガードゲージUIに登録する。
		 */
		void RegisterPlayersToGuardGauge();

		/**
		 * @brief PlayerHubの生成。
		 */
		void BuildPlayerHub();

		/**
		 * @brief Bossの生成。
		 */
		void BuildBoss();

		/**
		 * @brief CharacterHPの生成。
		 */
		void BuildCharacterHP();

		/**
		 * @brief ゲーム制限時間の生成。
		 */
		void BuildGameTimeLimit();

		/**
		 * @brief ゲーム開始演出の生成。
		 */
		void BuildGameStartDirection();

		/**
		 * @brief ポーズの生成。
		 */
		void BuildPause();

		/**
		 * @brief 生成完了処理。
		 */
		void FinishBuild();


	private:
		InGameBuildRequest m_request;                //! 生成に必要な情報。
		InGameBuildResult m_result;                  //! 生成結果。

		std::vector<BuildFunction> m_buildFunctions; //! 生成関数のリスト。
		std::vector<PlayerSpawnData> m_partyData;    //! プレイヤーの生成に必要なデータのリスト。
		std::vector<nsActor::Player*> m_players;     //! 生成されたプレイヤーのリスト。

		ControllerType m_controllerType[4] = { ControllerType::Player_1P }; //! キャラクターごとのコントローラーの種類。

		bool m_isFinished = false;                   //! 生成が完了しているか。
		int m_currentBuildIndex = 0;                 //! 現在実行中の生成関数のインデックス。
	};
}