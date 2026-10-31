#ifndef ATN_D2A_TASK_H
#define ATN_D2A_TASK_H

#include "atn/Task.h"
#include "atn/D2A_Data.h"
#include "atn/LH_Binary.h"

namespace atn {

// inferred: task that loads and plays a d2anime file (names of the members invented)
class D2A_Task : public Task {
public:
    D2A_Task(Task* parent, const char* path, int loop);               // 0x801F407C
    virtual ~D2A_Task();
    virtual int vf0C();  // inferred: loading
    virtual void vf10(); // inferred: update
    virtual void vf14(); // inferred: draw

    void ApplyFrame();                             // 0x801F45F4
    void SetParam1_unk(const char* name, const float* v);     // 0x801F4818
    void SetParam2_unk(const char* name, const float* v);     // 0x801F48C8
    void SetColor_unk(const char* name, unsigned int color);  // 0x801F4998
    void SetColor_unk(const char* name, float r, float g, float b); // 0x801F4AA0
    void SetColor255_unk(const char* name, const float* rgb); // 0x801F4B4C
    void SetText_unk(const char* name, int textId);           // 0x801F4C14
    void SetNumber_unk(const char* name, int value, int digits); // 0x801F4CB4
    void fn_801F4D80(const char* name, int arg);
    void fn_801F4DDC(const char* name, float arg);
    void fn_801F4E30(const char* name, int arg);
    D2A_DrawColor GetColor_unk(unsigned int id); // 0x801F4F08
    void GetDrawParams_unk(unsigned int id, D2A_DrawRect* rect, D2A_DrawColor* color, D2A_DrawExtra* extra,
                           void* work); // 0x801F4F84
    void SetFloat_unk(const char* name, float value);
    // inferred: weak copies in game/GameUI (0x803D2A54-style setters); names invented
    void SetFrame_unk(float f) { mFrame = f; }
    void SetSpeed_unk(float s) { mSpeed = s; }
    bool IsEnd_unk() const { // inferred: the frame reached the end frame
        float end = mData.mEndFrame;
        return mFrame >= end;
    }

    D2A_Data mData;  // 0x48
    LH_Binary mFile; // 0xF0
    int mState;      // 0xF8
    int mLoop : 1;   // 0xFC
    int mVisible : 1;
    int mPlaying : 1;
    int mRepeat : 1;
    int mLoaded : 1;
    int mPath : 1;
    float mFrame;    // 0x100
    float mSpeed;    // 0x104
    int mBlend;      // 0x108
    int m10C;
};

} // namespace atn

// The functions below are members of D2A_Task too, but other files still call them by these
// automatic names (game/CampSaveLoad.cpp, atn/D3Anime.cpp), so they keep them for now.
extern "C" atn::D2A_Task* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // Create
extern "C" void fn_801F4484(atn::D2A_Task* task);                                     // Update
extern "C" void fn_801F465C(atn::D2A_Task* task, int setCurrent);                     // Draw
extern "C" void fn_801F4728(atn::D2A_Task* task, const char* name, const float* v);   // SetParam0
extern "C" atn::D2A_DrawRect fn_801F4E8C(atn::D2A_Task* task, unsigned int id);       // GetRect

#endif
