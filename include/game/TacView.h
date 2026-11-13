#ifndef GAME_TACVIEW_H
#define GAME_TACVIEW_H

#include "atn/FixedArray.h"
#include "atn/Task.h"
#include "game/AiTarget.h"
#include "math/Vec.h"
#include "atn/SoundRef.h"
#include <string.h>

class SkillEffect;
namespace atn {
class D2A_Task;
template <int N, int M>
class D2A_TaskInstanceBase;
} // namespace atn
// inferred: stops the sound (defined as extern "C" by atn/SoundManager.cpp, like the other SoundRef
// helpers at 0x800CB5B4-0x800CB688)
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int detach);
class CharaTask;
struct HitInfo_unk;

// inferred: 0xC-byte entry of TacView::mEntries (names invented)
struct TacViewEntry_unk {
    TacViewEntry_unk() : mChara(0), mHit(0), mState(0) {}
    TacViewEntry_unk(CharaTask* chara, HitInfo_unk* hit, unsigned int state) : mChara(chara), mHit(hit), mState(state) {}
    CharaTask* mChara;
    HitInfo_unk* mHit;
    unsigned int mState; // inferred: 0, 1 active; bit 1: finished
};

// inferred: character entry of TacView (0x98 bytes; TacView + 0x54 and the 16 entries at 0xF0). Names invented.
struct TacViewChara_unk {
    TacViewChara_unk() { fn_805AE188(); } // weak copy 0x805A757C
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
    void* m54[4]; // inferred: skill of each command (HitInfo_unk, or SkillParam_unk for the kinds >= 6)
    int m64[4];
    int m74[4];
    int m84[4];
    int m94;
};

// inferred: characters of the view (TacView + 0xEC; the destructor checks the address twice, as
// AiTargetList_unk)
class TacViewCharaList_unk : public atn::FixedArray_unk<TacViewChara_unk, 16> {};

// inferred: object aimed at by the cursor of TacView (names invented); m08 is a character
struct TacViewTarget_unk {
    int m00;
    int m04;
    CharaTask* m08;
};
// inferred: cursor state of TacView (1 on a target, 2 none; names invented)
struct TacViewSel_unk {
    int mState;
    CharaTask* m04; // inferred: character (TacView::m48, compared by GameUI)
    TacViewTarget_unk* mTarget;
};

// inferred: cursor state cleared by its constructor (TacView + 0x15F0 and TacViewSlot_unk); name invented
struct TacViewCursor_unk : public TacViewSel_unk {
    TacViewCursor_unk() {
        mState = 2;
        m04 = 0;
        mTarget = 0;
    }
};

// inferred: 0x2C-byte entry of TacView (64 at 0xA8C; also a local of fn_805AB828); the constructor
// (weak copy 0x805A7600) clears the whole entry after the cursor state. Names invented.
struct TacViewSlot_unk {
    TacViewSlot_unk() { memset(this, 0, sizeof(TacViewSlot_unk)); }
    // inferred: weak copies 0x805AB1B0 and 0x805AB1C0 (fn_805AB828)
    void fn_805AB1B0(CharaTask* chara) {
        mSel.mState = 0;
        mSel.m04 = chara;
    }
    bool fn_805AB1C0() const { return mSel.mState != 2; }
    TacViewCursor_unk mSel;
    // inferred: nearest slot in each of the 8 directions of fn_805A6D20 (filled by fn_805AB828)
    TacViewSlot_unk* mNear[8];
};

// inferred: layout tasks of the view (TacView + 0x15B0, cleared by its constructor); names invented
struct TacViewLayouts_unk {
    TacViewLayouts_unk() { memset(this, 0, sizeof(TacViewLayouts_unk)); }
    atn::D2A_Task* mSelect;  // ui_command_select.d2a
    atn::D2A_Task* mFace;    // ui_command_face.d2a
    atn::D2A_Task* mHpFlash; // ui_command_hp_flash.d2a
    // inferred: ui_select_4_u/l/r/d.d2a (layout anime tasks made by fn_801F64D0)
    atn::D2A_TaskInstanceBase<32, 8>* mDirs[4];
    atn::Task* m1C;
    atn::Task* m20;
};

