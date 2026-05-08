#include "stdafx.h"
#include "GameOverDirection.h"
#include "Src/Fade/Fade.h"

namespace {
	/*テキストUI。*/
	const float TEXT_UI_WIDTH = 1024;/*テキストUIの幅。*/

	const float TEXT_UI_HEIGHT = 256;/*テキストUIの高さ。*/

	const Vector3 TEXT_UI_INIT_POSITION = { 0.0f,275.0f,0.0f };/*テキストUIの初期位置。*/

	const float TEXT_UI_INIT_ROTATION_ANGLE = 15.0f;/*テキストUIの初期回転角度。*/

	const Vector3 TEXT_UI_INIT_SCALE = { 1.45f,1.45f,1.0f };/*テキストUIの初期大きさ。*/

	const Vector2 TEXT_UI_INIT_PIVOT = { 0.5f,0.5f };/*テキストUIの初期ピボット。*/

	const Vector4 TEXT_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.0f };/*テキストUIの初期乗算色。*/

	/*UIアニメーション。*/
	const float POSITION_UI_ANIMATION_PLAY_SPEED = 2.15f;/*UIの位置を変えるアニメーションの再生速度。*/

	const float ROTATION_UI_ANIMATION_PLAY_SPEED = 2.15f;/*UIの回転を変えるアニメーションの再生速度。*/

	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 2.0f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const Vector3 AFTER_UI_ANIMATION_POSITION[nsApp::nsGame::GameOverDirection::EnPositionUIAnimationCount::enPositionUIAnimationCount_Num] = 
	{ 
		Vector3{0.0f,50.0f,0.0f},/*1回目。*/
		Vector3{0.0f,150.0f,0.0f},/*2回目。*/
		Vector3{0.0f,50.0f,0.0f},/*3回目。*/
		Vector3{0.0f,120.0f,0.0f},/*4回目。*/
		Vector3{0.0f,40.0f,0.0f},/*5回目。*/
		Vector3{0.0f,50.0f,0.0f},/*6回目。*/
	};/*UIの位置を変えるアニメーション後の位置。*/

	const float AFTER_UI_ANIMATION_ROTATION_ANGLE[nsApp::nsGame::GameOverDirection::EnRotationUIAnimationCount::enRotationUIAnimationCount_Num] = 
	{
		-15.0f,/*1回目。*/
		5.0f,/*2回目。*/
		-5.0f,/*3回目。*/
		1.0f,/*4回目。*/
		0.0f,/*5回目。*/
	};/*UIの回転を変えるアニメーション後の回転の角度。*/

	const float AFTER_UI_ANIMATION_ALPHA = 1.0f;/*UIの透明度を変えるアニメーション後の透明度。*/
}

namespace nsApp
{
	namespace nsGame
	{
		/*開始処理。*/
		bool GameOverDirection::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			/*半透明フェードアウトに切り替える。*/
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_HalfFadeOut);

