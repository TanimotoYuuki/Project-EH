#pragma once
#include "Src/UIAnimation/UIAnimation.h"
/**
 * @file Result.h。
 * @brief リザルトクラス。
 * @author Tanimoto。
 * @date 2026/03/25。
 */
namespace nsApp
{
	class GameEndSelect;
	namespace nsResult
	{
		/*リザルトクラス。*/
		class Result : public IGameObject
		{
		public:
			Result() {};/*コンストラクタ。*/ 
			~Result();/*デストラクタ。*/

		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*ランクUI。*/
			enum EnRankUI : uint8_t	
			{
				enRankUI_Platinum,/*プラチナ。*/
				enRankUI_Gold,/*ゴールド。*/
				enRankUI_Silver,/*シルバー。*/
				enRankUI_Bronze,/*ブロンズ。*/
				enRankUI_None,/*なし。*/
				enRankUI_Num,/*ランクUI数。*/
			};

			/*ボーナス。*/
			enum EnBonus : uint8_t
			{
				enBonus_ShotDown,/*撃墜。*/
				enBonus_SpeedClear,/*スピードクリア。*/
				enBonus_Num,/*ボーナス数。*/
			};

			/*リザルトでスコアを表示するUI。*/
			enum EnScoreDisplayUI : uint8_t
			{
				enScoreDisplayUI_Zero,/*0。*/
				enScoreDisplayUI_One,/*1。*/
				enScoreDisplayUI_Two,/*2。*/
				enScoreDisplayUI_Three,/*3。*/
				enScoreDisplayUI_Four,/*4。*/
				enScoreDisplayUI_Five,/*5。*/
				enScoreDisplayUI_Six,/*6。*/
				enScoreDisplayUI_Seven,/*7。*/
				enScoreDisplayUI_Eight,/*8。*/
				enScoreDisplayUI_Nine,/*9。*/
				enScoreDisplayUI_Num,/*表示するUI数。*/
			};

			/*スコア。*/
			enum EnScore : uint8_t
			{
				enScore_TenThousand,/*〇万点。*/
				enScore_Thousand,/*〇千点。*/
				enScore_Hundred,/*〇百点。*/
				enScore_Ten,/*〇十点。*/
				enScore_One,/*〇点。*/
				enScore_Num,/*スコアの数。*/
			};

			/*リザルトでクリア時間を表示するUI。*/
			enum EnClearTimeDisplayUI : uint8_t
			{
				enClearTimeDisplayUI_Zero,/*0。*/
				enClearTimeDisplayUI_One,/*1。*/
				enClearTimeDisplayUI_Two,/*2。*/
				enClearTimeDisplayUI_Three,/*3。*/
				enClearTimeDisplayUI_Four,/*4。*/
				enClearTimeDisplayUI_Five,/*5。*/
				enClearTimeDisplayUI_Six,/*6。*/
				enClearTimeDisplayUI_Seven,/*7。*/
				enClearTimeDisplayUI_Eight,/*8。*/
				enClearTimeDisplayUI_Nine,/*9。*/
				enClearTimeDisplayUI_Colon,/*コロン。*/
				enClearTimeDisplayUI_Num,/*表示するUI数。*/
			};

			/*時間。*/
			enum EnTime
			{
				enTime_Minute,/*〇分。*/
				enTime_Colon,/*〇分と〇秒の間のコロン。*/
				enTime_Second_Ten,/*〇秒の十の位。*/
				enTime_Second_One,/*〇秒の一の位。*/
				enTime_Num/*時間の数。*/
			};

			/*大きさ。*/
			enum EnScale
			{
				enScale_Down,/*小さくする。*/
				enScale_Up,/*大きくする。*/
				enScale_Num,/*スケールの数。*/
			};

