#pragma once
#include "Src/UIAnimation/UIAnimation.h"
#include "RoleSelect.h"
/**
 * @file MemberSelect.h。
 * @brief メンバー選択クラス。
 * @author Tanimoto。
 * @date 2026/03/07。
 */
namespace nsApp
{
	namespace nsActor
	{
		class IWeaponCharacter;
	}

	namespace nsSelect
	{
		/*メンバー選択クラス。*/
		class MemberSelect : public IGameObject
		{
		public:
			MemberSelect() {};/*コンストラクタ。*/
			~MemberSelect();/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc);/*描画処理。*/

			void ReleaseAllCharacterModels();

		public:/*列挙型。*/

			/*キャラクター枠UI。*/
			enum EnCharacterFrameUI : uint8_t
			{
				enCharacterFrameUI_One,/*1人目のキャラクター枠。*/
				enCharacterFrameUI_Two,/*2人目のキャラクター枠。*/
				enCharacterFrameUI_Three,/*3人目のキャラクター枠。*/
				enCharacterFrameUI_Four,/*4人目のキャラクター枠。*/
				enCharacterFrameUI_Num/*キャラクター枠数。*/
			};

			/*NPC用のキャラクター枠UI。*/
			enum EnNpcCharacterFrameUI : uint8_t
			{
				enNpcCharacterFrameUI_Two,/*2人目のキャラクター枠。*/
				enNpcCharacterFrameUI_Three,/*3人目のキャラクター枠。*/
				enNpcCharacterFrameUI_Four,/*4人目のキャラクター枠。*/
				enNpcCharacterFrameUI_Num/*NPC用のキャラクター枠数。*/
			};

			/*ボタンUI。*/
			enum EnButtonUI : uint8_t
			{
				enButtonUI_A,/*Aボタン。*/
				enButtonUI_B,/*Bボタン。*/
				enButtonUI_X,/*Xボタン。*/
				enButtonUI_Y,/*Yボタン。*/
				enButtonUI_Num/*ボタンUI数。*/
			};

			/*テキストUI。*/
			enum EnTextUI : uint8_t
			{
				enTextUI_Decision,/*決定。*/
				enTextUI_Return,/*戻る。*/
				enTextUI_GameOption,/*ゲームオプション。*/
				enTextUI_HowToPlay,/*操作方法。*/
				enTextUI_Num/*テキストUI数、*/
			};

			/*スライドさせる方向。*/
			enum EnSlide : uint8_t
			{
				enSlide_Left,/*左。*/
				enSlide_Right,/*右。*/
				enSlide_Num/*スライドさせる方向数。*/
			};

			/*選択している内容。*/
			enum EnSelect : uint8_t
			{
				enSelect_OneCharacter,/*1人目のキャラクター。*/
				enSelect_TwoCharacter,/*2人目のキャラクター。*/
				enSelect_ThreeCharacter,/*3人目のキャラクター。*/
				enSelect_FourCharacter,/*4人目のキャラクター。*/
				enSelect_Deploy,/*出撃。*/
				enSelect_Num/*選択リスト数。*/
			};

			/*選択している内容の特徴。*/
			enum EnSelectFauture : uint8_t
			{
				enSelectFauture_Character,/*キャラクター。*/
				enSelectFauture_Deploy,/*出撃。*/
				enSelectFauture_Num/*選択している内容の特徴数。*/
			};

			/*位置。*/
			enum EnPosition : uint8_t
			{
				enPosition_Down,/*下降。*/
				enPosition_Up,/*上昇。*/
				enPosition_Num/*位置を変更する種類。*/
			};

			/*UIをスライドさせるアニメーションに適用するスプライト。*/
			enum EnSlideUIAnimationSprite : uint8_t
			{
				enSlideUIAnimationSprite_CharacterSelectUI,/*キャラクター選択UI。*/
				enSlideUIAnimationSprite_OneCharacterFrameUI,/*1人目のキャラクター枠UI。*/
				enSlideUIAnimationSprite_TwoCharacterFrameUI,/*2人目のキャラクター枠UI。*/
				enSlideUIAnimationSprite_ThreeCharacterFrameUI,/*3人目のキャラクター枠UI。*/
				enSlideUIAnimationSprite_FourCharacterFrameUI,/*4人目のキャラクター枠UI。*/
				enSlideUIAnimationSprite_TwoNpcCharacterFrameUI,/*NPC用の2人目のキャラクタ枠UI。*/
				enSlideUIAnimationSprite_ThreeNpcCharacterFrameUI,/*NPC用の3人目のキャラクタ枠UI。*/
				enSlideUIAnimationSprite_FourNpcCharacterFrameUI,/*NPC用の4人目のキャラクタ枠UI。*/
				enSlideUIAnimationSprite_DeploySelectUI,/*出撃選択UI。*/
				enSlideUIAnimationSprite_DeployTextUI,/*出撃テキストUI。*/
				enSlideUIAnimationSprite_Num/*UIアニメーションさせるスプライト数。*/
			};

