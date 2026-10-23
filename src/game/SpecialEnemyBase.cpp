#include "game/SpecialEnemyBase.h"
#include "atn/Csv.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

#include "game/ChaserGuard.h"
#include "game/Em314Ivy.h"
#include "game/Em316LaserSphere.h"
#include "game/Em316LastLaser.h"
#include "game/ChaserNpc.h"
#include "game/CopyRobotTask.h"
#include "game/Em002Skelton.h"
#include "game/Em005DoppelGanger.h"
#include "game/Em101ElderZesyaSecond.h"
#include "game/Em101LittleZesyaSecond.h"
#include "game/Em002_01BlackSkelton.h"
#include "game/Em002_02SkeltonKing.h"
#include "game/Em006DarkMaster.h"
#include "game/Em101ElderZesya.h"
#include "game/Em001Necromancer.h"
#include "game/Em201ZanGurg.h"
#include "game/Em201ZanGurgFinal.h"
#include "game/Em204Mulue.h"
#include "game/Em205Ogre.h"
#include "game/Em301LShade.h"
#include "game/Em302Kraken.h"
#include "game/Em302KrakenMp.h"
#include "game/Em303KrakenFoot.h"
#include "game/Em303KrakenFootMp.h"
#include "game/Em306GurgBeast.h"
#include "game/Em308InvertCrab.h"
#include "game/Em308ParmCrab.h"
#include "game/Em309GhoulSimple.h"
#include "game/Em312DarkServant.h"
#include "game/Em315Coffin.h"
#include "game/Em401_01_WhiteLeoPard.h"
#include "game/Em403WarLion.h"
#include "game/Em202Spider.h"
#include "game/Em202SpiderBase.h"
#include "game/Em203GreaterDaemon.h"
#include "game/Em208Queen.h"
#include "game/Em209BigGolem.h"
#include "game/Em209SmallGolem.h"
#include "game/Em305VampireMp.h"
#include "game/EnemyFieldMagician.h"
#include "game/EnemyJill.h"
#include "game/EnemyLizerdBlade.h"
#include "game/EnemyQuarkFirst.h"
#include "game/EnemyQuarkSecond.h"
#include "game/EnemyQuarkStand.h"
#include "game/EnemySeaLizerd.h"
#include "game/EnemyTasha.h"
#include "game/EnemyTrista.h"
#include "game/LastCocoonChrysalis.h"
#include "game/Np131LivingArmor.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int atoi(const char* s);
extern "C" double cos(double x);
extern "C" double acos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;  // inferred: player manager
extern "C" void fn_800EAECC(CharaTask* chara);       // inferred: BattleCharaTask
extern "C" void fn_8011CD84(CharaD74_unk* label, CharaTask* chara, int kind); // inferred: shows a label
extern "C" CharaInfo_unk* fn_8020924C(int id);
// inferred: debug menu entries (as in gmk/SwingObject.cpp)
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
// inferred: field (lbl_8087F430); event flags (fn_80370A78/fn_80370AE4) and global flags
// (fn_80370174/fn_80370320); m74/m80: camera position and target, m13C: camera matrix, m868: camera
// mode
struct Field_unk {
    unsigned char m0000[0x74];
    atn::Vector3 m74;
    atn::Vector3 m80;
    unsigned char m008C[0x13C - 0x8C];
    Mtx m13C;
    unsigned char m016C[0x868 - 0x16C];
    int m868;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
extern "C" int fn_80370174(Field_unk* field, int index);
extern "C" void fn_80370320(Field_unk* field, int index, int value, int arg);
struct Camera_unk;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" void fn_800BFAC8(atn::Vector3* out, Camera_unk* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" int fn_80051B70(const atn::Vector3* from, const atn::Vector3* to); // inferred: visibility test
extern "C" int fn_803ABE54(const atn::Vector3* from, const atn::Vector3* to, float radius);
// inferred: arena of the multiplayer mode (lbl_8087F490)
struct MpArena_unk;
extern "C" MpArena_unk* lbl_8087F490;
extern "C" void fn_803E6ADC(MpArena_unk* arena, const atn::Vector3* a, const atn::Vector3* b);
extern "C" void fn_803E3050(MpArena_unk* arena, const atn::Vector3* pos, const void* rect, const void* size,
                            float dist, float arg);
extern "C" atn::SoundRef fn_800C31F4(const char* name, float volume, int arg2, int arg3);
// inferred: gimmicks (lbl_8087F4A0): list of objects with a kind (m50), a position (vf54), a size
// (vf50) and a message handler (vf44); names invented
struct GimmickObj_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44(void* msg);
    virtual void vf48();
    virtual void vf4C();
    virtual struct GimmickSize_unk* vf50();
    virtual atn::Vector3 vf54();
    unsigned char m04[0x50 - 0x04];
    int m50;
    unsigned char m54[0x5C - 0x54];
    GimmickObj_unk* mNext;
};
struct GimmickSize_unk {
    unsigned char m00[0x48];
    float m48;
};
struct GimmickList_unk {
    unsigned char m00[0x48];
    GimmickObj_unk* mHead;
};
extern "C" GimmickList_unk* lbl_8087F4A0;

