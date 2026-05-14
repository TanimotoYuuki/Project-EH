#include "stdafx.h"
#include "Boss.h"

#include "BossIdleState.h"
#include "BossMoveState.h"
#include "BossAttackState.h"
#include "BossDamageState.h"
#include "BossRoarState.h"
#include "BossDethState.h"

namespace
{
	const Vector3 START_POSITION{ 200.0,100.0f,0.0f };
	const int BOSS_MAX_HP = 5000;
	const float ROT_SPEED = 5.0f;
}

namespace nsApp
{
	namespace nsActor
	{
		/*ボスごとのベースオフセット。モデルの原点位置に合わせて調整。*/
		const std::unordered_map<CharacterModelType, Vector3> BOSS_OFFSETS =
		{
			{CharacterModelType::GrayDragon,	Vector3(0.0f,0.0f,0.0f)},
			{CharacterModelType::TutorialBoss,	Vector3(0.0f,0.0f,0.0f)},
			{CharacterModelType::RedDragon,		Vector3(0.0f,0.0f,0.0f)},
			{CharacterModelType::GreenDragon,	Vector3(0.0f,0.0,0.0f)},
		};

		/*ボスタイプを文字列に変換。*/
		static const char* BossTypeToString(CharacterModelType bossType)
		{
			switch (bossType)
			{
			case CharacterModelType::GrayDragon:   return "GrayDragon";
			case CharacterModelType::GreenDragon:  return "GreenDragon";
			case CharacterModelType::RedDragon:    return "RedDragon";
			case CharacterModelType::TutorialBoss: return "TutorialBoss";
			default:                               return "TutorialBoss";
			}
		}

		/*特定のステートの時だけ追加で浮かせるテーブル。*/
		const std::unordered_map<BossStateID, Vector3> STATE_OFFSETS =
		{

			{BossStateID::enDamage,		Vector3(0.0f,5.0f,0.0f)},
		};

		/*ステータス。*/
		void Boss::InitStatus()
		{
			/*ボスの体力。*/
			m_characterStatus.hp.maxHP = BOSS_MAX_HP;
			/*現在の体力。*/
			m_characterStatus.hp.currentHP = BOSS_MAX_HP;
			m_prevHP = BOSS_MAX_HP;

			m_hitStopFlame = 0;
		}

		bool Boss::Start()
		{
			if (!m_stateMachine)
			{
				m_stateMachine = new nsState::StateMachine<Actor>(this);
			}


			/*アニメーションの初期化。*/
			m_BossAimation = std::make_unique<BossAnimation>();
			m_BossAimation->Init(BossTypeToString(m_bossType));

			/*モデルの読み込み。*/
			m_model.LoadCharacterModel
			(
				m_bossType,
				m_BossAimation->GetAnimationClips(),
				(int)BossAnimationID::Max
			);

			auto it = BOSS_OFFSETS.find(m_bossType);
			if (it != BOSS_OFFSETS.end())
			{
				m_modelOffset = it->second;
			}

			/*初期座標を設定。*/
			m_position = START_POSITION;
			m_BossController.SetPosition(m_position);
			m_model.SetPosition(m_position);

			m_forward = Vector3::Left;

			m_rotation.SetRotationYFromDirectionXZ(m_forward);

			/*キャラコンを初期化。*/
			m_BossController.Init(40.0, 20.0f, m_position);

			m_BossController.SetPosition(m_position);
			m_model.SetPosition(m_position);

			/*スケールを設定。*/
			m_model.SetCharacterScale(Vector3::One * 0.2f);


			/*ヒット判定初期化。*/
			m_BiteHit.Init(3.0f);
			m_TailHit.Init(4.0f);
			m_FireHit.Init(6.0f);

			/*ステート登録。*/
			RegisterState();
			m_stateMachine->ChangeState(m_stateFactory[enIdle]());

			/*初期化前にステータスを確定。*/
			InitStatus();

			/*初期化。*/
			m_prevHP = m_characterStatus.hp.currentHP;

			return true;
		}

