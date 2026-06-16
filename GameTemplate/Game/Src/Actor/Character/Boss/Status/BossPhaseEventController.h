#pragma once

/**
 * @file   BossPhaseEventController.h
 * @brief  ボスのHP閾値を監視し、フェーズイベントを制御するクラス。
 * @date   2026/06/12
 */

#include <vector>

namespace nsApp
{
	class Camera;
	class PlayerControlerHub;

	namespace nsActor { class Boss; class Player; }
	namespace nsUI { class CommentaryUIManager; }
	namespace nsGame { class GameTimeLimit; }


	/**
	 * @class BossPhaseEventController
	 * @brief ボスのHP閾値を監視し、フェーズイベントを制御する。
	 */
	class BossPhaseEventController
	{
	public:
		/* コンストラクタとデストラクタ。*/
		BossPhaseEventController() = default;
		~BossPhaseEventController() = default;


	public:
		/**
		 * @brief 初期化処理。
		 * @param boss       ボスのポインタ。
		 * @param camera     カメラのポインタ。
		 * @param commentary コメンタリーUIマネージャーのポインタ。
		 * @param timeLimit  ゲーム制限時間のポインタ。
		 * @param playerHub  プレイヤーコントローラーハブのポインタ。
		 * @param players    吹っ飛ばし対象のプレイヤーリスト。
		 */
		void Initialize(nsActor::Boss* boss, Camera* camera, nsUI::CommentaryUIManager* commentary, nsGame::GameTimeLimit* timeLimit, PlayerControlerHub* playerHub, const std::vector<nsActor::Player*>& players);

		/**
		 * @brief 更新処理。Game2::Update()から毎フレーム呼び出す。
		 */
		void Update();


	public:
		/**
		 * @brief 咆哮イベントが進行中かどうか。
		 * @return 進行中の場合 true。
		 */
		inline bool IsEventActive() const
		{
			return m_isEventActive;
		}


	private:
		/**
		 * @brief HP閾値の変化を検出し、各フェーズ処理を呼び出す。
		 */
		void CheckPhaseTransitions();

		/**
		 * @brief 50%フェーズイベントを開始する。
		 */
		void TriggerPhase1Event();

		/**
		 * @brief 25%フェーズ演出を行う。
		 */
		void ApplyPhase2Effect();

		/**
		 * @brief 咆哮イベントを終了し、復帰待ちタイマーを開始する。
		 */
		void EndEvent();


	private:
		static constexpr float SHAKE_DURATION = 3.0f;			//! カメラシェイクの時間（秒）。
		static constexpr float SHAKE_INTENSITY = 15.0f;			//! カメラシェイクの強さ。
		static constexpr float BLOWAWAY_VELOCITY = 15.0f;		//! 吹っ飛びの初速。

		static constexpr float SHAKE_DELAY = 1.0f;				//! アナウンス後にシェイクを開始するまでの秒数。
		static constexpr float BLOWAWAY_DELAY = 2.0f;			//! シェイク後に吹っ飛びを発生させるまでの秒数。
		static constexpr float RECOVERY_DELAY = 2.0f;			//! 咆哮終了後にプレイヤーが起き上がるまでの秒数。


	private:
		/* ポインタ型。*/
		nsActor::Boss* m_boss = nullptr;						//! ボスのポインタ。
		Camera* m_camera = nullptr;								//! カメラのポインタ。
		nsUI::CommentaryUIManager* m_commentary = nullptr;		//! コメンタリーUIマネージャーのポインタ。
		nsGame::GameTimeLimit* m_timeLimit = nullptr;			//! ゲーム制限時間のポインタ。
		PlayerControlerHub* m_playerHub = nullptr;				//! プレイヤーコントローラーハブのポインタ。

		/* コンテナ型。*/
		std::vector<nsActor::Player*> m_players;				//! 吹っ飛ばし対象のプレイヤーリスト。

		/* float 型。*/
		float m_eventTimer = 0.0f;								//! イベント開始からの経過時間（秒）。
		float m_recoveryTimer = 0.0f;							//! 咆哮終了後の復帰待ち経過時間（秒）。

		/* bool 型。*/
		bool m_phase1Notified = false;							//! 50%フェーズ処理済みフラグ。
		bool m_phase2Notified = false;							//! 25%フェーズ処理済みフラグ。
		bool m_isEventActive = false;							//! 咆哮イベント進行中フラグ。
		bool m_shakeStarted = false;							//! カメラシェイク発火済みフラグ。
		bool m_blowAwayStarted = false;							//! 吹っ飛び発火済みフラグ。
		bool m_waitingForRecovery = false;						//! 復帰待ち中フラグ。
	};
}