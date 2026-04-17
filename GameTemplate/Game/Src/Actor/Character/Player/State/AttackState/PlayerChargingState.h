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

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"
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
        class PlayerChargingState :public IState<nsActor::Actor>
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
            bool RequestID(uint8_t& id);


        /* ヘルパー。*/
        private:
            /**
             * @brief チャージ段階に応じたエフェクトの座標を求める。 
             */
            inline void ComputeEffectLevelByPosition()
            {
                m_currentEffectLevel = (std::max)(1, (std::min)(m_chargingTimer / 30, 3));
                m_player->SetChargeLevel(m_currentEffectLevel);
            }

            /**
             * @brief エフェクトを生成する。
             */
            inline void CreateChargeEffect()
            {
                m_spawnEffectPosition = m_player->GetPosition();
                m_spawnEffectPosition.y = EFFECT_POSITION_Y;
                m_chargeEffect = m_player->GetEffectList().PlayEffect(nsEffect::Charge, m_spawnEffectPosition);
            }

            /**
             * @brief チャージ段階に応じたエフェクトの大きさを求める。
             */
            inline void ComputeEffectLevelByScale()
            {
                m_effectScaleMultiplier = (std::min)(10.0f + (m_chargingTimer / 5.0f),30.0f);
                m_chargeEffect->SetScale(Vector3::One * m_effectScaleMultiplier);
            }

           /**
            * @brief エフェクトを追従更新させる。
            */
            inline void UpdateEffectPosition()
            {
                m_currentEffectPosition = m_player->GetPosition();
				m_currentEffectPosition.y = EFFECT_POSITION_Y;
				m_chargeEffect->SetPosition(m_currentEffectPosition);
            }

            /**
             * @brief  炎エフェクトを生成する。
             * @detail HammerCharacterクラスのみ適応。
             */
            void CreateFireEffect();


        private:
			nsActor::Player* m_player;          //! プレイヤーのポインタ。
            nsK2EngineLow::EffectEmitter* m_chargeEffect = nullptr;
            nsK2EngineLow::EffectEmitter* m_hammerEffect = nullptr;


        private:
			Vector3 m_effectPosition;                             //! エフェクトの座標。
			Vector3 m_effectScale;                                //! エフェクトのスケール。
			Vector3 m_spawnEffectPosition;                        //! エフェクトの生成座標。
			Vector3 m_currentEffectPosition;                      //! エフェクトの現在座標。
			Vector3 m_weaponPosition;                             //! 武器の座標。

			Quaternion m_fireEffectAngle = Quaternion::Identity;  //! 炎エフェクトの角度。

			int m_chargingTimer = 0;                              //! チャージ時間を管理するタイマー。
			int m_currentEffectLevel = 0;                         //! 現在のエフェクトのレベル。

			float m_effectScaleMultiplier = 1.0f;                 //! エフェクトの大きさの倍率。
			float m_fireEffectScale = 0;                          //! 炎エフェクトの大きさ。
        };


    }
}

