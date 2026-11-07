#ifndef DB_SKILLPARAM_UNK_H
#define DB_SKILLPARAM_UNK_H

#include "db/ParseUtil_unk.h"

// inferred: one row of /boot/skill_param.csv (0xD0 bytes); the constructor (0x80219EC4) is not
// inline
struct SkillParam_unk : public BulletParam_unk {
    SkillParam_unk();

    int mBC;
    int mC0; // inferred: chant time (HitInfo_unk::mC0)
    int mC4; // inferred: from a field with a two letter prefix, -1 if it is empty
    int mC8;
    int mCC;
};

#endif
