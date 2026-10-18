#include "game/MapPartsRain.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* str); // inferred: string to float
extern "C" int fn_800DC12C(const char* str);    // inferred: string to int
extern "C" int lbl_8087E0D0; // inferred: minimum rain drop count
extern "C" int lbl_8087E0D4; // inferred: maximum rain drop count

// inferred: clamp helper (the bounds are read twice at 0x8049CFB8)
template <class T>
inline T __Clamp_unk(T v, const T& lo, const T& hi) {
    return (v < lo) ? lo : ((v > hi) ? hi : v);
}

MapPartsRain* MapPartsRain::fn_8049CDBC(atn::Task* parent, void* arg, atn::CsvTab* csv) {
    if (parent != 0) {
        return new (12, "", "", 0) MapPartsRain(parent, arg, csv);
    }
    return 0;
}

// NONMATCHING: 99.8% - with -ipa file the stack addresses of `a`, `b` and `box.mMax` hoisted out
// of the loop land in different registers (r24-r26), and box.mMax is read back through its own
// register instead of box + 0xC. It matched with -inline deferred.
MapPartsRain::MapPartsRain(atn::Task* parent, void* arg, atn::CsvTab* csv)
    : MapPartsBase(parent, arg), mOn(0) {
    const char* field = csv->mField;
    do {
        if (strcmp(field, "ON") == 0) {
            mOn = 1;
        } else if (strcmp(field, "COLOR") == 0) {
            Color_unk color;
            color.r = fn_800DC288(csv->fn_8005B9CC());
            color.g = fn_800DC288(csv->fn_8005B9CC());
            color.b = fn_800DC288(csv->fn_8005B9CC());
            color.a = fn_800DC288(csv->fn_8005B9CC());
            mColor = color;
        } else if (strcmp(field, "DIR") == 0) {
            atn::Vector3 dir;
            dir.x = fn_800DC288(csv->fn_8005B9CC());
            dir.y = fn_800DC288(csv->fn_8005B9CC());
            dir.z = fn_800DC288(csv->fn_8005B9CC());
            mDir = dir;
        } else if (strcmp(field, "COUNT") == 0) {
            int count = fn_800DC12C(csv->fn_8005B9CC());
            mCount = __Clamp_unk(count, lbl_8087E0D0, lbl_8087E0D4);
        } else if (strcmp(field, "SIZE") == 0) {
            mSize = fn_800DC288(csv->fn_8005B9CC());
        } else if (strcmp(field, "SPEED") == 0) {
            float min = fn_800DC288(csv->fn_8005B9CC());
            float max = fn_800DC288(csv->fn_8005B9CC());
            mSpeedMin = min;
            mSpeedMax = max;
        } else if (strcmp(field, "LENGTH") == 0) {
            mLength = fn_800DC288(csv->fn_8005B9CC());
        } else if (strcmp(field, "LENGTHV") == 0) {
            mLengthV = fn_800DC288(csv->fn_8005B9CC());
        } else if (strcmp(field, "BOX") == 0) {
            atn::Vector3 a;
            a.x = fn_800DC288(csv->fn_8005B9CC());
            a.y = fn_800DC288(csv->fn_8005B9CC());
            a.z = fn_800DC288(csv->fn_8005B9CC());
            atn::Vector3 b;
            b.x = fn_800DC288(csv->fn_8005B9CC());
            b.y = fn_800DC288(csv->fn_8005B9CC());
            b.z = fn_800DC288(csv->fn_8005B9CC());
            Box_unk box(a, b);
            mBox.mMin = box.mMin;
            mBox.mMax = box.mMax;
        }
        field = csv->fn_8005B9CC();
    } while (field != 0 && strcmp("", field) != 0);
}

void MapPartsRain::fn_8049D1A4(MapPartsRain* a, MapPartsRain* b) {
    mRain.fn_800B28F8(&a->mRain, &b->mRain);
}

void MapPartsRain::vf1C(void* menu) {
    mRain.fn_800B23C8();
}

MapPartsRain::~MapPartsRain() {}

int MapPartsRain::vf0C() {
    return 1;
}

void MapPartsRain::vf10() {
    if (mOn != 0) {
        mRain.fn_800B2D2C();
    }
}

void MapPartsRain::vf14() {
    if (mOn != 0) {
        mRain.fn_800B1C1C();
    }
}
