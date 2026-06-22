#include "stdafx.h"
#include "Player.h"
#include "Src/Parameter/PlayerStatusParameterTable.h"
#include "Src/UI/Character/CharacterHP.h"
#include "Src/SceneLoader/SceneLoader.h"
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
#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Character/NPC/State/BasicState/NPCHelpState.h"
#include "Src/Utilty/ResourceUtility.h"
#include "Src/Direction/GameStartDirection.h"
#include "Src/Actor/Character/Player/InputSystem/VirtualInputAdapter.h"

namespace
{
	const auto CHARACON_RADIUS = 12.5f;                 //! キャラクターコントローラーの半径。
	const auto CHARACON_HEIGHT = 30.0f;                 //! キャラクターコントローラーの高さ。
	const auto WEAPON_HIT_RADIUS = 40.0f;               //! 武器の当たり判定の半径。
	const auto ANGLE_Y = 90.0f;                         //! プレイヤーの初期角度。
	const auto CHARACTER_SCALE = 0.5f;                  //! プレイヤーのスケール。

	const Vector3 POS = Vector3(0.0f, 100.0f, 0.0f);    //! プレイヤーの初期座標。

	/* カメラクランプ。*/
	constexpr float BATTLE_MIN_X = -300.0f;				//! カメラのクランプ範囲（最小X）
	constexpr float BATTLE_MAX_X = 260.0f;				//! カメラのクランプ範囲（最大X）
	constexpr float BATTLE_MIN_Z = -100.0f;				//! カメラのクランプ範囲（最小Z）
	constexpr float BATTLE_MAX_Z = 100.0f;;				//! カメラのクランプ範囲（最大Z）

	/**
	 * @brief 値を最小値と最大値の範囲内にクランプする関数。
	 * @param value クランプする値。
	 * @param minValue 最小値。
	 * @param maxValue 最大値。
	 * @return クランプされた値。
	 */
	auto ClampValue(float value, float minValue, float maxValue)
	{
		if (value < minValue)
			return minValue;

		if (value > maxValue)
			return maxValue;

		return value;
	}
}

namespace nsApp
{
	namespace nsActor
	{
		Player::~Player()
		{
			/* NPCの脳を削除する。*/
			if (m_brain != nullptr)
				delete m_brain;
		}


		bool Player::Start()
		{
			/* アニメーションとモデルを準備する。*/
			/* アニメーションクラスの初期化処理をコール。*/
			m_playerAnimation.Initialize(m_currentWeapon);
			/* 今の武器をセットする。*/
			m_playerAnimation.LoadAnimation(m_currentWeapon);

			/* モデルの種類/アニメーションの種類/アニメーションの数をセットする。*/
			m_model.LoadCharacterModel(
				m_modelType,                               //! モデルの種類。
				m_playerAnimation.GetAnimatiocClip(),      //! アニメーションの種類。
				m_playerAnimation.GetAnimationClips()      //! アニメーションの数。
			);

			/* モデルの大きさをセットする。*/
			m_model.SetCharacterScale(Vector3::One * CHARACTER_SCALE);

			/* 角度をセットする。*/
			m_angle.AddRotationDegY(ANGLE_Y);
			m_model.SettRotation(m_angle);

			/* ステータスを初期化。*/
			InitAttackStatus();

			/* ステートを生成する。*/
			RegisterState();

			/* 
				ローディング中は当たり判定を作らない。
				2026/06/20追記: 出撃時のクラッシュの原因となっていました。
			*/
			if(nsScene::SceneLoader::GetInstance() ->GetCurrentSceneID() != IScene::enSceneID_Select)
				m_characterController.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_currentPosition);

			m_model.SettRotation(m_angle * m_postureOffset);
			m_model.SetPosition(m_currentPosition);

			SetWaitInputTimer(10);
			m_effectList.Init();

			/* 武器の当たり判定を設定。*/
			m_weaponHitDetection.Init(WEAPON_HIT_RADIUS);

			m_forwardVector = Vector3::Right;

			/* エフェクトリスナーを初期化。*/
			m_effectListener.Initialize(this);
			m_gunShooter.Subscribe(&m_effectListener);

			/* ステートマシーンを初期化する。*/
			m_stateMachine->ChangeState(m_stateFactory[PlayerStateID::enIdle]());
			return true;
		}


