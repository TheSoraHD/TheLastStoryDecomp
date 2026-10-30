#include "gmk/CharaPathMove.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "std/string_members.h"
#include "atn/DrawHelper.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int rand();
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" int fn_8006AA20(const char* path); // inferred: the file exists
extern "C" float fn_800DC288(const char* s);  // inferred: string to float
extern "C" int fn_800DC12C(const char* s);    // inferred: string to int
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" void fn_80097CCC(atn::Model* model, int track, float value);   // inferred: stops a track
extern "C" void fn_8032AF90(gmk::PathData_unk* path, std::vector<atn::Vector3>* points); // inferred: builds the path
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: debug drawing
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: empty names passed to operator new (in .sdata of another file)
extern "C" char lbl_8087DFC0[4];
extern "C" char lbl_8087DFC4[4];
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x74];
    int m74; // inferred: debug display
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: clear of a vector of trivially destructible elements (MSL erases the range; name
// invented)
template <class T>
static inline void Clear_unk(std::vector<T>& v) {
    v.__shrink_unk(0);
}

namespace gmk {

GimmickCommon* CharaPathMove::fn_8043D594(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) CharaPathMove(parent, id, arg);
    }
    return 0;
}

CharaPathMove::CharaPathMove(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mLoop(0), mInterval(0), mWait(0), mSpeed(0.2f), mNext(0), mShowPath(0) {
    m54 = 0;
    Clear_unk(mCharas);
    Clear_unk(mPoints);
}

void CharaPathMove::DeleteAll_unk() {
    for (unsigned int i = 0; i < mCharas.size(); i++) {
        delete mCharas[i];
    }
}

// NONMATCHING: 99.44% - register numbers of the delete loop and of the two clears.
CharaPathMove::~CharaPathMove() {
    DeleteAll_unk();
    Clear_unk(mCharas);
    Clear_unk(mPoints);
}

// inferred: the character has finished loading (then it is shown); name invented
static inline int IsLoading_unk(PathChara_unk* pc) {
    GmkChara* chara = pc->mChara;
    if (chara != 0 && chara->IsFlag_unk(2)) {
        chara->fn_800D246C(0);
        return 0;
    }
    return 1;
}

int CharaPathMove::vf0C() {
    if (fn_803EC91C() == 0) {
        for (unsigned int i = 0; i < mCharas.size(); i++) {
            if (IsLoading_unk(mCharas[i])) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

void CharaPathMove::Step_unk() {
    if (mNext < (int)mCharas.size() && mCharas[mNext]->mState == 0 && --mWait < 0) {
        mCharas[mNext]->fn_8043E770();
        mWait = mInterval + rand() % 15;
        mNext++;
        if ((int)mCharas.size() <= mNext) {
            mNext = 0;
        }
    }
}

void CharaPathMove::UpdateAll_unk(int skip) {
    for (unsigned int i = 0; i < mCharas.size(); i++) {
        mCharas[i]->fn_8043EA64(skip);
    }
}

void CharaPathMove::Start() {
    vf2C();
    GimmickMsg_unk msg(1);
    vf40(&msg);
    mWait = rand() % 60;
    mNext = 0;
    if (mF8 == 0) {
        for (int k = 0; k < 1500; k++) {
            Step_unk();
            UpdateAll_unk(1);
        }
    }
}

void CharaPathMove::vf10() {
    if (m54 == 0) {
        Start();
    }
    Step_unk();
    UpdateAll_unk(0);
}

void CharaPathMove::vf14() {
    if (m9C != 0) {
        if (lbl_8087F0A8->m74 != 0) {
            fn_803EDCF4();
        }
        if (mShowPath != 0) {
            for (unsigned int i = 0; i < mPoints.size(); i++) {
                lbl_8087EEB0->fn_80063200(12, 0xFF0000FF, mPoints[i].x, 0.2f + mPoints[i].y, mPoints[i].z, 0.0f, 10.0f);
            }
        }
    }
}

int CharaPathMove::vf1C() {
    return 0;
}

static GmkChara* CreateChara_unk(atn::Task* parent, const std::string& path);

// NONMATCHING: the original goes over the inline budget (docs/inlining.md): the path reset, the
// clears, the CSV constructor (fn_803EFCA8), the string/vector members (fn_8003E4A4,
// fn_8020A780, fn_801207D4, fn_8032AC1C, fn_8004212C, fn_800E0AA8), the character creation
// (0x8043E134) and push_back (0x8043E1F0) are all called out of line there; here they are inlined.
void CharaPathMove::vf20() {
    mPath.fn_8043E07C();
    Clear_unk(mPoints);
    mUpperMotions.clear();
    atn::Vector3 point;
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "CHARA") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            if (fn_8006AA20(path) != 0) {
                PathChara_unk* pc = new ((MemoryTag)12, "", "", 0) PathChara_unk(this);
                pc->mChara = CreateChara_unk(this, std::string(path));
                pc->mChara->fn_800D246C(1);
                mCharas.push_back(pc);
            }
        } else if (strcmp(field, "MOTION") == 0) {
            mMotion = csv.fn_8005B9CC();
        } else if (strcmp(field, "UPPER_MOTION") == 0) {
            mUpperMotions.push_back(std::string(csv.fn_8005B9CC()));
        } else if (strcmp(field, "UPPER_MOTION_INTERVAL") == 0) {
            mUpperInterval = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(field, "PATH_POINT") == 0) {
            point.x = fn_800DC288(csv.fn_8005B9CC());
            point.y = fn_800DC288(csv.fn_8005B9CC());
            point.z = fn_800DC288(csv.fn_8005B9CC());
            mPoints.push_back(point);
        } else if (strcmp(field, "MOVE_SPEED") == 0) {
            mSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "PARAM") == 0) {
            mLoop = fn_800DC12C(csv.fn_8005B9CC());
            mInterval = fn_800DC12C(csv.fn_8005B9CC());
            mF8 = fn_800DC12C(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
    fn_8032AF90(&mPath, &mPoints);
    for (unsigned int i = 0; i < mCharas.size(); i++) {
        mCharas[i]->fn_8043E6B0(mMotion.c_str(), mSpeed, &mUpperMotions);
    }
}

// NONMATCHING: 83.52% - register numbers of the four clears.
void PathData_unk::fn_8043E07C() {
    m00 = 0;
    m04 = 0.0f;
    m08 = lbl_807C7030;
    m14 = lbl_807C7030;
    Clear_unk(m20);
    Clear_unk(m2C);
    Clear_unk(m38);
    Clear_unk(m44);
}

PathChara_unk::PathChara_unk(CharaPathMove* owner)
    : mOwner(owner), mState(0), mChara(0), m0C(lbl_807C7030), m18(lbl_807C7030), mSpeed(0.2f), mPoint(0),
      mTimer(0) {}

// inferred: creates the character of the file `path` (name invented)
static GmkChara* CreateChara_unk(atn::Task* parent, const std::string& path) {
    return parent != 0 ? new ((MemoryTag)1, lbl_8087DFC4, lbl_8087DFC0, 0) GmkChara(parent, path) : 0;
}

int CharaPathMove::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    if ((unsigned int)m54 > 1) {
        m54 = 0;
    }
    return m54;
}

void CharaPathMove::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

int CharaPathMove::vf60() {
    return m54 == 1;
}

// NONMATCHING: 99.41% - .sdata2 order: the original places 1.0f (used later by fn_8043E770)
// before the constants of this function.
void CharaPathMove::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "ShowPath", &mShowPath, 0, 0);
}

