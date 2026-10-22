// inferred: network statistics timer (counters per second) and the friend list of the Wi-Fi
// Connection (DWC friend data, names and friend keys). Names invented. Not reconstructed:
// 0x8050EBC8 (adding a friend by friend key); its two L"" literals come first in the wide string
// pool, so every function that uses the pool is NONMATCHING by its offsets until it is written.

#include "std/string.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" long long OSGetTime();
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" unsigned int fn_800DC500(const void* data, unsigned long len); // inferred: hash
extern "C" int DWC_CheckDirtyFlag(void* data);
extern "C" unsigned long long DWC_CreateFriendKey(void* userData);
extern "C" int DWC_GetFriendDataType(void* friendData);
extern "C" int DWC_IsValidFriendData(void* friendData);
extern "C" int DWC_GetGsProfileId(void* userData, void* friendData);
extern "C" int fn_806CF0E0(void* friendData);
extern "C" unsigned long long fn_806D0030(void* friendData); // inferred: friend key of the data
extern "C" unsigned char fn_806ABCF0(void* friendData, void* status); // inferred: online state
extern "C" int fn_806CF160(void* friendData);

#define OS_TIMER_CLOCK_UNK (*(unsigned long*)0x800000F8 / 4)

// inferred: one counter: current, maximum and last value per period
struct NetCounter_unk {
    NetCounter_unk() : mCount(0), mMax(0), mLast(0) {}
    void Update() {
        mLast = mCount;
        if (mCount > mMax) {
            mMax = mCount;
        }
        mCount = 0;
    }
    int mCount;
    int mMax;
    int mLast;
    int m0C;
};

// inferred: statistics of the network manager (period, last update, four counters)
struct NetStats_unk {
    NetStats_unk();
    void fn_8050E6CC(int ms);
    void fn_8050E6F4();
    float fn_8050E7DC(float value, int unit);

    unsigned int mPeriod;  // 0x00: in ticks
    unsigned int m04;
    long long mLastTime;   // 0x08
    NetCounter_unk m10;
    NetCounter_unk m20;
    NetCounter_unk m30;
    NetCounter_unk m40;
};

NetStats_unk::NetStats_unk() : mPeriod(OS_TIMER_CLOCK_UNK), mLastTime(0) {}

void NetStats_unk::fn_8050E6CC(int ms) {
    mPeriod = ms * (OS_TIMER_CLOCK_UNK / 1000);
}

void NetStats_unk::fn_8050E6F4() {
    long long now = OSGetTime();
    if (now - mLastTime > (int)mPeriod) {
        m10.Update();
        m30.Update();
        m20.Update();
        m40.Update();
        mLastTime = now;
    }
}

// inferred: value per period, per tick, per millisecond or per second
float NetStats_unk::fn_8050E7DC(float value, int unit) {
    float result = 0.0f;
    switch (unit) {
    case 0:
        result = value;
        break;
    case 1:
        result = value / (int)mPeriod;
        break;
    case 2:
        result = value / ((long long)(int)mPeriod / (OS_TIMER_CLOCK_UNK / 1000));
        break;
    case 3:
        result = value / ((long long)(int)mPeriod / OS_TIMER_CLOCK_UNK);
        break;
    }
    return result;
}

// inferred: friend data of DWC (12 bytes)
struct DWCFriendData_unk {
    unsigned char m00[0xC];
};

// inferred: friend list (see MP/D2AMpMenuFriend.cpp: lbl_8087F628 + 0x430)
struct MpFriendList_unk {
    MpFriendList_unk();

    unsigned char mUserData[0x40];       // 0x000: DWC user data
    DWCFriendData_unk mFriends[32];      // 0x040
    wchar_t mName[16];                   // 0x1C0
    int m1E0;
    int m1E4;
    int m1E8;
    unsigned int m1EC;
    wchar_t mFriendNames[32][16];        // 0x1F0
    unsigned char mFriendKeys[32][16];   // 0x5F0
    unsigned char m7F0[0x1E0];
    unsigned int mHash;                  // 0x9D0
    unsigned char m9D4[0x9F8 - 0x9D4];
    unsigned long long mFriendKey;       // 0x9F8
    int mA00;
    int mA04;
};

