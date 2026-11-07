#ifndef GAME_TACVIEW_H
#define GAME_TACVIEW_H

class SkillEffect;
class CharaTask;
struct HitInfo_unk;

// inferred: 0xC-byte entry of TacView::mEntries (names invented)
struct TacViewEntry_unk {
    CharaTask* mChara;
    HitInfo_unk* mHit;
    unsigned int mState; // inferred: 0, 1 active; bit 1: finished
};

// inferred: tactical view of the battle (game/TacView.cpp, 0x805A6D20-0x805AE26C; instance lbl_8087F9F8).
// Only the members used by the decompiled units are declared; other files still use partial views
// (TacView_unk in game/CharaTask.cpp, chr/ControlCharge.cpp, game/PlayerInput_unk.cpp).
class TacView {
public:
    // inferred: end of the effect of a skill started by the player: clears the flag 0x10000 of its
    // user (CharaTask::m12A8) and calls CharaTask::fn_80178864 (0x805AA4C8; bound by
    // BattleCharaTask::fn_800E2FE0)
    void fn_805AA014(CharaTask* chara, int type); // inferred: type 6 when a redirection ends (AIManager)
    void fn_805AA4C8(SkillEffect* effect);
    void fn_805ADC00();
    void fn_805ADC10();
    // inferred: an entry for the character (and hit, if given) exists; `active` selects the state
    int fn_805ADCC0(CharaTask* chara, HitInfo_unk* hit, int active);
    HitInfo_unk* fn_805ADD3C(CharaTask* chara); // inferred: hit of the active entry of the character
    void fn_805ADD80(CharaTask* chara, HitInfo_unk* hit); // inferred: marks the entries as finished

    // inferred: state 0 (inlined by GameUI::fn_803DCA84); name invented
    bool IsIdle_unk() const { return m48 == 0; }
    // inferred: weak copies in game/GameUI.cpp (0x803D718C, 0x803D7194); names invented
    int GetOpen_unk() const { return mA70; }
    int* GetCursor_unk() { return &m15D8; }

    unsigned char m00[0x48 - 0x00];
    int m48;              // inferred: state (GameUI::fn_803DCA84 compares it with 0 and 2)
    CharaTask* m4C;       // inferred: character under the cursor
    unsigned char m50[0xA70 - 0x50];
    int mA70;             // inferred: nonzero while the view is open
    unsigned char mA74[0x1594 - 0xA74];
    int m1594;
    unsigned char m1598[0x15D8 - 0x1598];
    int m15D8;            // inferred: cursor shown by the view (chr/ControlCharge.cpp)
    unsigned char m15DC[0x15FC - 0x15DC];
    unsigned int mEntryCount;       // 0x15FC
    TacViewEntry_unk mEntries[16];  // 0x1600
    unsigned char m16C0[0x16C8 - 0x16C0];
};

extern "C" TacView* lbl_8087F9F8;

#endif