			/*UIをスライドさせるアニメーションに適用するスプライト。*/
			enum EnSlideUIAnimationSprite : uint8_t
			{
				enSlideUIAnimationSprite_RankFrameUI,/*ランク枠UI。*/
				enSlideUIAnimationSprite_BonusUI_ShotDown,/*ボーナスUI(撃墜)。*/
				enSlideUIAnimationSprite_BonusUI_SpeedClear,/*ボーナスUI(スピードクリア)。*/
				enSlideUIAnimationSprite_NoBonusUI_ShotDown,/*ノーボーナスUI(撃墜なし)。*/
				enSlideUIAnimationSprite_NoBonusUI_SpeedClear,/*ノーボーナスUI(スピードクリアなし)。*/
				enSlideUIAnimationSprite_Num,/*スプライト数。*/
			};
			
			/*UIの透明度を変えるアニメーションに適用するスプライト。*/
			enum EnAlphaUIAnimationSprite : uint8_t
			{
				enAlphaUIAnimationSprite_TargetUI,/*ターゲットUI。*/
				enAlphaUIAnimationSprite_ScoreFrameUI,/*スコア枠UI。*/
				enAlphaUIAnimationSprite_ClearTimeFrameUI,/*クリア時間枠UI。*/
				enAlphaUIAnimationSprite_ClearTimeUI_Minute,/*クリア時間UI(〇分)。*/
				enAlphaUIAnimationSprite_ClearTimeUI_Colon,/*クリア時間UI(コロン)。*/
				enAlphaUIAnimationSprite_ClearTimeUI_Second_Ten,/*クリア時間UI(〇秒の十の位)。*/
				enAlphaUIAnimationSprite_ClearTimeUI_Second_One,/*クリア時間UI(〇秒の一の位)。*/
				enAlphaUIAnimationSprite_RankFrameUI,/*ランク枠UI。*/
				enAlphaUIAnimationSprite_RankUI,/*ランクUI。*/
				enAlphaUIAnimationSprite_BonusUI_ShotDown,/*ボーナスUI(撃墜)。*/
				enAlphaUIAnimationSprite_BonusUI_SpeedClear,/*ボーナスUI(スピードクリア)。*/
				enAlphaUIAnimationSprite_NoBonusUI_ShotDown,/*ノーボーナスUI(撃墜なし)。*/
				enAlphaUIAnimationSprite_NoBonusUI_SpeedClear,/*ノーボーナスUI(スピードクリアなし)。*/
				enAlphaUIAnimationSprite_Num,/*スプライト数。*/
			};

			/*ランクUIの大きさを変えるアニメーションの回数。*/
			enum EnScaleRankUIAnimationCount : uint8_t
			{
				enScaleRankUIAnimationCount_One,/*1回目のUIアニメーション。*/
				enScaleRankUIAnimationCount_Two,/*2回目のUIアニメーション。*/
				enScaleRankUIAnimationCount_Num,/*UIアニメーションの回数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief リザルトで表示するためのデータの計算。
			*/
			void CalcResultDisplayData();

			/**
			* @brief スコアを表示するためのデータの計算。
			*/
			void CalcScoreDisplayData();

			/**
			* @brief クリア時間を表示するためのデータの計算。
			*/
			void CalcClearTimeDisplayData();

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief 背景の初期化。
			*/
			void InitBackGround();

			/**
			* @brief ターゲットUIの初期化。
			*/
			void InitTargetUI();

			/**
			* @brief スコア枠UIの初期化。
			*/
			void InitScoreFrameUI();

			/**
			* @brief スコアUIの初期化。
			* @param score スコア。
			* @param scoreIndex スコアのインデックス。
			*/
			void InitScoreUI(EnScore score, int scoreIndex);

			/**
			* @brief クリア時間枠UIの初期化。
			*/
			void InitClearTimeFrameUI();

			/**
			* @brief クリア時間UIの初期化。
			* @param time 時間。
			* @param timeIndex 時間のインデックス。
			*/
			void InitClearTimeUI(EnTime time, int timeIndex);
			
			/**
			* @brief ランク枠UIの初期化。
			*/
			void InitRankFrameUI();

			/**
			* @brief ランクUIの初期化。
			*/
			void InitRankUI();

