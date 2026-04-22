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
			enHeelMagic,    //! 回復魔法。
			enNone,         //! 魔法なし。
		};

		namespace
		{
			/*
			 * @struct
			 * @brief
			 * 魔法を構成する要素をまとめた構造体。
			 */
			struct MagicParam
			{
				float speed = 0.0f;     //! 魔法の射出速度。
				float damage = 0.0f;    //! 魔法のダメージ量。
				std::string modelPath;  //! モデル名。
				float hitRadius = 0.0f; //! 魔法の当たり判定の半径。
				float hitHeight = 0.0f; //! 魔法の当たり判定の高さ。
			};

			const std::unordered_map<MagicType, MagicParam> MAGIC_PARAM_TABLE =
			{
				{ MagicType::enNormalMagic, {2.0f, 10.0f, } },
			};
		}


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
			 * @brief　描画処理。
			 * @detail ここでは攻撃時に射出するミサイルオブジェクトの描画を行う。
			 */
			void Render(RenderContext& rc);


		/* セッター。*/
		public:
			/* ダメージ数を設定。*/
			inline void SetDamage(float damage)
			{
				m_damage = damage;
			}


		/* ゲッター。*/
		public:
			/* 魔法の種類を取得。*/
			inline MagicType GetMagicType() const
			{
				return m_magicType;
			}


		/* ヘルパー。*/
		private:
			/* ファイルパスを取得。*/
			inline const std::string SearchMissileModelPath(const std::string modelName) 
			{
				const std::string missileModelPath = "Assets/modelData/Character/Weapon/" + modelName + ".tkm";
				return missileModelPath;
			}



		private:
			MagicType m_magicType = MagicType::enNone;  //! 魔法の種類。

			WeaponHitDetection m_hitDetection;          //! 魔法の当たり判定を管理するクラス。

			Vector3 m_position = Vector3::Zero;         //! 魔法の位置。
			Vector3 m_velocity = Vector3::Zero;         //! 魔法の速度。
			Vector3 m_forwardDirection = Vector3::Zero; //! 魔法の前方向ベクトル。

			ModelRender m_missileMddel;                 //! ミサイルを描画するレンダー。

			int m_lifeTimer = 0;                        //! 魔法の生成時間を管理するタイマー。
			
			float m_damage = 0.0f;                      //! 魔法のダメージ量。
		};
	}
}




