#ifndef ATN_D2A_TASKINSTANCEC_H
#define ATN_D2A_TASKINSTANCEC_H

#include "atn/D2A_TaskInstance.h"

// The members of D2A_TaskInstanceBase<32, 8> called by other files keep their automatic names
// (atn/D2A_TaskInstanceBase_64_8.cpp).
extern "C" atn::D2A_TaskInstanceBase<32, 8>* fn_801F64D0(atn::Task* parent, const char* path); // Create
extern "C" void fn_801F69A8(atn::D2A_TaskInstanceBase<32, 8>* inst);                           // Update
extern "C" void fn_801F6A38(atn::D2A_TaskInstanceBase<32, 8>* inst);                           // Draw
extern "C" atn::D2A_Data* fn_801F6C10(atn::D2A_TaskInstanceBase<32, 8>* inst);                 // GetData
extern "C" float fn_801F6C2C(atn::D2A_TaskInstanceBase<32, 8>* inst);                          // GetEndFrame
extern "C" void* fn_801F6C38(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name);        // GetTrack_unk
extern "C" void fn_801F6C80(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name, float value);
extern "C" void fn_801F6D7C(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name, int index, float value);
extern "C" void fn_801F6E78(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name, atn::D2A_DrawRect r);
extern "C" void fn_801F7590(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name, atn::D2A_DrawColor c);
extern "C" void fn_801F791C(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name, int index, float value);
extern "C" void fn_801F7DF0(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name, float r, float g, float b);
extern "C" void fn_801F837C(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name, const wchar_t* text);
extern "C" void fn_801F8598(atn::D2A_TaskInstanceBase<32, 8>* inst, const char* name, int value, int digits);

namespace atn {
template <>
inline void D2A_TaskInstanceBase<32, 8>::SetEnd_unk() {
    mFrame = fn_801F6C2C(this);
}
template <>
inline bool D2A_TaskInstanceBase<32, 8>::IsEnd_unk() {
    return mFrame >= fn_801F6C2C(this);
}
} // namespace atn

#endif
