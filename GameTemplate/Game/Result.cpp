#include "stdafx.h"
#include "Result.h"
#include "GameEndSelect.h"
#include "Src/Fade/Fade.h"

namespace {
	/*背景。*/
	const float BACK_GROUND_WIDTH = 1920.0f;/*背景の幅。*/

	const float BACK_GROUND_HEIGHT = 1080.0f;/*背景の高さ。*/

	/*ターゲットUI。*/
	const float TARGET_UI_WIDTH = 1833.0f;/*ターゲットUIの幅。*/

	const float TARGET_UI_HEIGHT = 153.0f;/*ターゲットUIの高さ。*/

	const Vector3 TARGET_UI_INIT_POSITION = Vector3(-25.0f, 425.0f, 0.0f);/*ターゲットUIの初期位置。*/

	const Vector3 TARGET_UI_INIT_SCALE = Vector3(1.0f, 1.0f, 1.0f);/*ターゲットUIの初期大きさ。*/

	const Vector4 TARGET_UI_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.0f);/*ターゲットUIの乗算色。*/

	/*スコア枠UI。*/ 
	const float SCORE_FRAME_UI_WIDTH = 765.0f;/*スコア枠UIの幅。*/

	const float SCORE_FRAME_UI_HEIGHT = 240.0f;/*スコア枠UIの高さ。*/

	const Vector3 SCORE_FRAME_UI_INIT_POSITION = Vector3(-400.0f, 100.0f, 0.0f);/*スコア枠UIの初期位置。*/

	const Vector3 SCORE_FRAME_UI_INIT_SCALE = Vector3(1.0f, 1.0f, 1.0f);/*スコア枠UIの初期大きさ。*/

	const Vector4 SCORE_FRAME_UI_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.0f);/*スコア枠UIの乗算色。*/

	/*スコアUI。*/
	const float SCORE_UI_WIDTH = 1024.0f;/*スコアUIの幅。*/

	const float SCORE_UI_HEIGHT = 128.0f;/*スコアUIの高さ。*/

	const Vector3 SCORE_UI_INIT_POSITION = Vector3(0.0f, 0.0f, 0.0f);/*スコアUIの初期位置。*/

	const float SCORE_UI_POSITION_INTERVAL = 55.0f;/*スコアUIの位置の間隔。*/

	const Vector3 SCORE_UI_INIT_SCALE = Vector3(0.65f, 0.65f, 1.0f);/*スコアUIの初期大きさ。*/

	/*クリア時間枠UI。*/
	const float CLEAR_TIME_FRAME_UI_WIDTH = 610.0f;/*クリア時間枠UIの幅。*/

	const float CLEAR_TIME_FRAME_UI_HEIGHT = 198.0f;/*クリア時間枠UIの高さ。*/

	const Vector3 CLEAR_TIME_FRAME_UI_INIT_POSITION = Vector3(-400.0f, -100.0f, 0.0f);/*クリア時間枠UIの初期位置。*/

	const Vector3 CLEAR_TIME_FRAME_UI_INIT_SCALE = Vector3(1.0f, 1.0f, 1.0f);/*クリア時間枠UIの初期大きさ。*/

	const Vector4 CLEAR_TIME_FRAME_UI_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.0f);/*クリア時間枠UIの乗算色。*/

	/*クリア時間UI。*/
	const float CLEAR_TIME_UI_WIDTH = 1024.0f;/*クリア時間UIの幅。*/

	const float CLEAR_TIME_UI_HEIGHT = 128.0f;/*クリア時間UIの高さ。*/

	const Vector3 CLEAR_TIME_UI_INIT_POSITION = Vector3(-350.0f, -110.0f, 0.0f);/*クリア時間UIの初期位置。*/

	const float CLEAR_TIME_UI_POSITION_INTERVAL = 50.0f;/*クリア時間UIの位置の間隔。*/

	const Vector3 CLEAR_TIME_UI_INIT_SCALE = Vector3(0.65f, 0.65f, 1.0f);/*クリア時間UIの初期大きさ。*/

	const Vector4 CLEAR_TIME_UI_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.0f);/*クリア時間UIの乗算色。*/

	/*ランク枠UI。*/
	const float RANK_FRAME_UI_WIDTH = 1024.0f;/*ランク枠UIの幅。*/

	const float RANK_FRAME_UI_HEIGHT = 1024.0f;/*ランク枠UIの高さ。*/

	const Vector3 RANK_FRAME_UI_INIT_POSITION = Vector3(850.0f, 0.0f, 0.0f);/*ランク枠UIの初期位置。*/

	const Vector3 RANK_FRAME_UI_INIT_SCALE = Vector3(0.5f, 0.5f, 1.0f);/*ランク枠UIの初期大きさ。*/

	const Vector4 RANK_FRAME_UI_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.0f);/*ランク枠UIの乗算色。*/

	/*ランクUI。*/
	const float RANK_UI_WIDTH = 1024.0f;/*ランクUIの幅。*/

	const float RANK_UI_HEIGHT = 1024.0f;/*ランクUIの高さ。*/

	const Vector3 RANK_UI_INIT_POSITION = Vector3(350.0f, 0.0f, 0.0f);/*ランクUIの初期位置。*/

	const Vector3 RANK_UI_INIT_SCALE = Vector3(0.575f, 0.575f, 1.0f);/*ランクUIの初期大きさ。*/

	const Vector4 RANK_UI_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.0f);/*ランクUIの乗算色。*/

	/*ボーナスUI。(共通)*/
	const float BONUS_UI_WIDTH = 836.0f;/*ボーナスUIの幅。*/

	const float BONUS_UI_HEIGHT = 147.0f;/*ボーナスUIの高さ。*/

	const Vector4 BONUS_UI_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.0f);/*ボーナスUIの乗算色。*/

	/*ボーナスUI。(撃墜)*/
	const Vector3 SHOT_DOWN_BONUS_UI_INIT_POSITION = Vector3(-1550.0f, -400.0f, 0.0f);/*撃墜ボーナスUIの初期位置。*/

	const Vector3 SHOT_DOWN_BONUS_UI_INIT_SCALE = Vector3(1.0f, 1.0f, 1.0f);/*撃墜ボーナスUIの初期大きさ。*/

	/*ボーナスUI。(スピードクリア)*/
	const Vector3 SPEED_CLEAR_BONUS_UI_INIT_POSITION = Vector3(-2450.0f, -400.0f, 0.0f);/*スピードクリアボーナスUIの初期位置。*/

	const Vector3 SPEED_CLEAR_BONUS_UI_INIT_SCALE = Vector3(1.0f, 1.0f, 1.0f);/*スピードクリアボーナスUIの初期大きさ。*/

	/*ノーボーナスUI。(共通)*/
	const float NO_BONUS_UI_WIDTH = 836.0f;/*ノーボーナスUIの幅。*/

	const float NO_BONUS_UI_HEIGHT = 147.0f;/*ノーボーナスUIの高さ。*/

	const Vector4 NO_BONUS_UI_MUL_COLOR = Vector4(1.0f, 1.0f, 1.0f, 0.0f);/*ノーボーナスUIの乗算色。*/

	/*ノーボーナスUI。(撃墜)*/
	const Vector3 SHOT_DOWN_NO_BONUS_UI_INIT_POSITION = Vector3(-1550.0f, -400.0f, 0.0f);/*撃墜ノーボーナスUIの初期位置。*/

	const Vector3 SHOT_DOWN_NO_BONUS_UI_INIT_SCALE = Vector3(1.0f, 1.0f, 1.0f);/*撃墜ノーボーナスUIの初期大きさ。*/

	/*ノーボーナスUI。(スピードクリア)*/
	const Vector3 SPEED_CLEAR_NO_BONUS_UI_INIT_POSITION = Vector3(-2450.0f, -400.0f, 0.0f);/*スピードクリアノーボーナスUIの初期位置。*/

	const Vector3 SPEED_CLEAR_NO_BONUS_UI_INIT_SCALE = Vector3(1.0f, 1.0f, 1.0f);/*スピードクリアノーボーナスUIの初期大きさ。*/

	/*UIアニメーション。*/
	const Vector3 AFTER_SCORE_UI_ANIMATION_POSITION[nsApp::nsResult::Result::EnScore::enScore_Num] = {
		Vector3{-400.0f, 75.0f, 0.0f},/*〇万点。*/
		Vector3{-400.0f + SCORE_UI_POSITION_INTERVAL * 1, 75.0f, 0.0f},/*〇千点。*/
		Vector3{-400.0f + SCORE_UI_POSITION_INTERVAL * 2, 75.0f, 0.0f},/*〇百点。*/
		Vector3{-400.0f + SCORE_UI_POSITION_INTERVAL * 3, 75.0f, 0.0f},/*〇十分点。*/
		Vector3{-400.0f + SCORE_UI_POSITION_INTERVAL * 4, 75.0f, 0.0f}/*〇点。*/
	};/*スコアUIをスライドさせるアニメーションの後の位置。*/

	const Vector2 AFTER_SCORE_UI_ANIMATION_SCALE = Vector2{ 1.0f, 1.0f };/*スコアUIの大きさを変えるアニメーションの後の大きさ。*/

	const Vector3 AFTER_SLIDE_UI_ANIMATION_POSITION[nsApp::nsResult::Result::EnSlideUIAnimationSprite::enSlideUIAnimationSprite_Num] = {
		Vector3{ 350.0f, 0.0f, 0.0f },/*ランク枠UI。*/
		Vector3{ 450.0f, -400.0f, 0.0f },/*ボーナスUI(撃墜)。*/
		Vector3{ -450.0f, -400.0f, 0.0f },/*ボーナスUI(スピードクリア)。*/
		Vector3{ 450.0f, -400.0f, 0.0f },/*ノーボーナスUI(撃墜)。*/
		Vector3{ -450.0f, -400.0f, 0.0f }/*ノーボーナスUI(スピードクリア)。*/
	};/*UIをスライドさせるアニメーションの後の位置。*/

	const float AFTER_ALPHA_UI_ANIMATION_ALPHA = 1.0f;/*UIの透明度を変えるアニメーションの後の透明度。*/

	const Vector2 AFTER_RANK_UI_ANIMATION_SCALE[nsApp::nsResult::Result::EnScale::enScale_Num][nsApp::nsResult::Result::enScaleRankUIAnimationCount_Num] = {
		/*ランクUIを小さくした後の大きさ。*/
		Vector2{ 0.35f, 0.35f},/*1回目。*/
		Vector2{ 0.5f, 0.5f },/*2回目。*/
		/*ランクUIを大きくした後の大きさ。*/
		Vector2{ 0.525f, 0.525f },/*1回目。*/
		Vector2{ 1.0f, 1.0f },/*2回目。*/ 
	};/*ランクUIの大きさを変えるアニメーションの後の大きさ。*/

	const float SLIDE_UI_ANIMATION_PLAY_SPEED = 2.5f;/*UIをスライドさせるアニメーションの再生速度。*/

	const float ALPHA_UI_ANIMATION_PLAY_SPEED = 4.0f;/*UIの透明度を変えるアニメーションの再生速度。*/

	const float SCALE_UP_UI_ANIMATION_PLAY_SPEED = 2.3f;/*UIの大きさを変えるアニメーションの再生速度。*/

	const float SCALE_DOWN_UI_ANIMATION_PLAY_SPEED = 2.3f;/*UIの大きさを元に戻すアニメーションの再生速度。*/
}

