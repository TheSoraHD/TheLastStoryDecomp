// inferred: popups of the battle HUD (combo, critical, weapon skill, headshot, steal... d2anime
// files in .rodata). Class from RTTI "PopupTask" (vtable 0x80794A84: dtor, vf0C, vf10, vf14).
// vf14 (0x8054B324, 9 KB) is still asm. Names invented.
#include "game/PopupTask.h"
#include "atn/FixedArray.h"
#include "atn/Task.h"
#include "atn/Memory.h"
#include "atn/static_string.h"
#include "math/Vec.h"
#include "game/CharaTask.h"
#include "game/SetConfig.h"
#include "game/GameTask.h"
#include "game/GameUI.h"
#include "game/GameConfig_unk.h"
#include "atn/SoundRef.h"

extern "C" int rand();
extern "C" GameTask* lbl_8087F430;
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" int fn_800DC1DC(const wchar_t* s); // inferred: number of a text
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch

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


extern "C" PopupTask* fn_8054A6E8(atn::Task* parent) {
    if (lbl_8087F8A8 == 0) {
        lbl_8087F8A8 = new ((MemoryTag)3, "", "", 0) PopupTask(parent);
    }
    return lbl_8087F8A8;
}

PopupTask::PopupTask(atn::Task* parent) : atn::Task(parent), m48(0) {
    m16D0.mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_combo.d2a", 0);
    m16D0.mTask->fn_800D246C(1);
    m16D0.mTask->mFlagFC = 0;
    m16DC.mTask = 0;
    for (int i = 0; i < 3; i++) {
        m16E8[i].mTask = 0;
    }
    for (int i = 0; i < 11; i++) {
        m170C[i].mTask = 0;
    }
    for (int i = 0; i < 3; i++) {
        m1790[i].mTask = 0;
    }
    m17B4.mTask = 0;
    m17C0.mTask = 0;
    m17CC.mTask = 0;
}

PopupTask::~PopupTask() {
    lbl_8087F8A8 = 0;
}

