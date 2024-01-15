#include "argh.h"
#include "CMenu.h"
CKillSay gKillSay;
#include <cstring>


void CKillSay::InitKillSay()
{
	gInts.EventManager->AddListener(this, "player_death", false);
	gInts.EventManager->AddListener(this, "player_hurt", false);
	gInts.EventManager->AddListener(this, "player_disconnect", false);
	gInts.EventManager->AddListener(this, "player_changeclass", false);
	gInts.EventManager->AddListener(this, "vote_cast", false);
	wasEventInitialized = true;
}
void CKillSay::FireGameEvent(IGameEvent *event)
{	/*
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