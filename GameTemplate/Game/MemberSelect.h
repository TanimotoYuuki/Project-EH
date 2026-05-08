#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file MemberSelect.h。
 * @brief メンバー選択クラス。
 * @author Tanimoto。
 * @date 2026/03/07。
 */
namespace nsApp
{
	namespace nsSelect
	{
		/*メンバー選択クラス。*/
		class MemberSelect : public IGameObject
		{
		public:
			MemberSelect() {};/*コンストラクタ。*/
			~MemberSelect() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc);/*描画処理。*/

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

			/*UIをスライドさせるアニメーションに適用するスプライト。*/
			enum EnSlideUIAnimationSprite : uint8_t
			{
				enSlideUIAnimationSprite_CharacterSelectUI,/*キャラクター選択UI。*/
				enSlideUIAnimationSprite_OneCharacterFrameUI,/*1人目のキャラクター枠UI。*/
				enSlideUIAnimationSprite_TwoCharacterFrameUI,/*2人目のキャラクター枠UI。*/
				enSlideUIAnimationSprite_ThreeCharacterFrameUI,/*3人目のキャラクター枠UI。*/
				enSlideUIAnimationSprite_FourCharacterFrameUI,/*4人目のキャラクター枠UI。*/
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
			* @brief 出撃選択UIの初期化。
			*/
			void InitDeploySelectUI();

			/**
			* @brief 出撃テキストUIの初期化。
			*/
			void InitDeployTextUI();

			/**
			* @brief UIアニメーションの初期化。
			*/
			void InitUIAnimation();

			/**
			* @brief UIをスライドさせるアニメーションの初期化。
			*/
			void InitSlideUIAnimation();

			/**
			* @brief UIの透明度を変えるアニメーションの初期化。
			*/
			void InitAlphaUIAnimation();

			/**
			* @brief 選択の更新処理。
			*/
			void UpdateSelect();

			/**
			* @brief 演出の更新処理。
			*/
			void UpdateDirection();

			/**
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

		public:/*メンバ関数。*/

			/**
			* @brief UIをスライドさせるアニメーションのリセット処理。
			* @param slide スライドさせる方向。
			*/
			void ResetSlideUIAnimation(EnSlide slide);

			/**
			* @brief UIの透明度を変えるアニメーションのリセット処理。
			*/
			void ResetAlphaUIAnimation();

			/**
			* @brief 選択のリセット処理。
			*/
			inline void ResetSelect()
			{
				m_currentSelect = enSelect_OneCharacter;
				m_characterSelectUI.SetPosition(m_characterFrameUI[m_currentSelect].GetPosition());
				m_characterSelectUI.Update();
			}

			/**
			* @brief UIをスライドさせるアニメーションの再生終了したか？
			* @param slide スライドさせる方向。
			* @param slideUIAnimationSprite アニメーションさせるスプライト。
			* @return trueなら再生終了している。
			*/
			bool IsEndSlideUIAnimation(EnSlide slide, EnSlideUIAnimationSprite slideUIAnimationSprite) const
			{
				return m_slideUIAnimation[slide][slideUIAnimationSprite]->IsEnd();
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

		private:/*メンバ変数。*/
			SpriteRender m_memberSelectTextUI;/*メンバー選択テキストUI。*/
			SpriteRender m_characterSelectUI;/*キャラクター選択UI。*/
			SpriteRender m_characterFrameUI[enCharacterFrameUI_Num];/*キャラクター枠UI。*/
			SpriteRender m_deploySelectUI;/*出撃選択UI。*/
			SpriteRender m_deployTextUI;/*出撃テキストUI。*/
			int m_currentSlide = enSlide_Left;/*現在のスライドさせる方向。*/
			int m_currentSelect = enSelect_OneCharacter;/*現在の選択している内容。*/
			int m_previousSelect = enSelect_Num;/*前に選択していた内容。*/
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_slideUIAnimation[enSlide_Num][enSlideUIAnimationSprite_Num];/*UIをスライドさせるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation[enAlphaUIAnimationSprite_Num];/*透明度を変えるアニメーション。*/
			bool m_isDirection = false;/*演出中？*/
			bool m_didSelect = false;/*選択できたか？*/
			bool m_isBackSelect = false;/*前の選択に戻るか？*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_memberSelectTextUIFilePath = "Assets/sprite/select/memberSelect/text/redBer/memberSelect.dds";/*メンバー選択テキストUIのファイルパス。*/
			std::string m_characterSelectUIFilePath = "Assets/sprite/select/memberSelect/select/characterSelect.dds";/*キャラクター選択UIのファイルパス。*/
			std::string m_characterFrameUIFilePath[enCharacterFrameUI_Num] = {
				"Assets/sprite/select/memberSelect/characterFrame/onePlayer.dds",
				"Assets/sprite/select/memberSelect/characterFrame/twoNpc.dds",
				"Assets/sprite/select/memberSelect/characterFrame/threeNpc.dds",
				"Assets/sprite/select/memberSelect/characterFrame/fourNpc.dds"
			};/*キャラクター枠UIのファイルパス。*/
			std::string m_deploySelectUIFilePath = "Assets/sprite/select/memberSelect/select/deploySelect.dds";/*出撃選択UIのファイルパス。*/
			std::string m_deployTextUIFilePath = "Assets/sprite/select/memberSelect/text/redFrame/deploy.dds";/*出撃テキストUIのファイルパス。*/
		};
	}
}

