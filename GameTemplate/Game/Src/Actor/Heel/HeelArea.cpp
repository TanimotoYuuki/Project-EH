#include "stdafx.h"
#include "HeelArea.h"

namespace nsApp
{
	void HeelArea::Update()
	{
		/* 経過時間を取得。*/
		const auto deltaTime = g_gameTime->GetFrameDeltaTime();

		/* タイマーを更新。*/
		m_timer += deltaTime;
		m_healTimer += deltaTime;


		if (m_timer >= m_lifeTime)
		{
			StopHeelEffect();
			DeleteGO(this);
			return;
		}

		if (m_healTimer >= m_healInterval)
		{
			m_healTimer = 0.0f;
			HeelPlayersInArea();
		}
	}


	void HeelArea::HeelPlayersInArea()
	{
		const char* playerNames[] = { "player1","player2","player3","player4" };

		for (const char* name : playerNames)
		{
			auto player = FindGO<nsActor::Player>(name);
			if (!CanHeal(player))
				continue;

			const float distance = (player->GetPosition() - m_position).Length();

			if (distance <= m_radius)
				player->ApplyDamage(-m_healAmount);
		}
	}


	bool HeelArea::CanHeal(nsActor::Player* target) const
	{
		if (target == nullptr)
			return false;

		const auto& hp = target->GetCharacterStatus().hp;

		if (hp.currentHP <= 0)
			return false;

		if (hp.currentHP >= hp.maxHP)
			return false;

		return true;
	}


	void HeelArea::PlayHeelEffect()
	{
		auto owner = FindGO<nsActor::Player>("player1");
		if (owner == nullptr)
			return;

		m_effectPosition = m_position;
		m_effectPosition.y += 0.2f;

		/* 回復エフェクト。*/
		m_healEffect = owner->GetEffectList().PlayEffect(
			nsEffect::HeelMagic,
			m_effectPosition,
			Quaternion::Identity,
			Vector3::One * 20.0f
		);

		/* 回復エフェクトのパーティクル。*/
		m_particleEffect = owner->GetEffectList().PlayEffect(
			nsEffect::HeelMagic_Particle,
			m_effectPosition,
			Quaternion::Identity,
			Vector3::One * 20.0f
		);
	}


	void HeelArea::StopHeelEffect()
	{
		if (m_healEffect != nullptr)
		{
			m_healEffect->Stop();
			m_healEffect = nullptr;
		}

		if (m_particleEffect != nullptr)
		{
			m_particleEffect->Stop();
			m_particleEffect = nullptr;
		}
	}
}