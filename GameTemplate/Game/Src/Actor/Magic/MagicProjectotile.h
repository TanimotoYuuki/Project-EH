#pragma once

/**
 * @file   MagicProjectotile.h
 * @brief  攻撃の際に飛ばすオブジェクトを管理するクラス。
 * @author Yamaguchi Hayato。
 * @date   2026/03/23
 */

#include "Src/Actor/Character/Common/WeaponHitDetection.h"


namespace nsApp
{
	namespace nsActor
	{
		enum class MagicType : uint8_t
		{
			enNormalMagic,  //! 通常魔法。
			enRushMagic,    //! 連打魔法。
			enAirMagic,     //! 空中魔法。
			enChargeMagic,  //! チャージ魔法。
			enNone,         //! 魔法なし。
		};


		class MagicProjectotile : public IGameObject
		{
		public:
			/* コンストラクタとデストラクタ。*/
			MagicProjectotile() = default;
			virtual ~MagicProjectotile() = default;


		public:
			/* 
			 * @brief 初期化処理。
			 * @param type:             魔法の種類。
			 * @param spawnPosition: 　 魔法の出現位置。
			 * @param forwardDirection: 前方向のベクトル。
			 */
			void Initialize(MagicType type, const Vector3& spawnPosition, const Vector3& forwardDirection);


			/**
			 * @brief 更新処理。   
			 */
			void Update();

			/**
			 * @brief 描画処理。
			 * @detail ここでは攻撃時に射出するミサイルオブジェクトの描画を行う。
			 */
			void Render(RenderContext& rc);


		private:
			/**
			* @brief ターゲットに向かって移動する処理。
			*/
			void TargetMoving();



		/* セッター。*/
		public:
			/* ダメージ数を設定。*/
			inline void SetDamage(float damage)
			{
				m_damage = damage;
			}

			/* 目標を設定。*/
			inline void SetTarget(nsActor::ICharacter* target)
			{
				m_target = target;
			}


		/* ゲッター。*/
		public:
			/* 魔法の種類を取得。*/
			inline MagicType GetMagicType() const
			{
				return m_magicType;
			}

			/* モデル名を取得。*/
			inline const std::string SearchMissileModelPath(const std::string& modelName)
			{
				return "Assets/modelData/Character/Weapon/" + modelName + ".tkm";
			}

			/* 角度計算。*/
			Quaternion MakeAngle(float degX, float degY, float degZ)
			{
				Quaternion angleX, angleY, angleZ;
				angleX.SetRotationDegX(degX);
				angleY.SetRotationDegY(degY);
				angleZ.SetRotationDegZ(degZ);
				return angleX * angleY * angleZ;
			}


		private:
			nsActor::ICharacter* m_target = nullptr;       //! 目標。


		private:
			MagicType m_magicType = MagicType::enNone;     //! 魔法の種類。

			WeaponHitDetection m_hitDetection;             //! 魔法の当たり判定を管理するクラス。

			Vector3 m_position = Vector3::Zero;            //! 魔法の位置。
			Vector3 m_velocity = Vector3::Zero;            //! 魔法の速度。
			Vector3 m_forwardDirection = Vector3::Zero;    //! 魔法の前方向ベクトル。
			Vector3 m_scale = Vector3::One;                //! 魔法のスケール。
			Vector3 m_targetPosition = Vector3::Zero;      //! 目標位置。
			Vector3 m_toTargetVector = Vector3::Zero;      //! 目標と自身の距離ベクトル。
			Vector3 m_currentDirection = Vector3::Zero;    //! 現在の移動方向。
			Vector3 m_newPosition = Vector3::Zero;         //! 新しい位置。

			Quaternion m_angle = Quaternion::Identity;     //! 魔法の回転角。
			Quaternion m_direction = Quaternion::Identity; //! 角度。


			ModelRender m_missileMddel;                    //! ミサイルを描画するレンダー。

			int m_lifeTimer = 0;                           //! 魔法の生成時間を管理するタイマー。
			
			float m_damage = 0.0f;                         //! 魔法のダメージ量。
			float m_moveSpeed = 0.0f;                      //! 移動速度。
		};
	}
}