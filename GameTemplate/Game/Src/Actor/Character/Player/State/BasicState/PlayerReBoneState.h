#pragma once
/**
 * @file    PlayerReBoneState.h
 * @brief   プレイヤーの蘇生（リボーン）状態を管理するクラス。
 * @author  Yamaguchi Hayato
 * @date    2026/03/24
 * @details ダウン味方への移動と蘇生進行を担当するステート。
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
			bool RequestID(uint8_t& id) override
			{
				return false; 
			}


		public:
			/**
			 * @brief 蘇生対象を外部から指定する（NPC 救助用）。
			 * @param target 蘇生対象のプレイヤー。
			 */
			inline void SetTargetCharacter(nsActor::Player* target)
			{
				m_targetPlayer = target;
			}


		private:
			/**
			 * @brief 蘇生対象を解決する。
			 */
			void ResolveTargetCharacter();

			/**
			 * @brief 蘇生対象として有効か判定する。
			 * @return 有効な蘇生対象なら true。
			 */
			bool IsValidTargetCharacter() const;

			/**
			 * @brief 蘇生進行を開始する。
			 */
			void BeginHelpProgress();

			/**
			 * @brief 蘇生完了時の処理。
			 */
			void CompleteHelp();

			/**
			 * @brief 待機ステートへ戻る。
			 */
			void ChangeIdleState();

			/**
			 * @brief 蘇生対象へ近づく。
			 * @return 移動したら true。
			 */
			bool MoveTowardTarget();

			/**
			 * @brief 蘇生対象までの水平距離を更新する。
			 */
			void UpdateDistanceToTarget();


		private:
			nsActor::Player* m_player = nullptr;       //! プレイヤー本体。
			nsActor::Player* m_targetPlayer = nullptr; //! 蘇生対象のプレイヤー。


		private:
			Vector3 m_difference = Vector3::Zero;      //! 対象への差分ベクトル。

			float m_distance = 0.0f;				   //! 対象までの水平距離。
			float m_helpRange = 80.0f;				   //! 蘇生開始距離（NPCHelpState と合わせる）。
			float m_searchRange = 120.0f;			   //! 蘇生対象として認識する最大距離。

			bool m_isHelpStarted = false;			   //! 蘇生進行を開始したか。
			bool m_isHelpCompleted = false;			   //! 蘇生を完了したか。
			bool m_shouldReturnIdle = false;		   //! 次 Update で待機へ戻るか。
		};
	}
}