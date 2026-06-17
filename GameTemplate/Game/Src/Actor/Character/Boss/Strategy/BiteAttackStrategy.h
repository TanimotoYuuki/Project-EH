#pragma once
/**
 * @file   BiteAttackStrategy.h
 * @brief  ボス噛みつき攻撃ストラテジー。
 * @date   2026/06/16: クラス更新日。
 */

#include "Src/Actor/Character/Boss/Strategy/IBossAttackStrategy.h"

namespace nsApp
{
	namespace nsActor  {
		class Boss; 
	}

	namespace nsAI
	{
		/**
		 * @class BiteAttackStrategy
		 * @brief 噛みつきアニメーションとヒット判定を制御する攻撃ストラテジー。
		 */
		class BiteAttackStrategy : public IBossAttackStrategy
		{
		public:
			/**
			 * @brief 攻撃開始。Boss::StartBiteAttack() から呼ぶ。
			 * @param boss 攻撃主体のボス。アニメーション再生やヒット判定配置のために必要。
			 */
			void Enter(nsActor::Boss* boss) override;

			/**
			 * @brief 更新処理。
			 * @param boss 攻撃主体のボス。アニメーション再生やヒット判定配置のために必要。Boss::Update() から呼ぶ。
			 */
			void Update(nsActor::Boss* boss) override;

			/**
			 * @brief 攻撃終了。Boss::EndBiteAttack() から呼ぶ。
			 * @param boss 攻撃主体のボス。アニメーション停止やヒット判定削除のために必要。
			 */
			void Exit(nsActor::Boss* boss) override;

			/**
			 * @brief 攻撃終了判定。Boss::Update() から呼ぶ。
			 * @return 攻撃が終了しているかどうか。終了している場合は次の行動へ移行するため、true を返すべき。
			 */
			bool IsEnd() override;


		private:
			float m_timer = 0.0f;		//! 攻撃開始からの経過時間。攻撃の進行管理に使用。
			bool m_isAttack = false;	//! 攻撃中かどうか。攻撃の進行管理に使用。
			bool m_hasPlayedSound =false;	//! すでにSEを再生下かどうか。
		};
	}
}
