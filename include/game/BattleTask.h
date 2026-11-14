#ifndef GAME_BATTLETASK_H
#define GAME_BATTLETASK_H

#include "atn/Task.h"
#include "atn/EfpHandle_unk.h"
#include "atn/EffectSystem_unk.h"
#include "game/BattleCalc_unk.h"
#include "game/HomingBullet.h"
#include "atn/Model_unk.h"
#include "game/ResHandle.h"

class CharaTask;
struct ColorF_unk;
struct HitInfo_unk;
struct RayObj_unk;
class PlayerTask;
namespace atn {
struct Vector3;
}
namespace atn {
class D2A_Task;
struct NodeChunk;
struct ModelNodeCallbackContext;
struct Model;
struct EffFile;
class Texture;
}

// inferred: object hit by the cursor ray (as game/BattleCharaTask.cpp)
struct RayObj_unk {
    unsigned char m00[0x08];
    unsigned int m08; // inferred: flags (2: the object is a character, 8: another object)
    CharaTask* m0C;
};

// inferred: 0x934-byte entry of BattleTask::mCharas (constructor 0x80108090, built by __construct_array)
struct BattleChara_unk {
    BattleChara_unk();  // 0x80108090
    ~BattleChara_unk(); // 0x80108150
    void fn_801088B8(int id, int value); // inferred: adds a hate entry (at most 10 per id; drops the oldest)
    void fn_8010895C(int id, int all);   // inferred: removes the entries of an id and compacts the list
    void fn_801081D8();                  // inferred: resets the entry
    int fn_801083D8(int id);             // inferred: hate value toward the entry `id`

    CharaTask* mChara;
    CharaTask* m04;     // inferred: nearest opponent of the character (BattleTask::vf10)
    int m08[72];        // inferred: one value per entry of BattleTask::mCharas
    float m128[72];
    float m248;
    float m24C[72];
    int m36C[72];
    int m48C[72];
    int m5AC[72];
    int m6CC;           // inferred: random value 0..70
    short* mHateValues; // 0x6D0: inferred
    short* mHateIds;    // 0x6D4: inferred
    int mHateCount;     // 0x6D8: inferred: size of both arrays (10, or 5 in multiplayer)
    int m6DC;
    int m6E0;
    float m6E4[72];
    int m804;
    int m808;
    int m80C[72];
    unsigned char m92C;
    unsigned char m92D;
    unsigned char m92E;
    unsigned char m92F;
    int m930;
};

// inferred from __vt__10BattleTask (0x80779DA0): battle manager (lbl_8087F048). Other units still use
// partial views of it named Effect_unk (CharaTask, CharaStatus, AI...); the methods those call keep the
// Effect_unk name until the views are unified. Method names invented.
// inferred: 0x14-byte record copied member by member (fn_8010ABF4): totals of the battle (fn_8010ACCC); names
// invented
struct BattleSel_unk {
    bool IsValid_unk() const; // inferred: both sides are alive (defined in game/BattleTask.cpp)

    int m00;
    int m04;
    int m08;
    int m0C;
    unsigned char m10;
};

// inferred: marker of a character (0x78 bytes, 72 of them at BattleTask+0x367B8); names invented
struct BattleMarker_unk {
    atn::Matrix3x4 mMtx;  // 0x00: base of the line
    atn::Matrix3x4 mMtx2; // 0x30: head of the line
    atn::Vector3 m60;     // inferred: last end point of the line
    float m6C;            // inferred: last scale
    int m70;              // inferred: frames left of the appearance (12 when the target changes)
    unsigned short mMode; // 0x74: 1 own side or multiplayer, 2 `arg`, 3 the leader is flagged, 0 otherwise
    unsigned short mFlags; // 0x76: 0x01 used, 0x10 effect started
};

// inferred: markers and resources of the target lines (BattleTask+0x367B8..0x389F4); names invented. The
// methods are defined inline in game/BattleTask.cpp and declared here without parameter names: an inline body
// here (or including atn/ResLoader_unk.h and atn/Texture_unk.h for the members) renumbers the @N of every unit
// that includes this header, so the EffFile and Texture members are kept as raw storage.
struct BattleMarkers_unk {
    BattleMarker_unk* fn_800F8538(int); // weak copy 0x800F8538
    atn::EffFile* GetEfp_unk(int);
    atn::Texture* GetTex_unk();