		void Boss::Update()
		{
			/*ICharacterクラスの更新処理をコール。*/
			ICharacter::Update();

			if (IsHitStop())return;

			/*ステートマシンを更新する。*/
			m_stateMachine->Update();

			/*次のステートを決定する。*/
			uint8_t nextID = m_currentStateID;

			/*強制遷移刑を優先。*/
			if (m_characterStatus.hp.currentHP <= 0 )
			{
				nextID = BossStateID::enDeath ;
			}
			else if (IsDamage())
			{
				nextID = BossStateID::enDamage;
			}
			/*ステートからの遷移リクエストを確認。*/
			else
			{
				uint8_t reqID = 0;
				if (m_stateMachine->GetCurrentState()->RequestID(reqID))
				{
					nextID = reqID;
				}
			}

			/*ステートが変わるときだけステートを変更。*/
			if (nextID != m_currentStateID)
			{
				auto next = static_cast<BossStateID>(nextID);
				if (m_stateFactory.count(next) > 0)
				{
					m_currentStateID = next;
					m_stateMachine->ChangeState(m_stateFactory[next]());
				}
			}

			/*プレイヤーの方向を向く処理。*/
			UpdateRotation(g_gameTime->GetFrameDeltaTime());

			/*位置を固定。*/
			if (m_isYLocked)
			{
				m_position.y = m_lockedYPosition;
				m_BossController.SetPosition(m_position);
			}
			else
			{
				m_position = m_BossController.GetPosition();
			}
			/*z軸を固定。*/
			m_position.z = 0.0f;

			/*モデルの座標を設定。*/ 
			m_model.SetPosition(m_position);

			/*回転反映。*/
			m_model.SettRotation(m_rotation);

			Vector3 finalOffset = m_modelOffset + m_modelDynamicOffset;

			auto it = STATE_OFFSETS.find((BossStateID)m_currentStateID);
			if (it != STATE_OFFSETS.end())
			{
				finalOffset += it->second;
			}

			m_model.SetPosition(m_position + finalOffset);

			/*モデル更新。*/
			m_model.Update();
		}

		/*プレイヤーの方向を向く処理。*/
		void Boss::UpdateRotation(float deltaTime)
		{
			if (!m_target||m_target->IsDead()) return;

			Vector3 toTarget = m_target->GetPosition() - m_position;
			toTarget.y = 0.0f;

			if (toTarget.LengthSq() < FLT_EPSILON)
			{
				return;
			}

			toTarget.Normalize();
			m_forward = toTarget;

			Quaternion targetRot;
			targetRot.SetRotationYFromDirectionXZ(m_forward);

			float rate = ROT_SPEED * deltaTime;
			if (rate >= 1.0f)
			{
				m_rotation = targetRot;
			}
			else
			{
				Quaternion currentRot = m_rotation;
				m_rotation.Slerp(rate, currentRot, targetRot);
			}
		}

		/*噛みつき攻撃。*/ 
		void Boss::AttackBite() 
		{
			m_attackPosition = m_position + m_forward * 2.0f;

			m_BiteHit.Enable(); 
			m_BiteHit.Update(m_attackPosition);
		}

		/*尻尾攻撃。*/ 
		void Boss::AttackTail()
		{
			m_attackPosition = m_position - m_forward * 3.0f;

			m_TailHit.Enable();
			m_TailHit.Update(m_attackPosition);
		}

		/*火炎攻撃。*/
		void Boss::ShotFireBall()
		{
			if (m_forward.LengthSq() < 0.01f)m_forward = Vector3::Left;

			m_FireHit.Enable();

			/*ブレスを前に伸ばす。*/
			for (int i = 1; i <= 3; i++)
			{
				Vector3 pos = m_position + m_forward * (2.0f * i);
				m_FireHit.Update(pos);
			}
		}

		/*ダメージ判定。*/
		bool Boss::IsDamage()
		{
			bool isDamage = m_characterStatus.hp.currentHP < m_prevHP;
			m_prevHP = m_characterStatus.hp.currentHP;
			return isDamage;
		}

		void Boss::Render(RenderContext& rc)
		{
			ICharacter::Render(rc);
		}

		/*ステート。*/
		void Boss::RegisterState()
		{
			/*待機アニメーション。*/
			m_stateFactory[BossStateID::enIdle] = []() {return new nsState::BossIdleState(); };
			/*移動アニメーション。*/
			m_stateFactory[BossStateID::enMove] = []() {return new nsState::BossMoveState(); };
			/*攻撃アニメーション。*/
			m_stateFactory[BossStateID::enAttack] = []() {return new nsState::BossAttackState(); };
			/*咆哮アニメーション。*/
			m_stateFactory[BossStateID::enRoar] = []() {return new nsState::BossRoarState(); };
			/*被弾アニメーション。*/
			m_stateFactory[BossStateID::enDamage] = []() {return new nsState::BossDamageState(); };
			/*死亡アニメーション。*/
			m_stateFactory[BossStateID::enDeath] = []() {return new nsState::BossDethState(); };

		}
	}
}