			/**
			* @brief ボーナスUIの初期化。
			*/
			void InitBonusUI();

			/**
			* @brief ノーボーナスUIの初期化。
			*/
			void InitNoBonusUI();

			/**
			* @brief UIアニメーションの初期化。
			*/
			void InitUIAnimation();

			/**
			* @brief スコアUIをスライドさせるアニメーションの初期化。
			* @param score スコア。
			*/
			void InitSlideScoreUIAnimation(EnScore score);

			/**
			* @brief スコアUIの大きさを変えるアニメーション。
			* @param score スコア。
			*/
			void InitScaleScoreUIAnimation(EnScore score);

			/**
			* @brief UIをスライドさせるアニメーションの初期化。
			*/
			void InitSlideUIAnimation();

			/**
			* @brief UIの透明度を変えるアニメーションの初期化。
			*/
			void InitAlphaUIAnimation();

			/**
			* @brief ランクUIの大きさを変えるアニメーション。
			*/
			void InitScaleRankUIAnimation();

			/**
			* @brief 演出の更新処理。
			*/
			void UpdateDirection();

			/**
			* @brief スプライトの更新処理。
			*/
			void UpdateSprite();

		public:/*メンバ関数。*/

			/**
			* @brief ゲームが終了した時に選択する用のインスタンスの取得。
			* @return ゲームが終了した時に選択する用のインスタンス。
			*/
			inline GameEndSelect* GetGameEndSelectInstance() const
			{
				return m_gameEndSelect;
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

			/**
			* @brief 演出が終了したか？
			* @return trueなら終了している。
			*/
			bool IsDirectionFinished() const
			{
				return m_isDirectionFinished;
			}

		private:/*メンバ変数。*/
			SpriteRender m_backGround;/*背景。*/
			SpriteRender m_targetUI;/*ターゲットUI。*/
			SpriteRender m_scoreFrameUI;/*スコア枠UI。*/
			SpriteRender m_scoreUI[enScore_Num];/*スコアUI。*/
			SpriteRender m_clearTimeFrameUI;/*クリア時間枠UI。*/
			SpriteRender m_clearTimeUI[enTime_Num];/*クリア時間UI。*/
			SpriteRender m_rankFrameUI;/*ランク枠UI。*/
			SpriteRender m_rankUI;/*ランクUI。*/
			SpriteRender m_bonusUI[enBonus_Num];/*ボーナスUI。*/
			SpriteRender m_noBonusUI[enBonus_Num];/*ノーボーナスUI。*/
			int m_calcScoreData = 12345;/*スコアを表示するためのデータ。*/
			int m_score[enScore_Num] = { 0,0,0,0,0 };/*スコア。*/
			int m_calcClearTimeData = 180;/*クリア時間を表示するためのデータ。*/
			int m_clearTime[enTime_Num] = { 0,0,0,0 };/*クリア時間。*/
			int m_rank = enRankUI_Gold;/*ランク。*/
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_slideScoreUIAnimation[enScore_Num];/*スコアUIをスライドさせるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_scaleUpScoreUIAnimation[enScore_Num];/*スコアUIの大きさを大きくするアニメーション。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_scaleDownScoreUIAnimation[enScore_Num];/*スコアUIの大きさを小さくするアニメーション。*/
			std::unique_ptr<nsApp::nsUI::PositionUIAnimation> m_slideUIAnimation[enSlideUIAnimationSprite_Num];/*UIをスライドさせるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::AlphaUIAnimation> m_alphaUIAnimation[enAlphaUIAnimationSprite_Num];/*UIの透明度を変えるアニメーション。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_scaleDownRankUIAnimation[enScaleRankUIAnimationCount_Num];/*ランクUIの大きさを小さくするアニメーション。*/
			std::unique_ptr<nsApp::nsUI::ScaleUIAnimation> m_scaleUpRankUIAnimation[enScaleRankUIAnimationCount_Num];/*ランクUIの大きさを大きくするアニメーション。*/
			GameEndSelect* m_gameEndSelect = nullptr;/*ゲームが終了した時に選択する用のインスタンス。*/
			bool m_isGetBonus[enBonus_Num] = { true,false };/*ボーナス獲得フラグ。*/
			bool m_didSelect = false;/*選択できたか？*/
			bool m_isDirectionFinished = false;/*演出が終了したか。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			std::string m_backGroundFilePath = "Assets/sprite/result/background/background.dds";/*背景のファイルパス。*/
			std::string m_targetUIFilePath = "Assets/sprite/result/target/target.dds";/*ターゲットUIのファイルパス。*/
			std::string m_scoreFrameUIFilePath = "Assets/sprite/result/frame/scoreFrame.dds";/*スコア枠UIのファイルパス。*/
			std::string m_scoreUIFilePath[enScoreDisplayUI_Num] = {
				"Assets/sprite/result/score/zero.dds",/*0。*/
				"Assets/sprite/result/score/one.dds",/*1。*/
				"Assets/sprite/result/score/two.dds",/*2。*/
				"Assets/sprite/result/score/three.dds",/*3。*/
				"Assets/sprite/result/score/four.dds",/*4。*/
				"Assets/sprite/result/score/five.dds",/*5。*/
				"Assets/sprite/result/score/six.dds",/*6。*/
				"Assets/sprite/result/score/seven.dds",/*7。*/
				"Assets/sprite/result/score/eight.dds",/*8。*/
				"Assets/sprite/result/score/nine.dds",/*9。*/
			};/*スコアUIのファイルパス。*/
			std::string m_clearTimeFrameUIFilePath = "Assets/sprite/result/frame/clearTimeFrame.dds";/*クリア時間枠UIのファイルパス。*/
			std::string m_clearTimeUIFilePath[enClearTimeDisplayUI_Num] = {
				"Assets/sprite/result/clearTime/zero.dds",/*0。*/
				"Assets/sprite/result/clearTime/one.dds",/*1。*/
				"Assets/sprite/result/clearTime/two.dds",/*2。*/
				"Assets/sprite/result/clearTime/three.dds",/*3。*/
				"Assets/sprite/result/clearTime/four.dds",/*4。*/
				"Assets/sprite/result/clearTime/five.dds",/*5。*/
				"Assets/sprite/result/clearTime/six.dds",/*6。*/
				"Assets/sprite/result/clearTime/seven.dds",/*7。*/
				"Assets/sprite/result/clearTime/eight.dds",/*8。*/
				"Assets/sprite/result/clearTime/nine.dds",/*9。*/
				"Assets/sprite/result/clearTime/colon.dds",/*コロン。*/
			};/*クリア時間UIのファイルパス*/
			std::string m_rankFrameUIFilePath = "Assets/sprite/result/frame/rankFrame.dds";/*ランク枠UIのファイルパス。*/
			std::string m_rankUIFilePath[enRankUI_Num] = {
				"Assets/sprite/result/rank/platinum.dds",/*プラチナ。*/
				"Assets/sprite/result/rank/gold.dds",/*ゴールド。*/
				"Assets/sprite/result/rank/silver.dds",/*シルバー。*/
				"Assets/sprite/result/rank/bronze.dds",/*ブロンズ。*/
			};/*ランクUIのファイルパス。*/
			std::string m_bonusUIFilePath[enBonus_Num] = {
				"Assets/sprite/result/bonus/shotDownBonus.dds",/*撃墜。*/
				"Assets/sprite/result/bonus/speedClearBonus.dds",/*スピードクリア。*/
			};/*ボーナスUIのファイルパス。*/
			std::string m_noBonusUIFilePath[enBonus_Num] = {
				"Assets/sprite/result/bonus/noShotDownBonus.dds",/*撃墜なし。*/
				"Assets/sprite/result/bonus/noSpeedClearBonus.dds",/*スピードクリアなし。*/
			};/*ノーボーナスUIのファイルパス。*/
		};
	}
}
