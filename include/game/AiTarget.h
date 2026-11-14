#ifndef GAME_AITARGET_H
#define GAME_AITARGET_H

#include <boost/function.hpp>
#include "atn/FixedArray.h"
#include "atn/LH_Binary.h"
#include "atn/Task.h"
#include "game/ResHandle.h"
#include "math/Vec.h"
#include "std/vector.h"

class CharaTask;
class EnemyTask;
class NpcTask;
class PlayerTask;
struct HitInfo_unk;
namespace gs {
struct MovePoint;
}

// inferred: object with a position at +0x10 aimed at by AiTarget type 2 (class unknown)
struct AiTargetObj_unk {
    char pad00[0x10];
    atn::Vector3 m10;
};

// inferred from __vt__8AiTarget (0x80775B18) and the code of game/TargetPos_unk.cpp
// (AIManager keeps two of them at +0x424/+0x458). Size 0x34.
class AiTarget {
public:
    AiTarget() { fn_8003E918(); }
    virtual ~AiTarget() {}

    void fn_8003E918();               // inferred: reset (type 4 = none)
    atn::Vector3 fn_8003E950() const; // inferred: aimed position (target + offset)

    CharaTask* m04;   // inferred: owner (fn_800185B4)
    int m08;          // inferred: type (0 chara, 2 object, 3 fixed position, 4 none)
    CharaTask* m0C;   // inferred: target (AiTargetObj_unk* for type 2)
    atn::Vector3 m10; // inferred: fixed position (type 3)
    atn::Vector3 m1C; // inferred: offset added to the target position
    HitInfo_unk* m28; // inferred: hit redirected (fn_800185B4)
    int m2C;
    int m30;
};

// inferred: list of targets; the destructor of AIManager tests the address of the list twice before
// destroying the elements, so the fixed array is wrapped in a class (name invented)
class AiTargetList_unk : public atn::FixedArray_unk<AiTarget, 16> {};

// inferred: input block of the AI scripts (lbl_807C68C0, 0x180 bytes, returned by fn_8001C260; defined in
// AI/AI_archer_attack01.cpp with its own view AiInput_unk). Filled by AIManager::fn_80017128. Names invented.
struct AiScriptInput_unk {
    CharaTask* mChara;      // 0x00
    int mState;             // 0x04: CharaC58_unk::m00
    int mDone;              // 0x08: CharaC58_unk::mDone_unk
    int mFrame;             // 0x0C: CharaC58_unk::m04
    atn::Vector3 mPos;      // 0x10
    float mTargetDist;      // 0x1C: distance to mTarget minus its radius (9999: none)
    CharaTask* mTarget;     // 0x20: CharaC58_unk::mC4
    float mFollowDist;      // 0x24 (9999: none)
    CharaTask* mFollow;     // 0x28: CharaC58_unk::mCC
    int mHasGimmick;        // 0x2C
    int m30[3];             // 0x30: CharaRes_unk::m58-m60
    int m3C[3];             // 0x3C: CharaRes_unk::m30-m38 (times 3 in hard mode for enemies)
    int m48[4];             // 0x48: CharaRes_unk::m44, m48, m3C, m40
    int mLife;              // 0x58: percent of m04 over mParamB.m84
    int mMana;              // 0x5C: percent of m08 over mParamB.m88
    int m60[4];             // 0x60
    int m70[3];
    int m7C[9];             // 0x7C: CharaRes_unk::m64-m84 (floats)
    int mA0[2];
    int mA8[24];            // 0xA8: CharaC58_unk::m140 (floats)
    int m108;
    int m10C[16];           // 0x10C: AIManager::m9C
    int m14C[9];
    int m170[4];            // 0x170: CharaC58_unk::m1A0
};
// inferred: output block of the AI scripts (lbl_807C6A40, 0xF0 bytes, returned by fn_8001C26C). Names invented.
struct AiScriptOutput_unk {
    int mValid;    // 0x00: > 0 when the script ran
    int mState;    // 0x04
    int m08;
    int mPart;     // 0x0C: part to hold (-1: none)
    int mC0;       // 0x10
    int m14;
    int m18[24];   // 0x18: back to CharaC58_unk::m140
    int m78;
    int m7C[16];   // 0x7C: back to AIManager::m9C
    int mBC[9];
    int mE0[4];    // 0xE0: back to CharaC58_unk::m1A0
};

