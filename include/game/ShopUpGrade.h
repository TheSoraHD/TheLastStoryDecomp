#ifndef GAME_SHOPUPGRADE_H
#define GAME_SHOPUPGRADE_H

#include "game/ShopBase.h"
#include "db/ItemParam_unk.h"
#include "db/WeaponParam_unk.h"
#include "db/EquipParam_unk.h"
#include "atn/FixedArray.h"

class CharaTask;
namespace chr {
struct CharaEquip_unk;
}

// inferred: line of the upgrade list (0x58 bytes, built by fn_805969D4); names invented
// inferred: with a constructor its copies are member-wise as in the original (fn_8058CA44), but the
// original push_back (fn_805969D4) takes the POD path of MSL
struct ShopUpGradeEntry_unk {
    ShopUpGradeEntry_unk() : mCode(0), mPrice(0) { memset(m08, 0, sizeof(m08)); }
    int mCode;         // 0x00: inferred: item code
    int mPrice;        // 0x04
    unsigned char m08[0x40];
    int mCanUpgrade;   // 0x48
    int mHave;         // 0x4C: inferred: 1 for the items of the bag and the equipment, 0 for items to identify
    int mMember;       // 0x50: inferred: party member who wears it (-1: bag)
    int mSlot;         // 0x54: inferred: slot of the member (-1: bag)
};

// inferred: equipment to reload after an upgrade (0x0C bytes); names invented
struct ShopUpGradeReload_unk {
    int mKind;                   // inferred: 0 equipment model, 1 character
    chr::CharaEquip_unk* mEquip; // 0x04
    CharaTask* mChara;           // 0x08
};

// inferred: one top menu of the shop (rodata 0x80761D08): its entries and their number
struct ShopUpGradeMenu_unk {
    const int* mItems;
    int mCount;
};

// inferred from __vt__11ShopUpGrade (0x80796A44): upgrade shop (singleton, 0x8087F9E0); mMode selects the
// shop (0: upgrade, 1: Rotta, 2-3: extra upgrade)
class ShopUpGrade : public ShopBase {
public:
    static void fn_8058AABC(); // inferred: frees the upgraded items that nobody wears and that are not in the bag
    static ShopUpGrade* fn_8058ABF4(atn::Task* parent, int a, int b, int mode); // inferred: creates the instance
    ShopUpGrade(atn::Task* parent, int a, int b, int mode); // 0x8058AC90
    virtual ~ShopUpGrade();                                 // 0x8058B404
    virtual int vf0C();                                     // 0x8058B4A4
    virtual void vf10();                                    // 0x8058B864
    virtual void vf14();                                    // 0x8058B92C
    virtual void vf1C(int state);                           // 0x8058BBB0
    virtual void vf20(int result);                          // 0x8058BE78
    virtual int vf3C();                                     // 0x8059886C
    virtual void vf40();                                    // 0x805987C4
    // inferred: FixedArray-like push (placement copy, then the count)
    void PushReload_unk(const ShopUpGradeReload_unk& reload) {
        new (&mReload[mReloadNum]) ShopUpGradeReload_unk(reload);
        mReloadNum++;
    }
    void fn_8058C74C(); // inferred: input of the top menu
    void fn_8058CA44(); // inferred: input of the list
    void fn_8058CF60(); // inferred: result of the upgrade
    void fn_8058D16C(); // inferred: draws the list state
    void fn_8058D458(); // inferred: draws the result state
    void fn_8058D6D8(); // inferred: draws the confirmation states
    int fn_8058D7F8(ShopUpGradeEntry_unk* entry); // inferred: checks the entry, returns an error code
    int fn_8058D994(ShopUpGradeEntry_unk* entry, int give); // inferred: upgrades the item, returns the new code
    void fn_8058DB10(); // inferred: builds the list
    int IsTarget_unk(EquipParamEntry_unk* param);
    void fn_805969D4(int code, int member, int slot, int have); // inferred: adds a line to the list
    void fn_80597098(int a, int b, int tab); // inferred: restores the cursor of the tab
    void fn_80597164(const ShopUpGradeEntry_unk& entry, int& cursor, int& top); // inferred: finds the entry again
    void fn_80597268(int arg);
    void fn_80597554(ShopUpGradeEntry_unk* entry); // inferred: draws the selected item
    // inferred: draws an item line (price, needed number, number; -1 hides them)
    void fn_80598530(ShopLayout_unk* layout, ItemParamEntry_unk* item, int price, int need, int num, int have);

    int mMode;                       // 0x25B68
    ShopD2A_unk* mTop;               // 0x25B6C
    ShopD2A_unk* mContent;           // 0x25B70
    ShopD2A_unk* mNoItem;            // 0x25B74
    ShopD2A_unk* mListBase;          // 0x25B78
    ShopLayout_unk* mList[10];       // 0x25B7C
    ShopLayout_unk* mColor;          // 0x25BA4
    ShopLayout_unk* mSkill[2];       // 0x25BA8
    ShopLayout_unk* mSkillBlue[2];   // 0x25BB0
    ShopLayout_unk* mColorNoItem;    // 0x25BB8
    ShopLayout_unk* mColors[3];      // 0x25BBC
    ShopLayout_unk* mStatusUp[3];    // 0x25BC8
    ShopLayout_unk* mStatusDown[3];  // 0x25BD4
    ShopD2A_unk* mEffect[2];         // 0x25BE0: inferred: [0] never created, [1] "ui_upgrade00"
    ShopD2A_unk* mCursorAnime;       // 0x25BE8
    ShopD2A_unk* mSort;              // 0x25BEC
    ItemParamEntry_unk mItem;        // 0x25BF0: inferred: item after the upgrade
    WeaponParamEntry_unk mWeapon;    // 0x25D28
    EquipParamEntry_unk mEquipA;     // 0x25E58
    EquipParamEntry_unk mEquipB;     // 0x25F64
    int m26070;
    int mTopCursor;                  // 0x26074
    int m26078;
    int m2607C;
    int m26080;                      // inferred: index in mEffect (-1: none)
    int mSortOrder;                  // 0x26084
    int mTab;                        // 0x26088
    int mCursors[3];                 // 0x2608C
    int mTops[3];                    // 0x26098
    std::vector<ShopUpGradeEntry_unk> mItems; // 0x260A4
    unsigned int mReloadNum;              // 0x260B0
    ShopUpGradeReload_unk mReload[7];     // 0x260B4
    int m26108;
    int m2610C;
    int m26110;
    int m26114;

    static ShopUpGrade* sInstance;
};

#endif
