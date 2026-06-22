#include "stdafx.h"
#include "PlayerGuardState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"

namespace
{
	constexpr float GUARD_SCALE = 5.0f;									//! ガードエフェクトの基本スケール。
	constexpr float GUARD_OFFSET_Y = 2.0f;								//! ガードエフェクトのYオフセット。
	constexpr float GUARD_EFFECT_LIFETIME = 999.0f;						//! ガードエフェクトの寿命（秒）。

	// GUARD_SCALE_Y は削除（形状を変える原因）

	inline Vector3 GetGuardEffectPosition(const Vector3& feetPos)
	{
		return feetPos + Vector3(0.0f, GUARD_OFFSET_Y, 0.0f);
	}

	/**
	 * @brief ガードエフェクトのスケールを計算する関数。
	 * @return ガードエフェクトのスケール。HeelMagic と同じ均等スケール。
	 */
	inline Vector3 GetGuardEffectScale()
	{
		return Vector3::One * GUARD_SCALE;
	}
}

namespace nsApp
{
	namespace nsState
	{
		void PlayerGuardState::Enter()
		{
			/* キャスト。*/
			m_player = static_cast<nsActor::Player*>(m_owner);
			if (m_player == nullptr)
				return;

			/* ガードシステムを開始する。*/
			m_player->GetGuardSystem().BeginGuard();

			/* ガードアニメーションを再生。*/
			m_player->PlayBasicAnimation(CharacterBasicAnimationList::Guard);

			/*
			 * ガードエフェクトを再生。
			 * 回転・スケールは HeelMagic と同じ（Identity + 均等スケール）。
			 * 向きは Effekseer 側（Root 回転）で合わせる。
			 */
			m_guardEffect = m_player->GetEffectList().PlayEffect(
				ResolveGuardEffectId(),
				GetGuardEffectPosition(m_player->GetPosition()),
				Quaternion::Identity,
				GetGuardEffectScale(),
				GUARD_EFFECT_LIFETIME);

			/* ガードエフェクトの位置を更新。*/
			UpdateGuardEffectTransform();

			auto* soundManager = FindGO<nsSound::SoundLister>("SoundManager");
			if (soundManager != nullptr)
			{
				soundManager->GetSEList().PlaySE(
					nsSound::SE_ID::Guard,
					1.0f,   // 音量
					false,  // ループしない
					2.0f    // 寿命（秒）
				);
			}
		}

		void PlayerGuardState::Update()
		{
			/* キャスト。*/
			if (m_player == nullptr)
				return;

			/* ガード時間を更新する。*/
			auto& guard = m_player->GetGuardSystem();
			guard.Update(g_gameTime->GetFrameDeltaTime());

			/* ガードエフェクトをプレイヤーに追従。*/
			UpdateGuardEffectTransform();

			/* ガード入力が離れた、または時間切れで Idle に戻る。*/
			if (!m_player->GetInputClass().IsGuard() || guard.IsExpired())
				m_stateMachine->ChangeState(new PlayerIdleState());
		}

		void PlayerGuardState::Exit()
		{
			/* ガードシステムを終了する。*/
			if (m_player != nullptr)
				m_player->GetGuardSystem().EndGuard();

			/* ガードエフェクトを停止する。*/
			StopGuardEffect();
			m_player = nullptr;
		}


		nsEffect::Effect_ID PlayerGuardState::ResolveGuardEffectId() const
		{
			/* テスト中は全員 Guard_Blue。*/
			return nsEffect::Effect_ID::Guard_Blue;
		}


		void PlayerGuardState::UpdateGuardEffectTransform()
		{
			/* ガードエフェクトが存在しない、またはプレイヤーが存在しない場合は更新しない。*/
			if (m_guardEffect == nullptr || m_player == nullptr)
				return;

			/* ガードエフェクトの位置をプレイヤーの足元に合わせる。*/
			m_effectPos = GetGuardEffectPosition(m_player->GetPosition());

			/* ガードエフェクトの位置・回転・スケールを更新する。回転は Identity、スケールは GetGuardEffectScale() を使用。*/
			m_guardEffect->SetPosition(m_effectPos);
			m_guardEffect->SetRotation(Quaternion::Identity);
			m_guardEffect->SetScale(GetGuardEffectScale());
		}

		void PlayerGuardState::StopGuardEffect()
		{
			/* ガードエフェクトが存在しない場合は停止しない。*/
			if (m_guardEffect == nullptr)
				return;

			/* ガードエフェクトを停止し、リモコンを破棄する。*/
			m_guardEffect->Stop();
			m_player->GetEffectList().StopEffect(m_guardEffect);
			m_guardEffect = nullptr;
		}
	}
}