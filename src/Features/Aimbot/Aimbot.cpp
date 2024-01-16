#include "Aimbot.h"
#include "../../SDK/studiohdrfrom8dcc.h"
#include "../../CMenu.h"
#include "../../SDK/bspflags.h"
/*
*   Original Coder Of this aimbot: 8dcc.
*
*   Ported to TuxOwner by Vannie
*           13-01-2024
*/

CAim gAim;

void CAim::setting_to_hitboxes(int setting, int* min, int* max) 
{
    /*
    switch (gCheatMenu.aimbot_hitbox) {
        case 0: 
            *min = HITBOX_HEAD;
            *max = HITBOX_HEAD;
            break;
        case 1:
            *min = HITBOX_PELVIS;
            *max = HITBOX_SPINE3;
            break;
        case 2:
            *min = HITBOX_LEFT_UPPER_ARM;
            *max = HITBOX_RIGHT_HAND;
            break;
        case 3:
            *min = HITBOX_LEFT_HIP;
            *max = HITBOX_RIGHT_FOOT;
            break;
    }
    */
    if (gCheatMenu.aimbot_hitbox == 0)
    {
        *min = HITBOX_HEAD;
        *max = HITBOX_HEAD;
    }
    if (gCheatMenu.aimbot_hitbox == 1)
    {
        *min = HITBOX_PELVIS;
        *max = HITBOX_SPINE3;
    }
    if (gCheatMenu.aimbot_hitbox == 2)
    {
        *min = HITBOX_LEFT_UPPER_ARM;
        *max = HITBOX_RIGHT_HAND;        
    }
    if (gCheatMenu.aimbot_hitbox == 3)
    {
        *min = HITBOX_LEFT_HIP;
        *max = HITBOX_RIGHT_FOOT;        
    }
}
#define VEC_ZERO ((Vector){ 0.f, 0.f, 0.f })
#define HITBOX_SET 0
#define MAXSTUDIOBONES 128

float CAim::GetFOV(Vector angle, Vector src, Vector dst)
{
	Vector ang, aim;
	float mag, u_dot_v;
	ang = calc_angle(src, dst);


	MakeVector(angle, aim);
	MakeVector(ang, ang);

	mag = sqrtf(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
	u_dot_v = aim.Dot(ang);

	return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
}

Vector CAim::calc_angle(Vector src, Vector dst)
{
	Vector AimAngles, delta;
	float hyp;
	delta = src - dst;
	hyp = sqrtf((delta.x * delta.x) + (delta.y * delta.y)); //SUPER SECRET IMPROVEMENT CODE NAME DONUT STEEL
	AimAngles.x = atanf(delta.z / hyp) * RADPI;
	AimAngles.y = atanf(delta.y / delta.x) * RADPI;
	AimAngles.z = 0.0f;
	if (delta.x >= 0.0)
		AimAngles.y += 180.0f;
	return AimAngles;
}
void CAim::MakeVector(Vector angle, Vector& vector)
{
	float pitch, yaw, tmp;
	pitch = float(angle[0] * PI / 180);
	yaw = float(angle[1] * PI / 180);
	tmp = float(cos(pitch));
	vector[0] = float(-tmp * -cos(yaw));
	vector[1] = float(sin(yaw)*tmp);
	vector[2] = float(-sin(pitch));
}

float CAim::flGetDistance(Vector vOrigin, Vector vLocalOrigin)
{
	Vector vDelta = vOrigin - vLocalOrigin;

	float m_fDistance = sqrt(vDelta.Length());

	if (m_fDistance < 1.0f)
		return 1.0f;

	return m_fDistance;
}


int CAim::GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	int iBestTarget = -1;
						 //this num could be smaller 
	float flDistToBest = 99999.f;

	Vector vLocal = pLocal->GetEyePosition();

	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (!pEntity->IsAlive() || pEntity->GetTeamNum() == pLocal->GetTeamNum())
			continue;

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity = pEntity->get_hitbox_pos(iBestHitbox); //pEntity->GetWorldSpaceCenter(vEntity);

		if (!gCheatMenu.PlayerMode[i])
			continue;

		if (pEntity->GetCond() & TFCond_Ubercharged ||
			pEntity->GetCond() & TFCond_UberchargeFading ||
			pEntity->GetCond() & TFCond_Bonked)
			continue;
        if (pEntity->GetCond() & TFCond_Cloaked && gCheatMenu.priorties_ignorecloaked)
        {
            continue;
        }
        if (pEntity->GetCond() & TFCond_Taunting && gCheatMenu.priorties_ignoretaunting)
        {
            continue;
        }
        if (pEntity->GetCond() & TFCond_Disguised && gCheatMenu.priorties_ignoredisguised)
        {
            continue;
        }

		float flDistToTarget = (vLocal - vEntity).Length();
        double minimalDistance = 99999.0;
        float flFOV = GetFOV(pCommand->viewangles, vLocal, vEntity);
		float distance = flGetDistance(vEntity, pLocal->GetEyePosition());
        /*
		if (flDistToTarget < flDistToBest)
		{
			flDistToBest = flDistToTarget;
			iBestTarget = i;
		}
        */
        //if (distance < minimalDistance)//gCvars.aimbot.fov)
	//	{
	
	if (flFOV < gCheatMenu.aimbot_fov)
	{
		if (gCheatMenu.PlayerMode[i] == 2)
			return i;
		//flDistToBest = flDistToTarget;
		//flDistToBest = flFOV;
		gCheatMenu.iAimbotIndex = i;
		iBestTarget = i;
	}
		//}
	}

	return iBestTarget;
}


