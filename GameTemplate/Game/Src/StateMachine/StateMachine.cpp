#include "stdafx.h"
#include "StateMachine.h"
#include "NullState.h"
#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/NPC/NPCBrain.h"

/* @def
 * @brief テンプレートのマクロ。
 */
#define CLASS_T template<class CharacterTemplete>

/* @def
 * @brief テンプレート名のマクロ。
 */
#define T_NAME CharacterTemplete

namespace nsApp
{
	namespace nsState
	{
		CLASS_T
	    StateMachine<T_NAME>::StateMachine(T_NAME* owner) : m_owner(owner)
		{
			/* nullステートの作成。*/
			m_currentState = new NullState<T_NAME>();

			/* nullステートの初期化。*/ 
			m_currentState->Register(m_owner, this);
		}


		CLASS_T
		StateMachine<T_NAME>::~StateMachine()
		{
			/* */
			if (m_currentState != nullptr)
			{
				/* ステートを終了する。*/
				m_currentState->Exit();
				/* ステートの削除。*/
				delete m_currentState;
			}
		}


		CLASS_T
	    void StateMachine<T_NAME>::ChangeState(IState<T_NAME>* newState)
		{
			if (newState == nullptr)
			{
				newState = new NullState<T_NAME>();
			}

			if (m_isUpdating)
			{
				if (m_nextState != nullptr)
				{
					delete m_nextState;
					m_nextState = nullptr;
				}

				m_nextState = newState;
				return;
			}

			if(m_currentState != nullptr)
			{
				m_currentState->Exit();
				delete m_currentState;
				m_currentState = nullptr;
			}

			m_currentState = newState;
			m_currentState->Register(m_owner, this);
			m_currentState->Enter();
		}


		CLASS_T
	    void StateMachine<T_NAME>::Update()
		{
			m_isUpdating = true;

			if (m_currentState != nullptr)
			{
				m_currentState->Update();
			}

			m_isUpdating = false;

			if (m_nextState != nullptr)
			{
				IState<T_NAME>* nextState = m_nextState;
				m_nextState = nullptr;
				ChangeState(nextState);
			}
		}

		/* Actorクラスに対してテンプレートの使用可能にする。*/
		template class StateMachine<nsApp::nsActor::Actor>;

		/* NPC用ステートマシーンテンプレートを作成する。*/
		template class StateMachine<NPCBrain>;
	}
}