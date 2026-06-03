#include "stdafx.h"
#include "HowToPlay.h"

namespace {
	/*背景。*/
	const float BACK_GROUND_WIDTH = 1920;/*背景の幅。*/

	const float BACK_GROUND_HEIGHT = 1080;/*背景の高さ。*/

	/*操作方法テキストUI。*/
	const float HOW_TO_PLAY_TEXT_UI_WIDTH = 1024;/*操作方法テキストUIの幅。*/

	const float HOW_TO_PLAY_TEXT_UI_HEIGHT = 256;/*操作方法テキストUIの高さ。*/

	const Vector3 HOW_TO_PLAY_TEXT_UI_INIT_POSITION = { -700.0f,450.0f,0.0f };/*操作方法テキストUIの初期位置。*/

	const Vector3 HOW_TO_PLAY_TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*操作方法テキストUIの初期大きさ。*/

	/*操作方法UI。*/
	const float HOW_TO_PLAY_UI_WIDTH = 1578;/*操作方法UIの幅。*/

	const float HOW_TO_PLAY_UI_HEIGHT = 816;/*操作方法UIの高さ。*/

	const Vector3 HOW_TO_PLAY_UI_INIT_POSITION = { 0.0f,-50.0f,0.0f };/*操作方法UIの初期位置。*/

	const Vector3 HOW_TO_PLAY_UI_INIT_SCALE = Vector3(1.0f, 1.0f, 1.0f);/*操作方法UIの初期大きさ。*/

	/*矢印UI。*/
	const float ARROW_UI_WIDTH = 1024;/*矢印UIの幅。*/

	const float ARROW_UI_HEIGHT = 1024;/*矢印UIの高さ。*/

	const Vector3 ARROW_UI_INIT_POSITION[nsApp::nsHowToPlay::HowToPlay::EnArrowUI::enArrowUI_Num] = {
		Vector3{-790.0f,-75.0f,0.0f},/*左。*/
		Vector3{790.0f,-75.0f,0.0f},/*右。*/
	};/*役割UIの初期位置。*/

	const Vector3 ARROW_UI_INIT_SCALE = { 0.25f,0.25f,1.0f };/*矢印UIの初期大きさ。*/

	/*ボタンUI。*/
	const float BUTTON_UI_WIDTH = 1024;/*ボタンUIの幅。*/

	const float BUTTON_UI_HEIGHT = 1024;/*ボタンUIの高さ。*/

	const Vector3 BUTTON_UI_INIT_POSITION = { -850.0f,-450.0f,0.0f };/*ボタンUIの初期位置。*/

	const Vector3 BUTTON_UI_INIT_SCALE = { 0.1f,0.1f,1.0f };/*ボタンUIの初期大きさ。*/

	/*テキストUI。*/
	const float TEXT_UI_WIDTH = 1024;/*テキストUIの幅。*/

	const float TEXT_UI_HEIGHT = 256;/*テキストUIの高さ。*/

	const Vector3 TEXT_UI_INIT_POSITION = { -725.0f,-450.0f,0.0f };/*テキストUIの初期位置。*/

	const Vector3 TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*テキストUIの初期大きさ。*/

	/*UIアニメーション。*/
	const Vector3 AFTER_SLIDE_LEFT_HOW_TO_PLAY_UI_ANIMATION_POSITION = { -1750.0,-50.0f,0.0f };/*操作方法UIを左にスライドさせるアニメーション後の位置。*/

	const Vector3 AFTER_SLIDE_RIGHT_HOW_TO_PLAY_UI_ANIMATION_POSITION = { 1750.0,-50.0f,0.0f };/*操作方法UIを右にスライドさせるアニメーション後の位置。*/

	const Vector2 AFTER_SCALE_UP_ARROW_UI_ANIMATION_SCALE = { 0.3f,0.3f };/*矢印UIの大きさを大きくするアニメーション後の大きさ。*/

	const Vector3 AFTER_COLOR_ARROW_UI_ANIMATION_COLOR = { 2.0f,2.0f,2.0f };/*矢印UIの色を変えるアニメーション後の色。*/

