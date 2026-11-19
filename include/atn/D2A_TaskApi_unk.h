#ifndef ATN_D2A_TASKAPI_UNK_H
#define ATN_D2A_TASKAPI_UNK_H

#include "atn/D2A_Task.h"

// The functions below are members of D2A_Task too, but other files still call them by these
// automatic names (game/CampSaveLoad.cpp, atn/D3Anime.cpp), so they keep them for now.
extern "C" atn::D2A_Task* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // Create
extern "C" void fn_801F4484(atn::D2A_Task* task);                                     // Update
extern "C" void fn_801F465C(atn::D2A_Task* task, int setCurrent);                     // Draw
extern "C" void fn_801F4728(atn::D2A_Task* task, const char* name, const float* v);   // SetParam0
extern "C" atn::D2A_DrawRect fn_801F4E8C(atn::D2A_Task* task, unsigned int id);       // GetRect

#endif
