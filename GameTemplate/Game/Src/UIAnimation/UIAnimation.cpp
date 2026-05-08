#include "stdafx.h"
#include "UIAnimation.h"

namespace nsApp
{
	namespace nsUI
	{
		/*座標を変えるアニメーションの更新処理。*/
		void PositionUIAnimation::Update()
		{
			/*開始前の遅延時間があるか？*/
			/*ある場合は、遅延時間を減らしていく。*/
			if (m_startDelayTime > 0.0f)
			{
				m_startDelayTime -= g_gameTime->GetFrameDeltaTime();
				return;
			}

			/*繰り返し実行するか？*/
			if (!m_isLoop && m_isCompleted)
			{
				Vector3 completePostion = m_targetPosition;
				m_render->SetPosition(completePostion);
				m_render->Update();

				/*終了後の遅延時間があるか？*/
				/*ある場合は、遅延時間を減らしていく。*/
				if (m_endDelayTime > 0.0f)
				{
					m_endDelayTime -= g_gameTime->GetFrameDeltaTime();
					return;
				}

				m_isEnd = true;
				return;
			}

			/*経過時間の取得。*/
			const float deltaTime = g_gameTime->GetFrameDeltaTime();

			Vector3 targetPosition;
			Vector3 basePosition;

			/*ステップの切り替え処理。*/
			switch (m_currentStep)
			{
			case UIAnimationBase::enAnimationStep_Min:/*下限。*/
				targetPosition = m_targetPosition;
				basePosition = m_basePosition;
				break;
			case UIAnimationBase::enAnimationStep_Max:/*上限。*/
				targetPosition = m_basePosition;
				basePosition = m_targetPosition;
				break;
			default:
				break;
			}

			/*現在のアニメーション経過時間のパーセント。*/
			const float computePercent = m_elapsedTime / m_targetTime;

			/*初期値から目標値までをなめらかに変化。*/
			Vector3 computePosition = basePosition;
			computePosition.Lerp(computePercent, basePosition, targetPosition);

			/*位置の設定。*/
			m_render->SetPosition(computePosition);
			m_render->Update();

			/*経過時間の更新。*/
			m_elapsedTime += deltaTime * m_playSpeed;
			if (m_elapsedTime >= m_targetTime)
			{
				m_elapsedTime = 0.0f;

				m_currentStep = m_currentStep == enAnimationStep_Max ? enAnimationStep_Min : enAnimationStep_Max;

				/*完了。*/
				m_isCompleted = true;
			}
		}

		//回転を変えるアニメーションの更新処理
		void RotationUIAnimation::Update()
		{
			/*開始前の遅延時間があるか？*/
			/*ある場合は、遅延時間を減らしていく。*/
			if (m_startDelayTime > 0.0f)
			{
				m_startDelayTime -= g_gameTime->GetFrameDeltaTime();
				return;
			}

			/*繰り返し実行するか？*/
			if (!m_isLoop && m_isCompleted)
			{
				Quaternion completeRotation = m_targetRotation;
				m_render->SetRotation(completeRotation);
				m_render->Update();

				/*終了後の遅延時間があるか？*/
				/*ある場合は、遅延時間を減らしていく。*/
				if (m_endDelayTime > 0.0f)
				{
					m_endDelayTime -= g_gameTime->GetFrameDeltaTime();
					return;
				}

				m_isEnd = true;
				return;
			}

			/*経過時間の取得。*/
			const float deltaTime = g_gameTime->GetFrameDeltaTime();

			Quaternion targetRotation;
			Quaternion baseRotation;

			/*ステップの切り替え処理。*/
			switch (m_currentStep)
			{
			case UIAnimationBase::enAnimationStep_Min:/*下限。*/
				targetRotation = m_targetRotation;
				baseRotation = m_baseRotation;
				break;
			case UIAnimationBase::enAnimationStep_Max:/*上限。*/
				targetRotation = m_baseRotation;
				baseRotation = m_targetRotation;
				break;
			}

			/*現在のアニメーション経過時間のパーセント。*/
			const float computePercent = m_elapsedTime / m_targetTime;

			/*初期値から目標値までをなめらかに変化。*/
			Quaternion computeRotation = baseRotation;
			computeRotation.Slerp(computePercent, baseRotation, targetRotation);

			/*回転の設定。*/
			m_render->SetRotation(computeRotation);
			m_render->Update();

			/*経過時間の更新。*/
			m_elapsedTime += deltaTime * m_playSpeed;
			if (m_elapsedTime >= m_targetTime) {
				m_elapsedTime = 0.0f;

				m_currentStep = m_currentStep == enAnimationStep_Min ? enAnimationStep_Max : enAnimationStep_Min;

				/*完了。*/
				m_isCompleted = true;
			}
		}

