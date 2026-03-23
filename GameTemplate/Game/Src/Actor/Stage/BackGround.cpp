#include "stdafx.h"
#include "BackGround.h"

namespace nsApp
{
	namespace nsStage
	{
		BackGround::~BackGround()
		{
			if (m_skyCube != nullptr)
			{
				DeleteGO(m_skyCube);
			}
		}


		bool BackGround::Start()
		{
			/* 背景の空を生成 */
			m_skyCube = NewGO<nsK2Engine::SkyCube>(0, "skyCube");

			/* カメラを覆えるように巨大化 */
			m_skyCube->SetScale(2000.0f);

			/* 空のタイプを設定（トゥーン調の昼間） */
			m_skyCube->SetType(nsK2Engine::enSkyCubeType_DayToon);

			/* 環境光として空のテクスチャを登録 */
			g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 1.0f);

			return true;
		}


		void BackGround::Update()
		{
			/* 常にカメラの中心に空を追従させる */
			if (m_skyCube != nullptr)
			{
				m_skyCube->SetPosition(g_camera3D->GetPosition());
			}
		}
	}
}