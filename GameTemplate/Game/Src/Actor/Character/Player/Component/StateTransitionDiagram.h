#pragma once
/**
* @file   StateTransitionDiagram.h
* @brief  状態遷移図を管理するクラス。
* @author Yamaguchi Hayato
* @date 　2026/03/24
*/

#include "stdint.h"
#include "Src/Actor/Character/Player/PlayerInput.h"
#include "Src/Actor/Character/Player/Player.h"



namespace nsApp 
{
	namespace nsState
	{
		struct Transition
		{
			bool condition;                     //! 遷移条件。
			nsActor::PlayerStateID nextStateID; //! 遷移先のステートID。
		};


		class StateTransitionDiagram
		{
		public:
			/* 
			 * 状態遷移図を確認する。
			 * @param inputClass:   playerの入力を検知するクラス。
			 * @param transitionID: 遷移ID。
			 */
			static bool CheckCommonTransition(const PlayerInput& inputClass, uint8_t& transitionID)
			{
				/* 状態遷移図。*/
				Transition transitionDiagram[] =
				{
					/* 死亡状態。*/
					{ inputClass.IsDeath(), nsActor::PlayerStateID::enDeath },
					/* ダメージ状態。*/
					{ inputClass.IsDamage(), nsActor::PlayerStateID::enHit },
					/* 走り状態。*/
					{ inputClass.IsRun(), nsActor::PlayerStateID::enRun },
					/* ジャンプ状態。*/
					{ inputClass.IsJump(), nsActor::PlayerStateID::enJump },
					/* 歩き状態。*/
					{ inputClass.IsMove(), nsActor::PlayerStateID::enWalk },
					/* 通常攻撃状態。*/
					{ inputClass.IsNormalAttack(), nsActor::PlayerStateID::enNormalAttack },
					/* チャージ状態。*/
					{ inputClass.IsCharging(), nsActor::PlayerStateID::enCharging},
					/* チャージ攻撃状態。*/
					{ inputClass.IsChargeAttack(), nsActor::PlayerStateID::enChargeAttack },
					/* コンボ攻撃状態。*/
					{ inputClass.IsComboAttack(), nsActor::PlayerStateID::enComboAttack },
					/* 連続攻撃開始判定。*/
					{ inputClass.IsRushStart(), nsActor::PlayerStateID::enRushStart},
					/* 連続攻撃終了判定。*/
					{ inputClass.IsRushEnd(), nsActor::PlayerStateID::enRushEnd},
					/* ガード状態。*/
					{ inputClass.IsGuard(), nsActor::PlayerStateID::enGuard},
					/* 助ける状態。*/
					{ inputClass.IsHelp(), nsActor::PlayerStateID::enHelp},
					/* 魔法攻撃状態。*/
					{ inputClass.IsPressRB(), nsActor::PlayerStateID::enMagicAttack},
					/* 回復魔法状態。*/
					{ inputClass.IsPressRT(), nsActor::PlayerStateID::enHeelMagic},
				};


				/* 条件を満たしているのか確認。*/
				for (const auto& transion : transitionDiagram)
				{
					if (transion.condition)
					{
						/* 遷移先をキャスト。*/
						transitionID = static_cast<uint8_t>(transion.nextStateID);
						return true;
					}
				}

				/* どれにも満たしていない場合。*/
				return false;
			}
		};
	}
}
