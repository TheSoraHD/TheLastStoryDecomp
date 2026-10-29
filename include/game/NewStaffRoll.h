#ifndef GAME_NEWSTAFFROLL_H
#define GAME_NEWSTAFFROLL_H

#include "atn/Task.h"
#include "atn/LH_Binary.h"
#include "atn/Array_unk.h"
#include "atn/Texture_unk.h"
#include "game/ResHandle.h"

// inferred: atn::FadeColor returned by atn::FadeColor::Create (0x8006A250); names invented
struct StaffRollFade_unk {
    unsigned char m00[0x48];
    int m48;
    unsigned char m4C[0x58 - 0x4C];
    int m58;
    unsigned char m5C[0x68 - 0x5C];
    int m68;
    unsigned char m6C[0x74 - 0x6C];
    float m74;
};

// inferred from __vt__12NewStaffRoll (0x80793888): scrolls the staff roll read from
// "database/staffroll/staffroll_type1_xx.u16" (one line per CSV row)
class NewStaffRoll : public atn::Task {
public:
    // inferred: line of the staff roll (0x110 bytes)
    struct Line_unk {
        Line_unk() { mText[0] = 0; } // 0x80531810
        ~Line_unk() {}               // 0x805311D8
        int mType;         // inferred: 1 FONT, 2 COPYRIGHT (logo), 3 BRAKE, 4 END
        float mY;          // 0x04
        float mSize;       // 0x08
        int mAlign;        // 0x0C: inferred: CENTER, LEFT, RIGHT, LEFT_R, RIGHT_L
        wchar_t mText[0x80]; // 0x10
    };

    NewStaffRoll(atn::Task* parent); // 0x8053108C
    virtual ~NewStaffRoll();         // 0x80531218
    virtual int vf0C();              // 0x80531338
    virtual void vf10();             // 0x8053181C
    virtual void vf14();             // 0x805319C4

    void fn_80531394(); // inferred: reads the lines

    float mPos;      // 0x48: inferred: scroll position
    float mSpeed;    // 0x4C
    float mBrake;    // 0x50: inferred: position of the BRAKE line (the speed decreases after it)
    float mWait;     // 0x54
    float mStopPos;  // 0x58
    float mStopTime; // 0x5C
    float mEndTime;  // 0x60
    atn::LH_Binary mLoader;          // 0x64
    atn::SimpleArray<Line_unk> mLines; // 0x6C
    int mDrawOff;    // 0x74: inferred: debug switch "DrawOff"
    ResHandle mMenu; // 0x78: inferred: debug menu "StaffRoll"
    atn::Texture_unk mLogo; // 0x7C: inferred: "ui_logo_nm.texture"
    StaffRollFade_unk* mFade; // 0xAC
};

#endif
