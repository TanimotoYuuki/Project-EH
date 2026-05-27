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
            // 【変更なし】元のコードのまま、普通に新しいステートをセットします。
            // ※ただし、Updateの実行中にここが呼ばれると、一時的に m_currentState が入れ替わります。
            m_currentState->Exit();
            delete m_currentState;

            m_currentState = (newState == nullptr) ? new NullState<T_NAME>() : newState;

            m_currentState->Register(m_owner, this);
            m_currentState->Enter();
        }


        CLASS_T
        void StateMachine<T_NAME>::Update()
        {
            // 1. 現在のステートのポインタ（アドレス）をローカル変数にコピーしておく
            IState<T_NAME>* pActiveState = m_currentState;

            // 2. ステートの Update を実行する
            pActiveState->Update();

            // 3. 【超重要】もし Update の中で ChangeState() が呼び出されていた場合、
            //    m_currentState の中身は「新しいステート」にすり替わっています。
            //    逆に、pActiveState の中身は「deleteされて消えた古いステート」を指しています。
            
            // もし両者が一致していない ＝ Update中にステートが切り替わった、と判断できる
            if (m_currentState != pActiveState)
            {
                // すでに自分自身は破壊されている（ゾンビ状態）ので、
                // これ以上この関数（Update）の中で m_player などを触らないように
                // 即座にリターンして安全に処理を終わらせます。
                return;
            }
        }
		/* Actorクラスに対してテンプレートの使用可能にする。*/
		template class StateMachine<nsApp::nsActor::Actor>;

		/* NPC用ステートマシーンテンプレートを作成する。*/
		template class StateMachine<NPCBrain>;
	}
}