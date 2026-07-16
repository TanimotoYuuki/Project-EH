#include "stdafx.h"
#include "LoadingSceneController.h"

#include "Src/AsyncLoad/ParameterAsyncLoadTask.h"
#include "Src/AsyncLoad/StageAsyncLoadTask.h"
#include "Src/AsyncLoad/CharacterAsyncLoadTask.h"
#include "Src/AsyncLoad/AnimationAsyncLoadTask.h"
#include "Src/AsyncLoad/EffectAsyncLoadTask.h"
#include "Src/AsyncLoad/JobQueue.h"
#include "Src/Camera/Camera.h"
#include "Game2.h"

namespace
{
	const int MIN_LOADING_FRAME = 180;													   //! ローディング画面の最小表示フレーム数。これ未満の場合はローディング完了していても遷移しない。
	const auto VIEWPORT_WIDTH = 1280.0f;												   //! ビューポートの幅。
	const auto VIEWPORT_HEIGHT = 720.0f;												   //! ビューポートの高さ。
	const auto CAMERA_FOV_DEG = 45.0f;													   //! カメラの垂直方向の画角。
	const auto INGAME_BUILD_START_FRAME = 30;											   //! ローディング画面の開始から何フレームでInGame構築を開始するか。これにより、ローディング画面の開始直後のカクつきを緩和する。
	const auto RUNNER_UI_OFFSET_Y = 35.0f;												   //! UI上のプログレスバーとランナーの間の余白。これをもとにランナーのワールド座標Yが計算される。
	const auto RUNNER_WORLD_Z = 150.0f;													   //! ランナーのワールド座標Z。カメラ(350)と注視点(0)の中間に設定することで、UI上の位置とワールド上の位置が自然に対応するようになる。
	const auto RUNNER_WORLD_Y = 25.0f;													   //! ランナーのワールド座標Y。これをもとにランナーのワールド座標Xが計算される。
	const auto START = Vector3(50.0f, 25.0f, 250.0f);									   //! ランナーの移動開始位置。これをもとにランナーの移動範囲が設定される。YはRUNNER_WORLD_Y。ZはRUNNER_WORLD_Z。Xは適当に右端から少し内側に設定する。
	const auto END = Vector3(-50.0f, 25.0f, 250.0f);									   //! ランナーの移動終了位置。これをもとにランナーの移動範囲が設定される。YはRUNNER_WORLD_Y。ZはRUNNER_WORLD_Z。Xは適当に左端から少し内側に設定する。
	const Vector3 RUNNER_PROGRESS_START = Vector3(105.0f, RUNNER_WORLD_Y, RUNNER_WORLD_Z); //! ランナーの進捗0%のときのワールド座標。Xはプログレスバーの右端に対応させる。YはRUNNER_WORLD_Y。ZはRUNNER_WORLD_Z。
	const Vector3 RUNNER_PROGRESS_END = Vector3(-105.0f, RUNNER_WORLD_Y, RUNNER_WORLD_Z);  //! ランナーの進捗100%のときのワールド座標。Xはプログレスバーの左端に対応させる。YはRUNNER_WORLD_Y。ZはRUNNER_WORLD_Z。

}

namespace nsApp
{
	namespace nsLoading
	{
		LoadingSceneController::~LoadingSceneController()
		{
			/* 削除判定。*/
			delete m_inGameBuildHelper;
			m_inGameBuildHelper = nullptr;

			/* 非同期ロードの完了を待ってから、JobQueueをとめる。*/
			m_asyncLoadManager.Clear();

			/* JobQueueを終了する。*/
			nsJob::JobQueue::GetInstance().Shutdown();
		}


		void LoadingSceneController::Initialize(const InGameBuildRequest &request, nsScene::EnLoadingDestination destination)
		{
			/* JobQueueを起動する。*/
			nsJob::JobQueue::GetInstance().Startup(2);

			/* 初期化 */
			m_loadingFrame = 0;
			m_runnerBarProgress = 0.0f;
			m_debugRunnerProgress = 0.0f;
			m_isInGameStarted = false;
			m_isInGameBuildFinished = false;
			m_isTransitionReady = false;
			m_buildRequest = request;
			m_destination = destination;

			/* ローディング中、カメラを固定する。*/
			KeepLoadingCamera();

			/* ローディングUIの初期化 */
			m_loadingProgressUI.Initialize();

			/* ローディングテキストを初期化。*/
			m_loadingTextUI.Init();

			/* ランナーモデルの初期化 */
			m_runnerModel.Initialize();

			/* モデルの移動範囲を設定。*/
			SetupRunnerMoveRange();

			/* 非同期ロードタスクを追加して開始 */
			m_asyncLoadManager.AddTask(std::make_unique<ParameterAsyncLoadTask>());
			m_asyncLoadManager.AddTask(std::make_unique<StageAsyncLoadTask>());
			m_asyncLoadManager.AddTask(std::make_unique<CharacterAsyncLoadTask>(request));
			m_asyncLoadManager.AddTask(std::make_unique<AnimationAsyncLoadTask>(request));
			m_asyncLoadManager.AddTask(std::make_unique<EffectAsyncLoadTask>());
			m_asyncLoadManager.Start();
		}


