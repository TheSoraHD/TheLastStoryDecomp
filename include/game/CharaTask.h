#ifndef GAME_CHARATASK_H
#define GAME_CHARATASK_H

#include "atn/Task.h"
#include "atn/Array_unk.h"
#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/LH_MotionSet.h"
#include "atn/SoundRef.h"
#include "chr/Motion_unk.h"
#include "game/CharaStatus.h"
#include "game/CharaSub_unk.h"
#include "game/ResHandle.h"
#include "std/vector.h"
#include "math/Vec.h"
#include <boost/scoped_ptr.hpp>

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
}

// inferred: hit/damage description passed to CharaTask::vf3C and vf40 (0x801513D0);
// 0x10 holds a vector (knockback?) and 0x68/0x90 values that FriendArganan clears.
struct HitInfo_unk;

// inferred: argument of CharaTask::fn_80164B00 (defined in CharaTask.cpp)
struct CharaShade_unk;

// inferred: object at CharaTask+0xD74 (class unknown)
struct CharaD74_unk {
    int fn_8013A144();
    int fn_8011D1FC();
    int fn_8011D21C();

    unsigned char m00[0x20];
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

// inferred: object pointed to by CharaTask::m480 (destructor 0x800899DC; class unknown)
struct Chara480_unk {
    ~Chara480_unk();
};

// inferred: elements of the object at CharaTask+0x1188 (classes unknown)
struct Chara1188Entry_unk {
    Chara1188Entry_unk();  // 0x8011D2E0
    ~Chara1188Entry_unk(); // 0x8011D394
    unsigned char m00[0x20];
};
struct Chara1188Item_unk {
    Chara1188Item_unk();
    unsigned char m00[0x10];
};

// inferred: listeners notified by CharaTask (function-local static of fn_8014F8F4, destructor
// 0x8003EFB0); names invented
struct CharaListener_unk {
    int mId; // inferred: -1 for every event
    void (*mFunc)(int id, CharaTask* chara, int arg);
};
struct CharaListeners_unk {
    CharaListeners_unk() : mEnabled(1) {}
    void fn_8014F860(int id, CharaTask* chara, int arg); // inferred: notifies the listeners of `id`

    std::vector<CharaListener_unk> mList;
    int mEnabled;
};
CharaListeners_unk* fn_8014F8F4();

// inferred: type data returned by fn_80204E04 (CharaTask::m68)
struct CharaType_unk {
    int m00;
    int m04;
};

// inferred: entry of the character table returned by fn_8020924C (names invented)
struct CharaInfo_unk {
    unsigned char m00[0x10];
    int m10;         // inferred: passed to fn_802092C0 (binary path)
    int m14;         // inferred: passed to fn_8020A81C (character data)
    const char* m18; // inferred: motion set file name ("/database/chara/motion/<m18>.mchr")
    const char* m1C; // inferred: motion command file name
    const char* m20; // inferred: extension file name
    unsigned char m24[0x28 - 0x24];
    int m28;         // inferred: passed to fn_8021FC00
};

// inferred: object at CharaTask+0xC58 (constructor 0x8011BD14 called out of line; class unknown)
struct CharaC58_unk {
    CharaC58_unk();

    int m00;
    int m04;
    int m08;
    RouteWalker_unk m0C;
    unsigned char mB4[0xC0 - 0xB4];
    int mC0;        // 0xD18 in CharaTask: inferred: nonzero stops chr::ControlRide (0x801C6FE8)
    CharaTask* mC4; // 0xD1C: inferred: another character (its status is read by CharaStatus::fn_8012DB04)
    unsigned char mC8[0xE0 - 0xC8];
    unsigned short mHeld_unk : 1; // 0xD38: inferred: set while held by gmk::InteractionSpot
    unsigned short mE0 : 15;
    unsigned char mE2[0x110 - 0xE2];
    CharaD68_unk* m110; // 0xD68
    unsigned char m114[0x11C - 0x114];
    CharaD74_unk m11C;  // 0xD74
    unsigned char m140[0x2BC - 0x140];
};

// inferred: object at CharaTask+0x1154 (constructor 0x8007A75C called out of line; class unknown)
struct Chara1154_unk {
    Chara1154_unk();

    unsigned char m00[0x30];
    void* m30; // inferred: callback (CharaTask sets fn_80218260)
};

// inferred: object at CharaTask+0x1188 (constructor 0x8011D24C called out of line; class unknown)
struct Chara1188_unk {
    Chara1188_unk();
    void fn_8011D424(Model_unk* model);

