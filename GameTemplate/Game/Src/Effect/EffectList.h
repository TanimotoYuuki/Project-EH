#pragma once
/**
* @file   EffectList.h
* @brief  エフェクトを管理するリストクラス。
* @author YamaguchiHayato。
* @date   2026/03/23
*/



namespace nsApp
{
	namespace nsEffect 
	{
		/**
		* @enum  EffectID。
		* @brief エフェクトを識別子で管理する用の列挙型。
		*/
		enum Effect_ID : uint8_t
		{
			Attack,         //! 攻撃エフェクト。
			Charge,         //! チャージエフェクト。
			Fire,           //! 炎エフェクト。
			ShockWave,      //! 衝撃波エフェクト。

			NormalMagic,    //! 通常魔法エフェクト。
			RushMagic,      //! 連打魔法エフェクト。
			AirMagic,       //! 空中魔法エフェクト。
			HeelMagic,      //! 回復エフェクト。
			HeelMagic_Particle,
			MagicAttack,    //! 魔法攻撃エフェクト。
		};

		class EffectList
		{
		public:
			/* コンストラクタとデストラクタ。*/
			EffectList() = default;
			virtual ~EffectList() = default;


		public:
			/* エフェクトを初期化。*/
			void Init();

			/* 
			 * @def エフェクトを再生。
			 * @param id: エフェクトの識別子。
			 * @param position: エフェクトの出現位置。
			 * @param angle: エフェクトの回転角度。
			 * @param scale: エフェクトの拡大率。
			 */
			nsK2EngineLow::EffectEmitter* PlayEffect(Effect_ID id, const Vector3& position, const Quaternion& angle = Quaternion::Identity, const Vector3& scale = Vector3::One);


		public:
			/*
			 * @def ファイルパスを登録。
			 * @pararm name: エフェクトのファイル名。
			 */
			inline const std::u16string GetEffectFilePath(const std::u16string name)
			{
				const std::u16string path = u"Assets/effect/" + name + u".efk";
				return path;
			}


		private:
			/* 武器ごとにエフェクトを登録処理を分ける。*/
			/* Sword。*/
			void StorageGreatSwordEffect();

			/* Hammer。*/
			void StorageHammerEffect();

			/* Wand。*/
			void StorageWandEffect();



		private:
			std::unordered_map<Effect_ID, std::u16string> m_effectPathList; //! エフェクトの識別子とファイルパスを管理するマップ。

			EffectEmitter* m_effectEmitter = nullptr;						//! エフェクトクラスのインスタンス。
		};
	}
}
