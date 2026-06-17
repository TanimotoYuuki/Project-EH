#pragma once
/**
 * @file   ChargeAttackStrategy.h
 * @brief  ボスの突進攻撃ストラテジー。プレイヤーを突き抜けて方向転換する。
 */

#include "Src/Actor/Character/Boss/Strategy/IBossAttackStrategy.h"

namespace nsApp
{
    namespace nsActor { class Boss; }

    namespace nsAI
    {
        /**
         * @class ChargeAttackStrategy
         * @brief プレイヤーに向かって高速突進し、通り抜けた後にWalkしながら方向転換する攻撃。
         */
        class ChargeAttackStrategy : public IBossAttackStrategy
        {
        public:
            /**
             * @brief 初期化処理。
			 * @param boss　攻撃主体のボス。攻撃開始前に一度だけ呼び出される。
             */
            void Enter(nsActor::Boss* boss) override;

            /**
             * @brief 更新処理。
			 * @param boss 攻撃主体のボス。攻撃中は毎フレーム呼び出される。
             */
            void Update(nsActor::Boss* boss) override;

            /**
			 * @brief 終了処理。
			 * @param boss　攻撃主体のボス。攻撃終了後に一度だけ呼び出される。
             */
            void Exit(nsActor::Boss* boss) override;

            /**
			 * @brief 攻撃終了判定。突進と方向転換の両フェーズが完了したら true を返す。
			 * @return 攻撃終了判定。突進と方向転換の両フェーズが完了したら true。
             */
            bool IsEnd() override;


        private:
            /**
			 * @brief 突進フェーズと方向転換フェーズの 2 つのフェーズを管理する列挙型。
             */
            enum class Phase : uint8_t
            {
                Charge,     //! 高速突進フェーズ。
                TurnAround, //! Walk しながら方向転換するフェーズ。
            };


            float   m_timer = 0.0f;                   //! 突進フェーズの残り時間。
            float   m_chargeSpeed = 0.0f;             //! 突進速度。
            float   m_turnTimer = 0.0f;               //! 方向転換フェーズの残り時間。

            Vector3 m_targetPosition = Vector3::Zero; //! 突進先（プレイヤーの後方）。
            Vector3 m_chargeDir = Vector3::Zero;      //! 突進方向（正規化済み）。

            Phase   m_phase = Phase::Charge;          //! 現在のフェーズ。
        };
    }
}