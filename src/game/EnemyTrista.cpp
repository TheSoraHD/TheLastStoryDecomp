#include "game/EnemyTrista.h"
#include "atn/Csv.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "math/Vec.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
// inferred: game state (lbl_8087EFA8): m3A4 is the time scale (as in chr/ControlCliffhang.cpp)
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" GameState_unk* lbl_8087EFA8;
// inferred: debug menu entries (as in gmk/SwingObject.cpp)
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" int fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                           int c, float f);
extern "C" void fn_800FB4B0(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, int id, const atn::Vector3* pos,
                            int a, int time, int b, int c);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0; m0B8 is a mode used while
// the guard effect is created
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" int fn_80041A28(int arg, CharaTask* chara, CharaTask* target); // inferred: kind of attack
// inferred: collision world and its ray test (as in game/EnemySeaLizerd.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: camera parameters of the field (0x3C bytes); names invented
struct TristaCamParam_unk {
    TristaCamParam_unk(const TristaCamParam_unk& o) {
        Copy_unk(o);
    }
    TristaCamParam_unk& operator=(const TristaCamParam_unk& o) {
        Copy_unk(o);
        return *this;
    }
    void Copy_unk(const TristaCamParam_unk& o) {
        m00 = o.m00;
        m04 = o.m04;
        m08 = o.m08;
        m0C = o.m0C;
        m10 = o.m10;
        for (int i = 0; i < 9; i++) {
            m14[i] = o.m14[i];
        }
        m38 = o.m38;
    }
    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14[9];
    int m38;
};
// inferred: camera mode of the field (lbl_8087F430 + 0x4D8); names invented
struct FieldCamera_unk {
    void Set_unk(int mode, const TristaCamParam_unk& param) {
        if (mMode != mode) {
            mMode = mode;
            mParam = param;
            m08 = 0.05f;
            m04 = 1.0f;
        }
    }
    int mMode;
    float m04;
    float m08;
    TristaCamParam_unk mParam;
};
// inferred: point the camera looks at (lbl_8087F430 + 0x97C); names invented
struct FieldLook_unk {
    void Set_unk(const atn::Vector3& pos, bool flag) {
        m01 = m00;
        m00 = 1;
        mPos = pos;
        m24 = 0.0f;
        m04 = flag ? 1 : 0;
    }
    unsigned char m00;
    unsigned char m01;
    unsigned char m02[2];
    int m04;
    unsigned char m08[4];
    atn::Vector3 mPos;
    unsigned char m18[0x24 - 0x18];
    float m24;
};
// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78 and written with fn_80370AE4
struct Field_unk {
    unsigned char m0000[0x4D8];
    FieldCamera_unk m4D8;
    unsigned char m520[0x588 - 0x520];
    TristaCamParam_unk m588; // inferred: default camera parameters
    unsigned char m5C4[0x8A0 - 0x5C4];
    CharaTask* m8A0;         // inferred: character the camera follows
    unsigned char m8A4[0x97C - 0x8A4];
    FieldLook_unk m97C;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag

static const char* const sSounds[] = {"SE_BTCOM_026", "SE_BTCOM_040"};
static const char* const sEffect0 = "/effect/ef_sp007a.efp";
static const char* const sEffect1 = "/effect/ef_sp007d.efp";
static const char* const sEffect2 = "/effect/ef_sp007e.efp";

// inferred: names invented
inline void PlayMotion_unk(atn::Model* model, int motion, int arg3, int arg4) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->mTracks[0].m0C = 1.0f;
    model->fn_80097C08(0, motion, arg3, arg4, 1, 0.0f, 0.1f);
}
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, atn::Model* model) {
    Color4f_unk color;
    atn::Vector3 rot;
    atn::Vector3 pos;
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

namespace {

// inferred: "shingan" pose of Trista: plays motion 0x173 and turns the guard on when it ends
class ControlShingan : public chr::ControlBase {
public:
    ControlShingan(CharaTask* chara, int time);
    virtual int vf0C();

    float m08; // inferred: frames left
    int m0C;   // inferred: guard time
};

ControlShingan::ControlShingan(CharaTask* chara, int time) : chr::ControlBase(chara), m0C(time) {
    chara->m560 = 0x2E;
    atn::Model* model = &mChara->mMotion;
    mChara->fn_8015495C(0, 1, 1, 1);
    PlayMotion_unk(model, 0x173, 0, 0);
    m08 = model->fn_80097D7C(0);
    mChara->fn_801446F0();
    fn_800C344C(sSounds[0], &mChara->mPos, 0, -1, 1.0f);
    EnemyTrista* trista = (EnemyTrista*)mChara;
    fn_80232B7C(0, 0);
    PlayEffect_unk(&trista->m14F8, &trista->mMotion);
}

int ControlShingan::vf0C() {
    int result = 0;
    float scale = lbl_8087EFA8->m3A4;
    m08 -= scale;
    if (m08 <= 0.0f) {
        fn_800C344C(sSounds[1], &mChara->mPos, 0, -1, 1.0f);
        ((EnemyTrista*)mChara)->fn_8035DC58(1, m0C);
        result = 1;
    }
    return result;
}

} // namespace

// NONMATCHING: 98.6% - the original loads info->m20 before saving the registers
EnemyTrista::EnemyTrista(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14D0(-1), m14D4(-1), m14D8(0), m14DC(0),
      m14E0(0), m14E4(0.0f), m14E8(0), m14EC(0), m14F0(0), m14F4(0), m15A4(120), m15A8(30), m15AC(9.0f),
      m15B0(300) {
    mFlag22 = 1;
    m14B0.vf0C("database/chara/special/db_enemy_trista.csv");
    void* group = m15B4.Load("EnemyTrista");
    fn_800874C8(group, "WaitFrame", &m15A4, 0, 1000000, 1, 0, 0);
    fn_800874C8(group, "DamageSubtractWait", &m15A8, 0, 1000000, 1, 0, 0);
    fn_8008771C(group, "AttackChanceFrame", &m15AC, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_800874C8(group, "ChaseCancelFrame", &m15B0, 0, 1000000, 1, 0, 0);
    m14F8.fn_8023780C(sEffect0);
    m1504.fn_8023780C(sEffect1);
    m1510.fn_8023780C(sEffect2);
    m151C.PushBack_unk(fn_80219E6C(200));
}

int EnemyTrista::vf0C() {
    int result = 1;
    if (fn_8013655C() != 0) {
        result = 0;
    }
    if (m14B0.fn_80473F50() != 0) {
        result = 0;
    }
    if (m14F8.fn_80237874() != 0) {
        result = 0;
    }
    if (m1504.fn_80237874() != 0) {
        result = 0;
    }
    if (m1510.fn_80237874() != 0) {
        result = 0;
    }
    if (result) {
        m58C = 6;
        fn_8035C1B4();
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10001;
        mStatus.m18 |= 0x1000000;
        return 1;
    }
    return 0;
}

void EnemyTrista::fn_8035C1B4() {
    atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    do {
        const char* key = csv.fn_8005B9CC();
        if (*key == ';' || *key == '\0') {
            continue;
        }
        if (strcmp(key, "NormalAttackID") == 0) {
            m14E8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "GuardCounterID") == 0) {
            m14EC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "HighJumpAttackID") == 0) {
            m14F0 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "EventDamageSkillID") == 0) {
            m14F4 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WaitFrame") == 0) {
            m15A4 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackChanceFrame") == 0) {
            m15AC = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "DamageSubtractWait") == 0) {
            m15A8 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "ChaseCancelFrame") == 0) {
            m15B0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "Event") == 0) {
            TristaEvent_unk e;
            e.m00 = atoi(csv.fn_8005B9CC());
            e.m04 = atoi(csv.fn_8005B9CC());
            e.m08 = fn_800DC288(csv.fn_8005B9CC());
            m1540.PushBack_unk(e);
        }
    } while (csv.fn_8005BBF8());
}

