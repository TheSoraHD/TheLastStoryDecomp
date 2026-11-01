#ifndef EVENT_DIALOG_H
#define EVENT_DIALOG_H

#include "atn/Task.h"
#include "atn/Texture_unk.h"
#include "atn/LipSync_unk.h"
#include "math/Vec.h"
#include "MP/D2AMpMenu_unk.h"
#include "Event/Behavior.h"
#include "Event/DialogHead.h"

// Everything here is inferred from Event::Dialog (0x802280B4..0x80229DF0); names are invented.

// inferred: two floats copied with one paired single
struct DialogVec2_unk {
    DialogVec2_unk() {}
    DialogVec2_unk(const DialogVec2_unk& o) { __Vec2Copy(&x, &o.x); }
    DialogVec2_unk& operator=(const DialogVec2_unk& o) {
        __Vec2Copy(&x, &o.x);
        return *this;
    }
    float x, y;
};

// inferred: fog parameters (lbl_8087EFA8 + 0xD4)
struct DialogFog_unk {
    DialogFog_unk()
        : mEnable(1), m04(0), m08(0), m0C(1), m10(200.0f), m14(500.0f), m18(30.0f), m1C(10.0f), m20(0x140), m24(0xE0),
          m28(1.5f), m2C(1.5f) {}
    int mEnable;
    int m04;
    int m08;
    int m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    int m20;
    int m24;
    float m28;
    float m2C;
};

namespace Event {

// inferred: position of a balloon slot (fractions of the screen)
struct DialogPos_unk {
    float x;
    float y;
    int mKind;
};

// inferred from __vt__Q25Event6Dialog (0x807839E0): balloons of the event conversations
class Dialog : public atn::Task {
public:
    struct Head_unk {
        Head_unk() : mKey(0) {}
        unsigned int mKey;
        DialogHead* mHead;
    };
    struct Slot_unk {
        const DialogPos_unk* mPos;
        DialogVec2_unk mXY;
        DialogHead* mHead;
    };

    Dialog(atn::Task* parent);
    static Dialog* fn_802288D0(atn::Task* parent); // inferred: creates the task
    virtual ~Dialog();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    // inferred: the last two arguments (length and an option of Event::ActionDialogSpeaker) are not used
    void fn_80228AF4(int slot, Actor_unk* actor, int kind, int arg, int frames, int option);
    void fn_80228AF8(int slot, Actor_unk* actor, int kind, int arg, int frames, int option); // inferred: shows a balloon
    void fn_80228CA0(int slot, const wchar_t* text, int align, int size, int arg, float f1, float f2,
                     unsigned int color, int mode, int offset); // inferred: adds a text
    void fn_80228FD8(); // inferred: starts the conversation
    int fn_802291BC();
    void fn_80229240(); // inferred: ends the conversation
    void fn_802292AC(int hide);
    void fn_80229414(); // inferred: input
    DialogHead* fn_8022960C(Actor_unk* actor);
    void fn_802297AC(); // inferred: update

    void ResetPos_unk(); // inferred: defined in Dialog.cpp (uses its table sPos)
    static unsigned int MakeKey_unk(Actor_unk* actor, int kind); // inferred: defined in Dialog.cpp
    Head_unk* FindHead_unk(Actor_unk* actor, int kind);

    int m48; // inferred: number of balloons
    Head_unk mHeads[16];
    Slot_unk mSlots[4];
    MP::MpD2A_unk* m10C; // inferred: ui_c_event_button.d2a
    int m110;            // inferred: state
    int m114;
    int m118; // inferred: saved camera mode
    int m11C; // inferred: last slot with a text
    atn::Texture_unk m120;
    int m150; // inferred: fade counter
    DialogFog_unk mFog;
    LipSyncData_unk m184;
    LipSyncKeys_unk m190; // inferred: default keys
    unsigned char m198;
    unsigned char m199;
    int m19C;
};

} // namespace Event

#endif
