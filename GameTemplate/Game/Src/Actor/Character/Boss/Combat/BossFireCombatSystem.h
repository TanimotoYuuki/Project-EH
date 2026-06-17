#pragma once
/**
 * @file   BossFireCombatSystem.h
 * @brief  ボスの火球・Burst・Rain・着弾炎柱を管理するクラス。
 * @date   2026/06/16
 */

#include "Src/Actor/Character/Common/WeaponHitDetection.h"
#include "Src/Effect/EffectList.h"

namespace nsApp
{
	namespace nsActor
	{
		class Boss;

		/**
		 * @enum FirePattern
		 * @brief 火炎攻撃のパターン種別。
		 */
		enum class FirePattern : uint8_t
		{
			Straight, //! 単発の直線火球。
			Burst,    //! 3連射。
			Rain,     //! 降り注ぎ攻撃。
		};

		/**
		 * @struct RainDrop
		 * @brief Rain 攻撃で使用する 1 滴分の火球データ。
		 */
		struct RainDrop
		{
			bool active = false;                              //! 使用中かどうか。
			Vector3 pos = Vector3::Zero;                      //! 論理座標。
			Vector3 velocity = Vector3::Zero;                 //! 移動速度。
			nsK2EngineLow::EffectEmitter* emitter = nullptr;  //! 表示用エフェクト。
		};

		/**
		 * @class BossFireCombatSystem
		 * @brief 火球系攻撃の生成・更新・着弾処理を Boss から委譲する。
		 */
		class BossFireCombatSystem
		{
		public:
			/** @brief ヒット判定の初期化。Boss::Start() から呼ぶ。 */
			void Init();

			/**
			 * @brief 毎フレームの火球系更新処理。
			 * @param boss 更新対象のボス。
			 */
			void Update(Boss* boss);

			/**
			 * @brief 火球攻撃を開始する（FireAttackStrategy から呼ばれる）。
			 * @param boss 攻撃主体のボス。
			 */
			void ShotFireBall(Boss* boss);

			/**
			 * @brief 着弾エフェクトの更新とヒット判定処理。
			 * @return 着弾エフェクトが終了したかどうか。
			 */
			inline WeaponHitDetection& GetFireHit()
			{
				return m_FireHit;
			}


		private:
			/**
			 * @brief 火球の生成処理。FireAttackStrategy::ShotFireBall() から呼ばれる。
			 * @param boss 攻撃主体のボス。
			 */
			void CreateFireball(Boss* boss);

			/**
			 * @brief 火球の削除処理。火球の寿命が尽きたときや、着弾したときに呼ばれる。
			 * @param boss 攻撃主体のボス。
			 */
			void DeleteFireball(Boss* boss);

			/**
			 * @brief Burst 攻撃の開始処理。BurstAttackStrategy::StartBurst() から呼ばれる。
			 */
			void StartRain();

			/**
			 * @brief Rain 攻撃の更新処理。Boss::Update() から呼ばれる。
			 * @param boss 更新対象のボス。
			 */
			void UpdateRain(Boss* boss);

			/**
			 * @brief Rain 攻撃のスポーン処理。Boss::Update() から呼ばれる。
			 * @param boss スポーン位置の計算に必要なボスの情報を渡すために引数として受け取る。
			 */
			void SpawnRainDrop(Boss* boss);

			/**
			 * @brief Rain 攻撃の削除処理。Rain 攻撃の持続時間が尽きたときや、着弾したときに呼ばれる。
			 * @param boss 攻撃主体のボス。Rain 攻撃の状態をリセットするために引数として受け取る。
			 */
			void UpdateRainDrops(Boss* boss);

			/**
			 * @brief 火球の着弾エフェクトの更新とヒット判定処理。Boss::Update() から呼ばれる。
			 * @param boss 着弾エフェクトの位置計算に必要なボスの情報を渡すために引数として受け取る。
			 */
			void UpdateLandingEffect(Boss* boss);

			/**
			 * @brief 火球の着弾エフェクトのヒット判定処理。Boss::Update() から呼ばれる。
			 * @param boss ヒット判定の対象となるプレイヤーの位置計算に必要なボスの情報を渡すために引数として受け取る。
			 */
			void CheckLandingHitDamage(Boss* boss);

