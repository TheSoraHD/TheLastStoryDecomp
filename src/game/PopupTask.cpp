// inferred: popups of the battle HUD (combo, critical, weapon skill, headshot, steal... d2anime
// files in .rodata). Class from RTTI "PopupTask" (vtable 0x80794A84: dtor, vf0C, vf10, vf14).
// Only the small functions are written; vf10 (0x8054B09C), vf14 (0x8054B324, 9 KB) and the popup
// helpers (0x8054A9A8, 0x8054D794, 0x8054E0FC, 0x8054E574) are still asm. Names invented.
#include "atn/Task.h"
#include "atn/Memory.h"

extern "C" void* __construct_array(void* block, void (*ctor)(), void (*dtor)(), unsigned long size,
                                   unsigned long n);
extern "C" void __destroy_arr(void* block, void (*dtor)(), unsigned long size, unsigned long n);
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: d2anime task
extern "C" void fn_800D2338(atn::Task* task);                                   // inferred: deletes the task

// inferred: one popup (0xB4 bytes)
struct PopupEntry_unk {
    PopupEntry_unk()
        : m00(0), m04(0.0f), m08(0.0f), m0C(0), m10(0), m14(0), m18(0), m1C(1), m20(0), m60(0.0f), m64(0.0f),
          m68(0.0f), m6C(0.0f), m70(0.0f), m74(0.0f), m78(0), m7C(-1), m80(-1), m84(0), m88(0), m8C(0), m90(0),
          m94(0), m98(0), m9C(0), mA0(0), mA4(0), mA8(0), mAC(0), mB0(0) {}
    ~PopupEntry_unk() {}

    int m00;   // inferred: frame
    float m04; // inferred: duration
    float m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    short m20;
    unsigned char m22[0x60 - 0x22];
    float m60;
    float m64;
    float m68;
    float m6C;
    float m70;
    float m74;
    int m78;
    int m7C;
    int m80;
    int m84;
    int m88;
    unsigned int m8C;
    int m90;
    int m94;
    int m98;
    int m9C;
    int mA0;
    int mA4;
    int mA8;
    int mAC;
    int mB0;
};

// inferred: a d2anime of the popups and its state (0xC bytes)
struct PopupAnime_unk {
    atn::Task* mTask;
    int m04;
    int m08;
};

// inferred: d2anime task (bit 30 of the word at 0xFC is cleared after the creation)
struct PopupD2A_unk : public atn::Task {
    unsigned char m48[0xFC - 0x48];
    unsigned int mFlagFC_hi : 1;
    unsigned int mFlagFC : 1;
    unsigned int mFlagFC_lo : 30;
};

class PopupTask : public atn::Task {
public:
    PopupTask(atn::Task* parent);
    virtual ~PopupTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    void fn_8054AF08();
    void fn_8054E51C();
    void* fn_8054E528(unsigned int id);

    int m48;
    // inferred: counted array of popups (count at 0x4C, entries at 0x50)
    struct Entries_unk {
        Entries_unk() : mCount(0) {}
        ~Entries_unk() { Clear_unk(); }
        void Clear_unk() {}
        unsigned int mCount;
        PopupEntry_unk mData[0x20];
    } mEntries; // 0x4C
    struct Base_unk {
        unsigned char m00[0x04];
        unsigned int mFlags; // 0x16D4->m00 is a task, see the ctor
    };
    unsigned char m16D0[0x4]; // inferred: owner tag of the popups (its address is stored in m94)
    PopupD2A_unk* m16D4;
    unsigned char m16D8[0x16E0 - 0x16D8];
    atn::Task* m16E0;
    unsigned char m16E4[0x16EC - 0x16E4];
    PopupAnime_unk m16EC[3];
    PopupAnime_unk m1710[11];
    PopupAnime_unk m1794[3];
    PopupAnime_unk m17B8;
    PopupAnime_unk m17C4;
    struct {
        atn::Task* mTask;
        int m04;
    } m17D0;
};

extern "C" PopupTask* lbl_8087F8A8;

extern "C" PopupTask* fn_8054A6E8(atn::Task* parent) {
    if (lbl_8087F8A8 == 0) {
        lbl_8087F8A8 = new ((MemoryTag)3, "", "", 0) PopupTask(parent);
    }
    return lbl_8087F8A8;
}

PopupTask::PopupTask(atn::Task* parent) : atn::Task(parent), m48(0) {
    m16D4 = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_combo.d2a", 0);
    m16D4->fn_800D246C(1);
    m16D4->mFlagFC = 0;
    m16E0 = 0;
    for (int i = 0; i < 3; i++) {
        m16EC[i].mTask = 0;
    }
    for (int i = 0; i < 11; i++) {
        m1710[i].mTask = 0;
    }
    for (int i = 0; i < 3; i++) {
        m1794[i].mTask = 0;
    }
    m17B8.mTask = 0;
    m17C4.mTask = 0;
    m17D0.mTask = 0;
}

// NONMATCHING: 97.1% - the original tests the address of the popup array twice before __destroy_arr
// (an inlined destructor of the array class, not reproduced)
PopupTask::~PopupTask() {
    lbl_8087F8A8 = 0;
}

void PopupTask::fn_8054AF08() {
    m48 = 0;
    mEntries.mCount = 0;
    if (m16E0 != 0) {
        fn_800D2338(m16E0);
        m16E0 = 0;
    }
    for (int i = 0; i < 3; i++) {
        if (m16EC[i].mTask != 0) {
            fn_800D2338(m16EC[i].mTask);
            m16EC[i].mTask = 0;
        }
    }
    for (int i = 0; i < 11; i++) {
        if (m1710[i].mTask != 0) {
            fn_800D2338(m1710[i].mTask);
            m1710[i].mTask = 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (m1794[i].mTask != 0) {
            fn_800D2338(m1794[i].mTask);
            m1794[i].mTask = 0;
        }
    }
    if (m17B8.mTask != 0) {
        fn_800D2338(m17B8.mTask);
        m17B8.mTask = 0;
    }
    if (m17C4.mTask != 0) {
        fn_800D2338(m17C4.mTask);
        m17C4.mTask = 0;
    }
    if (m17D0.mTask != 0) {
        fn_800D2338(m17D0.mTask);
        m17D0.mTask = 0;
    }
}

int PopupTask::vf0C() {
    int ret = 1;
    if (fn_800D3FA4() == 0) {
        ret = 0;
    }
    if (ret != 0) {
        mEntries.mCount = 0;
    }
    return ret;
}

// inferred: clears all the popups
void PopupTask::fn_8054E51C() {
    mEntries.mCount = 0;
}

// inferred: detaches the popups of the object `id` (returns the owner tag)
void* PopupTask::fn_8054E528(unsigned int id) {
    for (unsigned int i = 0; i < mEntries.mCount; i++) {
        if ((void*)mEntries.mData[i].m94 == m16D0 && (void*)mEntries.mData[i].m8C == (void*)id) {
            mEntries.mData[i].m94 = 0;
        }
    }
    return m16D0;
}
