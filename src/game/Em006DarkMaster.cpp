#include "game/Em006DarkMaster.h"
#include "atn/Csv.h"
#include "atn/SoundRef.h"
#include "game/EnemyManager.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void* lbl_8087F048; // inferred: effect manager of the characters
extern "C" void fn_8010653C(void* mgr, CharaTask* chara, void* motion);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: length
// inferred: field (lbl_8087F430); m10D8: route tables
struct Field_unk {
    unsigned char m0000[0x10D8];
    void* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: route search (lbl_8087EE68); fn_80018D5C finds a route point around `pos` between the
// distances min and max (names invented)
struct RouteSearch_unk {
    int fn_80018D5C(const atn::Vector3* pos, int group, int arg3, int arg4, int arg5, int mask, int arg7, float dir,
                    float range, float min, float max);
};
extern "C" RouteSearch_unk* lbl_8087EE68;
extern "C" atn::Vector3 fn_803C11A4(void* routes, int point); // inferred: position of a route point

// inferred: servant of the master (enemy of kind 42); its master is at 0x14D4
struct DarkServant_unk : public EnemyTask {
    unsigned char m14B0[0x14D4 - 0x14B0];
    Em006DarkMaster* mMaster;
};
extern "C" int fn_8030E490(EnemyTask* servant); // inferred: nonzero once the servant is down
extern "C" void fn_8030E4AC(EnemyTask* servant);
extern "C" void fn_8030E530(EnemyTask* servant);

// inferred: normalizes unless the vector is (almost) zero; name invented (as in gmk/WaterLevel.cpp)
inline void SafeNormalize_unk(atn::Vector3* v) {
    if ((float)__fabs(fn_805F9920(v)) < 1.1920929e-7f) {
        return;
    }
    PSVECNormalize(v, v);
}

Em006DarkMaster::Em006DarkMaster(atn::Task* parent, EnemyInfo_unk* info) : SpecialEnemyBase(parent, info), m14D4(0) {
    m54C |= 0x02000000;
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after this string and the next one
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em006_00_00.csv\0");
    }
    m14B4.vf0C(path);
}

Em006DarkMaster::~Em006DarkMaster() {}

int Em006DarkMaster::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        return 0;
    }
    fn_80254808((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    m14D8.mCount = 0;
    m14A8 &= ~0x04000000;
    for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
        if (e->m146C == 42) {
            m14D8.PushBack_unk(e);
        }
    }
    vfFC();
    return 1;
}

void Em006DarkMaster::fn_80254808(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        csv.fn_8005B9CC();
    } while (csv.fn_8005BBF8());
}

void Em006DarkMaster::vf10() {
    if (!CharaTask::mFlag2 || !CanAct_unk()) {
        vf104();
        fn_802549C8();
    }
    fn_8014C540();
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
}

void Em006DarkMaster::fn_802549C8() {
    if (CanAct_unk()) {
        m14D4 = 0;
        if (EnemyManager::sInstance != 0) {
            for (unsigned int i = 0; i < m14D8.mCount; i++) {
                DarkServant_unk* e = (DarkServant_unk*)m14D8.mData[i];
                if (e->mMaster == this && e->CanAct_unk() && fn_8030E490(e) == 0) {
                    m14D4++;
                }
            }
            if (m58C != 9 && m14D4 <= 0) {
                vf100(9);
            }
        }
    }
}

void Em006DarkMaster::vf14() {
    fn_80149A30();
}

int Em006DarkMaster::vf10C(HitMsg_unk* msg) {
    if (m58C == 9 && msg->mAttacker != 0 && fn_80219558(msg->mAttacker->m50) == 0) {
        return 1;
    }
    if (msg->mData != 0 &&
        (((unsigned int)msg->mData->m90.mFlags & 4) == 4 || (msg->mAttacker != 0 && msg->mAttacker->mStatus.fn_80133EE8(0x19) != 0))) {
        mStatus.fn_8012DF7C(0, 4, msg->mData);
    }
    msg->m10 = lbl_807C7030;
    msg->m48 = 0;
    msg->m90 = 0;
    msg->m68 = 0;
    msg->m8C = 0;
    msg->m84 = 2;
    vf40((HitInfo_unk*)msg);
    return 0;
}

void Em006DarkMaster::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
        return;
    }
    if (m58C != 9 || (msg->mAttacker != 0 && fn_80219558(msg->mAttacker->m50) == 0)) {
        if (m58C == 9) {
            for (unsigned int i = 0; i < m14D8.mCount; i++) {
                DarkServant_unk* e = (DarkServant_unk*)m14D8.mData[i];
                if (e->mMaster == this && e->CanAct_unk() && fn_8030E490(e) != 0) {
                    fn_8030E530(e);
                }
            }
            mStatus.fn_8012F440(4, 0, 1, 0, 0, 0, 1);
        }
        if ((mStatus.m0C & 4) == 4) {
            for (unsigned int i = 0; i < m14D8.mCount; i++) {
                DarkServant_unk* e = (DarkServant_unk*)m14D8.mData[i];
                if (e->mMaster == this && e->CanAct_unk()) {
                    fn_8030E4AC(e);
                }
            }
        }
        vf100(8);
    } else if (lbl_8087F048 != 0) {
        fn_8010653C(lbl_8087F048, this, &mMotion);
    }
}

