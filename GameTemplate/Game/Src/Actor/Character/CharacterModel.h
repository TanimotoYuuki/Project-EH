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
	    /* モデルを読み込む。
		* @param characterType モデルの種類をしているする。
		* @param animationClip アニメーションクリップの配列。
		* @param numClips      アニメーションクリップの数。
		*/
		bool LoadCharacterModel(CharacterModelType characterType, AnimationClip* animationClip, int numClips);

		/**
		* 武器を読み込む。
		*/
		bool LoadWeaponModel(CharacterModelType weaponType);

		/* アニメーションを再生する。
		* @param animationNumber 再生するアニメーションの配列の要素数を指定する。
		* @param interpolateTime アニメーションの補間時間を指定する。デフォルトは0.2秒。
		*/
		void PlayAnimation(int animationNumber, float interpolateTime);

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

		/* 回転を設定。*/
		inline void SettRotation(const Quaternion& rotation)
		{
			if (m_characterModelRender)
				m_characterModelRender->SetRotation(rotation);
		}

		/* 大きさを設定。*/
		inline void SetScale(const Vector3& scale)
		{
			if (m_characterModelRender)
				m_characterModelRender->SetScale(scale);

			if (m_weaponModelRender)
				m_weaponModelRender->SetScale(scale);
		}

		/* 武器を創始するボーンの名前を変更する。*/
		inline void SetWeaponAttackBone(const std::wstring& boneName)
		{
			m_attackBoneName = boneName;
		}

	/* ゲッター。*/
	public:
		/* キャラモデルのファイルパスを格納。*/
		inline const std::string GetCharacterModelFilePath(std::string filePath)
		{
			 std::string modelPath = m_characterModelFilePath + filePath + m_modelExtension;
			 return modelPath;
		}

		/* 武器モデルのファイルパスを格納。*/
		inline const std::string GetWeaponModelFilePath(std::string filePath)
		{
			std::string weaponFilePath = m_weaponModelFilePath + filePath + m_modelExtension;
			return weaponFilePath;
		}

		/* 指定したボーンのワールド行列を取得する。
		* @param boneName 取得したいボーンの名前を指定する。
		*/
	    Matrix GetWorldMatrix(const wchar_t* boneName);


	private:	
		std::unordered_map<CharacterModelType, std::string> m_filePathList;		                                               /* モデルIDからファイルパスを文字列に変化。*/
		std::unique_ptr<ModelRender> m_characterModelRender;                                                                   /* モデルを管理。*/
		std::unique_ptr<ModelRender> m_weaponModelRender;                                                                      /* 武器モデルを管理。*/
		std::wstring m_attackBoneName;

		/* ファイルパスを定数化するための変数群。*/
		std::string m_modelFilePath;                                                                                           /* モデルのファイルパスを格納。*/
		std::string m_characterModelFilePath = "Assets/modelData/Character/CharacterModel/";                                   /* プレイヤー/NPCモデルのファイルパスを格納。*/
		std::string m_modelExtension = ".tkm";                                                                                 /* プレイヤー/NPCモデルの拡張子を格納。*/
		std::string m_weaponModelFilePath = "Assets/modelData/Character/Weapon/";                                              /* 武器モデルのファイルパスを格納。*/

		int boneID;                                                                                                            /* ボーンIDを格納。*/
		Vector3 m_matrixPosition;
		Quaternion m_matrixRotation;
	};	
}