    BattleMarker_unk mItems[72];
    // inferred: effects of the line (atn::EffFile, 0xC bytes each): 0/1 base and head toward an enemy, 2/3
    // appearance, 4/5 base and head toward the own side
    unsigned char mEfp[6][0xC]; // 0x38978
    int mHideLines;             // 0x389C0: inferred: nonzero hides the lines (sets mode 1)
    unsigned char mTex[0x30];   // 0x389C4: inferred: texture of the line (atn::Texture, DrawHelper::fn_80064F00)
};

// inferred: entry of BattleTask::mStuns (0xC bytes): character with a timed effect (groups 0x3E9/0x3EA)
// inferred: debug record of a bullet hit test (0x28 bytes; ring of 32 in BattleTask, filled by fn_800FAB80
// when enabled); names invented
struct BattleShot_unk {
    int mKind;          // inferred: 1 = bullet
    HitInfo_unk* mData;
    atn::Vector3 mPos;
    atn::Vector3 mDir;
    float mRadius;
    int m24;
};
struct BattleShots_unk {
    // inferred: iterator of the ring (holds the ring and an index; as Ring_unk of Event/Manager.h); the
    // inline members are defined in game/BattleTask.cpp
    struct iterator {
        iterator(BattleShots_unk*, unsigned int);
        BattleShot_unk& operator*() const;
        BattleShot_unk* operator->() const;
        iterator& operator++();
        bool operator!=(iterator) const;
        BattleShots_unk* mRing;
        unsigned int mIndex;
    };
    void Push_unk(const BattleShot_unk&); // inferred: inline, defined in game/BattleTask.cpp
    iterator begin();
    iterator end();
    BattleShot_unk& operator[](unsigned int);
    iterator erase(iterator); // inferred: removes a record, moving the following ones (BattleTask::vf10)
    unsigned int mCount;
    BattleShot_unk mItems[32];
    unsigned int mHead; // inferred: index of the oldest record
    int mEnabled;
};

// inferred: fixed array {count, entries} of BattleTask::mTargets (as atn::FixedArray_unk<CharaTask*, 64>; not
// the template so that the units that include this header keep their @N labels); the inline members are
// defined in game/BattleTask.cpp
struct BattleTargets_unk {
    unsigned int size() const;
    void clear();
    void push_back(CharaTask* const&); // inferred: constructs in place (null check of the slot, vf10)
    unsigned int mSize;
    CharaTask* mData[64];
};

// inferred: timer of BattleTask::mCBB0 (vf10 counts m04 down and removes the finished ones); names invented
struct BattleTimer_unk {
    float m00;
    int m04; // inferred: frames left
};

struct BattleStun_unk {
    CharaTask* mChara;
    int mTimer;   // inferred: frames left
    int mStarted; // inferred: the loop effect was started
};

class BattleTask : public atn::Task {
public:
    // BattleTask::BetoModel (name from the RTTI name of the node callback functor, 0x80735F10): 0x21C-byte
    // model drawn on a character with an active battle effect (mSlots)
    struct BetoModel {
        BetoModel(); // 0x8010D808 (destructor: weak copy 0x800F1F38)
        // inferred: node callback (bound by vf0C): the nodes follow the same node of the character, scaled
        void fn_8010D854(atn::Matrix3x4*, const atn::NodeChunk&, const atn::ModelNodeCallbackContext&);
        // inferred: follows the matrix of the character (scale from its largest axis); inline, defined in
        // game/BattleTask.cpp (vf10)
        void SetMtx_unk(const atn::Matrix3x4&);

        int mUsed;
        atn::ModelBase mModel; // 0x04
        CharaTask* mChara;     // 0x218
    };

