#include "stdafx.h"
#include "GameStartDirection.h"

namespace {
	/*テキストUI。(共通)*/
	const float TEXT_UI_WIDTH = 1024;/*テキストUIの幅。*/

	const float TEXT_UI_HEIGHT = 256;/*テキストUIの高さ。*/

	/*テキストUI。(Ready)*/
	const Vector3 READY_TEXT_UI_INIT_POSITION = { 700.0f,0.0f,0.0f };/*「Ready」テキストUIの初期位置。*/

	const Vector3 READY_TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*「Ready」テキストUIの初期大きさ。*/

	/*テキストUI。(Go)*/
	const Vector3 GO_TEXT_UI_INIT_POSITION = { 0.0f,0.0f,0.0f };/*「Go」テキストUIの初期位置。*/

	const Vector3 GO_TEXT_UI_INIT_SCALE = { 2.2f,2.2f,1.0f };/*「Go」テキストUIの初期大きさ。*/

	const Vector4 GO_TEXT_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.0f };/*「Go」テキストUIの初期乗算色。*/

	/*UIアニメーション。*/
	const float SLIDE_UI_ANIMATION_PLAY_SPEED = 3.0f;/*UIをスライドさせるアニメーションの再生速度。*/

	const float SCALE_UI_ANIMATION_PLAY_SPEED = 2.0f;/*UIの大きさを変えるアニメーションの再生速度。*/

	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 2.0f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const Vector3 AFTER_UI_ANIMATION_POSITION = { 0.0f,0.0f,0.0f };/*UIをスライドさせるアニメーション後の位置。*/

	const Vector2 AFTER_UI_ANIMATION_SCALE[nsApp::nsGame::GameStartDirection::EnScale::enScale_Num][nsApp::nsGame::GameStartDirection::enTextUI_Num] = {
		/*UIを小さくする時の大きさ。*/
		Vector2{0.7f,0.7},/*「Ready」。*/
		Vector2{1.5f,1.5},/*「Go」。*/
		/*UIを大きくする時の大きさ。*/
		Vector2{1.4f,1.4},/*「Ready」。*/
		Vector2{0.2f,0.2},/*「Go」。*/
	};/*UIの大きさを変えるアニメーション後の大きさ。*/

	const float AFTER_UI_ANIMATION_ALPHA[nsApp::nsGame::GameStartDirection::EnAlpha::enAlpha_Num][nsApp::nsGame::GameStartDirection::enTextUI_Num] = {
		/*UIを透明にする時の透明度。*/
		0.0f,/*「Ready」。*/
		0.0f,/*「Go」。*/
		/*UIを不透明にする時の透明度。*/
		1.0f,/*「Ready」。*/
		1.0f,/*「Go」。*/
	};/*UIの透明度を変えるアニメーション後の透明度。*/
}

namespace nsApp
{
	namespace nsGame
	{
		/*開始処理。*/
		bool GameStartDirection::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			return true;
		}

