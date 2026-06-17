#pragma once
#include "Src/Sound/SoundLister.h"
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file Title.h。
 * @brief タイトルクラス。
 * @author Tanimoto。
 * @date 2026/03/05。
 */
/*前方宣言。*/
namespace nsApp
{
	class UIInput;
}

namespace nsApp
{
	namespace nsSound
	{
		class SoundLister;
	}

	namespace nsTitle
	{
		/**
		 * @brief タイトルクラス。
		 */
		class Title : public IGameObject
		{
		public:
			Title() {};	 /*コンストラクタ。*/
			~Title() {}; /*デストラクタ。*/

		public:
			bool Start() override;			/*開始処理。*/
			void Update() override;			/*更新処理。*/
			void Render(RenderContext &rc); /*描画処理。*/

		public: /*列挙型。*/
			/*UIアニメーションをさせるスプライト。*/
			enum EnUIAnimationSprite : uint8_t
			{
				enUIAnimationSprite_TitleNameUI,	/*タイトル名UI*/
				enUIAnimationSprite_PressAButtonUI, /*Aボタンを押してくださいを促すUI*/
				enUIAnimationSprite_Num				/*UIアニメーションをさせるスプライト。*/
			};

		private: /*メンバ関数。*/
			/**
			 * @brief スプライトの初期化。
			 */
			void InitSprite();

			/**
			 * @brief タイトル名UIの初期化。
			 */
			void InitTitleNameUI();

			/**
			 * @brief Aボタンを押してくださいを促すUIの初期化。
			 */
			void InitPressAButtonUI();

			/**
			 * @brief UIアニメーションの初期化。
			 */
			void InitUIAnimation();

			/**
			 * @brief UIの大きさを小さくするアニメーションの初期化。
			 */
			void InitScaleDownUIAnimation();

			/*
			 * @brief UIの透明度を変えるアニメーションの初期化。
			 */
			void InitAlphaUIAnimation();

			/**
			 * @brief UIを上にスライドさせるアニメーションの初期化。
			 * @param spriteData スライドさせるスプライトのデータ。
			 * @param spriteIndex スライドさせるスプライトのインデックス。
			 */
			void InitSlideUpUIAnimation(SpriteRender *spriteData, int spriteIndex);

			/**
			 * @brief UIを下にスライドさせるアニメーションの初期化。
			 * @param spriteData スライドさせるスプライトのデータ。
			 * @param spriteIndex スライドさせるスプライトのインデックス。
			 */
			void InitSlideDownUIAnimation(SpriteRender *spriteData, int spriteIndex);

			/**
			 * @brief UIアニメーションの更新処理。
			 */
			void UpdateUIAnimation();

			/**
			 * @brief スプライトの更新処理。
			 */
			void UpdateSprite();

		public: /*メンバ関数。*/
			/**
			 * @brief UIを上にスライドさせるアニメーションのリセット処理。
			 */
			void ResetSlideUpUIAnimation();

			/**
			 * @brief UIを下にスライドさせるアニメーションのリセット処理。
			 */
			void ResetSlideDownUIAnimation();

			/**
			 * @brief Aボタンを押してくださいを促すUIの乗算色のリセット処理。
			 */
			inline void ResetPressAButtonMulColor()
			{
				m_pressAButtonUI.SetMulColor({1.0f, 1.0f, 1.0f, 1.0f});
				m_pressAButtonUI.Update();
			}

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
			 * @brief BGMの再生。
			 */
			void StopBGM()
			{
				m_bgm->GetBGMList().StopBGM(); //! BGMを停止する。
				DeleteGO(m_bgm);			   //! BGMクラスを削除する。
				m_bgm = nullptr;			   //! 再初期化する。
			}

			/**
			 * @brief BGM用のインスタンスを取得する。
			 * @return BGM用のインスタンス。
			 */
			nsSound::SoundLister *GetBGMInstance() const
			{
				return m_bgm;
			}

		private:																						/*メンバ変数。*/
			SpriteRender m_titleNameUI;																	/*タイトル名UI。*/
			SpriteRender m_pressAButtonUI;																/*Aボタンを押してくださいを促すUI。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_scaleDownUIAnimation;						/*UIの大きさを小さくするアニメーション。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation[enUIAnimationSprite_Num]; /*UIの透明度を変えるアニメーション。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideUpUIAnimation;		/*UIを上にスライドさせるアニメーション。*/
			std::vector<std::unique_ptr<nsApp::nsUI::PositionUIAnimation>> m_slideDownUIAnimation;		/*UIを下にスライドさせるアニメーション。*/
			std::vector<SpriteRender *> m_slideUIAnimationSprite;										/*スライドUIアニメーション用のスプライト。*/
			bool m_isSlideUpDirection = false;															/*スライド演出中(上)？*/
			bool m_isSlideDownDirection = false;														/*スライド演出中(下)？*/
			bool m_didSelect = false;																	/*選択できたか？*/
			nsSound::SoundLister *m_bgm = nullptr;														/*BGM。*/

		private:																				/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_titleNameUIFilePath = "Assets/sprite/title/text/titleName.dds";		/*タイトル名UIのファイルパス。*/
			std::string m_pressAButtonUIFilePath = "Assets/sprite/title/text/pressAButton.dds"; /*Aボタンを押してくださいを促すUIのファイルパス。*/

			/*UIInputクラスに対して、Titleのpriveteメンバへのアクセスを許可。*/
			friend class ::nsApp::UIInput;
		};
	}
}
