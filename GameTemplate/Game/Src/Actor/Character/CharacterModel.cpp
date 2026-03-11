#include "stdafx.h"
#include "CharacterModel.h"

namespace nsApp
{
	namespace nsActor
	{
		CharacterModel::CharacterModel()
		{
			/* モデルのファイルパスを格納。*/
			/* キャラモデルの格納。*/
			/* プレイヤーモデル。*/
			m_filePathList[CharacterModelType::Player_1P] = GetCharacterModelFilePath("1p/Player_1P"); /* 1Pモデル。*/
			m_filePathList[CharacterModelType::Player_2P] = GetCharacterModelFilePath("2p/Player_2P"); /* 2Pモデル。*/
			m_filePathList[CharacterModelType::Player_3P] = GetCharacterModelFilePath("3p/Player_3P"); /* 3Pモデル。*/
			m_filePathList[CharacterModelType::Player_4P] = GetCharacterModelFilePath("4p/Player_4P"); /* 4Pモデル。*/

			/* ボスモデル。*/
			/* @TODO: Boss担当にコードを記載してもらうように。*/

			/* 武器モデル。*/
			/* @TODO: 現状はPlayerモデルを切り替えれるかの検証のため、武器モデルは次に回す。*/
		}


		bool CharacterModel::LoadCharacterModel(CharacterModelType characterType)
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
				nullptr,
				0,
				enModelUpAxisZ
			);

			return true;
		}


		void CharacterModel::DrawCharacterModel(RenderContext& rc)
		{
			/* 中身がある場合、描画する。*/
			if(m_characterModelRender != nullptr)
				m_characterModelRender->Draw(rc);
		}


		void CharacterModel::Update()
		{
			/* 更新処理。*/
			if(m_characterModelRender != nullptr)
				m_characterModelRender->Update();
		}
	}
}