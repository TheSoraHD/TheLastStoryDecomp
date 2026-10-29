// inferred: network manager of the Wi-Fi Connection (atn::net::Manager), its worker thread and the
// ranking client of the game (DWC_Rnk). File name inferred from the class (see docs/splits-auto-D.md).
#include "atn/net/Manager.h"
#include "atn/MemoryManager.h"
#include "atn/net/PacketPack.h"
#include "MP/CryptMP.h"
#include "atn/FixedArray.h"

extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strcat(char* dst, const char* src);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" void* fn_800838B8(atn::MemoryManager* mgr, unsigned long size, unsigned long align, int heap,
                             const char* file, const char* func, int line);

// OS
extern "C" void OSInitMutex(NetOSMutex_unk* mutex);
extern "C" void OSLockMutex(NetOSMutex_unk* mutex);
extern "C" void OSUnlockMutex(NetOSMutex_unk* mutex);
extern "C" int OSIsThreadSuspended(NetOSThread_unk* thread);
extern "C" int OSIsThreadTerminated(NetOSThread_unk* thread);
extern "C" int OSResumeThread(NetOSThread_unk* thread);
extern "C" void OSWakeupThread(NetOSThreadQueue_unk* queue);
extern "C" void OSSleepThread(NetOSThreadQueue_unk* queue);
extern "C" void OSInitThreadQueue(NetOSThreadQueue_unk* queue);
extern "C" int OSCreateThread(NetOSThread_unk* thread, void* (*func)(void*), void* param, void* stack,
                              unsigned long stackSize, int priority, unsigned short attr);
extern "C" void OSSleepTicks(long long ticks);

#define OS_TIMER_CLOCK_UNK (*(unsigned long*)0x800000F8 / 4)
#define OSMillisecondsToTicks_unk(msec) ((msec) * (OS_TIMER_CLOCK_UNK / 1000))
#define OSMicrosecondsToTicks_unk(usec) (((usec) * (OS_TIMER_CLOCK_UNK / 125000)) / 8)

// SO (sockets)
struct SOLibraryConfig_unk {
    void* (*mAlloc)(int name, long size);
    void (*mFree)(int name, void* p, long size);
};
extern "C" int SOInit(SOLibraryConfig_unk* config);
extern "C" int SOStartup();
extern "C" int SOCleanup();
extern "C" int SOFinish();

// NET (AES)
struct NETAESContext_unk {
    unsigned char m00[0xFC];
};
extern "C" int NETAESCreate(NETAESContext_unk* ctx, const void* key, unsigned int keyLen, const void* iv);
extern "C" int NETAESEncrypt(NETAESContext_unk* ctx, void* dst, const void* src, unsigned int len);
extern "C" int NETAESDecrypt(NETAESContext_unk* ctx, void* dst, const void* src, unsigned int len);
extern "C" int NETAESDelete(NETAESContext_unk* ctx);

// DWC
extern "C" int DWC_Init(int flag, const char* gameName, unsigned long gameCode, void* alloc, void* free);
extern "C" void DWC_SetReportLevel(unsigned long level);
extern "C" void DWC_Shutdown();
extern "C" int DWC_GetLastErrorEx(int* code, unsigned int* type);
extern "C" void DWC_ClearError();
extern "C" int DWC_GetIngamesnCheckResult();
extern "C" int DWC_SetBuddyFriendCallback(void (*cb)(int index, void* param), void* param);
extern "C" int DWC_CheckDirtyFlag(void* data);
extern "C" void DWC_ClearDirtyFlag(void* data);
extern "C" int DWC_SetUserSendCallback(void (*cb)(int size, unsigned char aid, void* param), void* param);
extern "C" int DWC_SetUserRecvCallback(void (*cb)(unsigned char aid, void* data, int size, void* param),
                                       void* param);
extern "C" int DWC_SetUserRecvTimeoutCallback(void (*cb)(unsigned char aid, void* param), void* param);
extern "C" int DWC_SetUserPingCallback(void (*cb)(int latency, unsigned char aid, void* param), void* param);
extern "C" int DWC_SetRecvBuffer(unsigned char aid, void* buf, unsigned long size);
extern "C" int DWC_SetRecvTimeoutTime(unsigned char aid, unsigned long time);
extern "C" int DWC_SendUnreliable(unsigned char aid, const void* data, unsigned long size);
extern "C" int DWC_SendReliable(unsigned char aid, const void* data, unsigned long size);
extern "C" int DWC_IsSendableReliable(unsigned char aid);
extern "C" int DWC_Ping(unsigned char aid);
extern "C" int DWC_CheckProfanityAsync(const wchar_t** words, int num, const char* reserved, int timeout,
                                       void* result, int* badwordsNum);
extern "C" int DWC_CheckProfanityProcess();
extern "C" int DWC_RnkInitialize(const char* secretKey, void* userData);
extern "C" int DWC_RnkShutdown();
extern "C" int DWC_RnkPutScoreAsync(unsigned long category, int region, long score, void* data,
                                    unsigned long size);
extern "C" int DWC_RnkGetScoreAsync(int mode, unsigned long category, int region, void* param);
extern "C" int DWC_RnkCancelProcess();
extern "C" int DWC_RnkProcess();
extern "C" int DWC_RnkGetState();
extern "C" int DWC_RnkResGetRow(void* out, unsigned long index);
extern "C" int DWC_RnkResGetRowCount(unsigned long* count);

// DWC (unnamed in symbols.txt)
extern "C" void fn_806A8200(int arg);
extern "C" void fn_806A8270(void (*cb)(int aid, void* data, int size));
extern "C" void fn_806A8280(void (*cb)(int size, int aid));
extern "C" void fn_806A8290();
extern "C" void fn_806A8D90(int count, void (*cb)(int error));
extern "C" void fn_806A8DB0();
extern "C" int fn_806A8E80(unsigned char aid, const void* data, unsigned long size, int reliable);
extern "C" void fn_806AF6A0(MpFriendList_unk* list, int gameCode, const char* gameName, int arg3, int arg4,
                            void* friends, int count);
extern "C" void fn_806AF910();
extern "C" void fn_806AFAA0();
extern "C" int fn_806AFDB0(const wchar_t* name, int arg, void (*cb)(int error, int arg, void* param),
                           void* param);
extern "C" int fn_806B0180(const wchar_t* name, void (*cb0)(int error, int changed, void* param), void* param0,
                           void (*cb1)(int index, int status, void* param), void* param1,
                           void (*cb2)(int index, int index2, void* param), void* param2);
extern "C" int fn_806B0260(int count, int arg1, const char* filter, void* cb0, void* p0, void* cb1, void* p1,
                           void* cb2, void* p2, void* cb3, void* p3, void* p4);
extern "C" int fn_806B0410(int count, int arg1, void* cb0, void* p0, void* cb1, void* p1, void* cb2, void* p2,
                           void* p3);
extern "C" int fn_806B05B0(int count, int index, void* cb0, void* p0, void* cb1, void* p1, void* cb2,
                           const char* name, void* p3);
