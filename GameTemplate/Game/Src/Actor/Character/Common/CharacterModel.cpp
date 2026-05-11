#include "stdafx.h"
#include "CharacterModel.h"

namespace nsApp
{
	CharacterModel::CharacterModel()
	{
		/* モデルのファイルパスを格納。*/
		/* デバッグ用のサンドバッグモデル。*/
		m_filePathList[CharacterModelType::Sandbag] = "Assets/modelData/Character/Debag/PunchBagKun.tkm";

		/* キャラモデルの格納。*/
		/* プレイヤーモデル。*/
		m_filePathList[CharacterModelType::Player_1P] = GetCharacterModelFilePath("1p/Player_1P"); //! 1Pモデル。
		m_filePathList[CharacterModelType::Player_2P] = GetCharacterModelFilePath("2p/player_2p"); //! 2Pモデル。
		m_filePathList[CharacterModelType::Player_3P] = GetCharacterModelFilePath("3p/Player_3P"); //! 3Pモデル。
		m_filePathList[CharacterModelType::Player_4P] = GetCharacterModelFilePath("4p/Player_4P"); //! 4Pモデル。

		/* ボスモデル。*/
		m_filePathList[CharacterModelType::TutorialBoss] = GetBossModelFilePath("TutorialBoss/Tutorial_Boss");
		m_filePathList[CharacterModelType::GrayDragon] = GetBossModelFilePath("GrayDragon/GrayDragon");
		m_filePathList[CharacterModelType::GreenDragon] = GetBossModelFilePath("GreenDragon/GreenDragon");
		m_filePathList[CharacterModelType::RedDragon] = GetBossModelFilePath("RedDragon/RedDragon");

		/* 武器モデル。*/
		/* 大剣。*/
		m_filePathList[CharacterModelType::Weapon_GreatSword] = GetWeaponModelFilePath("GreatSword");
		/* 双銃。*/
		m_filePathList[CharacterModelType::Weapon_TwinGun] = GetWeaponModelFilePath("TwinGun");
		/* ハンマー。*/
		m_filePathList[CharacterModelType::Weapon_Hammer] = GetWeaponModelFilePath("Hammer");
		/* 杖。*/ 
		m_filePathList[CharacterModelType::Weapon_Wand] = GetWeaponModelFilePath("Wand");
	}


	bool CharacterModel::LoadCharacterModel(CharacterModelType characterType, AnimationClip* animationClip, int numClips)
	{
		/* 読み込むモデルがリストの中にあるか確認。*/
		if (m_filePathList.count(characterType) == 0)
			return false; /* 登録されていないならロード失敗。*/

		/* IDに対応するモデルパスを取り出す。*/
		m_modelFilePath = m_filePathList[characterType];

		/* モデルロードクラスを生成する。*/
		m_characterModelRender = std::make_unique<ModelRender>();

		/* モデルをロードする。*/
		m_characterModelRender->Init
		(
			m_modelFilePath.c_str(), //! モデルのファイルパス。
			animationClip,           //! アニメーション。
			numClips,                //! アニメーションの補完率。
			enModelUpAxisZ           //! モデルの上方向となる軸を設定。 今回はZ軸。
		);

		return true;
	}


	bool CharacterModel::LoadWeaponModel(CharacterModelType weaponType)
	{
		/* 読み込む武器がリストにあるのかを確認。*/
		if (m_filePathList.count(weaponType) == 0)
			return false;
		
		/* 武器モデルを生成してロードする。*/
		m_weaponModelRender = std::make_unique<ModelRender>();
		m_weaponModelRender->Init(
			m_filePathList[weaponType].c_str(),
			nullptr,
			0,
			enModelUpAxisZ
		);

		return true;
	}


	bool CharacterModel::LoadSubWeaponModel(CharacterModelType subWeaponType)
	{
		if (m_filePathList.count(subWeaponType) == 0)
			return false;

		m_subWeaponModelRender = std::make_unique<ModelRender>();
		m_subWeaponModelRender->Init(
			m_filePathList[subWeaponType].c_str(),
			nullptr,
			0,
			enModelUpAxisZ
		);
		return false;
	}


	void CharacterModel::PlayAnimation(int animationNumber, float interpolateTime)
	{
		if (m_characterModelRender != nullptr)
			m_characterModelRender->PlayAnimation(animationNumber, interpolateTime);
	}


	void CharacterModel::DrawCharacterModel(RenderContext& rc)
	{
		/* 中身がある場合、描画する。*/
		if(m_characterModelRender != nullptr)
			m_characterModelRender->Draw(rc);

		/* 武器も描画する。*/
		if (m_weaponModelRender != nullptr)
			m_weaponModelRender->Draw(rc);

		/* サブ武器も描画する。*/ 
		if (m_subWeaponModelRender != nullptr)
			m_subWeaponModelRender->Draw(rc);
	}