inline void EnemyTrista::ResetCamera_unk() {
    Field_unk* field = lbl_8087F430;
    if (m14D8 != 0) {
        field->m8A0 = 0;
        field->m4D8.mMode = 0;
        field->m97C.m00 = 0;
        m14D8 = 0;
    }
}

// inferred: front direction for a rotation around y (name invented)
inline atn::Vector3 FrontDir_unk(float rotY) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rotY);
    PSMTXMultVec(m, &dir, &dir);
    return dir;
}

// inferred: fires the event damage skill in front of the target (name invented)
inline void EnemyTrista::FireEvent_unk() {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', mC58.mC4->mRot.y);
    PSMTXMultVec(m, &dir, &dir);
    Effect_unk* mgr = lbl_8087F048;
    atn::Vector3 pos = mC58.mC4->mPos + dir;
    fn_800FB4B0(mgr, this, m14F4, fn_800F8548(mgr), &pos, 0, 30, -1, 1);
}

inline void EnemyTrista::StartEvent_unk() {
    unsigned int i = 0;
    TristaEvent_unk* e = m1540.mData;
    for (; i < m1540.mCount; i++) {
        if (fn_80370A78(lbl_8087F430, e->m00) == 0) {
            fn_80370AE4(lbl_8087F430, e->m00, 1);
            m14D0 = i;
            break;
        }
        e++;
    }
}