			/**
			 * @brief Burst 攻撃の更新処理。Boss::Update() から呼ばれる。
			 * @param boss 更新対象のボス。Burst 攻撃の状態を管理するために引数として受け取る。
			 * @param deltaTime 前フレームからの経過時間。Burst 攻撃のタイミング管理に使用するために引数として受け取る。
			 */
			void UpdateBurst(Boss* boss, float deltaTime);

			/**
			 * @brief Rain Drop 着弾時の共通処理。
			 * @param drop 着弾した Rain Drop。
			 */
			void LandRainDrop(RainDrop& drop);

			/**
			 * @brief 火球の生成位置を計算する。Boss::Update() から呼ばれる。
			 * @param boss 生成位置の計算に必要なボスの情報を渡すために引数として受け取る。
			 * @return 火球の生成位置。
			 */
			Vector3 GetMouthPosition(Boss* boss) const;

			/**
			 * @brief 火球のエフェクト表示位置を計算する。Boss::Update() から呼ばれる。
			 * @param logicPos 火球の論理座標。エフェクト表示位置の計算に必要な情報を渡すために引数として受け取る。
			 * @return 火球のエフェクト表示位置。
			 */
			Vector3 GetFireballEffectPosition(const Vector3& logicPos) const;

			/**
			 * @brief 着弾エフェクトの表示位置を計算する。Boss::Update() から呼ばれる。
			 * @param hitPos 着弾位置。エフェクト表示位置の計算に必要な情報を渡すために引数として受け取る。
			 * @return 着弾エフェクトの表示位置。
			 */
			Vector3 GetLandingEffectPosition(const Vector3& hitPos) const;

			/**
			 * @brief 着弾炎柱ヒット判定用の論理座標を返す。
			 * @return 着弾炎柱ヒット判定座標。
			 */
			Vector3 GetLandingHitPosition() const;

			/**
			 * @brief 火球の照準方向を計算する。Boss::Update() から呼ばれる。
			 * @param boss 照準方向の計算に必要なボスの情報を渡すために引数として受け取る。
			 * @param fromPos 照準開始位置。照準方向の計算に必要な情報を渡すために引数として受け取る。
			 * @param toPos 照準目標位置。照準方向の計算に必要な情報を渡すために引数として受け取る。
			 * @return 照準方向の単位ベクトル。
			 */
			Vector3 ComputeAimDirection(Boss* boss, const Vector3& fromPos, const Vector3& toPos) const;

			/**
			 * @brief 方向未設定時のフォールバック向きを返す。
			 * @return デフォルトの前方向。
			 */
			Vector3 GetDefaultForward() const;


		private:
			nsK2EngineLow::EffectEmitter* m_fireballEmitter = nullptr;

			FirePattern m_currentFirePattern = FirePattern::Straight;	//! 今の火炎の状態。
			RainDrop m_rainDrops[16];									//! 降らせる状態。MAX_RAIN_DROPS は .cpp 参照。

			WeaponHitDetection m_FireHit;								//! 炎の当たり判定。
			WeaponHitDetection m_LandingHit;							//! 噛みつきの当たり判定。
			WeaponHitDetection m_RainHit;								//! Rainの当たり判定。

			Vector3 m_fireballPos = Vector3::Zero;						//! 炎柱の当たり判定。
			Vector3 m_fireballVelocity = Vector3::Zero;					//! ファイアバーのベクトル。
			Vector3 m_landingPos = Vector3::Zero;						//! 噛みつく座標。

			float m_fireballLifeTime = 0.0f;							//! ファイアバーの寿命。
			float m_landingEffectTimer = 0.0f;							//! 噛みつき攻撃の寿命。
			float m_landingSpawnInterval = 0.0f;						//! 生成インターバル。
			float m_burstTimer = 0.0f;									//! タイマー。
			float m_rainTimer = 0.0f;									//! 降らせる時間。
			float m_rainSpawnTimer = 0.0f;								//! スポーンタイマー。

			int   m_burstRemaining = 0;									//! ブラスとを撃つか。

			bool  m_rainActive = false;									//! 降らせるかどうか。
			bool  m_fireballActive = false;								//! ファイアバーを出すかどうか。
		};
	}
}