		//大きさを変えるアニメーションの更新処理
		void ScaleUIAnimation::Update()
		{
			/*開始前の遅延時間があるか？*/
			/*ある場合は、遅延時間を減らしていく。*/
			if (m_startDelayTime > 0.0f)
			{
				m_startDelayTime -= g_gameTime->GetFrameDeltaTime();
				return;
			}

			/*繰り返し実行するか？*/
			if (!m_isLoop && m_isCompleted)
			{
				Vector2 completeScale = m_targetScale;
				m_render->SetScale(Vector3(completeScale.x, completeScale.y, 1.0f));
				m_render->Update();

				/*終了後の遅延時間があるか？*/
				/*ある場合は、遅延時間を減らしていく。*/
				if (m_endDelayTime > 0.0f)
				{
					m_endDelayTime -= g_gameTime->GetFrameDeltaTime();
					return;
				}

				m_isEnd = true;
				return;
			}

			/*経過時間の取得。*/
			const float deltaTime = g_gameTime->GetFrameDeltaTime();

			Vector2 targetScale;
			Vector2 baseScale;

			/*ステップの切り替え処理。*/
			switch (m_currentStep)
			{
			case UIAnimationBase::enAnimationStep_Min:/*下限。*/
				targetScale = m_targetScale;
				baseScale = m_baseScale;
				break;
			case UIAnimationBase::enAnimationStep_Max:/*上限。*/
				targetScale = m_baseScale;
				baseScale = m_targetScale;
				break;
			}

			/*現在のアニメーション経過時間のパーセント。*/
			const float computePercent = m_elapsedTime / m_targetTime;

			/*初期値から目標値までをなめらかに変化。*/
			Vector2 computeScale = baseScale;
			computeScale.Lerp(computePercent, baseScale, targetScale);

			/*大きさの設定。*/
			m_render->SetScale(Vector3(computeScale.x, computeScale.y, 1.0f));
			m_render->Update();

			/*経過時間の更新。*/
			m_elapsedTime += deltaTime * m_playSpeed;
			if (m_elapsedTime >= m_targetTime) {
				m_elapsedTime = 0.0f;

				m_currentStep = m_currentStep == enAnimationStep_Max ? enAnimationStep_Min : enAnimationStep_Max;

				/*完了。*/
				m_isCompleted = true;
			}
		}

