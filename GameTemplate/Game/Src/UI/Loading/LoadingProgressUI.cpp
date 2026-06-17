#include "stdafx.h"
#include "LoadingProgressUI.h"

#include <cmath>

namespace
{
	const char* SPRITE_SCENE_PATH = "Assets/sprite/loading/SpritteScene.DDS";						//! ローディング背景。
	const char* PROGRESS_BAR_PATH = "Assets/sprite/loading/progress.DDS";							//! 進捗に応じて伸びる青いバー。
	const char* PROGRESS_FRAME_PATH = "Assets/sprite/loading/flame.DDS";							//! プログレスバーの外枠。
	const char* HAMMER_ICON_PATH = "Assets/sprite/loading/Hammer.DDS";								//! ハンマーアイコン。
	const char* SWORD_ICON_PATH = "Assets/sprite/loading/Sword.DDS";								//! 剣アイコン。
	const char* WAND_ICON_PATH = "Assets/sprite/loading/Wand.DDS";									//! 杖アイコン。
	const char* GUN_ICON_PATH = "Assets/sprite/loading/gun.DDS";									//! 銃アイコン。

	const auto LOADING_BACKGROUND_WIDTH = 1920.0f;													//! ローディング背景の幅。
	const auto LOADING_BACKGROUND_HEIGHT = 1080.0f;												    //! ローディング背景の高さ。
	const auto LOADING_UI_SCALE = 0.92f;															//! ローディングUIのスケール。
			
	const auto PROGRESS_UI_Y = -390.0f;																//! プログレスバーのUI上のY座標。
	const auto PROGRESS_BAR_BASE_WIDTH = 1280.0f;													//! プログレスバーの元の幅。
	const auto PROGRESS_BAR_BASE_HEIGHT = 320.0f;													//! プログレスバーの元の高さ。
	const auto PROGRESS_BAR_MOVE_SPEED = 1.0f;													    //! プログレスバーの移動速度。
	const auto PROGRESS_BAR_DRAW_WIDTH = PROGRESS_BAR_BASE_WIDTH * LOADING_UI_SCALE;				//! プログレスバーの描画幅。
	const auto PROGRESS_BAR_DRAW_HEIGHT = PROGRESS_BAR_BASE_HEIGHT * LOADING_UI_SCALE;				//! プログレスバーの描画高さ。

	const auto PROGRESS_FRAME_BASE_WIDTH = 1280.0f;													//! プログレスバーの外枠の元の幅。					
	const auto PROGRESS_FRAME_BASE_HEIGHT = 320.0f;													//! プログレスバーの外枠の元の高さ。
	const auto PROGRESS_FRAME_DRAW_WIDTH = PROGRESS_FRAME_BASE_WIDTH * LOADING_UI_SCALE;			//! プログレスバーの外枠の描画幅。
	const auto PROGRESS_FRAME_DRAW_HEIGHT = PROGRESS_FRAME_BASE_HEIGHT * LOADING_UI_SCALE;			//! プログレスバーの外枠の描画高さ。

	const auto PROGRESS_FOLLOW_SPEED = 0.12f;														//! プログレスバーの表示が実際の進捗に追従する速さ。
	const auto PROGRESS_COMPLETE_THRESHOLD = 0.995f;												//! プログレスバーの表示が完了とみなすための閾値。
	const auto PROGRESS_RATE_COMPLETE = 1.0f;														//! プログレスバーの完了とみなす進捗率。


	const auto PI = 3.1415926535;																	//! 円周率。
	const auto WEAPON_ICON_SCALE = 0.94f;															//! 武器アイコンのスケール。
	const auto WEAPON_ICON_BOUNCE_DURATION_FRAME = 16.0f;											//! 武器アイコンのバウンスアニメーションの1周期のフレーム数。
	const auto WEAPON_ICON_BOUNCE_WAIT_FRAME = 6.0f;												//! 武器アイコンのバウンスアニメーションの待機フレーム数。
	const auto WEAPON_ICON_BASE_WIDTH = 180.0f;													    //! 武器アイコンの元の幅。
	const auto WEAPON_ICON_BASE_HEIGHT = 180.0f;													//! 武器アイコンの元の高さ。
	const auto WEAPON_ICON_DRAW_WIDTH = WEAPON_ICON_BASE_WIDTH * WEAPON_ICON_SCALE;				    //! 武器アイコンの描画幅。
	const auto WEAPON_ICON_DRAW_HEIGHT = WEAPON_ICON_BASE_HEIGHT * WEAPON_ICON_SCALE;				//! 武器アイコンの描画高さ。
	const auto WEAPON_ICON_UI_Y = 30.0f;															//! 武器アイコンのUI上のY座標。
	const auto WEAPON_ICON_INTERVAL = 185.0f * WEAPON_ICON_SCALE;									//! 武器アイコン同士の間隔。

