#ifndef GMK_CANNONBALL_H
#define GMK_CANNONBALL_H

#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "atn/SoundRef.h"
#include "game/GimmickCommon.h"
#include "gmk/DropItem.h"

class CharaTask;

namespace atn {
class ColliCapsule;
}

namespace gmk {

// inferred: cannonball (gimmick type 7) that a character carries ("rightwepoint" node), throws
// (message 5: to a target, message 6: in a number of frames) or drops (message 3); it explodes
// after "EXPLODE_PARAM" 1 frames firing the bullet "EXPLODE_PARAM" 2 and comes back after
// "RESET_PARAM" frames (message 8). It owns a gmk::DropItem (id 18000) when desc->m24 > 0. Model
// "MODEL"/"MODEL_LOD", effects "EFP" (explosion, fuse). The name is in the DOL
// (__vt__Q23gmk10Cannonball).
class Cannonball : public GimmickCommon {
public:
    Cannonball(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_803F9F34(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~Cannonball() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual atn::Model* vf38() { return &mModel; }
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag);
    virtual int vf60();
    virtual void vf70(int state);
    // inferred: vtable slot 0x4C. Here it takes the character that uses the cannonball, but
    virtual void vf4C(CharaTask* chara); // 0x803FB800
    float fn_803FB830(); // inferred: radius of the explosion bullet (chr::ControlCannonLift)
    void fn_803FB870(const atn::Vector3* pos, CharaTask* owner, int flag); // inferred: explodes
    void fn_803FBC34(const atn::Vector3* pos); // inferred: hits a wall while thrown
    void SetM520_unk(unsigned char value) { m520 = value; } // inferred: name invented
    // inferred: name invented; its out-of-line copy has the code (and the address) of
    // CharaObj1208_unk::fn_8013A18C, which is probably the same accessor
    int IsShown_unk() const { return m9C; }

    atn::Model mModel;              // 0xF4: "MODEL"
    GimmickDesc_unk* mDesc;        // 0x4C4
    CharaTask* mHolder;            // 0x4C8: inferred: character that carries it
    atn::ColliCapsule* mIgnore;    // 0x4CC: inferred: collision ignored while thrown
    int mTimer;                    // 0x4D0
    float mFallSpeed;              // 0x4D4
    atn::Vector3 mTarget;          // 0x4D8: inferred: target of the throw
    atn::Vector3 mVel;             // 0x4E4
    int mCanExplode;               // 0x4F0
    int mExplodeTime;              // 0x4F4: "EXPLODE_PARAM" 1
    int mBulletId;                 // 0x4F8: "EXPLODE_PARAM" 2
    int mResetTime;                // 0x4FC: "RESET_PARAM"
    atn::EfpFile mEfp;       // 0x500: "EFP" 1: inferred: explosion
    atn::EfpFile mEfpFuse;   // 0x50C: "EFP" 2
    atn::SoundRef mSound;          // 0x518
    DropItem* mDrop;               // 0x51C
    unsigned char m520;
    int m524;
};

} // namespace gmk

#endif
