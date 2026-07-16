#pragma once

/**
 * @file   EffectAsyncLoadTask.h
 * @brief  エフェクトの非同期先読みタスク。
 * @note   Effekseer は Main でのみ初期化する。
 *         Worker では実処理せず、Finalize で EffectList::Init する。
 *         Player::Start 側の Init はそのまま（登録済みならスキップ）。
 */

#include "AsyncLoadTaskBase.h"

namespace nsApp
{
	/**
	 * @class EffectAsyncLoadTask
	 * @brief Loading 中にエフェクトを EffectEngine へ登録するタスク。
	 */
	class EffectAsyncLoadTask : public AsyncLoadTaskBase
	{
	public:
		/* コンストラクタとデストラクタ。*/
		EffectAsyncLoadTask() = default;
		virtual ~EffectAsyncLoadTask() = default;


	protected:
		/**
		 * @brief ワーカースレッド側。
		 * @return 常に true（実処理は Main 側）。
		 * @note   Effekseer::Effect::Create は Worker で呼ばない。
		 */
		bool LoadOnWorkerThread() override;

		/**
		 * @brief メインスレッドでエフェクトを登録する。
		 * @return 登録に成功した場合は true。
		 */
		bool FinalizeOnMainThread() override;
	};
}