namespace nsApp
{
	namespace nsResult
	{
		/*デストラクタ。*/
		Result::~Result()
		{
			DeleteGO(m_gameEndSelect);
		}

		/*開始処理。*/
		bool Result::Start()
		{
			/*リザルトで表示するためのデータの計算。*/
			CalcResultDisplayData();

			/*スプライトの初期化。*/
			InitSprite();

			/*UIアニメーションの初期化。*/
			InitUIAnimation();

			/*フェードインに切り替える。*/
			nsApp::nsFade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_FadeIn);

			return true;
		}

		/*更新処理。*/
		void Result::Update()
		{
			if (nsApp::nsFade::Fade::GetInstance()->IsFadeIn())
			{
				return;
			}

			if (IsDirectionFinished())
			{
				if (!DidSelect())
				{
					if (g_pad[0]->IsTrigger(enButtonA))
					{
						nsFade::Fade::Fade::GetInstance()->ChangeFadeType(nsApp::nsFade::Fade::EnFadeType::enFadeType_HalfFadeOut);
						m_gameEndSelect = NewGO<GameEndSelect>(2, "gameEndSelect");
						EnableSelect();
					}
				}
				else
				{
					return;
				}
			}

			/*演出。*/
			UpdateDirection();
			
			/*スプライト。*/
			UpdateSprite();
		}

