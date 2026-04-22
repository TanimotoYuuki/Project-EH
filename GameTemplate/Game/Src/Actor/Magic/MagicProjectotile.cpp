#include "stdafx.h"
#include "MagicProjectotile.h"


namespace nsApp
{
	namespace nsActor
	{
		/*
		 * @struct
		 * @brief
         * 魔法を構成する要素をまとめた構造体。
         */
		struct MagicParam
		{
			float speed = 0.0f;      //! 魔法の射出速度。
			float damage = 0.0f;     //! 魔法のダメージ量。
			std::string modelPath;   //! モデル名。
			float hitRadius = 0.0f;  //! 魔法の当たり判定の半径。
		};

		const std::unordered_map<MagicType, MagicParam> MAGIC_PARAM_TABLE =
		{
			{ MagicType::enNormalMagic, {2.0f, 10.0f, SearchMissileModelPath("NormalAttackMissile"), 10.0f }},
			{ MagicType::enChargeMagic, {2.0f, 10.0f, SearchMissileModelPath("ChargeAttackMissile"), 10.0f}},
			{ MagicType::enRushMagic, {2.0f, 10.0f, SearchMissileModelPath("PressingAttackMissile"), 10.0f}},
			{ MagicType::enAirMagic, {2.0f, 10.0f, SearchMissileModelPath("NormalAttackMissile"), 10.0f}},
		};




		void MagicProjectotile::Initialize(MagicType type, const Vector3& spawnPosition, const Vector3& forwardDirection)
		{
			m_magicType = type;
			m_position = spawnPosition;
			m_lifeTimer = 0;

			/* テーブルから条件にあった魔法を取り出す。*/
			const auto& param = MAGIC_PARAM_TABLE.at(type);

			m_velocity = forwardDirection * param.speed;
			SetDamage(param.damage);
			m_missileMddel.Init(param.modelPath.c_str());
			m_hitDetection.Init(param.hitRadius);
		}


		void MagicProjectotile::Update()
		{
			/* 位置を更新する。*/
			m_position += m_velocity;

			/* 消滅処理。*/
			m_lifeTimer++;
			if (m_lifeTimer > 120.0f)
				DeleteGO(this);

			m_missileMddel.SetScale(Vector3::One);
			m_missileMddel.SetPosition(m_position);
			m_missileMddel.Update();
		}


		void MagicProjectotile::Render(RenderContext& rc)
		{
			m_missileMddel.Draw(rc);
		}
	}
}