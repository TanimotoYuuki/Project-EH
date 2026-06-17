#pragma once
/**
 * @file   BossAttackState.h
 * @brief  ボス攻撃ステート。距離と AI 設定に応じて攻撃ストラテジーを選択する。
 */

#include <memory>
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Boss/Strategy/IBossAttackStrategy.h"
#include "Src/Actor/Character/Boss/AI/BossAIConfig.h"

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsState
	{
		/**
		 * @class BossAttackState
		 * @brief 噛み・尻尾・火炎などの攻撃ストラテジーを実行するステート。
		 */
		class BossAttackState : public IState<nsActor::Actor>
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
			void Exit() override;

			/**
			 * @brief 攻撃ストラテジーから呼び出される、攻撃アニメーションの ID をリクエストするための関数。
			 * @param id 攻撃アニメーションの ID を受け取る参照。IBossAttackStrategy の実装クラスがこの関数を呼び出すときに、攻撃に対応するアニメーション ID をセットしてもらう。
			 * @return 攻撃アニメーションの ID を正常にセットできた場合は true、そうでない場合は false を返す。通常は、攻撃ストラテジーが要求するアニメーション ID を正しくセットできれば true を返し、何らかの理由でセットできない（例えば、攻撃ストラテジーが要求するアニメーション ID が存在しないなど）場合は false を返すことになる。
			 */
			bool RequestID(uint8_t& id) override;


		private:
			nsActor::Boss* m_boss = nullptr;

			/** @brief 現在実行中の攻撃ストラテジー。 */
			std::unique_ptr<nsAI::IBossAttackStrategy> m_strategy;
		};
	}
}
