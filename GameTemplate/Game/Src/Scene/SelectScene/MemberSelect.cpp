#include "stdafx.h"
#include "MemberSelect.h"
#include "Src/Fade/Fade.h"

#include "Src/Actor/Character/Player/CharacterByWeapon/SwordCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/HammerCharacter.h"

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

	/*役割アイコンUI。*/
	const float ROLE_ICON_UI_WIDTH = 1024;/*役割アイコンUIの幅。*/

	const float ROLE_ICON_UI_HEIGHT = 1024;/*役割アイコンUIの高さ。*/

	const Vector3 ROLE_ICON_UI_INIT_POSITION = { 1785.0f,50.0f,0.0f };/*役割アイコンUIの初期位置。*/

	const float ROLE_ICON_UI_POSITION_X_OFFSET = 135.0f;/*役割アイコンUIのX軸の位置のオフセット。*/
	
	const float ROLE_ICON_UI_POSITION_Y_OFFSET = 150.0f;/*役割アイコンUIのY軸の位置のオフセット。*/

	const Vector3 ROLE_ICON_UI_INIT_SCALE = { 0.105f,0.105f,1.0f };/*役割アイコンUIの初期大きさ。*/

	/*キャラクターフェイドUI。*/
	const float CHARACTER_FADE_UI_WIDTH = 990;/*キャラクターフェイドUIの幅。*/

	const float CHARACTER_FADE_UI_HEIGHT = 1080;/*キャラクターフェイドUIの高さ。*/

	const Vector3 CHARACTER_FADE_UI_INIT_SCALE = { 0.45f,0.45f,1.0f };/*キャラクターフェイドUIの初期大きさ。*/

	const Vector4 CHARACTER_FADE_UI_INIT_MUL_COLOR = { 1.0f,1.0f,1.0f,0.5f };/*キャラクターフェイドUIの初期乗算色。*/

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

	/*ボタンUI。*/
	const float BUTTON_UI_WIDTH = 1024;/*ボタンUIの幅。*/

	const float BUTTON_UI_HEIGHT = 1024;/*ボタンUIの高さ。*/

	const Vector3 BUTTON_UI_INIT_POSITION[nsApp::nsSelect::MemberSelect::EnButtonUI::enButtonUI_Num] = {
		Vector3{-600.0f,-450.0f,0.0f},/*Aボタン。*/
		Vector3{-850.0f,-450.0f,0.0f},/*Bボタン。*/
		Vector3{-345.0f,-450.0f,0.0f},/*Xボタン。*/
		Vector3{75.0f,-450.0f,0.0f}/*Yボタン。*/
	};/*ボタンUIの初期位置。*/

	const Vector3 BUTTON_UI_INIT_SCALE = { 0.1f,0.1f,1.0f };/*ボタンUIの初期大きさ。*/

	/*テキストUI。*/
	const float TEXT_UI_WIDTH = 1024;/*テキストUIの幅。*/

	const float TEXT_UI_HEIGHT = 256;/*テキストUIの高さ。*/

	const Vector3 TEXT_UI_INIT_POSITION[nsApp::nsSelect::MemberSelect::EnTextUI::enTextUI_Num] = {
		Vector3{-475.0f,-450.0f,0.0f},/*決定。*/
		Vector3{-725.0f,-450.0f,0.0f},/*戻る。*/
		Vector3{-135.0f,-450.0f,0.0f},/*ゲーム設定。*/
		Vector3{255.0f,-450.0f,0.0f}/*操作方法。*/
	};/*テキストUIの初期位置。*/

	const Vector3 TEXT_UI_INIT_SCALE = { 1.0f,1.0f,1.0f };/*テキストUIの初期大きさ。*/

	/*UIアニメーション。*/
	const float SLIDE_UI_ANIMATION_PLAY_SPEED = 4.0f;/*UIをスライドさせるアニメーションの再生速度。*/

	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 1.2f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const float SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED = 7.0f;/*選択したときの演出UIアニメーションの再生速度。*/

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

	const float DOWN_POSITION_OFFSET = 25.0f;/*選択したときの演出UIアニメーション時に下降する位置のオフセット。*/
}

namespace nsApp
{
	namespace nsSelect
	{
		/*デストラクタ。*/
		MemberSelect::~MemberSelect()
		{
			for (int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				//for (int j = 0; j < RoleSelect::EnRole::enRole_Num; j++)
				for (int j = 0; j <= RoleSelect::EnRole::enRole_Hammer; j++)
				{
					DeleteGO(m_characterModel[i][j]);
				}
			}
		}

