// inferred: popups of the battle HUD (combo, critical, weapon skill, headshot, steal... d2anime
// files in .rodata). Class from RTTI "PopupTask" (vtable 0x80794A84: dtor, vf0C, vf10, vf14).
// Names invented.
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
#include "atn/RenderManager.h"
#include "atn/GraphicsDevice.h"
#include "atn/DrawHelper.h"
#include "game/PlayerTask.h"
#include "game/CharaManager.h"

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

// inferred: the player aims (flag 31 and +0xC48; name invented)
static inline bool IsAiming_unk(PlayerTask* p) {
    bool r = false;
    if (p->mFlag31 && p->mC48 != 0) {
        r = true;
    }
    return r;
}
// inferred: alpha of a popup that fades in and out over `n` frames (name invented)
static inline float PopupFade_unk(PopupEntry_unk* e, float n) {
    float a = 1.0f;
    if ((float)e->m00 < n) {
        a = (float)e->m00 / n;
    } else if (e->m04 - (float)e->m00 < n) {
        a = (e->m04 - (float)e->m00) / n;
    }
    return a;
}
// inferred: progress of a popup (frame / duration; name invented)
static inline float PopupRate_unk(PopupEntry_unk* e) {
    float r = 1.0f;
    if (e->m04 > 0.0f) {
        r = (float)e->m00 / e->m04;
    }
    return r;
}
// inferred: places a d2anime of a popup: frame, base point (position, scale, depth); names invented
static inline void PopupPlace_unk(PopupEntry_unk* e, PopupAnime_unk* PopupEntry_unk::*anime, float x, float y,
                                  float sx, float sy, float z) {
    SetPaneValue_unk((e->*anime)->mTask->mLayout, "basePoint", 0, x);
    SetPaneValue_unk((e->*anime)->mTask->mLayout, "basePoint", 1, y);
    SetPaneValue_unk((e->*anime)->mTask->mLayout, "basePoint", 2, sx);
    SetPaneValue_unk((e->*anime)->mTask->mLayout, "basePoint", 3, sy);
    SetPaneValue_unk((e->*anime)->mTask->mLayout, "basePoint", 4, z);
}
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);
extern "C" void fn_801F4484(PopupD2A_unk* task); // inferred: updates the d2anime
extern "C" void fn_801F465C(PopupD2A_unk* task, int arg); // inferred: draws the d2anime
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void* lbl_8087EEC8;                    // inferred: font
extern "C" float fn_8006EF48(void* font, float size, const wchar_t* text, float f2, int a, int b); // inferred: width
extern "C" void* fn_801FEC74(void* layout, const char* name); // inferred: finds a pane
extern "C" std::basic_string<wchar_t> fn_800A4450(void* pane, int a, int b); // inferred: text of a pane
// inferred: message table (lbl_8087F1E4; as MP/MpCampMsgTable_unk.h, whose header conflicts here)
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct PopupMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" PopupMsgTable_unk* lbl_8087F1E4;
inline void SetPaneFloat_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}