		void Player::Update()
		{
			/* ローディングの描画用の表示モード。*/
			if (m_isLoadingPreview)
			{
				/*
					Loading中は入力・NPC思考・ステート更新を行わず、
					モデルとアニメーションだけ更新する。
				*/
				if (!m_isLoadingPreviewRunStarted)
				{
					PlayBasicAnimation(CharacterBasicAnimationList::Run);
					m_isLoadingPreviewRunStarted = true;
				}
				m_model.SettRotation(m_angle);
				m_model.SetPosition(m_currentPosition);
				m_model.Update();
				m_model.UpdateWorldOnly();
				return;
			}

			/* ヒットストップタイマー。*/
			UpdateHitStioTImer();

			/* 無敵時間タイマーを加算。*/
			m_damageInvincibilitySystem.Update(g_gameTime->GetFrameDeltaTime());

			/* すり抜け判定。*/
			if (!m_isIgnorePlayerSet)
				ComputeSlipThrough();

			/* ヒットストップ状態なら*/
			if (IsHitStop())
				return;

			/* 開始演出中（GO! まで）は動かない。*/
			if (auto* startDir = FindGO<nsGame::GameStartDirection>("gameStartDirection"))
			{
				if (!startDir->IsDirectionFinished())
				{
					if (m_brain != nullptr && m_isNpcControlled)
					{
						auto* vInput = m_brain->GetVirtualInputAdapter();
						if (vInput != nullptr)
							vInput->Reset();
					}
					m_playerInput.SetInputEnable(false);
					m_model.SettRotation(m_angle * m_postureOffset);
					m_model.SetPosition(m_currentPosition);
					m_model.Update();
					return;
				}
			}



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
				m_playerInput.SetInputEnable(false);

			/* NPC 操作キャラだけ Brain を動かす。*/
			if (m_brain != nullptr && m_isNpcControlled)
				m_brain->Update();

			/* モデルの更新より先に入力判定を更新する。*/
			m_playerInput.Update();

			/* 死亡判定を検出する。 */
			CheckDeth();

			if (IsDeath())
			{
				m_playerInput.SetInputEnable(false);
				m_stateMachine->Update();
				m_model.SettRotation(m_angle * m_postureOffset);
				m_model.SetPosition(m_currentPosition);
				m_model.Update();

				return;
			}

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

			/* 角度を更新。*/
			m_model.SettRotation(m_angle * m_postureOffset);
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
			/* TSVから武器種別ごとのステータスを取得。*/
			const auto& param = PlayerStatusParameterTable::GetParameter(m_currentWeapon);
			m_characterStatus.attack.normalDamage = param.normalDamage;
			m_characterStatus.attack.criticalRate = 0.05f;
			m_characterStatus.attack.criticalDamage = param.criticalDamage;
			m_characterStatus.hp.maxHP = param.maxHP;
			m_characterStatus.hp.currentHP = m_characterStatus.hp.maxHP;

			/* 歩き・走りは従来どおり固定（TSVのWalkSpeed/RunSpeedは倍率想定のため）。*/
			m_walkSpeed = 50.0f;
			m_runSpeed = 120.0f;
			m_jumpPower = param.jumpPower;
			m_airMoveSpeed = param.airMoveSpeed;
			m_gravity = param.gravity;
			m_maxFallVelocity = param.maxFallVelocity;

			/* ガードパラメータの取得。*/
			m_guardSystem.Initialize(m_currentWeapon);

			/* 無敵時間の付与。*/
			m_damageInvincibilitySystem.Initialize(m_currentWeapon);
		}


		void Player::ComputeSlipThrough()
		{
			// 自分の剛体ができているか確認。
			auto* myBody = m_characterController.GetRigidBody()->GetBody();
			if (myBody == nullptr) return;

			// 検索する全プレイヤーの名前リスト
			const char* playerNames[] = { "player1", "player2", "player3", "player4" };

			for (const char* name : playerNames)
			{
				auto otherPlayer = FindGO<nsActor::Player>(name);

				// 相手がまだいない場合や、自分自身の場合はスキップする
				if (otherPlayer == nullptr || otherPlayer == this)
					continue;

				// 相手の剛体を取得
				auto* otherBody = otherPlayer->GetCharacterController().GetRigidBody()->GetBody();

				// 相手の剛体も確実に存在していれば設定
				if (otherBody != nullptr)
				{
					// お互いに無視リストへ登録
					myBody->setIgnoreCollisionCheck(otherBody, true);
					otherBody->setIgnoreCollisionCheck(myBody, true);
				}
			}

			m_isIgnorePlayerSet = true;
		}


