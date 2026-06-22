#pragma once

/*
 * @file LoadingSceneController.h
 * @autor YamaguchiHayato
 * @date 2026/06/06
 */

#include "Src/Scene/Loading/LoadingDestination.h"
#include "Src/AsyncLoad/AsyncLoadManager.h"
#include "Src/Build/InGameBuildHelper.h"
#include "Src/UI/Loading/LoadingTextUI.h"
#include "Src/UI/Loading/LoadingProgressUI.h"
#include "Src/UI/Loading/LoadingRandomRunnerModel.h"

namespace nsApp
{
	namespace nsGame
	{
		class Game2;
	}

	using namespace nsScene;

	namespace nsLoading
	{
		class LoadingSceneController
		{
		public:
			/* コンストラクタとデストラクタ。*/
			LoadingSceneController() = default;
			virtual ~LoadingSceneController();

		public:
			/**
			 * @brief 初期化処理。
			 * @param request InGame生成に必要なリクエスト情報。
			 * @param destination ローディングの遷移先。
			 */
			void Initialize(const InGameBuildRequest& request, EnLoadingDestination destination);

			/**
			 * @brief 更新処理.
			 */
			void Update();

			/**
			 * @brief 描画処理.
			 * @param rc レンダリングコンテキスト.
			 */
			void Render(RenderContext& rc);


		public:
			/**
			 * @brief ローディングの完了判定.
			 * @return ローディングが完了している場合はtrue、そうでない場合はfalse.
			 */
		 	inline bool IsCompleted() const
			{
				/* ローディング画面の完了判定 */
				return m_isTransitionReady;
			}

			/**
			 * @brief ローディングの失敗判定.
			 * @return ローディングが失敗している場合はtrue、そうでない場合はfalse.
			 */
			inline bool IsFailed() const
			{
				return m_asyncLoadManager.IsFailed();
			}

			/**
			 * @brief ローディングのエラーメッセージの取得.
			 * @return ローディングのエラーメッセージ. ローディングが失敗していない場合は空文字列を返す.
			 */
			inline std::string GetErrorMessage() const
			{
				return m_asyncLoadManager.GetErrorMessage();
			}

			/**
			 * @brief ローディングの進捗率の取得.
			 * @return ローディングの進捗率. 0.0f ～ 1.0f の範囲で返す.
					   ローディングが失敗している場合は0.0fを返す.
			 */
			inline float GetDisplayProgress() const
			{
				return m_loadingProgressUI.GetDisplayProgress();
			}


		private:
			/**
			 * @brief ゲーム本編の生成を開始する.
			 * @param request InGame生成に必要なリクエスト情報.
			 */
			inline void StartInGameBuild(const InGameBuildRequest& request)
			{
				/* InGame構築補助クラスを生成してInGame構築を開始する */
				m_inGameBuildHelper = new InGameBuildHelper();
				/* InGame構築を開始する */
				m_inGameBuildHelper->Initialize(request);
			}

			/**
			 * @brief ゲーム本編の生成の更新処理.
			 */
			void UpdateInGameBuild();

			/**
			 * @brief ゲーム本編の生成の完了判定.
			 * @return ゲーム本編の生成が完了している場合はtrue、そうでない場合はfalse.
			 */
			float GetTotalProgress() const;

			/**
			 * @brief ローディングランナーの移動範囲を設定する。
			 */
			void SetupRunnerMoveRange();

			/**
			 * @brief ローディングUI座標をローディング用ワールド座標へ変換する。
			 * @param uiX UI座標X。
			 * @param uiY UI座標Y。
			 * @return ワールド座標。
			 */
			Vector3 ConvertLoadingUIToWorld(float uiX, float uiY) const;

			/**
			 * @brief 進捗率からランナーのワールド座標を取得する。
			 * @param progress 進捗率。
			 * @return ランナーのワールド座標。
			*/
			Vector3 GetRunnerWorldPosition(float progress) const;

			/**
			 * @brief ランナーのアニメーションが終了しているか判定する。
			 * @return ランナーのアニメーションが終了している場合はtrue、そうでない場合はfalse.
			 */
			inline bool IsRunnerAnimationFinished() const
			{
				return m_runnerModel.IsFinished();
			}

			/**
			 * @brief ローディング中のカメラを維持する。
			 */
			void KeepLoadingCamera();

			/**
			 * @brief バトルカメラへ切り替える。
			 */
			void ChangeToBattleCamera();

			/**
			 * @brief ローディングからゲーム本編への遷移の準備ができているか判定する。
			 * @return ローディングからゲーム本編への遷移の準備ができている場合はtrue、そうでない場合はfalse.
			 */
			bool IsTransitionReadyToGame() const;

			/**
			 * @brief ゲーム本編への遷移を実行する。
			 */
			void ExecuteGameTransition();


		private:
			Camera* m_camera = nullptr;							//! ローディング画面専用のカメラ。
			AsyncLoadManager m_asyncLoadManager;				//! 非同期ロードを管理するクラス。
			nsUI::LoadingProgressUI m_loadingProgressUI;		//! ローディングの進捗を表示するUIクラス。
			EnLoadingDestination m_destination = toInGame;	    //! ローディングの遷移先。
			nsUI::LoadingRandomRunnerModel m_runnerModel;		//! ロード画面で走るランナーモデル。
			nsUI::LoadingTextUI m_loadingTextUI;		        //! ローディングのテキストを管理するUIクラス。
			nsGame::Game2* m_game2 = nullptr;					//! ローディング完了後に生成されるゲーム本編のクラス。
			
			InGameBuildHelper* m_inGameBuildHelper = nullptr;   //! ゲーム生成補助クラス。
			InGameBuildRequest m_buildRequest;				    //! ゲーム生成に必要なリクエスト情報。


		private:
			bool m_isInGameBuildFinished = false;				//! ゲーム本編の生成が完了しているか。
			bool m_isTransitionReady = false;					//! ローディングからゲーム本編への遷移の準備ができているか。
			bool m_isInGameStarted = false;						//! ゲーム本編の生成が開始されているか。

			int m_loadingFrame = 0;								//! ローディング開始からの経過フレーム数。

			float m_ndcX = 0.0f;								//! ランナーのNDC座標X。これをもとにワールド座標が計算される。
			float m_debugRunnerProgress = 0.0f;					//! デバッグ用のランナーの進捗。これが1.0f以上になるとランナーのアニメーションが終了したとみなす。
			float m_runnerBarProgress = 0.0f;					//! ランナーの実際のプログレスバー上の進捗。これがGetTotalProgress()に合わせて更新される。
		};
	}
}