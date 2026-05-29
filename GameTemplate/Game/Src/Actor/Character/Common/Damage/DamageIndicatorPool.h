#pragma once

/**
 * @file  DamageIndicatorPool.h
 * @brief ダメージフォントのプールを管理するクラス。
 * @author Yamaguchi Hayato
 * @date  2026/05/22
 */

#include "Src/Actor/Character/Common/Pool/GameObjectPool.h"
#include "Src/Actor/Character/Common/Damage/PresentDamageIndicator.h"

namespace nsApp
{
	class DamageIndicatorPool final : public IGameObject
	{
	public:
		/* コンストラクタとデストラクタ。*/
		DamageIndicatorPool() = default;
		virtual ~DamageIndicatorPool() = default;


	public:
		/**
		 * @brief プールを初期化する。
		 * @return 初期化に成功したらtrue。
		 */
		bool Start() override;

		/**
		 * @brief ダメージ表示を生成する。
		 * @param damageValue 表示するダメージ値。
		 * @param position 表示位置。
		 */
		void SpawnDamageText(int damageValue, const Vector3& position);


	private:
		GameObjectPool<PresentDamageIndicator> m_damageTextPool; //! ダメージ表示用プール。
	};
}
