#pragma once
/**
 * @file   BossIdleState.h
 * @brief  ボス待機ステート。次行動の抽選と待機時間管理を行う。
 */

#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Boss/AI/BossAIConfig.h"

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsState
	{
		/**
		 * @class BossIdleState
		 * @brief ボスが Idle アニメーションで待機し、Move / Attack へ遷移するステート。
		 */
		class BossIdleState : public IState<nsActor::Actor>
		{
		public:
			/**
			 * @brief 初期化処理。
			 */
			void Enter() override;

			/**
			 * @brief 更新処理。
			 */
			void Update() override;

			/**
			 * @brief 終了処理。
			 */
			void Exit() override {};

			/**
			 * @brief 次の状態 ID を抽選する。BossStateID 列挙体の値を uint8_t にキャストして返すこと。
			 * @param id 抽選された次の状態 ID を格納する参照。BossStateID 列挙体の値を uint8_t にキャストして格納すること。
			 * @return 抽選が成功したかどうか。BossStateID 列挙体の値を uint8_t にキャストして返すこと。抽選に失敗する可能性はほとんどないが、万が一失敗した場合は false を返すこと。
			 */
			bool RequestID(uint8_t& id) override;

		private:
			nsActor::Boss* m_boss = nullptr; //! ボス本体へのポインタ。Enter() でキャストして取得する。


		private:
			float m_timer = 0.0f;			//! 待機時間の残り。Enter() で初期化し、Update() で減算していく。これが 0 以下になったら次の行動を抽選できるようになる。
		};
	}
}
