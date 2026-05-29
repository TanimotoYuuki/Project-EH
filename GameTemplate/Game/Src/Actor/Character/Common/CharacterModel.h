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
		/* デバッグ用モデル*/
		Sandbag,            /* サンドバッグ。*/

		/* プレイヤーモデル1P～4Pを定義。*/
		Player_1P,           /* 1P。*/
		Player_2P,           /* 2P。*/
		Player_3P,           /* 3P。*/
		Player_4P,           /* 4P。*/


		/* 敵モデルを定義。*/
		TutorialBoss,      /* チュートリアル用のボス。*/
		GrayDragon,        /* 灰色のドラゴン。*/
		GreenDragon,       /* 緑色のドラゴン。*/
		RedDragon,         /* 赤色のドラゴン。*/


		/* 武器モデルを定義。*/
		Weapon_GreatSword,   //! 大剣。
		Weapon_TwinGun,      //! 双銃。
		Weapon_Hammer,       //! ハンマー。
		Weapon_Wand,		 //! 杖。 
	};


	class CharacterModel
	{
	public:
		/* コンストラクタとデストラクタ。*/
		CharacterModel();
		virtual ~CharacterModel() = default;


	public:
		/* モデルを読み込む。
		 * @param characterType モデルの種類をしているする。
		 * @param animationClip アニメーションクリップの配列。
		 * @param numClips      アニメーションクリップの数。
		 */
		bool LoadCharacterModel(CharacterModelType characterType, AnimationClip* animationClip, int numClips);

		/**
		 * @brief 武器を読み込む。
		 * @param weaponType 武器の種類を指定する。
		 */
		bool LoadWeaponModel(CharacterModelType weaponType);

		/**
		 * @brief サブの武器を読み込む。

		 */
		bool LoadSubWeaponModel(CharacterModelType subWeaponType);

		/*
		 * @brief モデルを破棄する。
		 * @duration サブ武器モデルを破棄時に活用。
		 */
		inline void ResetSubWeaponModel()
		{
			m_subWeaponModelRender.reset();
		}


		/* アニメーションを再生する。
		 * @param animationNumber 再生するアニメーションの配列の要素数を指定する。
		 * @param interpolateTime アニメーションの補間時間を指定する。デフォルトは0.2秒。
		 */
		void PlayAnimation(int animationNumber, float interpolateTime);

		/* キャラクターを描画する。*/
		void DrawCharacterModel(RenderContext& rc);

		/* 更新処理。*/
		void Update();

		/* アニメーションが再生終了したか。*/
		bool IsPlayAnimation()
		{
			/* nullチェック。*/
			if (m_characterModelRender != nullptr)
				/* アニメーションが再生終了しているかを返す。*/
				return m_characterModelRender->IsPlayingAnimation();

			return false;
		}


	private:
		/* 行列から座標ベクトルを取り出す。*/
		void ExtractCoordinatesFromMatrix();

		/* 行列から回転ベクトルを取り出す。*/
		void ExtractRotationCoordinates();

		/* 取り出した回転行列データを手のMatrixデータに代入。*/
		void AssignMatrixToHandData();

		/* */


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

		/* キャラモデルの大きさを設定。*/
		inline void SetCharacterScale(const Vector3& scale)
		{
			if (m_characterModelRender)
				m_characterModelRender->SetScale(scale);
		}

		/* 武器モデルの大きさを設定。*/
		inline void SetWeaponScale(const Vector3& scale)
		{
			if (m_weaponModelRender)
				m_weaponModelRender->SetScale(scale);
		}

		/* 武器のずれを調整。*/
		inline void SetWeaponOffset(const Vector3& offset)
		{
			m_weaponOffset = offset;
		}

		/* 武器を装備するボーンの名前を変更する。*/
		inline void SetWeaponAttackBone(const std::wstring& boneName)
		{
			m_attackBoneName = boneName;
		}

		/* 武器の傾きを設定。*/
		inline void SetWeaponAngle(const Quaternion offset)
		{
			m_weaponRotationOffset = offset;
		}



		/* ゲッター。*/
	public:
		/* キャラモデルのファイルパスを格納。*/
		inline const std::string GetCharacterModelFilePath(std::string filePath)
		{
			std::string modelPath = m_characterModelFilePath + filePath + m_modelExtension;
			return modelPath;
		}

		/*ボスモデルのファイルパスを格納。*/
		inline const std::string GetBossModelFilePath(const std::string& filePath)
		{
			return m_modelBossModelFilePath + "/" + filePath + ".tkm";
		}

		/* 武器モデルのファイルパスを格納。*/
		inline const std::string GetWeaponModelFilePath(std::string filePath)
		{
			std::string weaponFilePath = m_weaponModelFilePath + filePath + m_modelExtension;
			return weaponFilePath;
		}

		/*
		* 指定したボーンのワールド行列を取得する。
		* @param boneName 取得したいボーンの名前を指定する。
		*/
		Matrix GetWorldMatrix(const wchar_t* boneName);

		/* 武器の座標を取得。*/
		inline const Vector3& GetWeaponPosition() const
		{
			return m_offsetPosition;
		}


	private:
		std::unordered_map<CharacterModelType, std::string> m_filePathList;		                                               //! モデルIDからファイルパスを文字列に変化。
		std::unique_ptr<ModelRender> m_characterModelRender;                                                                   //! モデルを管理。
		std::unique_ptr<ModelRender> m_weaponModelRender;                                                                      //! 武器モデルを管理。
		std::unique_ptr<ModelRender> m_subWeaponModelRender;                                                                   //! サブ武器モデルを管理。

		std::wstring m_attackBoneName = L"mixamorig:RightHand";                                                                //! デフォルトのボーン名。
		std::wstring m_subWeaponBoneName = L"mixamorig:LeftHand";                                                              //! サブ武器を装備するボーン名。

		/* ファイルパスを定数化するための変数群。*/
		std::string m_modelFilePath;                                                                                           /* モデルのファイルパスを格納。*/
		std::string m_characterModelFilePath = "Assets/modelData/Character/CharacterModel/";                                   /* プレイヤー/NPCモデルのファイルパスを格納。*/
		std::string m_modelExtension = ".tkm";                                                                                 /* プレイヤー/NPCモデルの拡張子を格納。*/
		std::string m_modelBossModelFilePath = "Assets/modelData/Boss";
		std::string m_weaponModelFilePath = "Assets/modelData/Character/Weapon/";                                              /* 武器モデルのファイルパスを格納。*/

		int boneID;                                                                                                            //! ボーンIDを格納。

		Quaternion m_matrixRotation;
		Quaternion m_weaponRotationOffset;                                                                                     //! 武器の傾きを制御。

		Matrix m_handMatrix; 																					               //! 武器を装備させるときの右手のボーンの行列を管理する変数。
		Matrix m_rotationMatrix;                                                                                               //! 武器を装備させたときの武器の傾きを制御。
		Matrix m_subWeaponHandMatrix;

		Vector3 m_xAxis;                                                                                                       //! 武器を装備させるときの右手のボーンの行列から抽出したX軸を管理する変数。
		Vector3 m_yAxis;                                                                                                       //! 武器を装備させるときの右手のボーンの行列から抽出したY軸を管理する変数。
		Vector3 m_zAxis;                                                                                                       //! 武器を装備させるときの右手のボーンの行列から抽出したZ軸を管理する変数。
		Vector3 m_matrixPosition;
		Vector3 m_weaponOffset;                                                                                                //! 武器の位置のずれを管理する変数。
		Vector3 m_offsetPosition;                                                                                              //! 武器の位置のずれを管理する変数。
		Vector3 m_subWeaponOffset;                                                                                             //! サブ武器の位置のずれを管理する変数。
	};
}
