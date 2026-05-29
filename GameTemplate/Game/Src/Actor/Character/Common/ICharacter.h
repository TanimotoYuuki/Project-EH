#pragma once
/**
* @file   ICharacter.h
* @brief  実態のあるキャラクターが継承する基底クラス。
*         ステータス管理もここで行います。
*
* @author Yamaguchi Hayato
* @date   2026/03/10
*/

#include "Src/Actor/Actor.h"
#include "Src/Actor/Character/Common/CharacterModel.h"


namespace nsApp
{
	/* キャラクターのステータスを管理する構造体。*/

    /* 攻撃のパラメータを定義。*/
    /* クリティカルダメージ, 通常の攻撃力, クリティカル率を保持。*/
	struct AttackStatus
	{
		float normalDamage;       /* 通常の攻撃力。*/
		float criticalDamage;     /* クリティカルダメージ。*/
		float criticalRate;       /* クリティカル率。*/
	};

	/* 体力のパラメータを定義。*/
	struct HPStatus
	{
		int currentHP;            /* 現在HP。*/
		int maxHP;                /* 最大HP。*/
	};


	/* 現在HP, 最大HP, 攻撃力を定義。*/
	struct CharacterStatus
	{
		AttackStatus attack;      /* 攻撃のステータス。*/
		HPStatus hp;              /* 体力のステータス。*/
	};


	namespace nsActor
	{
		class ICharacter : public Actor
		{
		public:
			/* コンストラクタとデストラクタ*/
			ICharacter() = default;
			virtual ~ICharacter() = default;


		public:
			/* ライフサイクル。*/
			/* 初期化処理。*/ 
			virtual bool Start() override = 0;
			/* 更新処理。*/
			virtual void Update() override;
			/* 描画処理。*/
			virtual void Render(RenderContext& rc) override;


		protected:
			/**
			 * @brief ヒットストップのタイマーを更新する処理。
			 */
			inline void UpdateHitStioTImer()
			{
				if (m_hitStopFlame > 0)
					m_hitStopFlame--;
			}


		public:
			/* 
			 * @brief ダメージを受け取る処理。
			 * @param damage 受けるダメージ量。
			 */
			virtual void ApplyDamage(int damage);


		/* セッター。*/
		public:
			/* ヒットストップを設定。*/
			inline void SetHitStop(int flame)
			{
				m_hitStopFlame = flame;
			}


		/* ゲッター。*/
		public:
			/* ステータスの受け取り口。*/
			inline const CharacterStatus& GetCharacterStatus() const
			{
				return m_characterStatus;
			}

			/* 座標を取得。*/
			virtual Vector3& GetPosition() = 0;

			/* ヒットストップ状態か検知する。*/
			inline bool IsHitStop() const
			{
				return m_hitStopFlame > 0;
			}


		protected:
			CharacterStatus m_characterStatus; //! キャラクターのステータス。
			CharacterModel m_model;            //! キャラクターモデル。

			int m_hitStopFlame;                //! ヒットストップのフレーム数。
		};
	}
}