void FixMovementForUserCmd(CUserCmd* cmd, old_movement_t mov) { // old, from potassium.
	float deltaView = cmd->viewangles.x - mov.angle.y;
	float f1;
	float f2;

	if (mov.angle.y < 0.f)
		f1 = 360.0f + mov.angle.y;
	else
		f1 = mov.angle.y;

	if (cmd->viewangles.y < 0.0f)
		f2 = 360.0f + cmd->viewangles.y;
	else
		f2 = cmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	cmd->forwardmove = cos(DEG2RAD(deltaView)) * mov.fwd + cos(DEG2RAD(deltaView + 90.f)) * mov.sdm;
	cmd->sidemove = sin(DEG2RAD(deltaView)) * mov.fwd + sin(DEG2RAD(deltaView + 90.f)) * mov.sdm;
}
void silentMovementFix(CUserCmd *pUserCmd, Vector angles) //Fix for silent movement    creds gir + f1ssi0n
{
	Vector vecSilent(pUserCmd->forwardmove, pUserCmd->sidemove, pUserCmd->upmove);
	float flSpeed = sqrt(vecSilent.x * vecSilent.x + vecSilent.y * vecSilent.y);
	Vector angMove;
	VectorAngles(vecSilent, angMove);
	float flYaw = DEG2RAD(angles.y - pUserCmd->viewangles.y + angMove.y);
	pUserCmd->forwardmove = cos(flYaw) * flSpeed;
	pUserCmd->sidemove = sin(flYaw) * flSpeed;
}


bool IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd)
{
	trace_t Trace;
	Ray_t Ray;			 // the future of variable naming
	CTraceFilter Filter;

	Filter.pSkip = pLocal;

	Ray.Init(vStart, vEnd);

	gInts.EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	return (Trace.m_pEnt == pEntity);
}

int CAim::GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	int iBestHitbox = -1;

	if (!gCheatMenu.aimbot_hitbox)
	{
		if (pLocal->GetClassNum() == TF2_Sniper)
			iBestHitbox = 0;
		else
			iBestHitbox = 4;
	}
	else
	{
		iBestHitbox = gCheatMenu.aimbot_hitbox - 1;
	}

	if (gCheatMenu.aimbot_hitscan)
	{
		for (int i = 0; i < 17; i++)
		{
			if (IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->get_hitbox_pos(i)))
				return i;
		}
	}

	if (pEntity->get_hitbox_pos(iBestHitbox) == VEC_ZERO)
		return -1;

	if (!IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->get_hitbox_pos(iBestHitbox)))
		return -1;
	
	return iBestHitbox;
}

bool CAim::CanAmbassadorHeadshot(CBaseEntity* pLocal)
{
	//return gInts.Engine->Time() - pLocal->GetActiveWeapon()->m_flLastFireTime() >= 0.930;//1.0;
}


