#include "stdafx.h"
#include "CharacterHP.h"
#include "Src/Actor/Character/Player/Player.h"

namespace {
	/* HPバーUI（1人目のキャラクターをベースに）。*/
	const float   HP_BAR_UI_WIDTH = 1024;											//! HPバーUIの幅。
	const float   HP_BAR_UI_HEIGHT = 256;											//! HPバーUIの高さ。
	const Vector3 HP_BAR_UI_INIT_POSITION = Vector3(-660.0f, -450.0f, 0.0f);		//! HPバーUIの初期位置。
	const float   HP_BAR_UI_POSITION_INTERVAL = 425.0f;								//! HPバーUIの位置の間隔。
	const Vector3 HP_BAR_UI_INIT_SCALE = Vector3(0.425f, 0.425f, 1.0f);				//! HPバーUIの初期大きさ。

	/* ダメージゲージUI（1人目のキャラクターをベースに）。*/
	const float   DAMAGE_GAUGE_UI_WIDTH = 682;									    //! ダメージゲージUIの幅。
	const float   DAMAGE_GAUGE_UI_HEIGHT = 61;										//! ダメージゲージUIの高さ。
	const Vector3 DAMAGE_GAUGE_UI_INIT_POSITION = Vector3(-760.5f, -461.0f, 0.0f);  //! ダメージゲージUIの初期位置。
	const float   DAMAGE_GAUGE_UI_POSITION_INTERVAL = 425.0f;						//! ダメージゲージUIの位置の間隔。
	const Vector3 DAMAGE_GAUGE_UI_INIT_SCALE = Vector3(0.425f, 0.425f, 1.0f);		//! ダメージゲージUIの初期大きさ。
	const Vector2 DAMAGE_GAUGE_UI_INIT_PIVOT = Vector2(0.0f, 0.5f);					//! ダメージゲージUIの初期ピボット。
	const Vector4 DAMAGE_GAUGE_UI_INIT_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.5f); //! ダメージゲージUIの初期乗算色。

	/* HPゲージUI（1人目のキャラクターをベースに）。*/
	const float   HP_GAUGE_UI_WIDTH = 682;											//! HPゲージUIの幅。
	const float   HP_GAUGE_UI_HEIGHT = 61;                                          //! HPゲージUIの高さ。
	const Vector3 HP_GAUGE_UI_INIT_POSITION = Vector3(-760.5f, -461.0f, 0.0f);		//! HPゲージUIの初期位置。
	const float   HP_GAUGE_UI_POSITION_INTERVAL = 425.0f;							//! HPゲージUIの位置の間隔。
	const Vector3 HP_GAUGE_UI_INIT_SCALE = Vector3(0.425f, 0.425f, 1.0f);			//! HPゲージUIの初期大きさ。
	const Vector2 HP_GAUGE_UI_INIT_PIVOT = Vector2(0.0f, 0.5f);						//! HPゲージUIの初期ピボット。

	/* 役割UI（1人目のキャラクターをベースに）。*/
	const float   ROLE_UI_WIDTH = 1024;												//! 役割UIの幅。
	const float   ROLE_UI_HEIGHT = 1024;											//! 役割UIの高さ。
	const Vector3 ROLE_UI_INIT_POSITION = Vector3(-810.0f, -453.65f, 0.0f);			//! 役割UIの初期位置。
	const float   ROLE_UI_POSITION_INTERVAL = 425.0f;								//! 役割UIの位置の間隔。
	const Vector3 ROLE_UI_INIT_SCALE = Vector3(0.065f, 0.065f, 1.0f);				//! 役割UIの初期大きさ。

	/* HP。*/
	const int HP_MAX = 100; //! UIスケール用のHP最大値。

	/* UIアニメーション。*/
	const float DAMAGE_GAUGE_SCALE_UI_ANIMATION_PLAY_SPEED = 1.5f;					//! ダメージゲージUIアニメーションの再生速度。
}

namespace nsApp
{
	namespace nsGame
	{
		bool CharacterHP::Start()
		{
			/* スプライトの初期化。*/
			InitSprite();

			for (int i = 0; i < enCharacter_Num; i++)
			{
				/* UIアニメーションの初期化。*/
				InitUIAnimation((EnCharacter)i);

				/* キャラクターのHPの設定。*/
				m_characterHP[i] = HP_MAX;
				m_characterMaxHP[i] = HP_MAX;
			}

			return true;
		}


