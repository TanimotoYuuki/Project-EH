#include "stdafx.h"
#include "PlayerHeelMagicState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Effect/EffectList.h"
#include "Src/Actor/Heel/HeelArea.h"
#include "Src/Actor/Magic/Parameter/MagicParameterTable.h"

namespace
{
	const auto ATTACK_TIMER_EXECUTE = 25;	 //! 回復エリアを出すフレーム。
	const auto ATTACK_TIMER_END = 80;		 //! 通常終了フレーム。
	const auto ATTACK_TIMER_FORCE_END = 120; //! 保険終了フレーム。

	const auto EFFECT_Y_OFFSET = 2.0f; //! エフェクトのY座標オフセット。

	const auto HEAL_AMOUNT_BASE = 100;	  //! 回復量の基本値。
	const auto HEAL_AREA_RADIUS = 60.0f;  //! 回復エリアの半径。
	const auto HEAL_AREA_LIFETIME = 5.0f; //! 回復エリアの寿命。
	const auto HEAL_AREA_INTERVAL = 0.5f; //! 回復エリアの回復間隔。
	const auto HEEL_AREA_Y_OFFSET = 2.0f; //! 回復エリアのY座標オフセット。

	const auto BOSS_AWAY_OFFSET = 40.0f; //! 自己回復時、ボスから離す距離。
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerHeelMagicState::PlayAttackAnimation()
		{
			/* 攻撃の種類を設定する。*/
			SetCurrentAttackType(AttackType::HeelMagic);

			/* 再生するアニメーションをセット。*/
			m_player->PlayWeaponAnimation(AttackType::HeelMagic);
		}

		void PlayerHeelMagicState::OnEnterAttack()
		{
			/* チャージレベルを取得。*/
			m_chargeLevel = static_cast<float>(m_player->GetEffectScale());

			/* チャージしていない場合は発動不可。*/
			if (m_chargeLevel <= 0.0f)
			{
				m_canExecuteHeelMagic = false;
				m_hasExecutedHeelMagic = true;
				return;
			}

			/* エフェクト再生。*/
			m_canExecuteHeelMagic = true;
			m_hasExecutedHeelMagic = false;
		}

		bool PlayerHeelMagicState::OnUpdateAttack()
		{
			/* 未チャージ状態なら即終了。*/
			if (!m_canExecuteHeelMagic)
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			/* 詠唱中も重力・地面判定を維持する（NPC浮き防止）。*/
			const float deltaTime = g_gameTime->GetFrameDeltaTime();
			m_player->MoveWithBattleClamp(Vector3::Zero, deltaTime);

			/* エフェクトの拡大。*/
			ComputeHeelEffectScale();

			/* 指定フレーム以降で回復魔法を1回だけ発動。*/
			if (!m_hasExecutedHeelMagic && m_attackTimer >= ATTACK_TIMER_EXECUTE)
			{
				ExecuteAreaHeal();
				m_hasExecutedHeelMagic = true;
			}

			/* 通常終了。*/
			if (m_attackTimer > ATTACK_TIMER_END && !m_player->IsPlayAnimation())
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			/* 保険。*/
			if (m_attackTimer > ATTACK_TIMER_FORCE_END)
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			return true;
		}

		void PlayerHeelMagicState::OnExitAttack()
		{
			/* 2種類のエフェクトの再生を止める。*/
			if (m_heelEffect != nullptr)
			{
				m_heelEffect->Stop();
				m_player->GetEffectList().StopEffect(m_heelEffect);
				m_heelEffect = nullptr;
			}
			if (m_particleEffect != nullptr)
			{
				m_particleEffect->Stop();
				m_player->GetEffectList().StopEffect(m_particleEffect);
				m_particleEffect = nullptr;
			}
		}

		void PlayerHeelMagicState::PlayHeelMagicEffect()
		{
			/* Playerクラスの座標を参照。*/
			m_heelEffectPosition = m_player->GetPosition();
			m_heelEffectPosition.y += EFFECT_Y_OFFSET;
			/* エフェクトを再生。*/
			m_heelEffect = m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic, m_heelEffectPosition, Quaternion::Identity, Vector3::One * 20.0f);
		}

		void PlayerHeelMagicState::PlayHeelMagicParticleEffect()
		{
			/* Playerクラスの座標を参照。*/
			m_particleEffectPosition = m_player->GetPosition();
			m_particleEffectPosition.y += EFFECT_Y_OFFSET;
			/* エフェクトを再生。*/
			m_particleEffect = m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic_Particle, m_particleEffectPosition, Quaternion::Identity, Vector3::One * 20.0f);
		}

		void PlayerHeelMagicState::ExecuteAreaHeal()
		{
			if (m_player == nullptr)
				return;

			/* 回復量。*/
			m_healAmount = HEAL_AMOUNT_BASE * static_cast<int>(m_chargeLevel);

			nsActor::Player *healTarget = nullptr;

			/* 回復ゾーンの中心位置を決める。*/
			m_areaPosition = m_player->GetPosition();

			if (m_player->GetBrain() != nullptr)
			{
				healTarget = m_player->GetBrain()->FindAllyNeedingHeal();
				if (healTarget != nullptr)
				{
					/* 味方回復は対象の足元。*/
					m_areaPosition = healTarget->GetPosition();
				}
			}

			/* 自己回復のときだけ、ボスから少し離す。*/
			if (healTarget == nullptr)
			{
				if (auto *boss = FindGO<nsActor::Boss>("boss"))
				{
					Vector3 away = m_areaPosition - boss->GetPosition();
					away.y = 0.0f;
					if (away.LengthSq() > 0.001f)
					{
						away.Normalize();
						m_areaPosition += away * BOSS_AWAY_OFFSET;
					}
				}
			}

			m_areaPosition.y += HEEL_AREA_Y_OFFSET;

			/* 既存ゾーンがあれば消してから作り直す。*/
			if (auto *existingArea = FindGO<HeelArea>("HeelArea"))
				DeleteGO(existingArea);

			m_healArea = NewGO<HeelArea>(0, "HeelArea");
			if (m_healArea == nullptr)
				return;

			m_healArea->SetPosition(m_areaPosition);
			m_healArea->SetRadius(HEAL_AREA_RADIUS);
			m_healArea->SetHealAmount(m_healAmount);
			m_healArea->SetLifeTime(HEAL_AREA_LIFETIME);
			m_healArea->SetInterval(HEAL_AREA_INTERVAL);
			m_healArea->SpawnArea();
		}
	}
}