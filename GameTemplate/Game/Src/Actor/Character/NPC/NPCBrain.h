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
		class Boss;
	}

	class VirtualInputAdapter;
	class NPCBrain
	{
	public:
		/* コンストラクタとデストラクタ。*/
		NPCBrain() = default;
		virtual ~NPCBrain();


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
         */
		nsActor::ICharacter* SearchTarget();

		/**
		 * @brief 攻撃できるかどうかを判定する処理。
		 * @return 攻撃できる場合はtrue、そうでない場合はfalse。
		 */
		inline bool CanAttack() const
		{
			return m_attackIntervalTimer <= 0;
		}

		/**
		 * @brief 攻撃インターバルを更新する処理。
		 */
		void UpdateAttackInterval();

		/**
		 * @brief 
		 * @return 
		 */
		nsActor::Player* SearchHelpTarget() const;


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

		/**
		 * @brief 攻撃のインターバルをセットする処理。
		 * @param intervalFrame 攻撃インターバルのフレーム数。
		 */
		inline void SetAttackInterval(int intervalFrame)
		{
			/* フレーム数が0を超えないように補正。*/
			if (intervalFrame < 0)
				intervalFrame = 0;

			/* インターバルフレームをセット。*/
			m_attackIntervalFrame = intervalFrame;
		}

		/**
		 * @brief 攻撃インターバルを開始する処理。
		 */
		inline void StartAttackInterval()
		{
			m_attackIntervalTimer = m_attackIntervalFrame;
		}


	/* ゲッター。*/
	public:
		/* Playerクラスを取得する。*/
		inline nsActor::Player* GetBody() const
		{
			return m_outer;
		}

		/* 仮想コントローラーを取得する。*/
		inline VirtualInputAdapter* GetVirtualInputAdapter() const
		{
			return m_virtualInputAdapter;
		}

		/**
		 * @brief 助ける対象を取得する処理。
		 */
		inline nsActor::Player* GetHelpTarget() const
		{
			return m_helpTarget;
		}



	private:
		nsActor::Player* m_outer = nullptr;                           //! ポインタを持つ対象。
		nsActor::Player* m_helpTarget = nullptr;                      //! 助ける対象。
		nsActor::Boss* m_bossTarget = nullptr;                        //! 攻撃対象。
		nsState::StateMachine<NPCBrain>* m_npcStateMachine = nullptr; //! NPCの状態を管理するステートマシン。
		VirtualInputAdapter* m_virtualInputAdapter = nullptr;         //! VirtualInputAdapterのポインタ。


	private:
		int m_attackIntervalTimer = 0;						          //! インターバルタイマー。
		int m_attackIntervalFrame = 60;						          //! インターバルフレーム。
		float m_helpSearchRange = 800.0f;							  //! 助ける対象を検索する範囲。
	};
}