extern "C" void fn_806B0950(void (*cb)(), void* param);
extern "C" void fn_806B0980();
extern "C" void fn_806B0CF0(unsigned char value);
extern "C" int fn_806B0EB0(unsigned char** aids);
extern "C" int fn_806B1170(unsigned char aid);
extern "C" void fn_806B3220(int arg, const char* key, int* value);
extern "C" void fn_806B3680(int arg, const char* key, const char* value);
extern "C" int fn_806B3C20();
extern "C" int fn_806B3C40(int open, void (*cb)(int result, int open, void* param), void* param);
extern "C" int fn_806B3CC0();
extern "C" void fn_806CE060(unsigned long value);
extern "C" int fn_80624D40();

// atn/DwcAllocator_unk.cpp
extern "C" void fn_8050D3E8();
extern "C" void fn_8050D484();
extern "C" void* fn_8050D500(int name, unsigned long size, int align);
extern "C" void fn_8050D5B8(int name, void* p, unsigned long size);
extern "C" void* fn_8050D67C(int name, long size);
extern "C" void fn_8050D728(int name, void* p, long size);

// atn/NetFriend_unk.cpp
extern "C" void fn_8050E994(MpFriendList_unk* list);
extern "C" int fn_8050EAE4(MpFriendList_unk* list);
extern "C" void fn_8050EAE8(MpFriendList_unk* list);
extern "C" void fn_8050EB88(MpFriendList_unk* list);
extern "C" void fn_8050F198(MpFriendList_unk* list);
extern "C" int fn_8050F5A8(MpFriendList_unk* list, int arg);
extern "C" int fn_8050F664(MpFriendList_unk* list, int arg);
extern "C" int fn_8050F754(MpFriendList_unk* list, int index);

// atn/PacketPack.cpp
extern "C" atn::net::PacketPack* lbl_8087F62C;

// inferred: debug key of the matchmaking (its literal and value are weak objects emitted first by
// atn/PacketPack.cpp: lbl_80791B00, lbl_8087E160)
inline void __SetDebugKey_unk() {
    static int sValue = 1;
    fn_806B3220(0, "lw_debug", &sValue);
}
// inferred: filter of the debug key (literal in .data, lbl_80793108)
inline void __SetDebugFilter_unk(char* dst) {
    strcpy(dst, "lw_debug = 1");
}

// inferred: empty object with a destructor, static local of an inline function of the messages (its
// guard and storage are emitted by atn/PacketPack.cpp: lbl_8087F5F8/lbl_8087F5FC)
struct NetMsgStatic_unk {
    ~NetMsgStatic_unk(); // 0x804D8188 (atn/PacketPack.cpp)
    int m00;
};
inline void __NetMsgStatic_unk() {
    static NetMsgStatic_unk s;
    s.m00 = 0;
}

// inferred: message with its size and id (only the payload is queued)
template <class T>
struct NetMsg_unk {
    NetMsg_unk() : mSize(2), mId(0) {}
    void Set(unsigned short id, const T& data) {
        __NetMsgStatic_unk();
        mSize = sizeof(T) + 2;
        mId = id;
        mData = data;
    }
    unsigned short mSize;
    unsigned short mId;
    T mData;
};