		//色を変えるアニメーションの更新処理
		void ColorUIAnimation::Update()
		{
			/*開始前の遅延時間があるか？*/
			/*ある場合は、遅延時間を減らしていく。*/
			if (m_startDelayTime > 0.0f)
			{
				m_startDelayTime -= g_gameTime->GetFrameDeltaTime();
				return;
			}

			/*繰り返し実行するか？*/
			if (!m_isLoop && m_isCompleted)
			{
				Vector4 completeColor = m_targetColor;
				m_render->SetMulColor(completeColor);
				m_render->Update();

				/*終了後の遅延時間があるか？*/
				/*ある場合は、遅延時間を減らしていく。*/
				if (m_endDelayTime > 0.0f)
				{
					m_endDelayTime -= g_gameTime->GetFrameDeltaTime();
					return;
				}

				m_isEnd = true;
				return;
			}

			/*経過時間の取得。*/
			const float deltaTime = g_gameTime->GetFrameDeltaTime();

			Vector4 targetColor;
			Vector4 baseColor;

			/*ステップの切り替え処理。*/
			switch (m_currentStep)
			{
			case UIAnimationBase::enAnimationStep_Min:/*下限。*/
				targetColor = m_targetColor;
				baseColor = m_baseColor;
				break;
			case UIAnimationBase::enAnimationStep_Max:/*上限。*/
				targetColor = m_baseColor;
				baseColor = m_targetColor;
				break;
			}

			/*現在のアニメーション経過時間のパーセント。*/
			const float computePercent = m_elapsedTime / m_targetTime;

			/*初期値から目標値までをなめらかに変化。*/
			Vector4 computeColor = nsK2EngineLow::Math::Lerp<Vector4>(computePercent, baseColor, targetColor);

			/*色の設定。*/
			m_render->SetMulColor(computeColor);
			m_render->Update();

			/*経過時間の更新。*/
			m_elapsedTime += deltaTime * m_playSpeed;
			if (m_elapsedTime >= m_targetTime) {
				m_elapsedTime = 0;

				m_currentStep = m_currentStep == enAnimationStep_Min ? enAnimationStep_Max : enAnimationStep_Min;

				/*完了。*/
				m_isCompleted = true;
			}
		}

		//透明度を変えるアニメーションの更新処理
		void AlphaUIAnimation::Update()
		{
			/*開始前の遅延時間があるか？*/
			/*ある場合は、遅延時間を減らしていく。*/
			if (m_startDelayTime > 0.0f)
			{
				m_startDelayTime -= g_gameTime->GetFrameDeltaTime();
				return;
			}

			/*繰り返し実行するか？*/
			if (!m_isLoop && m_isCompleted)
			{
				float completeAlpha = m_targetAlpha;
				m_render->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, completeAlpha));
				m_render->Update();

				/*終了後の遅延時間があるか？*/
				/*ある場合は、遅延時間を減らしていく。*/
				if (m_endDelayTime > 0.0f)
				{
					m_endDelayTime -= g_gameTime->GetFrameDeltaTime();
					return;
				}

				m_isEnd = true;
				return;
			}

			const float deltaTime = g_gameTime->GetFrameDeltaTime();

			float targetAlpha;
			float baseAlpha;

			/*ステップの切り替え処理。*/
			switch (m_currentStep)
			{
			case UIAnimationBase::enAnimationStep_Min:/*下限。*/
				targetAlpha = m_targetAlpha;
				baseAlpha = m_baseAlpha;
				break;
			case UIAnimationBase::enAnimationStep_Max:/*上限。*/
				targetAlpha = m_baseAlpha;
				baseAlpha = m_targetAlpha;
				break;
			}

			/*現在のアニメーション経過時間のパーセント。*/
			const float computePercent = m_elapsedTime / m_targetTime;

			/*初期値から目標値までをなめらかに変化。*/
			float computeAlpha = nsK2EngineLow::Math::Lerp<float>(computePercent, baseAlpha, targetAlpha);

			/*透明度の設定。*/
			m_render->SetMulColor(Vector4(1.0f, 1.0f, 1.0f, computeAlpha));
			m_render->Update();

			/*経過時間の更新。*/
			m_elapsedTime += deltaTime * m_playSpeed;
			if (m_elapsedTime >= m_targetTime) {
				m_elapsedTime = 0;

				m_currentStep = m_currentStep == enAnimationStep_Min ? enAnimationStep_Max : enAnimationStep_Min;

				/*完了。*/
				m_isCompleted = true;
			}
		}
	}
}