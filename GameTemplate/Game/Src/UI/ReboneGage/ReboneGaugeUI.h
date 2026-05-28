#pragma once
/**
 * @file   ReboneGaugeUI.h
 * @brief  復活ゲージUIを管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/06/15
 */

#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsUI
	{
		class ReboneGaugeUI
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
			 * @brief ターゲットを設定する。
			 * @param target 救助されているプレイヤー。
			 */
			void SetTarget(nsActor::Player* target);

			/**
			 * @brief ターゲットをクリアする。
			 */
			void ClearTarget();

			/**
			 * @brief UIの表示位置を設定する。
			 * @param position UIの基準座標。
			 */
			void SetPosition(const Vector3& position);

			/**
			 * @brief アクティブか判定する。
			 * @return アクティブならtrue。
			 */
			bool IsActive() const;

			/**
			 * @brief 現在の救助対象を取得する。
			 * @return 現在の救助対象。
			 */
			nsActor::Player* GetTarget() const
			{
				return m_target;
			}

		private:
			/**
			 * @brief 救助ゲージを描画する。
			 * @param rc 描画コンテキスト。
			 * @param rate 救助進行率。
			 */
			void DrawGauge(RenderContext& rc, float rate);

			/**
			 * @brief 進行率を0.0f〜1.0fに丸める。
			 * @param rate 進行率。
			 * @return 丸めた進行率。
			 */
			float ClampRate(float rate) const;


		private:
			nsActor::Player* m_target = nullptr; //! 救助されている対象。

			Vector3 m_position = Vector3::Zero;  //! UIの基準座標。

			float m_rate = 0.0f;                 //! 現在の表示進行率。
			bool m_isActive = false;             //! 表示中かどうか。

			SpriteRender m_frameSprite;          //! フレームのスプライト。
			SpriteRender m_energySprite;         //! エネルギーのスプライト。
		};
	}
}
