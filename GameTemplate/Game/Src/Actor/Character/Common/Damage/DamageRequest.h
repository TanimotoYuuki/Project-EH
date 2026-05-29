#pragma once
/*
 * @file DamageRequest.h
 * @brie 対象にどんな攻撃が当たったかを渡すためのデータ。
 * @author Yamaguchi Hayato
 * @date 2026/05/20
 */


#include "Src/Actor/Character/Common/ICharacter.h"
#include "Src/Actor/Character/Common/CharacterAnimation.h"

namespace nsApp
{
	/**
	 * struct DamageRequest
	 * ダメージフォントの描画リクエストをする際に必要な情報をまとめた構造体。
	 */
	struct DamageRequest 
	{
		nsActor::ICharacter* target = nullptr;			//! ダメージを受けるキャラクター。
		int damageAmount = 0;                           //! ダメージ量。
		Vector3 hitPosition = Vector3::Zero;         //! ダメージフォントを生成する座標。
	};
}