		/*描画処理。*/
		void Result::Render(RenderContext& rc)
		{
			/*背景の描画。*/
			m_backGround.Draw(rc);

			/*ターゲットUIの描画。*/
			m_targetUI.Draw(rc);

			/*スコア枠UIの描画。*/
			m_scoreFrameUI.Draw(rc);

			for (int i = 0; i < enScore_Num; i++)
			{
				/*スコアUIの描画。*/
				m_scoreUI[i].Draw(rc);
			}

			/*クリア時間枠UIの描画。*/
			m_clearTimeFrameUI.Draw(rc);

			for (int j = 0; j < enTime_Num; j++)
			{
				/*クリア時間UIの描画。*/
				m_clearTimeUI[j].Draw(rc);
			}

			/*ランク枠UIの描画。*/
			m_rankFrameUI.Draw(rc);

			if (m_rank != enRankUI_None)
			{
				/*ランクUIの描画。*/
				m_rankUI.Draw(rc);
			}

			for (int k = 0; k < enBonus_Num; k++)
			{
				if (m_isGetBonus[k])
				{
					/*ボーナスUIの描画。*/
					m_bonusUI[k].Draw(rc);
				}
				else
				{
					/*ノーボーナスUIの描画。*/
					m_noBonusUI[k].Draw(rc);
				}
			}
		}

		/*リザルトで表示するためのデータの計算。*/
		void Result::CalcResultDisplayData()
		{
			/*スコアを表示するためのデータ。*/
			CalcScoreDisplayData();

			/*クリア時間を表示するためのデータ。*/
			CalcClearTimeDisplayData();
		}

		/*スコアを表示するためのデータの計算。*/
		void Result::CalcScoreDisplayData()
		{
			/*万の位を算出するための計算。*/
			int tenThousand = m_calcScoreData / 10000;
			m_score[enScore_TenThousand] = tenThousand;

			/*千の位を算出するための計算。*/
			int thousand = (m_calcScoreData % 10000) / 1000;
			m_score[enScore_Thousand] = thousand;

			/*百の位を算出するための計算。*/
			int hundred = (m_calcScoreData % 1000) / 100;
			m_score[enScore_Hundred] = hundred;

			/*十の位を算出するための計算。*/
			int ten = (m_calcScoreData % 100) / 10;
			m_score[enScore_Ten] = ten;

			/*一の位を算出するための計算。*/
			int one = m_calcScoreData % 10;
			m_score[enScore_One] = one;
		}

		/*クリア時間を表示するためのデータの計算。*/
		void Result::CalcClearTimeDisplayData()
		{
			/*〇分を算出するための計算。*/
			m_clearTime[enTime_Minute] = m_calcClearTimeData / 60;

			/*〇分と〇秒の間のコロン*/
			m_clearTime[enTime_Colon] = enClearTimeDisplayUI_Colon;

			/*〇秒を算出するための計算。*/
			int second = (int)m_calcClearTimeData % 60;

			/*○秒を十の位に変換。*/
			 m_clearTime[enTime_Second_Ten] = second / 10;

			/*〇秒を一の位に変換。*/
			float secondOne = second % 10;
			m_clearTime[enTime_Second_One] = secondOne;
		}

		/*スプライトの初期化。*/
		void Result::InitSprite()
		{
			/*背景の初期化。*/
			InitBackGround();

			/*ターゲットUI。*/
			InitTargetUI();

			/*スコア枠UI。*/
			InitScoreFrameUI();

			for (int i = 0; i < enScore_Num; i++)
			{
				/*スコアUI*/
				InitScoreUI((EnScore)i, i);
			}

			/*クリア時間枠UI。*/
			InitClearTimeFrameUI();

			for (int j = 0; j < enTime_Num; j++)
			{
				/*クリア時間UI。*/
				InitClearTimeUI((EnTime)j, j);
			}

			/*ランク枠UI。*/
			InitRankFrameUI();

			/*ランクUI。*/
			InitRankUI();

			/*ボーナスUI。*/
			InitBonusUI();

			/*ノーボーナスUI。*/
			InitNoBonusUI();
		}

