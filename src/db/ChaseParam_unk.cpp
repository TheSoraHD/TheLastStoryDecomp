#include "atn/Csv.h"
#include "atn/FileList.h"
#include "game/GameConfig_unk.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int

// inferred: resource manager (lbl_8087F518): loads a whole file
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);

// inferred: chase parameters of the global object lbl_8087F0A8 (only the members set here;
// names from the CSV keys)
extern "C" GameConfig_unk* lbl_8087F0A8;

// inferred: accessor of the global object (the original loads the pointer before the call that
// reads the value)
static inline GameConfig_unk* Config_unk() { return lbl_8087F0A8; }

#define DEG_TO_RAD 0.017453292f

// inferred: loads /boot/chase_param.csv (one "key,value" row per parameter)
// NONMATCHING: 99.05% - register allocation only: the original gives r31 to the zero constant (later
// the string pool) and r30 to data (here swapped). The int copy of the size fixed the name/config
// registers. Tried: declaration orders of path/size/data/len, literal path, casts, for(;;) loop,
// releasing csv.m04, explicit zero local, int size.
extern "C" void fn_8020A90C() {
    const char* path = "/boot/chase_param.csv";
    unsigned long size = 0;
    void* data = fn_8046DC5C(lbl_8087F518, path, &size, 0x20);
    int len = size;
    atn::Csv csv((const char*)data, len);
    while (csv.fn_8005BBF8() != 0) {
        const char* name = csv.fn_8005B9CC();
        if (strcmp(name, "visSearchDistance") == 0) {
            Config_unk()->m4DC.mVisSearchDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "visChaseDistance") == 0) {
            Config_unk()->m4DC.mVisChaseDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "visAngle") == 0) {
            lbl_8087F0A8->m4DC.mVisAngle = DEG_TO_RAD * fn_800DC288(csv.fn_8005B9CC());
        }
        if (strcmp(name, "hearWalk") == 0) {
            Config_unk()->m4DC.mHearWalk = fn_800DC288(csv.fn_8005B9CC());
        }
        if (strcmp(name, "hearRun") == 0) {
            Config_unk()->m4DC.mHearRun = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "catchDistance") == 0) {
            Config_unk()->m4DC.mCatchDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "scanRate") == 0) {
            lbl_8087F0A8->m4DC.mScanRate = DEG_TO_RAD * fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "walkSpeed") == 0) {
            Config_unk()->m4DC.mWalkSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "runSpeed") == 0) {
            Config_unk()->m4DC.mRunSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "lostSpeed") == 0) {
            Config_unk()->m4DC.mLostSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "returnSpeed") == 0) {
            Config_unk()->m4DC.mReturnSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chaseDelay") == 0) {
            Config_unk()->m4DC.mChaseDelay = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "loseTime") == 0) {
            Config_unk()->m4DC.mLoseTime = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "findTime") == 0) {
            Config_unk()->m4DC.mFindTime = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "waitTime") == 0) {
            Config_unk()->m4DC.mWaitTime = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "catchDelay") == 0) {
            Config_unk()->m4DC.mCatchDelay = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "catchDamage") == 0) {
            Config_unk()->m4DC.mCatchDamage = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "releaseSwingCount") == 0) {
            Config_unk()->m4DC.mReleaseSwingCount = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "capturedTime") == 0) {
            Config_unk()->m4DC.mCapturedTime = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chWalkSpeed") == 0) {
            Config_unk()->m53C.mChWalkSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chMinSpeed") == 0) {
            Config_unk()->m53C.mChMinSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chMaxSpeed") == 0) {
            Config_unk()->m53C.mChMaxSpeed = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chRunToWalkFrames") == 0) {
            Config_unk()->m53C.mChRunToWalkFrames = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chRunToWalkDistance") == 0) {
            Config_unk()->m53C.mChRunToWalkDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chSeeRunDistance") == 0) {
            Config_unk()->m53C.mChSeeRunDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chMinRunDistance") == 0) {
            Config_unk()->m53C.mChMinRunDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chRunStopDistance") == 0) {
            Config_unk()->m53C.mChRunStopDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chCatchDistance") == 0) {
            Config_unk()->m53C.mChCatchDistance = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chCatchRest") == 0) {
            Config_unk()->m53C.mChCatchRest = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chAcceleration") == 0) {
            Config_unk()->m53C.mChAcceleration = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chDeceleration") == 0) {
            Config_unk()->m53C.mChDeceleration = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "chDecelerationFrames") == 0) {
            Config_unk()->m53C.mChDecelerationFrames = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chRunnerHit") == 0) {
            Config_unk()->m53C.mChRunnerHit = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(name, "chPlayerHit") == 0) {
            Config_unk()->m53C.mChPlayerHit = fn_800DC12C(csv.fn_8005B9CC());
        }
    }
    atn::FileFreeBuffer((char*)data, false);
}