	const auto SCREEN_RATE = 0.5f;																	//! スクリーンの中心からの距離に対する武器アイコンのバウンスの振幅の割合。

	const auto SCALE_BACKGROUND = Vector3::One;														//! ローディング背景のスケール。
}

namespace nsApp
{
	namespace nsUI
	{
		void LoadingProgressUI::Initialize()
		{
			m_progress = 0.0f;
			m_displayProgress = 0.0f;
			m_elapsedFrame = 0.0f;

			InitializeBasePositions();
			InitializeSprites();
		}


		void LoadingProgressUI::Update(float progress)
		{
			/* プログレスバーの進捗度を0～1で管理。*/
			m_progress = Clamp01(progress);

			/* プログレスバーの表示を滑らかに更新。*/
			m_displayProgress += (m_progress - m_displayProgress) * PROGRESS_FOLLOW_SPEED;

			/* 武器アイコンのバウンスアニメーションを更新。*/
			if (m_progress >= PROGRESS_RATE_COMPLETE && m_displayProgress >= PROGRESS_COMPLETE_THRESHOLD)
				m_displayProgress = PROGRESS_RATE_COMPLETE;

			/* プログレスバーの進行度。*/
			m_elapsedFrame += PROGRESS_BAR_MOVE_SPEED;

			/* プログレスバーを更新する。*/
			UpdateProgressBar();

			/* 武器アイコンのバウンドを更新する。*/
			UpdateWeaponIconBounce();

			/* ローディングに使う画像を更新する。*/
			for (auto& sprite : m_spriteList)
				sprite.Update();
		}


		void LoadingProgressUI::Render(RenderContext& rc)
		{
			/* 背面側の描画。*/
			RenderBack(rc);
			/* 前面側の描画。*/
			RenderFront(rc);
		}


		void LoadingProgressUI::RenderBack(RenderContext& rc)
		{
			/* ローディング背景を描画。*/
			m_spriteList[ToIndex(LoadingProgressUIKind::Background)].Draw(rc);

			/* プログレスバーの描画。*/
			m_index = ToIndex(LoadingProgressUIKind::ProgressBar);

			/* ビューポートを取得。*/
			m_viewport = rc.GetViewport();

			/* プログレスバーの位置を計算。*/
			m_position = m_basePositionList[m_index];

			/* プログレスバーの進捗率を計算。*/
			m_backProgress  = Clamp01(m_displayProgress);

			/* スクリーン中心の座標を計算。*/
			m_screenCenterX = m_viewport.Width * SCREEN_RATE;
			m_screenCenterY = m_viewport.Height * SCREEN_RATE;

			/* プログレスバーの描画位置を計算。*/
			m_barLeft = static_cast<LONG>(m_screenCenterX + m_position.x);
			m_barTop = static_cast<LONG>(m_screenCenterY - m_position.y - PROGRESS_BAR_DRAW_HEIGHT * 0.5f);
			m_barRight = static_cast<LONG>(m_barLeft + PROGRESS_BAR_DRAW_WIDTH * m_backProgress);
			m_barBottom = static_cast<LONG>(m_barTop + PROGRESS_BAR_DRAW_HEIGHT);

			/* プログレスバーの描画位置をビューポート内に収める。*/
			if (m_barRight <= m_barLeft)
				m_barRight = m_barLeft + 1;
			if (m_barLeft < 0)
				m_barLeft = 0;
			if (m_barTop < 0)
				m_barTop = 0;
			if (m_barRight > static_cast<LONG>(m_viewport.Width))
				m_barRight = static_cast<LONG>(m_viewport.Width);
			if (m_barBottom > static_cast<LONG>(m_viewport.Height))
				m_barBottom = static_cast<LONG>(m_viewport.Height);

			/* プログレスバーのクリッピング矩形を設定して描画。*/
			m_clipRect.left = m_barLeft;
			m_clipRect.top = m_barTop;
			m_clipRect.right = m_barRight;
			m_clipRect.bottom = m_barBottom;

			/* プログレスバーの描画。*/
			m_spriteList[m_index].SetClipRect(m_clipRect);
			m_spriteList[m_index].Draw(rc);
		}


