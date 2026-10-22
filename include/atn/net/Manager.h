#ifndef ATN_NET_MANAGER_H
#define ATN_NET_MANAGER_H

// inferred: network manager of the Wi-Fi Connection (atn::net::Manager, vtable 0x807930F0, source
// atn/NetManager.cpp). MP::ManagerMP derives from it; the instance is lbl_8087F628. Member names are
// invented; offsets checked against 0x80508B74 (constructor) and the methods of atn/NetManager.cpp.

#include "std/string.h"
#include "std/new.h"
#include "atn/FixedArray.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

struct MemoryManager_unk;
extern "C" MemoryManager_unk* fn_800827E0();
extern "C" void fn_80083AD4(MemoryManager_unk* mgr, void* p);
extern "C" unsigned char fn_806B0E30(); // inferred: own aid (DWC)
extern "C" int fn_806A8E40();           // inferred: own aid (local play)
extern "C" int fn_806B1250();           // inferred: aid of the server (DWC)

// inferred: OSThread / OSMutex / OSThreadQueue of the SDK (only the sizes matter here)
struct NetOSThread_unk {
    unsigned char m00[0x318];
};
struct NetOSMutex_unk {
    unsigned char m00[0x18];
};
struct NetOSThreadQueue_unk {
    void* mHead;
    void* mTail;
};

// inferred: one counter of NetStats_unk (atn/NetFriend_unk.cpp)
struct NetCounter_unk {
    int mCount;
    int mMax;
    int mLast;
    int m0C;
};

// inferred: statistics of the network manager (defined in atn/NetFriend_unk.cpp)
struct NetStats_unk {
    NetStats_unk();
    void fn_8050E6CC(int ms);
    void fn_8050E6F4();
    float fn_8050E7DC(float value, int unit);
    void AddSend_unk(int size) {
        m10.mCount += size;
        m20.mCount++;
    }
    void AddRecv_unk(int size) {
        m30.mCount += size;
        m40.mCount++;
    }

    unsigned int mPeriod;
    unsigned int m04;
    long long mLastTime;
    NetCounter_unk m10;
    NetCounter_unk m20;
    NetCounter_unk m30;
    NetCounter_unk m40;
};

// inferred: friend list of the Wi-Fi Connection (defined in atn/NetFriend_unk.cpp, 0xA08 bytes)
struct MpFriendList_unk {
    MpFriendList_unk();

    unsigned char mUserData[0x40];     // 0x000: DWC user data
    unsigned char mFriends[32][0xC];   // 0x040
    wchar_t mName[16];                 // 0x1C0
    unsigned char m1E0[0x7F0 - 0x1E0];
    unsigned char m7F0[0x9F8 - 0x7F0]; // 0x7F0: inferred: data saved by the game
    unsigned long long mFriendKey;     // 0x9F8
    int mA00;
    int mA04;
};

namespace atn {
namespace net {

class Manager;

// inferred: callbacks given by the game to the manager (copied as a block by 0x80508D9C)
struct NetCallbacks_unk {
    NetCallbacks_unk() : m00(0), m04(0), mRecv(0), mSaveData(0), m10(0), m14(0), mLoadFriends(0),
        mSaveFriends(0), m20(0), m24(0) {}

    int m00;
    int m04;
    void (*mRecv)(unsigned char aid, void* data, unsigned int size); // 0x08
    int (*mSaveData)(void* data, int arg);                           // 0x0C
    int (*m10)(int arg0, int arg1);                                  // 0x10
    int (*m14)(int arg0, int arg1);                                  // 0x14
    void (*mLoadFriends)(void* data, unsigned int size);             // 0x18
    void (*mSaveFriends)(void* data, unsigned int size);             // 0x1C
    int (*m20)();                                                    // 0x20
    int (*m24)();                                                    // 0x24
};

// inferred: one key of the matchmaking filter ("key = value", 0x24 bytes)
struct NetFilterKey_unk {
    NetFilterKey_unk() : mType(0), mFlags(0) {}

    std::string mKey;     // 0x00
    unsigned int mType;   // 0x0C: 1 = string, 2 = int
    std::string mString;  // 0x10
    int mInt;             // 0x1C
    unsigned int mFlags;  // 0x20: bit 0: not part of the filter string
};

// inferred: information about one player of the session (0x34 bytes, sent as message 6)
struct NetPlayerInfo_unk {
    NetPlayerInfo_unk() : mAid(0xFF), m04(0), mPing(0), mFlagsWord(0), m30(0) { wcscpy(mName, L"<none>"); }

