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
			void Enter() override;
			void Update() override;
			void Exit() override {};
			bool RequestID(uint8_t& id) override { return false; };


		public:
			/* 助ける対象を設定する。*/
			inline void SetTargetCharacter(nsActor::Player* target)
			{
				m_targetPlayer = target;
			}

		private:
			nsActor::Player* m_player = nullptr;       //! プレイヤーのポインタ。
			nsActor::Player* m_targetPlayer = nullptr; //! 助ける対象のプレイヤーのポインタ。

		};
	}
}