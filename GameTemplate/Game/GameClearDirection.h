#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file GameClearDirection.h。
 * @brief ゲームクリア演出クラス。
 * @author Tanimoto。
 * @date 2026/03/27。
 */
namespace nsApp
{
	namespace nsGame
	{
		/*ゲームクリア演出クラス。*/
		class GameClearDirection : public IGameObject
		{
		public:
			GameClearDirection() {};/*コンストラクタ。*/
			~GameClearDirection() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

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
			SpriteRender m_textUI;/*テキストUI。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation;/*UIの透明度を変えるアニメーション。*/
			bool m_isDirectionFinished = false;/*演出が終了したか。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_textUIFilePath = "Assets/sprite/inGame/direction/text/win.dds";/*テキストUIのファイルパス。*/
		};
	}
}