		void LoadingProgressUI::RenderFront(RenderContext& rc)
		{
			/* プログレスバーの外枠を描画。*/
			m_spriteList[ToIndex(LoadingProgressUIKind::ProgressFrame)].Draw(rc);

			/* 武器アイコンを描画。*/
			m_spriteList[ToIndex(LoadingProgressUIKind::WeaponIconSword)].Draw(rc);
			m_spriteList[ToIndex(LoadingProgressUIKind::WeaponIconHammer)].Draw(rc);
			m_spriteList[ToIndex(LoadingProgressUIKind::WeaponIconWand)].Draw(rc);
			m_spriteList[ToIndex(LoadingProgressUIKind::WeaponIconGun)].Draw(rc);
		}


		void LoadingProgressUI::InitializeSprites()
		{
			/* 背景画像を設定。*/
			SetupSpriteSetting(LoadingProgressUIKind::Background, SPRITE_SCENE_PATH, LOADING_BACKGROUND_WIDTH, LOADING_BACKGROUND_HEIGHT, Vector3(1.0f, 1.0f, 1.0f), true);

			/* プログレスバーと外枠を設定。*/
			SetupSpriteSetting(LoadingProgressUIKind::ProgressBar, PROGRESS_BAR_PATH, PROGRESS_BAR_BASE_WIDTH, PROGRESS_BAR_BASE_HEIGHT, Vector3(LOADING_UI_SCALE, LOADING_UI_SCALE, 1.0f), true);
			SetupSpriteSetting(LoadingProgressUIKind::ProgressFrame, PROGRESS_FRAME_PATH, PROGRESS_FRAME_BASE_WIDTH, PROGRESS_FRAME_BASE_HEIGHT, Vector3(LOADING_UI_SCALE, LOADING_UI_SCALE, 1.0f), false);

			/* 武器アイコンを設定。*/
			SetupSpriteSetting(LoadingProgressUIKind::WeaponIconHammer, HAMMER_ICON_PATH, WEAPON_ICON_BASE_WIDTH, WEAPON_ICON_BASE_HEIGHT, Vector3(WEAPON_ICON_SCALE, WEAPON_ICON_SCALE, 1.0f), false);
			SetupSpriteSetting(LoadingProgressUIKind::WeaponIconSword, SWORD_ICON_PATH, WEAPON_ICON_BASE_WIDTH, WEAPON_ICON_BASE_HEIGHT, Vector3(WEAPON_ICON_SCALE, WEAPON_ICON_SCALE, 1.0f), false);
			SetupSpriteSetting(LoadingProgressUIKind::WeaponIconWand, WAND_ICON_PATH, WEAPON_ICON_BASE_WIDTH, WEAPON_ICON_BASE_HEIGHT, Vector3(WEAPON_ICON_SCALE, WEAPON_ICON_SCALE, 1.0f), false);
			SetupSpriteSetting(LoadingProgressUIKind::WeaponIconGun, GUN_ICON_PATH, WEAPON_ICON_BASE_WIDTH, WEAPON_ICON_BASE_HEIGHT, Vector3(WEAPON_ICON_SCALE, WEAPON_ICON_SCALE, 1.0f), false);

			/* プログレスバーのピボットを左端に設定。*/
			m_spriteList[ToIndex(LoadingProgressUIKind::ProgressBar)].SetPivot(Vector2(0.0f, 0.5f));
			
			/* プログレスバーの基本スケールXを保存。*/
			m_progressBarBaseScaleX = m_spriteList[ToIndex(LoadingProgressUIKind::ProgressBar)].GetScale().x;
		}


		void LoadingProgressUI::InitializeBasePositions()
		{
			/* 背景UIの座標。*/
			SetBasePosition(LoadingProgressUIKind::Background, 0.0f, 0.0f);

			/* プログレスバーの座標。*/
			SetBasePosition(LoadingProgressUIKind::ProgressBar, -PROGRESS_BAR_DRAW_WIDTH * 0.5f, PROGRESS_UI_Y);

			/* フレームの座標。*/
			SetBasePosition(LoadingProgressUIKind::ProgressFrame, 0.0f, PROGRESS_UI_Y);

			/* 武器アイコンの座標。*/ 
			SetBasePosition(LoadingProgressUIKind::WeaponIconSword, -WEAPON_ICON_INTERVAL * 1.5f, WEAPON_ICON_UI_Y);
			SetBasePosition(LoadingProgressUIKind::WeaponIconHammer, -WEAPON_ICON_INTERVAL * 0.5f, WEAPON_ICON_UI_Y);
			SetBasePosition(LoadingProgressUIKind::WeaponIconWand, WEAPON_ICON_INTERVAL * 0.5f, WEAPON_ICON_UI_Y);
			SetBasePosition(LoadingProgressUIKind::WeaponIconGun, WEAPON_ICON_INTERVAL * 1.5f, WEAPON_ICON_UI_Y);
		}


