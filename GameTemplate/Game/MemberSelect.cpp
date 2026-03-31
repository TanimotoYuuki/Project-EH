#include "stdafx.h"
#include "MemberSelect.h"
#include "Src/Fade/Fade.h"

namespace {
	/*メンバー選択テキストUI。*/
	const float MEMBER_SELECT_TEXT_UI_WIDTH = 1024;/*メンバー選択テキストUIの幅。*/

	const float MEMBER_SELECT_TEXT_UI_HEIGHT = 256;/*メンバー選択テキストUIの高さ。*/

	const Vector3 MEMBER_SELECT_TEXT_UI_INIT_POSITION = { -700.0f,450.0f,0.0f };/*メンバー選択テキストUIの初期位置。*/

	const Vector3 MEMBER_SELECT_TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*メンバー選択テキストUIの初期大きさ。*/

	/*キャラクター選択UI。*/
	const float CHARACTER_SELECT_UI_WIDTH = 990;/*キャラクター選択UIの幅。*/

	const float CHARACTER_SELECT_UI_HEIGHT = 1080;/*キャラクター選択UIの高さ。*/

	const Vector3 CHARACTER_SELECT_UI_INIT_POSITION = { 1785.0f,50.0f,0.0f };/*キャラクター選択UIの初期位置。*/

	const Vector3 CHARACTER_SELECT_UI_INIT_SCALE = { 0.48f,0.48f,1.0f };/*キャラクター選択UIの初期大きさ。*/

	const Vector4 CHARACTER_SELECT_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.6f };/*キャラクター選択UIの初期乗算色。*/

	/*キャラクター枠UI(1人目のキャラクターをベースに)。*/
	const float CHARACTER_FRAME_UI_WIDTH = 990;/*キャラクター選択UIの幅。*/

	const float CHARACTER_FRAME_UI_HEIGHT = 1080;/*キャラクター選択UIの高さ。*/

	const Vector3 CHARACTER_FRAME_UI_INIT_POSITION = { 1785.0f,50.0f,0.0f };/*キャラクター選択UIの初期位置。*/

	const float CHARACTER_FRAME_UI_POSITION_INTERVAL = 475.0f;/*キャラクター枠UIの位置の間隔。*/

	const Vector3 CHARACTER_FRAME_UI_INIT_SCALE = { 0.45f,0.45f,1.0f };/*キャラクター選択UIの初期大きさ。*/

	/*出撃選択UI。*/
	const float DEPLOY_SELECT_UI_WIDTH = 1024;/*出撃選択UIの幅。*/

	const float DEPLOY_SELECT_UI_HEIGHT = 256;/*出撃選択UIの高さ。*/

	const Vector3 DEPLOY_SELECT_UI_INIT_POSITION = { 2500.0f,-350.0f,0.0f };/*出撃選択UIの初期位置。*/

	const Vector3 DEPLOY_SELECT_UI_INIT_SCALE = { 0.84f,1.03f,1.0f };/*出撃選択UIの初期大きさ。*/

	const Vector4 DEPLOY_SELECT_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.6f };/*出撃テキストUIの初期乗算色。*/

	/*出撃テキストUI。*/
	const float DEPLOY_TEXT_UI_WIDTH = 1024;/*出撃テキストUIの幅。*/

	const float DEPLOY_TEXT_UI_HEIGHT = 256;/*出撃テキストUIの高さ。*/

	const Vector3 DEPLOY_TEXT_UI_INIT_POSITION = { 2500.0f,-350.0f,0.0f };/*出撃テキストUIの初期位置。*/

	const Vector3 DEPLOY_TEXT_UI_INIT_SCALE = { 0.8f,0.8f,1.0f };/*出撃テキストUIの初期大きさ。*/

	/*UIアニメーション。*/
	const float SLIDE_UI_ANIMATION_PLAY_SPEED = 4.0f;/*UIをスライドさせるアニメーションの再生速度。*/

	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 1.2f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const Vector3 AFTER_UI_ANIMATION_POSITION[nsApp::nsSelect::MemberSelect::EnSlide::enSlide_Num][nsApp::nsSelect::MemberSelect::EnSlideUIAnimationSprite::enSlideUIAnimationSprite_Num] = {
		/*左側にスライドした時の位置。*/
		Vector3{-715.0f,50.0f,0.0f},/*キャラクター選択UI。*/
		Vector3{-715.0f,50.0f,0.0f},/*1人目のキャラクター枠UI。*/
		Vector3{-715.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 1,50.0f,0.0f},/*2人目のキャラクター枠UI。*/
		Vector3{-715.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 2,50.0f,0.0f},/*3人目のキャラクター枠UI。*/
		Vector3{-715.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 3,50.0f,0.0f},/*4人目のキャラクター枠UI。*/
		Vector3{0.0f,-350.0f,0.0f},/*出撃選択UI。*/
		Vector3{0.0f,-350.0f,0.0f},/*出撃テキストUI。*/

		/*右側にスライドした時の位置。*/
		Vector3{1785.0f,50.0f,0.0f},/*キャラクター選択UI。*/
		Vector3{1785.0f,50.0f,0.0f},/*1人目のキャラクター枠UI。*/
		Vector3{1785.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 1,50.0f,0.0f},/*2人目のキャラクター枠UI。*/
		Vector3{1785.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 2,50.0f,0.0f},/*3人目のキャラクター枠UI。*/
		Vector3{1785.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 3,50.0f,0.0f},/*4人目のキャラクター枠UI。*/
		Vector3{2500.0f,-350.0f,0.0f},/*出撃選択UI。*/
		Vector3{2500.0f,-350.0f,0.0f},/*出撃テキストUI。*/
	};/*UIをスライドさせるアニメーション後の位置。*/

	const float AFTER_UI_ANIMATION_ALPHA = 0.2f;/*UIの透明度を変えるアニメーション後の透明度。*/
}

