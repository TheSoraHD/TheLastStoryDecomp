#include "game/EnemyJill.h"
#include "atn/Keyboard.h"
#include "atn/Csv.h"
#include "math/Vec.h"
#include "gmk/Lemon.h"
#include "chr/ControlBase.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" HitInfo_unk* fn_80219E6C(int id);
// inferred: debug menu entries (as in gmk/SwingObject.cpp)
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
// inferred: render parameters (lbl_8087EFA8): color matrix at 0x324 (as in atn/PostFxColorMatrix.cpp)
struct JillParams_unk {
    unsigned char m000[0x324];
    JillColMat_unk m324;
};
extern "C" JillParams_unk* lbl_8087EFA8;
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id

// inferred: const info (the load of info->m20 is scheduled into the prologue only through a const pointer)
EnemyJill::EnemyJill(atn::Task* parent, const EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14CC(0), m14D0(0), m14D4(0), m14D8(0), m14DC(0),
      m14E8(0), m1570(-1), m1578(0), m157C(40), m1580(9.0f), m1584(0.3f), m1588(0.0f), m1590(0),
      m1594(0), m1598(0), m159C(0), m15A0(0), m15A4(0), m15A8(6), m15AC(0), m1654(0), m1658(0), m165C(0), m17A0(0), m17BC(0), m17C0(0), m17C4(1) {
    mFlag22 = 1;
    m14B0.vf0C("database/chara/special/db_enemy_jill.csv");
    void* group = m158C.Load("EnemyJill");
    fn_80087994(group, "DebugDisp", &m1654, 0, 0);
    fn_80087994(group, "DebugLog", &m1658, 0, 0);
    fn_800874C8(group, "WaitFrame", &m157C, 0, 1000000, 1, 0, 0);
    fn_8008771C(group, "AttackChanceFrame", &m1580, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_8008771C(group, "AttackBonusRate", &m1584, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_800874C8(group, "AttackTest", &m165C, -1, 100, 1, 0, 0);
    fn_8008771C(group, "GuardBonusRate", &m1588, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_80087994(group, "StopTalking", &m15AC, 0, 0);
    fn_800874C8(group, "ActionMode", &m58C, 0, 100000, 1, 0, 0);
    fn_80087E9C(group, "Pos", &mPos, 0, 0, -1000.0f, 1000.0f, 1.0f);
    fn_800874C8(group, "EventPlayIndex", &m1570, 0, 100, 1, 0, 0);
    fn_8008771C(group, "HP", &mStatus.m04, 0, 0, 0.0f, 100000.0f, 1.0f);
    m15B4.mCount = 0;
    m15E4.mCount = 0;
    m1614.mCount = 0;
    m17A4.LoadRequest("ef_ev114.efp");
    m17B0.LoadRequest("/effect/ef_ev107a_01.eff");
}

// inferred: resets the step and remembers the state (inlined everywhere; names invented)
inline void EnemyJill::SetStep_unk() {
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m594 = 1;
    m598 = 0;
    if (m58C != 6 && m58C != 7 && m58C != 14) {
        m15A8 = m58C;
    }
}
inline void EnemyJill::ResetState_unk() {
    if (m58C != 6 && m58C != 7) {
        m15A0 = 0;
    }
    SetStep_unk();
}
inline void EnemyJill::SetState_unk(int state) {
    SetStep_unk();
    m58C = state;
    mFlag5 = 0;
}

// NONMATCHING: 98.26% - r4/r5 of the destination and of lbl_8087EFA8 in the copy of the color matrix (8 variants:
// pointer/reference locals, member-wise operator=, helper)
int EnemyJill::vf0C() {
    if (fn_8013655C() == 0 && m14B0.fn_80473F50() == 0 && m17A4.IsLoading() == 0 &&
        m17B0.IsLoading() == 0) {
    m58C = 6;
    JillColMat_unk* mat = &m1660[0];
    *mat = lbl_8087EFA8->m324;
    m17A0 = mat;
    fn_8032E3DC();
    mStatus.m18 |= 0x140;
    mStatus.m18 |= 0x10011;
    mStatus.m18 |= 0x404000;
    return 1;
    }
    return 0;
}

// NONMATCHING: 99.87% - inside the inlined std::string copy into the voice array: the original reuses the
// register of the text (r26) for the destination and swaps the two iterator tag slots (7 variants: named
// local, push_back, placement new, accessor)
void EnemyJill::fn_8032E3DC() {
    atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    do {
        const char* key = csv.fn_8005B9CC();
        if (*key == ';' || *key == '\0') {
            continue;
        }
        if (strcmp(key, "NormalAttack") == 0) {
            m14D0 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "GuardCounter") == 0) {
            m14D4 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "SonicBoom") == 0) {
            m14DC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "HighJumpAttack") == 0) {
            m14D8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "ChaosAttackSkill") == 0) {
            m14E0 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "AbsorbHPSkill") == 0) {
            m14E4 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "EventDamageSkillID") == 0) {
            m14E8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WaitFrame") == 0) {
            m157C = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackChanceFrame") == 0) {
            m1580 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackBonusRate") == 0) {
            m1584 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "GuardBonusRate") == 0) {
            m1588 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "Event") == 0) {
            JillEvent_unk e;
            e.m00 = atoi(csv.fn_8005B9CC());
            e.m04 = atoi(csv.fn_8005B9CC());
            e.m08 = fn_800DC288(csv.fn_8005B9CC());
            e.m0C = atoi(csv.fn_8005B9CC());
            m14EC.PushBack_unk(e);
        } else if (strcmp(key, "Voice") == 0) {
            if (!m1614.full()) {
                m1614.PushBack_unk(std::string(csv.fn_8005B9CC()));
            }
        } else if (strcmp(key, "ColMat") == 0) {
            int i = atoi(csv.fn_8005B9CC());
            if (i < 4) {
                m1660[i].mEnable = 1;
                m1660[i].mRows[0].x = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[0].y = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[0].z = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[1].x = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[1].y = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[1].z = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[2].x = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[2].y = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[2].z = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[3].x = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[3].y = fn_800DC288(csv.fn_8005B9CC());
                m1660[i].mRows[3].z = fn_800DC288(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8());
}

// inferred: field state (lbl_8087F430 + 0x54E4, as in game/BattleResult.cpp)
struct JillField_unk {
    unsigned char m0000[0x54E4];
    int m54E4;
};
extern "C" JillField_unk* lbl_8087F430;

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects

// inferred: plays an effect on the model (name invented, as in game/Em002Skelton.cpp)
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

// inferred: control of the guard counter (the flag at 0x1C is set by fn_8032FDE8); name invented
struct JillControl_unk : public chr::ControlBase {
    unsigned char m08[0x1C - 0x08];
    int m1C;
};

// inferred: accessors kept out of line (weak copies of other files; C names)
struct Field_unk;
// inferred: field accessor (weak copy 0x80121F00; inlined unless the caller is over the inline budget)
extern "C" inline Field_unk* fn_80121F00() {
    return (Field_unk*)lbl_8087F430;
}
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
// inferred: input state of the field (0x80122550) with a byte flag at 0x910 and a value at 0x834 (weak
// copies 0x8026616C/0x80266174 of game/Em201ZanGurgFinal.cpp); names invented
struct PlayerInput_unk {
    unsigned char m000[0x6C];
    int m6C;
    unsigned char m070[0x834 - 0x70];
    int m834;
    unsigned char m838[0x910 - 0x838];
    unsigned char m910;
    unsigned char m911[0x914 - 0x911];
    int m914;
};
extern "C" inline PlayerInput_unk* fn_80122550(Field_unk* field) {
    return (PlayerInput_unk*)((char*)field + 0x6C);
}
extern "C" inline void fn_8026616C(PlayerInput_unk* input, int value) {
    input->m834 = value;
}
extern "C" inline void fn_80266174(PlayerInput_unk* input, int value) {
    input->m910 = 0;
    if (value != 0) {
        input->m914 = 0;
    }
}
extern "C" inline int* fn_8032ECA0(PlayerInput_unk* input) {
    return (int*)((char*)input + 0x46C);
}
extern "C" inline void fn_8032EC94(int* p) {
    *p = 0;
}
extern "C" inline JillEvent_unk* fn_8032ECA8(FixedArray_unk<JillEvent_unk, 8>* events, int i) {
    return &events->mData[i];
}
// inferred: player manager (as in gmk/Cannonball.cpp)
struct PlayerMgr_unk {
    unsigned char m00[0x48];
    CharaTask* mHead;
};
extern "C" PlayerMgr_unk* lbl_8087F8A0;
extern "C" inline PlayerMgr_unk* fn_8000D9E8() {
    return lbl_8087F8A0;
}
extern "C" inline CharaTask* fn_8000DCF4(PlayerMgr_unk* mgr) {
    return mgr->mHead;
}
Effect_unk* fn_8013A194();
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);

