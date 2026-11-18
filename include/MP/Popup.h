#ifndef MP_POPUP_H
#define MP_POPUP_H

#include "atn/Task.h"
#include "atn/FixedArray.h"

class GimmickCommon;

namespace MP {

// inferred: d2anime task used by the popup (fn_801F3FF8); only the members used here
struct PopupD2A_unk : public atn::Task {
    unsigned char m48[0xFC - 0x48];
    // inferred: flags as atn::D2A_Task::mLoop... (bitfields: the same bits on the Wii and on PC)
    unsigned int mLoop : 1; // 0xFC
    unsigned int mVisible : 1;
    unsigned int mPlaying : 1;
    unsigned int mRepeat : 1;
    unsigned int mFC_rest : 28;
};

// inferred: one popup slot (0xC bytes); names invented
struct PopupSlot_unk {
    PopupSlot_unk() : mTarget(0), mDist(1.0e16f), mDot(0.0f) {}
    GimmickCommon* mTarget; // inferred: item shown in the slot (a gimmick of type 0x12)
    float mDist;
    float mDot;
};

// inferred: item popups of the multiplayer mode (vtable 0x80792A70); singleton (0x8087F840)
class Popup : public atn::Task {
public:
    static Popup* fn_805012C4(atn::Task* parent); // inferred: creates the instance

    Popup(atn::Task* parent);
    virtual ~Popup();
    virtual int vf0C();
    virtual void vf10();

    atn::FixedArray_unk<PopupD2A_unk*, 4> m48;
    PopupSlot_unk m5C[4];
    int m8C;

    static Popup* sInstance;
};

} // namespace MP

#endif
