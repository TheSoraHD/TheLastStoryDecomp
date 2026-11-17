#ifndef GAME_HITUTIL_UNK_H
#define GAME_HITUTIL_UNK_H

#include "math/Vec.h"
#include "game/CharaTask.h"

// inferred: rotation z, y, x from the angles `rot` into `*p` (name invented; inlined by fn_800F80B8, fn_800F8574,
// fn_800FC410 and fn_800FCD24 of game/BattleTask.cpp and by atn/PacketPack; the temporaries of the rotations get
// the stack slots of the original)
inline void MakeRotZYX_unk(atn::Matrix3x4* p, const atn::Vector3& rot) {
    p->SetIdentity();
    if (0.0f != rot.z) {
        atn::Matrix3x4::RotateAxis_unk(*p, 'z', rot.z);
    }
    if (0.0f != rot.y) {
        atn::Matrix3x4::RotateAxis_unk(*p, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        atn::Matrix3x4::RotateAxis_unk(*p, 'x', rot.x);
    }
}

// inferred: center of the hit body of `chara`: the middle height of its collision capsule when mFlagB29 is
// set, else its first hit sphere (name invented; game/BattleTask.cpp and atn/PacketPack)
inline atn::Vector3 HitCenter_unk(CharaTask* chara) {
    atn::Vector3 center;
    if (chara->mFlagB29) {
        atn::Vector3 start = chara->mColli.m3C;
        atn::Vector3 end = chara->mColli.m48;
        center = chara->mColli.m3C;
        float half = 0.5f * (end.y - start.y);
        center.y += half;
    } else {
        center = chara->fn_80148B0C(0)->m04.mCenter;
    }
    return center;
}

#endif
