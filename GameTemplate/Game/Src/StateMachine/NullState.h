#pragma once
/**
 * @file   NullState.h
 * @brief  nullObjectパターンクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/03/05。
 */

#include "Src/Actor/Character/IState.h"

namespace nsApp
{
	namespace nsState
	{
		template <class CharacterTemplate>
		class NullState : public IState<CharacterTemplate>
		{
		public:
			/* 初期化処理。*/
			void Enter() override {};
			/* 更新処理。*/ 
			void Update() override {};
			/* 終了処理。*/
			void Exit() override {};
		};
	}
}
