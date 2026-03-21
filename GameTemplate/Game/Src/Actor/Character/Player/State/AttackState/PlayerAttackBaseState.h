#pragma once
/**
* @file   PlayerAttackBaseState.h
* @brief  プレイヤーの攻撃状態の基底クラス。
* @author Yamaguchi Hayato
* @date   2026/03/11
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Character/Player/Component/ComboRouteTable.h"

/** @def
 * プレイヤーの遷移状態を PLAYER_STATE_IDという名前で定義するマクロ。
 */
#define PLAYER_STATE_ID nsActor::PlayerStateID


namespace nsApp
{
	class PresentDamageIndicator;

	namespace nsState
	{
		class PlayerAttackBaseState : public IState<nsActor::Actor>
		{
		public:
			/* コンストラクタとデストラクタ。*/
			PlayerAttackBaseState() = default;
			virtual ~PlayerAttackBaseState() = default;


		public:
			/* ライフサイクル。*/
			/* 初期化処理。*/
			void Enter() override;
			/* 毎フレーム更新する処理。*/
			void Update() override;
			/* 描画処理。*/
			void Exit() override;


		/* セッター。*/
		public:
			/* 攻撃の時間をセット。*/
			inline void SetAttackTimer(int timer)
			{
				m_attackTimer = timer;
			}


		protected:
			/**
			 *  コンボが有効か確認する処理。
			 * @param currentStateID 現在の状態ID。
			 */
			bool CheckCombo(PLAYER_STATE_ID currentStateID, uint8_t& id);


			/*
             * ダメージテキストを表示する処理。
             * @param hitPosition ダメージテキストを表示する位置。
             * @param currentAttackType 現在の攻撃タイプ。
             */
			void OnHitDamageText();


		protected:
			nsActor::Player* m_player = nullptr;                      //! プレイヤーのポインタ。
			PresentDamageIndicator* m_damageIndicator = nullptr;      //! ダメージテキスト表示用のインスタンスへのポインタ。


		protected:
			int m_attackTimer = 0;                                    //! 攻撃の時間管理用タイマー。
			int m_rushCount = 0;                                      //! 連続攻撃の回数管理用カウンター。
			int m_finalDamage = 0;                                    //! 最終的なダメージ量を管理する変数。

			bool m_isInputMatch;									  //! 入力がコンボルートの条件に合致しているか。
			bool m_isGrounded;										  //! 地上にいるかどうか。

			float m_criticalRate = 0.0f;							  //! クリティカル補正の確率。

			std::unordered_map<ComboInputType, bool> m_inputRequests; //! 入力タイプとフラグを紐づけ。
			AttackType m_currentAttackType = AttackType::None;        //!< 現在の攻撃タイプ。

			Vector3 m_screenPosition = Vector3::Zero;				  //! ダメージテキストの描画位置。
			Vector3 m_forwardDirection = Vector3::Zero;				  //! プレイヤーの向いている方向を取得するための変数。
			Vector3 m_getPlayerPosition = Vector3::Zero;			  //! プレイヤーの座標を取得するための変数。
		};
	}
}
