#include "stdafx.h"
#include "TitleSelect.h"
#include "Src/Fade/Fade.h"

namespace {
	/*オフセット。*/
	const float OFFSET = -1080.0f;/*オフセット。*/

	/*選択UI。*/
	const float SELECT_UI_WIDTH = 512;/*選択UIの幅。*/

	const float SELECT_UI_HEIGHT = 512;/*選択UIの高さ。*/

	const Vector3 SELECT_UI_INIT_POSITION = { 0.0f,225.0f + OFFSET,0.0f };/*選択UIの初期位置。*/

	const float SELECT_UI_ROTATION_DEGREE = -90.0f;/*選択UIの回転角度。*/

	const Vector3 SELECT_UI_INIT_SCALE = Vector3(0.3f, 0.3f, 1.0f);/*選択UIの初期大きさ。*/

	/*選択肢テキストUI(「ゲームスタート」のテキストをベースに)。*/
	const float OPTION_TEXT_UI_WIDTH = 1024;/*選択肢テキストUIの幅。*/

	const float OPTION_TEXT_UI_HEIGHT = 256;/*選択肢テキストUIの高さ。*/

	const Vector3 OPTION_TEXT_UI_INIT_POSITION = { 0.0f,225.0f + OFFSET,0.0f };/*選択肢テキストUIの初期位置。*/

	const float OPTION_TEXT_UI_POSITION_INTERVAL = 200.0f;/*選択肢テキストUIの位置の間隔。*/

	const Vector3 OPTION_TEXT_UI_INIT_SCALE = { 1.3f,1.3f,1.0f };/*テキストUIの初期大きさ。*/

	/*ボタンUI。*/
	const float BUTTON_UI_WIDTH = 1024;/*ボタンUIの幅。*/

	const float BUTTON_UI_HEIGHT = 1024;/*ボタンUIの高さ。*/

	const Vector3 BUTTON_UI_INIT_POSITION[nsApp::nsTitle::TitleSelect::EnButtonUI::enButtonUI_Num] = { 
		Vector3{-600.0f,-450.0f + OFFSET,0.0f},/*Aボタン。*/
		Vector3{-850.0f,-450.0f + OFFSET,0.0f}/*Bボタン。*/
	};/*ボタンUIの初期位置。*/

	const Vector3 BUTTON_UI_INIT_SCALE = { 0.1f,0.1f,1.0f };/*ボタンUIの初期大きさ。*/

	/*テキストUI。*/
	const float TEXT_UI_WIDTH = 1024;/*テキストUIの幅。*/

	const float TEXT_UI_HEIGHT = 256;/*テキストUIの高さ。*/

	const Vector3 TEXT_UI_INIT_POSITION[nsApp::nsTitle::TitleSelect::EnTextUI::enTextUI_Num] = {
		Vector3{-475.0f,-450.0f + OFFSET,0.0f},/*決定。*/
		Vector3{-725.0f,-450.0f + OFFSET,0.0f}/*戻る。*/
	};/*テキストUIの初期位置。*/

	const Vector3 TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*テキストUIの初期大きさ。*/

	/*UIアニメーション。*/
	const float SLIDE_UI_ANIMATION_PLAY_SPEED = 1.0f;/*UIをスライドさせるアニメーションの再生速度。*/
}

namespace nsApp
{
	namespace nsTitle
	{
		/*開始処理。*/
		bool TitleSelect::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			return true;
		}

