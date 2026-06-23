#include "stdafx.h"
#include "RoleSelect.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Sound/UIInput.h"

namespace
{
	/*白枠UI。*/
	const float WHITE_FRAME_UI_WIDTH = 702; /*白枠UIの幅。*/

	const float WHITE_FRAME_UI_HEIGHT = 803; /*白枠UIの高さ。*/

	const Vector3 WHITE_FRAME_UI_INIT_POSITION = {0.0f, 0.0f, 0.0f}; /*白枠UIの初期位置。*/

	const Vector3 WHITE_FRAME_UI_INIT_SCALE = {0.5f, 0.5f, 1.0f}; /*白枠UIの初期大きさ。*/

	/*役割UI。*/
	const float ROLE_UI_WIDTH = 570; /*役割UIの幅。*/

	const float ROLE_UI_HEIGHT = 672; /*役割UIの高さ。*/

	const Vector3 ROLE_UI_INIT_POSITION = {0.0f, 0.0f, 0.0f}; /*役割UIの初期位置。*/

	const Vector3 ROLE_UI_INIT_SCALE = {0.5f, 0.5f, 1.0f}; /*役割UIの初期大きさ。*/

	/*矢印UI。*/
	const float ARROW_UI_WIDTH = 1024; /*矢印UIの幅。*/

	const float ARROW_UI_HEIGHT = 1024; /*矢印UIの高さ。*/

	const Vector3 ARROW_UI_INIT_POSITION[nsApp::nsSelect::RoleSelect::EnArrowUI::enArrowUI_Num] = {
		Vector3{-190.0f, 0.0f, 0.0f}, /*左。*/
		Vector3{190.0f, 0.0f, 0.0f},  /*右。*/
	}; /*役割UIの初期位置。*/

	const Vector3 ARROW_UI_INIT_SCALE = {0.25f, 0.25f, 1.0f}; /*矢印UIの初期大きさ。*/

	/*UIアニメーション。*/
	const Vector2 AFTER_SCALE_UP_ARROW_UI_ANIMATION_SCALE = {0.3f, 0.3f}; /*矢印UIの大きさを大きくするアニメーション後の大きさ。*/

	const Vector3 AFTER_COLOR_ARROW_UI_ANIMATION_COLOR = {2.0f, 2.0f, 2.0f}; /*矢印UIの色を変えるアニメーション後の色。*/

	const float DOWN_POSITION_OFFSET = 2.5f; /*選択したときの演出UIアニメーション時に下降する位置のオフセット。*/

	const float SCALE_UI_ANIMATION_PLAY_SPEED = 4.0f; /*UIの大きさを変えるアニメーションの再生速度。*/

	const float COLOR_UI_ANIMATION_PLAY_SPEED = 4.0f; /*UIの色を変えるアニメーションの再生速度。*/

	const float SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED = 7.0f; /*選択したときの演出UIアニメーションの再生速度。*/
}

namespace nsApp
{
	namespace nsSelect
	{
		/*開始処理。*/
		bool RoleSelect::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			auto *SoundManager = FindGO<nsSound::SoundLister>("SoundManager");
			if (SoundManager == nullptr)
			{
				SoundManager = NewGO<nsSound::SoundLister>(0, "SoundManager");
				SoundManager->InitSound();
			}

			return true;
		}

