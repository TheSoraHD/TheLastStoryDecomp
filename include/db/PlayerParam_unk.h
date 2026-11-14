#ifndef DB_PLAYERPARAM_UNK_H
#define DB_PLAYERPARAM_UNK_H

// inferred: skills of a player from a level on (0x88 bytes, 32 per player, 7 players; db/PlayerParam_unk.cpp,
// "/boot/player_param.csv"); fn_80219160 returns the entry of a player at a level. The constructor is inline
// in db/PlayerParam_unk.cpp (emitted out of line at 0x80219018 for __construct_new_array); declared here
// without a body, since an inline body takes @N labels in every unit that includes the header.
struct PlayerParam_unk {
    PlayerParam_unk();

    signed char mPlayer;
    signed char mLevel;
    bool mSkills[0x80];    // inferred: skill growth ids learnt (0 for every status bit the level gives)
    signed char mSlots[6]; // inferred: skill growth id of each slot
};

#endif
