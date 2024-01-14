#include "../SDK.h"

class CMath
{
public:
    Vector center_of_hitbox(studiohdr_t* studio, matrix3x4* bonemat, int set, int idx);
    void   VectorTransform(const Vector& vSome, const matrix3x4& vMatrix, Vector& vOut);

};

extern CMath gMath;