#include "stdafx.h"
#include "CharacterHP.h"

namespace {
	/*HPバーUI(1人目のキャラクターをベースに)。*/
	const float HP_BAR_UI_WIDTH = 1024; /*HPバーUIの幅。*/

	const float HP_BAR_UI_HEIGHT = 256; /*HPバーUIの高さ。*/

	const Vector3 HP_BAR_UI_INIT_POSITION = Vector3(-660.0f, -450.0f, 0.0f); /*HPバーUIの初期位置。*/

	const float HP_BAR_UI_POSITION_INTERVAL = 425.0f; /*HPバーUIの位置の間隔。*/

	const Vector3 HP_BAR_UI_INIT_SCALE = Vector3(0.425f, 0.425f, 1.0f); /*HPバーUIの初期大きさ。*/

	/*ダメージゲージUI(1人目のキャラクターをベースに)。*/
	const float DAMAGE_GAUGE_UI_WIDTH = 682; /*ダメージゲージUIの幅。*/

	const float DAMAGE_GAUGE_UI_HEIGHT = 61; /*ダメージゲージUIの高さ。*/

	const Vector3 DAMAGE_GAUGE_UI_INIT_POSITION = Vector3(-760.5f, -461.0f, 0.0f); /*ダメージゲージUIの初期位置。*/

	const float DAMAGE_GAUGE_UI_POSITION_INTERVAL = 425.0f; /*ダメージゲージUIの位置の間隔。*/

	const Vector3 DAMAGE_GAUGE_UI_INIT_SCALE = Vector3(0.425f, 0.425f, 1.0f); /*ダメージゲージUIの初期大きさ。*/

	const Vector2 DAMAGE_GAUGE_UI_INIT_PIVOT = Vector2(0.0f, 0.5f); /*ダメージゲージUIの初期ピボット。*/

	const Vector4 DAMAGE_GAUGE_UI_INIT_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.5f); /*ダメージゲージUIの初期乗算色。*/

	/*HPゲージUI(1人目のキャラクターをベースに)。*/
	const float HP_GAUGE_UI_WIDTH = 682; /*HPゲージUIの幅。*/

	const float HP_GAUGE_UI_HEIGHT = 61; /*HPゲージUIの高さ。*/

	const Vector3 HP_GAUGE_UI_INIT_POSITION = Vector3(-760.5f, -461.0f, 0.0f); /*HPゲージUIの初期位置。*/

	const float HP_GAUGE_UI_POSITION_INTERVAL = 425.0f; /*HPゲージUIの位置の間隔。*/

	const Vector3 HP_GAUGE_UI_INIT_SCALE = Vector3(0.425f, 0.425f, 1.0f); /*HPゲージUIの初期大きさ。*/

	const Vector2 HP_GAUGE_UI_INIT_PIVOT = Vector2(0.0f, 0.5f); /*HPゲージUIの初期ピボット。*/

	/*HP。*/
	const int HP_MAX = 100; /*HPの最大値。*/

	/*UIアニメーション。*/
	const float DAMAGE_GAUGE_SCALE_UI_ANIMATION_PLAY_SPEED = 1.5f; /*ダメージゲージUIのアニメーションの再生速度。*/
}

namespace nsApp
{
	namespace nsGame
	{
		/*開始処理。*/
		bool CharacterHP::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			for (int i = 0; i < enCharacter_Num; i++)
			{
				/*UIアニメーションの初期化。*/
				InitUIAnimation((EnCharacter)i);

				/*キャラクターのHPの設定。*/
				m_characterHP[i] = HP_MAX;
				m_characterMaxHP[i] = HP_MAX;
			}

			return true;
		}

		/*更新処理。*/
		void CharacterHP::Update()
		{
			if (g_pad[0]->IsTrigger(enButtonRB2))
			{
				SetCharacterDamage(enCharacter_One, 5);
			}

			for (int i = 0; i < enCharacter_Num; i++)
			{
				/*HPの更新処理。*/
				UpdateHP((EnCharacter)i);

				/*UIアニメーションの更新処理。*/
				UpdateUIAnimation((EnCharacter)i);

				/*スプライトの更新処理。*/
				UpdateSprite((EnCharacter)i);
			}
		}

		/*描画処理。*/
		void CharacterHP::Render(RenderContext& rc)
		{
			for (int i = 0; i < enCharacter_Num; i++)
			{
				/*HPバーUIの描画。*/
				m_hpBarUI[i].Draw(rc);

				/*ダメージゲージUIの描画。*/
				m_damageGaugeUI[i].Draw(rc);

				/*HPゲージUIの描画。*/
				m_hpGaugeUI[i].Draw(rc);
			}
		}

		/*スプライトの初期化。*/
		void CharacterHP::InitSprite()
		{
			for (int i = 0; i < enCharacter_Num; i++)
			{
				/*HPバーUI。*/
				InitHPBarUI((EnCharacter)i, i);

				/*ダメージゲージUI。*/
				InitDamageGaugeUI((EnCharacter)i, i);

				/*HPゲージUI。*/
				InitHPGaugeUI((EnCharacter)i, i);
			}
		}

		/*HPバーUIの初期化。*/
		void CharacterHP::InitHPBarUI(EnCharacter character, int characterIndex)
		{
			Vector3 initPosition = HP_BAR_UI_INIT_POSITION;/*初期位置。*/
			initPosition.x += HP_BAR_UI_POSITION_INTERVAL * characterIndex;/*位置の間隔を加算。*/

			m_hpBarUI[character].Init(m_hpBarUIFilePath[character].c_str(), HP_BAR_UI_WIDTH, HP_BAR_UI_HEIGHT);/*初期化。*/
			m_hpBarUI[character].SetPosition(initPosition);/*位置設定。*/
			m_hpBarUI[character].SetScale(HP_BAR_UI_INIT_SCALE);/*大きさ設定。*/
			m_hpBarUI[character].Update();/*更新処理。*/
		}