		/*更新処理。*/
		void GameStartDirection::Update()
		{
			/*演出。*/
			UpdateDirection();

			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void GameStartDirection::Render(RenderContext& rc)
		{
			for (int i = 0; i < enTextUI_Num; i++)
			{
				/*テキストUIの描画。*/
				m_textUI[i].Draw(rc);
			}
		}

		/*スプライトの初期化。*/
		void GameStartDirection::InitSprite()
		{
			/*テキストUI。*/
			InitTextUI();
		}

		/*テキストUIの初期化。*/
		void GameStartDirection::InitTextUI()
		{
			m_textUI[enTextUI_Ready].Init(m_textUIFilePath[enTextUI_Ready].c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);/*初期化。*/
			m_textUI[enTextUI_Ready].SetPosition(READY_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_textUI[enTextUI_Ready].SetScale(READY_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textUI[enTextUI_Ready].Update();/*更新処理。*/

			m_textUI[enTextUI_Go].Init(m_textUIFilePath[enTextUI_Go].c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);/*初期化。*/
			m_textUI[enTextUI_Go].SetPosition(GO_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_textUI[enTextUI_Go].SetScale(GO_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textUI[enTextUI_Go].SetMulColor(GO_TEXT_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_textUI[enTextUI_Go].Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void GameStartDirection::InitUIAnimation()
		{
			/*UIをスライドさせるアニメーション。*/
			InitSlideUIAnimation();

			/*UIの大きさを変えるアニメーション。*/
			InitScaleUIAnimation();

			/*UIの透明度を変えるアニメーション。*/
			InitAlphaUIAnimation();
		}

		/*UIをスライドさせるアニメーションの初期化。*/
		void GameStartDirection::InitSlideUIAnimation()
		{
			/*UIをスライドさせるアニメーションの値の設定。*/
			Vector3 basePosition = m_textUI[enTextUI_Ready].GetPosition();/*元の位置。*/
			Vector3 targetPosition = AFTER_UI_ANIMATION_POSITION;/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_textUI[enTextUI_Ready],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);
		}

		/*UIの大きさを変えるアニメーションの初期化。*/
		void GameStartDirection::InitScaleUIAnimation()
		{
			/*UIの大きさを変えるアニメーションの値の設定。*/
			Vector2 baseScale = {m_textUI[enTextUI_Ready].GetScale().x,m_textUI[enTextUI_Ready].GetScale().y };/*元の大きさ。*/
			Vector2 targetScale = AFTER_UI_ANIMATION_SCALE[enScale_Down][enTextUI_Ready];/*ターゲット大きさ。*/

			/*初期化。*/
			m_scaleUIAnimation[enScale_Down][enTextUI_Ready] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_textUI[enTextUI_Ready],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/
			);

			/*UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = {m_textUI[enTextUI_Go].GetScale().x,m_textUI[enTextUI_Go].GetScale().y };/*元の大きさ。*/
			targetScale = AFTER_UI_ANIMATION_SCALE[enScale_Down][enTextUI_Go];/*ターゲット大きさ。*/

			/*初期化。*/
			m_scaleUIAnimation[enScale_Down][enTextUI_Go] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_textUI[enTextUI_Go],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.5f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/
			);

			/*UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = AFTER_UI_ANIMATION_SCALE[enScale_Down][enTextUI_Ready];/*元の大きさ。*/
			targetScale = AFTER_UI_ANIMATION_SCALE[enScale_Up][enTextUI_Ready];/*ターゲット大きさ。*/

			/*初期化。*/
			m_scaleUIAnimation[enScale_Up][enTextUI_Ready] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_textUI[enTextUI_Ready],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/
			);

			/*UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = AFTER_UI_ANIMATION_SCALE[enScale_Down][enTextUI_Go];/*元の大きさ。*/
			targetScale = AFTER_UI_ANIMATION_SCALE[enScale_Up][enTextUI_Go];/*ターゲット大きさ。*/

			/*初期化。*/
			m_scaleUIAnimation[enScale_Up][enTextUI_Go] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_textUI[enTextUI_Go],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/
			);
		}

		/*UIの透明度を変えるアニメーションの初期化。*/
		void GameStartDirection::InitAlphaUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = AFTER_UI_ANIMATION_ALPHA[enAlpha_Up][enTextUI_Ready];/*元の透明度。*/
			float targetAlpha = AFTER_UI_ANIMATION_ALPHA[enAlpha_Down][enTextUI_Ready];/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlpha_Down][enTextUI_Ready] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_textUI[enTextUI_Ready],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = AFTER_UI_ANIMATION_ALPHA[enAlpha_Up][enTextUI_Ready];/*元の透明度。*/
			targetAlpha = AFTER_UI_ANIMATION_ALPHA[enAlpha_Down][enTextUI_Ready];/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlpha_Down][enTextUI_Go] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_textUI[enTextUI_Go],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = AFTER_UI_ANIMATION_ALPHA[enAlpha_Down][enTextUI_Ready];/*元の透明度。*/
			targetAlpha = AFTER_UI_ANIMATION_ALPHA[enAlpha_Up][enTextUI_Ready];/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlpha_Up][enTextUI_Ready] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_textUI[enTextUI_Ready],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = AFTER_UI_ANIMATION_ALPHA[enAlpha_Down][enTextUI_Go];/*元の透明度。*/
			targetAlpha = AFTER_UI_ANIMATION_ALPHA[enAlpha_Up][enTextUI_Go];/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlpha_Up][enTextUI_Go] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_textUI[enTextUI_Go],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.5f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);
		}

		/*演出の更新処理。*/
		void GameStartDirection::UpdateDirection()
		{
			if (!m_slideUIAnimation->IsEnd())
			{
				m_slideUIAnimation->Update();
			}
			else if (!m_scaleUIAnimation[enScale_Down][enTextUI_Ready]->IsEnd())
			{
				m_scaleUIAnimation[enScale_Down][enTextUI_Ready]->Update();
			}
			else if (!m_alphaUIAnimation[enScale_Down][enTextUI_Ready]->IsEnd())
			{
				m_scaleUIAnimation[enScale_Up][enTextUI_Ready]->Update();
				m_alphaUIAnimation[enAlpha_Down][enTextUI_Ready]->Update();
			}
			else if (!m_scaleUIAnimation[enScale_Down][enTextUI_Go]->IsEnd())
			{
				m_scaleUIAnimation[enScale_Down][enTextUI_Go]->Update();
				m_alphaUIAnimation[enAlpha_Up][enTextUI_Go]->Update();
			}
			else if(!m_alphaUIAnimation[enAlpha_Down][enTextUI_Go]->IsEnd())
			{
				m_alphaUIAnimation[enAlpha_Down][enTextUI_Go]->Update();
			}
			else
			{
				m_isDirectionFinished = true;/*演出終了。*/
			}
		}

		/*スプライトの更新処理。*/
		void GameStartDirection::UpdateSprite()
		{
			for (int i = 0; i < enTextUI_Num; i++)
			{
				m_textUI[i].Update();/*更新処理。*/
			}
		}
	}
}