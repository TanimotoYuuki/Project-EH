#pragma once
/**
* @file   WeaponHitDetection.h
* @brief  武器の当たり判定を管理するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/20
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
		* 当たり判定の初期化処理。
		* @param radius 武器の当たり判定の半径。
		**/
		void Init(float radius);

		/**
		* 当たり判定の更新処理。
		* @param weaponPosition 武器の現在の座標。
		*/
		void Update(const Vector3& weaponPosition);

		/**
		* 描画処理。
		*/
		void Render(RenderContext& rc);


	public:
		/* 当たり判定をオン。*/
		void Enable()
		{
			m_isActive = true;
		}

		/* 当たり判定をオフ。*/
		void Disable()
		{
			m_isActive = false;
			m_hitTargets.clear();
		}


	/* セッター。*/
	public:
		/* 当たり判定を設定。*/
		inline void SetRadius(float radius)
		{
			m_radius = radius;
		}


	/* ゲッター。*/
	public:
		/* 当たっているかを検知。*/
        bool IsHit(nsActor::ICharacter* target);

		/* 当たり判定の座標を取得。*/
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

