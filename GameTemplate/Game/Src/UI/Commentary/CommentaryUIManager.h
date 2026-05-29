#pragma once

#include <deque>
#include <string>
#include <vector>
#include "Src/Actor/Character/Common/CharacterAnimation.h"
#include "Src/UI/Commentary/CommentaryMessage.h"
#include "Src/UI/Commentary/CommentaryUI.h"

namespace nsApp
{
	namespace nsUI
	{
		enum class CommentaryActionType
		{
			NormalAttack,
			HeavyAttack,
			ChargeAttack,
			JumpAttack,
			Help,
			Revive,
			Down,
		};

		enum class CommentaryMessagePriority
		{
			Low,
			Normal,
			High,
			Critical,
		};


		/**
		 * @brief 実況UIを管理するクラス。
		 */
		class CommentaryUIManager : public IGameObject
		{
		private:
			/**
			 * @brief 表示待ち実況メッセージ。
			 */
			struct QueuedCommentaryMessage
			{
				CommentaryMessage message;                 //! 実況メッセージ本体。
				CommentaryMessagePriority priority;        //! メッセージ優先度。
			};

		public:
			/**
			 * @brief 開始処理。
			 * @return 初期化に成功した場合true。
			 */
			bool Start() override;

			/**
			 * @brief 更新処理。
			 */
			void Update() override;

			/**
			 * @brief 描画処理。
			 * @param rc 描画コンテキスト。
			 */
			void Render(RenderContext& rc) override;


		public:
			/**
			 * @brief コメントを追加する。
			 * @param text 追加するコメント。
			 */
			void AddMessage(const std::wstring& text);

			/**
			 * @brief 優先度付きでコメントを追加する。
			 * @param text 追加するコメント。
			 * @param priority メッセージ優先度。
			 */
			void AddMessage(const std::wstring& text, CommentaryMessagePriority priority);

			/**
			 * @brief 武器と行動名からコメントを追加する。
			 * @param weapon 武器の種類。
			 * @param actionName 行動名。
			 */
			void AddActionMessage(WeaponType weapon, const std::wstring& actionName);

			/**
			 * @brief 武器と行動タイプからコメントを追加する。
			 * @param weapon 武器の種類。
			 * @param actionType 行動の種類。
			 */
			void AddActionMessage(WeaponType weapon, CommentaryActionType actionType);


		private:
			/**
			 * @brief 表示時間を過ぎたメッセージを削除する。
			 */
			void RemoveExpiredMessages();

			/**
			 * @brief 表示中のメッセージがなければ、待機列から次のメッセージを表示する。
			 */
			void StartNextMessageIfNeeded();

			/**
			 * @brief メッセージを追加してよいか判定する。
			 * @param text 調べるコメント。
			 * @param priority メッセージ優先度。
			 * @retval true 追加してよい。
			 * @retval false 追加しない。
			 */
			bool CanAcceptMessage(const std::wstring& text, CommentaryMessagePriority priority) const;

			/**
			 * @brief 直近のメッセージと同じ内容か調べる。
			 * @param text 調べるコメント。
			 * @retval true 直近と同じ内容。
			 * @retval false 直近と異なる内容。
			 */
			bool IsSameAsLatestMessage(const std::wstring& text) const;

			/**
			 * @brief 待機列が詰まりすぎないように整理する。
			 * @param priority 追加予定メッセージの優先度。
			 */
			void TrimWaitingMessages(CommentaryMessagePriority priority);

			/**
			 * @brief 待機列から古い低優先度メッセージを1件削除する。
			 * @retval true 削除した。
			 * @retval false 削除対象がない。
			 */
			bool RemoveOldestLowPriorityWaitingMessage();

			/**
			 * @brief 武器名を取得する。
			 * @param weapon 武器の種類。
			 * @return 表示用の武器名。
			 */
			std::wstring GetWeaponName(WeaponType weapon) const;

			/**
			 * @brief 行動タイプから表示用の行動名を取得する。
			 * @param actionType 行動の種類。
			 * @return 表示用の行動名。
			 */
			std::wstring GetActionName(CommentaryActionType actionType) const;

			/**
			 * @brief 行動タイプから優先度を取得する。
			 * @param actionType 行動の種類。
			 * @return メッセージ優先度。
			 */
			CommentaryMessagePriority GetPriorityFromActionType(CommentaryActionType actionType) const;

			/**
			 * @brief 行動名から優先度を取得する。
			 * @param actionName 行動名。
			 * @return メッセージ優先度。
			 */
			CommentaryMessagePriority GetPriorityFromActionName(const std::wstring& actionName) const;


		private:
			static const size_t MAX_QUEUE_COUNT;                 //! 最大待機メッセージ数。
			static const float MESSAGE_DISPLAY_TIME;             //! 1メッセージの表示時間。
			static const float SAME_MESSAGE_COOLDOWN;            //! 同一メッセージ抑制時間。
			static const float LOW_PRIORITY_COOLDOWN;            //! 低優先度メッセージ抑制時間。

			std::vector<CommentaryMessage> m_messages;           //! 表示中の実況メッセージ。常に0件または1件にする。
			std::deque<QueuedCommentaryMessage> m_waitingMessages; //! 表示待ちの実況メッセージ。
			CommentaryUI m_commentaryUI;                         //! 実況UI描画クラス。

			std::wstring m_latestAcceptedText;                   //! 最後に受理した実況テキスト。
			float m_sameMessageCooldownTimer;                    //! 同一メッセージ抑制用タイマー。
			float m_lowPriorityCooldownTimer;                    //! 低優先度メッセージ抑制用タイマー。
		};
	}
}
