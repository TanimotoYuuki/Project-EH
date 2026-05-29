#pragma once

/**
 * @file   NPCCombatHelper.h
 * @brief  NPCAttackBaseStateの処理を委譲するクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/05/26
 */

#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	class NPCCombatHelper
	{
	public:
        /**
		 * @brief ターゲットとの距離を計算する処理。
		 * @param myPos 自分の位置。
		 * @param targetPos ターゲットの位置。
		 * @param outDiff 距離の差分を格納するための出力パラメータ。
		 *				  Y軸の差分は無視される。
         * @return 
         */
        static float ComputeDistance(const Vector3& myPos, const Vector3& targetPos, Vector3& outDiff);


        /**
		 * @brief ターゲットとの位置の差分を元にした押し返す方向を計算する処理。
		 * @param body 自分のキャラクター。
		 * @param target ターゲットのキャラクター。
		 * @param currentDistance 現在の距離。
		 * @param limitDistance クリッピングを防止するための距離の閾値。
		 *		  現在の距離がこの値より小さい場合に押し返す処理が行われる。
         */
		static void PreventClipping(nsActor::Player* body, nsActor::ICharacter *target, float currentDistance, float limitDistance);

        /**
		 * @brief ターゲットとの位置の差分を元にした向きを更新する処理。
		 * @param body 自分のキャラクター。
		 * @param diff ターゲットとの位置の差分。
		 *        Y軸の差分は無視されていることが前提。
		 *
		 * @param isAttacking 攻撃中かどうかを示すフラグ。
		 *        攻撃中の場合は、向きの更新がより迅速に行われる。
         */
        static void UpdateFacing(nsActor::Player* body, const Vector3& diff, bool isAttacking);
    };
}