		Vector3 Player::ClampBattleAreaMoveVector(const Vector3& moveVector, float frameTime) const
		{
			Vector3 currentPos = m_currentPosition;

			// moveVector は速度なので、frameTime を掛けて「今回の移動量」に変換する。
			Vector3 moveDelta = moveVector * frameTime;
			Vector3 nextPos = currentPos + moveDelta;

			Vector3 fixedMoveDelta = moveDelta;

			if (nextPos.x < BATTLE_MIN_X)
				fixedMoveDelta.x = BATTLE_MIN_X - currentPos.x;

			else if (nextPos.x > BATTLE_MAX_X)
				fixedMoveDelta.x = BATTLE_MAX_X - currentPos.x;

			if (nextPos.z < BATTLE_MIN_Z)
				fixedMoveDelta.z = BATTLE_MIN_Z - currentPos.z;

			else if (nextPos.z > BATTLE_MAX_Z)
				fixedMoveDelta.z = BATTLE_MAX_Z - currentPos.z;

			// CharacterController::Execute は速度ベクトルを受け取っているため、
			// 補正後の移動量を速度に戻す。
			if (frameTime > 0.0f)
				return fixedMoveDelta / frameTime;

			return Vector3::Zero;
		}


		void Player::MoveWithBattleClamp(const Vector3& moveVector, float frameTime)
		{
			Vector3 fixedMoveVector = ClampBattleAreaMoveVector(moveVector, frameTime);

			m_characterController.Execute(fixedMoveVector, frameTime);

			// キャラコンの結果をPlayer本体の座標へ反映する。
			SetPosition(m_characterController.GetPosition());
		}


		void Player::CheckDeth()
		{
			/* 早期リターン。*/
			if (m_isDown)
				return;

			/* HPが0じゃないかチェック。*/
			if (m_characterStatus.hp.currentHP > 0)
				return;

			/* Player専用のダウンフラグをセットする。
			 * IGameObject側の死亡/削除フラグと混同しない。
			 */
			m_isDown = true;

			/* 死亡した瞬間の座標をキャラコン基準で固定する。*/
			const Vector3 deathPosition = m_characterController.GetPosition();
			m_currentPosition = deathPosition;
			m_model.SetPosition(m_currentPosition);

			/* Dethステートに遷移。*/
			m_stateMachine->ChangeState(new nsState::PlayerDethState(deathPosition));


			/* 死亡時の処理。*/
			/* 入力をオフ。*/
			m_playerInput.SetInputEnable(false);
			/* 当たり判定をオフ。*/
			m_weaponHitDetection.Disable();
			/* ダウンカウントを加算する。*/
			m_rescueStatusManager.AddDownCount();
			/* 武器SEを止める。*/
			StopWeaponSE();
			/* サブ武器の描画をオフにする。*/
			ResetSubWeapon();
		}


		void Player::SetLoadingPreviewTransform(const Vector3& position, float angleY, const Vector3& scale)
		{
			/* ローディング表示用の座標を同期する。*/
			m_currentPosition = position;


			/* ローディング表示用の回転を同期する。*/
			m_angle = Quaternion::Identity;
			m_angle.SetRotationDegY(angleY);

			/* 大きさを同期する。*/
			m_model.SetCharacterScale(scale);
			m_model.SetWeaponScale(scale);

			/*
				ローディング表示では本編用の姿勢補正を使わない。
			*/
			m_model.SettRotation(m_angle);
			m_model.SetPosition(m_currentPosition);

			/*
				LoadingRandomRunnerModel側から座標を更新した直後に、
				モデルのワールド行列も即反映する。
				これをしないと、Player::Updateの後に座標だけ変わって、
				描画に使われる行列が前フレームのままになる。
			*/
			m_model.UpdateWorldOnly();
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

			/* --- ここからSE再生処理 --- */
			/* サウンド管理クラスを探す */
			auto soundManager = FindGO<nsSound::SoundLister>("SoundManager");
			if (soundManager != nullptr && reinterpret_cast<uint8_t>(soundManager) != 0xFFFFFFFFFFFFFFFF)
				m_currentWeaponSE = soundManager->GetSEList().PlayAttackSE(m_currentWeapon, attack);
		}


		void Player::ReceiveHelp()
		{
			/* ダウンフラグを解除する。*/
			m_isDown = false;

			/* ダウンカウントをリセットする。*/
			m_rescueStatusManager.ResetRescueStatus();

			/* 最大HPを参照する。*/
			m_characterStatus.hp.currentHP = m_characterStatus.hp.maxHP;

			/* 蘇生直後に HPBarUI を満タン同期する。*/
			if (auto* characterHP = FindGO<nsGame::CharacterHP>("characterHP"))
				characterHP->SyncPlayerHP(this);

			/* 起き上がりステート（PlayerGetUpState）へ強制移行。*/
			m_stateMachine->ChangeState(m_stateFactory[PlayerStateID::enGetUp]());
		}


