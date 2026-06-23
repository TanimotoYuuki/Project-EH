#include "stdafx.h"
#include "NPCChaseState.h"

/* Player。*/
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

/* NPC。*/
#include "Src/Actor/Character/NPC/State/BasicState/NPCIdleState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCSwordAttackState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCHammerAttackState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCWandAttackState.h"
#include "Src/Actor/Character/NPC/State/AttackState/NPCTwinGunAttackState.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCHelpState.h"
#include "Src/Actor/Character/NPC/Movement/NPCMovementController.h"
#include "Src/Actor/Character/NPC/Movement/NPCCombatRangeHelper.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCEvadeState.h"
#include "Src/Actor/Heel/HeelArea.h"

#include "Src/Actor/Character/NPC/Social/NPCSocialEvaluator.h"
#include "Src/Actor/Character/NPC/Social/NPCSocialContext.h"

namespace
{
	const auto MOVE_DEAD_ZONE = 0.001f;         //! 正規化前の安全閾値。
	const auto SEPARATION_BLEND_WEIGHT = 0.20f; //! 味方分離を移動方向に混ぜる強さ。
}

namespace nsApp
{
	namespace nsState
	{
		void NPCChaseState::Enter()
		{
			/* ブレインとコンポーネントの取得。*/
			m_brain = static_cast<NPCBrain*>(m_owner);

			/* ブレインがない場合はこの状態を維持できないため、ここで終了する。*/
			if (m_brain == nullptr)
			{
				m_body = nullptr;
				m_vInput = nullptr;
				return;
			}

			/* ブレインから必要なコンポーネントを取得。*/
			m_body = m_brain->GetBody();
			m_vInput = m_brain->GetVirtualInputAdapter();

			/* 移動入力をクリアしてから追跡を開始する。*/
			ClearMoveInput();
		}


		void NPCChaseState::Update()
		{
			if (!m_body || !m_vInput || !m_brain)
				return;

			/* HELP優先。*/
			auto* helpTarget = m_brain->GetHelpTarget();
			if (helpTarget != nullptr)
			{
				if (m_brain->ShouldRespondToHelp())
				{
					ClearMoveInput();
					if (m_stateMachine != nullptr)
						m_stateMachine->ChangeState(new NPCHelpState(helpTarget));
					return;
				}
			}

			/* 杖が魔法回復すべきときは HeelArea より魔法を優先。*/
			const bool wandNeedsMagicHeal = m_body->GetCurrentWeapon() == WeaponType::Wand && (m_brain->ShouldHealSelf() || m_brain->ShouldHealAlly());

			/* 杖が魔法回復すべきときは HeelArea より魔法を優先。*/
			if (!wandNeedsMagicHeal && TryMoveToHealArea())
				return;

			/* 杖の回復が必要なときは、攻撃インターバルより優先して回復行動を取る。*/
			if (wandNeedsMagicHeal)
			{
				/* 回復が必要か判断。*/
				auto* ally = m_brain->FindAllyNeedingHeal();
				if (ally != nullptr && m_brain->ShouldHealAlly())
				{
					/* 味方が回復範囲外なら近づく。*/
					const float dx = ally->GetPosition().x - m_body->GetPosition().x;
					if (fabsf(dx) > 80.0f)
					{
						/* 目標方向へ走って移動する。*/
						const float signX = (dx > 0.0f) ? 1.0f : -1.0f;
						Vector3 dir(signX, 0.0f, 0.0f);
						NPCMovementController::Apply(m_vInput, NPCMovementController::MakeMoveIntent(dir, true));
						return;
					}
				}

				/* 入力値をリセット。*/
				ClearMoveInput();

				/* 遷移。*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCWandAttackState());
				return;
			}

			/* 回避。*/
			if (m_brain->ShouldEvade())
			{
				ClearMoveInput();
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCEvadeState());
				return;
			}

			/* 追跡。*/
			auto* target = m_brain->SearchTarget();
			ExecuteChaseAction(target);
		}

		void NPCChaseState::Exit()
		{
			/* ブレインとコンポーネントがない場合は移動入力をクリアできないため、ここで終了する。*/
			ClearMoveInput();

			/* ブレインとコンポーネントのポインタをクリアする。*/
			m_brain = nullptr;
			m_body = nullptr;
			m_vInput = nullptr;
		}