void PathChara_unk::fn_8043E6B0(const char* motion, float speed, std::vector<std::string>* uppers) {
    if (mChara != 0) {
        fn_80097A88(&mChara->mMotion, 0x13F, motion);
        for (unsigned int i = 0; i < uppers->size(); i++) {
            fn_80097A88(&mChara->mMotion, 0xEF + i, (*uppers)[i].data());
        }
    }
    mSpeed = speed;
}

// NONMATCHING: 89.59% - 16 bytes smaller frame and one saved register less; owner reloaded for
// rand() % mUpperInterval; same .sdata2 order issue as vf6C.
void PathChara_unk::fn_8043E770() {
    ((void)0); // inferred: assertions compiled out (keep the function out of line in Start/vf10)
    ((void)0);
    if (mOwner != 0 && mChara != 0) {
        if (mOwner->mPoints.size() >= 2) {
            mChara->mPos = mOwner->mPoints[0];
            mPoint++;
            atn::Vector3 dir = mOwner->mPoints[1] - mOwner->mPoints[0];
            mChara->mRot = dir.ToRotation_unk();
        }
        mTimer = rand() % mOwner->mUpperInterval;
        mChara->mMotion.m34C = 1;
        mChara->mMotion.mTracks[0].m20 = 1.0f;
        mChara->mMotion.mTracks[0].m0C = 1.0f;
        mChara->mMotion.fn_80097C08(0, 0x13F, 1, 0, 1, 0.0f, 0.1f);
        mState = 1;
    }
}

