#include "stdafx.h"
#include "ConfirmationSelect.h"
#include "Src/Fade/Fade.h"

namespace {
	/*確認UI。*/
	const float CONFIRMATION_UI_WIDTH = 1980;/*確認UIの幅。*/

	const float CONFIRMATION_UI_HEIGHT = 1080;/*確認UIの高さ。*/

	const Vector3 CONFIRMATION_UI_INIT_POSITION = { 0.0f,0.0f,0.0f };/*確認UIの初期位置。*/

	const Vector3 CONFIRMATION_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*確認UIの初期大きさ。*/

	/*確認選択UI。*/
	const float CONFIRMATION_SELECT_UI_WIDTH = 1024;/*確認選択UIの幅。*/

	const float CONFIRMATION_SELECT_UI_HEIGHT = 256;/*確認選択UIの高さ。*/

	const Vector3 CONFIRMATION_SELECT_UI_INIT_POSITION = { -325.0f,-150.0f,0.0f };/*確認選択UIの初期位置。*/

	const Vector3 CONFIRMATION_SELECT_UI_INIT_SCALE = { 1.04f,1.23f,1.0f };/*確認選択UIの初期大きさ。*/

	const Vector4 CONFIRMATION_SELECT_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.6f };/*確認選択選択UIの初期乗算色。*/

	/*確認選択テキストUI。*/
	const float CONFIRMATION_SELECT_TEXT_UI_WIDTH = 1024;/*確認選択テキストUIの幅。*/

	const float CONFIRMATION_SELECT_TEXT_UI_HEIGHT = 256;/*確認選択テキストUIの高さ。*/

	const Vector3 CONFIRMATION_SELECT_TEXT_UI_INIT_POSITION = { -325.0f,-150.0f,0.0f };/*確認選択テキストUIの初期位置。*/

	const float CONFIRMATION_SELECT_TEXT_UI_POSITION_INTERVAL = 600.0f;/*確認選択テキストUIの位置の間隔。*/

	const Vector3 CONFIRMATION_SELECT_TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*確認選択テキストUIの初期大きさ。*/

	/*UIアニメーション。*/
	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 1.2f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const float AFTER_UI_ANIMATION_ALPHA = 0.2f;/*UIの透明度を変えるアニメーション後の透明度。*/
}

namespace nsApp
{
	namespace nsSelect
	{
		/*開始処理。*/
		bool ConfirmationSelect::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();
			
			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			return true;
		}