		/*更新処理。*/
		void TitleSelect::Update()
		{
			/*選択できていない　かつ
			* 上にスライド演出中ではないとき　かつ
			* 下にスライド演出中ではないときは選択できる。*/
			if (!DidSelect() && !IsSlideUpDirection() && !IsSlideDownDirection())
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
		void TitleSelect::Render(RenderContext& rc)
		{	
			/*選択UIの描画。*/
			m_selectUI.Draw(rc);

			for(int i = 0; i < enOptionTextUI_Num; i++)
			{
				/*選択肢テキストUIの描画。*/
				m_optionTextUI[i].Draw(rc);
			}

			for (int j = 0; j < enButtonUI_Num; j++)
			{
				/*ボタンUIの描画。*/
				m_buttonUI[j].Draw(rc);
			}

			for (int k = 0; k < enTextUI_Num; k++)
			{
				/*テキストUIの描画。*/
				m_textUI[k].Draw(rc);
			}
		}

		/*スプライトの初期化。*/
		void TitleSelect::InitSprite()
		{
			/*選択UI。*/
			InitSelectUI();

			for (int i = 0; i < enOptionTextUI_Num; i++)
			{
				/*選択肢テキストUI。*/
				InitOptionTextUI((EnOptionTextUI)i, i);
			}

			for (int j = 0; j < enButtonUI_Num; j++)
			{
				/*ボタンUI。*/
				InitButtonUI((EnButtonUI)j);
			}

			for (int k = 0; k < enTextUI_Num; k++)
			{
				/*テキストUI。*/
				InitTextUI((EnTextUI)k);
			}
		}
		
		/*選択UIの初期化。*/
		void TitleSelect::InitSelectUI()
		{
			m_selectUI.Init(m_selectUIFilePath.c_str(), SELECT_UI_WIDTH, SELECT_UI_HEIGHT);/*初期化。*/
			m_selectUI.SetPosition(SELECT_UI_INIT_POSITION);/*位置設定。*/
			Quaternion rotation;
			rotation.SetRotationDegZ(SELECT_UI_ROTATION_DEGREE);
			m_selectUI.SetRotation(rotation);/*回転設定。*/
			m_selectUI.SetScale(SELECT_UI_INIT_SCALE);/*大きさ設定。*/
			m_selectUI.Update();/*更新処理。*/

			m_slideUIAnimationSprite.push_back(&m_selectUI);/*選択UIをスライドUIアニメーションのスプライトに追加。*/
		}

		/*選択肢テキストUIの初期化。*/
		void TitleSelect::InitOptionTextUI(EnOptionTextUI optionTextUI, int optionTextIndex)
		{
			Vector3 initPosition = OPTION_TEXT_UI_INIT_POSITION;/*初期位置。*/
			initPosition.y -= OPTION_TEXT_UI_POSITION_INTERVAL * optionTextIndex;/*位置の間隔を減算。*/

			m_optionTextUI[optionTextUI].Init(m_optionTextUIFilePath[optionTextUI].c_str(), OPTION_TEXT_UI_WIDTH, OPTION_TEXT_UI_HEIGHT);/*初期化。*/
			m_optionTextUI[optionTextUI].SetPosition(initPosition);/*位置設定。*/
			m_optionTextUI[optionTextUI].SetScale(OPTION_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_optionTextUI[optionTextUI].Update();/*更新処理。*/

			m_slideUIAnimationSprite.push_back(&m_optionTextUI[optionTextUI]);/*選択肢テキストUIをスライドUIアニメーションのスプライトに追加。*/
		}

		/*ボタンUIの初期化。*/
		void TitleSelect::InitButtonUI(EnButtonUI buttonUI)
		{
			m_buttonUI[buttonUI].Init(m_buttonUIFilePath[buttonUI].c_str(), BUTTON_UI_WIDTH, BUTTON_UI_HEIGHT);/*初期化。*/
			m_buttonUI[buttonUI].SetPosition(BUTTON_UI_INIT_POSITION[buttonUI]);/*位置設定。*/
			m_buttonUI[buttonUI].SetScale(BUTTON_UI_INIT_SCALE);/*大きさ設定。*/
			m_buttonUI[buttonUI].Update();/*更新処理。*/

			m_slideUIAnimationSprite.push_back(&m_buttonUI[buttonUI]);/*ボタンUIをスライドUIアニメーションのスプライトに追加。*/
		}

		/*テキストUIの初期化。*/
		void TitleSelect::InitTextUI(EnTextUI textUI)
		{
			m_textUI[textUI].Init(m_textUIFilePath[textUI].c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT);/*初期化。*/
			m_textUI[textUI].SetPosition(TEXT_UI_INIT_POSITION[textUI]);/*位置設定。*/
			m_textUI[textUI].SetScale(TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textUI[textUI].Update();/*更新処理。*/
			
			m_slideUIAnimationSprite.push_back(&m_textUI[textUI]);/*テキストUIをスライドUIアニメーションのスプライトに追加。*/
		}

		/*UIアニメーションの初期化。*/
		void TitleSelect::InitUIAnimation()
		{
			for (int i = 0; i < m_slideUIAnimationSprite.size(); i++)
			{
				/*UIを上にスライドさせるアニメーション。*/
				InitSlideUpUIAnimation(m_slideUIAnimationSprite[i], i);

				/*UIを下にスライドさせるアニメーション。*/
				InitSlideDownUIAnimation(m_slideUIAnimationSprite[i], i);
			}
		}

		/*UIを上にスライドさせるアニメーションの初期化。*/
		void TitleSelect::InitSlideUpUIAnimation(SpriteRender* spriteData, int spriteIndex)
		{
			/*UIを上にスライドさせるアニメーションの値の設定。*/
			Vector3 basePosition = spriteData->GetPosition();/*元の位置。*/

			Vector3 currentSpritePosition = spriteData->GetPosition();/*現在のスプライトの位置。*/
			currentSpritePosition.y -= OFFSET;

			Vector3 targetPosition = currentSpritePosition;/*ターゲットの位置。*/

			m_slideUpUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/)
			);

			m_slideUpUIAnimation[spriteIndex]->SetEasingFunction([](float t)
			{
				float t_minus_1 = t - 1.0f;
				return 1.0f + (t_minus_1 * t_minus_1 * t_minus_1);
			});
		}

		/*UIを下にスライドさせるアニメーションの初期化。*/
		void TitleSelect::InitSlideDownUIAnimation(SpriteRender* spriteData, int spriteIndex)
		{
			/*UIを下にスライドさせるアニメーションの値の設定。*/
			Vector3 currentSpritePosition = spriteData->GetPosition();/*現在のスプライトの位置。*/
			currentSpritePosition.y -= OFFSET;
			Vector3 basePosition = currentSpritePosition;/*元の位置。*/
			Vector3 targetPosition = spriteData->GetPosition();/*ターゲットの位置。*/

			m_slideDownUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/)
			);

