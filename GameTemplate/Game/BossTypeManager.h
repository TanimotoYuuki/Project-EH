#pragma once

namespace nsApp
{
    namespace nsAI
    {
        enum class BossPersonality
        {
            Aggressive, /*攻撃的。*/
            Balanced,   /*バランス型。*/
            Defensive,  /*防御的。*/
            Tricky      /*トリッキー。*/
        };

        struct BossTypeParameters
        {
            /*性格。*/
            BossPersonality personality;

            /*移動パラメーター。*/
            float m_moveSpeed;
            float m_moveStopDistance;

            /*AIパラメーター。*/
            float m_idleTime;
            float m_moveDuration;
            float m_roarCooldown;

            /*攻撃パラメーター。*/
            int m_preferredBiteChance;
            int m_preferredTailChance;
            int m_preferredFireChance;

            /*ステータス。*/
            int m_baseHP;
            float m_Multiplier;

            /*特殊能力フラグ。*/
            bool hasSpecialAbility;
            bool canRoar;
        };

        class BossTypeManager
        {
        public:
            static const BossTypeParameters &GetBossTypeParameters(CharacterModelType bossType)
            {
                static const BossTypeParameters grayDragonParameters =
                    {
                        BossPersonality::Aggressive, /*攻撃的。*/
                        12.0f,                       /*移動速度。*/
                        50.0f,                       /*移動停止距離。*/
                        1.5f,                        /*待機時間。*/
                        2.5f,                        /*移動時間。*/
                        12.0f,                       /*咆哮クールダウン。*/
                        70, 20, 10,                  /*攻撃チャンス。*/
                        5000,                        /*HP。*/
                        1.0f,                        /*HP倍率。*/
                        false, true                  /*特殊能力なし。*/
                    };

                static const BossTypeParameters redDragonParameters =
                    {
                        BossPersonality::Balanced, /*バランス型。*/
                        10.0f,                     /*移動速度。*/
                        60.0f,                     /*移動停止距離。*/
                        2.0f,                      /*待機時間。*/
                        3.0f,                      /*移動時間。*/
                        15.0f,                     /*咆哮クールダウン。*/
                        20, 30, 50,                /*攻撃チャンス。*/
                        6000,                      /*HP。*/
                        1.2f,                      /*HP倍率。*/
                        true, true                 /*特殊能力あり。*/
                    };

                static const BossTypeParameters greenDragonParameters =
                    {
                        BossPersonality::Tricky, /*トリッキー。*/
                        14.0f,                   /*移動速度。*/
                        40.0f,                   /*移動停止距離。*/
                        1.0f,                    /*待機時間。*/
                        4.0f,                    /*移動時間。*/
                        10.0f,                   /*咆哮クールダウン。*/
                        15, 70, 15,              /*攻撃チャンス。*/
                        4500,                    /*HP。*/
                        0.9f,                    /*HP倍率。*/
                        false, false             /*特殊能力なし。*/
                    };

                static const BossTypeParameters tutorialBossParameters =
                    {
                        BossPersonality::Defensive, /*防御的。*/
                        8.0f,                       /*移動速度。*/
                        55.0f,                      /*移動停止距離。*/
                        2.5f,                       /*待機時間。*/
                        2.0f,                       /*移動時間。*/
                        20.0f,                      /*咆哮クールダウン。*/
                        50, 30, 20,                 /*攻撃チャンス。*/
                        3000,                       /*HP。*/
                        0.6f,                       /*HP倍率。*/
                        false, false                /*特殊能力なし。*/
                    };

                switch (bossType)
                {
                case CharacterModelType::GrayDragon:
                    return grayDragonParameters;
                case CharacterModelType::RedDragon:
                    return redDragonParameters;
                case CharacterModelType::GreenDragon:
                    return greenDragonParameters;
                case CharacterModelType::TutorialBoss:
                    return tutorialBossParameters;
                default:
                    return tutorialBossParameters; /*デフォルトはチュートリアルボスのパラメーター。*/
                }
            }
        };
    }
}