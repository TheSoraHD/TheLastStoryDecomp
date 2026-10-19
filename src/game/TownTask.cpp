#include "game/TownTask.h"

extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void* fn_804A7EA4(TownTask* task, const char* path); // inferred: loads a minimap csv
extern "C" void* lbl_8087EFB4;
extern "C" void* fn_800C16B4(void* obj);
extern "C" void fn_8049B780(MapObject_unk* obj, void* arg);

TownTask* lbl_8087FA20;

// NONMATCHING: 82% - the array loop of m190 is unrolled by 8 in the target without the signed count check that MWCC
// emits for our loop (tried member array with inline ctor, placement new, for/while/index loops)
TownTask::TownTask(atn::Task* parent)
    : MapTask(parent), m120(0), m124(1), m128(0), m12C(2), m130(0), m134(50.0f), m138(0), m13C(0),
      m140(0.0f, 0.0f, 0.0f), m164(1.1f), m168(0.0f, 0.0f, 0.0f), m174(0), m178(0), m180(-1), m184(-1), m188(0),
      m18C(0), mWidth(3), mHeight(3), m25C(0), m260(0), m264(0) {
    lbl_8087FA20 = this;
    mF8 = 1;
    m258 = fn_804A7EA4(this, "boot/minimap_town.csv");
}

// NONMATCHING: the dtor (0x805B6EAC: two inlined tree erases, strings, ResHandle) is not written yet

// NONMATCHING: 99.9% - code identical; the target string pool starts with "" (used by a function not written yet), so
// every string offset is one byte lower here
int TownTask::vf0C() {
    if (GetLoader_unk()->fn_80473F50() != 0) {
        return 0;
    }
    if (m120 == 0) {
        m120 = 1;
        fn_805B7B24();
        void* group = m11C.Load("Town");
        fn_800874C8(group, "Width", &mWidth, 1, 3, 1, 0, 0);
        fn_800874C8(group, "Height", &mHeight, 1, 3, 1, 0, 0);
    }
    if (m128 != 0) {
        return 0;
    }
    if (m124 != 0) {
        fn_805B8DB4(1);
        m124 = 0;
    }
    if (MapTask::vf0C() != 0) {
        for (MapObject_unk* obj = m70; obj != 0; obj = obj->mNext) {
            if (obj->mType == 4) {
                fn_8049B780(obj, fn_800C16B4(lbl_8087EFB4));
            }
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: vf10 (0x805B7398, 0x76C bytes) is not written yet

// NONMATCHING: 34% - the target stores z before the paired x/y store (psq_l, lfs, stfs, psq_st); no Vec.h helper
// has that order and new inline asm is not allowed
void TownTask::fn_805B7B04(const atn::Vector3& pos) {
    m128 = 0;
    m168 = pos;
}

void TownTask::vf14() {
    MapTask::vf14();
}

// NONMATCHING: fn_805B7B24 .. fn_805B99A8 (minimap/block loading and the tree helpers) are not written yet
