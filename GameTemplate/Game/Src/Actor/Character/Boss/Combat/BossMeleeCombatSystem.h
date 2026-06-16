#pragma once

/**
 * @file   BossMeleeCombatSystem.h
 * @brief  ボスの噛みつき・尻尾近接攻撃とヒット判定を管理するクラス。
 * @date   2026/06/16
 */

#include "Src/Actor/Character/Common/WeaponHitDetection.h"

namespace nsApp
{
	namespace nsActor
	{
		class Boss;

		/**
		 * @class BossMeleeCombatSystem
		 * @brief 近接攻撃の判定配置・ダメージ適用を Boss から委譲する。
		 */
		class BossMeleeCombatSystem
		{
		public:
			/**
			 * @brief ヒット判定の初期化。Boss::Start() から呼ぶ。
			 */
			void Init();

			/**
			 * @brief 噛みつき攻撃のヒット判定を配置する。
			 * @param boss 攻撃主体のボス。
			 */
			void AttackBite(Boss* boss);

			/**
			 * @brief 尻尾攻撃のヒット判定を配置する。
			 * @param boss 攻撃主体のボス。
			 * @return 攻撃判定位置（竜巻生成など Facade 側で使用）。
			 */
			Vector3 AttackTail(Boss* boss);

			/**
			 * @brief 近接ヒット判定とダメージ・吹き飛ばしを処理する。
			 * @param boss 攻撃主体のボス。
			 */
			void CheckHitAndDamagePlayers(Boss* boss);

			/**
			 * @brief 噛みつきヒット判定を取得する。
			 * @return ヒット判定参照。
			 */
			WeaponHitDetection& GetBiteHit() { return m_BiteHit; }

			/**
			 * @brief 尻尾ヒット判定を取得する。
			 * @return ヒット判定参照。
			 */
			WeaponHitDetection& GetTailHit() { return m_TailHit; }


		private:
			WeaponHitDetection m_BiteHit; //! 噛みつき。
			WeaponHitDetection m_TailHit; //! 尻尾。
		};
	}
}