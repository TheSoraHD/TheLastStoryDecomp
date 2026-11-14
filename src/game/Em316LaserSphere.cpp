#include "game/Em316LaserSphere.h"
#include "atn/Csv.h"
#include "atn/Material_unk.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/HitInfo_unk.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "math/VecAssign_unk.h"
#include "chr/ControlBase.h"
#include "game/BattleCharaTask.h"
#include "std/string.h"
#include "std/vector.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
// inferred: debug menu entries (as in game/CharaTask.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80094D88(atn::Model* model, int blend, int a, int b, int c, int d);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int
// inferred: control of a character caught by the tackle (only m4C, set to end it); name invented
struct LaserCatch_unk {
    unsigned char m00[0x4C];
    int m4C;
};
// inferred: accessors of the caught character and its control (weak copies 0x80318F90, 0x80319998)
extern "C" inline chr::ControlBase* fn_80318F90(CharaTask* chara) {
    return chara->mControl.get();
}
extern "C" inline void fn_80319998(LaserCatch_unk* control) {
    control->m4C = 1;
}
// inferred: game state (lbl_8087EFA8, see game/CharaTask.cpp): time of a frame at 0x3A4
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" GameState_unk* lbl_8087EFA8;
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: sound effects of the sphere (laser, laser end, ?, death); name invented
static const char* const sSounds_unk[] = {"SE_EM601_006", "SE_EM601_002", "SE_EM601_006", "SE_EM601_004"};

// inferred: alpha range of the trail models (.sdata 0x8087DC48/0x8087DC4C); names invented
static float sOne_unk = 1.0f;
static float sZero_unk = 0.0f;
static int sFirstCount_unk = 1; // inferred: count of a new group of spheres (fn_803176D0)

// inferred: shared by all the spheres: set once the targets have been shared this frame (lbl_8087F3F4)
// and the last angle (lbl_8087F3F0); names invented
static float sAngle_unk = atn::DegToRad_unk(70.0f);
static int sShared_unk;

// inferred: translation of a matrix (name invented)
inline atn::Vector3 GetTrans_unk(const Mtx m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: linear interpolation with the offset added last (atn::Lerp_unk adds `a` first); name invented
inline atn::Vector3 LaserLerp_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: the three components are zero (written with branches, not as a bool inline); name invented
inline int IsZero3_unk(const atn::Vector3& v) {
    int zero = 0;
    if (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f) {
        zero = 1;
    }
    return zero;
}

// inferred: color as four floats (as in game/Em315Coffin.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);

// inferred: plays `efp` on `model` with default placement (as CoffinEffect_unk of game/Em315Coffin.cpp)
inline void LaserEffect_unk(atn::EfpFile* efp, atn::Model* model) {
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

inline void Em316LaserSphere::SetAppear_unk(const atn::Vector3& pos, const atn::Vector3& offset) {
    m14D8 = pos + offset;
    m14E4 = pos + offset;
    mPos = m14E4;
}

inline void Em316LaserSphere::SetState_unk(int state) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}

// inferred: the tests of fn_80316F74 (bits 3 and 9 of the status flags); name invented
// inferred: asserts of the debug build, empty in release: they only count for the inline budget (with them
// vf10 goes over it and calls fn_80316E38, fn_80316F74, fn_80316F9C... out of line, as the original)
#define SPHERE_ASSERT_UNK(x) ((void)0)
extern "C" inline int fn_80316F74(CharaStatus* status) {
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0); SPHERE_ASSERT_UNK(status != 0);
    return (status->m0C & 8) == 8 || (status->m0C & 0x200) == 0x200;
}

// NONMATCHING: the tokenizer loop (same open case as LastCocoonChrysalis, docs/inlining.md)
Em316LaserSphere::Em316LaserSphere(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m14CC(1), m14D0(-1),
      m14D4(0), m14D8(0.0f, 0.0f, 0.0f), m14E4(0.0f, 0.0f, 0.0f), m14F0(0.25f), m14F4(0.0f, 0.0f, 0.0f),
      m1500(25.0f), m1520(0), m1524(0), m153C(0), m1540(0.0f, 0.0f, 0.0f), m155C(0.0f, 0.0f, 0.0f),
      m1568(0.0f, 0.0f, 0.0f), m1574(60), m1578(0), m15B4(0), m1C84(60), m1C88(15), m1C90(0), m1C94(0),
      m1C98(4), m1C9C(8) {
    std::string dir("database/chara/special/");
    std::string name("db_em601_00_00.csv");
    std::string params(info->m2C);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(params, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp(it->c_str(), "db_name=", 8) == 0) {
            name = it->c_str() + 8;
        } else if (strncmp(it->c_str(), "target=", 7) == 0) {
            m14D0 = fn_800DC12C(it->c_str() + 7);
        }
    }
    std::string path = fn_8006AFF8(dir, name);
    m14B0.vf0C(path.c_str());
    m1550.LoadRequest("ef_uc066.efp");
    m1584.LoadRequest("ef_sk043c_01.eff");
    m1590.LoadRequest("ef_sk043a.efp");
    m159C.LoadRequest("ef_sk043g_01.eff");
    m15A8.LoadRequest("ef_sk043g_02.eff");
    for (int i = 0; i < 3; i++) {
        ((atn::Model*)&m1648[i])->fn_8008AD4C("em316_00_bs00.model", 0);
    }
    mFlag21 = 1;
}

