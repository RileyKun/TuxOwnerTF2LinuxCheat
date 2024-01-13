#include "../../SDK.h"
#include "../../SDK/studiohdrfrom8dcc.h"

class CAim
{
public:
    void setting_to_hitboxes(int setting, int* min, int* max);
    static Vector get_hitbox_pos(CBaseEntity* pEntity, int hitbox_idx);
    int GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity);
    int GetBestTarget(CBaseEntity* pLocal);
    void Run(CBaseEntity* pLocal, CUserCmd* pCommand);
};

extern CAim gAim;