void CAim::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{    
	old_movement_t old_mov = old_movement_t();
    	/* "GetActiveWeapon" does not work. i think. */
    	//auto pWep = pLocal->GetActiveWeapon();

    	//if(pWep->GetSlot() == WPN_SLOT_MELEE) // dont try to melee, it wont work lol
    	//{
        //	return;
    	//}

	if (gCheatMenu.aimbot_silent) { // only backup the c_usercmd data when it's needed.
		old_mov.angle = pCommand->viewangles;
		old_mov.fwd = pCommand->forwardmove;
		old_mov.sdm = pCommand->sidemove;
	}

	gCheatMenu.iAimbotIndex = -1;

	if (!gCheatMenu.aimbot_active)
		return;

	//if (!pLocal->GetActiveWeapon())
	//	return;

	CBaseEntity* pEntity = GetBaseEntity(GetBestTarget(pLocal, pCommand));

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	Vector vEntity = pEntity->get_hitbox_pos(iBestHitbox);

	Vector vLocal = pLocal->GetEyePosition();

	Vector vAngs;
	Vector DIFF = vEntity - vLocal;
	VectorAngles(DIFF, vAngs);

	ClampAngle(vAngs);
	gCheatMenu.iAimbotIndex = pEntity->GetIndex();
	//auto pWep = pLocal->GetActiveWeapon();


	//if (pWep->GetItemDefinitionIndex() == spyweapons::WPN_Ambassador || pWep->GetItemDefinitionIndex() == spyweapons::WPN_FestiveAmbassador)
	//		if (!CanAmbassadorHeadshot(pLocal)) return;	
	//pCommand->viewangles = vAngs; // always set this cuz otherwise the viewangles will desync.

	if (!gCheatMenu.aimbot_silent) {
		gInts.Engine->SetViewAngles(pCommand->viewangles);
	}

	if (gCheatMenu.aimbot_silent) 
    { // apply our movement fix if silent aim is enabled.
		//FixMovementForUserCmd(pCommand, old_mov);
		if (pCommand->buttons & IN_ATTACK)
		{
			w(true, vAngs, pCommand);
		}
		else if (gCheatMenu.aimbot_autoshoot)
		{
			w(true, vAngs, pCommand);
			pCommand->buttons |= IN_ATTACK;
		}
		else
		{
			if (pCommand->buttons & IN_ATTACK)
				w(true, vAngs, pCommand);
		}
	}
	else
	{
		w(gCheatMenu.aimbot_silent, vAngs, pCommand);
		if (gCheatMenu.aimbot_autoshoot)
			pCommand->buttons |= IN_ATTACK;
	}
	/*
	if (gCheatMenu.aimbot_autoshoot)
	{
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		if (pLocal->szGetClass() == "Sniper")
		{
			if (!gCheatMenu.aimbot_zoomedonly)
			{
				if (flCurTime > flNextSend)
				{
					pCommand->buttons |= IN_ATTACK;
					flNextSend = (flCurTime + 0.2f); // this is retarded but fuck it 
				}
			}
			else if (gCheatMenu.aimbot_zoomedonly)
			{
				if (pLocal->IsScoped())
				{
					if (flCurTime > flNextSend)
					{
						pCommand->buttons |= IN_ATTACK;
						flNextSend = (flCurTime + 0.2f);
					}
				}
			}
		}
		if (pLocal->szGetClass() != "Sniper") // yey
		{
			pCommand->buttons |= IN_ATTACK;
		}
	}
	*/ // i dont play sniper.

	if (gCheatMenu.aimbot_autoscope && !pLocal->IsScoped() && pLocal->szGetClass() == "Sniper")
	{
		pCommand->buttons |= IN_ATTACK2;
	}
    
}


void CAim::w(bool silent, Vector vAngs, CUserCmd* pCommand)
{
	if (silent)
	{
		ClampAngle(vAngs);
		silentMovementFix(pCommand, vAngs);
		pCommand->viewangles = vAngs;

	}
	else
	{
		pCommand->viewangles = vAngs;
		gInts.Engine->SetViewAngles(pCommand->viewangles);
	}
}