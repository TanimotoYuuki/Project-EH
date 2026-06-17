#include "stdafx.h"
#include "CommentaryUIManager.h"

namespace nsApp
{
	namespace nsUI
	{
		const size_t CommentaryUIManager::MAX_QUEUE_COUNT = 3;
		const float CommentaryUIManager::MESSAGE_DISPLAY_TIME = 0.95f;
		const float CommentaryUIManager::SAME_MESSAGE_COOLDOWN = 0.65f;
		const float CommentaryUIManager::LOW_PRIORITY_COOLDOWN = 1.20f;

		bool CommentaryUIManager::Start()
		{
			m_commentaryUI.Init();
			m_messages.clear();
			m_waitingMessages.clear();
			m_latestAcceptedText.clear();
			m_sameMessageCooldownTimer = SAME_MESSAGE_COOLDOWN;
			m_lowPriorityCooldownTimer = LOW_PRIORITY_COOLDOWN;
			return true;
		}

		void CommentaryUIManager::Update()
		{
			m_sameMessageCooldownTimer += g_gameTime->GetFrameDeltaTime();
			m_lowPriorityCooldownTimer += g_gameTime->GetFrameDeltaTime();

			StartNextMessageIfNeeded();

			for (auto& message : m_messages)
			{
				message.timer += g_gameTime->GetFrameDeltaTime();
			}

			RemoveExpiredMessages();
			StartNextMessageIfNeeded();
		}

		void CommentaryUIManager::Render(RenderContext& rc)
		{
			m_commentaryUI.Render(rc, m_messages);
		}

		void CommentaryUIManager::AddMessage(const std::wstring& text)
		{
			AddMessage(text, CommentaryMessagePriority::Normal);
		}

		void CommentaryUIManager::AddMessage(const std::wstring& text, CommentaryMessagePriority priority)
		{
			if (!CanAcceptMessage(text, priority))
				return;

			TrimWaitingMessages(priority);

			QueuedCommentaryMessage queuedMessage;
			queuedMessage.message.text = text;
			queuedMessage.message.displayTime = MESSAGE_DISPLAY_TIME;
			queuedMessage.message.timer = 0.0f;
			queuedMessage.priority = priority;

			m_waitingMessages.push_back(queuedMessage);

			m_latestAcceptedText = text;
			m_sameMessageCooldownTimer = 0.0f;

			if (priority == CommentaryMessagePriority::Low)
				m_lowPriorityCooldownTimer = 0.0f;

			StartNextMessageIfNeeded();
		}

		void CommentaryUIManager::AddActionMessage(WeaponType weapon, const std::wstring& actionName)
		{
			AddMessage(GetWeaponName(weapon) + L"  " + actionName + L"!", GetPriorityFromActionName(actionName));
		}

		void CommentaryUIManager::AddActionMessage(WeaponType weapon, CommentaryActionType actionType)
		{
			AddMessage(GetWeaponName(weapon) + L"  " + GetActionName(actionType) + L"!", GetPriorityFromActionType(actionType));
		}
		void CommentaryUIManager::ForceShowMessage(const std::wstring& text, float displayTime)
		{
			/*  ҋ@  ƕ\      S N   A   đ    Ɋ  荞 ށB*/
			m_waitingMessages.clear();
			m_messages.clear();

			CommentaryMessage msg;
			msg.text = text;
			msg.displayTime = displayTime;
			msg.timer = 0.0f;

			m_messages.push_back(msg);

			m_latestAcceptedText = text;
			m_sameMessageCooldownTimer = 0.0f;
		}		void CommentaryUIManager::RemoveExpiredMessages()
		{
			if (m_messages.empty())
				return;

			if (m_messages.front().timer < m_messages.front().displayTime)
				return;

			m_messages.clear();
		}

		void CommentaryUIManager::StartNextMessageIfNeeded()
		{
			if (!m_messages.empty())
				return;

			if (m_waitingMessages.empty())
				return;

			m_messages.push_back(m_waitingMessages.front().message);
			m_waitingMessages.pop_front();
		}

