#pragma once
/**
* @file   MagicFactory.h
* @brief  魔法関連のオブジェクトを生成するクラス。
* @author Yamaguchi Hayato。
* @date   2026/05/12
* @detail MagicオブジェクトをFactoryPutterで設計する上の生成部分を管轄。
*/

#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include "Src/Actor/Character/Common/ICharacter.h"
#include "Src/Actor/Magic/MagicProjectotile.h"

namespace nsApp
{
	class MagicFactory
	{
	public:
		/**
		 * @brief マジックオブジェクトを生成する。
		 * @param type 魔法の種類。
		 * @param basePosition 生成の基準となる位置。
		 * @param forward 魔法の前方向ベクトル。
		 * @param target 魔法のターゲット(デフォルト値で初期化済み)。
		 */
		static void CreateMagicObject(nsActor::MagicType type, const Vector3& basePosition, const Vector3& forward, nsActor::ICharacter* target = nullptr);


	private:
		Vector3 m_forwardDirection = Vector3::Zero; //! 魔法の前方向ベクトル。


	};
}
