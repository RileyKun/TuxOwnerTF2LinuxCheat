#include "SDK.h"
/*
mstudiobbox_t *get_studio_hitbox(int Hitbox, studiohdr_t* Header)
{
	int HitboxSetIndex = *(int *)((uintptr_t)Header + 0xB0);
	mstudiohitboxset_t *Set = (mstudiohitboxset_t *)(((uintptr_t)Header) + HitboxSetIndex);

	return (mstudiobbox_t *)Set->pHitbox(Hitbox);
}
*/

/*
#define HITBOX_SET 0
static vec3_t get_hitbox_pos(Entity* ent, int hitbox_idx) {
    static matrix3x4_t bones[MAXSTUDIOBONES];

    Renderable* rend = GetRenderable(ent);

    if (!METHOD_ARGS(rend, SetupBones, bones, MAXSTUDIOBONES,
                     BONE_USED_BY_HITBOX, 0))
        return VEC_ZERO;

    const model_t* model = METHOD(rend, GetModel);
    if (!model)
        return VEC_ZERO;

    studiohdr_t* hdr = METHOD_ARGS(i_modelinfo, GetStudioModel, model);
    if (!hdr)
        return VEC_ZERO;

    return center_of_hitbox(hdr, bones, HITBOX_SET, hitbox_idx);
}
*/
/*
Vector CBaseEntity::GetHitboxPosition(int Hitbox)
{
	uintptr_t* model = this->GetModel();
	if (!model)
		return Vector();

	studiohdr_t *hdr = gInts.ModelInfo->GetStudiomodel(model);
	if (!hdr)
		return Vector();

	matrix3x4 matrix[128];
	if (!this->SetupBones(matrix, 128, 0x100, 0))
		return Vector();

	int HitboxSetIndex = *(int *)((uintptr_t*)hdr + 0xB0);
	if (!HitboxSetIndex)
		return Vector();

	mstudiohitboxset_t *pSet = (mstudiohitboxset_t *)(((unsigned char*)hdr) + HitboxSetIndex);

	mstudiobbox_t* box = pSet->pHitbox(Hitbox);
	if (!box)
		return Vector();

	Vector vCenter = (box->bbmin + box->bbmax) * 0.5f;

	Vector vHitbox;

	VectorTransform(vCenter, matrix[box->bone], vHitbox);

	return vHitbox;
}
*/