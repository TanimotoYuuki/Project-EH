#include "stdafx.h"
#include "BossAnimation.h"

namespace nsApp
{
	namespace nsActor
	{
		AnimationClip BossAnimation::m_clips[(int)BossAnimationID::Max];

		/* A j   [ V     ̃p X   擾 B*/
		std::string BossAnimation::GetAnimName(BossAnimationID id)
		{
			std::string name;

			switch (id)
			{
			case BossAnimationID::Idle:
				name = "Idle";
				break;
			case BossAnimationID::Walk:
				name = "Walk";
				break;
			case BossAnimationID::TakeOff:
				name = "TakeOff";
				break;
			case BossAnimationID::Land:
				name = "Land";
				break;

			case BossAnimationID::BiteAttack:
				name = "BiteAttack";
				break;
			case BossAnimationID::TailAttack:
				name = "TailAttack";
				break;
			case BossAnimationID::FireAttack:
				name = "FireAttack";
				break;

			case BossAnimationID::GetHit:
				name = "GetHit";
				break;
			case BossAnimationID::Death:
				name = "Die";
				break;

			case BossAnimationID::Scream:
				name = "Scream";
				break;
			case BossAnimationID::Sleep:
				name = "Sleep";
				break;

			default:
				name = "Idle";
				break;
			}

			/* { X   Ƃ̍    B*/
			if (m_type == "GrayDragon")
			{
				if (id == BossAnimationID::FireAttack)
					return "Scream";
			}

			if (m_type == "GreenDragon")
			{
				if (id == BossAnimationID::FireAttack)
					return "Flame";
				if (id == BossAnimationID::Land)
					return "Landing";
			}

			if (m_type == "TutorialBoss")
			{
				if (id == BossAnimationID::TakeOff)
					return "Jump";
			}
			return name;
		}

		/* p X     B*/
		std::string BossAnimation::GetPath(BossAnimationID id)
		{
			return "Assets/animData/" + m_type + "/" + m_type + "_" + GetAnimName(id) + ".tka";
		}

		/*   [ v    B*/
		bool BossAnimation::IsLoop(BossAnimationID id)
		{
			switch (id)
			{
			case BossAnimationID::Idle:
			case BossAnimationID::Walk:
			case BossAnimationID::Sleep:
				return true;

			default:
				return false;
			}
		}

		/*       B*/
		/*void BossAnimation::Init(const char* type)
		{
			/*std::string ւ̑
			m_type = type;

			for (int i = 0; i < (int)BossAnimationID::Max; i++)
			{
				BossAnimationID id=(BossAnimationID) i;

				std::string path = GetPath(id);

				/* t @ C       ݂  Ă  邩 ` F b N B
				if (PathFileExistsA(path.c_str()))
				{
					m_clips[i].Load(path.c_str());
					m_clips[i].SetLoopFlag(IsLoop(id));
				}
				else
				{
					std::string idlePath = GetPath(BossAnimationID::Idle);
					m_clips[i].Load(idlePath.c_str());
				}
			}
		}*/

		void BossAnimation::Init(const char *type)
		{
			m_type = type;

			for (int i = 0; i < (int)BossAnimationID::Max; i++)
			{
				std::string path = GetPath((BossAnimationID)i);

				if (PathFileExistsA(path.c_str()))
				{
					m_clips[i].Load(path.c_str());
					m_clips[i].BuildKeyFramesAndAnimationEvents();

					//    ` F b N1:  {   Ƀ  [ h ł  Ă  邩 m F
					if (m_clips[i].GetKeyFramePtrListArray().empty())
					{
						OutputDebugStringA(("ERROR: Load failed but file exists: " + path + "\n").c_str());
					}
				}
				else
				{
					//    ` F b N2:  t @ C     Ȃ  ꍇ AIdle     [ h
					std::string idlePath = GetPath(BossAnimationID::Idle);
					m_clips[i].Load(idlePath.c_str());
					m_clips[i].BuildKeyFramesAndAnimationEvents();

					OutputDebugStringA(("WARNING: File not found, using Idle: " + path + "\n").c_str());
				}
			}
		}

		/* Đ  B*/
		void BossAnimation::PlayAnimation(BossAnimationID id, ModelRender &model)
		{
			model.PlayAnimation((int)id, 0.2f);
		}
	}
}