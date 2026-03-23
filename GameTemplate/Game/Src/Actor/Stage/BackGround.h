#pragma once
/**
* @file   BackGround.h
* @brief  ステージ背景を管理するクラス。
* @author Yamaguchi Hayato。
* @date   2026/03/21
*/

namespace nsApp
{
	namespace nsStage
	{
		class BackGround : public IGameObject
		{
		public:
			BackGround() = default;
			virtual ~BackGround();

		public:
			bool Start() ;


		private:
			nsK2Engine::SkyCube* m_skyCube = nullptr; //! スカイキューブ。
		};
	}
}