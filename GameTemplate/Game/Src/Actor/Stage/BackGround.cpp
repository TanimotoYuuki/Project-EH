#include "stdafx.h"
#include "BackGround.h"

namespace nsApp
{
	namespace nsStage
	{
		BackGround::~BackGround()
		{
			if (m_skyCube != nullptr)
				DeleteGO(m_skyCube);
		}


		bool BackGround::Start()
		{
			m_skyCube = NewGO<SkyCube>(0, "skycube");

			m_skyCube->SetScale(Vector3::One * 100.0f);

			// IBLテクスチャを設定。
			g_renderingEngine->SetAmbientByIBLTexture(m_skyCube->GetTextureFilePath(), 1.0f);

			// SkyCubeのタイプを設定。
			m_skyCube->SetType(EnSkyCubeType::enSkyCubeType_Day);
			g_renderingEngine->SetDirectionLight(0, g_vec3Zero, g_vec3Zero);
			return true;
		}
	}
}