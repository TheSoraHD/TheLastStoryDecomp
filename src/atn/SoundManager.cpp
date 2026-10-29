#include "atn/SoundRef.h"
#include "atn/SoundManager.h"
#include "atn/DrawHelper.h"
#include "math/Vec.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

extern "C" void fn_80709F20(void*);
extern "C" int fn_8070ABE0(void*);
extern "C" void fn_80709AD0(void*, int);
extern "C" void fn_8070AD70(void*, float);
extern "C" char lbl_807C7030[];

extern "C" atn::SoundEngine_unk* lbl_8087EE90;
// inferred: functions of the audio device (atn/AudioDevice.cpp)
extern "C" void fn_80047C74(atn::SoundEngine_unk* engine);
extern "C" void fn_80047D20(atn::SoundEngine_unk* engine);
extern "C" int fn_800499D0(atn::SoundEngine_unk* engine);               // output mode
extern "C" void fn_800499F4(atn::SoundEngine_unk* engine, int mode);
extern "C" float fn_80049A28(atn::SoundEngine_unk* engine);             // master volume
extern "C" void fn_80049AAC(atn::SoundEngine_unk* engine, float volume);
extern "C" int fn_80049B2C(atn::SoundEngine_unk* engine, unsigned int id); // the sound is a stream
extern "C" void fn_8004A388(atn::SoundEngine_unk* engine, int compressor);
extern "C" void fn_8004A660(atn::SoundEngine_unk* engine);
// inferred: nw4r::snd::Sound3DListener setters
extern "C" void fn_80715D60(nw4r::snd::Sound3DListener* listener, float size);     // interior size
extern "C" void fn_80715D70(nw4r::snd::Sound3DListener* listener, float distance); // max volume distance
extern "C" void fn_80715D80(nw4r::snd::Sound3DListener* listener, float distance); // unit distance
// inferred: debug menu entries
extern "C" void fn_80087994(void* menu, const char* name, int* value, int callback, int arg);
extern "C" void fn_800874C8(void* menu, const char* name, int* value, int min, int max, int step, int callback,
                            int arg);
extern "C" void fn_8008771C(void* menu, const char* name, float* value, int callback, int arg, float min, float max,
                            float step);
// inferred: debug drawing (atn::DrawHelper)
extern "C" void fn_80060D58(atn::DrawHelper* draw, unsigned int color, float x, float y, float z, float w, float h);
extern "C" void fn_800616C0(atn::DrawHelper* draw, float x, float y, float z, float w, float h, float angle,
                            const char* text, unsigned int color, int arg6, int arg7, int arg8);
extern "C" void fn_80063D3C(atn::DrawHelper* draw, const atn::Vector3* pos, unsigned int color, float radius,
                            float arg);
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" void* lbl_8087EFB4;
extern "C" int sprintf(char* buf, const char* fmt, ...);

namespace atn {

SoundRef::SoundRef() : mPtr(0) {}

SoundRef::SoundRef(const SoundRef& other) : mPtr(0) {
    *this = other;
}

SoundRef::~SoundRef() {
    Reset();
}

// NONMATCHING: 99.33% - difiere la asignacion de registros del contador de referencias.
SoundRef& SoundRef::operator=(SoundObject* p) {
    Reset();
    mPtr = p;
    if (mPtr != 0) {
        ++mPtr->mRefCount;
    }
    return *this;
}

// NONMATCHING: 97.81% - Reset() inline asigna registros distintos.
SoundRef& SoundRef::operator=(const SoundRef& other) {
    Reset();
    mPtr = other.mPtr;
    if (mPtr != 0) {
        ++mPtr->mRefCount;
    }
    return *this;
}

void SoundRef::Reset() {
    if (mPtr != 0) {
        --mPtr->mRefCount;
        mPtr = 0;
    }
}
} // namespace atn

extern "C" int fn_800CB4A4(atn::SoundRef* self) {
    return self->mPtr != 0 ? ((self->mPtr->m9C >> 3) & 1) : 0;
}

extern "C" bool fn_800CB4C4(atn::SoundRef* self) {
    if (self->mPtr != 0) {
        return self->mPtr->mA0 != 0;
    }
    return false;
}

extern "C" int fn_800CB4EC(atn::SoundRef* self) {
    return self->mPtr != 0 ? reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA028() : 0;
}

extern "C" void fn_800CB504(atn::SoundRef* self) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA098();
}

extern "C" void fn_800CB518(atn::SoundRef* self) {
    if (self->mPtr != 0 && self->mPtr->mSound != 0) fn_80709F20(self->mPtr->mSound);
}

// NONMATCHING: 90.48% - comparacion > 0 del resultado del ternario.
extern "C" bool fn_800CB538(atn::SoundRef* self) {
    if (self->mPtr != 0) {
        return (self->mPtr->mSound != 0 ? fn_8070ABE0(self->mPtr->mSound) : 0) > 0;
    }
    return false;
}

extern "C" bool fn_800CB58C(atn::SoundRef* self) {
    if (self->mPtr != 0) {
        return self->mPtr->mSound != 0;
    }
    return false;
}

extern "C" void fn_800CB5B4(atn::SoundRef* self, float a1, int a2) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA0AC(a1, a2);
}

