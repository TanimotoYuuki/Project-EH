#pragma once
#include "Src/Sound/SoundLister.h"
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file Title.h。
 * @brief タイトルクラス。
 * @author Tanimoto。
 * @date 2026/03/05。
 */
namespace nsApp
{
	namespace nsSound {
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
			Title() {};/*コンストラクタ。*/
			~Title() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc);/*描画処理。*/

		public:/*列挙型。*/

			/*UIアニメーションをさせるスプライト。*/
			enum EnUIAnimationSprite : uint8_t
			{
				enUIAnimationSprite_TitleNameUI,/*タイトル名UI*/
				enUIAnimationSprite_PressAButtonUI,/*Aボタンを押してくださいを促すUI*/
				enUIAnimationSprite_Num/*UIアニメーションをさせるスプライト。*/
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
			* @brief 演出の更新処理。
			*/
			void UpdateDirection();

			/**
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

			/**
			 * @brief BGMの再生。 
			 */
		    void StopBGM()
			{
				m_bgm->GetBGMList().StopBGM(); //! BGMを停止する。
				DeleteGO(m_bgm);			   //! BGMクラスを削除する。
				m_bgm = nullptr;			   //! 再初期化する。
			}


		public:/*メンバ関数。*/

			/**
			* @brief 選択できたか？
			* @return trueなら選択できている。
			*/
			inline bool DidSelect() const
			{
				return m_didSelect;
			}


		private:
			nsSound::SoundLister* m_bgm = nullptr; //! BGM。


		private:/*メンバ変数。*/
			SpriteRender m_backGround;/*背景。*/
			SpriteRender m_titleNameUI;/*タイトル名UI。*/
			SpriteRender m_pressAButtonUI;/*Aボタンを押してくださいを促すUI。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_scaleDownUIAnimation;/*UIの大きさを小さくするアニメーション。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation[enUIAnimationSprite_Num];/*UIの透明度を変えるアニメーション。*/
			bool m_didSelect;/*選択できたか？*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_backGroundFilePath = "Assets/sprite/title/background/background.dds";/*背景のファイルパス。*/
			std::string m_titleNameUIFilePath = "Assets/sprite/title/text/titleName.dds";/*タイトル名UIのファイルパス。*/
			std::string m_pressAButtonUIFilePath = "Assets/sprite/title/text/pressAButton.dds";/*Aボタンを押してくださいを促すUIのファイルパス。*/
		};
	}
}

