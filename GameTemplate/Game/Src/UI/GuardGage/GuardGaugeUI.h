#pragma once
/**
 * @file   GuardGaugeUI.h
 * @brief  プレイヤー足元に追従するガードゲージUI。
 */

#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsUI
	{
		class GuardGaugeUI
		{
		public:
			/**
			 * @brief 初期化。
			 */
			void Init();

			/**
			 * @brief 更新。
			 */
			void Update();

			/**
			 * @brief 描画。
			 * @param rc 描画コンテキスト。
			 */
			void Render(RenderContext& rc);

			/**
			 * @brief 監視対象を設定する。
			 * @param target 監視するプレイヤー。
			 */
			void SetTarget(nsActor::Player* target);

			/**
			 * @brief 監視対象を解除する。
			 */
			void ClearTarget();

			/**
			 * @brief 表示中か判定する。
			 * @return 表示中なら true。
			 */
			bool IsActive() const
			{
				return m_isActive;
			}


		private:
			float ClampRate(float rate) const;
			Vector3 CalcScreenPosition(const Vector3& worldPos) const;
     		void DrawGauge(RenderContext& rc, float rate);


		private:
			nsActor::Player* m_target = nullptr; //! 監視対象プレイヤー。
			Vector3 m_position = Vector3::Zero;  //! UIの基準座標（画面中心基準）。
			float m_rate = 0.0f;                 //! 残量率。
			bool m_isActive = false;             //! 表示中か。

			SpriteRender m_frameSprite;          //! ガード枠UI。
			SpriteRender m_valueSprite;          //! ガード残量UI。
		};
	}
}