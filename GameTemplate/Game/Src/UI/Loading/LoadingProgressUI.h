#pragma once

#include <array>
#include <cstdint>


namespace nsApp
{
	namespace nsUI
	{
		/**
		 * @brief ローディングの進行度を表示するためのUIの種類を表す列挙型。
		 */
		enum class LoadingProgressUIKind : uint8_t
		{
			Background,		  //! ロ―ディング背景。

			ProgressBar,      //! 進捗に応じて伸びる青いバー。
			ProgressFrame,    //! プログレスバーの外枠。

			WeaponIconHammer, //! ハンマーアイコン。
			WeaponIconSword,  //! 剣アイコン。
			WeaponIconWand,   //! 杖アイコン。
			WeaponIconGun,    //! 銃アイコン。

			Num				  //! 総数。
		};


		/**
		 * @struct UISetting
		 * @brief ローディングUIの設定。
		 */
		struct UISetting
		{
			const char* filePath;		  //! テクスチャファイルのパス。
			float width;				  //! テクスチャの幅。
			float height;				  //! テクスチャの高さ。
			Vector3 position;			  //! UIの位置。
			Vector3 scale;				  //! UIのスケール。
			bool isBackGroundRender;	  //! モデルより先に描画するか。
		};

		class LoadingProgressUI
		{
		public:
			/* コンストラクタとデストラクタ。*/
			LoadingProgressUI() = default;
			virtual ~LoadingProgressUI() = default;


		public:
			/**
			 * @brief 初期化処理。
			 */
			void Initialize();

			/**
			 * @brief 更新処理。
			 * @param progress ローディングの進行度を表す0.0fから1.0fの値。
			 */
			void Update(float progress);

			/**
			 * @brief 通常描画処理。
			 * @details 3Dモデルを挟まない場合に使う。
			 */
			void Render(RenderContext& rc);

			/**
			 * @brief 背面側の描画処理。
			 * @details ProgressBarなど、3Dランナーモデルより奥に描画する。
			 */
			void RenderBack(RenderContext& rc);

			/**
			 * @brief 前面側の描画処理。
			 * @details ProgressFrameや武器アイコンなど、3Dランナーモデルより手前に描画する。
			 */
			void RenderFront(RenderContext& rc);


		private:
			/**
			 * @brief 各UIの初期化。
			 */
			void InitializeSprites();

			/**
			 * @brief 各UIの基本位置を初期化。
			 */
			void InitializeBasePositions();

			/**
			 * @brief プログレスバーを更新する。
			 */
			void UpdateProgressBar();

			/**
			 * @brief 武器アイコンの跳ねアニメーションを更新する。
			 */
			void UpdateWeaponIconBounce();

			/**
			 * @brief 指定したUI番号をintに変換する。
			 * @param kind 変換するUI番号。
			 */
			inline int ToIndex(LoadingProgressUIKind kind) const
			{
				return static_cast<int>(kind);
			}

			/**
			 * @brief 値を0.0fから1.0fの範囲にクランプする関数。
			 * @param value クランプする値。
			 */
			float Clamp01(float value) const;

			/**
			 * @brief UIの設定を行う関数。
			 * @param kind 設定するUIの種類。
			 * @param path テクスチャファイルのパス。
			 * @param width テクスチャの幅。
			 * @param height テクスチャの高さ。
			 * @param scale UIのスケール。
			 * @param isBgRender モデルより先に描画するかどうか。
			 */
			void SetupSpriteSetting(LoadingProgressUIKind kind, const char* path, float width, float height, const Vector3& scale, bool isBgRender);

			/**
			 * @brief 各UIの基本位置を設定するヘルパー関数
			 * @param kind 設定するUIの種類
			 * @param x X座標
			 * @param y Y座標
			 */
			inline void SetBasePosition(LoadingProgressUIKind kind, float x, float y)
			{
				m_basePositionList[ToIndex(kind)] = Vector3(x, y, 0.0f);
			}


		/* ゲッター。*/
		public:
			/**
			 * @brief ローディングアニメーションが完了したかどうかの取得。
			 * @return ローディングアニメーションが完了しているかどうか。
			 */
			inline bool IsProgressAnimationFinish() const 
			{
				return m_displayProgress >= 1.0f;;
			}

			/**
			 * @brief プログレスバーの左端のスクリーンX座標の取得。
			 * @return プログレスバーの左端のスクリーンX座標。
			 */
			inline float GetBarLeftScreenX() const
			{
				int progressIndex = ToIndex(LoadingProgressUIKind::ProgressBar);
				return m_basePositionList[progressIndex].x;
			}

