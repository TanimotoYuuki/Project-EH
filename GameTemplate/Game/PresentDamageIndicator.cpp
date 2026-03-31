#include "stdafx.h"
#include "PresentDamageIndicator.h"

namespace
{
	const auto MOVE_FRAME = 1.0f / 60.0f;    //! 1フレーム当たりの移動速度。
	const auto ALPHA_VALUE = 1.0f;           //! テキストの透明度。
	const auto SCALE_VALUE = 2.0f;           //! テキストの拡大率。
	const auto INITIAL_VALUE = 200;          //! ダメージテキストをスライド移動させるための初期値。
	const auto INITIAL_VALUE_Y = 20.0f;      //! ダメージテキストの初期Y座標。
	const auto REDUCTION_RATE_ALPHA = 0.02f; //! テキストの透明度を減少させる割合。
	const auto REDUCTION_RATE_SCALE = 0.2f;  //! テキストの透明度を減少させる割合。
}

namespace nsApp
{
	void PresentDamageIndicator::Init(int damageValue, const Vector3& drawPosition)
	{
		m_damageValue = damageValue;

		/* 描画する位置を設定する。*/
		m_drawPosition = drawPosition;
		m_drawPosition.y += INITIAL_VALUE_Y;

		/* 透明度、大きさ、初速を設定。*/
		SetTextParameter(ALPHA_VALUE, SCALE_VALUE, INITIAL_VALUE);

		/* テキストをセット。*/
		m_damageText = std::to_wstring(m_damageValue);
		m_damageFont.SetText(m_damageText.c_str());
	}


	void PresentDamageIndicator::Update()
	{
		/* スクロール速度を計算。*/
		m_drawPosition.y += m_moveUpSeed * MOVE_FRAME;

		/* テキストの大きさを徐々に戻す。*/
		m_scaleValue += (ALPHA_VALUE - m_scaleValue) * REDUCTION_RATE_SCALE;

		/* テキストの透明度をだんだん下げる。*/
		m_alphaValue -= REDUCTION_RATE_ALPHA;

		if (m_alphaValue <= 0.0f)
		{
			DeleteGO(this);
			return;
		}

		m_damageFont.SetPosition(m_drawPosition);
		m_damageFont.SetScale(m_scaleValue);
		m_damageFont.SetColor(Vector4(1.0f, 1.0f, 1.0f, m_alphaValue));
	}


	void PresentDamageIndicator::Render(RenderContext& rc)
	{
		m_damageFont.Draw(rc);
	}
}