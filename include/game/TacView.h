#ifndef GAME_TACVIEW_H
#define GAME_TACVIEW_H

#include "atn/FixedArray.h"
#include "atn/Task.h"
#include "game/AiTarget.h"
#include "math/Vec.h"

class SkillEffect;
class CharaTask;
struct HitInfo_unk;

// inferred: 0xC-byte entry of TacView::mEntries (names invented)
struct TacViewEntry_unk {
    TacViewEntry_unk() {}
    TacViewEntry_unk(CharaTask* chara, HitInfo_unk* hit, unsigned int state) : mChara(chara), mHit(hit), mState(state) {}
    CharaTask* mChara;
    HitInfo_unk* mHit;
    unsigned int mState; // inferred: 0, 1 active; bit 1: finished
};

// inferred: character entry of TacView (0x98 bytes; TacView + 0x54 and the 16 entries at 0xF0). Names invented.
struct TacViewChara_unk {
    atn::Vector3 fn_805AE138() const; // inferred: position of the entry
    void fn_805AE188();               // inferred: reset

    int mType;                // inferred: 0 character, 1 object
    CharaTask* mChara;
    AiTargetObj_unk* mObj;
    int m0C;
    int m10;
    int m14;
    AiTarget m18;
    int m4C;
    unsigned char m50[4];
    unsigned char m54[0x10];
    int m64[4];
    int m74[4];
    int m84[4];
    int m94;
};

// inferred: object aimed at by the cursor of TacView (names invented); m08 is a character
struct TacViewTarget_unk {
    int m00;
    int m04;
    CharaTask* m08;
};
// inferred: cursor state of TacView (1 on a target, 2 none; names invented)
struct TacViewSel_unk {
    int mState;
    int m04;
    TacViewTarget_unk* mTarget;
};

// inferred: tactical view of the battle (game/TacView.cpp, 0x805A6D20-0x805AE26C; instance lbl_8087F9F8).
// Only the members used by the decompiled units are declared; other files still use partial views
// (TacView_unk in game/CharaTask.cpp, chr/ControlCharge.cpp, game/PlayerInput_unk.cpp).
class TacView : public atn::Task {
public:
    TacView(atn::Task* parent); // 0x805A7158
    static TacView* fn_805A70F0(atn::Task* parent); // inferred: creates the singleton
    // inferred: end of the effect of a skill started by the player: clears the flag 0x10000 of its
    // user (CharaTask::m12A8) and calls CharaTask::fn_80178864 (0x805AA4C8; bound by
    // BattleCharaTask::fn_800E2FE0)
    void fn_805AA014(CharaTask* chara, int type); // inferred: type 6 when a redirection ends (AIManager)
    int fn_805AA390(CharaTask* chara); // inferred: kind of the charged skill (1 normal, 2 special, 3 other)
    void fn_805AA4C8(SkillEffect* effect);
    void fn_805AA4E4(); // inferred: opens the view
    void fn_805AA82C(atn::Vector3* pos); // inferred: moves `pos` toward m159C
    void fn_805ADC00();
    void fn_805ADC10();
    // inferred: an entry for the character (and hit, if given) exists; `active` selects the state
    void fn_805ADC20(CharaTask* chara, HitInfo_unk* hit); // inferred: adds (or restarts) the entry of the character
    int fn_805ADCC0(CharaTask* chara, HitInfo_unk* hit, int active);
    HitInfo_unk* fn_805ADD3C(CharaTask* chara); // inferred: hit of the active entry of the character
    void fn_805ADD80(CharaTask* chara, HitInfo_unk* hit); // inferred: marks the entries as finished
    void fn_805ADDD4(CharaTask* chara);
    void fn_805AE034(TacViewTarget_unk* target); // inferred: moves the cursor to `target` (sound when it changes) // inferred: passes the hits of the active entries on, then clears the states
    void fn_805ADF50(CharaTask* chara); // inferred: removes the entries of the character (all with null)

    // inferred: state 0 (inlined by GameUI::fn_803DCA84); name invented
    bool IsIdle_unk() const { return m48 == 0; }
    // inferred: weak copies in game/GameUI.cpp (0x803D718C, 0x803D7194); names invented
    int GetOpen_unk() const { return mA70; }
    int* GetCursor_unk() { return &m15D8; }

    int m48;              // inferred: state (GameUI::fn_803DCA84 compares it with 0 and 2)
    CharaTask* m4C;       // inferred: character under the cursor
    unsigned char m50[0xA70 - 0x50];
    int mA70;             // inferred: nonzero while the view is open
    unsigned char mA74[0x1594 - 0xA74];
    int m1594;
    unsigned char m1598[0x159C - 0x1598];
    atn::Vector3 m159C; // inferred: point the view moves to (fn_805AA82C)
    int m15A8;          // inferred: frames left to move
    unsigned char m15AC[0x15D8 - 0x15AC];
    int m15D8;            // inferred: cursor shown by the view (chr/ControlCharge.cpp)
    unsigned char m15DC[0x15F0 - 0x15DC];
    TacViewSel_unk m15F0; // inferred: target under the cursor (fn_805AE034)
    atn::FixedArray_unk<TacViewEntry_unk, 16> mEntries; // 0x15FC
    unsigned char m16C0[0x16C8 - 0x16C0];
};

extern "C" TacView* lbl_8087F9F8;

#endif
