#ifndef GAME_EM002SKELTON_H
#define GAME_EM002SKELTON_H

#include "game/EnemyTask.h"

// inferred from __vt__12Em002Skelton (0x807840A8)
class Em002Skelton : public EnemyTask {
public:
    Em002Skelton(atn::Task* parent, void* data);
    virtual ~Em002Skelton();
    virtual int vf0C();
    virtual void vfE8(HitMsg_unk* msg);
    virtual void vfF0();
    virtual void vf44();

    int m14B0; // inferred: hits left before the skeleton falls apart (CharaData_unk::mParam.m98)
    int m14B4; // inferred: frame counter
    atn::EfpFile m14B8; // inferred: "/effect/ef_uc001.efp"
    atn::EfpFile m14C4; // inferred: "/effect/ef_cm009.efp"
    int m14D0;
};

#endif
