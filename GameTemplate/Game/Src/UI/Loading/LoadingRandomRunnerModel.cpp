#include "stdafx.h"
#include "LoadingRandomRunnerModel.h"

#include "Src/Actor/Character/Player/CharacterByWeapon/SwordCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/HammerCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/WandCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/TwinGunCharacter.h"

#include <cstdlib>
#include <ctime>

namespace
{
	/* ローディング用モデルの大きさ */
	const float RUNNER_MODEL_SCALE_RATE = 0.125f;																			//! モデルのスケール率。必要に応じて調整する。
	const auto RUNNER_MODEL_SCALE = Vector3(RUNNER_MODEL_SCALE_RATE, RUNNER_MODEL_SCALE_RATE, RUNNER_MODEL_SCALE_RATE);		//! モデルのスケール。必要に応じて調整する。
			
	/* ランナーがターゲットに向かって移動する速度 */
	const float RUNNER_FOLLOW_SPEED = 0.15f;																				//! ランナーの進捗がターゲット進捗に追従する速度。0.0fから1.0fの範囲で調整する。値が大きいほど速く追従する。

	/* 完了判定の閾値 */
	const float RUNNER_COMPLETE_THRESHOLD = 0.995f;																			//! ローディング完了とみなす進捗率の閾値。0.0fから1.0fの範囲で調整する。値が小さいほど早く完了とみなす。

	/* ランナーの向き（Y軸回転） */
	const float RUNNER_RIGHT_ANGLE_Y = -90.0f;																				//! ランナーが左から右へ移動する場合のY軸角度。必要に応じて調整する。
	const float RUNNER_LEFT_ANGLE_Y = -90.0f;																				//! ランナーが右から左へ移動する場合のY軸角度。必要に応じて調整する。
}


namespace nsApp
{
	namespace nsUI
	{
		LoadingRandomRunnerModel::~LoadingRandomRunnerModel()
		{
			/* NewGOで作成したランダムランナーを削除する */
			if (m_runner != nullptr)
			{
				DeleteGO(m_runner);
				m_runner = nullptr;
			}
		}


		void LoadingRandomRunnerModel::Initialize()
		{
			/* 進捗を初期化 */
			m_progress = 0.0f;
			m_targetProgress = 0.0f;
			m_runnerProgress = 0.0f;

			/*
				本来ここで移動範囲を設定しない。
				実際の移動範囲はLoadingSceneController::SetupRunnerMoveRange()から受け取る。
			*/
			m_startPosition = Vector3::Zero;
			m_endPosition = Vector3::Zero;
			m_currentPosition = Vector3::Zero;
			m_lastRunnerX = m_currentPosition.x;

			m_isMoveRangeReady = false;

			/* ランナーをランダムに1体生成 */
			CreateRandomRunner();

			/* ランナーが作成されなかった場合は処理を止める */
			if (m_runner == nullptr)
				return;

			/* 初期化完了 */
			m_isInitialized = true;
		}


		void LoadingRandomRunnerModel::Update(float progress)
		{
			/* 初期化が完了していない場合は何もしない */
			if (!m_isInitialized)
				return;

			/* 移動範囲が設定されていない場合は何もしない */
			if (!m_isMoveRangeReady)
				return;

			/* ランナーが存在しない場合は何もしない */
			if (m_runner == nullptr)
				return;

			/* 受け取った進捗を 0.0f～1.0f にクリップ */
			m_runnerProgress = Clamp01(progress);
			m_targetProgress = m_runnerProgress;
			m_progress = m_runnerProgress;

			/* 進捗に基づいて位置を更新 */
			UpdatePosition(m_runnerProgress);

			/* ランナーのトランスフォームを設定 */
			m_runner->SetLoadingPreviewTransform( m_currentPosition, GetRunnerAngleY(), RUNNER_MODEL_SCALE );
		}


