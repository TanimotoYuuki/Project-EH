#include "stdafx.h"
#include "MemberSelect.h"
#include "Src/Fade/Fade.h"

#include "Src/Actor/Character/Player/CharacterByWeapon/SwordCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/HammerCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/WandCharacter.h"
#include "Src/Actor/Character/Player/CharacterByWeapon/TwinGunCharacter.h"

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

	const float SLIDE_UI_ANIMATION_POSITION_OFFSET = 2500.0f;/*UIをスライドさせるアニメーションの位置のオフセット。*/

	const Vector3 AFTER_UI_ANIMATION_POSITION[nsApp::nsSelect::MemberSelect::EnSlide::enSlide_Num][nsApp::nsSelect::MemberSelect::EnSlideUIAnimationSprite::enSlideUIAnimationSprite_Num] = {
		/*左側にスライドした時の位置。*/
		Vector3{-715.0f,50.0f,0.0f},/*キャラクター選択UI。*/
		Vector3{-715.0f,50.0f,0.0f},/*1人目のキャラクター枠UI。*/
		Vector3{-715.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 1,50.0f,0.0f},/*2人目のキャラクター枠UI。*/
		Vector3{-715.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 2,50.0f,0.0f},/*3人目のキャラクター枠UI。*/
		Vector3{-715.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 3,50.0f,0.0f},/*4人目のキャラクター枠UI。*/
		Vector3{-715.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 1,50.0f,0.0f},/*NPC用の2人目のキャラクター枠UI。*/
		Vector3{-715.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 2,50.0f,0.0f},/*NPC用の3人目のキャラクター枠UI。*/
		Vector3{-715.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 3,50.0f,0.0f},/*NPC用の4人目のキャラクター枠UI。*/
		Vector3{0.0f,-350.0f,0.0f},/*出撃選択UI。*/
		Vector3{0.0f,-350.0f,0.0f},/*出撃テキストUI。*/

		/*右側にスライドした時の位置。*/
		Vector3{1785.0f,50.0f,0.0f},/*キャラクター選択UI。*/
		Vector3{1785.0f,50.0f,0.0f},/*1人目のキャラクター枠UI。*/
		Vector3{1785.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 1,50.0f,0.0f},/*2人目のキャラクター枠UI。*/
		Vector3{1785.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 2,50.0f,0.0f},/*3人目のキャラクター枠UI。*/
		Vector3{1785.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 3,50.0f,0.0f},/*4人目のキャラクター枠UI。*/
		Vector3{1785.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 1,50.0f,0.0f},/*NPC用の2人目のキャラクター枠UI。*/
		Vector3{1785.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 2,50.0f,0.0f},/*NPC用の3人目のキャラクター枠UI。*/
		Vector3{1785.0f + CHARACTER_FRAME_UI_POSITION_INTERVAL * 3,50.0f,0.0f},/*NPC用の4人目のキャラクター枠UI。*/
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
				for (int j = 0; j < RoleSelect::EnRole::enRole_Num; j++)
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
				for (int j = 0; j < RoleSelect::EnRole::enRole_Num; j++)
				{
					/*キャラクターモデルの生成。*/
					CreateCharacterModel((EnCharacterFrameUI)i, (RoleSelect::EnRole)j, i);
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

			GamePad gamePad;
			for (int i = 0; i < GamePad::MAX_PAD; i++)
			{
				if (i == 0) 
				{ 
					m_isPlayerControle[i] = true;
					continue;
				}

				if (gamePad.GetPadState(i) == GamePad::EnXInputPadState::Connect)
				{
					m_isPlayerControle[i] = true;/*プレイヤーが操作している。*/
				}
				else
				{
					m_isPlayerControle[i] = false;/*プレイヤーが操作していない。*/
				}
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
				if (m_isPlayerControle[m_currentSelect])
				{
					/*選択したときの演出UIアニメーションが終わっていないとき描画する。*/
					if (!IsEndSelectEndDirectionUIAnimation(GetCurrentSelect() + 1))
					{
						/*キャラクター選択UIの描画。*/
						m_characterSelectUI.Draw(rc);
					}
				}
				else
				{
					/*選択したときの演出UIアニメーションが終わっていないとき描画する。*/
					if (!IsEndSelectEndDirectionUIAnimation(GetCurrentSelect() + 4))
					{
						/*キャラクター選択UIの描画。*/
						m_characterSelectUI.Draw(rc);
					}
				}
			}

			for (int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				if (m_isPlayerControle[i])
				{
					/*キャラクター枠UIの描画。*/
					m_characterFrameUI[i].Draw(rc);
				}
				else
				{
					/*NPC用のキャラクター枠UIの描画。*/
					m_npcCharacterFrameUI[i - 1].Draw(rc);
				}

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
				if (m_isPlayerControle[m_currentSelect])
				{
					/*選択したときの演出UIアニメーションが終わっているとき描画する。*/
					if (IsEndSelectEndDirectionUIAnimation(GetCurrentSelect() + 1))
					{
						/*キャラクターフェイドUIの描画。*/
						m_characterFadeUI.Draw(rc);
					}
				}
				else
				{
					/*選択したときの演出UIアニメーションが終わっているとき描画する。*/
					if (IsEndSelectEndDirectionUIAnimation(GetCurrentSelect() + 4))
					{
						/*キャラクターフェイドUIの描画。*/
						m_characterFadeUI.Draw(rc);
					}
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
				if (IsEndSelectEndDirectionUIAnimation(GetCurrentSelect() + 1))
				{
					if (j == enButtonUI_X || j == enButtonUI_Y) { continue; }
				}

				/*ボタンUIの描画。*/
				m_buttonUI[j].Draw(rc);
			}

			for (int k = 0; k < enTextUI_Num; k++)
			{
				/*選択したときの演出UIアニメーションが終わっているときはXボタンUIを描画しない。*/
				/*役割選択画面を表示していて存在しない操作をしないため。*/
				if (IsEndSelectEndDirectionUIAnimation(GetCurrentSelect() + 1))
				{
					if (k == enTextUI_GameOption || k == enTextUI_HowToPlay) { continue; }
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

			for (int k = 0; k < enNpcCharacterFrameUI_Num; k++)
			{
				/*NPC用のキャラクター枠UI。*/
				InitNpcCharacterFrameUI((EnNpcCharacterFrameUI)k);
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

			m_slideUIAnimationSprite.push_back(&m_characterSelectUI);/*UIをスライドさせるアニメーションのスプライトに追加。*/
			m_alphaUIAnimationSprite.push_back(&m_characterSelectUI);/*UIの透明度を変えるアニメーションのスプライトに追加。*/
			m_selectDirectionUIAnimationSprite.push_back(&m_characterSelectUI);/*選択したときの演出UIアニメーションのスプライトに追加。*/
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

			m_slideUIAnimationSprite.push_back(&m_characterFrameUI[characterFrameUI]);/*UIをスライドさせるアニメーションのスプライトに追加。*/
			m_selectDirectionUIAnimationSprite.push_back(&m_characterFrameUI[characterFrameUI]);/*選択したときの演出UIアニメーションのスプライトに追加。*/
		}

		/*NPC用のキャラクター枠UIの初期化。*/
		void MemberSelect::InitNpcCharacterFrameUI(EnNpcCharacterFrameUI npcCharacterFrameUI)
		{
			m_npcCharacterFrameUI[npcCharacterFrameUI].Init(m_npcCharacterFrameUIFilePath[npcCharacterFrameUI].c_str(), CHARACTER_FRAME_UI_WIDTH, CHARACTER_FRAME_UI_HEIGHT, true);/*初期化。*/
			m_npcCharacterFrameUI[npcCharacterFrameUI].SetPosition(m_characterFrameUI[npcCharacterFrameUI + 1].GetPosition());/*位置設定。*/
			m_npcCharacterFrameUI[npcCharacterFrameUI].SetScale(CHARACTER_FRAME_UI_INIT_SCALE);/*大きさ設定。*/
			m_npcCharacterFrameUI[npcCharacterFrameUI].Update();/*更新処理。*/

			m_slideUIAnimationSprite.push_back(&m_npcCharacterFrameUI[npcCharacterFrameUI]);/*UIをスライドさせるアニメーションのスプライトに追加。*/
			m_selectDirectionUIAnimationSprite.push_back(&m_npcCharacterFrameUI[npcCharacterFrameUI]);/*選択したときの演出UIアニメーションのスプライトに追加。*/
		}

		/*役割アイコンUIの初期化。*/
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

			m_slideUIAnimationSprite.push_back(&m_deploySelectUI);/*UIをスライドさせるアニメーションのスプライトに追加。*/
			m_alphaUIAnimationSprite.push_back(&m_deploySelectUI);/*UIの透明度を変えるアニメーションのスプライトに追加。*/
			m_selectDirectionUIAnimationSprite.push_back(&m_deploySelectUI);/*選択したときの演出UIアニメーションのスプライトに追加。*/
		}

		/*出撃テキストUIの初期化。*/
		void MemberSelect::InitDeployTextUI()
		{
			m_deployTextUI.Init(m_deployTextUIFilePath.c_str(), DEPLOY_TEXT_UI_WIDTH, DEPLOY_TEXT_UI_HEIGHT, true);/*初期化。*/
			m_deployTextUI.SetPosition(DEPLOY_TEXT_UI_INIT_POSITION);/*位置設定。*/
			m_deployTextUI.SetScale(DEPLOY_TEXT_UI_INIT_SCALE);/*大きさ設定。*/
			m_deployTextUI.Update();/*更新処理。*/

			m_slideUIAnimationSprite.push_back(&m_deployTextUI);/*UIをスライドさせるアニメーションのスプライトに追加。*/
			m_selectDirectionUIAnimationSprite.push_back(&m_deployTextUI);/*選択したときの演出UIアニメーションのスプライトに追加。*/
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
			for (int i = 0; i < m_slideUIAnimationSprite.size(); i++)
			{
				/*UIを左にスライドさせるアニメーション。*/
				InitSlideLeftUIAnimation(m_slideUIAnimationSprite[i]);

				/*UIを右にスライドさせるアニメーション。*/
				InitSlideRightUIAnimation(m_slideUIAnimationSprite[i]);
			}

			for (int j = 0; j < m_alphaUIAnimationSprite.size(); j++)
			{
				/*UIの透明度を変えるアニメーション。*/
				InitAlphaUIAnimation(m_alphaUIAnimationSprite[j]);
			}

			for (int k = 0; k < m_selectDirectionUIAnimationSprite.size(); k++)
			{
				/*選択したときの演出UIアニメーション(開始)。*/
				InitSelectStartDirectionUIAnimation(m_selectDirectionUIAnimationSprite[k]);

				/*選択したときの演出UIアニメーション(終了)。*/
				InitSelectEndDirectionUIAnimation(m_selectDirectionUIAnimationSprite[k]);	
			}
		}

		/*UIを左にスライドさせるアニメーション。*/
		void MemberSelect::InitSlideLeftUIAnimation(SpriteRender* spriteData)
		{
			/*UIをスライドさせるアニメーションの値の設定。*/
			Vector3 basePosition = spriteData->GetPosition();/*元の位置。*/
			Vector3 targetPosition = spriteData->GetPosition();/*ターゲットの位置。*/
			targetPosition.x -= SLIDE_UI_ANIMATION_POSITION_OFFSET;/*位置のオフセットを加算。*/

			/*初期化。*/
			m_slideLeftUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
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

		/*UIを右にスライドさせるアニメーション。*/
		void MemberSelect::InitSlideRightUIAnimation(SpriteRender* spriteData)
		{
			/*UIをスライドさせるアニメーションの値の設定。*/
			Vector3 basePosition = spriteData->GetPosition();/*元の位置。*/
			basePosition.x -= SLIDE_UI_ANIMATION_POSITION_OFFSET;/*位置のオフセットを加算。*/
			Vector3 targetPosition = spriteData->GetPosition();/*ターゲットの位置。*/

			/*初期化。*/
			m_slideRightUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
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

		/*UIの透明度を変えるアニメーションの初期化。*/
		void MemberSelect::InitAlphaUIAnimation(SpriteRender* spriteData)
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = spriteData->GetMulColor().a;/*元の透明度。*/
			float targetAlpha = AFTER_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation.push_back(std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				true,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/)
			);
		}

		/*選択したときの演出UIアニメーション(開始)。*/
		void MemberSelect::InitSelectStartDirectionUIAnimation(SpriteRender* spriteData)
		{
			/*選択したときの演出UIアニメーションの値の設定。*/
			Vector3 spritePosition = spriteData->GetPosition();/*対象となるスプライトの位置。*/
			spritePosition.x -= SLIDE_UI_ANIMATION_POSITION_OFFSET;

			Vector3 basePosition = spritePosition;/*元の位置。*/
			Vector3 targetPosition = spritePosition;/*ターゲットの位置。*/
			targetPosition.y -= DOWN_POSITION_OFFSET;

			/*初期化。*/
			m_selectStartDirectionUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/)
			);
		}

		/*選択したときの演出UIアニメーション(終了)。*/
		void MemberSelect::InitSelectEndDirectionUIAnimation(SpriteRender* spriteData)
		{
			/*選択したときの演出UIアニメーションの値の設定。*/
			Vector3 spritePosition = spriteData->GetPosition();/*対象となるスプライトの位置。*/
			spritePosition.x -= SLIDE_UI_ANIMATION_POSITION_OFFSET;

			Vector3 basePosition = spritePosition;/*元の位置。*/
			basePosition.y -= DOWN_POSITION_OFFSET;
			Vector3 targetPosition = spritePosition;/*ターゲットの位置。*/

			/*初期化。*/
			m_selectEndDirectionUIAnimation.push_back(std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				spriteData,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SELECT_DIRECTION_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/)
			);
		}

		/*キャラクターモデルの生成。*/
		void MemberSelect::CreateCharacterModel(EnCharacterFrameUI characterFrameUI, RoleSelect::EnRole role, int characterIndex)
		{
			Vector3 initPosition = m_characterFrameUI[characterFrameUI].GetPosition();
			initPosition.z += 1000.0f;

			std::string playerNo = std::to_string(characterIndex);

			switch (role)
			{
			case RoleSelect::EnRole::enRole_Sword:
				m_characterModel[characterFrameUI][role] = NewGO<nsActor::SwordCharacter>(0, ("player" + playerNo).c_str());
				m_characterModel[characterFrameUI][role]->SetPosition(initPosition);
				m_characterModel[characterFrameUI][role]->SetAngle(145.0f);
				break;
			case RoleSelect::EnRole::enRole_Hammer:
				m_characterModel[characterFrameUI][role] = NewGO<nsActor::HammerCharacter>(0, ("player" + playerNo).c_str());
				m_characterModel[characterFrameUI][role]->SetPosition(initPosition);
				m_characterModel[characterFrameUI][role]->SetAngle(155.0f);
				break;
			case RoleSelect::EnRole::enRole_Mage:
				m_characterModel[characterFrameUI][role] = NewGO<nsActor::WandCharacter>(0, ("player" + playerNo).c_str());
				m_characterModel[characterFrameUI][role]->SetPosition(initPosition);
				m_characterModel[characterFrameUI][role]->SetAngle(165.0f);
				break;
			case RoleSelect::EnRole::enRole_Gunner:
				m_characterModel[characterFrameUI][role] = NewGO<nsActor::TwinGunCharacter>(0, ("player" + playerNo).c_str());
				m_characterModel[characterFrameUI][role]->SetPosition(initPosition);
				m_characterModel[characterFrameUI][role]->SetAngle(175.0f);
				break;
			default:
				break;
			}
		}

		/*UIを左にスライドさせるアニメーションのリセット処理。*/
		void MemberSelect::ResetSlideLeftUIAnimation()
		{
			for (int i = 0; i < m_slideLeftUIAnimation.size(); i++)
			{
				m_slideLeftUIAnimation[i]->Reset();
			}
		}

		/*UIを右にスライドさせるアニメーションのリセット処理。*/
		void MemberSelect::ResetSlideRightUIAnimation()
		{
			for (int i = 0; i < m_slideRightUIAnimation.size(); i++)
			{
				m_slideRightUIAnimation[i]->Reset();
			}
		}

		/*UIの透明度を変えるアニメーションのリセット処理。*/
		void MemberSelect::ResetAlphaUIAnimation()
		{
			for (int i = 0; i < m_alphaUIAnimationSprite.size(); i++)
			{
				m_alphaUIAnimation[i]->Reset();
			}
		}

		/*選択したときの演出UIアニメーションのリセット処理(開始)。*/
		void MemberSelect::ResetSelectStartDirectionUIAnimation()
		{
			for (int i = 0; i < m_selectStartDirectionUIAnimation.size(); i++)
			{
				m_selectStartDirectionUIAnimation[i]->Reset();
			}
		}

		/*選択したときの演出UIアニメーションのリセット処理(終了)。*/
		void MemberSelect::ResetSelectEndDirectionUIAnimation()
		{
			for (int i = 0; i < m_selectEndDirectionUIAnimation.size(); i++)
			{
				m_selectEndDirectionUIAnimation[i]->Reset();
			}
		}

		/*全キャラクターモデルを無効化する。*/
		void MemberSelect::AllCharacterModelDeactivate()
		{
			for (int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				for (int j = 0; j < RoleSelect::EnRole::enRole_Num; j++)
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

					/*プレイヤーとNPCの区別。*/
					int player = m_currentSelect + 1;
					int npc = m_currentSelect + 4;
					if (m_currentSelect != enSelect_Deploy)
					{
						m_isPlayerControle[m_currentSelect] == true ?
							currentTargetUIFeature = player :/*プレイヤー。*/
							currentTargetUIFeature = npc;/*NPC。*/
					}

					if (!m_selectStartDirectionUIAnimation[currentTargetUIFeature]->IsEnd())
					{
						m_selectStartDirectionUIAnimation[currentTargetUIFeature]->Update();
						m_selectStartDirectionUIAnimation[currentSelectUIFeature]->Update();
					}
					else if (!m_selectEndDirectionUIAnimation[currentTargetUIFeature]->IsEnd())
					{
						m_selectEndDirectionUIAnimation[currentTargetUIFeature]->Update();
						m_selectEndDirectionUIAnimation[currentSelectUIFeature]->Update();
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
					if (m_currentSlide == enSlide_Left)
					{
						for (int i = 0; i < m_slideLeftUIAnimation.size(); i++)
						{
							m_slideLeftUIAnimation[i]->Update();
						}
					}
					else
					{
						for (int j = 0; j < m_slideRightUIAnimation.size(); j++)
						{
							m_slideRightUIAnimation[j]->Update();
						}
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
				if (m_isPlayerControle[m_currentSelect])
				{
					m_characterSelectUI.SetPosition(m_characterFrameUI[m_currentSelect].GetPosition());
				}
				else
				{
					m_characterSelectUI.SetPosition(m_npcCharacterFrameUI[m_currentSelect - 1].GetPosition());
				}
			}
			m_characterSelectUI.Update();

			/*キャラクター枠UI。*/
			for(int i = 0; i < enCharacterFrameUI_Num; i++)
			{
				if (m_isPlayerControle[i])
				{
					m_characterFrameUI[i].Update();
				}
				else
				{
					m_npcCharacterFrameUI[i].Update();
				}

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

				for (int j = 0; j < RoleSelect::EnRole::enRole_Num; j++)
				{
					m_characterModel[i][j]->GetCharacterController().SetPosition(currentPosition);
					m_characterModel[i][j]->SetPosition(currentPosition);

					/*選択したときの演出UIアニメーションが終わっていないときモデルの処理を有効化する。*/
					if (!IsEndSelectEndDirectionUIAnimation(GetCurrentSelect() + 1))
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
				if (IsEndSelectEndDirectionUIAnimation(GetCurrentSelect() + 1))
				{
					m_characterModel[m_currentSelect][m_currentRole[m_currentSelect]]->Deactivate();
				}
			}
		}
	}
}