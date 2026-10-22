#ifndef GAME_EM208QUEEN_H
#define GAME_EM208QUEEN_H

#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__10Em208Queen (0x80786860): the queen of the cannon battle (enemy id 4; the
// cannons of chr/ControlCannonLift.cpp aim at her). The factory allocates 0x1558 bytes
// (SpecialEnemyBase.cpp, case 26). Members keep their offsets (names invented).
class Em208Queen : public EnemyTask {
public:
    Em208Queen(atn::Task* parent, void* data); // 0x802C7BE4
    virtual ~Em208Queen();                     // 0x802C7CBC
    virtual int vf0C();                        // 0x802C7D3C
    virtual void vf10();                       // 0x802C7F10
    virtual void vf14();                       // 0x802C85B0
    virtual void vf20();                       // 0x802C9388
    virtual void vf24();                       // 0x802C9750
    virtual void vf3C(HitInfo_unk* hit);       // 0x802C881C
    virtual void vf40(HitInfo_unk* hit);       // 0x802C8930
    virtual void vf50();                       // 0x802C8A68
    virtual void vf54();                       // 0x802C92D4
    virtual atn::Vector3 vf64(int center);     // 0x802CCAAC
    virtual void vfEC();                       // 0x802CC0F4

    inline void SetState_unk(int state);
    inline void CastSkill_unk();
    inline int CollectPoints_unk(int* ids);
    inline void PlayMotion_unk(int motion);
    void fn_802C9880();
    void fn_802C9BF8(); // inferred: chooses the next action
    void fn_802CA490();
    void fn_802CA8C0();
    void fn_802CAFB8();
    void fn_802CB1CC();
    void fn_802CB43C();
    void fn_802CB74C();
    void fn_802CB8A0();
    void fn_802CC19C(int arg, const atn::Vector3* pos); // inferred: starts state 7 (jump)
    void fn_802CC39C(); // inferred: starts state 14 (releases the eaten character m14D0)
    void fn_802CC4C8(int arg, CharaTask* target); // inferred: starts state 15 (moves to a route point)
    void fn_802CC708(); // inferred: death
    void fn_802CC834(const atn::Vector3& pos, float radius, float angle, int arg); // inferred: pos is not read

    CharaTask* m14B0; // inferred: target (mC58.mC4 of this frame)
    CharaTask* m14B4; // inferred: target of the previous frame
    int m14B8;
    int m14BC;        // inferred: frame counter of the state
    int m14C0;
    int m14C4;        // inferred: hunger level (0, 1, 2)
    int m14C8;
    float m14CC;      // inferred: "NowHungryRate"
    CharaTask* m14D0; // inferred: eaten character
    int m14D4;
    int m14D8;
    atn::Matrix3x4 m14DC; // inferred: matrix of the node "eat_point"
    int m150C;
    int m1510;        // inferred: id of the route point of state 15
    int m1514;
    int m1518;
    atn::Vector3 m151C; // inferred: destination of state 15
    int m1528;
    int m152C;
    atn::Vector3 m1530; // inferred: jump velocity (state 7)
    int m153C;
    atn::Task* m1540;   // inferred: d2anime "ui_point_slash.d2a" (made by fn_801F3FF8)
    int m1544;          // inferred: set when a cannon shot reaches the queen (chr/ControlCannonLift.cpp)
    ResHandle m1548;    // inferred: debug menu group
    int m154C;          // inferred: "DebugDisp"
    float m1550;        // inferred: "Hungry_Jump"
    float m1554;        // inferred: "Hungry_Tackle"
};

#endif
