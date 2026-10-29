#include "game/MapPartsEfp.h"
#include "atn/Memory.h"
#include "atn/LensFlare.h"

extern "C" int atoi(const char* str);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int rand();

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, int id, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, const Color4f_unk& color, int arg7, int arg8, int arg9);
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int arg);
extern "C" void fn_80232B7C(void* owner, int arg);

MapPartsEfp* MapPartsEfp::fn_804965EC(atn::Task* parent, void* arg, atn::CsvTab* csv) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) MapPartsEfp(parent, arg, csv);
    }
    return 0;
}

MapPartsEfp::MapPartsEfp(atn::Task* parent, void* arg, atn::CsvTab* csv)
    : MapPartsBase(parent, arg), m5C(0), mMode(0), mRate(0), mDone(0) {
    mEfp.LoadRequest(csv->fn_8005B9CC());
    const char* mode = csv->fn_8005B9CC();
    if (strcmp(mode, "ONCE") == 0) {
        mMode = 0;
    } else if (strcmp(mode, "RANDOM") == 0) {
        mMode = 1;
        mRate = (1 > atoi(csv->fn_8005B9CC())) ? 1 : atoi(csv->fn_8005B9CC());
    } else if (strcmp(mode, "LOOP") == 0) {
        mMode = 2;
    }
}

int MapPartsEfp::vf0C() {
    return mEfp.IsLoading() == 0;
}

// NONMATCHING: 98.6% - stores and stack layout match, but the 0.0f/1.0f constants land in
// swapped FPRs (target: 1.0f in f1). Tried: argument temporaries, default arguments, named locals
// in every order, one parameter struct, chained and per-field assignments, colour first. An inline
// helper with the three temporaries in the call gets the FPRs right but stores the colour after rot/pos
// (same score); named locals or a colour parameter in the helper go back to the swapped FPRs.
void MapPartsEfp::vf10() {
    switch (mMode) {
    case 0:
        if (mDone == 0) {
            fn_80232B7C(this, 0);
            lbl_8087F3C0->mB8 = 1;
            Color4f_unk color;
            atn::Vector3 rot;
            atn::Vector3 pos;
            int id = m5C;
            rot.x = 0.0f;
            rot.y = 0.0f;
            rot.z = 0.0f;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            fn_8023A8B4(lbl_8087F3C0, &mEfp, id, 0, pos, rot, color, -1, -1, 1);
            lbl_8087F3C0->mB8 = 0;
            mDone = 1;
        }
        break;
    case 1:
        if (rand() % mRate == 0) {
            fn_80232B7C(this, 0);
            Color4f_unk color;
            atn::Vector3 rot;
            atn::Vector3 pos;
            int id = m5C;
            rot.x = 0.0f;
            rot.y = 0.0f;
            rot.z = 0.0f;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            fn_8023A8B4(lbl_8087F3C0, &mEfp, id, 0, pos, rot, color, -1, -1, 1);
        }
        break;
    case 2:
        if (fn_80239D58(lbl_8087F3C0, this, 0) == 0) {
            fn_80232B7C(this, 0);
            Color4f_unk color;
            atn::Vector3 rot;
            atn::Vector3 pos;
            int id = m5C;
            rot.x = 0.0f;
            rot.y = 0.0f;
            rot.z = 0.0f;
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
            color.r = 1.0f;
            color.g = 1.0f;
            color.b = 1.0f;
            color.a = 1.0f;
            fn_8023A8B4(lbl_8087F3C0, &mEfp, id, 0, pos, rot, color, -1, -1, 1);
        }
        break;
    }
}

void MapPartsEfp::vf14() {}
