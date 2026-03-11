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
#include "Src/Actor/Character/CharacterModel.h"

namespace
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


	/* 現在HP, 最大HP, 攻撃力, 移動速度を定義。*/
	struct CharacterStatus
	{
		AttackStatus attack;      /* 攻撃のステータス。*/
		HPStatus hp;              /* 体力のステータス。*/
		float moveSpeed;          /* 移動速度。@TODO 要調整。*/
	};
}


namespace nsApp
{
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
			virtual void Draw(RenderContext& rc) override;


		/* ゲッター。*/
		public:
			/* ステータスの受け取り口。*/
			inline const CharacterStatus& GetCharacterStatus() const
			{
				return m_characterStatus;
			}


		protected:
			CharacterStatus m_characterStatus; /* キャラクターのステータス。*/
			CharacterModel m_model;            /* キャラクターモデル。*/
		};
	}
}
