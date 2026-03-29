#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file GameEndSelect.h。
 * @brief ゲームが終了した時に選択するクラス。
 * @author Tanimoto。
 * @date 2026/03/27。
 */
namespace nsApp
{
	/*ゲームが終了した時に選択するクラス。*/
	class GameEndSelect : public IGameObject
	{
	public:
		GameEndSelect() {};/*コンストラクタ。*/
		~GameEndSelect() {};/*デストラクタ。*/

	public:
		bool Start() override;/*開始処理。*/
		void Update() override;/*更新処理。*/
		void Render(RenderContext& rc) override;/*描画処理。*/

	public:/*列挙型。*/

		/*選択している内容。*/
		enum EnSelect : uint8_t
		{
			enSelect_Retry,/*リトライ。*/
			enSelect_QuestSelect,/*クエスト選択。*/
			enSelect_Num,/*選択リスト数。*/
		};

		/*選択テキストUI。*/
		enum EnSelectTextUI : uint8_t
		{
			enSelectTextUI_Retry,/*リトライ。*/
			enSelectTextUI_QuestSelect,/*クエスト選択。*/
			enSelectTextUI_Num,/*選択テキストUI数。*/
		};

	private:/*メンバ関数。*/

		/**
		* @brief スプライトの初期化。
		*/
		void InitSprite();

		/**
		* @brief 選択UIの初期化。
		*/
		void InitSelectUI();

		/**
		* @brief 選択UIテキストの初期化。
		* @param selectTextUI 選択テキストUIの種類。
		* @param selectTextIndex 選択テキストUIのインデックス。
		*/
		void InitSelectTextUI(EnSelectTextUI selectTextUI, int selectTextIndex);

		/**
		* @brief UIアニメーションの初期化。
		*/
		void InitUIAnimation();

		/**
		* @brief UIの透明度を変えるアニメーションの初期化。
		*/
		void InitAlphaUIAnimation();

		/**
		* @brief 選択の更新処理。
		*/
		void UpdateSelect();

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
		inline void ResetAlphaUIAnimation()
		{
			m_alphaUIAnimation->Reset();
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
		SpriteRender m_selectUI;/*選択UI。*/
		SpriteRender m_selectTextUI[enSelectTextUI_Num];/*選択テキストUI。*/
		int m_currentSelect = enSelect_Retry;/*現在選択している内容。*/
		std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation;/*透明度を変えるアニメーション。*/
		bool m_didSelect = false;/*選択したかどうか。*/

	private:/*スプライトを表示するファイルパス用のメンバ変数。*/
		std::string m_selectUIFilePath = "Assets/sprite/inGame/select/select.dds";/*選択UIのファイルパス。*/
		std::string m_selectTextUIFilePath[enSelectTextUI_Num] = {
			"Assets/sprite/inGame/text/retry.dds",
			"Assets/sprite/inGame/text/returnQuestSelect.dds",
		};/*選択テキストUIのファイルパス。*/
	};
}

