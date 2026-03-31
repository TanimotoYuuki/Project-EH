#include "stdafx.h"
#include "GameTimeLimit.h"

namespace {
	/*制限時間UI*/
	const float TIME_LIMIT_UI_WIDTH = 1024.0f;/*制限時間UIの幅。*/

	const float TIME_LIMIT_UI_HEIGHT = 128.0f;/*制限時間UIの高さ。*/

	const Vector3 TIME_LIMIT_UI_INIT_POSITION{ -700.0f,425.0f,0.0f };/*制限時間UIの初期位置。*/

	const float TIME_LIMIT_UI_POSITION_INTERVAL = 60.0f;/*制限時間UIの位置の間隔。*/

	const Vector3 TIME_LIMIT_UI_INIT_SCALE{ 0.7f,0.7f,1.0f };/*制限時間UIの初期大きさ。*/

	/*タイマーUI。*/
	const float TIMER_UI_WIDTH = 1024;//タイマーUIの幅。

	const float TIMER_UI_HEIGHT = 1024;//タイマーUIの高さ。

	const Vector3 TIMER_UI_INIT_POSITION(-875.0f, 425.0f, 0.0f);//タイマーUIの位置。

	const Vector3 TIMER_UI_INIT_SCALE(0.125f, 0.125f, 1.0f);//タイマーUIの大きさ。
}

namespace nsApp
{
	namespace nsGame
	{
		/*開始演出。*/
		bool GameTimeLimit::Start()
		{
			/*スプライトの初期化。*/
			InitSprite();

			m_timeLimitManage.reserve(6);

			/*制限時間UIの更新処理。*/
			TimeLimitUIUpdate();

			return true;
		}

		/*更新処理。*/
		void GameTimeLimit::Update()
		{
			//タイムアップしたら処理しない。
			if (m_isTimeUp || IsTimeStop())
			{
				return;
			}

			/*毎フレーム制限時間を管理する変数で格納しているデータを削除する。*/
			m_timeLimitManage.clear();

			m_timeLimit -= g_gameTime->GetFrameDeltaTime();

			if (m_timeLimit < 0.0f)
			{
				m_timeLimit = 0.0f;
				m_isTimeUp = true;
			}

			/*制限時間UIの更新処理。*/
			TimeLimitUIUpdate();
			
			m_timerUI.Update();
		}

		/*描画処理。*/
		void GameTimeLimit::Render(RenderContext& rc)
		{
			for (int i = 0; i < m_timeLimitManage.size(); i++)
			{
				/*制限時間UIの描画。*/
				m_timeLimitUI[i][m_timeLimitManage[i]].Draw(rc);
			}

			/*タイマーUIの描画。*/
			m_timerUI.Draw(rc);
		}

		/*スプライトの初期化。*/
		void GameTimeLimit::InitSprite()
		{
			for (int i = 0; i < enTime_Num; i++)
			{
				for (int j = 0; j < enTimeLimitDisplayUI_Num; j++)
				{
					/*制限時間UI。*/
					InitTimeLimitUI((EnTime)i, (EnTimeLimitDisplayUI)j);
				}
			}

			/*タイマーUI。*/
			InitTimerUI();
		}

		/*制限時間UIの初期化。*/
		void GameTimeLimit::InitTimeLimitUI(EnTime enTime, EnTimeLimitDisplayUI enTimeLimitDisplayUI)
		{
			m_timeLimitUI[enTime][enTimeLimitDisplayUI].Init(m_timeLimitDisplayUIFilePath[enTimeLimitDisplayUI].c_str(), TIME_LIMIT_UI_WIDTH, TIME_LIMIT_UI_HEIGHT);/*初期化。*/
			m_timeLimitUI[enTime][enTimeLimitDisplayUI].SetPosition(TIME_LIMIT_UI_INIT_POSITION);/*位置設定。*/
			m_timeLimitUI[enTime][enTimeLimitDisplayUI].SetScale(TIME_LIMIT_UI_INIT_SCALE);/*大きさ設定。*/
			m_timeLimitUI[enTime][enTimeLimitDisplayUI].Update();/*更新処理。*/
		}

		/*タイマーUIの初期化。*/
		void GameTimeLimit::InitTimerUI()
		{
			m_timerUI.Init(m_timerUIFilePath.c_str(), TIMER_UI_WIDTH, TIMER_UI_HEIGHT);/*初期化。*/
			m_timerUI.SetPosition(TIMER_UI_INIT_POSITION);/*位置設定。*/
			m_timerUI.SetScale(TIMER_UI_INIT_SCALE);/*大きさ設定。*/
			m_timerUI.Update();/*更新処理。*/
		}

		/*制限時間UIの更新処理。*/
		void GameTimeLimit::TimeLimitUIUpdate()
		{
			/*制限時間UIを表示するための計算。*/
			CalcDrawingTimeLimitUI();

			for (int i = 0; i < m_timeLimitManage.size(); i++)
			{
				/*制限時間UIの位置の更新処理。*/
				TimeLimitUIPositionUpdate(m_timeLimitManage, i, m_timeLimitUI[(EnTime)i][m_timeLimitManage[i]]);
			}
		}

		/*制限時間UIを表示するための計算。*/
		void GameTimeLimit::CalcDrawingTimeLimitUI()
		{
			/*残り時間の〇分の部分を求める。*/
			int minute = m_timeLimit / 60;

			/*残り時間の〇秒の部分を求める。*/
			int second = (int)m_timeLimit % 60;

			/*○秒の十の位に変換。*/
			int secondTen = second / 10;

			/*〇秒の一の位に変換。*/
			float secondOne = second % 10;

			/*リザルトのデータの格納。*/
			m_timeLimitManage.push_back((EnTimeLimitDisplayUI)minute);
			m_timeLimitManage.push_back(enTimeLimitDisplayUI_Colon);
			m_timeLimitManage.push_back((EnTimeLimitDisplayUI)secondTen);
			m_timeLimitManage.push_back((EnTimeLimitDisplayUI)secondOne);
		}

		//制限時間UIの位置の更新処理
		void GameTimeLimit::TimeLimitUIPositionUpdate(std::vector<EnTimeLimitDisplayUI> timeLimitData, int dataNum, SpriteRender& timeLimitUI)
		{
			/*制限時間データの総数を取得。*/
			const int resultDataNum = timeLimitData.size();

			/*UI全体の幅を計算。*/
			const float totalWidth = (resultDataNum - 1) * TIME_LIMIT_UI_POSITION_INTERVAL;

			/*最初のUIを配置すべきX座標を計算。*/
			const float startX = TIME_LIMIT_UI_INIT_POSITION.x - totalWidth / 2.0f;

			/*現在の描画するUIのX座標を計算。*/
			const float posX = startX + dataNum * TIME_LIMIT_UI_POSITION_INTERVAL;

			/*現在のUIの位置の取得。*/
			Vector3 pos = timeLimitUI.GetPosition();
			pos.x = posX; /*計算したX座標を適用。*/

			/*制限時間UIの位置の設定。*/
			timeLimitUI.SetPosition(pos);
			/*制限時間UIの更新処理。*/
			timeLimitUI.Update();
		}
	}
}