MpFriendList_unk::MpFriendList_unk() {
    m1E0 = 1;
    m1E4 = 0;
    m1E8 = 0;
    m1EC = 0;
    mFriendKey = 0;
    mA04 = 0;
    mName[0] = 0;
    memset(m7F0, 0, sizeof(m7F0));
    memset(mFriendNames, 0, sizeof(mFriendNames));
    memset(mFriendKeys, 0, sizeof(mFriendKeys));
    for (int i = 0; i < 32; i++) {
        fn_800DD3FC(mFriendNames[i], L"Friend%02d", i + 1);
    }
}

extern "C" void DWC_CreateUserData(void* userData);
extern "C" void DWC_ReportUserData(void* userData);
extern "C" void DWC_ReportFriendData(void* userData, void* friendData);
extern "C" wchar_t* wcsncpy(wchar_t* dst, const wchar_t* src, unsigned long n);
extern "C" void* lbl_8087EEC8; // inferred: font
// inferred: wide to multibyte text with the font
extern "C" void fn_8006F420(void* font, char* out, const wchar_t* in, int size);
extern "C" const wchar_t* fn_8050F2F0(MpFriendList_unk* list, DWCFriendData_unk* data, unsigned char* status,
                                      int* state);

// inferred: creates new DWC user data and clears the friend list (debug report of every friend)
// NONMATCHING: ~98.6% - saved register numbering of the loop and the pool offset of L"" (0x18 in the
// original: the empty strings of fn_8050EBC8, not reconstructed, come first)
extern "C" int fn_8050E994(MpFriendList_unk* list) {
    DWC_CreateUserData(list);
    DWC_ReportUserData(list);
    memset(list->mFriends, 0, sizeof(list->mFriends));
    unsigned char status[0x110];
    char buf[0x200];
    DWCFriendData_unk* data = list->mFriends;
    for (int i = 0; i < 32; i++, data++) {
        if (DWC_IsValidFriendData(data)) {
            status[0] = 0;
            std::basic_string<wchar_t> text(fn_8050F2F0(list, data, status, 0));
            fn_8006F420(lbl_8087EEC8, buf, text.c_str(), 0x200);
            DWC_ReportFriendData(list, data);
        }
    }
    wcsncpy(list->mName, L"", 16);
    memset(list->m7F0, 0, sizeof(list->m7F0));
    return 1;
}

extern "C" int fn_8050EAE4(MpFriendList_unk* list) {
    return DWC_CheckDirtyFlag(list);
}

// inferred: marks the list as saved (hash of everything before mHash)
extern "C" int fn_8050EAE8(MpFriendList_unk* list) {
    list->m1E0 = 1;
    list->mHash = fn_800DC500(list, (char*)&list->mHash - (char*)list);
    return 1;
}

// inferred: nonzero when the list changed since fn_8050EAE8
extern "C" int fn_8050EB2C(MpFriendList_unk* list) {
    if (list->m1E0 >= 1) {
        return fn_800DC500(list, (char*)&list->mHash - (char*)list) != list->mHash;
    }
    return 0;
}

extern "C" int fn_8050EB88(MpFriendList_unk* list) {
    list->mFriendKey = DWC_CreateFriendKey(list);
    return list->mFriendKey != 0;
}

extern "C" int DWC_CanChangeFriendList();
extern "C" void DWC_DeleteBuddyFriendData(void* friendData);
extern "C" void DWC_ClearDirtyFlag(void* data);
// inferred: writes `size` bytes at `offset` of a save file (atn/NandManager.cpp)
extern "C" int fn_800A5218(const char* path, int offset, const void* data, int size);

// inferred: removes friend `id`; with `save`, writes the user data (if changed) and the friend list
extern "C" int fn_8050EF14(MpFriendList_unk* list, int id, int save) {
    if (!DWC_CanChangeFriendList()) {
        return 0;
    }
    DWC_DeleteBuddyFriendData(&list->mFriends[id]);
    if (save) {
        if (DWC_CheckDirtyFlag(list)) {
            DWC_ClearDirtyFlag(list);
            fn_800A5218("online/playerinfo7", 0, list, sizeof(list->mUserData));
        }
        fn_800A5218("online/playerinfo7", sizeof(list->mUserData), list->mFriends, 0x9B0);
    }
    return 1;
}

