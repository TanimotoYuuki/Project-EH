#include "stdafx.h"
#include "PlayerHeelMagicState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Effect/EffectList.h"

namespace nsApp
{
	namespace nsState
	{
		void PlayerHeelMagicState::Enter()
		{
			/* キャストしてプレイヤーを取得 */
			m_player = static_cast<nsActor::Player*>(m_owner);

			/* 攻撃の種類をセットする。*/
			m_currentAttackType = AttackType::HeelMagic;

			/* 回復もチャージ解放のアニメーションを流用 */
			m_player->PlayWeaponAnimation(AttackType::HeelMagic);

			/* タイマーの初期化。*/
			SetAttackTimer(0);

			/* チャージレベルを取得。*/
			m_chargeLevel = static_cast<float>(m_player->GetEffectScale());
			if(m_chargeLevel <= 0.0f)
				m_chargeLevel = 1.0f;
		}

		void PlayerHeelMagicState::Update()
		{
				m_attackTimer++;

				ComputeHeelEffectScale();

				/* ★追加：杖を構えている間（例えば5〜20フレームの間）、毎フレーム小さなパーティクルを出し続ける！ */
				if (m_attackTimer >= 5 && m_attackTimer <= 20)
				{
					Vector3 pos = m_player->GetPosition();
					pos.y += 5.0f; // 足元から少し上
					// ランダムに散らすための計算（円状に広がるように）
					float randX = (rand() % 40) - 20.0f;
					float randZ = (rand() % 40) - 20.0f;
					pos.x += randX;
					pos.z += randZ;

					// 小さな光の粒を上に昇らせる（※エフェクトIDは調整してください）
					m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic_Particle, pos, Quaternion::Identity, Vector3::One * 2.0f);
				}


				/* 大爆発（メインの回復エフェクト）とエリアヒールの発動 */
				if (m_attackTimer == 25) {
					PlayHeelMagicEffect();
					PlayHeelMagicParticleEffect();
					ExecuteAreaHeal();
				}

				if (m_attackTimer > 25 && !m_player->IsPlayAnimation())
					m_stateMachine->ChangeState(new PlayerIdleState());
		}


		void PlayerHeelMagicState::Exit()
		{
			/* */
			if (m_heelEffect != nullptr) {
				m_heelEffect->Stop();
				m_heelEffect = nullptr;
			}

			/* */
			if (m_particleEffect != nullptr) {
				m_particleEffect->Stop();
				m_particleEffect = nullptr;
			}

			/* */
			PlayerAttackBaseState::Exit();
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
			m_heelEffect = m_player->GetEffectList().PlayEffect(nsEffect::HeelMagic_Particle, m_heelEffectPosition, Quaternion::Identity, Vector3::One * 20.0f);
		}


		void PlayerHeelMagicState::ExecuteAreaHeal()
		{
			if (!m_player)
				return;

			/* 回復量。*/
			m_healAmount = 500 * static_cast<int>(m_chargeLevel);

			/* 周囲の味方を探して回復*/
			const char* playerNames[] = { "player1", "player2", "player3", "player4" };
			for (const char* name : playerNames)
			{
				auto otherPlayer = FindGO<nsActor::Player>(name);
				if (otherPlayer && otherPlayer != m_player && otherPlayer->GetCharacterStatus().hp.currentHP > 0)
				{
					m_distance = (otherPlayer->GetPosition() - m_player->GetPosition()).Length();
					if (m_distance <= 150.0f * m_chargeLevel) {
						otherPlayer->ApplyDamage(-m_healAmount);
					}
				}
			}

			/* 自分も回復。*/
			m_player->ApplyDamage(-m_healAmount);
		}
	}
}