// NONMATCHING: 99.79% - r29/r30 of the current event and of the rotation pointer in the event damage (the
// rest matches)
void EnemyJill::vf10() {
    if (m1578 != 0) {
        return;
    }
    if (m55C == 2) {
        fn_8016E970(3);
    }
    if (m1590 != 0) {
        if (--m1594 < 0) {
            m1590 = 0;
            m1594 = 0;
        }
    } else {
        m1594 = 0;
    }
    if (mC58.mC0 != 0) {
        m14BC++;
        m15A0++;
        fn_8032F6A8();
        if (mC58.mC4 == 0) {
            mC58.mC4 = fn_8000DCF4(fn_8000D9E8());
        }
        if (fn_8032F608()) {
            fn_80332F78();
        }
        if (m1570 >= 0) {
            JillEvent_unk* e = fn_8032ECA8(&m14EC, m1570);
            if (fn_80370A78(fn_80121F00(), e->m00) != 1) {
                if (fn_80370A78(fn_80121F00(), e->m04) == 1) {
                    m1574 = e->m0C;
                    fn_803333BC();
                    m1598 = 1;
                } else {
                    mStatus.m04 = (float)mStatus.mParamB.m84 * (0.05f + e->m08);
                    if (m14E8 != 0) {
                        atn::Vector3* rot = GetRot_unk();
                        atn::Vector3 pos = *mC58.mC4->fn_8013C38C() + mC58.mC4->fn_8014052C();
                        int id = fn_800F8548(fn_8013A194());
                        fn_800FAB80(fn_8013A194(), this, m14E8, id, &pos, rot, 0, 30, -1, -1, 0.0f, 1.0f);
                    }
                }
                m1570 = -1;
            }
        }
        if (m58C != 9) {
            PlayerInput_unk* input = fn_80122550(fn_80121F00());
            if (m14CC != 0) {
                fn_8026616C(input, 0);
                fn_8032EC94(fn_8032ECA0(input));
                fn_80266174(input, 0);
                m14CC = 0;
            }
        }
        switch (m58C) {
        case 7:
            fn_8032FFF8();
            break;
        case 8:
            fn_80330218();
            break;
        case 9:
            fn_80330D74();
            break;
        case 11:
            fn_80331094();
            break;
        case 12:
            fn_8033135C();
            break;
        case 14:
            fn_803317F4();
            break;
        case 15:
            fn_80331DAC();
            break;
        case 16:
            fn_803323A0();
            break;
        case 17:
            fn_8033250C();
            break;
        case 18:
            fn_80331898();
            break;
        default:
            fn_8032FDE8();
            fn_8032EF38();
            break;
        }
    }
    fn_80139560();
    fn_8014C540();
    fn_80145334();
    if (fn_8032FBF4()) {
        fn_8032FD30();
    }
    if (m1658 != 0 && atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x43)) {
        fn_80332BE0();
    }
}