		/*背景の初期化。*/
		void Result::InitBackGround()
		{
			m_backGround.Init(m_backGroundFilePath.c_str(), BACK_GROUND_WIDTH, BACK_GROUND_HEIGHT);
		}

		/*ターゲットUIの初期化。*/
		void Result::InitTargetUI()
		{
			m_targetUI.Init(m_targetUIFilePath.c_str(), TARGET_UI_WIDTH, TARGET_UI_HEIGHT);/*初期化。*/
			m_targetUI.SetPosition(TARGET_UI_INIT_POSITION);/*位置設定。*/
			m_targetUI.SetScale(TARGET_UI_INIT_SCALE);/*大きさ設定。*/
			m_targetUI.SetMulColor(TARGET_UI_MUL_COLOR);/*乗算色設定。*/
			m_targetUI.Update();/*更新処理。*/
		}

		/*スコア枠UIの初期化。*/
		void Result::InitScoreFrameUI()
		{
			m_scoreFrameUI.Init(m_scoreFrameUIFilePath.c_str(), SCORE_FRAME_UI_WIDTH, SCORE_FRAME_UI_HEIGHT);/*初期化。*/
			m_scoreFrameUI.SetPosition(SCORE_FRAME_UI_INIT_POSITION);/*位置設定。*/
			m_scoreFrameUI.SetScale(SCORE_FRAME_UI_INIT_SCALE);/*大きさ設定。*/
			m_scoreFrameUI.SetMulColor(SCORE_FRAME_UI_MUL_COLOR);/*乗算色設定。*/
			m_scoreFrameUI.Update();/*更新処理。*/
		}

		/*スコアUIの初期化。*/
		void Result::InitScoreUI(EnScore score, int scoreIndex)
		{
			Vector3 initPosition = SCORE_UI_INIT_POSITION;/*初期位置。*/
			initPosition.x += SCORE_UI_POSITION_INTERVAL * scoreIndex;/*位置の間隔を加算。*/

			m_scoreUI[score].Init(m_scoreUIFilePath[m_score[score]].c_str(), SCORE_UI_WIDTH, SCORE_UI_HEIGHT);/*初期化。*/
			m_scoreUI[score].SetPosition(initPosition);/*位置設定。*/
			m_scoreUI[score].SetScale(SCORE_UI_INIT_SCALE);/*大きさ設定。*/
			m_scoreUI[score].Update();/*更新処理。*/
		}

		/*クリア時間枠UIの初期化。*/
		void Result::InitClearTimeFrameUI()
		{
			m_clearTimeFrameUI.Init(m_clearTimeFrameUIFilePath.c_str(), CLEAR_TIME_FRAME_UI_WIDTH, CLEAR_TIME_FRAME_UI_HEIGHT);/*初期化。*/
			m_clearTimeFrameUI.SetPosition(CLEAR_TIME_FRAME_UI_INIT_POSITION);/*位置設定。*/
			m_clearTimeFrameUI.SetScale(CLEAR_TIME_FRAME_UI_INIT_SCALE);/*大きさ設定。*/
			m_clearTimeFrameUI.SetMulColor(CLEAR_TIME_FRAME_UI_MUL_COLOR);/*乗算色設定。*/
			m_clearTimeFrameUI.Update();/*更新処理。*/
		}

		/*クリア時間UIの初期化。*/
		void Result::InitClearTimeUI(EnTime time, int timeIndex)
		{
			Vector3 initPosition = CLEAR_TIME_UI_INIT_POSITION;/*初期位置。*/
			initPosition.x += CLEAR_TIME_UI_POSITION_INTERVAL * timeIndex;/*位置の間隔を加算。*/

			m_clearTimeUI[time].Init(m_clearTimeUIFilePath[m_clearTime[time]].c_str(), CLEAR_TIME_UI_WIDTH, CLEAR_TIME_UI_HEIGHT);/*初期化。*/
			m_clearTimeUI[time].SetPosition(initPosition);/*位置設定。*/
			m_clearTimeUI[time].SetScale(CLEAR_TIME_UI_INIT_SCALE);/*大きさ設定。*/
			m_clearTimeUI[time].SetMulColor(CLEAR_TIME_UI_MUL_COLOR);/*乗算色設定。*/
			m_clearTimeUI[time].Update();/*更新処理。*/
		}

		/*ランク枠UIの初期化。*/
		void Result::InitRankFrameUI()
		{
			m_rankFrameUI.Init(m_rankFrameUIFilePath.c_str(), RANK_FRAME_UI_WIDTH, RANK_FRAME_UI_HEIGHT);/*初期化。*/
			m_rankFrameUI.SetPosition(RANK_FRAME_UI_INIT_POSITION);/*位置設定。*/
			m_rankFrameUI.SetScale(RANK_FRAME_UI_INIT_SCALE);/*大きさ設定。*/
			m_rankFrameUI.SetMulColor(RANK_FRAME_UI_MUL_COLOR);/*乗算色設定。*/
			m_rankFrameUI.Update();/*更新処理。*/
		}