			/*UIの透明度を変えるアニメーションに適用するスプライト。*/
			enum EnAlphaUIAnimationSprite : uint8_t
			{
				enAlphaUIAnimationSprite_CharacterSelectUI,/*キャラクター選択UI。*/
				enAlphaUIAnimationSprite_DeploySelectUI,/*出撃選択UI。*/
				enAlphaUIAnimationSprite_Num/*アニメーションさせるスプライト数。*/
			};

			/*選択したときの演出UIアニメーションに適用するスプライト。*/
			enum EnSelectDirectionUIAnimationSprite : uint8_t
			{
				enSelectDirectionUIAnimationSprite_CharacterSelectUI,/*キャラクター選択UI。*/
				enSelectDirectionUIAnimationSprite_OneCharacterFrameUI,/*1人目のキャラクター枠UI。*/
				enSelectDirectionUIAnimationSprite_TwoCharacterFrameUI,/*2人目のキャラクター枠UI。*/
				enSelectDirectionUIAnimationSprite_ThreeCharacterFrameUI,/*3人目のキャラクター枠UI。*/
				enSelectDirectionUIAnimationSprite_FourCharacterFrameUI,/*4人目のキャラクター枠UI。*/
				enSelectDirectionUIAnimationSprite_TwoNpcCharacterFrameUI,/*NPC用の2人目のキャラクタ枠UI。*/
				enSelectDirectionUIAnimationSprite_ThreeNpcCharacterFrameUI,/*NPC用の3人目のキャラクタ枠UI。*/
				enSelectDirectionUIAnimationSprite_FourNpcCharacterFrameUI,/*NPC用の4人目のキャラクタ枠UI。*/
				enSelectDirectionUIAnimationSprite_DeploySelectUI,/*出撃選択UI。*/
				enSelectDirectionUIAnimationSprite_DeployTextUI,/*出撃テキストUI。*/
				enSelectDirectionUIAnimationSprite_Num/*適用するスプライト数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief メンバー選択テキストUIの初期化。
			*/
			void InitMemberSelectTextUI();

			/**
			* @brief キャラクター選択UIの初期化。
			*/
			void InitCharacterSelectUI();

			/**
			* @brief キャラクター枠UIの初期化。
			* @param characterFrameUI キャラクター枠UIの種類。
			* @param characterIndex キャラクターのインデックス。0なら1人目のキャラクター枠、1なら2人目のキャラクター枠、2なら3人目のキャラクター枠、3なら4人目のキャラクター枠。
			*/
			void InitCharacterFrameUI(EnCharacterFrameUI characterFrameUI,int characterIndex);

			/**
			* @brief NPC用のキャラクター枠UIの初期化。
			* @param npcCharacterFrameUI NPC用のキャラクター枠UIの種類。
			*/
			void InitNpcCharacterFrameUI(EnNpcCharacterFrameUI npcCharacterFrameUI);

			/**
			* @brief 役職アイコンUIの初期化。
			* @param characterFrameUI キャラクター枠UIの種類。
			* @param role 役職アイコンUIの初期化。
			*/
			void InitRoleIconUI(EnCharacterFrameUI characterFrameUI, RoleSelect::EnRole role);

			/**
			* @brief キャラクターフェイドUIの初期化。
			*/
			void InitCharacterFadeUI();

			/**
			* @brief 出撃選択UIの初期化。
			*/
			void InitDeploySelectUI();

			/**
			* @brief 出撃テキストUIの初期化。
			*/
			void InitDeployTextUI();

			/**
			* @brief ボタンUIの初期化。
			* @param buttonUI ボタンUIの種類。
			*/
			void InitButtonUI(EnButtonUI buttonUI);

