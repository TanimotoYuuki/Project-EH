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

			/*確認UI。*/
			enum EnConfirmationUI : uint8_t
			{
				enConfirmationUI_GameStart,/*ゲーム開始。*/
				enConfirmationUI_RestartTheGame,/*ゲームをやり直す。*/
				enConfirmationUI_ReturnToQuestSelect,/*クエスト選択に戻る。*/
				enConfirmationUI_GameEnd,/*ゲーム終了。*/
				enConfirmationUI_Num,/*確認UI数。*/
			};

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

			/*位置。*/
			enum EnPosition : uint8_t
			{
				enPosition_Down,/*下降。*/
				enPosition_Up,/*上昇。*/
				enPosition_Num/*位置を変更する種類。*/
			};

			/*選択したときの演出UIアニメーションに適用するスプライト。*/
			enum EnSelectDirectionUIAnimationSprite : uint8_t
			{
				enSelectDirectionUIAnimationSprite_NoTextUI,/*「No」テキストUI。*/
				enSelectDirectionUIAnimationSprite_YesTextUI,/*「Yes」テキストUI。*/
				enSelectDirectionUIAnimationSprite_SelectUI,/*選択UI。*/
				enSelectDirectionUIAnimationSprite_Num/*適用するスプライト数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief 確認UIの初期化。
			* @param confirmationUI 確認選択UIの種類。
			*/
			void InitConfirmationUI(EnConfirmationUI confirmationUI);

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
			* @brief ボタンUIの初期化。
			*/
			void InitButtonUI();

			/**
			* @brief テキストUIの初期化。
			*/
			void InitTextUI();

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
			* @brief UIの透明度を変えるアニメーションの初期化。
			*/
			void InitAlphaUIAnimation();

			/**
			* @brief 選択したときの演出UIアニメーションの初期化。
			*/
			void InitSelectDirectionUIAnimation();

			/**
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

		public:/*メンバ関数。*/

			/**
			* @brief 確認UIの変更。
			* @param confirmationUI 確認UIの種類。
			*/
			inline void ChangeConfirmationUI(EnConfirmationUI confirmationUI) 
			{ 
				m_currentConfirmationUI = confirmationUI; 
			}

			/**
			* @brief UIの透明度を変えるアニメーションのリセット処理。
			*/
			inline void ResetAlphaUIAnimation()
			{
				m_alphaUIAnimation->Reset();
			}

			/**
			* @brief 選択したときの演出UIアニメーションのリセット処理。
			*/
			void ResetSelectDirectionUIAnimation();

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
			SpriteRender m_confirmationUI[enConfirmationUI_Num];/*確認UI。*/
			SpriteRender m_confirmationSelectUI;/*確認選択UI。*/
			SpriteRender m_confirmationSelectTextUI[enConfirmationSelectTextUI_Num];/*確認選択テキストUI。*/
			SpriteRender m_buttonUI;/*ボタンUI。*/
			SpriteRender m_textUI;/*テキストUI。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation;/*透明度を変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_selectDirectionUIAnimation[enPosition_Num][enSelectDirectionUIAnimationSprite_Num];/*選択したときの演出UIアニメーション。*/
			int m_currentSelect = enSelect_No;/*現在の選択している内容。*/
			int m_currentConfirmationUI = enConfirmationUI_GameStart;/*現在の確認UI。*/
			bool m_isDrawingUI = false;/*UIを描画しているか？*/
			bool m_isDirection = false;/*演出中？*/
			bool m_didSelect = false;/*選択できたか？*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_confirmationUIFilePath[enConfirmationUI_Num] = {
				"Assets/sprite/select/confirmationSelect/confirmation/gameStart.dds",
				"Assets/sprite/select/confirmationSelect/confirmation/restartGame.dds",
				"Assets/sprite/select/confirmationSelect/confirmation/returnToQuestSelect.dds",
				"Assets/sprite/select/confirmationSelect/confirmation/gameEnd.dds"
			};/*確認UIのファイルパス。*/
			std::string m_confirmationSelectUIFilePath = "Assets/sprite/select/confirmationSelect/select/confirmationSelect.dds";/*確認選択UIのファイルパス。*/
			std::string m_confirmationSelectTextFilePath[enConfirmationSelectTextUI_Num] = {
				"Assets/sprite/select/confirmationSelect/text/redFrame/no.dds",
				"Assets/sprite/select/confirmationSelect/text/redFrame/yes.dds"
			};/*HPゲージUIのファイルパス。*/
			std::string m_buttonUIFilePath = "Assets/sprite/gamePad/button/aButton.dds";/*ボタンUIのファイルパス。*/
			std::string m_textUIFilePath = "Assets/sprite/select/confirmationSelect/text/decision.dds";/*テキストUIのファイルパス。*/
		};
	}
}

