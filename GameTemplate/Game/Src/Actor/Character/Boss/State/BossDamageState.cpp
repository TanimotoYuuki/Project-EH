#include "stdafx.h"
#include "BossDamageState.h"
#include "Src/Actor/Character/Boss/Boss.h"


namespace nsApp
{
	namespace nsState
	{
		void BossDamageState::Enter()
		{
			/* ボスステートマシンのオーナーを Boss* にキャストして保持 */
			m_boss = static_cast<nsActor::Boss *>(m_owner);

			/* ダメージモーション再生と無敵時間開始 */
			m_timer = 0.5f;

			/* アニメーションの再生。*/
			m_boss->PlayAnimation(nsActor::BossAnimationID::GetHit);

			/* HPの初期化。*/
			m_boss->ResetPrevHP();
		}


		void BossDamageState::Update()
		{
			/* タイマー更新 */
			m_timer -= g_gameTime->GetFrameDeltaTime();
		}


		bool BossDamageState::RequestID(uint8_t &id)
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