			/**
			* @brief テキストUIの初期化。
			* @param textUI テキストUIの種類。
			*/
			void InitTextUI(EnTextUI textUI);

			/**
			* @brief UIアニメーションの初期化。
			*/
			void InitUIAnimation();

			/**
			* @brief UIを左にスライドさせるアニメーションの初期化。
			* @param spriteData　UIを左にスライドさせるアニメーション用のスプライトデータ。
			*/
			void InitSlideLeftUIAnimation(SpriteRender* spriteData);

			/**
			* @brief UIを右にスライドさせるアニメーションの初期化。
			* @param spriteData　UIを右にスライドさせるアニメーション用のスプライトデータ。
			*/
			void InitSlideRightUIAnimation(SpriteRender* spriteData);

			/**
			* @brief UIの透明度を変えるアニメーションの初期化。
			*/
			void InitAlphaUIAnimation(SpriteRender* spriteData);

			/**
			* @brief 選択したときの演出UIアニメーションの初期化(開始)。
			* @param spriteData　選択したときの演出UIアニメーション用のスプライトデータ(開始)。
			*/
			void InitSelectStartDirectionUIAnimation(SpriteRender* spriteData);

			/**
			* @brief 選択したときの演出UIアニメーションの初期化(終了)。
			* @param spriteData　選択したときの演出UIアニメーション用のスプライトデータ(終了)。
			*/
			void InitSelectEndDirectionUIAnimation(SpriteRender* spriteData);

			/**
			* @brief キャラクターモデルの生成。
			* @param characterFrameUI キャラクター枠UIの種類。
			* @param role 役職の種類。
			* @param characterIndex キャラクターのインデックス。
			*/
			void CreateCharacterModel(EnCharacterFrameUI characterFrameUI, RoleSelect::EnRole role, int characterIndex);

			/**
			* @brief 選択の更新処理。
			*/
			void UpdateSelect();

			/**
			* @brief UIアニメーションの更新処理。
			*/
			void UpdateUIAnimation();

			/**
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

			/**
			* @brief キャラクターモデルの更新処理。
			*/
			void UpdateCharacterModel();

		public:/*メンバ関数。*/

			/**
			* @brief UIを左にスライドさせるアニメーションのリセット処理。
			*/
			void ResetSlideLeftUIAnimation();

			/**
			* @brief UIを右にスライドさせるアニメーションのリセット処理。
			*/
			void ResetSlideRightUIAnimation();

			/**
			* @brief UIの透明度を変えるアニメーションのリセット処理。
			*/
			void ResetAlphaUIAnimation();

			/**
			* @brief 選択したときの演出UIアニメーション(開始)のリセット処理。
			*/
			void ResetSelectStartDirectionUIAnimation();

			/**
			* @breif 選択したときの演出UIアニメーション(終了)のリセット処理。
			*/
			void ResetSelectEndDirectionUIAnimation();

			/**
			* @brief 選択のリセット処理。
			*/
			void ResetSelect()
			{
				m_currentSelect = enSelect_OneCharacter;
				if (m_isPlayerControle[m_currentSelect])
				{
					m_characterSelectUI.SetPosition(m_characterFrameUI[m_currentSelect].GetPosition());
				}
				else
				{
					m_characterSelectUI.SetPosition(m_npcCharacterFrameUI[m_currentSelect - 1].GetPosition());
				}
				m_characterSelectUI.Update();
			}

			/**
			 * @brief 全キャラクターモデルを無効化する。
			 */
			void AllCharacterModelDeactivate();

			/**
			* @brief UIを左にスライドさせるアニメーション再生終了したか？
			* @return trueなら終了している。
			*/
			inline bool IsEndSlideLeftUIAnimation() const
			{
				return m_slideLeftUIAnimation[0]->IsEnd();
			}
			inline bool IsEndSlideLeftUIAnimation(int index) const
			{
				return m_slideLeftUIAnimation[index]->IsEnd();
			}

			/**
			* @brief UIを右にスライドさせるアニメーション再生終了したか？
			* @return trueなら終了している。
			*/
			inline bool IsEndSlideRightUIAnimation() const
			{
				return m_slideRightUIAnimation[0]->IsEnd();
			}
			inline bool IsEndSlideRightUIAnimation(int index) const
			{
				return m_slideRightUIAnimation[index]->IsEnd();
			}

