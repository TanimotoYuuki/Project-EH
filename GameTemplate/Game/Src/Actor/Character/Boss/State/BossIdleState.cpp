#include "stdafx.h"
#include "BossIdleState.h"
#include "Src/Actor/Character/Boss/Boss.h"
#include "Src/Actor/Character/Boss/Status/BossManager.h"
#include <random>


namespace
{
    const float IDLE_TIME = 2.0f;

    std::mt19937 g_randomGen(std::random_device{}());

    /**
	 * @brief 攻撃的な行動パターンの待機状態の行動決定。。
	 * @param distance プレイヤーとの距離。
	 * @param hpRatio HP 割合（0.0f から 1.0f）。攻撃的なボスは HP 割合にあまり影響されないが、極端に低い場合は攻撃頻度が少し下がることもある。
	 * @return 次の行動の状態 ID（BossStateID）。攻撃的なボスは近距離で攻撃を選びやすく、遠距離では移動を選びやすい傾向がある。
     */
    uint8_t DecideByAggressive(float distance, float hpRatio)
    {
        using namespace nsApp::nsActor;
        using namespace nsApp::nsAI;

        std::uniform_int_distribution<int> dist(0, 99);
        int choice = dist(g_randomGen);

        if (distance < BossAIConfig::MEDIUM_ATTACK_DISTANCE)
            return (choice < 80) ? static_cast<uint8_t>(BossStateID::enAttack) : static_cast<uint8_t>(BossStateID::enMove);

        return (choice < 50) ? static_cast<uint8_t>(BossStateID::enAttack) : static_cast<uint8_t>(BossStateID::enMove);
    }

    /**
	 * @brief 防御的な行動パターンの待機状態の行動決定。近距離では攻撃を選ぶこともあるが、中距離以上では移動や待機を選びやすい。HP 割合が低いほど攻撃頻度が下がる傾向がある。
	 * @param distance プレイヤーとの距離。
	 * @param hpRatio HP 割合（0.0f から 1.0f）。防御的なボスは HP 割合が低いほど攻撃頻度が下がる傾向がある。近距離では攻撃を選ぶこともあるが、中距離以上では移動や待機を選びやすい。
	 * @return 次の行動の状態 ID（BossStateID）。防御的なボスは近距離で攻撃を選ぶこともあるが、中距離以上では移動や待機を選びやすい傾向がある。HP 割合が低いほど攻撃頻度が下がる傾向がある。
     */
    uint8_t DecideByDefensive(float distance, float hpRatio)
    {
        using namespace nsApp::nsActor;
        using namespace nsApp::nsAI;

        std::uniform_int_distribution<int> dist(0, 99);
        int choice = dist(g_randomGen);

        if (distance < BossAIConfig::CLOSE_ATTACK_DISTANCE)
        {
            return static_cast<uint8_t>(BossStateID::enAttack);
        }
        else if (distance < BossAIConfig::MEDIUM_ATTACK_DISTANCE)
        {
            int attackChance = (hpRatio > BossAIConfig::HIGH_HP_RATIO) ? 40 : 20;
            return (choice < attackChance)
                ? static_cast<uint8_t>(BossStateID::enAttack)
                : static_cast<uint8_t>(BossStateID::enMove);
        }

        return (choice < 40)
            ? static_cast<uint8_t>(BossStateID::enMove)
            : static_cast<uint8_t>(BossStateID::enIdle);
    }

    /**
	 * @brief トリッキーな行動パターンの待機状態の行動決定。距離と HP 割合に応じて攻撃と移動をバランスよく選ぶ。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。
	 * @param distance プレイヤーとの距離。トリッキーなボスは近距離で攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。
	 * @param hpRatio HP 割合（0.0f から 1.0f）。トリッキーなボスは HP 割合があまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。距離と HP 割合に応じて攻撃と移動をバランスよく選ぶ。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
	 * @return 次の行動の状態 ID（BossStateID）。トリッキーなボスは距離と HP 割合に応じて攻撃と移動をバランスよく選ぶ。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。
     */
    uint8_t DecideByTricky(float distance, float hpRatio)
    {
        using namespace nsApp::nsActor;
        using namespace nsApp::nsAI;

        std::uniform_int_distribution<int> dist(0, 99);
        int choice = dist(g_randomGen);

        if (distance < BossAIConfig::CLOSE_ATTACK_DISTANCE)
        {
            return (choice < 60)
                ? static_cast<uint8_t>(BossStateID::enAttack)
                : static_cast<uint8_t>(BossStateID::enMove);
        }
        else if (distance < BossAIConfig::MEDIUM_ATTACK_DISTANCE)
        {
            return (choice < 50)
                ? static_cast<uint8_t>(BossStateID::enAttack)
                : static_cast<uint8_t>(BossStateID::enMove);
        }

        return (choice < 80)
            ? static_cast<uint8_t>(BossStateID::enMove)
            : static_cast<uint8_t>(BossStateID::enAttack);
    }