// inferred: loads the d2anime files of the popups (once)
void PopupTask::fn_8054A9A8() {
    if (m48 == 0) {
        m48 = 1;
        m16DC.mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_critical.d2a", 0);
        m16DC.mTask->fn_800D246C(1);
        m16DC.mTask->mFlagFC = 0;
        m16DC.mTask->mFlagFC2 = 0;
        m16E8[0].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_weapon_skill.d2a", 0);
        m16E8[0].mTask->fn_800D246C(1);
        m16E8[0].mTask->mFlagFC = 0;
        m16E8[0].mTask->mFlagFC2 = 0;
        SetPaneValue_unk(m16E8[0].mTask->mLayout, "basePoint", 2, 3.0f);
        m16E8[1].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_headshot.d2a", 0);
        m16E8[1].mTask->fn_800D246C(1);
        m16E8[1].mTask->mFlagFC = 0;
        m16E8[2].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_steal.d2a", 0);
        // inferred: the original clears this flag on the steal anime instead of the headshot one (a slip)
        m16E8[2].mTask->mFlagFC2 = 0;
        m16E8[2].mTask->fn_800D246C(1);
        m16E8[2].mTask->mFlagFC = 0;
        m16E8[2].mTask->mFlagFC2 = 0;
        m170C[0].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage.d2a", 0);
        m170C[0].mTask->fn_800D246C(1);
        m170C[0].mTask->mFlagFC = 0;
        m170C[0].mTask->mFlagFC2 = 0;
        m170C[1].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_guard.d2a", 0);
        m170C[1].mTask->fn_800D246C(1);
        m170C[1].mTask->mFlagFC = 0;
        m170C[1].mTask->mFlagFC2 = 0;
        m170C[2].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_fire.d2a", 0);
        m170C[2].mTask->fn_800D246C(1);
        m170C[2].mTask->mFlagFC = 0;
        m170C[2].mTask->mFlagFC2 = 0;
        m170C[3].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_ice.d2a", 0);
        m170C[3].mTask->fn_800D246C(1);
        m170C[3].mTask->mFlagFC = 0;
        m170C[3].mTask->mFlagFC2 = 0;
        m170C[4].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_holy.d2a", 0);
        m170C[4].mTask->fn_800D246C(1);
        m170C[4].mTask->mFlagFC = 0;
        m170C[4].mTask->mFlagFC2 = 0;
        m170C[5].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_forest.d2a", 0);
        m170C[5].mTask->fn_800D246C(1);
        m170C[5].mTask->mFlagFC = 0;
        m170C[5].mTask->mFlagFC2 = 0;
        m170C[6].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_star.d2a", 0);
        m170C[6].mTask->fn_800D246C(1);
        m170C[6].mTask->mFlagFC = 0;
        m170C[6].mTask->mFlagFC2 = 0;
        m170C[7].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_dark.d2a", 0);
        m170C[7].mTask->fn_800D246C(1);
        m170C[7].mTask->mFlagFC = 0;
        m170C[7].mTask->mFlagFC2 = 0;
        m170C[8].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_mydamage.d2a", 0);
        m170C[8].mTask->fn_800D246C(1);
        m170C[8].mTask->mFlagFC = 0;
        m170C[8].mTask->mFlagFC2 = 0;
        m170C[9].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_mydamage_guard.d2a", 0);
        m170C[9].mTask->fn_800D246C(1);
        m170C[9].mTask->mFlagFC = 0;
        m170C[9].mTask->mFlagFC2 = 0;
        m170C[10].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_damage_heal.d2a", 0);
        m170C[10].mTask->fn_800D246C(1);
        m170C[10].mTask->mFlagFC = 0;
        m170C[10].mTask->mFlagFC2 = 0;
        m1790[0].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_exp.d2a", 0);
        m1790[0].mTask->fn_800D246C(1);
        m1790[0].mTask->mFlagFC = 0;
        m1790[0].mTask->mFlagFC2 = 0;
        m1790[1].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_exp_summon.d2a", 0);
        m1790[1].mTask->fn_800D246C(1);
        m1790[1].mTask->mFlagFC = 0;
        m1790[1].mTask->mFlagFC2 = 0;
        m1790[2].mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_exp_bonus.d2a", 0);
        m1790[2].mTask->fn_800D246C(1);
        m1790[2].mTask->mFlagFC = 0;
        m1790[2].mTask->mFlagFC2 = 0;
        m17B4.mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_boost.d2a", 0);
        m17B4.mTask->fn_800D246C(1);
        m17B4.mTask->mFlagFC = 0;
        m17B4.mTask->mFlagFC2 = 0;
        m17C0.mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_point_powered.d2a", 0);
        m17C0.mTask->fn_800D246C(1);
        m17C0.mTask->mFlagFC = 0;
        m17C0.mTask->mFlagFC2 = 0;
        m17CC.mTask = (PopupD2A_unk*)fn_801F3FF8(this, "data/d2anime/ui_element_hit.d2a", 0);
        m17CC.mTask->fn_800D246C(1);
        m17CC.mTask->mFlagFC = 0;
        m17CC.mTask->mFlagFC2 = 0;
    }
}