		void LoadingRandomRunnerModel::CreateRandomRunner()
		{
			/* 乱数シードを初期化 */
			std::srand(static_cast<unsigned int>(std::time(nullptr)));

			/* 4体の中からランダムに1体を選択 */
			m_randomIndex = std::rand() % 4;

			switch (m_randomIndex)
			{
			case 0:
				/* 剣キャラクターを生成 */
				m_runner = NewGO<nsActor::SwordCharacter>(0, "loadingRunner");
				break;

			case 1:
				/* ハンマーキャラクターを生成 */
				m_runner = NewGO<nsActor::HammerCharacter>(0, "loadingRunner");
				break;

			case 2:
				/* 杖キャラクターを生成 */
				m_runner = NewGO<nsActor::WandCharacter>(0, "loadingRunner");
				break;

			case 3:
			default:
				/* 双銃キャラクターを生成 */
				m_runner = NewGO<nsActor::TwinGunCharacter>(0, "loadingRunner");
				break;
			}

			if (m_runner != nullptr)
			{
				m_runner->EnableLoadingPreview();
				m_runner->Activate();
			}
		}


		void LoadingRandomRunnerModel::UpdatePosition(float progress)
		{
			/* 進捗率を0.0f～1.0fの範囲にクリップ */
			m_updatePosRate = Clamp01(progress);

			/* 進捗率に基づいてターゲットX座標を計算 */
			m_targetX = m_startPosition.x + (m_endPosition.x - m_startPosition.x) * m_updatePosRate;
			
			/* ターゲットX座標を移動範囲内にクランプ */
			m_minX = (std::min)(m_startPosition.x, m_endPosition.x);
			m_maxX = (std::max)(m_startPosition.x, m_endPosition.x);

			/* クランプされたX座標を現在の位置に反映 */
			m_clampedX = ((std::max)(m_minX, (std::min)(m_targetX, m_maxX)));

			/* クランプされたX座標と、開始位置・終了位置のY,Z座標を組み合わせて現在の位置を更新 */
			m_currentPosition = Vector3( m_clampedX,
				m_startPosition.y + (m_endPosition.y - m_startPosition.y) * m_updatePosRate,
				m_startPosition.z + (m_endPosition.z - m_startPosition.z) * m_updatePosRate
			);
		}


		void LoadingRandomRunnerModel::SetMoveRange(const Vector3& startPosition, const Vector3& endPosition)
		{
			/*
				LoadingSceneControllerから受け取った開始位置・終了位置をそのまま使用する。
				これらはUI座標ではなく、ワールド座標である。
			*/
			m_startPosition = startPosition;
			m_endPosition = endPosition;
			m_currentPosition = m_startPosition;

			/* 進捗をリセット */
			m_progress = 0.0f;
			m_targetProgress = 0.0f;
			m_runnerProgress = 0.0f;
			m_lastRunnerX = m_currentPosition.x;

			/* 移動準備完了 */
			m_isMoveRangeReady = true;

			/* ランナーの初期位置を設定 */
			if (m_runner != nullptr)
				m_runner->SetLoadingPreviewTransform( m_currentPosition, GetRunnerAngleY(), RUNNER_MODEL_SCALE);
		}


		void LoadingRandomRunnerModel::SetPreviewPosition(const Vector3& position)
		{
			/* 初期化が完了していない場合は何もしない */
			if (!m_isInitialized)
				return;

			/* 移動範囲が設定されていない場合は何もしない */
			if (m_runner == nullptr)
				return;

			/* 受け取った位置をそのまま使用する。これはUI座標ではなく、ワールド座標である。 */
			m_currentPosition = position;

			/* ランナーの位置を更新 */
			m_runner->SetLoadingPreviewTransform( m_currentPosition, GetRunnerAngleY(), RUNNER_MODEL_SCALE);
		}


		float LoadingRandomRunnerModel::GetRunnerAngleY() const
		{
			/*
			  開始位置から終了位置への移動方向を計算して、
			  ランナー自身の向きを決める。
			*/
			if (m_endPosition.x >= m_startPosition.x)
				return RUNNER_RIGHT_ANGLE_Y;  // 左から右へ

			return RUNNER_LEFT_ANGLE_Y;       // 右から左へ
		}


		float LoadingRandomRunnerModel::Clamp01(float value) const
		{
			/* 値を0.0fから1.0fの範囲に補正する */
			if (value < 0.0f)
				return 0.0f;
			if (value > 1.0f)
				return 1.0f;

			/* それ以外の場合はそのままの値を返す */
			return value;
		}
	}
}