		/*ランクUIの初期化。*/
		void Result::InitRankUI()
		{
			if (m_rank != enRankUI_None)
			{
				m_rankUI.Init(m_rankUIFilePath[m_rank].c_str(), RANK_UI_WIDTH, RANK_UI_HEIGHT);/*初期化。*/
				m_rankUI.SetPosition(RANK_UI_INIT_POSITION);/*位置設定。*/
				m_rankUI.SetScale(RANK_UI_INIT_SCALE);/*大きさ設定。*/
				m_rankUI.SetMulColor(RANK_UI_MUL_COLOR);/*乗算色設定。*/
				m_rankUI.Update();/*更新処理。*/
			}
		}

		/*ボーナスUIの初期化。*/
		void Result::InitBonusUI()
		{
			if (m_isGetBonus[enBonus_ShotDown])
			{
				m_bonusUI[enBonus_ShotDown].Init(m_bonusUIFilePath[enBonus_ShotDown].c_str(), BONUS_UI_WIDTH, BONUS_UI_HEIGHT);/*初期化。*/
				m_bonusUI[enBonus_ShotDown].SetPosition(SHOT_DOWN_BONUS_UI_INIT_POSITION);/*位置設定。*/
				m_bonusUI[enBonus_ShotDown].SetScale(SHOT_DOWN_BONUS_UI_INIT_SCALE);/*大きさ設定。*/
				m_bonusUI[enBonus_ShotDown].SetMulColor(BONUS_UI_MUL_COLOR);/*乗算色設定。*/
				m_bonusUI[enBonus_ShotDown].Update();/*更新処理。*/
			}

			if (m_isGetBonus[enBonus_SpeedClear])
			{
				m_bonusUI[enBonus_SpeedClear].Init(m_bonusUIFilePath[enBonus_SpeedClear].c_str(), BONUS_UI_WIDTH, BONUS_UI_HEIGHT);/*初期化。*/
				m_bonusUI[enBonus_SpeedClear].SetPosition(SPEED_CLEAR_BONUS_UI_INIT_POSITION);/*位置設定。*/
				m_bonusUI[enBonus_SpeedClear].SetScale(SPEED_CLEAR_BONUS_UI_INIT_SCALE);/*大きさ設定。*/
				m_bonusUI[enBonus_SpeedClear].SetMulColor(BONUS_UI_MUL_COLOR);/*乗算色設定。*/
				m_bonusUI[enBonus_SpeedClear].Update();/*更新処理。*/
			}
		}

		/*ノーボーナスUIの初期化。*/
		void Result::InitNoBonusUI()
		{
			if (!m_isGetBonus[enBonus_ShotDown])
			{
				m_noBonusUI[enBonus_ShotDown].Init(m_noBonusUIFilePath[enBonus_ShotDown].c_str(), NO_BONUS_UI_WIDTH, NO_BONUS_UI_HEIGHT);/*初期化。*/
				m_noBonusUI[enBonus_ShotDown].SetPosition(SHOT_DOWN_NO_BONUS_UI_INIT_POSITION);/*位置設定。*/
				m_noBonusUI[enBonus_ShotDown].SetScale(SHOT_DOWN_NO_BONUS_UI_INIT_SCALE);/*大きさ設定。*/
				m_noBonusUI[enBonus_ShotDown].SetMulColor(NO_BONUS_UI_MUL_COLOR);/*乗算色設定。*/
				m_noBonusUI[enBonus_ShotDown].Update();/*更新処理。*/
			}

			if (!m_isGetBonus[enBonus_SpeedClear])
			{
				m_noBonusUI[enBonus_SpeedClear].Init(m_noBonusUIFilePath[enBonus_SpeedClear].c_str(), NO_BONUS_UI_WIDTH, NO_BONUS_UI_HEIGHT);/*初期化。*/
				m_noBonusUI[enBonus_SpeedClear].SetPosition(SPEED_CLEAR_NO_BONUS_UI_INIT_POSITION);/*位置設定。*/
				m_noBonusUI[enBonus_SpeedClear].SetScale(SPEED_CLEAR_NO_BONUS_UI_INIT_SCALE);/*大きさ設定。*/
				m_noBonusUI[enBonus_SpeedClear].SetMulColor(NO_BONUS_UI_MUL_COLOR);/*乗算色設定。*/
				m_noBonusUI[enBonus_SpeedClear].Update();/*更新処理。*/
			}
		}

		/*UIアニメーションの初期化。*/
		void Result::InitUIAnimation()
		{
			for (int i = 0; i < enScore_Num; i++)
			{
				if (m_score[i] == 0) { continue; };

				/*UIをスライドさせるアニメーション。*/
				InitSlideScoreUIAnimation((EnScore)i);

				/*スコアUIの大きさを変えるアニメーション。*/
				InitScaleScoreUIAnimation((EnScore)i);
			}

			/*UIをスライドさせるアニメーション。*/
			InitSlideUIAnimation();

			/*UIの透明度を変えるアニメーション。*/
			InitAlphaUIAnimation();

			/*ランクUIの大きさを変えるアニメーション。*/
			InitScaleRankUIAnimation();
		}

