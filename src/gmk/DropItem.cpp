// inferred: file boundaries from the vtable/RTTI of gmk::DropItem, its own 0.0f in .sdata2 (0x808862C0) and
// its creator 0x804093B4 (see docs/splits-auto-C.md, group 5)

#include "gmk/DropItem.h"
#include "game/Inventory.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int rand();
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" atn::Task* fn_801F64D0(atn::Task* parent, const char* path); // inferred: d2anime task
extern "C" void fn_801F6D7C(atn::Task* task, const char* name, int index, float value);
extern "C" void fn_801F837C(atn::Task* task, const char* name, const wchar_t* text); // inferred: sets a text
extern "C" void fn_801F791C(atn::Task* task, const char* name, int index, float value);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" void fn_80444CF8(wchar_t* text, int item, int count, int rare); // inferred: item name
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" atn::Vector3 lbl_807C7030;              // inferred: zero vector

// inferred: camera (lbl_8087EFB4)
struct DropCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" DropCamera_unk* lbl_8087EFB4;
// inferred: field (lbl_8087F430) and its map information
struct DropMapInfo_unk {
    unsigned char m00[0x48];
    int m48; // inferred: map type
};
struct DropField_unk {
    unsigned char m0000[0x74];
    atn::Vector3 m74; // inferred: camera target
    unsigned char m0080[0x9C - 0x80];
    float m9C;
    unsigned char m00A0[0x54E4 - 0xA0];
    int m54E4; // inferred: state (8 while an event runs)
};
extern "C" DropField_unk* lbl_8087F430;
extern "C" DropMapInfo_unk* fn_80373148(DropField_unk* field);
// inferred: effects
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const float* color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: bullets (lbl_8087F048): 256 entries of 0xA0 bytes
struct DropAttack_unk {
    float m00;
    int m04;
    int m08;
    struct Owner_unk {
        unsigned char m00[0xAC];
        unsigned int mAC;
    }* mOwner; // 0x0C
    int m10;
    float m14;
    float m18;
};
struct DropBulletOwner_unk {
    unsigned char m00[0x48];
    int m48;
};
struct DropBullet_unk {
    unsigned int mFlags;  // 0x154
    atn::Vector3 mPos;    // 0x158
    unsigned char m164[0x170 - 0x164];
    atn::Vector3 mDir;    // 0x170
    unsigned char m17C[0x1AC - 0x17C];
    DropAttack_unk mAttack; // 0x1AC
    unsigned char m1C8[0x1D8 - 0x1C8];
    DropBulletOwner_unk* mOwner; // 0x1D8
    unsigned char m1DC[0x1F4 - 0x1DC];
};
struct DropBulletMgr_unk {
    unsigned char m000[0x154];
    DropBullet_unk mBullets[256];
};
extern "C" DropBulletMgr_unk* lbl_8087F048;
// inferred: segment and capsule of the touch test (names invented)
struct DropSegment_unk {
    DropSegment_unk(const atn::Vector3& a, const atn::Vector3& b) : mA(a), mB(b) {}
    atn::Vector3 mA;
    atn::Vector3 mB;
};
struct DropCapsule_unk {
    DropCapsule_unk(const atn::Vector3& a, const atn::Vector3& b, float r) : mA(a), mB(b), mRadius(r) {}
    atn::Vector3 mA;
    atn::Vector3 mB;
    float mRadius;
};
extern "C" int fn_80051BC4(const DropSegment_unk* seg, const DropCapsule_unk* capsule);
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3& from, const atn::Vector3& to,
                           unsigned int flags, int arg5, int arg6);