void EnemyJill::vf14() {
    BattleCharaTask::vf14();
}

int EnemyJill::vfA8(int kind) {
    int result = 0;
    switch (kind) {
    case 0:
        if (rand() % 1001 / 1000.0f < m1584) {
            result = 2;
        }
        break;
    case 1:
        if (rand() % 1001 / 1000.0f < m1588) {
            result = 3;
        }
        break;
    }
    return result;
}

void EnemyJill::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    switch (m58C) {
    case 20:
    case 15:
    case 18:
        msg->m10 *= 0.0f;
        break;
    case 12:
        if (mMotion.mTracks[0].m08 < 10.0f) {
            msg->m10 *= 0.0f;
        }
        break;
    }
    CharaTask::vf3C(hit);
    m1590 = 0;
    m17BC++;
    if (m55C == 6 && (m560 == 22 || m560 == 23 || m560 == 20)) {
        ResetState_unk();
        m58C = 6;
    }
}

void EnemyJill::vf44() {
    mStatus.fn_8013322C(0x20);
}

void EnemyJill::fn_8032EF38() {
    if (m55C != 3) {
        return;
    }
    if (mC58.mC4 != 0) {
        if (m17C4 != 0) {
            if (m15A0 >= m157C) {
                SetState_unk(9);
                fn_8016D74C(m14D8, 0, 0);
                m17C4 = 0;
            }
        } else if (!fn_8032F0EC()) {
            SetStep_unk();
            m58C = 7;
            fn_80170A20(mC58.mC4, 0, 30.0f);
        }
    } else {
        ResetState_unk();
        m58C = 6;
    }
}

int EnemyJill::fn_8032F0EC() {
    float dist = (*mC58.mC4->fn_8013C38C() - mPos).Length_unk();
    if (dist < 30.0f) {
        if (m15A0 >= m157C) {
            if (m1598 != 0) {
                m1598 = 0;
                fn_80333024();
            } else if (rand() % 4 == 0) {
                fn_80332BE0();
            } else {
                fn_803326C0();
            }
        } else {
            if (m58C != 6) {
                fn_80332628();
            }
            return 1;
        }
    } else if (dist > 80.0f && fn_8032F204() && m15A8 != 9) {
        fn_80332B44();
    }
    return m58C != 6;
}

// inferred: collision world and its ray test (as in game/EnemyTrista.cpp)
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

int EnemyJill::fn_8032F204() {
    if (mC58.mC4 == 0) {
        return 0;
    }
    void* world;
    int result = 0;
    atn::Vector3 d = mC58.mC4->mPos - mPos;
    if (PSVECMag(&d) > 45.0f) {
        atn::Vector3 from(mPos);
        world = lbl_8087EE98;
        atn::Vector3 to(mC58.mC4->mPos);
        from.y += 10.0f;
        to.y += 10.0f;
        if (fn_8004ED34(world, 0, &from, &to, fn_80179D44() | 0x80000000, 0, 0) == 0) {
            result = 1;
        }
    }
    return result;
}

