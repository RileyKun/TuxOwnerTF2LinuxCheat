#pragma once

#include "SDK.h"

class CGlobalzz {
public:
	//CBaseEntity* local{};
	//CUserCmd* cmd{};
	//CUserCmd original_cmd{};
	bool isfiring, silenttime;
	bool* sendpacket;
};

bool Hooked_CreateMove(void *ClientMode, float input_sample_frametime, CUserCmd* pCommand);

extern CGlobalzz g;
