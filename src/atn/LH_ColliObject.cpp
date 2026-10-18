#include "atn/LH_ColliObject.h"
#include "atn/LoadManager.h"
#include "math/Vec.h"

extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix

// inferred: object built from the loaded data (constructor 0x8043F5C4, destructor 0x8043F63C).
// At most 0x34 bytes: the original places it at sp+0x14, after the 12-byte scale vector.
struct ColliObjectData_unk {
    ColliObjectData_unk(void* data);
    ~ColliObjectData_unk();
    unsigned char m00[0x20];
    void* m20; // the collision object
    unsigned char m24[0x34 - 0x24];
};

// inferred: collision object of the file
struct ColliObject_unk {
    unsigned char m00[0xC];
    void* mRoot; // 0x0C: inferred: root node
};

namespace atn {

// inferred from __vt__Q23atn15LoadColliObject (0x8078FC10)
class LoadColliObject : public LoadCore {
public:
    LoadColliObject(const char* path)
        : LoadCore(path), m5C(0), m60(0.0f), m64(0.0f), m68(0.0f), m6C(0.0f) {}
    virtual ~LoadColliObject();
    virtual void vf0C();
    // NONMATCHING: not decompiled yet (0x80472424: walks the node tree with the node matrices)
    void fn_80472424(void* node, const Matrix3x4& mtx, const Vector3& scale);

    ColliObject_unk* m5C;
    float m60;
    float m64;
    float m68;
    float m6C;
};

LoadColliObject::~LoadColliObject() {}

void LoadColliObject::vf0C() {
    if (mState == 2) {
        ColliObjectData_unk data(m54);
        m5C = (ColliObject_unk*)data.m20;
        fn_80472424(m5C->mRoot, lbl_807C7060, Vector3(1.0f, 1.0f, 1.0f));
        fn_80473104(3);
    }
}

void* LH_ColliObject::fn_80472A34() {
    if (mCore != 0) {
        return ((LoadColliObject*)mCore)->m5C;
    }
    return 0;
}

void LH_ColliObject::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadColliObject>(path, 1);
}

} // namespace atn
