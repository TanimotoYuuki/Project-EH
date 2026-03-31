#pragma once
/**
 * @file UIAnimation.h。
 * @brief UIアニメーションクラス。
 * @author Tanimoto。
 * @date 2026/03/05。
 */
namespace nsApp{
	namespace nsUI {
		/*UIアニメーションの基底クラス。*/
		class UIAnimationBase
		{
		public:
			/**
			* @brief 引数付きコンストラクタ。
			* @param render アニメーションさせるスプライト。
			* @param targetTime ターゲットの割合。
			* @param playSpeed アニメーションの再生速度。
			* @param isLoop ループするか？
			* @param startDelayTime アニメーションを開始する前の遅延時間。
			* @param endDelayTime アニメーションを終了した後の遅延時間。
			*/
			UIAnimationBase(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const float startDelayTime, const float endDelayTime)
				: m_render(render)
				, m_targetTime(targetTime)
				, m_playSpeed(playSpeed)
				, m_isLoop(isLoop)
				, m_startDelayTime(startDelayTime)
				, m_endDelayTime(endDelayTime)
			{
			}
			virtual ~UIAnimationBase() {};/*純粋仮想デストラクタ。*/

		public:
			virtual void Update() = 0;/*更新処理。*/

		protected:/*列挙型。*/

			//アニメーションステップ。
			enum EnAnimationStep
			{
				enAnimationStep_Min,//下限
				enAnimationStep_Max//上限
			};

		public:/*メンバ関数。*/

			/**
			* @brief リセット処理。
			*/
			inline void Reset()
			{
				m_elapsedTime = 0.0f;
				m_currentStep = enAnimationStep_Min;
				m_isCompleted = false;
				m_isEnd = false;
			}

			/**
			* @brief アニメーションの再生終了したか？
			* @return trueなら再生終了している。
			*/
			inline bool IsEnd() const
			{
				return m_isEnd;
			}

		protected:/*メンバ変数。*/
			SpriteRender* m_render = nullptr;/*アニメーションさせるスプライト。*/
			float m_elapsedTime = 0.0f;/*経過した割合。*/
			float m_targetTime = 0.0f;/*ターゲットの割合。*/
			float m_playSpeed = 1.0f;/*アニメーションの再生速度。*/
			float m_startDelayTime = 0.0f;/*アニメーションを開始する前の遅延時間。*/
			float m_endDelayTime = 0.0f;/*アニメーションを終了した後の遅延時間。*/
			EnAnimationStep m_currentStep = enAnimationStep_Min;/*現在のステップ。*/
			bool m_isLoop = false;/*ループするか ?*/
			bool m_isEnd = false;/*アニメーションの再生終了したか？*/
			bool m_isCompleted = false;/*処理が完了したか ?*/
		};

		/*座標を変えるUIアニメーションクラス。*/
		class PositionUIAnimation : public UIAnimationBase
		{
		public:
			/**
			* @brief 引数付きコンストラクタ。
			* @param render アニメーションをさせるスプライト。
			* @param targetTime ターゲットの割合。
			* @param playSpeed アニメーションの再生速度。
			* @param isLoop ループするか？
			* @param startDelayTime アニメーションを開始する前の遅延時間。
			* @param endDelayTime アニメーションを終了した後の遅延時間。
			* @param basePosition 元の位置。
			* @param targetPosition ターゲットの位置。
			*/
			PositionUIAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const float startDelayTime, const float endDelayTime, const Vector3 basePosition, const Vector3 targetPosition)
				: UIAnimationBase(render, targetTime, playSpeed, isLoop, startDelayTime, endDelayTime)
				, m_basePosition(basePosition)
				, m_targetPosition(targetPosition)
			{
			}

			~PositionUIAnimation() {};/*デストラクタ。*/

		public:
			void Update() override;/*更新処理。*/

			/**
			* @brief 元の位置を設定。
			* @param basePosition 元の位置
			*/
			void SetBasePosition(const Vector3 basePosition)
			{
				m_basePosition = basePosition; 
			}

			/**
			* @brief ターゲットの位置を設定。
			* @param targetPosition ターゲットの位置。
			*/
			void SetTargetPosition(const Vector3 targetPosition)
			{
				m_targetPosition = targetPosition;
			}

		private:/*メンバ変数。*/
			Vector3 m_basePosition = Vector3::Zero;/*元の位置。*/
			Vector3 m_targetPosition = Vector3::Zero;/*ターゲットの位置。*/
		};

		/*回転を変えるUIアニメーションクラス。*/
		class RotationUIAnimation : public UIAnimationBase
		{
		public:
			/**
			* @brief 引数付きコンストラクタ。
			* @param render アニメーションをさせるスプライト。
			* @param targetTime ターゲットの割合。
			* @param playSpeed アニメーションの再生速度。
			* @param isLoop ループするか？
			* @oaram startDelayTime アニメーションを開始する前の遅延時間。
			* @param endDelayTime アニメーションを終了した後の遅延時間。
			* @param basePosition 元の位置。
			* @param targetPosition ターゲットの位置。
			*/
			RotationUIAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const float startDelayTime, const float endDelayTime, const Quaternion baseRotation, const Quaternion targetRotation)
				: UIAnimationBase(render, targetTime, playSpeed, isLoop, startDelayTime, endDelayTime)
				, m_baseRotation(baseRotation)
				, m_targetRotation(targetRotation)
			{
			}

			~RotationUIAnimation() {};/*デストラクタ。*/

		public:
			void Update() override;/*更新処理。*/

			/**
			* @brief 元の回転を設定。
			* @param baseRotation 元の回転。
			*/
			void SetBaseRotation(const Quaternion baseRotation)
			{
				m_baseRotation = baseRotation;
			}