    int m00;
    Chara1188Entry_unk m04[2];
    atn::Vector_unk<Chara1188Item_unk> m44[2];
    int m5C;
    float m60;
    unsigned char m64[0x78 - 0x64];
    unsigned char m78;
    float m7C;
};

// inferred: motion command player at CharaTask+0x1220 (holds an atn::LH_MotionCommandData and a
// sound; constructor 0x80120F5C called out of line); name invented
struct MotionCommand_unk {
    MotionCommand_unk();  // 0x80120F5C
    ~MotionCommand_unk(); // 0x80121048
    void fn_80121E14(int arg);
    void fn_801210B4(const char* path); // inferred: loads the motion commands
    int fn_801210C4();                  // inferred: nonzero while loading
    void fn_80121114(CharaTask* chara);

    unsigned char m00[0x34];
};

// inferred: element of CharaTask::m125C
struct CharaPair_unk {
    CharaPair_unk() : m00(0.0f), m04(0) {}
    float m00;
    int m04;
};

// inferred from __vt__9CharaTask (0x8077C460). Base class of all characters
// (NpcTask, BattleCharaTask, ...). Virtual methods are named by vtable offset; most of their
// parameters are unknown.
class CharaTask : public atn::Task {
public:
    CharaTask(atn::Task* parent, int type, void* data); // 0x801354B4
    virtual ~CharaTask();                               // 0x80136138
    virtual int vf0C();                                 // 0x8017CB74
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C(const char* params, int arg); // inferred: params "path=..." (BattleCharaTask passes two arguments)
    virtual void vf30();
    virtual void vf34(void* target, int arg, float a, float b); // inferred: look at target
    virtual void vf38();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C();
    virtual void vf60();
    virtual atn::Vector3 vf64(int center); // inferred: position, or the center of the collision capsule
    virtual void vf68();
    virtual void vf6C();
    virtual void vf70();
    virtual void vf74();
    virtual void vf78();
    virtual void vf7C(int arg); // inferred: switches to chr::ControlGuarded(arg) (the target symbol says Fv)
    virtual void vf80();
    virtual void vf84(int motion, float blend); // inferred: bound by chr::ControlAppear::vf10
    virtual void vf88();
    virtual void vf8C(const atn::Vector3& v); // inferred: bound by chr::ControlAppear::vf10
    virtual void vf90();
    virtual void vf94();
    virtual void vf98();
    virtual void vf9C();
    virtual void vfA0();
    virtual int vfA4();
    virtual void vfA8();
    virtual atn::Vector3 vfAC(); // inferred: returns a position (CharaStatus::fn_8012E540)
    virtual void vfB0();
    virtual void vfB4();
    virtual int vfB8(wchar_t* name); // inferred: copies the character name
    virtual void vfBC();
    virtual void vfC0();
    virtual bool vfC4();
    virtual void vfC8();
    virtual void vfCC();
    virtual atn::Vector3 vfD0(float angle); // inferred: position at mid height
    virtual void vfD4();
    virtual void vfD8();
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
    void fn_80148CC4(atn::Matrix3x4* mtx, const atn::NodeChunk& node, const atn::ModelNodeCallbackContext& context);
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
    void fn_8016E970(int state); // inferred: sets m55C (state 6 = a control is running)
    void fn_8015E4B0(const atn::Vector3& dir, int arg); // inferred: knock back along `dir` (0x801B59E0)
    void fn_8015E7A0(const atn::Vector3& dir, int arg, int arg2);
    void fn_80139560();
    void fn_80145334();
    void fn_80149A30();

    // inferred: bounding sphere returned by value
    struct Sphere_unk {
        atn::Vector3 mCenter;
        float mRadius;
    };

