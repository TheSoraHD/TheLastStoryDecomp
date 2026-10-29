// inferred file name (splits-auto): HOME button menu glue on top of the HBM library, written after
// the SDK sample (HBMDataInfo setup by language, digital cursor, controller update, drawing).
// HBMRso* are C++ functions (mangled names in symbols.txt), declared in game/HBMRso.h.

#include "math/Vec.h"
#include "atn/FileList.h"
#include "atn/MemoryManager.h"
#include "game/HBMRso.h"
#include "atn/GraphicsDevice.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" char* strcat(char* dst, const char* src);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" unsigned char SCGetLanguage();
extern "C" unsigned long VIGetTvFormat();
extern "C" void GXSetColorUpdate(unsigned char enable);
extern "C" void C_MTXOrtho(float m[4][4], float t, float b, float l, float r, float n, float f);

extern "C" void* fn_800839EC(atn::MemoryManager* mgr, unsigned long size, int align, int heap, const char* name,
                             const char* file, int line, int arg); // inferred: allocation
extern "C" void* fn_8006BA8C(const char* path, unsigned long* size, int arg); // inferred: loads a file

// inferred: HBM library (unnamed in symbols.txt)
extern "C" void HBMCreate(HBMDataInfo* info);
extern "C" void HBMCreateSound(void* soundData, void* memBuf, unsigned long size);
extern "C" void HBMDelete();
extern "C" void HBMDeleteSound();
extern "C" void HBMInit();
extern "C" void fn_805C3ED0(int flag);                                        // HBMSetAdjustFlag
extern "C" void fn_805C3FB0();                                                // HBMUpdateSound
extern "C" int HBMCalc(HBMControllerData* con);
extern "C" int fn_805C3EA0();                                                 // inferred: selected button
extern "C" void fn_805C3E70();                                                // HBMDraw
extern "C" int WPADProbe(int chan, unsigned long* type);

extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;
extern "C" void fn_80071E04(atn::GraphicsDevice_unk* device);
extern "C" void fn_80076760(atn::GraphicsDevice_unk* device, int arg1, int arg2);

extern "C" unsigned char* lbl_8087F018; // inferred: input manager; KPAD buffers of 0xF00 bytes first

static const char* const sHbmDir_unk = "/hbm/HomeButton2";

extern "C" int fn_80440B24(int event, int arg) {
    return 0;
}

// inferred: InitHomeButtonInfo of the SDK sample
extern "C" void fn_80440B2C(HBMDataInfo* info) {
    char path[0x100];
    strcpy(path, sHbmDir_unk);
    info->region = SCGetLanguage();
    switch (info->region) {
    case 0:
        strcat(path, "/homeBtn.arc");
        break;
    case 1:
        strcat(path, "/homeBtn_ENG.arc");
        break;
    case 2:
        strcat(path, "/homeBtn_GER.arc");
        break;
    case 3:
        strcat(path, "/homeBtn_FRA.arc");
        break;
    case 4:
        strcat(path, "/homeBtn_SPA.arc");
        break;
    case 5:
        strcat(path, "/homeBtn_ITA.arc");
        break;
    case 6:
        strcat(path, "/homeBtn_NED.arc");
        break;
    case 7:
        strcat(path, "/homeBtn_CHN.arc");
        break;
    case 9:
        strcat(path, "/homeBtn_KOR.arc");
        break;
    default:
        info->region = 0;
        strcat(path, "/homeBtn.arc");
        break;
    }
    unsigned long size = 0;
    void* data = fn_8006BA8C(path, &size, 0);
    info->layoutBuf = fn_800839EC(atn::MemoryManager::Instance(), size, 0x20, 1, "", "", 0, 0);
    memcpy(info->layoutBuf, data, size);
    atn::FileFreeBuffer((char*)data, false);
    strcpy(path, sHbmDir_unk);
    strcat(path, "/SpeakerSe.arc");
    info->spkSeBuf = fn_8006BA8C(path, &size, 0);
    strcpy(path, sHbmDir_unk);
    strcat(path, "/home.csv");
    info->msgBuf = fn_8006BA8C(path, &size, 0);
    strcpy(path, sHbmDir_unk);
    strcat(path, "/config.txt");
    info->configBuf = fn_8006BA8C(path, &size, 0);
    info->configBufSize = size;
    info->sound_callback = fn_80440B24;
    info->backFlag = 0;
    info->cursor = 0;
    info->adjustX = 1.0f;
    info->adjustY = 1.0f;
    info->frameDelta = 1.0f;
    info->mem = 0;
    info->memSize = 0;
    info->pAllocator = atn::MemoryManager::Instance()->mSdk;
    info->messageFlag = 0;
}

