#include "stdafx.h"
#include "Title.h"
#include "Src/SceneLoader/SceneLoader.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Fade/Fade.h"


namespace {
	/*背景。*/
	const float BACK_GROUND_WIDTH = 1920;/*背景の幅。*/

	const float BACK_GROUND_HEIGHT = 1080;/*背景の高さ。* /

	/*タイトル名UI。*/
	const float TITLE_NAME_UI_WIDTH = 1024.0f;/*タイトル名UIの幅。*/

	const float TITLE_NAME_UI_HEIGHT = 512.0f;/*タイトル名UIの高さ。*/

	const Vector3 TITLE_NAME_UI_INIT_POSITION = { 0.0f,150.0f,0.0f };/*タイトル名UIの初期位置。*/

	const Vector3 TITLE_NAME_UI_INIT_SCALE = { 1.75f,1.75f,1.0f };/*タイトル名UIの初期大きさ。*/

	const Vector4 TITLE_NAME_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.0f };/*タイトル名UIの初期乗算色。*/

	/*Aボタンを押してくださいを促すUI。*/
	const float PRESS_A_BUTTON_UI_WIDTH = 1024.0f;/*Aボタンを押してくださいを促すUIの幅。*/

	const float PRESS_A_BUTTON_UI_HEIGHT = 256.0f;/*Aボタンを押してくださいを促すUIの高さ。*/

	const Vector3 PRESS_A_BUTTON_UI_INIT_POSITION = { 0.0f,-150.0f,0.0f };/*Aボタンを押してくださいを促すUIの初期位置。*/

	const Vector3 PRESS_A_BUTTON_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*Aボタンを押してくださいを促すUIの初期大きさ。*/

	const Vector4 PRESS_A_BUTTON_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.0f };/*Aボタンを押してくださいを促すUIの初期乗算色。*/

	/*UIアニメーション。*/
	const float SCALE_DOWN_UI_ANIMATION_PLAY_SPEED = 1.0f;/*UIの大きさを小さくするアニメーションの再生速度。*/

	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 1.0f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const Vector2 AFTER_UI_ANIMATION_SCALE = { 1.5f,1.5f };/*UIの大きさを小さくするアニメーションの再生後の大きさ。*/

	const float AFTER_UI_ANIMATION_ALPHA = 1.0f;/*UIの透明度を変えるアニメーションの再生後の透明度。*/
}

namespace nsApp
{
	namespace nsTitle
	{
		/*開始処理。*/
		bool Title::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			/*フェードインに切り替える。*/
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeIn);/*フェードイン開始。*/

			/* タイトルBGMを再生。*/
			m_bgm = NewGO<nsSound::SoundLister>(0, "SoundManager");
			m_bgm->GetBGMList().Init();
			m_bgm->GetBGMList().PlayBGM(nsSound::BGM_ID::Title, 1.0f);

