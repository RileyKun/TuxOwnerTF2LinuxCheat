#include "../../SDK.h"

class CMisc
{
public:
    void Run(CBaseEntity* pLocal, CUserCmd* pCmd);
    void BHOP(CUserCmd* pCmd);
    void AutoStrafe(CUserCmd* pCmd);


};

extern CMisc gMisc;