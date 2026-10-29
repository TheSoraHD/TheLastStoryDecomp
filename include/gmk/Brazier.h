#ifndef GMK_BRAZIER_H
#define GMK_BRAZIER_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "atn/SoundRef.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: brazier that rolls when it is pushed (messages 3 and 4), is crushed (message 5, fires a
// bullet) and comes back to its place after "PARAM" 4 frames. Models "MODEL"/"MODEL_CRUSH",
// collision "COLLI_OBJECT", effects "EFP"/"EFP_ROLL"/"EFP_CRUSH". The name is in the DOL
// (__vt__Q23gmk7Brazier).
class Brazier : public GimmickCommon {
public:
    Brazier(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_80406754(atn::Task* parent, int id, int arg);        // inferred: factory
    static GimmickCommon* fn_804067D8(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~Brazier();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf44(GimmickHitMsg_unk* msg);
    virtual int vf60();
    void fn_80407D24(); // inferred: moves while rolling
    int fn_80407E8C();  // inferred: nonzero when it can come back (nobody near its place)
    inline int IsBlocked_unk();

    atn::Model mModel;            // 0xF4: "MODEL"
    atn::Model mModelCrush;       // 0x4C4: "MODEL_CRUSH"
    atn::ColliObject mColli;     // 0x894: "COLLI_OBJECT"
    atn::EfpFile mEfp;     // 0x91C: "EFP"
    atn::EfpFile mEfpRoll; // 0x928: "EFP_ROLL"
    atn::EfpFile mEfpCrush; // 0x934: "EFP_CRUSH"
    atn::SoundRef mSound;        // 0x940
    atn::Vector3 mDir;           // 0x944: inferred: rolling direction
    float mSpeed;                // 0x950: "PARAM" 1
    float mRadius;               // 0x954: "PARAM" 2
    int mBulletId;               // 0x958: "PARAM" 3
    int mWait;                   // 0x95C: "PARAM" 4: inferred: frames before it comes back
    int mTimer;                  // 0x960
    GimmickDesc_unk* mDesc;      // 0x964
    int mEnabled;                // 0x968: inferred: last mDesc->m7C
    int m96C;                    // inferred: unused (the object is 0x970 bytes)
};

} // namespace gmk

#endif
