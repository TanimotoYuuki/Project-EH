#include "stdafx.h"
#include "EffectList.h"

namespace nsApp
{
	namespace nsEffect
	{
		void EffectList::Init()
		{
			/* 大剣のエフェクトパスを登録する。*/
			StorageGreatSwordEffect();
		}


		void EffectList::StorageGreatSwordEffect()
		{
			/* チャージ時のエフェクト。*/
			m_effectPathList[Effect_ID::Charge] = GetEffectFilePath(u"chargeAttackEffect");

			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Charge, m_effectPathList[Effect_ID::Charge].c_str());
		}


		nsK2EngineLow::EffectEmitter* EffectList::PlayEffect(Effect_ID id, const Vector3& position, const Quaternion& angle, const Vector3& scale)
		{
			if (m_effectPathList.find(id) == m_effectPathList.end())
				return nullptr;


			/* エフェクトクラスを生成する。*/
			m_effectEmitter = NewGO<EffectEmitter>(0, "effect");
			/* エフェクトを読み込む。*/
			m_effectEmitter->Init(id);
			/* エフェクトの座標をセットする。*/
			m_effectEmitter->SetPosition(position);
			/* エフェクトの角度をセットする。*/
			m_effectEmitter->SetRotation(angle);
			/* エフェクトの大きさをセットする。*/
			m_effectEmitter->SetScale(scale);

			/* エフェクトを再生する。*/
			m_effectEmitter->Play();

			return m_effectEmitter;
		}
	}
}