// inferred: tactical view of the battle (game/TacView.cpp, 0x805A6D20-0x805AE26C; instance lbl_8087F9F8).
// Only the members used by the decompiled units are declared; other files still use partial views
// (TacView_unk in game/CharaTask.cpp, chr/ControlCharge.cpp, game/PlayerInput_unk.cpp).
class TacView : public atn::Task {
public:
    TacView(atn::Task* parent); // 0x805A7158
    virtual ~TacView();         // 0x805A764C
    virtual int vf0C();         // 0x805A76E0
    virtual void vf10();        // 0x805A781C
    virtual void vf14();        // 0x805A96D0
    static TacView* fn_805A70F0(atn::Task* parent); // inferred: creates the singleton
    // inferred: end of the effect of a skill started by the player: clears the flag 0x10000 of its
    // user (CharaTask::m12A8) and calls CharaTask::fn_80178864 (0x805AA4C8; bound by
    // BattleCharaTask::fn_800E2FE0)
    void fn_805AA014(CharaTask* chara, int type); // inferred: type 6 when a redirection ends (AIManager)
    int fn_805AA390(CharaTask* chara); // inferred: kind of the charged skill (1 normal, 2 special, 3 other)
    void fn_805ABE74(); // inferred: commands of the character under the cursor (m54)
    void fn_805AA4C8(SkillEffect* effect);
    void fn_805AA4E4(); // inferred: opens the view
    void fn_805AA734(); // inferred: closes the view
    int fn_805AA824();  // inferred: always 0 (tested by GameUI)
    void fn_805AB828(int mode); // inferred: changes the mode of the view (mA78)
    // inferred: inlined in fn_805AA4E4/fn_805AA734 (name invented)
    void ChangeMode_unk(int mode) {
        if (mA78 != mode) {
            fn_805AB828(mode);
        }
        mA78 = mode;
        fn_800CB5C8(&m16C0, 15, 0);
    }
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
    bool IsIdle_unk() const { return m48.mState == 0; }
    // inferred: weak copies in game/GameUI.cpp (0x803D718C, 0x803D7194); names invented
    int GetOpen_unk() const { return mA70; }
    int* GetCursor_unk() { return &m15D8; }

    // inferred: cursor of the view (state compared with 0 and 2 by GameUI::fn_803DCA84; m04 is the
    // character under the cursor); copied from the entries of mSlots
    TacViewCursor_unk m48;
    TacViewChara_unk m54;
    TacViewCharaList_unk mCharas; // 0xEC
    int mA70;             // inferred: nonzero while the view is open
    int mA74;
    int mA78;
    int mA7C;
    int mA80;
    int mA84;
    atn::FixedArray_unk<TacViewSlot_unk, 64> mSlots; // 0xA88
    int m158C;
    int m1590;
    int m1594;
    int m1598;
    atn::Vector3 m159C; // inferred: point the view moves to (fn_805AA82C)
    int m15A8;          // inferred: frames left to move
    int m15AC;
    TacViewLayouts_unk mLayouts; // 0x15B0
    unsigned char m15D4;
    int m15D8;            // inferred: cursor shown by the view (chr/ControlCharge.cpp)
    atn::Vector3 m15DC;
    float m15E8;
    unsigned char m15EC;
    unsigned char m15ED;
    TacViewCursor_unk m15F0; // inferred: target under the cursor (fn_805AE034)
    atn::FixedArray_unk<TacViewEntry_unk, 16> mEntries; // 0x15FC
    atn::SoundRef m16C0;
    int m16C4;
};

extern "C" TacView* lbl_8087F9F8;

#endif