    BattleTask(atn::Task* parent); // 0x800F1430
    static BattleTask* fn_800F13BC(atn::Task* parent); // inferred: creates the singleton
    virtual ~BattleTask();     // 0x800F1F94
    virtual int vf0C();        // 0x800F26D4
    virtual void vf10();       // 0x800F3EF4
    virtual void vf14();       // 0x800F5314
    int fn_800F3490(); // inferred: nonzero while the battle effects load
    int fn_800F3B4C(); // inferred: nonzero while the other resources load
    void fn_800F39A4(); // inferred: load requests of the resources
    void fn_80371664(); // inferred: defined in game/GameTask.cpp (0x80371664, clears mCBAC)
    // inferred: index of the character in mCharas, -1 if missing (inlined)
    int FindChara_unk(CharaTask* chara) {
        if (chara == 0) {
            return -1;
        }
        for (int i = 0; i < mCharaCount; i++) {
            if (mCharas[i].mChara == chara) {
                return i;
            }
        }
        return -1;
    }
    // inferred: start (model, group) and stop (character, group) of the battle effects; the stop
    // functions call fn_80239DAC with the effect group of the start
    void fn_800FDE60(int effect, CharaTask* chara, const atn::Vector3& dir); // inferred: starts an attack effect
    int fn_800FDF14(BattleCalcParam_unk* param); // inferred: returns the result of the hit (fn_800FD658)
    // inferred: hit sphere of `chara` nearest to the position (-1: none or the hit does not reach it); game/Bullet
    int fn_800FC1D0(CharaTask*, CharaTask*, HitInfo_unk*, int, const atn::Vector3*, int, int);
    // inferred: stops the effects (fn_801055A8 and effect group 5); callback of the debug menu (fn_80100BF0)
    static void fn_80100BC0(void*, void*);
    void fn_80100BF0(); // inferred: debug menu "Battle" (called by the constructor)
    void fn_801018E4(BattleCalcParam_unk*); // inferred: hit effect of the attack parameters
    void fn_80101CDC(BattleCalcParam_unk*); // inferred: hit sound of the attack parameters
    void fn_800F72F4(class BattleCharaTask* chara, int arg); // inferred: draws the target line of `chara`
    void fn_801055A8(int arg);
    // inferred: plays the battle effect `id` (fn_800EFBC4) / skill effect (fn_800EFDA0) after stopping the group
    void fn_801070C8(int id, unsigned int owner, int group, atn::Model* model, const atn::Vector3* pos,
                     const atn::Vector3* rot, const EffectColor_unk* color, float scale);
    void fn_80107168(int id, unsigned int owner, int group, atn::Model* model, const atn::Vector3* pos,
                     const atn::Vector3* rot, const EffectColor_unk* color, float scale);
    // inferred: chant effect (CharaTask::fn_80145334): shown at pos (restarted when `changed`)
    void fn_8010743C(const atn::Vector3* pos, int changed, float f);
    void fn_8010C3E4(); // inferred: updates the hate table m391E0
    CharaTask* fn_8010C948(CharaTask* chara); // inferred: the character `chara` hates most
    CharaTask* GetHated_unk(CharaTask*); // inferred: inline form of fn_8010C948 (vf14), defined in game/BattleTask.cpp
    void fn_80105DCC(CharaTask* chara, atn::Model* model, int level); // inferred: charge effect of the level
    Bullet* fn_801010A0();       // inferred: free bullet
    HomingBullet* fn_801010E8(); // inferred: free homing bullet
    void fn_80101138();
    int fn_80101340(CharaTask* chara); // inferred: bullets following `chara`          // inferred: vf14(0) on the active bullets
    void fn_801011B8();          // inferred: removes the active bullets
    void fn_8010129C();          // inferred: vf18 on every bullet
    // inferred: target of the camera: entry of mTargets nearest to `pos` in the cone `angle` around `dir`
    CharaTask* fn_801002DC(CharaTask*, const atn::Vector3*, const atn::Vector3*, int, float, float);
    int fn_80100BB8(CharaTask** out, CharaTask* chara, const atn::Vector3* dir, float range, float theta);
    void fn_801021B4(RayObj_unk* obj, int a, int b); // inferred: cursor target
    void fn_80102824(CharaTask* chara, PlayerTask* leader);
    void fn_801031D0(CharaTask* chara, CharaTask* other, int all); // inferred: removes the hate entries of `other`
    void fn_8010337C(CharaTask* chara, CharaTask* other);
    int fn_80103484(CharaTask* chara, CharaTask* other); // inferred: hate toward `other` (average without `chara`) // inferred: clears m6E4 of `other`
    int fn_80103B58(CharaTask*, CharaTask*, int); // inferred: `chara` may aim at `target`
    void fn_80103E60(CharaTask* target); // inferred: counts the characters aiming at `target`
    void fn_801027A4(CharaTask* chara);
    void fn_801028E4(CharaTask* chara, CharaTask* other, int value); // inferred: sets m08 of `other` (hate) // inferred: adds the character to mCharas
    int fn_80102890(CharaTask* chara); // inferred: index of the character in mCharas
    void fn_80102A40(CharaTask* chara, CharaTask* target, int a, int b); // inferred: attack order (hate)
    void fn_80103600(CharaTask* chara);
    BattleChara_unk* fn_80103F60(CharaTask* chara); // inferred: entry of the character, null if missing
    void fn_80103FD4();
    void UpdateAlert_unk(CharaTask*, PlayerTask*); // inferred: inline, defined in game/BattleTask.cpp
    void fn_80109F7C(); // inferred: wakes up the enemies that see the player, are hurt or have a busy ally near
    void fn_8010A868(); // inferred: updates mStuns
    void fn_8010ABF4();
    void fn_8010AC60(); // inferred: clears the selection and the gimmicks of the characters
    void fn_8010ACCC();
    void fn_8010AEF4();
    void fn_80105510(int value); // inferred: sets m363BC (clamped; 9999 sets the field flag 10)
    void fn_801058F8(); // inferred: updates every player
    void fn_80105950(void* owner, atn::Model* model);
    void fn_801059F8(CharaTask* chara); // inferred: called when a magic ends
    void fn_80105A08(void* owner, atn::Model* model);
    void fn_80105AB0(void* owner, atn::Model* model, const EffectColor_unk& color);
    void fn_80105B3C(CharaTask* chara);
    void fn_80105B4C(void* owner, atn::Model* model, const EffectColor_unk* color);
    void fn_80105BD8(atn::Model* model);
    void fn_80105C9C(CharaTask* chara, atn::Model* model);
    void fn_80105F3C(void* owner);
    void fn_80105F4C(CharaTask* chara, atn::Model* model);
    void fn_8010607C(void* owner);
    void fn_8010608C(CharaTask* chara, atn::Model* model);
    void fn_8010652C(void* owner);
    void fn_8010653C(void* owner, atn::Model* model);
    void fn_801065E4(void* owner);
    void fn_801065F4(CharaTask* chara, atn::Model* model, const ColorF_unk* color); // inferred: tints the model
    void fn_80106680(void* owner, atn::Model* model);
    void fn_80106728(int arg, atn::Model* model); // inferred: called when a part is destroyed
    void fn_801067D0(CharaTask* chara, atn::Model* model);
    void fn_80107208(int id, const atn::Vector3* pos, float scale); // inferred: sound of the battle effect table
    int fn_80107274(CharaTask* chara);  // inferred: adds the character to mSlots (0: already there or full)
    void fn_80107380(CharaTask* chara); // inferred: removes the character from mSlots
    void fn_80107574(); // inferred: hides the chant effect
    void fn_80107584(); // inferred: updates the chant effect (fades it in or out)
    void fn_801076D0(void* owner, atn::Model* model); // inferred: paralysis effect
    void fn_80107798(void* owner, int flag);
    void fn_801077A8(void* owner, atn::Model* model);
    void fn_80107850(void* owner);
    void fn_80107860(void* owner, atn::Model* model);
    void fn_80107908(void* owner, atn::Model* model);
    void fn_801079B0(void* owner);
    void fn_801079C0(void* owner, atn::Model* model);
    void fn_80107A68(void* owner);
    void fn_80107A78(void* owner, atn::Model* model);
    void fn_80107B20(void* owner);
    void fn_80107B30(void* owner, atn::Model* model);
    void fn_80107BD8(void* owner);
    void fn_80107BE8(void* owner, atn::Model* model);
    void fn_80107CA8(void* owner, int flag);
    void fn_80107CEC(void* owner, atn::Model* model, float scale);
    void fn_80107D94(void* owner);
    void fn_80107DA4(void* owner, atn::Model* model, float scale, const atn::Vector3& pos);
    void fn_80107E58(void* owner);
    void fn_80107E68(void* owner, atn::Model* model);
    void fn_80107F10(void* owner);
    void fn_80107F20(void* owner, atn::Model* model, float scale);
    void fn_80107FC8(void* owner);
    void fn_80107FD8(void* owner, atn::Model* model);
    void fn_80108080(void* owner);
    int fn_80108378(CharaTask* chara); // inferred: index of the character in mCharas of the instance, -1 if missing
    void fn_80108A54(CharaTask* chara);
    // inferred: keeps the lock-on target of `chara` (CharaTask::mFC0) or picks a new one
    void fn_80108A5C(CharaTask* chara, const atn::Vector3* dir);
    // inferred: nearest target of `chara` in the cone `theta` around `dir` within `range`
    CharaTask* fn_800FFE68(CharaTask* chara, const atn::Vector3* dir, int a, int b, float range, float theta);
    void fn_80109828(const wchar_t* text); // inferred: debug log message
    void fn_80109864(struct SpEffect_unk*, CharaTask*); // inferred: log message of a status effect
    void fn_8010AB34();
    void fn_8010AB94();
    int fn_8010B180(CharaTask* a, CharaTask* b); // inferred: the two characters can reach each other (AI areas)
    int fn_8010B398(BattleCalcParam_unk*); // inferred: status effects of the hit (nonzero if one is shown)
    float fn_8010CA24();
    float fn_8010CA2C();
    float fn_8010CA34();
    float fn_8010CA3C(CharaTask* chara, float rate);
    float fn_8010CB2C(CharaTask*, float); // inferred: rate of the gauge gained by `chara`
    float fn_8010CCD0();
    int fn_8010CD3C(int, CharaTask*, CharaTask*); // inferred: chance of a status effect
    float fn_8010CCD8(CharaTask* chara); // inferred: gauge gained by the magic
    int fn_8010D770(HitInfo_unk* hit);
    int fn_8010D7B4(HitInfo_unk* hit);