// NONMATCHING: 99.89% - r3/r4 of the copy of the free point into m14C0 (known __VecCopy register case)
int EnemyJill::fn_8032F314() {
    if (mC58.mC4 == 0) {
        return 0;
    }
    void* world;
    unsigned int i;
    int result = 0;
    CollResult_unk hit;
    float base = 6.2831855f * (rand() % 1001 / 1000.0f) + -3.1415927f;
    CharaTask* target = mC58.mC4;
    atn::Vector3 to = target->mPos - mPos;
    if (PSVECSquareMag(&to) < 0.0001f) {
        to.Set_unk(0.0f, 0.0f, 1.0f);
    } else {
        PSVECNormalize(&to, &to);
    }
    for (i = 0; i < 8; i++) {
        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', 0.7853982f * (float)i + base);
        PSMTXMultVec(m, &dir, &dir);
        if (!(PSVECDotProduct(&dir, &to) > 0.8660254f)) {
            atn::Vector3 from(mPos);
            world = lbl_8087EE98;
            atn::Vector3 dest = mPos + dir * 50.0f;
            from.y += 10.0f;
            dest.y += 10.0f;
            if (fn_8004ED34(world, &hit, &from, &dest, fn_80179D44() | 0x80000000, 0, 0) == 0) {
                m14C0 = dest;
                m14C0.y -= 10.0f;
                result = 1;
                break;
            }
        }
    }
    return result;
}

int EnemyJill::fn_8032F608() {
    atn::Vector3 d = mC58.mC4->mPos - mPos;
    PSVECMag(&d);
    if (m55C == 6) {
        return 0;
    }
    if (mC58.mC4 != 0 && (mC58.mC4->mStatus.m0C & 0x20) == 0x20) {
        return 1;
    }
    return 0;
}

// inferred: function stripped by the linker (no caller left); only its constants remain in .sdata2 between
// those of fn_8032F314 and fn_8032F828 (3.1415927f, the ToRotation_unk constants and 57.295776f). Body and name
// invented: what matters is the order of its literals (as CharaTask::Stripped1_unk)
float EnemyJill::Stripped1_unk() {
    float y = atn::WrapAngle_unk(mRot.y);
    atn::Vector3 rot = (mC58.mC4->mPos - mPos).ToRotation_unk();
    return 57.295776f * (rot.y - y);
}

// inferred: arena (lbl_8087F490): two players at 0x263C/0x2640 (nonzero m50 = talking); names invented
struct JillArenaPlayer_unk {
    unsigned char m00[0x50];
    int m50;
};
struct JillArena_unk {
    unsigned char m0000[0x263C];
    JillArenaPlayer_unk* m263C;
    JillArenaPlayer_unk* m2640;
};
extern "C" JillArena_unk* lbl_8087F490;
extern "C" void* lbl_8087F498; // inferred: voice manager
extern "C" atn::SoundRef fn_803EA09C(void* mgr, CharaTask* chara, const char* name, int a, int b, int c, float volume);
extern "C" int fn_800CB58C(atn::SoundRef* sound); // inferred: playing

void EnemyJill::fn_8032F6A8() {
    if (m15B4.mCount == 0) {
        fn_8032F828();
    }
    int ok = 1;
    if (m15AC != 0) {
        ok = 0;
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 != 0) {
        ok = 0;
    }
    JillArena_unk* arena = lbl_8087F490;
    if (arena != 0) {
        if (arena->m263C != 0 && arena->m263C->m50 != 0) {
            ok = 0;
        }
        if (arena->m2640 != 0 && arena->m2640->m50 != 0) {
            ok = 0;
        }
    }
    if (ok && (m15B0.mPtr == 0 || fn_800CB58C(&m15B0) == 0)) {
        m15B0 = fn_803EA09C(lbl_8087F498, this, m1614[m15B4.Front_unk()].c_str(), 0, 1, 0, 1.0f);
        m15B4.Pop_unk();
    }
}

// NONMATCHING: 93.8% - registers and order of the ring buffer updates (load of the count of the history before
// its head, r6/r5 of the modulo constant) and the 0.2f product (9 variants: Pop_unk/Next_unk, locals, while/for)
void EnemyJill::fn_8032F828() {
    float weights[5];
    float one = 1.0f;
    weights[0] = one;
    weights[1] = one;
    weights[2] = one;
    weights[3] = one;
    weights[4] = one;
    if (m15E4.mCount == 0) {
        float w = 0.2f * one;
        weights[0] = w;
        weights[1] = w;
        weights[2] = w;
        weights[3] = w;
        weights[4] = w;
    } else {
        float rate = 0.7f;
        unsigned int i = 0;
        while (i < m15E4.mCount) {
            int k = m15E4.mData[(m15E4.mHead + i++) % 10];
            weights[k] *= rate;
            rate *= 0.8f;
        }
        weights[m15E4.mData[(m15E4.mHead + m15E4.mCount - 1) % 10]] *= 0.001f;
    }
    int i;
    for (i = 0; i < 5; i++) {
        if (rand() % 1001 / 1000.0f < weights[i]) {
            m15B4.Push_unk(i);
            if (m15E4.mCount >= 10) {
                unsigned int head = m15E4.mHead + 1;
                m15E4.mCount--;
                m15E4.mHead = head % 10;
            }
            m15E4.Push_unk(i);
            break;
        }
    }
    if (i == 5) {
        int best = 0;
        for (int j = 0; j < 5; j++) {
            if (j != best && weights[j] > weights[best]) {
                best = j;
            }
        }
        m15B4.Push_unk(best);
        if (m15E4.mCount >= 10) {
            unsigned int head = m15E4.mHead + 1;
                m15E4.mCount--;
                m15E4.mHead = head % 10;
        }
        m15E4.Push_unk(best);
    }
}

