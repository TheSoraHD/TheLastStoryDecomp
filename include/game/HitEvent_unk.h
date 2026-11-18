#ifndef GAME_HITEVENT_UNK_H
#define GAME_HITEVENT_UNK_H

class CharaTask;

// inferred: data of the events 6 and 7 of the character listeners (a hit computed by fn_8003F1E4 and
// fn_8003EA3C; game/DamageCalc_unk.cpp and atn/PacketPack); names invented. Not the event 15 data of
// game/BattleTask.cpp nor the event 14 data of game/CharaTask.cpp (other layouts with the same local name)
struct HitEvent_unk {
    CharaTask* mAttacker;
    CharaTask* mTarget;
    int mId;
    unsigned int mFlags;
    int mFlag31;
    float mRate;
};

#endif
