#include "game/Arms.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaTask.h"
#include "game/HitInfo_unk.h"
#include "std/string.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" unsigned long strlen(const char* s);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" atn::Matrix3x4 lbl_807C7060;
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* node); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, const char* node, float distance); // inferred: LOD    // inferred: identity matrix

extern "C" ArmsData_unk* fn_80206B14(void* arg, int item);     // inferred: equipment data
extern "C" ArmsEffect_unk* fn_802072AC(void* arg, int item);   // inferred: effect data of the equipment
extern "C" std::string fn_8006B2D8(const std::string& path);  // inferred: name without extension
extern "C" HitInfo_unk* fn_80219E6C(int id);                   // inferred: bullet/skill data by id

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);

// inferred: field object (class unknown)
struct Field_unk {
    unsigned char m0000[0x5590];
    void* m5590;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80375184(Field_unk* field); // inferred: saving is allowed
extern "C" void fn_80570A74(void* obj, atn::EfpFile* efp);

struct ArmsEffectMgr_unk;
extern "C" ArmsEffectMgr_unk* lbl_8087F048; // inferred: effect manager of the characters
extern "C" int fn_8010D770(ArmsEffectMgr_unk* mgr, HitInfo_unk* hit);


Arms::Arms(void* arg04, int arg08, int arg0C, int arg270)
    : m04(arg04), m08(arg08), m0C(arg0C), mModel(0x505), m224(0), m258(0.0f, 0.0f, 0.0f), m264(1.0f, 1.0f, 1.0f),
      m270(arg270), m278(-1), m284(3), m288(-1), m28C(0), m2A0(0), m2A4(0) {
    m228.SetIdentity();
    m274 = fn_80206B14(m04, m08);
    m2A4 = fn_802072AC(m04, m08);
    const char* name = m274->m8C;
    if (strlen(name) != 0) {
        std::string file = fn_8006B2D8(std::string(name));
        char path[0x100];
        sprintf(path, "/database/equipment/%s.chr", file.c_str());
        m27C.vf0C(path);
        m284 = 0;
    }
    for (int i = 0; i < 3; i++) {
        m290[i] = fn_80219E6C(m274->mAC[i]);
    }
}

Arms::~Arms() {
    fn_80044E0C(this, 0);
}

int Arms::vf0C() {
    switch (m284) {
    case 0:
        if (m27C.fn_80473F50()) {
            return 1;
        }
        m284 = 1;
    case 1:
        fn_8004439C();
        m27C.fn_80473F88();
        m284 = 2;
    case 2:
        if (GetModel_unk()->fn_8008B140()) {
            return 1;
        }
        if (m29C && m29C->IsLoading()) {
            return 1;
        }
        m278 = GetModel_unk()->GetNodeNoByName("shot", 0);
        m284 = 3;
    }
    return 0;
}

void Arms::fn_8004439C() {
    atn::CsvTab csv((const char*)m27C.fn_80470580(), m27C.fn_8047059C());
    char path[0x100];
    char node[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            strcpy(node, csv.fn_8005B9CC());
            if (strcmp(node, "") == 0) {
                fn_8008AD4C(GetModel_unk(), path, 0);
            } else {
                fn_8008AD4C(GetModel_unk(), path, node);
            }
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            if (strcmp(node, "") == 0) {
                fn_80092F1C(GetModel_unk(), path, 0, fn_800DC288(csv.fn_8005B9CC()));
            } else {
                fn_80092F1C(GetModel_unk(), path, node, fn_800DC288(csv.fn_8005B9CC()));
            }
        } else if (strcmp(field, "ROTATION") == 0) {
            m258.x = fn_800DC288(csv.fn_8005B9CC());
            m258.y = fn_800DC288(csv.fn_8005B9CC());
            m258.z = fn_800DC288(csv.fn_8005B9CC());
            m258.x = atn::DegToRad_unk(m258.x);
            m258.y = atn::DegToRad_unk(m258.y);
            m258.z = atn::DegToRad_unk(m258.z);
        } else if (strcmp(field, "SCALE") == 0) {
            m264.x = fn_800DC288(csv.fn_8005B9CC());
            m264.y = fn_800DC288(csv.fn_8005B9CC());
            m264.z = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            m29C.reset(new (1, "", "", 0) atn::EfpFile);
            m29C->LoadRequest(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
    if (!m29C && m2A4 != 0) {
        m29C.reset(new (1, "", "", 0) atn::EfpFile);
        char buf[0x100];
        sprintf(buf, "%s.efp", m2A4->m10);
        m29C->LoadRequest(buf);
    }
}

// inferred: *this = *this * rot(axis, rad) (as in game/CharaTask.cpp; name invented)
inline void Rotate_unk(atn::Matrix3x4& mtx, char axis, float rad) {
    Mtx t;
    Mtx r;
    PSMTXRotRad(r, axis, rad);
    PSMTXConcat(mtx.m, r, t);
    __MtxCopy(mtx.m, t);
}

// inferred: mtx = mtx * rot_y * rot_x * rot_z (names invented)
inline void RotateYXZ_unk(atn::Matrix3x4& mtx, const atn::Vector3& rot) {
    atn::Matrix3x4 t;
    atn::Matrix3x4 m;
    atn::Matrix3x4* p = &m;
    p->SetIdentity();
    if (0.0f != rot.y) {
        Rotate_unk(*p, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        Rotate_unk(*p, 'x', rot.x);
    }
    if (0.0f != rot.z) {
        Rotate_unk(*p, 'z', rot.z);
    }
    PSMTXConcat(mtx.m, p->m, t.m);
    mtx = t;
}

// inferred: mtx = mtx * scale(s) (name invented)
inline void Scale_unk(atn::Matrix3x4& mtx, const atn::Vector3& s) {
    atn::Matrix3x4 t;
    atn::Matrix3x4 m;
    PSMTXScale(m.m, s.x, s.y, s.z);
    PSMTXConcat(mtx.m, m.m, t.m);
    mtx = t;
}

// inferred: largest of three values (name invented)
inline float Max3_unk(const atn::Vector3& v) {
    float yz = v.y > v.z ? v.y : v.z;
    return v.x > yz ? v.x : (v.y > v.z ? v.y : v.z);
}

// NONMATCHING: 99.90% - stack slot of the scale vector `s` (the original keeps it below the three
// column temporaries); tried a temporary argument and an inline helper (both reorder the rotation
// blocks).
void Arms::vf10() {
    if (m284 != 3) {
        vf0C();
        return;
    }
    atn::Matrix3x4 mtx = m224 != 0 ? *(atn::Matrix3x4*)m224 : lbl_807C7060;
    atn::Matrix3x4 t;
    PSMTXConcat(mtx.m, m228.m, t.m);
    mtx = t;
    RotateYXZ_unk(mtx, m258);
    Scale_unk(mtx, m264);
    __MtxCopy(GetModel_unk()->mMtx, mtx.m);
    atn::Vector3 s(atn::Vector3(mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]).Length_unk(),
                   atn::Vector3(mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]).Length_unk(),
                   atn::Vector3(mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]).Length_unk());
    GetModel_unk()->m54 = Max3_unk(s);
    if (GetModel_unk()->m16C != 0) {
        GetModel_unk()->CalcMatrixStack(0, atn::ModelNodeCallback_unk());
    }
}

void Arms::vf14() {
    if (m284 == 3 && GetModel_unk()->m16C != 0) {
        GetModel_unk()->fn_8008CD60();
    }
}

atn::Vector3 Arms::fn_80044BD4() {
    if (m278 >= 0) {
        return ((atn::Matrix3x4*)GetModel_unk()->GetNodeMtx_unk(m278))->GetTrans_unk();
    }
    return ((atn::Matrix3x4*)GetModel_unk()->mMtx)->GetTrans_unk();
}

HitInfo_unk* Arms::fn_80044C30(int i) {
    if (i < 0) {
        return 0;
    }
    return m290[i];
}

int Arms::fn_80044C50(CharaTask* chara) {
    int ret = 0;
    if (m2A4 != 0) {
        ArmsCond_unk cond;
        cond.m08 = m2A4->m08;
        cond.m0C = m2A4->m0C;
        if (fn_80045510(&cond, chara)) {
            ret = 1;
        }
    } else {
        ret = 1;
    }
    return ret;
}

// inferred: plays the effect of the weapon on its model (name invented)
inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

void Arms::fn_80044CCC() {
    if (m29C && m2A0 == 0 && lbl_8087F3C0 != 0) {
        lbl_8087F3C0->mB8 = 2;
        fn_80232B7C(&mModel, 0);
        PlayEffect_unk(m29C.get(), GetModel_unk());
        lbl_8087F3C0->mB8 = 0;
        m2A0 = 1;
    }
}

// inferred: stops the effect
// (declared in game/CharaTask.h with a void* object)
extern "C" void fn_80044E0C(void* object, int arg) {
    Arms* self = (Arms*)object;
    if (self->m2A0 != 0 && lbl_8087F3C0 != 0) {
        lbl_8087F3C0->mB8 = 2;
        fn_80239DAC(lbl_8087F3C0, &self->mModel, 0, arg);
        lbl_8087F3C0->mB8 = 0;
        self->m2A0 = 0;
        if (lbl_8087F430 != 0 && lbl_8087F430->m5590 != 0) {
            fn_80570A74(lbl_8087F430->m5590, self->m29C.get());
        }
    }
}

int Arms::fn_80044E98(int i, int a, int b, int flags, CharaTask* user, CharaTask* chara, HitInfo_unk* hit) {
    if (m274 == 0) {
        return 0;
    }
    if (i < 0 || 2 <= i) {
        return 0;
    }
    if (chara != 0 && (chara->mFlag22 == 1 || (chara->mStatus.m18 & 1)) && m274->mC4[i].m00 == 0x10) {
        return 0;
    }
    return fn_80044FBC(&m274->mC4[i], a, b, flags, user, chara, hit) != 0;
}

int Arms::fn_80044F54(int i, CharaTask* chara) {
    if (m274 == 0) {
        return 0;
    }
    if (i < 0 || 2 <= i) {
        return 0;
    }
    return fn_80045510(&m274->mC4[i], chara) != 0;
}

int Arms::fn_80044FBC(ArmsCond_unk* cond, int a, int b, unsigned int flags, CharaTask* user,
                      CharaTask* chara, HitInfo_unk* hit) {
    if (user == 0 || chara == 0) {
        return 0;
    }
    int effect = lbl_8087F048 != 0 ? fn_8010D770(lbl_8087F048, hit) : 0;
    int state = user->m55C;
    int sub = user->m560;
    switch (cond->m08) {
    case 2:
        if ((user->fn_8014FCC4() && (a == 0 || effect != 0)) || (user->fn_8014FCE8() && (a == 1 || a == 2))) {
            return 1;
        }
        if (state == 6 && (sub == 11 || (sub >= 75 && sub <= 81))) {
            return 1;
        }
        if (hit != 0 && hit->m04 == 0x4C0) {
            return 1;
        }
        break;
    case 3:
        if (!user->fn_8014FCE8() && a == 2) {
            if (state == 6 && ((sub >= 75 && sub <= 81) || sub == 11)) {
                break;
            }
            if (hit != 0 && hit->m04 == 0x4C0) {
                break;
            }
            return 1;
        }
        break;
    case 4:
        if (cond->m0C > 0 && cond->m0C == (signed char)chara->m5C->m120[0]) {
            return 1;
        }
        break;
    case 5: {
        ArmsCond_unk cond3;
        ArmsCond_unk cond23;
        cond3.m08 = 3;
        cond23.m08 = 23;
        if (fn_80044FBC(&cond3, a, b, flags, user, chara, hit)) {
            break;
        }
        if (fn_80044FBC(&cond23, a, b, flags, user, chara, hit)) {
            break;
        }
        int v = cond->m0C;
        if (v > 0 && v == chara->m5C->m122) {
            return 1;
        }
        if ((v == 2 && chara->m5C->m122 == 3) || (v == 3 && chara->m5C->m122 == 2)) {
            return 1;
        }
        break;
    }
    case 6: {
        ArmsCond_unk cond3;
        ArmsCond_unk cond23;
        cond3.m08 = 3;
        cond23.m08 = 23;
        if (fn_80044FBC(&cond3, a, b, flags, user, chara, hit)) {
            break;
        }
        if (fn_80044FBC(&cond23, a, b, flags, user, chara, hit)) {
            break;
        }
        if (cond->m0C > 0) {
            if (cond->m0C == (signed char)chara->m5C->m120[1]) {
                return 1;
            }
            if (cond->m0C == 6 && (signed char)chara->m5C->m120[1] == 8) {
                return 1;
            }
        }
        break;
    }
    case 7:
        switch (cond->m0C) {
        case 1:
            if (chara->m55C == 6 && chara->m560 == 29) {
                return 1;
            }
            break;
        case 2:
            if (chara->mFlag5) {
                return 1;
            }
            break;
        }
        break;
    case 8:
        if (chara->mFE4 > 0) {
            CharaTask* last;
            if ((last = chara->mFDC) != user) {
                if (cond->m0C == 0) {
                    return 1;
                }
                if (last != 0 && last->fn_8014FCC4() &&
                    cond->m0C == ((Arms*)last->m648)->m274->m80 - 300) {
                    return 1;
                }
            }
        }
        break;
    case 11:
        if (state == 6 && (sub == 6 || sub == 7)) {
            return 1;
        }
        break;
    case 12:
        if (state == 6 && sub == 10) {
            return 1;
        }
        break;
    case 14:
        if ((signed char)chara->m5C->m123[0] == 1) {
            return 1;
        }
        break;
    case 15:
        switch (cond->m0C) {
        case 1:
            if (chara->mStatus.IsState_unk(1)) {
                return 1;
            }
            break;
        case 2:
            if (chara->mStatus.IsState_unk(0x40)) {
                return 1;
            }
            break;
        }
        break;
    case 16:
        if ((flags & 0x10) == 0x10) {
            return 1;
        }
        break;
    case 17:
        if ((flags & 0x80000000) == 0x80000000) {
            return 1;
        }
        break;
    case 19:
        return 1;
    case 20:
        if (cond->m0C == chara->fn_80179A34()) {
            return 1;
        }
        break;
    case 21:
        if (state == 6 && sub == 11) {
            return 1;
        }
        break;
    case 23:
        if (hit != 0 && ((hit->m04 >= 0x1FA && hit->m04 <= 0x1FC) || (hit->m04 >= 0x204 && hit->m04 <= 0x206))) {
            return 1;
        }
        break;
    case 24:
        if (state == 6 && sub == 14 && user->mMotion.mTracks[0].m00 == 349) {
            return 1;
        }
        break;
    case 26:
        return 1;
    }
    return 0;
}

int Arms::fn_80045510(ArmsCond_unk* cond, CharaTask* chara) {
    if (chara == 0) {
        return 0;
    }
    switch (cond->m08) {
    case 1:
        return 1;
    case 9:
        if (chara->mFlag5) {
            return 1;
        }
        break;
    case 10:
        if (chara->mStatus.m228 >= 1.0f || chara->mStatus.HasTimer_unk()) {
            return 1;
        }
        break;
    case 11:
        if (chara->m55C == 6 && chara->m560 == 6) {
            return 1;
        }
        break;
    case 12:
        if (chara->m55C == 6 && chara->m560 == 10) {
            return 1;
        }
        break;
    case 18:
        if (chara->mFlag28) {
            return 1;
        }
        break;
    case 21:
        if (chara->m55C == 6 && chara->m560 == 11) {
            return 1;
        }
        break;
    case 22:
        if (lbl_8087F430 == 0 || fn_80375184(lbl_8087F430)) {
            return 1;
        }
        break;
    case 25:
        for (unsigned int i = 0; i < chara->m12D0.mCount; i++) {
            CharaHit_unk& h = chara->m12D0.mData[i];
            HitInfo_unk* hit = h.mHit;
            if (h.m04 != 0) {
                hit = (HitInfo_unk*)h.m04;
            }
            if (hit != 0 && hit->m9C >= 50) {
                return 1;
            }
        }
        break;
    }
    return 0;
}

ArmsCond_unk* Arms::fn_80045694(int kind, int value, int kind2, int value2) {
    ArmsCond_unk* found = 0;
    if (kind >= 0) {
        found = 0;
        bool ok = false;
        for (int i = 0; i < 2; i++) {
            ArmsCond_unk* c = &m274->mC4[i];
            if (c->m08 == kind && (value < 0 || c->m0C == value)) {
                found = c;
                ok = true;
                break;
            }
        }
        if (!ok) {
            return 0;
        }
    }
    if (kind2 >= 0) {
        found = 0;
        bool ok = false;
        for (int i = 0; i < 2; i++) {
            ArmsCond_unk* c = &m274->mC4[i];
            if (c->m00 == kind2 && (value2 < 0 || c->m04 == value2)) {
                found = c;
                ok = true;
                break;
            }
        }
        if (!ok) {
            return 0;
        }
    }
    return found;
}

void Arms::vf18(const atn::Vector3& a, const atn::Vector3& b, int c, int d) {}