// NONMATCHING: 98.8% - registers of the event loop of StartEvent_unk (pointer/counter swapped)
void EnemyTrista::vf10() {
    if (m55C == 2) {
        fn_8016E970(3);
    }
    if (mC58.mC0 != 0) {
        m14B8++;
        if (mC58.mC4 == 0) {
            mC58.mC4 = (CharaTask*)lbl_8087F8A0->mHead;
        }
        if (m14D0 >= 0) {
            TristaEvent_unk* e = &m1540.mData[m14D0];
            if (fn_80370A78(lbl_8087F430, e->m00) != 1) {
                if (fn_80370A78(lbl_8087F430, e->m04) == 0) {
                    m14D4 = m14D0;
                    if (m14D4 >= 1) {
                        m14C0 = 1;
                        m14B8 = m15A4 - 15;
                    }
                } else {
                    FireEvent_unk();
                }
                m14D0 = -1;
            }
        }
        if (m58C != 8) {
            ResetCamera_unk();
        }
        if (m14DC != 0 && !(m55C == 6 && 20 <= m560 && 23 >= m560)) {
            m14DC = 0;
        }
        if (m14E0 != 0) {
            float scale = lbl_8087EFA8->m3A4;
            m14E4 -= scale;
            if (m14E4 <= 0.0f || (mStatus.m0C & 0x20) == 0x20) {
                fn_8035DC58(0, 0.0f);
            }
        }
        switch (m58C) {
        case 7:
            fn_8035CF78();
            break;
        case 8:
            fn_8035D294();
            break;
        case 9:
            switch (m55C) {
            case 6:
                if (m560 != 2) {
                    m58C = 6;
                    m14B8 = m15A4 / 2;
                }
                break;
            case 7:
                break;
            default:
                m58C = 6;
                m14B8 = m15A4 / 2;
                break;
            }
            break;
        case 10:
            switch (m55C) {
            case 6:
                if (m560 != 0x2E) {
                    m58C = 6;
                    m14B8 = 0;
                }
                break;
            case 7:
                break;
            default:
                m58C = 6;
                m14B8 = 0;
                break;
            }
            break;
        case 11: {
            CharaTask* target = mC58.mC4;
            if (target != 0 && target->m55C == 6 && target->vf78() != 0) {
                StartEvent_unk();
                fn_8016E970(3);
                m58C = 6;
                m14B8 = 0;
            }
            break;
        }
        default:
            fn_8035C8CC();
            break;
        }
    }
    fn_80139560();
    fn_8014C540();
    fn_80145334();
    if (fn_8035DA94()) {
        StartEvent_unk();
    }
}

// inferred: side direction of the character (name invented)
inline atn::Vector3 Side_unk(CharaTask* chara, float up) {
    atn::Vector3 v(0.0f, up, 0.0f);
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &front, &front);
    atn::Vector3 side;
    PSVECCrossProduct(&front, &v, &side);
    return side;
}

