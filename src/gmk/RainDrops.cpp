#include "gmk/RainDrops.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, int id, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, const Color4f_unk& color, int arg7, int arg8, int arg9,
                            float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);

namespace gmk {

GimmickCommon* RainDrops::fn_8042AFE4(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) RainDrops(parent, id, arg);
    }
    return 0;
}

RainDrops::~RainDrops() {}

int RainDrops::vf0C() {
    if (fn_803EC91C() == 0 && fn_800D3FA4() != 0) {
        for (int i = 0; i < 9; i++) {
            if (mRain[i] != 0) {
                mRain[i]->fn_800D246C(0);
                mRain[i]->mFlags |= 4;
            }
        }
        return 1;
    }
    return 0;
}

void RainDrops::Start() {
    int type = m58;
    if (type > 0) {
        GimmickMsg_unk msg(type);
        vf40(&msg);
    }
}

void RainDrops::vf10() {
    if (m54 != mCurrent) {
        fn_80239DAC(lbl_8087F3C0, this, mCurrent, 1);
        if (mEfp[m54].mResource != 0) {
            lbl_8087F3C0->mB8 = 1;
            fn_80232B7C(this, m54);
            atn::EfpFile* efp = &mEfp[m54];
            float scale = m90.x;
            fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, m6C, m78, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, scale);
            lbl_8087F3C0->mB8 = 0;
        }
        if (mRain[m54] != 0) {
            mRain[m54]->mFlags &= ~4;
        }
        if (mRain[mCurrent] != 0) {
            mRain[mCurrent]->mFlags |= 4;
        }
        mCurrent = m54;
    }
}

void RainDrops::vf14() {}

int RainDrops::vf1C() {
    for (int i = 0; i < 9; i++) {
        if (mEfp[i].IsLoading() != 0) {
            return 1;
        }
    }
    return 0;
}

void RainDrops::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    int n = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "EFP") == 0) {
            if (++n >= 9) {
                n = 8;
            }
            mEfp[n].LoadRequest(csv.fn_8005B9CC());
        } else if (strcmp(field, "RAINDROPS") == 0) {
            mRain[n] = MapPartsRain::fn_8049CDBC(this, (void*)8, &csv);
            mRain[n]->fn_800D246C(1);
            if (mRain[n]->mOn != 0) {
                mRain[n]->mRain.fn_800B2180();
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

int RainDrops::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    return m54 = msg->mType;
}

} // namespace gmk