		/*更新処理。*/
		void RoleSelect::Update()
		{
			/*演出していないとき選択する。*/
			if (!IsSlideDirection() && !IsSelectDirection())
			{
				/*選択。*/
				UpdateSelect();

				Vector3 basePosition[enSelectDirectionUIAnimationSprite_Num] = {
					m_whiteFrameUI.GetPosition(),
					m_roleUI[enRole_Sword].GetPosition(),
					m_roleUI[enRole_Hammer].GetPosition(),
					m_roleUI[enRole_Mage].GetPosition(),
					m_roleUI[enRole_Gunner].GetPosition(),
					m_arrowUI[enArrowUI_Left].GetPosition(),
					m_arrowUI[enArrowUI_Right].GetPosition()};

				Vector3 targetPosition[enSelectDirectionUIAnimationSprite_Num] = {
					m_whiteFrameUI.GetPosition(),
					m_roleUI[enRole_Sword].GetPosition(),
					m_roleUI[enRole_Hammer].GetPosition(),
					m_roleUI[enRole_Mage].GetPosition(),
					m_roleUI[enRole_Gunner].GetPosition(),
					m_arrowUI[enArrowUI_Left].GetPosition(),
					m_arrowUI[enArrowUI_Right].GetPosition()};

				for (int i = 0; i < enSelectDirectionUIAnimationSprite_Num; i++)
				{
					m_selectDirectionUIAnimationBasePosition[i] = basePosition[i];
					m_selectDirectionUIAnimationTargetPosition[i] = targetPosition[i];
				}
			}
			else
			{
				/*UIアニメーション。*/
				UpdateUIAnimation();
			}

			/*演出していないとき更新処理する。*/
			if (!IsSelectDirection())
			{
				/*スプライト。*/
				UpdateSprite();
			}
		}

		/*描画処理。*/
		void RoleSelect::Render(RenderContext &rc)
		{
			/*UIを描画しない状態なら処理しない。*/
			if (!IsDrawingUI())
			{
				return;
			}

			/*白枠UIの描画。*/
			m_whiteFrameUI.Draw(rc);

			/*役割UIの描画。*/
			m_roleUI[m_displayRoleUI].Draw(rc);

			for (int j = 0; j < enArrowUI_Num; j++)
			{
				/*矢印UIの描画。*/
				m_arrowUI[j].Draw(rc);
			}
		}

		/*スプライトの初期化。*/
		void RoleSelect::InitSprite()
		{
			/*白枠UI。*/
			InitWhiteFrameUI();

			for (int i = 0; i < enRole_Num; i++)
			{
				/*役割UI。*/
				InitRoleUI((EnRole)i);
			}

			for (int j = 0; j < enArrowUI_Num; j++)
			{
				/*矢印UI。*/
				InitArrowUI((EnArrowUI)j);
			}
		}

		/*白枠UIの初期化。*/
		void RoleSelect::InitWhiteFrameUI()
		{
			m_whiteFrameUI.Init(m_whiteFrameUIFilePath.c_str(), WHITE_FRAME_UI_WIDTH, WHITE_FRAME_UI_HEIGHT); /*初期化。*/
			m_whiteFrameUI.SetPosition(WHITE_FRAME_UI_INIT_POSITION);										  /*位置設定。*/
			m_whiteFrameUI.SetScale(WHITE_FRAME_UI_INIT_SCALE);												  /*大きさ設定。*/
			m_whiteFrameUI.Update();																		  /*更新処理。*/
		}

		/*役割UIの初期化。*/
		void RoleSelect::InitRoleUI(EnRole role)
		{
			m_roleUI[role].Init(m_roleUIFilePath[role].c_str(), ROLE_UI_WIDTH, ROLE_UI_HEIGHT); /*初期化。*/
			m_roleUI[role].SetPosition(ROLE_UI_INIT_POSITION);									/*位置設定。*/
			m_roleUI[role].SetScale(ROLE_UI_INIT_SCALE);										/*大きさ設定。*/
			m_roleUI[role].Update();															/*更新処理。*/
		}

