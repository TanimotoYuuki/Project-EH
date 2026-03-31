#pragma once
/**
 * @file GameTimeLimit.h。
 * @brief 制限時間クラス。
 * @author Tanimoto。
 * @date 2026/03/23。
 */
namespace nsApp
{
	namespace nsGame
	{
		/*制限時間クラス。*/
		class GameTimeLimit : public IGameObject
		{
		public:
			GameTimeLimit() {};/*コンストラクタ。*/
			~GameTimeLimit() {};/*デストラクタ。*/
			
		public:
			bool Start() override;/*開始処理。*/
			void Update() override;/*更新処理。*/
			void Render(RenderContext& rc) override;/*描画処理。*/

		public:/*列挙型。*/

			/*制限時間で表示するUI。*/
			enum EnTimeLimitDisplayUI : uint8_t
			{
				enTimeLimitDisplayUI_Zero,/*0。*/
				enTimeLimitDisplayUI_One,/*1。*/
				enTimeLimitDisplayUI_Two,/*2。*/
				enTimeLimitDisplayUI_Three,/*3。*/
				enTimeLimitDisplayUI_Four,/*4。*/
				enTimeLimitDisplayUI_Five,/*5。*/
				enTimeLimitDisplayUI_Six,/*6。*/
				enTimeLimitDisplayUI_Seven,/*7。*/
				enTimeLimitDisplayUI_Eight,/*8。*/
				enTimeLimitDisplayUI_Nine,/*9。*/
				enTimeLimitDisplayUI_Colon,/*〇分と〇秒の間のコロン。*/
				enTimeLimitDisplayUI_Num/*リザルトで描画するUIの数。*/
			};

			/*時間。*/
			enum EnTime : uint8_t
			{
				enTime_Minute,/*〇分。*/
				enTime_Colon,/*〇分と〇秒の間のコロン。*/
				enTime_Second_Ten,/*〇秒の十の位。*/
				enTime_Second_One,/*〇秒の一の位。*/
				enTime_Num/*時間の数。*/
			};

		private:/*メンバ関数。*/

			/**
			* @brief スプライトの初期化。
			*/
			void InitSprite();

			/**
			* @brief 制限時間UIの初期化。
			* @param time 時間。
			* @param timeLimitDisplay 描画するUI。
			*/
			void InitTimeLimitUI(EnTime time, EnTimeLimitDisplayUI timeLimitDisplay);

			/**
			* @brief タイマーUIの初期化。
			*/
			void InitTimerUI();

			/**
			* @brief 制限時間UIの更新処理。
			*/
			void TimeLimitUIUpdate();

			/**
			* @brief 制限時間UIを表示するための計算。
			*/
			void CalcDrawingTimeLimitUI();

			/**
			* @brief 制限時間UIの位置の更新処理。
			* @param timeLimitData 制限時間UIに使うデータ。
			* @param dataNum 制限時間UIに使うデータの順番。
			* @param timeLimitUI 制限時間UI。
			*/
			void TimeLimitUIPositionUpdate(std::vector<EnTimeLimitDisplayUI> timeLimitData, int dataNum, SpriteRender& timeLimitUI);

		public:/*メンバ関数。*/

			/**
			* @brief 制限時間の設定。
			* @param time 制限時間。
			*/
			inline void SetTimeLimit(float time)
			{
				m_timeLimit = time;
			}

			/**
			* @brief 制限時間の取得。
			* @return 制限時間。
			*/
			inline float GetTimeLimit() const
			{
				return m_timeLimit;
			}

			/**
			* @brief 時間切れになったかどうか？
			* @return trueなら時間切れになっている。
			*/
			inline bool IsTimeUp() const
			{
				return m_isTimeUp;
			}

			/**
			* @brief 制限時間を止める状態にする。
			*/
			inline void EnableTimeStop()
			{
				m_isTimeStop = true;
			}

			/**
			* @brief 制限時間を止めない状態にする。
			*/
			inline void DisableTimeStop()
			{
				m_isTimeStop = false;
			}

			/**
			* @brief 時間を止めているか？
			* @return trueなら時間を止めている。
			*/
			inline bool IsTimeStop() const
			{
				return m_isTimeStop;
			}

		private:/*メンバ変数。*/
			SpriteRender m_timeLimitUI[enTime_Num][enTimeLimitDisplayUI_Num];/*制限時間UI。*/
			SpriteRender m_timerUI;/*タイマーUI。*/
			float m_timeLimit = 0.0f;/*制限時間。*/
			bool m_isTimeUp = false;/*時間切れになったかどうか？*/
			bool m_isDrawingUI = false;/*UIを描画するか？*/
			bool m_isTimeStop = false;/*時間を止めるか ? */
			std::vector<EnTimeLimitDisplayUI> m_timeLimitManage;/*制限時間を管理する変数。*/

		private:/*スプライトを表示するファイルパス用のメンバ変数。*/
			const std::string m_timeLimitDisplayUIFilePath[enTimeLimitDisplayUI_Num] = {
				"Assets/sprite/inGame/time/zero.dds",/*0。*/
				"Assets/sprite/inGame/time/one.dds",/*1。*/
				"Assets/sprite/inGame/time/two.dds",/*2。*/
				"Assets/sprite/inGame/time/three.dds",/*3。*/
				"Assets/sprite/inGame/time/four.dds",/*4。*/
				"Assets/sprite/inGame/time/five.dds",/*5。*/
				"Assets/sprite/inGame/time/six.dds",/*6。*/
				"Assets/sprite/inGame/time/seven.dds",/*7。*/
				"Assets/sprite/inGame/time/eight.dds",/*8。*/
				"Assets/sprite/inGame/time/nine.dds",/*9。*/
				"Assets/sprite/inGame/time/colon.dds",/*〇分と〇秒の間のコロン。*/
			};/*制限時間UIのファイルパス。*/
			const std::string m_timerUIFilePath = "Assets/sprite/inGame/time/timer.dds";/*タイマーUIのファイルパス。*/
		};
	}
}

