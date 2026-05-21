#pragma once
/**
 * @file TitleBackGround.h。
 * @brief タイトル背景クラス。
 * @author Tanimoto。
 * @date 2026/05/13。
 */
namespace nsApp
{
	namespace nsTitle
	{
		class TitleBackGround : public IGameObject
		{
		public:
			TitleBackGround() {};/*コンストラクタ。*/
			~TitleBackGround() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc);/*描画処理。*/

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
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

		private:/*メンバ変数。*/
			SpriteRender m_backGround;/*背景。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_backGroundFilePath = "Assets/sprite/title/background/background.dds";/*背景のファイルパス。*/
		};
	};
};


