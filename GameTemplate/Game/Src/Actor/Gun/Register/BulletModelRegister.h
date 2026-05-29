#pragma once
/**
 * @file   BulletModelRegister.h
 * @brief  弾丸のモデルを登録するクラス。
 * @author Yamaguchi Hayato
 * @date 2026/04/30
 */


namespace nsApp
{
	/**
	 * @enum BulletType
	 * @brief 弾丸の種類を識別するための列挙型。
	 */
	enum class BulletType : uint8_t
	{
		enNormal,		//! 通常攻撃弾。     
		enCharge,		//! チャージ攻撃弾。
		enRush,			//! 乱射弾。
		enExplosive,	//! 重い攻撃弾。
		enAirial,		//! 空中攻撃弾。
		enDash,			//! ダッシュ攻撃弾。
		enNum,          //! 弾丸の種類。
		enNone,         //! null。
	};


	/**
	 * @struct BulletModelInfo
	 * @brief 弾丸モデルの種類を格納。
	 */
	struct BulletModelInfo
	{
		std::string  bulletModelName; //! 弾丸モデルの名前。
		float 	bulletScale;          //! 弾丸モデルのスケール。
	};


	class BulletModelRegister
	{
	public:
		/**
		 * @brief 弾丸の種類にに応じてモデルを情報を取得する。
		 * @param type 弾丸の種類。
     	 */
		static const BulletModelInfo& GetBulletModelInfo(BulletType type);


	private:
		/**
		 * @brief モデル名を定数化する。
		 * @param name モデル名。
		 * @return ファイルパスを格納したモデル名。
		 */
		inline static std::string GetBulletModelFilePath(const std::string name)
		{
			 return "Assets/modelData/Character/Weapon/" + name + ".tkm";
		}


	private:
		static const std::unordered_map<BulletType, BulletModelInfo> m_bulletTable; //! 弾丸の種類とモデルの情報を紐づけるテーブル。
	};
}