// NONMATCHING: 99.49% - in the last loop the original steps `this` itself (0x164C(r30)) for the model
// flags and keeps the counter in r27; ~12 variants (pointer, index, increments, locals)
int Em316LaserSphere::vf0C() {
    atn::ModelBase* models = m1648;
    for (int i = 0; i < 3; i++) {
        if (models[i].GetModel_unk()->fn_8008B140() != 0) {
            return 0;
        }
    }
    if (fn_8013655C() == 0 && m14B0.fn_80473F50() == 0 && m1550.IsLoading() == 0 &&
        m1584.IsLoading() == 0 && m1590.IsLoading() == 0 && m159C.IsLoading() == 0 &&
        m15A8.IsLoading() == 0) {
        void* group = m1C8C.Load("Bit");
        fn_80087994(group, "DebugDisp", &m1C90, 0, 0);
        fn_800874C8(group, "LaserWait", &m1C84, 1, 10000, 1, 0, 0);
        fn_800874C8(group, "MoveWait", &m1C88, 1, 10000, 1, 0, 0);
        fn_80087994(group, "DebugLog", &m1C94, 0, 0);
        fn_8008771C(group, "AppearMoveRate", &m14F0, 0, 0, 0.01f, 0.99f, 0.1f);
        fn_800874C8(group, "ActionMode", &m58C, 1, 100, 1, 0, 0);
        fn_80087E9C(group, "Pos", &mPos, 0, 0, 1.0f, 100.0f, 1.0f);
        if (m14B0.fn_80470580() != 0) {
            atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "LaserID") == 0) {
                    m157C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "TackleID") == 0) {
                    m1580 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "MoveWait") == 0) {
                    m1C88 = atoi(csv.fn_8005B9CC());
                } else if (strcmp(key, "LaserWait") == 0) {
                    m1C84 = atoi(csv.fn_8005B9CC());
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x2880000;
        if (m14D0 <= 0) {
            m14D4 = lbl_8087F8A0->mHead;
        } else {
            m14D4 = lbl_8087F8A0->Find_unk(m14D0);
        }
        m54C |= 0x200;
        for (int i = 0; i < 3; i++) {
            atn::Model* model = models[i].GetModel_unk();
            fn_80094D88(model, 3, 0, 0, 0, 0);
            float t = (float)(i + 1) / 4.0f;
            float a = sOne_unk + t * (sZero_unk - sOne_unk);
            atn::Model::Float4_unk color;
            color.m00 = 1.0f;
            color.m04 = 1.0f;
            color.m08 = 1.0f;
            color.m0C = a;
            model->m40 = color;
            models[i].GetModel_unk()->mFlags |= 0x10;
        }
        return 1;
    }
    return 0;
}

// inferred: matrix of a model (weak copy 0x80266154, first emitted by game/Em201ZanGurgFinal; as in
// gmk/Cannonball.cpp)
extern "C" inline atn::Matrix3x4* fn_80266154(atn::Model* model) {
    return (atn::Matrix3x4*)model->mMtx.m;
}
// inferred: largest of three values (as in game/Gun.cpp)
inline float LaserMax3_unk(const atn::Vector3& v) {
    float yz = v.y > v.z ? v.y : v.z;
    return v.x > yz ? v.x : (v.y > v.z ? v.y : v.z);
}
inline float LaserColumn_unk(const atn::Matrix3x4& m, int i) {
    atn::Vector3 v(m.m[0][i], m.m[1][i], m.m[2][i]);
    return PSVECMag(&v);
}
// inferred: sets the world matrix of a model and its largest scale axis (weak copy 0x80316E38, called by
// gmk/Cannonball.cpp and gmk/FlamingArrow.cpp; as SetMtx_unk of game/Gun.cpp)
extern "C" inline void fn_80316E38(atn::Model* model, const atn::Matrix3x4* mtx) {
    __MtxCopy(model->mMtx.m, mtx->m);
    float sz = LaserColumn_unk(*mtx, 2);
    float sy = LaserColumn_unk(*mtx, 1);
    float sx = LaserColumn_unk(*mtx, 0);
    atn::Vector3 s(sx, sy, sz);
    model->m54 = LaserMax3_unk(s);
}
// inferred: color of a material (weak copy 0x8000DD98 in Event/Puppeteer_CharaTask)
extern "C" inline GXColor fn_8000DD98(Material_unk* material) {
    return material->mColor;
}
// inferred: color as four floats (as in game/Em315Coffin.cpp; constructor from GXColor: weak copy 0x8000DD14
// in Event/Puppeteer_CharaTask)
struct ColorF_unk {
    ColorF_unk() {}
    GXColor ToGX() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};
extern "C" inline void fn_8000DD14(ColorF_unk* c, const GXColor* g) {
    c->r = g->r / 255.0f;
    c->g = g->g / 255.0f;
    c->b = g->b / 255.0f;
    c->a = g->a / 255.0f;
}
// inferred: sets the color of a material (weak copy 0x80316F9C)
extern "C" inline void fn_80316F9C(Material_unk* material, const GXColor* c) {
    material->mColor.r = c->r;
    material->mColor.g = c->g;
    material->mColor.b = c->b;
    material->mColor.a = c->a;
}
inline float LaserMaxF_unk(float a, float b) {
    return a > b ? a : b;
}
inline float LaserMinF_unk(float a, float b) {
    return a < b ? a : b;
}

