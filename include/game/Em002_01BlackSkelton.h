#ifndef GAME_EM002_01BLACKSKELTON_H
#define GAME_EM002_01BLACKSKELTON_H

#include "atn/EfpHandle_unk.h"
#include "game/EnemyTask.h"
#include "std/vector.h"

class GimmickCommon;

// inferred: characters already carried to a mirror (a class derived from std::vector: its
// destructor tests `this` four times); name invented
struct CharaList_unk : public std::vector<CharaTask*> {
    // inferred: erase of one element (memmove of the rest)
    void Erase_unk(CharaTask** it);
};

// inferred from __vt__20Em002_01BlackSkelton (0x80783E48): skeleton that drags the allies that are
// down to a mirror (gimmick kind 29, gmk::SkeltonsMirror). States in m58C: 0 searching, 1 grabbing,
// 2 walking to the mirror, 3 entering it, 4 dropped.
class Em002_01BlackSkelton : public EnemyTask {
public:
    Em002_01BlackSkelton(atn::Task* parent, void* data);
    virtual ~Em002_01BlackSkelton();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);

    void fn_80247C68(CharaTask* chara); // inferred: sets the event flags of the carried character
    void fn_80247DA4();                 // inferred: state 0
    void fn_80247FB0();                 // inferred: state 2
    void fn_8024845C();                 // inferred: state 3
    void fn_80248854(CharaTask* target, GimmickCommon* mirror); // inferred: grabs `target`
    void fn_80248A4C(int arg);          // inferred: drops the character

    int m14B0;                // inferred: step of the current state
    int m14B4;
    CharaTask* m14B8;         // inferred: carried character
    CharaList_unk m14BC;
    GimmickCommon* m14C8;     // inferred: mirror
    int m14CC;                // inferred: frames before searching
    atn::EfpFile m14D0; // inferred: "/effect/ef_uc023.efp"
};

#endif