extern "C" void* lbl_8087EE98;
// inferred: tutorial message (lbl_8087F490)
struct DropTutorial_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
};
struct DropMessage_unk {
    unsigned char m000[0x764];
    DropTutorial_unk m764;
};
extern "C" DropMessage_unk* lbl_8087F490;
// inferred: arena (lbl_8087F610)
struct DropArena_unk {
    unsigned char m000[0x4FC];
    int m4FC; // inferred: state
    unsigned char m500[0x50C - 0x500];
    int m50C;
};
struct DropArenaSlot_unk {
    int mId;
    int mArg;
};
extern "C" DropArena_unk* lbl_8087F610;
extern "C" int fn_804F5E38(DropArena_unk* arena, unsigned char id);
extern "C" DropArenaSlot_unk* fn_804F5B08(DropArena_unk* arena, unsigned char id);
extern "C" void fn_804F5C9C(DropArena_unk* arena, unsigned char id);
extern "C" void fn_804DB1C4(DropArena_unk* arena, PlayerTask* player, int count);
// inferred: options (lbl_8087F0A8)
struct DropOptions_unk {
    unsigned char m000[0x170];
    int m170;
};
extern "C" DropOptions_unk* lbl_8087F0A8;
// inferred: global object at lbl_8087F628 (as in gmk/Item.cpp)
extern "C" unsigned char fn_806B0E30();
extern "C" int fn_806A8E40();
struct DropState_unk {
    unsigned char m000[0x1F8];
    unsigned int m1F8_31_20 : 12;
    unsigned int mDisabled : 1;
    unsigned int m1F8_18_0 : 19;
    unsigned char m1FC[0xC215 - 0x1FC];
    unsigned char mC215;

    unsigned char Get1_unk() {
        if (mC215 == 0) {
            return mDisabled ? 0 : fn_806B0E30();
        } else {
            return mDisabled ? 0 : fn_806A8E40();
        }
    }
};
extern "C" DropState_unk* lbl_8087F628;
// inferred: items
struct DropWeapon_unk {
    unsigned char m00[0x78];
    int m78; // inferred: kind
    unsigned char m7C[0x80 - 0x7C];
    int m80;
};
extern "C" int fn_8044909C(void* mgr);
extern "C" void fn_80445130(void* mgr, int* id, int* count, int arg);
extern "C" int fn_80448F9C(void* mgr, int id);
extern "C" int fn_80444EFC(CharaTask* chara);
extern "C" int fn_8021E4E4(int* itemId, int* num, int id, unsigned int flags);
extern "C" int fn_80450B84(int table, int arg1, int arg2);
extern "C" int fn_80450B60(int item);
extern "C" DropItemData_unk* fn_80211480(int id);
extern "C" int fn_80206B9C(int id);
extern "C" DropWeapon_unk* fn_80206C50(int id);
extern "C" int fn_8020EF04(int id);
extern "C" DropWeapon_unk* fn_8020EFEC(int id);
extern "C" int fn_8040B394(int item);

// inferred: as in gmk/Item.cpp (the local static is shared with the copy in CharaTask.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}

// inferred: sound of the pick up and pane of the prompt position
static const char* const sGetSound = "SE_UICOM_008";
static const char* const sPosPane = "pos0001(1)";
// inferred: text panes and colour panes of the prompt
static const char* const sTextPanes[] = {"arrowName02", "string0006(1)", "string0003(1)"};
static const char* const sColorPanes[] = {"col0001_01", "col0003", "col0006"};

// inferred: the player can take items (flag 8 of m54C clear; name invented)
inline bool IsFree_unk(CharaTask* chara) {
    return (chara->m54C & 8) != 8;
}

// inferred: the attack can not take items (name invented)
inline bool IsBlocked_unk(DropAttack_unk attack) {
    return (attack.mOwner->mAC & 0x8000) == 0x8000;
}

extern "C" GimmickCommon* fn_804093B4(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) gmk::DropItem(parent, id, arg);
    }
    return 0;
}

namespace gmk {

DropItem::DropItem(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), m154(0), mItem(0), mCount(0), mData(0), mAlpha(0.0f), m168(-1), m17C(0),
      m180(1), m184(0.0f, 12.0f, 0.0f), m190(0), m194(0), m198(0), m29C(0xFF), m2A0(-1) {
    m54 = 0;
    mSlot.fn_8044D9BC(this);
    mSlot.mBits29 = 0;
    mSlot.mFlag24 = 1;
    DropLayout_unk* layout = (DropLayout_unk*)fn_801F64D0(this, "/data/d2anime/ui_slot_ok.d2b");
    if (mLayouts.size() < 2) {
        mLayouts.push_back(layout);
    }
    layout = (DropLayout_unk*)fn_801F64D0(this, "/data/d2anime/ui_slot_ok_rare.d2b");
    if (mLayouts.size() < 2) {
        mLayouts.push_back(layout);
    }
    for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
        if (*it != 0) {
            (*it)->fn_800D246C(1);
        }
    }
}