		/*ダメージゲージUIの初期化。*/
		void CharacterHP::InitDamageGaugeUI(EnCharacter character, int characterIndex)
		{
			Vector3 initPosition = DAMAGE_GAUGE_UI_INIT_POSITION;/*初期位置。*/
			initPosition.x += DAMAGE_GAUGE_UI_POSITION_INTERVAL * characterIndex;/*位置の間隔を加算。*/

			m_damageGaugeUI[character].Init(m_damageGaugeUIFilePath.c_str(), DAMAGE_GAUGE_UI_WIDTH, DAMAGE_GAUGE_UI_HEIGHT);/*初期化。*/
			m_damageGaugeUI[character].SetPosition(initPosition);/*位置設定。*/
			m_damageGaugeUI[character].SetScale(DAMAGE_GAUGE_UI_INIT_SCALE);/*大きさ設定。*/
			m_damageGaugeUI[character].SetPivot(DAMAGE_GAUGE_UI_INIT_PIVOT);/*ピボット設定。*/
			m_damageGaugeUI[character].SetMulColor(DAMAGE_GAUGE_UI_INIT_MUL_COLOR);/*乗算色設定。*/
			m_damageGaugeUI[character].Update();/*更新処理。*/
		}

		/*HPゲージUIの初期化。*/
		void CharacterHP::InitHPGaugeUI(EnCharacter character, int characterIndex)
		{
			Vector3 initPosition = HP_GAUGE_UI_INIT_POSITION;/*初期位置。*/
			initPosition.x += HP_GAUGE_UI_POSITION_INTERVAL * characterIndex;/*位置の間隔を加算。*/

			m_hpGaugeUI[character].Init(m_hpGaugeUIFilePath.c_str(), HP_GAUGE_UI_WIDTH, HP_GAUGE_UI_HEIGHT);/*初期化。*/
			m_hpGaugeUI[character].SetPosition(initPosition);/*位置設定。*/
			m_hpGaugeUI[character].SetScale(HP_GAUGE_UI_INIT_SCALE);/*大きさ設定。*/
			m_hpGaugeUI[character].SetPivot(HP_GAUGE_UI_INIT_PIVOT);/*ピボット設定。*/
			m_hpGaugeUI[character].Update();/*更新処理。*/

			m_hpGaugeUIBaseScale[character] = m_hpGaugeUI[character].GetScale();/*基準の大きさ設定。*/
		}

		/*UIアニメーションの初期化。*/
		void CharacterHP::InitUIAnimation(EnCharacter character)
		{
			/*ダメージゲージUIの大きさを変えるアニメーションの値の設定。*/
			Vector2 baseScale = { m_damageGaugeUI[character].GetScale().x,m_damageGaugeUI[character].GetScale().y};/*元の大きさ。*/
			Vector2 targetScale = { m_damageGaugeUI[character].GetScale().x,m_damageGaugeUI[character].GetScale().y };/*ターゲットの大きさ。*/

			/*初期化。*/
			m_damageGaugeScaleUIAnimation[character] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_damageGaugeUI[character],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				DAMAGE_GAUGE_SCALE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ*/
				targetScale/*ターゲットの大きさ。*/
			);
		}

		/*HPの更新処理。*/
		void CharacterHP::UpdateHP(EnCharacter character)
		{
			if (m_characterHP[character] <= 0.0f) { return; }/*HPがなければ処理しない。*/
			if (m_characterDamage[character] <= 0.0f) { return; }/*ダメージがなければ処理しない。*/

			m_characterHP[character] -= m_characterDamage[character];/*HPからダメージを減算。*/
			float hpRate = (float)m_characterHP[character] / (float)m_characterMaxHP[character];/*HPの割合を計算。*/
			Vector3 hpGaugeScale = m_hpGaugeUIBaseScale[character];/*HPゲージUIの大きさを取得。*/
			hpGaugeScale.x *= hpRate;/*HPの割合を乗算。*/
			m_hpGaugeUI[character].SetScale(hpGaugeScale);/*HPゲージUIの大きさを設定。*/
			m_characterDamage[character] = 0.0f;/*ダメージをリセット。*/

			Vector2 baseScale = { m_damageGaugeUI[character].GetScale().x,m_damageGaugeUI[character].GetScale().y };/*元の大きさ。*/
			Vector2 targetScale = { m_hpGaugeUI[character].GetScale().x,m_hpGaugeUI[character].GetScale().y};/*ターゲットの大きさ。*/

			m_damageGaugeScaleUIAnimation[character]->Reset();
			m_damageGaugeScaleUIAnimation[character]->SetBaseScale(baseScale);
			m_damageGaugeScaleUIAnimation[character]->SetTargetScale(targetScale);
		}

		/*UIアニメーションの更新処理。*/
		void CharacterHP::UpdateUIAnimation(EnCharacter character)
		{
			/*ダメージゲージUIの大きさを変えるアニメーションの更新処理。*/
			m_damageGaugeScaleUIAnimation[character]->Update();
		}

		/*スプライトの更新処理。*/
		void CharacterHP::UpdateSprite(EnCharacter character)
		{
			/*HPバーUI。*/
			m_hpBarUI[character].Update();

			/*ダメージゲージUI。*/
			m_damageGaugeUI[character].Update();

			/*HPゲージUI。*/
			m_hpGaugeUI[character].Update();
		}
	}
}