		/*矢印UIの初期化。*/
		void RoleSelect::InitArrowUI(EnArrowUI arrowUI)
		{
			m_arrowUI[arrowUI].Init(m_arrowUIFilePath[arrowUI].c_str(), ARROW_UI_WIDTH, ARROW_UI_HEIGHT); /*初期化。*/
			m_arrowUI[arrowUI].SetPosition(ARROW_UI_INIT_POSITION[arrowUI]);							  /*位置設定。*/
			m_arrowUI[arrowUI].SetScale(ARROW_UI_INIT_SCALE);											  /*大きさ設定。*/
			m_arrowUI[arrowUI].Update();																  /*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void RoleSelect::InitUIAnimation()
		{
			/*役割UIの大きさを変えるアニメーション。*/
			InitScaleRoleUIAnimation();

			/*矢印UIの大きさを変えるアニメーション。*/
			InitScaleArrowUIAnimation();

			/*矢印UIの色を変えるアニメーション。*/
			InitColorArrowUIAnimation();

			/*選択したときの演出UIアニメーション。*/
			InitSelectDirectionUIAnimation();
		}

		/*役割UIの大きさを変えるアニメーションの初期化。*/
		void RoleSelect::InitScaleRoleUIAnimation()
		{
			/*役割UIの大きさを変えるアニメーションの値の設定。*/
			Vector2 baseScale = {m_roleUI[enRole_Sword].GetScale().x, m_roleUI[enRole_Sword].GetScale().y}; /*元の大きさ。*/
			Vector2 targetScale = {0.0f, m_roleUI[enRole_Sword].GetScale().y};								/*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleRoleUIAnimation[enScale_Down][enRole_Sword] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_roleUI[enRole_Sword],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*役割UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = {m_roleUI[enRole_Hammer].GetScale().x, m_roleUI[enRole_Hammer].GetScale().y}; /*元の大きさ。*/
			targetScale = {0.0f, m_roleUI[enRole_Hammer].GetScale().y};								  /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleRoleUIAnimation[enScale_Down][enRole_Hammer] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_roleUI[enRole_Hammer],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*役割UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = {m_roleUI[enRole_Mage].GetScale().x, m_roleUI[enRole_Mage].GetScale().y}; /*元の大きさ。*/
			targetScale = {0.0f, m_roleUI[enRole_Mage].GetScale().y};							  /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleRoleUIAnimation[enScale_Down][enRole_Mage] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_roleUI[enRole_Mage],		   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*役割UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = {m_roleUI[enRole_Gunner].GetScale().x, m_roleUI[enRole_Gunner].GetScale().y}; /*元の大きさ。*/
			targetScale = {0.0f, m_roleUI[enRole_Gunner].GetScale().y};								  /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleRoleUIAnimation[enScale_Down][enRole_Gunner] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_roleUI[enRole_Gunner],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*役割UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = {0.0f, m_roleUI[enRole_Sword].GetScale().y};								  /*元の大きさ。*/
			targetScale = {m_roleUI[enRole_Sword].GetScale().x, m_roleUI[enRole_Sword].GetScale().y}; /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleRoleUIAnimation[enScale_Up][enRole_Sword] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_roleUI[enRole_Sword],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*役割UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = {0.0f, m_roleUI[enRole_Hammer].GetScale().y};									/*元の大きさ。*/
			targetScale = {m_roleUI[enRole_Hammer].GetScale().x, m_roleUI[enRole_Hammer].GetScale().y}; /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleRoleUIAnimation[enScale_Up][enRole_Hammer] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_roleUI[enRole_Hammer],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*役割UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = {0.0f, m_roleUI[enRole_Mage].GetScale().y};									/*元の大きさ。*/
			targetScale = {m_roleUI[enRole_Mage].GetScale().x, m_roleUI[enRole_Mage].GetScale().y}; /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleRoleUIAnimation[enScale_Up][enRole_Mage] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_roleUI[enRole_Mage],		   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*役割UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = {0.0f, m_roleUI[enRole_Gunner].GetScale().y};									/*元の大きさ。*/
			targetScale = {m_roleUI[enRole_Gunner].GetScale().x, m_roleUI[enRole_Gunner].GetScale().y}; /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleRoleUIAnimation[enScale_Up][enRole_Gunner] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_roleUI[enRole_Gunner],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);
		}

		/*矢印UIの大きさを変えるアニメーションの初期化。*/
		void RoleSelect::InitScaleArrowUIAnimation()
		{
			/*矢印UIの大きさを変えるアニメーションの値の設定。*/
			Vector2 baseScale = {m_arrowUI[enArrowUI_Left].GetScale().x, m_arrowUI[enArrowUI_Left].GetScale().y}; /*元の大きさ。*/
			Vector2 targetScale = AFTER_SCALE_UP_ARROW_UI_ANIMATION_SCALE;										  /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleArrowUIAnimation[enScale_Down][enArrowUI_Left] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_arrowUI[enArrowUI_Left],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*矢印UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = {m_arrowUI[enArrowUI_Right].GetScale().x, m_arrowUI[enArrowUI_Right].GetScale().y}; /*元の大きさ。*/
			targetScale = AFTER_SCALE_UP_ARROW_UI_ANIMATION_SCALE;											/*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleArrowUIAnimation[enScale_Down][enArrowUI_Right] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_arrowUI[enArrowUI_Right],   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*矢印UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = AFTER_SCALE_UP_ARROW_UI_ANIMATION_SCALE;											/*元の大きさ。*/
			targetScale = {m_arrowUI[enArrowUI_Left].GetScale().x, m_arrowUI[enArrowUI_Left].GetScale().y}; /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleArrowUIAnimation[enScale_Up][enArrowUI_Left] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_arrowUI[enArrowUI_Left],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);

			/*矢印UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = AFTER_SCALE_UP_ARROW_UI_ANIMATION_SCALE;											  /*元の大きさ。*/
			targetScale = {m_arrowUI[enArrowUI_Right].GetScale().x, m_arrowUI[enArrowUI_Right].GetScale().y}; /*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleArrowUIAnimation[enScale_Up][enArrowUI_Right] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_arrowUI[enArrowUI_Right],   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				SCALE_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseScale,					   /*元の大きさ。*/
				targetScale					   /*ターゲットの大きさ。*/
			);
		}

		/*矢印UIの色を変えるアニメーションの初期化。*/
		void RoleSelect::InitColorArrowUIAnimation()
		{
			/*矢印UIの色を変えるアニメーションの値の設定。*/
			Vector3 baseColor = {
				m_arrowUI[enArrowUI_Left].GetMulColor().x,
				m_arrowUI[enArrowUI_Left].GetMulColor().y,
				m_arrowUI[enArrowUI_Left].GetMulColor().z};				/*元の色。*/
			Vector3 targetColor = AFTER_COLOR_ARROW_UI_ANIMATION_COLOR; /*ターゲットの色。*/

			/*初期化。*/
			m_colorArrowUIAnimation[enChangeColor_Before][enArrowUI_Left] = std::make_unique<nsApp::nsUI::ColorUIAnimation>(
				&m_arrowUI[enArrowUI_Left],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				COLOR_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseColor,					   /*元の色。*/
				targetColor					   /*ターゲットの色。*/
			);

			/*矢印UIの色を変えるアニメーションの値の設定。*/
			baseColor = {
				m_arrowUI[enArrowUI_Right].GetMulColor().x,
				m_arrowUI[enArrowUI_Right].GetMulColor().y,
				m_arrowUI[enArrowUI_Right].GetMulColor().z};	/*元の色。*/
			targetColor = AFTER_COLOR_ARROW_UI_ANIMATION_COLOR; /*ターゲットの色。*/

			/*初期化。*/
			m_colorArrowUIAnimation[enChangeColor_Before][enArrowUI_Right] = std::make_unique<nsApp::nsUI::ColorUIAnimation>(
				&m_arrowUI[enArrowUI_Right],   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				COLOR_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseColor,					   /*元の色。*/
				targetColor					   /*ターゲットの色。*/
			);

			/*矢印UIの色を変えるアニメーションの値の設定。*/
			baseColor = AFTER_COLOR_ARROW_UI_ANIMATION_COLOR; /*元の色。*/
			targetColor = {
				m_arrowUI[enArrowUI_Left].GetMulColor().x,
				m_arrowUI[enArrowUI_Left].GetMulColor().y,
				m_arrowUI[enArrowUI_Left].GetMulColor().z}; /*ターゲットの色。*/

			/*初期化。*/
			m_colorArrowUIAnimation[enChangeColor_After][enArrowUI_Left] = std::make_unique<nsApp::nsUI::ColorUIAnimation>(
				&m_arrowUI[enArrowUI_Left],	   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				COLOR_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseColor,					   /*元の色。*/
				targetColor					   /*ターゲットの色。*/
			);

			/*矢印UIの色を変えるアニメーションの値の設定。*/
			baseColor = AFTER_COLOR_ARROW_UI_ANIMATION_COLOR; /*元の色。*/
			targetColor = {
				m_arrowUI[enArrowUI_Right].GetMulColor().x,
				m_arrowUI[enArrowUI_Right].GetMulColor().y,
				m_arrowUI[enArrowUI_Right].GetMulColor().z}; /*ターゲットの色。*/

			/*初期化。*/
			m_colorArrowUIAnimation[enChangeColor_After][enArrowUI_Right] = std::make_unique<nsApp::nsUI::ColorUIAnimation>(
				&m_arrowUI[enArrowUI_Right],   /*アニメーションをさせるスプライト。*/
				1.0f,						   /*ターゲットの割合。*/
				COLOR_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,						   /*ループするか？*/
				0.0f,						   /*アニメーションを開始する前の遅延時間。*/
				0.0f,						   /*アニメーションを終了した後の遅延時間。*/
				baseColor,					   /*元の色。*/
				targetColor					   /*ターゲットの色。*/
			);
		}

		/*選択したときの演出UIアニメーションの初期化。*/
		void RoleSelect::InitSelectDirectionUIAnimation()
		{
			/*このアニメーションに設定している元とターゲットの位置は
			 *キャラクター枠の位置によって変わるためVector3::Zeroに設定している。*/

			/*選択したときの演出UIアニメーションの値の設定。*/
			Vector3 basePosition = Vector3::Zero;	/*元の位置。*/
			Vector3 targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_WhiteFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_whiteFrameUI,						  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_SwordRoleUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_roleUI[enRole_Sword],				  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_HammerRoleUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_roleUI[enRole_Hammer],				  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_MageRoleUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_roleUI[enRole_Mage],					  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_GunnerRoleUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_roleUI[enRole_Gunner],				  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_LeftArrowUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_arrowUI[enArrowUI_Left],				  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_RightArrowUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_arrowUI[enArrowUI_Right],			  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_WhiteFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_whiteFrameUI,						  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_SwordRoleUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_roleUI[enRole_Sword],				  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_HammerRoleUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_roleUI[enRole_Hammer],				  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_MageRoleUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_roleUI[enRole_Mage],					  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_GunnerRoleUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_roleUI[enRole_Gunner],				  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_LeftArrowUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_arrowUI[enArrowUI_Left],				  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = Vector3::Zero;	/*元の位置。*/
			targetPosition = Vector3::Zero; /*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_RightArrowUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_arrowUI[enArrowUI_Right],			  /*アニメーションをさせるスプライト。*/
				1.0f,									  /*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED, /*アニメーションの再生速度。*/
				false,									  /*ループするか？*/
				0.0f,									  /*アニメーションを開始する前の遅延時間。*/
				0.0f,									  /*アニメーションを終了した後の遅延時間。*/
				basePosition,							  /*元の位置。*/
				targetPosition							  /*ターゲットの位置。*/
			);
		}

		/*役割UIの大きさを変えるアニメーションのリセット処理。*/
		void RoleSelect::ResetScaleRoleUIAnimation()
		{
			for (int i = 0; i < enScale_Num; i++)
			{
				for (int j = 0; j < enRole_Num; j++)
				{
					m_scaleRoleUIAnimation[i][j]->Reset();
				}
			}
		}

		/*矢印UIの大きさを変えるアニメーションのリセット処理。*/
		void RoleSelect::ResetScaleArrowUIAnimation()
		{
			for (int i = 0; i < enScale_Num; i++)
			{
				for (int j = 0; j < enArrowUI_Num; j++)
				{
					m_scaleArrowUIAnimation[i][j]->Reset();
				}
			}
		}

		/*矢印UIの色を変えるアニメーションのリセット処理。*/
		void RoleSelect::ResetColorArrowUIAnimation()
		{
			for (int i = 0; i < enChangeColor_Num; i++)
			{
				for (int j = 0; j < enArrowUI_Num; j++)
				{
					m_colorArrowUIAnimation[i][j]->Reset();
				}
			}
		}

		/*選択したときの演出UIアニメーションのリセット処理。*/
		void RoleSelect::ResetSelectDirectionUIAnimation()
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
		void RoleSelect::UpdateSelect()
		{
			auto *SoundManager = FindGO<nsSound::SoundLister>("SoundManager");

			/*左を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonLeft))
			{
				/*カーソル移動音。*/
				if (SoundManager)
				{
					SoundManager->GetSEList().PlaySE(nsSound::SE_ID::Cursor, 1.0f, false, 1.0f);
				}

				EnableSlideDirection();

				m_previousRole = m_currentRole;
				m_inputDirection = enDirection_Left;
				/*現在の選択がソードの役職を選択していたら。*/
				if (m_currentRole == enRole_Sword)
				{
					m_currentRole = enRole_Gunner;
					return;
				}

				/*左隣りの役割を選択する。*/
				m_currentRole--;
				return;
			}

			/*右を入力したら。*/
			if (g_pad[0]->IsTrigger(enButtonRight))
			{
				/*カーソル移動音。*/
				if (SoundManager)
				{
					SoundManager->GetSEList().PlaySE(nsSound::SE_ID::Cursor, 1.0f, false, 1.0f);
				}

				EnableSlideDirection();

				m_previousRole = m_currentRole;
				m_inputDirection = enDirection_Right;
				/*現在の選択がガンナーの役職を選択していたら。*/
				if (m_currentRole == enRole_Gunner)
				{
					m_currentRole = enRole_Sword;
					return;
				}

				/*右隣りの役割を選択する。*/
				m_currentRole++;
				return;
			}

			/*Aボタンを押したら選択したときの演出を流すようにする。*/
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				/*決定音。*/
				if (SoundManager)
				{
					SoundManager->GetSEList().PlaySE(nsSound::SE_ID::Enter, 1.0f, false, 1.0f);
				}

				EnableSelectDirection();
				return;
			}

			/*Bボタンを押したらクエスト選択画面に戻る。*/
			if (g_pad[0]->IsTrigger(enButtonB))
			{
				/*キャンセル音。*/
				if (SoundManager)
				{
					SoundManager->GetSEList().PlaySE(nsSound::SE_ID::Exit, 1.0f, false, 1.0f);
				}

				EnableBackSelect();
			}
		}

		/*UIアニメーションの更新処理。*/
		void RoleSelect::UpdateUIAnimation()
		{
			/*選択したときの演出UIアニメーション中なら。*/
			if (IsSelectDirection())
			{
				/*選択したときの演出UIアニメーション用の元の位置とターゲットの位置の設定。*/
				for (int i = 0; i < enSelectDirectionUIAnimationSprite_Num; i++)
				{
					m_selectDirectionUIAnimationTargetPosition[i].y -= DOWN_POSITION_OFFSET;

					m_selectDirectionUIAnimation[enPosition_Down][i]->SetBasePosition(m_selectDirectionUIAnimationBasePosition[i]);
					m_selectDirectionUIAnimation[enPosition_Down][i]->SetTargetPosition(m_selectDirectionUIAnimationTargetPosition[i]);

					m_selectDirectionUIAnimation[enPosition_Up][i]->SetBasePosition(m_selectDirectionUIAnimationTargetPosition[i]);
					m_selectDirectionUIAnimation[enPosition_Up][i]->SetTargetPosition(m_selectDirectionUIAnimationBasePosition[i]);
				}

				/*選択したときの演出UIアニメーション(位置を下降する)が終わっていなければ再生し続ける。*/
				if (!m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_WhiteFrameUI]->IsEnd())
				{
					for (int j = 0; j < enSelectDirectionUIAnimationSprite_Num; j++)
					{
						m_selectDirectionUIAnimation[enPosition_Down][j]->Update();
					}
				}
				/*選択したときの演出UIアニメーション(位置を上昇する)が終わっていなければ再生し続ける。*/
				else if (!m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_WhiteFrameUI]->IsEnd())
				{
					for (int k = 0; k < enSelectDirectionUIAnimationSprite_Num; k++)
					{
						m_selectDirectionUIAnimation[enPosition_Up][k]->Update();
					}
				}
				/*選択したときの演出UIアニメーションが終わっていたら役割選択画面を閉じる。*/
				else
				{
					ResetSelectDirectionUIAnimation();
					EnableSelect();
				}
			}
			/*UIをスライドさせるアニメーション中なら。*/
			else if (IsSlideDirection())
			{
				/*役割UIの大きさを変えるアニメーション(大きくする)が終わっていなければ再生し続ける。*/
				if (!m_scaleRoleUIAnimation[enScale_Down][m_previousRole]->IsEnd())
				{
					m_scaleRoleUIAnimation[enScale_Down][m_previousRole]->Update();
					m_scaleArrowUIAnimation[enScale_Down][m_inputDirection]->Update();
					m_colorArrowUIAnimation[enChangeColor_Before][m_inputDirection]->Update();
				}
				/*役割UIの大きさを変えるアニメーション(小さくする)が終わっていなければ再生し続ける。*/
				else if (!m_scaleRoleUIAnimation[enScale_Up][m_currentRole]->IsEnd())
				{

					m_displayRoleUI = m_currentRole; /*役割UIの大きさを変えるアニメーション(大きくする)が終わったタイミングで役割UIを切り替える。*/
					m_scaleRoleUIAnimation[enScale_Up][m_currentRole]->Update();
					m_scaleArrowUIAnimation[enScale_Up][m_inputDirection]->Update();
					m_colorArrowUIAnimation[enChangeColor_After][m_inputDirection]->Update();
				}
				/*役割UIの大きさを変えるアニメーションが終わっていたら初期化処理する。*/
				else
				{
					ResetScaleRoleUIAnimation();
					ResetScaleArrowUIAnimation();
					ResetColorArrowUIAnimation();
					DisableSlideDirection();
				}
			}
		}

		/*スプライトの更新処理。*/
		void RoleSelect::UpdateSprite()
		{
			Vector3 initPosition = m_referencePosition;

			/*白枠UI。*/
			m_whiteFrameUI.SetPosition(initPosition);
			m_whiteFrameUI.Update();

			for (int i = 0; i < enRole_Num; i++)
			{
				/*役割UI。*/
				m_roleUI[i].SetPosition(initPosition);
				if (!IsSlideDirection() && m_previousRole != enRole_Num)
				{
					m_roleUI[m_previousRole].SetScale(ROLE_UI_INIT_SCALE);
				}
				m_roleUI[i].Update();
			}

			for (int j = 0; j < enArrowUI_Num; j++)
			{
				/*矢印UI。*/
				initPosition = m_referencePosition + ARROW_UI_INIT_POSITION[j];
				m_arrowUI[j].SetPosition(initPosition);
				m_arrowUI[j].Update();
			}
		}
	}
}