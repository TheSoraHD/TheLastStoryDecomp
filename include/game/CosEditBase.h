#ifndef GAME_COSEDITBASE_H
#define GAME_COSEDITBASE_H

#include "game/NewCampPartsBase.h"
#include "atn/Camera_unk.h"
#include "game/ResHandle.h"
#include "std/new.h"

namespace atn {
class D3AnimeInstance;
}
namespace chr {
struct CharaEquip_unk;
}
class CharaTask;
class CostumeEditChara;

// inferred: party member shown by the costume menu (entry of the parent's list); names invented
struct CosEditMember_unk {
    int mId;          // 0x00: inferred: compared with CostumeEditChara::mFace
    CharaTask* mChara; // 0x04
};

// inferred: list of one page of the menu (equipment, color or parts): 13 rows, a base and a scroll bar
struct CosEditList_unk {
    CosEditList_unk() : mBase(0), mScrollBar(0), m3C(-1), m40(-1), mRows(13) {}
    atn::D3AnimeInstance* mItems[13]; // 0x00
    atn::D3Anime* mBase;              // 0x34
    atn::D3AnimeInstance* mScrollBar; // 0x38
    int m3C;                          // inferred: selected row
    int m40;
    int mRows;                        // 0x44
};

// inferred: equipment being loaded (type 0: CharaEquip_unk of a character, type 1: the character)
struct CosEditLoad_unk {
    int mType;
    chr::CharaEquip_unk* mEquip;
    CharaTask* mChara;
};

// inferred: item given to fn_801CF3C0 (id at 0x04, inventory index at 0x08, negative if none)
struct CosEditItem_unk {
    int m00;
    int mId;
    int mIndex;
};

// inferred from __vt__11CosEditBase (0x80782608): base of the costume edit menus (equipment
// change); derived by CosEditProtector. Member names invented.
class CosEditBase : public NewCampPartsBase {
public:
    CosEditBase(atn::Task* parent);           // 0x801CCE50
    virtual ~CosEditBase();                   // 0x801CD184
    virtual void vf10();                      // 0x801CD210
    virtual void vf14();                      // 0x801CDD2C
    virtual void vf20();                      // 0x801CD5A4
    virtual void vf2C() {}                    // 0x801CFC54
    virtual void vf34(int state) { m7C = state; } // 0x801CFC4C
    virtual void vf40(void* group);           // 0x801CFACC
    virtual void vf64();                      // 0x801CDD98
    virtual void vf68();                      // 0x801CD644
    virtual void vf6C();                      // 0x801CE144
    virtual void vf70() {}                    // 0x801CFC48
    virtual void vf74(int a, int b);          // 0x801CE634

    void fn_801CD648(); // inferred: rotation and visibility of the characters
    void fn_801CDA00(CosEditList_unk* list); // inferred: places a list
    CosEditMember_unk* fn_801CDD9C(int step); // inferred: member next to the selected one
    void fn_801CDE14(CosEditMember_unk* member, int dir); // inferred: scrolls to `member`
    void fn_801CE148(CosEditMember_unk* member); // inferred: creates the characters around `member`
    int fn_801CE4CC();  // inferred: the main character is loading its equipment
    void fn_801CE518(); // inferred: removes the characters
    void fn_801CE5B0(int b, int c);
    void* fn_801CE648(int slot);
    int fn_801CE7E8(int slot);
    void* fn_801CE87C(int slot);
    void fn_801CEA50(int member, CharaTask* chara, int slot, int a, int b, int c);
    void fn_801CEA98(int member, CharaTask* chara, int slot, int item, int a, int b, int c, int d);
    CostumeEditChara* fn_801CF334(); // inferred: character of the selected member
    // inferred: appends to mLoads (placement copy, as atn::FixedArray_unk::push_back)
    void AddLoad_unk(const CosEditLoad_unk& load) {
        new (&mLoads[mLoadCount]) CosEditLoad_unk(load);
        mLoadCount++;
    }
    int fn_801CF3C0(int member, CharaTask* chara, CosEditItem_unk* item, int slot, int add, int remove);
    void* fn_801CFA5C(int member, int slot);
    void* fn_801CFAA8(int member, int slot);

    CostumeEditChara* mChara[3];  // 0xD4: inferred: selected, next and previous members
    float mE0;                    // inferred: target rotation
    float mE4;                    // inferred: rotation
    atn::Task* mE8;
    atn::Task* mEC;
    int mF0;                      // inferred: the characters must be placed again
    int mF4;                      // inferred: scroll direction
    float mF8;
    int mFC;
    int m100;                     // inferred: selected member
    CosEditMember_unk* m104;      // inferred: member being shown
    CosEditList_unk mLists[3];    // 0x108
    int m1E0;                     // inferred: list of the members (parent 0x1224 or 0x1268)
    int m1E4;
    int m1E8;
    int m1EC;
    int m1F0;
    int m1F4;
    int m1F8;
    int m1FC;
    int m200;
    int m204;
    unsigned char m208[0x210 - 0x208];
    int m210;
    int m214;
    int m218;
    short m21C;
    Camera_unk m220;
    float m414;                   // inferred: aspect ratio
    float m418;                   // inferred: fovy
    int m41C;
    unsigned char m420[0x424 - 0x420];
    float m424;
    unsigned char m428[0x430 - 0x428];
    float m430[12];
    unsigned char m460[0x46C - 0x460];
    atn::Vector3 m46C;
    atn::Vector3 m478;            // inferred: offset of the characters
    unsigned int mLoadCount;      // 0x484: inferred: equipment being loaded
    CosEditLoad_unk mLoads[128];  // 0x488
    int mA88;
    int mA8C;                     // inferred: sound to play when the load ends
    int mA90;
    int mA94;
    int mA98;
    float mA9C;
    ResHandle mAA0;               // 0xAA0: inferred: debug menu group (vf40)
    float mAA4;
    atn::Vector3 mCharaUpperOfs;  // 0xAA8
    atn::Vector3 mCharaLowerOfs;  // 0xAB4
    atn::Vector3 mCharaZoomOfs;   // 0xAC0
};

#endif
