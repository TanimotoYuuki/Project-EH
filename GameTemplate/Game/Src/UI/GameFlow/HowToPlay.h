#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file Title.h。
 * @brief 操作方法クラス。
 * @author Tanimoto。
 * @date 2026/05/20。
 */
namespace nsApp
{
	namespace nsHowToPlay
	{
		class HowToPlay : public IGameObject
		{
		public:
			HowToPlay() {};/*コンストラクタ。*/
			~HowToPlay() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*操作方法UIの種類。*/
			enum EnHowToPlayUI : uint8_t
			{
				enHowToPlayUI_GeneralPage,/*共通の操作方法UI。*/
				enHowToPlayUI_Sword_OnePage,/*ソードの操作方法UI（1ページ）。*/
				enHowToPlayUI_Sword_TwoPage,/*ソードの操作方法UI（2ページ）。*/
				enHowToPlayUI_Hammer_OnePage,/*ハンマーの操作方法UI（1ページ）。*/
				enHowToPlayUI_Mage_OnePage,/*メイジの操作方法UI（1ページ）。*/
				enHowToPlayUI_Mage_TwoPage,/*メイジの操作方法UI（2ページ）。*/
				enHowToPlayUI_Gunner_OnePage,/*ガンナーの操作方法UI（1ページ）。*/
				enHowToPlayUI_Num,/*操作方法UIの種類の数。*/
			};

			/*矢印UI。*/
			enum EnArrowUI : uint8_t
			{
				enArrowUI_Left,/*左。*/
				enArrowUI_Right,/*右。*/
				enArrowUI_Num,/*矢印UIの数。*/
			};

			/*スライドさせる方向。*/
			enum EnSlide : uint8_t
			{
				enSlide_Left,/*左。*/
				enSlide_Right,/*右。*/
				enSlide_Num/*スライドさせる方向数。*/
			};

			/*大きさ。*/
			enum EnScale : uint8_t
			{
				enScale_Down,/*大きくする。*/
				enScale_Up,/*小さくする。*/
				enScale_Num/*大きさを変更する種類。*/
			};

			/*色の変更。*/
			enum EnChangeColor : uint8_t
			{
				enChangeColor_Before,/*変更前の色。*/
				enChangeColor_After,/*変更後の色。*/
				enChangeColor_Num/*色の変更の種類。*/
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
			* @brief 操作方法テキストUIの初期化。
			*/
			void InitHowToPlayTextUI();

			/**
			* @brief 操作方法UIの初期化。
			* @param howToPlayUI 操作方法UIの種類。
			*/
			void InitHowToPlayUI(EnHowToPlayUI howToPlayUI);

			/**
			* @brief 矢印UIの初期化。
			* @param arrowUI 矢印UIの種類。
			*/
			void InitArrowUI(EnArrowUI arrowUI);

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
			* @brief 操作方法UIを左にスライドさせるアニメーション(開始)の初期化。
			* @param spriteData 左にスライドさせる操作方法UIのスプライトデータ。
			*/
			void InitSlideLeftStartHowToPlayUIAnimation(SpriteRender* spriteData);

			/**
			* @brief 操作方法UIを左にスライドさせるアニメーション(終了)の初期化。
			* @param spriteData 左にスライドさせる操作方法UIのスプライトデータ。
			*/
			void InitSlideLeftEndHowToPlayUIAnimation(SpriteRender* spriteData);

			/**
			* @brief 操作方法UIを右にスライドさせるアニメーション(開始)の初期化。
			* @param spriteData 操作方法UIを右にスライドさせる操作方法UIのスプライトデータ。
			*/
			void InitSlideRightStartHowToPlayUIAnimation(SpriteRender* spriteData);

			/**
			* @brief 操作方法UIを右にスライドさせるアニメーション(終了)の初期化。
			* @param spriteData 操作方法UIを右にスライドさせる操作方法UIのスプライトデータ。
			*/
			void InitSlideRightEndHowToPlayUIAnimation(SpriteRender* spriteData);