		bool CommentaryUIManager::CanAcceptMessage(const std::wstring& text, CommentaryMessagePriority priority) const
		{
			if (text.empty())
				return false;

			if (priority != CommentaryMessagePriority::Critical && IsSameAsLatestMessage(text))
				return false;

			if (priority != CommentaryMessagePriority::Critical &&
				m_latestAcceptedText == text &&
				m_sameMessageCooldownTimer < SAME_MESSAGE_COOLDOWN)
				return false;

			if (priority == CommentaryMessagePriority::Low)
			{
				if (m_lowPriorityCooldownTimer < LOW_PRIORITY_COOLDOWN)
					return false;

				if (!m_waitingMessages.empty())
					return false;
			}

			if (m_waitingMessages.size() >= MAX_QUEUE_COUNT)
			{
				if (priority == CommentaryMessagePriority::Low ||
					priority == CommentaryMessagePriority::Normal)
					return false;
			}

			return true;
		}

		bool CommentaryUIManager::IsSameAsLatestMessage(const std::wstring& text) const
		{
			if (!m_messages.empty() && m_messages.back().text == text)
				return true;

			for (const auto& waitingMessage : m_waitingMessages)
			{
				if (waitingMessage.message.text == text)
					return true;
			}

			return false;
		}

		void CommentaryUIManager::TrimWaitingMessages(CommentaryMessagePriority priority)
		{
			while (m_waitingMessages.size() >= MAX_QUEUE_COUNT)
			{
				if (priority == CommentaryMessagePriority::High ||
					priority == CommentaryMessagePriority::Critical)
				{
					if (RemoveOldestLowPriorityWaitingMessage())
						continue;
				}

				m_waitingMessages.pop_front();
			}
		}

		bool CommentaryUIManager::RemoveOldestLowPriorityWaitingMessage()
		{
			for (auto it = m_waitingMessages.begin(); it != m_waitingMessages.end(); ++it)
			{
				if (it->priority == CommentaryMessagePriority::Low)
				{
					m_waitingMessages.erase(it);
					return true;
				}
			}

			return false;
		}

		std::wstring CommentaryUIManager::GetWeaponName(WeaponType weapon) const
		{
			switch (weapon)
			{
			case WeaponType::GreatSword:
				return L"ソード";

			case WeaponType::Hammer:
				return L"ハンマー";

			case WeaponType::Wand:
				return L"ワンダー";

			case WeaponType::TwinGun:
				return L"ガンナー";

			default:
				return L"";
			}
		}

		std::wstring CommentaryUIManager::GetActionName(CommentaryActionType actionType) const
		{
			switch (actionType)
			{
			case CommentaryActionType::NormalAttack:
				return L"通常攻撃";

			case CommentaryActionType::HeavyAttack:
				return L"強攻撃";

			case CommentaryActionType::ChargeAttack:
				return L"チャージ攻撃";

			case CommentaryActionType::JumpAttack:
				return L"ジャンプ攻撃";

			case CommentaryActionType::Help:
				return L"助け";

			case CommentaryActionType::Revive:
				return L"復活";

			case CommentaryActionType::Down:
				return L"ダウン";

			default:
				return L"";
			}
		}


		CommentaryMessagePriority CommentaryUIManager::GetPriorityFromActionType(CommentaryActionType actionType) const
		{
			switch (actionType)
			{
			case CommentaryActionType::NormalAttack:
			case CommentaryActionType::JumpAttack:
				return CommentaryMessagePriority::Low;

			case CommentaryActionType::HeavyAttack:
				return CommentaryMessagePriority::Normal;

			case CommentaryActionType::ChargeAttack:
				return CommentaryMessagePriority::High;

			case CommentaryActionType::Help:
			case CommentaryActionType::Revive:
			case CommentaryActionType::Down:
				return CommentaryMessagePriority::Critical;

			default:
				return CommentaryMessagePriority::Normal;
			}
		}

		CommentaryMessagePriority CommentaryUIManager::GetPriorityFromActionName(const std::wstring& actionName) const
		{
			if (actionName == L"通常攻撃" || actionName == L"ジャンプ攻撃")
				return CommentaryMessagePriority::Low;
			if (actionName == L"チャージ攻撃" || actionName == L"強攻撃")
				return CommentaryMessagePriority::High;
			if (actionName == L"助け" || actionName == L"復活" || actionName == L"ダウン")
				return CommentaryMessagePriority::Critical;
			return CommentaryMessagePriority::Normal;
		}
	}
}
