#pragma once
/**
* @file   PlayerDethState.h
* @brief  プレイヤーの死亡状態を管理するクラス。
* @author Yamaguchi Hayato。
* @date   2026/03/16
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		class PlayerDethState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerDethState() = default;

			explicit PlayerDethState(const Vector3& desathPos)
				: m_dethPosition(desathPos)
				, m_hasDeathPosition(true) {}

			virtual ~PlayerDethState() = default;


		public:
		    /* ライフサイクル。*/
			void Enter() override;
			void Update() override;
			void Exit() override {};
			bool RequestID(uint8_t& id) override;


		private:
			nsActor::Player* m_player = nullptr; /* プレイヤーの実態へのポインタ。*/


		private:
			Vector3 m_fallSpeed;					   //! 落下速度。
			Vector3 m_dethPosition = Vector3::Zero;    //! 死亡した位置。

			bool m_hasDeathPosition = false;		   //! 死亡位置が設定されたかどうかのフラグ。

		};
	}
}
