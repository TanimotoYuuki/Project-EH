#include "stdafx.h"
#include "GameClearDirection.h"
#include "Src/Fade/Fade.h"

namespace {
	/*テキストUI。*/
	const float TEXT_UI_WIDTH = 1024;/*テキストUIの幅。*/

	const float TEXT_UI_HEIGHT = 256;/*テキストUIの高さ。*/

	const Vector3 TEXT_UI_INIT_POSITION = { 0.0f,375.0f,0.0f };/*テキストUIの初期位置。*/

	const Vector3 TEXT_UI_INIT_SCALE = { 1.25f,1.25f,1.0f };/*テキストUIの初期大きさ。*/

	const Vector4 TEXT_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.0f };/*テキストUIの初期乗算色。*/

	/*UIアニメーション。*/
	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 2.0f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const float AFTER_UI_ANIMATION_ALPHA = 1.0f;/*UIの透明度を変えるアニメーション後の透明度。*/
}

namespace nsApp
{
	namespace nsGame
	{
		/*開始処理。*/
		bool GameClearDirection::Start()
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
		void GameClearDirection::Update()
		{
			if (!nsApp::nsFade::Fade::GetInstance()->IsHalfFadeOut())
			{
				/*演出。*/
				UpdateDirection();
			}

			if (IsDirectionFinished())
			{
				/*フェードアウトに切り替える。*/
				nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeOut);
			}

			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void GameClearDirection::Render(RenderContext& rc)
		{
			/*テキストUIの描画。*/
			m_textUI.Draw(rc);
		}

		/*スプライトの初期化。*/
		void GameClearDirection::InitSprite()
		{
			/*テキストUI。*/
			InitTextUI();
		}

		/*テキストUIの初期化。*/
		void GameClearDirection::InitTextUI()
		{
			m_textUI.Init(m_textUIFilePath.c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);/*初期化。*/
			m_textUI.SetPosition(TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_textUI.SetScale(TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textUI.SetMulColor(TEXT_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_textUI.Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void GameClearDirection::InitUIAnimation()
		{
			/*UIの透明度を変えるアニメーション。*/
			InitAlphaUIAnimation();
		}

		/*UIの透明度を変えるアニメーションの初期化。*/
		void GameClearDirection::InitAlphaUIAnimation()
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
				1.5f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);
		}

		/*演出の更新処理。*/
		void GameClearDirection::UpdateDirection()
		{
			if (!m_alphaUIAnimation->IsEnd())
			{
				m_alphaUIAnimation->Update();
			}
			else
			{
				m_isDirectionFinished = true;/*演出終了。*/
			}
		}

		/*スプライトの更新処理。*/
		void GameClearDirection::UpdateSprite()
		{
			m_textUI.Update();/*更新処理。*/
		}
	}
}