// NONMATCHING: 86.3% - the original keeps `this`, the index and the byte offset of the event in three saved
// registers and rebuilds the element address each iteration (here the offset is folded into `this`); 9 variants
// (element pointer/reference, while loop, operator[], helper, reversed comparison)
int EnemyJill::fn_8032FBF4() {
    if (m1570 >= 0) {
        return 0;
    }
    if (mC58.mC4 == 0) {
        return 0;
    }
    if (mC58.mC4->m55C != 6 || mC58.mC4->vf78() == 0) {
        return 0;
    }
    if (mC58.mC4->m638 == 0) {
        return 0;
    }
    float ratio = mStatus.m04 / (float)mStatus.mParamB.m84;
    for (int i = m14EC.mCount - 1; i >= 0; i--) {
        if (fn_80370A78(fn_80121F00(), m14EC.mData[i].m00) == 0 && ratio <= m14EC.mData[i].m08) {
            return 1;
        }
    }
    return 0;
}

extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound

void EnemyJill::fn_8032FD30() {
    JillEvent_unk* e = m14EC.mData;
    for (unsigned int i = 0; i < m14EC.mCount; i++) {
        if (fn_80370A78(fn_80121F00(), e->m00) == 0) {
            fn_80370AE4(fn_80121F00(), e->m00, 1);
            m1570 = i;
            fn_800CB5C8(&m15B0, 20, 0);
            m15B4.mCount = 0;
            if (m1570 + 1 == m14EC.mCount) {
                m1578 = 1;
            }
            break;
        }
        e++;
    }
}

// NONMATCHING: 98.2% - registers of the guard counter start (r0/r3/r4 for the attack data, the state 8 and the
// flag; 5 variants: control local, accessor, SetHit_unk written by hand)
void EnemyJill::fn_8032FDE8() {
    switch (m55C) {
    case 6:
        if (m560 == 10) {
            SetStep_unk();
            m58C = 8;
            m59D = 0;
            m59C = 0;
            SetHit_unk(m14D4);
            m59F = 1;
            ((JillControl_unk*)mControl.get())->m1C = 1;
        } else if (m560 == 0x27) {
            m14BC = 0;
        }
        break;
    case 7:
        fn_8016E970(3);
        break;
    default:
        if (m1590 == 0) {
            mFlag5 = 1;
        }
        mFC0 = mC58.mC4;
        fn_8016E970(3);
        if (m17BC > 5 && (mStatus.m0C & 0x1000000) != 0x1000000) {
            if (rand() % 3 < 2 && fn_8032F314()) {
                m15A0 = 0;
                SetState_unk(11);
                fn_80155A88(m14C0, 0);
            } else {
                int n = m157C + 1;
                if (m15A0 > n) {
                    n = m15A0;
                }
                m15A0 = n;
                m17C0 = 1;
            }
            m17BC = 0;
        }
        break;
    }
}

void EnemyJill::fn_8032FFF8() {
    if (m55C == 7) {
        if ((mC58.mC4->mStatus.m0C & 0x20) == 0x20) {
            ResetState_unk();
            m58C = 6;
        }
        if (mRoute.m2C != 0) {
            if (!fn_8032F0EC()) {
                ResetState_unk();
                m58C = 6;
            }
        } else if (m14BC > 120) {
            ResetState_unk();
            m58C = 6;
        }
    } else {
        ResetState_unk();
        m58C = 6;
    }
}

// inferred: the float argument is declared before the last int: the order of the arguments (evaluated right to
// left) puts the load of the last int after the float; the ABI is the same with any position of the float
// inferred: the real prototype ends with (int c, float f) as in the other units; here the float is declared
// first because only this order gives the load order of the original at the three call sites (fn_80330218,
// fn_8033135C: with (int, float) they drop to 99.45% / 96.87%, the same residue as EnemyTrista::fn_8035CF78).
// The registers are the same (r10 and f1), so the call is equivalent.
extern "C" int fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                           float f, int c);
extern "C" int fn_80041A28(int arg, CharaTask* chara, CharaTask* target); // inferred: kind of attack