// NONMATCHING: 79.5% - over the inline budget as the original now (empty asserts in fn_80316F74), but the
// original calls GetModel_unk once and builds another boost::function. Before: it calls fn_80266154, fn_80316E38, fn_80316F74,
// fn_80316F9C, fn_80316FC0, fn_8000DD14, fn_8000DD98 and the boost::function constructor out of line, while
// here everything is inlined (state functions of the switch included); written to place its strings and
// floats in the pools
void Em316LaserSphere::vf10() {
    atn::ModelBase* models = m1648;
    for (int i = 2; i >= 0; i--) {
        atn::Matrix3x4* m;
        if (i == 0) {
            m = fn_80266154(&mMotion);
        } else {
            m = fn_80266154(m1648[i - 1].GetModel_unk());
        }
        fn_80316E38(models[i].GetModel_unk(), m);
        models[i].GetModel_unk()->CalcMatrixStack(0, atn::ModelNodeCallback_unk(0));
    }
    fn_80317EA0();
    m14C0++;
    m1574--;
    m1520++;
    if (mC58.mC0 == 0 || m14B8 == 0) {
        fn_8016E970(3);
    } else {
        if (m14B8 != 0) {
            switch (m58C) {
            case 7:
            case 12:
                fn_80319384();
                break;
            case 2:
                vfEC();
                break;
            case 6:
                fn_803191FC();
                break;
            case 10:
                fn_803199A4();
                break;
            case 8:
                fn_80319578();
                break;
            case 9:
                fn_8031958C();
                break;
            case 11:
                fn_80318F9C();
                break;
            default:
                fn_80318F98();
                fn_80317EF8();
                break;
            }
        }
        if (m58C != 2 && m58C != 11 && m58C != 12) {
            fn_80318A30();
        }
        if (sShared_unk == 0) {
            fn_803176D0();
            sShared_unk = 1;
        }
        if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000) || mStatus.IsState_unk(0x1000000)) {
            Material_unk* material = mMotion.fn_80092954("em316_light");
            GXColor g = fn_8000DD98(material);
            ColorF_unk c;
            fn_8000DD14(&c, &g);
            c.r = LaserMaxF_unk(0.2f, c.r - 0.1f);
            c.g = LaserMaxF_unk(0.2f, c.g - 0.1f);
            c.b = LaserMaxF_unk(0.2f, c.b - 0.1f);
            GXColor out = c.ToGX();
            fn_80316F9C(material, &out);
        } else {
            Material_unk* material = mMotion.fn_80092954("em316_light");
            GXColor g = fn_8000DD98(material);
            ColorF_unk c;
            fn_8000DD14(&c, &g);
            c.r = LaserMinF_unk(0.6f, 0.1f + c.r);
            c.g = LaserMinF_unk(0.6f, 0.1f + c.g);
            c.b = LaserMinF_unk(0.6f, 0.1f + c.b);
            GXColor out = c.ToGX();
            fn_80316F9C(material, &out);
        }
        for (int i = 2; i >= 0; i--) {
            Material_unk* dst = models[i].GetModel_unk()->fn_80092954("em316_light");
            GXColor col = fn_8000DD98(mMotion.fn_80092954("em316_light"));
            fn_80316F9C(dst, &col);
        }
    }
    fn_80318720();
    fn_8014C540();
    fn_80145334();
    fn_80318594();
}

void Em316LaserSphere::vf14() {
    sShared_unk = 0;
    fn_80149A30();
    for (int i = 0; i < 3; i++) {
        atn::Vector3 d = GetTrans_unk(mMotion.mMtx.m) - GetTrans_unk(m1648[i].GetModel_unk()->mMtx.m);
        if (PSVECSquareMag(&d) > 25.0f) {
            m1648[i].GetModel_unk()->fn_8008CD60();
        }
    }
}

void Em316LaserSphere::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m10 *= 0.0f;
    fn_80151448(msg);
    vf40(hit);
}

// NONMATCHING: 99.16% - stack slots of the inlined fn_80318648 (pos/translation swapped) and the offset of
// "spine1" in the pool; ~12 variants (locals, chained copies, accessors)
void Em316LaserSphere::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_8031A484();
        return;
    }
    HitInfo_unk* data = ((HitMsg_unk*)hit)->mData;
    if (data != 0 && (data->m90.mFlags & 0x800000) == 0x800000) {
        SetState_unk(12);
        atn::Vector3 pos = fn_80318648(m14B8);
        m1510 = m1504 = mPos;
        if (m1510.y > pos.y) {
            m1510.y = pos.y;
        }
        m151C = 0.0f;
        return;
    }
    fn_80319C10();
}

// NONMATCHING: 99.80% - only the offset of "spine1" in @stringBase0 (strings of vf10 not written yet)
// NONMATCHING: 99.69% - stack layout: the original keeps the two sums of the inline at the bottom of the
// frame and the locals of the effect above the rotation; ~12 variants
void Em316LaserSphere::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    atn::Vector3 epos;
    atn::Vector3 erot;
    Color4f_unk color;
    int zero = 0;
    SetState_unk(6);
    SetAppear_unk(pos, offset);
    if (offset.x == 0.0f && offset.y == 0.0f && offset.z == 0.0f) {
        zero = 1;
    }
    if (!zero) {
        atn::Vector3 rot(offset);
        rot.fn_80139F60();
        mRot = rot;
    }
    erot.x = 0.0f;
    erot.y = 0.0f;
    erot.z = 0.0f;
    epos.x = 0.0f;
    epos.y = 0.0f;
    epos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, &m1550, &mMotion, 0, epos, erot, color, -1, -1, 1, 1.0f);
    fn_800C31F4(sSounds_unk[0], 0, -1, 1.0f);
}

