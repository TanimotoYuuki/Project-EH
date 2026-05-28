#pragma once

#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	class HeelArea : public IGameObject
	{
	public:
		/* コンストラクタとデストラクタ。*/
		HeelArea() = default;
	    virtual ~HeelArea() = default;


	public:
		/* ライフサイクル。*/
		void Update() override;


	public:
		/**
		 * @brief エフェクトの可視化。
		 */
	     void SpawnArea()
		{
			m_timer = 0.0f;
			m_healTimer = 0.0f;
			m_effectTimer = 0.0f;

			PlayHeelEffect();
		}


	public:
		/**
		 * @brief 回復エリアの位置を設定する。
		 * @param position 回復エリアの中心位置。
		 */
		inline void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief 回復エリアの半径を設定する。
		 * @param radius 回復エリアの半径。
		 */
		inline void SetRadius(float radius)
		{
			m_radius = radius;
		}

		/**
		 * @brief 回復エリアの回復量を設定する。
		 * @param healAmount 回復エリアが回復するHPの量。
		 */
		inline void SetHealAmount(int healAmount)
		{
			m_healAmount = healAmount;
		}

		/**
		 * @brief 回復エリアの寿命を設定する。
		 */
		inline void SetLifeTime(float lifeTime)
		{
			m_lifeTime = lifeTime;
		}

		/**
		 * @brief 回復エリアがHPを回復する間隔を設定する。
		 * @param interval 回復エリアがHPを回復する間隔。
		 */
		inline void SetInterval(float interval)
		{
			m_healInterval = interval;
		}


	private:
		/**
		 * @brief 回復エリア内のプレイヤーを回復する。
		 */
		void HeelPlayersInArea();

		/**
		 * @brief 回復エリア内のプレイヤーを回復する。
		 * @param target 回復対象のプレイヤー。
		 * @return 回復可能な場合はtrue、そうでない場合はfalse。
		 */
		bool CanHeal(nsActor::Player* target) const;

		/**
		 * @brief 回復エリアのエフェクトを再生する。
		 */
		void PlayHeelEffect();

		/**
		 * @brief 回復エリアのエフェクトを停止する。
		 */
		void StopHeelEffect();


	private:
		nsK2EngineLow::EffectEmitter* m_healEffect = nullptr;		
		nsK2EngineLow::EffectEmitter* m_particleEffect = nullptr;


	private:
		Vector3 m_position = Vector3::Zero;        //! 回復エリアの中心位置。
		Vector3 m_effectPosition = Vector3::Zero;  //! 回復エリアのエフェクトの位置。

		float m_radius = 0.0f;					   //! 回復エリアの半径。
		float m_lifeTime = 0.0f;				   //! 回復エリアの寿命。
		float m_timer = 0.0f;                      //! 回復エリアの経過時間。
		float m_healInterval = 1.0f;			   //! 回復エリアがHPを回復する間隔。
		float m_healTimer = 0.0f;				   //! 回復エリアの回復間隔の経過時間。
		float m_effectTimer = 0.0f;				   //! エフェクト再生維持用タイマー。
		float m_effectInterval = 0.7f;			   //! エフェクトを再生し直す間隔。	

		int m_healAmount;						   //! 回復エリアが回復するHPの量。
	};
}