// inferred: first free friend slot (-1: none)
extern "C" int fn_8050F404(MpFriendList_unk* list) {
    for (int i = 0; i < 32; i++) {
        if (!DWC_IsValidFriendData(&list->mFriends[i])) {
            return i;
        }
    }
    return -1;
}

extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" void fn_806AC0A0(const char* keyValues, int arg); // inferred: DWC buddy status data

// NONMATCHING: 99.5% - pool offset of L"\\name\\" and the stack slots of the two string temporaries
// inferred: sets the own name (reported to the friends as "\name\<name>"); with `save`, writes the list
extern "C" void fn_8050EFCC(MpFriendList_unk* list, const wchar_t* name, int save) {
    if (wcscmp(list->mName, name) == 0) {
        return;
    }
    wcscpy(list->mName, name);
    if (save) {
        if (DWC_CheckDirtyFlag(list)) {
            DWC_ClearDirtyFlag(list);
            fn_800A5218("online/playerinfo7", 0, list, sizeof(list->mUserData));
        }
        fn_800A5218("online/playerinfo7", sizeof(list->mUserData), list->mFriends, 0x9B0);
    }
    std::basic_string<wchar_t> status(L"\\name\\");
    status += list->mName;
    char buf[0x100];
    fn_8006F420(lbl_8087EEC8, buf, status.c_str(), status.size() * 2);
    fn_806AC0A0(buf, 0);
}

// NONMATCHING: 99.8% - pool offset of L"\\name\\" (see fn_8050E994)
// inferred: reports the own name again
extern "C" void fn_8050F198(MpFriendList_unk* list) {
    std::basic_string<wchar_t> status(L"\\name\\");
    status += list->mName;
    char buf[0x100];
    fn_8006F420(lbl_8087EEC8, buf, status.c_str(), status.size() * 2);
    fn_806AC0A0(buf, 0);
}

// NONMATCHING: 99.4% - pool offsets of the texts (see fn_8050E994)
// inferred: state of a friend as text (and the raw state in `state`)
extern "C" const wchar_t* fn_8050F2F0(MpFriendList_unk* list, DWCFriendData_unk* data, unsigned char* status,
                                      int* state) {
    if (status != 0) {
        *status = 0;
    }
    if (data == 0) {
        return 0;
    }
    unsigned char s = fn_806ABCF0(data, status);
    if (state != 0) {
        *state = s;
    }
    switch (s) {
    case 0:
        return L"OFFLINE";
    case 1:
        return L"ONLINE";
    case 2:
        return L"PLAYING";
    case 3:
        return L"MATCH ANYBODY";
    case 4:
        return L"MATCH FRIEND";
    case 6:
        return L"MATCH SERVER";
    case 5:
        return L"MATCH CLIENT";
    }
    return 0;
}

extern "C" std::basic_string<wchar_t> fn_8050F470(unsigned long long key);

// inferred: friend key of the list as text
extern "C" std::basic_string<wchar_t> fn_8050F464(MpFriendList_unk* list) {
    return fn_8050F470(list->mFriendKey);
}

// inferred: friend key as text ("0000-0000-0000", dashes when there is none)
// NONMATCHING: 99.85% - only the offsets of the two strings in the wide string pool (the strings
// before them belong to functions that are not reconstructed, as for fn_8050F734)
extern "C" std::basic_string<wchar_t> fn_8050F470(unsigned long long key) {
    wchar_t buf[0x100];
    if (key != 0) {
        fn_800DD3FC(buf, L"%04d-%04d-%04d", (int)(key / 10000 / 10000 % 10000), (int)(key / 10000 % 10000),
                    (int)(key % 10000));
    } else {
        fn_800DD3FC(buf, L"----------------");
    }
    return std::basic_string<wchar_t>(buf);
}

static unsigned int lbl_8087F880;

// inferred: registered friend (friend key exchanged)
static inline bool __IsRegistered_unk(DWCFriendData_unk* data) {
    if (DWC_GetFriendDataType(data) == 3 && fn_806CF0E0(data) != 0) {
        return true;
    }
    return false;
}

