#ifndef GAME_SHOPIDENTIFY_H
#define GAME_SHOPIDENTIFY_H

#include "game/ShopBase.h"

// inferred: item waiting to be identified (0x10 bytes); names invented
struct ShopIdentifyEntry_unk {
    int mIndex; // inferred: index in the party list (fn_8044D500)
    int mItem;  // 0x04: inferred: item id
    int mPrice; // 0x08
    int mDone;  // 0x0C: inferred: already identified
};

// inferred from __vt__12ShopIdentify (0x80796994): identification shop (singleton, 0x8087F9D8)
class ShopIdentify : public ShopBase {
public:
    static ShopIdentify* fn_80588684(atn::Task* parent, int a, int b); // inferred: creates the instance
    ShopIdentify(atn::Task* parent, int a, int b);                     // 0x80588710
    virtual ~ShopIdentify();                                           // 0x80588864
    virtual int vf0C();                                                // 0x80588904
    virtual void vf10();                                               // 0x80588B18
    virtual void vf14();                                               // 0x80588BE0
    virtual void vf1C(int state);                                      // 0x80588E24
    virtual void vf20(int result);                                     // 0x80589108
    virtual void vf24(int message);                                    // 0x80589270
    virtual int vf3C();                                                // 0x8058A3A4
    virtual void vf40();                                               // 0x8058A390
    int fn_805892CC(int* cursor, int remove); // inferred: sells the selected item, returns the next state
    void fn_8058944C(); // inferred: input of the top menu
    void fn_805895B4(); // inferred: input of the list
    void fn_805897D4(); // inferred: result window
    void fn_80589958(); // inferred: draws the confirmation states
    void fn_80589AF4(); // inferred: draws the list
    void fn_80589DE0(ShopIdentifyEntry_unk* entry, int help); // inferred: draws the selected item
    void fn_8058A0F0(ShopIdentifyEntry_unk* entry);           // inferred: draws the result window
    void ReadItems_unk(); // inferred: inline (name invented)

    ShopD2A_unk* mTop;         // 0x25B68
    ShopD2A_unk* mContent;     // 0x25B6C
    ShopD2A_unk* mListBase;    // 0x25B70
    ShopLayout_unk* mList[10]; // 0x25B74
    ShopD2A_unk* mResult;      // 0x25B9C
    unsigned char m25BA0[0x25C18 - 0x25BA0];
    int m25C18;
    int mTopCursor;            // 0x25C1C
    int mCursor;               // 0x25C20
    int mScrollTop;            // 0x25C24
    std::vector<ShopIdentifyEntry_unk> mItems; // 0x25C28
    int m25C34;

    static ShopIdentify* sInstance;
};

#endif
