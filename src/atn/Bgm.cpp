#include "atn/Bgm.h"
#include "atn/SoundCore.h"
#include "atn/SoundManager.h"

extern "C" void fn_80709AD0(void* sound, int frames);
extern "C" unsigned long strlen(const char* s);

// inferred: sound engine (lbl_8087EE90); only the member read here
struct BgmEngine_unk {
    void* m00;
    void* m04; // inferred: nonzero once the engine is ready
};
extern "C" BgmEngine_unk* lbl_8087EE90;
extern "C" unsigned int fn_80049B74(BgmEngine_unk* engine, const char* name); // inferred: sound id of a name
extern "C" int fn_800CB4EC(atn::SoundRef* sound);
extern "C" void fn_800CB504(atn::SoundRef* sound);
extern "C" void fn_800CB5B4(atn::SoundRef* sound, int frames, float volume);
extern "C" void fn_800CB654(atn::SoundRef* sound, float a, float b, float c);
extern "C" void fn_800CB518(atn::SoundRef* sound, int frames); // inferred: fades the sound in

// inferred: OSCalendarTime (0x28 bytes)
struct BgmCalendarTime_unk {
    int m00[10];
};
extern "C" long long OSGetTime();
extern "C" void OSTicksToCalendarTime(long long ticks, BgmCalendarTime_unk* out);

namespace atn {

Bgm::Bgm() : m04(0), m0C(0), m10(0) {}

Bgm::~Bgm() {
    // inferred: inlined handle stop (cf. nw4r::snd::SoundHandle::Stop)
    if (mSound.mPtr != 0 && mSound.mPtr->mSound != 0) {
        fn_80709AD0(mSound.mPtr->mSound, 0);
    }
    mSound.Reset();
}

// inferred: non-empty name
static inline bool __IsName_unk(const char* name) {
    bool ok = false;
    if (name != 0 && strlen(name) != 0) {
        ok = true;
    }
    return ok;
}

int Bgm::fn_8004AE84(const char* name, int start) {
    mSound.Reset();
    if (lbl_8087EE90->m04 == 0 || lbl_8087EFE8 == 0 || !__IsName_unk(name)) {
        return 0;
    }
    SoundObject* sound = (SoundObject*)lbl_8087EFE8->fn_800D0AB0(fn_80049B74(lbl_8087EE90, name));
    if (sound == 0) {
        return 0;
    }
    sound->mA8 = lbl_8087EFE8->fn_800CFDA0(m0C);
    ((SoundCore*)sound)->fn_800CA7D4(start);
    ((SoundCore*)sound)->fn_800C93D4(name);
    m04 = 0;
    if (sound->m90 == 0) {
        m04 = 1;
    }
    mSound = sound;
    return m04 == 1;
}

int Bgm::fn_8004AFAC(const char* name, int arg, int start) {
    mSound.Reset();
    if (lbl_8087EE90->m04 == 0 || lbl_8087EFE8 == 0 || !__IsName_unk(name)) {
        return 0;
    }
    SoundObject* sound = (SoundObject*)lbl_8087EFE8->fn_800D0AB0(fn_80049B74(lbl_8087EE90, name));
    if (sound == 0) {
        return 0;
    }
    sound->mA8 = lbl_8087EFE8->fn_800CFDA0(m0C);
    ((SoundCore*)sound)->fn_800CA7D4(start);
    if (lbl_8087EFE8->m3dEnabled != 0) {
        ((SoundCore*)sound)->fn_800C97F8(name, (const Vector3*)arg);
    } else {
        ((SoundCore*)sound)->fn_800C93D4(name);
    }
    m04 = 0;
    if (sound->m90 == 0) {
        m04 = 1;
    }
    mSound = sound;
    return m04 == 1;
}

int Bgm::fn_8004B0E4() {
    if (mSound.mPtr == 0) {
        return 0;
    }
    if (m04 == 1) {
        if (fn_800CB4EC(&mSound) != 0) {
            return 1;
        }
        m04 = 2;
        return 0;
    }
    return 0;
}

int Bgm::fn_8004B158(float volume, int fade) {
    fn_800CB504(&mSound);
    fn_800CB5B4(&mSound, 0, volume);
    fn_800CB654(&mSound, 0.0f, 0.0f, 0.0f);
    fn_800CB518(&mSound, fade);
    lbl_8087EFE8->fn_800D19FC((SoundCore*)mSound.mPtr, 1.0f);
    return 1;
}

void Bgm::fn_8004B1EC(int fade) {
    if (mSound.mPtr != 0 && mSound.mPtr->mSound != 0) {
        fn_80709AD0(mSound.mPtr->mSound, fade);
    }
}

} // namespace atn

// inferred: current calendar time (used by MP::SaveLoad)
extern "C" void fn_8004B20C(BgmCalendarTime_unk* out) {
    BgmCalendarTime_unk time;
    OSTicksToCalendarTime(OSGetTime(), &time);
    *out = time;
}
