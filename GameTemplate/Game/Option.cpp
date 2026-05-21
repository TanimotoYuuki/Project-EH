#include "stdafx.h"
#include "Option.h"

namespace {
	/*背景。*/
	const float BACK_GROUND_WIDTH = 1754;/*背景の幅。*/

	const float BACK_GROUND_HEIGHT = 1011;/*背景の高さ。*/

	/*バーUI。*/
	const float BAR_UI_WIDTH = 915;/*バーUIの幅。*/

	const float BAR_UI_HEIGHT = 85;/*バーUIの高さ。*/

	const Vector3 BAR_UI_INIT_POSITION[nsApp::nsOption::Option::enBarUI_Num] = {
		Vector3{235.0f,275.0f,0.0f},/*マスターボリューム。*/
		Vector3{235.0f,95.0f,0.0f},/*BGM。*/
		Vector3{235.0f,-85.0f,0.0f},/*SE。*/
	};/*バーUIの初期位置。*/

	const Vector3 BAR_UI_INIT_SCALE = {0.7f,0.7f,1.0f};/*バーUIの初期大きさ。*/

	/*ゲージUI。*/
	const float GAUGE_UI_WIDTH = 908;/*ゲージUIの幅。*/

	const float GAUGE_UI_HEIGHT = 79;/*ゲージUIの高さ。*/

	const Vector3 GAUGE_UI_INIT_POSITION[nsApp::nsOption::Option::enGaugeUI_Num] = {
		Vector3{-83.25f,275.0f,0.0f},/*マスターボリューム。*/
		Vector3{-83.25f,95.0f,0.0f},/*BGM。*/
		Vector3{-83.25f,-85.0f,0.0f},/*SE。*/
	};/*ゲージUIの初期位置。*/

	const Vector3 GAUGE_UI_INIT_SCALE = {0.7f,0.7f,1.0f};/*ゲージUIの初期大きさ。*/

	const Vector2 GAUGE_UI_INIT_PIVOT = { 0.0f,0.5f };/*ゲージUIの初期ピボット。*/

	/*音量の数字UI。*/
	const float VOLUME_NUMBER_UI_WIDTH = 1024;/*音量の数字UIの幅。*/

	const float VOLUME_NUMBER_UI_HEIGHT = 128;/*音量の数字UIの高さ。*/

	const Vector3 VOLUME_NUMBER_UI_INIT_POSITION[nsApp::nsOption::Option::enGaugeUI_Num] = {
		Vector3{645.0f, 275.0f, 0.0f},/*マスターボリューム。*/
		Vector3{645.0f, 95.0f, 0.0f},/*BGM。*/
		Vector3{645.0f, -85.0f, 0.0f},/*SE。*/
	};/*音量の数字UIの初期位置。*/

	const float VOLUME_NUMBER_UI_POSITION_INTERVAL = 50.0f;/*音量の数字UIの位置の間隔。*/

	const Vector3 VOLUME_NUMBER_UI_INIT_SCALE = Vector3(0.6f, 0.6f, 1.0f);/*音量の数字UIの初期大きさ。*/

	/*円選択UI。*/
	const float CIRCLE_SELECT_UI_WIDTH = 128;/*円選択UIの幅。*/

	const float CIRCLE_SELECT_UI_HEIGHT = 127;/*円選択UIの高さ。*/

	const Vector3 CIRCLE_SELECT_UI_INIT_POSITION = { 555.0f,215.0f,0.0f };/*円選択UIの初期位置。*/

	const Vector3 CIRCLE_SELECT_UI_INIT_SCALE = { 0.9f,0.9f,1.0f };/*円選択UIの初期大きさ。*/

	/*円UI。*/
	const float CIRCLE_UI_WIDTH = 128;/*円UIの幅。*/

	const float CIRCLE_UI_HEIGHT = 127;/*円UIの高さ。*/