// inferred: target of a sphere for the sharing (the leader when mFlag28, else the default target); name invented
inline CharaTask* SharedTarget_unk(Em316LaserSphere* sphere) {
    CharaTask* leader = lbl_8087F8A0->mHead;
    if (leader->mFlag28) {
        return leader;
    }
    return sphere->m14D4;
}

// inferred: the character is active, free and not grappled (as IsTarget2_unk of game/Em201ZanGurgFinal.cpp)
inline bool SphereCanAct_unk(CharaTask* c) {
    bool active;
    bool free;
    bool r;
    r = false;
    active = false;
    free = false;
    if ((c->mFlags & 4) != 4 && (c->mFlags & 1) != 1) {
        free = true;
    }
    if (free && (c->mStatus.m0C & 0x20) != 0x20) {
        active = true;
    }
    if (active) {
        bool grappled = false;
        if (c->m55C == 6 && c->m560 == 28) {
            grappled = true;
        }
        if (!grappled) {
            r = true;
        }
    }
    return r;
}

// inferred: shares the targets between the laser spheres (called once per frame): sorts the active spheres
// by the id of their target and gives each one its index (m14C8) and the size of its group (m14CC)
// NONMATCHING: 97.7% - the original tests the leader flag once for both targets of the sort, and the destructors
// of the two vectors test the vector once more (std/vector.h)
void Em316LaserSphere::fn_803176D0() {
    std::vector<Em316LaserSphere*> spheres;
    for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
        if (!SphereCanAct_unk(e)) {
            continue;
        }
        if (e->m146C != 45 && e->m146C != 49) {
            continue;
        }
        Em316LaserSphere* sphere = (Em316LaserSphere*)e;
        bool inserted = false;
        for (Em316LaserSphere** it = spheres.begin(); it != spheres.end(); ++it) {
            int key = SharedTarget_unk(sphere)->m50;
            if (SharedTarget_unk(*it)->m50 >= key) {
                spheres.insert(it, sphere);
                inserted = true;
                break;
            }
        }
        if (!inserted) {
            spheres.push_back(sphere);
        }
    }
    std::vector<int> counts;
    int last = -1;
    int index = 0;
    for (unsigned int i = 0; i < spheres.size(); i++) {
        Em316LaserSphere* sphere = spheres[i];
        if (last != SharedTarget_unk(sphere)->m50) {
            sphere->m14C4 = 0;
            index = 0;
            sphere->m14C8 = 0;
            counts.push_back(sFirstCount_unk);
        } else {
            index++;
            counts[counts.size() - 1]++;
            sphere->m14C4 = 1;
            sphere->m14C8 = index;
        }
        last = SharedTarget_unk(sphere)->m50;
    }
    last = -1;
    int group = -1;
    for (unsigned int i = 0; i < spheres.size(); i++) {
        Em316LaserSphere* sphere = spheres[i];
        if (last != SharedTarget_unk(sphere)->m50) {
            group++;
            sphere->m14CC = counts[group];
        } else {
            sphere->m14CC = counts[group];
        }
        last = SharedTarget_unk(sphere)->m50;
    }
}

void Em316LaserSphere::fn_80317EA0() {
    CharaTask* leader = lbl_8087F8A0->mHead;
    if (leader->mFlag28) {
        mC58.mC8 = (int)mC58.mC4;
        m1454 = 0;
        m14B8 = leader;
        mC58.mC4 = leader;
    } else {
        m1454 = 1;
        m14B8 = mC58.mC4;
    }
    if (m14B8 == 0) {
        m14B8 = m14D4;
    }
}

// inferred: random value between `lo` and `hi` (name invented)
inline float LaserRand_unk(float lo, float hi) {
    return lo + (hi - lo) * ((float)rand() / 32767.0f);
}
// inferred: rotation matrix y, x, z (as ZesyaRotZYX_unk of game/Em101LittleZesya.cpp; name invented)
inline void LaserRotYXZ_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    __MtxRotate(m.m, 'y', rot.y);
    __MtxRotate(m.m, 'x', rot.x);
    __MtxRotate(m.m, 'z', rot.z);
}

// inferred: random place around `t` at m1500 (zero without a target); name invented
inline atn::Vector3 LaserMovePlace_unk(Em316LaserSphere* self, CharaTask* t) {
    if (t == 0) {
        return lbl_807C7030;
    }
    atn::Vector3 center = self->fn_80318648(t);
    atn::Vector3 ofs(0.0f, 0.0f, self->m1500 * LaserRand_unk(1.0f, 1.2f));
    atn::Vector3 rot(LaserRand_unk(0.0f, -0.7853982f), LaserRand_unk(0.0f, 6.2831855f), 0.0f);
    atn::Matrix3x4 m;
    LaserRotYXZ_unk(m, rot);
    PSMTXMultVec(m.m, &ofs, &ofs);
    return center + ofs;
}

