#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file CharacterHP.h。
 * @brief キャラクターHPクラス。
 * @author Tanimoto。
 * @date 2026/03/20。
 */
namespace nsApp
{
	namespace nsGame
	{
		/*キャラクターHPクラス。*/
		class CharacterHP : public IGameObject
		{
		public:
			CharacterHP() {};/*コンストラクタ。*/
			~CharacterHP() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*キャラクター。*/
			enum EnCharacter : uint8_t
			{
				enCharacter_One,/*1人目のキャラクター。*/
				enCharacter_Two,/*2人目のキャラクター。*/
				enCharacter_Three,/*3人目のキャラクター。*/
				enCharacter_Four,/*4人目のキャラクター。*/
				enCharacter_Num/*キャラクターの数。*/
			};

			/*役割。*/
			enum EnRole : uint8_t
			{
				enRole_Sword,/*ソード。*/
				enRole_Hammer,/*ハンマー。*/
				enRole_Mage,/*メイジ。*/
				enRole_Gunner,/*ガンナー。*/
				enRole_Num/*役割の数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief HPバーUIの初期化。
			* @param character キャラクター。
			* @param characterIndex キャラクターのインデックス。
			*/
			void InitHPBarUI(EnCharacter character,int characterIndex);

			/**
			* @brief ダメージゲージUIの初期化。
			* @param character キャラクター。
			* @param characterIndex キャラクターのインデックス。
			*/
			void InitDamageGaugeUI(EnCharacter character, int characterIndex);

			/**
			* @brief HPゲージUIの初期化。
			* @param character キャラクター。
			* @param characterIndex キャラクターのインデックス。
			*/
			void InitHPGaugeUI(EnCharacter character, int characterIndex);

			/**
			* @brief 役割UIの初期化。
			* @param character キャラクター。
			* @param role 役割。
			* @param characterIndex キャラクターのインデックス。
			*/
			void InitRoleUI(EnCharacter character, int characterIndex);

			/**
			* @brief UIアニメーションの初期化。
			* @param character キャラクター。
			*/
			void InitUIAnimation(EnCharacter character);

			/**
			* @brief HPの更新処理。
			* @param character キャラクター。
			*/
			void UpdateHP(EnCharacter character);

			/**
			* @brief UIアニメーションの更新処理。
			* @param character キャラクター。
			*/
			void UpdateUIAnimation(EnCharacter character);

			/**
			* @brief スプライトの更新処理。
			* @param character キャラクター。
			*/
			void UpdateSprite(EnCharacter character);

		public:/*メンバ関数。*/

			/**
			* @brief *キャラクターのダメージの設定。
			* @param character キャラクター。
			* @param damage ダメージ。
			*/
			inline void SetCharacterDamage(EnCharacter character, int damage)
			{
				m_characterDamage[character] = damage;
			}

			/**
			* @brief 各キャラクターの役割の設定。
			* @param playerIndex プレイヤーのインデックス。
			*/
			inline void SetCharacterRole(int playerIndex,int role)
			{
				m_characterRole[playerIndex] = role;
			}

			/**
			* @brief 各キャラクターの役割の取得。
			* @param playerIndex プレイヤーのインデックス。
			* @return キャラクターの役割。
			*/
			inline int GetCharacterRole(int playerIndex)const
			{
				return m_characterRole[playerIndex];
			}

		private:/*メンバ変数。*/
			SpriteRender m_hpBarUI[enCharacter_Num];/*HPバーUI。*/
			Vector3 m_hpBarUIScale[enCharacter_Num];/*HPバーUIの大きさ。*/
			SpriteRender m_damageGaugeUI[enCharacter_Num];/*ダメージゲージUI。*/
			Vector3 m_damageGaugeUIScale[enCharacter_Num];/*ダメージゲージUIの大きさ。*/
			SpriteRender m_hpGaugeUI[enCharacter_Num];/*HPゲージUI。*/
			Vector3 m_hpGaugeUIScale[enCharacter_Num];/*HPゲージUIの大きさ。*/
			Vector3 m_hpGaugeUIBaseScale[enCharacter_Num];/*HPゲージUIの基準の大きさ。*/
			SpriteRender m_roleUI[enCharacter_Num];/*役割アイコンUI。*/
			int m_characterHP[enCharacter_Num] = { 0,0,0,0 };/*キャラクターのHP。*/
			int m_characterMaxHP[enCharacter_Num] = { 0,0,0,0 };/*キャラクターの最大HP。*/
			int m_characterDamage[enCharacter_Num] = { 0,0,0,0 };/*キャラクターのダメージ。*/
			int m_characterRole[enCharacter_Num] = { 0,0,0,0 };/*キャラクターの役割。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_damageGaugeScaleUIAnimation[enCharacter_Num];/*ダメージゲージUIの大きさを変えるアニメーション。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_hpBarUIFilePath[enCharacter_Num] = {
				"Assets/sprite/inGame/hp/bar/onePlayer.dds",
				"Assets/sprite/inGame/hp/bar/twoPlayer.dds",
				"Assets/sprite/inGame/hp/bar/threePlayer.dds",
				"Assets/sprite/inGame/hp/bar/fourPlayer.dds"
			};/*HPバーUIのファイルパス。*/
			std::string m_damageGaugeUIFilePath = "Assets/sprite/inGame/hp/gauge/damageGauge.dds";/*ダメージゲージUIのファイルパス。*/
			std::string m_hpGaugeUIFilePath = "Assets/sprite/inGame/hp/gauge/hpGauge.dds";/*HPゲージUIのファイルパス。*/
			std::string m_roleUIFilePath[enRole_Num] = {
				"Assets/sprite/role/sword.dds",/*ソード。*/
				"Assets/sprite/role/hammer.dds",/*ハンマー。*/
				"Assets/sprite/role/mage.dds",/*メイジ。*/
				"Assets/sprite/role/gunner.dds",/*ガンナー。*/
			};/*役割UIのファイルパス。*/
		};
	}
}

