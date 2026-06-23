#pragma once

/**
 * @file   NPCMind.h
 * @brief  NPCの左脳思考を管理するクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/06/22: クラス作成日。
 */

#include "Src/Actor/Character/NPC/Brain/NPCMindContext.h"

namespace nsApp
{
	class NPCCombatMind
	{
	public:
		/**
		 * @brief 攻撃のインターバルをセットする処理。
		 * @param intervalFrame 攻撃インターバルのフレーム数。
		 */
		void SetAttackInterval(int intervalFrame, float aggression);

		/**
		 * @brief 攻撃インターバルを開始する処理。
		 */
		void StartAttackInterval();

		/**
		 * @brief 攻撃インターバルを更新する処理。
		 */
		void UpdateAttackInterval();

		/**
		 * @brief 攻撃できるかどうかを判定する処理。
		 * @return 攻撃できる場合はtrue、そうでない場合はfalse。
		 */
		bool CanAttack() const;

		/**
		 * @brief ボスが攻撃チャンス（被ダメ硬直など）か。
		 * @return ボスが攻撃チャンスの場合はtrue、そうでない場合はfalse。
		 */
		bool IsBossAttackWindow() const;


	private:
		int m_attackIntervalTimer = 0;  //! 攻撃インターバルのタイマー。
		int m_attackIntervalFrame = 60; //! 攻撃インターバルのフレーム数。
	};
}