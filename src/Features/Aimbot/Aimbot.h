#include "../../SDK.h"
#include "../../SDK/studiohdrfrom8dcc.h"

class CAim
{
public:
    void setting_to_hitboxes(int setting, int* min, int* max);
    //static Vector get_hitbox_pos(CBaseEntity* pEntity, int hitbox_idx);
    int GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity);
    int GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCommand);
    void MakeVector(Vector angle, Vector& vector);
    Vector calc_angle(Vector src, Vector dst);
    float flGetDistance(Vector vOrigin, Vector vLocalOrigin);
    void Run(CBaseEntity* pLocal, CUserCmd* pCommand);
    float GetFOV(Vector angle, Vector src, Vector dst);
    bool CanAmbassadorHeadshot(CBaseEntity* pLocal);
    void w(bool silent, Vector vAngs, CUserCmd* pCommand);
};

extern CAim gAim;