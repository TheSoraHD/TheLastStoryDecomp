// inferred: popups of the battle HUD (combo, critical, weapon skill, headshot, steal... d2anime
// files in .rodata). Class from RTTI "PopupTask" (vtable 0x80794A84: dtor, vf0C, vf10, vf14).
// vf14 (0x8054B324, 9 KB) and the popup helpers (0x8054D794, 0x8054E0FC, 0x8054E574) are still
// asm. Names invented.
#include "atn/FixedArray.h"
#include "atn/Task.h"
#include "atn/Memory.h"
#include "atn/static_string.h"
#include "math/Vec.h"

extern "C" void* __construct_array(void* block, void (*ctor)(), void (*dtor)(), unsigned long size,
                                   unsigned long n);
extern "C" void __destroy_arr(void* block, void (*dtor)(), unsigned long size, unsigned long n);
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: d2anime task
extern "C" void fn_800D2338(atn::Task* task);                                   // inferred: deletes the task
extern "C" unsigned int fn_800DC6B4(const char* s);                             // inferred: string hash
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
// inferred: sets the value `index` of the pane `name` (as in game/MenuCommonTask.cpp)
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);

namespace atn {
// inferred: the wide string of the popups (as in game/NewCampPartsOption.h; the copy keeps the source
// pointer in a register: a local in the copy assignment)
template <>
inline static_string<32, wchar_t, std::char_traits<wchar_t> >::static_string() {
    mStr[0] = 0;
}
template <>
inline static_string<32, wchar_t, std::char_traits<wchar_t> >::~static_string() {}
template <>
inline static_string<32, wchar_t, std::char_traits<wchar_t> >&
static_string<32, wchar_t, std::char_traits<wchar_t> >::operator=(const static_string& o) {
    const wchar_t* str = o.mStr;
    if (str != mStr) {
        wmemcpy(mStr, str, wcslen(str) + 1);
    }
    return *this;
}
} // namespace atn

// inferred: one popup (0xB4 bytes; copied member by member when a popup is removed, PopupTask::vf10)
struct PopupEntry_unk {
    PopupEntry_unk()
        : m00(0), m04(0.0f), m08(0.0f), m0C(0), m10(0), m14(0), m18(0), m1C(1), m60(0.0f, 0.0f, 0.0f),
          m6C(0.0f, 0.0f, 0.0f), m78(0), m7C(-1), m80(-1), m84(0), m88(0), m8C(0), m90(0), m94(0), m98(0), m9C(0),
          mA0(0), mA4(0), mA8(0), mAC(0), mB0(0) {}
    ~PopupEntry_unk() {}
    PopupEntry_unk& operator=(const PopupEntry_unk& o) {
        m00 = o.m00;
        m04 = o.m04;
        m08 = o.m08;
        m0C = o.m0C;
        m10 = o.m10;
        m14 = o.m14;
        m18 = o.m18;
        m1C = o.m1C;
        m20 = o.m20;
        m60 = o.m60;
        m6C = o.m6C;
        m78 = o.m78;
        m7C = o.m7C;
        m80 = o.m80;
        m84 = o.m84;
        m88 = o.m88;
        m8C = o.m8C;
        m90 = o.m90;
        m94 = o.m94;
        m98 = o.m98;
        m9C = o.m9C;
        mA0 = o.mA0;
        mA4 = o.mA4;
        mA8 = o.mA8;
        mAC = o.mAC;
        mB0 = o.mB0;
        return *this;
    }

    int m00;   // inferred: frame
    float m04; // inferred: duration
    float m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    atn::static_string<32, wchar_t, std::char_traits<wchar_t> > m20; // inferred: text
    atn::Vector3 m60;
    atn::Vector3 m6C;
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

// inferred: d2anime task (bits 30 and 28 of the word at 0xFC are cleared after the creation)
struct PopupD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0xFC - 0x58]; // 0x58: inferred: layout (as MP::Popup's PopupTask_unk)
    unsigned int mFlagFC_hi : 1;
    unsigned int mFlagFC : 1;
    unsigned int mFlagFC_29 : 1;
    unsigned int mFlagFC2 : 1;
    unsigned int mFlagFC_lo : 28;
};