int DropItem::vf0C() {
    if (fn_803EC91C() == 0 && mSlot.fn_8044E1BC() == 0 && fn_800D3FA4() != 0) {
        for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(0);
            }
        }
        for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
            if (*it != 0) {
                (*it)->mFlags |= 4;
            }
        }
        m54 = 1;
        return 1;
    }
    return 0;
}

// inferred: the item waits on the ground until the player (or one of its bullets) touches it
// NONMATCHING: 90.61% - register allocation (the original keeps the player in r22 and uses r16-r31),
// order of the stores of the tutorial message and of the touch test locals
void DropItem::vf10() {
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    switch (m54) {
    case 3:
        m54 = 4;
        m2A0 = -1;
        break;
    case 4: {
        PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (player == 0) {
            break;
        }
        if (!IsFree_unk(player)) {
            break;
        }
        if (player->mFlagB1) {
            break;
        }
        int near = 0;
        int inRange = 0;
        int touch = 0;
        float nearDist = 70.0f;
        float rangeDist = 40.0f;
        if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0 && fn_80373148(lbl_8087F430)->m48 == 1) {
            rangeDist /= 1.5f;
            nearDist *= 0.5f;
        }
        atn::Vector3 d(m6C.x - lbl_8087EFB4->mPos.x, m6C.y - lbl_8087EFB4->mPos.y, m6C.z - lbl_8087EFB4->mPos.z);
        float camDist = fn_805F9920(&d);
        bool event = false;
        if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8) {
            event = true;
        }
        if (!event && camDist < 22500.0f) {
            atn::Vector3 pos = player->mColli.m3C;
            if (m6C.y - 15.0f < pos.y && pos.y < 15.0f + m6C.y) {
                pos.y = m6C.y;
                atn::Vector3 dd(m6C.x - pos.x, m6C.y - pos.y, m6C.z - pos.z);
                float dist = fn_805F9920(&dd);
                if (dist < nearDist * nearDist) {
                    near = 1;
                }
                if (dist < rangeDist * rangeDist) {
                    inRange = 1;
                }
                if (m190 != 0) {
                    if (m198 != 0) {
                        DropTutorial_unk tutorial;
                        tutorial.m00 = 6;
                        tutorial.m04 = 14;
                        tutorial.m08 = -1;
                        tutorial.m0C = 0;
                        tutorial.m10 = 0;
                        tutorial.m14 = 0;
                        tutorial.m18 = 1;
                        if (lbl_8087F490 != 0) {
                            lbl_8087F490->m764 = tutorial;
                        }
                    }
                    if (m194 != 0) {
                        touch = 1;
                    }
                } else if (dist < 56.25f) {
                    touch = 1;
                }
            }
        }
        if (lbl_8087F048 != 0 && m190 == 0) {
            for (int i = 0; i < 256; i++) {
                DropBullet_unk* bullet = &lbl_8087F048->mBullets[i];
                unsigned int flags = bullet->mFlags;
                if (((flags & 0x10000) == 0x10000 || (flags & 0x2000000) == 0x2000000) && bullet->mOwner != 0 &&
                    bullet->mOwner->m48 == 0) {
                    DropAttack_unk attack = bullet->mAttack;
                    if ((attack.mOwner == 0 || !IsBlocked_unk(attack)) && (flags & 0x200000) != 0x200000 &&
                        !((float)__fabs(fn_805F9920(&bullet->mDir)) < 1.1920929e-7f)) {
                        DropSegment_unk seg(bullet->mPos - bullet->mDir, bullet->mPos + bullet->mDir);
                        DropCapsule_unk capsule(m6C + atn::Vector3(0.0f, 3.5f, 0.0f),
                                                m6C + atn::Vector3(0.0f, 11.0f, 0.0f), 3.5f);
                        if (fn_80051BC4(&seg, &capsule) != 0) {
                            touch = 1;
                        }
                    }
                }
            }
        }
        m2A0 = -1;
        if (touch) {
            if (mSlot.IsBusy_unk()) {
                mSlot.fn_8044EA34(0);
            }
            if (lbl_8087F610 == 0) {
                mSlot.fn_8044F14C();
                fn_8040B200();
            } else {
                if (m190 != 0) {
                    player->fn_80161570(0x1EC, 0, 0, -1.0f, -1.0f);
                    player->mMotion.mTracks[0].m0C = 0.4f;
                }
                if (m29C == 0xFF) {
                    m29C = lbl_8087F628->Get1_unk();
                }
            }
            GimmickMsg_unk msg(5);
            vf40(&msg);
            near = 0;
            inRange = 0;
        }
        if (camDist < 22500.0f) {
            if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 != 0) {
                near = 0;
            }
            if (mSlot.IsBusy_unk()) {
                near = 0;
            }
            if (near) {
                if (mAlpha < 1.0f) {
                    mAlpha = MIN(mAlpha + 0.06666667f, 1.0f);
                }
            } else if (mAlpha > 0.0f) {
                mAlpha = MAX(mAlpha - 0.06666667f, 0.0f);
            }
            if (mAlpha > 0.8f && !(mSlot.mFlag31 && (mSlot.fn_8044E610() != 0 || mSlot.IsBusy_unk())) &&
                (m190 == 0 || lbl_8087F610 == 0)) {
                mSlot.mFlag26 = 1;
                mSlot.mFlag25 = 0;
            } else {
                mSlot.mFlag26 = 0;
                if (touch) {
                    mSlot.mFlag25 = 0;
                } else {
                    mSlot.mFlag25 = 1;
                }
            }
            if (inRange && mSlot.fn_8044E610() != 0) {
                mSlot.fn_8044E644();
            }
        } else {
            mAlpha = 0.0f;
            mSlot.mFlag26 = 0;
            mSlot.mFlag25 = 1;
        }
        bool event2 = false;
        bool show = false;
        if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8) {
            event2 = true;
        }
        if (!event2 && camDist < 160000.0f) {
            show = true;
        }
        if (show) {
            if (fn_80239D58(lbl_8087F3C0, this, 0) == 0) {
                lbl_8087F3C0->mB8 = 1;
                fn_80232B7C(this, 0);
                fn_8023A8B4(lbl_8087F3C0, &mEfp[m168], 0, 0, m6C, lbl_807C7030, mColor, -1, -1, 1, 1.0f);
                lbl_8087F3C0->mB8 = 0;
            }
        } else if (fn_80239D58(lbl_8087F3C0, this, 0) != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        }
        break;
    }
    case 5:
        if (lbl_8087F0A8->m170 == 0 || mSlot.fn_8044EEC0() != 0) {
            fn_8014F8F4()->mEnabled = 0;
            GimmickMsg_unk msg(6);
            vf40(&msg);
            fn_8014F8F4()->mEnabled = 1;
        }
        break;
    case 6:
        if (fn_80239D58(lbl_8087F3C0, this, 0) == 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
            if (m2A0 == -1 || lbl_8087F610 == 0) {
                mFlags |= 4;
            } else {
                m54 = 7;
            }
        }
        break;
    case 7: {
        unsigned int done = 1;
        if (m2A0 == -2) {
            if (m29C == lbl_8087F628->Get1_unk()) {
                if (mData != 0) {
                    mSlot.fn_8044F14C();
                }
                fn_8040B200();
            }
        } else if (m2A0 != -1) {
            int state = fn_804F5E38(lbl_8087F610, m2A0);
            if (state == 0) {
                done = 0;
            } else if (state == 1) {
                DropArenaSlot_unk* slot = fn_804F5B08(lbl_8087F610, m2A0);
                if (slot != 0 && slot->mId == mId && slot->mArg == m4C) {
                    if (mData != 0) {
                        mSlot.fn_8044F14C();
                    }
                    fn_8040B200();
                }
            }
        }
        if (done == 1) {
            m54 = 8;
            fn_804F5C9C(lbl_8087F610, m2A0);
        }
        break;
    }
    case 8:
        if (lbl_8087F0A8->m170 == 0 || mSlot.fn_8044EEC0() != 0) {
            mFlags |= 4;
        }
        break;
    }
    mSlot.fn_8044F434();
    m194 = 0;
    m198 = 0;
}