			/**
			 * @brief プログレスバーの右端のスクリーンX座標の取得。
			 * @return プログレスバーの右端のスクリーンX座標。
			 */
			float GetBarRightScreenX() const;

			/**
			 * @brief プログレスバーの上端のスクリーンY座標の取得。
			 * @return プログレスバーの上端のスクリーンY座標。
			 */
			float GetBarTopScreenY() const;

			/**
			 * @brief 表示用進捗率の取得。
			 * @return 補間済みの進捗率。
			 */
			inline float GetDisplayProgress() const
			{
				return m_displayProgress;
			}


		private:
			static const int UI_NUM = static_cast<int>(LoadingProgressUIKind::Num);				//! UIの総数。
			static const int WEAPON_ICON_NUM = 4;												//! 武器アイコンの数。

			std::array<SpriteRender, UI_NUM> m_spriteList;										//! 各UIのスプライト。
			std::array<Vector3, UI_NUM> m_basePositionList;										//! 各UIの基本位置。プログレスバーの伸びや武器アイコンの跳ねはこの位置を基準にする。

			std::array<LoadingProgressUIKind, WEAPON_ICON_NUM> m_weaponIconOrder = {			//! 武器アイコンの表示順。これも基本位置と同じ順番で定義する。
				LoadingProgressUIKind::WeaponIconSword,											//! 剣。
				LoadingProgressUIKind::WeaponIconHammer,										//! ハンマー。
				LoadingProgressUIKind::WeaponIconWand,											//! 杖。
				LoadingProgressUIKind::WeaponIconGun											//! 銃。
			};

			float m_progress = 0.0f;															//! 実際の進捗率。
			float m_displayProgress = 0.0f;														//! 見た目用に補間した進捗率。
			float m_elapsedFrame = 0.0f;														//! アニメーション用経過フレーム。
			float m_progressBarBaseScaleX = 1.0f;												//! プログレスバーの基本スケールX。
			float m_bounceHeight = 18.0f;														//! アイコンの跳ね高さ。
			float m_bounceSpeed = 0.18f;														//! アイコンの跳ね速度。
			float m_bounceDelayFrame = 8.0f;													//! アイコンごとの跳ね遅延。
			float m_backProgress = 0.0f;														//! 背面側の進捗率。武器アイコンの表示タイミングに使う。
			float m_screenCenterX = 0.0f;														//! スクリーン中心のX座標。
			float m_screenCenterY = 0.0f;														//! スクリーン中心のY座標。
			float m_cycleFrame = 0.0f;														    //! アイコンのアニメーション周期。		
			float m_loopFrame = 0.0f;															//! アイコンのアニメーションループ用フレーム。
			float m_startFrame = 0.0f;															//! アイコンのアニメーション開始フレーム。武器アイコンの表示タイミングに使う。
			float m_iconFrame = 0.0f;															//! アイコンのアニメーションフレーム。武器アイコンの表示タイミングに使う。
			float m_weponIconRate = 0.0f;														//! アイコンのアニメーション率。武器アイコンの表示タイミングに使う。
			float m_weponIconWave = 0.0f;														//! アイコンのアニメーション波形。武器アイコンの表示タイミングに使う。

			bool m_isInGameBuildStarted = false;												//! InGame生成が開始したかどうか。

			int m_index = 0;																	//! インデックス。
			int m_updateIndex = 0;																//! 更新用インデックス。武器アイコンの表示タイミングに使う。
			int m_setUpIndex = 0;																//! 設定用インデックス。
			int m_weponIconIndex = 0;															//! 武器アイコンのインデックス。武器アイコンの表示タイミングに使う。

			LoadingProgressUIKind m_kind;														//! UIの種類。

			D3D12_VIEWPORT m_viewport;															//! ビューポート。
			D3D12_RECT m_clipRect;																//! クリッピング矩形。

			Vector3 m_position = Vector3::Zero;													//! UI全体の位置。
			Vector3 m_updateScale = Vector3::One;												//! 更新用スケール。武器アイコンの表示タイミングに使う。
			Vector3 m_weponIconPosition = Vector3::Zero;										//! 武器アイコンの位置。武器アイコンの表示タイミングに使う。

			LONG m_barLeft = 0;																	//! プログレスバーの左端のスクリーンX座標。
			LONG m_barTop = 0;																	//! プログレスバーの上端のスクリーンY座標。
			LONG m_barRight = 0;																//! プログレスバーの右端のスクリーンX座標。
			LONG m_barBottom = 0;																//! プログレスバーの下端のスクリーンY座標。
		};
	}
}