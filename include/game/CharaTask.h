#ifndef GAME_CHARATASK_H
#define GAME_CHARATASK_H

#include "atn/Task.h"
#include "atn/LipSync_unk.h"
#include "atn/Array_unk.h"
#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/LH_MotionSet.h"
#include "atn/LH_MotionCommandData.h"
#include "atn/SoundRef.h"
#include "chr/Motion_unk.h"
#include "game/CharaStatus.h"
#include "game/CharaSub_unk.h"
#include "game/CharaFaceControl.h"
#include "game/ResHandle.h"
#include "std/vector.h"
#include "math/Vec.h"
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

class EquipItemData;
namespace atn {
struct NodeChunk;                // inferred: name from the boost::bind type string
struct ModelNodeCallbackContext; // inferred: name from the boost::bind type string
template <class T, int Sep, int Eol, int Quote> class CsvBase;
class CsvTab;
}

struct Model_unk;

namespace chr {
class ControlBase;
struct GmkTarget_unk;
}

// inferred: hit/damage description passed to CharaTask::vf3C and vf40 (0x801513D0);
// 0x10 holds a vector (knockback?) and 0x68/0x90 values that FriendArganan clears.
#include "game/HitInfo_unk.h"

// inferred: argument of CharaTask::fn_80164B00 (defined in CharaTask.cpp)
struct CharaShade_unk;

// inferred: object at CharaTask+0xD74 (class unknown)
struct CharaD74_unk {
    // inferred: inlined in the CharaC58_unk constructor (0x8011BD14)
    CharaD74_unk()
        : m00(0), m01(0), m02(0), m04(0), m06(0), m08(0), m0C(0), m10(0), m14(0.0f), m18(0.0f), m1C(0.0f), m20(0) {}
    int fn_8013A144();
    // inferred: sets the kind and starts it (BattleCharaTask, Em002Skelton, PickTask; name invented)
    void fn_8011CD84(CharaTask* chara, int kind);
    int fn_8011D1FC();
    int fn_8011D21C();
    // inferred: flag accessors of m20 (they reload the word, CharaD74_unk::fn_8011CD84); names invented
    bool IsFlag_unk(unsigned int flag) const { return (m20 & flag) == flag; }
    void SetFlag_unk(unsigned int flag) { m20 |= flag; }
    void ClearFlag_unk(unsigned int flag) { m20 &= ~flag; }

    signed char m00; // inferred: kind (6 is checked by CharaTask::fn_80139560)
    signed char m01; // inferred: label kind shown over the character (0x80149A30)
    short m02; // inferred: counter (game/PickTask)
    short m04;
    short m06; // inferred: step counter of kinds 2 and 4 (fn_8011CD84)
    int m08;   // inferred: "WarpPointId" (debug menu, vf30)
    int m0C;   // inferred: "SafeAreaId"
    int m10;   // inferred: "UnsafeAreaId"
    float m14; // inferred: "DetectRadius"
    float m18; // inferred: "WarnRadius"
    float m1C; // inferred: "SightRadius"
    unsigned int m20;
};

// inferred: object pointed to by CharaTask::m1208 (class unknown)
struct CharaObj1208_unk {
    int fn_8013A184();
    int fn_8013A18C();

    unsigned char m00[0x54];
    int m54;
    unsigned char m58[0x9C - 0x58];
    int m9C;
};

// inferred: objects registered in CharaTask::m120C by fn_805989A4 (class unknown); m04 is compared
// with the skill data of CharaStatus::m2D0 by CharaTask::fn_80176428
struct CharaAttach_unk {
    int m00;
    HitInfo_unk* m04;
    CharaTask* m08;
};

// inferred: object pointed to by CharaTask::mD68 (class unknown)
struct CharaD68_unk {
    bool fn_8013C514();
    atn::Vector3* fn_8013C53C();
    float fn_8013C544();

    int m00;
    int m04;
    unsigned char m08[0x10 - 0x08];
    atn::Vector3 m10;
    unsigned char m1C[0x28 - 0x1C];
    float m28;
    unsigned char m2C[0x5C - 0x2C];
    float m5C;
};

extern "C" void fn_80094E2C(void* model, int value); // inferred: sets a value on every node
extern "C" void fn_80044E0C(void* object, int arg);
extern "C" int fn_80219558(int id);
struct HitInfo_unk;
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id (see LandMine.cpp)

// inferred: object pointed to by CharaTask::m480 (destructor 0x800899DC; class unknown)
struct Chara480_unk {
    ~Chara480_unk();
};


// inferred: listeners notified by CharaTask (function-local static of fn_8014F8F4, destructor
// 0x8003EFB0); names invented
struct CharaListener_unk {
    int mId; // inferred: -1 for every event
    void (*mFunc)(int id, CharaTask* chara, void* arg);
};
struct CharaListeners_unk {
    CharaListeners_unk() : mEnabled(1) {}
    void fn_8014F860(int id, CharaTask* chara, void* arg); // inferred: notifies the listeners of `id` (arg: event data)

    std::vector<CharaListener_unk> mList;
    int mEnabled;
};
CharaListeners_unk* fn_8014F8F4();

// inferred: type data returned by fn_80204E04 (CharaTask::m68)
// inferred: entry of the character table returned by fn_8020924C (names invented)
struct CharaInfo_unk {
    unsigned char m00[0x0C];
    const char* m0C; // inferred: name (shown by fn_80149A30)
    const char* m10; // inferred: name (passed to fn_802092C0; digits read by fn_801546F4)
    int m14;         // inferred: passed to fn_8020A81C (character data)
    const char* m18; // inferred: motion set file name ("/database/chara/motion/<m18>.mchr")
    const char* m1C; // inferred: motion command file name
    const char* m20; // inferred: extension file name
    int m24;         // inferred: kind (2 and 3 revive as skeletons, CharaTask::fn_8015EFAC)
    int m28;         // inferred: passed to fn_8021FC00
};

// inferred: object at CharaTask+0xC58 (constructor 0x8011BD14 called out of line; class unknown)
class GimmickCommon;

struct SkillRef_unk;

struct CharaC58_unk {
    CharaC58_unk();
    atn::Vector3 fn_8011BF3C(int arg); // inferred: a position of mC4 (vf54 of mD0 when it is set)

