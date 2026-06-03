#include "stdafx.h"
#include "Pause.h"
#include "Src/Select/ConfirmationSelect.h"
#include "Src/Fade/Fade.h"

namespace {
	/*背景。*/
	const float BACK_GROUND_WIDTH = 1920;/*背景の幅。*/

	const float BACK_GROUND_HEIGHT = 1080;/*背景の高さ。*/

	/*ポーズテキストUI。*/
	const float PAUSE_TEXT_UI_WIDTH = 1024;/*ポーズテキストUIの幅。*/

	const float PAUSE_TEXT_UI_HEIGHT = 256;/*ポーズテキストUIの高さ。*/

	const Vector3 PAUSE_TEXT_UI_INIT_POSITION = { -700.0f,450.0f,0.0f };/*ポーズテキストUIの初期位置。*/

	const Vector3 PAUSE_TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*ポーズテキストUIの初期大きさ。*/

	/*白枠UI。*/
	const float WHITE_FRAME_UI_WIDTH = 1578;/*ゲージUIの幅。*/

	const float WHITE_FRAME_UI_HEIGHT = 816;/*ゲージUIの高さ。*/

	const Vector3 WHITE_FRAME_UI_INIT_POSITION = { 0.0f,-50.0f,0.0f };/*ゲージUIの初期位置。*/

	const Vector3 WHITE_FRAME_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*ゲージUIの初期大きさ。*/

	/*テキスト選択UI。*/
	const float TEXT_SELECT_UI_WIDTH = 1024;/*テキスト選択UIの幅。*/

	const float TEXT_SELECT_UI_HEIGHT = 256;/*テキスト選択UIの高さ。*/

	const Vector3 TEXT_SELECT_UI_INIT_POSITION = { 0.0f,165.0f,0.0f };/*テキスト選択UIの初期位置。*/

	const Vector3 TEXT_SELECT_UI_INIT_SCALE = Vector3(1.34f, 1.53f, 1.0f);/*テキスト選択UIの初期大きさ。*/

	/*選択肢テキストUI(「ゲームを続ける」のテキストをベースに)。*/
	const float OPTION_TEXT_UI_WIDTH = 1024;/*選択肢テキストUIの幅。*/

	const float OPTION_TEXT_UI_HEIGHT = 256;/*選択肢テキストUIの高さ。*/

	const Vector3 OPTION_TEXT_UI_INIT_POSITION = { 0.0f,255.0f,0.0f };/*選択肢テキストUIの初期位置。*/

	const float OPTION_TEXT_UI_POSITION_INTERVAL = 200.0f;/*選択肢テキストUIの位置の間隔。*/

	const Vector3 OPTION_TEXT_UI_INIT_SCALE = { 1.3f,1.3f,1.0f };/*テキストUIの初期大きさ。*/

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
	namespace nsGame
	{
		Pause::~Pause()
		{
			DeleteGO(m_confirmationSelect);
		}

		/*開始処理。*/
		bool Pause::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			m_confirmationSelect = NewGO<nsApp::nsSelect::ConfirmationSelect>(2, "confirmationSelect");

			return true;
		}

