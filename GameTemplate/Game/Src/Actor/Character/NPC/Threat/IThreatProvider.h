#pragma once

/**
 * @class	IThreatProvider
 * @brief   ワールド上の危険源から IThreatZone を収集する。
 * @author  Yamaguchi Hayato
 * @date    2024/06/21: クラス作成
 * @details ワールド上の危険源を提供するためのインターフェース。
 *			IThreatProvider を実装するクラスは、CollectThreats メソッドを通じて IThreatZone を収集し、
 *			NPC がそれらのゾーンを考慮して行動できるようにします。
 */


#include "IThreatZone.h"
#include <vector>

namespace nsApp
{
	namespace nsNPC
	{
		class IThreatProvider
		{
		public:
			/* デストラクタ。*/
			virtual ~IThreatProvider() = default;


		public:
			/**
			 * @brief ワールド上の危険源から IThreatZone を収集する。
			 * @param outZones 収集された IThreatZone を格納するための出力パラメータ。
			 */
			virtual void CollectThreats(std::vector<IThreatZone*>& outZones) = 0;
		};
	}
}