    /**
	 * @brief バランスの取れた行動パターンの待機状態の行動決定。距離と HP 割合に応じて攻撃と移動をバランスよく選ぶ。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。
	 * @param boss　攻撃主体のボス。バランスの取れたボスは距離と HP 割合に応じて攻撃と移動をバランスよく選ぶ。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。
     * @param distance 距離。
	 * @param hpRatio HP 割合（0.0f から 1.0f）。バランスの取れたボスは HP 割合があまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。距離と HP 割合に応じて攻撃と移動をバランスよく選ぶ。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
	 * @return 次の行動の状態 ID（BossStateID）。バランスの取れたボスは距離と HP 割合に応じて攻撃と移動をバランスよく選ぶ。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。
     */
    uint8_t DecideByBalanced(nsApp::nsActor::Boss* boss, float distance, float hpRatio)
    {
        using namespace nsApp::nsActor;
        using namespace nsApp::nsAI;

        std::uniform_int_distribution<int> dist(0, 99);
        int choice = dist(g_randomGen);

        if (distance < BossAIConfig::CLOSE_ATTACK_DISTANCE)
            return static_cast<uint8_t>(BossStateID::enAttack);

        if (distance < BossAIConfig::MEDIUM_ATTACK_DISTANCE)
        {
            int moveChance = (hpRatio > BossAIConfig::HIGH_HP_RATIO) ? 50 :
                (hpRatio < BossAIConfig::LOW_HP_RATIO) ? 70 : 60;
            return (choice < moveChance)
                ? static_cast<uint8_t>(BossStateID::enMove)
                : static_cast<uint8_t>(BossStateID::enAttack);
        }

        return (choice < BossAIConfig::ROAR_MOVE_CHANCE)
            ? static_cast<uint8_t>(BossStateID::enMove)
            : static_cast<uint8_t>(BossStateID::enRoar);
    }

    /**
	 * @brief RedDragon 専用の行動決定。距離に応じて攻撃と移動の選択確率が変わる。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
	 * @param distance プレイヤーとの距離。RedDragon は距離に応じて攻撃と移動の選択確率が変わる。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
	 * @param hpRatio HP 割合（0.0f から 1.0f）。RedDragon は HP 割合があまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。距離に応じて攻撃と移動の選択確率が変わる。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
	 * @return 次の行動の状態 ID（BossStateID）。RedDragon は距離に応じて攻撃と移動の選択確率が変わる。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
     */
    uint8_t DecideByRedDragon(float distance, float hpRatio)
    {
        using namespace nsApp::nsActor;
        using namespace nsApp::nsAI;

        std::uniform_int_distribution<int> dist(0, 99);
        const int choice = dist(g_randomGen);

        if (distance < 70.0f)
        {
            return (choice < 85)
                ? static_cast<uint8_t>(BossStateID::enMove)
                : static_cast<uint8_t>(BossStateID::enAttack);
        }

        if (distance <= 130.0f)
        {
            return (choice < 55)
                ? static_cast<uint8_t>(BossStateID::enAttack)
                : static_cast<uint8_t>(BossStateID::enMove);
        }

        return (choice < 65)
            ? static_cast<uint8_t>(BossStateID::enMove)
            : static_cast<uint8_t>(BossStateID::enAttack);
    }

