#include "stdafx.h"
#include "PresentDamageIndicator.h"

namespace
{
	const auto INITIAL_OFFSET_Y = 20.0f;       //! 初期表示位置のY補正。
	const auto FLOAT_UP_DISTANCE = 80.0f;      //! 表示中に上昇する距離。

	const auto START_SCALE = 2.0f;             //! 出現直後の拡大率。
	const auto END_SCALE = 1.0f;               //! 消える直前の拡大率。

	const auto SHADOW_OFFSET_X = 4.0f;         //! 影のX方向オフセット。
	const auto SHADOW_OFFSET_Y = -4.0f;        //! 影のY方向オフセット。
	const auto SHADOW_ALPHA_RATE = 1.0f;       //! 影の濃さ。
	const auto SHADOW_INIT_VALUE_R = 0.0f;     //! 影の初期R値。
	const auto SHADOW_INIT_VALUE_G = 0.0f;     //! 影の初期G値。
	const auto SHADOW_INIT_VALUE_B = 0.0f;     //! 影の初期B値。

	const auto DAMAGE_COLOR_R = 1.0f;		   //! 赤の調整値。	      
	const auto DAMAGE_COLOR_G = 0.85f;		   //! 緑の調整値。
	const auto DAMAGE_COLOR_B = 0.1f;          //! 青の調整値。

	const auto INIT_LIFE_TIME = 0.0f;          //! 初期寿命。
	const auto SET_LIFE_TIME = 0.8f;           //! 表示時間。
	const auto RETURN_LIFE_TIME = 1.0f;        //! 寿命のリセット値。
	const auto CLAMP_LIFE_RATE_MIN = 0.0f;	   //! 寿命の割合の最小値。
	const auto CLAMP_LIFE_RATE_MAX = 1.0f;	   //! 寿命の割合の最大値。

	const auto ALPHA_VALUE = 1.0f;             //! 設定アルファ値。
	const auto SCALE_VALUE = 1.0f;             //! 設定拡大率。
}


namespace nsApp
{
	void PresentDamageIndicator::Init(int damageValue, const Vector3& drawPosition)
	{
		/* 表示位置を初期化。*/
		m_damageValue = damageValue;

		/* タイマーを初期化。*/
		m_lifeTimer = INIT_LIFE_TIME;
		m_lifeTime = SET_LIFE_TIME;

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
		/* 非アクティブ時は描画しない。*/
		if (!m_isActive)
			return;

		/*影→本体の順で描画。 */
		m_shadowFont.Draw(rc); //! 影の描画。
		m_damageFont.Draw(rc); //! 本体の描画。
	}


	void PresentDamageIndicator::OnAcquire()
	{
		/* アクティブ化。*/
		m_isActive = true;
		/* タイマーを初期化。*/
		m_lifeTimer = INIT_LIFE_TIME;
		/* アルファ値を初期化。*/
		m_alphaValue = ALPHA_VALUE;
		/* 拡大率を初期化。*/
		m_scaleValue = START_SCALE;
	}


	void PresentDamageIndicator::OnRelease()
	{
		/* 非アクティブ化。*/
		m_isActive = false;
		/* タイマーの初期化。*/
		m_lifeTimer = INIT_LIFE_TIME;
		/* アルファ値を設定。*/
		m_alphaValue = ALPHA_VALUE;
		/* 拡大率を設定。*/ 
		m_scaleValue = SCALE_VALUE;
	}


	float PresentDamageIndicator::ComputeLifeRate() const
	{
		/* タイムの補正。*/
		if(m_lifeTime <= INIT_LIFE_TIME)
			return RETURN_LIFE_TIME;

		/* 割合を計算。*/
		auto finalLifeRate = m_lifeTimer / m_lifeTime;

		/* 0.0f～1.0fの範囲に補正。*/ 
		if(finalLifeRate < CLAMP_LIFE_RATE_MIN)
			finalLifeRate = CLAMP_LIFE_RATE_MIN;

		if(finalLifeRate > CLAMP_LIFE_RATE_MAX)
			finalLifeRate = CLAMP_LIFE_RATE_MAX;

		return finalLifeRate;
	}

	
	Vector3 PresentDamageIndicator::ComputeDrawPosition(float lifeRate) const
	{
		/* 最終的な座標データを取得する。*/
		Vector3 finalPosition = m_startPosition;
		/* 上昇する距離を加算。*/ 
		finalPosition.y += FLOAT_UP_DISTANCE * lifeRate;

		/* 最終的な座標を返す。*/
		return finalPosition;
	}


	float PresentDamageIndicator::ComputeScale(float lifeRate) const
	{
		/* 最終的な拡大率を計算。*/
		return START_SCALE + (END_SCALE - START_SCALE) * lifeRate;
	}


	Vector3 PresentDamageIndicator::ComputeShadowPosition(const Vector3& drawPosition) const
	{
		/* 影の座標を計算。*/
		Vector3 shadowPosition = drawPosition;
		shadowPosition.x += SHADOW_OFFSET_X;
		shadowPosition.y += SHADOW_OFFSET_Y;

		return shadowPosition;
	}


	Vector4 PresentDamageIndicator::ComputeTextColor(float alpha) const
	{
		/* テキストの色を計算。*/
		return Vector4(DAMAGE_COLOR_R, DAMAGE_COLOR_G, DAMAGE_COLOR_B, alpha);
	}


	Vector4 PresentDamageIndicator::ComputeShadowColor(float alpha) const
	{
		/* 影の色を計算。*/
		return Vector4(SHADOW_INIT_VALUE_R, SHADOW_INIT_VALUE_G, SHADOW_INIT_VALUE_B, alpha * SHADOW_ALPHA_RATE);
	}


	void PresentDamageIndicator::ApplyFontParameter()
	{
		/* 座標を取得する。*/
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