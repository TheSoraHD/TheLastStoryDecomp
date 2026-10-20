#include "game/GameSaveBackground.h"
#include "atn/Memory.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

extern "C" void* lbl_8087EF70;                               // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" void* lbl_8087F518;
extern "C" void fn_8046ECDC(void* obj, int arg);
extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_80060D58(void* draw, float x, float y, float z, float w, float h,
                            unsigned int color); // inferred: rectangle
// inferred: text (the float after the color is evaluated after it: see vf14)
extern "C" void fn_800616C0(void* draw, float x, float y, float z, float w, float h, const char* text,
                            unsigned int color, float f6, int a, int b, int c);

// inferred from __vt__12GameSaveTask (0x8078B508): debug save menu (slot list, overwrite prompt)
class GameSaveTask : public GameSaveLoadCommon {
public:
    GameSaveTask(atn::Task* parent, GameSaveBackground* background)
        : GameSaveLoadCommon(parent), mD68(0), mD6C(0), mD70(background) {
        memset(&mD80, 0, sizeof(GameSaveFile_unk));
        fn_803B8758(-1);
    }
    virtual ~GameSaveTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static GameSaveTask* fn_803B7944(atn::Task* parent, GameSaveBackground* background); // inferred: create
    void fn_803B7FC8(int slot); // inferred: saves in `slot`
    void fn_803B807C(int slot); // inferred: deletes `slot`

    // inferred: saves in the selected slot (inlined twice in vf10)
    void Save_unk() {
        mD80.mData.fn_803BDEE4();
        SaveTime_unk time;
        fn_8004B20C(&time);
        mSlots[m48].fn_803BE8C0(&mD80.mData, &time);
        memcpy(mD80.mHeader, &mSlots[m48], sizeof(GameSaveSlot_unk));
        if (mD70 != 0) {
            mD70->fn_803B8E60(&mD80, m48, 1);
        }
        mD6C = 4;
    }

    int mD68; // inferred: overwrite answer (1 = yes)
    int mD6C; // inferred: state
    GameSaveBackground* mD70;
    unsigned char mD74[0xD80 - 0xD74];
    GameSaveFile_unk mD80;
};

GameSaveTask* GameSaveTask::fn_803B7944(atn::Task* parent, GameSaveBackground* background) {
    if (parent != 0) {
        return new (1, "", "", 0) GameSaveTask(parent, background);
    }
    return 0;
}

GameSaveTask::~GameSaveTask() {}

int GameSaveTask::vf0C() {
    fn_8046ECDC(lbl_8087F518, 0);
    if (mD70 != 0 && mD70->m154 != 0) {
        return 0;
    }
    if (fn_803B87A8() == 0) {
        return 1;
    }
    return 0;
}

void GameSaveTask::vf10() {
    if (mD64 == 0) {
        return;
    }
    switch (mD6C) {
    case 0:
        mD6C = 1;
        break;
    case 1:
        if (fn_800A55AC(lbl_8087EF70, 0, 0) || fn_800A55AC(lbl_8087EF70, 0, 0x1A)) {
            m48 = m48 != 0 ? m48 - 1 : 16;
        } else if (fn_800A55AC(lbl_8087EF70, 0, 1) || fn_800A55AC(lbl_8087EF70, 0, 0x19)) {
            m48 = (m48 + 1) % 17;
        }
        if (fn_800A555C(lbl_8087EF70, 0, 4)) {
            if (mSlots[m48].fn_803BEAB4() == 0) {
                Save_unk();
            } else {
                mD6C = 2;
                mD68 = 0;
            }
        } else if (fn_800A555C(lbl_8087EF70, 0, 5)) {
            mD6C = 3;
        }
        break;
    case 2:
        if (fn_800A55AC(lbl_8087EF70, 0, 0x1A) || fn_800A55AC(lbl_8087EF70, 0, 0x19)) {
            mD68 = mD68 == 0;
        }
        if (fn_800A555C(lbl_8087EF70, 0, 4)) {
            mD6C = 1;
            if (mD68 != 0) {
                Save_unk();
            }
        } else if (fn_800A555C(lbl_8087EF70, 0, 5)) {
            mD6C = 1;
        }
        break;
    case 4:
        if (mD70 != 0) {
            if (mD70->m154 == 0) {
                mD6C = 1;
            }
        } else {
            mD6C = 1;
        }
        break;
    case 5:
        if (mD70 != 0) {
            if (mD70->m154 == 0) {
                mD6C = 1;
            }
        } else {
            mD6C = 1;
        }
        break;
    }
}

void GameSaveTask::vf14() {
    if (mD64 == 0) {
        return;
    }
    fn_803B8B08(mSlots);
    switch (mD6C) {
    case 2:
        fn_80060D58(lbl_8087EEB0, 180.0f, 180.0f, 0.0f, 200.0f, 200.0f, 0xFF000000);
        fn_800616C0(lbl_8087EEB0, 200.0f, 200.0f, 0.0f, 16.0f, 16.0f, "上書きしますか？", 0xFFFFFFFF, 0.0f, 1, 1, 0);
        fn_800616C0(lbl_8087EEB0, 250.0f, 240.0f, 0.0f, 16.0f, 16.0f, "はい",
                    mD68 != 0 ? 0xFFFFFF00 : 0xFFFFFFFF, 0.0f, 1, 1, 0);
        fn_800616C0(lbl_8087EEB0, 250.0f, 260.0f, 0.0f, 16.0f, 16.0f, "いいえ",
                    mD68 == 0 ? 0xFFFFFF00 : 0xFFFFFFFF, 0.0f, 1, 1, 0);
        break;
    case 4:
        fn_80060D58(lbl_8087EEB0, 180.0f, 180.0f, 0.0f, 200.0f, 200.0f, 0xFF000000);
        fn_800616C0(lbl_8087EEB0, 200.0f, 200.0f, 0.0f, 16.0f, 16.0f, "セーブ中です", 0xFFFFFFFF, 0.0f, 1, 1, 0);
        break;
    }
}

void GameSaveTask::fn_803B7FC8(int slot) {
    if (mD70 == 0) {
        mD6C = 1;
        return;
    }
    mD80.mData.fn_803BDEE4();
    GameSaveSlot_unk* header = &mSlots[slot];
    SaveTime_unk time;
    fn_8004B20C(&time);
    header->fn_803BE8C0(&mD80.mData, &time);
    memcpy(mD80.mHeader, header, sizeof(GameSaveSlot_unk));
    if (mD70 != 0) {
        mD70->fn_803B8E60(&mD80, slot, 1);
    }
    mD6C = 4;
}

void GameSaveTask::fn_803B807C(int slot) {
    if (mD70 == 0) {
        mD6C = 1;
    } else {
        if (mD70 != 0) {
            mD70->fn_803B8160(slot);
            memset(&mSlots[slot], 0, sizeof(GameSaveSlot_unk));
        }
        mD6C = 5;
    }
}