// inferred: draws the prompt with the item name
// NONMATCHING: 84.49% - the original also builds a dead local with the field camera value and tests
// the colour components against 0 and 1 without using the result (44 instructions, source unknown)
void DropItem::vf14() {
    if (vf60() == 0 || mData == 0) {
        return;
    }
    if (lbl_8087F430 == 0) {
        return;
    }
    if (GimmickManager::instance_ != 0 && GimmickManager::instance_->IsFlag_unk(1) && mSlot.mFlag26) {
        mSlot.mFlag26 = 0;
        mSlot.fn_8044F96C();
    }
    if (vf38() != 0) {
        fn_803ED774(vf38());
    }
    for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
        if (*it != 0) {
            (*it)->mFlags |= 4;
        }
    }
    for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
        if (*it != 0) {
            (*it)->m50 = 10.0f;
        }
    }
    for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
        if (*it != 0) {
            (*it)->m54 = 0.0f;
        }
    }
    if (lbl_8087F610 != 0) {
        bool ok = false;
        if (lbl_8087F610->m4FC == 30 && lbl_8087F610->m50C < 5) {
            ok = true;
        }
        if (!ok) {
            return;
        }
    }
    if (!(mAlpha > 0.0f)) {
        return;
    }
    DropField_unk* field = lbl_8087F430;
    atn::Vector3 d(m6C.x - field->m74.x, m6C.y - field->m74.y, m6C.z - field->m74.z);
    PSVECMag(&d);
    atn::Vector3 pos(m6C.x + m184.x, m6C.y + m184.y, m6C.z + m184.z);
    float scale = 1.0f;
    wchar_t text[0x40];
    if (mSlot.fn_8044E610() != 0) {
        fn_800DD3FC(text, L"?");
        scale *= 1.5f;
    } else if (mSlot.IsBusy_unk()) {
        fn_800DD3FC(text, L"");
    } else {
        fn_80444CF8(text, mItem, mCount, m17C);
    }
    atn::Vector3 screen;
    fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
    if (!(0.0f <= screen.z && screen.z <= 1.0f)) {
        return;
    }
    if (mSlot.fn_80450A78() != 0 &&
        ((mSlot.mFlag31 && (mSlot.IsState56_unk() || mSlot.fn_8044EEC0() != 0)) || !mSlot.mFlag31)) {
        mLayouts.mData[1]->mFlags &= ~4;
    } else {
        mLayouts.mData[0]->mFlags &= ~4;
    }
    for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
        if (*it != 0) {
            fn_801F6D7C(*it, sPosPane, 0, screen.x);
        }
    }
    for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
        if (*it != 0) {
            fn_801F6D7C(*it, sPosPane, 1, screen.y);
        }
    }
    for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
        if (*it != 0) {
            fn_801F6D7C(*it, sPosPane, 2, scale);
        }
    }
    for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
        if (*it != 0) {
            fn_801F6D7C(*it, sPosPane, 3, scale);
        }
    }
    for (unsigned int i = 0; i < 3; i++) {
        if (i == 0) {
            for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
                if (*it != 0) {
                    fn_801F837C(*it, sTextPanes[i], text);
                }
            }
        } else {
            for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
                if (*it != 0) {
                    fn_801F837C(*it, sTextPanes[i], L"");
                }
            }
        }
    }
    for (unsigned int i = 0; i < 3; i++) {
        float alpha = 255.0f * mAlpha;
        for (DropLayout_unk** it = &mLayouts.mData[0]; it != &mLayouts.mData[mLayouts.mSize]; it++) {
            if (*it != 0) {
                fn_801F791C(*it, sColorPanes[i], 0, alpha);
            }
        }
    }
}