	const float SLIDE_UI_ANIMATION_PLAY_SPEED = 4.0f;/*UIをスライドさせるアニメーションの再生速度。*/

	const float SCALE_UI_ANIMATION_PLAY_SPEED = 4.0f;/*UIの大きさを変えるアニメーションの再生速度。*/

	const float COLOR_UI_ANIMATION_PLAY_SPEED = 4.0f;/*UIの色を変えるアニメーションの再生速度。*/
}

namespace nsApp
{
	namespace nsHowToPlay
	{
		/*開始処理。*/
		bool HowToPlay::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			return true;
		}

		/*更新処理。*/
		void HowToPlay::Update()
		{
			/*演出していないとき選択する。*/
			if (!IsPreviousPageTransitionDirection() && !IsNextPageTransitionDirection())
			{
				/*選択。*/
				UpdateSelect();
			}

			/*UIアニメーション。*/
			UpdateUIAnimation();

			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void HowToPlay::Render(RenderContext& rc)
		{
			/*背景の描画。*/
			m_backGround.Draw(rc);

			/*操作方法テキストUIの描画。*/
			m_howToPlayTextUI.Draw(rc);

			/*操作方法UIの描画。*/
			m_howtToPlayUI[m_displayHowToPlayUI].Draw(rc);

			for (int i = 0; i < enArrowUI_Num; i++)
			{
				/*矢印UIの描画。*/
				m_arrowUI[i].Draw(rc);
			}

			/*ボタンUIの描画。*/
			m_buttonUI.Draw(rc);

			/*テキストUIの描画。*/
			m_textUI.Draw(rc);
		}

		/*スプライトの初期化。*/
		void HowToPlay::InitSprite()
		{
			/*背景。*/
			InitBackGround();

			/*操作方法テキストUI。*/
			InitHowToPlayTextUI();

			for (int i = 0; i < enHowToPlayUI_Num; i++)
			{
				/*操作方法UI。*/
				InitHowToPlayUI((EnHowToPlayUI)i);
			}

			for (int j = 0; j < enArrowUI_Num; j++)
			{
				/*矢印UI。*/
				InitArrowUI((EnArrowUI)j);
			}

			/*ボタンUI。*/
			InitButtonUI();

			/*テキストUI。*/
			InitTextUI();
		}

		/*背景の初期化。*/
		void HowToPlay::InitBackGround()
		{
			m_backGround.Init(m_backGroundFilePath.c_str(), BACK_GROUND_WIDTH, BACK_GROUND_HEIGHT);
		}

		/*操作方法テキストUIの初期化。*/
		void HowToPlay::InitHowToPlayTextUI()
		{
			m_howToPlayTextUI.Init(m_howToPlayTextUIFilePath.c_str(), HOW_TO_PLAY_TEXT_UI_WIDTH, HOW_TO_PLAY_TEXT_UI_HEIGHT);/*初期化。*/
			m_howToPlayTextUI.SetPosition(HOW_TO_PLAY_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_howToPlayTextUI.SetScale(HOW_TO_PLAY_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_howToPlayTextUI.Update();/*更新処理。*/
		}

		/*操作方法UIの初期化。*/
		void HowToPlay::InitHowToPlayUI(EnHowToPlayUI howToPlayUI)
		{
			m_howtToPlayUI[howToPlayUI].Init(m_howToPlayUIFliePath[howToPlayUI].c_str(), HOW_TO_PLAY_UI_WIDTH, HOW_TO_PLAY_UI_HEIGHT);/*初期化。*/
			m_howtToPlayUI[howToPlayUI].SetPosition(HOW_TO_PLAY_UI_INIT_POSITION);/*位置設定。*/
			m_howtToPlayUI[howToPlayUI].SetScale(HOW_TO_PLAY_UI_INIT_SCALE);/*大きさ設定。*/
			m_howtToPlayUI[howToPlayUI].Update();/*更新処理。*/

			m_slideHowToPlayUIAnimationSprite.push_back(&m_howtToPlayUI[howToPlayUI]);/*操作方法UIをスライドUIアニメーションのスプライトに追加。*/
		}

		/*矢印UIの初期化。*/
		void HowToPlay::InitArrowUI(EnArrowUI arrowUI)
		{
			m_arrowUI[arrowUI].Init(m_arrowUIFliePath[arrowUI].c_str(), ARROW_UI_WIDTH, ARROW_UI_HEIGHT);/*初期化。*/
			m_arrowUI[arrowUI].SetPosition(ARROW_UI_INIT_POSITION[arrowUI]);/*位置設定。*/
			m_arrowUI[arrowUI].SetScale(ARROW_UI_INIT_SCALE);/*大きさ設定。*/
			m_arrowUI[arrowUI].Update();/*更新処理。*/

			m_scaleArrowUIAnimationSprite.push_back(&m_arrowUI[arrowUI]);/*矢印UIを大きさを変えるUIアニメーションのスプライトに追加。。*/
			m_colorArrowUIAnimationSprite.push_back(&m_arrowUI[arrowUI]);/*矢印UIを色を変えるUIアニメーションのスプライトに追加。。*/
		}

		/*ボタンUIの初期化・*/
		void HowToPlay::InitButtonUI()
		{
			m_buttonUI.Init(m_buttonUIFliePath.c_str(), BUTTON_UI_WIDTH, BUTTON_UI_HEIGHT);/*初期化。*/
			m_buttonUI.SetPosition(BUTTON_UI_INIT_POSITION);/*位置設定。*/
			m_buttonUI.SetScale(BUTTON_UI_INIT_SCALE);/*大きさ設定。*/
			m_buttonUI.Update();/*更新処理。*/
		}

		/*テキストUIの初期化。*/
		void HowToPlay::InitTextUI()
		{
			m_textUI.Init(m_textUIFilePath.c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);/*初期化。*/
			m_textUI.SetPosition(TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_textUI.SetScale(TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textUI.Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void HowToPlay::InitUIAnimation()
		{
			for (int i = 0; i < m_slideHowToPlayUIAnimationSprite.size(); i++)
			{
				/*操作方法UIを左にスライドさせるアニメーション(開始)。*/
				InitSlideLeftStartHowToPlayUIAnimation(m_slideHowToPlayUIAnimationSprite[i]);

				/*操作方法UIを左にスライドさせるアニメーション(終了)。*/
				InitSlideLeftEndHowToPlayUIAnimation(m_slideHowToPlayUIAnimationSprite[i]);

				/*操作方法UIを右にスライドさせるアニメーション(開始)。*/
				InitSlideRightStartHowToPlayUIAnimation(m_slideHowToPlayUIAnimationSprite[i]);

				/*操作方法UIを右にスライドさせるアニメーション(終了)。*/
				InitSlideRightEndHowToPlayUIAnimation(m_slideHowToPlayUIAnimationSprite[i]);
			}

			for (int j = 0; j < m_scaleArrowUIAnimationSprite.size(); j++)
			{
				/*矢印UIの大きさを大きくするアニメーション。*/
				InitScaleUpArrowUIAnimation(m_scaleArrowUIAnimationSprite[j]);

				/*矢印UIの大きさを小さくするアニメーション。*/
				InitScaleDownArrowUIAnimation(m_scaleArrowUIAnimationSprite[j]);
			}

			for (int k = 0; k < m_colorArrowUIAnimationSprite.size(); k++)
			{
				/*矢印UIの色を変更前の色にするアニメーション。*/
				InitColorBeforeArrowUIAnimation(m_colorArrowUIAnimationSprite[k]);

				/*矢印UIの色を変更後の色にするアニメーション。*/
				InitColorAfterArrowUIAnimation(m_colorArrowUIAnimationSprite[k]);
			}
		}

		/*操作方法UIを左にスライドさせるアニメーション(開始)の初期化。*/
		void HowToPlay::InitSlideLeftStartHowToPlayUIAnimation(SpriteRender* spriteData)
		{
			/*操作方法UIを左にスライドさせるアニメーション(開始)の値の設定。*/
			Vector3 basePosition = AFTER_SLIDE_RIGHT_HOW_TO_PLAY_UI_ANIMATION_POSITION;/*元の位置。*/
			Vector3 targetPosition = spriteData->GetPosition();/*ターゲットの位置。*/

			m_slideLeftStartHowToPlayUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/)
			);
		}

		/*操作方法UIを左にスライドさせるアニメーション(終了)の初期化。*/
		void HowToPlay::InitSlideLeftEndHowToPlayUIAnimation(SpriteRender* spriteData)
		{
			/*操作方法UIを左にスライドさせるアニメーション(終了)の値の設定。*/
			Vector3 basePosition = spriteData->GetPosition();/*元の位置。*/
			Vector3 targetPosition = AFTER_SLIDE_LEFT_HOW_TO_PLAY_UI_ANIMATION_POSITION;/*ターゲットの位置。*/

			m_slideLeftEndHowToPlayUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/)
			);
		}

		/*操作方法UIを右にスライドさせるアニメーション(開始)の初期化。*/
		void HowToPlay::InitSlideRightStartHowToPlayUIAnimation(SpriteRender* spriteData)
		{
			/*操作方法UIを右にスライドさせるアニメーション(開始)の値の設定。*/
			Vector3 basePosition = AFTER_SLIDE_LEFT_HOW_TO_PLAY_UI_ANIMATION_POSITION;/*元の位置。*/
			Vector3 targetPosition = spriteData->GetPosition();/*ターゲットの位置。*/

			m_slideRightStartHowToPlayUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/)
			);
		}

