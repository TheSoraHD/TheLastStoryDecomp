#ifndef GAME_MARKETTASK_H
#define GAME_MARKETTASK_H

#include "atn/Task.h"
#include "atn/FixedArray.h"
#include "atn/SoundRef.h"
#include "game/ResHandle.h"

// inferred: one item of the market (0x10 bytes); names invented
struct MarketItem_unk {
    MarketItem_unk() : mId(0), mFlag(0), mFlag2(0), mFlag3(0) {}
    MarketItem_unk(int id, int flag, int flag2, int flag3) : mId(id), mFlag(flag), mFlag2(flag2), mFlag3(flag3) {}
    int mId;    // inferred: item id
    int mFlag;  // inferred: event flags of the item (251.., 350.., 470..)
    int mFlag2;
    int mFlag3;
};

// inferred from __vt__10MarketTask (0x80790778): prices of the market change over time (singleton,
// 0x8087F578)
class MarketTask : public atn::Task {
public:
    static MarketTask* fn_804A1DDC(atn::Task* parent); // inferred: creates the instance

    MarketTask(atn::Task* parent); // 0x804A1E44
    virtual ~MarketTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    void fn_804A233C(); // inferred: updates the prices
    int fn_804A24C4(int id);          // inferred: the market sells the item `id`
    int fn_804A251C(int id);          // inferred: current price of the item `id`
    int fn_804A26DC();                // inferred: event flag 251 modulo 11
    int fn_804A2724(int id);          // inferred: average of the bought prices (flag2 / flag3)
    void fn_804A2800(int id, int num); // inferred: records a purchase of `num` items
    void fn_804A28F8(int id);         // inferred: clears the purchase record

    // inferred: entry of the item `id` (0x26E-0x270 share the entry of 0x26D)
    MarketItem_unk* Find_unk(int id) {
        if ((unsigned int)(id - 0x26E) <= 2) {
            id = 0x26D;
        }
        MarketItem_unk* item = &m58[0];
        for (unsigned int i = 0; i < m58.size(); i++, item++) {
            if (id == item->mId) {
                return item;
            }
        }
        return 0;
    }

    int m48;
    int m4C;   // inferred: interval
    float m50; // inferred: power
    int m54;   // inferred: message id
    atn::FixedArray_unk<MarketItem_unk, 30> m58;
    atn::SoundRef m23C;
    int m240; // inferred: frames left of the sound
    ResHandle m244;

    static MarketTask* sInstance;
};

#endif