    unsigned char mAid; // 0x00
    int m04;            // 0x04
    int mPing;          // 0x08
    union {
        unsigned int mFlagsWord;
        struct {
            unsigned int mConnected : 1; // inferred
            unsigned int mServer : 1;    // inferred
            unsigned int mSelf : 1;      // inferred
            unsigned int mB3 : 1;
            unsigned int mB4 : 1;
            unsigned int mB5_31 : 27;
        } mFlags;
    };
    wchar_t mName[16]; // 0x10
    int m30;           // 0x30
};

// inferred: error code queue (0xBE80): the oldest entry is dropped when it is full
struct NetError_unk {
    NetError_unk() : mType(0) {}
    NetError_unk(int type, int code) : mType(type), mCode(code) {}
    int mType;
    int mCode;
};

struct NetErrorQueue_unk {
    NetErrorQueue_unk() : mCount(0) {}
    void Push(int type, int code) {
        if (mCount >= 8) {
            for (unsigned int i = 0; i < mCount - 1; i++) {
                mData[i] = mData[i + 1];
            }
            mCount = mCount - 1;
        }
        new (&mData[mCount]) NetError_unk(type, code);
        mCount++;
    }
    unsigned int size() const { return mCount; }
    NetError_unk& back() { return mData[mCount - 1]; }
    void clear() { mCount = 0; }

    unsigned int mCount;
    NetError_unk mData[8];
};

// inferred: worker thread of the manager (0xBED0): commands sent with Request
class NetThread_unk {
public:
    NetThread_unk() : mRunning(1), m330(0) { memset(&mThread, 0, sizeof(mThread)); }
    ~NetThread_unk() {
        if (mStack != 0) {
            fn_80083AD4(fn_800827E0(), mStack);
            mStack = 0;
        }
    }

    int fn_8050CDD0(void* (*func)(void*), int priority); // inferred: creates the thread
    int fn_8050CE9C();                                   // inferred: stops the thread
    int fn_8050CF14();                                   // inferred: thread terminated
    int Request(int cmd, void* arg);                     // 0x8050CF1C
    void fn_8050CF90();

    void* mStack;                // 0x000
    int m004;
    NetOSThread_unk mThread;     // 0x008
    void* mArg;                  // 0x320
    int mRunning;                // 0x324: inferred: the thread is waiting for a command
    int mCmd;                    // 0x328
    int mResult;                 // 0x32C
    int m330;                    // 0x330
    NetOSThreadQueue_unk mQueue; // 0x334
};

// inferred: buffer of a reliable packet (0x404 bytes)
struct NetSendBuffer_unk {
    int m000;
    unsigned char mData[0x400];
};

class Manager {
public:
    Manager();
    virtual ~Manager();
    virtual int vf0C(); // inferred: shuts the library down
    // inferred: player information (weak copies 0x8050D3C4 and 0x804C134C)
    virtual NetPlayerInfo_unk* vf10(unsigned int i) { return i >= mPlayers.size() ? 0 : &mPlayers[i]; }
    virtual NetPlayerInfo_unk* vf14(unsigned int i) { return i >= mPlayers.size() ? 0 : &mPlayers[i]; }

    int fn_80508D9C(NetCallbacks_unk* callbacks); // inferred: initializes the library
    int fn_80508FAC();                             // inferred: initializes the sockets
    int fn_805092FC();                             // inferred: update
    int fn_80509A98();
    int fn_80509AF8();
    int fn_80509B20(int arg0, int arg1, int arg2);
    int fn_80509B4C(int arg0, int arg1, int arg2);
    int fn_80509C88();
    int fn_80509CAC();
    int fn_80509DA0();
    void fn_80509F08(); // inferred: clears the last error
    int fn_80509F48();  // inferred: logs in
    int fn_8050A100();  // inferred: starts the matchmaking
    void Stripped1_unk();
    int fn_8050A3D0(int send);
    int fn_8050A4E0(int send);
    void fn_8050A5EC();
    int fn_8050A634(int open);
    void fn_8050AB58(int error, int arg);
    inline int Send_unk(const void* data, unsigned int size, unsigned char aid, int reliable); // 0x8050B7EC
    const char* fn_8050B968(int state); // inferred: name of a state
    int fn_8050BAC4();                  // inferred: updates the player flags
    int fn_8050BDC4();                  // inferred: sends the own player information
    int fn_8050C0B4();
    int fn_8050C128(int* code); // inferred: pops the last error
    int fn_8050C168();          // inferred: reads the last DWC error

