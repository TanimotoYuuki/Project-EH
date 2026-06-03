#include "stdafx.h"
#include "PlayerHeelMagicState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Effect/EffectList.h"
#include "Src/Actor/Heel/HeelArea.h"
#include "Src/Actor/Magic/Parameter/MagicParameterTable.h"

namespace
{
	const auto ATTACK_TIMER_25 = 25.0f;			  //! 攻撃タイマー25フレーム目の定数。
	const auto ATTACK_TIMER_35 = 25.0f;			  //! 攻撃タイマー35フレーム目の定数。
	const auto ATTACK_TIMER_90 = 25.0f;			  //! 攻撃タイマー90フレーム目の定数。
		
	const auto EFFECT_Y_OFFSET = 2.0f;			  //! エフェクトのY座標オフセット。

	const auto HEAL_AMOUNT_BASE = 100;			  //! 回復量の基本値。
	const auto HEAL_AREA_RADIUS = 45.0f;		  //! 回復エリアの半径。
	const auto HEAL_AREA_LIFETIME = 5.0f;		  //! 回復エリアの寿命。
	const auto HEAL_AREA_INTERVAL = 0.5f;         //! 回復エリアの回復間隔。
	const auto HEEL_AREA_Y_OFFSET = 2.0f;		  //! 回復エリアのY座標オフセット。

	const auto HEEL_AREA_SPAWN_DISTANCE = 120.0f; //! 回復エリアのスポーン距離。
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
				/* 即終了。*/
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}


			/* エフェクトの拡大。*/
			ComputeHeelEffectScale();

			/* 25フレーム目以降で回復魔法を1回だけ発動。*/
			if (!m_hasExecutedHeelMagic && m_attackTimer >= ATTACK_TIMER_25)
			{
				/* エリアヒール。*/
				ExecuteAreaHeal();

				/* エフェクト再生。*/
				m_hasExecutedHeelMagic = true;
			}

			/* 通常終了。*/
			if (m_attackTimer > ATTACK_TIMER_35 && !m_player->IsPlayAnimation())
			{
				m_stateMachine->ChangeState(new PlayerIdleState());
				return true;
			}

			/* 保険。アニメーション終了判定が戻らない場合でも抜ける。*/
			if (m_attackTimer > ATTACK_TIMER_90)
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
			m_heelEffectPosition.y += EFFECT_Y_OFFSET;
			/* エフェクトを再生。*/
			m_heelEffect =  m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic, m_heelEffectPosition, Quaternion::Identity, Vector3::One  *20.0f);
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

			/* プレイヤーの向いている方向の少し前に回復エリアを出す。*/
			m_areaPosition = m_player->GetPosition();

			/* プレイヤーの向いている方向を取得し、Y成分を0にして正規化。*/ 
			m_getForward = m_player->GetForwardVector();
			m_getForward.y = 0.0f;
			m_getForward.Normalize();

			/* プレイヤーの向いている方向の少し前に回復エリアを出す。*/
			m_areaPosition += m_getForward * HEEL_AREA_SPAWN_DISTANCE;
			m_areaPosition.y += HEEL_AREA_Y_OFFSET;

			// 回復エリアクラスを生成。
			m_healArea = NewGO<HeelArea>(0, "HeelArea");
			if (m_healArea == nullptr)
				return;

			/* エフェクトの位置を設定する。*/
			m_healArea->SetPosition(m_areaPosition);

			// 範囲が広すぎるならここを小さくする。
			m_healArea->SetRadius(HEAL_AREA_RADIUS);

			// 回復量を設定。
			m_healArea->SetHealAmount(m_healAmount);

			// 回復エリアが残る時間。
			m_healArea->SetLifeTime(HEAL_AREA_LIFETIME);

			// 0.5秒ごとに回復。
			m_healArea->SetInterval(HEAL_AREA_INTERVAL);

			// エフェクト再生。
			m_healArea->SpawnArea();
		}
	}
}