	void CharacterModel::Update()
	{
		/* キャラモデル本体が無いなら処理を終了させる。*/
		if (m_characterModelRender == nullptr)
			return;

		/* モデルの更新処理。*/
		m_characterModelRender->Update();

		/* 武器が確認できない場合、処理を終了させる。*/
		if (m_weaponModelRender == nullptr)
			return;

		/* 右手のボーンの計算。*/
		if (m_weaponModelRender != nullptr)
		{
			/* 右手のボーンの位置と傾きのデータを取得。*/
			m_handMatrix = GetWorldMatrix(m_attackBoneName.c_str());

			/* 行列から座標を抽出。*/
			m_matrixPosition.x = m_handMatrix.m[3][0];
			m_matrixPosition.y = m_handMatrix.m[3][1];
			m_matrixPosition.z = m_handMatrix.m[3][2];

			m_xAxis = Vector3(m_handMatrix.m[0][0], m_handMatrix.m[0][1], m_handMatrix.m[0][2]);
			m_yAxis = Vector3(m_handMatrix.m[1][0], m_handMatrix.m[1][1], m_handMatrix.m[1][2]);
			m_zAxis = Vector3(m_handMatrix.m[2][0], m_handMatrix.m[2][1], m_handMatrix.m[2][2]);

			/* 正規化。*/
			m_xAxis.Normalize();
			m_yAxis.Normalize();
			m_zAxis.Normalize();

			m_offsetPosition = m_matrixPosition + (m_xAxis * m_weaponOffset.x) + (m_yAxis * m_weaponOffset.y) + (m_zAxis * m_weaponOffset.z);
			m_weaponModelRender->SetPosition(m_offsetPosition);


			m_rotationMatrix = m_handMatrix;
			m_rotationMatrix.m[0][0] = m_xAxis.x; m_rotationMatrix.m[0][1] = m_xAxis.y; m_rotationMatrix.m[0][2] = m_xAxis.z;
			m_rotationMatrix.m[1][0] = m_yAxis.x; m_rotationMatrix.m[1][1] = m_yAxis.y; m_rotationMatrix.m[1][2] = m_yAxis.z;
			m_rotationMatrix.m[2][0] = m_zAxis.x; m_rotationMatrix.m[2][1] = m_zAxis.y; m_rotationMatrix.m[2][2] = m_zAxis.z;

			/* 正規化した行列から回転を抽出。*/
			m_matrixRotation.SetRotation(m_rotationMatrix);
			m_weaponModelRender->SetRotation(m_matrixRotation * m_weaponRotationOffset);

			/* 武器を更新。*/
			m_weaponModelRender->Update();

		}

		/* サブ武器のボーンの更新。*/ 
		if (m_subWeaponModelRender != nullptr) {
			m_subWeaponHandMatrix = GetWorldMatrix(m_subWeaponBoneName.c_str());

			Vector3 subMatrixPosition(m_subWeaponHandMatrix.m[3][0], m_subWeaponHandMatrix.m[3][1], m_subWeaponHandMatrix.m[3][2]);
			Vector3 subXAxis(m_subWeaponHandMatrix.m[0][0], m_subWeaponHandMatrix.m[0][1], m_subWeaponHandMatrix.m[0][2]);
			Vector3 subYAxis(m_subWeaponHandMatrix.m[1][0], m_subWeaponHandMatrix.m[1][1], m_subWeaponHandMatrix.m[1][2]);
			Vector3 subZAxis(m_subWeaponHandMatrix.m[2][0], m_subWeaponHandMatrix.m[2][1], m_subWeaponHandMatrix.m[2][2]);

			subXAxis.Normalize();
			subYAxis.Normalize();
			subZAxis.Normalize();

			// オフセット計算 (m_weaponOffset を流用するか、専用の m_subWeaponOffset を使う)
			Vector3 subOffsetPosition = subMatrixPosition + (subXAxis * m_weaponOffset.x) + (subYAxis * m_weaponOffset.y) + (subZAxis * m_weaponOffset.z);
			m_subWeaponModelRender->SetPosition(subOffsetPosition);

			Matrix subRotationMatrix = m_subWeaponHandMatrix;
			subRotationMatrix.m[0][0] = subXAxis.x; subRotationMatrix.m[0][1] = subXAxis.y; subRotationMatrix.m[0][2] = subXAxis.z;
			subRotationMatrix.m[1][0] = subYAxis.x; subRotationMatrix.m[1][1] = subYAxis.y; subRotationMatrix.m[1][2] = subYAxis.z;
			subRotationMatrix.m[2][0] = subZAxis.x; subRotationMatrix.m[2][1] = subZAxis.y; subRotationMatrix.m[2][2] = subZAxis.z;

			Quaternion subMatrixRotation;
			subMatrixRotation.SetRotation(subRotationMatrix);

			Quaternion flipOffset;
			flipOffset.SetRotationDegX(180.0f);
			m_subWeaponModelRender->SetRotation(subMatrixRotation * m_weaponRotationOffset);
			m_subWeaponModelRender->SetScale(Vector3::One * 2.0f); // 右手と同じスケールに

			m_subWeaponModelRender->Update();
		}
	}


	Matrix CharacterModel::GetWorldMatrix(const wchar_t* boneName)
	{
		if (m_characterModelRender)
		{
			/* ボーンIDを取得。*/
			boneID = m_characterModelRender->FindBoneID(boneName);

			if (boneID != -1)
				return m_characterModelRender->GetBone(boneID)->GetWorldMatrix();
		}
		/* ボーンが見つからない場合。*/
		return Matrix::Identity;
	}
}