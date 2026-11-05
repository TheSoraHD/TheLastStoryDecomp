#include "gmk/ArenaFloor.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/GimmickManager.h"
#include "game/MapTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: object of a part of type 4 (placed by its matrix)
struct ArenaObj_unk {
    unsigned char m00[0x30];
    atn::Matrix3x4 mMtx; // 0x30
};
// inferred: object embedded in a part (placed by its matrix)
struct ArenaPartObj_unk {
    unsigned char m00[0x8];
    atn::Matrix3x4 mMtx; // 0x58 in the part
};
// inferred: map part of the group (type at 0x48, next at 0x4C)
struct ArenaPart_unk {
    unsigned char m00[0x48];
    int mType;
    ArenaPart_unk* mNext;
    ArenaPartObj_unk m50;
    unsigned char m88[0x344 - 0x88];
    int mObjCount;          // 0x344: inferred: objects of a part of type 4
    ArenaObj_unk** mObjs;   // 0x348
};
extern "C" void fn_80499B9C(ArenaPart_unk* part);
extern "C" void fn_80499C48(ArenaPart_unk* part);

extern "C" void fn_8049994C(ArenaPart_unk* part, atn::Vector3* pos, atn::Vector3* rot); // inferred: places the gimmicks of a part
struct Params_unk {
    unsigned char m000[0x3A4];
    float mTimeScale; // 0x3A4
};
extern "C" Params_unk* lbl_8087EFA8;

namespace gmk {

GimmickCommon* ArenaFloor::fn_8041DDE4(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) ArenaFloor(parent, id, arg);
    }
    return 0;
}

// NONMATCHING: 97.6% - in the case 0 entry the original computes the index before loading the
// array (stwux base, index), and in the case 4 loop the counter and the entry pointer swap r6/r7.
// The array is an atn::SimpleArray (its inline Resize_unk has the new[] literals in .sdata).
int ArenaFloor::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 5;
        if (mGroup != 0) {
            int count = 0;
            for (ArenaPart_unk* part = (ArenaPart_unk*)mGroup->mParts; part != 0; part = part->mNext) {
                switch (part->mType) {
                case 0:
                    count++;
                    break;
                case 4:
                    count += part->mObjCount;
                    break;
                case 5:
                    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
                        if (g->m20 == part) {
                            count++;
                        }
                    }
                    break;
                }
            }
            mEntries.Resize_unk(count, 0);
            int n = 0;
            for (ArenaPart_unk* part = (ArenaPart_unk*)mGroup->mParts; part != 0; part = part->mNext) {
                switch (part->mType) {
                case 0: {
                    Entry_unk& e = mEntries.mData[n++];
                    e.mType = part->mType;
                    e.mObject = &part->m50;
                    e.mPos = part->m50.mMtx.GetTrans_unk() + m6C;
                    break;
                }
                case 4:
                    for (int i = 0; i < part->mObjCount; i++) {
                        ArenaObj_unk* obj = part->mObjs[i];
                        Entry_unk& e = mEntries.mData[n];
                        n++;
                        e.mType = part->mType;
                        e.mObject = obj;
                        e.mPos = obj->mMtx.GetTrans_unk() + m6C;
                    }
                    break;
                case 5:
                    fn_8049994C(part, &m6C, &m78);
                    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
                        if (g->m20 == part) {
                            Entry_unk& e = mEntries.mData[n++];
                            e.mType = part->mType;
                            e.mObject = g;
                            e.mPos = g->m6C + m6C;
                        }
                    }
                    break;
                }
            }
        }
        return 1;
    }
    return 0;
}

void ArenaFloor::Start() {
    GimmickMsg_unk msg(1);
    if (m58 == 1) {
        msg.mType = 4;
    }
    vf40(&msg);
}

// The original calls fn_8041E7E4 out of line here and in vf40 (__declspec(noinline) on it; with
// fn_8041E480 written the inlining budget might do the same).
void ArenaFloor::vf10() {
    if (m54 == 0) {
        Start();
    }
    if (m54 == 2) {
        GimmickMsg_unk msg(3);
        vf40(&msg);
    }
    if (m54 == 3 || m54 == 5) {
        float step = lbl_8087EFA8->mTimeScale;
        fn_8041E480(1.0f - mValue / mTime);
        if (m54 == 3) {
            mValue += step;
            if (mValue >= mTime) {
                mValue = mTime;
                GimmickMsg_unk msg(4);
                vf40(&msg);
                fn_8041E7E4(1);
            }
        } else {
            mValue -= step;
            if (mValue < 0.0f) {
                mValue = 0.0f;
                GimmickMsg_unk msg(1);
                vf40(&msg);
            }
        }
    }
}

__declspec(noinline) void ArenaFloor::fn_8041E7E4(int on) {
    if (on) {
        for (ArenaPart_unk* part = (ArenaPart_unk*)mGroup->mParts; part != 0; part = part->mNext) {
            if (part->mType == 5) {
                fn_80499B9C(part);
            }
        }
    } else {
        for (ArenaPart_unk* part = (ArenaPart_unk*)mGroup->mParts; part != 0; part = part->mNext) {
            if (part->mType == 5) {
                fn_80499C48(part);
            }
        }
    }
}

void ArenaFloor::vf14() {}

int ArenaFloor::vf1C() {
    if (mGroup != 0 && (mGroup->mFlags & 2) != 2) {
        return 1;
    }
    return 0;
}

// NONMATCHING: the target has a 16 bytes larger frame and keeps the constant 1 in r31.
void ArenaFloor::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MAP") == 0) {
            if (m20 != 0) {
                mGroup = MapTask::SetTask((atn::Task*)m20, csv.fn_8005B9CC()); // inferred: atn::Task::m20 is the parent (void*)
                if (mGroup != 0) {
                    mGroup->mF4 = 1;
                }
            }
        } else if (strcmp(field, "OFFSET") == 0) {
            mOffset.x = fn_800DC288(csv.fn_8005B9CC());
            mOffset.y = fn_800DC288(csv.fn_8005B9CC());
            mOffset.z = fn_800DC288(csv.fn_8005B9CC());
            mTime = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

// NONMATCHING: see vf10.
int ArenaFloor::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    switch (m54) {
    case 1:
    case 2:
    case 3:
        mValue = 0.0f;
        fn_8041E480(1.0f);
        fn_8041E7E4(0);
        break;
    case 4:
        mValue = mTime;
        fn_8041E480(0.0f);
        fn_8041E7E4(1);
        break;
    case 5:
        mValue = mTime;
        fn_8041E480(0.0f);
        fn_8041E7E4(0);
        break;
    }
    return m54;
}

void ArenaFloor::vf70(int state) {
    switch (state) {
    case 1:
    case 5:
        state = 1;
        break;
    case 2:
    case 3:
    case 4:
        state = 4;
        break;
    }
    GimmickMsg_unk msg(state);
    vf40(&msg);
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

} // namespace gmk
