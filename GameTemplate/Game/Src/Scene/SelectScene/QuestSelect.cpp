#include "stdafx.h"
#include "QuestSelect.h"
#include "Src/Fade/Fade.h"

namespace {
	/*クエスト選択テキストUI。*/
	const float QUEST_SELECT_TEXT_UI_WIDTH = 1024;/*クエスト選択テキストUIの幅。*/

	const float QUEST_SELECT_TEXT_UI_HEIGHT = 256;/*クエスト選択テキストUIの高さ。*/
	
	const Vector3 QUEST_SELECT_TEXT_UI_INIT_POSITION = { -700.0f,450.0f,0.0f };/*クエスト選択テキストUIの初期位置。*/

	const Vector3 QUEST_SELECT_TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*クエスト選択テキストUIの初期大きさ。*/

	/*ターゲットUI。*/
	const float TARGET_UI_WIDTH = 990;/*ターゲットUIの幅。*/

	const float TARGET_UI_HEIGHT = 1080;/*ターゲットUIの高さ。*/

	const Vector3 TARGET_UI_INIT_POSITION = { 450.0f,-50.0f,0.0f };/*ターゲットUIの初期位置。*/

	const Vector3 TARGET_UI_INIT_SCALE = { 0.85f,0.85f,1.0f };/*ターゲットUIの初期大きさ。*/

	/*ターゲット選択UI。*/
	const float TARGET_SELECT_UI_WIDTH = 1024;/*ターゲット選択UIの幅。*/

	const float TARGET_SELECT_UI_HEIGHT = 256;/*ターゲット選択UIの高さ。*/

	const Vector3 TARGET_SELECT_UI_INIT_POSITION = { -550.0f,200.0f,0.0f };/*ターゲット選択UIの初期位置。*/

	const Vector3 TARGET_SELECT_UI_INIT_SCALE = { 1.04f,1.15f,1.0f };/*ターゲット選択UIの初期大きさ。*/

	const Vector4 TARGET_SELECT_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.6f };/*ターゲット選択UIの初期乗算色。*/

	/*ターゲットテキストUI。*/
	const float TARGET_TEXT_UI_WIDTH = 1024;/*ターゲットテキストUIの幅。*/

	const float TARGET_TEXT_UI_HEIGHT = 256;/*ターゲットテキストUIの高さ。*/

	const Vector3 TARGET_TEXT_UI_INIT_POSITION = { -550.0f,200.0f,0.0f };/*ターゲットテキストUIの初期位置。*/

	const Vector3 TARGET_TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*ターゲットテキストUIの初期大きさ。*/

	/*ボタンUI。*/
	const float BUTTON_UI_WIDTH = 1024;/*ボタンUIの幅。*/

	const float BUTTON_UI_HEIGHT = 1024;/*ボタンUIの高さ。*/

	const Vector3 BUTTON_UI_INIT_POSITION[nsApp::nsSelect::QuestSelect::EnButtonUI::enButtonUI_Num] = {
		Vector3{-425.0f,-450.0f,0.0f},/*Aボタン。*/
		Vector3{-850.0f,-450.0f,0.0f},/*Bボタン。*/
	};/*ボタンUIの初期位置。*/

	const Vector3 BUTTON_UI_INIT_SCALE = { 0.1f,0.1f,1.0f };/*ボタンUIの初期大きさ。*/

	/*テキストUI。*/
	const float TEXT_UI_WIDTH = 1024;/*テキストUIの幅。*/

	const float TEXT_UI_HEIGHT = 256;/*テキストUIの高さ。*/

	const Vector3 TEXT_UI_INIT_POSITION[nsApp::nsSelect::QuestSelect::EnTextUI::enTextUI_Num] = {
		Vector3{-300.0f,-450.0f,0.0f},/*決定。*/
		Vector3{-650.0f,-450.0f,0.0f},/*ゲーム終了。*/
	};/*テキストUIの初期位置。*/

	const Vector3 TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*テキストUIの初期大きさ。*/

	/*UIアニメーション。*/
	const float SLIDE_UI_ANIMATION_PLAY_SPEED = 4.0f;/*UIをスライドさせるアニメーションの再生速度。*/

	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 1.2f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const float SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED = 7.0f;/*選択したときの演出UIアニメーションの再生速度。*/

	const Vector3 AFTER_UI_ANIMATION_POSITION[nsApp::nsSelect::QuestSelect::EnSlide::enSlide_Num][nsApp::nsSelect::QuestSelect::EnSlideUIAnimationSprite::enSlideUIAnimationSprite_Num] = {
		/*左側にスライドした時の位置。*/
		Vector3{-2050.0f,-50.0f,0.0f},/*ターゲットUI。*/
		Vector3{-3050.0f,200.0f,0.0f},/*ターゲット選択UI。*/
		Vector3{-3050.0f,200.0f,0.0f},/*ターゲットテキストUI。*/

		/*右側にスライドした時の位置。*/
		Vector3{450.0f,-50.0f,0.0f},/*ターゲットUI。*/
		Vector3{-550.0f,200.0f,0.0f},/*ターゲット選択UI。*/
		Vector3{-550.0f,200.0f,0.0f},/*ターゲットテキストUI。*/
	};/*UIをスライドさせるアニメーション後の位置。*/

	const float AFTER_UI_ANIMATION_ALPHA = 0.2f;/*UIの透明度を変えるアニメーション後の透明度。*/

	const float DOWN_POSITION_OFFSET = 25.0f;/*選択したときの演出UIアニメーション時に下降する位置のオフセット。*/
}

