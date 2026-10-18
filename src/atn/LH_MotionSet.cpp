#include "atn/LH_MotionSet.h"
#include "atn/Array_unk.h"
#include "atn/LH_Motion.h"
#include "atn/LoadManager.h"
#include "atn/Csv.h"
#include "gmk/SeTable.h"
#include "std/vector.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)

// inferred: one "MOTION" line of the set file: id given by the callback and motion path
struct MotionSetLine_unk {
    int mId;
    atn::FixedString_unk<0x100> mPath; // 0x04
};

typedef int (*MotionSetCallback_unk)(const char* name); // inferred: motion name to id

// inferred: reference to a shared resource of the load manager (functions 0x80470364/0x80470528)
struct ResRef_unk {
    ResRef_unk() : mRes(0), m04(0) {}
    ~ResRef_unk() { fn_80470528(); }
    void fn_80470364(const char* name, int a, int b, int c); // inferred: starts loading `name`
    void fn_80470528();                                      // inferred: drops the reference

    void* mRes;
    void* m04;
};

// inferred: motion of the set (0x10 bytes)
struct MotionSetEntry_unk {
    ~MotionSetEntry_unk();

    int mId;                // inferred: motion id given by the callback
    atn::LH_Motion mMotion; // 0x04
};

// inferred: defined at the top of the file (the copy at 0x80476D00 comes first)
MotionSetEntry_unk::~MotionSetEntry_unk() {}

namespace atn {

// inferred from __vt__Q23atn13LoadMotionSet (0x8078FEC8)
class LoadMotionSet : public LoadCore {
public:
    LoadMotionSet(const char* path) : LoadCore(path), mCallback(0), mParsed(0) {}
    virtual ~LoadMotionSet();
    virtual void vf0C();
    // NONMATCHING: 95.6% - see the definition
    void fn_80476ED4();

    Buffer_unk<MotionSetEntry_unk> mEntries; // 0x5C
    MotionSetCallback_unk mCallback;         // 0x64
    int mParsed;                             // 0x68
    ResRef_unk m6C;
};

LoadMotionSet::~LoadMotionSet() {}

// inferred: reads the CSV of the set ("MOTION <path> <name>" and "PACK <name>" lines).
// NONMATCHING: 95.6% - in the inlined std::vector::push_back the size is loaded after the capacity
// and reloaded in the slow path (the original keeps it in r16), and the copy loop that moves the old
// elements decrements the source pointer inside the copy instead of before the null check.
void LoadMotionSet::fn_80476ED4() {
    std::vector<MotionSetLine_unk> lines;
    CsvTab csv((const char*)m54, (int)m50);
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MOTION") == 0) {
            MotionSetLine_unk line;
            line.mPath = csv.fn_8005B9CC();
            line.mId = mCallback(csv.fn_8005B9CC());
            if (line.mId >= 0) {
                lines.push_back(line);
            }
        } else if (strcmp(field, "PACK") == 0) {
            if (m6C.m04 == 0) {
                m6C.fn_80470364(csv.fn_8005B9CC(), 0, 0, 1);
            }
        }
    } while (csv.fn_8005BBF8() != 0);
    mEntries.Resize_unk(lines.mSize, 6);
    for (unsigned int i = 0; i < mEntries.mSize; i++) {
        mEntries.mData[i].mId = lines.begin()[i].mId;
        mEntries.mData[i].mMotion.fn_800A03E4(lines.begin()[i].mPath.c_str());
    }
    fn_80473130();
}

void LoadMotionSet::vf0C() {
    if (mState == 2) {
        if (mParsed != 0) {
            if (mRefCount == 0) {
                fn_80473104(5);
            } else {
                int done = 1;
                for (unsigned int i = 0; i < mEntries.mSize; i++) {
                    if (mEntries.mData[i].mMotion.fn_800A0448() != 0) {
                        done = 0;
                    }
                }
                if (done) {
                    m6C.fn_80470528();
                    fn_80473104(3);
                }
            }
        } else {
            fn_80476ED4();
            mParsed = 1;
        }
    }
}

void LH_MotionSet::vf0C(const char* path) {
    fn_80473F88();
    LoadMotionSet* core = lbl_8087F518->Get_unk<LoadMotionSet>(path, 1);
    mCore = core;
    core->mCallback = (MotionSetCallback_unk)mCallback;
}

} // namespace atn
