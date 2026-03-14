#include "stdafx.h"
#include "CharacterModel.h"

namespace nsApp
{
	CharacterModel::CharacterModel()
	{
		/* モデルのファイルパスを格納。*/
		/* キャラモデルの格納。*/
		/* プレイヤーモデル。*/
//		m_filePathList[CharacterModelType::Player_1P] = GetCharacterModelFilePath("1p/Player_1P"); /* 1Pモデル。*/
		m_filePathList[CharacterModelType::Player_1P] = GetCharacterModelFilePath("1p/player"); /* 1Pモデル。*/
		m_filePathList[CharacterModelType::Player_2P] = GetCharacterModelFilePath("2p/Player_2P"); /* 2Pモデル。*/
		m_filePathList[CharacterModelType::Player_3P] = GetCharacterModelFilePath("3p/Player_3P"); /* 3Pモデル。*/
		m_filePathList[CharacterModelType::Player_4P] = GetCharacterModelFilePath("4p/Player_4P"); /* 4Pモデル。*/

		/* ボスモデル。*/
		/* @TODO: Boss担当にコードを記載してもらうように。*/

		/* 武器モデル。*/
		/* 大剣。*/
		m_filePathList[CharacterModelType::Weapon_GreatSword] = GetWeaponModelFilePath("GreatSword");
		/* 双剣。*/ 
	//	m_filePathList[CharacterModelType::Weapon_TwinSword] = GetWeaponModelFilePath("TwinSword");

		/* @TODO 他の武器モデルの登録は4月～とする。*/
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
			m_modelFilePath.c_str(), /* モデルのファイルパス。*/
			animationClip,           /* アニメーション。*/
			numClips,                /* アニメーションの補完率。*/
			enModelUpAxisZ           /* モデルの上方向となる軸を設定。 今回はZ軸。*/
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


	void CharacterModel::PlayAnimation(int animationNumber, float interpolateTime)
	{
		if (m_characterModelRender != nullptr)
		{
			m_characterModelRender->PlayAnimation(animationNumber, interpolateTime);
		}
	}


	void CharacterModel::DrawCharacterModel(RenderContext& rc)
	{
		/* 中身がある場合、描画する。*/
		if(m_characterModelRender != nullptr)
			m_characterModelRender->Draw(rc);

		/* 武器も描画する。*/
		//if (m_weaponModelRender != nullptr)
		//	m_weaponModelRender->Draw(rc);
	}


	void CharacterModel::Update()
	{
		/* 更新処理。*/
		if(m_characterModelRender != nullptr)
			m_characterModelRender->Update();

		/* 武器を装備させる。*/
		//if (m_characterModelRender != nullptr && m_weaponModelRender != nullptr)
		//{
		//	/* 右手のボーンの位置と傾きのデータを取得。*/
		//	Matrix handMatrix = GetWorldMatrix(L"mixamorig:RightHand");

		//	/* 行列から座標を抽出。*/
		//	m_matrixPosition.x = handMatrix.m[3][0];
		//	m_matrixPosition.y = handMatrix.m[3][1];
		//	m_matrixPosition.z = handMatrix.m[3][2];
		//	m_weaponModelRender->SetPosition(m_matrixPosition);

		//	/* 【修正部分】行列の各軸ベクトルを正規化してスケールを取り除く */
		//	Vector3 xAxis(handMatrix.m[0][0], handMatrix.m[0][1], handMatrix.m[0][2]);
		//	Vector3 yAxis(handMatrix.m[1][0], handMatrix.m[1][1], handMatrix.m[1][2]);
		//	Vector3 zAxis(handMatrix.m[2][0], handMatrix.m[2][1], handMatrix.m[2][2]);

		//	xAxis.Normalize();
		//	yAxis.Normalize();
		//	zAxis.Normalize();

		//	/* 回転抽出用のスケール1.0の行列を作成 */
		//	Matrix rotationMatrix = handMatrix;
		//	rotationMatrix.m[0][0] = xAxis.x; rotationMatrix.m[0][1] = xAxis.y; rotationMatrix.m[0][2] = xAxis.z;
		//	rotationMatrix.m[1][0] = yAxis.x; rotationMatrix.m[1][1] = yAxis.y; rotationMatrix.m[1][2] = yAxis.z;
		//	rotationMatrix.m[2][0] = zAxis.x; rotationMatrix.m[2][1] = zAxis.y; rotationMatrix.m[2][2] = zAxis.z;

		//	/* 正規化した行列から回転を抽出。*/
		//	m_matrixRotation.SetRotation(rotationMatrix);
		//	m_weaponModelRender->SetRotation(m_matrixRotation);

		//	/* 武器を更新。*/
		//	m_weaponModelRender->Update();
		//}
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