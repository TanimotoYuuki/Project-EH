#pragma once
/**
 * @file   ReboneGaugeUIManager.h
 * @brief  復活ゲージUIの管理クラス。
 */

#include <vector>
#include "Src/UI/ReboneGage/ReboneGaugeUI.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsUI
	{
		class ReboneGaugeUIManager
		{
		public:
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
			/**
			 * @brief 現在救助されているプレイヤーを探す。
			 * @return 救助中のプレイヤー。いなければnullptr。
			 */
			nsActor::Player* FindBeingHelpedPlayer() const;


		private:
			std::vector<nsActor::Player*> m_players; //! 監視対象のプレイヤー一覧。
			ReboneGaugeUI m_gaugeUI;                 //! 復活ゲージUI。
		};
	}
}