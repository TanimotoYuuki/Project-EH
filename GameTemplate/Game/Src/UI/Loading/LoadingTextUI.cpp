#include "stdafx.h"
#include "LoadingTextUI.h"

namespace
{
	const auto POSITION = Vector3(-80.0f, -80.0f, 0.0f);     //! ローディングテキストの位置。
	const auto SCALE = 1.0f;								 //! ローディングテキストのスケール。
	const auto COLOR = Vector4(1.0f, 1.0f, 1.0f, 1.0f);      //! ローディングテキストの色。
	const auto PIVOT = Vector2(0.5f, 0.5f);				     //! ローディングテキストのピボット。
}

namespace nsApp
{
	namespace nsUI
	{
		void LoadingTextUI::Init()
		{
			/* 座標をセット。*/
			m_loadingText.SetPosition(POSITION);

			/* スケールをセット。*/ 
			m_loadingText.SetScale(SCALE);

			/* 色をセット。*/
			m_loadingText.SetColor(COLOR);

			/* ピボットをセット。*/
			m_loadingText.SetPivot(PIVOT);
		}


		void LoadingTextUI::Update(float elasedTime)
		{
			/* タイマーを進め、ドットの数を増やす。*/
			m_timer += elasedTime;

			/* ドットの数を0から3の範囲でループさせる。*/
			m_dot = static_cast<int>(m_timer * 2.0f) % 4;

			/* ドットの数に応じてテキストを更新する。*/
			switch (m_dot)
			{
			case 0:
				m_loadingText.SetText(L"Loading");
				break;

			case 1:
				m_loadingText.SetText(L"Loading.");
				break;

			case 2:
				m_loadingText.SetText(L"Loading..");
				break;

			case 3:
				m_loadingText.SetText(L"Loading...");
				break;
			}
		}


		void LoadingTextUI::Render(RenderContext& rc)
		{
			/* ローディングテキストを描画する。*/
			m_loadingText.Draw(rc);
		}
	}
}