    int m00;
    int m04;  // 0xC5C: inferred: frame counter
    int m08;
    RouteWalker_unk m0C; // 0xC64
    int mB4; // 0xD0C: inferred: compared with the player's (fn_8017A9A8); a group or map id
    int mB8;
    int mBC;        // 0xD14: inferred: timer of mode 0 and 4
    int mC0;        // 0xD18 in CharaTask: inferred: nonzero stops chr::ControlRide (0x801C6FE8)
    CharaTask* mC4; // 0xD1C: inferred: another character (its status is read by CharaStatus::fn_8012DB04)
    int mC8;
    CharaTask* mCC; // 0xD24: inferred: character followed in mode 9
    GimmickCommon* mD0; // 0xD28: inferred: gimmick (its position, vf54, is used by CharaTask::fn_80154488)
    int mD4;
    unsigned int mD8; // 0xD30: inferred: flags (bit 2 is set by fn_8011BEB8)
    SkillRef_unk* mDC; // 0xD34: inferred: skill in use (its id is passed to fn_80219E6C)
    unsigned short mHeld_unk : 1; // 0xD38: inferred: set while held by gmk::InteractionSpot
    unsigned short mDone_unk : 1; // 0xD38 bit 14: inferred: the current mode has finished
    unsigned short mLoop_unk : 1; // bit 13: inferred: loops the last motion of modes 20-22
    unsigned short mE0_12 : 1;
    unsigned short mE0_11 : 1;    // bit 11: inferred: turning around mC4 (fn_8013A4E8)
    unsigned short mE0_10 : 1;    // bit 10: inferred: turn direction (toggled)
    unsigned short mE0 : 10;
    short mMode;    // 0xD3A: inferred: mode of fn_8013A4E8 (switch)
    short mE4;
    short mStep;    // 0xD3E: inferred: step inside the mode
    int mE8;        // 0xD40
    int mEC;        // 0xD44: inferred: count of motion loops
    short mMotions[4]; // 0xD48: inferred: motions played by modes 20-22 (-1 terminated)
    atn::Vector3 mDest; // 0xD50
    int mPlace;     // 0xD5C: inferred: id of a place of the route tables
    float mRadius;  // 0xD60
    CharaTask* m10C; // 0xD64
    CharaD68_unk* m110; // 0xD68
    int m114; // 0xD6C: inferred: copied from CharaRes_unk::m88 (fn_80154C08)
    int m118; // 0xD70: inferred: copied from CharaRes_unk::m8C
    CharaD74_unk m11C;  // 0xD74
    unsigned char m140[0x60]; // inferred: cleared together by fn_8011BE54
    unsigned char m1A0[0x10];
    unsigned char m1B0[0x100];
    int m2B0; // 0xF08: inferred: counter of frames without moving (EnemyLizerdBlade tackle)
    int m2B4; // 0xF0C: inferred: wait counter
    int m2B8; // 0xF10: inferred: frame counter (fn_80154C08)

    void fn_8011BE54();
    void fn_8011BEB8(int arg);
    void fn_8011BF94(); // inferred: full reset (BattleCharaTask 0x800EBA5C)
    void fn_8011C044(int arg);
    // inferred: AI orders (called by the dispatcher fn_8001AEDC of AI/AiCommon_unk); each one sets
    // the mode with fn_8011C074 and then its parameters. Names invented.
    void fn_8011C074(CharaTask* chara, int mode);
    void fn_8011C2D4(CharaTask* chara, int time);
    void fn_8011C314(CharaTask* chara, int start);
    void fn_8011C378(CharaTask* chara, int link);
    void fn_8011C3F4(CharaTask* chara, CharaTask* target, float radius);
    void fn_8011C46C(CharaTask* chara, int time);
    void fn_8011C4AC(CharaTask* chara);
    void fn_8011C610(CharaTask* chara, int count);
    void fn_8011C650(CharaTask* chara, unsigned int skill, CharaTask* target);
    void fn_8011C6C4(CharaTask* chara, int place, float radius);
    void fn_8011C7C0(CharaTask* chara, CharaTask* target, float radius);
    void fn_8011C850(CharaTask* chara);
    void fn_8011C8E4(CharaTask* chara);
    void fn_8011C8EC(CharaTask* chara, int place);
    void fn_8011C92C(CharaTask* chara, int place, float radius);
    void fn_8011C97C(CharaTask* chara, CharaTask* target, float radius);
    void fn_8011C9F8(CharaTask* chara);
    void fn_8011CA00(CharaTask* chara);
    void fn_8011CA08(CharaTask* chara, CharaTask* target, float radius);
    void fn_8011CA5C(CharaTask* chara, CharaTask* target, float radius, float angle);
    void fn_8011CAE4(CharaTask* chara, float radius);
    void fn_8011CC30(CharaTask* chara, int motion, int loop);
    void fn_8011CCA0(CharaTask* chara, int motion0, int motion1, int loop);
    void fn_8011CD1C(CharaTask* chara, int motion0, int motion1, int motion2, int loop);
};


// inferred: motion command player at CharaTask+0x1220 (plays the effects, sounds and camera shakes
// listed for the frames of the current motion; constructor 0x80120F5C called out of line). Names
// invented; game/MotionCommand_unk.cpp
struct MotionCommand_unk {
    MotionCommand_unk();  // 0x80120F5C
    ~MotionCommand_unk(); // 0x80121048
    void fn_80121E14(int arg);          // inferred: stops the looping effect and sound
    void fn_801210B4(const char* path); // inferred: loads the motion commands
    int fn_801210C4();                  // inferred: nonzero while loading
    void fn_80121114(CharaTask* chara);
    void fn_8012111C(); // inferred: per-frame update (CharaTask::fn_80145334)
    // inferred: runs the commands of `data` for the current frame; nonzero when one ran
    int fn_8012133C(MotionCmdData_unk* data, int* found);

    CharaTask* mChara;      // 0x00
    int m04;                // 0x04: inferred: motion id of the selected track
    int m08;                // 0x08: inferred: selected track (largest weight)
    int m0C;
    unsigned char m10;      // inferred: enabled
    unsigned char m11;      // inferred: sounds enabled
    unsigned char m12;      // inferred: a command was found on the last update (run again)
    atn::LH_MotionCommandData mLoader; // 0x14
    MotionCmdData_unk* m20; // inferred: loaded data
    atn::EfpHandle_unk* m24; // inferred: looping effect ("EFP_LOOP")
    atn::SoundRef m28;      // inferred: looping sound ("SE_LOOP")
    MtxPtr m2C;             // inferred: node the looping sound follows
    unsigned int m30;       // inferred: motion resource id of the last update
};

// inferred: resource entry returned by fn_80204E04 (CharaTask::m68); name invented
struct CharaRes_unk {
    int m00;
    int m04; // inferred: returned by fn_8017AC08
    unsigned char m08[0x30 - 0x08];
    float m30;
    float m34; // inferred: m30-m38 picked by the kind of the part in hand (AI/AiCommon_unk fn_8001BD14)
    float m38;
    unsigned char m3C[0x54 - 0x3C];
    float m54;
    unsigned char m58[0x88 - 0x58];
    int m88;
    int m8C;
};

// inferred: hit message received by CharaTask::vf3C (declared there as HitInfo_unk, which is the
// bullet data at m08); names invented
struct HitMsg_unk {
    CharaTask* mAttacker;
    CharaTask* m04; // inferred: owner of the attack (type 2 starts a combo, fn_80151448)
    HitInfo_unk* mData; // inferred: bullet/skill data
    unsigned int m0C; // inferred: flags
    atn::Vector3 m10; // inferred: knockback direction
    atn::Vector3 m1C;
    atn::Vector3 m28; // inferred: direction of the attack
    int m34; // inferred: id recorded by fn_8015076C
    int m38;
    unsigned char m3C[0x44 - 0x3C];
    int m44;
    int m48;
    unsigned char m4C[0x50 - 0x4C];
    unsigned int m50; // inferred: flags
    int m54; // inferred: passed to fn_8015E4B0
    int m58; // inferred: passed to fn_8015E7A0
    float m5C;
    float m60;
    int m64; // inferred: 1: the hit is applied by fn_8015076C
    int m68;
    int m6C;
    int m70;
    int m74; // inferred: combo count (CharaTask::mFE4)
    unsigned char m78[0x80 - 0x78];
    unsigned int mFlag31 : 1; // inferred: m28 is used as the knockback direction
    unsigned int mFlag30 : 1;
    unsigned int mFlag29 : 1;
    unsigned int mFlag28 : 1;
    unsigned int mFlag27_0 : 28;
    int m84; // inferred: reaction (0, 1 knocked down, 2 guarded)
    int m88;
    int m8C;
    int m90;
    unsigned int m94; // inferred: flags
};

// inferred: 8-byte entry of CharaTask::m6D0 (id at 0); name invented
struct CharaPair8_unk {
    CharaPair8_unk() {}
    CharaPair8_unk(int a, int b) : m00(a), m04(b) {} // inferred: weak copy 0x8015115C
    int m00;
    int m04;
};

