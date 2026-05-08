#pragma once
/**
 * @file PlayerInput.h
 * @brief プレイヤーの入力を管理するクラス。
 * @author Yamaguchi Hayato
 * @date 2026/03/15
 */

 /** @def
  *  RTボタン判定。
  *  エンジン内でのボタン判定はRT2だが、XboxコントローラーだとRTに該当し、視認性が悪いため定義する。
  */
#define BUTTON_RT enButtonRB2


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

		/** 
		 * @brief gamepadの要素数を受け取る。
		 * @param index 受け取るgamepadの要素数。
		 */
		inline void SetPadIndex(int index)
		{
			m_padInddex = index;
		}

		/** 
		 * @brief AI用の仮想コントローラー。
		 * @param stickX スティックのX軸の値。
		 * @param stickY スティックのY軸の値。
		 */
		inline void SetVirtualController(float stickX, float stickY)
		{
			m_virtualStickX = stickX;
			m_virtualStickY = stickY;
		}

		/* 押す/離す用。*/
		/* Aボタン。*/
		inline void SetVirtualButtonA(bool isPress)
		{
			m_isVirtualPressA = isPress; 
		}

		/* Bボタン。*/
		inline void SetVirtualButtonB(bool isPress) 
		{
			m_isVirtualPressB = isPress; 
		}

		/* Xボタン。*/
		inline void SetVirtualButtonX(bool isPress) 
		{
			m_isVirtualPressX = isPress; 
		}

		/* Yボタン。*/
		inline void SetVirtualButtonY(bool isPress) 
		{ 
			m_isVirtualPressY = isPress; 
		}

		/* LB1ボタン。*/
		inline void SetVirtualButtonLB1(bool isPress) 
		{ 
			m_isVirtualPressLB1 = isPress; 
		}

		/* LB2ボタン。*/
		inline void SetVirtualButtonLB2(bool isPress) 
		{
			m_isVirtualPressLB2 = isPress; 
		}

		/* RB1ボタン。*/
		inline void SetVirtualButtonRB1(bool isPress) 
		{
			m_isVirtualPressRB1 = isPress; 
		}

		/* RTボタン。*/
		inline void SetVirtualButtonRT(bool isPress) 
		{
			m_isVirtualPressRT = isPress; 
		}


		/* AI用仮想コントローラーの前フレーム保存用のセッター群。*/
		/* Aボタン。*/
		inline void SavePrevVirtualPressA() 
		{
			m_prevVirtualPressA = m_isVirtualPressA; 
		}

		/* Bボタン。*/
		inline void SavePrevVirtualPressB() 
		{
			m_prevVirtualPressB = m_isVirtualPressB; 
		}

		/* Xボタン。*/ 
		inline void SavePrevVirtualPressX() 
		{
			m_prevVirtualPressX = m_isVirtualPressX; 
		}

		/* Yボタン。*/
		inline void SavePrevVirtualPressY() 
		{
			m_prevVirtualPressY = m_isVirtualPressY; 
		}

		/* LB1ボタン。*/ 
		inline void SavePrevVirtualPressLB1() 
		{ 
			m_prevVirtualPressLB1 = m_isVirtualPressLB1; 
		}

		/* LB2ボタン。*/ 
		inline void SavePrevVirtualPressLB2()
		{
			m_prevVirtualPressLB2 = m_isVirtualPressLB2; 
		}

		/* RB1ボタン。*/
		inline void SavePrevVirtualPressRB1() 
		{
			m_prevVirtualPressRB1 = m_isVirtualPressRB1; 
		}

		/* RTボタン。*/
		inline void SavePrevVirtualPressRT() 
		{
			m_prevVirtualPressRT = m_isVirtualPressRT; 
		}

		/**
		 * @brief 各ボタン判定をまとめる処理。
		 */
		void SetVirtualAttackButtons()
		{
			SavePrevVirtualPressA();
			SavePrevVirtualPressB();
			SavePrevVirtualPressX();
			SavePrevVirtualPressY();
			SavePrevVirtualPressLB1();
			SavePrevVirtualPressLB2();
			SavePrevVirtualPressRB1();
			SavePrevVirtualPressRT();
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

		/* gamepad の要素数を受け取る。*/
		inline int GetPadIndex() const
		{
			return m_padInddex;
		}


	private:
		bool m_isAttack = false;             //! 攻撃したかどうかを判定。
		bool m_isJump = false;               //! ジャンプしたかどうかを判定。
		bool m_isMove = false;               //! 移動したかどうかを判定。
		bool m_isRun = false;                //! 走ったかどうかを判定。
		bool m_isDamage = false;             //! ダメージを受けたかどうかを判定。
		bool m_isDeath = false;              //! 死亡したかどうかを判定。
		bool m_isNormalAttack = false;       //! 攻撃入力を検知。
		bool m_isAirAttack = false;          //! 空中攻撃入力を検知。
		bool m_isComboAttack = false;        //! コンボ攻撃入力を検知。
		bool m_isInputEnable = true;         //! 入力を検知。
		bool m_isPressButton = false;        //! Bボタンが押されているかどうかを判定。
		bool m_isRushStart = false;          //! 連続攻撃が始まっているかを検知。
		bool m_isRushEnd = false;            //! 連続攻撃が終わっているかを検知。
		bool m_isJumpRequested = false;      //! ジャンプの入力があったかどうかを判定。
		bool m_isSlashUp = false;            //! 斬り上げの入力を検知。
		bool m_isChargeStart = false;        //! チャージ攻撃の入力を検知。
		bool m_isChargeAttack = false;       //! チャージ攻撃の入力を検知。
		bool m_isGuard = false;              //! ガードの入力を検知。
		bool m_isHelp = false;               //! 助ける入力を検知。
		bool m_isPressX = false;             //! Xボタンが押されているかどうかを判定。
		bool m_isPressRB = false;            //! RBボタンが押されているかどうかを判定。
		bool m_isPressRT = false;			 //! RTボタンが押されているかどうかを判定。
		bool m_isVirtualAttackPress = false; //! 

		float m_stickX = 0.0f;               //! スティックのX軸の値。
		float m_stickY = 0.0f;               //! スティックのY軸の値。
		float m_chargeButtonTimer = 0.0f;    //! チャージ攻撃と判定するために必要なBボタンを長押ししなければならない時間。

		int m_padInddex = 0;                 //! 入力を検知するパッドのインデックス。

		Vector3 m_moveVec = Vector3::Right;  //! 移動ベクトル。


	private:
		/* AI用の仮想コントローラー変数群。*/
		float m_virtualStickX = 0.0f;        //! 仮想スティックのX軸の値。
		float m_virtualStickY = 0.0f;        //! 仮想スティックのY軸の値。
		bool m_isVirtualPressA = false;      //! 仮想Aボタンが押されているかどうかを判定。
		bool m_isVirtualPressB = false;      //! 仮想Bボタンが押されているかどうかを判定。
		bool m_isVirtualPressX = false;      //! 仮想Xボタンが押されているかどうかを判定。
		bool m_isVirtualPressY = false;      //! 仮想Yボタンが押されているかどうかを判定。
		bool m_isVirtualPressLB1 = false;    //! 仮想LB1ボタンが押されているかどうかを判定。	
		bool m_isVirtualPressLB2 = false;    //! 仮想LB2ボタンが押されているかどうかを判定。
		bool m_isVirtualPressRB1 = false;    //! 仮想RB1ボタンが押されているかどうかを判定。
		bool m_isVirtualPressRT = false;     //! 仮想RTボタンが押されているかどうかを判定。

		/* トリガー判定用（前フレームの記憶） */
		bool m_prevVirtualPressA = false;    // 仮想Aボタンの前フレームの状態を記憶。
		bool m_prevVirtualPressB = false;    // 仮想Bボタンの前フレームの状態を記憶。
		bool m_prevVirtualPressX = false;    // 仮想Xボタンの前フレームの状態を記憶。
		bool m_prevVirtualPressY = false;    // 仮想Yボタンの前フレームの状態を記憶。
		bool m_prevVirtualPressLB1 = false;  // 仮想LB1ボタンの前フレームの状態を記憶。
		bool m_prevVirtualPressLB2 = false;  // 仮想LB2ボタンの前フレームの状態を記憶。
		bool m_prevVirtualPressRB1 = false;  // 仮想RB1ボタンの前フレームの状態を記憶。
		bool m_prevVirtualPressRT = false;   // 仮想RTボタンの前フレームの状態を記憶。
	};
}
