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
			Attack,  //! 攻撃エフェクト。
			Charge,  //! チャージエフェクト。
			Fire,    //! 炎エフェクト。
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

			/* エフェクトを再生。*/
			nsK2EngineLow::EffectEmitter* PlayEffect(Effect_ID id, const Vector3& position, const Quaternion& angle = Quaternion::Identity, const Vector3& scale = Vector3::One);


		public:
			/* ファイルパスを登録。*/
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


		private:
			std::unordered_map<Effect_ID, std::u16string> m_effectPathList;

			EffectEmitter* m_effectEmitter = nullptr;
		};
	}
}
