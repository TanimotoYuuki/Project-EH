#pragma once
/**
 * @file   TailAttackStrategy.h
 * @brief  ボス尻尾攻撃ストラテジー。
 */

#include "Src/Actor/Character/Boss/Strategy/IBossAttackStrategy.h"

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsAI
	{
		/**
		 * @class TailAttackStrategy
		 * @brief 尻尾攻撃アニメーション・ヒット・竜巻生成を制御する攻撃ストラテジー。
		 */
		class TailAttackStrategy : public IBossAttackStrategy
		{
		public:
			/**
			 * @brief 初期化処理。
			 * @param boss 攻撃主体のボス。Boss::Start() から呼ぶ。
			 */
			void Enter(nsActor::Boss* boss) override;

			/**
			 * @brief 攻撃更新処理。Boss::Update() から呼ぶ。
			 * @param boss 攻撃主体のボス。Boss::Update() から呼ぶ。
			 */
			void Update(nsActor::Boss* boss) override;

			/**
			 * @brief 終了処理。攻撃終了時に一度だけ呼ばれる。Boss::Update() から呼ぶ。
			 * @param boss 攻撃主体のボス。Boss::Update() から呼ぶ。
			 */
			void Exit(nsActor::Boss* boss) override;

			/**
			 * @brief 攻撃終了判定。Boss::Update() から呼ぶ。
			 * @return 攻撃終了とみなす条件を満たしている場合 true。そうでない場合 false。
			 */
			bool IsEnd() override;

		private:
			float m_timer = 0.0f; //! 攻撃開始からの経過時間。攻撃終了判定やヒット判定のタイミングに使用する。
			bool m_isHit = false; //! ヒット判定を既に行ったかどうか。これが false のときにヒット判定を行い、ヒット判定を行ったら true にする。
		};
	}
}