// inferred: turns toward the target by `t` of the angle and starts the normal attack (inlined twice in
// fn_80330218; names invented)
// inferred: angle from `from` toward `to` by `t` along the shortest way (name invented)
inline float LerpAngle_unk(float to, float from, float t) {
    float d = to - from;
    if (d > 3.1415927f) {
        return t * (d - 6.2831855f) + from;
    } else if (d < -3.1415927f) {
        return t * (6.2831855f + d) + from;
    }
    return t * d + from;
}

inline void EnemyJill::TurnTo_unk(float t) {
    if (mC58.mC4 != 0) {
        atn::Vector3 dir = mC58.mC4->mPos - mPos;
        if (!(atn::Abs_unk(PSVECSquareMag(&dir)) < 1.1920929e-7f)) {
            PSVECNormalize(&dir, &dir);
        }
        atn::Vector3 rot = dir.ToRotation_unk();
        float y = LerpAngle_unk(rot.y, mRot.y, t);
        atn::WrapAngle_unk(y - mRot.y);
        mRot.y = y;
        atn::WrapAngle_unk(mRot.y);
    }
}

inline void EnemyJill::AttackTarget_unk(float t) {
    TurnTo_unk(t);
    int kind = fn_80041A28(0, this, mC58.mC4);
    if (m17C0 != 0 && mC58.mC4->fn_8016ADF4(this)) {
        kind = 5;
    }
    m17C0 = 0;
    if (kind == 2) {
        fn_8015783C(m14D0, mC58.mC4->mPos, 3.0f);
    } else if (kind == 5) {
        fn_8015802C(m14D0, mC58.mC4);
    } else {
        fn_801562A0(m14D0, mC58.mC4->mPos, 0, 0, 0, 0, 3.0f);
    }
}

void EnemyJill::fn_80330218() {
    switch (m55C) {
    case 6:
        if (!fn_80158BB4()) {
            if (m560 == 4 || m560 == 9) {
                float frame = mMotion.mTracks[0].m08;
                if (frame >= mMotion.fn_80097D7C(0) - 6.0001f) {
                    mMotion.mTracks[0].m0C = 6.0f / (6.0f + m1580);
                }
            }
            if (lbl_8087F048 != 0 && fn_80158CA4()) {
                if (mControl && mControl->vf30() > m598) {
                    m598 = mControl->vf30();
                    m594 = 1;
                    m590 = fn_800F8548(lbl_8087F048);
                }
                if (m594 != 0) {
                    fn_80144710();
                    int n = fn_800F8C6C(lbl_8087F048, 0, 0, this, m638, m590, 30, 0.0f, m594);
                    if (n != 0) {
                        CharaTask* player = lbl_8087F8A0->mHead;
                        if (player != 0 && player->mFlag5) {
                            m1590 = 1;
                            m1594 = 30;
                        } else {
                            m1590 = 0;
                        }
                    }
                    if (m594 > 0) {
                        m594 -= n;
                    }
                }
            }
        } else {
            if (m560 == 4 || m560 - 9U <= 1) {
                fn_8016E970(3);
            }
            ResetState_unk();
            m58C = 6;
        }
        break;
    case 7:
        if ((mC58.mC4->mStatus.m0C & 0x20) == 0x20) {
            ResetState_unk();
            m58C = 6;
        }
        if (mRoute.m2C != 0) {
            AttackTarget_unk(1.0f);
        } else {
            atn::Vector3 d = mC58.mC4->mPos - mPos;
            if (PSVECMag(&d) < 25.0f) {
                AttackTarget_unk(1.0f);
            } else if (m14BC > 120) {
                ResetState_unk();
                m58C = 6;
            }
        }
        break;
    default:
        ResetState_unk();
        m58C = 6;
        break;
    }
}