// inferred: a d2anime of the popups and its state (0xC bytes)
struct PopupAnime_unk {
    PopupD2A_unk* mTask;
    int m04;
    int m08;
};

class PopupTask : public atn::Task {
public:
    PopupTask(atn::Task* parent);
    virtual ~PopupTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    void fn_8054A9A8();
    void fn_8054AF08();
    void fn_8054E51C();
    void* fn_8054E528(unsigned int id);

    int m48;
    // inferred: counted array of popups (count at 0x4C, entries at 0x50)
    // (a class of its own over the fixed array: the destructor tests the address twice, 0x8054A950)
    struct Entries_unk : public atn::FixedArray_unk<PopupEntry_unk, 0x20> {} mEntries; // 0x4C
    struct Base_unk {
        unsigned char m00[0x04];
        unsigned int mFlags; // 0x16D4->m00 is a task, see the ctor
    };
    unsigned char m16D0[0x4]; // inferred: owner tag of the popups (its address is stored in m94)
    PopupD2A_unk* m16D4;
    unsigned char m16D8[0x16E0 - 0x16D8];
    PopupD2A_unk* m16E0;
    unsigned char m16E4[0x16EC - 0x16E4];
    PopupAnime_unk m16EC[3];
    PopupAnime_unk m1710[11];
    PopupAnime_unk m1794[3];
    PopupAnime_unk m17B8;
    PopupAnime_unk m17C4;
    struct {
        PopupD2A_unk* mTask;
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

PopupTask::~PopupTask() {
    lbl_8087F8A8 = 0;
}

// inferred: loads the d2anime files of the popups (once)
void PopupTask::fn_8054A9A8() {
    if (m48 == 0) {
        m48 = 1;
        m16E0 = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_critical.d2a", 0);
        m16E0->fn_800D246C(1);
        m16E0->mFlagFC = 0;
        m16E0->mFlagFC2 = 0;
        m16EC[0].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_weapon_skill.d2a", 0);
        m16EC[0].mTask->fn_800D246C(1);
        m16EC[0].mTask->mFlagFC = 0;
        m16EC[0].mTask->mFlagFC2 = 0;
        SetPaneValue_unk(m16EC[0].mTask->mLayout, "basePoint", 2, 3.0f);
        m16EC[1].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_headshot.d2a", 0);
        m16EC[1].mTask->fn_800D246C(1);
        m16EC[1].mTask->mFlagFC = 0;
        m16EC[2].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_steal.d2a", 0);
        // inferred: the original clears this flag on the steal anime instead of the headshot one (a slip)
        m16EC[2].mTask->mFlagFC2 = 0;
        m16EC[2].mTask->fn_800D246C(1);
        m16EC[2].mTask->mFlagFC = 0;
        m16EC[2].mTask->mFlagFC2 = 0;
        m1710[0].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage.d2a", 0);
        m1710[0].mTask->fn_800D246C(1);
        m1710[0].mTask->mFlagFC = 0;
        m1710[0].mTask->mFlagFC2 = 0;
        m1710[1].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_guard.d2a", 0);
        m1710[1].mTask->fn_800D246C(1);
        m1710[1].mTask->mFlagFC = 0;
        m1710[1].mTask->mFlagFC2 = 0;
        m1710[2].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_fire.d2a", 0);
        m1710[2].mTask->fn_800D246C(1);
        m1710[2].mTask->mFlagFC = 0;
        m1710[2].mTask->mFlagFC2 = 0;
        m1710[3].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_ice.d2a", 0);
        m1710[3].mTask->fn_800D246C(1);
        m1710[3].mTask->mFlagFC = 0;
        m1710[3].mTask->mFlagFC2 = 0;
        m1710[4].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_holy.d2a", 0);
        m1710[4].mTask->fn_800D246C(1);
        m1710[4].mTask->mFlagFC = 0;
        m1710[4].mTask->mFlagFC2 = 0;
        m1710[5].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_forest.d2a", 0);
        m1710[5].mTask->fn_800D246C(1);
        m1710[5].mTask->mFlagFC = 0;
        m1710[5].mTask->mFlagFC2 = 0;
        m1710[6].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_star.d2a", 0);
        m1710[6].mTask->fn_800D246C(1);
        m1710[6].mTask->mFlagFC = 0;
        m1710[6].mTask->mFlagFC2 = 0;
        m1710[7].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_dark.d2a", 0);
        m1710[7].mTask->fn_800D246C(1);
        m1710[7].mTask->mFlagFC = 0;
        m1710[7].mTask->mFlagFC2 = 0;
        m1710[8].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_mydamage.d2a", 0);
        m1710[8].mTask->fn_800D246C(1);
        m1710[8].mTask->mFlagFC = 0;
        m1710[8].mTask->mFlagFC2 = 0;
        m1710[9].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_mydamage_guard.d2a", 0);
        m1710[9].mTask->fn_800D246C(1);
        m1710[9].mTask->mFlagFC = 0;
        m1710[9].mTask->mFlagFC2 = 0;
        m1710[10].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_heal.d2a", 0);
        m1710[10].mTask->fn_800D246C(1);
        m1710[10].mTask->mFlagFC = 0;
        m1710[10].mTask->mFlagFC2 = 0;
        m1794[0].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_exp.d2a", 0);
        m1794[0].mTask->fn_800D246C(1);
        m1794[0].mTask->mFlagFC = 0;
        m1794[0].mTask->mFlagFC2 = 0;
        m1794[1].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_exp_summon.d2a", 0);
        m1794[1].mTask->fn_800D246C(1);
        m1794[1].mTask->mFlagFC = 0;
        m1794[1].mTask->mFlagFC2 = 0;
        m1794[2].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_exp_bonus.d2a", 0);
        m1794[2].mTask->fn_800D246C(1);
        m1794[2].mTask->mFlagFC = 0;
        m1794[2].mTask->mFlagFC2 = 0;
        m17B8.mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_boost.d2a", 0);
        m17B8.mTask->fn_800D246C(1);
        m17B8.mTask->mFlagFC = 0;
        m17B8.mTask->mFlagFC2 = 0;
        m17C4.mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_point_powered.d2a", 0);
        m17C4.mTask->fn_800D246C(1);
        m17C4.mTask->mFlagFC = 0;
        m17C4.mTask->mFlagFC2 = 0;
        m17D0.mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_element_hit.d2a", 0);
        m17D0.mTask->fn_800D246C(1);
        m17D0.mTask->mFlagFC = 0;
        m17D0.mTask->mFlagFC2 = 0;
    }
}

void PopupTask::fn_8054AF08() {
    m48 = 0;
    mEntries.mSize = 0;
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
        mEntries.mSize = 0;
    }
    return ret;
}

// inferred: counts the frames of the popups and removes the finished ones
// NONMATCHING: 99.69% - the index of the erase loop and the text pointer of the copy swap r24/r25;
// tried the text pointer as a local of an explicit PopupEntry_unk::operator= (first or in place),
// a setter, and the erase loop written out.
void PopupTask::vf10() {
    for (PopupEntry_unk* e = mEntries.begin(); e != mEntries.end();) {
        e->m00++;
        if (e->m00 >= e->m04) {
            if (e->m94 != 0) {
                e->m94 = 0;
            }
            if (e->m90 != 0) {
                e->m90 = 0;
            }
            if (e->m98 != 0) {
                e->m98 = 0;
            }
            if (e->m9C != 0) {
                e->m9C = 0;
            }
            if (e->mA0 != 0) {
                e->mA0 = 0;
            }
            if (e->mA4 != 0) {
                e->mA4 = 0;
            }
            if (e->mA8 != 0) {
                e->mA8 = 0;
            }
            if (e->mAC != 0) {
                e->mAC = 0;
            }
            mEntries.erase(e);
        } else {
            e++;
        }
    }
}

// inferred: clears all the popups
void PopupTask::fn_8054E51C() {
    mEntries.mSize = 0;
}

// inferred: detaches the popups of the object `id` (returns the owner tag)
void* PopupTask::fn_8054E528(unsigned int id) {
    for (unsigned int i = 0; i < mEntries.mSize; i++) {
        if ((void*)mEntries.mData[i].m94 == m16D0 && (void*)mEntries.mData[i].m8C == (void*)id) {
            mEntries.mData[i].m94 = 0;
        }
    }
    return m16D0;
}
