#include "stdafx.h"
#include "Fade.h"

namespace {
	/*フェード用の背景。*/
	const float FADE_BACK_GROUND_WIDTH = 1920;/*フェイド用の背景の幅。*/

	const float FADE_BACK_GROUND_HEIGHT = 1080;/*フェイド用の背景の高さ。*/

	/*透明度*/
	const float ALPHA_MAX = 1.0f;/*最大の透明度。*/

	const float ALPHA_MIN = 0.0f;/*最小の透明度。*/

	const float ALPHA_HALF = 0.5f;/*半分の透明度。*/
}

namespace nsApp
{
	namespace nsFade
	{
		Fade* Fade::m_instance = nullptr;/*シングルトンインスタンスの初期化。*/

		/*開始処理。*/
		bool Fade::Start() {
			/*スプライトの初期化。*/
			InitSprite();

			return true;
		}

		/*更新処理。*/
		void Fade::Update()
		{
			switch (m_currentFadeType)
			{
			case nsApp::nsFade::Fade::enFadeType_FadeIn:
				m_currentAlpha -= g_gameTime->GetFrameDeltaTime();

				if (m_currentAlpha < ALPHA_MIN)
				{
					m_currentAlpha = ALPHA_MIN;
					m_currentFadeType = enFadeType_End;
				}

				break;
			case nsApp::nsFade::Fade::enFadeType_FadeOut:
				m_currentAlpha += g_gameTime->GetFrameDeltaTime();

				if (m_currentAlpha > ALPHA_MAX)
				{
					m_currentAlpha = ALPHA_MAX;
					m_currentFadeType = enFadeType_End;
				}

				break;
			case nsApp::nsFade::Fade::enFadeType_HalfFadeIn:
				m_currentAlpha -= g_gameTime->GetFrameDeltaTime();

				if (m_currentAlpha < ALPHA_HALF)
				{
					m_currentAlpha = ALPHA_HALF;
					m_currentFadeType = enFadeType_End;
				}

				break;
			case nsApp::nsFade::Fade::enFadeType_HalfFadeOut:
				m_currentAlpha += g_gameTime->GetFrameDeltaTime();

				if (m_currentAlpha > ALPHA_HALF)
				{
					m_currentAlpha = ALPHA_HALF;
					m_currentFadeType = enFadeType_End;
				}

				break;
			default:
				break;
			}

			m_fadeBackGround.SetMulColor(Vector4{ 1.0f,1.0f,1.0f,m_currentAlpha });/*フェード用の背景の透明度の設定。*/
			m_fadeBackGround.Update();
		}

		/*描画処理。*/
		void Fade::Render(RenderContext& rc)
		{
			/*フェイド用の背景の描画。*/
			m_fadeBackGround.Draw(rc);
		}

		/*スプライトの初期化。*/
		void Fade::InitSprite()
		{
			/*フェイド用の背景*/
			InitFadeBackGround();
		}

		/*フェイド用の背景の初期化。*/
		void Fade::InitFadeBackGround()
		{
			m_fadeBackGround.Init(m_fadeBackGroundFilePath.c_str(), FADE_BACK_GROUND_WIDTH, FADE_BACK_GROUND_HEIGHT);/*初期化。*/
			m_currentAlpha = m_fadeBackGround.GetMulColor().a;/*現在の透明度の取得。*/
		}
	}
}