﻿#include "stdafx.h"
#include "BossDamageState.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Sound/SoundLister.h"
#include "Src/Sound/SEList.h"

namespace nsApp
{
	namespace nsState
	{
		void BossDamageState::Enter()
		{
			/* ボスステートマシンのオーナーを Boss* にキャストして保持 */
			m_boss = static_cast<nsActor::Boss*>(m_owner);

			/* ダメージモーション再生と無敵時間開始 */
			m_timer = 0.5f;

			/* アニメーションの再生。*/
			m_boss->PlayAnimation(nsActor::BossAnimationID::GetHit);

			/* HPの初期化。*/
			m_boss->ResetPrevHP();

			/*被弾時の音再生。*/
			auto soundManager = FindGO<nsSound::SoundLister>("SoundManager");
			if (soundManager != nullptr && reinterpret_cast<uintptr_t>(soundManager))
			{
				soundManager->GetSEList().PlaySE(nsSound::SE_ID::HitDamage, 1.0f, false, 100.0f);
			}
		}


		void BossDamageState::Update()
		{
			/* タイマー更新 */
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}


		bool BossDamageState::RequestID(uint8_t& id)
		{
			if (m_timer <= 0.0f)
			{
				/* ダメージステートの時間が経過したら待機ステートへ遷移を要求 */
				id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);
				return true;
			}
			return false;
		}
	}
}
