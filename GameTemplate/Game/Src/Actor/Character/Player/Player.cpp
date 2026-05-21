#include "stdafx.h"
#include "Player.h"

/* 基本動作ステート。*/
#include "Src/Actor/Character/Player/State/BasicState/PlayerIdleState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerWalkState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerJumpState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerRunState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerHitState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerDethState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerGuardState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerReBoneState.h"
#include "Src/Actor/Character/Player/State/BasicState/PlayerGetUpState.h"

/* 攻撃ステート。*/
#include "Src/Actor/Character/Player/State/AttackState/PlayerNormalAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargeAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerAirAttackState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushStartState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerRushEndState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerSlashUpState.h"
#include "Src/Actor/Character/Player/State/AttackState/ComboState/PlayerPushState.h"
#include "Src/Actor/Character/Player/State/AttackState/PlayerChargingState.h"

#include "Src/Actor/Character/Common/WeaponHitDetection.h"
#include "Src/Sound/SoundLister.h"

#include "Src/SceneLoader/SceneLoader.h""

namespace
{
	const auto CHARACON_RADIUS = 12.5f;                 //! キャラクターコントローラーの半径。
	const auto CHARACON_HEIGHT = 30.0f;                 //! キャラクターコントローラーの高さ。
	const auto WEAPON_HIT_RADIUS = 40.0f;               //! 武器の当たり判定の半径。
	const auto ANGLE_Y = 90.0f;                         //! プレイヤーの初期角度。
	const auto CHARACTER_SCALE = 0.5f;                  //! プレイヤーのスケール。

	const Vector3 POS = Vector3(0.0f,100.0f, 0.0f);     //! プレイヤーの初期座標。

	struct PlayerSetupData
	{
		const char* name;                               //! モデルの名前。
		nsApp::CharacterModelType modelType;            //! モデルの種類。
		int padIndex;                                   //! 要素数の何番目を用いて操作を指示するのか。
	};

	const PlayerSetupData SET_UP_DATA[4] =
	{
		/*
		 * @brief  生成名/識別子/コントローラーのインデックスを付与。
		 * @detail 右端のインデックス数は0がメインPCの番号となる。
		 * @TODO:  NPC実装のためにも実装を考える必要あり。
		 */
		{"player1", nsApp::CharacterModelType::Player_1P, 0},
		{"player2", nsApp::CharacterModelType::Player_2P, 0},
		{"player3", nsApp::CharacterModelType::Player_3P, 2},
		{"player4", nsApp::CharacterModelType::Player_4P, 0}
	};
}

namespace nsApp
{
	namespace nsActor
	{
		bool Player::Start()
		{

			for (int i = 0; i < 4; i++)
			{
				if (IsMatchName(SET_UP_DATA[i].name))
				{
					m_playerInput.SetPadIndex(SET_UP_DATA[i].padIndex);
					m_modelType = SET_UP_DATA[i].modelType;
					break;
				}
			}

			/* アニメーションとモデルを準備する。*/
			/* アニメーションクラスの初期化処理をコール。*/
			m_playerAnimation.Initialize();
			/* 今の武器をセットする。*/
			m_playerAnimation.LoadAnimation(m_currentWeapon);

			/* モデルの種類/アニメーションの種類/アニメーションの数をセットする。*/
			m_model.LoadCharacterModel(
				m_modelType,                               //! モデルの種類。
				m_playerAnimation.GetAnimatiocClip(),      //! アニメーションの種類。
				m_playerAnimation.GetAnimationClips()      //! アニメーションの数。
			);
			m_model.SetCharacterScale(Vector3::One * CHARACTER_SCALE);

			/* 初期座標をセットする。*/
			m_model.SetPosition(POS);

			/* 角度をセットする。*/
			m_angle.AddRotationDegY(ANGLE_Y);
			m_model.SettRotation(m_angle);

			/* ステータスを初期化。*/
			InitAttackStatus();

			/* ステートを生成する。*/
			RegisterState();


			/* 
			 * ダミーモデル配置用。
			 * MPCの実装が完了後、削除可。
			 */
		    ///////////////////////////////////////////////////////////////////////////////////////
			if (IsMatchName("player2"))
				InitDummyModel();

			else
				m_stateMachine->ChangeState(m_stateFactory[PlayerStateID::enIdle]());

			//////////////////////////////////////////////////////////////////////////////////////////

			m_currentPosition = POS;
			m_characterController.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_currentPosition);
			m_model.SetPosition(m_currentPosition);

			SetWaitInputTimer(10);
			m_effectList.Init();

			/* 武器の当たり判定を設定。*/
			m_weaponHitDetection.Init(WEAPON_HIT_RADIUS);

			return true;
		}