		void NPCChaseState::ExecuteChaseAction(nsActor::ICharacter* target)
		{
			if (target == nullptr)
			{
				/* 入力値をリセット。*/
				ClearMoveInput();

				/* 待機ステートに遷移。*/
				if (m_stateMachine != nullptr)
					m_stateMachine->ChangeState(new NPCIdleState());
				return;
			}

			/* ブレインとコンポーネントがない場合は追跡行動を実行できないため、ここで終了する。*/
			const NPCBehaviorProfile& profile = m_brain->GetBehaviorProfile();
			const WeaponType weapon = m_body->GetCurrentWeapon();

			/* 目標との距離を計算。*/
			ComputeDistance(target);

			/* 遠距離武器：帯の外だけ動く。帯内は止まって攻撃。*/
			if (nsNPC::IsRangedWeaponByProfile(profile))
			{
				/* 攻撃可能なら後退より先に Attack へ */
				if (nsNPC::CanBeginAttack(m_distance, weapon, profile))
				{
					TransitionToAttackState();
					return;
				}
				if (nsNPC::ShouldRetreatWithHysteresis(m_distance, profile, m_brain->GetIsRetreatingFlag()))
				{
					MoveAwayFromTarget();
					return;
				}



				/* ヒステリシス帯の外なら、フラグなしでも動く */
				if (!m_brain->GetIsApproachingFlag() && !m_brain->GetIsRetreatingFlag())
				{
					if (m_distance > nsNPC::GetRangedAttackMaxDistanceByProfile(profile))
					{
						MoveTowardTarget();
						return;
					}
					if (m_distance < nsNPC::GetRangedRetreatEnterDistance(profile))
					{
						MoveAwayFromTarget();
						return;
					}
				}

				/* 杖：回復需要があれば攻撃インターバルより優先。*/
				if (weapon == WeaponType::Wand)
				{
					if (m_brain->ShouldHealSelf() || m_brain->ShouldHealAlly())
					{
						TransitionToAttackState();
						return;
					}
				}

				/* 攻撃帯に入ったらインターバル待ちでも攻撃ステートへ（棒立ち防止）。*/
				if (nsNPC::CanBeginAttack(m_distance, weapon, profile))
				{
					TransitionToAttackState();
					return;
				}
				/* まだ遠い／近すぎる場合のみ移動。*/
				if (m_distance > nsNPC::GetRangedAttackMaxDistanceByProfile(profile))
				{
					MoveTowardTarget();
					return;
				}
				if (m_distance < nsNPC::GetRangedRetreatEnterDistance(profile))
				{
					MoveAwayFromTarget();
					return;
				}
				/* 帯の端：微調整。*/
				MoveTowardTarget();
				return;
			}

			
			const float meleeMax = nsNPC::GetMeleeMaxAttackRange(weapon);
			if (nsNPC::CanBeginAttack(m_distance, weapon, profile))
			{
				TransitionToAttackState();
				return;
			}
			if (m_distance < meleeMax * 0.6f)
			{
				MoveAwayFromTarget();
				return;
			}
			MoveTowardTarget();
			return;
		}


		void NPCChaseState::TransitionToAttackState()
		{
			/* ブレインとコンポーネントがない場合は攻撃状態に遷移できないため、ここで終了する。*/
			if (m_stateMachine == nullptr || m_body == nullptr)
				return;

			/* 攻撃状態に遷移する前に、移動入力をリセットして NPC が攻撃中に不自然に動くのを防ぐ。*/
			ClearMoveInput();

			/* 現在の武器の種類を取得する。*/
			m_myWeapon = m_body->GetCurrentWeapon();

			/* 武器の種類に応じて、対応する攻撃状態に遷移する。*/
			if (m_myWeapon == WeaponType::Wand)
				m_stateMachine->ChangeState(new NPCWandAttackState());

			else if (m_myWeapon == WeaponType::Hammer)
				m_stateMachine->ChangeState(new NPCHammerAttackState());

			else if (m_myWeapon == WeaponType::TwinGun)
				m_stateMachine->ChangeState(new NPCTwinGunAttackState());

			else
				m_stateMachine->ChangeState(new NPCSwordAttackState());
		}


		void NPCChaseState::ClearMoveInput()
		{
			/* ブレインとコンポーネントがない場合は移動入力をクリアできないため、ここで終了する。*/
			NPCMovementController::Stop(m_vInput);
		}


