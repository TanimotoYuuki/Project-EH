#pragma once
/**
 * @file   PlayerReBoneState.h
 * @brief  プレイヤーのリボーン状態クラス
 * @author YamaguchiHayato
 * @date   202/03/24
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerReBoneState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerReBoneState() = default;
			virtual ~PlayerReBoneState() = default;


		public:
			/* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override;
			bool RequestID(uint8_t& id) override { return false; };
			 

		public:
			/**
			 * @brief 助ける対象を設定する。
			 * @param target 助ける対象のプレイヤー
			 */
			inline void SetTargetCharacter(nsActor::Player* target)
			{
				m_targetPlayer = target;
			}


		private:
			/**
			 * @brief 救助対象を解決する。
			 */
			void ResolveTargetCharacter();

			/**
			 * @brief 救助対象として有効か判定する。
			 * @return 有効な救助対象ならtrue。
			 */
			bool IsValidTargetCharacter() const;

			/**
			 * @brief 救助進行を開始する。
			 */
			void BeginHelpProgress();

			/**
			 * @brief 救助完了後の処理。
			 */
			void CompleteHelp();

			/**
			 * @brief 待機状態へ戻る。
			 */
			void ChangeIdleState();


		private:
			nsActor::Player* m_player = nullptr;       //! プレイヤーのポインタ。
			nsActor::Player* m_targetPlayer = nullptr; //! 助ける対象のプレイヤーのポインタ。


		private:
			bool m_isHelpStarted = false;              //! 救助進行を開始したか。
			bool m_isHelpCompleted = false;            //! 救助完了済みか。
			bool m_shouldReturnIdle = false;           //! 次のUpdateで待機状態へ戻すか。
		};
	}
}