		/*更新処理。*/
		void ConfirmationSelect::Update()
		{
			/*選択。*/
			UpdateSelect();

			/*UIアニメーション。*/
			UpdateUIAnimation();

			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void ConfirmationSelect::Render(RenderContext& rc)
		{
			/*UIを描画しない状態なら処理しない。*/
			if (!IsDrawingUI())
			{
				return;
			}

			/*確認UIの描画。*/
			m_confirmationUI.Draw(rc);

			/*確認選択の描画。*/
			m_confirmationSelectUI.Draw(rc);

			for (int i = 0; i < enConfirmationSelectTextUI_Num; i++)
			{
				/*確認選択テキストUIの描画。*/
				m_confirmationSelectTextUI[i].Draw(rc);
			}
		}

		/*スプライトの初期化。*/
		void ConfirmationSelect::InitSprite()
		{
			/*確認UI。*/
			InitConfirmationUI();

			/*確認選択UI。*/
			InitConfirmationSelectUI();

			for (int i = 0; i < enConfirmationSelectTextUI_Num; i++)
			{
				/*確認選択テキストUI。*/
				InitConfirmationSelectTextUI((EnConfirmationSelectTextUI)i, i);
			}
		}

		/*確認UIの初期化。*/
		void ConfirmationSelect::InitConfirmationUI()
		{
			m_confirmationUI.Init(m_confirmationUIFilePath.c_str(), CONFIRMATION_UI_WIDTH, CONFIRMATION_UI_HEIGHT);/*初期化。*/
			m_confirmationUI.SetPosition(CONFIRMATION_UI_INIT_POSITION);/*位置設定。*/
			m_confirmationUI.SetScale(CONFIRMATION_UI_INIT_SCALE);/*大きさ設定。*/
			m_confirmationUI.Update();/*更新処理。*/
		}

		/*確認選択UIの初期化。*/
		void ConfirmationSelect::InitConfirmationSelectUI()
		{
			m_confirmationSelectUI.Init(m_confirmationSelectUIFilePath.c_str(), CONFIRMATION_SELECT_UI_WIDTH, CONFIRMATION_SELECT_UI_HEIGHT);/*初期化。*/
			m_confirmationSelectUI.SetPosition(CONFIRMATION_SELECT_UI_INIT_POSITION);/*位置設定。*/
			m_confirmationSelectUI.SetScale(CONFIRMATION_SELECT_UI_INIT_SCALE);/*大きさ設定。*/
			m_confirmationSelectUI.SetMulColor(CONFIRMATION_SELECT_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_confirmationSelectUI.Update();/*更新処理。*/
		}

		/*確認選択テキストUIの初期化。*/
		void ConfirmationSelect::InitConfirmationSelectTextUI(EnConfirmationSelectTextUI confirmationSelectTextUI, int confirmationSelectTextIndex)
		{
			Vector3 initPosition = CONFIRMATION_SELECT_TEXT_UI_INIT_POSITION;/*初期位置。*/
			initPosition.x += CONFIRMATION_SELECT_TEXT_UI_POSITION_INTERVAL * confirmationSelectTextIndex;/*位置の間隔を加算。*/

			m_confirmationSelectTextUI[confirmationSelectTextUI].Init(m_confirmationSelectTextFilePath[confirmationSelectTextUI].c_str(), CONFIRMATION_SELECT_TEXT_UI_WIDTH, CONFIRMATION_SELECT_TEXT_UI_HEIGHT);/*初期化。*/
			m_confirmationSelectTextUI[confirmationSelectTextUI].SetPosition(initPosition);/*位置設定。*/
			m_confirmationSelectTextUI[confirmationSelectTextUI].SetScale(CONFIRMATION_SELECT_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_confirmationSelectTextUI[confirmationSelectTextUI].Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void ConfirmationSelect::InitUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = m_confirmationSelectUI.GetMulColor().a;/*元の透明度。*/
			float targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_confirmationSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				true,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);
		}

		/*選択の更新処理。*/
		void ConfirmationSelect::UpdateSelect()
		{
			/*左を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonLeft))
			{
				ResetAlphaUIAnimation();
				/*現在の選択が「いいえ」を選択していたら。*/
				if (m_currentSelect == enSelect_No) { m_currentSelect = enSelect_Yes; return; }

				/*「いいえ」を選択する。*/
				m_currentSelect--;
				return;
			}

			/*右を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonRight))
			{
				ResetAlphaUIAnimation();
				/*現在の選択が「はい」を選択していたら。*/
				if (m_currentSelect == enSelect_Yes) { m_currentSelect = enSelect_No; return; }

				/*「はい」を選択する。*/
				m_currentSelect++;
				return;
			}

			/*Aボタンを押したら内容を元に処理する。*/
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				if (m_currentSelect == enSelect_Yes)
				{
					nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::enFadeType_FadeOut);
				}
				else
				{
					nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::enFadeType_FadeIn);
				}
				EnableSelect();
			}
		}

		/*UIアニメーションの更新処理。*/
		void ConfirmationSelect::UpdateUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの再生。*/
			m_alphaUIAnimation->Update();
		}

		/*スプライトの更新処理。*/
		void ConfirmationSelect::UpdateSprite()
		{
			/*確認UI。*/
			m_confirmationUI.Update();

			/*確認選択UI。*/
			m_confirmationSelectUI.SetPosition(m_confirmationSelectTextUI[m_currentSelect].GetPosition());
			m_confirmationSelectUI.Update();

			for (int i = 0; i < enConfirmationSelectTextUI_Num; i++)
			{
				/*確認選択テキストUI。*/
				m_confirmationSelectTextUI[i].Update();
			}
		}
	}
}