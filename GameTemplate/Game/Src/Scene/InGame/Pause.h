#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file Pause.h。
 * @brief ゲームを一時中断するクラス。
 * @author Tanimoto。
 * @date 2026/03/05。
 */
namespace nsApp
{
	namespace nsSelect
	{
		class ConfirmationSelect;
	}

	namespace nsGame
	{
		/*ゲームを一時中断するクラス。*/
		class Pause : public IGameObject
		{
		public:
			Pause() {};/*コンストラクタ。*/
			~Pause();/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*選択肢テキストUI。*/
			enum EnOptionTextUI : uint8_t
			{
				enOptionTextUI_ReturnToGame,/*ゲームを続ける。*/
				enOptionTextUI_HowToPlay,/*操作方法。*/
				enOptionTextUI_RestartTheGame,/*ゲームをやり直す。*/
				enOptionTextUI_ReturnToQuestSelect,/*クエスト選択に戻る。*/
				enOptionTextUI_Num,/*テキストUIの数。*/
			};

			/*選択している内容。*/
			enum EnSelect : uint8_t
			{
				enSelect_ReturnToGame,/*ゲームを続ける。*/
				enSelect_HowToPlay,/*操作方法。*/
				enSelect_RestartTheGame,/*ゲームをやり直す。*/
				enSelect_ReturnToQuestSelect,/*クエスト選択に戻る。*/
				enSelect_Num,/*選択している内容の数。*/
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
				enSelectDirectionUIAnimationSprite_ReturnToGameTextUI,/*「ゲームを続ける」テキストUI。*/
				enSelectDirectionUIAnimationSprite_HowToPlayTextUI,/*「操作方法」テキストUI。*/
				enSelectDirectionUIAnimationSprite_RestartTheGameTextUI,/*「ゲームをやり直す」テキストUI。*/
				enSelectDirectionUIAnimationSprite_ReturnToQuestSelectTextUI,/*「クエスト選択に戻る」テキストUI。*/
				enSelectDirectionUIAnimationSprite_SelectUI,/*選択UI。*/
				enSelectDirectionUIAnimationSprite_Num/*適用するスプライト数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief 背景の初期化。
			*/
			void InitBackGround();

			/**
			* @brief ポーズテキストUIの初期化。
			*/
			void InitPauseTextUI();

			/**
			* @brief 白枠UIの初期化。
			*/
			void InitWhiteFrameUI();

			/**
			* @brief テキスト選択UIの初期化。
			*/
			void InitTextSelectUI();

			/**
			* @brief 選択肢テキストUIの初期化。
			* @param optionTextUI 選択肢テキストUIの種類。
			* @param optionTextIndex 選択肢テキストのインデックス。
			*/
			void InitOptionTextUI(EnOptionTextUI optionTextUI, int optionTextIndex);

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
			* @brief UIの透明度を変えるアニメーションの初期化。
			*/
			void InitAlphaUIAnimation();

			/**
			* @brief 選択したときの演出UIアニメーションの初期化。
			*/
			void InitSelectDirectionUIAnimation();

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

		public:/*メンバ関数。 */

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

			/**
			* @brief 確認選択用のインスタンスの取得。
			* @return 確認選択用のインスタンス。
			*/
			nsApp::nsSelect::ConfirmationSelect* GetConfirmationSelectInstance()
			{
				return m_confirmationSelect;
			}

		private:/*メンバ変数。*/
			SpriteRender m_backGround;/*背景。*/
			SpriteRender m_pauseTextUI;/*ポーズテキストUI。*/
			SpriteRender m_whiteFrameUI;/*白枠UI。*/
			SpriteRender m_textSelectUI;/*テキスト選択UI。*/
			SpriteRender m_optionTextUI[enOptionTextUI_Num];/*選択肢テキストUI。*/
			SpriteRender m_buttonUI;/*ボタンUI。*/
			SpriteRender m_textUI;/*テキストUI。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation;/*透明度を変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_selectDirectionUIAnimation[enPosition_Num][enSelectDirectionUIAnimationSprite_Num];/*選択したときの演出UIアニメーション。*/
			int m_currentSelect = enSelect_ReturnToGame;/*現在選択している内容。*/
			bool m_isDrawingUI = false;/*UIを描画しているか？*/
			bool m_isDirection = false;/*演出中？*/
			bool m_didSelect = false;/*選択できたか？*/
			nsApp::nsSelect::ConfirmationSelect* m_confirmationSelect = nullptr;/*確認選択用のインスタンス。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_backGroundFilePath = "Assets/sprite/pause/background/backGround.dds";/*背景のファイルパス。*/
			std::string m_whiteFrameUIFilePath = "Assets/sprite/pause/frame/whiteFrame.dds";/*白枠UIのファイルパス。*/
			std::string m_pauseTextUIFilePath = "Assets/sprite/pause/text/pause.dds";/*ポーズテキストUIのファイルパス。*/
			std::string m_textSelectUIFilePath = "Assets/sprite/pause/select/select.dds";/*テキスト選択UIのファイルパス。*/
			std::string m_optionTextUIFilePath[enOptionTextUI_Num] ={
				"Assets/sprite/pause/text/returnToGame.dds",/*ゲームを続ける。*/
				"Assets/sprite/pause/text/howToPlay.dds",/*操作方法。*/
				"Assets/sprite/pause/text/restartGame.dds",/*ゲームをやり直す。*/
				"Assets/sprite/pause/text/returnToQuestSelect.dds",/*クエスト選択に戻る。*/
			};/*選択肢テキストUIのファイルパス。*/
			std::string m_buttonUIFilePath = "Assets/sprite/gamePad/button/aButton.dds";/*ボタンUIのファイルパス。*/
			std::string m_textUIFilePath = "Assets/sprite/pause/text/decision.dds";/*テキストUIのファイルパス。*/
		};
	}
}

