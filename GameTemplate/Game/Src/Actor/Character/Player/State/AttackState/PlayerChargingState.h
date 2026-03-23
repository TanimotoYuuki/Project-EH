#pragma once
#pragma once
/**
* @file PlayerChargingState.h
* @brief プレイヤーのチャージ状態を管理するクラス。
* @author Yamaguchi Hayato
* @date 2026/03/23
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Effect/EffectList.h"
#include "stdint.h"

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

        private:
			nsActor::Player* m_player;          //! プレイヤーのポインタ。
            nsK2EngineLow::EffectEmitter* m_chargeEffect = nullptr;


        private:
			Vector3 m_effectPosition;                   //! エフェクトの座標。
        };


    }
}

