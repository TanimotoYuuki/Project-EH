#pragma once

#include "Src/Actor/Character/NPC/Component/NPCBehaviorProfile.h"
#include "Src/Actor/Character/NPC/Threat/ThreatCollector.h"

namespace nsApp
{
	namespace nsActor {
		class Player;
	}

	/**
	 * @struct NPCMindContext
	 * @brief 各 Mind が毎フレーム参照する共有コンテキスト（脳幹が組み立てる）。
	 */
	struct NPCMindContext
	{
		nsActor::Player* outer = nullptr;		     //! ポインタを持つ対象。
		const NPCBehaviorProfile* profile = nullptr; //! NPCの行動プロファイル。
		nsNPC::ThreatCollector* threats = nullptr;   //! ThreatCollector へのポインタ。
		int partyIndex = 0;							 //! パーティインデックス。
		nsActor::Player* helpTarget = nullptr;	     //! 助ける対象。
	};
}