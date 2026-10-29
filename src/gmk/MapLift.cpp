#include "gmk/MapLift.h"
#include "atn/LoadHandle.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/GimmickManager.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int atoi(const char* s);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_802180A8(const char* s);   // inferred: motion id from its name
// inferred: motion length (m08->m10)
extern "C" float fn_800A08D4(atn::LH_Motion* motion);
// inferred: pose of the node `node` of the motion at `frame`
extern "C" int fn_800A04A0(atn::LH_Motion* motion, MotionPose_unk* pose, const char* node, char* work,
                           float frame);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: object of a map part (matrix at 0x30; as ArenaObj_unk in gmk/ArenaFloor.cpp)
namespace atn {
class ModelInstance {
public:
    void fn_8009EE30(const Matrix3x4& m); // inferred: sets the matrix
    unsigned char m00[0x30];
    Matrix3x4 mMtx; // 0x30
};
} // namespace atn

// inferred: part of a map group (as ArenaPart_unk in gmk/ArenaFloor.cpp)
struct MapPart_unk {
    unsigned char m00[0x48];
    int mType;
    MapPart_unk* mNext;
    unsigned char m50[0x344 - 0x50]; // inferred: by type: model (0), name at 0x5C (4), resource at 0x58 (5)
    int mObjCount;               // 0x344
    atn::ModelInstance** mObjs;  // 0x348
};
// inferred: map information (as ArenaGroup_unk in gmk/ArenaFloor.cpp)
struct MapGroup_unk {
    unsigned char m00[0x38];
    unsigned int mFlags; // 0x38: inferred: bit 1 when loaded
    unsigned char m3C[0x70 - 0x3C];
    MapPart_unk* mParts; // 0x70
};
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" MapGroup_unk* fn_80373148(Field_unk* field);
// inferred: places the gimmicks of a part (pointers in gmk/ArenaFloor.cpp; references here, the
// offset is a temporary)
extern "C" void fn_8049994C(MapPart_unk* part, const atn::Vector3& pos, const atn::Vector3& rot);

// inferred: column of a matrix (name invented)
static inline atn::Vector3 Col_unk(const atn::Matrix3x4& m, int i) {
    return atn::Vector3(m.m[0][i], m.m[1][i], m.m[2][i]);
}

// inferred: sets the translation (name invented)
static inline void SetTrans_unk(atn::Matrix3x4& m, const atn::Vector3& v) {
    m.m[0][3] = v.x;
    m.m[1][3] = v.y;
    m.m[2][3] = v.z;
}

// inferred: a macro (the inner maximum is evaluated twice)
#define MAX_UNK(a, b) ((a) > (b) ? (a) : (b))
// inferred: largest component (name invented)
static inline float MaxAxis_unk(const atn::Vector3& v) {
    return MAX_UNK(v.x, MAX_UNK(v.y, v.z));
}
// inferred: largest scale of the axes of a matrix (name invented)
static inline float MaxScale_unk(const atn::Matrix3x4& m) {
    return MaxAxis_unk(atn::Vector3(Col_unk(m, 0).Length_unk(), Col_unk(m, 1).Length_unk(), Col_unk(m, 2).Length_unk()));
}

namespace gmk {

GimmickCommon* MapLift::fn_80429668(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) MapLift(parent, desc);
    }
    return 0;
}

