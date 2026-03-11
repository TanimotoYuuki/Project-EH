#pragma once
/**
 * @file   Actor.h
 * @brief  IGameObjectを継承し、必要な要素を抜き取ったクラス。
 * @author Yamaguchi Hayato
 * @date   2026/03/05
 */

#include "Src/StateMachine/StateMachine.h"

namespace nsApp
{
	namespace nsActor
	{
		class Actor : public IGameObject
		{
		public:
			/* コンストラクタとデストラクタ。*/
			Actor();          
			virtual ~Actor();


		public:
			/* ライフサイクル。*/
			/* 初期化処理。*/
			virtual bool Start() = 0;
			/* 更新処理。*/ 
			virtual void Update();
			/* 描画処理。*/
			virtual void Draw(RenderContext& rc) = 0;


		protected:
			/* 共通のステートマシーン。*/
			nsState::StateMachine<Actor>* m_stateMachine = nullptr;
		};
	}
}
