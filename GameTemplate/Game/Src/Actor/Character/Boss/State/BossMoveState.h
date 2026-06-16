#pragma once
/**
 * @file   BossMoveState.h
 * @brief  �{�X�ړ��X�e�[�g�B�ڋ߁E�� Walk�E��ނ𐧌䂷��B
 */

#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Boss/Boss.h"

namespace nsApp
{
	namespace nsActor { class Boss; }

	namespace nsState
	{
		/**
		 * @class BossMoveState
		 * @brief ボスの移動状態。地上を歩く、空中をダッシュする、ステージ端で方向転換する、などの行動を管理する。
		 */
		class BossMoveState : public IState<nsActor::Actor>
		{
		public:
			/**
			 * @brief BossMoveState の初期化。Boss::Start() から呼ぶ。
			 */
			void Enter()  override;

			/**
			 * @brief BossMoveState の更新。Boss::Update() から呼ぶ。移動行動のタイミング管理や、移動中のモーション・エフェクトの制御などを行う。
			 */
			void Update() override;

			/**
			 * @brief BossMoveState の終了処理。Boss::Update() から呼ぶ。移動状態から攻撃状態などへ遷移する際のクリーンアップ処理を行う。
			 */
			void Exit()   override {};

			/**
			 * @brief BossMoveState の状態遷移条件。Boss::Update() から呼ぶ。攻撃状態への遷移条件などを判定する。
			 * @param id 遷移先状態の ID を返す参照引数。遷移条件を満たす場合は true を返し、遷移先状態の ID を id にセットする。遷移条件を満たさない場合は false を返す。
			 * @return 遷移条件を満たす場合は true、満たさない場合は false。
			 */
			bool RequestID(uint8_t& id) override;

		private:
			nsActor::Boss* m_boss = nullptr;


		private:
			float m_timer = 0.0f;

			bool  m_isDashing = false;  //! Dashフラグ。
			bool  m_isSideStep = false; //! SideStepフラグ。
			bool  m_isRetreat = false;  //! Retreatフラグ。

			float m_sideDir = 1.0f;     //! SideStepの方向補助。左右どちらにステップするかを決めるための値。正なら右、負なら左。
		};
	}
}