    void fn_801539E0();
    Sphere_unk fn_80176548(const atn::Vector3& pos); // inferred
    void fn_80155DAC(int arg);
    void fn_8016DA4C();
    void fn_801446F0();
    atn::Vector3 fn_80178018(); // inferred: returns a direction or target vector (y used as an angle)
    float fn_80178078();
    void fn_8017AC3C(int arg);
    void fn_80164DCC(int arg);
    void fn_8016E4C4(int arg);
    int fn_80179D44();
    void fn_801750FC();
    void fn_80154344();
    void fn_8015495C(int a, int b, int c, int d);
    void fn_8013CB68(void* target, int arg, float a, float b); // inferred: same arguments as vf34
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
    // inferred: entry of the array at m62C (0x14 bytes)
    // inferred: has a constructor but no destructor (delete[] frees p - 0x10 without a destroy loop)
    struct Item14_unk {
        Item14_unk();
        unsigned char m00[0x14];
    };
    Item14_unk* fn_80148B0C(int index);
    int fn_8014969C();
    void fn_8014C0B4(int a, int b);
    void fn_8014C228();
    void fn_8014C468();
    void fn_8014DEE4(int a, int b, int c);
    void fn_8014EE48();
    void fn_8014EEC4(int keep);
    void fn_8014F2B4(int a, int b, int index);
    void fn_8014F4FC(int a, int b);
    void fn_8014F5D4(int index, int a, int b);
    bool fn_8014FAD0();
    bool fn_8014FCC4();
    bool fn_8014FCE8();
    bool fn_8014FD0C();
    void fn_8014FD30();
    void fn_80150178(int slot, chr::CharaEquip_unk* equip);
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
    // inferred: state checks inlined in many places (names invented)
    bool IsActive_unk() { return (mFlags & 4) != 4 && (mFlags & 1) != 1; }
    bool IsFree_unk() { return IsActive_unk() && (mStatus.m0C & 0x20) != 0x20; }
    bool IsGrappled_unk() { return m55C == 6 && m560 == 28; }
    bool CanAct_unk() { return IsFree_unk() && !IsGrappled_unk(); }
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
    int m64;
    CharaType_unk* m68[5]; // inferred: m68[0] is set from fn_80204E04 and then cleared with the array
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
    int m638;
    int m63C;
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
    int m6D0;
    unsigned char m6D4[0x7D4 - 0x6D4];
    CharaStatus mStatus; // 0x7D4
    unsigned int mC10; // inferred: flags or-ed into CharaStatus::mParamA.m9C (fn_8012B3E8)
    atn::Vector3 mC14;
    unsigned char mC20[0xC38 - 0xC20];
    int mC38;
    int mC3C;
    int mC40;
    unsigned char mC44[0xC48 - 0xC44];
    int mC48;
    int mC4C;
    int mC50;
    int mC54;
    CharaC58_unk mC58;
    int mF14;
    int mF18;
    int mF1C;  // inferred: cleared when chr::ControlRide ends
    atn::Matrix3x4 mF20;
    int mF50; // inferred: set by chr::ControlDraggedStart
    int mF54; // inferred: set by chr::ControlDraggingStart
    int mF58;
    int mF5C;
    atn::Vector3 mF60; // inferred: set to the position by chr::ControlAppearSkelton
    atn::Vector3 mF6C;
    float mF78;
    int mF7C;
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
    float mFBC;
    CharaTask* mFC0;
    int mFC4;
    atn::Vector3 mFC8;
    float mFD4;
    int mFD8;
    int mFDC;
    int mFE0;
    int mFE4;
    unsigned char mFE8[0x40]; // inferred: cleared with memset
    int m1028;
    int m102C;
    RouteWalker_unk mRoute; // 0x1030
    CharaLook_unk mLook;    // 0x10D8
    Chara1154_unk m1154;
    Chara1188_unk m1188;
    CharaObj1208_unk* m1208;
    unsigned char m120C[0x10]; // inferred: cleared with memset
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
    unsigned int mFlag1_0 : 2;
    // 0x12A8: second flag word
    unsigned int mFlagB31_28 : 4;
    unsigned int mFlagB27 : 1; // inferred: "HAIR_BACK_CONTROL"
    unsigned int mFlagB26 : 1;
    unsigned int mFlagB25 : 1; // inferred: "USE_MAP_BRIGHTNESS"
    unsigned int mFlagB24_21 : 4;
    unsigned int mFlagB20 : 1;
    unsigned int mFlagB19 : 1;
    unsigned int mFlagB18 : 1;
    unsigned int mFlagB17 : 1;
    unsigned int mFlagB16_15 : 2;
    unsigned int mFlagB14 : 1;
    unsigned int mFlagB13_11 : 3;
    unsigned int mFlagB10 : 1;
    unsigned int mFlagB9_8 : 2;
    unsigned int mFlagB7 : 1;
    unsigned int mFlagB6_2 : 5;
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
    int m1374;
    unsigned char m1378[0x137C - 0x1378];
    unsigned int m137C; // inferred: flags (bit 3 clears the model flag 4 on load)
    int m1380;
    int m1384;
    short m1388;
    short m138A;
    int m138C;
    int m1390; // inferred: random value 0-3
    CharaSub_unk* m1394;
    int m1398;
    int m139C;
    unsigned char m13A0[0x13AC - 0x13A0];
    int m13AC;
    int m13B0;
    float m13B4;
    atn::Vector3 m13B8[2]; // inferred: cleared with memset, set to the position on load
    float m13D0[2];        // inferred: cleared with memset, set to the rotation on load
    float m13D8;
    float m13DC;
    float m13E0;
    float m13E4;
    float m13E8;
    float m13EC;
    float m13F0;
    int m13F4;
    int m13F8;
    int m13FC;
    atn::Vector_unk<int> m1400; // inferred: element type unknown (plain data)
    int m140C;
    unsigned char m1410[0x1414 - 0x1410];
    atn::SoundRef m1414;
    int m1418;
    float m141C;
    float m1420;
};

#endif
