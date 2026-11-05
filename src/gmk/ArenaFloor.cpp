#include "gmk/ArenaFloor.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/GimmickManager.h"
#include "game/MapTask.h"
#include "game/MapInstance_unk.h"
#include "game/MapPartsGimmickLoc_unk.h"
#include "game/MapPartsLocatorSet.h"
#include "game/MapPartsModel.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float


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
            for (MapPartsBase* part = mGroup->mParts; part != 0; part = part->mNext) {
                switch (part->mType) {
                case 0:
                    count++;
                    break;
                case 4:
                    count += ((MapPartsLocatorSet*)part)->mInstances.mSize;
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
            for (MapPartsBase* part = mGroup->mParts; part != 0; part = part->mNext) {
                switch (part->mType) {
                case 0: {
                    Entry_unk& e = mEntries.mData[n++];
                    e.mType = part->mType;
                    e.mObject = &((MapPartsModel*)part)->mModel;
                    e.mPos = ((MapPartsModel*)part)->mModel.mMtx.GetTrans_unk() + m6C;
                    break;
                }
                case 4:
                    for (int i = 0; i < ((MapPartsLocatorSet*)part)->mInstances.mSize; i++) {
                        MapInstance_unk* obj = ((MapPartsLocatorSet*)part)->mInstances[i];
                        Entry_unk& e = mEntries.mData[n];
                        n++;
                        e.mType = part->mType;
                        e.mObject = obj;
                        e.mPos = obj->mInst.mMtx.GetTrans_unk() + m6C;
                    }
                    break;
                case 5:
                    ((MapPartsGimmickLoc*)part)->fn_8049994C(m6C, m78);
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
        for (MapPartsBase* part = mGroup->mParts; part != 0; part = part->mNext) {
            if (part->mType == 5) {
                ((MapPartsGimmickLoc*)part)->fn_80499B9C();
            }
        }
    } else {
        for (MapPartsBase* part = mGroup->mParts; part != 0; part = part->mNext) {
            if (part->mType == 5) {
                ((MapPartsGimmickLoc*)part)->fn_80499C48();
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
