#include "Aimbot.h"
#include "../../SDK/studiohdrfrom8dcc.h"
#include "../../CMenu.h"
#include "../../SDK/bspflags.h"
#include <SDL2/SDL.h>

/*
*   Original Coder of the studiohdr & gethitbox: 8dcc.
*
*   Ported to TuxOwner by Vannie
*           13-01-2024
*/

CAim gAim;
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

/*
    	if (distance < minimalDistance)//gCvars.aimbot.fov)
		{
	
			if (flFOV < gCheatMenu.aimbot_fov)
			{
				if (gCheatMenu.PlayerMode[i] == 2)
					return i;
				if (pEntity->GetHealth() < 100)
					return i;
				if (pEntity->GetClassNum() == TF2_Heavy && gCheatMenu.priorties_preferheavy)
					return i;
				if (pEntity->GetClassNum() == TF2_Medic && gCheatMenu.priorties_prefermedic)
					return i;
				gCheatMenu.iAimbotIndex = i;
				iBestTarget = i;
			}
		}
		*/

int CAim::GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	int iBestTarget = -1;
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
			//pEntity->GetCond() & TFCond_UberchargeFading || <- ubercharge fading ignores forever for some reason. Get rid of it!
			pEntity->GetCond() & TFCond_Bonked)
			continue;
        if (pEntity->GetCond() & TFCond_Cloaked && gCheatMenu.priorties_ignorecloaked && gCheatMenu.PlayerMode[i] != 2)
        {
            continue;
        }
        if (pEntity->GetCond() & TFCond_Taunting && gCheatMenu.priorties_ignoretaunting && gCheatMenu.PlayerMode[i] != 2)
        {
            continue;
        }
        if (pEntity->GetCond() & TFCond_Disguised && gCheatMenu.priorties_ignoredisguised && gCheatMenu.PlayerMode[i] != 2)
        {
            continue;
        }
		

		float flDistToTarget = (vLocal - vEntity).Length();
        double minimalDistance = 99999.0;
        float flFOV = GetFOV(pCommand->viewangles, vLocal, vEntity);
		float distance = flGetDistance(vEntity, pLocal->GetEyePosition());
		float max_delta = FLT_MAX;
		float delta = calc_angle(pLocal->GetEyePosition(), pEntity->GetEyePosition(), pCommand->viewangles).Length();
		if (delta < max_delta && delta < gCheatMenu.aimbot_fov) 
		{
			max_delta = delta;
			gCheatMenu.iAimbotIndex = i;
			if (pEntity->GetHealth() < 100 && gCheatMenu.aimbot_preferlowhp)
				return i;
			if (pEntity->GetClassNum() == TF2_Heavy && gCheatMenu.priorties_preferheavy)
				return i;
			if (pEntity->GetClassNum() == TF2_Medic && gCheatMenu.priorties_prefermedic)
				return i;
			iBestTarget = i;
		}
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

	const Uint8 *epic = SDL_GetKeyboardState(NULL);


	old_movement_t old_mov = old_movement_t();
    // i genuinely think get slot is fucked lmao
	if (!pLocal->GetActiveWeapon())
		return;
	
	
	if (pLocal->GetActiveWeapon()->GetWeaponID() == TF_WEAPON_KNIFE)
		return;  

	//if (pLocal->GetActiveWeapon()->GetSlot() == WPN_SLOT_MELEE)
	//	return; // dont melee



	//if (pLocal->GetActiveWeapon()->GetSlot() == WPN_SLOT_PRIMARY && pLocal->szGetClass() == "Spy")
	//	return; // you are retarded if you think you can backstab by using the aimbot. We have a fucking autobackstab feature for a reason lmao
	if (gCheatMenu.aimbot_silent) { // only backup the c_usercmd data when it's needed.
		old_mov.angle = pCommand->viewangles;
		old_mov.fwd = pCommand->forwardmove;
		old_mov.sdm = pCommand->sidemove;
	}

	gCheatMenu.iAimbotIndex = -1;

	if (!gCheatMenu.aimbot_active)
		return;
	
	if (pCommand->buttons & IN_RELOAD) // if we are reloading, fucking stop trying to shoot
	{
		return;
	}
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

	if (gCheatMenu.warp_enable && !epic[SDL_SCANCODE_R])
	{
		return;
	}

	//if (pWep->GetItemDefinitionIndex() == spyweapons::WPN_Ambassador || pWep->GetItemDefinitionIndex() == spyweapons::WPN_FestiveAmbassador)
	//		if (!CanAmbassadorHeadshot(pLocal)) return;	
	pCommand->viewangles = vAngs; // always set this cuz otherwise the viewangles will desync.

	if (!gCheatMenu.aimbot_silent) {
		gInts.Engine->SetViewAngles(pCommand->viewangles);
	}

	if (gCheatMenu.aimbot_silent) 
    { // apply our movement fix if silent aim is enabled.
		FixMovementForUserCmd(pCommand, old_mov);
	}
	
	if (gCheatMenu.aimbot_autoshoot)
	{
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		if (gCheatMenu.aimbot_zoomedonly)
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
		if (!gCheatMenu.aimbot_zoomedonly) // yey
		{
			pCommand->buttons |= IN_ATTACK;
		}
	}

	if (gCheatMenu.aimbot_autoscope && !pLocal->IsScoped() && pLocal->GetClassNum() == TF2_Sniper)
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