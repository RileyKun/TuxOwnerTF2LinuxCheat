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
    Vector calc_angle(const Vector& source, const Vector& destination, const Vector& view_angles) {
		Vector delta = source - destination;
		auto radians_to_degrees = [](float radians) { return radians * 180 / static_cast<float>(M_PI); };
		Vector angles;
		angles.x = radians_to_degrees(atanf(delta.z / std::hypotf(delta.x, delta.y))) - view_angles.x;
		angles.y = radians_to_degrees(atanf(delta.y / delta.x)) - view_angles.y;
		angles.z = 0.0f;

		if (delta.x >= 0.0)
			angles.y += 180.0f;

		// normalize
		angles.x = std::isfinite(angles.x) ? std::remainderf(angles.x, 360.0f) : 0.0f;
		angles.y = std::isfinite(angles.y) ? std::remainderf(angles.y, 360.0f) : 0.0f;
		angles.z = 0.0f;

		return angles;
	}
};

extern CAim gAim;