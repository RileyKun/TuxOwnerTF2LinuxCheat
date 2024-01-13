#include "../../SDK.h"

class CAutoBackStab
{
public:
    bool CanBackstab(const Vector &vSrc, const Vector &vDst, Vector vWSCDelta);
    bool TraceMelee(CBaseEntity *pLocal, CBaseCombatWeapon *pWeapon, const Vector &vViewAngles, CBaseEntity **pEntityOut);
    bool IsEntityValid(CBaseEntity *pLocal, CBaseEntity *pEntity);
    void RunRage(CBaseEntity *pLocal, CBaseCombatWeapon *pWeapon, CUserCmd *pCmd);
};

extern CAutoBackStab gStab;