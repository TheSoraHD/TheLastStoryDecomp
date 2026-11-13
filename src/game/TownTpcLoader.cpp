#include "game/TownTpcLoader.h"
#include "game/GameEventScript.h"
#include "game/GameVoice.h"
#include "game/GameScene.h"
#include "atn/Memory.h"
#include "math/Vec.h"
#include "game/CharaTask.h"
#include "game/TownTask.h"

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
extern "C" atn::Vector3 lbl_807C7030;
struct Target_unk {
    unsigned char m000[0xD3C];
    int mD3C;
    int mD40;
    atn::Vector3 mD44;
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

extern "C" Field_unk* lbl_8087F430;
extern "C" Target_unk* lbl_8087F490;
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task

struct Chara_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    void* m58;
};

TownTpcLoader* TownTpcLoader::fn_805B9B08(atn::Task* parent, int cellX, int cellZ, atn::Task* wait) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) TownTpcLoader(parent, cellX, cellZ, wait);
    }
    return 0;
}

// inferred: starts the loaded characters (name invented; the inline gives the loop registers)
static inline void StartCharas_unk(atn::FixedArray_unk<atn::Task*, 64>& charas) {
    for (int i = 0; i < charas.size(); i++) {
        ((CharaTask*)charas[i])->fn_8016E970(2);
        charas[i]->fn_800D246C(0);
    }
}

int TownTpcLoader::vf0C() {
    if (mState < 2) {
        if (mState >= 1) {
            CharaList_unk* list = lbl_8087F430->m10D8;
            if (list->m48 > 0) {
                for (unsigned int i = 0; i < list->mNum; i++) {
                    atn::Vector3 pos = ((GameScene*)list)->fn_803CC754(i);
                    int x = 0;
                    int z = 0;
                    lbl_8087FA20->fn_805B8110(&pos, &x, &z);
                    if (x == mCellX && z == mCellZ) {
                        atn::Task* chara = ((GameScene*)list)->fn_803C0634(i);
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
        StartCharas_unk(mCharas);
    }
    return ready;
}

void TownTpcLoader::fn_805B9D58() {
    CharaList_unk* list = lbl_8087F430->m10D8;
    for (int i = 0; i < mCharas.size(); i++) {
        ((GameScene*)list)->fn_803CC774(mCharas[i]);
        ((GameEventScript*)list->m134)->fn_803957F0((int)((Chara_unk*)mCharas[i])->m58);
        ((GameScene*)list)->fn_803CC718(mCharas[i], 0);
        if (lbl_8087F498 != 0) {
            lbl_8087F498->fn_803EAC3C((CharaTask*)mCharas[i]);
        }
        if (lbl_8087F490 != 0) {
            lbl_8087F490->Clear_unk(mCharas[i]);
        }
        fn_800D2338(mCharas[i]);
    }
    fn_800D2338(this);
}

TownTpcLoader::~TownTpcLoader() {}
