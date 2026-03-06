#include "stdafx.h"
#include "LoadStageData.h"

namespace
{
	/* ステージ1のファイルパス。*/
	const char*  STAGE1 = "Assets/modelData/stage/seaStage.tkm";
}

namespace nsApp
{
	namespace nsStage
	{
		/* 静的メンバ変数の実態を定義。*/
		LoadStageData LoadStageData::m_instance;


		LoadStageData::LoadStageData()
		{
			/* ステージリストを作成。*/
			CreateStageDataList();
		}

		
		void LoadStageData::CreateStageDataList()
		{
			/* 配列をクリア。*/
			m_stageDataList.clear();

			/* ステージデータを登録する。*/
			{
				/* ステージ1の情報を初期化・リストに登録。*/
				/* 配列, ファイルパス, Playerをセットする座標をセット。*/
				InitStage1();
			}
		}


		void LoadStageData::ChangeStage(StageID requestStageID)
		{
			/* 現在のステージを破棄。*/
			m_currentStage.reset();

			/* リストから該当するステージIDを検索。*/
			for (const auto& stageData : m_stageDataList)
			{
				if (stageData.stageID == requestStageID)
				{
					/* 共有のStageクラスにデータを渡して生成する。*/
					m_currentStage = std::make_unique<Stage>(stageData);

					/* モデルの初期化処理を実行。*/
					m_currentStage->Initialize();

					/* @TODO: 必要ならここでPlayerの座標を更新。*/
					return;
				}
			}
		}


		void LoadStageData::Update()
		{
			/* 現在のステージが有効なら更新を行う。*/
			if (m_currentStage)
				m_currentStage->Update();
		}


		void LoadStageData::Draw(RenderContext& rc)
		{
			if(m_currentStage)
				m_currentStage->Draw(rc);
		}


		void LoadStageData::InitStage1()
		{
			/* ステージ1として配列にセット。*/
			m_currentStageData.stageID = StageID::stage1;

			/* ファイパスをセット。*/
			m_currentStageData.stageBackGroundModel = STAGE1;

			/* スケールをセット。*/
			m_currentStageData.playerInitPosition = Vector3::Zero;

			/* リストに追加する。*/
			m_stageDataList.push_back(m_currentStageData);
		}
	}
}