#pragma once
/**
* @file   PlayerAttackBaseState.h
* @brief  プレイヤーの攻撃状態の基底クラス。
* @author Yamaguchi Hayato
* @date   2026/03/11
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/Component/ComboRouteTable.h"

/** @def
 * プレイヤーの遷移状態を PLAYER_STATE_IDという名前で定義するマクロ。
 */
#define PLAYER_STATE_ID nsActor::PlayerStateID


namespace nsApp
{
	namespace nsState
	{
		class PlayerAttackBaseState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerAttackBaseState() = default;
			virtual ~PlayerAttackBaseState() = default;


		public:
			/* ライフサイクル。*/
			/* 初期化処理。*/
			void Enter() override;
			/* 毎フレーム更新する処理。*/
			void Update() override;
			/* 描画処理。*/
			void Exit() override;


		/* セッター。*/
		public:
			/* 攻撃の時間をセット。*/
			inline void SetAttackTimer(int timer)
			{
				m_attackTimer = timer;
			}


		protected:
			/**
			 *  
			 */
			bool CheckCombo(PLAYER_STATE_ID currentStateID, uint8_t& id);


		protected:
			nsActor::Player* m_player = nullptr;                      //! プレイヤーのポインタ。


		protected:
			int m_attackTimer = 0;                                    //! 攻撃の時間管理用タイマー。
			int m_rushCount = 0;                                      //! 連続攻撃の回数管理用カウンター。

			bool m_isInputMatch;									  //! 入力がコンボルートの条件に合致しているか。
			bool m_isGrounded;										  //! 地上にいるかどうか。

			std::unordered_map<ComboInputType, bool> m_inputRequests; //! 入力タイプとフラグを紐づけ。
		};
	}
}
