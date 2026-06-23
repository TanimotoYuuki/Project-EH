#include "stdafx.h"
#include "BossParameterTable.h"
#include "Src/Utilty/TSVTable.h"

namespace nsApp
{
    namespace nsAI
    {
        std::unordered_map<CharacterModelType, BossTypeParameters> BossParameterTable::m_table;


        bool BossParameterTable::LoadTSVFile(const char* filePath)
        {
            std::unordered_map<CharacterModelType, BossTypeParameters> loaded;
            CreateDefaultTable(loaded);

            TSVTable table;
            if (!table.LoadTSVFile(filePath))
            {
                m_table.swap(loaded);
                return false;
            }

            for (int i = 0; i < table.GetRowCount(); ++i)
            {
                const std::string typeName = table.GetString(i, "BossType");
                if (typeName.empty())
                    continue;

                CharacterModelType type;
                if (!ConvertBossType(typeName, type))
                    continue;

                loaded[type] = CreateParameterFromRow(table, i);
            }

            m_table.swap(loaded);
            return true;
        }


        const BossTypeParameters& BossParameterTable::GetParameters(CharacterModelType bossType)
        {
            if (m_table.empty())
                CreateDefaultTable(m_table);

            auto it = m_table.find(bossType);
            if (it != m_table.end())
                return it->second;

            return m_table[CharacterModelType::TutorialBoss];
        }


        BossPersonality BossParameterTable::ConvertPersonality(const std::string& name)
        {
            if (name == "Aggressive") return BossPersonality::Aggressive;
            if (name == "Balanced")   return BossPersonality::Balanced;
            if (name == "Defensive")  return BossPersonality::Defensive;
            if (name == "Tricky")     return BossPersonality::Tricky;
            return BossPersonality::Balanced;
        }


        bool BossParameterTable::ConvertBossType(const std::string& name, CharacterModelType& outType)
        {
            if (name == "GrayDragon") { outType = CharacterModelType::GrayDragon;   return true; }
            if (name == "RedDragon") { outType = CharacterModelType::RedDragon;    return true; }
            if (name == "GreenDragon") { outType = CharacterModelType::GreenDragon;  return true; }
            if (name == "TutorialBoss") { outType = CharacterModelType::TutorialBoss; return true; }
            return false;
        }


        BossTypeParameters BossParameterTable::CreateParameterFromRow(
            const TSVTable& table, int rowIndex)
        {
            BossTypeParameters p;

            p.personality = ConvertPersonality(table.GetString(rowIndex, "personality"));
            p.m_moveSpeed = table.GetFloat(rowIndex, "moveSpeed", 10.0f);
            p.m_moveStopDistance = table.GetFloat(rowIndex, "moveStopDistance", 55.0f);
            p.m_idleTime = table.GetFloat(rowIndex, "idleTime", 2.0f);
            p.m_moveDuration = table.GetFloat(rowIndex, "moveDuration", 2.0f);
            p.m_roarCooldown = table.GetFloat(rowIndex, "roarCooldown", 15.0f);

            p.m_preferredBiteChance = static_cast<int>(table.GetFloat(rowIndex, "biteChance", 50.0f));
            p.m_preferredTailChance = static_cast<int>(table.GetFloat(rowIndex, "tailChance", 30.0f));
            p.m_preferredFireChance = static_cast<int>(table.GetFloat(rowIndex, "fireChance", 20.0f));

            p.m_baseHP = static_cast<int>(table.GetFloat(rowIndex, "baseHP", 3000.0f));
            p.m_Multiplier = table.GetFloat(rowIndex, "hpMultiplier", 1.0f);

            const std::string hasSpec = table.GetString(rowIndex, "hasSpecialAbility");
            p.hasSpecialAbility = (hasSpec == "TRUE" || hasSpec == "true" || hasSpec == "1");

            const std::string canRoarStr = table.GetString(rowIndex, "canRoar");
            p.canRoar = (canRoarStr == "TRUE" || canRoarStr == "true" || canRoarStr == "1");

            p.m_dashSpeedMultiplier = table.GetFloat(rowIndex, "dashSpeedMultiplier", 1.0f);
            p.m_sideStepChance = table.GetFloat(rowIndex, "sideStepChance", 0.0f);

            const std::string airStr = table.GetString(rowIndex, "useAirReposition");
            p.m_useAirReposition = (airStr == "TRUE" || airStr == "true" || airStr == "1");

            return p;
        }


        void BossParameterTable::CreateDefaultTable( std::unordered_map<CharacterModelType, BossTypeParameters>& table)
        {
            table[CharacterModelType::GrayDragon] = { BossPersonality::Aggressive, 12.0f, 50.0f, 1.5f, 2.5f, 12.0f, 70, 20, 10, 5000, 1.0f, false, true,  2.0f, 0.0f, false };
            table[CharacterModelType::RedDragon] = { BossPersonality::Balanced,   10.0f, 60.0f, 2.0f, 3.0f, 15.0f, 20, 30, 50, 6000, 1.2f, true,  true,  1.2f, 0.2f, false };
            table[CharacterModelType::GreenDragon] = { BossPersonality::Tricky, 14.0f, 40.0f, 1.0f, 4.0f, 10.0f, 10, 90, 0, 4500, 0.9f, false, false, 1.5f, 0.5f, true };
            table[CharacterModelType::TutorialBoss] = { BossPersonality::Defensive, 8.0f, 55.0f, 2.5f, 2.0f, 20.0f, 34, 33, 33, 3000, 0.6f, false, false, 0.8f, 0.1f, false };
        }
    }
}