// inferred: camera of the field (lbl_8087F430): mode at 0x4D8, default parameters at 0x588, character followed
// at 0x8A0 and look point at 0x97C (as in game/EnemyTrista.cpp); names invented
struct JillCamParam_unk {
    JillCamParam_unk(const JillCamParam_unk& o)
        : m00(o.m00), m04(o.m04), m08(o.m08), m0C(o.m0C), m10(o.m10), m14(o.m14), m18(o.m18), m1C(o.m1C),
          m20(o.m20), m24(o.m24), m28(o.m28), m2C(o.m2C), m30(o.m30), m34(o.m34), m38(o.m38) {}
    JillCamParam_unk& operator=(const JillCamParam_unk& o) {
        m00 = o.m00;
        m04 = o.m04;
        m08 = o.m08;
        m0C = o.m0C;
        m10 = o.m10;
        m14 = o.m14;
        m18 = o.m18;
        m1C = o.m1C;
        m20 = o.m20;
        m24 = o.m24;
        m28 = o.m28;
        m2C = o.m2C;
        m30 = o.m30;
        m34 = o.m34;
        m38 = o.m38;
        return *this;
    }
    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    float m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    int m38;
};
struct JillCamera_unk {
    void Set_unk(int mode, const JillCamParam_unk& param) {
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
    JillCamParam_unk mParam;
};
struct JillLook_unk {
    unsigned char m00;
    unsigned char m01;
    unsigned char m02[2];
    int m04;
    unsigned char m08[4];
    atn::Vector3 mPos;
    unsigned char m18[0x24 - 0x18];
    float m24;
    void Set_unk(const atn::Vector3& pos, bool flag) {
        m01 = m00;
        m00 = 1;
        mPos = pos;
        m24 = 0.0f;
        m04 = flag ? 1 : 0;
    }
};
struct JillCamField_unk {
    unsigned char m0000[0x4D8];
    JillCamera_unk m4D8;
    unsigned char m520[0x588 - 0x520];
    JillCamParam_unk m588;
    unsigned char m5C4[0x8A0 - 0x5C4];
    CharaTask* m8A0;
    unsigned char m8A4[0x97C - 0x8A4];
    JillLook_unk m97C;
};

// NONMATCHING: 90.8% - the original addresses the camera mode (0x4D8) and the look point (0x97C) through base
// registers (addi/lbzu) and loads the default parameters before storing the followed character (6 variants:
// local pointers, helper, order of the parameter stores)
void EnemyJill::fn_80330D74() {
    JillCamField_unk* field = (JillCamField_unk*)lbl_8087F430;
    switch (m55C) {
    case 6:
        if (m560 == 13) {
            break;
        }
        if (m560 == 14) {
            atn::Vector3 pos(mPos);
            pos.y += 15.0f;
            if (m14CC == 0) {
                field->m8A0 = this;
                JillCamParam_unk param = field->m588;
                param.m08 = -0.43633232f;
                param.m04 = 0.7853982f;
                param.m00 = 15.0f;
                param.m10 = 8.0f;
                field->m4D8.Set_unk(4, param);
                atn::Vector3 look(mPos);
                look.y += 25.0f;
                field->m97C.Set_unk(look, false);
                m14CC = 1;
            } else {
                field->m8A0 = this;
                field->m97C.mPos = pos;
            }
            break;
        }
        if (m14CC != 0) {
            field->m8A0 = 0;
            field->m4D8.mMode = 0;
            field->m97C.m00 = 0;
            m14CC = 0;
        }
        ResetState_unk();
        m58C = 6;
        break;
    case 7:
        break;
    default:
        ResetState_unk();
        m58C = 6;
        break;
    }
}

void EnemyJill::fn_80331094() {
    switch (m55C) {
    case 6:
        if (m560 != 2) {
            ResetState_unk();
            m58C = 6;
            m14BC = m157C / 2;
        }
        break;
    case 7:
        break;
    default:
        if (rand() % 3 > 0 && fn_8032F204()) {
            SetState_unk(9);
            fn_8016D74C(m14D8, 0, 0);
        } else {
            ResetState_unk();
            m58C = 6;
            m14BC = m157C / 2;
        }
        break;
    }
}

// inferred: identity rotated by z, y and x (name invented, as in game/ChaserNpc.cpp)
inline void JillRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    if (rot.z != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.y != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.x != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
}

// inferred: fires the hit `data` (names invented)
inline void EnemyJill::FireHit_unk(HitInfo_unk* data) {
    fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 10, 0.0f, m594);
}

void EnemyJill::fn_8033135C() {
    atn::Vector3 move(0.0f, 0.0f, 5.0f / mMotion.fn_80097D7C(0));
    atn::Matrix3x4 m;
    JillRotZYX_unk(m, mRot);
    PSMTXMultVec(m.m, &move, &move);
    mPos += move;
    if (m55C == 6) {
        if (m560 == 0x16) {
            ResetState_unk();
            m58C = 6;
            return;
        }
        if (mMotion.mTracks[0].m08 >= 15.0f && mMotion.mTracks[0].m08 <= 20.0f) {
            FireHit_unk(m14E0);
        } else if (mMotion.mTracks[0].m08 >= 27.0f && mMotion.mTracks[0].m08 <= 32.0f) {
            FireHit_unk(m14E0);
        } else if (mMotion.mTracks[0].m08 >= 46.0f && mMotion.mTracks[0].m08 <= 51.0f) {
            if (fn_800F8C6C(lbl_8087F048, 0, 0, this, m14D0, m590, 10, 0.0f, m594) != 0) {
                CharaTask* player = lbl_8087F8A0->mHead;
                if (player != 0 && player->mFlag5) {
                    m1590 = 1;
                    m1594 = 30;
                } else {
                    m1590 = 0;
                }
            }
        }
        float end = mMotion.mTracks[0].m08;
        if (end >= mMotion.fn_80097D7C(0)) {
            ResetState_unk();
            m58C = 6;
        }
    } else {
        ResetState_unk();
        m58C = 6;
    }
}

void EnemyJill::fn_803317F4() {
    if (m55C != 6) {
        ResetState_unk();
        m58C = 6;
    }
}

