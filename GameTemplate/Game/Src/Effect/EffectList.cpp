#include "stdafx.h"
#include "EffectList.h"

namespace
{
	static constexpr int MAX_EFFECT_COUNT = 64; //! 同時に再生できるエフェクト数。
}

namespace nsApp
{
	namespace nsEffect
	{
		EffectList::~EffectList()
		{
			/* エフェクトを初期化。*/
			// Clear();

			m_playingEffects.clear();
		}

		void EffectList::Init()
		{
			/* 大剣のエフェクトパスを登録する。*/
			StorageGreatSwordEffect();

			/* ハンマーのエフェクトパスを登録する。*/
			StorageHammerEffect();

			/* 杖のエフェクトのパスを登録する。*/
			StorageWandEffect();

			/* 双銃のエフェクトのパスを登録する。*/
			StorageTwinGunEffect();

			/* Bossエフェクト。*/
			StorageBossEffect();

			/* ガード用エフェクト。*/
			StorageGuardEffect();
		}

		void EffectList::Update(float deltaTime)
		{
			/* 再生中のエフェクトのリストをループして、寿命が切れたエフェクトを削除する。*/
			for (auto iterator = m_playingEffects.begin(); iterator != m_playingEffects.end();)
			{
				/* 0以下の場合は描画しない。*/
				if (iterator->currentTime >= iterator->lifeTime)
				{
					++iterator;
					continue;
				}

				/* タイマーの加算。*/
				iterator->currentTime += deltaTime;

				if (iterator->currentTime >= iterator->lifeTime)
				{
					if (iterator->emitter != nullptr)
					{
						DeleteGO(iterator->emitter);
						iterator->emitter = nullptr;
					}

					iterator = m_playingEffects.erase(iterator);
				}
				else
					++iterator;
			}
		}

		void EffectList::Clear()
		{
			const bool isManagerAvailable = nsK2EngineLow::GameObjectManager::IsActive();

			for (auto &EffectInfo : m_playingEffects)
			{
				if (EffectInfo.emitter == nullptr)
				{
					continue;
				}

				if (isManagerAvailable)
				{
					DeleteGO(EffectInfo.emitter);
				}

				EffectInfo.emitter = nullptr;
			}
			m_playingEffects.clear();
		}

		nsK2EngineLow::EffectEmitter *EffectList::PlayEffect(Effect_ID id, const Vector3 &position, const Quaternion &angle, const Vector3 &scale, float lifeTime)
		{
			/* 引数のエフェクトIDがエフェクトパスのリストにない場合はnullptrを返す。*/
			if (m_effectPathList.find(id) == m_effectPathList.end())
				return nullptr;

			/* 同時に再生できるエフェクト数を超える場合はnullptrを返す。*/
			if (m_playingEffects.size() >= MAX_EFFECT_COUNT)
				return nullptr;

			/* エフェクトクラスの生成。*/
			m_effectEmitter = NewGO<nsK2EngineLow::EffectEmitter>(0, "effect");

			/* エフェクトを初期化する。*/
			m_effectEmitter->Init(id);

			/* エフェクトの座標をセットする。*/
			m_effectEmitter->SetPosition(position);

			/* エフェクトの角度をセットする。*/
			m_effectEmitter->SetRotation(angle);

			/* エフェクトの大きさをセットする。*/
			m_effectEmitter->SetScale(scale);

			/* エフェクトを再生する。*/
			m_effectEmitter->Play();

			/* 再生中のリストに登録。*/
			m_info.emitter = m_effectEmitter;
			m_info.lifeTime = lifeTime;
			m_info.currentTime = 0.0f;
			m_playingEffects.emplace_back(m_info);

			return m_effectEmitter;
		}

		void EffectList::StopEffect(nsK2EngineLow::EffectEmitter *effect)
		{
			/* 引数のエフェクトがnullptrの場合は何もしない。*/
			if (effect == nullptr)
				return;

			for (auto iterator = m_playingEffects.begin(); iterator != m_playingEffects.end(); ++iterator)
			{
				if (iterator->emitter == effect)
				{
					DeleteGO(iterator->emitter);
					iterator->emitter = nullptr;

					m_playingEffects.erase(iterator);
					effect = nullptr;
					return;
				}
			}

			/* リストにない場合も念のため削除する。*/
			// DeleteGO(effect);
			effect = nullptr;
		}

