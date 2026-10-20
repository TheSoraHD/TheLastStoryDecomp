#ifndef GMK_ONOFF_H
#define GMK_ONOFF_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "atn/ResLoader_unk.h"
#include "game/GimmickCommon.h"

// inferred: group of map parts returned by fn_8049D68C (see gmk/ArenaFloor.cpp)
struct OnOffMap_unk;

namespace gmk {

// inferred: switchable object (messages 1 off, 2 on, 3 off with an effect): model "MODEL",
// collisions "COLLI_OBJECT"/"COLLI_BOX"/"COLLI_SPHERE", effects "EFF"/"EFP", map parts "MAP",
// initial state "INIT" and option flags "FLAG". The name is in the DOL (__vt__Q23gmk5OnOff).
class OnOff : public GimmickCommon {
public:
    OnOff(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_803F744C(atn::Task* parent, int id, int arg);        // inferred: factory
    static GimmickCommon* fn_803F74D0(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~OnOff();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual Model_unk* vf38();
    virtual void vf3C(void* lens); // 0x803F940C
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag);
    virtual void vf4C(CharaTask* chara);
    void fn_803F8324(); // inferred: places the collisions
    void fn_803F8C98(atn::CsvTab* csv); // inferred: reads the "FLAG" names
    void fn_803F8F30(int on);           // inferred: switches on or off

    Model_unk mModel;          // 0xF4: "MODEL"
    int mHasModel;             // 0x4C4
    atn::ColliObject mColli;   // 0x4C8: "COLLI_OBJECT"
    int mHasColli;             // 0x550
    atn::ColliBox mBox;        // 0x554: "COLLI_BOX"
    int mHasBox;               // 0x8CC
    atn::ColliSphere mSphere;  // 0x8D0: "COLLI_SPHERE"
    int mHasSphere;            // 0x91C
    ResLoader_unk mEff;        // 0x920: "EFF"
    atn::EfpHandle_unk mEfp;   // 0x92C: "EFP"
    atn::Matrix3x4 mMtx;       // 0x938: inferred: world matrix while on
    OnOffMap_unk* mMap;        // 0x968: "MAP"
    int mOn;                   // 0x96C
    int mMotion;               // 0x970: "MOTION" id
    atn::Vector3 mBoxPos;      // 0x974: "COLLI_BOX" 1-3
    atn::Vector3 mBoxRot;      // 0x980: "COLLI_BOX" 4-6
    atn::Vector3 mBoxSize;     // 0x98C: "COLLI_BOX" 7-9
    atn::Vector3 mSpherePos;   // 0x998: "COLLI_SPHERE" 1-3
    float mSphereRadius;       // 0x9A4: "COLLI_SPHERE" 4
    int mInit;                 // 0x9A8: "INIT": 1 off, 2 on
    unsigned int mFlags;       // 0x9AC: "FLAG"
    GimmickDesc_unk* mDesc;    // 0x9B0
    int m9B4;                  // inferred: unused (the object is 0x9B8 bytes)
};

} // namespace gmk

#endif