    /**
	 * @brief TutorialBoss 専用の行動決定。距離に応じて攻撃と移動の選択確率が変わる。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
	 * @param distance プレイヤーとの距離。TutorialBoss は距離に応じて攻撃と移動の選択確率が変わる。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
	 * @param hpRatio HP 割合（0.0f から 1.0f）。TutorialBoss は HP 割合があまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。距離に応じて攻撃と移動の選択確率が変わる。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
	 * @return 次の行動の状態 ID（BossStateID）。TutorialBoss は距離に応じて攻撃と移動の選択確率が変わる。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。
     */
    uint8_t DecideByTutorialBoss(float distance, float hpRatio)
    {
        using namespace nsApp::nsActor;
        using namespace nsApp::nsAI;

        std::uniform_int_distribution<int> dist(0, 99);
        const int choice = dist(g_randomGen);

        if (distance < BossAIConfig::CLOSE_ATTACK_DISTANCE)
        {
            return (choice < 80)
                ? static_cast<uint8_t>(BossStateID::enAttack)
                : static_cast<uint8_t>(BossStateID::enMove);
        }

        if (distance < BossAIConfig::MEDIUM_ATTACK_DISTANCE)
        {
            return (choice < 65)
                ? static_cast<uint8_t>(BossStateID::enAttack)
                : static_cast<uint8_t>(BossStateID::enMove);
        }

        return (choice < 50)
            ? static_cast<uint8_t>(BossStateID::enAttack)
            : static_cast<uint8_t>(BossStateID::enMove);
    }

    /**
	 * @brief GreenDragon 専用の行動決定。距離とボスの X 座標に応じて攻撃と移動の選択確率が変わる。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。また、ボスが画面左端付近にいる場合は攻撃を選びやすい傾向がある。
	 * @param distance プレイヤーとの距離。GreenDragon は距離とボスの X 座標に応じて攻撃と移動の選択確率が変わる。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。また、ボスが画面左端付近にいる場合は攻撃を選びやすい傾向がある。
	 * @param hpRatio HP 割合（0.0f から 1.0f）。GreenDragon は HP 割合があまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。距離とボスの X 座標に応じて攻撃と移動の選択確率が変わる。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。また、ボスが画面左端付近にいる場合は攻撃を選びやすい傾向がある。
	 * @param bossX ボスの X 座標。GreenDragon は距離とボスの X 座標に応じて攻撃と移動の選択確率が変わる。HP 割合はあまり影響しないが、極端に低い場合は攻撃頻度が少し下がることもある。近距離では攻撃を選びやすく、中距離では攻撃と移動が半々、遠距離では移動を選びやすい傾向がある。また、ボスが画面左端付近にいる場合は攻撃を選びやすい傾向がある。
     * @return 
     */
    uint8_t DecideByGreenDragon(float distance, float hpRatio, float bossX)
    {
        using namespace nsApp::nsActor;
        using namespace nsApp::nsAI;
        std::uniform_int_distribution<int> dist(0, 99);
        const int choice = dist(g_randomGen);
        const bool nearLeftEdge = (bossX <= 95.0f);
        if (nearLeftEdge)
        {
            return (choice < 75)
                ? static_cast<uint8_t>(BossStateID::enMove)
                : static_cast<uint8_t>(BossStateID::enAttack);
        }
        if (distance < BossAIConfig::CLOSE_ATTACK_DISTANCE)
        {
            return (choice < 45)
                ? static_cast<uint8_t>(BossStateID::enAttack)
                : static_cast<uint8_t>(BossStateID::enMove);
        }
        if (distance < BossAIConfig::MEDIUM_ATTACK_DISTANCE)
        {
            return (choice < 40)
                ? static_cast<uint8_t>(BossStateID::enAttack)
                : static_cast<uint8_t>(BossStateID::enMove);
        }
        return (choice < 85)
            ? static_cast<uint8_t>(BossStateID::enMove)
            : static_cast<uint8_t>(BossStateID::enAttack);
    }