    void Cleanup_unk();
    int Match_unk();
    int IsJoined_unk();
    void SetB13_unk(int flag);
    // inferred: queues an error code (the oldest one is dropped when the queue is full)
    void PushError_unk(int type, int code) {
        if (mErrors.mCount >= 8) {
            for (unsigned int i = 0; i < mErrors.mCount - 1; i++) {
                mErrors.mData[i] = mErrors.mData[i + 1];
            }
            mErrors.mCount = mErrors.mCount - 1;
        }
        new (&mErrors.mData[mErrors.mCount]) NetError_unk(type, code);
        mErrors.mCount++;
    }
    void SetRecvBuffers_unk(); // 0x8050B670

    // inferred: own aid
    unsigned char GetMyAid_unk() {
        if (mC215 == 0) {
            return mFlags.mB12 ? 0 : fn_806B0E30();
        } else {
            return mFlags.mB12 ? 0 : fn_806A8E40();
        }
    }
    // inferred: aid of the server
    unsigned char GetServerAid_unk() {
        if (mC215 == 0) {
            return mFlags.mB12 ? 0 : (unsigned char)fn_806B1250();
        } else {
            return 0;
        }
    }

    int m04;                         // 0x004
    int m08;                         // 0x008
    int m0C;                         // 0x00C
    char m10[0x80];                  // 0x010: inferred: matchmaking filter
    unsigned char m90;               // 0x090
    unsigned char m91[0x98 - 0x91];
    NetCallbacks_unk mCallbacks;     // 0x098
    unsigned char mC0;               // 0x0C0
    char mC1[0xC8 - 0xC1];           // 0x0C1
    int mState;                      // 0x0C8
    unsigned int mFrame;             // 0x0CC
    int mD0;                         // 0x0D0
    int mD4;                         // 0x0D4
    NetFilterKey_unk mKeys[8];       // 0x0D8
    union {
        unsigned int mFlagsWord;     // 0x1F8
        struct {
            unsigned int mB0 : 1;
            unsigned int mB1 : 1;
            unsigned int mB2 : 1;
            unsigned int mB3 : 1;
            unsigned int mB4 : 1;
            unsigned int mB5 : 1;
            unsigned int mB6 : 1;
            unsigned int mB7 : 1;
            unsigned int mB8 : 1;
            unsigned int mB9 : 1;
            unsigned int mB10 : 1;
            unsigned int mB11 : 1;
            unsigned int mB12 : 1; // inferred: network disabled
            unsigned int mB13 : 1;
            unsigned int mB14 : 1;
            unsigned int mB15 : 1;
            unsigned int mB16_31 : 16;
        } mFlags;
    };
    int m1FC;
    NetStats_unk mStats;             // 0x200
    int m250;                        // 0x250
    unsigned int m254;               // 0x254
    unsigned int m258;               // 0x258
    int m25C;                        // 0x25C
    int m260;                        // 0x260
    int m264;                        // 0x264
    int m268;                        // 0x268
    int m26C;                        // 0x26C
    FixedArray_unk<NetPlayerInfo_unk, 8> mPlayers; // 0x270
    NetOSMutex_unk mMutex;           // 0x414
    int m42C;
    MpFriendList_unk mFriendList;    // 0x430
    int mE38;                        // 0xE38
    NetSendBuffer_unk mSendBuffers[12]; // 0xE3C
    int mSendIndex;                  // 0x3E6C
    unsigned char m3E70[0x3E80 - 0x3E70];
    unsigned char mRecvBuffers[8][0x1000]; // 0x3E80
    NetErrorQueue_unk mErrors;       // 0xBE80
    int mBEC4;                       // 0xBEC4
    int mBEC8;                       // 0xBEC8
    int mBECC;                       // 0xBECC
    NetThread_unk mThread;           // 0xBED0
    int mC20C;
    int mC210;                       // 0xC210
    unsigned char mC214;             // 0xC214
    unsigned char mC215;             // 0xC215: inferred: local play instead of DWC
    int mC218;                       // 0xC218
};

} // namespace net
} // namespace atn

extern "C" atn::net::Manager* lbl_8087F628;

#endif
