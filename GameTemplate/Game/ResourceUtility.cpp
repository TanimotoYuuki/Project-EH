#include "stdafx.h"
#include "ResourceUtility.h"
#include "Src/Actor/Character/Player/Player.h"

namespace
{
	const char* kPlayerTagList[] = { "player1", "player2", "player3", "player4" };
}


namespace nsApp 
{
	namespace nsActor
	{
		Player* ResourceUtility::SearchNearestDownCharacter(Player* helper, float searchRange)
		{
			if (helper == nullptr)
				return nullptr;

			SearchResult searchResult;
			searchResult.nearestTarget = nullptr;
			searchResult.nearestDistance = searchRange;

			const Vector3 helperPosition = helper->GetPosition();

			for (const char* tag : kPlayerTagList)
			{
				Player* target = FindGO<Player>(tag);

				if (!IsValidRescueTarget(helper, target))
					continue;

				// ‚·‚Å‚É‘¼‚ÌNPC‚ª‹~•‚ÉŒü‚©‚Á‚Ä‚¢‚é‘ÎÛ‚ÍœŠO‚·‚é
				if (target->GetRescueStatusManager().IsBeingHelped())
					continue;

				Vector3 difference = target->GetPosition() - helperPosition;
				difference.y = 0.0f;

				const float distance = difference.Length();

				if (distance < searchResult.nearestDistance)
				{
					searchResult.nearestDistance = distance;
					searchResult.nearestTarget = target;
				}
			}

			return searchResult.nearestTarget;
		}


		bool ResourceUtility::IsValidRescueTarget(Player* helper, Player* target)
		{
			if (helper == nullptr || target == nullptr)
				return false;

			if (helper == target)
				return false;

			return target->IsDeath() || target->GetCharacterStatus().hp.currentHP <= 0;
		}


		const char* ResourceUtility::FindPlayerTag(Player* player)
		{
			if (player == nullptr)
				return nullptr;

			for (const char* tag : kPlayerTagList)
			{
				Player* current = FindGO<Player>(tag);
				
				if(current == player)
					return tag;
			}
			return nullptr;
		}


		Player* ResourceUtility::FindPlayerByTag(const char* tag)
		{
			if (tag == nullptr)
				return nullptr;

			return FindGO<Player>(tag);
		}
	}
}