// NONMATCHING: 99.90% - the original destroys the motion array with fn_800970CC (a destructor
// with two null checks, also used by atn::Model::Model) instead of atn::LH_Motion::~LH_Motion.
MapLift::MapLift(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mF8(0), mFC(-1), mPos(0.0f, 0.0f, 0.0f),
      mPrevPos(0.0f, 0.0f, 0.0f) {
    m54 = 0;
    memset(m158, 0, sizeof(m158));
    memset(mMotionId, 0, sizeof(mMotionId));
    memset(m190, 0, sizeof(m190));
    memset(mSpeed, 0, sizeof(mSpeed));
    memset(mLoop, 0, sizeof(mLoop));
    memset(m1E4, 0, sizeof(m1E4));
    memset(m26C, 0, sizeof(m26C));
    memset(mNode, 0, sizeof(mNode));
    mLocators.clear();
}

// NONMATCHING: 99.84% - same destructor of the motion array as the constructor.
MapLift::~MapLift() {}

// inferred: the name contains one of the locators
bool MapLift::HasLocator_unk(const char* name) {
    unsigned int n = mLocators.size();
    for (unsigned int i = 0; i < n; i++) {
        if (strstr(name, mLocators[i].data()) != 0) {
            return true;
        }
    }
    return false;
}

// inferred: map group of the field (null without a field)
static inline MapGroup_unk* GetMapGroup_unk() {
    MapGroup_unk* group = 0;
    if (lbl_8087F430 != 0) {
        group = fn_80373148(lbl_8087F430);
    }
    return group;
}

// NONMATCHING: 96.65% - register allocation only (the original keeps this in r31 and the group
// in r21; tried an inline group getter and a member locator helper).
int MapLift::vf0C() {
    MapGroup_unk* group = GetMapGroup_unk();
    if (fn_803EC91C() == 0 && group != 0 && (group->mFlags & 2) == 2) {
        int count = 0;
        for (MapPart_unk* part = group->mParts; part != 0; part = part->mNext) {
            switch (part->mType) {
            case 0:
                if (HasLocator_unk(((atn::Model*)part->m50)->fn_8008B130())) {
                    count++;
                }
                break;
            case 4:
                if (HasLocator_unk((const char*)part->m50 + 0xC)) {
                    count += part->mObjCount;
                }
                break;
            case 5:
                if (HasLocator_unk(((atn::LoadHandle*)(part->m50 + 8))->fn_80473F18())) {
                    count++;
                    for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
                        if (g->m20 == part) {
                            count++;
                        }
                    }
                }
                break;
            }
        }
        mEntries.Resize_unk(count, 0);
        int n = 0;
        for (MapPart_unk* part = group->mParts; part != 0; part = part->mNext) {
            switch (part->mType) {
            case 0:
                if (HasLocator_unk(((atn::Model*)part->m50)->fn_8008B130())) {
                    Entry_unk& e = mEntries.mData[n++];
                    e.mType = 0;
                    e.mObject = part->m50;
                    e.mPos = ((atn::Matrix3x4*)((atn::Model*)part->m50)->mMtx)->GetTrans_unk();
                }
                break;
            case 4:
                if (HasLocator_unk((const char*)part->m50 + 0xC)) {
                    for (int i = 0; i < part->mObjCount; i++) {
                        atn::ModelInstance* obj = part->mObjs[i];
                        Entry_unk& e = mEntries.mData[n++];
                        e.mType = 1;
                        e.mObject = obj;
                        e.mPos = obj->mMtx.GetTrans_unk();
                    }
                }
                break;
            case 5:
                if (HasLocator_unk(((atn::LoadHandle*)(part->m50 + 8))->fn_80473F18())) {
                    Entry_unk& e = mEntries.mData[n++];
                    e.mType = 3;
                    e.mObject = part;
                    e.mPos = lbl_807C7030;
                    for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
                        if (g->m20 == part) {
                            Entry_unk& e2 = mEntries.mData[n++];
                            e2.mType = 2;
                            e2.mObject = g;
                            e2.mPos = g->m6C;
                        }
                    }
                }
                break;
            }
        }
        return 1;
    }
    return 0;
}

void MapLift::vf28() {
    GimmickMsg_unk msg;
    if (m58 > 0) {
        msg.mType = m58;
    } else {
        msg.mType = (mFC >= 0) ? mFC : m58;
    }
    vf40(&msg);
    vf2C();
}

// inferred: motion index of a state (states 2, 4, 6, 7, 8, 10, 11 play a motion)
static inline int MotionIndex_unk(int state) {
    switch (state) {
    case 2:
        return 0;
    case 4:
        return 1;
    case 6:
        return 2;
    case 7:
        return 3;
    case 8:
        return 4;
    case 10:
        return 5;
    case 11:
        return 6;
    }
    return -1;
}

void MapLift::vf10() {
    if (m54 != mF8) {
        int idx = MotionIndex_unk(m54);
        if (idx >= 0 && 7 > idx && mSpeed[idx] < 0.0f) {
            mFrame = fn_800A08D4(&mMotion[idx]);
        } else {
            mFrame = 0.0f;
        }
        mF8 = m54;
    }
    if (m54 == 0) {
        GimmickMsg_unk msg(1);
        vf40(&msg);
    }
    int idx = MotionIndex_unk(m54);
    if (idx >= 0 && 7 > idx) {
        mFrame += mSpeed[idx];
        atn::LH_Motion* motion = &mMotion[idx];
        if (fn_800A08D4(motion) <= mFrame) {
            if (mLoop[idx] != 0) {
                mFrame -= fn_800A08D4(motion);
            } else {
                mFrame = fn_800A08D4(motion);
                GimmickMsg_unk msg(m54 + 1);
                vf40(&msg);
            }
        } else if (mFrame < 0.0f) {
            if (mLoop[idx] != 0) {
                mFrame += fn_800A08D4(motion);
            } else {
                mFrame = 0.0f;
                GimmickMsg_unk msg(m54 + 1);
                vf40(&msg);
            }
        }
        fn_800A04A0(motion, &mPose, mNode, m26C, mFrame);
        mPos = mPose.mPos;
        fn_8042AB58(&mPos, &mPrevPos);
    }
    mPrevPos = mPos;
}

