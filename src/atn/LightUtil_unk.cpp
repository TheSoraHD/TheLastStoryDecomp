// atn/LightUtil_unk.cpp: lista de luces de la escena y seleccion de las luces de un modelo
// (nombre de fichero inventado).
#include "atn/Light_unk.h"
#include "atn/Memory.h"

extern "C" void fn_800795DC(Light_unk* light); // inferred: updates a light (atn/Light_unk.cpp)

LightSet_unk::LightSet_unk() {
    Clear_unk();
}

void LightSet_unk::fn_80079EFC(Light_unk* light, float dist) {
    int pos = -1;
    for (int i = 0; i < mCount; i++) {
        if (dist < mDist[i]) {
            pos = i;
            break;
        }
    }
    if (pos != -1) {
        for (int i = 3; i > pos; i--) {
            mLight[i] = mLight[i - 1];
            mDist[i] = mDist[i - 1];
        }
        mLight[pos] = light;
        mDist[pos] = dist;
        if (mCount < 4) {
            mCount++;
        }
    } else if (mCount < 4) {
        mLight[mCount] = light;
        mDist[mCount] = dist;
        mCount++;
    }
}

LightList_unk* lbl_8087EEF8;

// inferred: creates the light list
extern "C" void fn_8007A100() {
    lbl_8087EEF8 = new (6, "", "", 0) LightList_unk;
}

// NONMATCHING: 98.73% - the original reads mCount again after the bounds check (here the value is reused)
void LightList_unk::fn_8007A154(int index) {
    if (index < 0 || (int)mCount <= index) {
        return;
    }
    Light_unk* dst = &mData[index];
    Light_unk* end = &mData[mCount];
    if (end - dst - 1 != 0) {
        for (Light_unk* src = dst + 1; src < end; src++, dst++) {
            *dst = *src;
        }
    }
    mCount--;
}

// NONMATCHING: 99.50% - order of the two loop increments (i before the offset in the original)
void LightSet_unk::fn_8007A36C(LightList_unk* list, const atn::Vector3& pos, float radius, int arg) {
    Clear_unk();
    int n = 4;
    if (list->mCount <= 4) {
        n = list->mCount;
    }
    for (int i = 0; i < n; i++) {
        Light_unk* light = &list->mData[i];
        if (light->m04 != 0) {
            fn_80079EFC(light, 0.0f);
        }
    }
}

// NONMATCHING: 99.50% - as fn_8007A36C
void LightSet_unk::fn_8007A41C(LightList_unk* list) {
    Clear_unk();
    int n = 4;
    if (list->mCount <= 4) {
        n = list->mCount;
    }
    for (int i = 0; i < n; i++) {
        Light_unk* light = &list->mData[i];
        if (light->m04 != 0) {
            fn_80079EFC(light, 0.0f);
        }
    }
}

void LightList_unk::fn_8007A4CC() {
    for (unsigned int i = 0; i < mCount; i++) {
        fn_800795DC(&mData[i]);
    }
}