			/**
			* @brief UIの透明度を変えるアニメーションの再生終了したか？
			* @param alphaUIAnimationSprite アニメーションさせるスプライト。
			* @return trueなら再生終了している。
			*/
			bool IsEndAlphaUIAnimation(EnAlphaUIAnimationSprite alphaUIAnimationSprite) const
			{
				return m_alphaUIAnimation[alphaUIAnimationSprite]->IsEnd();
			}

			/**
			* @brief 選択したときの演出UIアニメーション(開始)再生終了したか？
			* @return trueなら再生終了している。
			*/
			inline bool IsEndSelectStartDirectionUIAnimation() const
			{
				return m_selectStartDirectionUIAnimation[0]->IsEnd();
			}
			inline bool IsEndSelectStartDirectionUIAnimation(int index) const
			{
				return m_selectStartDirectionUIAnimation[index]->IsEnd();
			}

			/**
			* @brief 選択したときの演出UIアニメーション(終了)再生終了したか？
			* @return trueなら再生終了している。
			*/
			inline bool IsEndSelectEndDirectionUIAnimation() const
			{
				return m_selectEndDirectionUIAnimation[0]->IsEnd();
			}
			inline bool IsEndSelectEndDirectionUIAnimation(int index) const
			{
				return m_selectEndDirectionUIAnimation[index]->IsEnd();
			}

			/**
			* @brief 役職アイコンUIの変更。
			* @param characterFrameUI キャラクター枠UIの種類。
			* @param role 役職の種類。
			*/
			inline void ChangeRoleIconUI(EnCharacterFrameUI characterFrameUI, RoleSelect::EnRole role)
			{
				m_currentRole[characterFrameUI] = role;
			}

			/**
			* @brief 現在の選択している内容の取得。
			* @return 現在の選択している内容。
			*/
			inline EnSelect GetCurrentSelect() 
			{ 
				return (EnSelect)m_currentSelect; 
			};

			/**
			* @brief 現在選択しているキャラクター枠UIの役割の取得。
			* @param characterFrameUI キャラクター枠UIの種類。
			* @return 現在選択しているキャラクター枠UIの役割。
			*/
			inline nsApp::nsSelect::RoleSelect::EnRole GetCurrentRole(EnCharacterFrameUI characterFrameUI)
			{
				return (nsApp::nsSelect::RoleSelect::EnRole)m_currentRole[characterFrameUI];
			}

			/**
			* @brief 現在選択しているキャラクター枠UIの位置の取得。
			* @return 現在選択しているキャラクター枠UIの位置。
			*/
			inline const Vector3& GetCurrentSelectCharacterFrameUIPosition() const
			{
				return m_characterFrameUI[m_currentSelect].GetPosition();
			}

			/**
			* @brief プレイヤーがキャラクターを操作する状態かどうかを取得。
			* @return trueならプレイヤーがキャラクターを操作する状態。
			*/
			inline bool IsPlayerControle(int playerIndex) const
			{
				return m_isPlayerControle[playerIndex];
			}

			/**
			* @brief 演出できる状態にする。
			*/
			inline void EnableDirection()
			{
				m_isDirection = true;
			}

			/**
			* @brief 演出できない状態にする。
			*/
			inline void DisableDirection()
			{
				m_isDirection = false;
			}

			/**
			* @brief 演出中？
			* @return trueなら演出中。
			*/
			inline bool IsDirection() const
			{
				return m_isDirection;
			}

			/**
			* @brief スライドさせる方向の変更。
			*/
			inline void ChangeSlide(EnSlide slide)
			{
				m_currentSlide = slide;
			}

			/**
			* @brief 選択できる状態にする。
			*/
			inline void EnableSelect()
			{
				m_didSelect = true;
			}

			/**
			* @brief 選択できない状態にする。
			*/
			inline void DisableSelect()
			{
				m_didSelect = false;
			}

			/**
			* @brief 選択できたか？
			* @return trueなら選択できている。
			*/
			inline bool DidSelect() const
			{
				return m_didSelect;
			}

			/**
			* @brief 前の選択に戻る状態にする。
			*/
			inline void EnableBackSelect()
			{
				m_isBackSelect = true;
			}

			/**
			* @brief 前の選択に戻らない状態にする。
			*/
			inline void DisableBackSelect()
			{
				m_isBackSelect = false;
			}

