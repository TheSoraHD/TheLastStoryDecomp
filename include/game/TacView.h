#ifndef GAME_TACVIEW_H
#define GAME_TACVIEW_H

class SkillEffect;

// inferred: tactical view of the battle (game/TacView.cpp, 0x805A6D20-0x805AE26C; instance lbl_8087F9F8).
// Only the members used by the decompiled units are declared; other files still use partial views
// (TacView_unk in game/CharaTask.cpp, chr/ControlCharge.cpp, game/PlayerInput_unk.cpp).
class TacView {
public:
    // inferred: end of the effect of a skill started by the player: clears the flag 0x10000 of its
    // user (CharaTask::m12A8) and calls CharaTask::fn_80178864 (0x805AA4C8; bound by
    // BattleCharaTask::fn_800E2FE0)
    void fn_805AA4C8(SkillEffect* effect);
};

extern "C" TacView* lbl_8087F9F8;

#endif
