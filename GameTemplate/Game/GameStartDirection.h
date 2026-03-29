#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file GameStartDirection.h。
 * @brief ゲーム開始演出クラス。
 * @author Tanimoto。
 * @date 2026/03/23。
 */
namespace nsApp
{
	namespace nsGame
	{
		/*ゲーム開始演出クラス。*/
		class GameStartDirection : public IGameObject
		{
		public:
			GameStartDirection() {};/*コンストラクタ。*/
			~GameStartDirection() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*テキストUI。*/ 
			enum EnTextUI : uint8_t
			{
				enTextUI_Ready,/*Ready。*/
				enTextUI_Go,/*Go。*/
				enTextUI_Num,/*テキスト数。*/
			};

			/*大きさ。*/
			enum EnScale : uint8_t
			{
				enScale_Down,/*小さくなる。*/
				enScale_Up,/*大きくなる。*/
				enScale_Num,/*大きさの変化数。*/
			};

			/*透明度。*/
			enum EnAlpha : uint8_t
			{
				enAlpha_Down,/*透明になる。*/
				enAlpha_Up,/*不透明になる。*/
				enAlpha_Num,/*透明度の変化数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief テキストUIの初期化。
			* @param textUI テキストUIの種類。
			*/
			void InitTextUI();

			/**
			* @brief UIアニメーションの初期化。
			*/
			void InitUIAnimation();

			/**
			* @brief UIをスライドさせるアニメーションの初期化。
			*/
			void InitSlideUIAnimation();

			/**
			* @brief UIの大きさを変えるアニメーションの初期化。
			*/
			void InitScaleUIAnimation();

			/**
			* @brief UIの透明度を変えるアニメーションの初期化。
			*/
			void InitAlphaUIAnimation();

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
			* @brief 演出が終了したか？
			* @return trueなら終了している。
			*/
			bool IsDirectionFinished() const 
			{ 
				return m_isDirectionFinished;
			}

		private:/*メンバ変数。*/
			SpriteRender m_textUI[enTextUI_Num];/*テキストUI。*/
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_slideUIAnimation;/*UIをスライドさせるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_scaleUIAnimation[enScale_Num][enTextUI_Num];/*UIの大きさを変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation[enAlpha_Num][enTextUI_Num];/*UIの透明度を変えるアニメーション。*/
			bool m_isDirectionFinished = false;/*演出が終了したか。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_textUIFilePath[enTextUI_Num] = {
				"Assets/sprite/inGame/direction/text/ready.dds",
				"Assets/sprite/inGame/direction/text/go.dds"
			};/*テキストUIのファイルパス。*/
		};
	}
}