extern "C" void fn_800CB5C8(atn::SoundRef* self, int frames, int detach) {
    if (self->mPtr != 0 && self->mPtr->mSound != 0) {
        fn_80709AD0(self->mPtr->mSound, frames);
    }
    if (detach == 0) self->Reset();
}

extern "C" void fn_800CB640(atn::SoundRef* self, int a1, unsigned long a2, int a3) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA1D4(a1, a2, a3);
}

extern "C" void fn_800CB654(atn::SoundRef* self, float a1, float a2, float a3) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA2FC(a1, a2, a3);
}

extern "C" void fn_800CB668(atn::SoundRef* self, float value) {
    if (self->mPtr != 0 && self->mPtr->mSound != 0) fn_8070AD70(self->mPtr->mSound, value);
}

extern "C" void fn_800CB688(atn::SoundRef* self, int a1, float a2) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA42C(a2, a1);
}

extern "C" void fn_800CB69C(atn::SoundRef* self, int a1, float a2) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA5E8(a2, a1);
}

extern "C" void fn_800CB6B0(atn::SoundRef* self, int a1, float a2) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA798(a1, a2);
}

extern "C" void* fn_800CB6C4(atn::SoundRef* self) {
    return self->mPtr != 0 ? reinterpret_cast<char*>(self->mPtr) + 0x70 : lbl_807C7030;
}

extern "C" void fn_800CB6E4(atn::SoundRef* self, const void* value) {
    if (self->mPtr != 0) reinterpret_cast<atn::SoundCore*>(self->mPtr)->fn_800CA7E8(static_cast<const atn::Vector3*>(value));
}

extern "C" int fn_800CB6F8(atn::SoundRef* self) {
    return self->mPtr != 0 ? self->mPtr->m98 : 0;
}


// inferred: operations applied to the sounds through boost::bind (their type names are in the DOL,
// e.g. "void (*)(atn::SoundCore &, int)")
extern "C" void fn_800CB714(atn::SoundCore& sound, int pause, unsigned int mask, int frames) {
    sound.fn_800CA1D4(pause, mask, frames);
}

// inferred: stops the sound (unless it is still referenced and onlyUnused is set)
extern "C" void fn_800CB718(atn::SoundCore& sound, int frames, int onlyUnused) {
    if (!onlyUnused || sound.mA4 <= 0) {
        sound.mHandle.Stop(frames);
        sound.fn_800C82A0(frames <= 0);
    }
}

// inferred: stops the sound if it is playing (m9C bit 3)
extern "C" void fn_800CB788(atn::SoundCore& sound, int frames) {
    if ((sound.m9C & 8) == 8) {
        sound.mHandle.Stop(frames);
        sound.fn_800C82A0(frames <= 0);
    }
}

extern "C" void fn_800CB7F0(atn::SoundCore& sound, int frames) {
    sound.fn_800CA144(frames);
}

extern "C" void fn_800CB7F4(atn::SoundCore& sound, int frames) {
    sound.fn_800CA514(frames);
}

extern "C" void fn_800CB7F8(atn::SoundCore& sound, int frames) {
    sound.fn_800CA6C4(frames);
}

extern "C" void fn_800CB7FC(atn::SoundCore& sound) {
    sound.fn_800CA3D4();
}



using atn::SoundCategory_unk;
using atn::SoundManager;

// inferred: callback of the debug menu entries of a player
// NONMATCHING: 99.9% - intercambio de dos registros.
extern "C" void fn_800CB800(SoundCategory_unk* param) {
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->SetPlayerMute_unk(param->m00, param->mMute, 0);
        lbl_8087EFE8->fn_800D0240(param->m00, param->mVolume, 0);
        lbl_8087EFE8->SetPlayerPitch_unk(param->m00, param->mPitch, 0);
        lbl_8087EFE8->SetPlayerLpf_unk(param->m00, param->mPan, 0);
        lbl_8087EFE8->fn_800D0684(param->m00, param->mFxSend);
    }
}

// inferred: callback of the debug menu entries of a category
// NONMATCHING: 99.9% - intercambio de dos registros.
extern "C" void fn_800CBE8C(SoundCategory_unk* param) {
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->SetCategoryMute_unk(param->m00, param->mMute, 0);
        lbl_8087EFE8->fn_800D03AC(param->m00, param->mVolume, 0);
        lbl_8087EFE8->SetCategoryPitch_unk(param->m00, param->mPitch, 0);
        lbl_8087EFE8->SetCategoryLpf_unk(param->m00, param->mPan, 0);
        lbl_8087EFE8->SetCategoryFxSend_unk(param->m00, param->mFxSend);
    }
}

// inferred: callback of the debug menu entries of a scope
// NONMATCHING: 99.9% - intercambio de dos registros.
extern "C" void fn_800CC518(SoundCategory_unk* param) {
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->SetScopeMute_unk(param->m00, param->mMute, 0);
        lbl_8087EFE8->fn_800D0518(param->m00, param->mVolume, 0);
        lbl_8087EFE8->SetScopePitch_unk(param->m00, param->mPitch, 0);
        lbl_8087EFE8->SetScopeLpf_unk(param->m00, param->mPan, 0);
        lbl_8087EFE8->SetScopeFxSend_unk(param->m00, param->mFxSend);
    }
}

