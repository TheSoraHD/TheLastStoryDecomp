#include "game/Gun.h"
#include "game/Inventory.h"
#include "atn/Memory.h"
#include "game/CrossbowMenuTask.h"
#include "game/HitInfo_unk.h"

extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* node); // inferred: loads a model
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" int fn_804444E8(void* items, int id);       // inferred: number of items `id` owned

// inferred: owner of the weapon (the character, Arms::m270); only the members read here
struct GunOwner_unk {
    unsigned char m000[0x48];
    int m48;
    unsigned char m04C[0x50 - 0x4C];
    int m50;
    unsigned char m054[0x55C - 0x54];
    int m55C; // inferred: state
    int m560; // inferred: sub state
};

// inferred: largest of three values (name invented; as in game/Arms.cpp)
inline float Max3_unk(const atn::Vector3& v) {
    float yz = v.y > v.z ? v.y : v.z;
    return v.x > yz ? v.x : (v.y > v.z ? v.y : v.z);
}

// inferred: translation matrix (name invented)
inline void Trans_unk(atn::Matrix3x4& m, atn::Vector3 v) {
    PSMTXTrans(m.m, v.x, v.y, v.z);
}

// inferred: length of the column `i` of a matrix (name invented; as in game/CostumeEditChara.cpp)
inline float ColumnLength_unk(const atn::Matrix3x4& m, int i) {
    atn::Vector3 v(m.m[0][i], m.m[1][i], m.m[2][i]);
    return PSVECMag(&v);
}

// inferred: largest scale axis of a matrix (name invented)
inline float MaxScale_unk(const atn::Matrix3x4& mtx) {
    atn::Vector3 s(ColumnLength_unk(mtx, 0), ColumnLength_unk(mtx, 1), ColumnLength_unk(mtx, 2));
    return Max3_unk(s);
}

// inferred: sets the world matrix of a model and its largest scale axis (name invented)
inline void SetMtx_unk(atn::Model* model, const atn::Matrix3x4& mtx) {
    __MtxCopy(model->mMtx, mtx.m);
    float sz = ColumnLength_unk(mtx, 2);
    float sy = ColumnLength_unk(mtx, 1);
    float sx = ColumnLength_unk(mtx, 0);
    atn::Vector3 s(sx, sy, sz);
    model->m54 = Max3_unk(s);
}

Gun::Gun(void* arg04, int arg08, int arg0C, int arg270) : Arms(arg04, arg08, arg0C, arg270) {
    GunOwner_unk* owner = (GunOwner_unk*)m270;
    if (owner->m48 == 0 || owner->m50 == 1) {
        m2A8.reset(new ((MemoryTag)3, "", "", 0) EffectModel_unk(0x505));
        fn_8008AD4C(m2A8->GetModel_unk(), "it090_00.model", 0);
    }
}

Gun::~Gun() {}

int Gun::vf0C() {
    int result = 0;
    if (Arms::vf0C()) {
        result = 1;
    }
    if (m2A8 && m2A8->GetModel_unk()->fn_8008B140()) {
        result = 1;
    }
    return result;
}

void Gun::vf18(const atn::Vector3& a, const atn::Vector3& b, int c, int d) {
    fn_80219E6C(d);
}

// NONMATCHING: 99.91% - the original loads the translation constants z first (pool order 3.7, 0.5,
// 0.0) but stores the vector x first; tried a named vector, z/y/x assignments, Set_unk, inline
// helpers by value/reference/components.
void Gun::vf14() {
    if (m284 != 3) {
        return;
    }
    int show = 1;
    int arrow = 0;
    if (CrossbowMenuTask::sInstance != 0) {
        CrossbowMenuTask* menu = CrossbowMenuTask::sInstance;
        switch (menu->m4C[menu->m150]) {
        case 0x70:
            arrow = 0;
            if (Inventory::instance_ != 0 && fn_804444E8(Inventory::instance_, 0x70) > 0) {
                arrow = 1;
            }
            show = 0;
            break;
        case 0x71:
            show = 0;
            break;
        case 0x6F:
            show = 0;
            break;
        }
    }
    GunOwner_unk* owner = (GunOwner_unk*)m270;
    if (owner->m55C == 6 && owner->m560 == 0x62) {
        arrow = 0;
        show = 0;
    }
    GetModel_unk()->fn_8009373C("op_01_arrow", show);
    if (arrow && m2A8 && m2A8->GetModel_unk()->m16C != 0) {
        atn::Matrix3x4 mtx(GetModel_unk()->mMtx);
        atn::Matrix3x4 r;
        atn::Matrix3x4 t;
        Trans_unk(t, atn::Vector3(0.0f, 0.5f, 3.7f));
        PSMTXConcat(mtx.m, t.m, r.m);
        mtx = r;
        SetMtx_unk(m2A8->GetModel_unk(), mtx);
        m2A8->GetModel_unk()->CalcMatrixStack(0, atn::ModelNodeCallback_unk());
        m2A8->GetModel_unk()->fn_8008CD60();
    }
    Arms::vf14();
}
