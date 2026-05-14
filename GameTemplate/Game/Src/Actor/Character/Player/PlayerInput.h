#pragma once
/**
 * @file PlayerInput.h
 * @brief プレイヤーの入力を管理するクラス。
 * @author Yamaguchi Hayato
 * @date 2026/05/12
 */

 /** @def
  *  RTボタン判定。
  *  エンジン内でのボタン判定はRT2だが、XboxコントローラーだとRTに該当し、視認性が悪いため定義する。
  */
#define BUTTON_RT enButtonRB2

#include "Src/Actor/Character/Player/InputSystem/IInputDevice.h"

namespace nsApp
{
	class PlayerInput
	{
	public:
		/* 入力判定の更新。*/
		void Update();

		/**
		 * @brief Press入力判定があるかチェックする。
		 * @param コントローラーの列挙型をセットする。
		 */
		bool CheckButtonPress(nsK2EngineLow::EnButton inputButtonType);

		/**
		 * @brief Trigger入力判定があるかチェックする。
		 * @param コントローラーの列挙型をセットする。
		 */
		bool CheckButtonTrigger(nsK2EngineLow::EnButton inputButtonType);

		/**
         * @brief ジャンプ/斬り上げ判定をセットする。
         */
		void EvaluateJumpAndSlashUp();


	/* セッター。*/
	public:
		/** 
		 * @brief 入力のオンオフを切り替える。
		 * @param isEnable 入力を有効にするかどうか。
		 */
		inline void SetInputEnable(bool isEnable)
		{
			m_isInputEnable = isEnable;
		}

		/* 
		 * @brief Jumpフラグをセット。
		 * @param flag ジャンプフラグ。
		 */
		inline void SetJumpFlag(bool flag)
		{
			m_isJump = flag;
		}

		/* 
		 * @brief SlashUpフラグをセット。
		 * @param flag 斬り上げフラグ。
		 */
		inline void SetSlashUpFlag(bool flag)
		{
			m_isSlashUp = flag;
		}

		/**
		 * @brief 入力デバイスをセットする。
		 * @param inputDevice 入力デバイスのインスタンスをセットする。
		 */
		inline void SetInputDevice(IInputDevice* inputDevice)
		{
			m_inputDevice = inputDevice;
		}


	private:
		/**
		 *  @brief Bボタンの入力判定をまとめる。
		 */
		void SummarizeButtonB()
		{
			m_isPressButton = CheckButtonPress(enButtonB);
			m_isAttack = CheckButtonTrigger(enButtonB);
			m_isAirAttack = CheckButtonTrigger(enButtonB);
		}

		/**
		 * @brief その他のボタンの入浴判定をまとめる。
		 */
		void SummarizeOtherButtons()
		{
			/* ガード判定。*/
			m_isGuard = CheckButtonPress(enButtonLB2);

			/* 救出判定。*/
			m_isHelp = CheckButtonTrigger(enButtonY);

			/* ダメージ判定。*/
			m_isDamage = CheckButtonTrigger(enButtonLeft);

			/* 志望判定。*/
			m_isDeath = CheckButtonTrigger(enButtonDown);

			/* Xボタン判定。*/
			m_isPressX = CheckButtonTrigger(enButtonX); 

			/* RBボタン判定。*/
			m_isPressRB = CheckButtonPress(enButtonRB1);

			/* RTボタン判定。*/
			m_isPressRT = CheckButtonPress(BUTTON_RT);
		}


		/**
		 * @brief 仮チャージ判定を更新する。。
		 */
		void UpdateChargeTranslation()
		{
			m_isNormalAttack = (!m_isPressButton && m_chargeButtonTimer > 0.0f && m_chargeButtonTimer < 30.0f);
			m_isChargeStart = (m_isPressButton && m_chargeButtonTimer >= 12.0f);
			m_isChargeAttack = (!m_isPressButton && m_chargeButtonTimer >= 30.0f);
			m_chargeButtonTimer = m_isPressButton ? m_chargeButtonTimer + 1.0f : 0.0f;
		}

		/**
		 * @brief 入力判定を再初期化する。
		 */
		void InitInputJudgment();


	/* ゲッター。*/
	public:
		/* アクション判定。*/
		inline bool IsAttack() const
		{
			return m_isAttack;
		}

		/* 通常攻撃判定。*/
		inline bool IsNormalAttack() const
		{
			return m_isNormalAttack;
		}

