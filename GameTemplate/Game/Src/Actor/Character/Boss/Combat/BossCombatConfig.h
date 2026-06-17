#pragma once
/**
 * @file   BossCombatConfig.h
 * @brief  ボス戦闘（火球・Rain・竜巻・フェーズ倍率）の定数定義。
 * @date   2026/06/15
 */

namespace nsApp
{
	namespace nsActor
	{
		/**
		 * @namespace BossCombatConfig
		 * @brief ボス戦闘パラメータを集約する。インスタンス化しない。
		 */
		namespace BossCombatConfig
		{
			/* フェーズ強化倍率 */
			static constexpr float PHASE1_DAMAGE_MULTIPLIER = 1.3f;		//! フェーズ1攻撃力倍率。
			static constexpr float PHASE2_DAMAGE_MULTIPLIER = 1.3f;		//! フェーズ2攻撃力倍率。
			static constexpr float PHASE1_IDLE_TIME_MULTIPLIER = 0.75f; //! フェーズ1待機時間倍率。
			static constexpr float PHASE2_IDLE_TIME_MULTIPLIER = 0.5f;  //! フェーズ2待機時間倍率。

			/* ステータス初期値 */
			static constexpr float BASE_ATTACK_DAMAGE = 50.0f;			//! 通常攻撃力初期値。
			static constexpr float BASE_CRITICAL_DAMAGE = 80.0f;		//! クリティカル攻撃力初期値。
			static constexpr float BASE_CRITICAL_RATE = 0.1f;		    //! クリティカル率初期値。

			/* 火球飛翔 */
			static constexpr float FIREBALL_SPEED = 85.0f;				//! 火球速度。
			static constexpr float FIREBALL_LIFE = 3.0f;				//! 火球寿命（秒）。

			/* 口位置オフセット */
			static constexpr float MOUTH_FORWARD = 110.0f;				//! 口先方向オフセット。
			static constexpr float MOUTH_HEIGHT = 90.0f;			    //! 口高さオフセット。
			static constexpr float AIM_HEIGHT_OFFSET = 30.0f;		    //! 照準Y補正（胸〜上半身）。

			/* ステージ地面・着弾炎柱 */
			static constexpr float STAGE_GROUND_Y = 50.0f;              //! キャラ足元Y。
			static constexpr float LANDING_EFFECT_Y_OFFSET = -15.0f;    //! 炎柱表示Y補正。
			static constexpr float LANDING_EFFECT_DURATION = 3.0f;      //! 火球着弾後の炎柱持続（秒）。
			static constexpr float LANDING_EFFECT_SPAWN_INTERVAL = 0.8f;//! 炎柱エフェクト再生成間隔（秒）。
			static constexpr float RAIN_LANDING_DURATION = 2.5f;        //! Rain着弾後の炎柱持続（秒）。

			/* 飛翔中火球エフェクト（当たり判定と分離） */
			static constexpr float FIREBALL_EFFECT_Y_OFFSET = 0.0f;     //! 上下補正。
			static constexpr float FIREBALL_EFFECT_Z_OFFSET = -10.0f;   //! 手前→奥補正。
			static constexpr float FIREBALL_EFFECT_SCALE = 6.0f;        //! 火球エフェクトスケール。
			static constexpr float RAIN_EFFECT_SCALE = 5.0f;            //! Rainエフェクトスケール。
			static constexpr float LANDING_PILLAR_SCALE = 5.0f;         //! 着弾炎柱スケール。

			/* Burst */
			static constexpr float BURST_SHOT_INTERVAL = 0.35f;         //! 3連射間隔（秒）。
			static constexpr int   BURST_SHOT_COUNT = 3;			    //! 連射数。

			/* Rain */
			static constexpr float RAIN_DURATION = 4.0f;			    //! 降り注ぎ時間（秒）。
			static constexpr float RAIN_SPAWN_INTERVAL = 0.75f;			//! Drop生成間隔（秒）。
			static constexpr float RAIN_SPAWN_HEIGHT = 260.0f;			//! 生成高さオフセット。
			static constexpr float RAIN_FALL_SPEED = 120.0f;			//! 降下速度。
			static constexpr int   MAX_RAIN_DROPS = 16;					//! Rain Drop最大数。

			/* 火球ヒット判定半径 */
			static constexpr float FIRE_HIT_RADIUS = 15.0f;             //! 飛翔中火球の当たり半径。
			static constexpr float LANDING_HIT_RADIUS = 35.0f;          //! 着弾炎柱の当たり半径。
			static constexpr float RAIN_HIT_RADIUS = 18.0f;             //! Rain Drop の当たり半径。

