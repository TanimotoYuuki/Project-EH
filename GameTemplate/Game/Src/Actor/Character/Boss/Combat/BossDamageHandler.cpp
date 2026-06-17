#include "stdafx.h"
#include "BossDamageHandler.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Boss/AI/BossAIConfig.h"


namespace nsApp
{
	namespace nsActor
	{
		void BossDamageHandler::Init()
		{
			/* 硬直関連の内部状態を初期化 */
			m_accumulatedDamage = 0;
			m_damageResetTimer = 0.0f;
			m_flinchCooldownTimer = 0.0f;
		}


		void BossDamageHandler::UpdateTimers(float deltaTime)
		{
			/* 累積ダメージリセットタイマー */
			if (m_damageResetTimer > 0.0f)
			{
				m_damageResetTimer -= deltaTime;
				if (m_damageResetTimer <= 0.0f)
					m_accumulatedDamage = 0;
			}

			/* 硬直クールダウンタイマー */
			if (m_flinchCooldownTimer > 0.0f)
				m_flinchCooldownTimer -= deltaTime;
		}


		void BossDamageHandler::HandleDamageEvent(Boss* boss, int damage)
		{
			if (boss == nullptr)
				return;

			/* 死亡・被ダメ・攻撃中は硬直しない */
			if (boss->GetCharacterStatus().hp.currentHP <= 0)
				return;

			const uint8_t stateID = boss->GetCurrentStateID();
			if (stateID == BossStateID::enDamage || stateID == BossStateID::enDeath)
				return;

			if (stateID == BossStateID::enAttack)
				return;

			/* 短時間内の被ダメージを累積 */
			if (damage > 0)
			{
				m_accumulatedDamage += damage;
				m_damageResetTimer = nsAI::BossAIConfig::DAMAGE_RESET_TIME;
			}

			/* 累積ダメージが閾値を超え、硬直クールダウン中でなければ被ダメステートへ */
			if (m_accumulatedDamage >= nsAI::BossAIConfig::FLINCH_DAMAGE_THRESHOLD &&
				m_flinchCooldownTimer <= 0.0f)
			{
				boss->EnterFlinchDamageState();
				m_accumulatedDamage = 0;
				m_flinchCooldownTimer = nsAI::BossAIConfig::FLINCH_COOLDOWN;
			}
		}
	}
}