// NONMATCHING: 53.9% - interpolation of the color matrix toward m1660[m1574]: the original keeps the four rows
// of the current matrix and of the step in FPRs (f15-f31, r27-r31 saved) with a different schedule (2 variants:
// loop, unrolled rows)
void EnemyJill::fn_80331898() {
    if (m55C == 6) {
        if (mMotion.mTracks[0].m08 >= 10.0f) {
            JillParams_unk* params = lbl_8087EFA8;
            JillColMat_unk cur(params->m324);
            JillColMat_unk& dst = m1660[m1574];
            cur.mRows[0] = cur.mRows[0] + (dst.mRows[0] - cur.mRows[0]) * 0.1f;
            cur.mRows[1] = cur.mRows[1] + (dst.mRows[1] - cur.mRows[1]) * 0.1f;
            cur.mRows[2] = cur.mRows[2] + (dst.mRows[2] - cur.mRows[2]) * 0.1f;
            cur.mRows[3] = cur.mRows[3] + (dst.mRows[3] - cur.mRows[3]) * 0.1f;
            params->m324 = cur;
        }
    } else {
        fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
        ResetState_unk();
        m58C = 6;
    }
}

extern "C" double fmod(double x, double y);

// inferred: starts the absorb (state 16) unless the target is already held (names invented)
inline void EnemyJill::StartAbsorb_unk() {
    SetState_unk(16);
    fn_80161570(0x143, 1, 0, -1.0f, -1.0f);
    fn_80232B7C(this, 1000);
    PlayEffect_unk(&m17A4, &mMotion);
}

void EnemyJill::fn_80331DAC() {
    if (m55C == 6) {
        if (m560 != 0x27) {
            if (mC58.mC4->m55C != 6 || mC58.mC4->m560 != 0x82) {
                ResetState_unk();
                m58C = 6;
            } else {
                StartAbsorb_unk();
            }
        } else if (atn::Abs_unk(mMotion.mTracks[0].m08 - 20.0f) < 1.1920929e-7f) {
            CharaTask* target = mC58.mC4;
            atn::Vector3 d = target->mPos - mPos;
            atn::Vector3 rot = d.GetNormalize().ToRotation_unk();
            float a = fmod(57.295776f * (rot.y - mRot.y), 360.0);
            if (a > 180.0f) {
                a -= 360.0f;
            }
            if (a < -180.0f) {
                a += 360.0f;
            }
            float dist = PSVECMag(&d);
            if (-25.0f < a && a < 25.0f && dist < 40.0f) {
                mC58.mC4->fn_8015C988((int)this, (int)m14E4);
            }
        }
    } else if (mC58.mC4->m55C != 6 || mC58.mC4->m560 != 0x82) {
        ResetState_unk();
        m58C = 6;
    } else {
        StartAbsorb_unk();
    }
}

void EnemyJill::fn_803323A0() {
    if (m55C == 6) {
        if (mC58.mC4->m55C == 6 && mC58.mC4->m560 == 0x82) {
            return;
        }
        SetState_unk(17);
        fn_80161570(0x144, 0, 0, -1.0f, -1.0f);
        fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
    } else {
        SetState_unk(17);
        fn_80161570(0x144, 0, 0, -1.0f, -1.0f);
        fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
    }
}

void EnemyJill::fn_8033250C() {
    if (m55C == 6) {
        if (m560 != 0x27) {
            ResetState_unk();
            m58C = 6;
        }
    } else {
        ResetState_unk();
        m58C = 6;
    }
}

void EnemyJill::fn_80332628() {
    ResetState_unk();
    m58C = 6;
}

void EnemyJill::fn_803326C0() {
    SetStep_unk();
    m58C = 8;
    atn::Vector3 d = mC58.mC4->mPos - mPos;
    if (PSVECMag(&d) < 25.0f) {
        mFlag5 = 0;
        AttackTarget_unk(1.0f);
    } else {
        fn_80170A20(mC58.mC4, 0, 30.0f);
    }
}

void EnemyJill::fn_80332B44() {
    SetState_unk(9);
    fn_8016D74C(m14D8, 0, 0);
}

void EnemyJill::fn_80332BE0() {
    TurnTo_unk(1.0f);
    SetState_unk(12);
    fn_80161570(0x145, 0, 0, -1.0f, -1.0f);
}

void EnemyJill::fn_80332F78() {
    SetState_unk(14);
    fn_80161570(0x140, 0, 0, -1.0f, -1.0f);
    mMotion.mTracks[0].m0C = 0.9f;
}

void EnemyJill::fn_80333024() {
    TurnTo_unk(1.0f);
    SetState_unk(15);
    fn_80161570(0x142, 0, 0, -1.0f, -1.0f);
}

void EnemyJill::fn_803333BC() {
    SetState_unk(18);
    fn_80161570(0x140, 0, 0, -1.0f, -1.0f);
    fn_80232B7C(this, 1000);
    PlayEffect_unk(&m17A4, &mMotion);
}
