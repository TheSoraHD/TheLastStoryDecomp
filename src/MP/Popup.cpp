#include "MP/Popup.h"
#include "atn/Memory.h"
#include "game/CharaTask.h"
#include "game/GimmickCommon.h"
#include "db/WeaponParam_unk.h"
#include "math/Vec.h"
#include <string.h>

// inferred: creates a d2anime task playing `path` (returns the task)
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int arg);

static const char sPathStr[] = "/data/d2a/ui_item_pop2.d2a";
static const char* const sPath = sPathStr; // inferred: separate .rodata array + .sdata2 pointer

namespace atn {
// inferred: d2anime task members used here (declared as in MP/D2AMpMenu_unk.h)
class D2A_Task : public Task {
public:
    void SetColor_unk(const char* name, float r, float g, float b); // 0x801F4AA0
    void SetNumber_unk(const char* name, int value, int digits);    // 0x801F4CB4
};
} // namespace atn

namespace MP {

Popup* Popup::sInstance;

inline Popup::Popup(atn::Task* parent) : atn::Task(parent) {
    for (unsigned int i = 0; i < 4; i++) {
        PopupD2A_unk* item = (PopupD2A_unk*)fn_801F3FF8(this, sPath, 0);
        if (m48.size() < 4) {
            m48.push_back(item);
        }
    }
    for (PopupD2A_unk** it = &m48[0]; it != &m48[m48.size()]; it++) {
        if (*it != 0) {
            (*it)->fn_800D246C(1);
        }
    }
}

Popup* Popup::fn_805012C4(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) Popup(parent);
    }
    return sInstance;
}

Popup::~Popup() {
    sInstance = 0;
}

int Popup::vf0C() {
    if (fn_800D3FA4() != 0) {
        for (PopupD2A_unk** it = &m48[0]; it != &m48[m48.size()]; it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(0);
            }
        }
        for (PopupD2A_unk** it = &m48[0]; it != &m48[m48.size()]; it++) {
            if (*it != 0) {
                (*it)->mFlags |= 4;
            }
        }
        for (PopupD2A_unk** it = &m48[0]; it != &m48[m48.size()]; it++) {
            if (*it != 0) {
                (*it)->mFC &= ~0x10000000;
            }
        }
        return 1;
    }
    return 0;
}

// inferred: player of the session (only the character is read)
struct PopupPlayer_unk {
    CharaTask* m00;
};
// inferred: multiplayer session (lbl_8087F610); only the members read here
struct PopupSession_unk {
    unsigned char m000[0x4FC];
    int m4FC; // inferred: state
    unsigned char m500[0x50C - 0x500];
    int m50C;
    // inferred: name invented
    bool IsPlaying_unk() { return m4FC == 30 && m50C < 5; }
};
extern "C" PopupSession_unk* lbl_8087F610;
extern "C" PopupPlayer_unk* fn_804EB870(PopupSession_unk* s); // inferred: local player
extern "C" atn::Task* lbl_8087F430;                          // inferred: field task
extern "C" void* lbl_8087EFB4;                               // inferred: camera
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" float fn_805F9920(const atn::Vector3* v);       // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" int fn_80450A5C(void* slot);                     // inferred: item code of an ItemSlotAnime
extern "C" const wchar_t* fn_80116FC0(int a, int id);       // inferred: system text
extern "C" void fn_8021C6A4(wchar_t* out, const void* param); // inferred: text of the item type
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);


// inferred: the popup d2anime task with its layout (as MP::MpD2A_unk)
struct PopupTask_unk : public atn::D2A_Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0xA0 - 0x58]; // 0x58
    unsigned char mA0[0x100 - 0xA0];
    float mFrame; // 0x100
};

inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}

// inferred: item gimmick (gmk::DropItem: ItemSlotAnime at 0x19C, its position at 0x1A4)
struct PopupItem_unk {
    unsigned char m000[0x19C];
    unsigned char mSlot[0x08];
    atn::Vector3 mPos; // 0x1A4
};

inline void SetPaneAlpha_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