// inferred: draws the popups: the text of each one (damage, heal, text kinds 3/4/5/8) over its screen
// position, then its animes (level up, damage, critical, combo, element...) stacked above it, and the
// notices of its flags (steal, headshot...)
// NONMATCHING: 97.3% - the original keeps more float constants spilled as doubles on the stack (30, 60, 72...)
// and reloads them; float registers and scheduling of the popup loop
void PopupTask::vf14() {
    PlayerTask* player = lbl_8087F8A0->mHead;
    float z = 150.0f;
    if (player != 0) {
        if (player->mFlag6 || (player->mFlag31 && IsAiming_unk(player))) {
            z = -5.0f;
        }
    }
    for (PopupEntry_unk* e = mEntries.begin(); e != mEntries.end(); e++) {
        atn::Vector3 pos = e->m60;
        if (e->mB0 != 0) {
            MtxPtr m = (MtxPtr)e->mB0;
            atn::Vector3 trans(m[0][3], m[1][3], m[2][3]);
            pos += trans;
        }
        atn::Vector3 screen = atn::RenderManager::instance_->GetScreenPosition(pos);
        screen.x += e->m6C.x;
        screen.y += e->m6C.y;
        screen.z += e->m6C.z;
        if (screen.z < 0.0f || screen.z > 1.0f) {
            continue;
        }
        float size = 16.0f;
        if (e->m1C == 0) {
            size *= 1.5f;
        } else if (e->m1C == 2) {
            size *= 0.75f;
        }
        float mul = 1.0f;
        if (lbl_8087F430->mFinish.m00 != 0 && (e->m10 == 0 || e->m10 == 4)) {
            mul *= 2.0f;
        }
        size *= mul;
        unsigned int alpha = 0xFF000000;
        if (((unsigned int)e->m14 & 0x40000) == 0x40000) {
            // inferred: kept inside the screen
            atn::Vector3 center(0.5f * (float)lbl_8087EEE0->mWidth, 0.5f * (float)lbl_8087EEE0->mHeight, 0.0f);
            float radius = 0.5f * (0.9f * (float)lbl_8087EEE0->mWidth);
            float width = fn_8006EF48(lbl_8087EEC8, size, e->m20.mStr, 0.0f, 1, 1);
            atn::Vector3 d = screen - center;
            if (d.Length_unk() > radius) {
                d.Normalize_unk();
                screen = center + d * radius;
            }
            float margin = 30.0f + width;
            if (screen.x < margin) {
                screen.x = margin;
            } else if (screen.x > (float)lbl_8087EEE0->mWidth - margin) {
                screen.x = (float)lbl_8087EEE0->mWidth - margin;
            }
            if (screen.y < 60.0f) {
                screen.y = 60.0f;
            } else if (screen.y > (float)lbl_8087EEE0->mHeight - (30.0f + size)) {
                screen.y = (float)lbl_8087EEE0->mHeight - (30.0f + size);
            }
        }
        float y = screen.y;
        float x;
        switch (e->m10) {
        case 0:
        case 1:
            if (((unsigned int)e->m14 & 8) != 8) {
                alpha = (unsigned int)(255.0f * PopupFade_unk(e, 10.0f)) << 24;
            } else {
                alpha = (unsigned int)(255.0f * PopupFade_unk(e, 10.0f)) << 24;
            }
            break;
        case 3: {
            float fade = PopupFade_unk(e, 10.0f);
            float rise = 20.0f * PopupRate_unk(e);
            alpha = (unsigned int)(160.0f * fade) << 24;
            float width = fn_8006EF48(lbl_8087EEC8, size, e->m20.mStr, 0.0f, 1, 1);
            y -= rise;
            x = screen.x - 0.5f * width;
            unsigned int color = 0x33CC00;
            if (e->m10 == 3) {
                color = e->m78 & 0xFFFFFF;
            }
            lbl_8087EEB0->fn_80061824(2.0f + x, 2.0f + y, z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, 0xA0000000,
                                      1, 1, 0, 0, 0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y, z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, color | alpha, 1, 1, 0, 0,
                                      0xFF000000);
            break;
        }
        case 4: {
            float width = fn_8006EF48(lbl_8087EEC8, size, e->m20.mStr, 0.0f, 1, 1);
            y -= 1.2f * size;
            x = screen.x - 0.5f * width;
            lbl_8087EEB0->fn_80061824(1.5f + x, y, 1e-4f + z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, alpha, 1, 4,
                                      0, 0, 0xFF000000);
            lbl_8087EEB0->fn_80061824(x, 1.5f + y, 1e-4f + z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, alpha, 1, 4,
                                      0, 0, 0xFF000000);
            lbl_8087EEB0->fn_80061824(x - 1.5f, y, 1e-4f + z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, alpha, 1, 4,
                                      0, 0, 0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y - 1.5f, 1e-4f + z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, alpha, 1, 4,
                                      0, 0, 0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y, z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, alpha | 0xFFC040, 1, 4, 0, 0,
                                      0xFF000000);
            break;
        }
        case 5: {
            float fade = PopupFade_unk(e, 6.0f);
            float rise = 15.0f * (1.0f < (float)e->m00 / 6.0f ? 1.0f : (float)e->m00 / 6.0f);
            alpha = (unsigned int)(160.0f * fade) << 24;
            float width = fn_8006EF48(lbl_8087EEC8, size, e->m20.mStr, 0.0f, 1, 1);
            y -= rise;
            x = screen.x - 0.5f * width;
            unsigned int color = e->m78 & 0xFFFFFF;
            lbl_8087EEB0->fn_80061824(2.0f + x, 2.0f + y, z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, 0xA0000000,
                                      1, 4, 0, 0, 0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y, z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, color | alpha, 1, 4, 0, 0,
                                      0xFF000000);
            break;
        }
        case 8: {
            int value = e->m78;
            float fade = PopupFade_unk(e, 10.0f);
            float rise = 30.0f * PopupRate_unk(e);
            alpha = (unsigned int)(240.0f * fade) << 24;
            size *= 3.0f;
            float width = fn_8006EF48(lbl_8087EEC8, size, e->m20.mStr, 0.0f, 1, 1);
            x = screen.x - 0.5f * width;
            if (value > 0) {
                y -= rise;
            } else {
                y += rise;
            }
            unsigned int color = value > 0 ? 0xFFFFFF : 0x888888;
            lbl_8087EEB0->fn_80061824(2.0f + x, 2.0f + y, z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, 0xA0000000,
                                      1, 4, 0, 0, 0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y, z, size, size, 0.0f, 0.0f, 0.0f, e->m20.mStr, color | alpha, 1, 4, 0, 0,
                                      0xFF000000);
            break;
        }
        }
        if (e->mA4 != 0) {
            e->mA4->mTask->mFrame_unk = (float)e->m00;
            PopupPlace_unk(e, &PopupEntry_unk::mA4, screen.x, y, mul, mul, z);
            SetPaneFloat_unk(e->mA4->mTask->mLayout, "data", (float)(unsigned int)e->m78);
            fn_801F4484(e->mA4->mTask);
            fn_801F465C(e->mA4->mTask, 1);
            y -= 20.0f * mul;
        }
        if (e->m9C != 0) {
            if (e->m0C != 0 && 10.0f <= (float)e->m00 - e->m08) {
                e->m00 = (int)e->m04 - 10;
                e->m0C = 0;
            }
            e->m9C->mTask->mFrame_unk = 0.0f > (float)e->m00 - e->m08 ? 0.0f : (float)e->m00 - e->m08;
            PopupPlace_unk(e, &PopupEntry_unk::m9C, screen.x, y, mul, mul, z);
            SetPaneText_unk(e->m9C->mTask->mLayout, "damage", e->m20.mStr);
            SetPaneText_unk(e->m9C->mTask->mLayout, "damageSub", e->m20.mStr);
            fn_801F4484(e->m9C->mTask);
            fn_801F465C(e->m9C->mTask, 1);
            y -= 20.0f * mul;
        }
        if (e->mA0 != 0) {
            e->mA0->mTask->mFrame_unk = (float)e->m00;
            PopupPlace_unk(e, &PopupEntry_unk::mA0, screen.x, y, mul, mul, z);
            SetPaneText_unk(e->mA0->mTask->mLayout, "damage", e->m20.mStr);
            float width = fn_8006EF48(lbl_8087EEC8, 20.0f, e->m20.mStr, 0.0f, 1, 4);
            SetPaneFloat_unk(e->mA0->mTask->mLayout, "ofs", width);
            fn_801F4484(e->mA0->mTask);
            fn_801F465C(e->mA0->mTask, 1);
            y -= 20.0f * mul;
        }
        wchar_t text[32];
        if (e->m18 != 0) {
            unsigned int color = e->m18 > 0 ? 0x7FFF7F : 0xFF0000;
            color |= alpha;
            fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x69));
            float width = fn_8006EF48(lbl_8087EEC8, size, text, 0.0f, 1, 1);
            y -= 1.2f * size;
            x = screen.x - 0.5f * width;
            lbl_8087EEB0->fn_80061824(2.0f + x, 2.0f + y, z, size, size, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y, z, size, size, 0.0f, 0.0f, 0.0f, text, color, 1, 4, 0, 0, 0xFF000000);
        }
        if (e->m94 != 0) {
            y -= 3.0f;
            e->m94->mTask->mFrame_unk = (float)e->m00;
            SetPaneValue_unk(e->m94->mTask->mLayout, "basePoint", 0, screen.x);
            SetPaneValue_unk(e->m94->mTask->mLayout, "basePoint", 1, y);
            if (e->m88 != 0 && ((CharaTask*)e->m88)->mType == 0) {
                SetPaneValue_unk(e->m94->mTask->mLayout, "basePoint", 2, 1.9f);
                SetPaneValue_unk(e->m94->mTask->mLayout, "basePoint", 3, 1.9f);
            } else {
                SetPaneValue_unk(e->m94->mTask->mLayout, "basePoint", 2, 1.0f);
                SetPaneValue_unk(e->m94->mTask->mLayout, "basePoint", 3, 1.0f);
            }
            wchar_t combo[4];
            if (e->m84 < 100) {
                fn_800DD3FC(combo, L"%d", e->m84);
                SetPaneFloat_unk(e->m94->mTask->mLayout, "showMul", 1.0f);
            } else {
                fn_800DD3FC(combo, L"MAX");
                SetPaneFloat_unk(e->m94->mTask->mLayout, "showMul", 0.0f);
            }
            SetPaneText_unk(e->m94->mTask->mLayout, "comboNum", combo);
            SetPaneText_unk(e->m94->mTask->mLayout, "comboNum2", combo);
            if (e->m84 < 10) {
                SetPaneFloat_unk(e->m94->mTask->mLayout, "ofsX", 18.0f);
            } else {
                SetPaneFloat_unk(e->m94->mTask->mLayout, "ofsX", 0.0f);
            }
            fn_801F4484(e->m94->mTask);
            fn_801F465C(e->m94->mTask, 1);
            y -= 20.0f;
        }
        if (e->mA8 != 0) {
            y -= 3.0f;
            e->mA8->mTask->mFrame_unk = (float)e->m00;
            PopupPlace_unk(e, &PopupEntry_unk::mA8, screen.x, y, 1.5f * mul, 1.5f * mul, z);
            switch (e->m80) {
            case 0:
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colR", 255.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colG", 155.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colB", 55.0f);
                break;
            case 1:
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colR", 100.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colG", 150.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colB", 200.0f);
                break;
            case 3:
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colR", 50.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colG", 200.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colB", 50.0f);
                break;
            case 4:
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colR", 255.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colG", 255.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colB", 200.0f);
                break;
            case 6:
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colR", 0.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colG", 100.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colB", 200.0f);
                break;
            case 5:
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colR", 150.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colG", 50.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colB", 150.0f);
                break;
            default:
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colR", 255.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colG", 255.0f);
                SetPaneFloat_unk(e->mA8->mTask->mLayout, "colB", 255.0f);
                break;
            }
            fn_801F4484(e->mA8->mTask);
            fn_801F465C(e->mA8->mTask, 1);
            y -= 20.0f * mul;
        }
        if (e->m90 != 0) {
            y -= 3.0f;
            e->m90->mTask->mFrame_unk = (float)e->m00;
            PopupPlace_unk(e, &PopupEntry_unk::m90, screen.x, y, 1.5f * mul, 1.5f * mul, z);
            fn_801F4484(e->m90->mTask);
            fn_801F465C(e->m90->mTask, 1);
            y -= 20.0f * mul;
        }
        if (e->m98 != 0) {
            y -= 3.0f;
            e->m98->mTask->mFrame_unk = (float)e->m00;
            SetPaneValue_unk(e->m98->mTask->mLayout, "basePoint", 0, screen.x);
            SetPaneValue_unk(e->m98->mTask->mLayout, "basePoint", 1, y);
            SetPaneValue_unk(e->m98->mTask->mLayout, "basePoint", 4, z);
            float w = fn_8006EF48(lbl_8087EEC8, 16.0f,
                                  fn_800A4450(fn_801FEC74(e->m98->mTask->mLayout, "string0007"), 0, 0).c_str(), 0.0f,
                                  1, 1);
            w /= 72.0f;
            SetPaneValue_unk(e->m98->mTask->mLayout, "basePoint", 2, 1.5f * w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos0007", 2, 11.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos0001", 2, 96.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos0002", 2, 63.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos0003", 2, 41.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos0004", 2, 84.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos0004", 0, -35.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos0005", 2, 72.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos0006", 2, -21.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos0008", 2, -15.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos00010", 2, 20.0f / w);
            SetPaneValue_unk(e->m98->mTask->mLayout, "pos00011", 2, 40.0f / w);
            fn_801F4484(e->m98->mTask);
            fn_801F465C(e->m98->mTask, 1);
            y -= 20.0f;
        }
        if (e->mAC != 0) {
            y -= 3.0f;
            e->mAC->mTask->mFrame_unk = (float)e->m00;
            SetPaneValue_unk(e->mAC->mTask->mLayout, "basePoint", 0, screen.x);
            SetPaneValue_unk(e->mAC->mTask->mLayout, "basePoint", 1, y);
            SetPaneValue_unk(e->mAC->mTask->mLayout, "basePoint", 4, z);
            fn_801F4484(e->mAC->mTask);
            fn_801F465C(e->mAC->mTask, 1);
            y -= 20.0f;
        }
        if (((unsigned int)e->m14 & 0x400) == 0x400) {
            wcscpy(text, lbl_8087F1E4->Get_unk(0x6B));
            float s = 0.6f * size;
            float width = fn_8006EF48(lbl_8087EEC8, s, text, 0.0f, 1, 1);
            y -= 1.2f * s;
            x = screen.x - 0.5f * width;
            lbl_8087EEB0->fn_80061824(2.0f + x, 2.0f + y, z, s, s, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y, z, s, s, 0.0f, 0.0f, 0.0f, text, alpha | 0xFFC040, 1, 4, 0, 0, 0xFF000000);
        }
        if (((unsigned int)e->m14 & 1) != 1 && (((unsigned int)e->m14 & 8) == 8 || ((unsigned int)e->m14 & 0x8000) == 0x8000 ||
                                  ((unsigned int)e->m14 & 0x40000000) == 0x40000000 || ((unsigned int)e->m14 & 0x100) == 0x100)) {
            if (((unsigned int)e->m14 & 0x8000) == 0x8000) {
                wcscpy(text, lbl_8087F1E4->Get_unk(0x73));
            } else if (((unsigned int)e->m14 & 0x40000000) == 0x40000000) {
                wcscpy(text, lbl_8087F1E4->Get_unk(0x83));
            } else if (((unsigned int)e->m14 & 0x100) == 0x100) {
                wcscpy(text, lbl_8087F1E4->Get_unk(0x85));
            } else {
                wcscpy(text, lbl_8087F1E4->Get_unk(0x6F));
            }
            float s = 0.8f * size;
            float width = fn_8006EF48(lbl_8087EEC8, s, text, 0.0f, 1, 1);
            y -= 1.2f * s;
            x = screen.x - 0.5f * width;
            lbl_8087EEB0->fn_80061824(1.5f + x, y, 1e-4f + z, s, s, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x, 1.5f + y, 1e-4f + z, s, s, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x - 1.5f, y, 1e-4f + z, s, s, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y - 1.5f, 1e-4f + z, s, s, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y, z, s, s, 0.0f, 0.0f, 0.0f, text, alpha | 0xFFC040, 1, 4, 0, 0, 0xFF000000);
        }
        if (((unsigned int)e->m14 & 0x4000) == 0x4000) {
            wcscpy(text, lbl_8087F1E4->Get_unk(0x75));
            float s = 0.6f * size;
            float width = fn_8006EF48(lbl_8087EEC8, s, text, 0.0f, 1, 1);
            y -= 1.2f * s;
            x = screen.x - 0.5f * width;
            lbl_8087EEB0->fn_80061824(2.0f + x, 2.0f + y, z, s, s, 0.0f, 0.0f, 0.0f, text, 0, 1, 4, 0, 0, 0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y, z, s, s, 0.0f, 0.0f, 0.0f, text, alpha | 0xFFC040, 1, 4, 0, 0, 0xFF000000);
        }
        if (((unsigned int)e->m14 & 0x200000) == 0x200000) {
            wcscpy(text, lbl_8087F1E4->Get_unk(0x77));
            float width = fn_8006EF48(lbl_8087EEC8, size, text, 0.0f, 1, 1);
            y -= 1.2f * size;
            x = screen.x - 0.5f * width;
            lbl_8087EEB0->fn_80061824(1.5f + x, y, 1e-4f + z, size, size, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x, 1.5f + y, 1e-4f + z, size, size, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x - 1.5f, y, 1e-4f + z, size, size, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y - 1.5f, 1e-4f + z, size, size, 0.0f, 0.0f, 0.0f, text, alpha, 1, 4, 0, 0,
                                      0xFF000000);
            lbl_8087EEB0->fn_80061824(x, y, z, size, size, 0.0f, 0.0f, 0.0f, text, alpha | 0xFFC040, 1, 4, 0, 0,
                                      0xFF000000);
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
void* PopupTask::fn_8054E528(unsigned int id, int) {
    for (unsigned int i = 0; i < mEntries.mSize; i++) {
        if (mEntries.mData[i].m94 == &m16D0 && (void*)mEntries.mData[i].m8C == (void*)id) {
            mEntries.mData[i].m94 = 0;
        }
    }
    return &m16D0;
}
