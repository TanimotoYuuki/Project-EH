#pragma once
/**
* @file   CharacterModel.h
* @brief  キャラクターモデル/アニメーションを管理するクラス。
*         BossModel/Player1p～4pモデルを管理。
* @author Yamaguchi Hayato。
* @date   2026/05/29
*/


namespace nsApp
{
	enum class CharacterModelType
	{
		/* プレイヤーモデル1P～4Pを定義。*/
		Player_1P,           //! 1P。
		Player_2P,           //! 2P。
		Player_3P,           //! 3P。
		Player_4P,           //! 4P。


		/* 敵モデルを定義。*/
		TutorialBoss,        //! チュートリアル用のボス。
		GrayDragon,          //! 灰色のドラゴン。
		GreenDragon,         //! 緑色のドラゴン。
		RedDragon,           //! 赤色のドラゴン。


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
		/* 
		 * @brief モデルを読み込む。
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
		 * @param interpolateTime アニメーションの補間時間を指定する。
		 */
		inline void PlayAnimation(int animationNumber, float interpolateTime)
		{
			/* キャラモデルが確認できない場合、処理を終了させる。*/
			if (m_characterModelRender != nullptr)
				m_characterModelRender->PlayAnimation(animationNumber, interpolateTime);
		}

		/**
		 * @brief キャラモデルを描画する。
		 * @param rc 描画に必要な情報を管理するクラスの参照を渡す。
		 */
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


	/* セッター。*/
	public:
		/**
		 * @brief キャラモデルの座標を設定する。
		 * @param position 設定したい座標を指定する。
		 */
		inline void SetPosition(const Vector3& position)
		{
			if (m_characterModelRender)
				m_characterModelRender->SetPosition(position);
		}

		/**
		 * @brief キャラモデルの回転を設定する。
		 * @param rotation 設定したい回転をクォータニオンで指定する。
		 */
		inline void SettRotation(const Quaternion& rotation)
		{
			if (m_characterModelRender)
				m_characterModelRender->SetRotation(rotation);
		}

		/**
		 * @brief キャラモデルの大きさを設定する。
		 * @param scale 設定したい大きさを指定する。
		 */
		inline void SetCharacterScale(const Vector3& scale)
		{
			if (m_characterModelRender)
				m_characterModelRender->SetScale(scale);
		}

		/**
		 * @brief 武器の大きさを設定する。
		 * @param scale 設定したい大きさを指定する。
		 */
		inline void SetWeaponScale(const Vector3& scale)
		{
			if (m_weaponModelRender)
				m_weaponModelRender->SetScale(scale);
		}

		/**
		 * @brief 武器の座標のずれを設定する。
		 * @param offset 設定したい座標のずれを指定する。
		 */
		inline void SetWeaponOffset(const Vector3& offset)
		{
			m_weaponOffset = offset;
		}

		/**
		 * @brief 武器を装備させるボーンの名前を設定する。
		 * @param boneName 設定したいボーンの名前を指定する。
		 */
		inline void SetWeaponAttackBone(const std::wstring& boneName)
		{
			m_attackBoneName = boneName;
		}

		/**
		 * @brief 武器の回転のずれを設定する。
		 * @param offset 設定したい回転のずれをクォータニオンで指定する。
		 */
		inline void SetWeaponAngle(const Quaternion offset)
		{
			m_weaponRotationOffset = offset;
		}


		/* ゲッター。*/
	public:
		/**
		 * @brief キャラモデルのファイルパスを格納。
		 * @param filePath 格納したいファイルパスを指定する。
		 * @return キャラモデルのファイルパスを返す。
		 */
		inline const std::string GetCharacterModelFilePath(std::string filePath)
		{
			std::string modelPath = m_characterModelFilePath + filePath + m_modelExtension;
			return modelPath;
		}

