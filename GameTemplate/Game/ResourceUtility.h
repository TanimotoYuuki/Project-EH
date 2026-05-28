#pragma once
/**
 * @file   ResourceUtility.h
 * @brief  リソース管理に関するユーティリティクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/05/26
 */

#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsActor
	{
		class ResourceUtility
		{
		public:
			/**
			 * @brief 指定した範囲内で最も近いダウン中のキャラクターを検索する処理。
			 * @param helper 検索の基準となるキャラクター。
			 * @param searchRange 検索範囲の半径。
			 * @return 最も近い救助対象。見つからなければ nullptr。
			 */
			static Player* SearchNearestDownCharacter(Player* helper, float searchRange);

			
			/**
			 * @brief 指定キャラが救助対象として有効か判定する。
			 *
			 * @param helper 救助する側のキャラクター。
			 * @param target 救助される側のキャラクター。
			 * @return 救助対象なら true。
			 */
			static bool IsValidRescueTarget(Player* helper, Player* target);


			static const char* FindPlayerTag(Player* player);

			static Player* FindPlayerByTag(const char* tag);

		private:
			struct SearchResult
			{
				Player* nearestTarget = nullptr;
				float nearestDistance = 0.0f;
			};
		};
	}
}