	const Vector3 CIRCLE_UI_INIT_POSITION[nsApp::nsOption::Option::EnCircleUI::enCircleUI_Num] = {
		Vector3{555.0f,275.0f,0.0f},/*マスターボリューム。*/
		Vector3{555.0f,95.0f,0.0f},/*BGM。*/
		Vector3{555.0f,-85.0f,0.0f},/*SE。*/
	};/*円UIの初期位置。*/

	const Vector3 CIRCLE_UI_POSITION_MIN[nsApp::nsOption::Option::EnCircleUI::enCircleUI_Num] = {
		Vector3{-83.5f,275.0f,0.0f},/*マスターボリューム。*/
		Vector3{-83.5f,95.0f,0.0f },/*BGM。*/
		Vector3{-83.5f,-85.0f,0.0f },/*SE。*/
	};/*円UIの位置の下限。*/

	const Vector3 CIRCLE_UI_INIT_SCALE = { 0.7f,0.7f,1.0f };/*円UIの初期大きさ。*/

	/*「OK」テキスト選択UI。*/
	const float OK_TEXT_SELECT_UI_WIDTH = 549;/*「OK」テキスト選択UIの幅。*/

	const float OK_TEXT_SELECT_UI_HEIGHT = 81;/*「OK」テキスト選択UIの高さ。*/

	const Vector3 OK_TEXT_SELECT_UI_INIT_POSITION = { 0.0f,-300.0f,0.0f };/*「OK」テキスト選択UIの初期位置。*/

	const Vector3 OK_TEXT_SELECT_UI_INIT_SCALE = { 1.04f,1.23f,1.0f };/*「OK」テキスト選択UIの初期大きさ。*/

	/*選択肢テキストUI。*/
	const float OPTION_TEXT_UI_WIDTH[nsApp::nsOption::Option::EnOptionTextUI::enOptionTextUI_Num] = {
		940,/*マスターボリューム。*/
		297,/*BGM。*/
		170,/*SE。*/
		549,/*OK。*/
	};/*選択肢テキストUIの幅。*/

	const float OPTION_TEXT_UI_HEIGHT[nsApp::nsOption::Option::EnOptionTextUI::enOptionTextUI_Num] = {
		113,/*マスターボリューム。*/
		114,/*BGM。*/
		114,/*SE。*/
		81,/*OK。*/
	};/*選択肢テキストUIの高さ。*/

	const Vector3 OPTION_TEXT_UI_INIT_POSITION[nsApp::nsOption::Option::EnOptionTextUI::enOptionTextUI_Num] = {
		Vector3{-445.0f,275.0f,0.0f},/*マスターボリューム。*/
		Vector3{-445.0f,95.0f,0.0f},/*BGM。*/
		Vector3{-445.0f,-85.0f,0.0f},/*SE。*/
		Vector3{0.0f,-300.0f,0.0f},/*OK。*/
	};/*選択肢テキストUIの初期位置。*/

	const Vector3 OPTION_TEXT_UI_INIT_SCALE[nsApp::nsOption::Option::EnOptionTextUI::enOptionTextUI_Num] = {
		Vector3{0.6f,0.6f,1.0f},/*マスターボリューム。*/
		Vector3{0.6f,0.6f,1.0f},/*BGM。*/
		Vector3{0.6f,0.6f,1.0f},/*SE。*/
		Vector3{1.0f,1.0f,1.0f},/*OK。*/
	};/*選択肢テキストUIの初期大きさ。*/

	/*音量。*/
	const int VOLUME_MAX = 100;/*音量の最大値。*/

	/*UIアニメーション。*/
	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 1.2f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const float SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED = 7.0f;/*選択したときの演出UIアニメーションの再生速度。*/

	const float AFTER_UI_ANIMATION_ALPHA = 0.2f;/*UIの透明度を変えるアニメーション後の透明度。*/

	const float DOWN_POSITION_OFFSET = 25.0f;/*選択したときの演出UIアニメーション時に下降する位置のオフセット。*/
}