// NONMATCHING: 93.10% - stack slots of the rotation temporaries (0x30 bytes lower here) and one
// saved register more; same .sdata2 order issue as vf6C.
void PathChara_unk::fn_8043EA64(int skip) {
    if (mOwner == 0) {
        return;
    }
    if (mState == 1) {
        if (mChara != 0) {
            if (mOwner->mPoints.size() >= 2) {
                atn::Vector3 target = mOwner->mPoints[mPoint];
                atn::Vector3 d = target - mChara->mPos;
                atn::Vector3 rot = mChara->mRot;
                float dist = d.Length_unk();
                atn::Vector3 want = d.ToRotation_unk();
                float turn = atn::WrapAngle_unk(want.y - rot.y);
                if (atn::Abs_unk(turn) > 0.034906585f) {
                    turn = turn > 0.0f ? 0.034906585f : -0.034906585f;
                }
                rot.y += turn;
                mChara->mRot = rot;
                atn::Vector3 pos = mChara->mPos;
                atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
                atn::Matrix3x4 m;
                PSMTXRotRad(m.m, 'y', mChara->mRot.y);
                PSMTXMultVec(m.m, &fwd, &fwd);
                pos += fwd * mSpeed;
                mChara->mPos = pos;
                if (dist < 10.0f) {
                    if (++mPoint >= (int)mOwner->mPoints.size()) {
                        mPoint = 0;
                        if (mOwner->mLoop == 0) {
                            mState = 0;
                        }
                    }
                }
            } else {
                mChara->mPos = atn::Vector3(0.0f, -1000.0f, 0.0f);
                mChara->mRot = lbl_807C7030;
            }
        }
        if (skip == 0 && mOwner->mUpperMotions.size() != 0) {
            mTimer++;
            atn::Model* motion = &mChara->mMotion;
            if (mTimer >= mOwner->mUpperInterval + rand() % 100) {
                mTimer = 0;
                motion->fn_80097C08(3, 0xEF + rand() % mOwner->mUpperMotions.size(), 0, 0, 1, 0.0f, 0.1f);
            }
            float frame = motion->mTracks[3].m08;
            if (frame >= motion->fn_80097D7C(3)) {
                fn_80097CCC(motion, 3, 0.0f);
            }
        }
    } else {
        mChara->mPos = atn::Vector3(0.0f, -1000.0f, 0.0f);
        mChara->mRot = lbl_807C7030;
        fn_80097CCC(&mChara->mMotion, 3, 0.0f);
        mTimer = 0;
    }
}

} // namespace gmk
