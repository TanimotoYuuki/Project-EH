#pragma once

/**
 * @file   LoadingRandomRunnerModel.h
 * @brief  ローディング画面でランダムな走りモデルを表示するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/06/09: リファクタリング。
 * @detail MemberSelectと同じように、キャラクタークラスをNewGOして表示する。
 * @detail NewGO… 本開発エンジンにて用いるゲームオブジェクト生成関数。引数は順に、生成レイヤー数,タグ名。
 */

namespace nsApp
{
	namespace nsActor {
		class IWeaponCharacter;
	}


	namespace nsUI
	{
		class LoadingRandomRunnerModel 
		{
		public:
			/* コンストラクタとデストラクタ。*/
			LoadingRandomRunnerModel() = default;
			virtual ~LoadingRandomRunnerModel();


		public:
			/**
			 * @brief 初期化処理。
			 */
			void Initialize();

			/**
			 * @brief 更新処理。
			 * @param progress ローディング進捗率。0.0fから1.0f。
			 */
			void Update(float progress);

			/**
			 * @brief 描画処理。
			 * @param rc レンダリングコンテキスト。
			 */
			void Render(RenderContext& rc) {};


		/* セッター。*/
		public:
			/**
			 * @brief ランナーモデルの移動範囲を設定する。
			 * @param startPosition 移動開始位置。
			 * @param endPosition 移動終了位置。
			 */
			void SetMoveRange(const Vector3& startPosition, const Vector3& endPosition);

			/**
			 * @brief ランナーを指定座標へ直接配置する。
			 * @param position 配置するワールド座標。
			 */
			void SetPreviewPosition(const Vector3& position);


		/* ゲッター。*/
		public:
			/**
			 * @brief ランナーの向き角度を取得する。
			 * @return ランナーのY軸角度。
			 */
			float GetRunnerAngleY() const;

			/**
			 * @brief ローディングが完了しているかどうかを取得する。
			 * @return ローディングが完了している場合はtrue、そうでない場合はfalse.
			 */
			inline bool IsFinished() const
			{
				return m_progress >= 1.0f;
			}


		/* ヘルパー。*/
		private:
			/**
			 * @brief 値を0.0fから1.0fの範囲に補正する。
			 * @param value 補正したい値。
			 * @return 補正後の値。
			 */
			float Clamp01(float value) const;

			/**
			 * @brief 表示するキャラクターをランダムに生成する。
			 */
			void CreateRandomRunner();

			/**
			 * @brief 進捗率に応じて座標を更新する。
			 * @param progress ローディング進捗率。
			 */
			void UpdatePosition(float progress);


		private:
			nsActor::IWeaponCharacter* m_runner = nullptr; //! ローディング中に走るキャラクター。


		private:
			Vector3 m_startPosition = Vector3::Zero;       //! 進捗0%時の座標。
			Vector3 m_endPosition = Vector3::Zero;		   //! 進捗100%時の座標。
			Vector3 m_currentPosition = Vector3::Zero;     //! 現在座標。

			int m_randomIndex = 0;						   //! ランダーのランダムインデックス。

			float m_progress = 0.0f;					   //! 現在の進捗率。
			float m_targetProgress = 0.0f;				   //! 外から受け取った目標進捗率。
			float m_runnerProgress = 0.0f;				   //! ランナー専用の進捗率。
			float m_lastRunnerX = 0.0f;					   //! 前回のランナーX座標。
			float m_nextTargetProgress = 0.0f;			   //! 次の目標進捗率。
			float m_updatePosRate = 0.0f;				   //! 位置更新のための累積時間。
			float m_targetX = 0.0f;						   //! 目標X座標。
			float m_minX = 0.0f;						   //! 移動範囲の最小X座標。
			float m_maxX = 0.0f;						   //! 移動範囲の最大X座標。
			float m_clampedX = 0.0f;					   //! クランプされたX座標。

			bool m_isInitialized = false;				   //! 初期化済みかどうか。
			bool m_isMoveRangeReady = false;			   //! 移動範囲が設定済みかどうか。
		};
	}
}