namespace nsApp
{
	namespace nsSelect
	{
		/*開始処理。*/
		bool MemberSelect::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			return true;
		}

		/*更新処理。*/
		void MemberSelect::Update()
		{
			/*演出中でなければ。*/
			if (!IsDirection())
			{
				/*選択。*/
				UpdateSelect();
			}
			
			/*演出。*/
			UpdateDirection();

			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void MemberSelect::Render(RenderContext& rc)
		{
			/*メンバー選択テキストUIの描画。*/
			m_memberSelectTextUI.Draw(rc);

			if (m_currentSelect != enSelect_Deploy)
			{
				/*キャラクター選択UIの描画。*/
				m_characterSelectUI.Draw(rc);
			}

			for (int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				/*キャラクター枠UIの描画。*/
				m_characterFrameUI[i].Draw(rc);
			}

			if (m_currentSelect == enSelect_Deploy)
			{
				/*出撃選択UIの描画。*/
				m_deploySelectUI.Draw(rc);
			}

			/*出撃テキストUIの描画。*/
			m_deployTextUI.Draw(rc);
		}

		/*スプライトの初期化。*/
		void MemberSelect::InitSprite()
		{
			/*メンバー選択テキストUI。*/
			InitMemberSelectTextUI();

			/*キャラクター選択UI。*/
			InitCharacterSelectUI();

			for (int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				/*キャラクター枠UI。*/
				InitCharacterFrameUI((EnCharacterFrameUI)i, i);
			}

			/*出撃選択UI。*/
			InitDeploySelectUI();

			/*出撃テキストUI。*/
			InitDeployTextUI();
		}

		/*メンバー選択テキストUIの初期化。*/
		void MemberSelect::InitMemberSelectTextUI()
		{
			m_memberSelectTextUI.Init(m_memberSelectTextUIFilePath.c_str(), MEMBER_SELECT_TEXT_UI_WIDTH, MEMBER_SELECT_TEXT_UI_HEIGHT);/*初期化。*/
			m_memberSelectTextUI.SetPosition(MEMBER_SELECT_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_memberSelectTextUI.SetScale(MEMBER_SELECT_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_memberSelectTextUI.Update();/*更新処理。*/
		}

		/*キャラクター選択UIの初期化。*/
		void MemberSelect::InitCharacterSelectUI()
		{
			m_characterSelectUI.Init(m_characterSelectUIFilePath.c_str(), CHARACTER_SELECT_UI_WIDTH, CHARACTER_SELECT_UI_HEIGHT);/*初期化。*/
			m_characterSelectUI.SetPosition(CHARACTER_SELECT_UI_INIT_POSITION);/*位置設定。*/
			m_characterSelectUI.SetScale(CHARACTER_SELECT_UI_INIT_SCALE);/*大きさ設定。*/
			m_characterSelectUI.SetMulColor(CHARACTER_SELECT_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_characterSelectUI.Update();/*更新処理。*/
		}

		/*キャラクター枠UIの初期化。*/
		void MemberSelect::InitCharacterFrameUI(EnCharacterFrameUI characterFrameUI, int characterIndex)
		{
			Vector3 initPosition = CHARACTER_FRAME_UI_INIT_POSITION;/*初期位置。*/
			initPosition.x += CHARACTER_FRAME_UI_POSITION_INTERVAL * characterIndex;/*位置の間隔を加算。*/

			m_characterFrameUI[characterFrameUI].Init(m_characterFrameUIFilePath[characterFrameUI].c_str(), CHARACTER_FRAME_UI_WIDTH, CHARACTER_FRAME_UI_HEIGHT);/*初期化。*/
			m_characterFrameUI[characterFrameUI].SetPosition(initPosition);/*位置設定。*/
			m_characterFrameUI[characterFrameUI].SetScale(CHARACTER_FRAME_UI_INIT_SCALE);/*大きさ設定。*/
			m_characterFrameUI[characterFrameUI].Update();/*更新処理。*/
		}

		/*出撃選択UIの初期化。*/
		void MemberSelect::InitDeploySelectUI()
		{
			m_deploySelectUI.Init(m_deploySelectUIFilePath.c_str(), DEPLOY_SELECT_UI_WIDTH, DEPLOY_SELECT_UI_HEIGHT);/*初期化。*/
			m_deploySelectUI.SetPosition(DEPLOY_SELECT_UI_INIT_POSITION);/*位置設定。*/
			m_deploySelectUI.SetScale(DEPLOY_SELECT_UI_INIT_SCALE);/*大きさ設定。*/
			m_deploySelectUI.SetMulColor(DEPLOY_SELECT_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_deploySelectUI.Update();/*更新処理。*/
		}

		/*出撃テキストUIの初期化。*/
		void MemberSelect::InitDeployTextUI()
		{
			m_deployTextUI.Init(m_deployTextUIFilePath.c_str(), DEPLOY_TEXT_UI_WIDTH, DEPLOY_TEXT_UI_HEIGHT);/*初期化。*/
			m_deployTextUI.SetPosition(DEPLOY_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_deployTextUI.SetScale(DEPLOY_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_deployTextUI.Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void MemberSelect::InitUIAnimation()
		{
			/*UIをスライドさせるアニメーションの初期化。*/
			InitSlideUIAnimation();

			/*UIの透明度を変えるアニメーションの初期化。*/
			InitAlphaUIAnimation();
		}

		/*UIをスライドさせるアニメーションの初期化。*/
		void MemberSelect::InitSlideUIAnimation()
		{
			/*UIをスライドさせるアニメーションの値の設定。*/
			Vector3 basePosition = m_characterSelectUI.GetPosition();/*元の位置。*/
			Vector3 targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_CharacterSelectUI];/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_CharacterSelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = m_characterFrameUI[enCharacterFrameUI_One].GetPosition();/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_OneCharacterFrameUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_OneCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_One],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = m_characterFrameUI[enCharacterFrameUI_Two].GetPosition();/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TwoCharacterFrameUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_TwoCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Two],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = m_characterFrameUI[enCharacterFrameUI_Three].GetPosition();/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_ThreeCharacterFrameUI];/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_ThreeCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Three],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = m_characterFrameUI[enCharacterFrameUI_Three].GetPosition();/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_FourCharacterFrameUI];/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_FourCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Four],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = m_deploySelectUI.GetPosition();/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeploySelectUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_DeploySelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_deploySelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = m_deployTextUI.GetPosition();/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeployTextUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Left][enSlideUIAnimationSprite_DeployTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_deployTextUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);


			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_CharacterSelectUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_CharacterSelectUI];/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_CharacterSelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_OneCharacterFrameUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_OneCharacterFrameUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_OneCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_One],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TwoCharacterFrameUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_TwoCharacterFrameUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_TwoCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Two],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_ThreeCharacterFrameUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_ThreeCharacterFrameUI];/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_ThreeCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Three],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_FourCharacterFrameUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_FourCharacterFrameUI];/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_FourCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Four],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeploySelectUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_DeploySelectUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_DeploySelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_deploySelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*UIをスライドさせるアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeployTextUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Right][enSlideUIAnimationSprite_DeployTextUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlide_Right][enSlideUIAnimationSprite_DeployTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_deployTextUI,/*アニメーションをさせるスプライト。*/
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
		void MemberSelect::InitAlphaUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = m_characterSelectUI.GetMulColor().a;/*元の透明度。*/
			float targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_CharacterSelectUI] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_characterSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				true,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_deploySelectUI.GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_DeploySelectUI] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_deploySelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				true,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);
		}

		/*UIをスライドさせるアニメーションのリセット処理。*/
		void MemberSelect::ResetSlideUIAnimation(EnSlide slide)
		{
			for (int i = 0; i < enSlideUIAnimationSprite_Num; i++)
			{
				m_slideUIAnimation[slide][i]->Reset();
			}
		}

		/*UIの透明度を変えるアニメーションのリセット処理。*/
		void MemberSelect::ResetAlphaUIAnimation()
		{
			for (int i = 0; i < enAlphaUIAnimationSprite_Num; i++)
			{
				m_alphaUIAnimation[i]->Reset();
			}
		}

		/*選択の更新処理。*/ 
		void MemberSelect::UpdateSelect()
		{
			/*現在の選択が出撃選択なら。*/
			if (m_currentSelect == enSelect_Deploy)
			{
				/*上か下を入力したら前に選択したキャラクターを選択する。*/
				if (g_pad[0]->IsTrigger(enButtonUp) || g_pad[0]->IsTrigger(enButtonDown))
				{
					ResetAlphaUIAnimation();
					m_currentSelect = m_previousSelect;
					return;
				}

				if (g_pad[0]->IsTrigger(enButtonA))
				{
					nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::enFadeType_HalfFadeOut);
					EnableSelect();
					return;
				}
			}
			/*現在の選択が出撃以外の選択なら。*/
			else
			{
				/*上か下を入力したら出撃を選択する。*/
				if (g_pad[0]->IsTrigger(enButtonUp) || g_pad[0]->IsTrigger(enButtonDown))
				{
					ResetAlphaUIAnimation();
					m_currentSelect = enSelect_Deploy;
					return;
				}

				/*前に選択した内容を設定。*/
				m_previousSelect = m_currentSelect;
				
				/*左を入力したら。*/
				if (g_pad[0]->IsTrigger(enButtonLeft))
				{
					ResetAlphaUIAnimation();
					/*現在の選択が1人目のキャラクターを選択していたら。*/
					if (m_currentSelect == enSelect_OneCharacter) { m_currentSelect = enSelect_FourCharacter; return; }

					/*左隣りのキャラクターを選択する。*/
					m_currentSelect--;
					return;
				}

				/*右を入力したら。*/
				if (g_pad[0]->IsTrigger(enButtonRight))
				{
					ResetAlphaUIAnimation();
					/*現在の選択が4人目のキャラクターを選択していたら。*/
					if (m_currentSelect == enSelect_FourCharacter) { m_currentSelect = enSelect_OneCharacter; return; }

					/*右隣りのキャラクターを選択する。*/
					m_currentSelect++;
					return;
				}

				/*TODO:今後Aボタンを押したら役職を選択する画面に切り替える処理を入れる。*/
			}

			/*Bボタンを押したら前の選択に戻る。*/
			if (g_pad[0]->IsTrigger(enButtonB))
			{
				EnableBackSelect();
				EnableDirection();
			}
		}

		/*演出の更新処理。*/
		void MemberSelect::UpdateDirection()
		{
			/*演出中であったら。*/
			if (IsDirection())
			{
				for (int i = 0; i < enSlideUIAnimationSprite_Num; i++)
				{
					m_slideUIAnimation[m_currentSlide][i]->Update();
				}
			}
			else
			{
				/*選択した内容の特徴の設定。*/
				int currentSelectFeature = enSelectFauture_Num;
				/*現在の選択の内容から特徴を分岐する。*/
				m_currentSelect != enSelect_Deploy ? 
					currentSelectFeature = enSelectFauture_Character : 
					currentSelectFeature = enSelectFauture_Deploy;

				m_alphaUIAnimation[currentSelectFeature]->Update();
			}
		}

		/*スプライトの更新処理。*/
		void MemberSelect::UpdateSprite()
		{
			/*メンバー選択テキストUI。*/
			m_memberSelectTextUI.Update();

			/*出撃選択UI。*/
			m_deploySelectUI.Update();

			/*出撃テキストUI。*/
			m_deployTextUI.Update();

			/*キャラクター選択UI。*/
			if (m_currentSelect != enSelect_Deploy)
			{
				m_characterSelectUI.SetPosition(m_characterFrameUI[m_currentSelect].GetPosition());
			}
			m_characterSelectUI.Update();

			/*キャラクター枠UI。*/
			for(int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				m_characterFrameUI[i].Update();
			}
		}
	}
}