			return true;
		}

		/*更新処理。*/
		void GameOverDirection::Update()
		{
			if (!nsApp::nsFade::Fade::GetInstance()->IsHalfFadeOut())
			{
				/*演出。*/
				UpdateDirection();
			}

			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void GameOverDirection::Render(RenderContext& rc)
		{
			/*テキストUIの描画。*/
			m_textUI.Draw(rc);
		}

		/*スプライトの初期化。*/
		void GameOverDirection::InitSprite()
		{
			/*テキストUI。*/
			InitTextUI();
		}

		/*テキストUIの初期化。*/
		void GameOverDirection::InitTextUI()
		{
			m_textUI.Init(m_textUIFilePath.c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);/*初期化。*/
			m_textUI.SetPosition(TEXT_UI_INIT_POSITION);/*位置設定。*/

			Quaternion initRotation;/*初期回転。*/
			initRotation.SetRotationDegZ(TEXT_UI_INIT_ROTATION_ANGLE);/*初期回転の角度設定。*/
			m_textUI.SetRotation(initRotation);/*回転設定。*/

			m_textUI.SetScale(TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textUI.SetPivot(TEXT_UI_INIT_PIVOT);/*ピボット設定。*/
			m_textUI.SetMulColor(TEXT_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_textUI.Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void GameOverDirection::InitUIAnimation()
		{
			/*UIの位置を変えるアニメーション。*/
			InitPositionUIAnimation();

			/*UIの回転を変えるアニメーション。*/
			InitRotationUIAnimation();

			/*UIの透明度を変えるアニメーション。*/
			InitAlphaUIAnimation();
		}

		/*UIの位置を変えるアニメーションの初期化。*/
		void GameOverDirection::InitPositionUIAnimation()
		{
			/*UIの位置を変えるアニメーションの値の設定。*/
			Vector3 basePosition = m_textUI.GetPosition();/*元の位置。*/
			Vector3 targetPosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_One];/*ターゲットの位置。*/

			/*初期化。*/
			m_positionUIAnimation[enPositionUIAnimationCount_One] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				POSITION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIの位置を変えるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_One];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_Two];/*ターゲットの位置。*/

			/*初期化。*/
			m_positionUIAnimation[enPositionUIAnimationCount_Two] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				POSITION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIの位置を変えるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_Two];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_Three];/*ターゲットの位置。*/

			/*初期化。*/
			m_positionUIAnimation[enPositionUIAnimationCount_Three] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				POSITION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIの位置を変えるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_Three];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_Four];/*ターゲットの位置。*/

			/*初期化。*/
			m_positionUIAnimation[enPositionUIAnimationCount_Four] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				POSITION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIの位置を変えるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_Four];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_Five];/*ターゲットの位置。*/

			/*初期化。*/
			m_positionUIAnimation[enPositionUIAnimationCount_Five] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				POSITION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIの位置を変えるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_Five];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enPositionUIAnimationCount_Six];/*ターゲットの位置。*/

			/*初期化。*/
			m_positionUIAnimation[enPositionUIAnimationCount_Six] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				POSITION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				1.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);
		}

		/*UIの大きさを変えるアニメーションの初期化。*/
		void GameOverDirection::InitRotationUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			Quaternion baseRotation = m_textUI.GetRotation();/*元の回転。*/
			Quaternion targetRotation;
			targetRotation.SetRotationDegZ(AFTER_UI_ANIMATION_ROTATION_ANGLE[enRotationUIAnimationCount_One]);/*ターゲットの回転。*/

			/*初期化。*/
			m_rotationUIAnimation[enRotationUIAnimationCount_One] = std::make_unique<nsApp::nsUI::RotationUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ROTATION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseRotation,/*元の透明度。*/
				targetRotation/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseRotation.SetRotationDegZ(AFTER_UI_ANIMATION_ROTATION_ANGLE[enRotationUIAnimationCount_One]);/*元の回転。*/
			targetRotation.SetRotationDegZ(AFTER_UI_ANIMATION_ROTATION_ANGLE[enRotationUIAnimationCount_Two]);/*ターゲットの回転。*/

			/*初期化。*/
			m_rotationUIAnimation[enRotationUIAnimationCount_Two] = std::make_unique<nsApp::nsUI::RotationUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ROTATION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseRotation,/*元の透明度。*/
				targetRotation/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseRotation.SetRotationDegZ(AFTER_UI_ANIMATION_ROTATION_ANGLE[enRotationUIAnimationCount_Two]);/*元の回転。*/
			targetRotation.SetRotationDegZ(AFTER_UI_ANIMATION_ROTATION_ANGLE[enRotationUIAnimationCount_Three]);/*ターゲットの回転。*/

			/*初期化。*/
			m_rotationUIAnimation[enRotationUIAnimationCount_Three] = std::make_unique<nsApp::nsUI::RotationUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ROTATION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseRotation,/*元の透明度。*/
				targetRotation/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseRotation.SetRotationDegZ(AFTER_UI_ANIMATION_ROTATION_ANGLE[enRotationUIAnimationCount_Three]);/*元の回転。*/
			targetRotation.SetRotationDegZ(AFTER_UI_ANIMATION_ROTATION_ANGLE[enRotationUIAnimationCount_Four]);/*ターゲットの回転。*/

			/*初期化。*/
			m_rotationUIAnimation[enRotationUIAnimationCount_Four] = std::make_unique<nsApp::nsUI::RotationUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ROTATION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseRotation,/*元の透明度。*/
				targetRotation/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseRotation.SetRotationDegZ(AFTER_UI_ANIMATION_ROTATION_ANGLE[enRotationUIAnimationCount_Four]);/*元の回転。*/
			targetRotation.SetRotationDegZ(AFTER_UI_ANIMATION_ROTATION_ANGLE[enRotationUIAnimationCount_Five]);/*ターゲットの回転。*/

			/*初期化。*/
			m_rotationUIAnimation[enRotationUIAnimationCount_Five] = std::make_unique<nsApp::nsUI::RotationUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ROTATION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseRotation,/*元の透明度。*/
				targetRotation/*ターゲットの透明度。*/
			);
		}

		/*UIの透明度を変えるアニメーションの初期化。*/
		void GameOverDirection::InitAlphaUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = m_textUI.GetMulColor().a;/*元の透明度。*/
			float targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_textUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);
		}

		/*演出の更新処理。*/
		void GameOverDirection::UpdateDirection()
		{
			if (!m_positionUIAnimation[enPositionUIAnimationCount_One]->IsEnd())
			{
				m_positionUIAnimation[enPositionUIAnimationCount_One]->Update();
				m_alphaUIAnimation->Update();
			}
			else if (!m_positionUIAnimation[enPositionUIAnimationCount_Two]->IsEnd())
			{
				m_positionUIAnimation[enPositionUIAnimationCount_Two]->Update();
				m_rotationUIAnimation[enRotationUIAnimationCount_One]->Update();
			}
			else if (!m_positionUIAnimation[enPositionUIAnimationCount_Three]->IsEnd())
			{
				m_positionUIAnimation[enPositionUIAnimationCount_Three]->Update();
				m_rotationUIAnimation[enRotationUIAnimationCount_Two]->Update();
			}
			else if (!m_positionUIAnimation[enPositionUIAnimationCount_Four]->IsEnd())
			{
				m_positionUIAnimation[enPositionUIAnimationCount_Four]->Update();
				m_rotationUIAnimation[enRotationUIAnimationCount_Three]->Update();
			}
			else if (!m_positionUIAnimation[enPositionUIAnimationCount_Five]->IsEnd())
			{
				m_positionUIAnimation[enPositionUIAnimationCount_Five]->Update();
				m_rotationUIAnimation[enRotationUIAnimationCount_Four]->Update();
			}
			else if (!m_positionUIAnimation[enPositionUIAnimationCount_Six]->IsEnd())
			{
				m_positionUIAnimation[enPositionUIAnimationCount_Six]->Update();
				m_rotationUIAnimation[enRotationUIAnimationCount_Five]->Update();
			}
			else
			{
				m_isDirectionFinished = true;/*演出終了。*/
			}
		}

		/*スプライトの更新処理。*/
		void GameOverDirection::UpdateSprite()
		{
			m_textUI.Update();/*更新処理。*/
		}
	}
}