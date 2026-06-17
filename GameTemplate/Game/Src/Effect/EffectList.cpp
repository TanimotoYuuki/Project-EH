#include "stdafx.h"
#include "EffectList.h"

namespace
{
	static constexpr int MAX_EFFECT_COUNT = 64; //! �����ɍĐ��ł���G�t�F�N�g���B
}

namespace nsApp
{
	namespace nsEffect
	{
		EffectList::~EffectList()
		{
			/* �G�t�F�N�g���������B*/
			Clear();
		}

		void EffectList::Init()
		{
			/* �匕�̃G�t�F�N�g�p�X��o�^����B*/
			StorageGreatSwordEffect();

			/* �n���}�[�̃G�t�F�N�g�p�X��o�^����B*/
			StorageHammerEffect();

			/* ��̃G�t�F�N�g�̃p�X��o�^����B*/
			StorageWandEffect();

			/* �o�e�̃G�t�F�N�g�̃p�X��o�^����B*/
			StorageTwinGunEffect();

			/* Boss�G�t�F�N�g�B*/
			StorageBossEffect();
		}

		void EffectList::Update(float deltaTime)
		{
			for (auto iterator = m_playingEffects.begin(); iterator != m_playingEffects.end();)
			{
				/* 0�ȉ��̏ꍇ�͕`������Ȃ��B*/
				if (iterator->currentTime >= iterator->lifeTime)
				{
					++iterator;
					continue;
				}

				/* �^�C�}�[�̉��Z�B*/
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
			for (auto &effectInfo : m_playingEffects)
			{
				DeleteGO(effectInfo.emitter);
				effectInfo.emitter = nullptr;
			}

			m_playingEffects.clear();
		}

		nsK2EngineLow::EffectEmitter *EffectList::PlayEffect(Effect_ID id, const Vector3 &position, const Quaternion &angle, const Vector3 &scale, float lifeTime)
		{
			if (m_effectPathList.find(id) == m_effectPathList.end())
				return nullptr;

			if (m_playingEffects.size() >= MAX_EFFECT_COUNT)
				return nullptr;

			/* �G�t�F�N�g�N���X�̐����B*/
			m_effectEmitter = NewGO<nsK2EngineLow::EffectEmitter>(0, "effect");

			/* �G�t�F�N�g������������B*/
			m_effectEmitter->Init(id);

			/* �G�t�F�N�g�̍��W���Z�b�g����B*/
			m_effectEmitter->SetPosition(position);

			/* �G�t�F�N�g�̊p�x���Z�b�g����B*/
			m_effectEmitter->SetRotation(angle);

			/* �G�t�F�N�g�̑傫�����Z�b�g����B*/
			m_effectEmitter->SetScale(scale);

			/* �G�t�F�N�g���Đ�����B*/
			m_effectEmitter->Play();

			/* �Đ����̃��X�g�ɓo�^�B*/
			m_info.emitter = m_effectEmitter;
			m_info.lifeTime = lifeTime;
			m_info.currentTime = 0.0f;
			m_playingEffects.emplace_back(m_info);

			return m_effectEmitter;
		}

		void EffectList::StopEffect(nsK2EngineLow::EffectEmitter *effect)
		{
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

			/* ���X�g�ɂȂ��ꍇ���O�̂��ߍ폜����B*/
			DeleteGO(effect);
			effect = nullptr;
		}

		void EffectList::StorageGreatSwordEffect()
		{
			/* �`���[�W���̃G�t�F�N�g�B*/
			m_effectPathList[Effect_ID::Charge] = GetEffectFilePath(u"chargeAttackEffect");

			/* �G�t�F�N�g��o�^�B*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Charge, m_effectPathList[Effect_ID::Charge].c_str());
		}

		void EffectList::StorageHammerEffect()
		{
			/* �`���[�W����Hammer�ɕt�^����G�t�F�N�g�B*/
			m_effectPathList[Effect_ID::Fire] = GetEffectFilePath(u"fire");

			/* �󒆍U�����̒��n���ɕt�^����G�t�F�N�g�B*/
			m_effectPathList[Effect_ID::ShockWave] = GetEffectFilePath(u"airAttack");

			/* �G�t�F�N�g��o�^�B*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Fire, m_effectPathList[Effect_ID::Fire].c_str());

			EffectEngine::GetInstance()->ResistEffect(Effect_ID::ShockWave, m_effectPathList[Effect_ID::ShockWave].c_str());
		}

		void EffectList::StorageWandEffect()
		{
			/* �񕜖��@(�񕜕���)��o�^�B*/
			m_effectPathList[Effect_ID::HeelMagic] = GetEffectFilePath(u"heelEffect_Heel");

			/* �񕜖��@(�p�[�e�B�N��)��o�^�B*/
			m_effectPathList[Effect_ID::HeelMagic_Particle] = GetEffectFilePath(u"heelEffect_Particle");

			/* ���@�U����o�^�B*/
			m_effectPathList[Effect_ID::MagicAttack] = GetEffectFilePath(u"magicAttack");

			/* �q�b�g�G�t�F�N�g��o�^�B*/
			m_effectPathList[Effect_ID::Hit] = GetEffectFilePath(u"Hit");

			/* �G�t�F�N�g��o�^�B*/
			/* �񕜃G�t�F�N�g�B*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::HeelMagic, m_effectPathList[Effect_ID::HeelMagic].c_str());

			/* �񕜃G�t�F�N�g�̃p�[�e�B�N�������B*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::HeelMagic_Particle, m_effectPathList[Effect_ID::HeelMagic_Particle].c_str());

			/* ���@�U���G�t�F�N�g�B*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::MagicAttack, m_effectPathList[Effect_ID::MagicAttack].c_str());

			/* �q�b�g�G�t�F�N�g�B*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Hit, m_effectPathList[Effect_ID::Hit].c_str());
		}

		void EffectList::StorageTwinGunEffect()
		{
			/* �V���b�g�̃G�t�F�N�g��o�^�B*/
			m_effectPathList[Effect_ID::Shot] = GetEffectFilePath(u"Shot");

			/* �G�t�F�N�g��o�^�B*/
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::Shot, m_effectPathList[Effect_ID::Shot].c_str());
		}

		void EffectList::StorageBossEffect()
		{
			/*�{�X�t�@�C�A�{�[�����ăG�t�F�N�g�B*/
			m_effectPathList[Effect_ID::FireBall] = GetEffectFilePath(u"fireBall");

			/*�{�X�����e�G�t�F�N�g�i�����j�B*/
			m_effectPathList[Effect_ID::BossFireAttack] = GetEffectFilePath(u"fireAttack");

			EffectEngine::GetInstance()->ResistEffect(Effect_ID::FireBall, m_effectPathList[Effect_ID::FireBall].c_str());
			EffectEngine::GetInstance()->ResistEffect(Effect_ID::BossFireAttack, m_effectPathList[Effect_ID::BossFireAttack].c_str());
		}
	}
}