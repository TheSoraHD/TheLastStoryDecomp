#include "game/MapPartsOcean.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" double sin(double x);
extern "C" const char* fn_8005B9CC(void* data); // inferred: returns the next CSV field
extern "C" float fn_800DC288(const char* str);  // inferred: string to float
extern "C" int fn_800DC12C(const char* str);    // inferred: string to int
extern "C" void* fn_8008937C(void* menu, const char* name); // inferred: child by name
extern "C" void fn_8008771C(void* menu, const char* name, float* value, int arg3, int arg4,
                            float min, float max, float step); // inferred: debug float slider
extern "C" void fn_80087858(void* menu, const char* name, float* value, int arg3, int arg4,
                            float min, float max, float step); // inferred: debug angle slider
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);

// inferred: CSV reader; the current field is a buffer at +0x10
struct Csv_unk {
    unsigned char m00[0x10];
    char mField[1];
};

MapPartsOcean* MapPartsOcean::fn_8049C8DC(atn::Task* parent, void* arg, void* data) {
    if (parent != 0) {
        return new (12, "", "", 0) MapPartsOcean(parent, arg, data);
    }
    return 0;
}

MapPartsOcean::MapPartsOcean(atn::Task* parent, void* arg, void* data) : MapPartsModel(parent, arg, data) {
    mTime = 0.0f;
    m450 = 15.0f;
    mWave[0].mAmplitude = 12.0f;
    mWave[0].mPeriod = 1.0f;
    mWave[0].mPhase = 0.5235988f;
    mWave[1].mAmplitude = 8.0f;
    mWave[1].mPeriod = 0.7f;
    mWave[1].mPhase = 1.2217305f;
    const char* field = ((Csv_unk*)data)->mField;
    do {
        if (strcmp(field, "WAVE") == 0) {
            unsigned int i = fn_800DC12C(fn_8005B9CC(data));
            if (i <= 1) {
                mWave[i].mAmplitude = fn_800DC288(fn_8005B9CC(data));
                mWave[i].mPeriod = fn_800DC288(fn_8005B9CC(data));
                mWave[i].mPhase = 0.017453292f * fn_800DC288(fn_8005B9CC(data));
            }
        }
        field = fn_8005B9CC(data);
    } while (field != 0 && strcmp("", field) != 0);
}

void MapPartsOcean::vf1C(void* menu) {
    for (int i = 0; i < 2; i++) {
        char name[0x100];
        sprintf(name, "Wave %d\n", i);
        void* group = fn_8008937C(menu, name);
        fn_8008771C(group, "Amplitude", &mWave[i].mAmplitude, 0, 0, -10000.0f, 10000.0f, 0.01f);
        fn_8008771C(group, "Period", &mWave[i].mPeriod, 0, 0, -10000.0f, 10000.0f, 0.01f);
        fn_80087858(group, "Phase", &mWave[i].mPhase, 0, 0, -10000.0f, 10000.0f, 0.01f);
    }
}

MapPartsOcean::~MapPartsOcean() {}

int MapPartsOcean::vf0C() {
    if (mModel.fn_8008B140() == 0) {
        mJoint[0] = mModel.fn_80092814("joint1", 0);
        mJoint[1] = mModel.fn_80092814("joint2", 0);
        return 1;
    }
    return 0;
}

void MapPartsOcean::vf10() {
    float offset[2];
    for (int i = 0; i < 2; i++) {
        offset[i] = mWave[i].mAmplitude * (float)sin(mTime * mWave[i].mPeriod + mWave[i].mPhase);
    }
    for (int i = 0; i < 2; i++) {
        MtxPtr node = mModel.GetNodeMtx_unk(mJoint[i]);
        atn::Vector3 trans(0.0f, offset[i], 0.0f);
        Mtx m;
        PSMTXTrans(m, trans.x, trans.y, trans.z);
        __MtxCopy(node, m);
        PSMTXConcat(mModel.mMtx, node, node);
    }
    mTime += 1.0f / 30.0f;
    MapPartsModel::vf10();
}