			/* 照準・方向 */
			static constexpr float MIN_DIRECTION_LENGTH_SQ = 0.01f;     //! 方向ベクトルが無効とみなす長さの2乗。
			static constexpr float DEFAULT_AIM_DIR_X = -1.0f;           //! 方向未設定時のフォールバック X。
			static constexpr float DEFAULT_AIM_DIR_Y = 0.0f;            //! 方向未設定時のフォールバック Y。
			static constexpr float DEFAULT_AIM_DIR_Z = 0.0f;            //! 方向未設定時のフォールバック Z。

			/* 火炎パターン抽選（Straight / Burst / Rain） */
			static constexpr int FIRE_PATTERN_ROLL_MIN = 0;             //! パターン抽選の下限。
			static constexpr int FIRE_PATTERN_ROLL_MAX = 2;             //! パターン抽選の上限。

			/* Rain スポーン位置 */
			static constexpr int RAIN_SPAWN_X_OFFSET_MIN = -120;        //! ターゲット基準 X オフセット最小。
			static constexpr int RAIN_SPAWN_X_OFFSET_MAX = 120;         //! ターゲット基準 X オフセット最大。

			/* ステージ座標 */
			static constexpr float STAGE_CENTER_Z = 0.0f;               //! ステージ中央 Z（着弾判定用）。
			static constexpr float LANDING_POS_UNSET_Y = 0.0f;          //! 着弾位置未設定とみなす Y 閾値。

			/* Rain Drop 初速（Y のみ降下） */
			static constexpr float RAIN_VELOCITY_X = 0.0f;              //! Rain Drop 水平速度 X。
			static constexpr float RAIN_VELOCITY_Z = 0.0f;              //! Rain Drop 水平速度 Z。

			/* 竜巻 */
			static constexpr int   MAX_TORNADOES = 4;					//! 竜巻最大数。
			static constexpr float TORNADO_SPEED = 62.0f;				//! 竜巻移動速度。
			static constexpr float TORNADO_BASE_SCALE = 5.0f;			//! 竜巻基本スケール。
			static constexpr float TORNADO_EFFECT_Y_OFFSET = 0.0f;		//! 竜巻エフェクト上下補正。
			static constexpr float TORNADO_EFFECT_Z_OFFSET = -15.0f;	//! 竜巻エフェクト手前→奥補正。
			static constexpr float TORNADO_STAGE_MIN_X = -110.0f;		//! 竜巻移動範囲X最小値（ステージ左端からの相対位置）。
			static constexpr float TORNADO_STAGE_MAX_X = 110.0f;		//! 竜巻移動範囲X最大値（ステージ右端からの相対位置）。
			static constexpr float TORNADO_STAGE_EXIT_MARGIN = 30.0f;	//! 竜巻がステージ外に出るとみなすマージン。
			static constexpr float TORNADO_PUSH_LIFE = 4.5f;			//! 竜巻押し出し時間（秒）。
			static constexpr float TORNADO_PASS_LIFE = 4.0f;			//! 竜巻通過時間（秒）。押し出しと合わせて約8.5秒で完全に消える想定。
			static constexpr float TORNADO_WANDER_LIFE = 5.0f;			//! 竜巻徘徊時間（秒）。移動開始から消えるまでの時間。押し出しと通過時間の合計より長く、ステージ内を徘徊する時間を確保。
			static constexpr float TORNADO_WANDER_RANGE = 55.0f;		//! 竜巻徘徊範囲（中心からの最大距離）。ステージ幅の半分程度で、竜巻がステージ内を動き回るのに十分な範囲。
			static constexpr float TORNADO_WANDER_FREQ = 2.8f;			//! 竜巻徘徊方向変化頻度（秒）。この値で竜巻の動きがどれくらい頻繁に変わるかを調整。小さいほど頻繁に動きが変わる。
			static constexpr float TORNADO_DAMAGE_INTERVAL = 0.45f;		//! 竜巻ダメージ間隔（秒）。竜巻に触れてから次にダメージを受けるまでの時間。これで竜巻のダメージ頻度を調整。
			static constexpr float TORNADO_EFFECT_RESPAWN = 0.20f;		//! 竜巻エフェクト再生成間隔（秒）。これで竜巻のエフェクトがどれくらい頻繁に更新されるかを調整。小さいほどエフェクトが滑らかになる。
			static constexpr float TORNADO_DAMAGE_RATE = 0.65f;			//! 竜巻ダメージ倍率。これで竜巻のダメージ量を調整。通常攻撃の何倍のダメージを与えるかを設定。
		}
	}
}