int DropItem::vf1C() {
    for (int i = 0; i < 7; i++) {
        if (mEfp[i].IsLoading() != 0) {
            return 1;
        }
    }
    return mEfpGet.IsLoading() != 0;
}

void DropItem::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    atn::EfpFile* efp = mEfp;
    do {
        const char* field = csv.fn_8005B9CC();
        if (*field == ';') {
            continue;
        }
        if (strcmp(field, "EFP") == 0) {
            efp->LoadRequest(csv.fn_8005B9CC());
            efp++;
        } else if (strcmp(field, "EFP_GET") == 0) {
            mEfpGet.LoadRequest(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

// NONMATCHING: 99.93% - the temporaries of the ground ray test are above the position of the item
// animation in the original (here the position is a named local, above them)
int DropItem::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 3:
    case 4: {
        int ok = 0;
        mColor[0] = 1.0f;
        mColor[1] = 1.0f;
        mColor[2] = 1.0f;
        mColor[3] = 1.0f;
        mData = 0;
        if (msg->m0C == 0) {
            if (fn_8021E4E4(&mItem, &mCount, m154, Inventory::instance_ != 0 ? fn_8044909C(Inventory::instance_) : 0) == 0) {
                mItem = 0;
                mCount = 0;
            }
        } else {
            mItem = msg->m04;
            mCount = msg->m08;
            m154 = 0;
        }
        int item = mItem;
        if (Inventory::instance_ != 0) {
            fn_80445130(Inventory::instance_, &mItem, &mCount, 0);
        }
        mData = fn_80211480(mItem);
        if (mData != 0) {
            m168 = fn_8040B394(mItem);
            ok = 1;
            mColor[0] = 1.0f;
            mColor[1] = 1.0f;
            mColor[2] = 1.0f;
            mColor[3] = 1.0f;
            m184.y = 14.5f;
            m17C = 1;
            if (fn_80206B9C(mItem)) {
                m17C = 0;
                int chance = fn_80448F9C(Inventory::instance_, mItem);
                if (chance > rand() % 100) {
                    m17C = 1;
                }
            } else if (fn_8020EF04(mItem) == 0 && mItem == 10004) {
                mCount = fn_80444EFC((CharaTask*)msg->m10);
            }
            ok = 1;
        }
        if (msg->m0C == 0) {
            if (msg->m10 != 0) {
                m6C = ((CharaTask*)msg->m10)->mPos;
                m6C.y += 0.5f;
                m78 = ((CharaTask*)msg->m10)->mRot;
            }
        } else {
            m6C = msg->mVec;
            m6C.y += 0.5f;
            m78 = lbl_807C7030;
            m17C = 1;
        }
        if (m180 != 0) {
            atn::Vector3 hit;
            atn::Vector3 to = m6C + atn::Vector3(0.0f, -200.0f, 0.0f);
            atn::Vector3 from = m6C + atn::Vector3(0.0f, 10.0f, 0.0f);
            if (fn_8004ECC0(lbl_8087EE98, &hit, from, to, 0x80000000, 0, 0) != 0) {
                m6C = hit;
                m6C.y += 0.5f;
            }
        }
        m190 = 0;
        if (lbl_8087F610 != 0 && mData != 0 && fn_80206B9C(mData->mCode)) {
            m17C = 1;
            DropWeapon_unk* weapon = fn_80206C50(mData->mCode);
            if (weapon != 0 && weapon->m78 == 0) {
                m190 = 1;
            }
        }
        if (ok) {
            if (m154 > 0) {
                mSlot.mFlag31 = fn_80450B84(m154, mItem, 0);
            } else {
                mSlot.mFlag31 = fn_80450B60(item);
            }
            m54 = msg->mType;
            m29C = 0xFF;
            mAlpha = 0.0f;
            const atn::Vector3& pos = m6C + m184;
            mSlot.fn_8044E418(&pos, 25, 25, mData, mCount, m17C, m154);
            if (fn_80239D58(lbl_8087F3C0, this, 0) == 0) {
                lbl_8087F3C0->mB8 = 1;
                fn_80232B7C(this, 0);
                atn::EfpFile* efp = &mEfp[m168];
                EffectMgr_unk* mgr = lbl_8087F3C0;
                fn_8023A8B4(mgr, efp, 0, 0, m6C, lbl_807C7030, mColor, -1, -1, 1, 1.0f);
                lbl_8087F3C0->mB8 = 0;
            }
        } else {
            GimmickMsg_unk drop(6);
            vf40(&drop);
        }
        break;
    }
    case 5:
        m54 = msg->mType;
        mAlpha = 0.0f;
        if (fn_80239D58(lbl_8087F3C0, this, 0) != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        }
        if (msg->m0C == 1) {
            mSlot.mFlag26 = 0;
            fn_80232B7C(this, 1);
            EffectMgr_unk* mgr = lbl_8087F3C0;
            fn_8023A8B4(mgr, &mEfpGet, 0, 0, m6C, lbl_807C7030, mColor, -1, -1, 1, 1.0f);
        }
        break;
    case 6:
        if (fn_80239D58(lbl_8087F3C0, this, 0) != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        }
        mAlpha = 0.0f;
        m54 = msg->mType;
        break;
    }
    msg->m04 = mItem;
    msg->m08 = mCount;
    msg->mVec = m6C;
    CharaListeners_unk* listeners = fn_8014F8F4();
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || listener.mId == 11) {
                listener.mFunc(11, (CharaTask*)this, msg);
            }
        }
    }
    return m54;
}

