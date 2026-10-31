#include "game/GameUI.h"
#include "game/SpecialEnemyBase.h"
#include "atn/Csv.h"
#include "atn/ColliBase.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
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
#include "game/Em309Ghoul.h"
#include "game/Em309GhoulSimple.h"
#include "game/Em312DarkServant.h"
#include "game/Em315Coffin.h"
#include "game/Em401_01_WhiteLeoPard.h"
#include "game/Em403WarLion.h"
#include "game/Em202Spider.h"
#include "game/Em202_01_BSpider.h"
#include "game/Em202SpiderBase.h"
#include "game/Em203GreaterDaemon.h"
#include "game/Em203GreaterDaemonRed.h"
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
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;  // inferred: player manager
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
extern "C" void fn_803E3050(GameUI* ui, const atn::Vector3* pos, const void* rect, const void* size,
                            float dist, float arg);
extern "C" atn::SoundRef fn_800C31F4(const char* name, float volume, int arg2, int arg3);

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
            sprintf(name, "%s\0", str.c_str());
        }
        return 1;
    }
    return 0;
}

// inferred: instantiated right after its first use, as in atn/FileUtil_unk (0x80069BF4): its
// "basic_string: out_of_range" follows the strings of fn_805A3D68 in the pool (the linker drops the copy)
template std::string::basic_string(const std::string& str, std::string::size_type pos, std::string::size_type n);

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
    fn_800EAECC();
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
            rot = v.GetNormalize().ToRotation_unk();
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
    fn_8013CB68(rot, 0, speed, m568 * scale);
}

// NONMATCHING: 98.2% - registers and load order of the two character loops
int SpecialEnemyBase::fn_805A45F8(float radius, float angle, float rotOffset) {
    int count = 0;
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(mRot.y);
    PSMTXMultVec(m.m, &front, &front);
    Mtx m2;
    PSMTXRotRad(m2, 'y', rotOffset);
    PSMTXMultVec(m2, &front, &front);
    float limit = cos(0.017453292f * angle);
    if (lbl_8087F8A0 != 0) {
        for (CharaTask* c = (CharaTask*)lbl_8087F8A0->mHead; c != 0; c = ((EnemyTask*)c)->mNext) {
            if (c->CanAct_unk() && (c->m54C & 0x2000) != 0x2000 && !c->fn_8016F3D0(this, 1)) {
                atn::Vector3 d = c->mPos - mPos;
                float len = PSVECSquareMag(&d);
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
                float len = PSVECSquareMag(&d);
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
            lbl_8087F490->fn_803E6ADC(a, b);
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
    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        if (g->m50 == 1) {
            atn::Vector3 p = g->vf54();
            atn::Vector3 d(p.x - pos->x, 0.0f, p.z - pos->z);
            float dist = d.Length_unk();
            atn::ColliSphere* size = (atn::ColliSphere*)g->vf50();
            if (size != 0) {
                dist -= size->mSphere.mRadius;
            }
            if (dist <= radius) {
                PSVECNormalize(&d, &d);
                atn::Vector3 v;
                v.Assign_unk(*dir);
                v.y = 0.0f;
                if ((float)acos(PSVECDotProduct(&d, &v)) <= 0.5f * angle) {
                    GimmickHitMsg_unk msg;
                    memset(&msg, 0, sizeof(msg));
                    msg.mType = id;
                    g->vf44(&msg);
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