			/**
			* @brief 矢印UIの大きさを大きくするアニメーションの初期化。
			* @param spriteData 矢印UIの大きさを大きくするアニメーションのスプライトデータ。
			*/
			void InitScaleUpArrowUIAnimation(SpriteRender* spriteData);

			/**
			* @brief 矢印UIの大きさを小さくするアニメーションの初期化。
			* @param spriteData 矢印UIの大きさを小さくするアニメーションのスプライトデータ。
			*/
			void InitScaleDownArrowUIAnimation(SpriteRender* spriteData);

			/**
			* @brief 矢印UIの色を変更前の色にするアニメーションの初期化。
			* @param spriteData 矢印UIの色を変更前の色にするアニメーションのスプライトデータ。
			*/
			void InitColorBeforeArrowUIAnimation(SpriteRender* spriteData);

			/**
			* @brief 矢印UIの色を変更後の色にするアニメーションの初期化。
			* @param spriteData 矢印UIの色を変更後の色にするアニメーションのスプライトデータ。
			*/
			void InitColorAfterArrowUIAnimation(SpriteRender* spriteData);

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
			* @brief 操作方法UIを左にスライドさせるアニメーション(開始)をリセット処理。
			*/
			void ResetSlideLeftStartHowToPlayUIAnimation();

			/**
			* @brief 操作方法UIを左にスライドさせるアニメーション(終了)をリセット処理。
			*/
			void ResetSlideLeftEndHowToPlayUIAnimation();

			/**
			* @brief 操作方法UIを右にスライドさせるアニメーション(開始)をリセット処理。
			*/
			void ResetSlideRightStartHowToPlayUIAnimation();

			/**
			* @brief 操作方法UIを右にスライドさせるアニメーション(終了)をリセット処理。
			*/
			void ResetSlideRightEndHowToPlayUIAnimation();

			/**
			* @brief 矢印UIの大きさを大きくするアニメーションをリセット処理。
			*/
			void ResetScaleUpArrowUIAnimation();

			/**
			* @brief 矢印UIの大きさを小さくするアニメーションをリセット処理。
			*/
			void ResetScaleDownArrowUIAnimation();

			/**
			* @brief 矢印UIの色を変更前の色にするアニメーションをリセット処理。
			*/
			void ResetColorBeforeArrowUIAnimation();

			/**
			* @brief 矢印UIの色を変更後の色にするアニメーションをリセット処理。
			*/
			void ResetColorAfterArrowUIAnimation();

			/**
			* @brief 前のページに移行する演出をできる状態にする。
			*/
			inline void EnablePreviousPageTransitionDirection()
			{
				m_previousPageTransitionDirection = true;
			}

			/**
			* @brief 前のページに移行する演出をできない状態にする。
			*/
			inline void DisablePreviousPageTransitionDirection()
			{
				m_previousPageTransitionDirection = false;
			}

			/**
			* @brief 前のページに移行する演出中？。
			* @return trueなら前のページに移行する演出中。
			*/
			inline bool IsPreviousPageTransitionDirection() const
			{
				return m_previousPageTransitionDirection;
			}

			/**
			* @brief 次のページに移行する演出をできる状態にする。
			*/
			inline void EnableNextPageTransitionDirection()
			{
				m_nextPageTransitionDirection = true;
			}

			/**
			* @brief 次のページに移行する演出をできない状態にする。
			*/
			inline void DisableNextPageTransitionDirection()
			{
				m_nextPageTransitionDirection = false;
			}

