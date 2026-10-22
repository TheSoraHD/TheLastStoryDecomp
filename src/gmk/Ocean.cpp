#include "gmk/Ocean.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" double sin(double x);
extern "C" double modf(double x, double* ip);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087858(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: angle
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);

// inferred: the ocean being shown (read by the characters)
static gmk::Ocean* sInstance;
// inferred: size of a cell of the wave grid
static float sGridSize = 45.455f;

// inferred: translation matrix by value (name invented)
inline atn::Matrix3x4 TransMtx_unk(const atn::Vector3& v) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, v.x, v.y, v.z);
    return m;
}

namespace gmk {

GimmickCommon* Ocean::fn_80418198(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) Ocean(parent, id, arg);
    }
    return 0;
}

Ocean::~Ocean() {
    sInstance = 0;
}

} // namespace gmk

// inferred: the current ocean (CharaTask calls it as a "water id")
extern "C" gmk::Ocean* fn_80418318() {
    return sInstance;
}

namespace gmk {

int Ocean::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
        sInstance = this;
        return 1;
    }
    return 0;
}

void Ocean::vf10() {
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    fn_804186D8();
}

void Ocean::vf14() {
    if (vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int Ocean::vf1C() {
    int ret = 0;
    if (mModel.fn_8008B140() != 0) {
        ret = 1;
    }
    return ret;
}

// The wave members are written through mWaves[state][index] each time (no reference): that gives the
// order of the address computation of the original (same in gmk::FallingTower::vf20)
void Ocean::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "WAVE") == 0) {
            int state = fn_800DC12C(csv.fn_8005B9CC());
            unsigned int index = fn_800DC12C(csv.fn_8005B9CC());
            if (index <= 1) {
                mWaves[state][index].mAmp = fn_800DC288(csv.fn_8005B9CC());
                mWaves[state][index].mSpeed = fn_800DC288(csv.fn_8005B9CC());
                mWaves[state][index].mPhase = 0.017453292f * fn_800DC288(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

void Ocean::vf28() {
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

void Ocean::fn_804186D8() {
    int nodes[2];
    nodes[0] = mModel.fn_80092814("joint1", 0);
    nodes[1] = mModel.fn_80092814("joint2", 0);
    float heights[2];
    for (int i = 0; i < 2; i++) {
        Wave_unk& wave = mWaves[m54][i];
        float t = mTime;
        heights[i] = wave.mAmp * (float)sin(atn::WrapAngle_unk(wave.mSpeed * t + wave.mPhase));
    }
    for (int i = 0; i < 2; i++) {
        if (nodes[i] > -1) {
            MtxPtr mtx = mModel.mNodeMtx[nodes[i]];
            *(atn::Matrix3x4*)mtx = TransMtx_unk(atn::Vector3(0.0f, heights[i], 0.0f));
            PSMTXConcat(mModel.mMtx, mtx, mtx);
        }
    }
    mTime += 0.033333335f;
}

} // namespace gmk

// inferred: point of the sea surface under `pos` (the start of a segment)
// NONMATCHING: 77.8% - scheduling/registers of the index arithmetic, the table copy and the bilinear
// weights. Found: joints in a 2-iteration loop (one shared temporary), modf results kept as double,
// z offset uses origin.x, ints converted to locals before the modulo, bilinear weights in an inline.
// inferred: bilinear interpolation of the four corners (name invented)
inline float Bilinear_unk(const float* c, float u, float w) {
    return c[0] * ((1.0f - u) * (1.0f - w)) + c[1] * (u * (1.0f - w)) + c[2] * ((1.0f - u) * w) + c[3] * (u * w);
}
extern "C" atn::Vector3 fn_804188A0(gmk::Ocean* ocean, const atn::Vector3* pos) {
    atn::Vector3 origin(-1000.0f, 0.0f, -1000.0f);
    atn::Matrix3x4 inv(*(atn::Matrix3x4*)ocean->mModel.mMtx);
    PSMTXInverse(inv.m, inv.m);
    atn::Vector3 local(*pos);
    PSMTXMultVec(inv.m, &local, &local);
    double ix;
    double fx = modf((local.x - origin.x) / sGridSize, &ix);
    double iz;
    double fz = modf((local.z - origin.x) / sGridSize, &iz);
    float table[4] = {1.0f, 0.5f, 0.0f, 0.5f};
    int xi = (int)ix;
    int zi = (int)iz;
    int zoff = 4 - zi % 4;
    int base = xi % 4 + zoff;
    float corners[4];
    corners[0] = table[base % 4];
    corners[1] = table[(base + 1) % 4];
    corners[2] = table[(base + 3) % 4];
    corners[3] = table[(base + 4) % 4];
    float v = Bilinear_unk(corners, fx, fz);
    float t = 1.0f <= v ? 1.0f : v;
    t = 0.0f >= t ? 0.0f : t;
    float y[2];
    int nodes[2];
    nodes[0] = ocean->mModel.fn_80092814("joint1", 0);
    nodes[1] = ocean->mModel.fn_80092814("joint2", 0);
    for (int i = 0; i < 2; i++) {
        if (nodes[i] > -1) {
            y[i] = ((atn::Matrix3x4*)ocean->mModel.mNodeMtx[nodes[i]])->GetTrans_unk().y;
        }
    }
    atn::Vector3 ret(*pos);
    ret.y = y[0] * t + y[1] * (1.0f - t);
    return ret;
}

namespace gmk {

int Ocean::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    return m54 = msg->mType;
}

void Ocean::vf70(int state) {
    GimmickMsg_unk msg(0);
    if ((unsigned int)state <= 1) {
        msg.mType = 1;
        vf40(&msg);
    }
}

// NONMATCHING: 97.6% - scheduling of the first debug item (string pool address).
void Ocean::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    fn_800874C8(group, "locData", &m58, 0, 99, 1, 0, 0);
    mB0.fn_803F11F8(group);
    for (int i = 0; i < 2; i++) {
        char stateName[0x100];
        sprintf(stateName, "State %d\n", i);
        void* stateGroup = fn_8008937C(group, stateName);
        for (int j = 0; j < 2; j++) {
            char waveName[0x100];
            sprintf(waveName, "Wave %d\n", j);
            void* waveGroup = fn_8008937C(stateGroup, waveName);
            fn_8008771C(waveGroup, "Amplitude", &mWaves[i][j].mAmp, 0, 0, -10000.0f, 10000.0f, 0.01f);
            fn_8008771C(waveGroup, "Period", &mWaves[i][j].mSpeed, 0, 0, -10000.0f, 10000.0f, 0.01f);
            fn_80087858(waveGroup, "Phase", &mWaves[i][j].mPhase, 0, 0, -10000.0f, 10000.0f, 0.01f);
        }
    }
}

Model_unk* Ocean::vf38() {
    return &mModel;
}

} // namespace gmk