		/*スコアUIをスライドさせるアニメーション。*/
		void Result::InitSlideScoreUIAnimation(EnScore score)
		{
			/*UIをスライドさせるアニメーションの値の設定。*/
			Vector3 basePosition = m_scoreUI[score].GetPosition();/*元の位置。*/
			Vector3 targetPosition = AFTER_SCORE_UI_ANIMATION_POSITION[score];/*ターゲット位置。*/

			/*初期化。*/
			m_slideScoreUIAnimation[score] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_scoreUI[score],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);
		}

		/*スコアUIの大きさを変えるアニメーション。*/
		void Result::InitScaleScoreUIAnimation(EnScore score)
		{
			/*UIの大きさを変えるアニメーションの値の設定。*/
			Vector2 baseScale = { m_scoreUI[score].GetScale().x, m_scoreUI[score].GetScale().y };/*元の大きさ。*/
			Vector2 targetScale = AFTER_SCORE_UI_ANIMATION_SCALE;/*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleUpScoreUIAnimation[score] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_scoreUI[score],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_UP_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/
			);

			/*UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = AFTER_SCORE_UI_ANIMATION_SCALE;/*元の大きさ。*/
			targetScale = { m_scoreUI[score].GetScale().x, m_scoreUI[score].GetScale().y };/*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleDownScoreUIAnimation[score] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_scoreUI[score],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_DOWN_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/
			);
		}

		/*UIをスライドさせるアニメーション。*/
		void Result::InitSlideUIAnimation()
		{
			/*UIをスライドさせるアニメーションの値の設定。*/
			Vector3 basePosition = m_rankFrameUI.GetPosition();/*元の位置。*/
			Vector3 targetPosition = AFTER_SLIDE_UI_ANIMATION_POSITION[enSlideUIAnimationSprite_RankFrameUI];/*ターゲット位置。*/

			/*初期化。*/
			m_slideUIAnimation[enSlideUIAnimationSprite_RankFrameUI] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
				&m_rankFrameUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				basePosition,/*元の位置。*/
				targetPosition/*ターゲットの位置。*/
			);

			if (m_isGetBonus[enBonus_ShotDown])
			{
				/*UIをスライドさせるアニメーションの値の設定。*/
				basePosition = m_bonusUI[enBonus_ShotDown].GetPosition();/*元の位置。*/
				targetPosition = AFTER_SLIDE_UI_ANIMATION_POSITION[enSlideUIAnimationSprite_BonusUI_ShotDown];/*ターゲット位置。*/

				/*初期化。*/
				m_slideUIAnimation[enSlideUIAnimationSprite_BonusUI_ShotDown] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
					&m_bonusUI[enBonus_ShotDown],/*アニメーションをさせるスプライト。*/
					1.0f,/*ターゲットの割合。*/
					SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
					false,/*ループするか？*/
					0.0f,/*アニメーションを開始する前の遅延時間。*/
					0.0f,/*アニメーションを終了した後の遅延時間。*/
					basePosition,/*元の位置。*/
					targetPosition/*ターゲットの位置。*/
				);
			}
			else
			{
				/*UIをスライドさせるアニメーションの値の設定。*/
				basePosition = m_noBonusUI[enBonus_ShotDown].GetPosition();/*元の位置。*/
				targetPosition = AFTER_SLIDE_UI_ANIMATION_POSITION[enSlideUIAnimationSprite_NoBonusUI_ShotDown];/*ターゲット位置。*/

				/*初期化。*/
				m_slideUIAnimation[enSlideUIAnimationSprite_NoBonusUI_ShotDown] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
					&m_noBonusUI[enBonus_ShotDown],/*アニメーションをさせるスプライト。*/
					1.0f,/*ターゲットの割合。*/
					SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
					false,/*ループするか？*/
					0.0f,/*アニメーションを開始する前の遅延時間。*/
					0.0f,/*アニメーションを終了した後の遅延時間。*/
					basePosition,/*元の位置。*/
					targetPosition/*ターゲットの位置。*/
				);
			}

			if (m_isGetBonus[enBonus_SpeedClear])
			{
				/*UIをスライドさせるアニメーションの値の設定。*/
				basePosition = m_bonusUI[enBonus_SpeedClear].GetPosition();/*元の位置。*/
				targetPosition = AFTER_SLIDE_UI_ANIMATION_POSITION[enSlideUIAnimationSprite_BonusUI_SpeedClear];/*ターゲット位置。*/

				/*初期化。*/
				m_slideUIAnimation[enSlideUIAnimationSprite_BonusUI_SpeedClear] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
					&m_bonusUI[enBonus_SpeedClear],/*アニメーションをさせるスプライト。*/
					1.0f,/*ターゲットの割合。*/
					SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
					false,/*ループするか？*/
					0.0f,/*アニメーションを開始する前の遅延時間。*/
					0.0f,/*アニメーションを終了した後の遅延時間。*/
					basePosition,/*元の位置。*/
					targetPosition/*ターゲットの位置。*/
				);
			}
			else
			{
				/*UIをスライドさせるアニメーションの値の設定。*/
				basePosition = m_noBonusUI[enBonus_SpeedClear].GetPosition();/*元の位置。*/
				targetPosition = AFTER_SLIDE_UI_ANIMATION_POSITION[enSlideUIAnimationSprite_NoBonusUI_SpeedClear];/*ターゲット位置。*/

				/*初期化。*/
				m_slideUIAnimation[enSlideUIAnimationSprite_NoBonusUI_SpeedClear] = std::make_unique<nsApp::nsUI::PositionUIAnimation>(
					&m_noBonusUI[enBonus_SpeedClear],/*アニメーションをさせるスプライト。*/
					1.0f,/*ターゲットの割合。*/
					SLIDE_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
					false,/*ループするか？*/
					0.0f,/*アニメーションを開始する前の遅延時間。*/
					0.0f,/*アニメーションを終了した後の遅延時間。*/
					basePosition,/*元の位置。*/
					targetPosition/*ターゲットの位置。*/
				);
			}
		}

		/*UIの透明度を変えるアニメーション。*/
		void Result::InitAlphaUIAnimation()
		{
			/*UIの透明度を変えるアニメーションの値の設定。*/
			float baseAlpha = m_targetUI.GetMulColor().a;/*元の透明度。*/
			float targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_TargetUI] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_targetUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_scoreFrameUI.GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_ScoreFrameUI] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_scoreFrameUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_clearTimeFrameUI.GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_ClearTimeFrameUI] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_clearTimeFrameUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_clearTimeUI[enTime_Minute].GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_ClearTimeUI_Minute] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_clearTimeUI[enTime_Minute],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_clearTimeUI[enTime_Colon].GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_ClearTimeUI_Colon] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_clearTimeUI[enTime_Colon],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_clearTimeUI[enTime_Second_Ten].GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_ClearTimeUI_Second_Ten] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_clearTimeUI[enTime_Second_Ten],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);
			
			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_clearTimeUI[enTime_Second_One].GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_ClearTimeUI_Second_One] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_clearTimeUI[enTime_Second_One],/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_rankFrameUI.GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_RankFrameUI] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_rankFrameUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);
			
			/*UIの透明度を変えるアニメーションの値の設定。*/
			baseAlpha = m_rankUI.GetMulColor().a;/*元の透明度。*/
			targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

			/*初期化。*/
			m_alphaUIAnimation[enAlphaUIAnimationSprite_RankUI] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
				&m_rankUI,/*アニメーションをさせるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.3f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseAlpha,/*元の透明度。*/
				targetAlpha/*ターゲットの透明度。*/
			);

			if (m_isGetBonus[enBonus_ShotDown])
			{
				/*UIの透明度を変えるアニメーションの値の設定。*/
				baseAlpha = m_bonusUI[enBonus_ShotDown].GetMulColor().a;/*元の透明度。*/
				targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

				/*初期化。*/
				m_alphaUIAnimation[enAlphaUIAnimationSprite_BonusUI_ShotDown] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
					&m_bonusUI[enBonus_ShotDown],/*アニメーションをさせるスプライト。*/
					1.0f,/*ターゲットの割合。*/
					ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
					false,/*ループするか？*/
					0.0f,/*アニメーションを開始する前の遅延時間。*/
					0.0f,/*アニメーションを終了した後の遅延時間。*/
					baseAlpha,/*元の透明度。*/
					targetAlpha/*ターゲットの透明度。*/
				);
			}
			else
			{
				/*UIの透明度を変えるアニメーションの値の設定。*/
				baseAlpha = m_noBonusUI[enBonus_ShotDown].GetMulColor().a;/*元の透明度。*/
				targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

				/*初期化。*/
				m_alphaUIAnimation[enAlphaUIAnimationSprite_NoBonusUI_ShotDown] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
					&m_noBonusUI[enBonus_ShotDown],/*アニメーションをさせるスプライト。*/
					1.0f,/*ターゲットの割合。*/
					ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
					false,/*ループするか？*/
					0.0f,/*アニメーションを開始する前の遅延時間。*/
					0.0f,/*アニメーションを終了した後の遅延時間。*/
					baseAlpha,/*元の透明度。*/
					targetAlpha/*ターゲットの透明度。*/
				);
			}

			if (m_isGetBonus[enBonus_SpeedClear])
			{
				/*UIの透明度を変えるアニメーションの値の設定。*/
				baseAlpha = m_bonusUI[enBonus_SpeedClear].GetMulColor().a;/*元の透明度。*/
				targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

				/*初期化。*/
				m_alphaUIAnimation[enAlphaUIAnimationSprite_BonusUI_SpeedClear] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
					&m_bonusUI[enBonus_SpeedClear],/*アニメーションをさせるスプライト。*/
					1.0f,/*ターゲットの割合。*/
					ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
					false,/*ループするか？*/
					0.0f,/*アニメーションを開始する前の遅延時間。*/
					0.0f,/*アニメーションを終了した後の遅延時間。*/
					baseAlpha,/*元の透明度。*/
					targetAlpha/*ターゲットの透明度。*/
				);
			}
			else
			{
				/*UIの透明度を変えるアニメーションの値の設定。*/
				baseAlpha = m_noBonusUI[enBonus_SpeedClear].GetMulColor().a;/*元の透明度。*/
				targetAlpha = AFTER_ALPHA_UI_ANIMATION_ALPHA;/*ターゲットの透明度。*/

				/*初期化。*/
				m_alphaUIAnimation[enAlphaUIAnimationSprite_NoBonusUI_SpeedClear] = std::make_unique<nsApp::nsUI::AlphaUIAnimation>(
					&m_noBonusUI[enBonus_SpeedClear],/*アニメーションをさせるスプライト。*/
					1.0f,/*ターゲットの割合。*/
					ALPHA_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
					false,/*ループするか？*/
					0.0f,/*アニメーションを開始する前の遅延時間。*/
					0.0f,/*アニメーションを終了した後の遅延時間。*/
					baseAlpha,/*元の透明度。*/
					targetAlpha/*ターゲットの透明度。*/
				);
			}
		}

		/*ランクUIの大きさを変えるアニメーション。*/
		void Result::InitScaleRankUIAnimation()
		{
			/*UIの大きさを変えるアニメーションの値の設定。*/
			Vector2 baseScale = { m_rankUI.GetScale().x, m_rankUI.GetScale().y };/*元の大きさ。*/
			Vector2 targetScale = AFTER_RANK_UI_ANIMATION_SCALE[enScale_Down][enScaleRankUIAnimationCount_One];/*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleDownRankUIAnimation[enScaleRankUIAnimationCount_One] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_rankUI,/*大きさを変えるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_DOWN_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.3f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/
			);

			/*UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = AFTER_RANK_UI_ANIMATION_SCALE[enScale_Down][enScaleRankUIAnimationCount_One];/*元の大きさ。*/
			targetScale = AFTER_RANK_UI_ANIMATION_SCALE[enScale_Up][enScaleRankUIAnimationCount_One];/*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleUpRankUIAnimation[enScaleRankUIAnimationCount_One] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_rankUI,/*大きさを変えるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_UP_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/
			);

			/*UIの大きさを変えるアニメーションの値の設定。*/
			baseScale = AFTER_RANK_UI_ANIMATION_SCALE[enScale_Up][enScaleRankUIAnimationCount_One];/*元の大きさ。*/
			targetScale = AFTER_RANK_UI_ANIMATION_SCALE[enScale_Down][enScaleRankUIAnimationCount_Two];/*ターゲットの大きさ。*/

			/*初期化。*/
			m_scaleDownRankUIAnimation[enScaleRankUIAnimationCount_Two] = std::make_unique<nsApp::nsUI::ScaleUIAnimation>(
				&m_rankUI,/*大きさを変えるスプライト。*/
				1.0f,/*ターゲットの割合。*/
				SCALE_DOWN_UI_ANIMATION_PLAY_SPEED,/*アニメーションの再生速度。*/
				false,/*ループするか？*/
				0.0f,/*アニメーションを開始する前の遅延時間。*/
				0.0f,/*アニメーションを終了した後の遅延時間。*/
				baseScale,/*元の大きさ。*/
				targetScale/*ターゲットの大きさ。*/
			);
		}

		/*演出の更新処理。*/
		void Result::UpdateDirection()
		{
			if (!m_slideScoreUIAnimation[enScore_One]->IsEnd())
			{
				for (int i = 0; i < enScore_Num; i++)
				{
					if (m_score[i] == 0) { continue; };
					/*スコアUIをスライドさせるアニメーション。*/
					m_slideScoreUIAnimation[i]->Update();
				}
			}
			else if (!m_scaleUpScoreUIAnimation[enScore_One]->IsEnd())
			{
				for (int i = 0; i < enScore_Num; i++)
				{
					if (m_score[i] == 0) { continue; };
					/*UIをスライドさせるアニメーション。*/
					m_scaleUpScoreUIAnimation[i]->Update();
				}
			}
			else if (!m_scaleDownScoreUIAnimation[enScore_One]->IsEnd())
			{
				for (int i = 0; i < enSlideUIAnimationSprite_Num; i++)
				{
					if (m_slideUIAnimation[i] == nullptr) { continue; }

					/*UIをスライドさせるアニメーション。*/
					m_slideUIAnimation[i]->Update();
				}

				for (int j = 0; j < enAlphaUIAnimationSprite_Num; j++)
				{
					if (j == enAlphaUIAnimationSprite_RankUI) { continue; }
					if (m_alphaUIAnimation[j] == nullptr) { continue; }

					/*UIの透明度を変えるアニメーション。*/
					m_alphaUIAnimation[j]->Update();
				}

				for (int k = 0; k < enScore_Num; k++)
				{
					if (m_score[k] == 0) { continue; };

					/*UIをスライドさせるアニメーション。*/
					m_scaleDownScoreUIAnimation[k]->Update();
				}
			}
			else if (!m_scaleDownRankUIAnimation[enScaleRankUIAnimationCount_One]->IsEnd())
			{
				/*UIの透明度を変えるアニメーション。*/
				m_alphaUIAnimation[enAlphaUIAnimationSprite_RankUI]->Update();

				/*ランクUIの大きさを小さくするアニメーション。*/
				m_scaleDownRankUIAnimation[enScaleRankUIAnimationCount_One]->Update();
			}
			else if (!m_scaleUpRankUIAnimation[enScaleRankUIAnimationCount_One]->IsEnd())
			{
				/*ランクUIの大きさを大きくするアニメーション。*/
				m_scaleUpRankUIAnimation[enScaleRankUIAnimationCount_One]->Update();
			}
			else if (!m_scaleDownRankUIAnimation[enScaleRankUIAnimationCount_Two]->IsEnd())
			{
				/*ランクUIの大きさを小さくするアニメーション。*/
				m_scaleDownRankUIAnimation[enScaleRankUIAnimationCount_Two]->Update();
			}
			else
			{
				m_isDirectionFinished = true;/*演出終了。*/
			}
		}

		/*スプライトの更新処理。*/
		void Result::UpdateSprite()
		{
			/*背景。*/
			m_backGround.Update();

			/*ターゲットUI。*/
			m_targetUI.Update();

			/*スコア枠UI。*/
			m_scoreFrameUI.Update();

			for (int i = 0; i < enBonus_Num; i++)
			{
				/*スコアUI。*/
				m_scoreUI[i].Update();
			}

			/*クリア時間枠UI。*/
			m_clearTimeFrameUI.Update();

			for (int j = 0; j < enBonus_Num; j++)
			{
				/*クリア時間UI。*/
				m_clearTimeUI[j].Update();
			}

			/*ランク枠UI。*/
			m_rankFrameUI.Update();

			if (m_rank != enRankUI_None)
			{
				/*ランクUI。*/
				m_rankUI.Update();
			}

			for (int k = 0; k < enBonus_Num; k++)
			{
				/*ボーナスUI。*/
				m_bonusUI[k].Update();

				/*ノーボーナスUI。*/
				m_noBonusUI[k].Update();
			}
		}
	}
}