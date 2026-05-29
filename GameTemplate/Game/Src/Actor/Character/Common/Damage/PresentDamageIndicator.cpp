#include "stdafx.h"
#include "PresentDamageIndicator.h"

namespace
{
	const auto INITIAL_OFFSET_Y = 20.0f;       //! 初期表示位置のY補正。
	const auto FLOAT_UP_DISTANCE = 80.0f;      //! 表示中に上昇する距離。
	const auto LIFE_TIME = 0.8f;               //! 表示時間。

	const auto START_SCALE = 2.0f;             //! 出現直後の拡大率。
	const auto END_SCALE = 1.0f;               //! 消える直前の拡大率。

	const auto SHADOW_OFFSET_X = 4.0f;         //! 影のX方向オフセット。
	const auto SHADOW_OFFSET_Y = -4.0f;        //! 影のY方向オフセット。
	const auto SHADOW_ALPHA_RATE = 1.0f;       //! 影の濃さ。

	const auto DAMAGE_COLOR_R = 1.0f;		   //! 赤の調整値。	      
	const auto DAMAGE_COLOR_G = 0.85f;		   //! 緑の調整値。
	const auto DAMAGE_COLOR_B = 0.1f;          //! 青の調整値。
}


namespace nsApp
{
	void PresentDamageIndicator::Init(int damageValue, const Vector3& drawPosition)
	{
		/* 表示位置を初期化。*/
		m_damageValue = damageValue;

		/* タイマーを初期化。*/
		m_lifeTimer = 0.0f;
		m_lifeTime = LIFE_TIME;

		/* 座標の初期化。*/
		m_startPosition = drawPosition;
		m_startPosition.y += INITIAL_OFFSET_Y;
		m_drawPosition = m_startPosition;

		/* テキストの初期化。*/
		m_damageText = std::to_wstring(m_damageValue);
		m_damageFont.SetText(m_damageText.c_str());
		m_shadowFont.SetText(m_damageText.c_str());

		/* フォントの設定値を反映。*/
		ApplyFontParameter();
	}


	void PresentDamageIndicator::Update()
	{
		if (!m_isActive)
			return;

		/* 単位時間当たりのフレームを取得。*/
		const auto deltaTime = g_gameTime->GetFrameDeltaTime();

		/* タイマーの加算。*/
		m_lifeTimer += deltaTime;

		/* 寿命の進行率を計算。*/
		const auto lifeRate = ComputeLifeRate();

		/* 描画位置の計算。*/
		m_drawPosition = ComputeDrawPosition(lifeRate);
		/* 割合値を計算。*/
		m_scaleValue = ComputeScale(lifeRate);
		/* 透明度の計算。*/ 
		m_alphaValue = ComputeAlpha(lifeRate);

		/* フォントの設定値を反映。*/
		ApplyFontParameter();

		/* 削除処理。*/
		if (IsDead())
		{
			Deactivate();
			return;
		}
	}


	void PresentDamageIndicator::Render(RenderContext& rc)
	{
		if (!m_isActive)
			return;

		m_shadowFont.Draw(rc);
		m_damageFont.Draw(rc);
	}


	void PresentDamageIndicator::OnAcquire()
	{
		m_isActive = true;

		m_lifeTimer = 0.0f;
		m_alphaValue = 1.0f;
		m_scaleValue = START_SCALE;
	}


	void PresentDamageIndicator::OnRelease()
	{
		m_isActive = false;

		m_lifeTimer = 0.0f;
		m_alphaValue = 1.0f;
		m_scaleValue = 1.0f;
	}


	float PresentDamageIndicator::ComputeLifeRate() const
	{
		if(m_lifeTime <= 0.0f)
			return 1.0f;

		/* 割合を計算。*/
		auto finalLifeRate = m_lifeTimer / m_lifeTime;

		if(finalLifeRate < 0.0f)
			finalLifeRate = 0.0f;

		if(finalLifeRate > 1.0f)
			finalLifeRate = 1.0f;

		return finalLifeRate;
	}

	
	Vector3 PresentDamageIndicator::ComputeDrawPosition(float lifeRate) const
	{
		Vector3 finalPosition = m_startPosition;

		finalPosition.y += FLOAT_UP_DISTANCE * lifeRate;
		return finalPosition;
	}


	float PresentDamageIndicator::ComputeScale(float lifeRate) const
	{
		return START_SCALE + (END_SCALE - START_SCALE) * lifeRate;
	}


	Vector3 PresentDamageIndicator::ComputeShadowPosition(const Vector3& drawPosition) const
	{
		Vector3 shadowPosition = drawPosition;
		shadowPosition.x += SHADOW_OFFSET_X;
		shadowPosition.y += SHADOW_OFFSET_Y;

		return shadowPosition;
	}


	Vector4 PresentDamageIndicator::ComputeTextColor(float alpha) const
	{
		return Vector4(DAMAGE_COLOR_R, DAMAGE_COLOR_G, DAMAGE_COLOR_B, alpha);
	}


	Vector4 PresentDamageIndicator::ComputeShadowColor(float alpha) const
	{
		return Vector4(0.0f, 0.0f, 0.0f, alpha * SHADOW_ALPHA_RATE);
	}


	void PresentDamageIndicator::ApplyFontParameter()
	{
		const Vector3 shadowPosition = ComputeShadowPosition(m_drawPosition);

		/* 影。*/
		m_shadowFont.SetPosition(shadowPosition);
		m_shadowFont.SetScale(m_scaleValue);
		m_shadowFont.SetColor(ComputeShadowColor(m_alphaValue));

		/* 本体文字。*/ 
		m_damageFont.SetPosition(m_drawPosition);
		m_damageFont.SetScale(m_scaleValue);
		m_damageFont.SetColor(ComputeTextColor(m_alphaValue));
	}
}