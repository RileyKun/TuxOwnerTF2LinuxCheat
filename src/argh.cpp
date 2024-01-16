#include "argh.h"
#include "CMenu.h"
#include <cstring>

CKillSay gKillSay;
void CKillSay::InitKillSay()
{
	gInts.EventManager->AddListener(this, "player_death", false);
	gInts.EventManager->AddListener(this, "player_hurt", false);
	gInts.EventManager->AddListener(this, "player_disconnect", false);
	gInts.EventManager->AddListener(this, "player_changeclass", false);
	gInts.EventManager->AddListener(this, "vote_cast", false);
	wasEventInitialized = true;
}

inline int GetPlayerForUserID(int userID)
{
	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		player_info_t playerInfo;
		if (i == me)
			continue;
		CBaseEntity* pEntity = GetBaseEntity(i);
		if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &playerInfo))
		{
			continue;
		}
		// Found player
		if (playerInfo.userID == userID)
		{
			return i;
		}
	}
	return 0;
}

void CKillSay::FireGameEvent(IGameEvent *event)
{	
	if (!strcmp(event->GetName(), "player_death"))
	{
		int vid = event->GetInt("userid");
		int kid = event->GetInt("attacker");
		//const int attacker = GetPlayerForUserID(event->GetInt("attacker"));
		//const int userid = GetPlayerForUserID(event->GetInt("userid"));
		if (vid == gInts.Engine->GetLocalPlayer())
		{
			return;
		}

		if (kid != gInts.Engine->GetLocalPlayer())
		{
			return;
		}
		const auto& pEntity = gInts.EntList->GetClientEntity(vid);
		if (!pEntity)
		{
			return;
		}
		player_info_t pInfo;
		if (!gInts.Engine->GetPlayerInfo(kid, &pInfo))
			return;
		gInts.ClientMode->m_pChat->ChatPrintf(0, "\x6[\x3Tux\x6] \x4%s died! RIP LOL! ", pInfo.name);	
	}
	/*
	if (strcmp(event->GetName(), "player_death"))
		return;

	gInts.ClientMode->m_pChat->ChatPrintf(0, "[Tux] Some nigga died (player_death)");
	
	if (!strcmp(event->GetName(), "player_changeclass")) 
	{
		//auto userId = event->GetInt("userid");
		//int playerID = gInts.Engine->GetPlayerForUserID(userId);
		//if(const auto& entity = gInts.EntList->GetClientEntity(gInts.Engine->GetPlayerForUserID(event->GetInt("userid"))));
		//{

		
			//if (!entity)
			//	return;
			player_info_t pInfo;
			if (!gInts.Engine->GetPlayerInfo(event->GetInt("userid"), &pInfo))
				return;

			gInts.ClientMode->m_pChat->ChatPrintf(0, "\x6[\x3Tux\x6] \x4%s changed class to ", pInfo.name);
		//}
	}
	*/
}