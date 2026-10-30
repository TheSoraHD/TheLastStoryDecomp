#ifndef GAME_SHOPCOLOR_H
#define GAME_SHOPCOLOR_H

#include "game/ShopBase.h"

// inferred: one colour of the shop (0x24 bytes, read from the shop table); names invented
struct ShopColorEntry_unk {
    int mId;       // inferred: item id of the colour
    int mMat[3];   // 0x04: inferred: needed materials
    int mNum[3];   // 0x10: inferred: needed numbers
    int mOwned;    // 0x1C
    int mShow;     // 0x20
};


// inferred from __vt__9ShopColor (0x807968E8): colour shop (singleton, 0x8087F9D0)
class ShopColor : public ShopBase {
public:
    static ShopColor* fn_80586DB0(atn::Task* parent, int a, int b); // inferred: creates the instance
    ShopColor(atn::Task* parent, int a, int b);                     // 0x80586E3C
    virtual ~ShopColor();                                           // 0x80586FCC
    virtual int vf0C();                                             // 0x8058706C
    virtual void vf10();                                            // 0x8058724C
    virtual void vf14();                                            // 0x805873B8
    virtual void vf1C(int state);                                   // 0x80587B70
    virtual void vf20(int result);                                  // 0x80587C58
    virtual void vf38(const char* data, int size);                  // 0x80587624
    virtual int vf3C();                                             // 0x8058867C
    void fn_80587D10(); // inferred: input of the top menu
    void fn_80587E3C(); // inferred: input of the list
    int fn_80588004(ShopColorEntry_unk* entry); // inferred: buys the colour, returns an error code
    void fn_805880F0(); // inferred: draws the list
    void fn_805883D8(ShopColorEntry_unk* entry); // inferred: draws the needed materials

    ShopD2A_unk* mInfo;        // 0x25B68: inferred: camp_info
    ShopD2A_unk* mTop;         // 0x25B6C
    ShopD2A_unk* mContent;     // 0x25B70
    ShopD2A_unk* mListBase;    // 0x25B74
    ShopLayout_unk* mList[10]; // 0x25B78
    ShopLayout_unk* mNeed[3];  // 0x25BA0
    int m25BAC;
    int mTopCursor;            // 0x25BB0
    int mCursor;               // 0x25BB4
    int mScrollTop;            // 0x25BB8
    std::vector<ShopColorEntry_unk> mColors; // 0x25BBC

    static ShopColor* sInstance;
};

#endif
