#pragma once
/**
* @file   CharacterModel.h
* @brief  キャラクターモデル/アニメーションを管理するクラス。
*         BossModel/Player1p～4pモデルを管理。
* @author Yamaguchi Hayato。
* @date   2026/03/10
*/


namespace nsApp
{
	namespace nsActor
	{
		enum class CharacterModelType
		{
			/* プレイヤーモデル1P～4Pを定義。*/
			Player_1P,           /* 1P。*/
			Player_2P,           /* 2P。*/
			Player_3P,           /* 3P。*/
			Player_4P,           /* 4P。*/


			/* 敵モデルを定義。*/
			/* @TODO Enemy担当者にIDを増やしてもらう。*/
			Enemy_Tutorial,      /* チュートリアル用の敵。*/


			/* 武器モデルを定義。*/
			Weapon_GreatSword,   /* 大剣。*/
			Weapon_TwinSword,    /* 双剣。*/
			Weapon_Parasol,      /* パラソル。*/
			Weapon_Axe,          /* 斧。*/
			Weapon_Hammer,       /* ハンマー。*/
			Weapon_Wand,		 /* 杖。 @TODO 3Dモデルを探す。*/
		};


		class CharacterModel 
		{
		public:
			/* コンストラクタとデストラクタ。*/
			CharacterModel();
			~CharacterModel() = default;


		public:
			/* モデルを読み込む。*/
			bool LoadCharacterModel(CharacterModelType characterType);

			/* キャラクターを描画する。*/
			void DrawCharacterModel(RenderContext& rc);

			/* 更新処理。*/
			void Update();


		/* セッター。*/
		public:
			/* 座標を設定。*/
			inline void SetPosition(const Vector3& position)
			{
				if (m_characterModelRender)
					m_characterModelRender->SetPosition(position);
			}


		
		public:
			/* キャラモデルのファイルパスを格納。*/
			inline const std::string GetCharacterModelFilePath(std::string filePath)
			{
				 std::string modelPath = "Assets/modelData/Character/CharacterModel/" + filePath + ".tkm";
				 return modelPath;
			}

			/* 武器モデルのファイルパスを格納。*/
			inline const std::string GetWeaponModelFilePath(std::string filePath)
			{
				std::string weaponFilePath = "Assets/modelData/Weapon/" + filePath + ".tkm";
				return weaponFilePath;
			}


		private:	
			std::unordered_map<CharacterModelType, std::string> m_filePathList;		/* モデルIDからファイルパスを文字列に変化。*/
			std::unique_ptr<ModelRender> m_characterModelRender;                    /* モデルを管理。*/

			std::string m_modelFilePath;                                            /* モデルのファイルパスを格納。*/
		};
	}
}
