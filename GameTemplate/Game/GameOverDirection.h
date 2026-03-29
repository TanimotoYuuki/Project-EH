#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file GameOverDirection.h。
 * @brief ゲームオーバー演出クラス。
 * @author Tanimoto。
 * @date 2026/03/27。
 */
namespace nsApp
{
	namespace nsGame
	{
		/*ゲームオーバー演出クラス。*/
		class GameOverDirection : public IGameObject
		{
		public:
			GameOverDirection() {};/*コンストラクタ。*/
			~GameOverDirection() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*UIの位置を変えるアニメーションの回数。*/
			enum EnPositionUIAnimationCount : uint8_t
			{
				enPositionUIAnimationCount_One,/*1回目のUIアニメーション。*/
				enPositionUIAnimationCount_Two,/*2回目のUIアニメーション。*/
				enPositionUIAnimationCount_Three,/*3回目のUIアニメーション。*/
				enPositionUIAnimationCount_Four,/*4回目のUIアニメーション。*/
				enPositionUIAnimationCount_Five,/*5回目のUIアニメーション。*/
				enPositionUIAnimationCount_Six,/*6回目のUIアニメーション。*/
				enPositionUIAnimationCount_Num,/*UIアニメーションの回数。*/
			};

			/*UIの回転を変えるアニメーションの回数。*/
			enum EnRotationUIAnimationCount : uint8_t
			{
				enRotationUIAnimationCount_One,/*1回目のUIアニメーション。*/
				enRotationUIAnimationCount_Two,/*2回目のUIアニメーション。*/
				enRotationUIAnimationCount_Three,/*3回目のUIアニメーション。*/
				enRotationUIAnimationCount_Four,/*4回目のUIアニメーション。*/
				enRotationUIAnimationCount_Five,/*5回目のUIアニメーション。*/
				enRotationUIAnimationCount_Num,/*UIアニメーションの回数。*/
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
			* @brief UIの位置を変えるアニメーションの初期化。
			*/
			void InitPositionUIAnimation();

			/**
			* @brief UIの回転を変えるアニメーションの初期化。
			*/
			void InitRotationUIAnimation();

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
			inline bool IsDirectionFinished() const
			{
				return m_isDirectionFinished;
			}

		private:/*メンバ変数。*/
			SpriteRender m_textUI;/*テキストUI。*/
			Quaternion m_ro;
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_positionUIAnimation[enPositionUIAnimationCount_Num];/*UIの位置を変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::RotationUIAnimation> m_rotationUIAnimation[enRotationUIAnimationCount_Num];/*UIの回転を変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation;/*UIの透明度を変えるアニメーション。*/
			bool m_isDirectionFinished = false;/*演出が終了したか。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_textUIFilePath = "Assets/sprite/inGame/direction/text/gameOver.dds";/*テキストUIのファイルパス。*/
		};
	}
}