		void NPCChaseState::MoveTowardTarget()
		{
			/* ブレインとコンポーネントがない場合は移動入力を入れられないため、ここで終了する。*/
			if (m_vInput == nullptr)
				return;

			/* 目標までの距離が閾値以下の場合は移動入力をクリアする。*/
			if (m_distance <= MOVE_DEAD_ZONE || m_difference.LengthSq() <= MOVE_DEAD_ZONE)
			{
				/* 目標が近すぎる場合は移動入力をクリアして停止する。*/
				ClearMoveInput();
				return;
			}

			/* 目標方向へ走って移動する（味方分離を少し混ぜる）。*/
			const auto signX = (m_difference.x > 0.0f) ? 1.0f : -1.0f;
			Vector3 direction(signX, 0.0f, 0.0f);

			/* 移動入力を入れる。*/
			const NPCMovementIntent intent = NPCMovementController::MakeMoveIntent(direction, true);
			NPCMovementController::Apply(m_vInput, intent);
		}


		void NPCChaseState::MoveAwayFromTarget()
		{
			if (m_vInput == nullptr)
				return;

			float signX = 0.0f;
			if (fabsf(m_difference.x) > MOVE_DEAD_ZONE)
				signX = (m_difference.x > 0.0f) ? -1.0f : 1.0f;
			else
				signX = (m_body->GetPosition().x >= m_difference.x + m_body->GetPosition().x) ? 1.0f : -1.0f;
			/* 上が分かりにくい場合は単純に: */
			// else signX = (m_partyIndex % 2 == 0) ? -1.0f : 1.0f;

			/* より安全: ボスより右にいれば左へ、左にいれば右へ */
			// （target は ExecuteChaseAction から渡すか、差分符号で判定）

			Vector3 awayDirection(signX, 0.0f, 0.0f);
			const NPCMovementIntent intent = NPCMovementController::MakeMoveIntent(awayDirection, true);
			NPCMovementController::Apply(m_vInput, intent);
		}


		bool NPCChaseState::TryMoveToHealArea()
		{
			if (m_body == nullptr || m_vInput == nullptr || m_brain == nullptr)
				return false;

			const auto& hp = m_body->GetCharacterStatus().hp;
			if (hp.maxHP <= 0)
				return false;

			const float rate = static_cast<float>(hp.currentHP) / static_cast<float>(hp.maxHP);
			if (rate > 0.65f)
				return false;

			auto* heelArea = FindGO<nsApp::HeelArea>("HeelArea");
			if (heelArea == nullptr || !heelArea->IsAlive())
				return false;

			const float dx = heelArea->GetAreaPosition().x - m_body->GetPosition().x;
			if (fabsf(dx) <= heelArea->GetRadius())
			{
				ClearMoveInput();
				return true;
			}

			const float signX = (dx > 0.0f) ? 1.0f : -1.0f;
			Vector3 dir(signX, 0.0f, 0.0f);
			NPCMovementController::Apply(
				m_vInput,
				NPCMovementController::MakeMoveIntent(dir, true));
			return true;
		}


		void NPCChaseState::ComputeDistance(nsActor::ICharacter* targetObject)
		{
			m_difference = targetObject->GetPosition() - m_body->GetPosition();
			m_difference.y = 0.0f;
			m_difference.z = 0.0f;
			m_distance = fabsf(m_difference.x);
		}


		Vector3 NPCChaseState::BlendSeparation(const Vector3& moveDirection) const
		{
			if (m_body == nullptr || moveDirection.LengthSq() <= MOVE_DEAD_ZONE)
				return moveDirection;

			nsNPC::NPCSocialContext context;
			context.self = m_body;

			const Vector3 separation = nsNPC::NPCSocialEvaluator::CalcSeparationVector(m_body->GetPosition(), context);
			if (separation.LengthSq() <= MOVE_DEAD_ZONE)
				return moveDirection;

			/* 前後移動のときは「横成分だけ」分離する（左右カタカタ防止）。*/
			Vector3 forward = moveDirection;
			forward.y = 0.0f;
			forward.Normalize();

			const float dot = separation.x * forward.x + separation.z * forward.z;
			Vector3 lateral;
			lateral.x = separation.x - forward.x * dot;
			lateral.z = separation.z - forward.z * dot;
			lateral.y = 0.0f;

			if (lateral.LengthSq() <= MOVE_DEAD_ZONE)
				return moveDirection;

			lateral.Normalize();

			Vector3 blended = forward;
			blended += lateral * SEPARATION_BLEND_WEIGHT;
			blended.y = 0.0f;

			if (blended.LengthSq() <= MOVE_DEAD_ZONE)
				return moveDirection;

			blended.Normalize();
			return blended;
		}
	}
}