void MapLift::vf14() {}

int MapLift::vf1C() {
    for (int i = 0; i < 7; i++) {
        if (mMotion[i].fn_800A0448() != 0) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 83.37% - register allocation and the inlined std::vector push_back (see
// include/std/vector.h, whose fast path is known not to match).
void MapLift::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    int n = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MOTION") == 0) {
            if (n < 7) {
                mMotion[n].fn_800A03E4(csv.fn_8005B9CC());
                mMotionId[n] = fn_802180A8(csv.fn_8005B9CC());
                m190[n] = fn_800DC288(csv.fn_8005B9CC());
                mSpeed[n] = fn_800DC288(csv.fn_8005B9CC());
                mLoop[n] = atoi(csv.fn_8005B9CC()) == 1;
                m1E4[n] = atoi(csv.fn_8005B9CC()) == 1;
                n++;
            }
        } else if (strcmp(field, "MOTION_NODE") == 0) {
            strcpy(mNode, csv.fn_8005B9CC());
        } else if (strcmp(field, "LOCATOR") == 0) {
            mLocators.push_back(std::string(csv.fn_8005B9CC()));
        } else if (strcmp(field, "INIT_STATE") == 0) {
            mFC = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void MapLift::vf24() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MOTION") == 0) {
        }
    } while (csv.fn_8005BBF8() != 0);
}

int MapLift::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    if ((unsigned int)m54 > 12) {
        m54 = 0;
    }
    fn_8042AE60(m54);
    return m54;
}

void MapLift::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
    fn_8042AE60(state);
}

// NONMATCHING: 88.73% - stack slots of the three column temporaries are in the opposite order
// and the first block is scheduled differently (tried named/temporary scale and helpers).
void MapLift::fn_8042AB58(const atn::Vector3* pos, const atn::Vector3* prev) {
    for (unsigned int i = 0; i < mEntries.mSize; i++) {
        Entry_unk& e = mEntries.mData[i];
        if (e.mType == 0) {
            atn::Model* model = (atn::Model*)e.mObject;
            atn::Matrix3x4& mtx = *(atn::Matrix3x4*)model->mMtx;
            atn::Matrix3x4 m(mtx);
            SetTrans_unk(m, e.mPos + *pos);
            mtx = m;
            model->m54 = MaxScale_unk(m);
        } else if (e.mType == 1) {
            atn::ModelInstance* obj = (atn::ModelInstance*)e.mObject;
            atn::Matrix3x4 m(obj->mMtx);
            SetTrans_unk(m, e.mPos + *pos);
            obj->fn_8009EE30(m);
        } else if (e.mType == 2) {
            ((GimmickCommon*)e.mObject)->m6C = e.mPos + *pos;
        } else if (e.mType == 3) {
            fn_8049994C((MapPart_unk*)e.mObject, *pos - *prev, lbl_807C7030);
        }
    }
}

void MapLift::fn_8042AE60(int state) {
    switch (state) {
    case 0:
    case 1:
        fn_800A04A0(&mMotion[0], &mPose, mNode, m26C, 0.0f);
        mPos = mPose.mPos;
        fn_8042AB58(&mPos, &mPrevPos);
        mPrevPos = mPos;
        break;
    case 3:
        fn_800A04A0(&mMotion[0], &mPose, mNode, m26C, fn_800A08D4(&mMotion[0]));
        mPos = mPose.mPos;
        fn_8042AB58(&mPos, &mPrevPos);
        mPrevPos = mPos;
        break;
    case 5:
        fn_800A04A0(&mMotion[1], &mPose, mNode, m26C, fn_800A08D4(&mMotion[1]));
        mPos = mPose.mPos;
        fn_8042AB58(&mPos, &mPrevPos);
        mPrevPos = mPos;
        break;
    case 9:
        fn_800A04A0(&mMotion[4], &mPose, mNode, m26C, fn_800A08D4(&mMotion[4]));
        mPos = mPose.mPos;
        fn_8042AB58(&mPos, &mPrevPos);
        mPrevPos = mPos;
        break;
    }
}

} // namespace gmk
