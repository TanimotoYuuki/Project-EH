#pragma once
/**
 * @file   GunShooter.h
 * @brief  銃の発射処理を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/07
 * @detail 攻撃ステートから発射情報を受け取り、Factoryクラスに弾丸の描画を依頼、Listenerクラスに通知を送る中核クラス。
 */

#include "BulletFireRequest.h"
#include "IBulletFireListener.h"
#include <vector>

namespace nsApp
{
	class GunShooter
	{
	public:
		/**
		 * @brief 情報を送る主を指定する。 
		 * @param listener 発射情報を受け取るリスナー。
		 */
		void Subscribe(IBulletFireListener* listener);

		/**
		 * @brief 銃の発射処理を行う。
		 * @param request 発射に必要な情報をまとめた構造体。
		 */
		void Fire(const BulletFireRequest& request);


	private:
		std::vector<IBulletFireListener*> m_listeners; //! 発射情報を受け取るリスナーのリスト。
	};
}