		/* チャージ中判定。*/
		inline bool IsCharging() const
		{
			return m_isChargeStart;
		}

		/* チャージ攻撃判定。*/
		inline bool IsChargeAttack() const
		{
			return m_isChargeAttack;
		}

		/* 空中攻撃判定。*/
		inline bool IsAirAttack() const
		{
			return m_isAirAttack;
		}

		/* 連続攻撃開始判定。*/
		inline bool IsRushStart() const
		{
			return m_isRushStart;
		}

		/* 連続攻撃終了判定。*/
		inline bool IsRushEnd() const
		{
			return m_isRushEnd;
		}

		/* 斬り上げ判定。*/
		inline bool IsSlashUp() const
		{
			return m_isSlashUp;
		}

		/* コンボ攻撃判定。*/
		inline bool IsComboAttack() const
		{
			return m_isComboAttack;
		}

		/* ジャンプ判定。*/ 
		inline bool IsJump() const
		{
			return m_isJump;
		}

		/* 移動判定。*/
		inline bool IsMove() const
		{
			return m_isMove;
		}

		/* 走り判定。*/
		inline bool IsRun() const
		{
			return m_isRun;
		}

		/* ダメージ判定。*/
		inline bool IsDamage() const
		{
			return m_isDamage;	
		}

		/* 死亡判定。*/
		inline bool IsDeath() const
		{
			return m_isDeath;
		}

		/* 移動ベクトルを取得。*/
		inline Vector3 GetMoveVector() const
		{
			return m_moveVec;
		}

		/* チャージ時間を取得する。*/
		inline float GetChargeTimer() const
		{
			return m_isChargeStart;
		}

		/* ガード判定。*/
		inline bool IsGuard() const
		{
			return m_isGuard;
		}

		/* 助ける判定。*/
		inline bool IsHelp() const
		{
			return m_isHelp;
		}

		/* Xボタン判定。*/
		inline bool IsPressX() const
		{
			return m_isPressX;
		}

		/* RBボタン判定。*/
		inline bool IsPressRB() const
		{
			return m_isPressRB;
		}

		/* RTボタン判定。*/
		inline bool IsPressRT() const 
		{
			return m_isPressRT;
		}


	private:
		IInputDevice* m_inputDevice = nullptr;  //! 入力デバイス。

		bool m_isAttack = false;				//! 攻撃したかどうかを判定。
		bool m_isJump = false;                  //! ジャンプしたかどうかを判定。
		bool m_isMove = false;					//! 移動したかどうかを判定。
		bool m_isRun = false;					//! 走ったかどうかを判定。
		bool m_isDamage = false;				//! ダメージを受けたかどうかを判定。
		bool m_isDeath = false;					//! 死亡したかどうかを判定。
		bool m_isNormalAttack = false;			//! 攻撃入力を検知。
		bool m_isAirAttack = false;				//! 空中攻撃入力を検知。
		bool m_isComboAttack = false;			//! コンボ攻撃入力を検知。
		bool m_isInputEnable = true;			//! 入力を検知。
		bool m_isPressButton = false;			//! Bボタンが押されているかどうかを判定。
		bool m_isRushStart = false;				//! 連続攻撃が始まっているかを検知。
		bool m_isRushEnd = false;				//! 連続攻撃が終わっているかを検知。
		bool m_isJumpRequested = false;			//! ジャンプの入力があったかどうかを判定。
		bool m_isSlashUp = false;				//! 斬り上げの入力を検知。
		bool m_isChargeStart = false;			//! チャージ攻撃の入力を検知。
		bool m_isChargeAttack = false;			//! チャージ攻撃の入力を検知。
		bool m_isGuard = false;					//! ガードの入力を検知。
		bool m_isHelp = false;					//! 助ける入力を検知。
		bool m_isPressX = false;				//! Xボタンが押されているかどうかを判定。
		bool m_isPressRB = false;				//! RBボタンが押されているかどうかを判定。
		bool m_isPressRT = false;				//! RTボタンが押されているかどうかを判定。

		float m_stickX = 0.0f;					//! スティックのX軸の値。
		float m_stickY = 0.0f;					//! スティックのY軸の値。
		float m_chargeButtonTimer = 0.0f;		//! チャージ攻撃と判定するために必要なBボタンを長押ししなければならない時間。

		Vector3 m_moveVec = Vector3::Right;		//! 移動ベクトル。
	};
}
