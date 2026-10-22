#ifndef GAME_EM315COFFIN_H
#define GAME_EM315COFFIN_H

#include "atn/EfpHandle_unk.h"
#include "atn/ResLoader_unk.h"
#include "game/ResHandle.h"
#include "atn/Model_unk.h"
#include "game/SpecialEnemyBase.h"

// inferred: skill with a cool time (id, interval and counter; 0x803140B4 is the weak copy of Reset_unk);
// names invented
struct CoffinSkill_unk {
    CoffinSkill_unk() : mId(0), m04(0), mInterval(0), mCount(0) {}
    void Reset_unk() { mCount = mInterval; }
    void Tick_unk() {
        if (mCount > 0) {
            mCount--;
        }
    }

    int mId;       // inferred: skill id
    int m04;
    int mInterval; // inferred: cool time in frames
    int mCount;    // inferred: frames left
};

// inferred: value interpolated over a number of frames (fade of the material "em315_02"); names invented
struct CoffinFade_unk {
    CoffinFade_unk(const float& from, const float& to)
        : mTime(0.0f), mDuration(0.0f), mSpeed(1.0f), mActive(0), mFrom(from), mTo(to), mValue(from),
          mDelta(mTo - mFrom) {}
    void End_unk() {
        if (mSpeed >= 0.0f) {
            mValue = mTo;
            mTime = mDuration;
        } else {
            mValue = mFrom;
            mTime = 0.0f;
        }
        mActive = 0;
    }
    void FadeTo_unk(const float& to, float frames) {
        mTime = 0.0f;
        mDuration = frames;
        mSpeed = 1.0f;
        mActive = 1;
        mFrom = mValue;
        mTo = to;
        mDelta = mValue - mFrom;
        if (mDuration <= 0.0f) {
            End_unk();
        }
    }
    void Lerp_unk() {
        float ratio = mTime / mDuration;
        float range = mTo - mFrom;
        float prev = mValue;
        float d = ratio * range;
        mValue = mFrom + d;
        mDelta = mValue - prev;
    }
    bool IsActive_unk() const { return mActive; }
    void Update_unk() {
        if (!mActive) {
            return;
        }
        mTime += mSpeed;
        if (mSpeed >= 0.0f) {
            if (mTime < 0.0f) {
                mValue = mFrom;
                mTime = 0.0f;
            } else if (mTime < mDuration) {
                Lerp_unk();
            } else {
                End_unk();
            }
        } else {
            if (mTime < 0.0f) {
                End_unk();
            } else if (mTime < mDuration) {
                Lerp_unk();
            } else {
                mValue = mTo;
                mTime = mDuration;
            }
        }
    }

    float mTime;
    float mDuration;
    float mSpeed;
    unsigned char mActive;
    float mFrom;
    float mTo;
    float mValue;
    float mDelta;
};

// inferred from __vt__11Em315Coffin (0x807885D0): coffin enemy. Data file "/database/chara/special/" +
// db_name (default "db_em315_00_00.csv").
class Em315Coffin : public SpecialEnemyBase {
public:
    Em315Coffin(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em315Coffin();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf28();
    virtual void vf30(void* menu);
    virtual void vf40(HitInfo_unk* hit);
    virtual float vfA0();
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();
    virtual int vf10C(HitMsg_unk* msg);

    void fn_80313AAC(const char* text, int length); // inferred: reads the data file
    void fn_80315654(); // inferred: updates the collision capsule
    void fn_80315964(); // inferred: starts the ray attack
    void fn_80315FE0(); // inferred: stops the effect of the coffin

    CharaRes_unk m14D4;       // inferred: own copy of the character resource (m64 points to it)
    atn::EfpHandle_unk m1564; // inferred: "/effect/ef_bs023a.efp"
    ResLoader_unk m1570;      // inferred: "/effect/ef_bs023b.eff"
    atn::EfpHandle_unk m157C; // inferred: "/effect/ef_bs023d.efp"
    unsigned char m1588[0x15B8 - 0x1588];
    int m15B8;                // inferred: "Frame" of the ray attack
    unsigned char m15BC[0x15C8 - 0x15BC];
    CoffinSkill_unk m15C8;    // inferred: "DrainSkillId" / "DrainIntervalFrame"
    CoffinSkill_unk m15D8;    // inferred: "WideSkillId" / "WideIntervalFrame"
    CoffinSkill_unk m15E8;    // inferred: "ShadeRaySkillId" / "ShadeRayIntervalFrame"
    atn::Vector3 m15F8;       // inferred: position of the target when the drain starts
    atn::Vector3 m1604;
    atn::Vector3 m1610;       // inferred: position when the drain starts
    CharaTask* m161C;         // inferred: target of the drain
    atn::EfpHandle_unk m1620; // inferred: "/effect/ef_cm007.efp"
    int m162C;
    int m1630;
    int m1634;                // inferred: "StableStart"
    CoffinFade_unk m1638;     // inferred: alpha of the material m1658
    Material_unk* m1658;      // inferred: material "em315_02" (faded out by "StableStart")
    ResHandle m165C;          // inferred: debug menu group
};

#endif
