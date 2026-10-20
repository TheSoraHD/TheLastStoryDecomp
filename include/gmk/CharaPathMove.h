#ifndef GMK_CHARAPATHMOVE_H
#define GMK_CHARAPATHMOVE_H

#include "game/GimmickCommon.h"
#include "gmk/GmkChara.h"
#include "std/vector.h"

namespace gmk {

class CharaPathMove;

// inferred: character walking along the path of a gmk::CharaPathMove (0x30 bytes, built with new
// in vf20). Names invented.
struct PathChara_unk {
    PathChara_unk(CharaPathMove* owner); // 0x8043E0E8 (weak, out of line)
    void fn_8043E6B0(const char* motion, float speed, std::vector<std::string>* uppers); // inferred: loads the motions
    void fn_8043E770();          // inferred: starts walking from the first point
    void fn_8043EA64(int skip);  // inferred: update (skip = no upper motions)

    CharaPathMove* mOwner;
    int mState;           // 0x04: inferred: 1 while walking
    GmkChara* mChara;     // 0x08
    atn::Vector3 m0C;
    atn::Vector3 m18;
    float mSpeed;         // 0x24
    int mPoint;           // 0x28: inferred: index of the next point
    int mTimer;           // 0x2C: inferred: frames until the next upper motion
};

// inferred: list of the characters (4 null checks of `this` in the destructor: derived from
// std::vector)
struct PathCharaList_unk : public std::vector<PathChara_unk*> {
    int m0C; // inferred: unused (0x10 bytes: the next member is at 0x134)
};
// inferred: data of the path built by fn_8032AF90 from the points (0x54 bytes; element types
// unknown, trivially destructible)
struct PathVecList_unk : public std::vector<atn::Vector3> {};
struct PathData_unk {
    PathData_unk() : m00(0), m04(0.0f) {}
    void fn_8043E07C(); // inferred: reset (weak, out of line)
    int m00;
    float m04;
    atn::Vector3 m08;
    atn::Vector3 m14;
    std::vector<atn::Vector3> m20;
    std::vector<atn::Vector3> m2C;
    std::vector<atn::Vector3> m38;
    PathVecList_unk m44;
    int m50; // inferred: unused (0x54 bytes)
};

// inferred: characters ("CHARA") walking along a path ("PATH_POINT") with a motion ("MOTION") and
// random upper body motions ("UPPER_MOTION", "UPPER_MOTION_INTERVAL"). The name is in the DOL
// (__vt__Q23gmk13CharaPathMove, 0x1A0 bytes).
class CharaPathMove : public GimmickCommon {
public:
    CharaPathMove(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_8043D594(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~CharaPathMove();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    inline void Step_unk();
    inline void UpdateAll_unk(int skip);
    inline void DeleteAll_unk(); // inferred: deletes the characters // inferred: updates the characters // inferred: starts the next character when its turn comes

    int mLoop;      // 0xF4: "PARAM" 1: inferred: loop the path
    int mF8;        // "PARAM" 3: inferred: 0 = simulate 1500 frames on start
    int mInterval;  // 0xFC: "PARAM" 2: inferred: frames between two characters
    int mWait;      // 0x100
    float mSpeed;   // 0x104: "MOVE_SPEED"
    std::string mMotion;                    // 0x108: "MOTION"
    std::vector<std::string> mUpperMotions; // 0x114: "UPPER_MOTION"
    int mNext;      // 0x120: inferred: next character to start
    PathCharaList_unk mCharas;              // 0x124
    std::vector<atn::Vector3> mPoints;      // 0x134: "PATH_POINT"
    PathData_unk mPath;                     // 0x140
    int mUpperInterval; // 0x194: "UPPER_MOTION_INTERVAL"
    int mShowPath;      // 0x198: "ShowPath" (debug)
    int m19C;           // inferred: unused (the object is 0x1A0 bytes)
};

} // namespace gmk

#endif
