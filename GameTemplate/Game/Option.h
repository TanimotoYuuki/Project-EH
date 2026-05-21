#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file Option.h。
 * @brief ゲームの設定クラス。
 * @author Tanimoto。
 * @date 2026/04/02。
 */
namespace nsApp
{
	namespace nsOption
	{
		/*ゲームの設定クラス。*/
		class Option : public IGameObject
		{
		public:
			Option() {};/*コンストラクタ。*/
			~Option() {};/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*選択肢テキストUI。*/
			enum EnOptionTextUI : uint8_t
			{
				enOptionTextUI_MasterVolume,/*マスターボリューム。*/
				enOptionTextUI_BGM,/*BGM。*/
				enOptionTextUI_SE,/*SE。*/
				enOptionTextUI_OK,/*OK。*/
				enOptionTextUI_Num,/*選択肢テキスト数。*/
			};

			/*バーUI。*/
			enum EnBarUI : uint8_t
			{
				enBarUI_MasterVolume,/*マスターボリューム。*/
				enBarUI_BGM,/*BGM。*/
				enBarUI_SE,/*SE。*/
				enBarUI_Num/*バーUI数。*/
			};

			/*ゲージUI。*/
			enum EnGaugeUI : uint8_t
			{
				enGaugeUI_MasterVolume,/*マスターボリューム。*/
				enGaugeUI_BGM,/*BGM。*/
				enGaugeUI_SE,/*SE。*/
				enGaugeUI_Num/*ゲージUI数。*/
			};

			/*音量の数字を表示するUI。*/
			enum EnVolumeNumberDisplayUI : uint8_t
			{
				enVolumeNumberDisplayUI_Zero,/*0。*/
				enVolumeNumberDisplayUI_One,/*1。*/
				enVolumeNumberDisplayUI_Two,/*2。*/
				enVolumeNumberDisplayUI_Three,/*3。*/
				enVolumeNumberDisplayUI_Four,/*4。*/
				enVolumeNumberDisplayUI_Five,/*5。*/
				enVolumeNumberDisplayUI_Six,/*6。*/
				enVolumeNumberDisplayUI_Seven,/*7。*/
				enVolumeNumberDisplayUI_Eight,/*8。*/
				enVolumeNumberDisplayUI_Nine,/*9。*/
				enVolumeNumberDisplayUI_Num/*音量の数字UI数。*/
			};

			/*円UI。*/
			enum EnCircleUI : uint8_t
			{
				enCircleUI_MasterVolume,/*マスターボリューム。*/
				enCircleUI_BGM,/*BGM。*/
				enCircleUI_SE,/*SE。*/
				enCircleUI_Num/*円UI数。*/
			};

			/*選択している内容。*/
			enum EnSelect : uint8_t
			{
				enSelect_MasterVolume,/*マスターボリューム。*/
				enSelect_BGM,/*BGM。*/
				enSelect_SE,/*SE。*/
				enSelect_OK,/*OK。*/
				enSelect_Num/*選択リスト数。*/
			};

			/*音量の桁数。*/
			enum EnVolume : uint8_t
			{
				enVolume_Hundred,/*100。*/
				enVolume_Ten,/*10。*/
				enVolume_One,/*1。*/
				enVolume_Num/*音量の桁数。*/
			};

			/*位置。*/
			enum EnPosition : uint8_t
			{
				enPosition_Down,/*下降。*/
				enPosition_Up,/*上昇。*/
				enPosition_Num/*位置を変更する種類。*/
			};

			/*選択しているUIの形状。*/
			enum EnSelectUIShape : uint8_t
			{
				enSelectUIShape_Circle,/*円。*/
				enSelectUIShape_Bar,/*バー。*/
				enSelectUIShape_Num/*選択しているUIの形状数。*/
			};

			/*UIの透明度を変えるアニメーションに適用するスプライト。*/
			enum EnAlphaUIAnimationSprite : uint8_t
			{
				enAlphaUIAnimationSprite_Circle,/*円。*/
				enAlphaUIAnimationSprite_Ok,/*OK。*/
				enAlphaUIAnimationSprite_Num/*アニメーションさせるスプライト数。*/
			};

			/*選択したときの演出UIアニメーションに適用するスプライト。*/
			enum EnSelectDirectionUIAnimationSprite : uint8_t
			{
				enSelectDirectionUIAnimationSprite_OKTextUI,/*「OK」テキストUI。*/
				enSelectDirectionUIAnimationSprite_SelectUI,/*選択UI。*/
				enSelectDirectionUIAnimationSprite_Num/*適用するスプライト数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief 背景の初期化。
			*/
			void InitBackGround();

			/**
			* @brief バーUIの初期化。
			* @param barUI バーUIの種類。
			*/
			void InitBarUI(EnBarUI barUI);

