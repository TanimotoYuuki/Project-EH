#include "stdafx.h"
#include "PlayerHeelMagicState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Effect/EffectList.h"
#include "Src/Actor/Heel/HeelArea.h"

namespace
{
	
			
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerHeelMagicState::PlayAttackAnimation()
		{
			/* 攻撃の種類をセット。*/
			m_currentAttackType = AttackType::HeelMagic;

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


			/* エフェクトの拡大。*/
			ComputeHeelEffectScale();

			/* 25フレーム目以降で回復魔法を1回だけ発動。*/
			if (!m_hasExecutedHeelMagic && m_attackTimer >= 25)
			{
				/* エリアヒール。*/
				ExecuteAreaHeal();

				m_hasExecutedHeelMagic = true;
			}

			/* 通常終了。*/
			if (m_attackTimer > 35 && !m_player->IsPlayAnimation())
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			/* 保険。アニメーション終了判定が戻らない場合でも抜ける。*/
			if (m_attackTimer > 90)
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			/* この魔法ステートでは親の共通終了判定に行かせない。*/
			return true;
		}


		void PlayerHeelMagicState::OnExitAttack()
		{
			/* 2種類のエフェクトの再生を止める。*/
			if (m_heelEffect != nullptr) {
				m_heelEffect->Stop();
				m_heelEffect = nullptr;
			}
			if (m_particleEffect != nullptr) {
				m_particleEffect->Stop();
				m_particleEffect = nullptr;
			}
		}


		void PlayerHeelMagicState::PlayHeelMagicEffect()
		{
			/* Playerクラスの座標を参照。*/
			m_heelEffectPosition = m_player->GetPosition();
			m_heelEffectPosition.y += 2.0f;
			/* エフェクトを再生。*/
			m_heelEffect =  m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic, m_heelEffectPosition, Quaternion::Identity, Vector3::One  *20.0f);
		}


		void PlayerHeelMagicState::PlayHeelMagicParticleEffect()
		{
			/* Playerクラスの座標を参照。*/
			m_particleEffectPosition = m_player->GetPosition();
			m_particleEffectPosition.y += 2.0f;
			/* エフェクトを再生。*/
			m_particleEffect = m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic_Particle, m_particleEffectPosition, Quaternion::Identity, Vector3::One * 20.0f);		
		}


		void PlayerHeelMagicState::ExecuteAreaHeal()
		{
			if (m_player == nullptr)
				return;

			// 回復量。
			m_healAmount = 100 * static_cast<int>(m_chargeLevel);

			// プレイヤーの向いている方向の少し前に回復エリアを出す。
			m_areaPosition = m_player->GetPosition();

			m_getForward = m_player->GetForwardVector();
			m_getForward.y = 0.0f;
			m_getForward.Normalize();

			const float spawnDistance = 120.0f;
			m_areaPosition += m_getForward * spawnDistance;

			// エフェクトを地面付近に出したいので、Yは少しだけ上げる。
			m_areaPosition.y += 2.0f;

			// 回復エリアクラスを生成。
			m_healArea = NewGO<HeelArea>(0, "HeelArea");
			if (m_healArea == nullptr)
				return;

			m_healArea->SetPosition(m_areaPosition);

			// 範囲が広すぎるならここを小さくする。
			m_healArea->SetRadius(45.0f);

			// 回復量を設定。
			m_healArea->SetHealAmount(m_healAmount);

			// 回復エリアが残る時間。
			m_healArea->SetLifeTime(5.0f);

			// 0.5秒ごとに回復。
			m_healArea->SetInterval(0.5f);

			// エフェクト再生。
			m_healArea->SpawnArea();
		}
	}
}