		/**
		 * @brief ボスモデルのファイルパスを格納。
		 * @param filePath 格納したいファイルパスを指定する。
		 * @return ボスモデルのファイルパスを返す。
		 */
		inline const std::string GetBossModelFilePath(const std::string& filePath)
		{
			return m_modelBossModelFilePath + "/" + filePath + ".tkm";
		}

		/**
		 * @brief 武器モデルのファイルパスを格納。
		 * @param filePath 格納したいファイルパスを指定する。
		 * @return 武器モデルのファイルパスを返す。
		 */
		inline const std::string GetWeaponModelFilePath(std::string filePath)
		{
			std::string weaponFilePath = m_weaponModelFilePath + filePath + m_modelExtension;
			return weaponFilePath;
		}

		/**
		 * 指定したボーンのワールド行列を取得する。
		 * @param boneName 取得したいボーンの名前を指定する。
		 */
		Matrix GetWorldMatrix(const wchar_t* boneName);

		/**
		 * @brief 武器の座標を取得する。
		 * @return 武器の座標を返す。
		 */
		inline const Vector3& GetWeaponPosition() const
		{
			return m_offsetPosition;
		}


	private:
		std::unordered_map<CharacterModelType, std::string> m_filePathList;		              //! モデルIDからファイルパスを文字列に変化。
		std::unique_ptr<ModelRender> m_characterModelRender;                                  //! モデルを管理。
		std::unique_ptr<ModelRender> m_weaponModelRender;                                     //! 武器モデルを管理。
		std::unique_ptr<ModelRender> m_subWeaponModelRender;                                  //! サブ武器モデルを管理。
																							
		std::wstring m_attackBoneName = L"mixamorig:RightHand";                               //! デフォルトのボーン名。
		std::wstring m_subWeaponBoneName = L"mixamorig:LeftHand";                             //! サブ武器を装備するボーン名。
																							
		std::string m_modelFilePath;                                                          //! モデルのファイルパスを格納。
		std::string m_characterModelFilePath = "Assets/modelData/Character/CharacterModel/";  //! プレイヤー/NPCモデルのファイルパスを格納。
		std::string m_modelExtension = ".tkm";                                                //! プレイヤー/NPCモデルの拡張子を格納。
		std::string m_modelBossModelFilePath = "Assets/modelData/Boss";						
		std::string m_weaponModelFilePath = "Assets/modelData/Character/Weapon/";             //! 武器モデルのファイルパスを格納。
																							
		int m_boneID = 0;                                                                     //! ボーンIDを格納。
																													
		Quaternion m_matrixRotation = Quaternion::Identity;									  //! ボーンの行列から抽出した回転を管理する変数。
		Quaternion m_weaponRotationOffset = Quaternion::Identity;                             //! 武器の傾きを制御。
																							  
		Matrix m_handMatrix; 																  //! 武器を装備させるときの右手のボーンの行列を管理する変数。
		Matrix m_rotationMatrix;                                                              //! 武器を装備させたときの武器の傾きを制御。
		Matrix m_subWeaponHandMatrix;														  
																							  
		Vector3 m_xAxis = Vector3::Zero;                                                      //! 武器を装備させるときの右手のボーンの行列から抽出したX軸を管理する変数。
		Vector3 m_yAxis = Vector3::Zero;                                                      //! 武器を装備させるときの右手のボーンの行列から抽出したY軸を管理する変数。
		Vector3 m_zAxis = Vector3::Zero;                                                      //! 武器を装備させるときの右手のボーンの行列から抽出したZ軸を管理する変数。
		Vector3 m_matrixPosition = Vector3::Zero;											  
		Vector3 m_weaponOffset = Vector3::Zero;                                               //! 武器の位置のずれを管理する変数。
		Vector3 m_offsetPosition = Vector3::Zero;                                             //! 武器の位置のずれを管理する変数。
		Vector3 m_subWeaponOffset = Vector3::Zero;                                            //! サブ武器の位置のずれを管理する変数。
	};
}
