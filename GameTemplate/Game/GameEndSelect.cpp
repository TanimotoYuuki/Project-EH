#include "stdafx.h"
#include "GameEndSelect.h"
#include "Src/Fade/Fade.h"

namespace {
	/*選択UI。*/
	const float SELECT_UI_WIDTH = 1024;/*選択UIの幅。*/

	const float SELECT_UI_HEIGHT = 256;/*選択UIの高さ。*/

	const Vector3 SELECT_UI_INIT_POSITION = { 0.0f,100.0f,0.0f };/*選択UIの初期位置。*/

	const Vector3 SELECT_UI_INIT_SCALE = { 1.34f,1.53f,1.0f };/*選択UIの初期大きさ。*/

	const Vector4 SELECT_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.6f };/*選択UIの初期乗算色。*/

	/*選択テキストUI。(「リトライ」のテキストをベースに)*/
	const float SELECT_TEXT_UI_WIDTH = 1024;/*選択テキストUIの幅。*/

	const float SELECT_TEXT_UI_HEIGHT = 256;/*選択テキストUIの高さ。*/

	const Vector3 SELECT_TEXT_UI_INIT_POSITION = { 0.0f,100.0f,0.0f };/*選択テキストUIの初期位置。*/

	const float SELECT_TEXT_UI_POSITION_INTERVAL = 200.0f;/*選択テキストUIの位置の間隔。*/

	const Vector3 SELECT_TEXT_UI_INIT_SCALE = { 1.3f,1.3f,1.0f };/*選択テキストUIの初期大きさ。*/

	/*UIアニメーション。*/
	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 1.2f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const float AFTER_UI_ANIMATION_ALPHA = 0.2f;/*UIの透明度を変えるアニメーション後の透明度。*/
}

namespace nsApp
{
	/*開始処理。*/
	bool GameEndSelect::Start()
	{
		/*スプライトの初期化。*/
		InitSprite();

		/*UIアニメーションの初期化。*/
		InitUIAnimation();

		return true;
	}

	/*更新処理。*/
	void GameEndSelect::Update()
	{
		/*選択していなければ。*/
		if (!DidSelect())
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
	void GameEndSelect::Render(RenderContext& rc)
	{
		/*選択UIの描画。*/
		m_selectUI.Draw(rc);

		for (int i = 0; i < enSelectTextUI_Num; i++)
		{
			/*選択テキストUIの描画。*/
			m_selectTextUI[i].Draw(rc);
		}
	}

	/*スプライトの初期化。*/
	void GameEndSelect::InitSprite()
	{
		/*選択UI。*/
		InitSelectUI();

		/*選択テキストUI。*/
		for (int i = 0; i < enSelectTextUI_Num; i++)
		{
			/*選択テキストUI。*/
			InitSelectTextUI((EnSelectTextUI)i, i);
		}
	}

	/*選択UIの初期化。*/
	void GameEndSelect::InitSelectUI()
	{
		m_selectUI.Init(m_selectUIFilePath.c_str(), SELECT_UI_WIDTH, SELECT_UI_HEIGHT);/*初期化。*/
		m_selectUI.SetPosition(SELECT_UI_INIT_POSITION);/*位置設定。*/
		m_selectUI.SetScale(SELECT_UI_INIT_SCALE);/*大きさ設定。*/
		m_selectUI.SetMulColor(SELECT_UI_INIT_MUL_COLOR);/*乗算色設定。*/
		m_selectUI.Update();/*更新処理。*/
	}

	/*選択テキストUIの初期化。*/
	void GameEndSelect::InitSelectTextUI(EnSelectTextUI selectTextUI, int selectTextIndex)
	{
		Vector3 initPosition = SELECT_TEXT_UI_INIT_POSITION;/*初期位置。*/
		initPosition.y -= SELECT_TEXT_UI_POSITION_INTERVAL * selectTextIndex;/*位置の間隔を加算。*/

		m_selectTextUI[selectTextUI].Init(m_selectTextUIFilePath[selectTextUI].c_str(), SELECT_TEXT_UI_WIDTH, SELECT_TEXT_UI_HEIGHT);/*初期化。*/
		m_selectTextUI[selectTextUI].SetPosition(initPosition);/*位置設定。*/
		m_selectTextUI[selectTextUI].SetScale(SELECT_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
		m_selectTextUI[selectTextUI].Update();/*更新処理。*/
	}

	/*UIアニメーションの初期化。*/
	void GameEndSelect::InitUIAnimation()
	{
		/*UIの透明度を変えるアニメーション。*/
		InitAlphaUIAnimation();
	}

	/*UIの透明度を変えるアニメーションの初期化。*/
	void GameEndSelect::InitAlphaUIAnimation()
	{
		/*UIの透明度を変えるアニメーションの値の設定。*/
		float baseAlpha = m_selectUI.GetMulColor().a;/*元の透明度。*/
		float targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

		/*初期化。*/
		m_alphaUIAnimation = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
			&m_selectUI,/*アニメーションをさせるスプライト。*/
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
	void GameEndSelect::UpdateSelect()
	{
		/*上を入力したら。*/
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			ResetAlphaUIAnimation();
			/*現在の選択が1人目のキャラクターを選択していたら。*/
			if (m_currentSelect == enSelect_Retry) { m_currentSelect = enSelect_QuestSelect; return; }

			/*上隣りのキャラクターを選択する。*/
			m_currentSelect--;
			return;
		}

		/*下を入力したら。*/
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			ResetAlphaUIAnimation();
			/*現在の選択が4人目のキャラクターを選択していたら。*/
			if (m_currentSelect == enSelect_QuestSelect) { m_currentSelect = enSelect_Retry; return; }

			/*下隣りのキャラクターを選択する。*/
			m_currentSelect++;
			return;
		}

		/*決定したら選択した内容を元に遷移する。*/
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeOut);
			EnableSelect();
		}
	}

	/*UIアニメーションの更新処理。*/
	void GameEndSelect::UpdateUIAnimation()
	{
		/*UIの透明度を変えるアニメーション。*/
		m_alphaUIAnimation->Update();
	}

	/*スプライトの更新処理。*/
	void GameEndSelect::UpdateSprite()
	{
		/*選択UI。*/
		m_selectUI.SetPosition(m_selectTextUI[m_currentSelect].GetPosition());
		m_selectUI.Update();

		for (int i = 0; i < enSelectTextUI_Num; i++)
		{
			/*選択テキストUI。*/
			m_selectTextUI[i].Update();
		}
	}
}