		/*更新処理。*/
		void Pause::Update()
		{
			/*選択できていなければ。*/
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
		void Pause::Render(RenderContext& rc)
		{
			/*UIを描画しない状態なら処理しない。*/
			if (!IsDrawingUI())
			{
				return;
			}

			/*背景。*/
			m_backGround.Draw(rc);

			/*ポーズテキストUI。*/
			m_pauseTextUI.Draw(rc);

			/*白枠UI。*/
			m_whiteFrameUI.Draw(rc);

			/*テキスト選択UI。*/
			m_textSelectUI.Draw(rc);

			for (int i = 0; i < enOptionTextUI_Num; i++)
			{
				/*選択肢テキストUI。*/
				m_optionTextUI[i].Draw(rc);
			}

			/*ボタンUIの描画。*/
			m_buttonUI.Draw(rc);

			/*テキストUIの描画。*/
			m_textUI.Draw(rc);
		}

		/*スプライトの初期化。*/
		void Pause::InitSprite()
		{
			/*背景。*/
			InitBackGround();

			/*ポーズテキストUI。*/
			InitPauseTextUI();

			/*白枠UI。*/
			InitWhiteFrameUI();

			/*テキスト選択UI。*/
			InitTextSelectUI();

			for(int i = 0; i < enOptionTextUI_Num; i++)
			{
				/*選択肢テキストUI。*/
				InitOptionTextUI((EnOptionTextUI)i, i);
			}

			/*ボタンUI。*/
			InitButtonUI();

			/*テキストUI。*/
			InitTextUI();
		}

		/*背景の初期化。*/
		void Pause::InitBackGround()
		{
			m_backGround.Init(m_backGroundFilePath.c_str(), BACK_GROUND_WIDTH, BACK_GROUND_HEIGHT);
		}

		/*ポーズテキストUIの初期化。*/
		void Pause::InitPauseTextUI()
		{
			m_pauseTextUI.Init(m_pauseTextUIFilePath.c_str(), PAUSE_TEXT_UI_WIDTH, PAUSE_TEXT_UI_HEIGHT);/*初期化。*/
			m_pauseTextUI.SetPosition(PAUSE_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_pauseTextUI.SetScale(PAUSE_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_pauseTextUI.Update();/*更新処理。*/
		}

		/*白枠UIの初期化。*/
		void Pause::InitWhiteFrameUI()
		{
			m_whiteFrameUI.Init(m_whiteFrameUIFilePath.c_str(), WHITE_FRAME_UI_WIDTH, WHITE_FRAME_UI_HEIGHT);/*初期化。*/
			m_whiteFrameUI.SetPosition(WHITE_FRAME_UI_INIT_POSITION);/*位置設定。*/
			m_whiteFrameUI.SetScale(WHITE_FRAME_UI_INIT_SCALE);/*大きさ設定。*/
			m_whiteFrameUI.Update();/*更新処理。*/
		}

		/*テキスト選択UIの初期化。*/
		void Pause::InitTextSelectUI()
		{
			m_textSelectUI.Init(m_textSelectUIFilePath.c_str(), TEXT_SELECT_UI_WIDTH, TEXT_SELECT_UI_HEIGHT);/*初期化。*/
			m_textSelectUI.SetPosition(TEXT_SELECT_UI_INIT_POSITION);/*位置設定。*/
			m_textSelectUI.SetScale(TEXT_SELECT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textSelectUI.Update();/*更新処理。*/
		}

		/*選択肢テキストUIの初期化。*/
		void Pause::InitOptionTextUI(EnOptionTextUI optionTextUI, int optionTextIndex)
		{
			Vector3 initPosition = OPTION_TEXT_UI_INIT_POSITION;/*初期位置。*/
			initPosition.y -= OPTION_TEXT_UI_POSITION_INTERVAL * optionTextIndex;/*位置の間隔を減算。*/

			m_optionTextUI[optionTextUI].Init(m_optionTextUIFilePath[optionTextUI].c_str(), OPTION_TEXT_UI_WIDTH, OPTION_TEXT_UI_HEIGHT);/*初期化。*/
			m_optionTextUI[optionTextUI].SetPosition(initPosition);/*位置設定。*/
			m_optionTextUI[optionTextUI].SetScale(OPTION_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_optionTextUI[optionTextUI].Update();/*更新処理。*/
		}

		/*ボタンUIの初期化。*/
		void Pause::InitButtonUI()
		{
			m_buttonUI.Init(m_buttonUIFilePath.c_str(), BUTTON_UI_WIDTH, BUTTON_UI_HEIGHT);/*初期化。*/
			m_buttonUI.SetPosition(BUTTON_UI_INIT_POSITION);/*位置設定。*/
			m_buttonUI.SetScale(BUTTON_UI_INIT_SCALE);/*大きさ設定。*/
			m_buttonUI.Update();/*更新処理。*/
		}

		/*テキストUIの初期化。*/
		void Pause::InitTextUI()
		{
			m_textUI.Init(m_textUIFilePath.c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);/*初期化。*/
			m_textUI.SetPosition(TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_textUI.SetScale(TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textUI.Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void Pause::InitUIAnimation()
		{
			/*UIの透明度を変えるアニメーション。*/
			InitAlphaUIAnimation();

			/*選択したときの演出UIアニメーション。*/
			InitSelectDirectionUIAnimation();
		}

		/*UIの透明度を変えるアニメーションの初期化。*/
		void Pause::InitAlphaUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = m_textSelectUI.GetMulColor().a;/*元の透明度。*/
			float targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_textSelectUI,/*アニメーションをさせるスプライト。*/
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
		void Pause::InitSelectDirectionUIAnimation()
		{
			/*選択したときの演出UIアニメーションの値の設定。*/
			Vector3 basePosition = m_optionTextUI[enOptionTextUI_ReturnToGame].GetPosition();/*元の位置。*/
			Vector3 targetPosition = m_optionTextUI[enOptionTextUI_ReturnToGame].GetPosition();/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_ReturnToGameTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_ReturnToGame],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_optionTextUI[enOptionTextUI_HowToPlay].GetPosition();/*元の位置。*/
			targetPosition = m_optionTextUI[enOptionTextUI_HowToPlay].GetPosition();/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_HowToPlayTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_HowToPlay],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_optionTextUI[enOptionTextUI_RestartTheGame].GetPosition();/*元の位置。*/
			targetPosition = m_optionTextUI[enOptionTextUI_RestartTheGame].GetPosition();/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_RestartTheGameTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_RestartTheGame],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_optionTextUI[enOptionTextUI_ReturnToQuestSelect].GetPosition();/*元の位置。*/
			targetPosition = m_optionTextUI[enOptionTextUI_ReturnToQuestSelect].GetPosition();/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_ReturnToQuestSelectTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_ReturnToQuestSelect],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_textSelectUI.GetPosition();/*元の位置。*/
			targetPosition = m_textSelectUI.GetPosition();/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_SelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_textSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_optionTextUI[enOptionTextUI_ReturnToGame].GetPosition();/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = m_optionTextUI[enOptionTextUI_ReturnToGame].GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_ReturnToGameTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_ReturnToGame],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_optionTextUI[enOptionTextUI_HowToPlay].GetPosition();/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = m_optionTextUI[enOptionTextUI_HowToPlay].GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_HowToPlayTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_HowToPlay],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_optionTextUI[enOptionTextUI_RestartTheGame].GetPosition();/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = m_optionTextUI[enOptionTextUI_RestartTheGame].GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_RestartTheGameTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_RestartTheGame],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_optionTextUI[enOptionTextUI_ReturnToQuestSelect].GetPosition();/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = m_optionTextUI[enOptionTextUI_ReturnToQuestSelect].GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_ReturnToQuestSelectTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_ReturnToQuestSelect],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_textSelectUI.GetPosition();/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = m_textSelectUI.GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_SelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_textSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);
		}

		/*選択したときの演出UIアニメーションのリセット処理。*/
		void Pause::ResetSelectDirectionUIAnimation()
		{
			for (int i = 0; i < enPosition_Num; i++)
			{
				for (int j = 0; j < enSelectDirectionUIAnimationSprite_Num; j++)
				{
					m_selectDirectionUIAnimation[i][j]->Reset();
				}
			}
		}

		/*選択の更新処理。*/
		void Pause::UpdateSelect()
		{
			/*上を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonUp))
			{
				ResetAlphaUIAnimation();
				/*現在の選択が「ゲームに戻る」を選択していたら。*/
				if (m_currentSelect == enSelect_ReturnToGame) { m_currentSelect = enSelect_ReturnToQuestSelect; return; }

				/*上を選択する。*/
				m_currentSelect--;
				return;
			}

			/*下を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonDown))
			{
				ResetAlphaUIAnimation();
				/*現在の選択が「メンバー選択に戻る」を選択していたら。*/
				if (m_currentSelect == enSelect_ReturnToQuestSelect) { m_currentSelect = enSelect_ReturnToGame; return; }

				/*下を選択する。*/
				m_currentSelect++;
				return;
			}

			/*Aボタンを押したら選択したときの演出を流すようにする。*/
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				EnableDirection();
			}
		}

		/*UIアニメーションの更新処理。*/
		void Pause::UpdateUIAnimation()
		{
			if (DidSelect())
			{
				return;
			}

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
				/*選択したときの演出UIアニメーション終わっていたら選択を元に処理する。*/
				else
				{
					/*「ゲームを続ける」と「操作方法」以外を選択していたら半透明フェードアウトする。*/
					if (m_currentSelect != enSelect_ReturnToGame && m_currentSelect != enSelectDirectionUIAnimationSprite_HowToPlayTextUI)
					{
						nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_HalfFadeOut);
					}

					ResetSelectDirectionUIAnimation();
					EnableSelect();
				}
				return;
			}

			/*UIの透明度を変えるアニメーションの再生。*/
			m_alphaUIAnimation->Update();
		}

		/*スプライトの更新処理。*/
		void Pause::UpdateSprite()
		{
			/*背景。*/
			m_backGround.Update();
			
			/*ポーズテキストUI。*/
			m_pauseTextUI.Update();

			/*白枠UI。*/
			m_whiteFrameUI.Update();

			/*テキスト選択UI。*/
			m_textSelectUI.SetPosition(m_optionTextUI[m_currentSelect].GetPosition());
			m_textSelectUI.Update();

			for (int i = 0; i < enOptionTextUI_Num; i++)
			{
				/*選択肢テキストUI。*/
				m_optionTextUI[i].Update();
			}

			/*ボタンUI。*/
			m_buttonUI.Update();

			/*テキストUI。*/
			m_textUI.Update();
		}
	}
}