// inferred: reference to a flag of the data files ("G" = global flag, else event flag); names invented
struct SpecialFlag_unk {
    int mGlobal;
    int mIndex;
};
// inferred: target the player can aim at (Em301LShade + 0x1658); names invented
struct SpecialAim_unk {
    MtxPtr mNode;        // inferred: node matrix
    atn::Vector3 mOffset;
    float mRadius;
    unsigned char m14[0x1C - 0x14];
    unsigned char mEnabled;
};
struct SpecialRect_unk {
    float mHeight;
    float mWidth;
    float mX;
    float mY;
};

static const char* const sSound = "SE_UICOM_011";

SpecialEnemyBase::SpecialEnemyBase(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0((int)info), m14BC(0), m14C0(0), m14C4(0), m14C8(0) {
    mFlag31 = 0;
    mFlag30 = 0;
    mFlag29 = 0;
    mFlag28 = 0;
    mFlag27 = 1;
    m14D0 = 0;
    mStatus.m18 |= 0x41D1;
    mStatus.m18 |= 0x2010000;
}

SpecialEnemyBase::~SpecialEnemyBase() {}

// NONMATCHING: 99.9% - register of the ".csv" literal (r27 instead of the reused r29)
int SpecialEnemyBase::fn_805A3D68(char* name, const char* params) {
    if (params == 0 || *params == '\0') {
        return 0;
    }
    const char* s = strstr(params, "db_name=");
    if (s != 0) {
        std::string str(s + 8);
        std::string::size_type pos = str.find(".csv");
        if (pos != std::string::npos) {
            str = std::string(str, 0, pos + 4);
        }
        if (name != 0) {
            sprintf(name, "%s", str.c_str());
        }
        return 1;
    }
    return 0;
}

int SpecialEnemyBase::vfF8() {
    int result = 0;
    if (mC58.mC0 != 0 && mC58.m11C.fn_8011D21C() != 0 && mC58.mC4 != 0 && mC58.mC4->mC58.mC0 != 0) {
        result = 1;
    }
    return result;
}

int SpecialEnemyBase::vf10C(HitMsg_unk* msg) {
    if (mFlag29) {
        return 0;
    }
    if (mFlag28) {
        msg->m10 *= 0.0f;
    }
    return 1;
}

void SpecialEnemyBase::vf3C(HitInfo_unk* hit) {
    if (m58C != 2 && vf10C((HitMsg_unk*)hit) != 0) {
        fn_80151448((HitMsg_unk*)hit);
        if (!mFlag2) {
            vf40(hit);
        } else {
            vf118(hit);
        }
    }
}

void SpecialEnemyBase::fn_805A40B8() {
    if (!mFlag25) {
        if (m55C == 2) {
            float frame = mMotion.mTracks[0].m08;
            if (frame < mMotion.fn_80097D7C(0) && mMotion.mTracks[0].m18 == 0) {
                goto end;
            }
        }
        if (m55C != 6 || fn_8015EB2C() != 0) {
            goto dead;
        }
    end:
        vf108();
        m1450 = 0;
        return;
    }
dead:
    mColli.mFlags &= ~1;
    mFlag25 = 1;
    fn_800EAECC(this);
}

void SpecialEnemyBase::fn_805A4188(HitMsg_unk* msg) {
    if ((mStatus.m0C & 0x40) != 0x40 && mC58.m11C.fn_8011D21C() == 0) {
        if (msg->mAttacker != 0 && msg->mAttacker->mFlag28) {
            fn_8011CD84(&mC58.m11C, this, 1);
        } else if (mC58.m11C.m08 >= 0 && mC58.m11C.m00 >= 2) {
            if (msg->m44 == 1) {
                fn_8011CD84(&mC58.m11C, this, 4);
            } else {
                fn_8011CD84(&mC58.m11C, this, 1);
            }
        }
    }
}

void SpecialEnemyBase::fn_805A4254() {
    float speed = 0.0f;
    float scale = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 2) {
        if (m1400.mSize >= 2 && fn_8017B3C0()) {
            fn_8017B434();
        }
    } else if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 v = mRoute.m58;
        speed = v.Length_unk();
        if (speed > 0.0001f) {
            rot = v.GetNormalized_unk().ToRotation_unk();
        } else {
            rot = mRot;
        }
        if ((*(unsigned int*)((char*)this + 0xD94) & 1) == 1) {
            scale *= 0.3f;
        }
        if (m1400.mSize >= 2) {
            bool loop = false;
            if (m140C >= 0 && mFlagB14) {
                loop = true;
            }
            if (loop) {
                if (mRoute.m2C != 0) {
                    fn_8017B434();
                } else if (!fn_8017B3C0()) {
                    fn_80171DB0();
                    fn_8017BC4C();
                }
            }
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8013CB68(&rot, 0, speed, m568 * scale);
}

// NONMATCHING: 98.2% - registers and load order of the two character loops
int SpecialEnemyBase::fn_805A45F8(float radius, float angle, float rotOffset) {
    int count = 0;
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', mRot.y);
    PSMTXMultVec(m, &front, &front);
    Mtx m2;
    PSMTXRotRad(m2, 'y', rotOffset);
    PSMTXMultVec(m2, &front, &front);
    float limit = cos(0.017453292f * angle);
    if (lbl_8087F8A0 != 0) {
        for (CharaTask* c = (CharaTask*)lbl_8087F8A0->mHead; c != 0; c = ((EnemyTask*)c)->mNext) {
            if (c->CanAct_unk() && (c->m54C & 0x2000) != 0x2000 && !c->fn_8016F3D0(this, 1)) {
                atn::Vector3 d = c->mPos - mPos;
                float len = fn_805F9920(&d);
                if (len > 0.0f) {
                    PSVECNormalize(&d, &d);
                }
                if (len <= radius * radius && !(PSVECDotProduct(&front, &d) < limit)) {
                    count++;
                }
            }
        }
    }
    if (EnemyManager::sInstance != 0) {
        for (EnemyTask* c = EnemyManager::sInstance->mHead; c != 0; c = c->mNext) {
            if (c->CanAct_unk() && (c->m54C & 0x2000) != 0x2000 && !c->fn_8016F3D0(this, 1)) {
                atn::Vector3 d = c->mPos - mPos;
                float len = fn_805F9920(&d);
                if (len > 0.0f) {
                    PSVECNormalize(&d, &d);
                }
                if (len <= radius * radius && !(PSVECDotProduct(&front, &d) < limit)) {
                    count++;
                }
            }
        }
    }
    return count;
}

void SpecialEnemyBase::fn_805A4980(SpecialFlag_unk* flag, atn::Csv* csv) {
    flag->mGlobal = *csv->fn_8005B9CC() == 'G';
    flag->mIndex = atoi(csv->fn_8005B9CC());
}

int SpecialEnemyBase::fn_805A49E4(SpecialFlag_unk* flag) {
    if (lbl_8087F430 != 0) {
        if (flag->mGlobal == 0) {
            return fn_80370A78(lbl_8087F430, flag->mIndex);
        }
        if (flag->mGlobal == 1) {
            return fn_80370174(lbl_8087F430, flag->mIndex);
        }
    }
    return -1;
}

void SpecialEnemyBase::fn_805A4A1C(SpecialFlag_unk* flag, int value) {
    if (lbl_8087F430 != 0) {
        if (flag->mGlobal == 0) {
            fn_80370AE4(lbl_8087F430, flag->mIndex, value);
        } else if (flag->mGlobal == 1) {
            fn_80370320(lbl_8087F430, flag->mIndex, value, 0);
        }
    }
}

// NONMATCHING: 66.3% - approximation of the aim test (camera ray against the target, screen rectangle)
int SpecialEnemyBase::fn_805A4A54(SpecialAim_unk* aim) {
    if (aim->mNode == 0 || aim->mRadius <= 0.0f || aim->mEnabled == 0) {
        return 0;
    }
    Field_unk* field = lbl_8087F430;
    int mode = field->m868;
    if (mode == 3 || mode == 1 || mode == 4) {
        atn::Vector3 eye = field->m74;
        atn::Vector3 ray = field->m80 - eye;
        ray.Normalize_unk();
        ray = eye + ray * 800.0f;
        atn::Vector3 center(aim->mNode[0][3] + aim->mOffset.x, aim->mNode[1][3] + aim->mOffset.y,
                            aim->mNode[2][3] + aim->mOffset.z);
        float radius = aim->mRadius;
        float dist = (center - eye).Length_unk();
        atn::Vector3 lo(-radius, -radius, 0.0f);
        atn::Vector3 hi(radius, radius, 0.0f);
        Mtx m;
        __MtxCopy(m, field->m13C);
        m[0][3] = 0.0f;
        m[1][3] = 0.0f;
        m[2][3] = 0.0f;
        PSMTXMultVec(m, &lo, &lo);
        PSMTXMultVec(m, &hi, &hi);
        atn::Vector3 screenLo;
        atn::Vector3 posLo = center + lo;
        fn_800BFAC8(&screenLo, lbl_8087EFB4, &posLo);
        atn::Vector3 screenHi;
        atn::Vector3 posHi = center + hi;
        fn_800BFAC8(&screenHi, lbl_8087EFB4, &posHi);
        SpecialRect_unk rect;
        rect.mHeight = 0.5f * atn::Abs_unk(screenLo.y - screenHi.y) + 12.0f;
        rect.mWidth = 0.5f * atn::Abs_unk(screenLo.x - screenHi.x) + 12.0f;
        rect.mX = 0.5f * (screenLo.x + screenHi.x);
        rect.mY = 0.5f * (screenLo.y + screenHi.y);
        if (fn_80051B70(&eye, &center) != 0 && fn_803ABE54(&eye, &center, radius) != 0) {
            atn::Vector3 a = center + lo;
            atn::Vector3 b = center + hi;
            fn_803E6ADC(lbl_8087F490, &a, &b);
            fn_800C31F4(sSound, 1.0f, 0, -1);
            return 1;
        }
        if (dist < 800.0f && fn_803ABE54(&eye, &center, radius) != 0) {
            fn_803E3050(lbl_8087F490, &center, &rect.mX, &rect, dist, screenLo.z);
        }
    }
    return 0;
}

void SpecialEnemyBase::fn_805A4F1C(int motion, int arg2, int arg3, float speed, float blend) {
    mMotion.mTracks[0].m0C = speed;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, motion, arg2, arg3, 1, blend, 0.1f);
}