namespace nsApp
{
	namespace nsSelect
	{
		/*開始処理。*/
		bool QuestSelect::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			return true;
		}

		/*更新処理。*/
		void QuestSelect::Update()
		{
			if (nsApp::nsFade::Fade::GetInstance()->IsFadeIn())
			{
				return;
			}

			/*演出中でないかつ
			* 選択できていなければかつ
			* ゲームを終了するボタンを押していなければ。
			*/
			if (!IsDirection() && !DidSelect() && !IsPushGameEndButton())
			{
				/*Aボタンを押したら選択完了。*/
				if (g_pad[0]->IsTrigger(enButtonA))
				{
					EnableSelect();
					EnableDirection();
				}
				/*Bボタンを押したらゲームを終了するかを確認する画面を表示する。*/
				else if (g_pad[0]->IsTrigger(enButtonB))
				{
					nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::enFadeType_HalfFadeOut);
					EnablePushGameEndButton();
				}
			}

			/*UIアニメーション。*/
			UpdateUIAnimation();

			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void QuestSelect::Render(RenderContext& rc)
		{
			/*クエスト選択テキストUIの描画。*/
			m_questSelectTextUI.Draw(rc);

			/*ターゲットUIの描画。*/
			m_targetUI.Draw(rc);

			/*ターゲット選択UIの描画。*/
			m_targetSelectUI.Draw(rc);
			
			/*ターゲットテキストUIの描画。*/
			m_targetTextUI.Draw(rc);

			if (!IsDrawingButtonAndTextUI())
			{
				return;
			}

			for (int i = 0; i < enButtonUI_Num; i++)
			{
				/*ボタンUIの描画。*/
				m_buttonUI[i].Draw(rc);
			}

			for (int j = 0; j < enTextUI_Num; j++)
			{
				/*テキストUIの描画。*/
				m_textUI[j].Draw(rc);
			}
		}

		/*スプライトの初期化。*/
		void QuestSelect::InitSprite()
		{
			/*クエスト選択テキストUI。*/
			InitQuestSelectTextUI();

			/*ターゲットUI。*/
			InitTargetUI();

			/*ターゲット選択UI。*/
			InitTargetSelectUI();

			/*ターゲットテキストUI。*/
			InitTargetTextUI();

			for (int i = 0; i < enButtonUI_Num; i++)
			{
				/*ボタンUI。*/
				InitButtonUI((EnButtonUI)i);
			}

			for (int j = 0; j < enTextUI_Num; j++)
			{
				/*テキストUI。*/
				InitTextUI((EnTextUI)j);
			}
		}

