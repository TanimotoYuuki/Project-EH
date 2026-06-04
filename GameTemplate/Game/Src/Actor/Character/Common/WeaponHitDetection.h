#pragma once
/**
* @file   WeaponHitDetection.h
* @brief  武器の当たり判定を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/05/29
*/

#include "Src/Actor/Character/Common/ICharacter.h"

namespace nsApp
{
	class WeaponHitDetection
	{
	public:
		/* コンストラクタとデストラクタ。*/
		WeaponHitDetection() = default;
		virtual ~WeaponHitDetection() = default;


	public:
		/** 
		 * @brief当たり判定の初期化処理。
		 * @param radius 武器の当たり判定の半径。
		 **/
		void Init(float radius);

		/**
		 * @brief当たり判定の更新処理。
		 * @param weaponPosition 武器の現在の座標。
		 */
		inline void Update(const Vector3& weaponPosition)
		{
			/* 座標を保持する。*/
			m_position = weaponPosition;
		}

		/**
		 * @brief描画処理。
		 * @param rc レンダリングコンテキスト。
		 */
		void Render(RenderContext& rc);


	public:
		/**
		 * @brief 当たり判定をオン。
		 */
		inline void Enable()
		{
			m_isActive = true;
		}

		/**
		 * @brief 当たり判定をオフ。
		 */
		inline void Disable()
		{
			m_isActive = false;
			m_hitTargets.clear();
		}


	/* セッター。*/
	public:
		/**
		 * @brief 当たり判定の半径を設定。
		 * @param radius 当たり判定の半径。
		 */
		inline void SetRadius(float radius)
		{
			m_radius = radius;
		}


	/* ゲッター。*/
	public:
        /**
		 * @brief 指定したターゲットが当たり判定にヒットしているかどうかを判定。
		 * @param target 判定対象のキャラクター。
		 * @return ターゲットが当たり判定にヒットしている場合はtrue、そうでない場合はfalse。
         */
        bool IsHit(nsActor::ICharacter* target);

		/**
		 * @brief 当たり判定の中心座標を取得。
		 * @return 当たり判定の中心座標。
		 */
		inline const Vector3& GetPosition()
		{
			return m_position;
		}


	private:
		std::vector<nsActor::ICharacter*> m_hitTargets; //! 当たった対象のリスト。


	private:
		Vector3 m_position = Vector3::Zero;             //! 当たり判定の中心座標。
		Vector3 m_targetPosition = Vector3::Zero;       //! ターゲットの座標。
		Vector3 m_diffVector = Vector3::Zero;           //! 武器とターゲットの距離ベクトル。

		float m_radius = 0.0f;                          //! 当たり判定の半径。
		float m_diff = 0.0f;                            //! 武器とターゲットの距離。

		bool m_isActive = false;                        //! 当たり判定が有効かどうか。
	};
}

