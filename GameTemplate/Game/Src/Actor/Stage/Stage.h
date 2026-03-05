#pragma once
#include "StageData.h"
/**/


namespace nsApp
{
	namespace nsStage
	{
		class Stage
		{
		protected:
			StageData m_stageData;                          /* ステージのデータ。*/ 
			std::unique_ptr<ModelRender> m_BackGroundModel; /* ステージ背景モデルを管理するリスト。*/ 
			PhysicsStaticObject m_stagePhysicsStaticObject; /* ステージの物理静的オブジェクト。*/

		public:
			/* コンストラクタとデストラクタ。*/
			Stage(const StageData& stageData) : m_stageData(stageData) {};
			virtual ~Stage();


		public:
			/* 初期化処理。*/
			virtual void Initialize();
			/* 更新処理。*/
			virtual void Update();
			/* 描画処理。*/
			virtual void Draw(RenderContext& rc);

		public:
			/* ステージデータを取得。*/ 
			inline const StageData GetStageData() const
			{
				return m_stageData;
			}
		};
	}
}