// inferred: state 1: fires when the attack wait (m1574) is over (state 8), otherwise after a random wait moves to
// a random place around the target (state 7)
// NONMATCHING: 90.8% - the original keeps the zero and the random yaw in f31/f30 and one more saved GPR (the
// MoveWait read twice); 4 variants
void Em316LaserSphere::fn_80317EF8() {
    CharaTask* target = m14B8;
    if (target == 0) {
        return;
    }
    atn::Vector3 d = mPos - fn_80318648(target);
    PSVECMag(&d);
    if (m1574 < 0) {
        SetState_unk(8);
        fn_80232B7C(this, 0x3E8);
        LaserEffect_unk(&m1590, &mMotion);
        return;
    }
    int wait = m1C88 + rand() % m1C88;
    if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
        wait *= 3;
    }
    if (m1520 > wait) {
        SetState_unk(7);
        fn_80318648(m14B8);
        m1504 = mPos;
        atn::Vector3 to = LaserMovePlace_unk(this, m14B8);
        m1510 = to;
        m151C = 0.0f;
        fn_800C344C(sSounds_unk[2], &mPos, 0, -1, 1.0f);
    }
}

// NONMATCHING: 88.7% - order of the copies to the capsule (the original stores m3C.z through a frsp and the
// radius m620 last) and the stack slots of top/bottom; ~16 variants
void Em316LaserSphere::fn_80318594() {
    m620 = m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    atn::Vector3 bottom(mPos);
    atn::Vector3 top(mPos);
    top.y += 0.1f;
    mColli.m3C = bottom;
    mColli.m48 = top;
    mColli.m54 = m620;
}

atn::Vector3 Em316LaserSphere::fn_80318648(CharaTask* chara) {
    atn::Model* model = &chara->mMotion;
    MtxPtr m = model->GetNodeMtx_unk("spine1");
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    if (m != 0) {
        pos = ((atn::Matrix3x4*)m)->GetTrans_unk();
    } else {
        pos = chara->mPos;
        pos.y += 10.0f;
    }
    return pos;
}

// NONMATCHING: 96.4% - the original frame is 0x30 bytes larger and stores three 0.0f early (0x98, 0xC, 0x1C:
// more temporaries of a rotation inline not identified); ~5 variants
void Em316LaserSphere::fn_80318720() {
    CharaTask* target = m14B8;
    if (target != 0) {
        atn::Vector3 d = fn_80318648(target) - mPos;
        PSVECNormalize(&d, &d);
        mRot = d.ToRotation_unk();
    }
}

CharaTask* Em316LaserSphere::fn_80318A30() {
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        if (fn_80318AA4(p) != 0) {
            fn_8031A670(p);
            return p;
        }
    }
    return 0;
}

// inferred: the magic `data` of `chara` targets `target` (bullet manager); returns < 0 when not (as in
// game/Em403WarLion.cpp)
extern "C" int fn_800FBA9C(Effect_unk* mgr, CharaTask* chara, CharaTask* target, HitInfo_unk* data, int arg,
                           float f);
extern "C" int fn_80051BC4(const atn::ColliCapsuleShape_unk* a, const atn::ColliCapsuleShape_unk* b);
// inferred: control of a character that casts a magic (as in game/Em101ElderZesyaSecond.cpp)
struct LaserMagic_unk {
    unsigned char m00[0x1D];
    unsigned char m1D; // inferred: nonzero once the magic is fired
};

// inferred: `chara` aims at the sphere: the magic it holds (state 6/30) once fired, its magic or skill, or the
// line of its aimed shot crosses the capsule of the sphere
// NONMATCHING: 98.4% - the original frame is 0x10 bytes smaller (slots of the lines and of the rotation), the
// state test in r0 and the bullet manager loaded after the skill; 6 variants
int Em316LaserSphere::fn_80318AA4(CharaTask* chara) {
    if (chara->IsActive_unk()) {
        if (chara->m55C == 6 && chara->m560 == 30) {
            LaserMagic_unk* magic = (LaserMagic_unk*)fn_80318F90(chara);
            if (chara->mC58.mC4 == this && magic->m1D != 0) {
                atn::ColliCapsuleShape_unk line;
                line.mStart = chara->mPos;
                line.mStart.y += 20.0f;
                atn::VecAssign_unk(line.mEnd, line.mStart + ((chr::ControlBase*)magic)->vf18() * 200.0f);
                if (fn_80051BC4(&line, (atn::ColliCapsuleShape_unk*)&mColli.m3C)) {
                    return 1;
                }
            }
        } else if (chara->vf78() != 0 || chara->fn_80158CA4() != 0) {
            if (chara->m55C == 6 && chara->m560 == 30) {
                if (chara->mC58.mC4 == this) {
                    return 1;
                }
            } else {
                if (fn_800FBA9C(lbl_8087F048, chara, this, chara->m638, -1, 0.0f) >= 0) {
                    return 1;
                }
                float rot = chara->fn_80158E1C().ToRotation_unk().y;
                if (fn_800FBA9C(lbl_8087F048, chara, this, chara->m638, -1,
                                atn::WrapAngle_unk(rot - chara->mRot.y)) >= 0) {
                    return 1;
                }
            }
        } else if (chara->mFlag6 || (chara->mFlag31 && chara->mC48 != 0)) {
            atn::Vector3 from;
            atn::Vector3 dir;
            ((BattleCharaTask*)chara)->fn_800E932C(&from, &dir, 1);
            atn::ColliCapsuleShape_unk line;
            line.mStart = from;
            atn::VecAssign_unk(line.mEnd, from + dir * 200.0f);
            if (fn_80051BC4(&line, (atn::ColliCapsuleShape_unk*)&mColli.m3C)) {
                return 1;
            }
        }
    }
    return 0;
}

