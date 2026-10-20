#ifndef GAME_SHOPBUYSELL_H
#define GAME_SHOPBUYSELL_H

#include "game/ShopBase.h"

// inferred from __vt__11ShopBuySell (0x80796864): buy/sell shop (singleton, 0x8087F9C8)
class ShopBuySell : public ShopBase {
public:
    static ShopBuySell* fn_80584FE4(atn::Task* parent, int kind, int a, int b); // inferred: creates the instance
    ShopBuySell(atn::Task* parent, int kind, int a, int b);                     // 0x80585080
    virtual ~ShopBuySell();                                                     // 0x80585308
    virtual int vf0C();                                                         // 0x80585378
    virtual void vf10();                                                        // 0x805855C8
    virtual void vf14();                                                        // 0x80585690
    virtual void vf1C(int state);                                               // 0x80585990
    virtual void vf20(int result);                                              // 0x80585C6C
    virtual int vf3C();                                                         // 0x80586DA8
    void fn_80585D08(); // inferred: input of the top menu
    void fn_80585E80(); // inferred: input of the list
    void fn_80585FD4(); // inferred: input of the number window
    void fn_805860F0(); // inferred: draws the confirmation states
    void fn_8058621C(); // inferred: draws the list
    void fn_80586620(int item); // inferred: draws the status of the members
    void fn_805866F4(int item, int member, int slot); // inferred: draws the status of one member

    ShopD2A_unk* mTop;           // 0x25B68
    ShopD2A_unk* mStatusBase;    // 0x25B6C
    ShopD2A_unk* mStatus[7];     // 0x25B70
    ShopLayout_unk* mUp[7][3];   // 0x25B8C
    ShopLayout_unk* mDown[7][3]; // 0x25BE0
    ShopLayout_unk* mEven[7][3]; // 0x25C34
    int m25C88;
    int mTopCursor;              // 0x25C8C
    int mCursor;                 // 0x25C90
    int mScrollTop;              // 0x25C94

    static ShopBuySell* sInstance;
};

#endif
