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

void VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut)
{
	for (auto i = 0; i < 3; i++)
		vOut[i] = vSome.Dot((Vector&)vMatrix[i]) + vMatrix[i][3];
}

Vector center_of_hitbox(studiohdr_t* studio, matrix3x4* bonemat, int set, int idx) 
{
    studiobbox_t* bbox = studiohdr_pHitbox(studio, set, idx);
    if (!bbox)
        return VEC_ZERO;

    Vector min, max;
    VectorTransform(bbox->bbmin, bonemat[bbox->bone], min);
    VectorTransform(bbox->bbmax, bonemat[bbox->bone], max);

    return (Vector){
        (min.x + max.x) * 0.5f,
        (min.y + max.y) * 0.5f,
        (min.z + max.z) * 0.5f,
    };
}
#define BONE_USED_BY_HITBOX 0x00000100
static Vector CAim::get_hitbox_pos(CBaseEntity* pEntity, int hitbox_idx) 
{
    static matrix3x4 bones[MAXSTUDIOBONES];
    if (!pEntity->SetupBones(bones, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, 0))
        return VEC_ZERO;

    const auto model = pEntity->GetModel();
    if (!model)
        return VEC_ZERO;

    studiohdr_t* hdr = gInts.ModelInfo->GetStudiomodel(model);
    if (!hdr)
        return VEC_ZERO;

    return center_of_hitbox(hdr, bones, HITBOX_SET, hitbox_idx);
}

int CAim::GetBestTarget(CBaseEntity* pLocal)
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

		Vector vEntity = get_hitbox_pos(pEntity, iBestHitbox); //pEntity->GetWorldSpaceCenter(vEntity);

		if (!gCheatMenu.PlayerMode[i])
			continue;

		if (pEntity->GetCond() & TFCond_Ubercharged ||
			pEntity->GetCond() & TFCond_UberchargeFading ||
			pEntity->GetCond() & TFCond_Bonked)
			continue;

		float flDistToTarget = (vLocal - vEntity).Length();

		if (flDistToTarget < flDistToBest)
		{
			flDistToBest = flDistToTarget;
			iBestTarget = i;
		}

		if (gCheatMenu.PlayerMode[i] == 2) //always aim at rage targets first
			return i;
	}

	return iBestTarget;
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
			if (IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), get_hitbox_pos(pEntity, i)))
				return i;
		}
	}

	if (get_hitbox_pos(pEntity, iBestHitbox) == VEC_ZERO)
		return -1;

	if (!IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), get_hitbox_pos(pEntity, iBestHitbox)))
		return -1;
	
	return iBestHitbox;
}

void CAim::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{    
	old_movement_t old_mov = old_movement_t();

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

	CBaseEntity* pEntity = GetBaseEntity(GetBestTarget(pLocal));

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	Vector vEntity = get_hitbox_pos(pEntity,iBestHitbox);

	Vector vLocal = pLocal->GetEyePosition();

	Vector vAngs;
	Vector DIFF = vEntity - vLocal;
	VectorAngles(DIFF, vAngs);

	ClampAngle(vAngs);
	gCheatMenu.iAimbotIndex = pEntity->GetIndex();

	pCommand->viewangles = vAngs; // always set this cuz otherwise the viewangles will desync.

	if (!gCheatMenu.aimbot_silent) {
		gInts.Engine->SetViewAngles(pCommand->viewangles);
	}

	if (gCheatMenu.aimbot_silent) { // apply our movement fix if silent aim is enabled.
		//Util->FixMovementForUserCmd(pCommand, old_mov);
	}

	if (gCheatMenu.aimbot_autoshoot)
		pCommand->buttons |= IN_ATTACK;
}