void Em316LaserSphere::fn_80318F98() {}

void Em316LaserSphere::fn_80318F9C() {
    if (m14C0 > 90) {
        fn_80319C10();
        int wait = m1C84;
        m1574 = wait + rand() % wait;
        if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
            m1574 *= 2;
        }
    }
}

extern "C" void* lbl_8087EE98; // inferred: collision world
// inferred: collision result (as in game/Em003LesserDaemon.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x34 - 0x1C];
    void* m34;
    void* m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" int fn_8004D388(void* world, CollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg7, float radius);

// NONMATCHING: 94.2% - the mask 0x80000080 is built in r0 then or-ed into r7 in the original, the 0.5f is
// loaded after the fabs, and r3/r4 of the copy of the zero vector (__VecCopy family); ~8 variants
void Em316LaserSphere::vfEC() {
    if (m14C0 >= 30) {
        fn_80239DAC(lbl_8087F3C0, this, 1001, 0);
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        mFlag15 = 1;
        fn_801765D8();
    } else {
        CollResult_unk result;
        Sphere_unk sphere = fn_80176548(mPos);
        unsigned int mask = 0x80000000;
        int hit = fn_8004D388(lbl_8087EE98, &result, &sphere, &m1540, mask | 0x80, &mColli, 0, sphere.mRadius);
        m1540.y += m154C;
        if (hit != 0) {
            m1540 *= -0.3f;
            if (mPos.y < result.m10.y) {
                mPos.y = result.m10.y;
                if (atn::Abs_unk(m1540.y) < 0.5f) {
                    m1540 = lbl_807C7030;
                }
            }
        }
    }
    mPos += m1540;
}

void Em316LaserSphere::fn_803191FC() {
    if (m14C0 >= 20) {
        mPos = m14E4;
        m1578 = 0;
        int wait = m1C84;
        m1574 = wait + rand() % wait;
        if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
            m1574 *= 2;
        }
        fn_80319C10();
    } else {
        mPos = LaserLerp_unk(mPos, m14E4, m14F0);
    }
}

// NONMATCHING: 99.67% - stack slots of the lerp temporaries (difference above the scaled vector) and f4/f5
// of the rate; ~15 variants
void Em316LaserSphere::fn_80319384() {
    float rate = 0.34f;
    if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
        rate *= 0.1f;
    }
    if (mStatus.IsState_unk(0x1000000)) {
        rate *= 0.01f;
    }
    m151C = lbl_8087EFA8->m3A4 * rate + m151C;
    float t = m151C < 1.0f ? m151C : 1.0f;
    atn::Vector3 d = m1510 - m1504;
    atn::Vector3 s = d * t;
    atn::Vector3 r = s + m1504;
    mPos = r;
    if (IsMoveDone_unk()) {
        if (m58C == 12) {
            SetState_unk(11);
            fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
        } else {
            fn_80319C10();
        }
        m1520 = 0;
    }
}

void Em316LaserSphere::fn_80319578() {
    if (m14C0 > 5) {
        fn_80319C90();
    }
}

// inferred: state and sub-state of a character (weak copies 0x80267B20/0x80267B28 of another file, as in
// game/GameUI.cpp)
// inferred: asserts of the debug build, empty in release: with them fn_8031958C goes over the inline budget
// and calls everything out of line as the original (see SPHERE_ASSERT_UNK)
#define SPHERE_ASSERT10_UNK(x)                                                                                 \
    SPHERE_ASSERT_UNK(x); SPHERE_ASSERT_UNK(x); SPHERE_ASSERT_UNK(x); SPHERE_ASSERT_UNK(x); SPHERE_ASSERT_UNK(x); \
    SPHERE_ASSERT_UNK(x); SPHERE_ASSERT_UNK(x); SPHERE_ASSERT_UNK(x); SPHERE_ASSERT_UNK(x); SPHERE_ASSERT_UNK(x)
#define SPHERE_ASSERT100_UNK(x)                                                                                \
    SPHERE_ASSERT10_UNK(x); SPHERE_ASSERT10_UNK(x); SPHERE_ASSERT10_UNK(x); SPHERE_ASSERT10_UNK(x);              \
    SPHERE_ASSERT10_UNK(x); SPHERE_ASSERT10_UNK(x); SPHERE_ASSERT10_UNK(x); SPHERE_ASSERT10_UNK(x);              \
    SPHERE_ASSERT10_UNK(x); SPHERE_ASSERT10_UNK(x)
extern "C" inline int fn_80267B20(CharaTask* chara) {
    SPHERE_ASSERT100_UNK(chara != 0);
    SPHERE_ASSERT100_UNK(chara != 0);
    SPHERE_ASSERT100_UNK(chara != 0);
    return chara->m55C;
}
extern "C" inline int fn_80267B28(CharaTask* chara) {
    return chara->m560;
}
// inferred: weak copies 0x800F7FA0 and 0x800F7FA8 of another file (as in game/Em203GreaterDaemon.cpp)
extern "C" inline EffectMgr_unk* fn_800F7FA0() {
    return lbl_8087F3C0;
}
extern "C" inline void fn_800F7FA8(EffectMgr_unk* mgr, void* owner, int arg) {
    fn_80232B7C(owner, arg);
}
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_8023A680(EffectMgr_unk* mgr, void* res, int a, int b, void* mtx, const atn::Vector3* pos,
                            const atn::Vector3* rot, const float* color, float scale, int e, int f, int g);
