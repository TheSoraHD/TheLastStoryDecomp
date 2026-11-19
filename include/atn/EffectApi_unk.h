#ifndef ATN_EFFECTAPI_UNK_H
#define ATN_EFFECTAPI_UNK_H

#include "atn/EffectManager.h"

// inferred: C entry points of the effect manager (atn/EffectManager.cpp). Kept out of atn/EffectManager.h: the
// gimmick and enemy units still declare them with their own argument types (void* owner, colors by value...)
extern "C" void fn_80232B7C(unsigned int owner, int group);
extern "C" void* fn_8023A680(atn::EffectManager* mgr, EffectRes_unk* res, int a, int b, int c, const atn::Vector3* pos,
                             const atn::Vector3* rot, const EffectColor_unk* color, int d, int e, int f, float scale);
extern "C" void fn_8023A02C(atn::EffectManager* mgr, unsigned int owner, int group, unsigned int flags);
extern "C" void fn_80239DAC(atn::EffectManager* mgr, unsigned int owner, int group, int flag);

#endif