		bool Player::TryBeginHelpToTarget(nsActor::Player* target)
		{
			/* NPC 以外、または救助対象が無い場合は開始しない。*/
			if (m_brain == nullptr || target == nullptr)
				return false;

			/* 自分がダウン中は救助できない。*/
			if (IsDeath() || GetCharacterStatus().hp.currentHP <= 0)
				return false;

			/* 自分自身は救助対象にできない。*/
			if (target == this)
				return false;

			/* 救助対象がダウンしていない場合は開始しない。*/
			if (!target->IsDeath() && target->GetCharacterStatus().hp.currentHP > 0)
				return false;

			/* 既に救助中ならそのまま継続。*/
			if (m_playerStateID == PlayerStateID::enHelp)
				return true;

			/* 救助ステートへ直接遷移する。*/
			auto* helpState = static_cast<nsState::PlayerReBoneState*>(m_stateFactory[PlayerStateID::enHelp]());
			helpState->SetTargetCharacter(target);

			m_playerStateID = PlayerStateID::enHelp;
			m_currentStateID = static_cast<uint8_t>(PlayerStateID::enHelp);
			m_stateMachine->ChangeState(helpState);

			return true;
		}


		bool Player::TryBeginHeelMagic()
		{
			/* NPC 以外は Brain 経由で呼ばない想定。*/
			if (m_brain == nullptr)
				return false;

			/* 杖以外は回復不可。*/
			if (GetCurrentWeapon() != WeaponType::Wand)
				return false;

			/* ダウン中は詠唱不可。*/
			if (IsDeath() || GetCharacterStatus().hp.currentHP <= 0)
				return false;

			/* 救助中は回復より救助優先。*/
			if (m_playerStateID == PlayerStateID::enHelp)
				return false;

			/* 既に詠唱中ならそのまま継続。*/
			if (m_playerStateID == PlayerStateID::enHeelMagic)
				return true;

			/* チャージ省略。最低レベル 1 で HeelMagic が発動する。*/
			SetChargeLevel(1);

			/* 回復魔法ステートへ直接遷移する。*/
			m_playerStateID = PlayerStateID::enHeelMagic;
			m_currentStateID = static_cast<uint8_t>(PlayerStateID::enHeelMagic);
			m_stateMachine->ChangeState(m_stateFactory[PlayerStateID::enHeelMagic]());

			return true;
		}


		nsActor::Player* Player::SearchCharacter()
		{
			return ResourceUtility::SearchNearestDownCharacter(this, 120.0f);
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

			/* ガード状態。*/
			m_stateFactory[PlayerStateID::enGuard] = []() { return new nsState::PlayerGuardState(); };

			/* 復活状態。*/
			m_stateFactory[PlayerStateID::enHelp] = []() { return new nsState::PlayerReBoneState(); };

			/* 助けられ状態。*/
			m_stateFactory[PlayerStateID::enGetUp] = []() { return new nsState::PlayerGetUpState(); };
		}


		void Player::ForceBlowAway(float knockbackVelocity, float dirX)
		{
			/* すでに死亡している場合は吹き飛ばさない。*/
			if (m_currentStateID == static_cast<uint8_t>(PlayerStateID::enDeath))
				return;

			/* ガード中でノックバック無効なら吹き飛ばさない。*/
			if (GetGuardSystem().BlocksKnockback())
				return;

			/* 無敵時間中でノックバック無効なら吹き飛ばさない。*/
			if (m_damageInvincibilitySystem.BlocksKnockback())
				return;

			/* すでにノックバック状態なら吹き飛ばさない。*/
			if (IsInKnockBackState())
				return;


			/* ノックバック状態に遷移する。*/
			auto* hitState = static_cast<nsState::PlayerHitState*>( m_stateFactory[PlayerStateID::enHit]());

			/* ノックバックの初速をセットする。*/
			hitState->SetKnockBackVelocity(knockbackVelocity);
			hitState->SetKnockBackSpeed(Vector3(dirX * 100.0f, 50.0f, 0.0f));
			hitState->SetHitTimer(90);
			hitState->SetGetUpFlag(false);  
			m_playerStateID = PlayerStateID::enHit;
			m_currentStateID = static_cast<uint8_t>(PlayerStateID::enHit);
			m_stateMachine->ChangeState(hitState);

		}


		void Player::ForceGetUp()
		{
			/* ダウンフラグを解除する。*/
			if (m_currentStateID == static_cast<uint8_t>(PlayerStateID::enDeath))
				return;

			/* ダウンフラグを解除する。*/
			m_currentStateID = static_cast<uint8_t>(PlayerStateID::enIdle);

			/* ダウンカウントをリセットする。*/
			m_stateMachine->ChangeState(m_stateFactory[PlayerStateID::enIdle]());
		}	
	}
}
