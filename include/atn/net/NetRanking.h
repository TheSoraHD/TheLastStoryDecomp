#ifndef ATN_NET_NETRANKING_H
#define ATN_NET_NETRANKING_H

#include "atn/FixedArray.h"

extern "C" int DWC_RnkShutdown();
extern "C" int DWC_RnkCancelProcess();

namespace atn {
namespace net {

// inferred: one row of a ranking (DWCRnkData, 0x1C bytes)
struct NetRankData_unk {
    unsigned long mOrder;
    unsigned long mPid;
    long mScore;
    unsigned long mRegion;
    unsigned long mLastUpdate;
    unsigned long mSize;
    void* mUserData;
};

// inferred: ranking client of the game (lbl_8087F5A4); names invented
class NetRanking_unk {
public:
    int fn_8050C1D8(void* userData);
    int fn_8050C2F4();
    int fn_8050C7F0();
    int fn_8050C840(unsigned int index);
    int fn_8050C898(unsigned long category, unsigned long limit, unsigned long since,
                    void (*cb)(int, NetRankData_unk*));
    int fn_8050C9D4(unsigned long category, int limit, unsigned long since, void (*cb)(int, NetRankData_unk*));
    int fn_8050CB24(unsigned long category, int limit, unsigned long since, void (*cb)(int, NetRankData_unk*));
    int fn_8050CCD0(unsigned long category, long score, const wchar_t* name, unsigned long size);
    int fn_8050CDA8();

    void Fail_unk() {
        DWC_RnkCancelProcess();
        DWC_RnkShutdown();
        if (mIndex != -1) {
            mTimes[mIndex] = -1;
        }
        mState = 2;
    }

    int mState;                          // 0x000
    int mTimes[4];                       // 0x004
    FixedArray_unk<NetRankData_unk, 65> mRows; // 0x014
    void (*mCallback)(int, NetRankData_unk*);       // 0x734
    int mProfanityResult;                // 0x738
    int mProfanityCount;                 // 0x73C
    unsigned long mCategory;             // 0x740
    long mScore;                         // 0x744
    const wchar_t* mName;                // 0x748
    unsigned long mDataSize;             // 0x74C
    int mIndex;                          // 0x750
};

} // namespace net
} // namespace atn

extern "C" atn::net::NetRanking_unk* lbl_8087F5A4;

#endif