// inferred: creates the sound manager
extern "C" void fn_800CCBA4() {
    if (lbl_8087EFE8 == 0) {
        lbl_8087EFE8 = new (10, "", "", 0) SoundManager();
    }
}

namespace atn {

SoundManager::SoundManager()
    : mNext(0), mOutputMode(0), mMasterVolume(1.0f), mEnabled(1), mStreamEnabled(1), m3dEnabled(1),
      mMaxVolumeDistance(20.0f), mUnitDistance(80.0f), mInteriorSize(160.0f), mFxEnabled(1), mFx2dEnabled(0),
      m34B8(1), mCompressor(0), mDupLimitFrame(1), mMinimumVolume(0.05f), m34C8(1), mRandomPitch(0.4f), m34D0(0),
      m34D8(0), mDebugLevel(0) {
    mOutputMode = fn_800499D0(lbl_8087EE90);
    mMasterVolume = fn_80049A28(lbl_8087EE90);
    SetPlayerParam_unk(SoundCategory_unk(0));
    SetPlayerParam_unk(SoundCategory_unk(1));
    SetPlayerParam_unk(SoundCategory_unk(2));
    fn_800CFDF0(SoundCategory_unk(0));
    fn_800CFFB8(SoundCategory_unk(0));
    fn_800D1430();
}

// NONMATCHING: 99.21% - falta el doble beq del destructor de FixedArray_unk.
SoundManager::~SoundManager() {
    fn_800CF45C(0, 0);
}

void SoundManager::fn_800CDF84() {
    fn_800CF45C(0, 0);
    lbl_8087EE90->m408 = mFxEnabled;
    fn_80047C74(lbl_8087EE90);
    for (int i = 0; i < 8; i++) {
        mDefPlayers[i] = mPlayers[i];
    }
    for (int i = 0; i < 32; i++) {
        mDefCategories[i] = mCategories[i];
    }
    for (int i = 0; i < 16; i++) {
        mDefScopes[i] = mScopes[i];
    }
}

// inferred: copies the parameters except the mute flags
inline void RestoreParam_unk(SoundCategory_unk& dst, const SoundCategory_unk& src) {
    dst.m00 = src.m00;
    dst.mVolume = src.mVolume;
    dst.mPitch = src.mPitch;
    dst.mPan = src.mPan;
    dst.mFxSend = src.mFxSend;
}

void SoundManager::fn_800CE368() {
    fn_800CF45C(0, 0);
    fn_80047D20(lbl_8087EE90);
    for (int i = 0; i < 8; i++) {
        RestoreParam_unk(mPlayers[i], mDefPlayers[i]);
    }
    for (int i = 0; i < 32; i++) {
        RestoreParam_unk(mCategories[i], mDefCategories[i]);
    }
    for (int i = 0; i < 16; i++) {
        RestoreParam_unk(mScopes[i], mDefScopes[i]);
    }
}

// NONMATCHING: 98.81% - numeracion de registros en erase().
void SoundManager::fn_800CE6E0() {
    fn_800499F4(lbl_8087EE90, mOutputMode);
    fn_80049AAC(lbl_8087EE90, mMasterVolume);
    fn_8004A388(lbl_8087EE90, mCompressor);
    fn_80715D70(&mListener.mListener, mMaxVolumeDistance);
    fn_80715D80(&mListener.mListener, mUnitDistance);
    fn_80715D60(&mListener.mListener, mInteriorSize);
    SoundCore* sound = mSounds;
    for (int i = 0; i < 32; i++, sound++) {
        sound->fn_800C82FC();
        if (!sound->fn_800C8278() && sound->mA4 <= 0 && sound->mE8 == 0 && sound->fn_800CA844() < mMinimumVolume) {
            sound->mHandle.Stop(0);
        }
    }
    SoundLogEntry_unk* it = mLog.begin();
    while (it != mLog.end()) {
        it->mAge++;
        it->mVolume = it->mSound->fn_800CA844();
        if (it->mAge >= 120) {
            mLog.erase(it);
        } else if (it->mSound != 0 && (it->mSound->fn_800C8278() || it->mId != it->mSound->m98)) {
            mLog.erase(it);
        } else {
            it++;
        }
    }
    if (mDebugLevel > 0 && mDebugLevel != 7) {
        fn_8004A660(lbl_8087EE90);
    }
}

} // namespace atn