void PopupTask::fn_8054AF08() {
    m48 = 0;
    mEntries.mSize = 0;
    if (m16DC.mTask != 0) {
        fn_800D2338(m16DC.mTask);
        m16DC.mTask = 0;
    }
    for (int i = 0; i < 3; i++) {
        if (m16E8[i].mTask != 0) {
            fn_800D2338(m16E8[i].mTask);
            m16E8[i].mTask = 0;
        }
    }
    for (int i = 0; i < 11; i++) {
        if (m170C[i].mTask != 0) {
            fn_800D2338(m170C[i].mTask);
            m170C[i].mTask = 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (m1790[i].mTask != 0) {
            fn_800D2338(m1790[i].mTask);
            m1790[i].mTask = 0;
        }
    }
    if (m17B4.mTask != 0) {
        fn_800D2338(m17B4.mTask);
        m17B4.mTask = 0;
    }
    if (m17C0.mTask != 0) {
        fn_800D2338(m17C0.mTask);
        m17C0.mTask = 0;
    }
    if (m17CC.mTask != 0) {
        fn_800D2338(m17CC.mTask);
        m17CC.mTask = 0;
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

// inferred: std::min of two floats
static inline float MinF_unk(float a, float b) {
    return a < b ? a : b;
}

// NONMATCHING: 99.15% - TakeAnime_unk keeps the anime in a register for m16E8[2]/[1] but recomputes it for
// m16E8[0] in the original (and the loop pointer/anime registers are swapped); L"%+d" in the .data pool of
// the wide strings of vf14 (not written)
// inferred: adds a popup of the kind `kind` (0: damage, 1: heal, 2: anime `value`, 3/4: text, 6/7: animes,
// 8: number `value`) with its animes selected by `flags`, `combo` and `elem`
void PopupTask::fn_8054D794(int kind, const wchar_t* text, const atn::Vector3* pos, int value, unsigned int flags,
                            int combo, int arg7, CharaTask* src, CharaTask* owner, int m1C, int elem, int m80) {
    if (mEntries.size() >= 0x20) {
        return;
    }
    if (owner != 0 && (owner->mType == 0 || owner->mType == 3) && SetConfig::sInstance->mDrawFriendPopup == 0) {
        return;
    }
    if (owner != 0 && owner->mType == 2 && SetConfig::sInstance->mDrawEnemyPopup == 0) {
        return;
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8) {
        return;
    }
    int hud = 0;
    if (owner != 0 && owner->mType == 2 && kind == 1) {
        GameUI* ui = lbl_8087F490;
        ui->fn_803E6BF0((unsigned int)owner, fn_800DC1DC(text));
        hud = 1;
    }
    PopupEntry_unk e;
    e.m10 = kind;
    e.m20 = text;
    e.m60 = *pos;
    e.m6C = lbl_807C7030;
    e.m78 = value;
    e.m14 = flags;
    e.m1C = m1C;
    e.m88 = (int)src;
    e.m8C = (unsigned int)owner;
    e.m7C = elem;
    e.m80 = m80;
    e.m04 = 30.0f;
    e.m08 = 0.0f;
    e.m0C = 0;
    if (hud == 0) {
        switch (kind) {
        case 0:
            e.m04 = 30.0f;
            if (owner != 0 && (owner->mType == 0 || owner->mType == 3)) {
                if ((flags & 8) == 8 || (flags & 0x40000000) == 0x40000000) {
                    e.m9C = Anime_unk(&m170C[9]);
                } else {
                    e.m9C = Anime_unk(&m170C[8]);
                }
            } else {
                switch (elem) {
                case 0:
                    e.m9C = Anime_unk(&m170C[2]);
                    break;
                case 1:
                    e.m9C = Anime_unk(&m170C[3]);
                    break;
                case 3:
                    e.m9C = Anime_unk(&m170C[5]);
                    break;
                case 4:
                    e.m9C = Anime_unk(&m170C[4]);
                    break;
                case 6:
                    e.m9C = Anime_unk(&m170C[6]);
                    break;
                case 5:
                    e.m9C = Anime_unk(&m170C[7]);
                    break;
                case 2:
                    break;
                default:
                    if (elem < 0) {
                        if ((flags & 8) == 8 || (flags & 0x40000000) == 0x40000000) {
                            e.m9C = Anime_unk(&m170C[1]);
                        } else {
                            e.m9C = Anime_unk(&m170C[0]);
                        }
                    }
                    break;
                }
            }
            break;
        case 1:
            e.m04 = 30.0f;
            e.m9C = Anime_unk(&m170C[10]);
            break;
        case 2:
            e.mA0 = Anime_unk(&m1790[value]);
            if (e.mA0 != 0) {
                e.m04 = e.mA0->mTask->mLastFrame;
            }
            break;
        case 3:
        case 4:
            e.m04 = 30.0f;
            break;
        case 6:
            e.mA4 = TakeLevelUp_unk();
            if (e.mA4 != 0) {
                e.m04 = e.mA4->mTask->mLastFrame;
            }
            break;
        case 7:
            e.mA4 = Anime_unk(&m17C0);
            if (e.mA4 != 0) {
                e.m04 = e.mA4->mTask->mLastFrame;
            }
            break;
        case 8: {
            e.m04 = 45.0f;
            wchar_t number[256];
            fn_800DD3FC(number, L"%+d", value);
            e.m20 = number;
            break;
        }
        }
        if (src != 0 && combo > 1 && src->fn_8017A450() == 0) {
            e.m94 = TakeCombo_unk(owner);
            if (e.m94 != 0) {
                e.m84 = combo;
                e.m04 = e.m94->mTask->mLastFrame;
                atn::SoundRef sound = fn_800C31F4("SE_UICOM_023", 0, -1, 1.3f);
                float pitch = 1.0f;
                for (int n = combo - 1; n > 0; n--) {
                    pitch *= 1.122462f;
                }
                fn_800CB688(&sound, 0, MinF_unk(pitch, 2.0f));
            }
        }
        if ((flags & 0x200) == 0x200) {
            e.m98 = TakeAnime_unk(&m16E8[2], src, owner);
        } else if ((flags & 0x2000000) == 0x2000000) {
            e.m98 = TakeAnime_unk(&m16E8[0], src, owner);
        }
        if ((flags & 0x80000000) == 0x80000000) {
            e.mAC = TakeAnime_unk(&m16E8[1], src, owner);
        }
        if ((flags & 0x80) == 0x80) {
            e.m90 = TakeCritical_unk(src, owner);
        }
        if (owner != 0 && owner->mType == 2 && lbl_8087F0A8->m194 == 0 && m80 >= 0) {
            e.mA8 = Anime_unk(&m17CC);
        }
    }
    fn_8054E0FC(&e);
}

// NONMATCHING: 99.87% - the owner of the new popup gets r4 and SetConfig::sInstance r3 (r3/r4 in the original)
// inferred: adds a popup: popups of friends need "draw friend popup", of enemies "draw enemy popup"; a popup at
// the same place as another moves up and gets a random direction (a quarter turn more for each one)
void PopupTask::fn_8054E0FC(PopupEntry_unk* entry) {
    if (mEntries.size() >= 0x20) {
        return;
    }
    if (entry->m8C != 0 && (((CharaTask*)entry->m8C)->mType == 0 || ((CharaTask*)entry->m8C)->mType == 3) &&
        SetConfig::sInstance->mDrawFriendPopup == 0) {
        return;
    }
    if (entry->m8C != 0 && ((CharaTask*)entry->m8C)->mType == 2 && SetConfig::sInstance->mDrawEnemyPopup == 0) {
        return;
    }
    float angle = 0.0f;
    float near = 1.0f;
    float far = 8.0f;
    float step = 1.5707964f; // inferred: a quarter turn
    for (unsigned int i = 0; i < mEntries.size(); i++) {
        PopupEntry_unk& other = mEntries[i];
        if (other.m00 == 0) {
            if ((other.m60 - entry->m60).Length_unk() < near) {
                CharaTask* owner = (CharaTask*)other.m8C;
                if (owner != 0 && (owner->mType == 0 || owner->mType == 3)) {
                    entry->m08 += near;
                    entry->m04 += near;
                    other.m0C = 1;
                    other.m9C = m170C[9].mTask == 0 ? 0 : &m170C[9];
                } else {
                    entry->m08 += far;
                    entry->m04 += far;
                }
                atn::Vector3 dir(-20.0f, 0.0f, 0.0f);
                Mtx m;
                PSMTXRotRad(m, 'z', angle + step * ((float)(rand() % 1001) / 1000.0f));
                PSMTXMultVec(m, &dir, &dir);
                angle += step;
                entry->m6C = dir;
            }
        }
    }
    mEntries.push_back(*entry);
}

// inferred: clears all the popups
void PopupTask::fn_8054E51C() {
    mEntries.mSize = 0;
}

// inferred: detaches the popups of the object `id` (returns the owner tag)
void* PopupTask::fn_8054E528(unsigned int id) {
    for (unsigned int i = 0; i < mEntries.mSize; i++) {
        if (mEntries.mData[i].m94 == &m16D0 && (void*)mEntries.mData[i].m8C == (void*)id) {
            mEntries.mData[i].m94 = 0;
        }
    }
    return &m16D0;
}