		/*操作方法UIを右にスライドさせるアニメーション(終了)の初期化。*/
		void HowToPlay::InitSlideRightEndHowToPlayUIAnimation(SpriteRender* spriteData)
		{
			/*操作方法UIを右にスライドさせるアニメーション(終了)の値の設定。*/
			Vector3 basePosition = spriteData->GetPosition();/*元の位置。*/
			Vector3 targetPosition = AFTER_SLIDE_RIGHT_HOW_TO_PLAY_UI_ANIMATION_POSITION;/*ターゲットの位置。*/

			m_slideRightEndHowToPlayUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/)
			);
		}

		/*矢印UIの大きさを大きくするアニメーションの初期化。*/
		void HowToPlay::InitScaleDownArrowUIAnimation(SpriteRender* spriteData)
		{
			/*矢印UIの大きさを大きくするアニメーションの値の設定。*/
			Vector2 baseScale = { spriteData->GetScale().x,spriteData->GetScale().y };/*元の大きさ。*/
			Vector2 targetScale = AFTER_SCALE_UP_ARROW_UI_ANIMATION_SCALE;/*ターゲットの大きさ。*/

			m_scaleUpArrowUIAnimation.push_back(std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/)
			);
		}

		/*矢印UIの大きさを小さくするアニメーションの初期化。*/
		void HowToPlay::InitScaleUpArrowUIAnimation(SpriteRender* spriteData)
		{
			/*矢印UIの大きさを小さくするアニメーションの値の設定。*/
			Vector2 baseScale = AFTER_SCALE_UP_ARROW_UI_ANIMATION_SCALE;/*元の大きさ。*/
			Vector2 targetScale = { spriteData->GetScale().x,spriteData->GetScale().y };/*ターゲットの大きさ。*/

			m_scaleDownArrowUIAnimation.push_back(std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/)
			);
		}

		/*矢印UIの色を変更前の色にするアニメーションの初期化。*/
		void HowToPlay::InitColorBeforeArrowUIAnimation(SpriteRender* spriteData)
		{
			/*矢印UIの色を変更前の色にするアニメーションの値の設定。*/
			Vector3 baseColor = {
				spriteData->GetMulColor().x,
				spriteData->GetMulColor().y,
				spriteData->GetMulColor().z
			};/*元の色。*/
			Vector3 targetColor = AFTER_COLOR_ARROW_UI_ANIMATION_COLOR;/*ターゲットの色。*/

			m_colorBeforeArrowUIAnimation.push_back(std::make_unique<nsApp::nsUI::ColorUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				COLOR_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseColor,/*元の色。*/
				targetColor/*ターゲットの色。*/)
			);
		}

		/*矢印UIの色を変更後の色にするアニメーションの初期化。*/
		void HowToPlay::InitColorAfterArrowUIAnimation(SpriteRender* spriteData)
		{
			/*矢印UIの色を変更後の色にするアニメーションの値の設定。*/
			Vector3 baseColor = AFTER_COLOR_ARROW_UI_ANIMATION_COLOR;/*元の色。*/
			Vector3 targetColor = {
				spriteData->GetMulColor().x,
				spriteData->GetMulColor().y,
				spriteData->GetMulColor().z
			};/*ターゲットの色。*/

			m_colorAfterArrowUIAnimation.push_back(std::make_unique<nsApp::nsUI::ColorUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				COLOR_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseColor,/*元の色。*/
				targetColor/*ターゲットの色。*/)
			);
		}

		/*操作方法UIを左にスライドさせるアニメーション(開始)のリセット処理。*/
		void HowToPlay::ResetSlideLeftStartHowToPlayUIAnimation()
		{
			for (int i = 0; i < m_slideHowToPlayUIAnimationSprite.size(); i++)
			{
				m_slideLeftStartHowToPlayUIAnimation[i]->Reset();
			}
		}

		/*操作方法UIを左にスライドさせるアニメーション(終了)のリセット処理。*/
		void HowToPlay::ResetSlideLeftEndHowToPlayUIAnimation()
		{
			for (int i = 0; i < m_slideHowToPlayUIAnimationSprite.size(); i++)
			{
				m_slideLeftEndHowToPlayUIAnimation[i]->Reset();
			}
		}

		/*操作方法UIを右にスライドさせるアニメーション(開始)のリセット処理。*/
		void HowToPlay::ResetSlideRightStartHowToPlayUIAnimation()
		{
			for (int i = 0; i < m_slideHowToPlayUIAnimationSprite.size(); i++)
			{
				m_slideRightStartHowToPlayUIAnimation[i]->Reset();
			}
		}

		/*操作方法UIを右にスライドさせるアニメーション(終了)のリセット処理。*/
		void HowToPlay::ResetSlideRightEndHowToPlayUIAnimation()
		{
			for (int i = 0; i < m_slideHowToPlayUIAnimationSprite.size(); i++)
			{
				m_slideRightEndHowToPlayUIAnimation[i]->Reset();
			}
		}

		/*矢印UIの大きさを小さくするアニメーションのリセット処理。*/
		void HowToPlay::ResetScaleUpArrowUIAnimation()
		{
			for (int i = 0; i < m_scaleArrowUIAnimationSprite.size(); i++)
			{
				m_scaleUpArrowUIAnimation[i]->Reset();
			}
		}

		/*矢印UIの大きさを大きくするアニメーションのリセット処理。*/
		void HowToPlay::ResetScaleDownArrowUIAnimation()
		{
			for (int i = 0; i < m_scaleArrowUIAnimationSprite.size(); i++)
			{
				m_scaleDownArrowUIAnimation[i]->Reset();
			}
		}

		/*矢印UIの色を変更前の色にするアニメーションをリセット処理。*/
		void HowToPlay::ResetColorBeforeArrowUIAnimation()
		{
			for (int i = 0; i < m_colorArrowUIAnimationSprite.size(); i++)
			{
				m_colorBeforeArrowUIAnimation[i]->Reset();
			}
		}

		/*矢印UIの色を変更後の色にするアニメーションをリセット処理。*/
		void HowToPlay::ResetColorAfterArrowUIAnimation()
		{
			for (int i = 0; i < m_colorArrowUIAnimationSprite.size(); i++)
			{
				m_colorAfterArrowUIAnimation[i]->Reset();
			}
		}

		/*選択の更新処理。*/
		void HowToPlay::UpdateSelect()
		{
			/*左を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonLeft))
			{
				EnablePreviousPageTransitionDirection();

				m_previousPage = m_currentPage;
				/*現在の選択が共通ページを選択していたら。*/
				if (m_currentPage == enHowToPlayUI_GeneralPage) { m_currentPage = enHowToPlayUI_Gunner_OnePage; return; }

				/*左隣りのページを選択する。*/
				m_currentPage--;
				return;
			}

			/*右を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonRight))
			{
				EnableNextPageTransitionDirection();

				m_previousPage = m_currentPage;
				/*現在の選択がガンナー(1ページ)を選択していたら。*/
				if (m_currentPage == enHowToPlayUI_Gunner_OnePage) { m_currentPage = enHowToPlayUI_GeneralPage; return; }

				/*右隣りのページを選択する。*/
				m_currentPage++;
				return;
			}

			/*Bボタンを押したら前の画面に戻る。*/
			if (g_pad[0]->IsTrigger(enButtonB))
			{
				EnableBackSelect();
			}
		}

		/*UIアニメーションの更新処理。*/
		void HowToPlay::UpdateUIAnimation()
		{
			if (IsNextPageTransitionDirection())
			{
				if (!m_slideLeftEndHowToPlayUIAnimation[m_previousPage]->IsEnd())
				{
					m_slideLeftEndHowToPlayUIAnimation[m_previousPage]->Update();
					m_scaleUpArrowUIAnimation[enArrowUI_Right]->Update();
					m_colorBeforeArrowUIAnimation[enArrowUI_Right]->Update();
				}
				else if (!m_slideLeftStartHowToPlayUIAnimation[m_currentPage]->IsEnd())
				{
					m_displayHowToPlayUI = m_currentPage;/*操作方法UIを左にスライドさせるアニメーションが終わったタイミングで操作方法UIを切り替える。*/
					m_slideLeftStartHowToPlayUIAnimation[m_currentPage]->Update();
					m_scaleDownArrowUIAnimation[enArrowUI_Right]->Update();
					m_colorAfterArrowUIAnimation[enArrowUI_Right]->Update();
				}
				else
				{
					DisableNextPageTransitionDirection();
				}
			}
			else if (IsPreviousPageTransitionDirection())
			{
				if (!m_slideRightEndHowToPlayUIAnimation[m_previousPage]->IsEnd())
				{
					m_slideRightEndHowToPlayUIAnimation[m_previousPage]->Update();
					m_scaleUpArrowUIAnimation[enArrowUI_Left]->Update();
					m_colorBeforeArrowUIAnimation[enArrowUI_Left]->Update();
				}
				else if (!m_slideRightStartHowToPlayUIAnimation[m_currentPage]->IsEnd())
				{
					m_displayHowToPlayUI = m_currentPage;/*操作方法UIを右にスライドさせるアニメーションが終わったタイミングで操作方法UIを切り替える。*/
					m_slideRightStartHowToPlayUIAnimation[m_currentPage]->Update();
					m_scaleDownArrowUIAnimation[enArrowUI_Left]->Update();
					m_colorAfterArrowUIAnimation[enArrowUI_Left]->Update();
				}
				else
				{
					DisablePreviousPageTransitionDirection();
				}
			}
			else
			{
				/*操作方法UIをスライドさせるアニメーション(開始)のリセット処理。*/
				ResetSlideLeftStartHowToPlayUIAnimation();
				ResetSlideRightStartHowToPlayUIAnimation();

				/*操作方法UIをスライドさせるアニメーション(終了)のリセット処理。*/
				ResetSlideLeftEndHowToPlayUIAnimation();
				ResetSlideRightEndHowToPlayUIAnimation();

				/*矢印UIの大きさを変えるアニメーションのリセット処理。*/
				ResetScaleUpArrowUIAnimation();
				ResetScaleDownArrowUIAnimation();

				/*矢印Uiの色を変えるアニメーションのリセット処理。*/
				ResetColorBeforeArrowUIAnimation();
				ResetColorBeforeArrowUIAnimation();
			}
		}

		/*スプライトの更新処理。*/
		void HowToPlay::UpdateSprite()
		{
			/*背景。*/
			m_backGround.Update();

			/*操作方法テキストUI。*/
			m_howToPlayTextUI.Update();

			for (int i = 0; i < enHowToPlayUI_Num; i++)
			{
				/*操作方法UI。*/
				m_howtToPlayUI[i].Update();
			}

			for (int j = 0; j < enArrowUI_Num; j++)
			{
				/*矢印UI。*/
				m_arrowUI[j].Update();
			}

			/*ボタンUI。*/
			m_buttonUI.Update();

			/*テキストUI。*/
			m_textUI.Update();
		}
	}
}