#include "stdafx.h"
#include "ReboneGaugeUI.h"

namespace nsApp
{
	namespace nsUI
	{
		void ReboneGaugeUI::Init()
		{
			m_target = nullptr;
			m_position = Vector3::Zero;
			m_rate = 0.0f;
			m_isActive = false;

			/*
				Frame と Energy は同じサイズで初期化する。
				Energy だけ進行率に合わせて等比スケールする。
			*/
			m_frameSprite.Init("Assets/sprite/inGame/battle/reboneUI_Frame.DDS", 180.0f, 270.0f);
			m_energySprite.Init("Assets/sprite/inGame/battle/reboneUI_Energy.DDS", 180.0f, 270.0f);

			/* どちらも同じ基準点にする。*/
			m_frameSprite.SetPosition(m_position);
			m_energySprite.SetPosition(m_position);

			/*
				下から溜まるように、下端を基準にする。
				もし上から縮む場合は Vector2(0.5f, 0.0f) に変更して確認する。
			*/
			m_frameSprite.SetPivot(Vector2(0.5f, 1.0f));
			m_energySprite.SetPivot(Vector2(0.5f, 1.0f));

			m_frameSprite.SetScale(Vector3::One);
			m_energySprite.SetScale(Vector3(0.0f, 0.0f, 1.0f));

			m_frameSprite.Update();
			m_energySprite.Update();
		}


		void ReboneGaugeUI::Update()
		{
			if (m_target == nullptr)
			{
				m_isActive = false;
				m_rate = 0.0f;
				return;
			}

			auto& rescue = m_target->GetRescueStatusManager();

			if (!m_target->IsDeath() || !rescue.IsBeingHelped())
			{
				m_isActive = false;
				m_rate = 0.0f;
				return;
			}

			m_isActive = true;
			m_rate = ClampRate(rescue.GetHelpRate());
		}


		void ReboneGaugeUI::Render(RenderContext& rc)
		{
			if (!m_isActive)
				return;

			/* 確認用。必ず半分表示されるはず。*/
			DrawGauge(rc, m_rate);
		}


		void ReboneGaugeUI::DrawGauge(RenderContext& rc, float rate)
		{
			rate = ClampRate(rate);

			const float width = 180.0f;
			const float height = 270.0f;

			/* 現在のビューポートを取得する。*/
			D3D12_VIEWPORT viewport = rc.GetViewport();

			/*
				SpriteRenderの座標は画面中心基準。
				D3D12_RECTのシザー座標は画面左上基準。
				そのため、Sprite座標をスクリーン座標へ変換する。
			*/
			const float screenX = viewport.Width * 0.5f + m_position.x;
			const float screenY = viewport.Height * 0.5f + m_position.y;

			/* フレーム位置。*/
			m_frameSprite.SetPosition(m_position);
			m_frameSprite.SetPivot(Vector2(0.5f, 1.0f));
			m_frameSprite.SetScale(Vector3::One);
			m_frameSprite.Update();

			/*
				Energy本体は常にフルサイズで描画する。
				増えて見せる処理はスケールではなく、シザー矩形で行う。
			*/
			m_energySprite.SetPosition(m_position);
			m_energySprite.SetPivot(Vector2(0.5f, 1.0f));
			m_energySprite.SetScale(Vector3::One);
			m_energySprite.Update();

			/* 下から見える高さを増やす。*/
			const float fillHeight = height * rate;

			/*
				Spriteの表示位置は、今の見た目では m_position が画像の中心扱いになっている。
				そのため、シザー範囲の下端は screenY ではなく screenY + height * 0.5f にする。
			*/
			const float screenLeft = screenX - width * 0.5f;
			const float screenRight = screenX + width * 0.5f;
			const float screenBottom = screenY + height * 0.5f;
			const float screenTop = screenBottom - fillHeight;

			D3D12_RECT clipRect;
			clipRect.left = static_cast<LONG>(screenLeft);
			clipRect.top = static_cast<LONG>(screenTop);
			clipRect.right = static_cast<LONG>(screenRight);
			clipRect.bottom = static_cast<LONG>(screenBottom);


			/* 念のため画面外にはみ出さないようにする。*/
			if (clipRect.left < 0)
				clipRect.left = 0;

			if (clipRect.top < 0)
				clipRect.top = 0;

			if (clipRect.right > static_cast<LONG>(viewport.Width))
				clipRect.right = static_cast<LONG>(viewport.Width);

			if (clipRect.bottom > static_cast<LONG>(viewport.Height))
				clipRect.bottom = static_cast<LONG>(viewport.Height);

			/*
				高さが0以下ならEnergyは描画しない。
				これを入れないと、rateが0の時に変な1ラインが出る可能性がある。
			*/
			if (rate > 0.0f && clipRect.top < clipRect.bottom)
			{
				m_energySprite.SetClipRect(clipRect);
				m_energySprite.Draw(rc);
			}

			/* 最後にフレームを上から描画する。*/
			m_frameSprite.Draw(rc);
		}


		void ReboneGaugeUI::SetTarget(nsActor::Player* target)
		{
			if (m_target == target)
				return;

			m_target = target;
			m_rate = 0.0f;
			m_isActive = target != nullptr;
		}


		void ReboneGaugeUI::ClearTarget()
		{
			m_target = nullptr;
			m_rate = 0.0f;
			m_isActive = false;
		}


		void ReboneGaugeUI::SetPosition(const Vector3& position)
		{
			m_position = position;
		}


		bool ReboneGaugeUI::IsActive() const
		{
			return m_isActive;
		}


		float ReboneGaugeUI::ClampRate(float rate) const
		{
			if (rate < 0.0f)
				return 0.0f;

			if (rate > 1.0f)
				return 1.0f;

			return rate;
		}
	}
}
