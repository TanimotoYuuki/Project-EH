#pragma once
/**
 * @file   EffectList.h
 * @brief  エフェクトを管理するリストクラス。
 * @author Yamaguchi Hayato
 * @date   2026/03/23
 */

namespace nsApp
{
	namespace nsEffect
	{
		/**
		 * @struct EffectInfo
		 * @brief  再生中のエフェクトの情報を管理する。
		 */
		struct EffectInfo
		{
			nsK2EngineLow::EffectEmitter* emitter; //! エフェクトのエミッター。
			float lifeTime;                        //! エフェクトの寿命。
			float currentTime;                     //! エフェクトの現在の経過時間。
		};

		/**
		 * @enum  Effect_ID
		 * @brief エフェクトを識別子で管理するための列挙型。
		 */
		enum Effect_ID : uint8_t
		{
			Attack,             //! 攻撃エフェクト。
			Charge,             //! チャージエフェクト。
			Fire,               //! チャージ攻撃時の炎エフェクト。
			ShockWave,          //! 着地時の衝撃波エフェクト。
			Hit,                //! 被弾エフェクト。

			NormalMagic,        //! 通常魔法エフェクト。
			RushMagic,          //! 突進魔法エフェクト。
			AirMagic,           //! 空中魔法エフェクト。
			HeelMagic,          //! 回復エフェクト。
			HeelMagic_Particle, //! 回復エフェクトのパーティクル。
			MagicAttack,        //! 魔法攻撃エフェクト。
			Shot,               //! 射撃エフェクト。

			FireBall,           //! ボスファイアボール着弾エフェクト。
			BossFireAttack,     //! ボス火炎弾着弾エフェクト（地面）。

			Guard_Blue,         //! ガード用バリア（青）。
			Guard_Red,          //! ガード用バリア（赤）。
		};

		/**
		 * @class EffectList
		 * @brief エフェクトの登録・再生・停止を管理するクラス。
		 */
		class EffectList
		{
		public:
			/* コンストラクタとデストラクタ。*/
			EffectList() = default;
			virtual ~EffectList();

		public:
			/**
			 * @brief エフェクトを初期化する。
			 */
			void Init();

			/**
			 * @brief エフェクトを更新する。
			 * @param deltaTime 前フレームからの経過時間。
			 */
			void Update(float deltaTime);

			/**
			 * @brief エフェクトのキャッシュを初期化する。
			 */
			void Clear();

			/**
			 * @brief 指定したエフェクトを停止/削除する。
			 * @param effect 停止/削除するエフェクトのインスタンス。
			 */
			void StopEffect(nsK2EngineLow::EffectEmitter* effect);

			/**
			 * @brief エフェクトを再生する。
			 * @param id       エフェクトの識別子。
			 * @param position エフェクトの出現位置。
			 * @param angle    エフェクトの回転角度。
			 * @param scale    エフェクトの拡大率。
			 * @param lifeTime エフェクトを維持する時間。
			 * @return 再生したエフェクトのエミッター。失敗時はnullptr。
			 */
			nsK2EngineLow::EffectEmitter* PlayEffect( Effect_ID id, const Vector3& position, const Quaternion& angle = Quaternion::Identity, const Vector3& scale = Vector3::One, float lifeTime = 2.0f);


		public:
			/**
			 * @brief エフェクトファイルのパスを取得する。
			 * @param name エフェクトのファイル名（拡張子なし）。
			 * @return エフェクトファイルのパス。
			 */
			inline const std::u16string GetEffectFilePath(const std::u16string name)
			{
				const std::u16string path = u"Assets/effect/" + name + u".efk";
				return path;
			}

		private:
			/* 武器ごとにエフェクト登録処理を分ける。*/

			/** @brief 大剣のエフェクトを登録する。 */
			void StorageGreatSwordEffect();

			/** @brief ハンマーのエフェクトを登録する。 */
			void StorageHammerEffect();

			/** @brief 杖のエフェクトを登録する。 */
			void StorageWandEffect();

			/** @brief 双銃のエフェクトを登録する。 */
			void StorageTwinGunEffect();

			/** @brief ボスのエフェクトを登録する。 */
			void StorageBossEffect();

			/** @brief ガード用エフェクトを登録する。 */
			void StorageGuardEffect();

		private:
			nsK2EngineLow::EffectEmitter* m_effectEmitter = nullptr; //! エフェクトのエミッターのインスタンス。

			std::unordered_map<Effect_ID, std::u16string> m_effectPathList; //! エフェクトの識別子とファイルパスを管理するマップ。
			std::vector<EffectInfo> m_playingEffects;                       //! 現在再生中のエフェクトの情報を管理するリスト。
			EffectInfo m_info;                                              //! エフェクトの情報を管理するインスタンス。
		};
	}
}