			/**
			* @brief 前の選択に戻るか？
			* @return trueなら前の選択に戻る。
			*/
			inline bool IsBackSelect() const
			{
				return m_isBackSelect;
			}

			/**
			* @brief ゲーム設定ボタンを押した状態にする。
			*/
			inline void EnablePushGameOptionButton()
			{
				m_isPushGameOptionButton = true;
			}

			/**
			* @brief ゲーム設定ボタンを押していない状態にする。
			*/
			inline void DisablePushGameOptionButton()
			{
				m_isPushGameOptionButton = false;
			}

			/**
			* @brief ゲーム設定ボタンを押したか？
			* @return trueならゲーム設定ボタンを押している。
			*/
			inline bool IsPushGameOptionButton() const
			{
				return m_isPushGameOptionButton;
			}

			/**
			* @brief 操作方法ボタンを押した状態にする。
			*/
			inline void EnablePushHowToPlayButton()
			{
				m_isPushHowToPlayButton = true;
			}

			/**
			* @brief 操作方法ボタンを押していない状態にする。
			*/
			inline void DisablePushHowToPlayButton()
			{
				m_isPushHowToPlayButton = false;
			}

			/**
			* @brief 操作方法ボタンを押したか？
			* @return trueなら操作方法ボタンを押している。
			*/
			inline bool IsPushHowToPlayButton() const
			{
				return m_isPushHowToPlayButton;
			}

			/**
			* @brief ボタンUIとテキストUIを描画する状態にする。
			*/
			inline void EnableDrawingButtonAndTextUI()
			{
				m_isDrawingButtonAndTextUI = true;
			}

			/**
			* @brief ボタンUIとテキストUIを描画しない状態にする。
			*/
			inline void DisableDrawingButtonAndTextUI()
			{
				m_isDrawingButtonAndTextUI = false;
			}

			/**
			* @brief ボタンUIとテキストUIを描画するか？
			* @return trueならボタンUIとテキストUIを描画する。
			*/
			inline bool IsDrawingButtonAndTextUI() const
			{
				return m_isDrawingButtonAndTextUI;
			}