			/**
			* @brief ゲージUIの初期化。
			* @param gaugeUI ゲージUIの種類。
			*/
			void InitGaugeUI(EnGaugeUI gaugeUI);

			/**
			* @brief 音量の数字UIの初期化。
			* @param gaugeUI ゲージUIの種類。
			* @param volume 音量の桁数。
			* @param volumeNumberDisplaUI 音量の数字UIの種類。
			* @param volumeIndex 音量の桁数のインデックス。
			*/
			void InitVolumeNumberUI(EnGaugeUI gaugeUI, EnVolume volume, EnVolumeNumberDisplayUI volumeNumberDisplaUI, int volumeIndex);

			/**
			* @brief 円選択UIの初期化。
			*/
			void InitCircleSelectUI();

			/**
			* @brief 円UIの初期化。
			* @param circleUI 円UIの種類。
			*/
			void InitCircleUI(EnCircleUI circleUI);

			/**
			* @brief 「OK」テキスト選択UIの初期化。
			*/
			void InitOkTextSelectUI();

			/**
			* @brief 選択肢テキストUIの初期化。
			* @param optionTextUI 選択肢テキストUIの種類。
			*/
			void InitOptionTextUI(EnOptionTextUI optionTextUI);

			/**
			* @brief UIアニメーションの初期化。
			*/
			void InitUIAnimation();

			/**
			* @brief UIの透明度を変えるアニメーションの初期化。
			*/
			void InitAlphaUIAnimation();

			/**
			* @brief 選択したときの演出UIアニメーションの初期化。
			*/
			void InitSelectDirectionUIAnimation();

			/**
			* @brief 選択の更新処理。
			*/
			void UpdateSelect();

			/**
			* @brief ゲージと円の更新処理。
			* @param gaugeUI ゲージUIの種類。
			* @param circleUI 円UIの種類。
			*/
			void UpdateGaugeAndCircle(EnGaugeUI gaugeUI, EnCircleUI circleUI);

			/**
			* @brief 音量の数字を表示するためのデータの計算。
			* @param gaugeUI ゲージUIの種類。
			* @param volumeRate 音量の割合。
			*/
			void CalcVolumeNumverDisplayData(EnGaugeUI gaugeUI, float volumeRate);

			/**
			* @brief UIアニメーションの更新処理。
			*/
			void UpdateUIAnimation();

			/**
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

		public:/*メンバ関数。*/

			/**
			* @brief UIの透明度を変えるアニメーションのリセット処理。
			*/
			void ResetAlphaUIAnimation();

			/**
			* @brief 選択したときの演出UIアニメーションのリセット処理。
			*/
			void ResetSelectDirectionUIAnimation();

			/**
			* @brief 選択のリセット処理。
			*/
			inline void ResetSelect()
			{
				m_currentSelect = enSelect_MasterVolume;
				m_circleSelectUI.SetPosition(m_circleUI[m_currentSelect].GetPosition());
				m_circleSelectUI.Update();
			}

			/**
			* @brief 現在の選択している内容の取得。
			* @return 現在の選択している内容。
			*/
			inline EnSelect GetCurrentSelect()
			{
				return (EnSelect)m_currentSelect;
			};

			/**
			* @brief UIを描画する状態にする。
			*/
			inline void EnableDrawingUI()
			{
				m_isDrawingUI = true;
			}

			/**
			* @brief UIを描画しない状態にする。
			*/
			inline void DisableDrawingUI()
			{
				m_isDrawingUI = false;
			}

			/**
			* @brief UIを描画しているか？
			* @return trueならUIを描画している。
			*/
			inline bool IsDrawingUI()
			{
				return m_isDrawingUI;
			}

			/**
			* @brief 演出できる状態にする。
			*/
			inline void EnableDirection()
			{
				m_isDirection = true;
			}

			/**
			* @brief 演出できない状態にする。
			*/
			inline void DisableDirection()
			{
				m_isDirection = false;
			}

			/**
			* @brief 演出中？
			* @return trueなら演出中。
			*/
			inline bool IsDirection() const
			{
				return m_isDirection;
			}

			/**
			* @brief 選択できる状態にする。
			*/
			inline void EnableSelect()
			{
				m_didSelect = true;
			}

			/**
			* @brief 選択できない状態にする。
			*/
			inline void DisableSelect()
			{
				m_didSelect = false;
			}

			/**
			* @brief 選択できたか？
			* @return trueなら選択できている。
			*/
			inline bool DidSelect() const
			{
				return m_didSelect;
			}