		void EffectList::StorageGreatSwordEffect()
		{
			/* チャージ攻撃のエフェクト。*/
			m_effectPathList[Effect_ID::Charge] = GetEffectFilePath(u"chargeAttackEffect");

			/* エフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Charge, m_effectPathList[Effect_ID::Charge].c_str());
		}

		void EffectList::StorageHammerEffect()
		{
			/* チャージ攻撃時 Hammer に付与するエフェクト。*/
			m_effectPathList[Effect_ID::Fire] = GetEffectFilePath(u"fire");

			/* 空中攻撃時の着地時に付与するエフェクト。*/
			m_effectPathList[Effect_ID::ShockWave] = GetEffectFilePath(u"airAttack");

			/* エフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Fire, m_effectPathList[Effect_ID::Fire].c_str());

			/* 空中攻撃の着地エフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::ShockWave, m_effectPathList[Effect_ID::ShockWave].c_str());
		}

		void EffectList::StorageWandEffect()
		{
			/* 回復魔法（回復部分）を登録。*/
			m_effectPathList[Effect_ID::HeelMagic] = GetEffectFilePath(u"heelEffect_Heel");

			/* 回復魔法（パーティクル）を登録。*/
			m_effectPathList[Effect_ID::HeelMagic_Particle] = GetEffectFilePath(u"heelEffect_Particle");

			/* 魔法攻撃を登録。*/
			m_effectPathList[Effect_ID::MagicAttack] = GetEffectFilePath(u"magicAttack");

			/* ヒットエフェクトを登録。*/
			m_effectPathList[Effect_ID::Hit] = GetEffectFilePath(u"Hit");

			/* エフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::HeelMagic, m_effectPathList[Effect_ID::HeelMagic].c_str());

			/* 回復魔法のパーティクルエフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::HeelMagic_Particle, m_effectPathList[Effect_ID::HeelMagic_Particle].c_str());

			/* 魔法攻撃エフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::MagicAttack, m_effectPathList[Effect_ID::MagicAttack].c_str());

			/* ヒットエフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Hit, m_effectPathList[Effect_ID::Hit].c_str());
		}

		void EffectList::StorageTwinGunEffect()
		{
			/* ショットのエフェクトを登録。*/
			m_effectPathList[Effect_ID::Shot] = GetEffectFilePath(u"Shot");

			/* エフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Shot, m_effectPathList[Effect_ID::Shot].c_str());
		}

		void EffectList::StorageBossEffect()
		{
			/* ボスファイアボール着弾エフェクト。*/
			m_effectPathList[Effect_ID::FireBall] = GetEffectFilePath(u"fireBall");

			/* ボス火炎弾着弾エフェクト（地面）。*/
			m_effectPathList[Effect_ID::BossFireAttack] = GetEffectFilePath(u"fireAttack");

			/* エフェクトを登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::FireBall, m_effectPathList[Effect_ID::FireBall].c_str());

			/* ボス火炎弾着弾エフェクト（地面）を登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::BossFireAttack, m_effectPathList[Effect_ID::BossFireAttack].c_str());
		}

		void EffectList::StorageGuardEffect()
		{
			/* ガード用バリア（青）。*/
			m_effectPathList[Effect_ID::Guard_Blue] = GetEffectFilePath(u"Guard_Blue");

			/* ガード用バリア（赤）。*/
			m_effectPathList[Effect_ID::Guard_Red] = GetEffectFilePath(u"Guard_Red");

			/* エンジンへ登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Guard_Blue, m_effectPathList[Effect_ID::Guard_Blue].c_str());

			/* ガード用バリア（赤）を登録。*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Guard_Red, m_effectPathList[Effect_ID::Guard_Red].c_str());
		}
	}
}