// inferred: bullet manager and collision world (weak copies 0x8013A194/0x801404F8 of another file)
inline Effect_unk* fn_8013A194() {
    return lbl_8087F048;
}
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, void* param, int flags, int arg7, float arg8, float arg9);
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
// inferred: translation matrix by value, rotation and scale (weak copies of another file, as in
// game/BattleTask.cpp)
extern "C" inline atn::Matrix3x4 fn_800F80A8(const atn::Vector3& pos) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, pos.x, pos.y, pos.z);
    return m;
}
extern "C" inline void fn_800F80B8(MtxPtr mtx, const atn::Vector3* rot) {
    ((atn::Matrix3x4*)mtx)->RotateZYX_unk(*rot);
}
extern "C" inline void fn_800F8290(MtxPtr mtx, float x, float y, float z) {
    Mtx s;
    PSMTXScale(s, x, y, z);
    Mtx t;
    PSMTXConcat(mtx, s, t);
    __MtxCopy(mtx, t);
}

// inferred: state 9 (tackle): the laser follows the caught character (or goes straight until it hits a wall)
// for 90 frames; over the inline budget, every inline is called out of line as in the original
void Em316LaserSphere::fn_8031958C() {
    if (m14C0 > 90) {
        fn_80319C10();
        if (fn_80267B20(m15B4) == 6 && fn_80267B28(m15B4) == 0x8C) {
            fn_80319998((LaserCatch_unk*)fn_80318F90(m15B4));
            m15B4 = 0;
        }
        fn_80239DAC(fn_800F7FA0(), this, 1000, 1);
        fn_80239DAC(fn_800F7FA0(), this, 1001, 1);
        return;
    }
    atn::Vector3 target;
    if (fn_80267B20(m15B4) == 6 && fn_80267B28(m15B4) == 0x8C) {
        target = *m15B4->fn_8013C38C();
        target.y += 13.0f;
        target += m15B4->fn_8014052C() * 4.0f;
        m155C = target - (mPos + m155C * m5B0);
        float len = m155C.Length_unk() / 50.0f;
        m155C.Normalize_unk();
        atn::Vector3 rot = m155C.ToRotation_unk();
        m15B8 = fn_800F80A8(mPos + m155C * m5B0);
        fn_800F80B8(m15B8.m, &rot);
        m15E8 = m15B8;
        fn_800F8290(m15B8.m, 1.0f, 1.0f, len);
    } else {
        m14C0 = m14C0 < 60 ? 60 : m14C0;
        if (m14C0 % 10 == 0) {
            fn_800F8574(fn_8013A194(), this, m157C, &mPos, &m155C, 0, 0, 0, 0.0f, 1.0f);
        }
        CollResult_unk result;
        atn::Vector3 from(mPos);
        atn::Vector3 to = from + m155C * 1000.0f;
        if (fn_8004ED34(fn_801404F8(), &result, &from, &to, 0x80000004, 0, 0) != 0) {
            target = result.m04;
        } else {
            target = to;
        }
    }
    m155C = target - (mPos + m155C * m5B0);
    float len = m155C.Length_unk() / 50.0f;
    m155C.Normalize_unk();
    atn::Vector3 rot = m155C.ToRotation_unk();
    m15B8 = fn_800F80A8(mPos + m155C * m5B0);
    fn_800F80B8(m15B8.m, &rot);
    m15E8 = m15B8;
    fn_800F8290(m15B8.m, 1.0f, 1.0f, len);
    m1618 = fn_800F80A8(target);
    if (fn_80239D58(fn_800F7FA0(), this, 1001) == 0) {
        fn_800F7FA8(fn_800F7FA0(), this, 1001);
        fn_8023A680(fn_800F7FA0(), &m1584, -1, 5, &m1618, 0, 0, 0, 1.0f, 0, -1, 1);
    }
}

// NONMATCHING: 99.90% - only the offset of "spine1" in @stringBase0 and of 0.35f in .sdata2 (functions of the
// unit not written yet)
void Em316LaserSphere::fn_803199A4() {
    if (atn::Abs_unk(m1534 - m1538) <= 0.01f) {
        fn_80319C10();
        return;
    }
    float rate = 0.35f;
    if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
        rate *= 0.1f;
    }
    if (mStatus.IsState_unk(0x1000000)) {
        rate *= 0.01f;
    }
    m1534 = rate * (m1538 - m1534) + m1534;
    atn::Vector3 v(m1528);
    atn::Matrix3x4 rot;
    rot.fn_8013A13C(m1534);
    PSMTXMultVec(rot.m, &v, &v);
    mPos = fn_80318648(m14B8) + v * 25.0f;
}

void Em316LaserSphere::fn_80319C10() {
    SetState_unk(0);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
    fn_80239DAC(lbl_8087F3C0, this, 1001, 1);
}