		void LoadingSceneController::Update()
		{
			/* メインスレッドのJobタスクを消化する。*/
			nsJob::JobQueue::GetInstance().PumpMain();

			/* ローディング画面の経過フレームをカウント */
			++m_loadingFrame;

			/* 非同期ロードを更新 */
			m_asyncLoadManager.Update();

			if (m_destination == toInGame)
			{
				/*
					ローディング画面が表示されているInGame構築を開始する。
					これにより、Loading画面に居る間のカクつきを緩和する。
				*/
				if (!m_isInGameStarted && m_loadingFrame >= INGAME_BUILD_START_FRAME && m_asyncLoadManager.IsCompleted())
				{
					/* InGame構築を開始する */
					StartInGameBuild(m_buildRequest);
					/* InGame構築開始フラグを立てる */
					m_isInGameStarted = true;
				}

				/* InGame構築を1ステップ進める */
				UpdateInGameBuild();
			}

			/* 実進捗を一度だけ取る。*/
			const float totalProgress = GetTotalProgress();

			/* プログレスバーを更新 */
			m_loadingProgressUI.Update(totalProgress);

			/* テキストを更新。*/
			m_loadingTextUI.Update(g_gameTime->GetFrameDeltaTime());

			/* ランナーをプログレスに合わせて更新 */
			m_runnerModel.Update(m_loadingProgressUI.GetDisplayProgress());

			/* 遷移準備完了判定 */
			if (IsTransitionReadyToGame())
			{
				/* 遷移準備完了判定 */
				if (m_destination == toInGame)
					ExecuteGameTransition();

				else
					m_isTransitionReady = true;
			}
		}


		void LoadingSceneController::UpdateInGameBuild()
		{
			/* InGame構築が完了している場合は処理しない */
			if (m_isInGameBuildFinished)
				return;

			/* InGame構築が開始していない場合は処理しない */
			if (m_inGameBuildHelper == nullptr)
				return;

			/* InGame構築を1ステップ進める */
			m_inGameBuildHelper->Update();

			/* InGame構築が完了しているか確認 */
			if (m_inGameBuildHelper->IsFinished())
				m_isInGameBuildFinished = true;
		}


		float LoadingSceneController::GetTotalProgress() const
		{
			/* 非同期ロードの進捗を取得する。*/
			float asyncProgress = m_asyncLoadManager.GetProgress();

			/* 遷移先が Select の場合は非同期ロードの進捗のみ。*/
			if (m_destination == toSelect)
				return asyncProgress;

			float buildProgress = 0.0f;

			if (m_inGameBuildHelper != nullptr)
				buildProgress = m_inGameBuildHelper->GetProgress();
			else if (m_isInGameBuildFinished)
				buildProgress = 1.0f;

			/* 実ロード進捗（async と InGame 構築を半々）。*/
			const float contentProgress = asyncProgress * 0.5f + buildProgress * 0.5f;

			/*
				最低表示フレームに合わせた時間進捗。
				先読みが速すぎて実進捗が先に 1.0 になっても、
				ゲージが最初から満タンに見えないようにする。
			*/
			const float t = static_cast<float>(m_loadingFrame) / static_cast<float>(MIN_LOADING_FRAME);
			const float timeProgress = (t < 0.0f) ? 0.0f : (t > 1.0f) ? 1.0f : t;

			/* 実進捗と時間進捗の小さい方を表示する。*/
			return (contentProgress < timeProgress) ? contentProgress : timeProgress;
		}


		void LoadingSceneController::SetupRunnerMoveRange()
		{
			/* ランナーの移動範囲を設定する */
			m_runnerModel.SetMoveRange(START, END);
		}


		Vector3 LoadingSceneController::ConvertLoadingUIToWorld(float uiX, float uiY) const
		{
			float ndcX = (uiX / 1280.0f) * 2.0f - 1.0f;
			float ndcY = (uiY / 720.0f) * 2.0f - 1.0f;

			// NDC → ワールド座標への変換
			// Z=50 の平面上の点を計算
			float targetZ = 50.0f;
			float cameraZ = 350.0f;

			// 簡略化：Z値から逆算するスケール計算
			float distance = cameraZ - targetZ;							   // 300
			float halfHeight = distance * tanf(45.0f * 3.14159f / 360.0f); // FOV 45度の半高さ
			float halfWidth = halfHeight * (1280.0f / 720.0f);			   // アスペクト比

			float worldX = ndcX * halfWidth;
			float worldY = ndcY * halfHeight;
			float worldZ = targetZ;

			return Vector3(worldX, worldY, worldZ);
		}


