#pragma once
/**
 * @file PlayerChargingState.h
 * @brief プレイヤーのチャージ状態を管理するクラス。
 * @author Yamaguchi Hayato
 * @date 2026/03/23 
 */

/*
 * @def 
 * Windowsのminとmaxのマクロ定義を無効化するための定義。
 */
#define NOMINMAX

#include "Src/Actor/Character/Player/State/AttackState/PlayerAttackBaseState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Common/IState.h"
#include "Src/Effect/EffectList.h"
#include "stdint.h"

namespace
{
	const auto EFFECT_POSITION_Y = 50.0f;            //! エフェクトの位置。
}

namespace nsApp
{
    namespace nsEffect {
        class EffectList;
    }

    namespace nsActor {
        class Player;
	}

    namespace nsState
    {
        class PlayerChargingState :public PlayerAttackBaseState
        {
        public:
            /* コンストラクタとデストラクタ。*/
			PlayerChargingState() = default;
			virtual ~PlayerChargingState() = default;


        public:
            /* ステートの初期化処理。*/
            void Enter() override;
            /* ステートの更新処理。*/
            void Update() override;
            /* ステートの終了処理。*/
			void Exit() override;
            /* 遷移。*/
            bool RequestID(uint8_t& id) override;


        /* ヘルパー。*/
        private:
            /**
             * @brief チャージ段階に応じたエフェクトの座標を求める。 
             */
            inline void ComputeEffectLevel()
            {
                m_timerValue = static_cast<int>(m_chargingTimer);
                m_currentEffectLevel = (std::max<int>)(1, (std::min<int>)(m_timerValue / 30, 3));
                m_player->SetChargeLevel(m_currentEffectLevel);
            }


        /* ゲッター。*/
        public:
            /**
             * @brief チャージレベルのスケールを返す。
             */
            inline float GetChargeEffectScale()
            {
                return (std::min)(10.0f +( m_chargingTimer / 5.0f), 30.0f);
            }

            /**
             * @brief チャージエフェクトの座標を返す。
             */
            inline Vector3 GetChargeEffectPosition()
            {
                m_getPlayerPosition = m_player->GetPosition();
                m_getPlayerPosition.y = EFFECT_POSITION_Y;
				return m_getPlayerPosition;
            }

            /**
             * @brief チャージエフェクトの大きさを返す。
             */
            inline float GetFireEffectScale()
            {
                return 1.25f + (m_currentEffectLevel * 2.5f);
            }


        private:
            /**
             * @brief  炎エフェクトを生成する。
             * @detail HammerCharacterクラスのみ適応。
             */
            void CreateFireEffect();

            /**
             * @brief チャージエフェクトを生成する。
             */
			void CreateChargeEffect();
            

        private:
            nsK2EngineLow::EffectEmitter* m_chargeEffect = nullptr;
            nsK2EngineLow::EffectEmitter* m_hammerEffect = nullptr;


        private:
			Vector3 m_effectPosition;                                   //! エフェクトの座標。
			Vector3 m_effectScale;                                      //! エフェクトのスケール。
			Vector3 m_spawnEffectPosition;                              //! エフェクトの生成座標。
			Vector3 m_currentEffectPosition;                            //! エフェクトの現在座標。
			Vector3 m_weaponPosition;                                   //! 武器の座標。

			Quaternion m_fireEffectAngle = Quaternion::Identity;        //! 炎エフェクトの角度。

			int m_timerValue = 0;                                       //! タイマーの値。
			int m_chargingTimer = 0;                                    //! チャージ時間を管理するタイマー。
			int m_currentEffectLevel = 0;                               //! 現在のエフェクトのレベル。

			float m_effectScaleMultiplier = 1.0f;                       //! エフェクトの大きさの倍率。
			float m_fireEffectScale = 0;                                //! 炎エフェクトの大きさ。
        };
    }
}

