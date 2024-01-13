#include "ESP.h"
#include "../../CMenu.h"
ESP gESP;
void ESP::StartThisshit(CBaseEntity* pLocal)
{
    for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (!pEntity->IsAlive())
			continue;
		if (gCheatMenu.esp_active) // finally added this lmao
			ActualfreakinESP(pEntity);

	}
}

int ESP::GetPlayerColor(CBaseEntity* pPlayer)
{
	switch (pPlayer->GetTeamNum())
	{
	case 2: //RED
		return COLORCODE(255, 20, 20, 255);
	case 3: //BLU
		return COLORCODE(0, 153, 255, 255);
	default:
		return COLORCODE(0, 153, 255, 255);
	}
	//return Color(0, 0, 0, 0); //no reason for this to be here, i just wanna look smart
}


/*
TODO:
Team Color. <- DONE!
Conditions. <- DONE!
*/
void ESP::ActualfreakinESP(CBaseEntity* pEntity)
{
    Vector pEntvecWorld, pEntvecScreen;

    player_info_t pInfo;

	if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
		return;
    /*
	pLocal->GetWorldSpaceCenter(pLocalvecWorld); //Get the center of the player.

	if ( gDrawManager.WorldToScreen(pLocalvecWorld, pLocalvecScreen) ) //If the player is visble.
	{
		gDrawManager.DrawString( pLocalvecScreen.x, pLocalvecScreen.y, 0xFFFFFFFF, "pLocal" ); //Draw on the player.
	}
    */ // meh i dont need plocal esp 
    pEntity->GetWorldSpaceCenter(pEntvecWorld);
    int health = pEntity->GetHealth();
	int teamcolor = GetPlayerColor(pEntity); // you should know what this means
	//int healthcolor = COLORCODE(255, 20, 20, 255); // healthcolor (RED) looks DISGASTANG!
	int CONDCOLOR = COLORCODE(255, 255, 0, 255); // yellow
	
    if (gDrawManager.WorldToScreen(pEntvecWorld, pEntvecScreen))
    {
        gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y, teamcolor, pInfo.name ); 
        gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y + 10, teamcolor, "HP: %d", health); 
		if (pEntity->GetTeamNum() == 2)
		{
			gDrawManager.DrawString(pEntvecScreen.x, pEntvecScreen.y + 20, teamcolor, "RED");
		}
		if (pEntity->GetTeamNum() == 3)
		{ 
			gDrawManager.DrawString(pEntvecScreen.x, pEntvecScreen.y + 20, teamcolor, "BLU");
		}			
		if (pEntity->GetCond() == TFCond_Bonked)
		{
			gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y + 30, CONDCOLOR, "*BONKED*"); 
		}
		if (pEntity->GetCond() == TFCond_Taunting)
		{
			gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y + 30, CONDCOLOR, "*TAUNTING*"); 
		}
		if (pEntity->GetCond() == TFCond_Ubercharged)
		{
			gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y + 30, CONDCOLOR, "*UBERED!*"); 
		}
		if (pEntity->GetCond() == TFCond_Bleeding)
		{
			gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y + 30, CONDCOLOR, "*BLEEDIN'*"); 
		}
		if (pEntity->GetCond() == TFCond_Cloaked)
		{
			gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y + 30, CONDCOLOR, "*CLOAKED'*"); 
		}
		if (pEntity->GetCond() == TFCond_Zoomed)
		{
			gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y + 30, CONDCOLOR, "*ZOOMED*"); 
		}
			
	}

}