namespace nsApp
{
	namespace nsOption
	{
		/*開始処理。*/
		bool Option::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			for (int i = 0; i < enGaugeUI_Num; i++)
			{
				/*音量の割合を設定。*/
				m_volumeRate[i] = VOLUME_MAX;
				m_volumeMaxRate[i] = VOLUME_MAX;

				m_volumeNumverDisplayManager[i].push_back(enVolumeNumberDisplayUI_One);
				m_volumeNumverDisplayManager[i].push_back(enVolumeNumberDisplayUI_Zero);
				m_volumeNumverDisplayManager[i].push_back(enVolumeNumberDisplayUI_Zero);

				m_volumeNumverDisplayManager[i].reserve(6);
			}

			return true;
		}

		/*更新処理。*/
		void Option::Update()
		{
			/*選択。*/
			UpdateSelect();

			if (m_currentSelect != enSelect_OK)
			{
				/*ゲージと円。*/
				UpdateGaugeAndCircle((EnGaugeUI)m_currentSelect, (EnCircleUI)m_currentSelect);
			}

			/*UIアニメーション。*/
			UpdateUIAnimation();

			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void Option::Render(RenderContext& rc)
		{
			/*UIを描画しない状態なら処理しない。*/
			if (!IsDrawingUI())
			{
				return;
			}

			/*背景。*/
			m_backGround.Draw(rc);

			for (int i = 0; i < enBarUI_Num; i++)
			{
				/*バーUI。*/
				m_barUI[i].Draw(rc);
			}

			for (int j = 0; j < enGaugeUI_Num; j++)
			{
				/*ゲージUI。*/
				m_gaugeUI[j].Draw(rc);

				for (int k = 0; k < m_volumeNumverDisplayManager[j].size(); k++)
				{
					/*音量の数字UI。*/
					m_volumeNumberUI[j][k][m_volumeNumverDisplayManager[j][k]].Draw(rc);
				}
			}

			if (m_currentSelect != enSelect_OK)
			{
				/*円選択UI。*/
				m_circleSelectUI.Draw(rc);
			}

			for (int l = 0; l < enCircleUI_Num; l++)
			{
				/*円UI。*/
				m_circleUI[l].Draw(rc);
			}

			if (m_currentSelect == enSelect_OK)
			{
				/*「OK」テキスト選択UI。*/
				m_okTextSelectUI.Draw(rc);
			}

			for (int m = 0; m < enOptionTextUI_Num; m++)
			{
				/*選択肢テキストUI。*/
				m_optionTextUI[m].Draw(rc);
			}
		}

		/*スプライトの初期化。*/
		void Option::InitSprite()
		{
			/*背景。*/
			InitBackGround();

			for (int i = 0; i < enBarUI_Num; i++)
			{
				/*バーUI。*/
				InitBarUI((EnBarUI)i);
			}

			for (int j = 0; j < enGaugeUI_Num; j++)
			{
				/*ゲージUI。*/
				InitGaugeUI((EnGaugeUI)j);

				for (int k = 0; k < enVolume_Num; k++)
				{
					for (int l = 0; l < enVolumeNumberDisplayUI_Num; l++)
					{
						/*音量の数字UI。*/
						InitVolumeNumberUI((EnGaugeUI)j, (EnVolume)k, (EnVolumeNumberDisplayUI)l, k);
					}
				}
			}

			/*円選択UI。*/
			InitCircleSelectUI();

			for (int m = 0; m < enCircleUI_Num; m++)
			{
				/*円UI。*/
				InitCircleUI((EnCircleUI)m);
			}

			/*「OK」テキスト選択UI。*/
			InitOkTextSelectUI();

			for (int n = 0; n < enOptionTextUI_Num; n++)
			{
				/*選択肢テキストUI。*/
				InitOptionTextUI((EnOptionTextUI)n);
			}
		}

		/*背景の初期化。*/
		void Option::InitBackGround()
		{
			m_backGround.Init(m_backGroundFilePath.c_str(), BACK_GROUND_WIDTH, BACK_GROUND_HEIGHT);
		}

		/*「OK」テキスト選択UIの初期化。*/
		void Option::InitOkTextSelectUI()
		{
			m_okTextSelectUI.Init(m_okTextSelectUIFilePath.c_str(), OK_TEXT_SELECT_UI_WIDTH, OK_TEXT_SELECT_UI_HEIGHT);/*初期化。*/
			m_okTextSelectUI.SetPosition(OK_TEXT_SELECT_UI_INIT_POSITION);/*位置設定。*/
			m_okTextSelectUI.SetScale(OK_TEXT_SELECT_UI_INIT_SCALE);/*大きさ設定。*/
			m_okTextSelectUI.Update();/*更新処理。*/
		}

		/*テキストUIの初期化。*/
		void Option::InitOptionTextUI(EnOptionTextUI optionTextUI)
		{
			m_optionTextUI[optionTextUI].Init(m_optionTextUIFilePath[optionTextUI].c_str(), OPTION_TEXT_UI_WIDTH[optionTextUI], OPTION_TEXT_UI_HEIGHT[optionTextUI]);/*初期化。*/
			m_optionTextUI[optionTextUI].SetPosition(OPTION_TEXT_UI_INIT_POSITION[optionTextUI]);/*位置設定。*/
			m_optionTextUI[optionTextUI].SetScale(OPTION_TEXT_UI_INIT_SCALE[optionTextUI]);/*大きさ設定。*/
			m_optionTextUI[optionTextUI].Update();/*更新処理。*/
		}

		/*バーUIの初期化。*/
		void Option::InitBarUI(EnBarUI barUI)
		{
			m_barUI[barUI].Init(m_barUIFilePath[barUI].c_str(), BAR_UI_WIDTH, BAR_UI_HEIGHT);/*初期化。*/
			m_barUI[barUI].SetPosition(BAR_UI_INIT_POSITION[barUI]);/*位置設定。*/
			m_barUI[barUI].SetScale(BAR_UI_INIT_SCALE);/*大きさ設定。*/
			m_barUI[barUI].Update();/*更新処理。*/
		}

		/*ゲージUIの初期化。*/
		void Option::InitGaugeUI(EnGaugeUI gaugeUI)
		{
			m_gaugeUI[gaugeUI].Init(m_gaugeUIFilePath[gaugeUI].c_str(), GAUGE_UI_WIDTH, GAUGE_UI_HEIGHT);/*初期化。*/
			m_gaugeUI[gaugeUI].SetPosition(GAUGE_UI_INIT_POSITION[gaugeUI]);/*位置設定。*/
			m_gaugeUI[gaugeUI].SetScale(GAUGE_UI_INIT_SCALE);/*大きさ設定。*/
			m_gaugeUI[gaugeUI].SetPivot(GAUGE_UI_INIT_PIVOT);/*ピボット設定。*/
			m_gaugeUI[gaugeUI].Update();/*更新処理。*/

			m_gaugeUIBaseScale[gaugeUI] = m_gaugeUI[gaugeUI].GetScale();/*基準の大きさ設定。*/
		}

		/*音量の数字UIの初期化。*/
		void Option::InitVolumeNumberUI(EnGaugeUI gaugeUI, EnVolume volume, EnVolumeNumberDisplayUI volumeNumberDisplaUI, int volumeIndex)
		{
			Vector3 initPosition = VOLUME_NUMBER_UI_INIT_POSITION[gaugeUI];/*初期位置。*/
			initPosition.x += VOLUME_NUMBER_UI_POSITION_INTERVAL * volumeIndex;/*位置の間隔を加算。*/

			m_volumeNumberUI[gaugeUI][volume][volumeNumberDisplaUI].Init(m_volumeNumberUIFilePath[volumeNumberDisplaUI].c_str(),VOLUME_NUMBER_UI_WIDTH,VOLUME_NUMBER_UI_HEIGHT);/*初期化。*/
			m_volumeNumberUI[gaugeUI][volume][volumeNumberDisplaUI].SetPosition(initPosition);/*位置設定。*/
			m_volumeNumberUI[gaugeUI][volume][volumeNumberDisplaUI].SetScale(VOLUME_NUMBER_UI_INIT_SCALE);/*大きさ設定。*/
			m_volumeNumberUI[gaugeUI][volume][volumeNumberDisplaUI].Update();/*更新処理。*/
		}

		/*円選択UIの初期化。*/
		void Option::InitCircleSelectUI()
		{
			m_circleSelectUI.Init(m_circleSelectUIFilePath.c_str(), CIRCLE_SELECT_UI_WIDTH, CIRCLE_SELECT_UI_HEIGHT);/*初期化。*/
			m_circleSelectUI.SetPosition(CIRCLE_SELECT_UI_INIT_POSITION);/*位置設定。*/
			m_circleSelectUI.SetScale(CIRCLE_SELECT_UI_INIT_SCALE);/*大きさ設定。*/
			m_circleSelectUI.Update();/*更新処理。*/
		}

		/*円UIの初期化。*/
		void Option::InitCircleUI(EnCircleUI circleUI)
		{
			m_circleUI[circleUI].Init(m_circleUIFilePath[circleUI].c_str(), CIRCLE_UI_WIDTH, CIRCLE_UI_HEIGHT);/*初期化。*/
			m_circleUI[circleUI].SetPosition(CIRCLE_UI_INIT_POSITION[circleUI]);/*位置設定。*/
			m_circleUI[circleUI].SetScale(CIRCLE_UI_INIT_SCALE);/*大きさ設定。*/
			m_circleUI[circleUI].Update();/*更新処理。*/

			m_circleUIBasePosition[circleUI] = m_circleUI[circleUI].GetPosition();/*基準の位置設定。*/
		}

		/*UIアニメーションの初期化。*/
		void Option::InitUIAnimation()
		{
			/*UIの透明度を変えるアニメーション。*/
			InitAlphaUIAnimation();

			/*選択したときの演出UIアニメーション。*/
			InitSelectDirectionUIAnimation();
		}

		/*UIの透明度を変えるアニメーションの初期化。*/
		void Option::InitAlphaUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = m_circleSelectUI.GetMulColor().a;/*元の透明度。*/
			float targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_Circle] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_circleSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				true,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_okTextSelectUI.GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_Ok] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_okTextSelectUI,/*アニメーションをさせるスプライト。*/
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
		void Option::InitSelectDirectionUIAnimation()
		{
			/*選択したときの演出UIアニメーションの値の設定。*/
			Vector3 basePosition = m_optionTextUI[enOptionTextUI_OK].GetPosition();/*元の位置。*/
			Vector3 targetPosition = m_optionTextUI[enOptionTextUI_OK].GetPosition();/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_OKTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_OK],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_okTextSelectUI.GetPosition();/*元の位置。*/
			targetPosition = m_okTextSelectUI.GetPosition();/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_SelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_okTextSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_optionTextUI[enOptionTextUI_OK].GetPosition();/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = m_optionTextUI[enOptionTextUI_OK].GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_OKTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_optionTextUI[enOptionTextUI_OK],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = m_okTextSelectUI.GetPosition();/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = m_okTextSelectUI.GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_SelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_okTextSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);
		}

		/*UIの透明度を変えるアニメーションのリセット処理。*/
		void Option::ResetAlphaUIAnimation()
		{
			for (int i = 0; i < enAlphaUIAnimationSprite_Num; i++)
			{
				m_alphaUIAnimation[i]->Reset();
			}
		}

		/*選択したときの演出UIアニメーションのリセット処理。*/
		void Option::ResetSelectDirectionUIAnimation()
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
		void Option::UpdateSelect()
		{
			/*上を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonUp))
			{
				ResetAlphaUIAnimation();
				/*現在の選択が「マスターボリューム」を選択していたら。*/
				if (m_currentSelect == enSelect_MasterVolume) { m_currentSelect = enSelect_OK; return; }

				/*上を選択する。*/
				m_currentSelect--;
				return;
			}

			/*下を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonDown))
			{
				ResetAlphaUIAnimation();
				/*現在の選択が「OK」を選択していたら。*/
				if (m_currentSelect == enSelect_OK) { m_currentSelect = enSelect_MasterVolume; return; }

				/*下を選択する。*/
				m_currentSelect++;
				return;
			}

			/*現在の選択が「OK」以外の場合。*/
			if (m_currentSelect != enSelect_OK)
			{
				/*音量を下げるようにする。*/
				if (g_pad[0]->IsPress(enButtonLeft))
				{
					m_isGaugeDown = true;
					return;
				}

				/*音量を上げるようにする。*/
				if (g_pad[0]->IsPress(enButtonRight))
				{
					m_isGaugeUp = true;
					return;
				}
			}

			/*現在の選択が「OK」の場合。*/
			if (m_currentSelect == enSelect_OK)
			{
				/*Aボタンを押したら選択したときの演出を流すようにする。。*/
				if (g_pad[0]->IsTrigger(enButtonA))
				{
					EnableDirection();
				}
			}
		}

		/*ゲージと円の更新処理。*/
		void Option::UpdateGaugeAndCircle(EnGaugeUI gaugeUI, EnCircleUI circleUI)
		{
			if (m_isGaugeDown)
			{
				m_volumeChangeValue = 1;
				m_volumeRate[gaugeUI] -= m_volumeChangeValue;/*音量の割合を下げる。*/

				if (m_volumeRate[gaugeUI] <= 0)
				{
					m_volumeRate[gaugeUI] = 0;/*HPの割合が0以下にならないようにする。*/
				}
				m_isGaugeDown = false;
			}
			else if (m_isGaugeUp)
			{
				m_volumeChangeValue = 1;
				m_volumeRate[gaugeUI] += m_volumeChangeValue;/*音量の割合を上げる。。*/

				if (m_volumeRate[gaugeUI] >= 100)
				{
					m_volumeRate[gaugeUI] = 100;/*HPの割合が100以上にならないようにする。*/
				}
				m_isGaugeUp = false;
			}

			if (m_volumeChangeValue <= 0) { return; }/*音量の値に変化がなければ処理しない。*/

			/*ゲージUI。*/
			float volumeRate = (float)m_volumeRate[gaugeUI] / (float)m_volumeMaxRate[gaugeUI];/*音量の割合を計算。*/
			Vector3 gaugeScale = m_gaugeUIBaseScale[gaugeUI];/*ゲージUIの大きさを取得。*/
			gaugeScale.x *= volumeRate;/*音量の割合を乗算。*/
			m_gaugeUI[gaugeUI].SetScale(gaugeScale);/*ゲージUIの大きさを設定。*/
			m_volumeChangeValue = 0;/*音量の変化量をリセット。*/
			CalcVolumeNumverDisplayData(gaugeUI, volumeRate);/*音量の数字UIに表示するためのデータの計算。*/

			/*円UI。*/
			float maxLength = CIRCLE_UI_INIT_POSITION[circleUI].x - CIRCLE_UI_POSITION_MIN[circleUI].x;/*円UIの移動できる距離の最大値。*/
			float moveLength = maxLength * (1.0f - volumeRate);/*円UIの移動距離。*/
			Vector3 circlePosition = m_circleUIBasePosition[circleUI];/*円UIの位置を取得。*/
			circlePosition.x = CIRCLE_UI_INIT_POSITION[gaugeUI].x - moveLength;/*円UIの位置を設定。*/
			m_circleUI[circleUI].SetPosition(circlePosition);/*円UIの位置を設定。*/
		}
		
		/*音量の数字UIに表示するためのデータの計算。*/
		void Option::CalcVolumeNumverDisplayData(EnGaugeUI gaugeUI, float volumeRate)
		{
			m_volumeNumverDisplayManager[gaugeUI].clear();/*音量の数字UIのデータを管理するクラスをリセット。*/
			volumeRate *= 100.0f;/*音量の割合を100倍する。*/

			/*百の位を算出するための計算。*/
			int hundred = (int)volumeRate / 100;

			/*十の位を算出するための計算。*/
			int ten = ((int)volumeRate % 100) / 10;

			/*一の位を算出するための計算。*/
			int one = (int)volumeRate % 10;

			/*音量の数字UIのデータの格納。*/
			m_volumeNumverDisplayManager[gaugeUI].push_back((EnVolumeNumberDisplayUI)hundred);
			m_volumeNumverDisplayManager[gaugeUI].push_back((EnVolumeNumberDisplayUI)ten);
			m_volumeNumverDisplayManager[gaugeUI].push_back((EnVolumeNumberDisplayUI)one);
		}

		/*UIアニメーションの更新処理。*/
		void Option::UpdateUIAnimation()
		{
			/*演出中なら。*/
			if (IsDirection())
			{
				/*選択したときの演出UIアニメーション(位置を下降する)が終わっていなければ再生し続ける。*/
				if (!m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_OKTextUI]->IsEnd())
				{
					m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_OKTextUI]->Update();
					m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_SelectUI]->Update();
				}
				/*選択したときの演出UIアニメーション(位置を上昇する)が終わっていなければ再生し続ける。*/
				else if (!m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_OKTextUI]->IsEnd())
				{
					m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_OKTextUI]->Update();
					m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_SelectUI]->Update();
				}
				/*選択したときの演出UIアニメーションが終わっていたら設定画面を閉じる。*/
				else
				{
					ResetSelectDirectionUIAnimation();
					EnableSelect();
				}
				return;
			}

			/*選択したUIの形状の設定。*/
			int currentSelectUIShape = enSelectUIShape_Num;
			/*現在の選択の内容からUIの形状を分岐する。*/
			m_currentSelect != enSelect_OK ?
				currentSelectUIShape = enSelectUIShape_Circle:
				currentSelectUIShape = enSelectUIShape_Bar;

			m_alphaUIAnimation[currentSelectUIShape]->Update();
		}

		/*スプライトの更新処理。*/
		void Option::UpdateSprite()
		{
			/*背景。*/
			m_backGround.Update();

			for (int i = 0; i < enBarUI_Num; i++)
			{
				/*バーUI。*/
				m_barUI[i].Update();
			}

			for (int j = 0; j < enGaugeUI_Num; j++)
			{
				/*ゲージUI。*/
				m_gaugeUI[j].Update();

				for (int k = 0; k < enVolume_Num; k++)
				{
					for (int l = 0; l < enVolumeNumberDisplayUI_Num; l++)
					{
						/*音量の数字UI。*/
						m_volumeNumberUI[j][k][l].Update();
					}
				}
			}

			/*円選択UI。*/
			if (m_currentSelect != enSelect_OK)
			{
				m_circleSelectUI.SetPosition(m_circleUI[m_currentSelect].GetPosition());
			}
			m_circleSelectUI.Update();

			for (int m = 0; m < enCircleUI_Num; m++)
			{
				/*円UI。*/
				m_circleUI[m].Update();
			}

			/*「OK」テキスト選択UI。*/
			m_okTextSelectUI.Update();

			for (int n = 0; n < enOptionTextUI_Num; n++)
			{
				/*選択肢テキストUI。*/
				m_optionTextUI[n].Update();
			}
		}
	}
}