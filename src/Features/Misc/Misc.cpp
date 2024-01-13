#include "Misc.h"
#include "../../CMenu.h"
CMisc gMisc;

void CMisc::Run(CBaseEntity* pLocal, CUserCmd* pCmd)
{
    if (!(pLocal->GetFlags() & FL_ONGROUND) && pCmd->buttons & IN_JUMP)
    {
        if (gCheatMenu.misc_bunnyhop)
        {
            BHOP(pCmd);
        }
        if (gCheatMenu.misc_autostrafe)
        {
            AutoStrafe(pCmd);
        }
    }
}

void CMisc::BHOP(CUserCmd* pCmd) // yes i know i shouldnt make a another function just for one line of code but fuck you i can do whatever i want lol this is my cheeto.
{
    pCmd->buttons &= ~IN_JUMP; 
}
void CMisc::AutoStrafe(CUserCmd* pCmd)
{
    if (pCmd->mousedx > 1 || pCmd->mousedx < -1)
    {
        pCmd->sidemove = pCmd->mousedx > 1 ? 450.f : -450.f;
    }  //> 1 < -1 so we have some wiggle room
}