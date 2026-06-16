#include "stdafx.h"
#include "BossPhaseController.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Boss/AI/BossAIConfig.h"


namespace nsApp
{
	namespace nsActor
	{
		void BossPhaseController::Init()
		{
			/* フェーズ状態を初期値に戻す */
			m_phase1Triggered = false;
			m_phase2Triggered = false;
			m_idleTimeMultiplier = 1.0f;
		}


		void BossPhaseController::CheckAndApplyPhase(Boss* boss)
		{
			if (boss == nullptr)
				return;

			if (boss->GetCharacterStatus().hp.currentHP <= 0)
				return;

			const float hpRatio = boss->GetHPRatio();

			/* フェーズ1：HP 閾値到達で攻撃力・待機時間を強化 */
			if (!m_phase1Triggered && hpRatio <= nsAI::BossAIConfig::PHASE1_HP_THRESHOLD)
			{
				m_phase1Triggered = true;
				boss->MultiplyAttackStats(m_phase1DamageMultiplier);
				m_idleTimeMultiplier = m_phase1IdleTimeMultiplier;
			}

			/* フェーズ2：さらに HP が減ったら追加強化 */
			if (!m_phase2Triggered && hpRatio <= nsAI::BossAIConfig::PHASE2_HP_THRESHOLD)
			{
				m_phase2Triggered = true;
				boss->MultiplyAttackStats(m_phase2DamageMultiplier);
				m_idleTimeMultiplier = m_phase2IdleTimeMultiplier;
			}
		}
	}
}