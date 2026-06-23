#pragma once
#include "stdint.h"

namespace nsApp
{
	/**
	 * @enum NPCIntent
	 * @brief NPCの行動意図を表す列挙型。
	 */
	enum class NPCIntent : uint8_t
	{
		Idle,	//! 待機。
		Chase,  //! 追跡。
		Attack,	//! 攻撃。
		Evade,	//! 回避。
		Help,	//! 助け。
		Heal,	//! 回復。
		Guard,	//! ガード。
		Num		//! 初期化用。
	};
}