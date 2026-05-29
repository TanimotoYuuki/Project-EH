#pragma once

/**
 * @file   NPCHelpState.h
 * @brief  NPCの救助状態を管理するクラス。
 * @author Yamaguchi Hayato
 * @date   2026/05/25
 */

#include "Src/Actor/Character/Common/IState.h"
#include "Src/Actor/Character/NPC/NPCBrain.h"
#include "Src/Actor/Character/Player/Player.h"

namespace nsApp
{
	class VirtualInputAdapter;

	namespace nsState
	{
		class NPCHelpState : public IState<NPCBrain>
		{
		public:
			/**
			 * @brief コンストラクタ。
			 * @param helpTarget 救助対象。
			 */
			explicit NPCHelpState(nsActor::Player* helpTarget);

			/**
			 * @brief 開始処理。
			 */
			void Enter() override;

			/**
			 * @brief 更新処理。
			 */
			void Update() override;

			/**
			 * @brief 終了処理。
			 */
			void Exit() override;

		private:
			/**
			 * @brief 救助対象へ移動する。
			 */
			void MoveToHelpTarget();

			/**
			 * @brief 救助を実行する。
			 */
			void ExecuteHelp();

			/**
			 * @brief 救助対象が有効か判定する。
			 * @return 有効ならtrue。
			 */
			bool IsValidHelpTarget() const;

			/**
			 * @brief SEを再生する。。
			 */
			void StartHelpSE();

			/**
			 * @brief SEを停止する。。
			 */
			void StopHelpSE();


		private:
			NPCBrain* m_brain = nullptr;					//! NPCの思考クラス。
			nsActor::Player* m_body = nullptr;				//! NPC本体。
			nsActor::Player* m_helpTarget = nullptr;		//! 救助対象。
			VirtualInputAdapter* m_vInput = nullptr;		//! NPCの仮想入力。
			const char* m_helpTargetTag = nullptr;
			nsK2EngineLow::SoundSource* m_helpSE = nullptr; //! 救助中SE。

		private:
			Vector3 m_difference = Vector3::Zero;			//! NPCと救助対象の差分。
			float m_distance = 0.0f;						//! NPCと救助対象の距離。
			float m_helpRange = 40.0f;						//! 救助を実行する距離。

			bool m_hasRequestedHelp = false;
		};
	}
}