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

			/* ハンマーのエフェクトパスを登録する。*/
			StorageHammerEffect();
		}


		void EffectList::StorageGreatSwordEffect()
		{
			/* チャージ時のエフェクト。*/
			m_effectPathList[Effect_ID::Charge] = GetEffectFilePath(u"chargeAttackEffect");

			/* エフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Charge, m_effectPathList[Effect_ID::Charge].c_str());
		}


		void EffectList::StorageHammerEffect()
		{
			/* チャージ時のHammerに付与するエフェクト。*/
			m_effectPathList[Effect_ID::Fire] = GetEffectFilePath(u"fire");

			/* 空中攻撃時の着地時に付与するエフェクト。*/
			m_effectPathList[Effect_ID::ShockWave] = GetEffectFilePath(u"airAttack");

			/* エフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Fire, m_effectPathList[Effect_ID::Fire].c_str());

			EffectEngine::GetInstance()->ResistEffect(Effect_ID::ShockWave, m_effectPathList[Effect_ID::ShockWave].c_str());

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