		/*開始処理。*/
		bool MemberSelect::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			for (int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				//for (int j = 0; j < RoleSelect::EnRole::enRole_Num; j++)
				for (int j = 0; j <= RoleSelect::EnRole::enRole_Hammer; j++)
				{
					/*キャラクターモデルの生成。*/
					CreateCharacterModel((EnCharacterFrameUI)i, (RoleSelect::EnRole)j);
				}
			}

			return true;
		}

		/*更新処理。*/
		void MemberSelect::Update()
		{
			if (nsApp::nsFade::Fade::GetInstance()->IsFadeIn())
			{
				return;
			}

			/*演出中でないかつ
			* 選択できていないかつ
			* ゲーム設定ボタンを押していなければ。
			*/
			if (!IsDirection() && !DidSelect() && !IsPushGameOptionButton())
			{
				/*選択。*/
				UpdateSelect();
			}
			
			/*UIアニメーション。*/
			UpdateUIAnimation();

			/*スプライト。*/
			UpdateSprite();

			/*キャラクターモデル。*/
			UpdateCharacterModel();
		}

		/*描画処理。*/
		void MemberSelect::Render(RenderContext& rc)
		{
			/*メンバー選択テキストUIの描画。*/
			m_memberSelectTextUI.Draw(rc);

			/*現在の選択が出撃以外のとき。*/
			if (m_currentSelect != enSelect_Deploy)
			{
				/*選択したときの演出UIアニメーションが終わっていないとき描画する。*/
				if (!IsEndSelectDirectionUIAnimation(
					enPosition_Up,
					(EnSelectDirectionUIAnimationSprite)(GetCurrentSelect() + 1)
					)
				)
				{
					/*キャラクター選択UIの描画。*/
					m_characterSelectUI.Draw(rc);
				}
			}

			for (int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				/*キャラクター枠UIの描画。*/
				m_characterFrameUI[i].Draw(rc);

				/*役割アイコン。*/
				m_roleIconUI[i][(RoleSelect::EnRole)m_currentRole[i]].Draw(rc);
			}

			/*現在の選択が出撃のとき描画する。*/
			if (m_currentSelect == enSelect_Deploy)
			{
				/*出撃選択UIの描画。*/
				m_deploySelectUI.Draw(rc);
			}

			/*現在の選択が出撃以外のとき。*/
			if (m_currentSelect != enSelect_Deploy)
			{
				/*選択したときの演出UIアニメーションが終わっているとき描画する。*/
				if (IsEndSelectDirectionUIAnimation(
					enPosition_Up,
					(EnSelectDirectionUIAnimationSprite)(GetCurrentSelect() + 1)
					)
				)
				{
					/*キャラクターフェイドUIの描画。*/
					m_characterFadeUI.Draw(rc);
				}
			}

			/*出撃テキストUIの描画。*/
			m_deployTextUI.Draw(rc);

			/*ボタンUIとテキストUIを描画しない状態なら処理しない。*/
			if (!IsDrawingButtonAndTextUI())
			{
				return;
			}

			for (int j = 0; j < enButtonUI_Num; j++)
			{
				/*選択したときの演出UIアニメーションが終わっているときはXボタンUIを描画しない。*/
				/*役割選択画面を表示していて存在しない操作をしないため。*/
				if (IsEndSelectDirectionUIAnimation(
					enPosition_Up,
					(EnSelectDirectionUIAnimationSprite)(GetCurrentSelect() + 1)
					)
				)
				{
					if (j == enButtonUI_X) { continue; }
				}

				/*ボタンUIの描画。*/
				m_buttonUI[j].Draw(rc);
			}

			for (int k = 0; k < enTextUI_Num; k++)
			{
				/*選択したときの演出UIアニメーションが終わっているときはXボタンUIを描画しない。*/
				/*役割選択画面を表示していて存在しない操作をしないため。*/
				if (IsEndSelectDirectionUIAnimation(
					enPosition_Up,
					(EnSelectDirectionUIAnimationSprite)(GetCurrentSelect() + 1)
					)
				)
				{
					if (k == enTextUI_GameOption) { continue; }
				}

				/*テキストUIの描画。*/
				m_textUI[k].Draw(rc);
			}
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

				for (int j = 0; j < RoleSelect::EnRole::enRole_Num; j++)
				{
					/*役割アイコンUI。*/
					InitRoleIconUI((EnCharacterFrameUI)i, (RoleSelect::EnRole)j);
				}
			}