			m_slideDownUIAnimation[spriteIndex]->SetEasingFunction([](float t)
			{
				float t_minus_1 = t - 1.0f;
				return 1.0f + (t_minus_1 * t_minus_1 * t_minus_1);
			});
		}

		/*UIを上にスライドさせるアニメーションのリセット処理。*/
		void TitleSelect::ResetSlideUpUIAnimation()
		{
			for (int i = 0; i < m_slideUpUIAnimation.size(); i++)
			{
				m_slideUpUIAnimation[i]->Reset();
			}
		}

		/*UIを下にスライドさせるアニメーションのリセット処理。*/
		void TitleSelect::ResetSlideDownUIAnimation()
		{
			for (int i = 0; i < m_slideDownUIAnimation.size(); i++)
			{
				m_slideDownUIAnimation[i]->Reset();
			}
		}

		/*選択の更新処理。*/
		void TitleSelect::UpdateSelect()
		{
			/*上を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonUp))
			{
				/*現在の選択が「ゲームスタート」を選択していたら。*/
				if (m_currentSelect == enSelect_Start) { m_currentSelect = enSelect_ExitGame; return; }

				/*上を選択する。*/
				m_currentSelect--;
				return;
			}

			/*下を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonDown))
			{
				/*現在の選択が「ゲーム終了」を選択していたら。*/
				if (m_currentSelect == enSelect_ExitGame) { m_currentSelect = enSelect_Start; return; }

				/*下を選択する。*/
				m_currentSelect++;
				return;
			}

			/*Aボタンを押したら選択したときの演出を流すようにする。*/
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				if (m_currentSelect == enSelect_Start)
				{
					/*フェードアウトに切り替え。*/
					nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeOut);
				}
				EnableSelect();
				return;
			}

			/*Bボタンを押したら下にスライドする演出を流すようにする。*/
			if (g_pad[0]->IsTrigger(enButtonB))
			{
				EnableBackScene();
				EnableSlideDownDirection();
			}
		}

		/*UIアニメーションの更新処理。*/
		void TitleSelect::UpdateUIAnimation()
		{
			if (IsSlideUpDirection())
			{
				for (int i = 0; i < m_slideUpUIAnimation.size(); i++)
				{
					/*UIを上にスライドさせるアニメーションの再生。*/
					m_slideUpUIAnimation[i]->Update();
				}
			}
			else if (IsSlideDownDirection())
			{
				for (int i = 0; i < m_slideDownUIAnimation.size(); i++)
				{
					/*UIを下にスライドさせるアニメーションの再生。*/
					m_slideDownUIAnimation[i]->Update();
				}
			}
		}

		/*スプライトの更新処理。*/
		void TitleSelect::UpdateSprite()
		{
			/*選択UI。*/
			float offsetX = -500.0f;/*選択UIをX軸にずらす用のオフセット。*/
			Vector3 currentSelectUIPosition = m_optionTextUI[m_currentSelect].GetPosition();/*現在の選択UIの位置。*/
			currentSelectUIPosition.x += offsetX;/*選択UIをX軸にずらす。*/

			m_selectUI.SetPosition(currentSelectUIPosition);
			m_selectUI.Update();

			for (int i = 0; i < enOptionTextUI_Num; i++)
			{
				/*選択肢テキストUI。*/
				m_optionTextUI[i].Update();
			}

			for (int j = 0; j < enButtonUI_Num; j++)
			{
				/*ボタンUI。*/
				m_buttonUI[j].Update();
			}

			for (int k = 0; k < enTextUI_Num; k++)
			{
				/*テキストUI。*/
				m_textUI[k].Update();
			}
		}
	}
}