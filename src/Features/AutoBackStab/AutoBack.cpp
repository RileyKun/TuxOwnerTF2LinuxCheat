#include "AutoBack.h"
/*
CAutoBackStab gStab;

bool CAutoBackStab::CanBackstab(const Vector &vSrc, const Vector &vDst, Vector vWSCDelta)
{
	vWSCDelta.z = 0;
	vWSCDelta.NormalizeInPlace();

	Vector vecEyeSpy = Vector();
	AngleVectors(vSrc, &vecEyeSpy);
	vecEyeSpy.z = 0;
	vecEyeSpy.NormalizeInPlace();

	Vector vecEyeVictim = Vector();
	AngleVectors(vDst, &vecEyeVictim);
	vecEyeVictim.z = 0;
	vecEyeVictim.NormalizeInPlace();

	if (vWSCDelta.Dot(vecEyeVictim) <= 0.01f)
		return false;

	if (vWSCDelta.Dot(vecEyeSpy) <= 0.5f)
		return false;

	if (vecEyeSpy.Dot(vecEyeVictim) <= -0.3f)
		return false;

	return true;
}

void TraceHull( const Vector& vecStart, const Vector& vecEnd, const Vector& vecHullMin, const Vector& vecHullMax, unsigned int nMask, CTraceFilter *pFilter, CGameTrace *pTrace)
{
	Ray_t ray;
    Vector vecPenisStart = vecStart;
    Vector vecPenisEnd   = vecEnd;
    Vector vecHullRealM  = vecHullMax;
    Vector vecHullMini   = vecHullMin;
	ray.Init(vecPenisStart, vecPenisEnd, vecHullMini, vecHullRealM);
	gInts.EngineTrace->TraceRay(ray, nMask, pFilter, pTrace);
}
#include "../../SDK/bspflags.h"
bool CAutoBackStab::TraceMelee(CBaseEntity *pLocal, CBaseCombatWeapon *pWeapon, const Vector &vViewAngles, CBaseEntity **pEntityOut)
{
	float flRange = (48.0f * 0.9f); // used to be "Vars::Triggerbot::Stab.m_var"

	if (flRange <= 0.0f)
		return false;

	Vector vForward = Vector();
	AngleVectors(vViewAngles, &vForward);
	Vector vTraceStart = pLocal->GetShootPos();
	Vector vTraceEnd = (vTraceStart + (vForward * flRange));

	trace_t Trace = {};
	CTraceFilter Filter = {};
	Filter.pSkip = pLocal;
	TraceHull(vTraceStart, vTraceEnd, { -18.0f, -18.0f, -18.0f }, { 18.0f, 18.0f, 18.0f }, MASK_SOLID, &Filter, &Trace);

	if (IsEntityValid(pLocal, Trace.m_pEnt))
	{
		if (pEntityOut && !*pEntityOut)
			*pEntityOut = Trace.m_pEnt;

		return true;
	}

	return false;
}

bool CAutoBackStab::IsEntityValid(CBaseEntity *pLocal, CBaseEntity *pEntity)
{
	if (!pEntity || !pEntity->IsAlive() || pEntity->GetTeamNum() == pLocal->GetTeamNum())
		return false;

	//if (Vars::Triggerbot::Global::IgnoreInvlunerable.m_Var && !pEntity->IsVulnerable())
	//	return false;

	//if (Vars::Triggerbot::Global::IgnoreCloaked.m_Var && pEntity->IsCloaked())
	//	return false;

	//if (Vars::Triggerbot::Global::IgnoreFriends.m_Var && g_EntityCache.Friends[pEntity->GetIndex()])
	//	return false;
    //  for latuh.
	return true;
}


void CAutoBackStab::RunRage(CBaseEntity *pLocal, CBaseCombatWeapon *pWeapon, CUserCmd *pCmd)
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
		//if (pEnemy->GetClassNum() == ETFClass::CLASS_SNIPER && pEnemy->GetWeaponFromSlot(EWeaponSlots::SLOT_SECONDARY)->GetItemDefIndex() == Sniper_s_TheRazorback)
		//	continue;

		CBaseEntity *pTraceEnemy = nullptr;

		Vec3 vAngleTo = Math::CalcAngle(pLocal->GetShootPos(), pEnemy->GetHitboxPos(HITBOX_BODY));

		if (!TraceMelee(pLocal, pWeapon, vAngleTo, &pTraceEnemy) || pTraceEnemy != pEnemy)
			continue;

		if (!CanBackstab(vAngleTo, pEnemy->GetEyeAngles(), (pEnemy->GetWorldSpaceCenter() - pLocal->GetWorldSpaceCenter())))
			continue;

		if (Vars::Triggerbot::Stab::Silent.m_Var) {
			Utils::FixMovement(pCmd, vAngleTo);
			g_GlobalInfo.m_bSilentTime = true;
		}

		pCmd->viewangles = vAngleTo;
		pCmd->buttons |= IN_ATTACK;
		m_bShouldDisguise = true;

		if (Vars::Misc::DisableInterpolation.m_Var)
		{
			pCmd->tick_count = TIME_TO_TICKS(pEnemy->GetSimulationTime()
				+ std::max(g_ConVars.cl_interp->GetFloat(), g_ConVars.cl_interp_ratio->GetFloat() / g_ConVars.cl_updaterate->GetFloat()));
		}

		break;
	}
}

*/