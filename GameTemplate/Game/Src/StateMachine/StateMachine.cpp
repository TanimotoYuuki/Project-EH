#include "stdafx.h"
#include "StateMachine.h"
#include "NullState.h"
#include "Src/Actor/Actor.h"

/* テンプレートのマクロ。*/
#define CLASS_T template<class CharacterTemplete>
/* テンプレート名のマクロ。*/
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
			/* 現在のステートを終了する。*/
			m_currentState->Exit();
			/* 現在のステートを削除する。*/
			delete m_currentState;

			/* nullが渡されているならNullStateで保護する。*/
			m_currentState = (newState == nullptr) ? new NullState<T_NAME>() : newState;

			/* ステートの登録。*/
			m_currentState->Register(m_owner, this);
			/* ステートの初期化処理を予備だす。*/
			m_currentState->Enter();
		}


		CLASS_T
	    void StateMachine<T_NAME>::Update()
		{
			m_currentState->Update();
		}


		/* Actorクラスに対してテンプレートの使用可能にする。*/
		template class StateMachine<nsApp::nsActor::Actor>;
	}
}