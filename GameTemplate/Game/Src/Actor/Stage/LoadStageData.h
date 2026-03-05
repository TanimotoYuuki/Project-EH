#pragma once
#include "Src/Actor/Stage/Stage.h"
 
namespace nsApp
{
	namespace nsStage
	{
		class LoadStageData
		{ 
		private:
			std::vector<StageData> m_stageDataList;                     /* 全ステージのマスターデータ。*/
			std::unique_ptr<Stage> m_currentStage;                      /* 現在実行中のステージインスタンス。*/
			StageData m_currentStageData;                               /* 現在のステージのマスターデータ。*/
			StageID m_currentStageID = StageID::stageNum;               /* 現在のステージID。*/


		private:
			LoadStageData();
			virtual ~LoadStageData() = default;


		private:
			static LoadStageData m_instance;       /* シングルトンインスタンス。*/


		/* ゲッター。*/
		public:
			/* インスタンスを取得。*/
			inline static LoadStageData& GetInstance()
			{ 
				return m_instance;
			}

			/* 現在のステージを取得。*/
			inline Stage* GetCurrentStage() const
			{
				return m_currentStage.get();
			}

		public:
			 /* コピ―ムーブを禁止する。*/
			LoadStageData(const LoadStageData&) = delete;
			void operator = (const LoadStageData&) = delete;


		public:
			/* ステージリストを読み込む。*/
			void CreateStageDataList();

			/* ステージを切り替える処理。*/
			void ChangeStage(StageID requestStageID);

			/* 更新処理。*/
			void Update();

			/* 描画処理。*/
			void Draw(RenderContext& rc);


		private:
			/* ステージ1を初期化。*/
			void InitStage1();
		};
	}
}