		void LoadingProgressUI::UpdateProgressBar()
		{
			/* プログレスバーのスケールを更新。*/
			m_updateIndex =  ToIndex(LoadingProgressUIKind::ProgressBar);

			/* プログレスバーのスケールを取得。*/
			m_updateScale = m_spriteList[m_updateIndex].GetScale();

			/*
				ProgressBar画像自体は変形させない。
				進捗表示はRenderBack側のクリップ矩形で制御する。
			*/
			m_updateScale.x = m_progressBarBaseScaleX;

			/* スケールを反映。*/
			m_spriteList[m_updateIndex].SetScale(m_updateScale);
		}


		void LoadingProgressUI::UpdateWeaponIconBounce()
		{
			/* アイコンのアニメーション周期を計算。*/
		    m_cycleFrame = m_bounceDelayFrame * static_cast<float>(WEAPON_ICON_NUM - 1) + WEAPON_ICON_BOUNCE_DURATION_FRAME + WEAPON_ICON_BOUNCE_WAIT_FRAME;
			
			/* アイコンのアニメーションループ用フレームを計算。*/
			m_loopFrame = std::fmod(m_elapsedFrame, m_cycleFrame);

			/* 各武器アイコンの位置を更新。*/
			for (int i = 0; i < WEAPON_ICON_NUM; ++i)
			{
				/* アイコンの種類を取得。*/
				m_kind = m_weaponIconOrder[i];
				m_weponIconIndex = ToIndex(m_kind);

				/* アイコンの基本位置を取得。*/
				m_weponIconPosition = m_basePositionList[m_weponIconIndex];

				/* アイコンのアニメーション開始フレームを計算。*/
				m_startFrame = m_bounceDelayFrame * static_cast<float>(i);
				m_iconFrame = m_loopFrame - m_startFrame;

				/* アイコンのアニメーションフレームが0～WEAPON_ICON_BOUNCE_DURATION_FRAMEの範囲内なら、バウンスアニメーションを適用。*/
				if(m_iconFrame >= 0.0f && m_iconFrame <= WEAPON_ICON_BOUNCE_DURATION_FRAME)
				{
					/* アイコンのアニメーション率を計算。0～1の範囲になる。*/
					m_weponIconRate = m_iconFrame / WEAPON_ICON_BOUNCE_DURATION_FRAME;
					m_weponIconWave = std::sin(m_weponIconRate * PI);
					m_weponIconPosition.y += m_weponIconWave * m_bounceHeight;
				}

				/* アイコンの位置を反映。*/
				m_spriteList[m_weponIconIndex].SetPosition(m_weponIconPosition);
			}
		}


		float LoadingProgressUI::Clamp01(float value) const
		{
			/* 値を0～1の範囲に収める。*/
			if (value < 0.0f)
				return 0.0f;
			if (value > 1.0f)
				return 1.0f;

			/* 値が0～1の範囲内ならそのまま返す。*/
			return value;
		}


		void LoadingProgressUI::SetupSpriteSetting(LoadingProgressUIKind kind, const char* path, float width, float height, const Vector3& scale, bool isBgRender)
		{
			/* 指定されたUIのスプライト設定を更新する。*/
			m_setUpIndex = ToIndex(kind);

			/* スプライトを初期化。*/
			m_spriteList[m_setUpIndex].Init(path, width, height, isBgRender);

			/* 計算後の座標をセット。*/
			m_spriteList[m_setUpIndex].SetPosition(m_basePositionList[m_setUpIndex]);

			/* スケールをセット。*/
			m_spriteList[m_setUpIndex].SetScale(scale);
		}


		float LoadingProgressUI::GetBarRightScreenX() const
		{
			/* プログレスバーの左端のスクリーンX座標 + プログレスバーの描画幅 = プログレスバーの右端のスクリーンX座標。*/
			return GetBarLeftScreenX() + PROGRESS_BAR_DRAW_WIDTH;
		}


		float LoadingProgressUI::GetBarTopScreenY() const
		{
			/* プログレスバーの外枠の上端のスクリーンY座標は、プログレスバーの外枠の中心Y座標 + 半分の高さ。*/
			int getIndex = ToIndex(LoadingProgressUIKind::ProgressFrame);

			/*
				UI座標はY上方向がプラス。
				フレーム中心Y + 半分の高さ = 上端。
			*/
			return m_basePositionList[getIndex].y + PROGRESS_FRAME_DRAW_HEIGHT * 0.5f;
		}
	}
}