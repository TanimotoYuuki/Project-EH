#pragma once
/**
 * @file   BossRoarState.h
 * @brief  ボス咆哮ステート。
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
		 * @class BossRoarState
		 * @brief フェーズイベント等でボスが咆哮アニメーションを再生するステート。
		 */
		class BossRoarState : public IState<nsActor::Actor>
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
			 * @brief 終了処理。特に何もしない。
			 */
			void Exit() override {};

			/**
			 * @brief 次のステートへの遷移要求。咆哮時間が終了したら、HP に応じて Move か Attack をランダムに選んで遷移要求する。
			 * @param id 次のステート ID を格納する参照。遷移要求する場合はこれに遷移先ステートの ID をセットすること。
			 * @return 遷移要求する場合は true、しない場合は false。
			 */
			bool RequestID(uint8_t& id) override;

		private:
			nsActor::Boss* m_boss = nullptr;


		private:
			float m_timer = 0.0f;
		};
	}
}