// inferred: first friend slot (only registered ones when `registered` is 1)
// NONMATCHING: 94.0% - the slot pointer and the index swap r30/r31 and the found test is laid out
// as beq+b in the original. Tried: break + test after the loop, index initialised at declaration.
extern "C" int fn_8050F5A8(MpFriendList_unk* list, unsigned int registered) {
    unsigned int i;
    lbl_8087F880 = 0;
    for (i = 0; i < 32; i++) {
        lbl_8087F880++;
        if (registered != 1 || __IsRegistered_unk(&list->mFriends[i])) {
            if (DWC_IsValidFriendData(&list->mFriends[i])) {
                goto found;
            }
        }
    }
    i = -1;
found:
    return i;
}

// inferred: next friend slot
extern "C" int fn_8050F664(MpFriendList_unk* list, unsigned int registered) {
    for (unsigned int i = lbl_8087F880; i < 32; i++) {
        lbl_8087F880++;
        if (registered != 1 || __IsRegistered_unk(&list->mFriends[i])) {
            if (DWC_IsValidFriendData(&list->mFriends[i])) {
                return i;
            }
        }
    }
    return -1;
}

extern "C" wchar_t* fn_8050F724(MpFriendList_unk* list, int id) {
    return list->mFriendNames[id];
}

// NONMATCHING: 99.3% - only the offset of L"%s" in the wide string pool (the functions that use
// the strings before it are not reconstructed)
extern "C" void fn_8050F734(MpFriendList_unk* list, int id, const wchar_t* name) {
    fn_800DD3FC(list->mFriendNames[id], L"%s", name);
}

extern "C" int fn_8050F754(MpFriendList_unk* list, int id) {
    return DWC_GetGsProfileId(list, &list->mFriends[id]);
}

extern "C" unsigned long long fn_8050F764(MpFriendList_unk* list, int id) {
    unsigned long long key = fn_806D0030(&list->mFriends[id]);
    if (key == 0) {
        memcpy(&key, list->mFriendKeys[id], 8);
    }
    return key;
}

// inferred: state of a friend (0 offline, 1 online, 2 playing, 3 matching, -1 other)
extern "C" int fn_8050F7D8(MpFriendList_unk* list, int id) {
    switch (fn_806ABCF0(&list->mFriends[id], 0)) {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
    case 3:
    case 4:
    case 5:
        return 2;
    case 6:
        return 3;
    default:
        return -1;
    }
}


extern "C" const wchar_t* fn_8050F858(MpFriendList_unk* list, int id, unsigned char* out, int* arg) {
    return fn_8050F2F0(list, &list->mFriends[id], out, arg);
}

extern "C" int fn_8050F868(MpFriendList_unk* list, int id) {
    return DWC_GetFriendDataType(&list->mFriends[id]) == 3;
}

extern "C" int fn_8050F8A0(MpFriendList_unk* list, int id) {
    return fn_806CF160(&list->mFriends[id]);
}

static const char sLwMode[] = "lw_mode";
static const char sLwGroup[] = "lw_group";
static const char sLwLevel[] = "lw_level";
static const char sLwPenalty[] = "lw_penalty";
static const char sLwInvite[] = "lw_invite";
static const char* sLwKeys[] = {sLwMode, sLwGroup, sLwLevel, sLwPenalty, sLwInvite};

// inferred: key name of the matchmaking values
extern "C" const char* fn_8050F8B0(MpFriendList_unk* list, int index) {
    return sLwKeys[index];
}

// inferred: adjusts the penalty (+5 up to 128 on a disconnection, else -5, and -10 more every ten
// games)
extern "C" void fn_8050F8C4(MpFriendList_unk* list, unsigned int disconnected) {
    if (disconnected == 1) {
        list->m1E8 += 5;
        if (list->m1E8 > 0x80) {
            list->m1E8 = 0x80;
        }
        return;
    }
    list->m1EC++;
    if (list->m1EC % 10 == 0) {
        list->m1E8 -= 10;
    }
    list->m1E8 -= 5;
    if (list->m1E8 < 0) {
        list->m1E8 = 0;
    }
}