// NONMATCHING: 82.1% - stack slots of the temporaries of the side jump (Side_unk) and the registers of
// SetHit_unk; same code otherwise
void EnemyTrista::fn_8035C8CC() {
    switch (m55C) {
    case 6:
        if (m560 == 10) {
            m58C = 7;
            m14B8 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m598 = 0;
            m59D = 0;
            m59C = 0;
            SetHit_unk(m14EC);
            m594 = 1;
            m59F = 1;
            ((int*)mControl.get())[7] = 1;
        } else if (m560 == 0x27) {
            m14B8 = 0;
        }
        break;
    case 7:
        fn_8016E970(3);
        if (m14BC != 0) {
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &dir, &dir);
            fn_80155790(dir);
            m14BC = 0;
            m14B8 += 500;
        }
        break;
    default: {
        CharaTask* target = mC58.mC4;
        mFC0 = target;
        mFlag5 = 1;
        if (target != 0 && (target->mStatus.m0C & 0x20) == 0x20) {
            m58C = 6;
            m14B8 = 0;
            fn_80161880(0x142, (int)&target->mMotion.mMtx, 0);
            mFlag5 = 0;
        } else if (m14BC != 0) {
            atn::Vector3 side = Side_unk(this, 1.0f);
            atn::Vector3 from = mPos;
            from.y += 5.0f;
            atn::Vector3 to = from + side * 10.0f;
            void* world = lbl_8087EE98;
            if (fn_8004ED34(world, 0, &from, &to, fn_80179D44() | 0x80000000, 0, 0) == 0) {
                fn_80155790(side);
            } else {
                side = Side_unk(this, -1.0f);
                to = from + side * 10.0f;
                void* world2 = lbl_8087EE98;
                if (fn_8004ED34(world2, 0, &from, &to, fn_80179D44() | 0x80000000, 0, 0) == 0) {
                    fn_80155790(side);
                }
            }
            m14BC = 0;
            m14B8 += 100;
        } else if (m14B8 > m15A4) {
            if (target == 0) {
                m58C = 6;
                m14B8 = 0;
            } else if (m14C0 != 0) {
                m14C0 = 0;
                m58C = 10;
                m14B8 = 0;
                fn_80178208(new (0, "", "", 0) ControlShingan(this, 900000));
                fn_80370AE4(lbl_8087F430, 0x67, 1);
            } else {
                bool jump;
                if (mC58.mC4 == 0) {
                    jump = false;
                } else {
                    jump = false;
                    if ((mC58.mC4->mPos - mPos).Length_unk() > 65.0f) {
                        atn::Vector3 from = mPos;
                        atn::Vector3 to = mC58.mC4->mPos;
                        from.y += 9.5f;
                        to.y += 9.5f;
                        void* world = lbl_8087EE98;
                        if (fn_8004ED34(world, 0, &from, &to, fn_80179D44() | 0x80000000, 0, 0) == 0) {
                            jump = true;
                        }
                    }
                }
                if (jump) {
                    m58C = 8;
                    m14B8 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    m594 = 1;
                    m598 = 0;
                    mFlag5 = 0;
                    fn_8016D74C(m14F0, 0, 0);
                } else {
                    fn_8035CE58();
                }
            }
        }
        break;
    }
    }
}

void EnemyTrista::fn_8035CE58() {
    m58C = 7;
    m14B8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m594 = 1;
    m598 = 0;
    if ((mC58.mC4->mPos - mPos).Length_unk() < 25.0f) {
        mFlag5 = 0;
        if (fn_80041A28(0, this, mC58.mC4) == 2) {
            fn_8015783C(m14E8, mC58.mC4->mPos, 3.0f);
        } else {
            fn_801562A0(m14E8, mC58.mC4->mPos, 0, 0, 0, 0, 3.0f);
        }
    } else {
        fn_80170A20(mC58.mC4, 0, 20.0f);
    }
}