		private:/*メンバ変数。*/
			SpriteRender m_memberSelectTextUI;/*メンバー選択テキストUI。*/
			SpriteRender m_characterSelectUI;/*キャラクター選択UI。*/
			SpriteRender m_characterFrameUI[enCharacterFrameUI_Num];/*キャラクター枠UI。*/
			SpriteRender m_npcCharacterFrameUI[enNpcCharacterFrameUI_Num];/*NPC用のキャラクター枠UI。*/
			SpriteRender m_roleIconUI[enCharacterFrameUI_Num][RoleSelect::EnRole::enRole_Num];/*役割アイコンUI。*/
			SpriteRender m_characterFadeUI;/*キャラクターフェイドUI。*/
			SpriteRender m_deploySelectUI;/*出撃選択UI。*/
			SpriteRender m_deployTextUI;/*出撃テキストUI。*/
			SpriteRender m_buttonUI[enButtonUI_Num];/*ボタンUI。*/
			SpriteRender m_textUI[enTextUI_Num];/*テキストUI。*/
			int m_currentSlide = enSlide_Left;/*現在のスライドさせる方向。*/
			int m_currentSelect = enSelect_OneCharacter;/*現在の選択している内容。*/
			int m_previousSelect = enSelect_Num;/*前に選択していた内容。*/
			int m_currentRole[enCharacterFrameUI_Num] = { RoleSelect::EnRole::enRole_Sword };/*現在の役割。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideLeftUIAnimation;/*UIを左にスライドさせるアニメーション。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideRightUIAnimation;/*UIを右にスライドさせるアニメーション。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_selectStartDirectionUIAnimation;/*選択したときの演出UIアニメーション(開始)。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_selectEndDirectionUIAnimation;/*選択したときの演出UIアニメーション(終了)。*/
			std::vector<std::unique_ptr<nsApp::nsUI::AlphaUIAnimation>> m_alphaUIAnimation;/*UIの透明度を変えるアニメーション。*/
			std::vector<SpriteRender*> m_slideUIAnimationSprite;/*UIをスライドさせるアニメーション用のスプライト。*/
			std::vector<SpriteRender*> m_selectDirectionUIAnimationSprite;/*選択したときの演出UIアニメーション用のスプライト。*/
			std::vector<SpriteRender*> m_alphaUIAnimationSprite;/*UIの透明度を変えるアニメーション用のスプライト。*/
			nsActor::IWeaponCharacter* m_characterModel[enCharacterFrameUI_Num][RoleSelect::EnRole::enRole_Num];/*キャラクターモデル。*/
			bool m_isPlayerControle[enCharacterFrameUI_Num] = { true,true,true,true };/*プレイヤーがキャラクターを操作するか？*/
			bool m_isDirection = false;/*演出中？*/
			bool m_didSelect = false;/*選択できたか？*/
			bool m_isBackSelect = false;/*前の選択に戻るか？*/
			bool m_isPushGameOptionButton = false;/*ゲーム設定ボタンを押したか？*/
			bool m_isPushHowToPlayButton = false;/*操作方法ボタンを押したか？*/
			bool m_isDrawingButtonAndTextUI = false;/*ボタンUIとテキストUIを描画するか？*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_memberSelectTextUIFilePath = "Assets/sprite/select/memberSelect/text/redBer/memberSelect.dds";/*メンバー選択テキストUIのファイルパス。*/
			std::string m_characterSelectUIFilePath = "Assets/sprite/select/memberSelect/select/characterSelect.dds";/*キャラクター選択UIのファイルパス。*/
			std::string m_characterFrameUIFilePath[enCharacterFrameUI_Num] = {
				"Assets/sprite/select/memberSelect/characterFrame/onePlayer.dds",
				"Assets/sprite/select/memberSelect/characterFrame/twoPlayer.dds",
				"Assets/sprite/select/memberSelect/characterFrame/threePlayer.dds",
				"Assets/sprite/select/memberSelect/characterFrame/fourPlayer.dds"
			};/*キャラクター枠UIのファイルパス。*/
			std::string m_npcCharacterFrameUIFilePath[enNpcCharacterFrameUI_Num] = {
				"Assets/sprite/select/memberSelect/characterFrame/twoNpc.dds",
				"Assets/sprite/select/memberSelect/characterFrame/threeNpc.dds",
				"Assets/sprite/select/memberSelect/characterFrame/fourNpc.dds"
			};/*NPC用のキャラクター枠UIのファイルパス。*/
			std::string m_roleIconUIFilePath[enCharacterFrameUI_Num][RoleSelect::EnRole::enRole_Num] = {
				"Assets/sprite/role/sword.dds", "Assets/sprite/role/hammer.dds", "Assets/sprite/role/mage.dds", "Assets/sprite/role/gunner.dds",
				"Assets/sprite/role/sword.dds", "Assets/sprite/role/hammer.dds", "Assets/sprite/role/mage.dds", "Assets/sprite/role/gunner.dds",
				"Assets/sprite/role/sword.dds", "Assets/sprite/role/hammer.dds", "Assets/sprite/role/mage.dds", "Assets/sprite/role/gunner.dds",
				"Assets/sprite/role/sword.dds", "Assets/sprite/role/hammer.dds", "Assets/sprite/role/mage.dds", "Assets/sprite/role/gunner.dds",
			};/*役割アイコンUIのファイルパス。*/
			std::string m_characterFadeUIFilePath = "Assets/sprite/select/memberSelect/fade/characterFade.dds";/*キャラクターフェイドUIのファイルパス。*/
			std::string m_deploySelectUIFilePath = "Assets/sprite/select/memberSelect/select/deploySelect.dds";/*出撃選択UIのファイルパス。*/
			std::string m_deployTextUIFilePath = "Assets/sprite/select/memberSelect/text/redFrame/deploy.dds";/*出撃テキストUIのファイルパス。*/
			std::string m_buttonUIFilePath[enButtonUI_Num] = {
				"Assets/sprite/gamePad/button/aButton.dds",
				"Assets/sprite/gamePad/button/bButton.dds",
				"Assets/sprite/gamePad/button/xButton.dds",
				"Assets/sprite/gamePad/button/yButton.dds"
			};/*ボタンUIのファイルパス。*/
			std::string m_textUIFilePath[enTextUI_Num] = {
				"Assets/sprite/select/memberSelect/text/decision.dds",
				"Assets/sprite/select/memberSelect/text/return.dds",
				"Assets/sprite/select/memberSelect/text/gameOption.dds",
				"Assets/sprite/select/memberSelect/text/howToPlay.dds"
			};/*テキストUIのファイルパス。*/
		};
	}
}

