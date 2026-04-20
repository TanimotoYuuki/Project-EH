#pragma once
/*
* @file   ComboRouteTable.h
* @brief  プレイヤーの攻撃コンボ(攻撃の組み合わせによって発生する特殊攻撃)の樹圭樹を管理。
* @author Yamaguchi Hayato
* @date   2026/03/18
*/

#include "Src/Actor/Character/Player/Player.h"

/** @def
 * プレイヤーの遷移状態を PLAYER_STATE_IDという名前で定義するマクロ。
 */
#define PLAYER_STATE_ID nsActor::PlayerStateID

/** @def
 * コンボルールリストを COMBO_RULEという名前で定義するマクロ。
 */
#define COMBO_RULE std::vector<ComboRule>


namespace nsApp
{
	/**
	 * @enum ComboInputType。
	 *コンボを発生させるボタン入力の条件を管理する列挙型。
	 */
	enum class ComboInputType : uint8_t
	{
		PressA,        //! Aボタンを1回押す。
		PressB,        //! Bボタンを1回押す。
		RushB,         //! Bボタンを連続で押す。
		PressLB2,      //! LB2(背面)ボタンを押す。
		PressX,        //! Xボタンを1回押す。
		PressUpAndJump //! ジャンプしながらスティック上入力をする。
		
	};

	/** 
	 * @struct ComboRule。
	 * コンボのルートを管理する構造体。
	 */
	struct ComboRule
	{
		ComboInputType inputType;           //! コンボを発生させるボタン入力の条件。
		int cancelTime;                     //! コンボを発生させるための入力の猶予時間。
		PLAYER_STATE_ID nextStateID;        //! コンボ攻撃の次の状態ID。
	};


	class ComboRouteTable
	{
	public:

		/**
		 * 今の技IDを譲渡すると、次のルートリストを返してくれる処理。
		 * @param stateID 現在のステート状態。
		 * @param isGrounded 地上にいるかどうか。
		 */
		static const COMBO_RULE& GetRoutes(PLAYER_STATE_ID currentStateID, bool isGrounded);


	public:
		static const std::unordered_map<PLAYER_STATE_ID, COMBO_RULE> m_groundTable; //! 地上攻撃用のコンボテーブル。
		static const std::unordered_map<PLAYER_STATE_ID, COMBO_RULE> m_airTable;    //! 空中コンボ用


	private:
		static const COMBO_RULE m_empty; //! ルートがない場合のnullルート。
	};
}

