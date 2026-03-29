#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file ConfirmationSelect.h。
 * @brief 確認選択クラス。
 * @author Tanimoto。
 * @date 2026/03/18。
 */
namespace nsApp
{
	namespace nsSelect
	{
		/*確認選択クラス。*/
		class ConfirmationSelect : public IGameObject
		{
		public:
			ConfirmationSelect() {};/*コンストラクタ。*/
			~ConfirmationSelect() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*確認選択テキストUI。*/
			enum EnConfirmationSelectTextUI : uint8_t
			{
				enConfirmationSelectTextUI_No,/*いいえ。*/
				enConfirmationSelectTextUI_Yes,/*はい。*/
				enConfirmationSelectTextUI_Num,/*確認選択テキスト数。*/
			};

			/*選択している内容。*/
			enum EnSelect : uint8_t
			{
				enSelect_No,/*いいえ。*/
				enSelect_Yes,/*はい。*/
				enSelect_Num/*選択リスト数。*/
			};

			/*UIの透明度を変えるアニメーションに適用するスプライト。*/
			enum EnAlphaUIAnimationSprite : uint8_t
			{
				enAlphaUIAnimationSprite_No,/*いいえ。*/
				enAlphaUIAnimationSprite_Yew,/*はい。*/
				enAlphaUIAnimationSprite_Num/*アニメーションさせるスプライト数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief 確認UIの初期化。
			*/
			void InitConfirmationUI();

			/**
			* @brief 確認選択UIの初期化。
			*/
			void InitConfirmationSelectUI();

			/**
			* @brief 確認選択テキストUIの初期化。
			* @param confirmationSelectTextUI 確認選択テキストUIの種類。
			* @param confirmationSelectTextIndex 確認選択テキストUIのインデックス。
			*/
			void InitConfirmationSelectTextUI(EnConfirmationSelectTextUI confirmationSelectTextUI,int confirmationSelectTextIndex);

			/**
			* @brief UIアニメーションの初期化。
			*/
			void InitUIAnimation();

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

		public:/*メンバ関数。*/

			/**
			* @brief UIの透明度を変えるアニメーションのリセット処理。
			*/
			inline void ResetAlphaUIAnimation()
			{
				m_alphaUIAnimation->Reset();
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
			* @brief UIを描画する状態にする。
			*/
			inline void EnableDrawingUI() 
			{
				m_isDrawingUI = true; 
			}

			/**
			* @brief UIを描画しない状態にする。
			*/
			inline void DisableDrawingUI()
			{ 
				m_isDrawingUI = false; 
			}

			/**
			* @brief UIを描画しているか？
			* @return trueならUIを描画している。
			*/
			inline bool IsDrawingUI()
			{ 
				return m_isDrawingUI;
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

		private:/*メンバ変数。*/
			SpriteRender m_confirmationUI;/*確認UI。*/
			SpriteRender m_confirmationSelectUI;/*確認選択UI。*/
			SpriteRender m_confirmationSelectTextUI[enConfirmationSelectTextUI_Num];/*確認選択テキストUI。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation;/*透明度を変えるアニメーション。*/
			int m_currentSelect = enSelect_No;/*現在の選択している内容。*/
			bool m_isDrawingUI = false;/*UIを描画しているか？*/
			bool m_didSelect = false;/*選択できたか？*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_confirmationUIFilePath = "Assets/sprite/select/confirmationSelect/confirmation/confirmation.dds";/*ダメージゲージUIのファイルパス。*/
			std::string m_confirmationSelectUIFilePath = "Assets/sprite/select/confirmationSelect/select/confirmationSelect.dds";/*確認選択UIのファイルパス。*/
			std::string m_confirmationSelectTextFilePath[enConfirmationSelectTextUI_Num] = {
				"Assets/sprite/select/confirmationSelect/text/redFrame/no.dds",
				"Assets/sprite/select/confirmationSelect/text/redFrame/yes.dds"
			};/*HPゲージUIのファイルパス。*/
		};
	}
}

