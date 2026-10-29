#ifndef ATN_EFFECTMODEL_UNK_H
#define ATN_EFFECTMODEL_UNK_H

#include "atn/Model_unk.h"

// inferred: model object of 0x214 bytes with constructor 0x8008A4E0 and virtual destructor
// 0x8008A76C (also declared in atn/LH_Effect.cpp). Its methods are the ones declared on atn::Model
// at 0x8008xxxx (fn_8008AD4C, fn_8008B140, fn_8008CD60...), so it is probably the base of the model
// class; members embed it directly (gmk::SavePoint, gmk::SwingObject, gmk::Chain).
struct EffectModel_unk {
    // inferred: arrays of models use the default flags 9 (default constructor 0x803165E4 in
    // game/Em316LaserSphere.cpp)
    EffectModel_unk(unsigned int flags = 9); // 0x8008A4E0
    virtual ~EffectModel_unk();          // 0x8008A76C
    // inferred: the same object seen through the atn::Model declarations (name invented)
    atn::Model* GetModel_unk() { return (atn::Model*)this; }
    unsigned char m04[0x214 - 0x4];
};

#endif
