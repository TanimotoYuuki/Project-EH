#include "stdafx.h"
#include "ParameterSystem.h"
#include "Src/Actor/Gun/Parameter/BulletParameterTable.h"
#include "Src/Actor/Magic/Parameter/MagicParameterTable.h"
#include "Src/Actor/Character/Status/AttackParameterTable.h"
#include "Src/Parameter/PlayerStatusParameterTable.h"
#include "Src/Actor/Character/Status/NPCStatusParameterTable.h"
#include "Src/Actor/Character/NPC/Component/NPCActionParameterTable.h"
#include "Src/Actor/Character/NPC/Component/RescueTimeTable.h"
#include "Src/Actor/Character/Player/Component/Guard/GuardParameterTable.h"
#include "Src/Actor/Character/NPC/Component/NPCBehaviorProfileTable.h"
#include "Src/Actor/Character/Boss/Status/BossParameterTable.h"

namespace nsApp
{
	bool ParameterSystem::LoadAll(const std::function<void(float)>& onProgress)
	{
		return LoadAllParameterTables(onProgress);
	}


	bool ParameterSystem::LoadAllParameterTables(const std::function<void(float)>& onProgress)
	{
		m_isSuccess = true;

		/* 読み込むテーブル数（進捗計算用）。*/
		constexpr int kTableCount = 10;
		int loadedCount = 0;

		/* 1テーブル読み込むごとに進捗を通知する。*/
		auto notifyProgress = [&]() { ++loadedCount;
			if (onProgress)
				onProgress(static_cast<float>(loadedCount) / static_cast<float>(kTableCount));
		};

		/* 銃弾パラメーターを読み込む。*/
		m_isSuccess &= BulletParameterTable::LoadTSVFile(MakeParameterPath("BulletParameterTable.tsv").c_str());
		notifyProgress();
		/* 魔法パラメーターを読み込む。*/
		m_isSuccess &= MagicParameterTable::LoadTSVFile(MakeParameterPath("MagicParameterTable.tsv").c_str());
		notifyProgress();
		/* NPCの行動パラメーターを読み込む。*/
		m_isSuccess &= NPCActionParameterTable::LoadTSVFile(MakeParameterPath("NPCActionParameter.tsv").c_str());
		notifyProgress();
		/* NPCのステータスパラメーターを読み込む。*/
		m_isSuccess &= NPCStatusParameterTable::LoadTSVFile(MakeParameterPath("NPCStatusParameter.tsv").c_str());
		notifyProgress();
		/* プレイヤーの攻撃ステータスパラメーターを読み込む。*/
		m_isSuccess &= AttackParameterTable::LoadTSVFile(MakeParameterPath("PlayerAttackParameterTable.tsv").c_str());
		notifyProgress();
		/* プレイヤーのステータスパラメーターを読み込む。*/
		m_isSuccess &= PlayerStatusParameterTable::LoadTSVFile(MakeParameterPath("PlayerStatusParameter.tsv").c_str());
		notifyProgress();
		/* 救助時間テーブルを読み込む。*/
		m_isSuccess &= RescueTimeTable::LoadTSVFile(MakeParameterPath("RescueTimeTable.tsv").c_str());
		notifyProgress();
		/* ガードパラメーターを読み込む。*/
		m_isSuccess &= GuardParameterTable::LoadTSVFile(MakeParameterPath("GuardParameter.tsv").c_str());
		notifyProgress();
		/* NPCの行動プロファイルを読み込む。*/
		m_isSuccess &= NPCBehaviorProfileTable::LoadTSVFile(MakeParameterPath("NPCBehaviorProfile.tsv").c_str());
		notifyProgress();
		/* ボスのパラメーターを読み込む。*/
		m_isSuccess &= nsAI::BossParameterTable::LoadTSVFile(MakeParameterPath("BossType.tsv").c_str());
		notifyProgress();

		return m_isSuccess;
	}


	std::string ParameterSystem::MakeParameterPath(const char* fileName) const
	{
		return m_parameterDirectory + fileName;
	}
}
