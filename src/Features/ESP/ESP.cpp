#include "ESP.h"

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

		ActualfreakinESP(pEntity);
	}
}
/*
TODO:
Team Color.
Conditions.
more shit
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
    if (gDrawManager.WorldToScreen(pEntvecWorld, pEntvecScreen))
    {
        gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y, 0xFFFFFFFF, pInfo.name ); //Draw on the player.
        gDrawManager.DrawString( pEntvecScreen.x, pEntvecScreen.y + 10, 0xFFFFFFFF, "HP: %d", health); //Draw on the player.

    }

}