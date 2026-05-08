#include "stdafx.h"
#include "SEList.h"
#include "Src/Sound/SoundLister.h"

namespace
{
	const auto SE_VOLUME = 1.0f;			  //! SEの音量。
	static constexpr int MAX_SE_COUNT = 64;   //! 同時に再生できるSEの数。
}

namespace nsApp
{
	namespace nsSound
	{
		SEList::~SEList()
		{
			/* SEを解放する。*/
			Clear();
		}


		void SEList::Init()
		{
			/* ファイルパスを登録。*/
			/* ソード。*/
			StorageGreatSwordSE();

			/* ハンマー。*/
			StorageHammerSE();

			/* その他。*/
			StorageOtherSE();

			/* 攻撃手段/武器の種類ごとのSEを登録。*/
			/* ソード。*/
			RegisterSwordSEBank();

			/* ハンマー。*/
			RegisterHammerSEBank();
		}


		void SEList::StorageGreatSwordSE()
		{
			/* ソードのSEファイルパスを登録。*/
			/* 通常攻撃。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::NormalAttack_Sword, GetSEFilePath("GreatSword_NormalAttack").c_str());

			/* 連続攻撃。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::RushAttack_Sword, GetSEFilePath("GreatSword_RushAttack").c_str());
		}


		void SEList::StorageHammerSE()
		{
			/* ハンマーのSEファイルパスを登録。*/
			/* 通常攻撃。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::NormalAttack_Hammer, GetSEFilePath("Hammer_NormalAttack").c_str());

			/* 重攻撃。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::HeavyAttack_Hammer, GetSEFilePath("Hammer_HeavyAttack").c_str());

			/* 空中攻撃。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::AirAttack_Hammer, GetSEFilePath("Hammer_AirAttack").c_str());

			/* 貯め攻撃。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::ChargeAttack_Hammer, GetSEFilePath("Hammer_ChargeAttack").c_str());

			/* ダッシュ攻撃。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::DashAttack_Hammer, GetSEFilePath("Hammer_DashAttack").c_str());
		}


		void SEList::StorageOtherSE()
		{
			/* チャージ中の効果音。*/
			g_soundEngine->ResistWaveFileBank(SE_ID::Charge, GetSEFilePath("Charge").c_str());
		}


		void SEList::Update(float deltaTime)
		{
			for (auto it = m_playingSEs.begin(); it != m_playingSEs.end();)
			{
				/* ループSEは自動削除しない。*/
				if (it->isLoop)
				{
					++it;
					continue;
				}

				it->currentTime += deltaTime;

		
				if (it->currentTime >= it->lifeTime)
				{
					if (it->source != nullptr)
					{
						DeleteGO(it->source);
						it->source = nullptr;
					}

					it = m_playingSEs.erase(it);
				}

				else
				{
					++it;
				}
			}
		}


		void SEList::Clear()
		{
			for (auto& se : m_playingSEs)
			{
				if (se.source != nullptr)
				{
					DeleteGO(se.source);
					se.source = nullptr;
				}
			}

			m_playingSEs.clear();
		}


		nsK2EngineLow::SoundSource* SEList::PlaySE(SE_ID id, float volume, bool flag, float lifeTime)
		{
			if (m_playingSEs.size() >= MAX_SE_COUNT)
				return nullptr;

			/* 音源クラスの生成。*/
			auto* seSource = NewGO<nsK2EngineLow::SoundSource>(0, "SE");

			/* データを代入。*/
			seSource->Init(id);

			/* 音量をセット。*/
			seSource->SetVolume(volume);

			/* 再生。*/
			seSource->Play(flag);

			/* 再生中SEとして登録。*/
			SEInfo info;
			info.source = seSource;
			info.lifeTime = lifeTime;
			info.currentTime = 0.0f;
			info.isLoop = flag;

			m_playingSEs.emplace_back(info);

			/* ループSEは呼び出し側で止める必要があるので返す。*/
			if (flag)
				return seSource;


			/* 単発SEはSEList側で自動削除する。*/
			return nullptr;
		}


		nsK2EngineLow::SoundSource* SEList::PlayAttackSE(WeaponType wepon, AttackType attack)
		{
			/* 武器がテーブルに登録されているか確認。*/
			if (m_attackSEmap.count(wepon) > 0)
			{
				/* テーブルから武器の情報を取得。*/
				auto& attackTable = m_attackSEmap[wepon];

				/* 対応するSEが登録されている場合、SEを再生する。*/
				if (attackTable.count(attack) > 0)
				{
					m_isLoop = (attack == AttackType::Charging);

					return PlaySE(attackTable[attack], SE_VOLUME, m_isLoop, 2.0f);
				}
			}

			return nullptr;
		}


		void SEList::StopSE(nsK2EngineLow::SoundSource*& soundSource)
		{
			if (soundSource == nullptr)
			{
				return;
			}

			for (auto it = m_playingSEs.begin(); it != m_playingSEs.end(); ++it)
			{
				if (it->source == soundSource)
				{
					it->source->Stop();
					DeleteGO(it->source);
					it->source = nullptr;

					m_playingSEs.erase(it);
					soundSource = nullptr;
					return;
				}
			}

			/* リストに無い場合でも、念のため削除する。*/
			soundSource->Stop();
			DeleteGO(soundSource);
			soundSource = nullptr;
		}

		void SEList::RegisterSwordSEBank()
		{
			m_attackSEmap[WeaponType::GreatSword] =
			{
				{ AttackType::NormalAttack, SE_ID::NormalAttack_Sword},
				{ AttackType::RushAttack_Start, SE_ID::RushAttack_Sword},
				{ AttackType::RushAttack_End, SE_ID::RushAttack_Sword},
				{ AttackType::Charging, SE_ID::Charge}
			};
		}


		void SEList::RegisterHammerSEBank()
		{
			m_attackSEmap[WeaponType::Hammer] =
			{
				{ AttackType::NormalAttack, SE_ID::NormalAttack_Hammer},
				{ AttackType::HeavyAttack, SE_ID::HeavyAttack_Hammer},
				{ AttackType::AirAttack, SE_ID::AirAttack_Hammer},
				{ AttackType::ChargeAttack, SE_ID::ChargeAttack_Hammer},
				{ AttackType::PushForward, SE_ID::DashAttack_Hammer},
				{ AttackType::Charging, SE_ID::Charge}
			};
		}
	}
}