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

namespace nsApp
{
	bool ParameterSystem::LoadAll()
	{
		return LoadAllParameterTables();
	}


	bool ParameterSystem::LoadAllParameterTables()
	{
		m_isSuccess = true;

		/* 銃弾パラメーターを読み込む。*/
		m_isSuccess &= BulletParameterTable::LoadTSVFile(MakeParameterPath("BulletParameterTable.tsv").c_str());
		/* 魔法パラメーターを読み込む。*/
		m_isSuccess &= MagicParameterTable::LoadTSVFile(MakeParameterPath("MagicParameterTable.tsv").c_str());
		/* NPCの行動パラメーターを読み込む。*/
		m_isSuccess &= NPCActionParameterTable::LoadTSVFile(MakeParameterPath("NPCActionParameter.tsv").c_str());
		/* NPCのステータスパラメーターを読み込む。*/
		m_isSuccess &= NPCStatusParameterTable::LoadTSVFile(MakeParameterPath("NPCStatusParameter.tsv").c_str());
		/* プレイヤーの攻撃ステータスパラメーターを読み込む。*/
		m_isSuccess &= AttackParameterTable::LoadTSVFile(MakeParameterPath("PlayerAttackParameterTable.tsv").c_str());
		/* プレイヤーのステータスパラメーターを読み込む。*/
		m_isSuccess &= PlayerStatusParameterTable::LoadTSVFile(MakeParameterPath("PlayerStatusParameter.tsv").c_str());
		/* 救助時間テーブルを読み込む。*/
		m_isSuccess &= RescueTimeTable::LoadTSVFile(MakeParameterPath("RescueTimeTable.tsv").c_str());
		/* ガードパラメーターを読み込む。*/
		m_isSuccess &= GuardParameterTable::LoadTSVFile(MakeParameterPath("GuardParameter.tsv").c_str());
		/* NPCの行動プロファイルを読み込む。*/
		m_isSuccess &= NPCBehaviorProfileTable::LoadTSVFile(MakeParameterPath("NPCBehaviorProfile.tsv").c_str());
	
		return m_isSuccess;
	}

	std::string ParameterSystem::MakeParameterPath(const char* fileName) const
	{
		return m_parameterDirectory + fileName;
	}
}
