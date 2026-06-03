#include "stdafx.h"
#include "MagicProjectotile.h"
#include "Src/Actor/Character/Common/Damage/DamageProcessor.h"
#include "Boss.h"

#include "Src/Effect/EffectList.h"

namespace
{
	/* スケール関連 */
	const auto RUSH_MAGIC_MAX_SCALE = 1.5f;     //! 連打魔法の最大スケール。
	const auto SCALE_GROWTH_RATE = 0.02f;       //! スケールの拡大スピード。

	/* 追尾・座標関連 */
	const auto AIM_HEIGHT_OFFSET = 10.0f;       //! ターゲットの足元から少し上を狙うオフセット。
	const auto HOMING_TURN_RATE = 0.08f;        //! 追尾時の旋回性能。

	/* システム・計算用 */
	const auto BASE_FPS = 60.0f;                //! 基準となるフレームレート。
	const auto VECTOR_EPSILON = 0.001f;         //! ゼロ除算回避のための極小値。

	/* ボス・当たり判定関連 */
	const auto BOSS_AIM_HEIGHT_OFFSET = 50.0f;  //! ボスの中心座標（狙う位置）の高さ調整
	const auto BOSS_HIT_RADIUS = 150.0f;        //! すり抜け防止用の当たり判定の広さ（半径）

	/* エフェクト関連 */
	const auto HIT_EFFECT_OFFSET_Y = 10.0f;     //! ヒットエフェクトを出す高さの調整
	const auto HIT_EFFECT_SCALE = 8.0f;         //! ヒットエフェクトの大きさ

	/* システム関連 */
	const auto HIDE_POSITION_Y = -100000.0f;    //! 無効化時に画面外（はるか下）へ飛ばすY座標
}

namespace nsApp
{
	namespace nsActor
	{
		MagicProjectotile::~MagicProjectotile()
		{
			if (m_magicCollider != nullptr)
				DeleteGO(m_magicCollider);
		}


		void MagicProjectotile::Initialize(MagicType type, const Vector3& spawnPosition, const Vector3& forwardDirection, const MagicParameter& param)
		{
			m_isInUse = true;
			m_target = nullptr;
			m_magicType = type;
			m_position = spawnPosition;
			m_previousPosition = spawnPosition;


			/* 寿命と速度を初期化（追尾計算で使うため保存）*/
			m_currentLifeTime = param.lifeTime;
			m_moveSpeed = param.speedPerSecond;
			m_velocity = forwardDirection * m_moveSpeed;

			/* モデルの大きさを初期化 */
			m_scale = param.scale;

			/* モデルの回転方向を初期化 */
			m_direction.SetRotation(Vector3::Front, forwardDirection);
			m_angle = m_direction * param.angle;

			/* ダメージ量を初期化 */
			SetDamage(param.damage);

			/* 各ミサイルモデルのパスを初期化 */
			if (!m_isModelInitialized)
			{
				m_missileModel = std::make_unique<ModelRender>();
				m_missileModel->Init(param.modelPath.c_str());
				m_isModelInitialized = true;
			}

			/* ミサイルの位置と回転を初期化 */
			if (m_missileModel != nullptr)
			{
				m_missileModel->SetScale(m_scale);
				m_missileModel->SetRotation(m_angle);
				m_missileModel->SetPosition(m_position);
				m_missileModel->Update();
			}

			/* ミサイルの当たり判定を初期化 */
			if (m_magicCollider == nullptr)
			{
				m_magicCollider = NewGO<nsK2Engine::CollisionObject>(0, "MagicCollision");
				m_magicCollider->CreateSphere(m_position, m_angle, param.radius);
				m_magicCollider->SetIsEnableAutoDelete(false);
			}
			else
				m_magicCollider->SetPosition(m_position);
		}


		void MagicProjectotile::Update()
		{
			if (!m_isInUse)
				return;

			m_previousPosition = m_position;

			/* フレーム間の時間を取得 */
			float deltaTime = g_gameTime->GetFrameDeltaTime();

			/* 位置を更新する（超高速にならないよう deltaTime を掛ける！）*/
			m_position += m_velocity * deltaTime;

			/* 寿命を秒数で減らして消滅処理 */
			m_currentLifeTime -= deltaTime;
			if (m_currentLifeTime <= 0.0f)
			{
				Deactivate();
				return;
			}

			/* 連打魔法の場合、発動 */
			if (m_magicType == MagicType::enRushMagic)
				TargetMoving();

			/* 当たり判定の追従 */
			if (m_magicCollider != nullptr)
				m_magicCollider->SetPosition(m_position);

			/* ミサイルの位置と回転を更新 */
			if (m_missileModel != nullptr)
			{
				m_missileModel->SetRotation(m_angle);
				m_missileModel->SetScale(m_scale);
				m_missileModel->SetPosition(m_position);
				m_missileModel->Update();
			}
			if (CheckHitBoss())
			{
				Deactivate();
				return;
			}
		}


		void MagicProjectotile::Render(RenderContext& rc)
		{
			/* ミサイルの描画 */
			if (!m_isInUse)
				return;

			/* ミサイルのモデルが存在する場合、描画する */
			if (m_missileModel != nullptr)
				m_missileModel->Draw(rc);
		}