			/**
			* @brief ターゲットの回転を設定。
			* @param targetRotation ターゲットの回転。
			*/
			void SetTargetRotation(const Quaternion targetRotation)
			{
				m_targetRotation = targetRotation;
			}

		private:
			Quaternion m_baseRotation = Quaternion::Identity;/*元の回転。*/
			Quaternion m_targetRotation = Quaternion::Identity;/*ターゲットの回転。*/
		};

		/*大きさを変えるアニメーションクラス。*/
		class ScaleUIAnimation : public UIAnimationBase
		{
		public:
			/**
			* @brief 引数付きコンストラクタ。
			* @param render アニメーションをさせるスプライト。
			* @param targetTime ターゲットの割合。
			* @param playSpeed アニメーションの再生速度。
			* @param isLoop ループするか？
			* @param startDelayTime アニメーションを開始する前の遅延時間。
			* @param endDelayTime アニメーションを終了した後の遅延時間。
			* @param basePosition 元の大きさ。
			* @param targetPosition ターゲットの大きさ。
			*/
			ScaleUIAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const float startDelayTime, const float endDelayTime, const Vector2 baseScale, const Vector2 targetScale)
				: UIAnimationBase(render, targetTime, playSpeed, isLoop, startDelayTime, endDelayTime)
				, m_baseScale(baseScale)
				, m_targetScale(targetScale)
			{
			}

			~ScaleUIAnimation() {};/*デストラクタ。*/

		public:
			void Update() override;/*更新処理。*/

			/**
			* @brief 元の大きさを設定。
			* @param baseScale 元の大きさ。
			*/
			void SetBaseScale(const Vector2 baseScale)
			{
				m_baseScale = baseScale;
			}

			/**
			* @brief ターゲットの大きさを設定。
			* @param targetScale ターゲットの大きさ。
			*/
			void SetTargetScale(const Vector2 targetScale)
			{
				m_targetScale = targetScale;
			}

		private:
			Vector2 m_baseScale = Vector2::Zero;/*元の大きさ。*/
			Vector2 m_targetScale = Vector2::Zero;/*ターゲットの大きさ。*/
		};

		/*色を変えるアニメーションクラス。*/
		class ColorUIAnimation : public UIAnimationBase
		{
		public:
			/**
			* @brief 引数付きコンストラクタ。
			* @param render アニメーションをさせるスプライト。
			* @param targetTime ターゲットの割合。
			* @param playSpeed アニメーションの再生速度。
			* @param isLoop ループするか？
			* @param startDelayTime アニメーションを開始する前の遅延時間。
			* @param endDelayTime アニメーションを終了した後の遅延時間。
			* @param basePosition 元の色。
			* @param targetPosition ターゲットの色。
			*/
			ColorUIAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const float startDelayTime, const float endDelayTime, const Vector3 baseColor, const Vector3 targetColor)
				: UIAnimationBase(render, targetTime, playSpeed, isLoop, startDelayTime, endDelayTime)
				, m_baseColor(baseColor)
				, m_targetColor(targetColor)
			{
			}

			~ColorUIAnimation() {};/*デストラクタ。*/ 
			
		public:
			void Update() override;/*更新処理。*/

			/**
			* @brief 元の色を設定。
			* @param baseColor 元の色。
			*/
			void SetBaseColor(const Vector4 baseColor)
			{
				m_baseColor = baseColor;
			}

			/**
			* @brief ターゲットの色を設定。
			* @param targetColor ターゲットの色。
			*/
			void SetTargetColor(const Vector4 targetColor)
			{
				m_targetColor = targetColor;
			}

		private:
			Vector4 m_baseColor = Vector4::White;/*元の色。*/
			Vector4 m_targetColor = Vector4::White;/*ターゲットの色。*/
		};

		/*透明度を変えるアニメーションクラス。*/
		class AlphaUIAnimation : public UIAnimationBase
		{
		public:
			/**
			* @brief 引数付きコンストラクタ。
			* @param render アニメーションをさせるスプライト。
			* @param targetTime ターゲットの割合。
			* @param playSpeed アニメーションの再生速度。
			* @param isLoop ループするか？
			* @param startDelayTime アニメーションを開始する前の遅延時間。
			* @param endDelayTime アニメーションを終了した後の遅延時間。
			* @param basePosition 元の透明度。
			* @param targetPosition ターゲットの透明度。
			*/
			AlphaUIAnimation(SpriteRender* render, const float targetTime, const float playSpeed, const bool isLoop, const float startDelayTime, const float endDelayTime, const float baseAlpha, const float targetAlpha)
				: UIAnimationBase(render, targetTime, playSpeed, isLoop, startDelayTime, endDelayTime)
				, m_baseAlpha(baseAlpha)
				, m_targetAlpha(targetAlpha)
			{
			}

			~AlphaUIAnimation() {};/*デストラクタ。*/

		public:
			void Update() override;/*更新処理。*/

			/**
			* @brief 元の透明度を設定。
			* @param baseAlpha 元の透明度。
			*/
			void SetBaseAlpha(const float baseAlpha)
			{
				m_baseAlpha = baseAlpha;
			}

			/**
			* @brief ターゲットの透明度を設定。
			* @param targetAlpha ターゲットの透明度。
			*/
			void SetTargetAlpha(const float targetAlpha)
			{
				m_targetAlpha = targetAlpha;
			}

		private:
			float m_baseAlpha = 0.0f;/*元の透明度。*/
			float m_targetAlpha = 0.0f;/*ターゲットの透明度。*/
		};
	}
}