// inferred: ledge description passed to CharaTask::fn_80153698; name invented
struct Ledge_unk {
    // inferred: name invented
    atn::Vector3 GetDir_unk() const {
        atn::Vector3 dir;
        dir = m24 - m18;
        PSVECNormalize(&dir, &dir);
        return dir;
    }

    unsigned char m00[0x18];
    atn::Vector3 m18; // inferred: start
    atn::Vector3 m24; // inferred: end
    int m30;
    int m34;
    int m38; // inferred: route links
    int m3C;
};

// inferred: skill reference (CharaTask+0xD34 points to one, BattleCharaTask embeds one at 0xABC);
// only the id is known
struct SkillRef_unk {
    int mId;
};

// inferred: element of CharaTask::m125C
struct CharaPair_unk {
    CharaPair_unk() : m00(0.0f), m04(0) {}
    // inferred: angle and character (fn_80176E30)
    CharaPair_unk(float angle, CharaTask* chara) : m00(angle), m04(chara) {}
    float m00;
    CharaTask* m04;
};

// inferred from __vt__9CharaTask (0x8077C460). Base class of all characters
// (NpcTask, BattleCharaTask, ...). Virtual methods are named by vtable offset; most of their
// parameters are unknown.
class CharaTask : public atn::Task {
public:
    CharaTask(atn::Task* parent, int type, void* data); // 0x801354B4
    virtual ~CharaTask();                               // 0x80136138
    virtual int vf0C();                                 // 0x8017CB74
    virtual int vf1C(); // inferred: nonzero skips the node hooks (fn_80148CC4)
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C(const char* params, int arg); // inferred: params "path=..." (BattleCharaTask passes two arguments)
    virtual void vf30(void* menu); // inferred: debug menu (the symbol is still named vf30__9CharaTaskFv)
    virtual void vf34(void* target, int arg, float a, float b); // inferred: look at target
    virtual void vf38(int arg); // inferred: fn_801765D8 passes 0 (the target symbol says Fv)
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf44();
    virtual void vf48();
    // inferred: hit reaction check (the symbol is still named vf4C__9CharaTaskFv)
    virtual int vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir);
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C(CharaTask* other); // inferred: other may be 0
    virtual void vf60();
    virtual atn::Vector3 vf64(int center); // inferred: position, or the center of the collision capsule
    // inferred: finishing move offered to `attacker` (EnemyTask 0x8035B8B0 returns a control kind 9-11)
    virtual int vf68(CharaTask* attacker);
    // inferred: starts the reaction control `kind` facing away from `other` (EnemyTask 0x8035BBCC)
    virtual void vf6C(int kind, CharaTask* other);
    virtual void vf70();
    virtual void vf74(int arg); // inferred: starts a control (the symbol is still named vf74__9CharaTaskFv)
    virtual int vf78(); // inferred: nonzero while a held character keeps hold (fn_8013A4E8)
    virtual void vf7C(int arg); // inferred: switches to chr::ControlGuarded(arg) (the target symbol says Fv)
    // inferred: appearance (chr::ControlAppearSkelton, ControlAppearCoffin or ControlAppear); the
    // target symbol says Fv
    virtual void vf80(const atn::Vector3& pos, const atn::Vector3& offset);
    virtual void vf84(int motion, float blend); // inferred: bound by chr::ControlAppear::vf10
    virtual void vf88();
    virtual void vf8C(const atn::Vector3& v); // inferred: bound by chr::ControlAppear::vf10
    virtual float vf90(); // inferred: returns f1 (0x800EF2AC), passed to fn_80107CEC
    virtual atn::Vector3 vf94(); // inferred: returns a vector (0x800EF370 returns lbl_807C7030)
    virtual void vf98();
    virtual void vf9C(int arg); // inferred: fn_80145334 passes 0 (the target symbol says Fv)
    virtual float vfA0(); // inferred: returns a size (fn_80176E30)
    virtual int vfA4();
    virtual int vfA8(int kind); // inferred: returns -1 (0x80041A20); EnemyTrista::vfA8 returns 0, 2 or 3
    virtual atn::Vector3 vfAC(); // inferred: returns a position (CharaStatus::fn_8012E540)
    // inferred: writes a position to `out`, nonzero when it is valid (0 in CharaTask, 0x800EF31C;
    // Em205Ogre 0x802BA64C writes the center of its second sphere)
    virtual int vfB0(atn::Vector3* out);
    // inferred: action to run for `chara` (empty in CharaTask: weak copy 0x800EF310; called by
    // chr::ControlWallUp::vf10)
    virtual boost::function<void()> vfB4(CharaTask* chara);
    virtual int vfB8(wchar_t* name); // inferred: copies the character name
    virtual void vfBC();
    virtual void vfC0(void* draw); // inferred: Np131LivingArmor passes `draw` to fn_803E2A30
    virtual bool vfC4();
    virtual void vfC8();
    virtual void vfCC(atn::Vector3* out, const atn::Vector3& dir); // inferred: four points (the target symbol says Fv)
    virtual atn::Vector3 vfD0(float angle); // inferred: position at mid height
    virtual void vfD4();
    // inferred: a free position next to the character, away from `target` (8 directions tried)
    virtual atn::Vector3 vfD8(CharaTask* target);
    virtual void vfDC();
    virtual void vfE0();

    int fn_8013655C(); // inferred: loading state machine on mAC, nonzero while loading
    int fn_801370B8(int index);
    void fn_801371AC(); // inferred: reads the character file (m84)
    void fn_80137B78(); // inferred: reloads the model LODs from the character file
    void fn_80137E68(Model_unk* model, atn::CsvTab* csv); // inferred: model draw flags
    void fn_80137FC0(); // inferred: reads the model options from the character file
    void fn_80138528(); // inferred: reads the motion parameters (mA4)
    int fn_80138E1C(int arg); // inferred: plays a random motion
    void fn_80148CC4(atn::Matrix3x4* mtx, const atn::NodeChunk& node, const atn::ModelNodeCallbackContext& context); // inferred: node matrix hook
    void fn_8014FF9C(int a, int b, int c);
    void fn_8017B1C4(atn::Vector_unk<int>* list);
    void fn_80178208(chr::ControlBase* control); // inferred: changes the current control
    void fn_8016EB48(int a, int b, int c); // inferred: bound with three bools (0x801C7390)
    // inferred: 0x8015BAF0-0x8015E7A0 each create a control and switch to it (fn_80178208 inlined)
    void fn_8015BAF0(int a, int b);
    void fn_8015BDD8(int a, const atn::Vector3& dir);
    void fn_8015C0C0(int a, int b);
    void fn_8015C3A8(int a);
    void fn_8015C6A0();
    void fn_8015C988(int a, int b);
    void fn_8015CC70(const atn::Vector3& a, const atn::Vector3& b, const atn::Vector3& c);
    void fn_8015CF98(const atn::Vector3& a);
    void fn_8015D2A0(const atn::Vector3& a, const atn::Vector3& b);
    void fn_8015D5B0(const atn::Vector3& a, const atn::Vector3& b);
    void fn_8015D8C0(float f, const atn::Vector3& dir, int a);
    void fn_8015DEC0(int a);
    void fn_8015E1B8(const atn::Vector3& dir);
    // inferred: 0x80161570-0x801647BC also create a control and switch to it
    void fn_80161570(int motion, int a, int b, float c, float d);
    void fn_80161880(int motion, int target, int a);
    void fn_80161B70(int motion, const atn::Vector3& pos);
    void fn_80161E58();
    void fn_80162140(const atn::Vector3& a, const atn::Vector3& b);
    void fn_80162428(const atn::Vector3& rot);
    void fn_80162720(const atn::Vector3& a, const atn::Vector3& b);
    void fn_80162A08(const atn::Vector3& a, const atn::Vector3& b);
    void fn_80162CF0(const atn::Vector3& a, const atn::Vector3& b, float f);
    void fn_80162FE8(const atn::Vector3& a, const atn::Vector3& b);
    void fn_801632D0(const atn::Vector3& a, const atn::Vector3& b);
    void fn_801635B8(const atn::Vector3& a, const atn::Vector3& b);
    void fn_801638A0(const atn::Vector3& a, const atn::Vector3& b);
    void fn_80163B88(const atn::Vector3& a, const atn::Vector3& b);
    void fn_80163E70(CharaTask* rider);
    void fn_80164168(int a, int b);
    void fn_80164454(int id, int a, int b);
    void fn_801644D4(const atn::Vector3& start, CharaTask* target);
    void fn_801647BC();
    void fn_80164B00(const CharaShade_unk& shade); // inferred: starts a shade (timers at mFC8)
    void fn_80164D24(); // inferred: counts the mFC8 timers down while mFlag28 is set
    int fn_80164EF4(); // inferred: remaining shade time
    void fn_80164F54(int arg); // inferred: chr::ControlDetect
    void fn_80165284(CharaTask* target); // inferred: chr::ControlDetectHateAtt
    void fn_801656A4(int time);
    void fn_80165C5C(int id, const atn::Vector3& a, const atn::Vector3& b); // inferred: chr::ControlShoot
    void fn_80165F54(const atn::Vector3& pos); // inferred: chr::ControlClimbDeck
    // inferred: 0x8016624C-0x80168F94 start or end the status conditions (chr::ControlCdn*, effects)
    void fn_8016624C(int a);
    void fn_80166760();
    void fn_8016676C();
    void fn_80166A54();
    void fn_80166D3C(int a);
    void fn_80167344(int a);
    void fn_8016794C();
    void fn_80167C34();
    void fn_80167F1C();
    void fn_80168048();
    void fn_80168170();
    void fn_80168190();
    void fn_801681B0(const atn::Vector3& dir, int time, int flag);
    void fn_801684A0();
    void fn_80168788();
    void fn_801687D8();
    void fn_80168824(int flag);
    void fn_80168B1C();
    void fn_80168E04();
    void fn_80168E60();
    void fn_80168E80();
    void fn_80168EA0();
    void fn_80168EC0();
    void fn_80168F3C();
    void fn_80168F5C();
    void fn_80168F94();
    void fn_8016939C(int a, int flags, float f); // inferred: chr::ControlCliffhang
    bool fn_8016DCD4(); // inferred: CanReact_unk out of line
    bool fn_8016DDB0(int force); // inferred: can start an action (force skips fn_8016DCD4)
    void fn_8016DF3C(int type); // inferred: changes mType (player <-> other)
    void fn_8016E5F0(int show); // inferred: shows/hides the model (parts only when hiding)
    // inferred: inlined in fn_8016E4C4/fn_8016E5F0 (names invented)
    void SetModelVisible_unk(int show) {
        if (show) {
            m54C &= ~8;
            vf98();
        } else {
            m54C |= 8;
            vf9C(0);
        }
    }
    void SetPartsVisible_unk(int show);
    void fn_8016E71C(int show, int arg); // inferred: shows/hides the parts and the equipment
    void fn_8016E800(int arg); // inferred: calls fn_80566398 on every equipment
    float fn_8016E864(); // inferred: speed rate of the character
    void fn_8016E970(int state); // inferred: sets m55C (state 6 = a control is running)
    void fn_8016B8D4(const atn::Vector3& dir, int a, int b, int c); // inferred: chr::ControlPushedDown
    void fn_8016D0A0(int a); // inferred: releases the gimmick and starts chr::ControlSlip
    // inferred: controls started by 0x8016939C-0x8016DC14 (class names from the constructors' vtables)
    void fn_801696E0(); // inferred: leaves the cliffhang control (id 0x3A)
    void fn_80169F04(const atn::Vector3& pos, float rotY);
    void fn_8016A20C(const atn::Vector3& pos, int alt, float rotY);
    void fn_8016A504(const atn::Vector3& pos, const atn::Vector3& rot);
    bool fn_8016A7EC(); // inferred: false while control 0x5D runs
    void fn_8016A814(float damage);
    void fn_8016AB0C(chr::GmkTarget_unk* target, void* arg);
    bool fn_8016ADF4(CharaTask* other); // inferred: `other` can swap places with this character
    bool fn_8016AFE8(const atn::Vector3& dir); // inferred: swaps places with the character ahead
    void fn_8016BEDC(CharaTask* other, atn::Vector3 v);
    void fn_8016C1E0(CharaTask* other);
    void fn_8016C4D8(int a, const atn::Vector3& v);
    void fn_8016C7D8(int a);
    void fn_8016CAD0();
    void fn_8016CDB8(int a, int b);
    bool fn_8016D3F8(); // inferred: control 0x63, or 0x27 with motion 0x8E
    void fn_8016D454(float angle);
    int fn_8016DAB0();
    void fn_8016DC14(CharaTask* target, int quiet); // inferred: sets mFC4 (plays a UI sound)
    void fn_8015E4B0(const atn::Vector3& dir, int arg); // inferred: knock back along `dir` (0x801B59E0)
    void fn_8015E7A0(const atn::Vector3& dir, int arg, int arg2);
    // inferred: the current motion (track 0) has reached its end (name invented)
    bool IsMotionEnd_unk() {
        if (mMotion.mTracks[0].m18 == 0) {
            float frame = mMotion.mTracks[0].m08;
            return frame >= mMotion.fn_80097D7C(0);
        }
        float speed = mMotion.mTracks[0].m0C;
        float frame = mMotion.mTracks[0].m08;
        return frame >= mMotion.fn_80097D7C(0) - speed;
    }
    int fn_8015EB2C(); // inferred: the current action can be interrupted
    void fn_8015ECC4(); // inferred: get up (chr::ControlGetUp)
    void fn_8015EFAC(int arg); // inferred: revive (chr::ControlRevival / ControlRevivalSkelton)
    // inferred: starts chanting or casting `hit` at `pos` / `target`
    void fn_8015F568(const atn::Vector3& pos, CharaTask* target, HitInfo_unk* hit);
    // inferred: name invented; control 0x1D is chr::ControlChant
    bool fn_80160324();
    void fn_80160170(HitInfo_unk* hit, CharaTask* target); // inferred: changes the magic being chanted
    void fn_8016034C(); // inferred: ends the magic
    void fn_801603CC(float f, bool b); // inferred: casts the prepared magic (chr::ControlMagic)
    // inferred: uses the skill whose id is skill->mId (chr::ControlUseSkill)
    void fn_8016125C(const SkillRef_unk* skill, CharaTask* target, int motion, const char* name, float f);
    void fn_80139560(); // inferred: per-frame movement update
    void fn_8017B434();
    void fn_8017039C(int id, int arg); // inferred: moves to the place `id` (fn_8017B434)
    void fn_801718F8();
    int fn_8013A4E8(float* a, atn::Vector3* rot, float* b); // inferred: same values as vf34
    int fn_8015061C(CharaTask* chara);
    bool fn_8016F368(int type); // inferred: the type is on the same side as mType
    bool fn_8016F3D0(CharaTask* other, int check); // inferred: same side (check: arena and status tests)
    int fn_8016F4D8(CharaTask* chara); // inferred: HasLink_unk out of line
    int fn_8016F530(CharaTask* chara); // inferred: adds a link (nonzero if linked)
    int fn_8016F5EC(CharaTask* chara); // inferred: removes a link (nonzero if it was linked)
    void fn_8016F634(); // inferred: clears the links
    bool fn_8016F67C(); // inferred: nothing blocks the way in front of the character
    void fn_8016F824(int point, int arg); // inferred: "pointmove" to the route point `point`
    void fn_8016FDCC(int point, int arg); // inferred: "pointmove" to the hide point `point`
    void fn_80170A20(CharaTask* target, int arg, float radius);
    void fn_80170F20(const atn::Vector3* pos, int arg, float a, float radius);
    void fn_80171420(CharaTask* target, int arg, float radius);
    void fn_80171DB0(); // inferred: stops the route (fn_80127EF4) and releases the character
    void fn_8016D74C(HitInfo_unk* data, int a, int b);
    // inferred: weak copy 0x800F52F0 (name invented)
    CharaStatus* GetStatus_unk() { return &mStatus; }
    void fn_801698E4(const atn::Vector3& rot);
    int fn_80171F3C(const atn::Vector3* pos, const atn::Vector3* dest); // inferred: nonzero if fn_8036DBC0 accepts the move
    int fn_801720AC(atn::Vector3* pos, atn::Vector3* vel); // inferred: nonzero keeps the rotation (fn_80140588)
    int fn_801725EC(atn::Vector3* pos, atn::Vector3* vel, CharaTask** out); // inferred: climbs onto another character
    int fn_801749EC(atn::Vector3* pos, atn::Vector3* vel);
    // inferred: also called by fn_801720AC and BattleCharaTask (other: the character hit, may be 0)
    int fn_8017380C(atn::Vector3* pos, atn::Vector3* move);
    void fn_8016BBCC(const atn::Vector3* dir, int a, int b);
    // inferred: weak copy 0x800F7F80 (name invented)
    bool IsPlayer_unk() { return mType == 0; }
    void fn_80141674(void* target, float a, float b);
    void fn_80140588(void* target, float a, float b);
    void fn_801426A4(void* target, float a, float b);
    // inferred: inlined in vf34 (names invented)
    float GetF94Rate_unk() { return 1.0f - (float)mF94 / (float)mF98; }
    bool HasRideParts_unk();
    bool IsLanded_unk(const struct CollResult_unk& result);
    void fn_80145334();
    void fn_80149A30(); // inferred: debug display and visibility update
    int fn_8014FB60();

    // inferred: bounding sphere returned by value
    struct Sphere_unk {
        atn::Vector3 mCenter;
        float mRadius;
    };

    void fn_801539E0();
    Sphere_unk fn_80176548(const atn::Vector3& pos); // inferred
    void fn_801765D8(); // inferred: disables the character (mFlags 4)
    void fn_80155DAC(int arg);
    void fn_8016DA4C();
    void fn_801446F0();
    void fn_80144710(); // inferred: builds the model matrix from mPos/mRot/m540 (or from mF1C)
    void fn_80144F4C(int a, void* b, float y0, float y1); // inferred: orthographic camera on the model
    void fn_80175B18(float distSq); // inferred: per-frame update, gets the squared distance to the camera
    void fn_80176E30();
    void fn_80176ACC();
    void fn_80176DFC(int a, int b, int c);
    atn::Vector3 fn_80178088();
    atn::Vector3 fn_80178164();
    atn::Vector3 fn_801781B0();
    atn::Vector3 fn_80178018(); // inferred: returns a direction or target vector (y used as an angle)
    float fn_80178078();
    void fn_8017AC3C(int arg);
    void fn_80164DCC(int arg);
    void fn_8016E4C4(int show); // inferred: shows/hides the model, parts and equipment
    int fn_80179D44();
    void fn_801750FC();
    void fn_80174E04(GimmickCommon* cannon); // inferred: lifted by a cannon (chr::ControlCannonLift)
    void fn_8017518C(); // inferred: thrown by a cannon (chr::ControlCannonThrow)
    void fn_8017547C(CharaTask* other); // inferred: starts dragging `other`
    void fn_80175A38(); // inferred: releases the dragged character (mF54)
    bool fn_80175AEC(); // inferred: control 0x74-0x77 is running
    int fn_8017639C(CharaAttach_unk* obj);
    int fn_801763E0(CharaAttach_unk* obj);
    int fn_80176428(int index);
    void fn_80154344();
    void fn_8015495C(int a, int b, int c, int d);
    void fn_8013CB68(void* target, int arg, float a, float b); // inferred: same arguments as vf34
    // inferred: collision responses called by fn_8013CB68 with the new position and the movement
    int fn_80172EFC(atn::Vector3* pos, atn::Vector3* move);
    int fn_80174104(atn::Vector3* pos, atn::Vector3* move);
    int fn_8016E484();
    void fn_8017AC2C();
    void fn_80166544();
    int fn_801789D8(int a, unsigned int flags);
    void fn_80167038();
    void fn_80167640();
    // inferred: accessors called out of line (0x80139E90-0x8013C478)
    void fn_80136544(const char* path); // inferred: starts loading m84
    // inferred: inlined in the constructor (name invented)
    bool IsPartyMember_unk() { return mType == 1 || mType == 4; }
    // inferred: valid index of a party member (inlined in fn_801371AC; name invented)
    static bool IsPartyIndex_unk(int index) { return index >= 0 && 7 > index; }
    int fn_80139E90();
    int fn_80139E9C();
    int fn_80139EB0();
    int fn_80139EBC();
    int fn_80139ED0();
    int fn_80139EDC();
    int fn_80139EE8();
    int fn_80139EFC();
    int fn_80139F04();
    int fn_8013A150();
    bool fn_8013A158();
    void fn_8013A258(); // inferred: updates the current control
    atn::Vector3* fn_8013C38C();
    int fn_8013C3A8();
    int fn_8013C404(unsigned int flag);
    float fn_8013C42C();
    bool fn_8013C444();
    chr::CharaPart_unk* fn_8013C458();
    void fn_8013C460(int value);
    void fn_8013C470(CharaTask* chara);
    CharaTask* fn_8013C478();
    bool fn_80140518();
    atn::Vector3 fn_8014052C(); // inferred: forward direction
    int fn_801479D8();
    void fn_80147A18(int value);
    int fn_80148334();
    void fn_80148B38(); // inferred: updates the parts and the equipment
    // inferred: entry of the array at m62C (0x14 bytes)
    // inferred: has a constructor but no destructor (delete[] frees p - 0x10 without a destroy loop)
    struct Item14_unk {
        // inferred: inline (weak copy 0x80148990, used by __construct_new_array)
        Item14_unk() : m00(0) {
            m04.mCenter = atn::Vector3(0.0f, 0.0f, 0.0f);
            m04.mRadius = 5.0f;
        }
        int m00;
        Sphere_unk m04; // inferred: drawn as a debug sphere (0x80149A30)
    };
    Item14_unk* fn_80148B0C(int index);
    int fn_8014969C();
    void fn_8014C0B4(int a, int b);
    void fn_8014C228();
    void fn_8014C540(); // inferred: per-frame update (called by the derived classes)
    // inferred: inlined in fn_8014C540 (names invented)
    struct Lens_unk* GetLens_unk();
    void SetEmoteMtx_unk();
    bool IsLedgeLocked_unk();
    HitInfo_unk* FindHitBullet_unk();
    bool IsSameSide_unk(CharaTask* other);
    void fn_8014C468();
    void fn_8014DEE4(int a, int b, int c);
    void fn_8014EE48();
    void fn_8014EEC4(int keep);
    void fn_8014EF48(); // inferred: attaches the weapons (m648, m64C) to the hand nodes
    void fn_8014F2B4(int a, int b, int index);
    void fn_8014F4FC(int a, int b);
    void fn_8014F5D4(int index, int a, int b);
    void fn_8014F698(int a, int b, int notify); // inferred: creates the parts m678 (and m67C)
    void fn_8014F960(int notify); // inferred: deletes the parts m678 and m67C
    bool fn_8014FAD0();
    bool fn_8014FCC4();
    bool fn_8014FCE8();
    bool fn_8014FD0C();
    void fn_8014FD30();
    void fn_80150178(int slot, chr::CharaEquip_unk* equip);
    void fn_80150460(CharaTask* other, int combo); // inferred
    void fn_8015076C(int id, const atn::Vector3& dir, const atn::Vector3& v2, int arg, unsigned int flags,
                     struct HitMsg_unk* msg); // inferred: also called by game/Bullet.cpp
    void fn_80151448(struct HitMsg_unk* msg); // inferred: applies a hit (called by vf3C)
    void fn_8017ABD4(int arg);
    int fn_8017AC08();
    float fn_8017AC24();
    void fn_8017AC44(int clear);
    bool fn_8017B16C();
    bool fn_8017B3C0();
    void fn_8017C7F8(unsigned int on);
    void fn_8017BC4C();
    int fn_8017C974();
    int fn_8017C9A0();
    int fn_8017CB2C(); // inferred: also used by GimmickManager::fn_803EE45C
    const char* fn_8017C904(); // inferred: "chXXXX_YY" name of the character
    bool fn_801533C8(int id, HitInfo_unk* hit);
    void fn_80153698(const Ledge_unk* ledge, int keep);
    int fn_80153B44(); // inferred: free side (1 or 2) next to the character
    int fn_80153FA0();
    int fn_80153FD8(int kind);
    void fn_80154488();
    void fn_80154654(int type, int arg);
    // inferred: the callers pass (int, Vector3 by value, float); this body does not read them
    void fn_801546F4(int arg, atn::Vector3 pos, float rot);
    int fn_8015487C();
    void fn_80154C08();
    bool fn_80154E38(CharaTask* target);
    void fn_80154EC4(const atn::Vector3& pos, int link); // inferred: starts chr::ControlSwatTurn
    void fn_801551AC();
    void fn_80155790(const atn::Vector3& dir);
    // inferred: 0x80155A88-0x8015B808 also create a control and switch to it
    void fn_80155A88(const atn::Vector3& dir, int a);
    bool fn_80155D70();
    bool fn_80156120(CharaTask* target);
    void fn_801561E4(HitInfo_unk* hit);
    // inferred: starts an attack (chr::ControlAttack*) toward `pos`
    void fn_801562A0(HitInfo_unk* hit, const atn::Vector3& pos, int type, int a, int b, CharaTask* target,
                     float dist);
    void fn_80157444(HitInfo_unk* hit, const atn::Vector3& pos);
    void fn_8015783C(HitInfo_unk* hit, const atn::Vector3& pos, float dist); // inferred: all callers pass f1 (unused here)
    void fn_80157C34(HitInfo_unk* hit, const atn::Vector3& pos);
    void fn_8015802C(HitInfo_unk* hit, CharaTask* target);
    // inferred: name invented
    void SetHit_unk(HitInfo_unk* hit) {
        m63C = m638;
        m638 = hit;
    }
    void fn_801588A0(HitInfo_unk* hit, const atn::Vector3& pos);
    bool fn_80158BB4();
    float GetControlF20_unk();
    void Stripped1_unk(); // inferred: stripped by the linker (only .data ptmf constants remain)
    void Stripped2_unk();
    void Stripped3_unk();
    void Stripped4_unk();
    void Stripped5_unk();
    void Stripped6_unk();
    void Stripped7_unk();
    int fn_80158CA4();
    atn::Vector3 fn_80158E1C(); // inferred: direction given by the running control
    void fn_80158EF0(int a);
    void fn_801591E8(const atn::Vector3& dir);
    void fn_801595BC(int a, int count, int motion, int releaseMotion);
    void fn_801598B4(int a, int b, int c);
    void fn_80159BA4(int a);
    void fn_80159E9C();
    void fn_8015A184();
    void fn_8015A46C(const atn::Vector3& dir);
    void fn_8015AC48(int motion);
    void fn_8015AF40(int a);
    void fn_8015B238();
    void fn_8015B520();
    void fn_8015B808(const atn::Vector3& dir, int a);
    void fn_80178A6C();
    int fn_801784BC(struct Bullet_unk* bullet, int flag);
    int fn_80178668(HitInfo_unk* hit, HitInfo_unk* parent, int time, CharaTask* attacker, int flag);
    int fn_80178864(HitInfo_unk* hit, CharaTask* attacker);
    unsigned int fn_80179730(HitInfo_unk* hit, int flag);
    unsigned int fn_801799BC();
    int fn_80179A34();
    int fn_80179AB4();
    void fn_80179AD0(unsigned int mask, int clear);
    bool fn_80179EBC(int kind);
    void fn_80179F98(int value);
    bool fn_80179FA8();
    int fn_8017A160();
    int fn_8017A228(int arg);
    void fn_8017A300(chr::CharaData_unk* data);
    void fn_8017A33C(CharaTask* rider, int release);
    // inferred: removes the hit `it` and clears the references to its bullet (names invented)
    void RemoveHit_unk(CharaHit_unk* it) {
        for (CharaHit_unk* j = m12D0.begin(); j != m12D0.end(); j++) {
            if ((HitInfo_unk*)j->m04 == it->mHit) {
                j->m04 = 0;
            }
        }
        m12D0.erase(it);
    }
    int fn_8017A450();
    void fn_8017A504();
    void fn_8017A5D8(int on);
    void fn_8017AD44(atn::Vector_unk<int>* list, void* arg);
    // inferred: name invented (m54C bit 2)
    bool IsHidden_unk() {
        if ((m54C & 4) == 4) {
            return true;
        }
        return false;
    }
    void fn_80178BFC();
    int fn_8017A9A8();
    // inferred: inlined helpers (names invented)
    bool IsPartsMode_unk();
    void ResetParts_unk();
    void ReleaseLedge_unk();
    void Unlink_unk();
    void ClearEffect_unk();
    void Reset_unk(int arg, int ledge, int flag, int effect);
    bool IsRes1_unk();
    // inferred: bool accessors (the original reloads the flag word after testing them)
    bool IsOnLedge_unk() const { return mFlag31; }
    bool IsFlag5_unk() const { return mFlag5; }
    bool IsFlag2_unk() const { return mFlag2; } // inferred: name invented (EnemySeaLizerd::vf40 reloads the word)
    bool IsFlag3_unk() const { return mFlag3; } // inferred: name invented (EnemySeaLizerd::vf40)
    bool IsFlag11_unk() const { return mFlag11; } // inferred: name invented (vf24 reloads the word)
    bool IsDoubleLink_unk() const { return mFlagB16; }
    bool CanClimbUp_unk(int kind) {
        bool result = false;
        if (mFlag30 && kind != 3) {
            result = true;
        }
        return result;
    }
    int GetLinkCount_unk() {
        int n = m1028;
        if (IsDoubleLink_unk()) {
            n *= 2;
        }
        return n;
    }
    bool HasLink_unk(CharaTask* chara) {
        for (int i = 0; i < GetLinkCount_unk(); i++) {
            if (mFE8[i] == chara) {
                return true;
            }
        }
        return false;
    }
    void RemoveLink_unk(CharaTask* chara) {
        for (int i = 0; i < 16; i++) {
            if (mFE8[i] == chara) {
                mFE8[i] = 0;
                return;
            }
        }
    }
    // inferred: names invented
    // inferred: names invented (fn_80151448)
    inline int ControlVf30_unk();
    bool IsStill_unk() { return atn::Abs_unk(mStatus.mParamB.m1C) < 1.1920929e-7f; }
    bool HasC48_unk() {
        bool result = false;
        if (mFlag31 && mC48 != 0) {
            result = true;
        }
        return result;
    }
    bool CanReact_unk() {
        if (!CanAct_unk()) {
            return false;
        }
        if (mStatus.IsState_unk(0x40) && mF94 == 0) {
            return false;
        }
        if (m55C == 6 && m560 >= 17 && m560 <= 19) {
            return false;
        }
        return true;
    }
    bool IsDown_unk() {
        bool result = true;
        if (!mStatus.IsState_unk(0x80000) && !mStatus.IsState_unk(0x1000000)) {
            result = false;
        }
        return result;
    }
    // inferred: bullet of the first hit with flag 0x04000000 (name invented)
    HitInfo_unk* GetFlaggedHitBullet_unk() {
        CharaHit_unk* hits = m12D0.mData;
        for (unsigned int i = 0; i < m12D0.mCount; i++) {
            HitInfo_unk* hit = hits[i].mHit;
            if ((hit->mAC & 0x04000000) == 0x04000000 && hit->m04 > 0) {
                return fn_80219E6C(hit->m04);
            }
        }
        return 0;
    }
    void* HasHitBullet_unk(int id) {
        CharaHit_unk* hits = m12D0.mData;
        for (unsigned int i = 0; i < m12D0.mCount; i++) {
            HitInfo_unk* hit = hits[i].mHit;
            if (hit->m04 == id) {
                return fn_80219E6C(hit->m04);
            }
        }
        return 0;
    }
    // inferred: bullet of the first hit with bullet id `id` (any if <= 0) and the flags `flags` (any
    // if 0); name invented
    HitInfo_unk* FindHit_unk(int id, unsigned int flags) {
        CharaHit_unk* hit = m12D0.mData;
        for (unsigned int i = 0; i < m12D0.mCount; i++) {
            if ((id <= 0 || hit->mHit->m04 == id) && (flags == 0 || flags == (flags & hit->mHit->mAC))) {
                int bullet = hit->mHit->m04;
                if (bullet > 0) {
                    return fn_80219E6C(bullet);
                }
            }
            hit++;
        }
        return 0;
    }
    // inferred: name invented
    bool HasWeapon_unk() {
        bool result = false;
        if (m648 != 0 && m648->m04 == 0) {
            result = true;
        }
        return result;
    }
    // inferred: state checks inlined in many places (names invented)
    bool IsActive_unk() { return (mFlags & 4) != 4 && (mFlags & 1) != 1; }
    bool IsFree_unk() { return IsActive_unk() && (mStatus.m0C & 0x20) != 0x20; }
    bool IsGrappled_unk() { return m55C == 6 && m560 == 28; }
    bool CanAct_unk() { return IsFree_unk() && !IsGrappled_unk(); }
    bool HasPart674_unk() { return m674 >= 0 ? true : false; } // inferred: name invented (0x80149A30)
    inline bool IsShadowField_unk();
    // inferred: names invented (weak copies 0x8000DD0C and 0x8012A1B8)
    Model_unk* GetModel_unk() { return (Model_unk*)&mMotion; }
    atn::Vector3* GetRot_unk() { return &mRot; }


    int mType;  // 0x48: also compared with 0 and 2 by the chr::Control* classes
    int m4C;    // 0x4C: same value as mType
    int m50;    // 0x50: inferred: third constructor argument (character id; compared with 1 and -1)
    ResHandle m54;
    int m58;
    chr::CharaData_unk* m5C; // inferred: character data (read by CharaStatus)
    CharaInfo_unk* m60;      // inferred: character table entry (fn_8020924C(m50))
    CharaRes_unk* m64; // inferred: current entry of m68 (fn_80154C08)
    CharaRes_unk* m68[5]; // inferred: m68[0] is set from fn_80204E04 and then cleared with the array
    int m7C;
    int mId;    // 0x80: inferred: unique id given by the character manager
    atn::LH_Binary m84;
    atn::LH_MotionSet m8C;
    atn::LH_MotionSet m98;
    atn::LH_Binary mA4;
    int mAC;    // inferred: set when m84 starts loading
    chr::Motion_unk mMotion; // 0xB0
    Chara480_unk* m480;
    int m484[31]; // inferred: motion ids, indexed by fn_801370B8 (m484[0] is played on load)
    float m500;
    float m504;
    float m508;
    float m50C;
    int m510;
    float m514;
    int m518;
    float m51C;
    int m520;
    int m524; // inferred: index into Motion_unk::m220
    atn::Vector3 mPos;  // 0x528
    atn::Vector3 mRot;  // 0x534
    atn::Vector3 m540;  // inferred: scale (1, 1, 1)
    unsigned int m54C; // inferred: flags
    atn::Vector3 m550;
    int m55C; // inferred: state compared with 6 by the grapple controls
    int m560;
    int m564; // inferred: set to 1 or 2 by gmk::InteractionSpot
    float m568;
    float m56C;
    float m570;
    atn::Vector3 m574;
    float m580;
    float m584;
    float m588;
    int m58C;
    int m590;
    int m594;
    int m598;
    unsigned char m59C;
    unsigned char m59D;
    unsigned char m59E;
    unsigned char m59F;
    float m5A0;
    atn::Vector3 m5A4;  // inferred: collision offset
    float m5B0; // inferred: collision radius (0x80188280)
    float m5B4;
    atn::ColliCapsule mColli; // 0x5B8
    int m610;
    atn::Vector3 m614;
    float m620;
    atn::Vector_unk<Item14_unk> m624;
    int m630;
    int m634;
    HitInfo_unk* m638; // inferred: current magic/attack (CharaTask::fn_8015F568)
    HitInfo_unk* m63C; // inferred: previous value of m638
    int m640;
    int m644;
    chr::CharaPart_unk* m648; // inferred: parts whose parameters are added (CharaStatus::fn_8012B988)
    chr::CharaPart_unk* m64C;
    FixedArray_unk<chr::CharaPart_unk*, 8> mParts; // 0x650
    int m674;
    chr::CharaPart_unk* m678;
    chr::CharaPart_unk* m67C;
    chr::CharaEquip_unk* m680[8]; // inferred: equipment (CharaStatus::fn_8012B988, fn_80147A18)
    int m6A0;
    FixedArray_unk<EquipItemData*, 4> m6A4; // inferred: items read from "EQUIP" lines
    atn::Vector3 m6B8;
    unsigned char m6C4[0x6D0 - 0x6C4];
    FixedArray_unk<CharaPair8_unk, 32> m6D0; // inferred: count and capacity 32 (fn_801533C8)
    CharaStatus mStatus; // 0x7D4
    unsigned int mC10; // inferred: flags or-ed into CharaStatus::mParamA.m9C (fn_8012B3E8)
    atn::Vector3 mC14; // inferred: direction of the ledge (fn_80153698)
    atn::Vector3 mC20; // inferred: ledge start
    atn::Vector3 mC2C; // inferred: ledge end
    int mC38; // inferred: route links (in use while holding the ledge)
    int mC3C;
    int mC40;
    unsigned int mC44; // inferred: flags of the ledge
    int mC48;
    int mC4C;
    int mC50;
    int mC54;
    CharaC58_unk mC58;
    int mF14;
    int mF18;
    int mF1C;  // inferred: cleared when chr::ControlRide ends; address of a matrix the character follows (fn_80144710; chr::ControlCaught stores an int)
    atn::Matrix3x4 mF20;
    int mF50; // inferred: set by chr::ControlDraggedStart
    int mF54; // inferred: set by chr::ControlDraggingStart
    int mF58;
    int mF5C;
    atn::Vector3 mF60; // inferred: set to the position by chr::ControlAppearSkelton
    atn::Vector3 mF6C;
    float mF78; // inferred: magic parameter (HitInfo_unk::m58, CharaTask::fn_8015F568)
    CharaTask* mF7C; // inferred: magic target
    boost::scoped_ptr<chr::ControlBase> mControl; // 0xF80: inferred: current control (updated by fn_8013A258)
    int mF84;
    atn::Vector3 mF88;
    int mF94;
    int mF98;
    int mF9C;
    int mFA0;
    atn::Vector3 mFA4;
    int mFB0;
    int mFB4;
    float mFB8;
    float mFBC; // inferred: chant time (<= 0: the magic is cast at once)
    CharaTask* mFC0;
    CharaTask* mFC4; // inferred: target set by fn_8016DC14 (BattleCharaTask copies it to mFC0)
    atn::Vector3 mFC8;
    float mFD4;
    int mFD8;
    CharaTask* mFDC; // inferred: last character passed to vf5C (attacker?)
    int mFE0;
    int mFE4;
    CharaTask* mFE8[16]; // inferred: linked characters (cleared with memset; fn_8015495C)
    int m1028;
    int m102C;
    RouteWalker_unk mRoute; // 0x1030
    CharaLook_unk mLook;    // 0x10D8
    Chara1154_unk m1154;
    CharaFaceControl m1188;
    CharaObj1208_unk* m1208;
    CharaAttach_unk* m120C[4]; // inferred: cleared with memset; filled by fn_8017639C
    boost::scoped_ptr<atn::EfpHandle_unk> m121C;
    MotionCommand_unk m1220;
    int m1254;
    float m1258;
    FixedArray_unk<CharaPair_unk, 8> m125C;
    int m12A0;
    // 0x12A4: flag word (bitfields, MSB first)
    unsigned int mFlag31 : 1;
    unsigned int mFlag30 : 1;
    unsigned int mFlag29 : 1;
    unsigned int mFlag28 : 1; // inferred: tested by CharaStatus::fn_8012F3D8
    unsigned int mFlag27 : 1;
    unsigned int mFlag26 : 1;
    unsigned int mFlag25 : 1;
    unsigned int mFlag24 : 1;
    unsigned int mFlag23 : 1;
    unsigned int mFlag22 : 1; // inferred: tested by CharaStatus::fn_8012B3E8
    unsigned int mFlag21 : 1;
    unsigned int mFlag20 : 1;
    unsigned int mFlag19 : 1;
    unsigned int mFlag18 : 1;
    unsigned int mFlag17 : 1;
    unsigned int mFlag16 : 1;
    unsigned int mFlag15 : 1;
    unsigned int mFlag14 : 1;
    unsigned int mFlag13 : 1;
    unsigned int mFlag12 : 1;
    unsigned int mFlag11 : 1;
    unsigned int mFlag10 : 1;
    unsigned int mFlag9 : 1;
    unsigned int mFlag8 : 1;
    unsigned int mFlag7 : 1;
    unsigned int mFlag6 : 1;
    unsigned int mFlag5 : 1;
    unsigned int mFlag4 : 1;
    unsigned int mFlag3 : 1;
    unsigned int mFlag2 : 1; // inferred: tested by CharaStatus::fn_8012E244
    unsigned int mFlag1 : 1; // inferred: cleared by fn_80145334 when mFlag2 is set
    unsigned int mFlag0 : 1;
    // 0x12A8: second flag word
    unsigned int mFlagB31 : 1; // inferred: set by fn_8014C540 when a hit has flag 2 (setting m294 clear); vf5C then always counts the hit as a combo
    unsigned int mFlagB30 : 1; // inferred: same, setting m294 set
    unsigned int mFlagB29 : 1; // inferred: set when the damage sphere is created (vf50)
    unsigned int mFlagB28 : 1;
    unsigned int mFlagB27 : 1; // inferred: "HAIR_BACK_CONTROL", enables the "hair_b" hook (fn_80148CC4)
    unsigned int mFlagB26 : 1;
    unsigned int mFlagB25 : 1; // inferred: "USE_MAP_BRIGHTNESS"
    unsigned int mFlagB24 : 1; // inferred: set by fn_80145334 when the magic effect is shown
    unsigned int mFlagB23 : 1;
    unsigned int mFlagB22 : 1;
    unsigned int mFlagB21 : 1; // inferred: set when the shield parts m678/m67C are created (fn_8014F698)
    unsigned int mFlagB20 : 1;
    unsigned int mFlagB19 : 1;
    unsigned int mFlagB18 : 1;
    unsigned int mFlagB17 : 1; // inferred: tested by vf34 for the player
    unsigned int mFlagB16 : 1; // inferred: doubles the number of linked characters (mFE8)
    unsigned int mFlagB15 : 1;
    unsigned int mFlagB14 : 1;
    unsigned int mFlagB13 : 1;
    unsigned int mFlagB12 : 1; // inferred: calls vf28 every frame (fn_80145334)
    unsigned int mFlagB11 : 1;
    unsigned int mFlagB10 : 1;
    unsigned int mFlagB9 : 1;
    unsigned int mFlagB8 : 1; // inferred: cleared by fn_8016034C (end of a magic)
    unsigned int mFlagB7 : 1;
    unsigned int mFlagB6 : 1;
    unsigned int mFlagB5 : 1; // inferred: keeps the left weapon on the identity offset (fn_8014EF48)
    unsigned int mFlagB4 : 1;
    unsigned int mFlagB3 : 1; // inferred: never hidden by distance (fn_80145334)
    unsigned int mFlagB2 : 1; // inferred: forces mFlag6 (fn_80155DAC)
    unsigned int mFlagB1 : 1; // inferred: set by fn_8017C7F8 (motion mode 4 instead of 1)
    unsigned int mFlagB0 : 1;
    int m12AC;
    int m12B0;
    int m12B4;
    int m12B8;
    int m12BC;
    int m12C0;
    int m12C4;
    int m12C8;
    float m12CC;
    // inferred: hits on the character (CharaStatus::fn_8012E540)
    FixedArray_unk<CharaHit_unk, 8> m12D0;
    unsigned int m1374; // inferred: hit flags of the previous frame (fn_80178BFC)
    unsigned char m1378[0x137C - 0x1378];
    unsigned int m137C; // inferred: flags (bit 3 clears the model flag 4 on load and hides the shadow, fn_80149A30)
    int m1380;
    int m1384;
    short m1388;
    short m138A;
    int m138C;
    int m1390; // inferred: random value 0-3
    CharaSub_unk* m1394;
    int m1398;
    CharaTask* m139C; // inferred: character whose motion follows this one (vf24)
    atn::Vector3 m13A0; // inferred: position of the bullets fired by fn_80139560
    int m13AC;
    int m13B0;
    float m13B4;
    atn::Vector3 m13B8[2]; // inferred: cleared with memset, set to the position on load
    float m13D0[2];        // inferred: cleared with memset, set to the rotation on load
    atn::Vector3 m13D8; // inferred: velocity added to mPos while mFlag2 is set (fn_80145334)
    float m13E4;        // inferred: turn speed added to mRot.y
    atn::Vector3 m13E8; // inferred: copied to the node m524 while mFlag2 is set
    int m13F4;
    int m13F8;
    CharaTask* m13FC; // inferred: character that "pointmove" (fn_8017039C) is forwarded to; set by fn_8017AC3C
    atn::Vector_unk<int> m1400; // inferred: element type unknown (plain data)
    int m140C;
    float m1410; // inferred: volume of the sound m1414 (fn_80178BFC)
    atn::SoundRef m1414;
    int m1418;
    float m141C;
    float m1420;
};

#endif