		void MagicProjectotile::TargetMoving()
		{
			/* フレーム間の時間を取得 */
			float deltaTime = g_gameTime->GetFrameDeltaTime();

			/* 連打魔法のスケールを徐々に大きくする */
			if (m_scale.x < RUSH_MAGIC_MAX_SCALE)
				m_scale += Vector3::One * (SCALE_GROWTH_RATE * BASE_FPS * deltaTime);

			/* ターゲットが存在する場合、追尾計算を行う */
			if (m_target != nullptr)
			{
				/* ターゲットの位置を取得し、少し上にオフセットする */
				m_targetPosition = m_target->GetPosition();
				m_targetPosition.y += AIM_HEIGHT_OFFSET;

				/* ターゲットへのベクトルを計算 */
				m_toTargetVector = m_targetPosition - m_position;

				/* ターゲットへのベクトルの長さが十分に大きい場合、追尾計算を行う */
				if (m_toTargetVector.LengthSq() > VECTOR_EPSILON)
				{
					/* ターゲットへのベクトルを正規化して、現在の移動方向と比較する */
					m_toTargetVector.Normalize();
					m_currentDirection = m_velocity;

					/* 現在の移動方向とターゲットへのベクトルを線形補間して、新しい移動方向を計算する */
					if (m_currentDirection.Length() > VECTOR_EPSILON)
					{
						/* 現在の移動方向を正規化して、線形補間を行う前に単位ベクトルにする */
						m_currentDirection.Normalize();

						/* 線形補間を行い、新しい移動方向を計算する */
						m_newPosition.Lerp(HOMING_TURN_RATE * BASE_FPS * deltaTime, m_currentDirection, m_toTargetVector);
						m_newPosition.Normalize();
						m_velocity = m_newPosition * m_moveSpeed; // 秒間速度を掛け直す
						m_direction.SetRotation(Vector3::Front, m_newPosition);
					}
				}
			}
		}


		bool MagicProjectotile::CheckHitBoss()
		{
			/* コライダーがない場合は判定しない */
			if (m_magicCollider == nullptr)
				return false;

			/* ボスが存在するか（ポインタが有効か）チェック */
			auto* boss = FindGO<nsActor::Boss>("boss");
			if (boss == nullptr || reinterpret_cast<uintptr_t>(boss) == 0xFFFFFFFFFFFFFFFF)
				return false;

			/* ダメージ処理をまとめたラムダ式。*/
			auto applyDamageToBoss = [this, boss]() {
				DamageProcessor::ApplyDamageToTarget(boss, static_cast<int>(m_damage));
				};

			/* 判定：コライダー同士の単純なヒット判定*/
			if (m_magicCollider->IsHit(boss->GetController()))
			{
				applyDamageToBoss();
				PlayHitEffect(m_position);
				return true;
			}

			/* すり抜け対策。*/
			m_bossPosition = boss->GetPosition();
			m_bossPosition.y += BOSS_AIM_HEIGHT_OFFSET;

			/* 1フレーム前の位置から今の位置へのベクトル（弾が動いた軌跡） */
			m_missileTrajectory = m_position - m_previousPosition;
			/* 1フレーム前の位置からボスへのベクトル */
			m_vectorToBossTarget = m_bossPosition - m_previousPosition;
			m_trajectoryLengthSquared = m_missileTrajectory.LengthSq();

			/* 弾がちゃんと移動している場合のみ計算する */
			if (m_trajectoryLengthSquared > VECTOR_EPSILON)
			{
				/* 軌跡の長さに対する、ボスまでの投影距離の割合（内積で計算） */
				m_closestPointRatio = m_vectorToBossTarget.Dot(m_missileTrajectory) / m_trajectoryLengthSquared;

				/* 割合が0.0〜1.0の間なら、弾の動いた線分の真横にボスがいるということ */
				if (m_closestPointRatio >= 0.0f && m_closestPointRatio <= 1.0f)
				{
					/* 軌跡の線上で一番ボスに近い座標を割り出す */
					m_closestPointOnTrajectory = m_previousPosition + (m_missileTrajectory * m_closestPointRatio);

					/* その一番近い点とボスの実際の距離を測る */
					m_distanceToBoss = (m_bossPosition - m_closestPointOnTrajectory).Length();

					/* 距離が当たり判定の半径より小さければ「かすった（当たった）」とみなす！ */
					if (m_distanceToBoss < BOSS_HIT_RADIUS)
					{
						applyDamageToBoss();
						// すり抜けて当たったので、今の位置ではなく軌跡上の当たった地点で爆発させる
						PlayHitEffect(m_closestPointOnTrajectory);
						return true;
					}
				}
			}

			return false; 
		}


		void MagicProjectotile::Deactivate()
		{
			/* 無効化フラグを立てる */
			m_isInUse = false;

			/* 各種パラメータを初期化して、画面外に飛ばす */
			m_currentLifeTime = 0.0f;
			m_velocity = Vector3::Zero;
			m_target = nullptr;
			m_effectList = nullptr;
			
			/* 画面外に飛ばす */
			m_position = Vector3(0.0f, HIDE_POSITION_Y, 0.0f);
			m_previousPosition = m_position;

			/* 当たり判定も画面外に飛ばす */
			if (m_magicCollider != nullptr)
				m_magicCollider->SetPosition(m_position);
		}


		void MagicProjectotile::PlayHitEffect(const Vector3& position)
		{
			if (m_effectList == nullptr)
				return;

			/* エフェクトを再生する座標を設定。*/
			m_effectPosition = position;
			m_effectPosition.y += 10.0f;

			/* エフェクトを設定。*/
			m_effectList->PlayEffect(nsEffect::Hit,
				m_effectPosition,
				Quaternion::Identity,
				Vector3::One * 8.0f);
		}
	}
}