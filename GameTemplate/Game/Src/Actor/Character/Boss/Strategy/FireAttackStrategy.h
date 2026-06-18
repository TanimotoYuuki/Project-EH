#pragma once
/**
 * @file   FireAttackStrategy.h
 * @brief  ボス火炎攻撃ストラテジー。
 */

#include "Src/Actor/Character/Boss/Strategy/IBossAttackStrategy.h"

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsAI
	{
		/**
		 * @class FireAttackStrategy
		 * @brief 火炎アニメーションと火球発射タイミングを制御する攻撃ストラテジー。
		 */
		class FireAttackStrategy : public IBossAttackStrategy
		{
		public:
			/**
			 * @brief 初期化処理。
			 * @param boss　攻撃主体のボス。Boss::Start() から呼ぶ。
			 */
			void Enter(nsActor::Boss* boss) override;

			/**
			 * @brief 更新処理。
			 * @param boss　攻撃主体のボス。Boss::Update() から呼ぶ。
			 */
			void Update(nsActor::Boss* boss) override;

			/**
			 * @brief 終了処理。
			 * @param boss 攻撃主体のボス。Boss::EndCurrentAttack() から呼ぶ。
			 */
			void Exit(nsActor::Boss* boss) override;

			/**
			 * @brief 攻撃終了判定。火炎攻撃は、火球を発射してから一定時間経過したら終了する。
			 * @return 攻撃終了条件を満たしているかどうか。Boss::Update() から呼ぶ。
			 */
			bool IsEnd() override;


		private:
			float m_timer = 0.0f;
			bool m_isShot = false;
		};
	}
}