		/*クエスト選択テキストUIの初期化。*/
		void QuestSelect::InitQuestSelectTextUI()
		{
			m_questSelectTextUI.Init(m_questSelectTextUIFilePath.c_str(), QUEST_SELECT_TEXT_UI_WIDTH, QUEST_SELECT_TEXT_UI_HEIGHT);/*初期化。*/
			m_questSelectTextUI.SetPosition(QUEST_SELECT_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_questSelectTextUI.SetScale(QUEST_SELECT_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_questSelectTextUI.Update();/*更新処理。*/
		}

		/*ターゲットUIの初期化。*/
		void QuestSelect::InitTargetUI()
		{
			m_targetUI.Init(m_targetUIFilePath.c_str(), TARGET_UI_WIDTH, TARGET_UI_HEIGHT);/*初期化。*/
			m_targetUI.SetPosition(TARGET_UI_INIT_POSITION);/*位置設定。*/
			m_targetUI.SetScale(TARGET_UI_INIT_SCALE);/*大きさ設定。*/
			m_targetUI.Update();/*更新処理。*/
		}

		/*ターゲット選択UIの初期化。*/
		void QuestSelect::InitTargetSelectUI()
		{
			m_targetSelectUI.Init(m_targetSelectUIFilePath.c_str(), TARGET_SELECT_UI_WIDTH, TARGET_SELECT_UI_HEIGHT);/*初期化。*/
			m_targetSelectUI.SetPosition(TARGET_SELECT_UI_INIT_POSITION);/*位置設定。*/
			m_targetSelectUI.SetScale(TARGET_SELECT_UI_INIT_SCALE);/*大きさ設定。*/
			m_targetSelectUI.SetMulColor(TARGET_SELECT_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_targetSelectUI.Update();/*更新処理。*/
		}
		
		/*ターゲットテキストUIの初期化。*/
		void QuestSelect::InitTargetTextUI()
		{
			m_targetTextUI.Init(m_targetTextUIFilePath.c_str(), TARGET_TEXT_UI_WIDTH, TARGET_TEXT_UI_HEIGHT);/*初期化。*/
			m_targetTextUI.SetPosition(TARGET_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_targetTextUI.SetScale(TARGET_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_targetTextUI.Update();/*更新処理。*/
		}

		/*ボタンUIの初期化。*/
		void QuestSelect::InitButtonUI(EnButtonUI buttonUI)
		{
			m_buttonUI[buttonUI].Init(m_buttonUIFilePath[buttonUI].c_str(), BUTTON_UI_WIDTH, BUTTON_UI_HEIGHT);/*初期化。*/
			m_buttonUI[buttonUI].SetPosition(BUTTON_UI_INIT_POSITION[buttonUI]);/*位置設定。*/
			m_buttonUI[buttonUI].SetScale(BUTTON_UI_INIT_SCALE);/*大きさ設定。*/
			m_buttonUI[buttonUI].Update();/*更新処理。*/
		}

		/*テキストUIの初期化。*/
		void QuestSelect::InitTextUI(EnTextUI textUI)
		{
			m_textUI[textUI].Init(m_textUIFilePath[textUI].c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);/*初期化。*/
			m_textUI[textUI].SetPosition(TEXT_UI_INIT_POSITION[textUI]);/*位置設定。*/
			m_textUI[textUI].SetScale(TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textUI[textUI].Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void QuestSelect::InitUIAnimation()
		{
			/*UIをスライドさせるアニメーション。*/
			InitSlideUIAnimation();

			/*UIの透明度を変えるアニメーション。*/
			InitAlphaUIAnimation();

			/*選択したときの演出UIアニメーション。*/
			InitSelectDirectionUIAnimation();
		}

		/*UIをスライドさせるアニメーションの初期化。*/
		void QuestSelect::InitSlideUIAnimation()
		{
			/*UIをスライドさせるアニメーションの値の設定。*/
			Vector3 basePosition = m_targetUI.GetPosition();/*元の位置。*/
			Vector3 targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TargetUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_TargetUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = m_targetSelectUI.GetPosition();/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TargetSelectUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_TargetSelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = m_targetTextUI.GetPosition();/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TargetTextUI];/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_TargetTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetTextUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TargetUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_TargetUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_TargetUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TargetSelectUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_TargetSelectUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_TargetSelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TargetTextUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_TargetTextUI];/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_TargetTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetTextUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);
		}

		/*UIの透明度を変えるアニメーションの初期化。*/
		void QuestSelect::InitAlphaUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = m_targetSelectUI.GetMulColor().a;/*元の透明度。*/
			float targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_targetSelectUI,/*アニメーションをさせるスプライト。*/
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
		void QuestSelect::InitSelectDirectionUIAnimation()
		{
			/*選択したときの演出UIアニメーションの値の設定。*/
			Vector3 basePosition = m_targetSelectUI.GetPosition();/*元の位置。*/
			Vector3 targetPosition = m_targetSelectUI.GetPosition();/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_TargetSelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_targetTextUI.GetPosition();/*元の位置。*/
			targetPosition = m_targetTextUI.GetPosition();/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_TargetTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetTextUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_targetSelectUI.GetPosition();/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = m_targetSelectUI.GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_TargetSelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_targetTextUI.GetPosition();/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = m_targetTextUI.GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_TargetTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_targetTextUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);
		}

		/*UIをスライドさせるアニメーションのリセット処理。*/
		void QuestSelect::ResetSlideUIAnimation(EnSlide slide)
		{
			for (int i = 0; i < enSlideUIAnimationSprite_Num; i++)
			{
				m_slideUIAnimation[slide][i]->Reset();
			}
		}

		/*選択したときの演出UIアニメーションのリセット処理。*/
		void QuestSelect::ResetSelectDirectionUIAnimation()
		{
			for (int i = 0; i < enPosition_Num; i++)
			{
				for (int j = 0; j < enSelectDirectionUIAnimationSprite_Num; j++)
				{
					m_selectDirectionUIAnimation[i][j]->Reset();
				}
			}
		}

		/*UIアニメーションの更新処理。*/
		void QuestSelect::UpdateUIAnimation()
		{
			/*演出中なら。*/
			if (IsDirection())
			{
				/*選択できている状態。*/
				if (DidSelect())
				{
					/*選択したときの演出UIアニメーション(位置を下降する)が終わっていなければ再生し続ける。*/
					if (!m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_TargetSelectUI]->IsEnd())
					{
						m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_TargetSelectUI]->Update();
						m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_TargetTextUI]->Update();
						return;
					}
					/*選択したときの演出UIアニメーション(位置を上昇する)が終わっていなければ再生し続ける。*/
					else if (!m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_TargetSelectUI]->IsEnd())
					{
						m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_TargetSelectUI]->Update();
						m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_TargetTextUI]->Update();
						return;
					}
				}

				for (int i=0; i<enSlideUIAnimationSprite_Num ;i++)
				{
					m_slideUIAnimation[m_currentSlide][i]->Update();
				}
				return;
			}

			m_alphaUIAnimation->Update();
		}

		/*スプライトの更新処理。*/
		void QuestSelect::UpdateSprite()
		{
			/*クエスト選択テキストUI。*/
			m_questSelectTextUI.Update();

			/*ターゲットUI。*/
			m_targetUI.Update();

			/*ターゲット選択UI。*/
			m_targetSelectUI.Update();

			/*ターゲットテキストUI。*/
			m_targetTextUI.Update();

			for (int i = 0; i < enButtonUI_Num; i++)
			{
				/*ボタンUI。*/
				m_buttonUI[i].Update();
			}

			for (int j = 0; j < enTextUI_Num; j++)
			{
				/*テキストUI。*/
				m_textUI[j].Update();
			}
		}
	}
}