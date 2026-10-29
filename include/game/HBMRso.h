#ifndef GAME_HBMRSO_H
#define GAME_HBMRSO_H

// Glue between the game and the HOME button menu library (game/HomeButton_unk.cpp).
// The struct tags are the SDK ones (revolution/hbm/HBMTypes.h): the mangled names in symbols.txt
// (HBMRsoCreate__FR11HBMDataInfoRPUcRPUc, HBMRsoCalc__FR17HBMControllerData) need them.
// Game units do not include SDK headers, so only the layout is repeated here.

// inferred: KPADStatus of the SDK, only the fields read by the glue
struct KPADStatus_unk {
    unsigned char m00[0x20];
    float posX; // 0x20
    float posY; // 0x24
    unsigned char m28[0x5E - 0x28];
    signed char dpd_valid_fg; // 0x5E
    unsigned char m5F[0x60 - 0x5F];
    unsigned long clHold; // 0x60: classic controller buttons
    unsigned char m64[0x6C - 0x64];
    float clStickX; // 0x6C
    float clStickY; // 0x70
};

// SDK HBMDataInfo (0x40 bytes)
struct HBMDataInfo {
    void* layoutBuf;                 // 0x00
    void* spkSeBuf;                  // 0x04
    void* msgBuf;                    // 0x08
    void* configBuf;                 // 0x0C
    void* mem;                       // 0x10
    int (*sound_callback)(int, int); // 0x14
    int backFlag;                    // 0x18
    int region;                      // 0x1C
    int cursor;                      // 0x20
    int messageFlag;                 // 0x24
    unsigned long configBufSize;     // 0x28
    unsigned long memSize;           // 0x2C
    float frameDelta;                // 0x30
    float adjustX;                   // 0x34
    float adjustY;                   // 0x38
    void* pAllocator;                // 0x3C
};

// SDK HBMKPadData / HBMControllerData (0x40 bytes)
struct HBMKPadData {
    KPADStatus_unk* kpad;      // 0x00
    float posX;                // 0x04
    float posY;                // 0x08
    unsigned long use_devtype; // 0x0C
};
struct HBMControllerData {
    HBMKPadData wiiCon[4];
};

void HBMRsoCreate(HBMDataInfo& info, unsigned char*& soundData, unsigned char*& soundBuf); // 0x80440F64
void HBMRsoDelete(HBMDataInfo& info, unsigned char*& soundData, unsigned char*& soundBuf); // 0x804410C0
void HBMRsoInit(HBMControllerData& con);                                                   // 0x80441170
int HBMRsoCalc(HBMControllerData& con); // 0x804411DC: selected button, or < 0 while open
void HBMRsoDraw();                      // 0x804413D4

#endif