		void Player::Update()
		{
			/* ICharacterクラスの更新処理をコール。*/
			ICharacter::Update();

			/* ヒットストップ状態なら*/
			if (IsHitStop())
				return;

			/*選択シーンではないとき。*/
			if (nsApp::nsScene::SceneLoader::GetInstance()->GetCurrentSceneID() != nsApp::IScene::enSceneID_Select)
			{
				/* ゲーム開始直後数フレームは入力を受け付けない*/
				/*@全体共有: その硬直はボス戦の開始演出でカバーする*/
				if (m_inputWaitTimer > 0)
				{
					m_inputWaitTimer--;
					m_playerInput.SetInputEnable(false);
				}
				else
					m_playerInput.SetInputEnable(true);
			}
			else/*選択シーンでは操作を受け付けないようにする。*/
			{
				m_playerInput.SetInputEnable(false);
			}


			/* モデルの更新より先に入力判定を更新する。*/
			m_playerInput.Update();

			/* リクエストを受け取って必要なステートをコール。*/
			if (m_stateMachine->GetCurrentState()->RequestID(m_currentStateID))
			{
				/* ステートの種類をキャストする。*/
				m_playerStateID = static_cast<PlayerStateID>(m_currentStateID);

				/* 登録されているステートならChangeStateに情報を渡す。*/
				if (m_stateFactory.count(m_playerStateID) > 0)
					m_stateMachine->ChangeState(m_stateFactory[m_playerStateID]());
			}

			/* ステートマシーンを更新する。*/
			m_stateMachine->Update();

			/* モデルの座標を更新する。*/
			m_model.SetPosition(m_currentPosition);

			/* モデルを更新する。*/
			m_model.Update();

			/* モデルの更新が終わった後に剣の当たり判定をテーブルに渡す。*/
			m_weaponHitDetection.Update(m_model.GetWeaponPosition());
		}


		void Player::Render(RenderContext& rc)
		{
			/* 描画。*/
			ICharacter::Render(rc);
		}


		void Player::InitAttackStatus()
		{
			/* 基本ダメージ数の初期化。*/
			m_characterStatus.attack.normalDamage = 100.0f;	

			/* クリティカル率の初期化。*/ 
			m_characterStatus.attack.criticalRate = 0.1f;

			/* クリティカルダメージの初期化。*/
			m_characterStatus.attack.criticalDamage = 2.0f;
		}


		void Player::InitDummyModel()
		{
			/* 座標をメインプレイヤーからずらす。*/
			m_currentPosition.x -= 150.0f;

			/* HPを0にしておく。*/
			m_characterStatus.hp.currentHP = 0;

			/* 最初から死亡ステートへ遷移させる。*/
			m_stateMachine->ChangeState(m_stateFactory[PlayerStateID::enIdle]());
		}


		void Player::PlayBasicAnimation(CharacterBasicAnimationList state)
		{
			int animIndex = m_playerAnimation.GetBasicAnimationIndex(state);
			m_model.PlayAnimation(animIndex, 0.2f);
		}


		void Player::PlayWeaponAnimation(AttackType attack)
		{
			/* 攻撃アニメーションの数を取得。*/
			animIndex = m_playerAnimation.GetAttackAnimationIndex(attack);
			/* 攻撃アニメーションはボタンを押した瞬間に切り替わってほしいため補完割合を低めに設定。*/
			m_model.PlayAnimation(animIndex, 0.0f);

			/* 古いSEが残っているなら強制停止する。*/
		//	StopWeaponSE();

			/* --- ここからSE再生処理 --- */
			/* サウンド管理クラスを探す */
			auto soundManager = FindGO<nsSound::SoundLister>("SoundManager");
			if (soundManager != nullptr && reinterpret_cast<uint8_t>(soundManager) != 0xFFFFFFFFFFFFFFFF)
				m_currentWeaponSE = soundManager->GetSEList().PlayAttackSE(m_currentWeapon, attack);
		}


		void Player::ReceiveHelp()
		{
			m_characterStatus.hp.currentHP = 1000;

			/* 自分のHPを回復させる。*/
			/* 起き上がりステート（PlayerGetUpState）へ強制移行。*/
			m_stateMachine->ChangeState(m_stateFactory[PlayerStateID::enGetUp]());
		}


		nsActor::Player* Player::SearchCharacter()
		{
			auto target = FindGO<nsActor::Player>("player2");
			if (target != nullptr && reinterpret_cast<uintptr_t>(target) != 0xFFFFFFFFFFFFFFFF)
			{
				if (target->GetCharacterStatus().hp.currentHP <= 0)
				{
					Vector3 diff = GetPosition() - target->GetPosition();
					if(diff.Length() < 150.0f)
						return target;
				}
			}
			return nullptr;
		}


		void Player::RegisterState()
		{
			/* 待機状態。*/
			m_stateFactory[PlayerStateID::enIdle] = []() { return new nsState::PlayerIdleState(); };

			/* 歩行状態。*/
			m_stateFactory[PlayerStateID::enWalk] = []() { return new nsState::PlayerWalkState(); };

			/* ジャンプ状態。*/
			m_stateFactory[PlayerStateID::enJump] = []() { return new nsState::PlayerJumpState(); };

			/* 走り状態。*/
			m_stateFactory[PlayerStateID::enRun] = []() { return new nsState::PlayerRunState(); };

			/* ダメージ状態。*/
			m_stateFactory[PlayerStateID::enHit] = []() { return new nsState::PlayerHitState(); };

			/* 死亡状態。*/
			m_stateFactory[PlayerStateID::enDeath] = []() { return new nsState::PlayerDethState(); };

			/* ガード状態。*/
			m_stateFactory[PlayerStateID::enGuard] = []() { return new nsState::PlayerGuardState(); };

			/* 復活状態。*/
			m_stateFactory[PlayerStateID::enHelp] = []() { return new nsState::PlayerReBoneState(); };

			/* 助けられ状態。*/
			m_stateFactory[PlayerStateID::enGetUp] = []() { return new nsState::PlayerGetUpState(); };
		}
	}
}
