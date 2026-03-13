#pragma once

/**
* @file   Player.h
* @brief  プレイヤーステートマシーンを更新するクラス。
* @author Yamaguchi Hayato
* @date   2026/03/11
*/

#include "Src/Actor/Character/ICharacter.h"
#include "Src/Actor/Character/CharacterAnimation.h"

namespace nsApp
{
	namespace nsActor
	{
		class Player : public ICharacter
		{
		public:
		    /* コンストラクタとデストラクタ。*/
			Player() = default;
			virtual ~Player() = default;


		public:
			/* ライフサイクル。*/
			/* 初期化処理。*/
			bool Start() override;
			/* 毎フレーム更新する処理。*/
			void Update() override;
			/* 描画処理。*/
			void Render(RenderContext& rc) override;


		public:
			/* 基本動作用アニメーションを再生。*/
			void PlayBasicAnimation(CharacterBasicAnimationList state);


		private:
			CharacterAnimation m_playerAnimation; /* プレイヤーのアニメーション。*/
			WeaponType m_currentWeapon = WeaponType::GreatSword;           /* 現在の武器。@TODO 武器の種類を増やす際に要調整。*/
		};
	}
}
