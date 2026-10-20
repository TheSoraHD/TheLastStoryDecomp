#ifndef GMK_BAT_H
#define GMK_BAT_H

#include "atn/Array_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

class Bat;

// inferred: one bat of gmk::Bat (0x42C bytes; constructor and destructor out of line for the
// new[] of the gimmick). It flies along two quadratic curves: start -> mEnd with the control point
// mCtrl. Names invented.
struct BatUnit_unk {
    void (Bat::*mState)(int index); // 0x00: state function
    unsigned int mStateNo;          // 0x0C: 1 waiting, 2 first curve, 3 second curve, 4 gone
    Model_unk mModel;               // 0x10
    atn::Vector3 mPos;              // 0x3E0: offset from the gimmick position
    atn::Vector3 mRot;              // 0x3EC
    atn::Vector3 mStart;            // 0x3F8: start of the curve
    atn::Vector3 mEnd;              // 0x404: end of the curve (from mStart)
    atn::Vector3 mCtrl;             // 0x410: control point (from mStart)
    float mHeight;                  // 0x41C
    float mAngle;                   // 0x420
    float mStep;                    // 0x424
    float mT;                       // 0x428
};

// inferred: bats ("NUM") that wait around the gimmick and fly away past the player when the gimmick
// gets a message or the player is near. The name is in the DOL (__vt__Q23gmk3Bat, 0x110 bytes).
class Bat : public GimmickCommon {
public:
    Bat(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_80402978(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~Bat();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual int vf40(GimmickMsg_unk* msg);
    void fn_80403308(); // inferred: puts the bats back
    void fn_80403BF4(int index); // inferred: state 4 (gone)
    void fn_80403BF8(int index); // inferred: state 1 (waiting)
    void fn_80403BFC(int index); // inferred: state 2 (first curve)
    void fn_80404064(int index); // inferred: state 3 (second curve)
    void fn_80404290(int index); // inferred: starts the flight of a bat

    bool IsLoading_unk();
    void UpdateModel_unk(int index);
    void Update_unk(int index);
    float Move_unk(int index);
    float Bezier_unk(int index);
    void InitFly_unk(int index);
    atn::Vector2_unk Dir_unk();
    void SetCtrl_unk(int index, const atn::Vector3& cam, float h, atn::Vector2_unk& dir);
    void SetEnd_unk(int index, atn::Vector3& cam, float h, atn::Vector2_unk& v);

    int mNum;                            // 0xF4: "NUM"
    unsigned char mStarted;              // 0xF8
    int mDelay;                          // 0xFC
    int mTimer;                          // 0x100
    GimmickDesc_unk* mDesc;              // 0x104
    atn::Buffer_unk<BatUnit_unk> mBats;  // 0x108
};

} // namespace gmk

#endif
