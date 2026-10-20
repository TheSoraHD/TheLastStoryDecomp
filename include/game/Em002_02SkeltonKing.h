#ifndef GAME_EM002_02SKELTONKING_H
#define GAME_EM002_02SKELTONKING_H

#include "game/EnemyTask.h"
#include "atn/ColliBase.h"

// inferred from __vt__19Em002_02SkeltonKing (0x80783F78): grabs other skeletons and throws them
class Em002_02SkeltonKing : public EnemyTask {
public:
    Em002_02SkeltonKing(atn::Task* parent, EnemyInfo_unk* info); // 0x80248E88
    virtual ~Em002_02SkeltonKing();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf20();
    virtual void vf3C(HitInfo_unk* hit);

    CharaTask* fn_80249224(); // inferred: nearest skeleton that can be grabbed
    void fn_80249394();       // inferred: state 6, walks to the target
    void fn_80249588();       // inferred: state 7, grabs and throws it
    // inferred: point in front of the king (inlined; name invented)
    inline atn::Vector3 GetFront_unk(float dist);

    atn::ColliSphere mHead; // 0x14B0: inferred: head collision ("neck" node)
    CharaTask* mTarget;     // 0x14FC
    CharaTask* mLastTarget; // 0x1500: inferred: skipped by fn_80249224
    int mWalkTime;          // 0x1504: inferred: frames spent walking to mTarget (gives up at 300)
};

#endif
