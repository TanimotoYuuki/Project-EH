#pragma once

namespace nsApp
{
	namespace nsActor
	{
		class Player;
	}

	namespace nsNPC
	{
		/**
		 * @struct NPCSocialContext
		 * @brief Pathfinder が Social スコアを計算するときの入力。
		 */
		struct NPCSocialContext
		{
			nsActor::Player* self = nullptr; //! 自分自身（ペナルティ計算から除外）。
		};
	}
}