// inferred: volume bar of a sound in the debug display
// NONMATCHING: 99.17% - orden de carga de floats.
extern "C" void fn_800CE9A4(atn::SoundCore* sound, float x, float y, float z, float w, float h) {
    if (sound->fn_800C8278() || sound->mHandle.mSound == 0) {
        return;
    }
    unsigned int color = 0x66FF8800;
    if (sound->mA8 != 0) {
        switch (sound->mA8->m00) {
        case 0:
            break;
        case 1:
            color = 0x660088FF;
            break;
        case 2:
            color = 0x668800FF;
            break;
        default:
            color = 0x668800FF;
            break;
        }
    }
    fn_80060D58(lbl_8087EEB0, 0x66888888, x, y, z, w, h);
    atn::DrawHelper* draw = lbl_8087EEB0;
    fn_80060D58(draw, color, x, y, z, w * sound->fn_800CACEC(), h);
    char buf[0x100];
    sprintf(buf, "%.2f%%", 100.0f * sound->fn_800CACEC());
    fn_800616C0(lbl_8087EEB0, 1.0f + (2.0f + x), 1.0f + y, z, h, h, 0.0f, buf, 0xCC000000, 1, 0, 0);
    fn_800616C0(lbl_8087EEB0, 2.0f + x, y, z, h, h, 0.0f, buf, 0xCCFFFFFF, 1, 0, 0);
    if (sound->fn_800CA028()) {
        fn_800616C0(lbl_8087EEB0, 1.0f + (5.0f + (x + w)), 1.0f + y, z, h, h, 0.0f, "Loading...", 0xCC000000, 1, 0, 0);
        fn_800616C0(lbl_8087EEB0, 5.0f + (x + w), y, z, h, h, 0.0f, "Loading...", 0xCCFFFFFF, 1, 0, 0);
    } else {
        int length = sound->fn_800CA910();
        sprintf(buf, "%6d / %6d\0", sound->fn_800CAA88(), length);
        fn_800616C0(lbl_8087EEB0, 1.0f + (5.0f + (x + w)), 1.0f + y, z, h, h, 0.0f, buf, 0xCC000000, 1, 0, 0);
        fn_800616C0(lbl_8087EEB0, 5.0f + (x + w), y, z, h, h, 0.0f, buf, 0xCCFFFFFF, 1, 0, 0);
    }
}

