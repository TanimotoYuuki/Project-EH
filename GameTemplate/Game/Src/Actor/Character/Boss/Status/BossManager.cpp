#include "stdafx.h"
#include "BossManager.h"
#include "BossParameterTable.h"

namespace nsApp
{
    namespace nsAI
    {
        const BossTypeParameters& BossTypeManager::GetBossTypeParameters(CharacterModelType bossType)
        {
            return BossParameterTable::GetParameters(bossType);
        }
    }
}