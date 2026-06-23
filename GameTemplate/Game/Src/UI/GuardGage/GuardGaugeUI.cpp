#include "stdafx.h"
#include "GuardGaugeUI.h"

namespace
{
	const float GUARD_UI_WIDTH = 512.0f;
	const float GUARD_UI_HEIGHT = 64.0f;
	const float GUARD_UI_WORLD_OFFSET_Y = 8.0f;   //! 足元より少し上（微調整）

	const Vector3 GUARD_UI_SCALE = Vector3(0.35f, 0.35f, 1.0f);
	const Vector2 GUARD_UI_PIVOT = Vector2(0.5f, 0.5f);
}


namespace nsApp
{
	namespace nsUI
	{
		void GuardGaugeUI::Init()
		{
			m_target = nullptr;
			m_position = Vector3::Zero;
			m_rate = 0.0f;
			m_isActive = false;

			m_frameSprite.Init("Assets/sprite/inGame/battle/guardUI_frame.DDS", GUARD_UI_WIDTH, GUARD_UI_HEIGHT);
			m_valueSprite.Init("Assets/sprite/inGame/battle/guardFrame_value.DDS", GUARD_UI_WIDTH, GUARD_UI_HEIGHT);

			m_frameSprite.SetPivot(GUARD_UI_PIVOT);
			m_valueSprite.SetPivot(GUARD_UI_PIVOT);
			m_frameSprite.SetScale(GUARD_UI_SCALE);
			m_valueSprite.SetScale(GUARD_UI_SCALE);
		}

		void GuardGaugeUI::SetTarget(nsActor::Player* target)
		{
			m_target = target;
		}

		void GuardGaugeUI::ClearTarget()
		{
			m_target = nullptr;
			m_isActive = false;
			m_rate = 0.0f;
		}

		Vector3 GuardGaugeUI::CalcScreenPosition(const Vector3& worldPos) const
		{
			if (g_camera3D == nullptr)
				return Vector3::Zero;

			/* 足元より少し上を基準にする。*/
			Vector3 anchor = worldPos;
			anchor.y += GUARD_UI_WORLD_OFFSET_Y;

			/* エンジン標準のワールド→スクリーン変換（画面中心基準）。*/
			Vector2 screenPos;
			g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, anchor);

			return Vector3(screenPos.x, screenPos.y, 0.0f);
		}


		void GuardGaugeUI::Update()
		{
			if (m_target == nullptr)
			{
				m_isActive = false;
				return;
			}

			const auto& guard = m_target->GetGuardSystem();
			if (!guard.IsGaugeVisible())
			{
				m_isActive = false;
				return;
			}

			m_isActive = true;
			m_rate = ClampRate(guard.GetRemainingRate());
		}
		

		void GuardGaugeUI::Render(RenderContext& rc)
		{
			if (!m_isActive || m_target == nullptr)
				return;

			/* 描画直前にカメラ基準で位置を求める。*/
			m_position = CalcScreenPosition(m_target->GetPosition());

			DrawGauge(rc, m_rate);
		}


		void GuardGaugeUI::DrawGauge(RenderContext& rc, float rate)
		{
			rate = ClampRate(rate);

			const float drawWidth = GUARD_UI_WIDTH * GUARD_UI_SCALE.x;
			const float drawHeight = GUARD_UI_HEIGHT * GUARD_UI_SCALE.y;

			D3D12_VIEWPORT viewport = rc.GetViewport();
			const float screenX = viewport.Width * 0.5f + m_position.x;
			const float screenY = viewport.Height * 0.5f - m_position.y; 

			// 1. 黒枠（常にフル）
			m_frameSprite.SetPosition(m_position);
			m_frameSprite.SetPivot(GUARD_UI_PIVOT);  // (0.5, 0.5)
			m_frameSprite.SetScale(GUARD_UI_SCALE);
			m_frameSprite.Update();
			m_frameSprite.Draw(rc);

			// 2. 赤（残量分だけ Clip）
			m_valueSprite.SetPosition(m_position);
			m_valueSprite.SetPivot(GUARD_UI_PIVOT);
			m_valueSprite.SetScale(GUARD_UI_SCALE);
			m_valueSprite.Update();


			const float fillWidth = drawWidth * rate;
			const float screenLeft = screenX - drawWidth * 0.5f;
			const float screenRight = screenLeft + fillWidth;
			const float screenTop = screenY - drawHeight * 0.5f;
			const float screenBottom = screenY + drawHeight * 0.5f;
			D3D12_RECT clipRect;
			clipRect.left = static_cast<LONG>(screenLeft);
			clipRect.top = static_cast<LONG>(screenTop);
			clipRect.right = static_cast<LONG>(screenRight);
			clipRect.bottom = static_cast<LONG>(screenBottom);

			if (rate > 0.0f && clipRect.right > clipRect.left)
			{
				m_valueSprite.SetClipRect(clipRect);
				m_valueSprite.Draw(rc);
			}
		}

		float GuardGaugeUI::ClampRate(float rate) const
		{
			if (rate < 0.0f) return 0.0f;
			if (rate > 1.0f) return 1.0f;
			return rate;
		}
	}
}