namespace atn {

// NONMATCHING: 90.72% - asignacion de registros de coordenadas.
void SoundManager::fn_800CECF0() {
    if (lbl_8087EEB0 == 0) {
        return;
    }
    float x = 10.0f;
    float z = -1000.0f;
    float h = 13.0f;
    char buf[0x100];
    if (mDebugLevel == 1) {
        float y = 20.0f;
        for (SoundLogEntry_unk* it = mLog.begin(); it != mLog.end(); it++) {
            fn_800616C0(lbl_8087EEB0, x, y, z, h, h, 0.0f, it->mText.mText, -1, 1, 1, 0);
            y += h;
        }
    }
    if (mDebugLevel == 2 || mDebugLevel == 3) {
        float y = 20.0f;
        for (int i = 0; i < 32; i++) {
            SoundCore& sound = mSounds[i];
            if (!sound.fn_800C8278()) {
                if ((sound.m9C & 2) == 2) {
                    Vector3 pos = sound.m08.mActor.mPosition;
                    float span = sound.fn_800CA8E0();
                    float pan = sound.fn_800CA8B0();
                    float volume = sound.fn_800CA844();
                    sprintf(buf, "[%s] Pos(%.2f, %.2f, %.2f) Vol:%.2f Pan:%.2f SPan:%.2f", sound.fn_800CA834(), pos.x,
                            pos.y, pos.z, volume, pan, span);
                } else {
                    float volume = sound.fn_800CA844();
                    sprintf(buf, "[%s] Vol:%.2f", sound.fn_800CA834(), volume);
                }
                fn_800616C0(lbl_8087EEB0, x, y, z, h, h, 0.0f, buf, -1, 1, 1, 0);
                y += h;
            }
        }
    }
    if (mDebugLevel == 3) {
        for (int i = 0; i < 32; i++) {
            SoundCore& sound = mSounds[i];
            if (!sound.fn_800C8278() && (sound.m9C & 2) == 2) {
                Vector3 screen;
                fn_800BFAC8(&screen, lbl_8087EFB4, &sound.m08.mActor.mPosition);
                if (screen.z >= 0.0f && 1.0f >= screen.z) {
                    unsigned int color = 0xFF00FFFF;
                    if ((sound.mHandle.mSound != 0 ? fn_8070ABE0(sound.mHandle.mSound) : 0) > 0) {
                        color = 0xFFFFFF00;
                    }
                    if (sound.mHandle.mSound == 0 && sound.mA4 > 0) {
                        color = 0xFFFF0000;
                    }
                    fn_80063D3C(lbl_8087EEB0, &sound.m08.mActor.mPosition, color, 2.0f, 0.0f);
                    fn_800616C0(lbl_8087EEB0, screen.x, screen.y, 0.0f, 12.0f, 12.0f, 0.0f, sound.fn_800CA834(), -1, 1,
                                1, 0);
                }
            }
        }
    }
    if (mDebugLevel == 4 || mDebugLevel == 8) {
        float y = 20.0f;
        for (int i = 0; i < 32; i++) {
            SoundCore& sound = mSounds[i];
            unsigned int color;
            if (sound.fn_800C8278()) {
                sprintf(buf, "%02d: None\0", i);
                color = 0xCC7F7F7F;
            } else {
                int scope = sound.mB0 != 0 ? sound.mB0->m00 : -1;
                int category = sound.mAC != 0 ? sound.mAC->m00 : -1;
                int player = sound.mA8 != 0 ? sound.mA8->m00 : -1;
                float volume = sound.fn_800CA844();
                sprintf(buf, "%02d: [%s] Vol:%.2f, Ply:%d, Ctg:%d Scp:%d\0", i, sound.fn_800CA834(), volume, player,
                        category, scope);
                color = 0xCCFFFFFF;
            }
            fn_800616C0(lbl_8087EEB0, 1.0f + x, 1.0f + y, z, h, h, 0.0f, buf, 0xCC000000, 1, 1, 0);
            fn_800616C0(lbl_8087EEB0, x, y, z, h, h, 0.0f, buf, color, 1, 1, 0);
            if (mDebugLevel == 8) {
                fn_800CE9A4(&sound, x + 330.0f, y, z, 60.0f, h);
            }
            y += h;
        }
    }
    if (mDebugLevel == 5) {
        SoundEngine_unk* engine = lbl_8087EE90;
        unsigned int color = -1;
        float y = 20.0f;
        char line[0x100];
        if (engine->mBankCount != 0) {
            color = -1;
            for (unsigned int i = 0; i < engine->mBankCount; i++) {
                sprintf(line, "%02d: %s\0", i, engine->mBanks + i * 0x54);
                fn_800616C0(lbl_8087EEB0, x, y, z, h, h, 0.0f, line, -1, 1, 1, 0);
                y += h;
            }
        }
        sprintf(line, "Reverb:%d <- %d\0", lbl_8087EE90->m8BC, lbl_8087EE90->m8C0);
        fn_800616C0(lbl_8087EEB0, x, y, z, h, h, 0.0f, line, color, 1, 1, 0);
    }
    if (mDebugLevel == 6 || mDebugLevel == 7) {
        float y = 20.0f;
        for (int i = 0; i < 32; i++) {
            SoundCore& sound = mSounds[i];
            if (!sound.fn_800C8278() && sound.mHandle.mSound != 0 && (sound.m9C & 8) == 8) {
                if (mDebugLevel == 6) {
                    float volume = sound.fn_800CA844();
                    sprintf(buf, "[%s] Vol:%.2f", sound.fn_800CA834(), volume);
                } else {
                    sprintf(buf, "[%d] Vol:%.2f", sound.m98, sound.fn_800CA844());
                }
                fn_800616C0(lbl_8087EEB0, 1.0f + x, 1.0f + y, z, h, h, 0.0f, buf, 0xCC000000, 1, 1, 0);
            fn_800616C0(lbl_8087EEB0, x, y, z, h, h, 0.0f, buf, 0xCCFFFFFF, 1, 1, 0);
                fn_800CE9A4(&sound, x + 180.0f, y, z, 60.0f, h);
                y += h;
            }
        }
    }
}

void SoundManager::fn_800CF45C(int frames, int onlyUnused) {
    fn_800D123C(boost::bind(&fn_800CB718, _1, frames, onlyUnused), -1, -1, -1);
}

void SoundManager::fn_800CF680(int category, int frames, int onlyUnused) {
    fn_800D0F34(boost::bind(&fn_800CB718, _1, frames, onlyUnused), category);
}

void SoundManager::fn_800CF7DC(int pause, unsigned int mask, int frames) {
    fn_800D123C(boost::bind(&fn_800CB714, _1, pause, mask, frames), -1, -1, -1);
}

void SoundManager::fn_800CFA28(int player, int pause, unsigned int mask, int frames) {
    fn_800D0DB0(boost::bind(&fn_800CB714, _1, pause, mask, frames), player);
}

void SoundManager::fn_800CFBA0(int category, int pause, unsigned int mask, int frames) {
    fn_800D0F34(boost::bind(&fn_800CB714, _1, pause, mask, frames), category);
}

void SoundManager::fn_800CFD18(const void* owner, int frames) {
    for (int i = 0; i < 32; i++) {
        SoundCore& sound = mSounds[i];
        if (sound.mHandle.mSound != 0 && (owner == 0 || sound.mB4 == owner)) {
            sound.mHandle.Stop(frames);
            sound.fn_800C82A0(frames <= 0);
        }
    }
}

SoundCategory_unk* SoundManager::fn_800CFDA0(int player) {
    return GetPlayer_unk(player);
}

SoundCategory_unk* SoundManager::fn_800CFDC8(int category) {
    return GetCategory_unk(category);
}

void SoundManager::fn_800CFDF0(const SoundCategory_unk& param) {
    SoundCategory_unk* p = GetCategory_unk(param.m00);
    if (p != 0) {
        *p = param;
        fn_800D03AC(param.m00, param.mVolume, 0);
    }
}

void SoundManager::fn_800CFFB8(const SoundCategory_unk& param) {
    SoundCategory_unk* p = GetScope_unk(param.m00);
    if (p != 0) {
        *p = param;
        fn_800D0518(param.m00, param.mVolume, 0);
    }
}

// inferred: setters of the parameter tables; each one applies the change to the sounds that use the
// entry (only Volume and FxSend of the players are also called out of line)
inline void SoundManager::SetPlayerMute_unk(int player, int mute, int frames) {
    SoundCategory_unk* p = GetPlayer_unk(player);
    if (p != 0) {
        p->mMute = mute;
        fn_800D0DB0(boost::bind(&fn_800CB7F0, _1, frames), player);
    }
}
inline void SoundManager::SetPlayerPitch_unk(int player, float pitch, int frames) {
    SoundCategory_unk* p = GetPlayer_unk(player);
    if (p != 0) {
        p->mPitch = pitch;
        fn_800D0DB0(boost::bind(&fn_800CB7F4, _1, frames), player);
    }
}
inline void SoundManager::SetPlayerLpf_unk(int player, float lpf, int frames) {
    SoundCategory_unk* p = GetPlayer_unk(player);
    if (p != 0) {
        p->mPan = lpf;
        fn_800D0DB0(boost::bind(&fn_800CB7F8, _1, frames), player);
    }
}
inline void SoundManager::SetCategoryMute_unk(int category, int mute, int frames) {
    SoundCategory_unk* p = GetCategory_unk(category);
    if (p != 0) {
        p->mMute = mute;
        fn_800D0F34(boost::bind(&fn_800CB7F0, _1, frames), category);
    }
}
inline void SoundManager::SetCategoryPitch_unk(int category, float pitch, int frames) {
    SoundCategory_unk* p = GetCategory_unk(category);
    if (p != 0) {
        p->mPitch = pitch;
        fn_800D0F34(boost::bind(&fn_800CB7F4, _1, frames), category);
    }
}
inline void SoundManager::SetCategoryLpf_unk(int category, float lpf, int frames) {
    SoundCategory_unk* p = GetCategory_unk(category);
    if (p != 0) {
        p->mPan = lpf;
        fn_800D0F34(boost::bind(&fn_800CB7F8, _1, frames), category);
    }
}
inline void SoundManager::SetScopeMute_unk(int scope, int mute, int frames) {
    SoundCategory_unk* p = GetScope_unk(scope);
    if (p != 0) {
        p->mMute = mute;
        fn_800D10B8(boost::bind(&fn_800CB7F0, _1, frames), scope);
    }
}
inline void SoundManager::SetScopePitch_unk(int scope, float pitch, int frames) {
    SoundCategory_unk* p = GetScope_unk(scope);
    if (p != 0) {
        p->mPitch = pitch;
        fn_800D10B8(boost::bind(&fn_800CB7F4, _1, frames), scope);
    }
}
inline void SoundManager::SetScopeLpf_unk(int scope, float lpf, int frames) {
    SoundCategory_unk* p = GetScope_unk(scope);
    if (p != 0) {
        p->mPan = lpf;
        fn_800D10B8(boost::bind(&fn_800CB7F8, _1, frames), scope);
    }
}
// inferred: player parameters (inlined in the constructor)
inline void SoundManager::SetPlayerParam_unk(const SoundCategory_unk& param) {
    SoundCategory_unk* p = GetPlayer_unk(param.m00);
    if (p != 0) {
        *p = param;
        fn_800D0240(param.m00, param.mVolume, 0);
    }
}



void SoundManager::fn_800D0240(int player, float volume, int frames) {
    SoundCategory_unk* p = GetPlayer_unk(player);
    if (p != 0) {
        p->mVolume = volume;
        fn_800D0DB0(boost::bind(&fn_800CB7F0, _1, frames), player);
    }
}

void SoundManager::fn_800D03AC(int category, float volume, int frames) {
    SoundCategory_unk* p = GetCategory_unk(category);
    if (p != 0) {
        p->mVolume = volume;
        fn_800D0F34(boost::bind(&fn_800CB7F0, _1, frames), category);
    }
}

void SoundManager::fn_800D0518(int scope, float volume, int frames) {
    SoundCategory_unk* p = GetScope_unk(scope);
    if (p != 0) {
        p->mVolume = volume;
        fn_800D10B8(boost::bind(&fn_800CB7F0, _1, frames), scope);
    }
}

void SoundManager::fn_800D0684(int player, float fxSend) {
    SoundCategory_unk* p = GetPlayer_unk(player);
    if (p != 0) {
        p->mFxSend = fxSend;
        fn_800D0DB0(boost::bind(&fn_800CB7FC, _1), player);
    }
}

inline void SoundManager::SetCategoryFxSend_unk(int category, float fxSend) {
    SoundCategory_unk* p = GetCategory_unk(category);
    if (p != 0) {
        p->mFxSend = fxSend;
        fn_800D0F34(boost::bind(&fn_800CB7FC, _1), category);
    }
}
inline void SoundManager::SetScopeFxSend_unk(int scope, float fxSend) {
    SoundCategory_unk* p = GetScope_unk(scope);
    if (p != 0) {
        p->mFxSend = fxSend;
        fn_800D10B8(boost::bind(&fn_800CB7FC, _1), scope);
    }
}

int SoundManager::fn_800D089C(int category, int onlyIdle) {
    SoundCategory_unk* p = GetCategory_unk(category);
    int count = 0;
    for (int i = 0; i < 32; i++) {
        SoundCore& sound = mSounds[i];
        if (sound.mHandle.mSound != 0 && (sound.m9C & 8) == 8 && (p == 0 || sound.mAC == p) &&
            (onlyIdle == 0 || sound.mA0 == 0)) {
            count++;
        }
    }
    return count;
}

void SoundManager::fn_800D0974(int category, int frames) {
    fn_800D0F34(boost::bind(&fn_800CB788, _1, frames), category);
}

// NONMATCHING: 99.5% - orden de registros.
SoundCore* SoundManager::fn_800D0AB0(unsigned int id) {
    if (lbl_8087EE90->m04 == 0 || mEnabled == 0) {
        return 0;
    }
    if (mStreamEnabled == 0 && fn_80049B2C(lbl_8087EE90, id)) {
        return 0;
    }
    SoundCore* found = 0;
    int i = mNext;
    do {
        i = (i + 1) % 32;
        SoundCore* sound = &mSounds[i];
        if (sound->fn_800C8278()) {
            mNext = i;
            found = sound;
            break;
        }
    } while (i != mNext);
    if (found != 0) {
        found->fn_800CA1D4(0, -1, 0);
        found->mAC = GetCategory_unk(m34D0);
        found->mB0 = GetScope_unk(m34D4);
        found->mB4 = m34D8;
        found->mE8 = m34C8 == 0;
    }
    return found;
}

// NONMATCHING: 99.6% - orden de registros.
int SoundManager::fn_800D0C1C(SoundCore* sound, float volume) {
    if (sound == 0 || lbl_8087EE90->m04 == 0) {
        return 0;
    }
    if (m34C8 != 0) {
        if (volume < mMinimumVolume) {
            return 0;
        }
        unsigned int id = sound->m98;
        if (id != 0) {
            for (SoundLogEntry_unk* it = mLog.begin(); it != mLog.end(); it++) {
                if (it->mId == id && it->mAge <= mDupLimitFrame) {
                    if (it->mSound->mA4 <= 0 && it->mVolume < volume) {
                        for (int i = 0; i < 32; i++) {
                            if (it->mSound == &mSounds[i]) {
                                mSounds[i].mHandle.Stop(0);
                                break;
                            }
                        }
                        it->mSound = sound;
                        it->mId = sound->m98;
                        it->mAge = 0;
                        it->mVolume = volume;
                        return 1;
                    }
                    if (it->mSound != 0 && !it->mSound->fn_800C8278() && id == it->mSound->m98) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

void SoundManager::fn_800D0DB0(Func func, int player) {
    if (func.empty()) {
        return;
    }
    SoundCategory_unk* p = GetPlayer_unk(player);
    for (int i = 0; i < 32; i++) {
        SoundCore& sound = mSounds[i];
        if (sound.mHandle.mSound != 0 && (p == 0 || sound.mA8 == p)) {
            func(sound);
        }
    }
}

void SoundManager::fn_800D0F34(Func func, int category) {
    if (func.empty()) {
        return;
    }
    SoundCategory_unk* p = GetCategory_unk(category);
    for (int i = 0; i < 32; i++) {
        SoundCore& sound = mSounds[i];
        if (sound.mHandle.mSound != 0 && (p == 0 || sound.mAC == p)) {
            func(sound);
        }
    }
}

void SoundManager::fn_800D10B8(Func func, int scope) {
    if (func.empty()) {
        return;
    }
    SoundCategory_unk* p = GetScope_unk(scope);
    for (int i = 0; i < 32; i++) {
        SoundCore& sound = mSounds[i];
        if (sound.mHandle.mSound != 0 && (p == 0 || sound.mB0 == p)) {
            func(sound);
        }
    }
}

void SoundManager::fn_800D123C(Func func, int player, int category, int scope) {
    if (func.empty()) {
        return;
    }
    SoundCategory_unk* p = GetPlayer_unk(player);
    SoundCategory_unk* c = GetCategory_unk(category);
    SoundCategory_unk* s = GetScope_unk(scope);
    for (int i = 0; i < 32; i++) {
        SoundCore& sound = mSounds[i];
        if (sound.mHandle.mSound != 0 && (p == 0 || sound.mA8 == p) && (c == 0 || sound.mAC == c) &&
            (s == 0 || sound.mB0 == s)) {
            func(sound);
        }
    }
}

void SoundManager::fn_800D1430() {
    void* menu = mMenu.Load("Sound");
    fn_80087994(menu, "Enabled", &mEnabled, 0, 0);
    fn_80087994(menu, "StreamEnabled", &mStreamEnabled, 0, 0);
    fn_800874C8(menu, "DebugLevel", &mDebugLevel, 0, 99, 1, 0, 0);
    fn_800874C8(menu, "OutputMode", &mOutputMode, 0, 3, 1, 0, 0);
    fn_8008771C(menu, "MasterVolume", &mMasterVolume, 0, 0, 0.0f, 2.0f, 0.1f);
    void* players = fn_8008937C(menu, "PlayerParam");
    for (int i = 0; i < 8; i++) {
        char name[0x20];
        sprintf(name, "Player %d\0", i);
        void* group = fn_8008937C(players, name);
        fn_80087994(group, "Mute", &mPlayers[i].mMute, (int)fn_800CB800, (int)&mPlayers[i]);
        fn_8008771C(group, "Volume", &mPlayers[i].mVolume, (int)fn_800CB800, (int)&mPlayers[i], 0.0f, 2.0f, 0.1f);
        fn_8008771C(group, "Pitch", &mPlayers[i].mPitch, (int)fn_800CB800, (int)&mPlayers[i], 0.1f, 10.0f, 0.1f);
        fn_8008771C(group, "LPF", &mPlayers[i].mPan, (int)fn_800CB800, (int)&mPlayers[i], 0.0f, 1.0f, 0.1f);
        fn_8008771C(group, "FxSendA", &mPlayers[i].mFxSend, (int)fn_800CB800, (int)&mPlayers[i], 0.0f, 1.0f, 0.1f);
    }
    void* categories = fn_8008937C(menu, "CategoryParam");
    for (int i = 0; i < 32; i++) {
        char name[0x20];
        sprintf(name, "Category %d\0", i);
        void* group = fn_8008937C(categories, name);
        fn_80087994(group, "Mute", &mCategories[i].mMute, (int)fn_800CBE8C, (int)&mCategories[i]);
        fn_8008771C(group, "Volume", &mCategories[i].mVolume, (int)fn_800CBE8C, (int)&mCategories[i], 0.0f, 2.0f,
                    0.1f);
        fn_8008771C(group, "Pitch", &mCategories[i].mPitch, (int)fn_800CBE8C, (int)&mCategories[i], 0.1f, 10.0f,
                    0.1f);
        fn_8008771C(group, "LPF", &mCategories[i].mPan, (int)fn_800CBE8C, (int)&mCategories[i], 0.0f, 1.0f, 0.1f);
        fn_8008771C(group, "FxSendA", &mCategories[i].mFxSend, (int)fn_800CBE8C, (int)&mCategories[i], 0.0f, 1.0f,
                    0.1f);
    }
    void* scopes = fn_8008937C(menu, "ScopeParam");
    for (int i = 0; i < 16; i++) {
        char name[0x20];
        sprintf(name, "Scope %d\0", i);
        void* group = fn_8008937C(scopes, name);
        fn_80087994(group, "Mute", &mScopes[i].mMute, (int)fn_800CC518, (int)&mScopes[i]);
        fn_8008771C(group, "Volume", &mScopes[i].mVolume, (int)fn_800CC518, (int)&mScopes[i], 0.0f, 2.0f, 0.1f);
        fn_8008771C(group, "Pitch", &mScopes[i].mPitch, (int)fn_800CC518, (int)&mScopes[i], 0.1f, 10.0f, 0.1f);
        fn_8008771C(group, "LPF", &mScopes[i].mPan, (int)fn_800CC518, (int)&mScopes[i], 0.0f, 1.0f, 0.1f);
        fn_8008771C(group, "FxSendA", &mScopes[i].mFxSend, (int)fn_800CC518, (int)&mScopes[i], 0.0f, 1.0f, 0.1f);
    }
    void* sound3d = fn_8008937C(menu, "3DSound");
    fn_80087994(sound3d, "Enabled", &m3dEnabled, 0, 0);
    fn_8008771C(sound3d, "MaxVolumeDistance", &mMaxVolumeDistance, 0, 0, 0.0f, 9999.0f, 1.0f);
    fn_8008771C(sound3d, "UnitDistance", &mUnitDistance, 0, 0, 0.0f, 9999.0f, 1.0f);
    fn_8008771C(sound3d, "InteriorSize", &mInteriorSize, 0, 0, 0.0f, 9999.0f, 1.0f);
    void* fx = fn_8008937C(menu, "SoundFx");
    fn_80087994(fx, "Enabled", &mFxEnabled, 0, 0);
    fn_80087994(fx, "2D Enabled", &mFx2dEnabled, 0, 0);
    fn_80087994(menu, "Compressor", &mCompressor, 0, 0);
    fn_800874C8(menu, "DupLimitFrame", &mDupLimitFrame, -1, 30, 1, 0, 0);
    fn_8008771C(menu, "MinimumVolume", &mMinimumVolume, 0, 0, 0.0f, 2.0f, 0.05f);
    fn_8008771C(menu, "RandomPitch", &mRandomPitch, 0, 0, 0.0f, 2.0f, 0.1f);
}

void SoundManager::fn_800D19FC(SoundCore* sound, float volume) {
    if (mLog.size() >= 32 || sound == 0) {
        return;
    }
    char text[0x100];
    if (mDebugLevel > 0) {
        if ((sound->m9C & 2) == 2) {
            Vector3 pos = sound->m08.mActor.mPosition;
            float pan;
            float span = sound->fn_800CA8E0();
            pan = sound->fn_800CA8B0();
            sprintf(text, "[%s] Pos(%.2f, %.2f, %.2f) Vol:%.2f Pan:%.2f SPan:%.2f", sound->fn_800CA834(), pos.x,
                    pos.y, pos.z, volume, pan, span);
        } else {
            sprintf(text, "[%s]\0", sound->fn_800CA834());
        }
    } else {
        text[0] = '\0';
    }
    for (SoundLogEntry_unk* it = mLog.begin(); it != mLog.end(); it++) {
        if (it->mId == sound->m98) {
            it->mText = text;
            it->mAge = 0;
            it->mVolume = volume;
            return;
        }
    }
    SoundLogEntry_unk entry;
    entry.mText = text;
    entry.mId = sound->m98;
    entry.mAge = 0;
    entry.mVolume = volume;
    entry.mSound = sound;
    mLog.push_back(entry);
}

} // namespace atn
