#include "stdafx.h"
#include "Stage.h"

namespace nsApp
{
	namespace nsStage
	{
		Stage::~Stage()
		{
			/* ステージ背景モデルを解放する。*/
			m_backGroundModel.reset();
		}


		void Stage::Initialize()
		{
			/* ステージ土台モデルのファイルパスが指定されていなければ初期化する。*/ 
			if (!m_stageData.stageBackGroundModel.empty())
			{
				/* ステージの土台モデルを作成する。*/
				m_backGroundModel = std::make_unique<ModelRender>();

				/* ModelRenderのInitを呼び出す。*/ 
				m_backGroundModel->Init(
					m_stageData.stageBackGroundModel.c_str(), /* モデルのファイルパス。*/
					nullptr,                                  /* アニメーションの指定。*/
					0,                                        
					enModelUpAxisZ,                           /* モデルの上方向。*/ 
					true                                      /* 影の設定。*/
				);

				/* ステージ土台モデルを原点に配置する。*/
				m_backGroundModel->SetPosition(Vector3::Zero);

				/* 当たり判定の作成。*/
				m_stagePhysicsStaticObject.CreateFromModel(m_backGroundModel->GetModel(), m_backGroundModel->GetModel().GetWorldMatrix());
			}
		}


		void Stage::Update()
		{
			/* ステージの更新。s*/
			if(m_backGroundModel)
			   m_backGroundModel->Update();
		}


		void Stage::Draw(RenderContext& rc)
		{
			/* ステージモデルを描画する。*/
			if(m_backGroundModel)
				m_backGroundModel->Draw(rc);
		}

	}
}
