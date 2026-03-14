#pragma once
/**
 * @file   IState.h
 * @brief  ステートのインターフェース。 
 * @author Yamaguchi Hayato。
 * @date   2026/03/05。
 */

namespace nsApp
{
	namespace nsState
	{
		/* 循環を防ぐために前方宣言。*/
		template <class CharacterTemplate>
		/* ステートマシーンクラス本体。*/
		class StateMachine;

		/* ステート作成用のテンプレート。*/
		template <class CharacterTemplate>
		class IState
		{
		protected:
			/* 継承先のステートで使う。*/
			CharacterTemplate* m_owner;
			/* ステートマシーンへのポインタ。*/ 
			StateMachine<CharacterTemplate>* m_stateMachine = nullptr;


		public:
			/* デストラクタ。*/
			virtual ~IState() = default;

			/**
			* @brief                  StateMachineの初期化、ステートの登録処理。
			* @param owner            ステートを持つ実態。
			* @param stateMachine     ステ ートマシーンへのポインタ。
			*/
			inline void Register(CharacterTemplate* owner, StateMachine<CharacterTemplate>* stateMachine)
			{
				m_owner = owner;
				m_stateMachine = stateMachine;
			}

			/**
			* @brief ステートに入った際、呼ばれる処理。
			*/
			virtual void Enter() = 0;

			/**
			* @brief ステート中の場合、毎フレーム更新する処理。
			*/
			virtual void Update() = 0;

			/**
			* @brief 別のステートに入る際、切り替わる直前に呼ぶ終了処理。
			*/
			virtual void Exit() = 0;

		};
	}
}
