#pragma once

namespace nsApp
{
	/**
	 * @struct NPCMovementIntent
	 * @brief NPCの移動意図を表す構造体。
	 */
	struct NPCMovementIntent
	{
		Vector3 direction = Vector3::Zero;  //! XZ 移動方向（正規化済み想定）。
		bool useRun = true;                 //! true なら LB1 押しっぱなし。
		bool stop = false;                  //! true なら移動入力を止める。	
	};
}