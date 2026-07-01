#include "stdafx.h"
#include "BossFlyState.h"
#include "Src/Actor/Character/Boss/Boss.h"

namespace nsApp
{
    namespace nsState
    {
        void BossFlyState::Enter()
        {
            m_boss = static_cast<nsActor::Boss *>(m_owner);

            m_flyTimer = 0.0f;
        }

        void BossFlyState::Update()
        {
            m_flyTimer += g_gameTime->GetFrameDeltaTime();
        }

        void BossFlyState::Exit()
        {
        }

        bool BossFlyState::RequestID(uint8_t &id)
        {
            if (m_flyTimer > 3.0f)
            {
                id = static_cast<uint8_t>(nsActor::BossStateID::enIdle);
                return true;
            }
            return false;
        }
    }
}