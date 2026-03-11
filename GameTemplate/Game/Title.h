#pragma once
/**
 * @file Title.h。
 * @brief タイトルクラス。
 * @author Tanimoto。
 * @date 2026/03/05。
 */
namespace nsApp
{
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

		private:/*メンバ変数。*/
			SpriteRender m_backGround;/*背景。*/
			SpriteRender m_titleNameUI;/*タイトル名UI。*/
			Vector3 m_titleNameUIPosition;/*タイトル名UIの位置。*/
			Vector3 m_titleNameUIScale;/*タイトル名UIの大きさ。*/
			SpriteRender m_pressAButtonUI;/*Aボタンを押してくださいを促すUI。*/
			Vector3 m_pressAButtonUIPosition;/*Aボタンを押してくださいを促すUIの位置。*/
			Vector3 m_pressAButtonUIScale;/*Aボタンを押してくださいを促すUIの大きさ。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_backGroundFilePath = "Assets/sprite/title/background/background.dds";/*背景。*/
			std::string m_titleNameUIFilePath = "Assets/sprite/title/text/titleName.dds";/*タイトル名UIのファイルパス。*/
			std::string m_pressAButtonUIFilePath = "Assets/sprite/title/text/pressAButton.dds";/*Aボタンを押してくださいを促すUIのファイルパス。*/
		};
	}
}

