#pragma once
#include "StageData.h"
/**
 * @file   Stage.h  
 * @brief  ステージの情報を格納するためのクラス。
 * @author Yamaguchi Hayato
 * @date   2026/03/05
 */


namespace nsApp
{
	namespace nsStage
	{
		class Stage
		{
		protected:
			StageData m_stageData;                          /* ステージのデータ。*/ 
			std::unique_ptr<ModelRender> m_BackGroundModel; /* ステージ背景モデルを管理するリスト。*/ 


		public:
			// コンストラクタとデストラクタ。
			Stage(const StageData& stageData) : m_stageData(stageData) {};
			virtual ~Stage();


		public:
			// 初期化処理。
			virtual void Initialize();
			// 更新処理。
			virtual void Update();
			// 描画処理。
			virtual void Draw(RenderContext& rc);
			
		public:
			// ステージデータを取得。
			inline const StageData GetStageData() const
			{
				return m_stageData;
			}
		};
	}
}
