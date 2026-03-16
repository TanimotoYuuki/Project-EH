#pragma once
/**
 * @file PlayerInput.h
 * @brief プレイヤーの入力を管理するクラス。
 * @author Yamaguchi Hayato
 * @date 2026/03/15
 */

namespace nsApp
{
	class PlayerInput
	{
	public:
		/* 入力判定の更新。*/
		void Update();


	public:
		/* 入力のオンオフを切り替える。*/
		inline void SetInputEnable(bool isEnable)
		{
			m_isInputEnable = isEnable;
		}


	/* ゲッター。*/
	public:
		/* アクション判定。*/
		inline bool IsAttack() const
		{
			return m_isAttack;
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


	private:
		bool m_isAttack = false;     /* 攻撃したかどうかを判定。*/
		bool m_isJump = false;       /* ジャンプしたかどうかを判定。*/
		bool m_isMove = false;       /* 移動したかどうかを判定。*/
		bool m_isRun = false;        /* 走ったかどうかを判定。*/
		bool m_isDamage = false;     /* ダメージを受けたかどうかを判定。*/
		bool m_isDeath = false;      /* 死亡したかどうかを判定。*/
		bool m_isInputEnable = true; /* 入力を検知。*/

		float m_stickX = 0.0f;        /* スティックのX軸の値。*/
		float m_stickY = 0.0f;        /* スティックのY軸の値。*/

		Vector3 m_moveVec;           /* 移動ベクトル。*/
	};
}