namespace atn {
namespace net {

static void fn_8050AAE0(int error, int arg, void* param);
void fn_8050AAD0(int aid, void* data, int size);
void fn_8050AA8C(int size, int aid);
static void fn_8050B224(int error, int cancel, int self, int isServer, int index, void* param);
static void fn_8050B658(int error);
void fn_8050B1A8(int result, int open, void* param);

// 0x805089E0: inferred: encrypts (decode == 0) or decrypts a packet
extern "C" int fn_805089E0(Crypt* crypt, int decode, void* data, unsigned int* size) {
    int result = 1;
    if (decode == 0) {
        if (crypt->vf14((unsigned char*)data, crypt->m04, (unsigned char*)data, *size) == 0) {
            return 0;
        }
        *size += 2;
    }
    *size = (((int)*size + 15) / 16) * 16;
    unsigned char* iv;
    if (decode == 0) {
        iv = crypt->vf1C(decode, (unsigned char*)crypt->m08, *size);
    } else {
        iv = crypt->vf1C(decode, (unsigned char*)data, *size);
    }
    NETAESContext_unk ctx;
    NETAESCreate(&ctx, crypt->vf0C(), crypt->vf10(), iv);
    if (decode == 0) {
        NETAESEncrypt(&ctx, crypt->m08, data, *size);
        *size += 16;
    } else {
        NETAESDecrypt(&ctx, crypt->m0C, data, *size - 16);
        result = crypt->vf18((unsigned char*)crypt->m0C, *size - 16);
    }
    NETAESDelete(&ctx);
    return result;
}

// NONMATCHING: 97.92% - register allocation of the constructor loop of the players (the end pointer
// takes r31 here and r27 in the original; same instructions)
Manager::Manager() : m04(0), m08(0), mState(0), mE38(-1) {
    fn_8050D3E8();
}

Manager::~Manager() {
    fn_8050D484();
}

// 0x80508D9C
int Manager::fn_80508D9C(NetCallbacks_unk* callbacks) {
    if (callbacks != 0) {
        mCallbacks = *callbacks;
    }
    mState = 1;
    OSInitMutex(&mMutex);
    DWC_Init(1, "laststorywii", 0x534C5350, (void*)fn_8050D500, (void*)fn_8050D5B8);
    if (mC215 == 1 && mC218 != 1) {
        fn_806A8200(0);
    }
    DWC_SetReportLevel(0);
    if (mCallbacks.mLoadFriends != 0) {
        mCallbacks.mLoadFriends(&mFriendList, 0x9F0);
    }
    m08 = 0;
    m04 = 0;
    mFrame = 0;
    m25C = 0;
    mC0 = 0;
    m264 = 8;
    mFlagsWord = 0;
    mFlags.mB9 = 1;
    m268 = 0;
    m260 = 2;
    m254 = 30;
    m258 = 300;
    m250 = 1000;
    mBEC8 = 0;
    mBEC4 = 0;
    mBECC = 0;
    mSendIndex = 0;
    m0C = 0;
    mPlayers.mSize = 8;
    return 1;
}

// 0x80508F08
int Manager::vf0C() {
    lbl_8087F628->mThread.Request(7, 0);
    if (mC215 == 1 && mC218 != 1) {
        fn_806A8DB0();
    }
    DWC_Shutdown();
    mState = 0;
    return 1;
}

// 0x80508FAC
int Manager::fn_80508FAC() {
    SOLibraryConfig_unk config;
    memset(&config, 0, sizeof(config));
    config.mAlloc = fn_8050D67C;
    config.mFree = fn_8050D728;
    int result = SOInit(&config);
    if (result != 0) {
        switch (result) {
        case -7:
            PushError_unk(1, -7);
            break;
        case -28:
            PushError_unk(2, 0);
            break;
        case -49:
            PushError_unk(3, 0);
            break;
        default:
            PushError_unk(4, result);
            break;
        }
        return 0;
    }
    if (mC215 != 1 || mC218 != 1) {
        result = SOStartup();
        if (result != 0) {
            PushError_unk(5, result);
            return 0;
        }
    }
    return 1;
}

// 0x805092FC
int Manager::fn_805092FC() {
    int running = mThread.mRunning;
    if (running == 0) {
        return 1;
    }
    switch (mState) {
    case 0:
    case 3:
        return 0;
    case 1:
        if (running == 1 && mCallbacks.m20() == 0) {
            if (mCallbacks.m24() != 0 || mCallbacks.m00 != 0 ||
                (mCallbacks.mSaveData != 0 && mCallbacks.mSaveData(mFriendList.m7F0, 0) == 0)) {
                fn_8050E994(&mFriendList);
                if (mCallbacks.mSaveData != 0) {
                    mCallbacks.mSaveData(mFriendList.m7F0, 1);
                }
            }
            fn_8050EB88(&mFriendList);
            mState = 2;
        }
        return 0;
    case 2:
        return 1;
    case 4:
        lbl_8087F628->mFlags.mB14 = 0;
        lbl_8087F628->mThread.Request(2, 0);
        mState = 5;
        break;
    case 5:
        if (lbl_8087F628->mThread.mRunning == 1) {
            if (lbl_8087F628->mThread.mResult != 0) {
                if (mState == 5) {
                    mState = 6;
                }
            } else {
                PushError_unk(0x10, 0);
                mState = 3;
            }
        }
        break;
    case 7:
        if (running == 1 && mCallbacks.m20() == 0) {
            if (mCallbacks.m24() != 0) {
                PushError_unk(0x13, 0);
                mState = 3;
            } else {
                mState = 8;
            }
        }
        break;
    case 14:
        lbl_8087F628->mThread.Request(7, 0);
        mState = 2;
        return 1;
    }
    if (mC215 == 0) {
        if (lbl_8087F628->mThread.mRunning == 1) {
            fn_806AFAA0();
        }
    } else if (mC218 > 1) {
        if (lbl_8087F628->mFlags.mB14 == 1) {
            fn_806A8290();
        }
    }
    int code;
    unsigned int type;
    int error = DWC_GetLastErrorEx(&code, &type);
    if (error != 0) {
        PushError_unk(0x11, error);
        mState = 3;
    }
    switch (mState) {
    case 9:
        if (mC215 == 0) {
            if (fn_8050A100() != 0) {
                mState = 10;
            } else {
                PushError_unk(0x12, 0);
                mState = 3;
            }
        } else {
            fn_8050B224(0, 0, 0, 0, 0, 0);
            lbl_8087F628->mFlags.mB0 = 1;
            lbl_8087F628->mFlags.mB1 = 1;
            lbl_8087F628->mFlags.mB6 = 1;
            lbl_8087F628->mFlags.mB7 = 1;
            lbl_8087F628->mFlags.mB2 = 1;
            lbl_8087F628->mFlags.mB3 = 1;
        }
        break;
    case 10:
        if (!lbl_8087F628->mFlags.mB15) {
            PushError_unk(0x12, 0);
            mState = 3;
        }
        break;
    case 11:
        if (mFrame % m254 == 0) {
            fn_8050BDC4();
        }
        fn_8050BAC4();
        break;
    case 12:
        mState = 13;
        break;
    case 13:
        if (mFrame % m254 == 0) {
            fn_8050BDC4();
        }
        fn_8050BAC4();
        break;
    }
    mStats.fn_8050E6CC(m250);
    mStats.fn_8050E6F4();
    mStats.fn_8050E7DC(mStats.m10.mLast, 3);
    mStats.fn_8050E7DC(mStats.m10.mMax, 3);
    mStats.fn_8050E7DC(mStats.m30.mLast, 3);
    mStats.fn_8050E7DC(mStats.m30.mMax, 3);
    mFrame++;
    if (m04 > 0) {
        if (--m04 == 0) {
            m08 = 0;
        }
    }
    return 1;
}

// 0x80509A88: inferred: locks the mutex of the manager
extern "C" void fn_80509A88(Manager* mgr) {
    OSLockMutex(&mgr->mMutex);
}

// 0x80509A90: inferred: unlocks the mutex of the manager
extern "C" void fn_80509A90(Manager* mgr) {
    OSUnlockMutex(&mgr->mMutex);
}

// 0x80509A98
int Manager::fn_80509A98() {
    if (mState != 2) {
        return 0;
    }
    if (fn_80624D40() != 0) {
        mState = 3;
    } else {
        mState = 4;
    }
    return 1;
}

// 0x80509AF8
int Manager::fn_80509AF8() {
    if (mState != 2) {
        mState = 14;
    } else {
        return 0;
    }
    return 1;
}

// 0x80509B20
int Manager::fn_80509B20(int arg0, int arg1, int arg2) {
    if (mState != 8) {
        return 0;
    }
    mD4 = arg2;
    mD0 = arg1;
    mState = 9;
    return 1;
}

// 0x80509B4C
int Manager::fn_80509B4C(int arg0, int arg1, int arg2) {
    if (mState != 11) {
        return 0;
    }
    mD4 = arg2;
    mD0 = arg1;
    if (mFlags.mB10 != 1 && mFlags.mB7 == 1) {
        lbl_8087F62C->fn_8050E094(-1, 0x13, 0, 1);
        lbl_8087F62C->fn_8050DDDC(-1, 1);
        fn_8050A634(1);
    }
    if (mFlags.mB7 == 1) {
        lbl_8087F62C->fn_8050E094(-1, 8, 0, 1);
        lbl_8087F62C->fn_8050DDDC(-1, 1);
        mState = 12;
    }
    return 1;
}

// 0x80509C88
int Manager::fn_80509C88() {
    if (mState != 13) {
        return 0;
    }
    mState = 11;
    return 1;
}

// 0x80509CAC
int Manager::fn_80509CAC() {
    if (mFlags.mB6 == 1) {
        if (mFlags.mB7 == 1) {
            fn_8050A634(0);
        }
        if (mC215 == 0) {
            fn_806B0980();
        }
        mFlags.mB6 = 0;
        mState = 8;
    }
    return 1;
}

// 0x80509DA0
int Manager::fn_80509DA0() {
    if (mFlags.mB0 == 1) {
        if (mFlags.mB7 == 1) {
            fn_8050A634(0);
        }
        mFlags.mB6 = 0;
        Cleanup_unk();
        mState = 2;
    }
    return 1;
}

// 0x80509F08
void Manager::fn_80509F08() {
    DWC_ClearError();
    mBEC8 = 0;
    mBEC4 = 0;
    mBECC = 0;
}


// 0x80509F48
int Manager::fn_80509F48() {
    if (fn_80508FAC() == 0) {
        mState = 3;
        return 0;
    }
    if (mC215 == 0) {
        fn_806AF6A0(&mFriendList, 0x32B9, "2sapf3", 0, 0, mFriendList.mFriends, 0x20);
        wchar_t name[16];
        wcscpy(name, mFriendList.mName);
        if (fn_806AFDB0(name, 0, fn_8050AAE0, 0) == 0) {
            return 0;
        }
        lbl_8087F628->mFlags.mB0 = 1;
        lbl_8087F628->mFlags.mB1 = 0;
    } else {
        if (mC218 != 1) {
            fn_806A8270(fn_8050AAD0);
            fn_806A8280(fn_8050AA8C);
        }
        if (mC218 == 1) {
            lbl_8087F628->mFlags.mB12 = 1;
            fn_8050B224(0, 0, 0, 0, 0, 0);
        } else {
            fn_806A8D90(mC218, fn_8050B658);
        }
        lbl_8087F628->mFlags.mB0 = 1;
        lbl_8087F628->mFlags.mB1 = 1;
        lbl_8087F628->mFlags.mB6 = 1;
        lbl_8087F628->mFlags.mB7 = 1;
        lbl_8087F628->mFlags.mB2 = 1;
        lbl_8087F628->mFlags.mB3 = 1;
        lbl_8087F628->mFlags.mB14 = 1;
    }
    return 1;
}

// 0x8050A100
int Manager::fn_8050A100() {
    memset(m10, 0, sizeof(m10));
    m26C = 0;
    mC210 = 0;
    __SetDebugKey_unk();
    __SetDebugFilter_unk(m10);
    NetFilterKey_unk* key = mKeys;
    for (int i = 0; i < 8; i++, key++) {
        char buf[0x80];
        buf[0] = 0;
        if (key->mType == 1) {
            fn_806B3680(0, key->mKey.c_str(), key->mString.c_str());
            if ((key->mFlags & 1) != 1) {
                sprintf(buf, "%s = %s\0", key->mKey.c_str(), key->mString.c_str());
            }
        } else if (key->mType == 2) {
            fn_806B3220(0, key->mKey.c_str(), &key->mInt);
            if ((key->mFlags & 1) != 1) {
                sprintf(buf, "%s = %d\0", key->mKey.c_str(), key->mInt);
            }
        } else {
            break;
        }
        if ((key->mFlags & 1) != 1) {
            strcat(m10, " and ");
            strcat(m10, buf);
        }
    }
    lbl_8087F628->mFlags.mB15 = 1;
    int ok = 0;
    if (m25C == 0) {
        if (lbl_8087F628->mC215 == 0) {
            lbl_8087F628->mFlags.mB7 = 0;
            ok = lbl_8087F628->mThread.Request(5, 0);
        }
    } else {
        lbl_8087F628->mFlags.mB7 = 0;
        ok = lbl_8087F628->mThread.Request(6, 0);
    }
    if (ok == 0) {
        return 0;
    }
    lbl_8087F628->mFlags.mB6 = 1;
    lbl_8087F628->mFlags.mB7 = 0;
    lbl_8087F628->mFlags.mB12 = 0;
    return 1;
}

// inferred: function removed by the linker; it explains the "" of the string pool after " and "
void Manager::Stripped1_unk() {
    strcpy(m10, "");
}

// 0x8050A3D0
int Manager::fn_8050A3D0(int send) {
    if (mFlags.mB7 == 1) {
        if (send != 0) {
            lbl_8087F62C->fn_8050E094(-1, 0x14, 0, 1);
            lbl_8087F62C->fn_8050DDDC(-1, 1);
        }
        return fn_8050A634(0);
    }
    return 0;
}

// 0x8050A4E0
int Manager::fn_8050A4E0(int send) {
    if (mFlags.mB10 == 1) {
        return 1;
    }
    if (mFlags.mB7 == 1) {
        if (send != 0) {
            lbl_8087F62C->fn_8050E094(-1, 0x13, 0, 1);
            lbl_8087F62C->fn_8050DDDC(-1, 1);
        }
        return fn_8050A634(1);
    }
    return 0;
}

// 0x8050A5EC
void Manager::fn_8050A5EC() {
    if (fn_806B3CC0() == 0) {
        mFlags.mB8 = 0;
        mFlags.mB10 = 0;
        mFlags.mB9 = 1;
    }
}

// 0x8050A634: inferred: opens or closes the group to new players
int Manager::fn_8050A634(int open) {
    if (!mFlags.mB9) {
        return 0;
    }
    if (fn_806B3CC0() == open && mFlags.mB8 == (open != 0)) {
        return 0;
    }
    mFlags.mB8 = (open != 0);
    mFlags.mB9 = 0;
    if (mC215 == 0) {
        int result = fn_806B3C40(open, fn_8050B1A8, 0);
        fn_8050B1A8(open, 0, 0);
        if (result == 0) {
            mFlags.mB9 = 1;
        }
        return result;
    }
    return 1;
}

// 0x8050A72C: inferred: message handler (player information)
void fn_8050A72C(unsigned int aid, const NetPlayerInfo_unk* info) {
    Manager* mgr = lbl_8087F628;
    if (aid >= mgr->mPlayers.mSize) {
        return;
    }
    if (info->mAid != aid) {
        return;
    }
    if (!mgr->mPlayers[aid].mFlags.mConnected && info->mFlags.mConnected && mgr->m0C == 1) {
        return;
    }
    int ping = mgr->mPlayers[aid].mPing;
    mgr->mPlayers[aid] = *info;
    lbl_8087F628->mPlayers[aid].mPing = ping;
}

// 0x8050A7F4: inferred: message handler (0x13)
void fn_8050A7F4(unsigned int aid, const void* data) {
    Manager* mgr = lbl_8087F628;
    if (mgr->mFlags.mB10 != 1 && mgr->mFlags.mB7 == 1) {
        mgr->fn_8050A634(1);
    }
}

// 0x8050A8A4: inferred: message handler (0x14)
void fn_8050A8A4(unsigned int aid, const void* data) {
    Manager* mgr = lbl_8087F628;
    if (mgr->mFlags.mB7 == 1) {
        mgr->fn_8050A634(0);
    }
}

// 0x8050A960: inferred: message handler (8)
void fn_8050A960(unsigned int aid, const void* data) {
    if (lbl_8087F628->mFlags.mB7 == 1) {
        lbl_8087F628->mState = 12;
    }
}

// 0x8050A980
void fn_8050A980(unsigned int aid, const void* data) {
    lbl_8087F628->mState = 11;
}

// 0x8050A990
void fn_8050A990(unsigned int aid, const unsigned char* data) {
    unsigned char value = *data;
    if (aid >= lbl_8087F628->mPlayers.mSize) {
        return;
    }
    if (lbl_8087F628->GetMyAid_unk() == lbl_8087F628->GetServerAid_unk()) {
        fn_806B0CF0(value);
    }
}

// 0x8050AA6C: inferred: send callback (release asserts: the original calls it out of line from
// fn_8050AA8C)
void fn_8050AA6C(int size, unsigned char aid, void* param) {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    lbl_8087F628->mStats.AddSend_unk(size);
}

// 0x8050AA8C
void fn_8050AA8C(int size, int aid) {
    fn_8050AA6C(size, aid, 0);
}

// 0x8050AA98: inferred: receive callback (release asserts: called out of line from fn_8050AAD0)
void fn_8050AA98(unsigned char aid, void* data, int size, void* param) {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    lbl_8087F628->mStats.AddRecv_unk(size);
    if (lbl_8087F628->mCallbacks.mRecv != 0) {
        lbl_8087F628->mCallbacks.mRecv(aid, data, size);
    }
}

// 0x8050AAD0
void fn_8050AAD0(int aid, void* data, int size) {
    fn_8050AA98(aid, data, size, 0);
}

// 0x8050AADC
static void fn_8050AADC() {}

// 0x8050AAE0: inferred: login callback, handled by the thread (command 3)
static void fn_8050AAE0(int error, int arg, void* param) {
    static int sArgs[2];
    sArgs[0] = error;
    sArgs[1] = arg;
    lbl_8087F628->mThread.Request(3, sArgs);
}

static void fn_8050ADA4(int index, void* param);
static void fn_8050ADF0(int index, int index2, void* param);
static void fn_8050AE3C(int error, int changed, void* param);
static void fn_8050B1A4(int index, int status, void* param);

// 0x8050AB58
void Manager::fn_8050AB58(int error, int arg) {
    if (lbl_8087F628->mState == 6) {
        if (error == 0) {
            if (DWC_GetIngamesnCheckResult() == 2) {
                lbl_8087F628->PushError_unk(7, 0);
                lbl_8087F628->mState = 3;
                return;
            }
            lbl_8087F628->mFlags.mB1 = 1;
            DWC_SetBuddyFriendCallback(fn_8050ADA4, 0);
            if (lbl_8087F628->mState == 6) {
                lbl_8087F628->mFlags.mB3 = 1;
                if (fn_806B0180(0, fn_8050AE3C, 0, fn_8050B1A4, 0, fn_8050ADF0, 0) == 0) {
                    lbl_8087F628->PushError_unk(10, 0);
                    lbl_8087F628->mState = 3;
                } else {
                    fn_8050F198(&lbl_8087F628->mFriendList);
                    lbl_8087F628->mFlags.mB4 = 1;
                    lbl_8087F628->mFlags.mB5 = 0;
                }
            }
            lbl_8087F628->mE38 = arg;
            lbl_8087F628->mFlags.mB2 = 1;
            lbl_8087F628->mFlags.mB3 = 0;
        } else {
            lbl_8087F628->mState = 3;
        }
    }
}

// 0x8050ADA4: inferred: friend callback
static void fn_8050ADA4(int index, void* param) {
    if (lbl_8087F628->mCallbacks.mSaveFriends != 0) {
        fn_8050EAE8(&lbl_8087F628->mFriendList);
        lbl_8087F628->mCallbacks.mSaveFriends(&lbl_8087F628->mFriendList, 0x9F0);
    }
}

// 0x8050ADF0: inferred: friend deleted callback
static void fn_8050ADF0(int index, int index2, void* param) {
    if (lbl_8087F628->mCallbacks.mSaveFriends != 0) {
        fn_8050EAE8(&lbl_8087F628->mFriendList);
        lbl_8087F628->mCallbacks.mSaveFriends(&lbl_8087F628->mFriendList, 0x9F0);
    }
}

inline void Manager::SetB13_unk(int flag) {
    mFlags.mB13 = flag;
    if (flag != 0) {
        fn_806CE060(0x19999999);
    } else {
        fn_806CE060(15000);
    }
}

// 0x8050AE3C: inferred: servers updated callback
static void fn_8050AE3C(int error, int changed, void* param) {
    if (error == 0) {
        if (changed != 0) {
            if (lbl_8087F628->mCallbacks.mSaveFriends != 0) {
                fn_8050EAE8(&lbl_8087F628->mFriendList);
                lbl_8087F628->mCallbacks.mSaveFriends(&lbl_8087F628->mFriendList, 0x9F0);
            }
        }
        if (lbl_8087F628->mState == 6) {
            lbl_8087F628->mFlags.mB5 = 1;
            fn_8050EB88(&lbl_8087F628->mFriendList);
            if (fn_8050EAE4(&lbl_8087F628->mFriendList) != 0) {
                if (DWC_CheckDirtyFlag(&lbl_8087F628->mFriendList) != 0) {
                    DWC_ClearDirtyFlag(&lbl_8087F628->mFriendList);
                }
                if (lbl_8087F628->mCallbacks.mSaveFriends != 0) {
                    fn_8050EAE8(&lbl_8087F628->mFriendList);
                    lbl_8087F628->mCallbacks.mSaveFriends(&lbl_8087F628->mFriendList, 0x9F0);
                }
                lbl_8087F628->mState = 7;
            } else {
                lbl_8087F628->mState = 8;
            }
            Manager* mgr = lbl_8087F628;
            int flag = mgr->mFlags.mB13;
            if (mgr->mState >= 8) {
                mgr->SetB13_unk(flag);
            }
        }
    } else {
        lbl_8087F628->PushError_unk(12, 0);
        lbl_8087F628->mState = 3;
    }
}

// 0x8050B018: inferred: receive timeout callback
static void fn_8050B018(unsigned char aid, void* param) {
    if (lbl_8087F628->GetMyAid_unk() != lbl_8087F628->GetServerAid_unk() &&
        aid != lbl_8087F628->GetServerAid_unk()) {
        NetMsg_unk<unsigned char> msg;
        msg.Set(0x10, aid);
        lbl_8087F62C->fn_8050E094(-1, 0x10, &msg.mData, 1);
        lbl_8087F62C->fn_8050DDDC(-1, 1);
    }
}

// 0x8050B1A4
static void fn_8050B1A4(int index, int status, void* param) {}

// 0x8050B1A8: inferred: group opened/closed callback
void fn_8050B1A8(int result, int open, void* param) {
    if (result == 0) {
        lbl_8087F628->mFlags.mB9 = 1;
        lbl_8087F628->mFlags.mB10 = (open != 0);
    }
}

// 0x8050B1DC: inferred: ping callback
static void fn_8050B1DC(int latency, unsigned char aid, void* param) {
    NetPlayerInfo_unk* info = lbl_8087F628->vf14(aid);
    if (info != 0) {
        info->mPing = latency;
    }
}

static void fn_8050B670(int arg0, int arg1, void* param);

// 0x8050B224: inferred: matched callback
static void fn_8050B224(int error, int cancel, int self, int isServer, int index, void* param) {
    if (error == 0) {
        if (cancel == 0) {
            lbl_8087F628->mPlayers.mSize = lbl_8087F628->m264;
            for (unsigned int i = 0; i < lbl_8087F628->mPlayers.mSize; i++) {
                NetPlayerInfo_unk* info = &lbl_8087F628->mPlayers.mData[i];
                memset(info->mName, 0, sizeof(info->mName));
                info->m30 = 0;
            }
            if (lbl_8087F628->GetMyAid_unk() > lbl_8087F628->mPlayers.mSize) {
                lbl_8087F628->PushError_unk(13, 0);
                lbl_8087F628->mState = 3;
                return;
            }
            lbl_8087F628->fn_8050BDC4();
            lbl_8087F628->SetRecvBuffers_unk();
            if (lbl_8087F628->mC215 == 0) {
                DWC_SetUserSendCallback(fn_8050AA6C, 0);
                DWC_SetUserRecvCallback(fn_8050AA98, 0);
                DWC_SetUserRecvTimeoutCallback(fn_8050B018, 0);
                fn_806B0950(fn_8050AADC, 0);
                DWC_SetUserPingCallback(fn_8050B1DC, 0);
            }
            Manager* mgr = lbl_8087F628;
            mgr->m26C = fn_806B3C20();
            lbl_8087F628->mFlags.mB7 = 1;
            lbl_8087F628->mState = 11;
        } else if (self != 0 || isServer != 0) {
            lbl_8087F628->PushError_unk(14, 0);
            lbl_8087F628->mState = 3;
        }
    } else {
        lbl_8087F628->PushError_unk(15, 0);
        lbl_8087F628->mState = 3;
    }
    if (lbl_8087F628->m08 > 0) {
        if (--lbl_8087F628->m08 == 0) {
            lbl_8087F628->m04 = 0;
        }
    }
}

// 0x8050B658
static void fn_8050B658(int error) {
    fn_8050B224(error, 0, 0, 0, 0, 0);
}

// 0x8050B670: inferred: new client callback (receive buffers of the other players)
static void fn_8050B670(int arg0, int arg1, void* param) {
    lbl_8087F628->SetRecvBuffers_unk();
}

inline void Manager::SetRecvBuffers_unk() {
    if (mC215 == 0) {
        unsigned char* aids;
        int count = fn_806B0EB0(&aids);
        int i;
        int skip;
        for (i = 0, skip = 0; i < count; i++) {
            if (aids[i] == GetMyAid_unk()) {
                skip++;
            } else {
                DWC_SetRecvBuffer(aids[i], mRecvBuffers[i - skip], 0x1000);
                DWC_SetRecvTimeoutTime(aids[i], 15000);
            }
        }
    }
}

// 0x8050B75C
static int fn_8050B75C(int arg0, int arg1) {
    if (lbl_8087F628->mCallbacks.m10 != 0) {
        return lbl_8087F628->mCallbacks.m10(arg0, arg1);
    }
    return 1;
}

// 0x8050B77C
static int fn_8050B77C(int arg0, int arg1) {
    Manager* mgr = lbl_8087F628;
    if (mgr->mFlags.mB8) {
        return 0;
    }
    int result = 1;
    if (mgr->mCallbacks.m14 != 0) {
        result = mgr->mCallbacks.m14(arg0, arg1);
    }
    if (result == 1) {
        lbl_8087F628->mC210++;
    }
    return result;
}

// 0x8050B7EC: inferred: sends a packet (extern "C" because atn/PPDATAU.cpp calls it with that name)
extern "C" int fn_8050B7EC(Manager* mgr, const void* data, unsigned int size, unsigned char aid, int reliable) {
    return mgr->Send_unk(data, size, aid, reliable);
}

int Manager::Send_unk(const void* data, unsigned int size, unsigned char aid, int reliable) {
    if (mFlags.mB12) {
        return -4;
    }
    if (mC215 == 0 && fn_806B1170(aid) == 0) {
        return -1;
    }
    int result;
    if (reliable == 0) {
        if (mC215 == 0) {
            result = DWC_SendUnreliable(aid, data, size);
        } else {
            result = fn_806A8E80(aid, data, size, 0);
        }
    } else {
        if (mC215 == 0 && DWC_IsSendableReliable(aid) == 0) {
            return -2;
        }
        memcpy(mSendBuffers[mSendIndex].mData, data, size);
        if (mC215 == 0) {
            result = DWC_SendReliable(aid, mSendBuffers[mSendIndex].mData, size);
        } else {
            result = fn_806A8E80(aid, mSendBuffers[mSendIndex].mData, size, 1);
        }
        if (++mSendIndex >= 12) {
            mSendIndex = 0;
        }
    }
    return result != 0 ? 0 : -3;
}

// 0x8050B968
const char* Manager::fn_8050B968(int state) {
    switch (state) {
    case 0:
        return "INVALID";
    case 2:
        return "INITIALIZE";
    case 3:
        return "ERROR";
    case 4:
        return "REGISTER";
    case 5:
        return "REGISTER2";
    case 6:
        return "REGISTERING";
    case 8:
        return "REGISTERED";
    case 9:
        return "JOIN";
    case 10:
        return "JOINING";
    case 11:
        return "JOINED";
    case 12:
        return "PLAY";
    case 13:
        return "PLAYING";
    case 14:
        return "UNREGISTER";
    }
    return "UNKNOW";
}

// 0x8050BA68: inferred: list of the connected aids
extern "C" int fn_8050BA68(unsigned char** aids) {
    if (lbl_8087F628->mC215 == 0) {
        return fn_806B0EB0(aids);
    }
    static unsigned char sAids[16];
    *aids = sAids;
    for (int i = 0; i < lbl_8087F628->mC218; i++) {
        sAids[i] = i;
    }
    return lbl_8087F628->mC218;
}

// 0x8050BAC4
int Manager::fn_8050BAC4() {
    int unchanged = 1;
    for (int i = 0; i < (int)mPlayers.mSize; i++) {
        NetPlayerInfo_unk* info = &mPlayers.mData[(unsigned int)i];
        if (info->mAid != i) {
            info->mAid = i;
            unchanged = 0;
        }
        if ((unsigned char)i != GetMyAid_unk()) {
            int connected;
            if (mC215 == 0) {
                connected = fn_806B1170(i);
            } else {
                connected = 1;
            }
            if ((info->mFlags.mConnected == 1 && connected == 0) || (info->mFlags.mConnected == 0 && connected != 0)) {
                info->mFlags.mConnected = (connected != 0);
                unchanged = 0;
            }
            int server = (unsigned char)i == GetServerAid_unk();
            if ((info->mFlags.mServer == 1 && server == 0) || (info->mFlags.mServer == 0 && server != 0)) {
                info->mFlags.mServer = (server != 0);
                unchanged = 0;
            }
            int self = (unsigned char)i == GetMyAid_unk();
            if ((info->mFlags.mSelf == 1 && self == 0) || (info->mFlags.mSelf == 0 && self != 0)) {
                info->mFlags.mSelf = (self != 0);
                unchanged = 0;
            }
        }
    }
    if (mFrame % m258 == 0) {
        for (int i = 0; i < (int)mPlayers.mSize; i++) {
            if (mPlayers.mData[(unsigned int)i].mFlags.mConnected) {
                if ((unsigned char)i != GetMyAid_unk() && mC215 == 0) {
                    DWC_Ping(i);
                }
            }
        }
    }
    return unchanged;
}

// 0x8050BDC4
int Manager::fn_8050BDC4() {
    NetPlayerInfo_unk* info = vf14(GetMyAid_unk());
    if (info == 0) {
        return 0;
    }
    info->mAid = GetMyAid_unk();
    info->m04 = mE38;
    info->mFlags.mConnected = 1;
    info->mFlags.mServer = GetMyAid_unk() == GetServerAid_unk();
    info->mFlags.mSelf = 1;
    info->mFlags.mB3 = mFlags.mB10 ^ 1;
    info->mFlags.mB4 = mFlags.mB11;
    wcscpy(info->mName, mFriendList.mName);
    info->m30 = mFriendList.mA00;
    NetMsg_unk<NetPlayerInfo_unk> msg;
    msg.Set(6, *info);
    msg.mData.mFlags.mSelf = 0;
    lbl_8087F62C->fn_8050E094(-1, 6, &msg.mData, 0);
    return 1;
}

// 0x8050C0B4
int Manager::fn_8050C0B4() {
    switch (mBECC) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 1;
    case 3:
    case 4:
    case 5:
        return 2;
    case 6:
        return 3;
    case 7:
    case 8:
        return 4;
    }
    return 0;
}

// 0x8050C128
int Manager::fn_8050C128(int* code) {
    if (mErrors.size() == 0) {
        return 0;
    }
    NetError_unk error = mErrors.back();
    mErrors.clear();
    *code = error.mCode;
    return error.mType;
}

// 0x8050C168
int Manager::fn_8050C168() {
    int code;
    unsigned int type;
    int error = DWC_GetLastErrorEx(&code, &type);
    lbl_8087F628->mBEC4 = code - code * 2;
    lbl_8087F628->mBEC8 = error;
    lbl_8087F628->mBECC = type;
    return type > 1;
}

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

// inferred: parameters of DWC_RnkGetScoreAsync (DWCRnkGetParam)
struct NetRankParam_unk {
    unsigned long mSize;
    unsigned long mSort;
    unsigned long mLimit;
    unsigned long mSince;
    unsigned long mFriends[64];
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

// inferred: the DWC_Rnk call succeeded
static inline bool __RnkOk_unk(int error) {
    switch (error) {
    case 1:
        return false;
    case 3:
        return false;
    case 4:
        return false;
    case 5:
        return false;
    case 6:
        return false;
    case 7:
    case 10:
        return false;
    case 16:
        return false;
    case 2:
        return false;
    case 8:
        return false;
    case 17:
        return false;
    case 9:
    case 12:
        return false;
    case 18:
        return false;
    case 15:
        return false;
    }
    return true;
}

// 0x8050C1D8
int NetRanking_unk::fn_8050C1D8(void* userData) {
    mState = 1;
    if (!__RnkOk_unk(DWC_RnkInitialize("ObYnBUEJZyZjEVrjqxjJ00005695000079650000004030025acblaststorywii", userData))) {
        Fail_unk();
        return 0;
    }
    mRows.clear();
    mIndex = -1;
    return 1;
}

// 0x8050C2F4
int NetRanking_unk::fn_8050C2F4() {
    switch (mState) {
    case 0:
        return 0;
    case 2:
        return 0;
    case 1:
        return 1;
    case 3:
        switch (DWC_CheckProfanityProcess()) {
        case 2:
            if (mProfanityCount != 0) {
                Fail_unk();
                return 0;
            }
            if (!__RnkOk_unk(DWC_RnkPutScoreAsync(mCategory, 4, mScore, (void*)mName, mDataSize))) {
                Fail_unk();
                return 0;
            }
            mState = 4;
            break;
        case 0:
        case 3:
            Fail_unk();
            return 0;
        }
        return 1;
    case 4:
        if (DWC_RnkProcess() != 0) {
            if (DWC_RnkGetState() == 6) {
                Fail_unk();
                return 0;
            }
            mState = 5;
        }
        return 1;
    case 6:
        if (DWC_RnkProcess() != 0) {
            if (DWC_RnkGetState() == 6) {
                Fail_unk();
                return 0;
            }
            unsigned long count;
            if (!__RnkOk_unk(DWC_RnkResGetRowCount(&count))) {
                Fail_unk();
                return 0;
            }
            for (unsigned long i = 0; i < count; i++) {
                NetRankData_unk data;
                if (!__RnkOk_unk(DWC_RnkResGetRow(&data, i))) {
                    Fail_unk();
                    return 0;
                }
                mRows.push_back(data);
            }
            if (mCallback != 0 && mRows.size() != 0) {
                mCallback(0, &mRows[0]);
            }
            mState = 8;
        }
        return 1;
    case 5:
    case 8:
        return 1;
    }
    return 1;
}

// 0x8050C7F0
int NetRanking_unk::fn_8050C7F0() {
    if (mState == 0) {
        return 0;
    }
    DWC_RnkShutdown();
    mRows.clear();
    mState = 0;
    return 1;
}

// 0x8050C840: inferred: the ranking `index` can be requested again
int NetRanking_unk::fn_8050C840(unsigned int index) {
    if (lbl_8087F628 == 0 || index > 3) {
        return 0;
    }
    int time = mTimes[index];
    if (time == -1) {
        return 1;
    }
    return (int)(lbl_8087F628->mFrame - time) >= 4500;
}

// 0x8050C898
int NetRanking_unk::fn_8050C898(unsigned long category, unsigned long limit, unsigned long since,
                                void (*cb)(int, NetRankData_unk*)) {
    mIndex = 1;
    mCallback = cb;
    NetRankParam_unk param;
    param.mSize = 0xC;
    param.mSort = 1;
    param.mLimit = limit;
    param.mSince = since;
    bool ok = __RnkOk_unk(DWC_RnkGetScoreAsync(1, category, 4, &param));
    if (ok) {
        mState = 6;
        mTimes[1] = lbl_8087F628->mFrame;
    } else {
        Fail_unk();
    }
    return ok;
}

// 0x8050C9D4
int NetRanking_unk::fn_8050C9D4(unsigned long category, int limit, unsigned long since,
                                void (*cb)(int, NetRankData_unk*)) {
    mIndex = 0;
    mCallback = cb;
    NetRankParam_unk param;
    param.mSize = 0xC;
    param.mSort = 1;
    param.mLimit = limit > 2 ? limit : 2;
    param.mSince = since;
    bool ok = __RnkOk_unk(DWC_RnkGetScoreAsync(2, category, 4, &param));
    if (ok) {
        mState = 6;
        mTimes[0] = lbl_8087F628->mFrame;
    } else {
        Fail_unk();
    }
    return ok;
}

// 0x8050CB24
int NetRanking_unk::fn_8050CB24(unsigned long category, int limit, unsigned long since,
                                void (*cb)(int, NetRankData_unk*)) {
    mIndex = 2;
    mCallback = cb;
    NetRankParam_unk param;
    memset(&param, 0, sizeof(param));
    param.mSize = 0x10C;
    param.mSort = 1;
    param.mLimit = limit > 2 ? limit : 2;
    param.mSince = since;
    int index = fn_8050F5A8(&lbl_8087F628->mFriendList, 1);
    unsigned long* friends = (unsigned long*)&param;
    while (index != -1) {
        friends[4] = fn_8050F754(&lbl_8087F628->mFriendList, index);
        index = fn_8050F664(&lbl_8087F628->mFriendList, 1);
        friends++;
    }
    bool ok = __RnkOk_unk(DWC_RnkGetScoreAsync(3, category, 4, &param));
    if (ok) {
        mState = 6;
        mTimes[2] = lbl_8087F628->mFrame;
    } else {
        Fail_unk();
    }
    return ok;
}

// 0x8050CCD0: inferred: checks the name, then sends the score
int NetRanking_unk::fn_8050CCD0(unsigned long category, long score, const wchar_t* name, unsigned long size) {
    mIndex = 3;
    const wchar_t* words[1];
    words[0] = name;
    mProfanityCount = 0;
    memset(&mProfanityResult, 0, sizeof(mProfanityResult));
    mCategory = category;
    mScore = score;
    mName = words[0];
    mDataSize = size;
    int result = DWC_CheckProfanityAsync(words, 1, 0, 0, &mProfanityResult, &mProfanityCount);
    if (result != 0) {
        mState = 3;
        mTimes[3] = lbl_8087F628->mFrame;
    } else {
        Fail_unk();
    }
    return result;
}

// 0x8050CDA8
int NetRanking_unk::fn_8050CDA8() {
    DWC_RnkCancelProcess();
    DWC_RnkShutdown();
    return 1;
}

// 0x8050CDD0
int NetThread_unk::fn_8050CDD0(void* (*func)(void*), int priority) {
    mStack = fn_800838B8(atn::MemoryManager::Instance(), 0xFFFF, 4, 5, "", "", 0);
    if (mStack == 0) {
        return 0;
    }
    mCmd = 0;
    OSInitThreadQueue(&mQueue);
    return OSCreateThread(&mThread, func, 0, (char*)mStack + 0xFFFF, 0xFFFF, priority, 1) != 0;
}

// 0x8050CE9C
int NetThread_unk::fn_8050CE9C() {
    if (!OSIsThreadTerminated(&mThread)) {
        Request(12, 0);
    }
    return 1;
}

// 0x8050CF14
int NetThread_unk::fn_8050CF14() {
    return OSIsThreadTerminated(&mThread);
}

// 0x8050CF1C
int NetThread_unk::Request(int cmd, void* arg) {
    if (OSIsThreadSuspended(&mThread) == 1) {
        OSResumeThread(&mThread);
    }
    mArg = arg;
    mCmd = cmd;
    OSWakeupThread(&mQueue);
    mRunning = 0;
    return 1;
}

// 0x8050CF90
void NetThread_unk::fn_8050CF90() {
    m330 = 0;
    if (mCmd == 8) {
        mCmd = 11;
    } else if (mCmd >= 5 && mCmd <= 6) {
        m330 = 1;
    }
}

inline void Manager::Cleanup_unk() {
    if (mFlags.mB6 == 1) {
        if (mC215 == 0) {
            fn_806B0980();
        }
        mFlags.mB6 = 0;
    }
    if (mFlags.mB2 == 1) {
        mFlags.mB2 = 0;
    }
    if (mFlags.mB0 == 1) {
        fn_806AF910();
        mFlags.mB0 = 0;
    }
    fn_80509F08();
    mErrors.mCount = 0;
    SOCleanup();
    SOFinish();
}

inline int Manager::Match_unk() {
    return fn_806B0260(lbl_8087F628->m260, 6, m10, (void*)fn_8050B224, 0, (void*)fn_8050B670, 0, (void*)fn_8050B75C,
                       0, (void*)fn_8050B77C, 0, 0);
}

inline int Manager::IsJoined_unk() {
    int joined = 0;
    if (mFlags.mB6 == 1 && mFlags.mB7 == 1) {
        joined = 1;
    }
    return joined;
}

// 0x8050CFC8: inferred: main function of the worker thread
// NONMATCHING: 98.47% - command 5 reloads lbl_8087F628 for m260 in the original (here it reuses mgr),
// and commands 8/default reuse mgr + 0x10000 from the switch (r4) instead of recomputing it
void* fn_8050CFC8(void* arg) {
    while (true) {
        Manager* mgr = lbl_8087F628;
        int done = 0;
        switch (mgr->mThread.mCmd) {
        case 1:
            lbl_8087F628->mThread.mResult = mgr->fn_80508D9C((NetCallbacks_unk*)mgr->mThread.mArg);
            done = 1;
            lbl_8087F628->mThread.mCmd = 11;
            break;
        case 2:
            lbl_8087F628->mThread.mResult = mgr->fn_80509F48();
            done = 1;
            lbl_8087F628->mThread.mCmd = 11;
            break;
        case 3: {
            int* args = (int*)mgr->mThread.mArg;
            mgr->fn_8050AB58(args[0], args[1]);
            lbl_8087F628->mThread.mCmd = 11;
            break;
        }
        case 7:
            mgr->Cleanup_unk();
            done = 1;
            lbl_8087F628->mThread.mCmd = 11;
            break;
        case 5: {
            int result = mgr->Match_unk();
            done = 1;
            lbl_8087F628->mFlags.mB6 = 1;
            lbl_8087F628->mFlags.mB7 = 0;
            lbl_8087F628->mFlags.mB12 = 0;
            lbl_8087F628->mFlags.mB15 = result;
            lbl_8087F628->mThread.mCmd = 8;
            break;
        }
        case 6: {
            int result;
            if (mgr->m90 == 1) {
                result = fn_806B0410(mgr->m260, 6, (void*)fn_8050B224, 0, (void*)fn_8050B670, 0, (void*)fn_8050B77C, 0,
                                     0);
            } else {
                result = fn_806B05B0(mgr->m260, mgr->mC214, (void*)fn_8050B224, 0, (void*)fn_8050B670, 0, 0,
                                     mgr->mC1, 0);
            }
            done = 1;
            lbl_8087F628->mFlags.mB6 = 1;
            lbl_8087F628->mFlags.mB7 = 0;
            lbl_8087F628->mFlags.mB12 = 0;
            lbl_8087F628->mFlags.mB15 = result;
            lbl_8087F628->mThread.mCmd = 8;
            break;
        }
        case 8:
            if (mgr->mThread.m330 == 1) {
                mgr->mThread.m330 = 0;
                lbl_8087F628->mThread.mCmd = 11;
            } else {
                if (mgr->IsJoined_unk() == 1) {
                    mgr->mThread.mCmd = 11;
                } else {
                    mgr->mC0 = 1;
                    fn_806AFAA0();
                    lbl_8087F628->mC0 = 0;
                    OSSleepTicks(OSMicrosecondsToTicks_unk((long long)16666));
                }
            }
            done = 1;
            break;
        case 12:
            goto exit;
        default:
            mgr->mThread.m330 = 0;
            lbl_8087F628->mThread.mRunning = 1;
            OSSleepThread(&lbl_8087F628->mThread.mQueue);
            done = 1;
            lbl_8087F628->mThread.mRunning = 0;
            break;
        }
        if (done == 0) {
            OSSleepTicks(OSMillisecondsToTicks_unk((long long)100));
        }
    }
exit:
    lbl_8087F628->mThread.mRunning = 1;
    return 0;
}

} // namespace net
} // namespace atn
