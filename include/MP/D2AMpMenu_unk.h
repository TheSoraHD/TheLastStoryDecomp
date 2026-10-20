#ifndef MP_D2AMPMENU_UNK_H
#define MP_D2AMPMENU_UNK_H

// Shared declarations of the multiplayer menus (MP/D2AMpMenu*.cpp, MP/D2ACommonDialog.cpp).
// Everything here is inferred from the uses in those files; names are invented.

#include "atn/Task.h"
#include "atn/FixedArray.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "std/string.h"

namespace atn {
// inferred: the d2anime task, declared here with only the members used by the menus
// (atn/D2A_Task.h declares fn_801F3FF8 with another return type, as game/MenuCommonTask.cpp notes)
class D2A_Task : public Task {
public:
    void SetColor_unk(const char* name, unsigned int color);          // 0x801F4998
    void SetColor_unk(const char* name, float r, float g, float b);   // 0x801F4AA0
    void SetNumber_unk(const char* name, int value, int digits);      // 0x801F4CB4
};
} // namespace atn

namespace MP {

// inferred: d2anime task (made by fn_801F3FF8, same layout as atn::D2A_Task); only the members
// used by the menus
struct MpD2A_unk : public atn::D2A_Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0xA0 - 0x58]; // 0x58: inferred: layout (atn::D2A_Data + 0x10)
    float mLastFrame;                   // 0xA0
    unsigned char mA4[0xFC - 0xA4];
    unsigned int mLoop : 1;             // 0xFC (as atn::D2A_Task)
    unsigned int mVisible : 1;
    unsigned int mPlaying : 1;
    unsigned int mRepeat : 1;
    unsigned int mFC_rest : 28;
    // inferred: name invented (as MenuD2A_unk::IsEnd_unk in game/MenuCommonTask.h)
    bool IsEnd_unk() {
        float last = mLastFrame;
        return mFrame >= last;
    }
    float mFrame;                       // 0x100
    float mSpeed;                       // 0x104
    int m108;
};

// inferred: rectangle of a d2anime pane (as atn::D2A_DrawRect)
struct MpRect_unk {
    MpRect_unk() : x(0.0f), y(0.0f), w(0.0f), h(0.0f), z(0.0f) {}
    float x, y, w, h, z;
};

// inferred: message table (lbl_8087F86C): {id, text} entries from 0x48
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct MpMsgTable_unk {
    struct Entry {
        int mId;
        const wchar_t* mText;
    };
    const wchar_t* Get_unk(int id) { return mEntries[id].mText != 0 ? mEntries[id].mText : lbl_808813D0; }
    unsigned char m00[0x48];
    Entry mEntries[1]; // 0x48
};

} // namespace MP

extern "C" MP::MpD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop);
extern "C" MP::MpRect_unk fn_801F4E8C(MP::MpD2A_unk* task, unsigned int hash);
extern "C" void fn_801F4728(MP::MpD2A_unk* task, const char* name, const MP::MpRect_unk* rect);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void* fn_801FEC74(void* layout, const char* name); // inferred: finds a pane
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FEDBC(void* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" atn::SoundRef fn_80117228(int id); // inferred: plays a system sound
// inferred: pad input
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" MP::MpMsgTable_unk* lbl_8087F86C;
// inferred: text of a pane (returned by value) and its width with the font lbl_8087EEC8
extern "C" std::basic_string<wchar_t> fn_800A4450(void* pane, int a, int b);
extern "C" void* lbl_8087EEC8;
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b);

namespace MP {

inline MpRect_unk GetPaneRect_unk(MpD2A_unk* task, const char* name) {
    return fn_801F4E8C(task, fn_800DC6B4(name));
}
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline void SetPaneValue2_unk(void* layout, const char* name, int index, float value) {
    fn_801FEDBC(layout, fn_800DC6B4(name), index, value);
}
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}
// inferred: starts playing an anime at `speed`
inline void Play_unk(MpD2A_unk* task, int repeat, float speed) {
    if (task != 0) {
        task->fn_800D246C(0);
        task->mSpeed = speed;
        task->mRepeat = repeat;
    }
}

// inferred: stops an anime on its first frame
inline void Stop_unk(MpD2A_unk* task) {
    if (task != 0) {
        task->fn_800D246C(1);
        task->mSpeed = -1.0f;
        task->mFrame = 0.0f;
    }
}

} // namespace MP

#endif
