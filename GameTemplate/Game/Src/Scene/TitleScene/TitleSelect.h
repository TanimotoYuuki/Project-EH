#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file TitleSelect.h。
 * @brief タイトル選択クラス。
 * @author Tanimoto。
 * @date 2026/05/12。
 */
namespace nsApp
{
	namespace nsTitle
	{
		/**
		* @brief タイトル選択クラス。
		*/
		class TitleSelect : public IGameObject
		{
		public:
			TitleSelect() {};/*コンストラクタ。*/
			~TitleSelect() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc);/*描画処理。*/

		public:/*列挙型。*/

			/*選択肢テキストUI。*/
			enum EnOptionTextUI : uint8_t
			{
				enOptionTextUI_ReturnToGame,/*ゲームを続ける。*/
				enOptionTextUI_RestartTheGame,/*ゲームをやり直す。*/
				enOptionTextUI_ReturnToQuestSelect,/*クエスト選択に戻る。*/
				enOptionTextUI_Num,/*テキストUIの数。*/
			};

			/*ボタンUI。*/
			enum EnButtonUI : uint8_t
			{
				enButtonUI_A,/*Aボタン。*/
				enButtonUI_B,/*Bボタン。*/
				enButtonUI_Num/*ボタンUI数。*/
			};

			/*テキストUI。*/
			enum EnTextUI : uint8_t
			{
				enTextUI_Decision,/*決定。*/
				enTextUI_Return,/*戻る。*/
				enTextUI_Num/*テキストUI数、*/
			};

			/*選択している内容。*/
			enum EnSelect : uint8_t
			{
				enSelect_Start,/*ゲームスタート。*/
				enSelect_Setting,/*設定。*/
				enSelect_ExitGame,/*ゲーム終了。*/
				enSelect_Num,/*選択している内容の数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief 選択UIの初期化。
			*/
			void InitSelectUI();

			/**
			* @brief 選択肢テキストUIの初期化。
			* @param optionTextUI 選択肢テキストUIの種類。
			* @param optionTextIndex 選択肢テキストのインデックス。
			*/
			void InitOptionTextUI(EnOptionTextUI optionTextUI, int optionTextIndex);

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
			* @brief UIを上にスライドさせるアニメーションの初期化。
			* @param spriteData 上にスライドさせるスプライトのデータ。
			* @param spriteIndex 上にスライドさせるスプライトのインデックス。
			*/
			void InitSlideUpUIAnimation(SpriteRender* spriteData, int spriteIndex);

			/**
			* @brief UIを下にスライドさせるアニメーションの初期化。
			* @param spriteData 下にスライドさせるスプライトのデータ。
			* @param spriteIndex 下にスライドさせるスプライトのインデックス。
			*/
			void InitSlideDownUIAnimation(SpriteRender* spriteData, int spriteIndex);

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
			* @brief UIを上にスライドさせるアニメーションのリセット処理。
			*/
			void ResetSlideUpUIAnimation();

			/**
			* @brief UIを下にスライドさせるアニメーションのリセット処理。
			*/
			void ResetSlideDownUIAnimation();

			/**
			* @brief UIを上にスライドさせるアニメーション再生終了したか？
			* @return trueなら再生終了している。
			*/
			inline bool IsEndSlideUpDirection() const
			{
				return m_slideUpUIAnimation[0]->IsEnd();
			}

			/**
			* @brief UIを下にスライドさせるアニメーション再生終了したか？
			* @return trueなら再生終了している。
			*/
			inline bool IsEndSlideDownDirection() const
			{
				return m_slideDownUIAnimation[0]->IsEnd();
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
			* @brief スライド演出(上)できる状態にする。
			*/
			inline void EnableSlideUpDirection()
			{
				m_isSlideUpDirection = true;
			}

			/**
			* @brief スライド演出(上)できない状態にする。
			*/
			inline void DisableSlideUpDirection()
			{
				m_isSlideUpDirection = false;
			}

			/**
			* @brief スライド演出中？(上)
			* @return trueなら上にスライド演出中(上)。
			*/
			inline bool IsSlideUpDirection() const
			{
				return m_isSlideUpDirection;
			}

			/**
			* @brief スライド演出(下)できる状態にする。
			*/
			inline void EnableSlideDownDirection()
			{
				m_isSlideDownDirection = true;
			}

			/**
			* @brief スライド演出(下)できない状態にする。
			*/
			inline void DisableSlideDownDirection()
			{
				m_isSlideDownDirection = false;
			}

			/**
			* @brief スライド演出中？(下)
			* @return trueなら上にスライド演出中(下)。
			*/
			inline bool IsSlideDownDirection() const
			{
				return m_isSlideDownDirection;
			}

			/**
			* @brief 前の画面に戻る状態にする。
			*/
			inline void EnableBackScene()
			{
				m_isBackScene = true;
			}

			/**
			* @brief 前の画面に戻らない状態にする。
			*/
			inline void DisableBackScene()
			{
				m_isBackScene = false;
			}

			/**
			* @brief 前の画面に戻るか？
			* @return trueなら前の選択に戻る。
			*/
			inline bool IsBackScene() const
			{
				return m_isBackScene;
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

		private:/*メンバ変数*/
			SpriteRender m_selectUI;/*選択UI。*/
			SpriteRender m_optionTextUI[enOptionTextUI_Num];/*選択肢テキストUI。*/
			SpriteRender m_buttonUI[enButtonUI_Num];/*ボタンUI。*/
			SpriteRender m_textUI[enTextUI_Num];/*テキストUI。*/
			int m_currentSelect = enSelect_Start;/*選択している内容。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideUpUIAnimation;/*UIを上にスライドさせるアニメーション。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideDownUIAnimation;/*UIを下にスライドさせるアニメーション。*/
			std::vector<SpriteRender*> m_slideUIAnimationSprite;/*スライドUIアニメーション用のスプライト。*/
			bool m_isSlideUpDirection = false;/*スライド演出中(上)？*/
			bool m_isSlideDownDirection = false;/*スライド演出中(下)？*/
			bool m_isBackScene = false;/*前の画面に戻るか？*/
			bool m_didSelect;/*選択できたか？*/
			
		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_selectUIFilePath = "Assets/sprite/select/titleSelect/select/select.dds";/*選択UIのファイルパス。*/
			std::string m_optionTextUIFilePath[enOptionTextUI_Num] = {
				"Assets/sprite/select/titleSelect/text/start.dds",
				"Assets/sprite/select/titleSelect/text/setting.dds",
				"Assets/sprite/select/titleSelect/text/exitGame.dds",
			};/*選択肢テキストUIのファイルパス。*/
			std::string m_buttonUIFilePath[enButtonUI_Num] = {
				"Assets/sprite/gamePad/button/aButton.dds",
				"Assets/sprite/gamePad/button/bButton.dds",
			};/*ボタンUIのファイルパス。*/
			std::string m_textUIFilePath[enTextUI_Num] = {
				"Assets/sprite/select/titleSelect/text/decision.dds",
				"Assets/sprite/select/titleSelect/text/return.dds",
			};/*テキストUIのファイルパス。*/
		};
	};
};