			/**
			* @brief 次のページに移行する演出中？。
			* @return trueなら次のページに移行する演出中。
			*/
			inline bool IsNextPageTransitionDirection() const
			{
				return m_nextPageTransitionDirection;
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
			SpriteRender m_backGround;/*背景。*/
			SpriteRender m_howToPlayTextUI;/*操作方法テキストUI。*/
			SpriteRender m_howtToPlayUI[enHowToPlayUI_Num];/*操作方法UI。*/
			SpriteRender m_arrowUI[enArrowUI_Num];/*矢印UI。*/
			SpriteRender m_buttonUI;/*ボタンUI。*/
			SpriteRender m_textUI;/*テキストUI。*/
			int m_previousPage = enHowToPlayUI_Num;/*前のページ。*/
			int m_currentPage = enHowToPlayUI_GeneralPage;/*現在のページ。*/
			int m_displayHowToPlayUI = enHowToPlayUI_GeneralPage;/*表示する操作方法UI。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideLeftStartHowToPlayUIAnimation;/*操作方法UIを左にスライドさせるアニメーション(開始)。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideLeftEndHowToPlayUIAnimation;/*操作方法UIを左にスライドさせるアニメーション(終了)。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideRightStartHowToPlayUIAnimation;/*操作方法UIを右にスライドさせるアニメーション(開始)。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideRightEndHowToPlayUIAnimation;/*操作方法UIを右にスライドさせるアニメーション(終了)。*/
			std::vector<std::unique_ptr<nsApp::nsUI::ScaleUIAnimation>> m_scaleUpArrowUIAnimation;/*矢印UIの大きさを大きくするアニメーション。*/
			std::vector<std::unique_ptr<nsApp::nsUI::ScaleUIAnimation>> m_scaleDownArrowUIAnimation;/*矢印UIの大きさを小さくするアニメーション。*/
			std::vector<std::unique_ptr<nsApp::nsUI::ColorUIAnimation>> m_colorBeforeArrowUIAnimation;/*矢印UIの色を変更前の色にするアニメーション。*/
			std::vector<std::unique_ptr<nsApp::nsUI::ColorUIAnimation>> m_colorAfterArrowUIAnimation;/*矢印UIの色を変更後の色にするアニメーション。*/
			std::vector<SpriteRender*> m_slideHowToPlayUIAnimationSprite;/*スライドUIアニメーション用のスプライト。*/
			std::vector<SpriteRender*> m_scaleArrowUIAnimationSprite;/*大きさを変えるUIアニメーション用のスプライト。*/
			std::vector<SpriteRender*> m_colorArrowUIAnimationSprite;/*色を変えるUIアニメーション用のスプライト。*/
			bool m_previousPageTransitionDirection = false;/*前のページに移行する演出中？。*/
			bool m_nextPageTransitionDirection = false;/*次のページに移行する演出中？。*/
			bool m_isBackSelect = false;/*前の選択に戻るか？*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_backGroundFilePath = "Assets/sprite/howToPlay/background/background.dds";/*背景のファイルパス。*/
			std::string m_howToPlayTextUIFilePath = "Assets/sprite/howToPlay/text/howToPlay.dds";
			std::string m_howToPlayUIFliePath[enHowToPlayUI_Num] = {
				"Assets/sprite/howToPlay/general/general.dds",
				"Assets/sprite/howToPlay/sword/swordOnePage.dds",
				"Assets/sprite/howToPlay/sword/swordTwoPage.dds",
				"Assets/sprite/howToPlay/hammer/hammerOnePage.dds",
				"Assets/sprite/howToPlay/mage/mageOnePage.dds",
				"Assets/sprite/howToPlay/mage/mageTwoPage.dds",
				"Assets/sprite/howToPlay/gunner/gunnerOnePage.dds",
			};/*操作方法UIのファイルパス。*/
			std::string m_arrowUIFliePath[enArrowUI_Num] = {
				"Assets/sprite/howToPlay/arrow/left.dds",
				"Assets/sprite/howToPlay/arrow/right.dds"
			};/*矢印UIのファイルパス。*/
			std::string m_buttonUIFliePath = "Assets/sprite/gamePad/button/bButton.dds";/*ボタンUIのファイルパス。*/
			std::string m_textUIFilePath = "Assets/sprite/howToPlay/text/return.dds";/*テキストUIのファイルパス。*/
		};
	}
}

