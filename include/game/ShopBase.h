#ifndef GAME_SHOPBASE_H
#define GAME_SHOPBASE_H

#include "atn/Task.h"
#include "atn/LH_Binary.h"
#include "std/vector.h"
#include "atn/ResRef_unk.h"
#include "game/ResHandle.h"
#include <string.h>

// inferred: d2anime task (made by fn_801F3FF8); only the members used by the shops
struct ShopD2A_unk : public atn::Task {
    // inferred: as MenuD2A_unk::IsEnd_unk (name invented)
    bool IsEnd_unk() {
        float last = mLastFrame;
        return mFrame >= last;
    }
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0xA0 - 0x58]; // 0x58: inferred: layout
    float mLastFrame;                   // 0xA0
    unsigned char mA4[0xFC - 0xA4];
    unsigned int mLoop : 1;             // 0xFC (as atn::D2A_Task)
    unsigned int mVisible : 1;
    unsigned int mPlaying : 1;
    unsigned int mRepeat : 1;
    unsigned int mFC_rest : 28;
    float mFrame; // 0x100
    float mSpeed; // 0x104
};
// inferred: layout anime task (made by fn_801F64D0); only the members used by the shops
struct ShopLayout_unk : public atn::Task {
    unsigned char m48[0x4D - 0x48];
    bool mRepeat; // 0x4D
    unsigned char m4E[0x50 - 0x4E];
    float mFrame; // 0x50
    float mSpeed; // 0x54
};

// inferred: entry of the first list of the shop (0x64 bytes, 128 entries; cleared by the constructor
// 0x8057F280); names invented
struct ShopEntry64_unk {
    ShopEntry64_unk() : m00(0), m04(0) {
        memset(m08, 0, sizeof(m08));
        m48 = 1.0f;
        m4C = 0;
        m50 = 100;
        m54 = 1;
        m5C = 0;
        m60 = 1;
    }
    int m00;           // inferred: item id
    int m04;           // inferred: price
    wchar_t m08[0x20]; // inferred: name override
    float m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    int m5C;
    int m60;
};
// inferred: entry of the second list of the shop (0x5C bytes, 1536 entries); names invented
struct ShopEntry5C_unk {
    ShopEntry5C_unk() : m00(0), m04(0) { memset(m08, 0, sizeof(m08)); }
    int m00; // inferred: item id
    int m04;
    char m08[0x40];
    int m48;
    int m4C;
    int m50;
    int m54;
    int m58;
};

// inferred: record of a purchase (fn_805849E8 pushes {id, m5C} of the entry); names invented
struct ShopBought_unk {
    int mId;
    int m04;
};

// inferred: list cursor of the shop (0x25B04); names invented
struct ShopScroll_unk {
    ShopScroll_unk() : mCursor(0), mCount(0), mTop(0), mPage(0) {}
    int mCursor; // 0x00
    int mCount;  // 0x04
    int mTop;    // 0x08
    int mPage;   // 0x0C
};

// inferred from __vt__8ShopBase (0x807967A8) and its constructor (0x8057F280): base of the shop menus.
// Only the members used by the derived shops are named.
class ShopBase : public atn::Task {
public:
    ShopBase(atn::Task* parent, int kind, int a, int b); // 0x8057F280
    virtual ~ShopBase();                                // 0x8057F7A8
    virtual void vf1C(int state) = 0;  // inferred: changes the state
    virtual void vf20(int result) = 0; // inferred: result of the confirmation window
    virtual void vf24(int message);    // 0x8057FDE0: inferred: end of a message window
    virtual void vf28();               // 0x8057FB68
    virtual void vf2C();               // 0x8057FC24
    virtual void vf30();               // 0x8057FCFC
    virtual void vf34();               // 0x8057FDE4
    virtual void vf38(const char* data, int size); // 0x80581A54: inferred: reads the shop table
    virtual int vf3C();                // 0x80580578
    virtual void vf40();               // 0x80584FD0

    int fn_8057F880();  // inferred: nonzero while loading
    void fn_8057F8F8();
    void fn_8057FB20();
    void fn_8058036C();
    void fn_80581674();
    void fn_8057FF38();
    void fn_805800FC();
    void fn_80580264();
    void fn_80580580();
    void fn_80580690();
    void fn_805807A4();
    void fn_80580B50();
    void fn_80580DE0();
    void fn_80581008(int id);
    void fn_805811DC(int id);
    void fn_80581570(int id, int arg);
    void fn_80581660(int index);
    void fn_8058181C(int a, int error, int b); // inferred: shows a message
    void fn_80581964(int a, int message, int b); // inferred: opens a confirmation window
    void fn_80581FD8();
    void fn_805846C0(ShopD2A_unk* anime, ShopLayout_unk* layout, int cursor, int top, int arg);
    void fn_80584750(ShopD2A_unk* anime, int top, int count, int page);
    void fn_805847EC(int arg);
    void fn_80584808(int arg);
    int fn_8058482C(ShopEntry64_unk* entry); // inferred: checks the entry, returns an error code
    int fn_805848E0(ShopEntry64_unk* entry);
    void fn_805849E8(ShopEntry64_unk* entry, int num);
    void fn_80584D78(ShopEntry5C_unk* entry, int arg);
    int fn_80584DD8(void* item, int num, float a, float b, float c); // inferred: sell price
    int fn_80584F44(void* item, float rate); // inferred: price of an item

    atn::Task* mParent;     // 0x48
    int mKind;              // 0x4C
    int m50;
    int m54;
    atn::LH_Binary mFile;   // 0x58
    int mTitle;             // 0x60: inferred: "shopTitle"
    int mName;              // 0x64: inferred: "shopName"
    ShopLayout_unk* m68;
    int m6C;
    ShopD2A_unk* m70;
    ShopD2A_unk* m74;
    int m78;
    int m7C;
    ShopD2A_unk* m80;
    int m84;
    int m88;
    int m8C;
    ShopD2A_unk* m90;
    ShopD2A_unk* m94;
    ShopLayout_unk* m98[2];
    int mA0;
    int mA4;
    ShopD2A_unk* mA8;
    ShopLayout_unk* mAC[10];
    ShopD2A_unk* mD4;
    ShopD2A_unk* mD8;
    int mDC;    // inferred: selected entry of m3300
    int mE0;
    int mState; // 0xE4
    int mPrevState; // 0xE8
    int mEC;
    int mF0;
    int mF4;
    int mF8;
    ShopEntry64_unk mEntries64[128]; // 0xFC
    int m32FC;
    ShopEntry5C_unk mEntries5C[1536]; // 0x3300
    int m25B00;
    ShopScroll_unk mScroll; // 0x25B04
    int m25B14;
    int m25B18;
    int m25B1C;
    int m25B20;
    int m25B24;
    int m25B28;
    ShopD2A_unk* m25B2C;
    std::vector<ShopBought_unk> m25B30; // inferred: items bought (fn_805849E8)
    int m25B3C;
    int m25B40;
    ResRef_unk m25B44; // inferred: "/preload/shop.pk"
    ResHandle m25B48;  // inferred: debug menu group "ShopParam"
    int m25B4C;
    int m25B50;
    int m25B54;
    int m25B58;
    int m25B5C;
    int m25B60;
    int m25B64;
};

// inferred: members of ShopBase (0x8057F280-0x80584FE4) still called by their automatic names
extern "C" void fn_80580D80(ShopD2A_unk* anime);
extern "C" void fn_80580DB0(atn::Task* anime);

#endif
