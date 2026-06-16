#pragma once
/**
 * @file   BossDethState.h
 * @brief  ボス死亡ステート。
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
		 * @class BossDethState
		 * @brief 死亡アニメーションを再生し、以降の遷移を行わないステート。
		 */
		class BossDethState : public IState<nsActor::Actor>
		{
		public:
			/**
			 * @brief 初期化処理。
			 */
			void Enter() override;

			/**
			 * @brief 更新日。
			 */
			void Update() override;

			/**
			 * @brief 終了処理。
			 */
			void Exit() override;

			/**
			 * @brief このステートは遷移を要求しないので、常に false を返す。
			 * @param id 遷移先ステート ID を受け取る参照。使用しないが、IState のインターフェースを満たすために引数として受け取る。
			 * @return 常に false を返す。
			 */
			bool RequestID(uint8_t& id) override;


		private:
			nsActor::Boss* m_boss = nullptr; //! ボスのポインタ。
		};
	}
}