		void CharacterHP::Update()
		{
			/* プレイヤーの実HP比率をUIのHPスケールに変換して同期する。*/
			for (int i = 0; i < enCharacter_Num; i++)
			{
				if (m_players[i] == nullptr)
					continue;

				const auto& hp = m_players[i]->GetCharacterStatus().hp;

				if (hp.maxHP <= 0)
					continue;

				/* プレイヤーのHP比率をUIスケール（HP_MAX=100）に変換する。*/
				int uiHP = static_cast<int>(static_cast<float>(hp.currentHP) / static_cast<float>(hp.maxHP) * HP_MAX);
				if (uiHP < 0)
					uiHP = 0;

				const EnCharacter character = static_cast<EnCharacter>(i);

				/* 前フレームより下がっていた分をダメージとして設定する。*/
				int delta = m_characterHP[i] - uiHP;
				if (delta > 0)
					SetCharacterDamage(character, delta);

				/* 前フレームより上がっていた分は回復・復活として UI を即同期する。*/
				else if (uiHP > m_characterHP[i])
					RefillCharacterHP(character, uiHP);
			}

			/* キャラクターごとのHP、UIアニメーション、スプライトの更新。*/
			for (int i = 0; i < enCharacter_Num; i++)
			{
				/* HPの更新処理。*/
				UpdateHP((EnCharacter)i);

				/* UIアニメーションの更新処理。*/
				UpdateUIAnimation((EnCharacter)i);

				/* スプライトの更新処理。*/
				UpdateSprite((EnCharacter)i);
			}
		}

		void CharacterHP::Render(RenderContext& rc)
		{
			for (int i = 0; i < enCharacter_Num; i++)
			{
				/* HPバーUIの描画。*/
				m_hpBarUI[i].Draw(rc);

				/* ダメージゲージUIの描画。*/
				m_damageGaugeUI[i].Draw(rc);

				/* HPゲージUIの描画。*/
				m_hpGaugeUI[i].Draw(rc);

				/* 役割UIの描画。*/
				m_roleUI[i].Draw(rc);
			}
		}


		void CharacterHP::SetPlayers(const std::vector<nsActor::Player*>& players)
		{
			for (int i = 0; i < enCharacter_Num; i++)
				m_players[i] = (i < static_cast<int>(players.size())) ? players[i] : nullptr;
		}


		void CharacterHP::InitSprite()
		{
			for (int i = 0; i < enCharacter_Num; i++)
			{
				/* HPバーUI。*/
				InitHPBarUI((EnCharacter)i, i);

				/* ダメージゲージUI。*/
				InitDamageGaugeUI((EnCharacter)i, i);

				/* HPゲージUI。*/
				InitHPGaugeUI((EnCharacter)i, i);

				/* 役割UI。*/
				InitRoleUI((EnCharacter)i, i);
			}
		}


		void CharacterHP::InitHPBarUI(EnCharacter character, int characterIndex)
		{
			Vector3 initPosition = HP_BAR_UI_INIT_POSITION;
			initPosition.x += HP_BAR_UI_POSITION_INTERVAL * characterIndex;

			m_hpBarUI[character].Init(m_hpBarUIFilePath[character].c_str(), HP_BAR_UI_WIDTH, HP_BAR_UI_HEIGHT);
			m_hpBarUI[character].SetPosition(initPosition);
			m_hpBarUI[character].SetScale(HP_BAR_UI_INIT_SCALE);
			m_hpBarUI[character].Update();
		}


		void CharacterHP::InitDamageGaugeUI(EnCharacter character, int characterIndex)
		{
			Vector3 initPosition = DAMAGE_GAUGE_UI_INIT_POSITION;
			initPosition.x += DAMAGE_GAUGE_UI_POSITION_INTERVAL * characterIndex;

			m_damageGaugeUI[character].Init(m_damageGaugeUIFilePath.c_str(), DAMAGE_GAUGE_UI_WIDTH, DAMAGE_GAUGE_UI_HEIGHT);
			m_damageGaugeUI[character].SetPosition(initPosition);
			m_damageGaugeUI[character].SetScale(DAMAGE_GAUGE_UI_INIT_SCALE);
			m_damageGaugeUI[character].SetPivot(DAMAGE_GAUGE_UI_INIT_PIVOT);
			m_damageGaugeUI[character].SetMulColor(DAMAGE_GAUGE_UI_INIT_MUL_COLOR);
			m_damageGaugeUI[character].Update();
		}


		void CharacterHP::InitHPGaugeUI(EnCharacter character, int characterIndex)
		{
			Vector3 initPosition = HP_GAUGE_UI_INIT_POSITION;
			initPosition.x += HP_GAUGE_UI_POSITION_INTERVAL * characterIndex;

			m_hpGaugeUI[character].Init(m_hpGaugeUIFilePath.c_str(), HP_GAUGE_UI_WIDTH, HP_GAUGE_UI_HEIGHT);
			m_hpGaugeUI[character].SetPosition(initPosition);
			m_hpGaugeUI[character].SetScale(HP_GAUGE_UI_INIT_SCALE);
			m_hpGaugeUI[character].SetPivot(HP_GAUGE_UI_INIT_PIVOT);
			m_hpGaugeUI[character].Update();

			m_hpGaugeUIBaseScale[character] = m_hpGaugeUI[character].GetScale();
		}


