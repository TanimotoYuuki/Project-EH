#pragma once

/**
 * @file   EffectListener.h
 * @brief  SEやエフェクトを担当するリスナークラス。
 * @author Yamaguchi Hayato
 * @date   2025/05/07
 */

#include "Src/Actor/Gun/Shooter/IBulletFireListener.h"
#include "Src/Sound/SoundLister.h"


namespace nsApp
{
	namespace nsActor {
		class Player;
	}

	class EffectListener : public IBulletFireListener
	{
	public:
		EffectListener() = default;
		virtual ~EffectListener() = default;


	public:
		/**
		 * @brief SEやエフェクトを再生するための関数。
		 * @param request 発射のリクエスト情報。
		 */
		inline void Initialize(nsActor::Player* target)
		{
			m_target = target;
		}

		/**
		 * @brief 弾丸が発砲された瞬間のコールバック関数。
		 * @param request 発射のリクエスト情報。
		 */
		void OnBulletFired(const BulletFireRequest& request) override;


	private:
		nsActor::Player* m_target;   //! プレイヤーへの参照。
		nsSound::SoundLister* m_soundManager = nullptr;

	private:
		WeaponType m_currentJob = WeaponType::None;   //! 現在の武器の種類。

		Vector3 m_gunMuzzlePosition = Vector3::Zero;  //! 銃口の位置。
		Vector3 m_gunMuzzleDirection = Vector3::Zero; //! 銃口の向き。
	};
}