void SpecialEnemyBase::vf110(SpecialState_unk* state) {
    state->m00 = m58C;
    state->m04 = m14BC;
}

void SpecialEnemyBase::vf114(const SpecialState_unk* state) {
    m58C = state->m00;
    m14BC = state->m04;
}

// inferred: listeners of the characters (function-local static, as in MP/CharaListeners_unk.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}

void SpecialEnemyBase::vf118(HitInfo_unk* hit) {
    CharaListeners_unk* listeners = fn_8014F8F4();
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk* l = &listeners->mList[i];
            if (l->mId == -1 || l->mId == 8) {
                l->mFunc(8, this, hit);
            }
        }
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf44();
    }
}

// NONMATCHING: 98.1% - load order of the position and of the z-first copy of `dir`
// inferred: sends `id` to the gimmicks of kind 1 in a cone (radius, angle in degrees x2)
extern "C" void fn_805A5078(const atn::Vector3* pos, const atn::Vector3* dir, int id, float radius, float angle) {
    for (GimmickObj_unk* g = lbl_8087F4A0->mHead; g != 0; g = g->mNext) {
        if (g->m50 == 1) {
            atn::Vector3 p = g->vf54();
            atn::Vector3 d(p.x - pos->x, 0.0f, p.z - pos->z);
            float dist = d.Length_unk();
            GimmickSize_unk* size = g->vf50();
            if (size != 0) {
                dist -= size->m48;
            }
            if (dist <= radius) {
                PSVECNormalize(&d, &d);
                atn::Vector3 v;
                v.Assign_unk(*dir);
                v.y = 0.0f;
                if ((float)acos(PSVECDotProduct(&d, &v)) <= 0.5f * angle) {
                    int msg[12];
                    memset(msg, 0, sizeof(msg));
                    msg[0] = id;
                    g->vf44(msg);
                }
            }
        }
    }
}