void Em006DarkMaster::vfFC() {
    fn_8016E970(2);
    vf100(6);
}

void Em006DarkMaster::vf100(int state) {
    if (CharaTask::mFlag2) {
        return;
    }
    int prev = m58C;
    m14BC = 0;
    m14C0 = 0;
    m14C4 = 0;
    m14C8 = 0;
    mFlag31 = 0;
    mFlag30 = 0;
    mFlag29 = 0;
    mFlag28 = 0;
    mFlag27 = 1;
    m14D0 = 0;
    m58C = state;
    switch (state) {
    case 6:
        mFlag30 = 1;
        break;
    case 7:
        fn_805A4F1C(0x66, 1, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        break;
    case 8: {
        fn_805A4F1C(0x1EA, 0, 1, 1.0f, 0.0f);
        if (lbl_8087F048 != 0) {
            fn_8010653C(lbl_8087F048, this, &mMotion);
        }
        void* routes = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
        RouteSearch_unk* search = lbl_8087EE68;
        int point = search->fn_80018D5C(&mPos, mC58.mB4, 0, -1, 0, fn_80179D44(), 1, 3.1415927f + mRot.y, 240.0f,
                                        100.0f, 200.0f);
        if (point <= 0) {
            search = lbl_8087EE68;
            point = search->fn_80018D5C(&mPos, mC58.mB4, 0, -1, 0, fn_80179D44(), 1, 3.1415927f + mRot.y, 240.0f,
                                        30.0f, 200.0f);
        }
        if (point <= 0) {
            search = lbl_8087EE68;
            point = search->fn_80018D5C(&mPos, mC58.mB4, 0, -1, 0, fn_80179D44(), 1, 3.1415927f + mRot.y, 360.0f,
                                        30.0f, 100.0f);
        }
        if (point > 0) {
            m14FC = fn_803C11A4(routes, point);
        } else {
            m14FC = mPos;
        }
        mFlag30 = 0;
        break;
    }
    case 9:
        fn_805A4F1C(0x1E1, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        break;
    case 2:
        if ((mStatus.m0C & 0x20) == 0x20 && prev != 2) {
            vf44();
        }
        mFlag30 = 0;
        break;
    }
}

void Em006DarkMaster::vf104() {
    m14C0++;
    if (CanAct_unk() && m58C != 6 && vfF8() == 0) {
        vf100(6);
    }
    switch (m58C) {
    case 6:
        if (m55C == 2 && vfF8() != 0) {
            vf100(7);
        }
        vf108();
        break;
    case 7:
        vf108();
        break;
    case 8:
        if (mFlag31) {
            vf100(6);
        }
        vf108();
        break;
    case 9:
        if (mFlag31) {
            vf100(6);
        }
        vf108();
        break;
    case 2:
        fn_805A40B8();
        break;
    default:
        vf100(6);
        break;
    }
}

void Em006DarkMaster::vf108() {
    float zero = 0.0f;
    float frame;
    float one = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 2) {
        switch (m58C) {
        case 6:
            mFlag31 = 1;
            break;
        case 7:
            mFlag31 = 1;
            break;
        case 8:
            frame = mMotion.mTracks[0].m08;
            if (m14BC == 0) {
                if (frame > mMotion.fn_80097D7C(0)) {
                    fn_800C344C("SE_SP014_001", &mPos, 0, -1, 1.0f);
                    mPos = m14FC;
                    CharaTask* target = mC58.mC4;
                    if (target != 0) {
                        atn::Vector3 dir = target->mPos - mPos;
                        SafeNormalize_unk(&dir);
                        mRot = dir.ToRotation_unk();
                    }
                    mMotion.fn_80097C08(0, 0x1EA, 0, 0, 1, 0.0f, 0.1f);
                    if (lbl_8087F048 != 0) {
                        fn_8010653C(lbl_8087F048, this, &mMotion);
                    }
                    m14BC++;
                    return;
                }
            } else if (frame > mMotion.fn_80097D7C(0)) {
                mFlag31 = 1;
            }
            break;
        case 9:
            frame = mMotion.mTracks[0].m08;
            if (m14BC == 0) {
                if (frame >= mMotion.fn_80097D7C(0)) {
                    m14BC++;
                    mMotion.fn_80097C08(0, 0x1E2, 1, 0, 1, 0.0f, 0.1f);
                }
            }
            break;
        }
    } else {
        fn_805A4254();
        return;
    }
    fn_8013CB68(rot, 0, zero, m568 * one);
}
