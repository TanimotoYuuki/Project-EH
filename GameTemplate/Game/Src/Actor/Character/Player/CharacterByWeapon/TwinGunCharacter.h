#pragma once
/**
 * @file    TwinGunCharacter.h。
 * @brief   TwinGunPlayerを管理を管轄するクラス。
 * @author  Yamaguchi Hayato。
 * @date    2026/04/24
 */

#include "Src/Actor/Character/Player/CharacterByWeapon/IWeaponCharacter.h"

namespace nsApp
{
	namespace nsActor
	{
		class TwinGunCharacter : public IWeaponCharacter
		{
		public:
			/* コンストラクタとデストラクタ。*/
			TwinGunCharacter() = default;
			virtual ~TwinGunCharacter() = default;


		public:
			/**
			 * 初期化。
			 */
			bool Start() override;


		protected:
			/**
			 * @brief 必要なステートを登録する。
			 */
			void RegisterState() override;


		/* ヘルパー。*/
		private:
			/**
			 * @brief 武器の角度を調整する。
			 * @param angleX X軸の角度。
			 * @param angleY Y軸の角度。
			 * @param angleZ Z軸の角度。
			*/	
		     Quaternion MakeGunAngle(float angleX, float angleY, float angleZ) const
			{
				Quaternion rotX, rotY, rotZ;
				rotX.SetRotationDegX(angleX);
				rotY.SetRotationDegY(angleY);
				rotZ.SetRotationDegZ(angleZ);

				return ComputeAngle(rotX, rotY, rotZ);
			}

			/**
			 * @brief 回転行列を計算する。
			 */
			inline Quaternion ComputeAngle(const Quaternion& rotX, const Quaternion& rotY, const Quaternion& rotZ) const
			{
				return rotZ * rotY * rotX;
			}


		private:
			Quaternion m_angle = Quaternion::Identity;        //! モデルの角度。
		};
	}
}
