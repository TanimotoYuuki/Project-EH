#pragma once
/**
 * @file   PlayerJumpState.h
 * @brief  PlayerのJump状態を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/03/15
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	namespace nsState
	{
		/**
		 * @brief プレイヤーのジャンプ状態を処理するステートクラス
		 */
		class PlayerJumpState : public IState<nsActor::Actor>
		{
		public:
			/**
			 * @brief コンストラクタ
			 */
			PlayerJumpState() = default;

			/**
			 * @brief デストラクタ
			 */
			virtual ~PlayerJumpState() = default;

		public:
			/**
			 * @brief ステートに入った時の初期化処理
			 */
			void Enter() override;

			/**
			 * @brief 毎フレームの更新処理
			 */
			void Update() override;

			/**
			 * @brief ステートを抜ける時の終了処理
			 */
			void Exit() override {};

			/**
			 * @brief 次のステートへの遷移条件を判定する
			 * @param[out] id 次のステートID
			 * @retval true 遷移リクエストあり
			 * @retval false 遷移リクエストなし
			 */
			bool RequestID(uint8_t& id) override;


		/* セッター。*/
		public:
			/* ジャンプの初速を設定。*/
			inline void SetJumpVelocity(float velocity) 
			{
				m_jumpVelocity = velocity; 
			}


		private:
			nsActor::Player* m_player = nullptr;    //!プレイヤーの実体へのポインタ。


		private:
			Vector3 m_moveSpeed = Vector3::Zero;    //! ジャンプ中の移動速度。

			float m_jumpVelocity = 0.0f;            //! ジャンプの初速。 
		};
	}
}