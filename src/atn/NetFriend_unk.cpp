// inferred: network statistics timer (counters per second) and the friend list of the Wi-Fi
// Connection (DWC friend data, names and friend keys). Names invented. Not reconstructed:
// 0x8050E994 (DWC user data report, std::wstring), 0x8050EBC8/0x8050EF14/0x8050EFCC/0x8050F198
// (adding/removing friends), 0x8050F2F0 (friend name), 0x8050F404, 0x8050F464/0x8050F470 (friend
// key as text).

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
extern "C" unsigned char fn_806ABCF0(void* friendData, int arg); // inferred: online state
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

extern "C" const wchar_t* fn_8050F2F0(MpFriendList_unk* list, DWCFriendData_unk* data, void* out, int arg);

extern "C" const wchar_t* fn_8050F858(MpFriendList_unk* list, int id, void* out, int arg) {
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