// inferred: moves the cursor with the classic controller buttons; 0 if none is held
extern "C" int fn_80440DC0(unsigned long hold, float* pos) {
    switch (hold & 0xC003) {
    case 0x0001:
        pos[1] -= 0.0703125f;
        break;
    case 0x0002:
        pos[0] -= 0.0703125f;
        break;
    case 0x4000:
        pos[1] += 0.0703125f;
        break;
    case 0x8000:
        pos[0] += 0.0703125f;
        break;
    case 0x0003:
        pos[1] -= 0.049717966f;
        pos[0] -= 0.049717966f;
        break;
    case 0x8001:
        pos[1] -= 0.049717966f;
        pos[0] += 0.049717966f;
        break;
    case 0x4002:
        pos[1] += 0.049717966f;
        pos[0] -= 0.049717966f;
        break;
    case 0xC000:
        pos[1] += 0.049717966f;
        pos[0] += 0.049717966f;
        break;
    default:
        return 0;
    }
    pos[0] = pos[0] > 1.0f ? 1.0f : (pos[0] < -1.0f ? -1.0f : pos[0]);
    pos[1] = pos[1] > 1.0f ? 1.0f : (pos[1] < -1.0f ? -1.0f : pos[1]);
    return 1;
}

// inferred: creates the menu (HBMCreate, HBMCreateSound)
void HBMRsoCreate(HBMDataInfo& info, unsigned char*& soundData, unsigned char*& soundBuf) {
    int pal60 = VIGetTvFormat() == 1;
    int wide = lbl_8087EEE0->mWide;
    fn_80440B2C(&info);
    if (wide == 0) {
        info.adjustX = 1.0f;
        info.adjustY = 1.0f;
    } else {
        info.adjustX = 1.3684211f;
        info.adjustY = 1.0f;
    }
    if (pal60 != 0) {
        info.frameDelta = 2.4f;
    } else {
        info.frameDelta = 2.0f;
    }
    HBMCreate(&info);
    unsigned long size = 0;
    char path[0x40];
    strcpy(path, sHbmDir_unk);
    strcat(path, "/HomeButtonSe.arc");
    void* data = fn_8006BA8C(path, &size, 0);
    soundData = (unsigned char*)fn_800839EC(atn::MemoryManager::Instance(), size, 0x20, 1, "", "", 0, 0);
    memcpy(soundData, data, size);
    atn::FileFreeBuffer((char*)data, false);
    soundBuf = (unsigned char*)fn_800839EC(atn::MemoryManager::Instance(), 0x18700, 0x20, 1, "", "", 0, 0);
    HBMCreateSound(soundData, soundBuf, 0x18700);
}

// inferred: destroys the menu and frees its buffers
void HBMRsoDelete(HBMDataInfo& info, unsigned char*& soundData, unsigned char*& soundBuf) {
    HBMDelete();
    HBMDeleteSound();
    void* mem = info.mem;
    if (mem != 0) {
        atn::MemoryManager::Instance()->Free(mem);
    }
    atn::MemoryManager::Instance()->Free(info.layoutBuf);
    atn::MemoryManager::Instance()->Free(info.spkSeBuf);
    atn::FileFreeBuffer((char*)info.msgBuf, false);
    atn::FileFreeBuffer((char*)info.configBuf, false);
    atn::MemoryManager::Instance()->Free(soundData);
    atn::MemoryManager::Instance()->Free(soundBuf);
}

