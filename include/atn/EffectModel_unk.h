#ifndef ATN_EFFECTMODEL_UNK_H
#define ATN_EFFECTMODEL_UNK_H

#include "atn/Model_unk.h"

// inferred: model object of 0x214 bytes with constructor 0x8008A4E0 and virtual destructor
// 0x8008A76C (also declared in atn/LH_Effect.cpp). Its methods are the ones declared on Model_unk
// at 0x8008xxxx (fn_8008AD4C, fn_8008B140, fn_8008CD60...), so it is probably the base of the model
// class; members embed it directly (gmk::SavePoint, gmk::SwingObject, gmk::Chain).
struct EffectModel_unk {
    EffectModel_unk(unsigned int flags); // 0x8008A4E0
    virtual ~EffectModel_unk();          // 0x8008A76C
    // inferred: the same object seen through the Model_unk declarations (name invented)
    Model_unk* GetModel_unk() { return (Model_unk*)this; }
    unsigned char m04[0x214 - 0x4];
};

#endif
