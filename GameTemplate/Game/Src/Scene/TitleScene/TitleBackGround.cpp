#include "stdafx.h"
#include "TitleBackGround.h"

namespace {
	/*背景。*/
	const float BACK_GROUND_WIDTH = 1920;/*背景の幅。*/

	const float BACK_GROUND_HEIGHT = 1080;/*背景の高さ。*/
}

namespace nsApp
{
	namespace nsTitle
	{
		/*開始処理。*/
		bool TitleBackGround::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			return true;
		}

		/*更新処理。*/
		void TitleBackGround::Update()
		{
			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void TitleBackGround::Render(RenderContext& rc)
		{
			/*背景の描画。*/
			m_backGround.Draw(rc);
		}

		/*スプライトの初期化。*/
		void TitleBackGround::InitSprite()
		{
			/*背景。*/
			InitBackGround();
		}

		/*背景の初期化。*/
		void TitleBackGround::InitBackGround()
		{
			m_backGround.Init(m_backGroundFilePath.c_str(), BACK_GROUND_WIDTH, BACK_GROUND_HEIGHT);/*初期化。*/
		}

		/*スプライトの更新処理。*/
		void TitleBackGround::UpdateSprite()
		{
			m_backGround.Update();
		}
	}
}