			return true;
		}

		/*更新処理。*/
		void Title::Update()
		{
			/*フェードイン中は処理しない。*/
			if (nsApp::nsFade::Fade::GetInstance()->IsFadeIn())
			{
				return;
			}

			/*演出。*/
			UpdateDirection();

			/*タイトルでの最初の演出が終わったら選択できる。*/
			if(m_scaleDownUIAnimation->IsEnd() && m_alphaUIAnimation[enUIAnimationSprite_TitleNameUI]->IsEnd())
			{
				/*Aボタンを押したら選択シーンに遷移する処理をかける。*/
				if (g_pad[0]->IsTrigger(enButtonA))
				{
					/*フェードアウトに切り替え。*/
					nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeOut);
					m_didSelect = true;

					/* BGNの停止処理。*/
					if (m_bgm != nullptr)
						StopBGM();
				}
			}

			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void Title::Render(RenderContext& rc)
		{
			/*背景の描画。*/
			m_backGround.Draw(rc);

			/*タイトル名を表示するUIの描画。*/
			m_titleNameUI.Draw(rc);

			/*Aボタンを押してくださいを促すUIの描画。*/
			m_pressAButtonUI.Draw(rc);
		}

		/*スプライトの初期化。*/
		void Title::InitSprite()
		{
			/*背景。*/
			InitBackGround();

			/*タイトル名。*/
			InitTitleNameUI();

			/*Aボタンを押してくださいを促すUI。*/
			InitPressAButtonUI();
		}

		/*背景の初期化。*/
		void Title::InitBackGround()
		{
			m_backGround.Init(m_backGroundFilePath.c_str(), BACK_GROUND_WIDTH, BACK_GROUND_HEIGHT);/*初期化。*/
		}

		/*タイトル名UIの初期化。*/
		void Title::InitTitleNameUI()
		{
			m_titleNameUI.Init(m_titleNameUIFilePath.c_str(), TITLE_NAME_UI_WIDTH, TITLE_NAME_UI_HEIGHT);/*初期化。*/
			m_titleNameUI.SetPosition(TITLE_NAME_UI_INIT_POSITION);/*位置設定。*/
			m_titleNameUI.SetScale(TITLE_NAME_UI_INIT_SCALE);/*大きさ設定。*/
			m_titleNameUI.SetMulColor(TITLE_NAME_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_titleNameUI.Update();/*更新処理。*/
		}

		/*Aボタンを押してくださいを促すUIの初期化。*/
		void Title::InitPressAButtonUI()
		{
			m_pressAButtonUI.Init(m_pressAButtonUIFilePath.c_str(), PRESS_A_BUTTON_UI_WIDTH, PRESS_A_BUTTON_UI_HEIGHT);/*初期化。*/
			m_pressAButtonUI.SetPosition(PRESS_A_BUTTON_UI_INIT_POSITION);/*位置設定。*/
			m_pressAButtonUI.SetScale(PRESS_A_BUTTON_UI_INIT_SCALE);/*大きさ設定。*/
			m_pressAButtonUI.SetMulColor(PRESS_A_BUTTON_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_pressAButtonUI.Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void Title::InitUIAnimation()
		{
			/*UIの大きさを小さくするアニメーションの値の設定。*/
			Vector2 baseScale = { m_titleNameUI.GetScale().x,m_titleNameUI.GetScale().y };/*元の大きさ。*/
			Vector2 targetScale = AFTER_UI_ANIMATION_SCALE;/*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleDownUIAnimation = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_titleNameUI,/*スプライトをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_DOWN_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ*/
				targetScale/*ターゲットの大きさ。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = m_titleNameUI.GetMulColor().a;/*元の透明度。*/
			float targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enUIAnimationSprite_TitleNameUI] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_titleNameUI,/*スプライトをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_pressAButtonUI.GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enUIAnimationSprite_PressAButtonUI] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_pressAButtonUI,/*スプライトをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				true,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);
		}

		/*演出の更新処理。*/
		void Title::UpdateDirection()
		{
			if (!m_scaleDownUIAnimation->IsEnd() && !m_alphaUIAnimation[enUIAnimationSprite_TitleNameUI]->IsEnd())
			{
				/*UIの大きさを小さくするアニメーションの再生。*/
				m_scaleDownUIAnimation->Update();

				/*UIの透明度を変えるアニメーションの再生。*/
				m_alphaUIAnimation[enUIAnimationSprite_TitleNameUI]->Update();
			}
			else
			{
				/*UIの透明度を変えるアニメーションの再生。*/
				m_alphaUIAnimation[enUIAnimationSprite_PressAButtonUI]->Update();
			}
		}
		
		/*スプライトの更新処理。*/
		void Title::UpdateSprite()
		{
			/*背景。*/
			m_backGround.Update();

			/*タイトル名UI。*/
			m_titleNameUI.Update();

			/*Aボタンを押してくださいを促すUI。*/
			m_pressAButtonUI.Update();
		}
	}
}