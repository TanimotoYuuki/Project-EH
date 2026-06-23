#pragma once

/**
 * @file NPCMovementController.h
 * @brief NPCの移動制御を行うクラスの宣言。
 * @author  Yamaguchi Hayato
 * @date    2024/06/21: クラスを作成。
 * @details NPCMovementIntentを受け取り、VirtualInputAdapterに対して移動入力を適用する。
 */ 

#include "NPCMovementIntent.h"

namespace nsApp
{
	class VirtualInputAdapter;

	class NPCMovementController
	{
	public:
		/**
		 * @brief NPCの移動意図をVirtualInputAdapterに適用する。
		 * @param vInput 仮想入力クラスのポインタ。
		 * @param intent　NPCの移動意図。
		 */
		static void Apply(VirtualInputAdapter* vInput, const NPCMovementIntent& intent);

		/**
		 * @brief NPCの移動入力を止める。
		 * @param vInput 仮想入力クラスのポインタ。
		 */
		static void Stop(VirtualInputAdapter* vInput);

		/**
		 * @brief NPCの移動意図を作成する。
		 * @param direction 移動方向。
		 * @param useRun true なら LB1 押しっぱなし。
		 * @return NPCの移動意図。
		 */
		static NPCMovementIntent MakeMoveIntent(const Vector3& direction, bool useRun);
	};
}