void DropItem::vf68(int arg1, int arg2) {
    m154 = arg1;
}

void DropItem::fn_8040B200() {
    if (mData == 0) {
        return;
    }
    PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    DropWeapon_unk* weapon = fn_80206C50(mData->mCode);
    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    fn_800C31F4(sGetSound, 0, -1, 1.0f);
    fn_80232B7C(this, 1);
    fn_8023A8B4(lbl_8087F3C0, &mEfpGet, 0, 0, m6C, lbl_807C7030, mColor, -1, -1, 1, 1.0f);
    mSlot.fn_80450778();
    if (m190 != 0 && player != 0 && weapon != 0) {
        player->fn_8014F698(weapon->m78, weapon->m80, 1);
    }
    if (lbl_8087F610 != 0) {
        switch (mData->mCode) {
        case 0x44D:
            fn_804DB1C4(lbl_8087F610, player, 1);
            break;
        case 0x44E:
            fn_804DB1C4(lbl_8087F610, player, 3);
            break;
        case 0x44F:
            fn_804DB1C4(lbl_8087F610, player, 5);
            break;
        case 0x450:
            fn_804DB1C4(lbl_8087F610, player, 10);
            break;
        }
    }
}

} // namespace gmk

// inferred: effect of an item kind: 0/1 weapons, 2/3 armour, 4 accessories, 5 special items, 6 others
extern "C" int fn_8040B394(int item) {
    int ret = 6;
    DropItemData_unk* data = item > 0 ? fn_80211480(item) : 0;
    if (fn_80206B9C(item)) {
        DropWeapon_unk* weapon = fn_80206C50(item);
        if (weapon != 0) {
            if (weapon->m78 == 0) {
                ret = 0;
            } else {
                ret = 1;
            }
        }
    } else if (fn_8020EF04(item)) {
        DropWeapon_unk* armour = fn_8020EFEC(item);
        if (armour != 0) {
            if (armour->m78 == 0 || armour->m78 == 2) {
                ret = 2;
            } else {
                ret = 3;
            }
        }
    } else if (item == 10004 || item == 10009) {
        ret = 5;
    } else if (data != 0 && data->mBC == 5) {
        ret = 4;
    }
    if (data != 0 && data->mCode == 411) {
        ret = 5;
    }
    return ret;
}

namespace gmk {

void DropItem::vf6C(void* menu) {}

} // namespace gmk
