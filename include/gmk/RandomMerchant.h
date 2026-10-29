#ifndef GMK_RANDOMMERCHANT_H
#define GMK_RANDOMMERCHANT_H

#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "atn/SoundRef.h"
#include "game/CharaTask.h"
#include "game/GimmickCommon.h"
#include "gmk/SeTable.h"
#include "std/string.h"

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: character of gmk::RandomMerchant. The name is in the DOL (__vt__19RandomMerchantChara,
// 0x1428 bytes, character type 1); all its members are inline (weak copies at the end of the file).
class RandomMerchantChara : public CharaTask {
public:
    RandomMerchantChara(atn::Task* parent, const std::string& path) : CharaTask(parent, 1, 0) {
        fn_80136544(path.data());
        m1188.fn_8011D424(&mMotion);
        m137C |= 1;
    }
    virtual ~RandomMerchantChara() {}
    virtual int vf0C() { return fn_8013655C() == 0; }
    virtual void vf10() { fn_80145334(); }
    virtual void vf14() { fn_80149A30(); }

    int m1424; // inferred: unused here (sizeof(RandomMerchantChara) == 0x1428)
};

namespace gmk {

// inferred: fixed string of 0x80 chars with an empty destructor (its weak copy 0x80069758 is the
// destructor passed to __construct_array); names invented
struct MerchantString_unk {
    MerchantString_unk() { mStr[0] = 0; }
    ~MerchantString_unk() {}
    MerchantString_unk& operator=(const char* str) {
        if (str != mStr) {
            memcpy(mStr, str, strlen(str) + 1);
        }
        return *this;
    }
    char mStr[0x80];
};

// inferred: one merchant of the "CHARA_SET" lines (0x29C bytes); names invented
struct MerchantEntry_unk {
    MerchantEntry_unk() : mMotionBase(-1), mItem(0), mChara(0), mItemGimmick(0), m290(0), mEvent1(-1), mEvent2(-1) {}

    MerchantString_unk mName;          // 0x000: character file
    MerchantString_unk m080;
    MerchantString_unk mMotions[3];    // 0x100: motion files (wait, accept, refuse)
    int mMotionBase;                   // 0x280
    int mItem;                         // 0x284: item id (kind 10 makes a gimmick)
    RandomMerchantChara* mChara;       // 0x288
    GimmickCommon* mItemGimmick;       // 0x28C
    int m290;                          // 0x290: message id
    int mEvent1;                       // 0x294: inferred: event started when the item is bought
    int mEvent2;                       // 0x298: inferred: event started when it is refused
};

// inferred: appearance place of the merchant ("APPEAR_POS"); names invented
struct MerchantPos_unk {
    atn::Vector3 mPos;
    atn::Vector3 mRot;
};

// inferred: merchant that appears at a random place ("APPEAR_POS") with a random character
// ("CHARA_SET") and sells an item. The name is in the DOL (__vt__Q23gmk14RandomMerchant,
// 0x30A8 bytes).
class RandomMerchant : public GimmickCommon {
public:
    RandomMerchant(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_804310B0(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~RandomMerchant();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38() { return &mModel; }
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag);
    virtual void vf4C(CharaTask* chara);
    virtual int vf60();
    virtual int vf64(int arg);
    virtual void vf70(int state);

    void PlayMotion_unk(int motion);
    void PlayMotion2_unk(RandomMerchantChara* chara, int index);
    void PlayMotion3_unk(atn::Model* motion, int index);

    atn::Model mModel;                    // 0xF4
    atn::EfpFile mEfp;             // 0x4C4
    int m4D0[3];
    int mState;                          // 0x4DC: 1 loaded, 2 shown, 3 bought, 4 refused
    atn::Vector3 mDir;                   // 0x4E0
    int mLoaded;                         // 0x4EC
    int mCur;                            // 0x4F0: current merchant
    int mCount;                          // 0x4F4
    MerchantEntry_unk mEntries[16];      // 0x4F8
    int mItemId;                         // 0x2EB8
    int mItemNum;                        // 0x2EBC
    int mPrice;                          // 0x2EC0
    MerchantPos_unk mPositions[16];      // 0x2EC4
    int mPosCount;                       // 0x3044
    int mTimer;                          // 0x3048
    int mBlank;                          // 0x304C: "APPEAR_BRANK" (frames)
    int mFirstTime;                      // 0x3050: "FIRST_APPEAR_TIME" (frames)
    int mLimit;                          // 0x3054
    int mTimeLimit;                      // 0x3058: "TIME_LIMIT" (frames)
    float mLength2;                      // 0x305C: "APPEAR_LENGTH" (second value)
    float mLength1;                      // 0x3060: "APPEAR_LENGTH" (first value)
    int mBought;                         // 0x3064
    int m3068;                           // 0x3068: message window data (fn_803E3BE8)
    int m306C;
    wchar_t* m3070;
    int m3074;
    atn::FixedString_unk<0x20> m3078;
    int m3098;
    int m309C;
    atn::SoundRef mSound;                // 0x30A0
    int m30A4; // inferred: unused (the object is 0x30A8 bytes)
};

} // namespace gmk

#endif
