#include "stdafx.h"
#include "Actor.h"
#include "Src/StateMachine/StateMachine.h"

namespace nsApp
{
	namespace nsActor
	{
		Actor::Actor()
		{
			/* Actorクラスをオーナーとし、StateMachineを作成。*/
			m_stateMachine = new nsState::StateMachine<Actor>(this);
		}


		Actor::~Actor()
		{
			/* 削除。*/
			delete m_stateMachine;
		}


		void Actor::Update()
		{
			/* ステートマシーンの更新。*/
			m_stateMachine->Update();
		}
	}
}