// NONMATCHING: 98.0% - stack slot of the scoped_ptr test (ptmf copy) and order of the arguments of
// fn_800F8C6C
void EnemyTrista::fn_8035CF78() {
    switch (m55C) {
    case 6:
        if (!fn_80158BB4()) {
            if (m560 == 4 || m560 == 9) {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0) - 6.0001f) {
                    mMotion.mTracks[0].m0C = 6.0f / (6.0f + m15AC);
                }
            }
            if (lbl_8087F048 != 0 && fn_80158CA4() != 0) {
                if (mControl && mControl->vf30() > m598) {
                    m598 = mControl->vf30();
                    m594 = 1;
                    m590 = fn_800F8548(lbl_8087F048);
                }
                if (m594 != 0) {
                    fn_80144710();
                    int n = fn_800F8C6C(lbl_8087F048, 0, 0, this, m638, m590, 30, m594, 0.0f);
                    if (m594 > 0) {
                        m594 -= n;
                    }
                }
            }
        } else {
            if (m560 == 4 || (m560 >= 9 && m560 <= 10)) {
                fn_8016E970(3);
            }
            m58C = 6;
            m14B8 = 0;
        }
        break;
    case 7:
        if (mRoute.m2C != 0) {
            fn_801562A0(m14E8, mC58.mC4->mPos, 0, 0, 0, 0, 3.0f);
        } else {
            atn::Vector3 d = mC58.mC4->mPos - mPos;
            if (d.Length_unk() < 25.0f) {
                if (fn_80041A28(0, this, mC58.mC4) == 2) {
                    fn_8015783C(m14E8, mC58.mC4->mPos, 3.0f);
                } else {
                    fn_801562A0(m14E8, mC58.mC4->mPos, 0, 0, 0, 0, 3.0f);
                }
            } else if (m14B8 > m15B0) {
                m58C = 6;
                m14B8 = 0;
            }
        }
        break;
    default:
        m58C = 6;
        m14B8 = 0;
        break;
    }
}

// NONMATCHING: 60.9% - the original copies the default camera parameters through FPRs into
// saved registers before the mode test (member-wise copy scheduled differently here)
void EnemyTrista::fn_8035D294() {
    Field_unk* field = lbl_8087F430;
    switch (m55C) {
    case 6:
        if (m560 == 13) {
            break;
        }
        if (m560 == 14) {
            atn::Vector3 pos = mPos;
            pos.y += 15.0f;
            if (m14D8 == 0) {
                field->m8A0 = this;
                TristaCamParam_unk param = field->m588;
                param.m00 = 15.0f;
                param.m04 = 0.7853982f;
                param.m08 = -0.43633232f;
                param.m10 = 8.0f;
                field->m4D8.Set_unk(4, param);
                atn::Vector3 look = mPos;
                look.y += 25.0f;
                field->m97C.Set_unk(look, false);
                m14D8 = 1;
            } else {
                field->m8A0 = this;
                field->m97C.mPos = pos;
            }
            break;
        }
        ResetCamera_unk();
        m58C = 6;
        m14B8 = 0;
        break;
    case 7:
        break;
    default:
        m58C = 6;
        m14B8 = 0;
        break;
    }
}

int EnemyTrista::fn_8035D4E0(HitMsg_unk* msg) {
    if (mC58.mC4 == 0) {
        return 0;
    }
    atn::Vector3 dir = msg->m28;
    atn::Vector3 from = mPos;
    dir.y = 0.0f;
    atn::Vector3 to = mPos + dir * 80.0f;
    from.y += 4.0f;
    to.y += 4.0f;
    CollResult_unk result;
    void* world = lbl_8087EE98;
    if (fn_8004ED34(world, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) == 0) {
        m14C4 = to;
        m14C4.y -= 4.0f;
        return 1;
    }
    atn::Vector3 d = result.m04 - from;
    if (d.Length_unk() > 10.0f) {
        m14C4 = result.m04;
        m14C4.y -= 4.0f;
        return 1;
    }
    m14C4 = mPos;
    return 1;
}

int EnemyTrista::vfA8(int kind) {
    int result = 0;
    switch (kind) {
    case 0: {
        float chance = 0.5f;
        if (m14E0 != 0) {
            chance = 1.0f;
        }
        if ((float)(rand() % 1001) / 1000.0f < chance) {
            result = 2;
        }
        break;
    }
    case 1: {
        float chance = 0.0f;
        if ((float)(rand() % 1001) / 1000.0f < chance) {
            result = 3;
        }
        break;
    }
    }
    return result;
}

