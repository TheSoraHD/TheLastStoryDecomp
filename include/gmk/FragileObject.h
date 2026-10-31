#ifndef GMK_FRAGILEOBJECT_H
#define GMK_FRAGILEOBJECT_H

#include "atn/Array_unk.h"
#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"
#include "gmk/MotionObject.h"

struct Material_unk;

namespace gmk {

// inferred: "MATGLOW material from to range chara": the alpha of a material goes from `from` to `to`
// as the nearest character of kind `chara` (type + id / 10000) comes closer than `range` (0x1C
// bytes; names invented)
struct FragileGlow_unk {
    FragileGlow_unk() : mMaterial(0), mFrom(1.0f), mTo(0.0f), mRange(0.0f), mType(-1), mId(-1), mMode(-1) {}
    void fn_803F42C8(atn::Model* model); // inferred: updates the alpha
    void Read_unk(atn::CsvTab* csv, atn::Model* model); // inferred: name invented

    Material_unk* mMaterial;
    float mFrom;
    float mTo;
    float mRange;
    int mType;
    int mId;
    int mMode; // inferred: draw mode of the material (restored when opaque)
};

// inferred: "TARGET_COLLISION x y z size HP ValidDamage NONE_ACTIVE" (0x28 bytes, cleared with
// memset; names invented)
struct FragileTarget_unk {
    atn::Vector3 mOffset; // 0x00
    unsigned char m0C[0x18 - 0x0C];
    float mSize;          // 0x18
    int mHp;              // 0x1C
    int mValidDamage;     // 0x20: inferred: smaller hits are ignored
    int mNoneActive;      // 0x24
};

// inferred: "LIGHTMAP_BLEND frames": light map blend of a model, counted up or down (names invented)
struct FragileBlend_unk {
    FragileBlend_unk() : mModel(0), mUp(0), mFrames(0), mFrame(0) {}
    atn::Model* mModel;
    int mUp;
    int mFrames;
    int mFrame;
};

// inferred: object that breaks: "MODEL_BEFORE" is shown until it is hit (TARGET_COLLISION HP) or
// crushed (message 4), then "MODEL_AFTER" plays its motion (states 4-6), fires "EXPLODE_PARAM" and
// switches the collisions "COLLISION_BEFORE"/"COLLISION_AFTER" at their frames. The name is in the
// DOL (__vt__Q23gmk13FragileObject, 0xAC8 bytes).
class FragileObject : public GimmickCommon {
public:
    FragileObject(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_803F1BB4(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~FragileObject();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void Start();
    virtual atn::Model* vf38();
    virtual void vf3C(void* lens);
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf44(GimmickHitMsg_unk* msg);
    virtual atn::ColliBase* vf50();
    virtual atn::Vector3 vf54();
    virtual float vf58();
    virtual int vf5C();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    // inferred: names invented
    void UpdateColli_unk();

    atn::ColliObject mColliBefore; // 0xF4: "COLLISION_BEFORE frame name"
    atn::ColliObject mColliAfter;  // 0x17C: "COLLISION_AFTER frame name"
    float mColliBeforeFrame;       // 0x204: inferred: frame of MODEL_AFTER that removes it
    float mColliAfterFrame;        // 0x208: inferred: frame of MODEL_AFTER that enables it
    int mHasColliBefore;           // 0x20C
    int mHasColliAfter;            // 0x210
    atn::ColliSphere mSphere;      // 0x214
    FragileTarget_unk mTarget;     // 0x260
    int mHasTarget;                // 0x288
    atn::Model mModelAfter;        // 0x28C: "MODEL_AFTER"
    atn::Model mModelBefore;       // 0x65C: "MODEL_BEFORE"
    atn::EfpFile mEfp;             // 0xA2C: "EFP"
    atn::EfpFile mEfpBefore;       // 0xA38: "EFP_BEFORE"
    atn::EfpFile mEfpWait;         // 0xA44: "EFP_WAIT"
    int mExplode;                  // 0xA50: "EXPLODE_PARAM": bullet id
    float mExplodeScale;           // 0xA54
    int mExplodeElza;              // 0xA58: inferred: the bullet belongs to the player
    int mCrushFrames;              // 0xA5C: "CRUSH_PARAM"
    int mCrushFrame;               // 0xA60
    float mDrawOffWait;            // 0xA64
    unsigned int mCrushFlags;      // 0xA68: 1 NO_AUTO_CRUSH, 2 AUTO_DRAW_OFF, 4 LOOP_CRUSH
    atn::SimpleArray<GimmickMaterialFade_unk> mFades;  // 0xA6C: "MATALPHA"
    atn::SimpleArray<GimmickMotionSwitch_unk> mSwitches; // 0xA74: "NODEVIS"
    atn::SimpleArray<FragileGlow_unk> mGlows;          // 0xA7C: "MATGLOW"
    FragileBlend_unk mBlendAfter;  // 0xA84: "LIGHTMAP_BLEND"
    FragileBlend_unk mBlendBefore; // 0xA94
    String32_unk mBoundNode;       // 0xAA4: "BOUND_CENTER_NODE"
    int mAC4; // inferred: unused (the object is 0xAC8 bytes)
};

} // namespace gmk

#endif
