#pragma once
/**
* @file  PresentDamageIndicator.h
* @brief ボスに与えるダメージ数を即座に描画/削除を行うクラス。
* @author Yamaguchi Hayato
* @date  2026/03/19
*/

#include "Src/Actor/Character/Common/Pool/IPoolable.h"


namespace nsApp
{
	class PresentDamageIndicator 
		: public IGameObject, public IPoolable
	{
	public:
		/* コンストラクタとデストラクタ。*/
		PresentDamageIndicator() = default;
		virtual ~PresentDamageIndicator() = default;


	public:
		/**
		 * ダメージ量と描画位置を受け取り初期化する。
		 * @param damageValue ダメージ量。
		 * @param drawPosition 描画位置。
		 */
		void Init(int damageValue, const Vector3& drawPosition);

		/*
		 * 更新処理。
		 */
		void Update();

		/*
		 * 描画処理。
		 */
		void Render(RenderContext& rc);


	public:
		/**
		 * @brief プールから取得された時の処理。
		 */
		void OnAcquire() override;

		/**
		 * @brief プールへ戻す時の処理。
		 */
		void OnRelease() override;

		/**
		 * @brief 使用中か判定する。
		 * @return 使用中ならtrue。
		 */
		inline bool IsActive() const override
		{
			return m_isActive;
		}

		/**
		 * @brief 非アクティブ化する。
		 */
		inline void Deactivate() override
		{
			OnRelease();
		}


	private:
		/**
		 * @brief 寿命の進行率を計算する。
		 * @return 0.0f～1.0fの進行率。
		 */
		float ComputeLifeRate() const;

		/**
		 * @brief 現在の描画位置を計算する。
		 * @param lifeRate 寿命の進行率。
		 * @return 現在の描画位置。
		 */
		Vector3 ComputeDrawPosition(float lifeRate) const;

		/**
		 * @brief 現在の拡大率を計算する。
		 * @param lifeRate 寿命の進行率。
		 * @return 現在の拡大率。
		 */
		float ComputeScale(float lifeRate) const;

		/**
		 * @brief 現在の透明度を計算する。
		 * @param lifeRate 寿命の進行率。
		 * @return 現在の透明度。
		 * @detail 1.0 全体割合。
		 */
		inline float ComputeAlpha(float lifeRate) const
		{
			return 1.0f - lifeRate;
		}

		/**
		 * @brief 影の描画位置を計算する。
		 * @param drawPosition 本体文字の描画位置。
		 * @return 影の描画位置。
		 */
		Vector3 ComputeShadowPosition(const Vector3& drawPosition) const;

		/**
		 * @brief ダメージ文字の色を計算する。
		 * @param alpha 透明度。
		 * @return ダメージ文字の色。
		 */
		Vector4 ComputeTextColor(float alpha) const;

		/**
		 * @brief 影の色を計算する。
		 * @param alpha 透明度。
		 * @return 影の色。
		 */
		Vector4 ComputeShadowColor(float alpha) const;

		/**
		 * @brief フォント描画用パラメータを反映する。
		 */
		void ApplyFontParameter();

		/**
		 * @brief 表示時間が終わったか判定する。
		 * @return 設定時間が終わっていればtrue。
		 */
		inline bool IsDead() const
		{
			return m_lifeTimer >= m_lifeTime;
		}


	private:
		int m_damageValue = 0;                      //! ダメージ量。

		float m_lifeTimer = 0.0f;                   //! 経過時間。
		float m_lifeTime = 0.8f;                    //! 表示寿命。
		float m_alphaValue = 1.0f;                  //! 現在の透明度。
		float m_scaleValue = 1.0f;                  //! 現在の拡大率。

		bool m_isActive = false;					//! 使用中か。

		Vector3 m_startPosition = Vector3::Zero;    //! 描画開始位置。
		Vector3 m_drawPosition = Vector3::Zero;     //! 現在の描画位置。

		FontRender m_damageFont;                    //! 本体文字。
		FontRender m_shadowFont;                    //! 影文字。

		std::wstring m_damageText;                  //! 表示する文字列。
	};
}