		Vector3 LoadingSceneController::GetRunnerWorldPosition(float progress) const
		{
			/* クリップ */
			if (progress < 0.0f)
				progress = 0.0f;
			if (progress > 1.0f)
				progress = 1.0f;

			/* 線形補間 */
			return Vector3(
				RUNNER_PROGRESS_START.x + (RUNNER_PROGRESS_END.x - RUNNER_PROGRESS_START.x) * progress,
				RUNNER_PROGRESS_START.y + (RUNNER_PROGRESS_END.y - RUNNER_PROGRESS_START.y) * progress,
				RUNNER_PROGRESS_START.z + (RUNNER_PROGRESS_END.z - RUNNER_PROGRESS_START.z) * progress);
		}


		void LoadingSceneController::KeepLoadingCamera()
		{
			/* ローディング画面専用のカメラを維持する */
			m_camera = FindGO<Camera>("camera");

			/* カメラが存在しない場合は新規作成する */
			if (m_camera == nullptr)
				m_camera = NewGO<Camera>(0, "camera");

			/* カメラが存在しない場合は処理しない */
			if (m_camera == nullptr)
				return;

			/* カメラをローディング用に切り替える */
			m_camera->ChangeToLoading();
		}


		void LoadingSceneController::ChangeToBattleCamera()
		{
			/* カメラを戦闘用に切り替える */
			m_camera = FindGO<Camera>("camera");

			/* カメラが存在しない場合は新規作成する */
			if (m_camera == nullptr)
				m_camera = NewGO<Camera>(0, "camera");

			/* カメラが存在しない場合は処理しない */
			if (m_camera == nullptr)
				return;

			/* カメラを戦闘用に切り替える */
			m_camera->ChangeToBattle();
		}


		bool LoadingSceneController::IsTransitionReadyToGame() const
		{
			/* ローディングからゲーム本編への遷移の準備ができているか判定する */
			if (m_isTransitionReady)
				return false;

			/* 遷移先がInGameでない場合は、非同期ロードの完了とランナーのアニメーション完了のみを条件とする */
			if (!m_asyncLoadManager.IsCompleted())
				return false;

			/* 遷移先がInGameの場合は、非同期ロードの完了、ランナーのアニメーション完了、ローディング画面の最小表示フレーム数の経過、InGame構築の完了を条件とする */
			if (!m_loadingProgressUI.IsProgressAnimationFinish())
				return false;

			/* ランナーのアニメーションが完了しているか判定する */
			if (!IsRunnerAnimationFinished())
				return false;

			/* ローディング画面の最小表示フレーム数の経過を判定する */
			if (m_loadingFrame < MIN_LOADING_FRAME)
				return false;

			/* InGame構築の完了を判定する */
			if (m_destination == toInGame && !m_isInGameBuildFinished)
				return false;

			return true;
		}


		void LoadingSceneController::ExecuteGameTransition()
		{
			/* ゲーム本編への遷移を実行する */
			if (m_inGameBuildHelper == nullptr)
				return;

			/* ゲーム本編のクラスを生成して初期化する */
			m_game2 = NewGO<nsGame::Game2>(0, "game");

			/* ゲーム本編のクラスが生成できない場合は処理しない */
			if (m_game2)
			{
				/* ゲーム本編のクラスにInGame構築の結果を適用する */
				m_game2->SetBossType(m_buildRequest.bossType);

				/* プレイヤーの役割と操作設定を適用する */
				for (int i = 0; i < 4; i++)
				{
					/* プレイヤーの役割を適用する */
					m_game2->SetCharacterRole(i, m_buildRequest.characterRole[i]);
					/* プレイヤーの操作設定を適用する */
					m_game2->SetPlayerControle(i, m_buildRequest.isPlayerControle[i]);
				}

				/* ゲーム本編のクラスにInGame構築の結果を適用する */
				m_game2->ApplyBuildResult(m_inGameBuildHelper->GetResult());
				/* ゲームを有効化する */
				m_game2->ActivateGame();
				/* 戦闘用カメラに切り替える */
				ChangeToBattleCamera();
			}

			/* 削除判定。*/
			delete m_inGameBuildHelper;
			m_inGameBuildHelper = nullptr;
			m_isTransitionReady = true;
		}


		void LoadingSceneController::Render(RenderContext &rc)
		{
			/* ローディングUIの背景を描画 */
			m_loadingProgressUI.RenderBack(rc);

			/* テキストの描画。*/
			m_loadingTextUI.Render(rc);

			/* ランナーモデルを描画 */
			m_runnerModel.Render(rc);

			/* ローディングUIの前面を描画 */
			m_loadingProgressUI.RenderFront(rc);
		}
	}
}