// inferred: fires the laser at the target (state 9): the beam goes from the edge of the sphere to "spine1" of the
// target, its matrix is translated, rotated along the beam and scaled to its length (in 50 units)
// NONMATCHING: 98.3% - stack slots of the temporaries and of the matrices of the inlined helpers; 2 variants
void Em316LaserSphere::fn_80319C90() {
    int wait = m1C84;
    m1574 = wait + rand() % wait;
    if (fn_80316F74(&mStatus) || mStatus.IsState_unk(0x800000)) {
        m1574 *= 2;
    }
    fn_80239DAC(lbl_8087F3C0, this, 0x3E8, 1);
    SetState_unk(9);
    m1568 = mPos;
    atn::Vector3 spine = fn_80318648(m14B8);
    m155C = spine - mPos;
    PSVECNormalize(&m155C, &m155C);
    m1578 = 0;
    m15B4 = m14B8;
    m155C = spine - (mPos + m155C * m5B0);
    float len = PSVECMag(&m155C) / 50.0f;
    PSVECNormalize(&m155C, &m155C);
    atn::Vector3 rot = m155C.ToRotation_unk();
    atn::Vector3 start = mPos + m155C * m5B0;
    m15B8 = fn_800F80A8(start);
    fn_800F80B8(m15B8.m, &rot);
    m15E8 = m15B8;
    atn::Vector3 scale(1.0f, 1.0f, len);
    fn_800F8290(m15B8.m, scale.x, scale.y, scale.z);
    fn_80232B7C(this, 0x3E8);
    fn_8023A680(lbl_8087F3C0, &m159C, -1, 5, &m15B8, 0, 0, 0, 1.0f, 0, -1, 1);
    fn_8023A680(lbl_8087F3C0, &m15A8, -1, 5, &m15E8, 0, 0, 0, 1.0f, 0, -1, 1);
}

// inferred: settings (lbl_8087F0A8, as in game/CopyRobotTask.cpp): frames per second at 0x30
struct LaserSettings_unk {
    unsigned char m00[0x30];
    int mFrameRate;
};
extern "C" LaserSettings_unk* lbl_8087F0A8;

// NONMATCHING: 99.23% - FPR numbering of the gravity (f3-f6 permuted) and r3/r4 of the copy to m1540
// (__VecCopy family); ~10 variants
void Em316LaserSphere::fn_8031A484() {
    int zero = 0;
    SetState_unk(2);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
    fn_80239DAC(lbl_8087F3C0, this, 1001, 1);
    fn_800C344C(sSounds_unk[3], &mPos, 0, -1, 1.0f);
    if (m6B8.x == 0.0f && m6B8.y == 0.0f && m6B8.z == 0.0f) {
        zero = 1;
    }
    if (!zero) {
        m1540 = m6B8.GetNormalize() * 2.0f;
        m1540.y *= 0.5f;
        if (m1540.y > 0.0f) {
            m1540.y = 0.0f;
        }
    }
    int fps = lbl_8087F0A8->mFrameRate;
    float g = -98.1f / (float)(fps * fps);
    m154C = 1.5f * (lbl_8087EFA8->m3A4 * g);
    CharaTask* caught = m15B4;
    if (caught != 0 && caught->m55C == 6 && caught->m560 == 0x8C) {
        fn_80319998((LaserCatch_unk*)fn_80318F90(caught));
        m15B4 = 0;
    }
    fn_800EB7A0();
}

// inferred: dodges the attack of `chara` (state 10, then 7): turns the line from the target by sAngle_unk to the
// side away from the aim of `chara` and moves there
// NONMATCHING: 30% - the original inlines the tiny members and two of the four "spine1" positions and calls
// ToRotation_unk and fn_80318648 (docs/inlining.md R5: its body is an inline over the budget); here it is over the
// budget and calls everything. A wrapper inline gives 67% but then fn_80318A30 inlines fn_8031A670 (100 -> 0)
void Em316LaserSphere::fn_8031A670(CharaTask* chara) {
    SetState_unk(10);
    fn_80239DAC(lbl_8087F3C0, this, 0x3E8, 1);
    fn_80239DAC(lbl_8087F3C0, this, 0x3E9, 1);
    m1524 = chara;
    m1534 = 0.0f;
    atn::Vector3 line = mPos - fn_80318648(m14B8);
    PSVECNormalize(&line, &line);
    m1528 = line;
    CharaTask* attacker = m1524;
    atn::Vector3 spine = fn_80318648(m14B8);
    atn::Vector3 back = mPos - spine;
    PSVECNormalize(&back, &back);
    int side;
    if (attacker == m14B8) {
        float aim = atn::WrapAngle_unk(attacker->fn_80158E1C().ToRotation_unk().y);
        float rot = atn::WrapAngle_unk(back.ToRotation_unk().y);
        side = atn::WrapAngle_unk(aim - rot) > 0.0f ? -1 : 1;
    } else {
        atn::Vector3 a = fn_80318648(attacker) - mPos;
        PSVECNormalize(&a, &a);
        float aim = atn::WrapAngle_unk(a.ToRotation_unk().y);
        atn::Vector3 b = spine - mPos;
        PSVECNormalize(&b, &b);
        float rot = atn::WrapAngle_unk(b.ToRotation_unk().y);
        side = atn::WrapAngle_unk(aim - rot) > 0.0f ? 1 : -1;
    }
    m1538 = sAngle_unk * (float)side;
    atn::Vector3 dir = m1528;
    Mtx m;
    PSMTXRotRad(m, 'y', m1538);
    PSMTXMultVec(m, &dir, &dir);
    m1504 = mPos;
    m1510 = fn_80318648(m14B8) + dir * 25.0f;
    m151C = 0.0f;
    m58C = 7;
}

int Em316LaserSphere::vfE4() {
    return m58C == 8;
}
