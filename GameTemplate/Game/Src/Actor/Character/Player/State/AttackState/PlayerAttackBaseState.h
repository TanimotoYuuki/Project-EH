#pragma once
/**
* @file   PlayerAttackBaseState.h
* @brief  プレイヤーの攻撃状態の基底クラス。
* @author Yamaguchi Hayato
* @date   2026/03/11
*/

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/Player/Player.h"
#include "Src/Actor/Magic/MagicProjectotile.h"
#include "Src/Actor/Magic/Factory/MagicFactory.h"
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
			/** 
			 * @brief 攻撃の時間をセット。
			 * @param timer 攻撃の時間を管理するタイマーの値。
			 */
			inline void SetAttackTimer(int timer)
			{
				m_attackTimer = timer;
			}


			/**
			 * @brief 生成する弾丸を通知する。
			 * @param request 弾丸の生成に必要な情報を格納する構造体。
			 * @param type 通知する弾丸の種類。
			 * @detail それぞれの攻撃関数での共通項の処理を関数化。
			 */
			void ConstructAndTransmitBulletRequest(BulletType type)
			{
				/* 弾丸の種類を格納する。*/
				BulletFireRequest request;

				/* 通知する弾丸の種類を設定。*/
				request.kind = type;

				/* 弾丸を生成する座標。*/
				request.basePosition = m_spawnPosition;

				/* 弾丸の発射方向。*/ 
				request.direction = m_forwardDirection;

				/* GunShooterクラスに発射処理を依頼。*/
				m_player->GetGunShooter().Fire(request);
			}


			/**
			 * @brief 魔法の生成
			 * @param type 生成する魔法の種類。
			 * @param target 魔法のターゲット(デフォルト値で初期化済み)。
			 * @detail プレイヤーの座標と向きを自動取得してセットするバージョン。
			 */
			inline void ConstructAndTransmitMagicRequest(nsActor::MagicType type, nsActor::ICharacter* target = nullptr)
			{
				m_spawnPosition = m_player->GetWeaponHitDetection().GetPosition();
				m_forwardDirection = m_player->GetForwardVector();
				MagicFactory::CreateMagicObject(type, m_spawnPosition, m_forwardDirection, target);
			}


			/**
			 * @brief 魔法の生成
			 * @param type 生成する魔法の種類。
			 * @param customPos 魔法の生成位置を指定するための引数。
			 * @param customDir 魔法の発射方向を指定するための引数。
			 * @param target 魔法のターゲット(デフォルト値で初期化済み)。
			 * @detail プレイヤーの座標と向きを使用せず、引数で指定した座標と向きで魔法を生成するバージョン。
			 */
			inline void ConstructAndTransmitMagicRequest(nsActor::MagicType type, const Vector3& customPos, const Vector3& customDir, nsActor::ICharacter* target = nullptr)
			{
				MagicFactory::CreateMagicObject(type, customPos, customDir, target);
			}


		protected:
			/**
			 *  コンボが有効か確認する処理。
			 * @param currentStateID 現在の状態ID。
			 */
			bool CheckCombo(PLAYER_STATE_ID currentStateID, uint8_t& id);


			/**
             * ダメージテキストを表示する処理。
             * @param hitPosition ダメージテキストを表示する位置。
             * @param currentAttackType 現在の攻撃タイプ。
             */
			void OnHitDamageText(nsActor::ICharacter* target);


		protected:
			nsActor::Player* m_player = nullptr;                      //! プレイヤーのポインタ。
			PresentDamageIndicator* m_damageIndicator = nullptr;      //! ダメージテキスト表示用のインスタンスへのポインタ。


		protected:
			int m_attackTimer = 0;                                    //! 攻撃の時間管理用タイマー。
			int m_rushCount = 0;                                      //! 連続攻撃の回数管理用カウンター。
			int m_finalDamage = 0;                                    //! 最終的なダメージ量を管理する変数。

			bool m_isInputMatch;									  //! 入力がコンボルートの条件に合致しているか。
			bool m_isGrounded;										  //! 地上にいるかどうか。
			bool m_isHit;											  //! 攻撃がヒットしたかどうか。

			float m_criticalRate = 0.0f;							  //! クリティカル補正の確率。

			std::unordered_map<ComboInputType, bool> m_inputRequests; //! 入力タイプとフラグを紐づけ。

			AttackType m_currentAttackType = AttackType::None;        //! 現在の攻撃タイプ。

			Vector3 m_screenPosition = Vector3::Zero;				  //! ダメージテキストの描画位置。
			Vector3 m_forwardDirection = Vector3::Zero;				  //! プレイヤーの向いている方向を取得するための変数。
			Vector3 m_getPlayerPosition = Vector3::Zero;			  //! プレイヤーの座標を取得するための変数。
			Vector3 m_spawnPosition = Vector3::Zero;				  //! 弾丸の発射位置を管理する変数。

		};
	}
}
