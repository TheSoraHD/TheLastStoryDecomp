#ifndef ATN_LIGHT_UNK_H
#define ATN_LIGHT_UNK_H

#include "math/Vec.h"

// inferred: engine light (0x44 bytes; constructor 0x80079044, atn/Light_unk.cpp). Names invented.
struct LightColor_unk {
    float r, g, b, a;
};

struct Light_unk {
    Light_unk();

    int m00;
    int m04; // inferred: type (0 = off; 1 directional, 2 spot, 3 point, 4 ambient?)
    atn::Vector3 m08;
    atn::Vector3 m14;
    int m20;
    float m24;
    float m28;
    float m2C;
    float m30;
    LightColor_unk m34;
};

// inferred: list of the scene lights (singleton lbl_8087EEF8, created by fn_8007A100)
struct LightList_unk {
    LightList_unk() : mData(0), mCount(0), mCapacity(0) {}
    void fn_8007A154(int index); // inferred: removes a light
    void fn_8007A4CC();          // inferred: updates every light

    Light_unk* mData;
    unsigned int mCount;
    unsigned int mCapacity;
};

// inferred: up to four lights sorted by distance (0x24 bytes)
struct LightSet_unk {
    LightSet_unk();
    void Clear_unk() {
        mCount = 0;
        for (int i = 0; i < 4; i++) {
            mLight[i] = 0;
            mDist[i] = 1e16f;
        }
    }
    void fn_80079EFC(Light_unk* light, float dist); // inferred: inserts a light
    void fn_8007A36C(LightList_unk* list, const atn::Vector3& pos, float radius, int arg);
    void fn_8007A41C(LightList_unk* list);

    int mCount;
    Light_unk* mLight[4];
    float mDist[4];
};

extern "C" LightList_unk* lbl_8087EEF8;

#endif
