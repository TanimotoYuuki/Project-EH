#pragma once
#include "Src/UIAnimation/UIAnimation.h"
#include <vector>

/**
 * @file  CharacterHP.h
 * @brief キャラクターHPバーUIを管理するクラス。
 * @author Tanimoto
 * @date  2026/06/21: 更新日。
 */

namespace nsApp { namespace nsActor { class Player; } }

namespace nsApp
{
	namespace nsGame
	{
		/**
		 * @class CharacterHP
		 * @brief 各プレイヤーのHPバーUIを管理するクラス。
		 */
		class CharacterHP : public IGameObject
		{
		public:
			/* コンストラクタとデストラクタ。*/
			CharacterHP() = default;
			virtual ~CharacterHP() = default;


		public:
			/**
			 * @brief 開始処理。
			 * @return 初期化に成功した場合 true。
			 */
			bool Start() override;

			/**
			 * @brief 更新処理。
			 */
			void Update() override;

			/**
			 * @brief 描画処理。
			 * @param rc 描画コンテキスト。
			 */
			void Render(RenderContext& rc) override;


		public:
			/* キャラクター列挙型。*/
			enum EnCharacter : uint8_t
			{
				enCharacter_One,   //! 1人目のキャラクター。
				enCharacter_Two,   //! 2人目のキャラクター。
				enCharacter_Three, //! 3人目のキャラクター。
				enCharacter_Four,  //! 4人目のキャラクター。
				enCharacter_Num    //! キャラクターの数。
			};

			/* 役割列挙型。*/
			enum EnRole : uint8_t
			{
				enRole_Sword,  //! ソード。
				enRole_Hammer, //! ハンマー。
				enRole_Mage,   //! メイジ。
				enRole_Gunner, //! ガンナー。
				enRole_Num     //! 役割の数。
			};


		public:
			/**
			 * @brief プレイヤーのポインタリストを設定する。
			 *        Update() でプレイヤーの currentHP を参照し UI に同期する。
			 * @param players プレイヤーポインタのリスト。
			 */
			void SetPlayers(const std::vector<nsActor::Player*>& players);

			/**
			 * @brief キャラクターのHPを回復・復活させる。
			 */
			void RefillCharacterHP(EnCharacter character, int uiHP);

			/**
			 * @brief 指定プレイヤーの実 HP から UI ゲージを即同期する。
			 *        蘇生・回復直後に呼ぶ。
			 * @param player 同期対象のプレイヤー。
			 */
			void SyncPlayerHP(nsActor::Player* player);

			/**
			 * @brief キャラクターのダメージを設定する。
			 * @param character キャラクター。
			 * @param damage    ダメージ量。
			 */
			inline void SetCharacterDamage(EnCharacter character, int damage)
			{
				m_characterDamage[character] = damage;
			}

			/**
			 * @brief キャラクターの役割を設定する。
			 * @param playerIndex プレイヤーのインデックス。
			 * @param role        役割。
			 */
			inline void SetCharacterRole(int playerIndex, int role)
			{
				m_characterRole[playerIndex] = role;
			}

			/**
			 * @brief キャラクターの役割を取得する。
			 * @param playerIndex プレイヤーのインデックス。
			 * @return キャラクターの役割。
			 */
			inline int GetCharacterRole(int playerIndex) const
			{
				return m_characterRole[playerIndex];
			}


		private:
			/**
			 * @brief スプライトを一括初期化する。
			 */
			void InitSprite();

			/**
			 * @brief HPバーUIを初期化する。
			 * @param character      キャラクター。
			 * @param characterIndex キャラクターのインデックス。
			 */
			void InitHPBarUI(EnCharacter character, int characterIndex);

			/**
			 * @brief ダメージゲージUIを初期化する。
			 * @param character      キャラクター。
			 * @param characterIndex キャラクターのインデックス。
			 */
			void InitDamageGaugeUI(EnCharacter character, int characterIndex);

