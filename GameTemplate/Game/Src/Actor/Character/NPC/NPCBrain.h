#pragma once

/**
 * @file   NPCBrain.h
 * @brief  NPCの思考部分を管理するクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/04/27
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Common/ICharacter.h"


namespace nsApp
{
	namespace nsActor {
		class Player;
	}

	class VirtualInputAdapter;
	class NPCBrain
	{
	public:
		/* コンストラクタとデストラクタ。*/
		NPCBrain() = default;
		virtual ~NPCBrain() = default;


	public:
		/**
         * @brief 更新処理。
         */
		void Update();


		/**
		 * @brief 思考を切り替える処理。
		 * @param 
		 */
		inline void ChangeState(nsState::IState<NPCBrain>* nextState)
		{
			if(m_npcStateMachine)
				m_npcStateMachine->ChangeState(nextState);
		}


		/**
         * @brief ターゲットを探索する処理。
         * @TODO: 現在のターゲットはテスト。
         */
		nsActor::ICharacter* SearchTarget();


	/* セッター。*/
	public:
		/* 
 	     * @brief 初期化処理。
		 * param outer: ポインタを持つ対象。
		 */
		void Init(nsActor::Player* outer);

		/**
		 * @brief VirtualInputAdapterをセットする処理。
		 * @param virtualInput VirtualInputAdapterのポインタ。
		 */
		inline void SetVirtualInputAdapter(VirtualInputAdapter* virtualInput)
		{
			m_virtualInputAdapter = virtualInput;
		}


	/* ゲッター。*/
	public:
		/* Playerクラスを取得する。*/
		inline nsActor::Player* GetBody() const
		{
			return m_outer;
		}

		/* 助ける対象を取得。*/
		inline nsActor::Player* GetHelpTarget() const
		{
			return m_helpTarget;
		}

		/* 仮想コントローラーを取得する。*/
		inline VirtualInputAdapter* GetVirtualInputAdapter() const
		{
			return m_virtualInputAdapter;
		}


	private:
		nsActor::Player* m_outer = nullptr;                           //! ポインタを持つ対象。
		nsActor::Player* m_helpTarget = nullptr;                      //! 助ける対象。
		nsState::StateMachine<NPCBrain>* m_npcStateMachine = nullptr; //! NPCの状態を管理するステートマシン。
		VirtualInputAdapter* m_virtualInputAdapter = nullptr;         //! VirtualInputAdapterのポインタ。


	private:
		int m_attackIntervalTimer = 0;						          //! 攻撃のインターバルタイマー。
	};
}

