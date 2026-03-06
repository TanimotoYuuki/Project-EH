#pragma once
/**
 * @file   StateMachine.h
 * @brief  ステートマシンの本体クラス。
 * @author Yamaguchi Hayato。
 * @date   2026/03/05。
 */

#include "IState.h"

namespace nsApp
{
	namespace nsState
	{
		template<class CharacterTemplete>
		class StateMachine
		{
		public:
			/**
			* @brief コンストラクタ。
			* @param owner ステートが所有するクラス。
			*/
			StateMachine(CharacterTemplete* owner);

		    /**
		     * @brief デストラクタ。
		     */
			virtual ~StateMachine();

		public:
		    /**
		     * @brief ステートを切り替える処理。 
		     */
			void ChangeState(IState<CharacterTemplete>* newState);

	     	/**
		     * @brief ステート中、毎フレーム更新する処理。
		     */
			void Update();


		/* ゲッター。*/
		public:
			/* 現在のステートを取得する。*/
			inline IState<CharacterTemplete>* GetCurrentState() const
			{
				return m_currentState;
			}


		private:
			/* ステートマシーンが持つ実態ポインタ。*/
			CharacterTemplete* m_owner;

			/* 現在実行中のステートポインタ。*/
			IState<CharacterTemplete>* m_currentState = nullptr;

		};

	}
}