// NONMATCHING: 99.6% - only the offsets of the pool (the original has "" and "basic_string:
// out_of_range" before "position")
// inferred: debug menu entries of a special enemy
void SpecialEnemyBase::fn_805A5220(void* group) {
    fn_80087E9C(group, "position", &mPos, 0, 0, -9999.0f, 9999.0f, 1.0f);
    fn_80088724(group, "rotation", &mRot, 0, 0, -360.0f, 360.0f, 10.0f);
    fn_80087E9C(group, "scale", &m540, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_8008771C(group, "moveSpeed", &m568, 0, 0, 0.0f, 9999.0f, 0.05f);
}

static const char* const sEnemyNames[] = {
    "",
    "NECROMANCER",
    "EM002",
    "EM002_01",
    "EM003",
    "EM005",
    "EM006",
    "EM101_ELDER",
    "EM101_ELDER_SECOND",
    "EM101_LITTLE",
    "EM101_LITTLE_SECOND",
    "EM201",
    "EM202",
    "EM202_01",
    "EM202_BASE",
    "EM202FOREST",
    "EM202MOTHERS",
    "EM203",
    "EM203_RED",
    "EM203_RED_MP",
    "EM204",
    "EM204_BIG",
    "EM204_MP",
    "EM205",
    "EM206",
    "EM206_MP",
    "EM208",
    "EM209_SMALL",
    "EM209_BIG",
    "EM301",
    "EM302",
    "EM303",
    "EM302_MP",
    "EM303_MP",
    "EM305",
    "EM305_MP",
    "EM306",
    "EM308",
    "EM308_INVERT",
    "EM309",
    "EM309_SIMPLE",
    "EM310",
    "EM312",
    "EM314",
    "EM315",
    "EM316",
    "EM316_LAST",
    "EM401_01",
    "EM403",
    "EM601_BIT",
    "QUADRUPED",
    "COPYROBOT",
    "LIVING_ARMOR",
    "CHASER",
    "CHASER_NPC",
    "EM_TRISTA",
    "LAST_COCOON_EGG",
    "LAST_COCOON_CHRYSALIS",
    "LAST_COCOON_QUADRUPED",
    "LAST_COCOON_IMAGO",
    "LAST_COCOON_IMAGO_FINAL",
    "EM_QUARK",
    "EM_QUARK_STAND",
    "EM_JILL",
    "EM_TASHA",
    "ZANGURG_FINAL",
    "FR_ARGANAN",
    "EM_FIELD_MAGICIAN",
    "EM002_02_SKELTON_KING",
    "EM_SEA_LIZERD",
    "EM_QUARK_1ST",
    "EM_QUARK_2ND",
    "EM_LIZERD_BLADE",
};

// NONMATCHING: 99.4% - data of the name table (the "" entry)
// inferred: index of an enemy name (0 if missing)
extern "C" int fn_805A52F4(const char* s) {
    int result = 0;
    for (int i = 0; i < 73; i++) {
        if (strcmp(s, sEnemyNames[i]) == 0) {
            result = i;
            break;
        }
    }
    return result;
}

// inferred: constructors of the enemies that are not decompiled yet (called through new)
extern "C" void* fn_8024A650(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_8025C998(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_8026FC0C(void* self, atn::Task* parent, void* data);
extern "C" void* fn_8027278C(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_80279860(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_80293738(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_8029CB00(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_802BA92C(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_802C0C38(void* self, atn::Task* parent, void* data);
extern "C" void* fn_802FC850(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_8030A144(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_80452F14(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_80458270(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_8045FAB0(void* self, atn::Task* parent, EnemyInfo_unk* info);
extern "C" void* fn_80566420(void* self, atn::Task* parent, void* data);

// NONMATCHING: 73.3% - saved registers (parent/info/kind/data) and the "" of new: the original uses
// a separate 4-byte object after the name table (lbl_807637DC)
// inferred: creates the enemy of the kind given by its character table entry (EnemyTask::fn_8035B534)
extern "C" EnemyTask* fn_805A5374(atn::Task* parent, EnemyInfo_unk* info) {
    void* data = info->m20;
    CharaInfo_unk* chara = fn_8020924C((int)data);
    if (chara != 0) {
        int kind = chara->m24;
        EnemyTask* enemy = 0;
        void* p;
        switch (kind) {
        case 1:
            enemy = new (3, "", "", 0) Em001Necromancer(parent, info);
            break;
        case 50:
            p = ::operator new(0x1568, 3, "", "", 0);
            if (p != 0) {
                p = fn_80566420(p, parent, data);
            }
            enemy = (EnemyTask*)p;
            break;
        case 36:
            enemy = new (3, "", "", 0) Em306GurgBeast(parent, info);
            break;
        case 12:
            enemy = new (3, "", "", 0) Em202Spider(parent, info);
            break;
        case 14:
            enemy = new (3, "", "", 0) Em202SpiderBase(parent, info);
            break;
        case 29:
            enemy = new (3, "", "", 0) Em301LShade(parent, info);
            break;
        case 20:
            enemy = new (3, "", "", 0) Em204Mulue(parent, info);
            break;
        case 21:
            enemy = new (3, "", "", 0) Em204BigMulue(parent, info);
            break;
        case 22:
            enemy = new (3, "", "", 0) Em204MulueMp(parent, info);
            break;
        case 34:
            enemy = new (3, "", "", 0) Em305Vampire(parent, info);
            break;
        case 13:
            p = ::operator new(0x1540, 3, "", "", 0);
            if (p != 0) {
                p = fn_8026FC0C(p, parent, data);
            }
            enemy = (EnemyTask*)p;
            break;
        case 11:
            enemy = new (3, "", "", 0) Em201ZanGurg(parent, info);
            break;
        case 24:
            p = ::operator new(0x1718, 3, "", "", 0);
            if (p != 0) {
                p = fn_802C0C38(p, parent, data);
            }
            enemy = (EnemyTask*)p;
            break;
        case 2:
            enemy = new (3, "", "", 0) Em002Skelton(parent, data);
            break;
        case 3:
            enemy = new (3, "", "", 0) Em002_01BlackSkelton(parent, data);
            break;
        case 39:
            p = ::operator new(0x1700, 3, "", "", 0);
            if (p != 0) {
                p = fn_802FC850(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 40:
            enemy = new (3, "", "", 0) Em309GhoulSimple(parent, info);
            break;
        case 41:
            p = ::operator new(0x1610, 3, "", "", 0);
            if (p != 0) {
                p = fn_8030A144(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 47:
            enemy = new (3, "", "", 0) Em401_01_WhiteLeoPard(parent, info);
            break;
        case 48:
            enemy = new (3, "", "", 0) Em403WarLion(parent, data);
            break;
        case 23:
            enemy = new (3, "", "", 0) Em205Ogre(parent, info);
            break;
        case 26:
            enemy = new (3, "", "", 0) Em208Queen(parent, data);
            break;
        case 52:
            enemy = new (3, "", "", 0) Np131LivingArmor(parent, info);
            break;
        case 43:
            enemy = new (3, "", "", 0) Em314Ivy(parent, info);
            break;
        case 37:
            enemy = new (3, "", "", 0) Em308ParmCrab(parent, info);
            break;
        case 38:
            enemy = new (3, "", "", 0) Em308InvertCrab(parent, info);
            break;
        case 15:
            p = ::operator new(0x1968, 3, "", "", 0);
            if (p != 0) {
                p = fn_8027278C(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 4:
            p = ::operator new(0x16f0, 3, "", "", 0);
            if (p != 0) {
                p = fn_8024A650(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 17:
            enemy = new (3, "", "", 0) Em203GreaterDaemon(parent, info);
            break;
        case 18:
            p = ::operator new(0x16d0, 3, "", "", 0);
            if (p != 0) {
                p = fn_80293738(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 19:
            p = ::operator new(0x16d0, 3, "", "", 0);
            if (p != 0) {
                p = fn_8029CB00(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 5:
            enemy = new (3, "", "", 0) Em005DoppelGanger(parent, info);
            break;
        case 30:
            enemy = new (3, "", "", 0) Em302Kraken(parent, info);
            break;
        case 31:
            enemy = new (3, "", "", 0) Em303KrakenFoot(parent, info);
            break;
        case 32:
            enemy = new (3, "", "", 0) Em302KrakenMp(parent, info);
            break;
        case 33:
            enemy = new (3, "", "", 0) Em303KrakenFootMp(parent, info);
            break;
        case 51:
            enemy = new (3, "", "", 0) CopyRobotTask(parent, info);
            break;
        case 44:
            enemy = new (3, "", "", 0) Em315Coffin(parent, info);
            break;
        case 6:
            enemy = new (3, "", "", 0) Em006DarkMaster(parent, info);
            break;
        case 42:
            enemy = new (3, "", "", 0) Em312DarkServant(parent, info);
            break;
        case 53:
            enemy = new (3, "", "", 0) ChaserGuard(parent, info);
            break;
        case 54:
            enemy = new (3, "", "", 0) ChaserNpc(parent, info);
            break;
        case 55:
            enemy = new (3, "", "", 0) EnemyTrista(parent, info);
            break;
        case 49:
            enemy = new (3, "", "", 0) Em316LaserSphere(parent, info);
            break;
        case 56:
            p = ::operator new(0x2a50, 3, "", "", 0);
            if (p != 0) {
                p = fn_80452F14(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 57:
            enemy = new (3, "", "", 0) LastCocoonChrysalis(parent, info);
            break;
        case 58:
            p = ::operator new(0x18f8, 3, "", "", 0);
            if (p != 0) {
                p = fn_8045FAB0(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 59:
            p = ::operator new(0x1990, 3, "", "", 0);
            if (p != 0) {
                p = fn_80458270(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 7:
            enemy = new (3, "", "", 0) Em101ElderZesya(parent, info);
            break;
        case 9:
            p = ::operator new(0x15d0, 3, "", "", 0);
            if (p != 0) {
                p = fn_8025C998(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 63:
            enemy = new (3, "", "", 0) EnemyJill(parent, info);
            break;
        case 45:
            enemy = new (3, "", "", 0) Em316LaserSphere(parent, info);
            break;
        case 46:
            enemy = new (3, "", "", 0) Em316LastLaser(parent, info);
            break;
        case 27:
            enemy = new (3, "", "", 0) Em209SmallGolem(parent, info);
            break;
        case 28:
            enemy = new (3, "", "", 0) Em209BigGolem(parent, info);
            break;
        case 62:
            enemy = new (3, "", "", 0) EnemyQuarkStand(parent, info);
            break;
        case 70:
            enemy = new (3, "", "", 0) EnemyQuarkFirst(parent, info);
            break;
        case 71:
            enemy = new (3, "", "", 0) EnemyQuarkSecond(parent, info);
            break;
        case 64:
            enemy = new (3, "", "", 0) EnemyTasha(parent, info);
            break;
        case 8:
            enemy = new (3, "", "", 0) Em101ElderZesyaSecond(parent, info);
            break;
        case 10:
            enemy = new (3, "", "", 0) Em101LittleZesyaSecond(parent, info);
            break;
        case 65:
            enemy = new (3, "", "", 0) Em201ZanGurgFinal(parent, info);
            break;
        case 25:
            p = ::operator new(0x1938, 3, "", "", 0);
            if (p != 0) {
                p = fn_802BA92C(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 67:
            enemy = new (3, "", "", 0) EnemyFieldMagician(parent, info);
            break;
        case 68:
            enemy = new (3, "", "", 0) Em002_02SkeltonKing(parent, info);
            break;
        case 69:
            enemy = new (3, "", "", 0) EnemySeaLizerd(parent, info);
            break;
        case 16:
            p = ::operator new(0x19c8, 3, "", "", 0);
            if (p != 0) {
                p = fn_80279860(p, parent, info);
            }
            enemy = (EnemyTask*)p;
            break;
        case 35:
            enemy = new (3, "", "", 0) Em305VampireMp(parent, info);
            break;
        case 72:
            enemy = new (3, "", "", 0) EnemyLizerdBlade(parent, info);
            break;
        }
    if (enemy != 0) {
            enemy->m146C = kind;
            return enemy;
        }
    }
    return 0;
}