			/*キャラクターフェイドUI。*/
			InitCharacterFadeUI();

			/*出撃選択UI。*/
			InitDeploySelectUI();

			/*出撃テキストUI。*/
			InitDeployTextUI();

			for (int k = 0; k < enButtonUI_Num; k++)
			{
				/*ボタンUI。*/
				InitButtonUI((EnButtonUI)k);
			}

			for (int l = 0; l < enTextUI_Num; l++)
			{
				/*テキストUI。*/
				InitTextUI((EnTextUI)l);
			}
		}

		/*メンバー選択テキストUIの初期化。*/
		void MemberSelect::InitMemberSelectTextUI()
		{
			m_memberSelectTextUI.Init(m_memberSelectTextUIFilePath.c_str(), MEMBER_SELECT_TEXT_UI_WIDTH, MEMBER_SELECT_TEXT_UI_HEIGHT, true);/*初期化。*/
			m_memberSelectTextUI.SetPosition(MEMBER_SELECT_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_memberSelectTextUI.SetScale(MEMBER_SELECT_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_memberSelectTextUI.Update();/*更新処理。*/
		}

		/*キャラクター選択UIの初期化。*/
		void MemberSelect::InitCharacterSelectUI()
		{
			m_characterSelectUI.Init(m_characterSelectUIFilePath.c_str(), CHARACTER_SELECT_UI_WIDTH, CHARACTER_SELECT_UI_HEIGHT, true);/*初期化。*/
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

			m_characterFrameUI[characterFrameUI].Init(m_characterFrameUIFilePath[characterFrameUI].c_str(), CHARACTER_FRAME_UI_WIDTH, CHARACTER_FRAME_UI_HEIGHT, true);/*初期化。*/
			m_characterFrameUI[characterFrameUI].SetPosition(initPosition);/*位置設定。*/
			m_characterFrameUI[characterFrameUI].SetScale(CHARACTER_FRAME_UI_INIT_SCALE);/*大きさ設定。*/
			m_characterFrameUI[characterFrameUI].Update();/*更新処理。*/
		}

		void MemberSelect::InitRoleIconUI(EnCharacterFrameUI characterFrameUI, RoleSelect::EnRole role)
		{
			m_roleIconUI[characterFrameUI][role].Init(m_roleIconUIFilePath[characterFrameUI][role].c_str(), ROLE_ICON_UI_WIDTH, ROLE_ICON_UI_HEIGHT, true);/*初期化。*/
			m_roleIconUI[characterFrameUI][role].SetPosition(ROLE_ICON_UI_INIT_POSITION);/*位置設定。*/
			m_roleIconUI[characterFrameUI][role].SetScale(ROLE_ICON_UI_INIT_SCALE);/*大きさ設定。*/
			m_roleIconUI[characterFrameUI][role].Update();/*更新処理。*/
		}

		/*キャラクターフェイドUIの初期化。*/
		void MemberSelect::InitCharacterFadeUI()
		{
			Vector3 initPosition = m_characterFrameUI[enCharacterFrameUI_One].GetPosition();/*初期位置。*/

			m_characterFadeUI.Init(m_characterFadeUIFilePath.c_str(), CHARACTER_FADE_UI_WIDTH, CHARACTER_FADE_UI_HEIGHT, true);/*初期化。*/
			m_characterFadeUI.SetPosition(initPosition);/*位置設定。*/
			m_characterFadeUI.SetScale(CHARACTER_FADE_UI_INIT_SCALE);/*大きさ設定。*/
			m_characterFadeUI.SetMulColor(CHARACTER_FADE_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_characterFadeUI.Update();/*更新処理。*/
		}

		/*出撃選択UIの初期化。*/
		void MemberSelect::InitDeploySelectUI()
		{
			m_deploySelectUI.Init(m_deploySelectUIFilePath.c_str(), DEPLOY_SELECT_UI_WIDTH, DEPLOY_SELECT_UI_HEIGHT, true);/*初期化。*/
			m_deploySelectUI.SetPosition(DEPLOY_SELECT_UI_INIT_POSITION);/*位置設定。*/
			m_deploySelectUI.SetScale(DEPLOY_SELECT_UI_INIT_SCALE);/*大きさ設定。*/
			m_deploySelectUI.SetMulColor(DEPLOY_SELECT_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_deploySelectUI.Update();/*更新処理。*/
		}

		/*出撃テキストUIの初期化。*/
		void MemberSelect::InitDeployTextUI()
		{
			m_deployTextUI.Init(m_deployTextUIFilePath.c_str(), DEPLOY_TEXT_UI_WIDTH, DEPLOY_TEXT_UI_HEIGHT, true);/*初期化。*/
			m_deployTextUI.SetPosition(DEPLOY_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_deployTextUI.SetScale(DEPLOY_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_deployTextUI.Update();/*更新処理。*/
		}

		/*ボタンUIの初期化。*/
		void MemberSelect::InitButtonUI(EnButtonUI buttonUI)
		{
			m_buttonUI[buttonUI].Init(m_buttonUIFilePath[buttonUI].c_str(), BUTTON_UI_WIDTH, BUTTON_UI_HEIGHT, true);/*初期化。*/
			m_buttonUI[buttonUI].SetPosition(BUTTON_UI_INIT_POSITION[buttonUI]);/*位置設定。*/
			m_buttonUI[buttonUI].SetScale(BUTTON_UI_INIT_SCALE);/*大きさ設定。*/
			m_buttonUI[buttonUI].Update();/*更新処理。*/
		}

		/*テキストUIの初期化。*/
		void MemberSelect::InitTextUI(EnTextUI textUI)
		{
			m_textUI[textUI].Init(m_textUIFilePath[textUI].c_str(), TEXT_UI_WIDTH, TEXT_UI_HEIGHT, true);/*初期化。*/
			m_textUI[textUI].SetPosition(TEXT_UI_INIT_POSITION[textUI]);/*位置設定。*/
			m_textUI[textUI].SetScale(TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_textUI[textUI].Update();/*更新処理。*/
		}

		/*UIアニメーションの初期化。*/
		void MemberSelect::InitUIAnimation()
		{
			/*UIをスライドさせるアニメーション。*/
			InitSlideUIAnimation();

			/*UIの透明度を変えるアニメーション。*/
			InitAlphaUIAnimation();

			/*選択したときの演出UIアニメーション*/
			InitSelectDirectionUIAnimation();
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

		/*選択したときの演出UIアニメーションの初期化。*/
		void MemberSelect::InitSelectDirectionUIAnimation()
		{
			/*選択したときの演出UIアニメーションの値の設定。*/
			Vector3 basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_CharacterSelectUI];/*元の位置。*/
			Vector3 targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_CharacterSelectUI];/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_CharacterSelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_OneCharacterFrameUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_OneCharacterFrameUI];/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_OneCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_One],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TwoCharacterFrameUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TwoCharacterFrameUI];/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_TwoCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Two],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_ThreeCharacterFrameUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_ThreeCharacterFrameUI];/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_ThreeCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Three],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_FourCharacterFrameUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_FourCharacterFrameUI];/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_FourCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Four],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeploySelectUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeploySelectUI];/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_DeploySelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_deploySelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeployTextUI];/*元の位置。*/
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeployTextUI];/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Down][enSelectDirectionUIAnimationSprite_DeployTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_deployTextUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);


			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_CharacterSelectUI];/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_CharacterSelectUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_CharacterSelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterSelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_OneCharacterFrameUI];/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_OneCharacterFrameUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_OneCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_One],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TwoCharacterFrameUI];/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_TwoCharacterFrameUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_TwoCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Two],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_ThreeCharacterFrameUI];/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_ThreeCharacterFrameUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_ThreeCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Three],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_FourCharacterFrameUI];/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_FourCharacterFrameUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_FourCharacterFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_characterFrameUI[enCharacterFrameUI_Four],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeploySelectUI];/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeploySelectUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_DeploySelectUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_deploySelectUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			/*選択したときの演出UIアニメーションの値の設定。*/
			basePosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeployTextUI];/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			targetPosition = AFTER_UI_ANIMATION_POSITION[enSlide_Left][enSlideUIAnimationSprite_DeployTextUI];/*ターゲットの位置。*/

			/*初期化。*/
			m_selectDirectionUIAnimation[enPosition_Up][enSelectDirectionUIAnimationSprite_DeployTextUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_deployTextUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);
		}

		/*キャラクターモデルの生成。*/
		void MemberSelect::CreateCharacterModel(EnCharacterFrameUI characterFrameUI, RoleSelect::EnRole role)
		{
			Vector3 initPosition = m_characterFrameUI[characterFrameUI].GetPosition();
			initPosition.z += 1000.0f;
			switch (role)
			{
			case RoleSelect::EnRole::enRole_Sword:
				m_characterModel[characterFrameUI][role] = NewGO<nsActor::SwordCharacter>(0, "player4");
				m_characterModel[characterFrameUI][role]->SetPosition(initPosition);
				m_characterModel[characterFrameUI][role]->SetAngle(180.0f);
				break;
			case RoleSelect::EnRole::enRole_Hammer:
				m_characterModel[characterFrameUI][role] = NewGO<nsActor::HammerCharacter>(0, "player4");
				m_characterModel[characterFrameUI][role]->SetPosition(initPosition);
				m_characterModel[characterFrameUI][role]->SetAngle(180.0f);
				break;
			//case RoleSelect::EnRole::enRole_Mage:
			//	m_characterModel[characterFrameUI][role] = NewGO<nsActor::MageCharacter>(0, "player4");
			//	m_characterModel[characterFrameUI][role]->SetPosition(initPosition);
			//	m_characterModel[characterFrameUI][role]->SetAngle(180.0f);
			//	break;
			//case RoleSelect::EnRole::enRole_Gunner:
			//	m_characterModel[characterFrameUI][role] = NewGO<nsActor::GunnerCharacter>(0, "player4");
			//	m_characterModel[characterFrameUI][role]->SetPosition(initPosition);
			//	m_characterModel[characterFrameUI][role]->SetAngle(180.0f);
			//	break;
			default:
				break;
			}
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

		/*選択したときの演出UIアニメーションのリセット処理。*/
		void MemberSelect::ResetSelectDirectionUIAnimation()
		{
			for (int i = 0; i < enPosition_Num; i++)
			{
				for (int j = 0; j < enSelectDirectionUIAnimationSprite_Num; j++)
				{
					m_selectDirectionUIAnimation[i][j]->Reset();
				}
			}
		}

		/*全キャラクターモデルを無効化する。*/
		void MemberSelect::AllCharacterModelDeactivate()
		{
			for (int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				//for (int j = 0; j < RoleSelect::EnRole::enRole_Num; j++)
				for (int j = 0; j <= RoleSelect::EnRole::enRole_Hammer; j++)
				{
					/*キャラクターモデルの生成。*/
					m_characterModel[i][j]->Deactivate();
				}
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

				/*Aボタンを押したら選択したときの演出を流すようにする。*/
				if (g_pad[0]->IsTrigger(enButtonA))
				{
					EnableSelect();
					EnableDirection();
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

				/*Aボタンを押したら選択したときの演出を流すようにする。*/
				if (g_pad[0]->IsTrigger(enButtonA))
				{
					EnableSelect();
					EnableDirection();
					return;
				}
			}

			/*Xボタンを押したら設定画面を表示する。*/
			if (g_pad[0]->IsTrigger(enButtonX))
			{
				EnablePushGameOptionButton();
				return;
			}

			/*Yボタンを押したら操作方法画面を表示する。*/
			if (g_pad[0]->IsTrigger(enButtonY))
			{
				EnablePushHowToPlayButton();
				return;
			}

			/*Bボタンを押したら前の選択に戻る。*/
			if (g_pad[0]->IsTrigger(enButtonB))
			{
				EnableBackSelect();
				EnableDirection();
			}
		}

		/*UIアニメーションの更新処理。*/
		void MemberSelect::UpdateUIAnimation()
		{
			/*演出中なら。*/
			if (IsDirection())
			{
				/*選択できている状態。*/
				if (DidSelect())
				{
					/*選択した内容の特徴の設定。*/
					int currentSelectFeature = enSelectFauture_Num;
					/*現在の選択の内容から特徴を分岐する。*/
					m_currentSelect != enSelect_Deploy ?
						currentSelectFeature = enSelectFauture_Character:/*キャラクター。*/
						currentSelectFeature = enSelectFauture_Deploy;/*出撃。*/

					/*選択UIの特徴の設定。*/
					int currentSelectUIFeature = enSelectDirectionUIAnimationSprite_Num;
					currentSelectFeature == enSelectFauture_Character ?
						currentSelectUIFeature = enSelectDirectionUIAnimationSprite_CharacterSelectUI:/*キャラクター。*/
						currentSelectUIFeature = enSelectDirectionUIAnimationSprite_DeploySelectUI;/*出撃。*/

					/*ターゲットUIの特徴の設定*/
					int currentTargetUIFeature = enSelectDirectionUIAnimationSprite_Num;
					int currentCharacterFrameUI = m_currentSelect + 1;
					currentSelectFeature == enSelectFauture_Character ?
						currentTargetUIFeature =  currentCharacterFrameUI:/*キャラクター。*/
						currentTargetUIFeature = enSelectDirectionUIAnimationSprite_DeployTextUI;/*出撃。*/

					if (!m_selectDirectionUIAnimation[enPosition_Down][currentTargetUIFeature]->IsEnd())
					{
						m_selectDirectionUIAnimation[enPosition_Down][currentTargetUIFeature]->Update();
						m_selectDirectionUIAnimation[enPosition_Down][currentSelectUIFeature]->Update();
					}
					else if (!m_selectDirectionUIAnimation[enPosition_Up][currentTargetUIFeature]->IsEnd())
					{
						m_selectDirectionUIAnimation[enPosition_Up][currentTargetUIFeature]->Update();
						m_selectDirectionUIAnimation[enPosition_Up][currentSelectUIFeature]->Update();
					}
					else
					{
						if (m_currentSelect == enSelect_Deploy)
						{
							nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::enFadeType_HalfFadeOut);
						}
					}
				}
				/*選択できていない状態。*/
				else
				{
					for (int i = 0; i < enSlideUIAnimationSprite_Num; i++)
					{
						m_slideUIAnimation[m_currentSlide][i]->Update();
					}
				}
				return;
			}

			/*選択した内容の特徴の設定。*/
			int currentSelectFeature = enSelectFauture_Num;
			/*現在の選択の内容から特徴を分岐する。*/
			m_currentSelect != enSelect_Deploy ? 
				currentSelectFeature = enSelectFauture_Character : 
				currentSelectFeature = enSelectFauture_Deploy;

			m_alphaUIAnimation[currentSelectFeature]->Update();
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

				Vector3 currentPosition = m_characterFrameUI[i].GetPosition();
				currentPosition.x += ROLE_ICON_UI_POSITION_X_OFFSET;
				currentPosition.y += ROLE_ICON_UI_POSITION_Y_OFFSET;

				m_roleIconUI[i][(RoleSelect::EnRole)m_currentRole[i]].SetPosition(currentPosition);
				m_roleIconUI[i][(RoleSelect::EnRole)m_currentRole[i]].Update();
			}

			/*キャラクターフェイドUI。*/
			m_characterFadeUI.SetPosition(m_characterFrameUI[m_currentSelect].GetPosition());
			m_characterFadeUI.Update();

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

		/*キャラクターモデルの更新処理。*/
		void MemberSelect::UpdateCharacterModel()
		{
			float offsetX = 560.0f;/*キャラクターモデル用でX軸にずらすオフセット。*/
			float positionY = 50.0f;/*キャラクターモデルのY軸の位置。*/
			float intervalPositon = 375.0f;/*間隔位置。*/

			for (int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				Vector3 currentCharacterFrameUIPosition = m_characterFrameUI[i].GetPosition();
				Vector3 currentPosition = currentCharacterFrameUIPosition;
				currentPosition.x += offsetX - intervalPositon * i;
				currentPosition.y = positionY;

				//for (int j = 0; j < RoleSelect::EnRole::enRole_Num; j++)
				for (int j = 0; j <= RoleSelect::EnRole::enRole_Hammer; j++)
				{
					m_characterModel[i][j]->GetCharacterController().SetPosition(currentPosition);
					m_characterModel[i][j]->SetPosition(currentPosition);

					/*選択したときの演出UIアニメーションが終わっていないときモデルの処理を有効化する。*/
					if (!IsEndSelectDirectionUIAnimation(
						enPosition_Up,
						(EnSelectDirectionUIAnimationSprite)(GetCurrentSelect() + 1)
						)
					)
					{
						if (m_currentRole[i] == j)
						{
							m_characterModel[i][j]->Activate();
						}
						else
						{
							m_characterModel[i][j]->Deactivate();
						}
					}
				}
			}

			/*現在の選択が出撃以外を選択していたら*/
			if (m_currentSelect != enSelect_Deploy)
			{
				/*選択したときの演出UIアニメーションが終わっていないときモデルの処理を有効化する。*/
				if (IsEndSelectDirectionUIAnimation(
					enPosition_Up,
					(EnSelectDirectionUIAnimationSprite)(GetCurrentSelect() + 1)
				)
					)
				{
					m_characterModel[m_currentSelect][m_currentRole[m_currentSelect]]->Deactivate();
				}
			}
		}
	}
}