		private:/*メンバ変数。*/
			SpriteRender m_backGround;/*背景。*/
			SpriteRender m_barUI[enBarUI_Num];/*バーUI。*/
			SpriteRender m_gaugeUI[enGaugeUI_Num];/*ゲージUI。*/
			SpriteRender m_volumeNumberUI[enGaugeUI_Num][enVolume_Num][enVolumeNumberDisplayUI_Num];/*音量の数字UI。*/
			SpriteRender m_circleSelectUI;/*円選択UI。*/
			SpriteRender m_circleUI[enCircleUI_Num];/*円UI。*/
			SpriteRender m_okTextSelectUI;/*「OK」テキスト選択UI。*/
			SpriteRender m_optionTextUI[enOptionTextUI_Num];/*選択肢テキストUI。*/
			Vector3 m_gaugeUIBaseScale[enGaugeUI_Num] = {Vector3::Zero,Vector3::Zero,Vector3::Zero };/*ゲージUIの基準の大きさ。*/
			Vector3 m_circleUIBasePosition[enCircleUI_Num] = { Vector3::Zero,Vector3::Zero,Vector3::Zero };/*円UIの基準の位置。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation[enAlphaUIAnimationSprite_Num];/*UIの透明度を変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_selectDirectionUIAnimation[enPosition_Num][enSelectDirectionUIAnimationSprite_Num];/*選択したときの演出UIアニメーション。*/
			int m_currentSelect = enSelect_MasterVolume;/*現在の選択している内容。*/
			int m_volumeChangeValue = 0;/*音量変化量。*/
			int m_volumeRate[enGaugeUI_Num] = { 0,0 };/*音量の割合。*/
			int m_volumeMaxRate[enGaugeUI_Num] = { 0,0 };/*最大音量の割合。*/
			int m_volumeNumber[enGaugeUI_Num][enVolume_Num] = { 0,0,0,0,0,0 };/*音量の数字。*/
			bool m_isGaugeUp = false;/*ゲージを上げるか？*/
			bool m_isGaugeDown = false;/*ゲージを下げるか？*/
			bool m_isDrawingUI = false;/*UIを描画しているか？*/
			bool m_isDirection = false;/*演出中？*/
			bool m_didSelect = false;/*選択できたか？*/
			std::vector<EnVolumeNumberDisplayUI> m_volumeNumverDisplayManager[enGaugeUI_Num];/*音量の数字UIを管理する変数。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_backGroundFilePath = "Assets/sprite/option/background/backGround.dds";/*背景のファイルパス。*/
			std::string m_barUIFilePath[enBarUI_Num] = {
				"Assets/sprite/option/bar/bar.dds",
				"Assets/sprite/option/bar/bar.dds",
				"Assets/sprite/option/bar/bar.dds",
			};/*バーUIのファイルパス。*/
			std::string m_gaugeUIFilePath[enGaugeUI_Num] = {
				"Assets/sprite/option/gauge/gauge.dds",
				"Assets/sprite/option/gauge/gauge.dds",
				"Assets/sprite/option/gauge/gauge.dds",
			};/*ゲージUIのファイルパス。*/
			std::string m_volumeNumberUIFilePath[enVolumeNumberDisplayUI_Num] = {
				"Assets/sprite/option/number/zero.dds",/*0。*/
				"Assets/sprite/option/number/one.dds",/*1。*/
				"Assets/sprite/option/number/two.dds",/*2。*/
				"Assets/sprite/option/number/three.dds",/*3。*/
				"Assets/sprite/option/number/four.dds",/*4。*/
				"Assets/sprite/option/number/five.dds",/*5。*/
				"Assets/sprite/option/number/six.dds",/*6。*/
				"Assets/sprite/option/number/seven.dds",/*7。*/
				"Assets/sprite/option/number/eight.dds",/*8。*/
				"Assets/sprite/option/number/nine.dds",/*9。*/
			};/*音量の数字UIのファイルパス。*/
			std::string m_circleSelectUIFilePath = "Assets/sprite/option/select/circleSelect.dds";/*円選択UIのファイルパス。*/
			std::string m_circleUIFilePath[enCircleUI_Num] = {
				"Assets/sprite/option/circle/circle.dds",
				"Assets/sprite/option/circle/circle.dds",
				"Assets/sprite/option/circle/circle.dds",
			};/*円UIのファイルパス。*/
			std::string m_okTextSelectUIFilePath = "Assets/sprite/option/select/okselect.dds";/*「OK」テキスト選択UIのファイルパス。*/
			std::string m_optionTextUIFilePath[enOptionTextUI_Num] = {
				"Assets/sprite/option/text/masterVolume.dds",/*マスターボリュームテキストUIのファイルパス。*/
				"Assets/sprite/option/text/bgm.dds",/*BGMテキストUIのファイルパス。*/
				"Assets/sprite/option/text/se.dds",/*SEテキストUIのファイルパス。*/
				"Assets/sprite/option/text/ok.dds",/*OKテキストUIのファイルパス。*/
			};/*選択肢テキストUIのファイルパス。*/
		};
	}
}