// inferred: AI manager (instance lbl_8087EE68, code in game/AiTarget.cpp; __vt__9AIManager 0x80775AE0,
// size 0x7E0 from the new of fn_800144FC)
class AIManager : public atn::Task {
public:
    AIManager(atn::Task* parent);
    virtual ~AIManager();
    virtual int vf0C();
    virtual void vf10();

    static AIManager* fn_800144FC(atn::Task* parent); // inferred: creates the singleton

    void fn_80016888();
    void fn_80016A94(CharaTask* chara, boost::function<int(gs::MovePoint*)> filter);
    // inferred: random move point weighted by its fields, of group `group` or accepted by `filter` (the
    // invoker is called with the point and its result is tested; the signature is inferred)
    int fn_80016C10(int group, boost::function<int(gs::MovePoint*)> filter);
    void fn_80016FB4(); // inferred: clears the targets and the target counters of the characters
    void fn_80017060();
    int fn_80017064(CharaTask* chara); // inferred: runs the AI of a character m98 times
    int fn_80017128(CharaTask* chara);
    int fn_8001794C(CharaTask* chara);
    void fn_80017E90(CharaTask* chara, CharaTask* target); // inferred: adds hate (BattleTask::fn_80102A40)
    void fn_80017EB0(CharaTask* chara, int gimmick, int arg); // inferred: makes a character use a gimmick
    int fn_80017F18(CharaTask* chara, atn::Vector3* pos);
    int fn_80018078(CharaTask* chara);
    int fn_80018240(CharaTask* chara, atn::Vector3* pos, float* radius);
    AiTarget* fn_80018394(CharaTask* chara);
    void fn_800183E0(CharaTask* chara, CharaTask* target, HitInfo_unk* hit, int frames);
    void fn_800184F4(const AiTarget* target);
    void fn_80018608(CharaTask* chara, HitInfo_unk* hit);
    AiTarget* fn_800185B4(CharaTask* chara, HitInfo_unk* hit); // inferred: redirection of `chara`
    void fn_80018834();
    int fn_80018BAC(CharaTask* chara);
    // inferred: random move point seen from `pos` in the direction `dir` (angle `range` in degrees) between
    // the distances min and max, linked to the nearest point (parameters: pos, group, arg3, arg4, arg5, mask,
    // arg7, dir, range, min, max; the names are left out of the declaration)
    int fn_80018D5C(const atn::Vector3*, int, int, int, int, int, int, float, float, float, float);

    int m48;
    int m4C;
    int m50; // inferred: "DebugMode" (Mindows)
    int m54; // inferred: "DrawHate" (Mindows)
    int m58;
    atn::LH_Binary m5C;
    atn::LH_Binary m64;
    std::vector<int> m6C; // inferred: element type unknown (no destructor)
    int m78;
    PlayerTask* m7C; // inferred: iterators of the character lists (fn_80016888)
    CharaTask* m80;
    NpcTask* m84;
    int m88;
    EnemyTask* m8C;
    int m90;
    float m94;
    int m98;
    float m9C[16]; // inferred: values kept between the AI script steps (fn_80017128)
    int mDC;
    AiTargetList_unk mE0;
    AiTarget m424;
    AiTarget m458;
    AiTargetList_unk m48C; // inferred: redirections (fn_800185B4)
    int m7D0; // "AttackTargetFrame"
    int m7D4; // "SupportTargetFrame"
    int m7D8; // "OffensiveWithTarget"
    ResHandle mMenu; // 0x7DC
};
extern "C" AIManager* lbl_8087EE68;

#endif
