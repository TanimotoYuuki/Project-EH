#pragma once
/**
 * @file   GuardGaugeUIManager.h
 * @brief  ガードゲージUIの管理クラス。
 */

#include <vector>
#include "Src/UI/GuardGage/GuardGaugeUI.h"

namespace nsApp
{
	namespace nsActor { class Player; }

	namespace nsUI
	{
		class GuardGaugeUIManager
		{
		public:
			static const int MAX_PLAYER_COUNT = 4;

			/**
			 * @brief 初期化。
			 */
			void Init();

			/**
			 * @brief 更新。
			 */
			void Update();

			/**
			 * @brief 描画。
			 * @param rc 描画コンテキスト。
			 */
			void Render(RenderContext& rc);

			/**
			 * @brief 監視するプレイヤーを登録する。
			 * @param player 登録するプレイヤー。
			 */
			void RegisterPlayer(nsActor::Player* player);

			/**
			 * @brief 登録を全て解除する。
			 */
			void ClearPlayers();

		private:
			GuardGaugeUI m_gaugeUI[MAX_PLAYER_COUNT];          //! 各プレイヤーのガードゲージUI。
			nsActor::Player* m_players[MAX_PLAYER_COUNT] = {}; //! 登録済みプレイヤー。
			int m_playerCount = 0;                             //! 登録数。
		};
	}
}