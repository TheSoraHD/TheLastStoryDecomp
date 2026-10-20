#include "atn/Csv.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int

// inferred: resource manager (lbl_8087F518): loads a whole file
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8006BB6C(void* data, int arg); // inferred: releases a loaded file

// inferred: chase parameters of the global object lbl_8087F0A8 (only the members set here;
// names from the CSV keys)
struct ChaseConfig_unk {
    char m000[0x4E0];
    float mVisChaseDistance; // 0x4E0
    float mVisSearchDistance; // 0x4E4
    float mVisAngle; // 0x4E8
    float mHearWalk; // 0x4EC
    float mHearRun; // 0x4F0
    float mCatchDistance; // 0x4F4
    float mScanRate; // 0x4F8
    float mWalkSpeed; // 0x4FC
    float mRunSpeed; // 0x500
    float mLostSpeed; // 0x504
    float mReturnSpeed; // 0x508
    int mLoseTime; // 0x50C
    int mFindTime; // 0x510
    int mChaseDelay; // 0x514
    int mCatchDelay; // 0x518
    int mWaitTime; // 0x51C
    float mCatchDamage; // 0x520
    int mReleaseSwingCount; // 0x524
    int mCapturedTime; // 0x528
    char m52C[0x14];
    float mChWalkSpeed; // 0x540
    float mChMinSpeed; // 0x544
    float mChMaxSpeed; // 0x548
    float mChAcceleration; // 0x54C
    int mChRunToWalkFrames; // 0x550
    float mChRunToWalkDistance; // 0x554
    float mChSeeRunDistance; // 0x558
    float mChMinRunDistance; // 0x55C
    float mChRunStopDistance; // 0x560
    float mChCatchDistance; // 0x564
    int mChCatchRest; // 0x568
    char m56C[0x8];
    float mChDeceleration; // 0x574
    int mChDecelerationFrames; // 0x578
    int mChRunnerHit; // 0x57C
    int mChPlayerHit; // 0x580
};
extern "C" ChaseConfig_unk* lbl_8087F0A8;

// inferred: accessor of the global object (the original loads the pointer before the call that
// reads the value)
static inline ChaseConfig_unk* Config_unk() { return lbl_8087F0A8; }

#define DEG_TO_RAD 0.017453292f

// inferred: loads /boot/chase_param.csv (one "key,value" row per parameter)
// NONMATCHING: 97.94% - register allocation only (data/zero and name/config pointer swapped; the
// original keeps the high half of the string pool in r29 from the first call); tried the order of
// the declarations of path, size, data and name
extern "C" void fn_8020A90C() {
    const char* path = "/boot/chase_param.csv";
    unsigned long size = 0;
    void* data = fn_8046DC5C(lbl_8087F518, path, &size, 0x20);
    atn::Csv csv((const char*)data, size);
    while (csv.fn_8005BBF8() != 0) {
        const char* name = csv.fn_8005B9CC();
        if (strcmp(name, "visSearchDistance") == 0) {
            Config_unk()->mVisSearchDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "visChaseDistance") == 0) {
            Config_unk()->mVisChaseDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "visAngle") == 0) {
            lbl_8087F0A8->mVisAngle = DEG_TO_RAD * fn_800DC288(csv.fn_8005B9CC());
        }
        if (strcmp(name, "hearWalk") == 0) {
            Config_unk()->mHearWalk = fn_800DC288(csv.fn_8005B9CC());
        }
        if (strcmp(name, "hearRun") == 0) {
            Config_unk()->mHearRun = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "catchDistance") == 0) {
            Config_unk()->mCatchDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "scanRate") == 0) {
            lbl_8087F0A8->mScanRate = DEG_TO_RAD * fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "walkSpeed") == 0) {
            Config_unk()->mWalkSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "runSpeed") == 0) {
            Config_unk()->mRunSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "lostSpeed") == 0) {
            Config_unk()->mLostSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "returnSpeed") == 0) {
            Config_unk()->mReturnSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chaseDelay") == 0) {
            Config_unk()->mChaseDelay = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "loseTime") == 0) {
            Config_unk()->mLoseTime = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "findTime") == 0) {
            Config_unk()->mFindTime = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "waitTime") == 0) {
            Config_unk()->mWaitTime = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "catchDelay") == 0) {
            Config_unk()->mCatchDelay = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "catchDamage") == 0) {
            Config_unk()->mCatchDamage = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "releaseSwingCount") == 0) {
            Config_unk()->mReleaseSwingCount = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "capturedTime") == 0) {
            Config_unk()->mCapturedTime = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chWalkSpeed") == 0) {
            Config_unk()->mChWalkSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chMinSpeed") == 0) {
            Config_unk()->mChMinSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chMaxSpeed") == 0) {
            Config_unk()->mChMaxSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chRunToWalkFrames") == 0) {
            Config_unk()->mChRunToWalkFrames = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chRunToWalkDistance") == 0) {
            Config_unk()->mChRunToWalkDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chSeeRunDistance") == 0) {
            Config_unk()->mChSeeRunDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chMinRunDistance") == 0) {
            Config_unk()->mChMinRunDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chRunStopDistance") == 0) {
            Config_unk()->mChRunStopDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chCatchDistance") == 0) {
            Config_unk()->mChCatchDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chCatchRest") == 0) {
            Config_unk()->mChCatchRest = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chAcceleration") == 0) {
            Config_unk()->mChAcceleration = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chDeceleration") == 0) {
            Config_unk()->mChDeceleration = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chDecelerationFrames") == 0) {
            Config_unk()->mChDecelerationFrames = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chRunnerHit") == 0) {
            Config_unk()->mChRunnerHit = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chPlayerHit") == 0) {
            Config_unk()->mChPlayerHit = fn_800DC12C(csv.fn_8005B9CC());
        }
    }
    fn_8006BB6C(data, 0);
}