// inferred: keeps the hit points above the ratio of the next event (name invented)
inline void EnemyTrista::ClampHp_unk() {
    int next = m14D4 + 1;
    if (next < (int)m1540.mCount) {
        float min = mStatus.mParamB.m84 * (m1540.mData[next].m08 - 0.01f);
        if (mStatus.m04 < min) {
            mStatus.m04 = min;
        }
    }
}

// inferred: the guard breaks only with the skills of m151C
inline bool HasHit_unk(FixedArray_unk<HitInfo_unk*, 8>& hits, HitInfo_unk* hit) {
    for (unsigned int i = 0; i < hits.mCount; i++) {
        if (hits.mData[i] == hit) {
            return true;
        }
    }
    return false;
}

// NONMATCHING: 95.0% - the original keeps r5 (a second argument of vf3C, unused by CharaTask::vf3C)
// and passes it to CharaTask::vf3C
void EnemyTrista::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->m44 == 1) {
        msg->m0C |= 0x8008;
        if (m55C == 6 && m560 == 14) {
            return;
        }
    } else if (m14E0 != 0) {
        if (HasHit_unk(m151C, msg->mData)) {
            fn_80370AE4(lbl_8087F430, 0x6D, 1);
            if (fn_8035D4E0(msg)) {
                m58C = 9;
                m14B8 = 0;
                mFlag5 = 0;
                fn_80155A88(m14C4, 0);
                m6D0.PushBack_unk(CharaPair8_unk(msg->m34, msg->m38));
                msg->m88 = -1;
                return;
            }
            msg->m0C |= 0x8008;
        } else {
            m151C.PushBack_unk(msg->mData);
            fn_8035DC58(0, 0.0f);
            fn_80370AE4(lbl_8087F430, 0x76, 1);
            msg->m50 |= 8;
            m14DC = 1;
        }
    }
    CharaTask::vf3C(hit);
    m14B8 += m15A8;
    ClampHp_unk();
}

void EnemyTrista::vf44() {
    ClampHp_unk();
    mStatus.fn_8013322C(0x20);
}

int EnemyTrista::fn_8035DA94() {
    if (m14E0 != 0 || m14DC != 0) {
        return 0;
    }
    if (m14D0 >= 0) {
        return 0;
    }
    if (mC58.mC4 == 0) {
        return 0;
    }
    if (mC58.mC4->m55C != 6 || mC58.mC4->vf78() == 0) {
        return 0;
    }
    HitInfo_unk* hit = mC58.mC4->m638;
    if (hit == 0) {
        return 0;
    }
    float dist = (mC58.mC4->mPos - mPos).Length_unk();
    float range;
    if (mC58.mC4->m560 == 14) {
        range = hit->m58;
    } else {
        range = hit->m40 + mC58.mC4->mStatus.mParamB.m28;
    }
    if (dist > 8.0f + range) {
        return 0;
    }
    float ratio = mStatus.m04 / mStatus.mParamB.m84;
    TristaEvent_unk* e = m1540.mData;
    for (unsigned int i = 0; i < m1540.mCount; i++) {
        if (fn_80370A78(lbl_8087F430, e->m00) == 0 && ratio <= e->m08) {
            return 1;
        }
        e++;
    }
    return 0;
}

void EnemyTrista::fn_8035DC58(int on, float time) {
    int prev = m14E0;
    m14E4 = time;
    m14E0 = on;
    if (on != 0) {
        if (prev == 0) {
            fn_80232B7C(this, 5);
            lbl_8087F3C0->mB8 = 2;
            PlayEffect_unk(&m1504, &mMotion);
            lbl_8087F3C0->mB8 = 0;
            fn_800C344C("SE_PC006_001", &mPos, 0, -1, 1.0f);
        }
    } else if (prev != 0) {
        fn_80239DAC(lbl_8087F3C0, this, 5, 1);
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m1510, &mMotion);
        fn_800C344C("SE_PC006_003", &mPos, 0, -1, 1.0f);
    }
}