// inferred: places the item popups over the nearest items in front of the player and shows the
// attack/magic difference with the equipped weapon
// NONMATCHING: 97.6% - registers (r18-r31 shifted by one: the original keeps three copies of the
// wide string pool address and the slot offset), the flag test of the field task (beq/b in the
// original), stack slots of the screen/front vectors and memset instead of the zero loop for the
// type text. Tried (close-units qD, 10 attempts): separate guards, the body in an if/inline,
// bool temporaries, task by index/pointer, screen declared first.
void Popup::vf10() {
    PopupSession_unk* session = lbl_8087F610;
    if (session == 0 || !session->IsPlaying_unk()) {
        return;
    }
    if (fn_804EB870(session) == 0) {
        return;
    }
    if (fn_804EB870(lbl_8087F610)->m00 == 0) {
        return;
    }
    if (lbl_8087F430 == 0) {
        return;
    }
    if (!lbl_8087F430->IsFlag_unk(2)) {
        return;
    }
    {
    CharaTask* chara = fn_804EB870(lbl_8087F610)->m00;
    atn::Vector3 screen;
    for (GimmickCommon* g = GimmickManager::instance_ != 0 ? GimmickManager::instance_->mHead : 0; g != 0; g = g->m5C) {
        if (g->m50 != 0x12 || g->vf60() == 0) {
            continue;
        }
        if (fn_80206C50(fn_80450A5C(((PopupItem_unk*)g)->mSlot)) == 0) {
            continue;
        }
        atn::Vector3 dir = g->m6C - chara->mPos;
        float dist = fn_805F9920(&dir);
        if (!(atn::Abs_unk(dist) < 1.1920929e-7f)) {
            PSVECNormalize(&dir, &dir);
        }
        float far = 0.0f;
        int slot = -1;
        for (int i = 0; i < 4; i++) {
            if (m5C[i].mTarget != 0 && m5C[i].mTarget == g) {
                far = 1.0e16f;
                slot = i;
                break;
            }
            if (far < m5C[i].mDist) {
                far = m5C[i].mDist;
                slot = i;
            }
        }
        if (dist < far && slot >= 0) {
            PopupSlot_unk* p = &m5C[slot];
            p->mTarget = g;
            p->mDist = dist;
            atn::Vector3 front(0.0f, 0.0f, 1.0f);
            Mtx mtx;
            PSMTXRotRad(mtx, 'y', chara->mRot.y);
            PSMTXMultVec(mtx, &front, &front);
            p->mDot = PSVECDotProduct(&dir, &front);
        }
    }
    for (PopupD2A_unk** it = &m48[0]; it != &m48[m48.size()]; it++) {
        if (*it != 0) {
            (*it)->mFlags |= 4;
        }
    }
    float add = 30.0f;
    float r = 20.0f;
    float gr = 100.0f;
    float b = 200.0f;
    for (int i = 0; i < 4; i++) {
        PopupSlot_unk* p = &m5C[i];
        if (p->mTarget == 0 || p->mTarget->vf60() == 0) {
            continue;
        }
        PopupTask_unk* task = (PopupTask_unk*)m48[i];
        fn_800BFAC8(&screen, lbl_8087EFB4, &((PopupItem_unk*)p->mTarget)->mPos);
        if (0.0f <= screen.z && screen.z <= 1.0f && p->mDist < 3600.0f && p->mDot >= 0.5f) {
        SetPaneValue_unk(task->mLayout, "pos0001(1)", 0, screen.x);
        SetPaneValue_unk(task->mLayout, "pos0001(1)", 1, screen.y);
        chr::CharaPartData_unk* own = chara->mParts.mCount != 0 ? chara->mParts[0]->m274 : 0;
        if (chara->m678 != 0) {
            own = chara->m678->m274;
        }
        WeaponParamEntry_unk* item = fn_80206C50(fn_80450A5C(((PopupItem_unk*)p->mTarget)->mSlot));
        if (own == 0 || item == 0) {
            continue;
        }
        int atk = (int)(item->m00 - own->mParam.m00);
        int matk = (int)(item->m04 - own->mParam.m04);
        if (atk > 0) {
            SetPaneText_unk(task->mLayout, "string0006(1)", L"+");
            task->SetNumber_unk("string0001(1)", atk, 0);
            SetPaneText_unk(task->mLayout, "stringUd0", fn_80116FC0(0, 0x79));
            task->SetColor_unk("col0006", r + add, gr + add, b + add);
            task->SetColor_unk("col0001", r + add, gr + add, b + add);
            task->SetColor_unk("colorUd0", r + add, gr + add, b + add);
            task->SetColor_unk("colorArrow00", r + add, gr + add, b + add);
            task->SetColor_unk("colorArrow01", r + add, gr + add, b + add);
            SetPaneAlpha_unk(task->mLayout, "atkOn", 1.0f);
            SetPaneAlpha_unk(task->mLayout, "atkUp", 0.0f);
        } else if (atk < 0) {
            SetPaneText_unk(task->mLayout, "string0006(1)", L"-");
            task->SetNumber_unk("string0001(1)", -atk, 0);
            SetPaneText_unk(task->mLayout, "stringUd0", fn_80116FC0(0, 0x72));
            task->SetColor_unk("col0006", 205.0f, 90.0f, 90.0f);
            task->SetColor_unk("col0001", 205.0f, 90.0f, 90.0f);
            task->SetColor_unk("colorUd0", 205.0f, 90.0f, 90.0f);
            task->SetColor_unk("colorArrow00", 205.0f, 90.0f, 90.0f);
            task->SetColor_unk("colorArrow01", 205.0f, 90.0f, 90.0f);
            SetPaneAlpha_unk(task->mLayout, "atkOn", 1.0f);
            SetPaneAlpha_unk(task->mLayout, "atkUp", 1.0f);
        } else {
            SetPaneText_unk(task->mLayout, "string0006(1)", L"");
            task->SetNumber_unk("string0001(1)", 0, 0);
            SetPaneText_unk(task->mLayout, "stringUd0", L"\x2015\x2015");
            task->SetColor_unk("col0006", 150.0f, 150.0f, 150.0f);
            task->SetColor_unk("col0001", 150.0f, 150.0f, 150.0f);
            task->SetColor_unk("colorUd0", 150.0f, 150.0f, 150.0f);
            task->SetColor_unk("colorArrow00", 150.0f, 150.0f, 150.0f);
            task->SetColor_unk("colorArrow01", 150.0f, 150.0f, 150.0f);
            SetPaneAlpha_unk(task->mLayout, "atkOn", 0.0f);
            SetPaneAlpha_unk(task->mLayout, "atkUp", 0.0f);
        }
        if (matk > 0) {
            SetPaneText_unk(task->mLayout, "string0013(1)", L"+");
            task->SetNumber_unk("string0007(1)", matk, 0);
            SetPaneText_unk(task->mLayout, "stringUd1", fn_80116FC0(0, 0x79));
            task->SetColor_unk("col0013", r + add, gr + add, b + add);
            task->SetColor_unk("col0007", r + add, gr + add, b + add);
            task->SetColor_unk("colorUd1", r + add, gr + add, b + add);
            task->SetColor_unk("colorArrow10", r + add, gr + add, b + add);
            task->SetColor_unk("colorArrow11", r + add, gr + add, b + add);
            SetPaneAlpha_unk(task->mLayout, "matkOn", 1.0f);
            SetPaneAlpha_unk(task->mLayout, "matkUp", 0.0f);
        } else if (matk < 0) {
            SetPaneText_unk(task->mLayout, "string0013(1)", L"-");
            task->SetNumber_unk("string0007(1)", -matk, 0);
            SetPaneText_unk(task->mLayout, "stringUd1", fn_80116FC0(0, 0x72));
            task->SetColor_unk("col0013", 205.0f, 90.0f, 90.0f);
            task->SetColor_unk("col0007", 205.0f, 90.0f, 90.0f);
            task->SetColor_unk("colorUd1", 205.0f, 90.0f, 90.0f);
            task->SetColor_unk("colorArrow10", 205.0f, 90.0f, 90.0f);
            task->SetColor_unk("colorArrow11", 205.0f, 90.0f, 90.0f);
            SetPaneAlpha_unk(task->mLayout, "matkOn", 1.0f);
            SetPaneAlpha_unk(task->mLayout, "matkUp", 1.0f);
        } else {
            SetPaneText_unk(task->mLayout, "string0013(1)", L"");
            task->SetNumber_unk("string0007(1)", 0, 0);
            SetPaneText_unk(task->mLayout, "stringUd1", L"\x2015\x2015");
            task->SetColor_unk("col0013", 150.0f, 150.0f, 150.0f);
            task->SetColor_unk("col0007", 150.0f, 150.0f, 150.0f);
            task->SetColor_unk("colorUd1", 150.0f, 150.0f, 150.0f);
            task->SetColor_unk("colorArrow10", 150.0f, 150.0f, 150.0f);
            task->SetColor_unk("colorArrow11", 150.0f, 150.0f, 150.0f);
            SetPaneAlpha_unk(task->mLayout, "matkOn", 0.0f);
            SetPaneAlpha_unk(task->mLayout, "matkUp", 0.0f);
        }
        wchar_t type[0x100];
        memset(type, 0, sizeof(type));
        fn_8021C6A4(type, item->mC4);
        SetPaneText_unk(task->mLayout, "string0016(1)", type);
        SetPaneAlpha_unk(task->mLayout, "type", 0.0f);
        task->mFlags &= ~4;
        } else {
            task->mFrame = 0.0f;
        }
    }
    }
}

} // namespace MP