// inferred: opens the menu
void HBMRsoInit(HBMControllerData& con) {
    HBMInit();
    fn_805C3ED0(1);
    for (int i = 0; i < 4; i++) {
        con.wiiCon[i].posX = 0.0f;
        con.wiiCon[i].posY = 0.0f;
        con.wiiCon[i].use_devtype = 0;
    }
}

// inferred: clamps to [-1, 1]
inline float Clamp1_unk(float x) {
    return x > 1.0f ? 1.0f : (x < -1.0f ? -1.0f : x);
}

// inferred: updates the menu; returns the selected button or -1
// NONMATCHING: 99.8% - the original loads the stick x/y into f0/f1 (here f1/f0) and swaps the loop
// counter/offset registers (r28/r29); tried declaration orders and a single expression.
int HBMRsoCalc(HBMControllerData& con) {
    fn_805C3FB0();
    HBMKPadData* pad = con.wiiCon;
    int offset = 0;
    for (int i = 0; i < 4; i++, pad++, offset += 0xF00) {
        unsigned long type;
        int ret = WPADProbe(i, &type);
        pad->use_devtype = type;
        if ((ret >= -4 && ret <= -2) || ret == -7 || ret == 0) {
            pad->kpad = (KPADStatus_unk*)(lbl_8087F018 + offset);
            int digital = fn_80440DC0(pad->kpad->clHold, &pad->posX);
            float sy;
            float sx = pad->kpad->clStickX / 14.222222f;
            sy = pad->kpad->clStickY / 14.222222f;
            float x = Clamp1_unk(sx);
            float y = Clamp1_unk(sy);
            int analog;
            if (0.0f == x && 0.0f == y) {
                analog = 0;
            } else {
                pad->posX = Clamp1_unk(pad->posX + x);
                pad->posY = Clamp1_unk(pad->posY - y);
                analog = 1;
            }
            if ((digital | analog) == 0 && pad->kpad->dpd_valid_fg > 0) {
                pad->posX = pad->kpad->posX;
                pad->posY = pad->kpad->posY;
            }
        } else {
            pad->kpad = 0;
        }
    }
    int result;
    if (HBMCalc(&con) >= 0) {
        result = fn_805C3EA0();
    } else {
        result = -1;
    }
    return result;
}

// inferred: draws the menu
// NONMATCHING: 84.5% - the original stores the high words (0x4330) of the two int->float conversions
// at the start of the function; here they are stored next to the conversion (tried named locals,
// function-scope matrix, an inline returning the matrix).
void HBMRsoDraw() {
    atn::GraphicsDevice_unk* rs = lbl_8087EEE0;
    atn::Matrix44_unk proj;
    GXSetColorUpdate(1);
    if (rs->mWide == 0) {
        float scale = 1.0f;
        C_MTXOrtho(proj.m, 228.0f * scale, 228.0f * -scale, 304.0f * -scale, 304.0f * scale, 0.0f, 500.0f);
    } else {
        float scale = 1.0f;
        C_MTXOrtho(proj.m, 228.0f * scale, 228.0f * -scale, 416.0f * -scale, 416.0f * scale, 0.0f, 500.0f);
    }
    fn_80071D74(rs, proj.m);
    fn_805C3E70();
    fn_80071E04(rs);
    {
        atn::Matrix44_unk ortho;
        C_MTXOrtho(ortho.m, 0.0f, (float)rs->mHeight, 0.0f, (float)rs->mWidth, 0.0f, -100.0f);
        __Mtx44Copy(proj.m, ortho.m);
    }
    fn_80071D74(rs, proj.m);
    fn_80076760(rs, 6, 0);
    fn_80076760(rs, 8, 0);
    fn_80076760(rs, 9, 7);
    fn_80076760(rs, 10, 0);
    fn_80076760(rs, 4, 4);
    fn_80076760(rs, 5, 5);
}