    BattleTargets_unk mTargets; // 0x48: inferred: characters the camera can lock on (fn_801002DC)
    int mBulletId;                // 0x14C: inferred: next bullet id (fn_800F8548)
    Bullet mBullets[256];         // 0x150: inferred: bullet pool
    HomingBullet mHomings[32];    // 0xA150: inferred: homing bullet pool
    BetoModel mSlots[8];           // 0xBA50: inferred: characters with an active battle effect
    atn::D2A_Task* mCB30;          // inferred: d2anime task (made by fn_800F39A4)
    int mCB34;                 // inferred: the reticle was updated this frame (fn_8010220C)
    atn::Vector2_unk mCB38;    // inferred: stick offset of the reticle
    atn::Vector2_unk mCB40;    // inferred: previous mCB38
    atn::Vector2_unk mCB48;    // inferred: center of the reticle (moves toward mCB50)
    atn::Vector2_unk mCB50;    // inferred: target of the center
    int mCB58;                     // inferred: set by fn_8010D320 (aim direction valid)
    float mCB5C;                   // inferred: alpha of the arrow mCB60 (vf14)
    atn::D2A_Task* mCB60;          // inferred: d2anime task (made by fn_800F39A4; may be null)
    atn::Vector3 mCB64;            // inferred: aimed direction smoothed by vf14 (turns the arrow)
    atn::Vector3 mCB70;            // inferred: screen direction to the aimed position (fn_8010D320)
    unsigned char mCB7C[0xCBAC - 0xCB7C]; // inferred: atn::Texture (raw storage, see BattleMarkers_unk)
    unsigned int mCBAC;            // inferred: entries of mCBB0
    BattleTimer_unk mCBB0[8];      // inferred: timers counted down by vf10
    BattleChara_unk mCharas[0x48]; // 0xCBF0: inferred: characters known to the battle (CharaTask::fn_80176E30)
    int m36290[72]; // inferred: times each entry of mCharas was chosen as a target (fn_80103600)
    int mCharaCount;               // 0x363B0
    unsigned char m363B4[0x363BC - 0x363B4];
    int m363BC;
    // inferred: battle effects; the atn::EffFile members (released with EffFile::Destroy by fn_800F3170) are raw
    // storage for the reason given at BattleMarkers_unk
    atn::EfpFile mEfp363C0[4];        // 0x363C0
    unsigned char mHitEff[3][7][0xC]; // 0x363F0: inferred: atn::EffFile, hit effects [level - 1][kind] (fn_80101434)
    atn::EfpFile mEfp364EC;           // 0x364EC
    atn::EfpFile mSplashEfp[3];       // 0x364F8: inferred: splashes on water (fn_8010A308; loaded by fn_800F39A4)
    unsigned char mEff3651C[5][0xC]; // 0x3651C: inferred: atn::EffFile, hit per level
    unsigned char mEff36558[5][0xC]; // 0x36558: inferred: atn::EffFile, strong hit or guard per level
    unsigned char mEff36594[3][0xC]; // 0x36594: inferred: atn::EffFile, status effects (poison, curse, 0x40)
    atn::EfpFile mEfp365B8[4];        // 0x365B8: inferred: [0]/[1] stone (fn_80101524), [2]/[3]
    atn::EfpFile mEfp365E8[7];        // 0x365E8
    atn::EfpFile mEfp3663C[31]; // 0x3663C: inferred: palette effects (ef_<palette>.efp, fn_800F39A4)
    int m367B0; // inferred: copied from GameTask::m5618 at the end of a map change; debug "DrawHateArrow"
    int m367B4; // inferred: debug "DrawHateNumber"
    BattleMarkers_unk mMarkers; // 0x367B8: inferred: target lines
    unsigned char m389F4[0xC]; // inferred: atn::EffFile of the chant effect (raw storage; its address is the owner tag)
    int m38A00; // inferred: nonzero draws the target lines of every friend (fn_801055A8)
    float m38A04; // inferred: gauge value, clamped to [0, m38A0C]
    int m38A08;   // inferred: debug "DebugMode" of the morale
    float m38A0C; // inferred: gauge maximum ("Max")
    float m38A10; // inferred: "DownSpeed"
    float m38A14; // inferred: "SwordAttack"
    float m38A18; // inferred: gauge gained by a bullet hit of a player (fn_800FDB20)
    float m38A1C; // inferred: gauge gained by a hit of a player (fn_800FB4B0)
    float m38A20; // inferred: "Defeat"
    float m38A24; // inferred: "Damage"
    float m38A28; // inferred: gauge increments
    float m38A2C;
    float m38A30; // inferred: "WaitMin"
    float m38A34; // inferred: "WaitMax"
    float m38A38; // inferred: rate of m38A1C for the characters with mFlag28
    float m38A3C; // inferred: "HateDownBoost"
    float m38A40; // inferred: "EnemyRatio"
    float m38A44; // inferred: tension "Attack"
    float m38A48; // inferred: gauge gained by a magic
    float m38A4C; // inferred: tension "Defeat"
    float m38A50; // inferred: tension "Damage"
    float m38A54; // inferred: tension "Boss"
    ResHandle m38A58; // inferred: debug folder "Battle" (fn_80100BF0; released by the destructor)
    BattleCalcParam_unk m38A5C; // inferred: parameters of the current attack
    atn::EfpFile mEfp38AF4[32];  // 0x38AF4: inferred: status and magic effects (EFP files)
    unsigned char m38C74[0x38C90 - 0x38C74];
    BattleSel_unk m38C90; // inferred: selection (copied to m38CB8 by fn_8010ABF4)
    BattleSel_unk m38CA4; // inferred: previous value of m38C90 (fn_8010ACCC)
    BattleSel_unk m38CB8;
    int m38CCC; // inferred: result of the comparison of the totals (1, 3; 2 when GameTask::fn_80375184 fails)
    int m38CD0; // inferred: previous m38CCC
    BattleShots_unk mShots; // 0x38CD4
    int m391E0[0x48][0x48]; // inferred: hate of each character toward each other (fn_8010C948)
    int m3E2E0; // inferred: character under the cursor
    CharaTask* m3E2E4; // inferred: object under the cursor
    RayObj_unk* m3E2E8; // inferred: cursor target
    int m3E2EC;
    int m3E2F0;
    atn::D2A_Task* m3E2F4; // inferred: d2anime task (made by fn_800F39A4)
    unsigned char m3E2F8[0x3E2FC - 0x3E2F8];
    float m3E2FC; // inferred: alpha of the chant effect
    int m3E300;   // inferred: state of the chant effect (1: fading in, 2: fading out)
    EffectColor_unk m3E304; // inferred: color of the chant effect (set to 1, 1, 1, 1)
    unsigned int mStunCount;       // 0x3E314: inferred: entries of mStuns (cleared by fn_800F3D28)
    BattleStun_unk mStuns[4];      // 0x3E318: inferred: characters with a timed effect (fn_8010A868)
    atn::EfpFile mStunEfp[2][3];   // 0x3E348: inferred: [enemy of the player][-, loop, end] effects
    int m3E390; // inferred: debug "DrawPick"
    int m3E394; // inferred: the battle effects are loaded (cleared by fn_800F3170)
    CharaTask* m3E398; // inferred: user of the bullet being made (set around fn_800FAB80 by fn_800FBA30)
    int m3E39C; // inferred: set while the event script makes a bullet (GameEventScript::fn_803A4A7C)
    float m3E3A0; // inferred: debug "ReticleBlur"
    float m3E3A4; // inferred: debug "ReticleCenterLerp"
    float m3E3A8; // inferred: debug "ReticleCenterOfs"
    unsigned char m3E3AC[0x3E3B0 - 0x3E3AC]; // inferred: size 0x3E3B0 (new of fn_800F13BC)
};

#endif
