#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"

// Loaders of the effect resources (ResLoader_unk: LH_Effect, EfpHandle_unk: LH_Efp) and the byte
// swapper of the effect files. Name of the file invented.

struct EffectRes_unk;
extern "C" int fn_802386FC(void* res); // inferred: (EffectRes_unk.cpp)

namespace atn {
// inferred: only the two methods used here (see src/atn/EffectManager.cpp)
class EffectManager {
public:
    void fn_8023A2A8(void* owner);
    void fn_8023A340(EffectRes_unk* res);
};
} // namespace atn

extern "C" atn::EffectManager* lbl_8087F3C0; // inferred: EffectManager instance

namespace std {
// inferred: the static objects of <iostream> (MSL): fn_8068B770 builds cin/cout/cerr on __files,
// fn_8068BD60 the wide streams. Names invented except ios_base.
class ios_base {
public:
    class Init {
    public:
        Init();
        ~Init();
    };
};
class __wInit_unk {
public:
    __wInit_unk();
    ~__wInit_unk();
};
static ios_base::Init __msl_ios_init_unk;
static __wInit_unk __msl_wios_init_unk;
} // namespace std

// inferred: byte swap description of a binary structure: list of {type, count} terminated by
// type -1; type 0 skips `count` bytes, 1/2/3 swap `count` 16/32/64-bit values, 4 repeats the
// sub-description `count` times. Names invented.
struct SwapDesc_unk {
    short mType;
    short mCount;
    SwapDesc_unk* mSub;
};

// inferred: 64-bit value in a struct with an array member: it is not scalarised, so its copy leaves
// dead stores on the stack like the original
struct Swap64_unk {
    unsigned long long mValue[1];
};

// NONMATCHING: 74.74% - the 16/32-bit swap expressions are recognised as sthbrx/stwbrx and the
// loops are unrolled as in the original (while (count-- > 0)), but the 16-bit loop uses lhzu
// (update form) instead of offsets + separate addresses, the 64-bit body schedules differently
// and the saved registers are shifted by one (r26-r31 vs r25-r31 in the original)
// inferred: swaps `data` in place following `desc`; returns the size processed
extern "C" int fn_802371BC(unsigned char* data, SwapDesc_unk* desc) {
    int size = 0;
    if (desc != 0) {
        for (; desc->mType != -1; desc++) {
            int count = desc->mCount;
            switch (desc->mType) {
            case 0:
                data += count;
                size += count;
                break;
            case 1:
                while (count-- > 0) {
                    int v = *(unsigned short*)data;
                    *(unsigned short*)data = ((v & 0xFF) << 8) | ((v & 0xFF00) >> 8);
                    size += 2;
                    data += 2;
                }
                break;
            case 2:
                while (count-- > 0) {
                    int v = *(int*)data;
                    *(int*)data = ((v & 0xFF) << 24) | ((v & 0xFF00) << 8) | ((v & 0xFF0000) >> 8) | ((v & 0xFF000000) >> 24);
                    size += 4;
                    data += 4;
                }
                break;
            case 3:
                while (count-- > 0) {
                    unsigned long long v = *(unsigned long long*)data;
                    Swap64_unk r;
                    r.mValue[0] = (v >> 56) | ((v >> 40) & 0x000000000000FF00ULL) |
                        ((v >> 24) & 0x0000000000FF0000ULL) | ((v >> 8) & 0x00000000FF000000ULL) |
                        ((v << 8) & 0x000000FF00000000ULL) | ((v << 24) & 0x0000FF0000000000ULL) |
                        ((v << 40) & 0x00FF000000000000ULL) | (v << 56);
                    *(Swap64_unk*)data = r;
                    size += 8;
                    data += 8;
                }
                break;
            case 4:
                for (int i = 0; i < count; i++) {
                    int n = fn_802371BC(data, desc->mSub);
                    data += n;
                    size += n;
                }
                break;
            }
        }
    }
    return size;
}

ResLoader_unk::ResLoader_unk() : mResource(0) {}

ResLoader_unk::~ResLoader_unk() {
    fn_8023772C();
}

void ResLoader_unk::fn_80237654(const char* path) {
    fn_8023772C();
    mLoader.vf0C(path);
}

int ResLoader_unk::fn_802376D0() {
    if (mLoader.fn_80473F50()) {
        return 1;
    }
    if (mResource == 0) {
        mResource = mLoader.fn_804738C4();
    }
    return 0;
}

void ResLoader_unk::fn_8023772C() {
    if (mResource != 0) {
        if (lbl_8087F3C0 != 0) {
            lbl_8087F3C0->fn_8023A2A8(this);
        }
        mLoader.fn_80473F88();
        mResource = 0;
    }
}

// inferred: (ResLoader_unk) value computed by the effect resource, 0 without resource
extern "C" int fn_80237784(ResLoader_unk* loader) {
    if (loader->mResource != 0) {
        return fn_802386FC(loader->mResource);
    }
    return 0;
}

// inferred: (ResLoader_unk) field 0x38 of the effect resource
extern "C" int fn_8023779C(ResLoader_unk* loader) {
    if (loader->mResource != 0) {
        return ((int*)loader->mResource)[0x38 / 4];
    }
    return 0;
}

namespace atn {

EfpHandle_unk::EfpHandle_unk() : mResource(0) {}

void EfpHandle_unk::fn_8023780C(const char* path) {
    mLoader.vf0C(path);
}

void EfpHandle_unk::fn_8023781C() {
    if (mResource != 0) {
        if (lbl_8087F3C0 != 0) {
            lbl_8087F3C0->fn_8023A340((EffectRes_unk*)this);
        }
        mLoader.fn_80473F88();
        mResource = 0;
    }
}

int EfpHandle_unk::fn_80237874() {
    if (mLoader.fn_80473F50() == 0) {
        mResource = mLoader.fn_80473C20();
        return 0;
    }
    return 1;
}

} // namespace atn

// inferred: number of loops of the effect (field 0x0C of the efp resource), 0 without resource
extern "C" int fn_802378C4(atn::EfpHandle_unk* efp) {
    if (efp->mResource != 0) {
        return ((int*)efp->mResource)[0x0C / 4];
    }
    return 0;
}