    /**
	 * @brief 次の行動を決定する。距離と HP 割合に加えて、ボスタイプやロアの使用状況も考慮して行動を選ぶ。ロアは HP 割合が特定の閾値を下回ったときに優先的に使用される。特定のボスタイプ（RedDragon、TutorialBoss、GreenDragon）は独自の行動決定ロジックを持ち、それ以外は性格（Aggressive、Defensive、Tricky、Balanced）に基づいて行動が決まる。
	 * @param boss 攻撃主体のボス。距離と HP 割合に加えて、ボスタイプやロアの使用状況も考慮して行動を選ぶ。ロアは HP 割合が特定の閾値を下回ったときに優先的に使用される。特定のボスタイプ（RedDragon、TutorialBoss、GreenDragon）は独自の行動決定ロジックを持ち、それ以外は性格（Aggressive、Defensive、Tricky、Balanced）に基づいて行動が決まる。
	 * @param distance プレイヤーとの距離。距離と HP 割合に加えて、ボスタイプやロアの使用状況も考慮して行動を選ぶ。ロアは HP 割合が特定の閾値を下回ったときに優先的に使用される。特定のボスタイプ（RedDragon、TutorialBoss、GreenDragon）は独自の行動決定ロジックを持ち、それ以外は性格（Aggressive、Defensive、Tricky、Balanced）に基づいて行動が決まる。
	 * @param hpRatio HP 割合（0.0f から 1.0f）。距離と HP 割合に加えて、ボスタイプやロアの使用状況も考慮して行動を選ぶ。ロアは HP 割合が特定の閾値を下回ったときに優先的に使用される。特定のボスタイプ（RedDragon、TutorialBoss、GreenDragon）は独自の行動決定ロジックを持ち、それ以外は性格（Aggressive、Defensive、Tricky、Balanced）に基づいて行動が決まる。
	 * @return 次の行動の状態 ID（BossStateID）。距離と HP 割合に加えて、ボスタイプやロアの使用状況も考慮して行動を選ぶ。ロアは HP 割合が特定の閾値を下回ったときに優先的に使用される。特定のボスタイプ（RedDragon、TutorialBoss、GreenDragon）は独自の行動決定ロジックを持ち、それ以外は性格（Aggressive、Defensive、Tricky、Balanced）に基づいて行動が決まる。
     */
    uint8_t DecideNextAction( nsApp::nsActor::Boss* boss, float distance, float hpRatio)
    {
        using namespace nsApp::nsActor;
        using namespace nsApp::nsAI;

        float roarThreshold1 = BossAIConfig::ROAR_HP_THRESHOLD_FIRST;
        float roarThreshold2 = BossAIConfig::ROAR_HP_THRESHOLD_SECOND;

        if (hpRatio <= roarThreshold1 && hpRatio > roarThreshold2 &&
            boss->GetRoarUsageCount() < 1 && boss->CanUseRoar())
        {
            boss->UseRoar();
            return static_cast<uint8_t>(BossStateID::enRoar);
        }

        if (hpRatio <= roarThreshold2 && boss->GetRoarUsageCount() < 2 && boss->CanUseRoar())
        {
            boss->UseRoar();
            return static_cast<uint8_t>(BossStateID::enRoar);
        }

        if (boss->GetBossType() == nsApp::CharacterModelType::RedDragon)
            return DecideByRedDragon(distance, hpRatio);

        if (boss->GetBossType() == nsApp::CharacterModelType::TutorialBoss)
            return DecideByTutorialBoss(distance, hpRatio);

        if (boss->GetBossType() == nsApp::CharacterModelType::GreenDragon)
            return DecideByGreenDragon(distance, hpRatio, boss->GetPosition().x);

        const auto& params = BossTypeManager::GetBossTypeParameters(boss->GetBossType());

        switch (params.personality)
        {
        case BossPersonality::Aggressive:
            return DecideByAggressive(distance, hpRatio);

        case BossPersonality::Defensive:
            return DecideByDefensive(distance, hpRatio);

        case BossPersonality::Tricky:
            return DecideByTricky(distance, hpRatio);

        case BossPersonality::Balanced:
        default:
            return DecideByBalanced(boss, distance, hpRatio);
        }
    }
}


namespace nsApp
{
    namespace nsState
    {
        void BossIdleState::Enter()
        {
			/* ボスの待機時間を設定し、待機アニメーションを再生 */
            m_boss = static_cast<nsActor::Boss*>(m_owner);
            m_timer = m_boss->GetEffectiveIdleTime();
            m_boss->PlayAnimation(nsActor::BossAnimationID::Idle);
        }


        void BossIdleState::Update()
        {
			/* 待機時間を減算。時間が来たら次の行動を決定できるようになる */
            m_timer -= g_gameTime->GetFrameDeltaTime();
        }


        bool BossIdleState::RequestID(uint8_t& id)
        {
            if (m_timer <= 0.0f)
            {
                if (!m_boss || !m_boss->GetTarget() || m_boss->GetTarget()->IsDead())
                {
                    m_timer = IDLE_TIME;
                    return false;
                }

                float distance = m_boss->GetDistanceToTarget();
                float hpRatio = m_boss->GetHPRatio();

                id = DecideNextAction(m_boss, distance, hpRatio);
                return true;
            }
            return false;
        }
    }
}