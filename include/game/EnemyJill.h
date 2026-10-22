#ifndef GAME_ENEMYJILL_H
#define GAME_ENEMYJILL_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/string.h"
#include "math/Vec.h"

// inferred: line "Event" of the data file: two event flags, a hit point ratio and the index of a voice
// (names invented)
struct JillEvent_unk {
    int m00; // inferred: flag set when the event starts
    int m04; // inferred: flag that ends it
    float m08; // inferred: hit point ratio that triggers it
    int m0C;
};

// inferred: line "ColMat" of the data file: three rows of a color matrix (0x50 bytes, the same layout as
// the color matrix of the render parameters lbl_8087EFA8 + 0x324); names invented
// inferred: 4-component vector copied with paired singles (as ColorVec4_unk of atn/PostFxColorMatrix.cpp)
struct JillVec4_unk {
    JillVec4_unk() {}
    JillVec4_unk(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    JillVec4_unk(const JillVec4_unk& v) { __Vec4Copy(&x, &v.x); }
    JillVec4_unk& operator=(const JillVec4_unk& v) {
        __Vec4Copy(&x, &v.x);
        return *this;
    }
    JillVec4_unk operator*(float f) const { return JillVec4_unk(x * f, y * f, z * f, w * f); }
    JillVec4_unk operator+(const JillVec4_unk& v) const { return JillVec4_unk(x + v.x, y + v.y, z + v.z, w + v.w); }
    JillVec4_unk operator-(const JillVec4_unk& v) const { return JillVec4_unk(x - v.x, y - v.y, z - v.z, w - v.w); }
    float x, y, z, w;
};
struct JillColMat_unk {
    JillColMat_unk() : mEnable(0), m44(0), m48(0), m4C(1.0f) {
        mRows[0] = JillVec4_unk(1.0f, 0.0f, 0.0f, 0.0f);
        mRows[1] = JillVec4_unk(0.0f, 1.0f, 0.0f, 0.0f);
        mRows[2] = JillVec4_unk(0.0f, 0.0f, 1.0f, 0.0f);
        mRows[3] = JillVec4_unk(0.0f, 0.0f, 0.0f, 0.0f);
    }
    int mEnable;
    JillVec4_unk mRows[4];
    int m44;
    int m48;
    float m4C;
};

// inferred: ring queue of ten ints (0x30 bytes); names invented
struct JillRing_unk {
    JillRing_unk() : mCount(0), mHead(0) {}
    unsigned int mCount;
    int mData[10];
    unsigned int mHead;
    int& Front_unk() { return mData[mHead % 10]; }
    int& At_unk(unsigned int i) { return mData[(mHead + i) % 10]; }
    // inferred: the head moves before the count is decremented (fn_8032F6A8)
    void Pop_unk() {
        mHead = Next_unk(mHead);
        mCount--;
    }
    static unsigned int Next_unk(unsigned int i) { return (i + 1) % 10; }
    void Push_unk(int x) {
        mData[(mHead + mCount) % 10] = x;
        mCount++;
    }
};

// inferred: voices of the data file (the destructor tests `this` twice before destroying the array: a
// class derived from the fixed array); name invented
struct JillVoices_unk : public FixedArray_unk<std::string, 5> {
    ~JillVoices_unk() {}
};

// inferred from __vt__9EnemyJill (0x80788E90): Jill (boss of the arena events). Data file
// "database/chara/special/db_enemy_jill.csv", debug menu "EnemyJill". The factory allocates 0x17C8
// bytes (SpecialEnemyBase.cpp, case 63). Members keep their offsets (names unknown).
class EnemyJill : public EnemyTask {
public:
    EnemyJill(atn::Task* parent, const EnemyInfo_unk* info);
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf44();
    virtual int vfA8(int kind);

    void fn_8032E3DC(); // inferred: reads the data file
    void fn_8032EF38();
    int fn_8032F0EC();
    int fn_8032F204();
    int fn_8032F314();
    int fn_8032F608();
    void fn_8032F6A8(); // inferred: voices
    void fn_8032F828();
    int fn_8032FBF4();
    void fn_8032FD30();
    void fn_8032FDE8();
    void fn_8032FFF8();
    void fn_80330218();
    void fn_80330D74();
    void fn_80331094();
    void fn_8033135C();
    void fn_803317F4();
    void fn_80331898();
    void fn_80331DAC();
    void fn_803323A0();
    void fn_8033250C();
    void fn_80332628();
    void fn_803326C0();
    void fn_80332B44();
    void fn_80332BE0();
    void fn_80332F78();
    void fn_80333024();
    void fn_803333BC();
    // inferred: starts the state `state` (inlined everywhere; name invented)
    inline void SetState_unk(int state);
    inline void ResetState_unk();
    inline void StartAbsorb_unk();
    inline void AttackTarget_unk(float t);
    inline void TurnTo_unk(float t);
    inline void SetStep_unk();

    atn::LH_Binary m14B0;   // inferred: "database/chara/special/db_enemy_jill.csv"
    int m14B8;              // inferred: step of the state
    int m14BC;              // inferred: frame counter
    atn::Vector3 m14C0;
    int m14CC;
    HitInfo_unk* m14D0;     // inferred: "NormalAttack"
    HitInfo_unk* m14D4;     // inferred: "GuardCounter"
    HitInfo_unk* m14D8;     // inferred: "HighJumpAttack"
    HitInfo_unk* m14DC;     // inferred: "SonicBoom"
    HitInfo_unk* m14E0;     // inferred: "ChaosAttackSkill"
    HitInfo_unk* m14E4;     // inferred: "AbsorbHPSkill"
    HitInfo_unk* m14E8;     // inferred: "EventDamageSkillID"
    FixedArray_unk<JillEvent_unk, 8> m14EC; // inferred: "Event"
    int m1570;              // inferred: running event (-1: none)
    int m1574;
    int m1578;
    int m157C;              // inferred: "WaitFrame"
    float m1580;            // inferred: "AttackChanceFrame"
    float m1584;            // inferred: "AttackBonusRate"
    float m1588;            // inferred: "GuardBonusRate"
    ResHandle m158C;        // inferred: debug menu group "EnemyJill"
    int m1590;
    int m1594;
    int m1598;
    int m159C;
    int m15A0;
    int m15A4;
    int m15A8;              // inferred: previous state
    int m15AC;              // inferred: "StopTalking"
    atn::SoundRef m15B0;    // inferred: voice being played
    JillRing_unk m15B4;     // inferred: voices queued
    JillRing_unk m15E4;     // inferred: history of the voices
    JillVoices_unk m1614;   // inferred: "Voice"
    int m1654;              // inferred: "DebugDisp"
    int m1658;              // inferred: "DebugLog"
    int m165C;              // inferred: "AttackTest"
    JillColMat_unk m1660[4]; // inferred: "ColMat"
    JillColMat_unk* m17A0;
    atn::EfpHandle_unk m17A4; // inferred: "ef_ev114.efp"
    ResLoader_unk m17B0;      // inferred: "/effect/ef_ev107a_01.eff"
    int m17BC;
    int m17C0;
    int m17C4;
};

#endif
