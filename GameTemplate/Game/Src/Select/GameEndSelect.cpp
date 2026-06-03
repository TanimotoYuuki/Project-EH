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
	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 1.2f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const float SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED = 7.0f;/*選択したときの演出UIアニメーションの再生速度。*/

	const float AFTER_UI_ANIMATION_ALPHA = 0.2f;/*UIの透明度を変えるアニメーション後の透明度。*/

	const float DOWN_POSITION_OFFSET = 25.0f;/*選択したときの演出UIアニメーション時に下降する位置のオフセット。*/
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

		/*ボタンUIの描画。*/
		m_buttonUI.Draw(rc);

		/*テキストUIの描画。*/
		m_textUI.Draw(rc);
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

		/*ボタンUI。*/
		InitButtonUI();

		/*テキストUI。*/
		InitTextUI();
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

	/*ボタンUIの初期化。*/
	void GameEndSelect::InitButtonUI()
	{
		m_buttonUI.Init(m_buttonUIFilePath.c_str(), BUTTON_UI_WIDTH, BUTTON_UI_HEIGHT);/*初期化。*/
		m_buttonUI.SetPosition(BUTTON_UI_INIT_POSITION);/*位置設定。*/
		m_buttonUI.SetScale(BUTTON_UI_INIT_SCALE);/*大きさ設定。*/
		m_buttonUI.Update();/*更新処理。*/
	}

	/*テキストUIの初期化。*/
	void GameEndSelect::InitTextUI()
	{
		m_textUI.Init(m_textUIFilePath.c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);/*初期化。*/
		m_textUI.SetPosition(TEXT_UI_INIT_POSITION);/*位置設定。*/
		m_textUI.SetScale(TEXT_UI_INIT_SCALE);/*大きさ設定。*/
		m_textUI.Update();/*更新処理。*/
	}

	/*UIアニメーションの初期化。*/
	void GameEndSelect::InitUIAnimation()
	{
		/*UIの透明度を変えるアニメーション。*/
		InitAlphaUIAnimation();

		/*選択したときの演出UIアニメーション。*/
		InitSelectDirectionUIAnimation();
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

	/*選択したときの演出UIアニメーションの初期化。*/
	void GameEndSelect::InitSelectDirectionUIAnimation()
	{
		/*選択したときの演出UIアニメーションの値の設定。*/
		Vector3 basePosition = m_selectTextUI[enSelectTextUI_Retry].GetPosition();/*元の位置。*/
		Vector3 targetPosition = m_selectTextUI[enSelectTextUI_Retry].GetPosition();/*ターゲットの位置。*/
		targetPosition.y -= DOWN_POSITION_OFFSET;

		/*初期化。*/
		m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_RetryTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
			&m_selectTextUI[enSelectTextUI_Retry],/*アニメーションをさせるスプライト。*/
			1.0f,/*ターゲットの割合。*/
			SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
			false,/*ループするか？*/
			0.0f,/*アニメーションを開始する前の遅延時間。*/
			0.0f,/*アニメーションを終了した後の遅延時間。*/
			basePosition,/*元の位置。*/
			targetPosition/*ターゲットの位置。*/
		);

		/*選択したときの演出UIアニメーションの値の設定。*/
		basePosition = m_selectTextUI[enSelect_QuestSelect].GetPosition();/*元の位置。*/
		targetPosition = m_selectTextUI[enSelect_QuestSelect].GetPosition();/*ターゲットの位置。*/
		targetPosition.y -= DOWN_POSITION_OFFSET;

		/*初期化。*/
		m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_QuestSelectTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
			&m_selectTextUI[enSelect_QuestSelect],/*アニメーションをさせるスプライト。*/
			1.0f,/*ターゲットの割合。*/
			SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
			false,/*ループするか？*/
			0.0f,/*アニメーションを開始する前の遅延時間。*/
			0.0f,/*アニメーションを終了した後の遅延時間。*/
			basePosition,/*元の位置。*/
			targetPosition/*ターゲットの位置。*/
		);

		/*選択したときの演出UIアニメーションの値の設定。*/
		basePosition = m_selectUI.GetPosition();/*元の位置。*/
		targetPosition = m_selectUI.GetPosition();/*ターゲットの位置。*/
		targetPosition.y -= DOWN_POSITION_OFFSET;

		/*初期化。*/
		m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_SelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
			&m_selectUI,/*アニメーションをさせるスプライト。*/
			1.0f,/*ターゲットの割合。*/
			SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
			false,/*ループするか？*/
			0.0f,/*アニメーションを開始する前の遅延時間。*/
			0.0f,/*アニメーションを終了した後の遅延時間。*/
			basePosition,/*元の位置。*/
			targetPosition/*ターゲットの位置。*/
		);

		/*選択したときの演出UIアニメーションの値の設定。*/
		basePosition = m_selectTextUI[enSelect_Retry].GetPosition();/*元の位置。*/
		basePosition.y -= DOWN_POSITION_OFFSET;
		targetPosition = m_selectTextUI[enSelect_Retry].GetPosition();/*ターゲットの位置。*/

		/*初期化。*/
		m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_RetryTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
			&m_selectTextUI[enSelect_Retry],/*アニメーションをさせるスプライト。*/
			1.0f,/*ターゲットの割合。*/
			SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
			false,/*ループするか？*/
			0.0f,/*アニメーションを開始する前の遅延時間。*/
			0.0f,/*アニメーションを終了した後の遅延時間。*/
			basePosition,/*元の位置。*/
			targetPosition/*ターゲットの位置。*/
		);

		/*選択したときの演出UIアニメーションの値の設定。*/
		basePosition = m_selectTextUI[enSelect_QuestSelect].GetPosition();/*元の位置。*/
		basePosition.y -= DOWN_POSITION_OFFSET;
		targetPosition = m_selectTextUI[enSelect_QuestSelect].GetPosition();/*ターゲットの位置。*/

		/*初期化。*/
		m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_QuestSelectTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
			&m_selectTextUI[enSelect_QuestSelect],/*アニメーションをさせるスプライト。*/
			1.0f,/*ターゲットの割合。*/
			SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
			false,/*ループするか？*/
			0.0f,/*アニメーションを開始する前の遅延時間。*/
			0.0f,/*アニメーションを終了した後の遅延時間。*/
			basePosition,/*元の位置。*/
			targetPosition/*ターゲットの位置。*/
		);

		/*選択したときの演出UIアニメーションの値の設定。*/
		basePosition = m_selectUI.GetPosition();/*元の位置。*/
		basePosition.y -= DOWN_POSITION_OFFSET;
		targetPosition = m_selectUI.GetPosition();/*ターゲットの位置。*/

		/*初期化。*/
		m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_SelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
			&m_selectUI,/*アニメーションをさせるスプライト。*/
			1.0f,/*ターゲットの割合。*/
			SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
			false,/*ループするか？*/
			0.0f,/*アニメーションを開始する前の遅延時間。*/
			0.0f,/*アニメーションを終了した後の遅延時間。*/
			basePosition,/*元の位置。*/
			targetPosition/*ターゲットの位置。*/
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

		/*決定したら選択したら選択したときの演出を流すようにする。*/
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			EnableDirection();
		}
	}

	/*UIアニメーションの更新処理。*/
	void GameEndSelect::UpdateUIAnimation()
	{
		/*演出中なら。*/
		if (IsDirection())
		{
			/*選択したときの演出UIアニメーション(位置を下降する)が終わっていなければ再生し続ける。*/
			if (!m_selectDirectionUIAnimation[enPosition_Down][m_currentSelect]->IsEnd())
			{
				m_selectDirectionUIAnimation[enPosition_Down][m_currentSelect]->Update();
				m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_SelectUI]->Update();
			}
			/*選択したときの演出UIアニメーション(位置を上昇する)が終わっていなければ再生し続ける。*/
			else if (!m_selectDirectionUIAnimation[enPosition_Up][m_currentSelect]->IsEnd())
			{
				m_selectDirectionUIAnimation[enPosition_Up][m_currentSelect]->Update();
				m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_SelectUI]->Update();
			}
			/*選択したときの演出UIアニメーションが終わっていたらフェードアウトする。*/
			else
			{
				nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeOut);
				EnableSelect();
			}
			return;
		}

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

		/*ボタンUI。*/
		m_buttonUI.Update();

		/*テキストUI。*/
		m_textUI.Update();
	}
}