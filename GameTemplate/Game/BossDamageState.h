#pragma once
/**
 * @file   BossDamageState.h
 * @brief  ボス被弾（怯み）ステート。
 * @date   2026/06/16: クラス更新日。
 */

#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/Common/IState.h"

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsState
	{
		/**
		 * @class BossDamageState
		 * @brief 被弾アニメーションを再生し、一定時間後に Idle へ戻るステート。
		 */
		class BossDamageState : public IState<nsActor::Actor>
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
			 * @brief 状態遷移要求処理。一定時間経過後に Idle へ遷移する。
			 */
			void Exit() override {};

			/**
			 * @brief 状態遷移要求処理。一定時間経過後に Idle へ遷移する。
			 * @param id 遷移先の状態 ID を格納する変数への参照。遷移可能な場合はこれに遷移先の ID をセットして true を返し、遷移不可能な場合は false を返す。
			 * @return 遷移可能なら true、そうでないなら false。
			 */
			bool RequestID(uint8_t& id) override;


		private:
			nsActor::Boss* m_boss = nullptr;	//! このステートの所有者である Boss へのポインタ。Enter() でキャストしてセットする。


		private:
			float m_timer = 0.5f;				//! 被弾ステートの残り時間。
		};
	}
}