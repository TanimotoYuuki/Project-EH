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
		/*
		 * @struct EffectInfo。
		 * @brief 再生するエフェクトの情報を管理する。
	     */
		struct EffectInfo
		{
			nsK2EngineLow::EffectEmitter* emitter;	//! エフェクトのエミッタ。
			float lifeTime;						    //! エフェクトの寿命。
			float currentTime;						//! エフェクトの現在の経過時間。
		};


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
			Shot,			//! 銃撃エフェクト。
		};

		class EffectList
		{
		public:
			/* コンストラクタとデストラクタ。*/
			EffectList() = default;
			virtual ~EffectList();


		public:
			/* エフェクトを初期化。*/
			void Init();

			/*
			 * @brief エフェクトを更新する。
			 * @param deltaTime: 前フレームからの経過時間。
			 */
			void Update(float deltaTime);

			/*
			 * @brief　エフェクトのキャッシュを解放する。 
			 */
			void Clear();

			/*
			 * @brief 指定したエフェクトを停止/削除する。
			 * @param effect: 停止/削除するエフェクトのインスタンス。
			 */
			void StopEffect(nsK2EngineLow::EffectEmitter* effect);

			/* 
			 * @def エフェクトを再生。
			 * @param id: エフェクトの識別子。
			 * @param position: エフェクトの出現位置。
			 * @param angle: エフェクトの回転角度。
			 * @param scale: エフェクトの拡大率。
			 * @param lifeTime: エフェクトを何秒描画するか。
			 */
			nsK2EngineLow::EffectEmitter* PlayEffect(Effect_ID id, const Vector3& position, const Quaternion& angle = Quaternion::Identity, const Vector3& scale = Vector3::One, float lifeTime = 2.0f);


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

			/* TwinGun。*/
			void StorageTwinGunEffect();


		private:
			nsK2EngineLow::EffectEmitter* m_effectEmitter;					//! エフェクトのエミッタのインスタンス。


		private:
			std::unordered_map<Effect_ID, std::u16string> m_effectPathList; //! エフェクトの識別子とファイルパスを管理するマップ。

			std::vector<EffectInfo> m_playingEffects;					    //! 現在再生中のエフェクトの情報を管理するリスト。

			EffectInfo m_info;												//! エフェクトの情報を管理するインスタンス。
		};
	}
}
