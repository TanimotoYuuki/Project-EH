#pragma once

namespace nsK2Engine {
	/// <summary>
	/// スプライトレンダラー。
	/// </summary>
	class SpriteRender : public IRenderer
	{
	public:
		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		/// <param name="w">画像の横幅。</param>
		/// <param name="h">画像の縦幅。</param>
		/// <param name="isBackGraundRender">背景描画パスで描画するかどうか？ trueならモデルよりも先に描画する。</param>
		/// <param name="alphaBlendMode">デフォルトは半透明合成。</param>
		void Init(const char* filePath, const float w, const float h, bool isBackGroundRender = false, AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans);
		/// <summary>
		/// 座標を設定。zは0.0fで。
		/// </summary>
		/// <param name="pos">座標。</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// 座標を取得。
		/// </summary>
		/// <returns>座標。</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// 大きさを設定。zは1.0fで。
		/// </summary>
		/// <param name="scale">大きさ。</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// 大きさを取得。
		/// </summary>
		/// <returns>大きさ。</returns>
		const Vector3& GetScale() const
		{
			return m_scale;
		}
		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <param name="rot">回転。</param>
		void SetRotation(const Quaternion& rot)
		{
			m_rotation = rot;
		}
		/// <summary>
		/// 回転を取得。
		/// </summary>
		/// <returns>回転。</returns>
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}
		/// <summary>
		/// ピボットを設定。
		/// </summary>
		/// <param name="pivot">ピボット。</param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// ピボットを取得。
		/// </summary>
		/// <returns>ピボット。</returns>
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}
		/// <summary>
		/// 乗算カラーを設定。
		/// </summary>
		/// <param name="mulColor">乗算カラー。</param>
		void SetMulColor(const Vector4& mulColor)
		{
			if (m_backGroundSprite.IsInited()) {
				m_backGroundSprite.SetMulColor(mulColor); 
				return; 
			};
			m_sprite.SetMulColor(mulColor);
		}
		/// <summary>
		/// 乗算カラーを取得。
		/// </summary>
		/// <returns></returns>
		const Vector4& GetMulColor() const
		{
			if (m_backGroundSprite.IsInited())
			{
				return m_backGroundSprite.GetMulColor();
			}
			return m_sprite.GetMulColor();
		}
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update()
		{
			if (m_backGroundSprite.IsInited()){
				m_backGroundSprite.Update(m_position,
					m_rotation,
					m_scale,
					m_pivot);
			}
			else {
				m_sprite.Update(m_position,
					m_rotation,
					m_scale,
					m_pivot);
			}
		}
		/// <summary>
		/// 描画処理。
		/// </summary>
		/// <param name="rc">レンダーコンテキスト。</param>
		void Draw(RenderContext& rc);


		/*
		 *@brief クリップ矩形を設定する。
		 * @param rect クリップ矩形。
		 */
		void SetClipRect(const D3D12_RECT& clipRect)
		{
			m_clipRect = clipRect;
			m_isClipEnabled = true;
		}

		/**
		 * @brief クリップ矩形を解除する。
		 */
		void ClearClipRect()
		{
			m_isClipEnabled = false;
		}


	private:
		/// <summary>
		/// 2D描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		void OnRender2D(RenderContext& rc) override
		{
			if (m_sprite.IsInited()) {
				if (m_isClipEnabled)
				{
					/* 現在のビューポートを取得して、描画後に元へ戻す。*/
					D3D12_VIEWPORT viewport = rc.GetViewport();

					D3D12_RECT defaultRect;
					defaultRect.left = 0;
					defaultRect.top = 0;
					defaultRect.right = static_cast<LONG>(viewport.Width);
					defaultRect.bottom = static_cast<LONG>(viewport.Height);

					/* クリップ範囲を設定して描画。*/
					rc.SetScissorRect(m_clipRect);
					m_sprite.Draw(rc);

					/* 次のSprite描画に影響しないように戻す。*/
					rc.SetScissorRect(defaultRect);
					return;
				}

				m_sprite.Draw(rc);
			}
		}
		/// <summary>
		/// 背景描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderBackGround(RenderContext& rc) override
		{
			if (m_backGroundSprite.IsInited()) {
				m_backGroundSprite.Draw(rc);
			}
		}

	private:
		Sprite			m_sprite;								//スプライト。
		Sprite          m_backGroundSprite;						//背景スプライト。
		Vector3			m_position = Vector3::Zero;				//座標。
		Quaternion		m_rotation = Quaternion::Identity;		//回転。
		Vector3			m_scale = Vector3::One;					//大きさ。
		Vector2			m_pivot = Sprite::DEFAULT_PIVOT;		//ピボット。
		bool m_isClipEnabled = false;   //! クリップ矩形が有効かどうか。
		D3D12_RECT m_clipRect{};        //! クリップ矩形。

	};
}
