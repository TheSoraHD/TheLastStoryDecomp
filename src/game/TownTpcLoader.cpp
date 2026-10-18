#include "game/TownTpcLoader.h"
#include "atn/Memory.h"
#include "math/Vec.h"

// inferred: classes of the globals are unknown
struct CharaList_unk {
    unsigned char m00[0x48];
    int m48;                  // 0x48
    unsigned char m4C[0x80 - 0x4C];
    unsigned int mNum;        // 0x80
    unsigned char m84[0x134 - 0x84];
    void* m134;               // 0x134
};
struct Field_unk {
    unsigned char m0000[0x10D8];
    CharaList_unk* m10D8; // 0x10D8
};
extern "C" Vec lbl_807C7030;
struct Target_unk {
    unsigned char m000[0xD3C];
    int mD3C;
    int mD40;
    Vec mD44;
    atn::Task* mD50;
    int mD54;
    int mD58;
    float mD5C;

    // inferred: clears the target if it is `chara`
    void Clear_unk(atn::Task* chara) {
        if (mD50 == chara) {
            mD40 = 0;
            mD50 = 0;
            mD44 = lbl_807C7030;
            mD54 = 0;
            mD58 = 0;
            mD5C = 0.0f;
            mD3C = 0;
        }
    }
};
struct TownMap_unk;

extern "C" Field_unk* lbl_8087F430;
extern "C" Target_unk* lbl_8087F490;
extern "C" void* lbl_8087F498;
extern "C" TownMap_unk* lbl_8087FA20;
extern "C" void fn_805B8110(TownMap_unk* map, const Vec* pos, int* x, int* z); // inferred: position to cell
extern "C" Vec fn_803CC754(CharaList_unk* list, unsigned int i); // inferred: position of entry i
extern "C" atn::Task* fn_803C0634(CharaList_unk* list, unsigned int i);
extern "C" void fn_803CC774(CharaList_unk* list, atn::Task* chara);
extern "C" void fn_803CC718(CharaList_unk* list, atn::Task* chara, int arg);
extern "C" void fn_803957F0(void* obj, void* arg);
extern "C" void fn_803EAC3C(void* obj, atn::Task* chara);
extern "C" void fn_8016E970(atn::Task* chara, int arg);
extern "C" void fn_800D246C(atn::Task* task, int arg);
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task

struct Chara_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    void* m58;
};

TownTpcLoader::~TownTpcLoader() {}

// NONMATCHING: 98.5% - the original recomputes the address of mCharas[i] for the last call instead of
// reusing it (a reference-parameter helper reproduces that but swaps two registers).
void TownTpcLoader::fn_805B9D58() {
    CharaList_unk* list = lbl_8087F430->m10D8;
    for (unsigned int i = 0; i < mCharas.size(); i++) {
        fn_803CC774(list, mCharas[i]);
        fn_803957F0(list->m134, ((Chara_unk*)mCharas[i])->m58);
        fn_803CC718(list, mCharas[i], 0);
        if (lbl_8087F498 != 0) {
            fn_803EAC3C(lbl_8087F498, mCharas[i]);
        }
        if (lbl_8087F490 != 0) {
            lbl_8087F490->Clear_unk(mCharas[i]);
        }
        fn_800D2338(mCharas[i]);
    }
    fn_800D2338(this);
}

// NONMATCHING: 99.6% - the pointer and the counter of the last loop are in swapped registers (r29/r30).
int TownTpcLoader::vf0C() {
    if (mState < 2) {
        if (mState >= 1) {
            CharaList_unk* list = lbl_8087F430->m10D8;
            if (list->m48 > 0) {
                for (unsigned int i = 0; i < list->mNum; i++) {
                    Vec pos = fn_803CC754(list, i);
                    int x = 0;
                    int z = 0;
                    fn_805B8110(lbl_8087FA20, &pos, &x, &z);
                    if (x == mCellX && z == mCellZ) {
                        atn::Task* chara = fn_803C0634(list, i);
                        if (chara != 0) {
                            mCharas.push_back(chara);
                        }
                    }
                }
            }
        }
        if (mWait == 0 || (mWait->mFlags & 2) == 2) {
            mState++;
        }
        return 0;
    }
    int ready = 1;
    for (int i = 0; i < mCharas.size(); i++) {
        if ((mCharas[i]->mFlags & 2) != 2) {
            ready = 0;
        }
    }
    if (ready) {
        for (int i = 0; i < mCharas.size(); i++) {
            fn_8016E970(mCharas[i], 2);
            fn_800D246C(mCharas[i], 0);
        }
    }
    return ready;
}

TownTpcLoader* TownTpcLoader::fn_805B9B08(atn::Task* parent, int cellX, int cellZ, atn::Task* wait) {
    if (parent != 0) {
        return new (1, "", "", 0) TownTpcLoader(parent, cellX, cellZ, wait);
    }
    return 0;
}