		void CharacterHP::InitRoleUI(EnCharacter character, int characterIndex)
		{
			Vector3 initPosition = ROLE_UI_INIT_POSITION;
			initPosition.x += ROLE_UI_POSITION_INTERVAL * characterIndex;

			m_roleUI[character].Init(m_roleUIFilePath[GetCharacterRole(characterIndex)].c_str(), ROLE_UI_WIDTH, ROLE_UI_HEIGHT);
			m_roleUI[character].SetPosition(initPosition);
			m_roleUI[character].SetScale(ROLE_UI_INIT_SCALE);
			m_roleUI[character].Update();
		}


		void CharacterHP::InitUIAnimation(EnCharacter character)
		{
			Vector2 baseScale = { m_damageGaugeUI[character].GetScale().x, m_damageGaugeUI[character].GetScale().y };
			Vector2 targetScale = { m_damageGaugeUI[character].GetScale().x, m_damageGaugeUI[character].GetScale().y };

			m_damageGaugeScaleUIAnimation[character] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_damageGaugeUI[character],
				1.0f,
				DAMAGE_GAUGE_SCALE_UI_ANIMATION_PLAY_SPEED,
				false,
				0.0f,
				0.0f,
				baseScale,
				targetScale
			);
		}


		void CharacterHP::UpdateHP(EnCharacter character)
		{
			if (m_characterHP[character] <= 0) { return; } /* HPがなければ処理しない。*/
			if (m_characterDamage[character] <= 0) { return; } /* ダメージがなければ処理しない。*/

			m_characterHP[character] -= m_characterDamage[character];

			float hpRate = (float)m_characterHP[character] / (float)m_characterMaxHP[character];
			Vector3 hpGaugeScale = m_hpGaugeUIBaseScale[character];
			hpGaugeScale.x *= hpRate;
			m_hpGaugeUI[character].SetScale(hpGaugeScale);

			m_characterDamage[character] = 0;

			Vector2 baseScale = { m_damageGaugeUI[character].GetScale().x, m_damageGaugeUI[character].GetScale().y };
			Vector2 targetScale = { m_hpGaugeUI[character].GetScale().x,     m_hpGaugeUI[character].GetScale().y };

			m_damageGaugeScaleUIAnimation[character]->Reset();
			m_damageGaugeScaleUIAnimation[character]->SetBaseScale(baseScale);
			m_damageGaugeScaleUIAnimation[character]->SetTargetScale(targetScale);
		}


		void CharacterHP::UpdateUIAnimation(EnCharacter character)
		{
			m_damageGaugeScaleUIAnimation[character]->Update();
		}


		void CharacterHP::UpdateSprite(EnCharacter character)
		{
			m_hpBarUI[character].Update();
			m_damageGaugeUI[character].Update();
			m_hpGaugeUI[character].Update();
			m_roleUI[character].Update();
		}


		void CharacterHP::RefillCharacterHP(EnCharacter character, int uiHP)
		{
			/* UI スケール値を有効範囲に収める。*/
			if (uiHP < 0)
				uiHP = 0;
			if (uiHP > m_characterMaxHP[character])
				uiHP = m_characterMaxHP[character];

			/* UI 上の HP を更新する。*/
			m_characterHP[character] = uiHP;

			/* 未処理のダメージ演出をクリアする。*/
			m_characterDamage[character] = 0;

			/* HP ゲージのスケールを計算する。*/
			float hpRate = (float)m_characterHP[character] / (float)m_characterMaxHP[character];
			Vector3 hpGaugeScale = m_hpGaugeUIBaseScale[character];
			hpGaugeScale.x *= hpRate;

			/* HP ゲージとダメージゲージを同じスケールに揃える（復活時は満タン表示）。*/
			m_hpGaugeUI[character].SetScale(hpGaugeScale);
			m_damageGaugeUI[character].SetScale(hpGaugeScale);

			/* ダメージゲージのアニメーションをリセットする。*/
			if (m_damageGaugeScaleUIAnimation[character] != nullptr)
			{
				Vector2 scale = { hpGaugeScale.x, hpGaugeScale.y };
				m_damageGaugeScaleUIAnimation[character]->Reset();
				m_damageGaugeScaleUIAnimation[character]->SetBaseScale(scale);
				m_damageGaugeScaleUIAnimation[character]->SetTargetScale(scale);
			}
		}


		void CharacterHP::SyncPlayerHP(nsActor::Player* player)
		{
			if (player == nullptr)
				return;

			for (int i = 0; i < enCharacter_Num; i++)
			{
				if (m_players[i] != player)
					continue;

				const auto& hp = player->GetCharacterStatus().hp;
				if (hp.maxHP <= 0)
					return;

				int uiHP = static_cast<int>(
					static_cast<float>(hp.currentHP) / static_cast<float>(hp.maxHP) * HP_MAX);

				if (uiHP < 0)
					uiHP = 0;

				RefillCharacterHP(static_cast<EnCharacter>(i), uiHP);
				return;
			}
		}
	}
}