			/**
			 * @brief HPゲージUIを初期化する。
			 * @param character      キャラクター。
			 * @param characterIndex キャラクターのインデックス。
			 */
			void InitHPGaugeUI(EnCharacter character, int characterIndex);

			/**
			 * @brief 役割UIを初期化する。
			 * @param character      キャラクター。
			 * @param characterIndex キャラクターのインデックス。
			 */
			void InitRoleUI(EnCharacter character, int characterIndex);

			/**
			 * @brief UIアニメーションを初期化する。
			 * @param character キャラクター。
			 */
			void InitUIAnimation(EnCharacter character);

			/**
			 * @brief HPゲージを更新する。
			 * @param character キャラクター。
			 */
			void UpdateHP(EnCharacter character);

			/**
			 * @brief UIアニメーションを更新する。
			 * @param character キャラクター。
			 */
			void UpdateUIAnimation(EnCharacter character);

			/**
			 * @brief スプライトを更新する。
			 * @param character キャラクター。
			 */
			void UpdateSprite(EnCharacter character);


		private:
			/* スプライト型。*/
			SpriteRender m_hpBarUI[enCharacter_Num];       //! HPバーUI。
			SpriteRender m_damageGaugeUI[enCharacter_Num]; //! ダメージゲージUI。
			SpriteRender m_hpGaugeUI[enCharacter_Num];     //! HPゲージUI。
			SpriteRender m_roleUI[enCharacter_Num];        //! 役割アイコンUI。

			/* Vector3 型。*/
			Vector3 m_hpBarUIScale[enCharacter_Num];       //! HPバーUIの大きさ。
			Vector3 m_damageGaugeUIScale[enCharacter_Num]; //! ダメージゲージUIの大きさ。
			Vector3 m_hpGaugeUIScale[enCharacter_Num];     //! HPゲージUIの大きさ。
			Vector3 m_hpGaugeUIBaseScale[enCharacter_Num]; //! HPゲージUIの基準の大きさ。

			/* int 型。*/
			int m_characterHP[enCharacter_Num] = { 0, 0, 0, 0 };     //! キャラクターのHP。
			int m_characterMaxHP[enCharacter_Num] = { 0, 0, 0, 0 };  //! キャラクターの最大HP。
			int m_characterDamage[enCharacter_Num] = { 0, 0, 0, 0 }; //! キャラクターのダメージ。
			int m_characterRole[enCharacter_Num] = { 0, 0, 0, 0 };   //! キャラクターの役割。

			/* ポインタ型。*/
			nsActor::Player* m_players[enCharacter_Num] = {}; //! 各キャラクターのプレイヤーポインタ。

			/* unique_ptr 型。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation>
				m_damageGaugeScaleUIAnimation[enCharacter_Num]; //! ダメージゲージUIのスケールアニメーション。


		private:
			/* ファイルパス。*/
			std::string m_hpBarUIFilePath[enCharacter_Num] = {
				"Assets/sprite/inGame/hp/bar/onePlayer.dds",
				"Assets/sprite/inGame/hp/bar/twoPlayer.dds",
				"Assets/sprite/inGame/hp/bar/threePlayer.dds",
				"Assets/sprite/inGame/hp/bar/fourPlayer.dds"
			}; //! HPバーUIのファイルパス。

			std::string m_damageGaugeUIFilePath =
				"Assets/sprite/inGame/hp/gauge/damageGauge.dds"; //! ダメージゲージUIのファイルパス。

			std::string m_hpGaugeUIFilePath =
				"Assets/sprite/inGame/hp/gauge/hpGauge.dds"; //! HPゲージUIのファイルパス。

			std::string m_roleUIFilePath[enRole_Num] = {
				"Assets/sprite/role/sword.dds",
				"Assets/sprite/role/hammer.dds",
				"Assets/sprite/role/mage.dds",
				"Assets/sprite/role/gunner.dds",
			}; //! 役割UIのファイルパス。
		};
	}
}