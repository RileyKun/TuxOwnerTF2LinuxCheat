#include "Misc.h"
#include "../../CMenu.h"
CMisc gMisc;
#include <SDL2/SDL.h>
const Uint8 *penisstate = SDL_GetKeyboardState(NULL);
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

	//Vector vX = entity->GetAngles();
	//Vector vY = entity->GetAnglesHTC();
	//auto *WritePitch = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]"));
	//auto *WriteYaw = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[1]"));
	/*
    if (gCvars.aimbot_resolver && resolver)
	{
		if (vX.x == -89.0f)
		{
			*WritePitch = 90.0f;
		}
		if (vX.x == 89.0f)
		{
			*WritePitch = -90.0f;
		}
	}
    */ // todo later
	if (gCheatMenu.misc_tauntspin)
	{
		TauntSpin(pCmd, pLocal);
	}
    if (gCheatMenu.misc_speedcrouch)
    {
        SpeedCrouch(pCmd);
    }
    if (gCheatMenu.misc_thirdperson)
    {
        float flCurTime = gInts.Engine->Time();
	    static float flNextSend = 0.0f;
	    static float thesleeptime = 0.1f;
         

        if (penisstate[SDL_SCANCODE_B] && flCurTime > flNextSend)
        {
            flNextSend = (flCurTime + thesleeptime);
            gCheatMenu.isThirdPersonEnabled = !gCheatMenu.isThirdPersonEnabled;

        
            pLocal->ForceTauntCam(gCheatMenu.isThirdPersonEnabled);
        }
    }
    if (gCheatMenu.misc_enablevmfov)
    {
        pLocal->setfov(gCheatMenu.misc_vmfov);
    }
    if (!gCheatMenu.misc_enablevmfov)
    {
        pLocal->setfov(100); // reset fov i guess sdfshdjdfhj
    }
    if (gCheatMenu.misc_svcheats)
    {
        static ConVar* cheats = gInts.cvar->FindVar("sv_cheats");
        cheats->SetValue(1);
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

void CMisc::SpeedCrouch(CUserCmd* pCmd)
{
    if ((pCmd->buttons & IN_DUCK)) // !(pCmd->buttons & IN_ATTACK)
    {
        Vector vLocalAngles = pCmd->viewangles;
	    float speed = pCmd->forwardmove;
	    if (fabs(speed) > 0.0f) 
        {
		    pCmd->forwardmove = -speed;
		    pCmd->sidemove = 0.0f;
		    pCmd->viewangles.y = vLocalAngles.y;
		    pCmd->viewangles.y -= 180.0f;
		    if (pCmd->viewangles.y < -180.0f) pCmd->viewangles.y += 360.0f;
		    pCmd->viewangles.z = 90.0f;
	    }
    }
}

void CMisc::TauntSpin(CUserCmd* pCmd, CBaseEntity* pLocal)
{
	Vector/*&*/ angles = pCmd->viewangles;
	if (pLocal->GetCond() & TFCond_Taunting)
	{
		static float spinAngle = 0.0f; 

		spinAngle += gCheatMenu.misc_spinspeed; 

		angles.y = spinAngle;
		
	}
}

