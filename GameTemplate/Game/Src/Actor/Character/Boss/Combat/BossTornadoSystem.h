#pragma once

/**
 * @file   BossTornadoSystem.h
 * @brief  ボスの竜巻攻撃（GreenDragon 専用）を管理するクラス。
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
		 * @brief 竜巻の移動パターン種別。
		 */
		enum class TornadoMoveType : uint8_t
		{
			DualPush,
			SidePass,
			StageWander,
		};

		/**
		 * @brief 竜巻 1 体分のランタイムデータ。Boss.cpp から BossTornadoSystem へ移行予定。
		 */
		struct BossTornado
		{
			bool active = false;									//! 使用中かどうか。
			TornadoMoveType moveType = TornadoMoveType::SidePass;	//! 移動パターン。
			Vector3 pos = Vector3::Zero;							//! 論理座標。
			Vector3 velocity = Vector3::Zero;						//! 移動速度。	
			float lifeTimer = 0.0f;									//! 残存時間。
			float wanderAnchorX = 0.0f;								//! 徘徊中心 X。徘徊パターンで使用。
			float wanderPhase = 0.0f;								//! 徘徊位相。徘徊パターンで使用。
			float damageTimer = 0.0f;								//! ダメージ間隔タイマー。これが 0 以下のときにプレイヤーにダメージを与えることができ、ダメージを与えたらこのタイマーを TORNADO_DAMAGE_INTERVAL にリセットする。
			float effectRespawnTimer = 0.0f;						//! エフェクト再生成タイマー。竜巻エフェクトは一定時間ごとに再生成して見た目をリフレッシュする。これが 0 以下のときにエフェクトを再生成し、再生成したらこのタイマーを TORNADO_EFFECT_RESPAWN_INTERVAL にリセットする。
			float baseScale = 2.0f;									//! 基本スケール。竜巻の大きさに影響する。スポーン時に TORNADO_BASE_SCALE をセットし、移動パターンや残存時間に応じて変化させることもある。
			nsK2EngineLow::EffectEmitter* emitter = nullptr;		//! 表示用エフェクト。スポーン時に生成し、竜巻の位置に追従させる。寿命が尽きるかステージ外に出たら削除���る。
		};

		class BossTornadoSystem
		{
		public:
			/**
			 * @brief 竜巻攻撃の初期化。Boss::Start() から呼ぶ。
			 */
			void Init();

			/**
			 * @brief 竜巻攻撃のヒット判定を配置する。Boss::AttackTail() から呼ぶ。
			 * @param boss 攻撃主体のボス。
			 */
			void Update(Boss* boss);

			/**
			 * @brief 竜巻攻撃のヒット判定とダメージを処理する。Boss::Update() から呼ぶ。
			 * @param boss 攻撃主体のボス。
			 * @param tailHitPos 尻尾攻撃のヒット判定位置。Boss::AttackTail() から渡される。s
			 */
			void SpawnFromTail(Boss* boss, const Vector3& tailHitPos);


		private:
			/**
			 * @brief 竜巻の論理座標からエフェクト表示位置を計算する。竜巻は地面に沿って移動するため、Y 座標は固定されるが、エフェクトは見た目の高さ調整のために Y オフセットが加えられる。
			 * @param logicPos 竜巻の論理座標。
			 * @return エフェクト表示位置。
			 */
			Vector3 GetTornadoEffectPosition(const Vector3& logicPos) const;


		private:
			WeaponHitDetection m_TornadoHit;		//! 竜巻攻撃のヒット判定。
			BossTornado m_tornadoes[4];				//! 同時に存在可能な竜巻の最大数。Boss.cpp 参照。
		};
	}
}