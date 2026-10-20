#include "game/SetConfig.h"
#include "atn/LoadHandle.h"
#include "game/CrowdManager.h"
#include "game/CharaTask.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "chr/ControlAttackBase.h"
#include "chr/ControlDamage.h"
#include "chr/ControlStickByQuark.h"
#include "chr/ControlOneMotion.h"
#include "chr/ControlInteractionBase.h"
#include "chr/ControlLadder.h"
#include "chr/ControlRide.h"
#include "chr/ControlJumpAt.h"
#include "chr/ControlAttractHateCharge.h"
#include "chr/ControlDetect.h"
#include "chr/ControlLookAround.h"
#include "chr/ControlShoot.h"
#include "chr/ControlCdn.h"
#include "chr/ControlCliffhang.h"
#include "chr/ControlChant.h"
#include "chr/ControlAppear.h"
#include "chr/ControlUseSkill.h"
#include "chr/ControlCaught.h"
#include "chr/ControlChaseCaught.h"
#include "chr/ControlCharge.h"
#include "chr/ControlWallUp.h"
#include "chr/ControlGuardMountle.h"
#include "chr/ControlStool.h"
#include "chr/ControlPushedDown.h"
#include "chr/ControlStagger.h"
#include "chr/ControlGmkCoffinOpening.h"
#include "chr/ControlTurnQuadruped.h"
#include "chr/ControlSitting.h"
#include "chr/ControlDraggingStart.h"
#include "chr/ControlDropDown.h"
#include "game/HitInfo_unk.h"
#include "atn/Memory.h"
#include "game/GimmickCommon.h"
#include "game/ColliAttrManager.h"
#include "game/CharaStatus.h"
#include "game/MapManager.h"
#include "game/GimmickCommon.h"
#include "game/HitInfo_unk.h"
#include <boost/scoped_ptr.hpp>
#include "std/string.h"
#include "std/string_members.h"
#include "atn/Csv.h"
#include "game/EquipItemData.h"
#include "game/EnemyManager.h"
#include "game/GimmickManager.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/tokenizer.hpp>
#include "macros.h"

// inferred: block of the game settings read by fn_8013A4E8 (names invented)
struct Block454_unk {
    int m00; // inferred: enabled
    unsigned char m04[0x18 - 0x04];
    float m18;
};

// inferred: game settings (lbl_8087F0A8, as in CharaStatus.cpp); names invented
struct Settings_unk {
    void* fn_80116BAC(); // inferred: pad (this + 0x48C)
    int fn_8013C41C();
    struct Block454_unk* fn_8013C434();
    float fn_8013C50C();

    unsigned char m00[0x20];
    int m20; // inferred: debug switches read by CharaTask::fn_80149A30 (m20, m34, m38, m54, m58)
    unsigned char m24[0x30 - 0x24];
    int m30; // inferred: frames per second (gravity -98.1 / (m30 * m30), CharaTask::fn_8013CB68)
    int m34;
    int m38;
    unsigned char m3C[0x48 - 0x3C];
    float m48;
    unsigned char m4C[0x54 - 0x4C];
    int m54;
    int m58;
    unsigned char m5C[0x98 - 0x5C];
    int m98; // inferred: tested with mType 3 in CharaTask::fn_8015F568
    unsigned char m9C[0xCC - 0x9C];
    int mCC; // inferred: nonzero makes every character a valid target (fn_80154E38)
    unsigned char mD0[0x250 - 0xD0];
    int m250; // inferred: copied to CharaTask::m1028 by fn_80164D24
    unsigned char m254[0x260 - 0x254];
    int m260; // inferred: nonzero enables the jump/dash attacks (CharaTask::fn_801562A0)
    unsigned char m264[0x278 - 0x264];
    int m278; // inferred: parts mode (fn_801539E0)
    unsigned char m27C[0x294 - 0x27C];
    int m294; // inferred: selects the flag set by CharaTask::fn_8014C540 for hits with flag 2
    unsigned char m298[0x2C0 - 0x298];
    int m2C0; // inferred: nonzero always tries fn_801749EC after a collision (fn_8013CB68)
    unsigned char m2C4[0x308 - 0x2C4];
    int m308; // inferred: nonzero lets the player react (CharaTask::fn_80179EBC)
    unsigned char m30C[0x314 - 0x30C];
    int m314; // inferred: tested with m50 == 1 before the chant effect (fn_80145334)
    unsigned char m318[0x334 - 0x318];
    float m334; // inferred: guard angle in degrees (fn_80151448: cos(0.5 * m334))
    unsigned char m338[0x368 - 0x338];
    unsigned int m368; // inferred: ARGB colors of the status effect (fn_8017A5D8)
    unsigned int m36C;
    unsigned int m370;
    unsigned char m374[0x3CC - 0x374];
    int m3CC; // inferred: id passed to fn_80210220 by CharaTask::fn_80164EF4
    int m3D0; // inferred: tested by CharaTask::fn_80164D24
    unsigned char m3D4[0x3D8 - 0x3D4];
    int m3D8;
    unsigned char m3DC[0x404 - 0x3DC];
    int m404; // inferred: level compared with 1, 2 and 3 by fn_8017A9A8
    unsigned char m408[0x414 - 0x408];
    float m414; // inferred: amount added to CharaStatus by fn_8014C540 (divided by 30)
    int m418;
    unsigned char m41C[0x454 - 0x41C];
    Block454_unk m454;
    unsigned char m470[0x57C - 0x470];
    int m57C; // inferred: enables the push back of mType 2 characters (fn_8013CB68)
    int m580; // inferred: enables the push back of the player (fn_8013CB68)
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: camera eye and target (field camera at lbl_8087F430+0x260, default camera at
// lbl_8087EFB4+0x104; the eye is the camera position used in Banana.cpp)
struct View_unk {
    unsigned char m00[8];
    atn::Vector3 mEye;    // 0x08
    atn::Vector3 mTarget; // 0x14
};

// inferred: line segment passed to fn_80050A1C
struct Segment_unk {
    Segment_unk() {}
    Segment_unk(const atn::Vector3& start, const atn::Vector3& end) : mStart(start), mEnd(end) {}
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};
// inferred: squared distance from a point to a segment; the closest point goes to `out` if given
// (0x80050A1C)
extern "C" float fn_80050A1C(const atn::Vector3* point, const Segment_unk* seg, atn::Vector3* out);

// inferred: field data (lbl_8087F430); m10D8 holds the route tables (see RouteWalker_unk.cpp)
struct Field_unk {
    RouteTable_unk* fn_8013C504();
    int fn_803761BC();
    int fn_803761DC(); // inferred: a mode compared with 1 and 2 by CharaTask::fn_8013CB68
    void fn_803750E4(int id); // inferred: called with 0xB5 when a knocked-down character had mFlag28

    unsigned char m00[0x48];
    int m48; // inferred: field kind (1, or 2 with a sub kind in m4C)
    int m4C;
    int m50;
    unsigned char m54[0x260 - 0x54];
    View_unk m260;
    unsigned char m280[0x868 - 0x280];
    int m868; // inferred: mode (3, 4 and 6 are tested by CharaTask::fn_80149A30)
    int m86C;
    unsigned char m870[0x10D8 - 0x870];
    RouteTable_unk* m10D8;
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4; // inferred: 9 makes every hit valid (fn_801533C8)
    unsigned char m54E8[0x54F4 - 0x54E8];
    unsigned char m54F4[4]; // inferred: name table (fn_803B3B38)
    unsigned char m54F8[0x56D4 - 0x54F8];
    CharaTask* m56D4; // inferred: a character never hidden by distance (fn_80145334); nonzero also pauses the status updates of fn_8014C540
    unsigned char m56D8[0x5744 - 0x56D8];
    int m5744; // inferred: copied to CharaTask::mFD8 by fn_80164B00
    unsigned char m5748[0x5760 - 0x5748];
    int m5760;
};
extern "C" Field_unk* lbl_8087F430;

// inferred: camera (the global one is lbl_8087EFB4; a model owns one through Model_unk::m208);
// 0x1F4 bytes, the eye (0x10C) is the camera position
struct Camera_unk {
    Camera_unk(int a = 0, int b = 0);
    ~Camera_unk();
    void fn_8004B378(); // inferred: updates the matrices (as in atn/LH_Scene.cpp)
    // inferred: fields 0x00-0x54 named as in atn/LH_Scene.cpp
    int mType;          // 0x00
    int mFovType;       // 0x04
    atn::Vector3 mPos;    // 0x08
    atn::Vector3 mTarget; // 0x14
    atn::Vector3 mUp;     // 0x20: inferred: set to (0, 1, 0) by CharaTask::fn_80144F4C
    atn::Vector3 mDir;    // 0x2C
    float m38;
    float m3C; // inferred: projection bounds (orthographic camera of fn_80144F4C)
    float m40;
    float m44;
    float m48;
    unsigned char m4C[0x104 - 0x4C];
    View_unk m104;
    unsigned char m124[0x1F4 - 0x124];
};
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);

// inferred: 0x934-byte entry of Effect_unk::mCBF0 (only the character is known)
struct EffectCharaSlot_unk {
    CharaTask* mChara;
    unsigned char m04[0x934 - 0x4];
};

// inferred: effect manager (lbl_8087F048, as in CharaStatus.cpp)
struct Effect_unk {
    void fn_8013A19C(int value);
    void fn_801065F4(CharaTask* chara, Model_unk* model, const struct ColorF_unk* color); // inferred: tints the model
    void fn_801059F8(CharaTask* chara); // inferred: called when a magic ends
    void fn_80105B3C(CharaTask* chara);
    atn::Vector3 fn_80109950(CharaTask* chara); // inferred: a position for the character
    CharaTask* fn_8010C948(CharaTask* chara);   // inferred: a target for the character
    // inferred: chant effect (fn_80145334): shown at pos, hidden by fn_80107574
    void fn_8010743C(const atn::Vector3* pos, int changed, float f);
    void fn_80107574();
    void fn_80106728(int arg, Model_unk* model); // inferred: called when a part is destroyed

    unsigned char m00[0xCBF0];
    EffectCharaSlot_unk mCBF0[0x48]; // inferred: characters known to the effect manager (fn_80176E30)
    unsigned char m36290[0x3E304 - 0x36290];
    Model_unk::Float4_unk m3E304; // inferred: color of the chant effect (set to 1, 1, 1, 1)
    unsigned char m3E314[0x3E39C - 0x3E314];
    int m3E39C;
};
extern "C" Effect_unk* lbl_8087F048;


extern "C" int fn_80375184(Field_unk* field);
extern "C" int fn_80370174(Field_unk* field, int id);  // inferred: event flag
extern "C" void fn_80370320(Field_unk* field, int id, int value, int arg); // inferred: sets an event flag
extern "C" int fn_800F8548(Effect_unk* mgr);
extern "C" void fn_800FD658(Effect_unk* mgr, int a, int b, void* bullet, int arg4, int arg5, int arg6);

// inferred: globals returned by accessors that are called out of line
extern "C" void* lbl_8087EE68;
extern "C" void* lbl_8087EE98;

// inferred: names of some status effects. The table itself is not in the DOL (unused, so the
// compiler drops it), but its strings are the first .rodata objects of the file (0x80737390) and are
// reused by sStatusNames_unk below.
static const char* const sBadStatusNames_unk[] = {
    "\215\254\227\220",                 // konran (confusion)
    "\203X\203\215\203E",               // surou (slow)
    "\216\200\226S",                    // shibou (death)
    "\220\316\211\273",                 // sekika (petrify)
    "\203}\203q",                       // mahi (paralysis)
    "\213[\216\227\226\263\223G",       // giji muteki (pseudo invincible)
    "\210\303\210\305",                 // kurayami (darkness)
    "\203x\203g\203x\203g",             // betobeto (sticky)
    "\203`\203\203\201[\203\200",       // chaamu (charm)
    "\220\270\220_\210\331\217\355",    // seishin ijou (mental disorder)
};

// inferred: sound effect names (0x80737490)
static const char* const sSoundNames_unk[] = {
    "SE_BTCOM_027", "SE_BTCOM_029", "SE_BTCOM_028", "SE_BTCOM_012", "SE_BTCOM_008",
    "SE_BTCOM_015", "SE_MB004_002", "SE_BTCOM_095", "SE_UI001_111", "SE_UI001_112",
};

// inferred: 0x807374B8
static const int sTable807374B8_unk[] = {0x238D, 0x238D, 0x238D, 0x238D, 0x238D, 0x238D, -1};

// inferred: names of the status effects, one per CharaStatus flag bit (0x807375C0)
static const char* const sStatusNames_unk[] = {
    "\203|\203C\203Y\203\223",                                          // poizun (poison)
    "\215\254\227\220",                                                 // konran (confusion)
    "\203T\203C\203\214\203\223\203X",                                  // sairensu (silence)
    "\203X\203\215\203E",                                               // surou (slow)
    "\203X\203\212\201[\203v",                                          // suriipu (sleep)
    "\216\200\226S",                                                    // shibou (death)
    "\220\316\211\273",                                                 // sekika (petrify)
    "\203}\203q",                                                       // mahi (paralysis)
    "\203V\203F\201[\203h",                                             // sheedo (shade)
    "\216\343\203X\203\215\203E",                                       // jaku surou (weak slow)
    "\203\214\203x\203\213\203_\203E\203\223",                          // reberu daun (level down)
    "HATE\217W\222\206\211e\213\277",                                   // HATE shuuchuu eikyou
    "\213[\216\227\226\263\223G",                                       // giji muteki (pseudo invincible)
    "\210\303\210\305",                                                 // kurayami (darkness)
    "\203t\203\211\203t\203\211",                                       // furafura (dizzy)
    "\203x\203g\203x\203g",                                             // betobeto (sticky)
    "\203`\203\203\201[\203\200",                                       // chaamu (charm)
    "\220\270\220_\210\331\217\355",                                    // seishin ijou (mental disorder)
    "\217\202\224j\211\363",                                            // tate hakai (shield break)
    "\203K\201[\203h\203u\203\214\203C\203N",                           // gaado bureiku (guard break)
    "\216\364\202\242",                                                 // noroi (curse)
    "\226\241\225\373\202\311\223\226\202\275\202\351",                 // mikata ni ataru (hits allies)
    "\216\343\202\242\203|\203C\203Y\203\223",                          // yowai poizun (weak poison)
    "\203M\203\203\203U\203\212\203\223\203O\203X\203\215\203E",        // gyazaringu surou (gathering slow)
    "\203X\203g\203b\203v",                                             // sutoppu (stop)
    "\203X\203\212\203b\203v",                                          // surippu (slip)
    "\203X\203L\203\213\203Q\201[\203W\227\255\202\334\202\347\202\310\202\242", // skill gauge does not fill
    "",
    "",
    "",
    "",
    "",
    "",
    0,
};

// inferred: model files replaced on load (0x807377B0): {searched name, replacement} pairs
struct ModelName_unk {
    const char* mFrom;
    const char* mTo;
};
static const ModelName_unk sModelNames_unk[] = {
    {"np011_a00.model", "np011_a01.model"}, {"np031_a00.model", "np031_a01.model"},
    {"np036_b00.model", "np036_b01.model"}, {"np042_a00.model", "np042_a01.model"},
    {"np037_b00.model", "np037_b01.model"}, {"np010_b00.model", "np010_b01.model"},
    {"np034_a00.model", "np034_a01.model"}, {"np038_f00.model", "np038_f01.model"},
    {"np035_k00.model", "np035_k01.model"}, {"np002_c00.model", "np002_c01.model"},
    {"em102_00_bs00.model", "em102_00_bs01.model"},
};

extern "C" int lbl_8087FA20;
extern "C" void* lbl_8087EEB8; // inferred: debug message output
extern "C" char lbl_807C7B28[32];
extern "C" int fn_8036823C(Field_unk* field);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int rand();
// inferred: debug message list (atn/FileUtil_unk.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" CharaInfo_unk* fn_8020924C(int id);
extern "C" int fn_802092C0(char* path, const char* name);
extern "C" chr::CharaData_unk* fn_8020A81C(int id);
extern "C" CharaRes_unk* fn_80204E04(int type, int arg);
extern "C" int fn_8021FC00(int id);
extern "C" void fn_80218260();
extern "C" void fn_8006AD24(std::string& path, const std::string& ext); // inferred: adds an extension

// inferred: nonzero in town fields (name invented)
inline bool IsTownField_unk() {
    if (lbl_8087F430 != 0 && fn_8036823C(lbl_8087F430) != 0) {
        return true;
    }
    return lbl_8087FA20 != 0;
}

// inferred: model heap of a character (name invented)
inline int GetModelHeap_unk(atn::Task* parent, int id) {
    if (id == 1) {
        return 0x23A;
    }
    if (parent == CrowdManager::sInstance) {
        return 0xEF;
    }
    return IsTownField_unk() ? 0x13F : 0x23A;
}

// inferred: projection of a point to the screen (as in GimmickCommon.cpp)
extern "C" void fn_800BFAC8(atn::Vector3* out, Camera_unk* camera, const atn::Vector3* pos);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: name invented
inline float LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}

// inferred: segment with a radius (name invented)
// inferred: segment with a radius
struct Capsule_unk : public Segment_unk {
    float mRadius;
};

extern "C" void fn_80565F34(chr::CharaEquip_unk* equip); // inferred: updates the equipment model

// inferred: player manager (lbl_8087F8A0, CharaManager<PlayerTask>, see gmk/TodomeCountUp.cpp)
struct PlayerManager_unk {
    unsigned char m00[0x48];
    CharaTask* mHead;
};
extern "C" PlayerManager_unk* lbl_8087F8A0;

// inferred: arena mode (lbl_8087F610, as in CharaStatus.cpp; nonzero also disables "PBONE");
// names invented
struct Arena_unk {
    unsigned char m000[0x4FC];
    int m4FC;
    unsigned char m500[0x50C - 0x500];
    int m50C;
    unsigned char m510[0x540 - 0x510];
    int m540;
    unsigned char m544[0x5C8 - 0x544];
    float m5C8; // inferred: damping of CharaTask::m13D8 and m13E4 (fn_80145334)
};
extern "C" Arena_unk* lbl_8087F610;
inline bool IsArenaMode_unk(int mode) {
    bool result = true;
    if (mode != 0 && mode != 1) {
        result = false;
    }
    return result;
}

// inferred: debug drawing (lbl_8087EEB0) and the font used to measure the text (lbl_8087EEC8)
extern "C" void* lbl_8087EEB0;
extern "C" void* lbl_8087EEC8;
extern "C" void fn_80063D3C(void* draw, const atn::Vector3* center, float radius, int color, float arg); // inferred: sphere
extern "C" void fn_80061824(void* draw, float x, float y, float z, float w, float h, float f6, float f7, float f8,
                            const wchar_t* text, unsigned int color, int a, int b, int c, int d,
                            unsigned int color2); // inferred: text
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width
extern "C" void fn_8006F2F0(void* font, wchar_t* dst, const char* src, int n); // inferred: to wide characters
extern "C" int swprintf(wchar_t* s, unsigned long n, const wchar_t* format, ...);
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" int fn_8021BE98(char* name, CharaStatusEntry_unk* entry);  // inferred: name of an entry

// inferred: area at a position (returned by MapManager::fn_804918A4)
struct MapArea_unk {
    int m00;
    char mName[1];
};
// inferred: map data returned by fn_80373148 and held by the map manager; m104 is passed to
// CharaTask::fn_80147A18 (map brightness, fn_80145334)
struct MapInfo_unk {
    unsigned char m00[0x48];
    int m48; // inferred: compared with 2 by CharaTask::fn_80176E30
    int m4C; // inferred: compared with 13 by CharaTask::fn_80176E30
    unsigned char m50[0x104 - 0x50];
    int m104;
};
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);

extern "C" void* lbl_8087F540;
extern "C" int fn_80481654(void* obj);
extern "C" void fn_8010A4A8(Effect_unk* effect, CharaTask* chara);

extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: message table (lbl_8087F1E4); name invented
struct MsgTable_unk {
    const wchar_t* Get_unk(int id) {
        if (mText[id] != 0) {
            return mText[id];
        }
        return lbl_808813D0;
    }
    const wchar_t* mText[1];
};
extern "C" MsgTable_unk* lbl_8087F1E4;
// inferred: global object at lbl_8087F098 (class unknown); m48 nonzero shows the labels of all
// characters (fn_80149A30)
struct Global98_unk {
    unsigned char m00[0x48];
    int m48;
};
extern "C" Global98_unk* lbl_8087F098;
// inferred: render manager (lbl_8087EEE0, see atn/LayerReflect.cpp)
struct RenderMgr_unk {
    unsigned char m00[0x40];
    int mHeight; // 0x40
};
extern "C" RenderMgr_unk* lbl_8087EEE0;

// inferred: node position (name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

CharaTask::CharaTask(atn::Task* parent, int type, void* data)
    : atn::Task(parent), mType(type), m4C(type), m50((int)data), m58(0), m5C(0), m60(0), m64(0),
      m7C(0), mId(0), mAC(0), mMotion(0x800504, GetModelHeap_unk(parent, (int)data)), m480(0), m510(2),
      m514(1.0f), m518(0), m51C(-1.0f), m520(-1), mPos(0.0f, 0.0f, 0.0f), mRot(0.0f, 0.0f, 0.0f),
      m540(1.0f, 1.0f, 1.0f), m54C(0), m550(0.0f, 0.0f, 0.0f), m55C(0), m560(0), m564(0), m568(1.0f),
      m56C(1.0f / 6.0f), m570(0.0f), m574(0.0f, 0.0f, 0.0f), m580(0.0f), m584(0.0f), m588(0.0f), m58C(0),
      m590(0), m594(0), m598(0), m59C(0), m59D(0), m59E(0), m59F(0), m5A0(1.0f), m610(0), m630(0),
      m634(-1), m638(0), m63C(0), m640(0), m644(1), m648(0), m64C(0), m674(-1), m678(0), m67C(0),
      m6A0(-1), m6B8(0.0f, 0.0f, 0.0f), mC10(0), mC14(0.0f, 0.0f, 0.0f), mC38(0), mC3C(0),
      mC40(4), mC48(0), mC4C(0), mC50(type), mC54(1), mF14(-1), mF18(-1), mF1C(0), mF50(0), mF54(0),
      mF58(0), mF5C(10000), mF60(0.0f, 0.0f, 0.0f), mF6C(0.0f, 0.0f, 0.0f), mF78(0.0f), mF7C(0), mF84(0),
      mF88(0.0f, 0.0f, 0.0f), mF94(-1), mF98(300), mF9C(0), mFA0(0), mFA4(0.0f, 0.0f, 0.0f), mFB0(0),
      mFB4(0), mFB8(0.0f), mFBC(0.0f), mFC0(0), mFC4(0), mFC8(0.0f, 0.0f, 0.0f), mFD4(0.0f), mFD8(0),
      mFDC(0), mFE0(0), mFE4(0), m1028(1), m102C(0), m1208(0), m121C(0), m1254(0), m1258(1.0f),
      m12A0(0), mFlag31(0), mFlag30(0), mFlag29(1), mFlag28(0), mFlag27(0), mFlag25(0), mFlag24(1),
      mFlag23(0), mFlag22(0), mFlag21(0), mFlag20(0), mFlag19(0), mFlag18(0), mFlag17(1), mFlag16(1),
      mFlag15(0), mFlag14(1), mFlag13(1), mFlag12(0), mFlag11(0), mFlag10(0), mFlag9(0), mFlag8(0), mFlag7(0), mFlag6(0), mFlag5(0), mFlag4(0), mFlag3(0),
      mFlag2(0), mFlag1(0), mFlag0(0), mFlagB31(0), mFlagB30(0), mFlagB29(0), mFlagB28(0), mFlagB27(0), mFlagB26(0), mFlagB25(0), mFlagB24(0), mFlagB23(0), mFlagB22(0), mFlagB21(0), mFlagB20(1), mFlagB18(0), mFlagB17(0), mFlagB16(0), mFlagB15(0), mFlagB14(1), mFlagB13(0), mFlagB12(0), mFlagB11(0),
      mFlagB10(1), mFlagB9(0), mFlagB8(0), mFlagB7(1), mFlagB6(0), mFlagB5(0), mFlagB4(0), mFlagB3(0), mFlagB2(0), mFlagB1(0), mFlagB0(0), m12AC(2), m12B0(0), m12B4(100), m12B8(100),
      m12BC(-1), m12C0(0), m12C4(0), m12C8(0), m12CC(0.0f), m1374(0), m137C(0), m1380(0), m1384(0),
      m1388(0), m138A(0), m138C(0), m1390(rand() % 4), m1398(0), m139C(0), m13AC(0), m13B0(0),
      m13B4(1.0f), m13D8(0.0f, 0.0f, 0.0f), m13E4(0.0f), m13E8(0.0f, 0.0f, 0.0f),
      m13F4(4), m13F8(0), m13FC(0), m1418(0), m141C(0.0f),
      m1420(20.0f) {
    if (IsPartyMember_unk()) {
        mC54 = 0;
    }
    if (type != 5 && (int)data != -1) {
        m60 = fn_8020924C(m50);
        if (m60 == 0) {
            if (lbl_8087EEB8 != 0) {
                char message[0x100];
                sprintf(lbl_807C7B28, "ch%04d_%02d", m50 / 100, m50 % 100);
                sprintf(message, "character not found! %s", lbl_807C7B28);
                ((DebugMessages_unk*)lbl_8087EEB8)->fn_800697D8(message);
            }
            m60 = fn_8020924C(0);
        }
        char path[0x100];
        if (fn_802092C0(path, m60->m10)) {
            fn_80136544(path);
        }
        m5C = fn_8020A81C(m60->m14);
    } else {
        m5C = fn_8020A81C(0);
    }
    m68[0] = fn_80204E04(mC50, mC54);
    if (m60 != 0) {
        m7C = fn_8021FC00(m60->m28);
    }
    m568 = m5C->m134;
    mF20.SetIdentity();
    m5B0 = 3.0f;
    m5B4 = 18.0f;
    m5A4.Set_unk(0.0f, 0.0f, 0.0f);
    mColli.mFlags |= 2;
    mColli.mFlags |= 0x80000000;
    mColli.m0C = (int)this;
    m624.clear();
    m614.Set_unk(0.0f, 0.0f, 0.0f);
    m620 = 3.0f;
    m500 = 0.2f;
    m504 = 0.8f;
    m508 = 1.0f;
    m50C = 0.25f;
    if (m60 != 0 && strcmp(m60->m1C, "") != 0) {
        std::string path("/database/motcmd/");
        path += m60->m1C;
        fn_8006AD24(path, "csv");
        m1220.fn_801210B4(path.c_str());
    }
    if (m60 != 0 && m60->m20[0] != 0) {
        std::string path("/database/chara/ext/");
        path += m60->m20;
        fn_8006AD24(path, "exchr");
        mA4.vf0C(path.c_str());
    }
    for (int i = 0; i < 8; i++) {
        m680[i] = 0;
    }
    memset(mFE8, 0, sizeof(mFE8));
    memset(m120C, 0, sizeof(m120C));
    memset(m68, 0, sizeof(m68));
    m1154.m30 = (void*)fn_80218260;
    mFlag26 = 1;
    mStatus.mChara = this;
    mStatus.fn_8013459C();
    m1394 = new (3, "", "", 0) CharaSub_unk(this);
    mRoute.mChara = this;
    mC58.m0C.mChara = this;
    memset(m13B8, 0, sizeof(m13B8));
    memset(m13D0, 0, sizeof(m13D0));
}

extern "C" void* lbl_8087F498;
extern "C" void fn_803EBBCC(void* manager, CharaTask* chara);

// NONMATCHING: 98.8% - in the inlined listener loop the original gives the list r27 and the
// index r28 (ours the other way round); tried a local for the list and other loop forms.
CharaTask::~CharaTask() {
    m1220.fn_80121E14(1);
    if (lbl_8087F498 != 0) {
        fn_803EBBCC(lbl_8087F498, this);
    }
    for (unsigned int i = 0; i < mParts.mCount; i++) {
        delete mParts.mData[i];
    }
    mParts.mCount = 0;
    if (m678 != 0) {
        delete m678;
        m678 = 0;
        if (m67C != 0) {
            delete m67C;
            m67C = 0;
        }
        if (m674 == 0) {
            fn_8014DEE4(0, 0, 0);
        }
        fn_8014F8F4()->fn_8014F860(9, this, 0);
    }
    for (unsigned int i = 0; i < 8; i++) {
        if (m680[i] != 0) {
            delete m680[i];
        }
    }
    for (unsigned int i = 0; i < m6A4.mCount; i++) {
        delete m6A4.mData[i];
    }
    m6A4.mCount = 0;
    delete m480;
    delete m1394;
}

void CharaTask::fn_80136544(const char* path) {
    mAC = 1;
    m84.vf0C(path);
}

struct ModelCamera_unk {
    ModelCamera_unk() : m00(0) {}
    int m00;
    void* m04;
    Camera_unk mCamera; // 0x08
};
// inferred: inlined in fn_8013655C (its literals are @STRING@ objects in .sdata); name invented
inline void CreateCamera_unk(Model_unk* model) {
    model->m208.reset(new (6, "", "", 0) ModelCamera_unk());
}


// inferred: party data (lbl_8087F4F0); one 0x43C-byte block per member (names invented)
struct PartyMember_unk {
    unsigned char mEquipFlags; // inferred: one bit per equipment slot
    unsigned char m001[0x43C - 0x001];
};
struct Party_unk {
    unsigned char m0000[0x671C];
    PartyMember_unk mMembers[7];
};
extern "C" Party_unk* lbl_8087F4F0;
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" char* strstr(const char* s, const char* find);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" int fn_802180A8(const char* name); // inferred: motion name to id
extern "C" int fn_8020EE8C(const char* name); // inferred: equipment slot by name
extern "C" int fn_8044D208(Party_unk* party, int slot, int id);
extern "C" void fn_804776F4(atn::LH_MotionSet* set, Model_unk* model); // inferred: applies the motions
// inferred: node callback of a model (the functor manager of the original uses the default allocator of
// boost::function3, std::allocator<function_base>)
typedef boost::function<void(atn::Matrix3x4*, const atn::NodeChunk&, const atn::ModelNodeCallbackContext&),
                        std::allocator<boost::function_base> >
    NodeCallback_unk;
extern "C" void fn_800F29D0(Model_unk* model, NodeCallback_unk callback);
std::string fn_8006B0C8(const std::string& path); // inferred: directory of a path
namespace std {
string operator+(const string& lhs, const char* rhs); // 0x8006D008
}

// inferred: node edit read from a CSV line or built by hand (0x2C bytes); names invented
struct NodeEdit_unk {
    unsigned char m00[0x2C];
};
extern "C" void fn_80089EE4(NodeEdit_unk* edit, atn::CsvTab* csv);
extern "C" void fn_80089D70(NodeEdit_unk* edit, const char* node, const atn::Vector3& a, const atn::Vector3& b,
                            const atn::Vector3& c, int flags);

// inferred: four floats read from the CSV (MATERIAL_EDIT); name invented
struct Color4_unk {
    Color4_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r;
    float g;
    float b;
    float a;
};

// inferred: adds a model LOD read from the CSV; not an inline function (its "" is the file pool
// one), inlined in fn_801371AC and fn_80137B78 by -ipa file; name invented
static void AddModelLod_unk(Model_unk* model, atn::CsvTab& csv, const char* node) {
    char path[0x100];
    strcpy(path, csv.fn_8005B9CC());
    if (strcmp(node, "") == 0) {
        model->fn_80092F1C(path, 0, fn_800DC288(csv.fn_8005B9CC()));
    } else {
        model->fn_80092F1C(path, node, fn_800DC288(csv.fn_8005B9CC()));
    }
}

// NONMATCHING: ~90% (72/727 lines) - register allocation only: in the equipment loops the original
// gives the flag r29, the index r28 and the pointer r27 (ours the other way round) and the
// attach loops keep the pointer in r29; tried: the flag as int, the checks in an inline helper,
// m680[i] directly or through a pointer, declarations at the top. The node callback type needs
// std::allocator<function_base> (name of its functor manager) and boost::function's extra copies.
int CharaTask::fn_8013655C() {
    int loading = 0;
    switch (mAC) {
    case 1:
        if (m84.fn_80473F50()) {
            loading = 1;
        }
        if (mA4.fn_80473F50()) {
            loading = 1;
        }
        if (loading) {
            break;
        }
        fn_801371AC();
        fn_80138528();
        if (m5C != 0) {
            for (int i = 0; i < 5; i++) {
                if (m5C->m108[i] > 0) {
                    fn_8014FF9C(i, m5C->m108[i], 0);
                }
            }
            for (int i = 0; i < 4; i++) {
                CharaStatusPair_unk pair = m5C->mE8[i];
                new (&mStatus.m2D0[mStatus.m2CC]) CharaStatusPair_unk(pair);
                mStatus.m2CC++;
            }
        }
        mAC = 2;
    case 2: {
        if (mMotion.fn_8008B140()) {
            loading = 1;
        }
        if (m8C.fn_80473F50()) {
            loading = 1;
        }
        if (m98.fn_80473F50()) {
            loading = 1;
        }
        if (fn_8014FAD0()) {
            loading = 1;
        }
        bool equipLoading = false;
        chr::CharaEquip_unk** equips1 = m680;
        for (unsigned int i = 0; i < 8; i++, equips1++) {
            chr::CharaEquip_unk* equip = *equips1;
            if (equip != 0 && equip->fn_8056405C()) {
                equipLoading = true;
            }
        }
        for (unsigned int i = 0; i < m6A4.mCount; i++) {
            if (m6A4.mData[i]->fn_8036354C()) {
                equipLoading = true;
            }
        }
        if (equipLoading) {
            loading = 1;
        }
        if (m121C && m121C->fn_80237874()) {
            loading = 1;
        }
        if (m1220.fn_801210C4()) {
            loading = 1;
        }
        if (loading) {
            break;
        }
        fn_804776F4(&m8C, &mMotion);
        if ((m54C & 0x4000) != 0x4000) {
            fn_804776F4(&m98, &mMotion);
        }
        if (mType == 0 || lbl_8087FA20 == 0) {
            CreateCamera_unk(&mMotion);
        }
        chr::CharaEquip_unk** equips2 = m680;
        for (unsigned int i = 0; i < 8; i++, equips2++) {
            chr::CharaEquip_unk* equip = *equips2;
            if (equip != 0) {
                equip->fn_80566010();
            }
        }
        fn_80137FC0();
        fn_8014C228();
        fn_800F29D0(&mMotion, boost::bind(&CharaTask::fn_80148CC4, this, _1, _2, _3));
        mMotion.fn_80091CFC("head");
        mMotion.fn_80091CFC("neck");
        mMotion.fn_80091CFC("hips");
        mMotion.fn_80091CFC("spine1");
        mMotion.fn_80091CFC("spine");
        mMotion.fn_80091CFC("rightshoulder");
        mMotion.fn_80091CFC("leftshoulder");
        mMotion.fn_80091CFC("eye_L");
        mMotion.fn_80091CFC("eye_R");
        mMotion.fn_80091CFC("eyeball_L");
        mMotion.fn_80091CFC("eyeball_R");
        if (mFlagB27) {
            mMotion.fn_80091CFC("hair_b");
        }
        if (m50 == 1 || m50 / 1000 == 100) {
            mFlagB25 = 1;
        }
        m524 = mMotion.fn_80092814("reference", 0);
        m520 = mMotion.fn_80092814("neck", 0);
        if (mMotion.mB0 < 0) {
            mMotion.fn_80095D44(mMotion.fn_80092814("hips", 0), -1);
        }
        if (strstr(mMotion.fn_8008B130(), "k00.") != 0 || strstr(mMotion.fn_8008B130(), "k01.") != 0) {
            mMotion.fn_80099E9C("hips", "nk000", 0.5325372f);
        }
        if (m50 == 700023) {
            mMotion.fn_80099E9C("hips", "em003", 1.6f);
        }
        mLook.fn_801298EC(this);
        m1188.fn_8011D424(&mMotion);
        m1220.fn_80121114(this);
        for (unsigned int i = 0; i < m6A4.mCount; i++) {
            m6A4.mData[i]->fn_80363960(&mMotion);
        }
        mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 1.0f, 0.1f);
        if (m12C0 > 0) {
            mMotion.mTracks[0].m08 = (float)m12C0 * ((float)(rand() % 1001) / 1000.0f);
        }
        mMotion.fn_80097E80(1);
        fn_8000D430(&mMotion, boost::function<void()>());
        mMotion.fn_80097510();
        if (mMotion.IsFlag5_unk()) {
            mMotion.mFlag4_unk = 1;
            chr::CharaEquip_unk** equips3 = m680;
        for (unsigned int i = 0; i < 8; i++, equips3++) {
                chr::CharaEquip_unk* equip = *equips3;
                if (equip != 0) {
                    equip->fn_805663F8();
                }
            }
        }
        m13B8[0] = mPos;
        m13B8[1] = mPos;
        m13D0[0] = mRot.y;
        m13D0[1] = mRot.y;
        vf20();
        vf50();
        if (mMotion.fn_80097D40(0x1DC) == 0) {
            mStatus.m18 |= 0x80;
        }
        if ((m5C->mParam.m9C & 0x10000) == 0x10000) {
            mStatus.m18 |= 4;
        }
        if (mMotion.fn_80097D40(0x45) == 0) {
            mC10 |= 2;
        }
        if (mMotion.fn_80097D40(0x1DD) == 0) {
            mC10 |= 4;
        }
        if (mMotion.fn_80097D40(0x46) == 0) {
            mC10 |= 0x80;
        }
        if (mMotion.fn_80097D40(0x68) == 0) {
            mC10 |= 0x20;
        }
        if (mMotion.fn_80097D40(0x1DE) == 0) {
            mC10 |= 0x2000;
        }
        if ((m137C & 8) == 8) {
            mMotion.mFlags &= ~4;
        }
        if (mFlag22 == 1 || mMotion.fn_80097D40(0x8E) == 0) {
            m54C |= 0x1000000;
        }
        if (m50 == 203001) {
            mStatus.m18 |= 1;
            mStatus.m18 |= 0x400000;
            mStatus.m18 |= 0x100;
            mStatus.m18 |= 0x10000;
        }
        if (mMotion.fn_80097D40(0x5D) == 0) {
            m54C |= 0x4000000;
        }
        mStatus.fn_8012B3E8();
        mStatus.fn_8012B988();
        mStatus.fn_8012D8B8();
        mStatus.m224 = mStatus.mParamB.m98;
        fn_8014FD30();
        mAC = 3;
    }
    }
    return loading;
}

int CharaTask::fn_801370B8(int index) {
    return m484[index];
}

// NONMATCHING: ~88% (74/627 lines) - register allocation only (the original reuses r16, the field
// pointer, in the branches; ours keeps other values in r18-r22); tried: loop forms of the model
// name table, named strings, range check forms.
void CharaTask::fn_801371AC() {
    atn::CsvTab csv((const char*)m84.fn_80470580(), m84.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    char node[0x100] = "";
    do {
        const char* key = csv.fn_8005B9CC();
        if (key[0] == ';') {
            continue;
        }
        if (strcmp(key, "MODEL") == 0) {
            char path[0x100];
            strcpy(path, csv.fn_8005B9CC());
            strcpy(node, csv.fn_8005B9CC());
            for (unsigned int i = 0; i < 11; i++) {
                if (strstr(path, sModelNames_unk[i].mFrom) != 0) {
                    strcpy(path, sModelNames_unk[i].mTo);
                    break;
                }
            }
            if (strcmp(node, "") == 0) {
                mMotion.fn_8008AD4C(path, 0);
            } else {
                mMotion.fn_8008AD4C(path, node);
            }
            fn_80137E68(&mMotion, &csv);
        } else if (strcmp(key, "MODEL_LOD") == 0) {
            AddModelLod_unk(&mMotion, csv, node);
        } else if (strcmp(key, "MODEL_LOD_BATTLE") == 0) {
            if (!IsTownField_unk()) {
                AddModelLod_unk(&mMotion, csv, node);
            }
        } else if (strcmp(key, "MODEL_LOD_TOWN") == 0) {
            if (IsTownField_unk()) {
                AddModelLod_unk(&mMotion, csv, node);
            }
        } else if (strcmp(key, "SHADOW_LOD") == 0) {
            char path[0x100];
            strcpy(path, csv.fn_8005B9CC());
            mMotion.fn_80092EC8(path);
        } else if (strcmp(key, "MOTION") == 0) {
            char path[0x100];
            strcpy(path, csv.fn_8005B9CC());
            mMotion.fn_80097A88(fn_802180A8(csv.fn_8005B9CC()), path);
        } else if (strcmp(key, "MOTION_SET") == 0) {
            std::string path(csv.fn_8005B9CC());
            fn_8006AD24(path, "mchr");
            if (path.find("na000_00.mchr") != std::string::npos && IsTownField_unk()) {
                path = fn_8006B0C8(path) + "/na000_00_town.mchr";
            }
            m8C.vf0C(path.c_str());
        } else if (strcmp(key, "MOTION_SET_EX") == 0) {
            if (m60 == 0 || strcmp(m60->m18, "") == 0) {
                m98.vf0C(csv.fn_8005B9CC());
            }
        } else if (strcmp(key, "EFP") == 0) {
            m121C.reset(new (1, "", "", 0) atn::EfpHandle_unk());
            m121C->fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(key, "EQUIP") == 0) {
            m6A4.PushBack_unk(new (1, "", "", 0) EquipItemData(&csv));
        } else if (strcmp(key, "PROTECTOR") == 0) {
            int slot = fn_8020EE8C(csv.fn_8005B9CC());
            char name[0x100];
            const char* src = csv.fn_8005B9CC();
            if (src != name) {
                memcpy(name, src, strlen(src) + 1);
            }
            int flag = atoi(csv.fn_8005B9CC());
            if (lbl_8087F4F0 != 0 && flag == 0) {
                int equipped = 0;
                if (lbl_8087F4F0 != 0 && IsPartyIndex_unk(fn_80219558(m50))) {
                    int index = fn_80219558(m50);
                    unsigned int mask = 1 << slot;
                    equipped = (mask & lbl_8087F4F0->mMembers[index].mEquipFlags) == mask;
                }
                fn_8014FF9C(slot, fn_8044D208(lbl_8087F4F0, slot, m50), equipped);
            } else {
                chr::CharaEquip_unk* equip = new (0, "", "", 0) chr::CharaEquip_unk(slot, -1, 0, this, 0, 0, 0, 1, 1);
                std::string path(name);
                equip->fn_80563A1C(path.c_str());
                if (m680[slot] != 0) {
                    delete m680[slot];
                }
                m680[slot] = equip;
                if (slot == 0) {
                    fn_8014FD30();
                }
            }
        }
    } while (csv.fn_8005BBF8());
}

void CharaTask::fn_80137B78() {
    mMotion.fn_80092F90();
    mMotion.fn_80093014(1);
    atn::CsvTab csv((const char*)m84.fn_80470580(), m84.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* key = csv.fn_8005B9CC();
        if (key[0] == ';') {
            continue;
        }
        if (strcmp(key, "MODEL_LOD") == 0) {
            AddModelLod_unk(&mMotion, csv, "");
        } else if (strcmp(key, "MODEL_LOD_BATTLE") == 0) {
            if (!IsTownField_unk()) {
                AddModelLod_unk(&mMotion, csv, "");
            }
        } else if (strcmp(key, "MODEL_LOD_TOWN") == 0) {
            if (IsTownField_unk()) {
                AddModelLod_unk(&mMotion, csv, "");
            }
        }
    } while (csv.fn_8005BBF8());
}

void CharaTask::fn_80137E68(Model_unk* model, atn::CsvTab* csv) {
    for (const char* key = csv->fn_8005B9CC(); key != 0 && strcmp("", key) != 0; key = csv->fn_8005B9CC()) {
        if (strcmp(key, "REFLECT") == 0) {
            model->mFlags |= 0x400;
        } else if (strcmp(key, "REFRACT") == 0) {
            model->mFlags |= 0x800;
        } else if (strcmp(key, "SHADOW_CASTER") == 0) {
            model->mFlags |= 4;
        } else if (strcmp(key, "SHADOW_CASTER_OFF") == 0) {
            model->mFlags &= ~4;
        } else if (strcmp(key, "SHADOW_RECEIVER") == 0) {
            model->mFlags |= 8;
        } else if (strcmp(key, "SHADOW_RECEIVER_OFF") == 0) {
            model->mFlags &= ~8;
        }
    }
}

// inferred: node index by name, -1 for an empty name (not inline: its "" is the file pool one); name
// invented
static int GetNodeIndex_unk(Model_unk* model, const char* name) {
    if (strcmp(name, "") == 0) {
        return -1;
    }
    return model->fn_80092814(name, 0);
}

void CharaTask::fn_80137FC0() {
    atn::CsvTab csv((const char*)m84.fn_80470580(), m84.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "OPTION_PARTS") == 0) {
            char name[0x100];
            strcpy(name, csv.fn_8005B9CC());
            mMotion.fn_8009373C(name, fn_800DC12C(csv.fn_8005B9CC()));
        } else if (strcmp(key, "MATERIAL_EDIT") == 0) {
            char name[0x100];
            strcpy(name, csv.fn_8005B9CC());
            float r = fn_800DC288(csv.fn_8005B9CC());
            float g = fn_800DC288(csv.fn_8005B9CC());
            float b = fn_800DC288(csv.fn_8005B9CC());
            Color4_unk diffuse(r, g, b, fn_800DC288(csv.fn_8005B9CC()));
            r = fn_800DC288(csv.fn_8005B9CC());
            g = fn_800DC288(csv.fn_8005B9CC());
            b = fn_800DC288(csv.fn_8005B9CC());
            Color4_unk ambient(r, g, b, fn_800DC288(csv.fn_8005B9CC()));
            r = fn_800DC288(csv.fn_8005B9CC());
            g = fn_800DC288(csv.fn_8005B9CC());
            b = fn_800DC288(csv.fn_8005B9CC());
            Color4_unk specular(r, g, b, fn_800DC288(csv.fn_8005B9CC()));
            mMotion.fn_80094958(name, &diffuse, &ambient, &specular);
        } else if (strcmp(key, "MATERIAL_HIDE") == 0) {
            mMotion.fn_80093D98(csv.fn_8005B9CC(), 0);
        } else if (strcmp(key, "NODE_EDIT") == 0) {
            NodeEdit_unk edit;
            fn_80089EE4(&edit, &csv);
            mMotion.fn_80093F58(&edit);
        } else if (strcmp(key, "COLLISION") == 0) {
            if (m50 != 700019) {
                m5B0 = fn_800DC288(csv.fn_8005B9CC());
                m5B4 = fn_800DC288(csv.fn_8005B9CC());
                mFlag13 = atoi(csv.fn_8005B9CC()) == 0;
                mFlag12 = atoi(csv.fn_8005B9CC());
            }
        } else if (strcmp(key, "SCALE") == 0) {
            m540.x = fn_800DC288(csv.fn_8005B9CC());
            m540.y = fn_800DC288(csv.fn_8005B9CC());
            m540.z = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "PBONE") == 0) {
            if (lbl_8087F610 == 0) {
                mMotion.fn_800973C0(&csv);
            }
        } else if (strcmp(key, "BOUND_CENTER_NODE") == 0) {
            const char* name = csv.fn_8005B9CC();
            int node = GetNodeIndex_unk(&mMotion, name);
            name = csv.fn_8005B9CC();
            mMotion.fn_80095D44(node, GetNodeIndex_unk(&mMotion, name));
        } else if (strcmp(key, "PRESET_MOTION_SPEED_RATIO") == 0) {
            m514 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "HAIR_BACK_CONTROL") == 0) {
            mFlagB27 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "USE_MAP_BRIGHTNESS") == 0) {
            mFlagB25 = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
    if (m50 == 700017 || m50 == 700018) {
        atn::Vector3 scale(1.0f, 1.0f, 1.0f);
        NodeEdit_unk edit;
        fn_80089D70(&edit, "hair_r", lbl_807C7030, lbl_807C7030, scale, 8);
        mMotion.fn_80093F58(&edit);
    }
}

// NONMATCHING: ~77% (52/228 lines) - register allocation only: r28 and r29 are swapped for the
// appended name and the CSV data; tried: append(), a named extension string, string forms.
void CharaTask::fn_80138528() {
    if (m60 != 0 && strcmp(m60->m18, "") != 0) {
        std::string path("/database/chara/motion/");
        path += m60->m18;
        fn_8006AD24(path, "mchr");
        m98.vf0C(path.c_str());
    }
    if (mA4.fn_80470580() != 0) {
        atn::CsvTab csv((const char*)mA4.fn_80470580(), mA4.fn_8047059C());
        csv.fn_8005BCE8(lbl_8087D710);
        do {
            const char* key = csv.fn_8005B9CC();
            if (key[0] == ';' || key[0] == '\0') {
                continue;
            }
            if (strcmp(key, "RANDOM_MOTION") == 0) {
                m12AC = atoi(csv.fn_8005B9CC());
                m12B4 = atoi(csv.fn_8005B9CC());
                m12B8 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(key, "WAIT_OFFSET") == 0) {
                m12C0 = atoi(csv.fn_8005B9CC());
            } else if (strcmp(key, "EFP") == 0) {
                m121C.reset(new (1, "", "", 0) atn::EfpHandle_unk());
                m121C->fn_8023780C(csv.fn_8005B9CC());
            }
        } while (csv.fn_8005BBF8());
    }
}

// NONMATCHING: the original is over the inlining budget and calls every inline candidate out of line
// (std::string, char_separator, tokenizer and iterator members, atn::Csv, atn::Vector_unk<int>:
// weak copies 0x801360C4, 0x801360D8, 0x80138A50); this compile inlines all of them (same open case
// as LoadSeTableTask::vf0C, docs/inlining.md).
void CharaTask::vf2C(const char* params, int arg) {
    std::string str(params);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(str, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp((*it).c_str(), "path=", 5) == 0) {
            std::string value((*it).c_str());
            atn::Csv csv(value.c_str() + 5, value.size() - 5);
            atn::Vector_unk<int> points;
            int id;
            while ((id = atoi(csv.fn_8005B9CC())) > 0) {
                points.push_back(id);
            }
            fn_8017B1C4(&points);
        }
    }
}

// inferred: (b - a) * t + a, the operands in the order of the original (atn::Lerp_unk adds a first)
inline atn::Vector3 LerpFrom_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}
atn::Vector3 CharaTask::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else {
        result = LerpFrom_unk(mColli.m3C, mColli.m48, 0.2f);
    }
    return result;
}

// NONMATCHING: ~76% (112/461 lines) - the inlined fn_80178208/fn_8016E970 chain: the stack slots of
// two ptmf temporaries are swapped and "m55C = 6" is stored before the mControl load (see
// fn_8016E970); everything else matches.
int CharaTask::fn_80138E1C(int arg) {
    int result = 0;
    if (arg == 0) {
        switch (m12AC) {
        case 1: {
            float frame = mMotion.mTracks[0].m08;
            if (frame > mMotion.fn_80097D7C(0) - 1.2f) {
                frame = mMotion.mTracks[0].m08;
                if (frame < mMotion.fn_80097D7C(0)) {
                    m12B0++;
                }
            }
            if (m12B0 >= m12B4) {
                m12B0 = 0;
                int motion = rand() % 4 + 0x71;
                fn_80178208(new (0, "", "", 0) chr::ControlOneMotion(this, motion, 0, 0, -1.0f, -1.0f));
                result = 1;
            }
            break;
        }
        case 2:
            if (m12B4 > 0 && rand() % m12B4 == 0) {
                int motion = rand() % 4 + 0x71;
                fn_80178208(new (0, "", "", 0) chr::ControlOneMotion(this, motion, 0, 0, -1.0f, -1.0f));
                result = 1;
            }
            break;
        }
    } else if (m12B8 > 0 && m12BC == -1 && rand() % m12B8 == 0) {
        m12BC = rand() % 4 + 0x71;
        mMotion.fn_80097C08(3, m12BC, 0, 0, 1, 0.0f, 0.1f);
        result = 1;
    }
    return result;
}

float Model_unk::fn_80139550(int track) {
    return mTracks[track].m08;
}

void* fn_80139EF4();
Effect_unk* fn_8013A194();
extern "C" Field_unk* fn_80121F00();
extern "C" Settings_unk* fn_800F52F8();
extern "C" void* fn_800F7F90();
extern "C" int fn_801231D0(void* pad, int arg); // inferred: button pressed
extern "C" void fn_80017064(void* routes, CharaTask* chara);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);

namespace chr {
// inferred: control of chr/ControlCannonLift.cpp (vtable stored by 0x801A1BE4)
class ControlCannonTarget : public ControlBase {
public:
    ControlCannonTarget(Chara_unk* chara); // 0x801A1BE4
    virtual int vf0C();
    unsigned char m08[0x30 - 0x08];
};
} // namespace chr

void CharaTask::fn_80139560() {
    atn::Vector3 rot(mRot);
    float a = 0.0f;
    float b = 1.0f;
    if (mFlag11) {
        return;
    }
    m1394->fn_8013537C();
    int state = m55C;
    if (state == 0 || state == 5 || state == 9 || mFlag2) {
        rot = mRot;
    } else if (state == 1) {
        if (fn_8017C974() || mFlag6 || fn_80139E90()) {
            rot = mRot;
        } else {
            a = fn_80178078();
            rot = fn_80178018();
            if (a < 0.4f) {
            } else {
                float t = (a - 0.4f) / 0.6f;
                a = 0.6f * (t * t) + 0.4f;
            }
            if (fn_80139E9C()) {
                b = 1.0f;
                if (fn_801231D0(fn_800F52F8()->fn_80116BAC(), 0x1F)) {
                    fn_801750FC();
                } else if (fn_801231D0(fn_800F52F8()->fn_80116BAC(), 0x20)) {
                    fn_80178208(new (0, "", "", 0) chr::ControlCannonTarget(this));
                    return;
                }
            } else if (fn_80139EB0()) {
                b = 1.0f;
            } else if (fn_80139EBC()) {
                b = 1.0f;
            } else if (fn_80121F00() && fn_80370174(fn_80121F00(), 3) == 1) {
                b = 0.4f;
            } else if (fn_80121F00() && fn_80370174(fn_80121F00(), 3) == 2) {
                b = 0.5f;
            } else {
                if (!fn_80139ED0() && !fn_80139EDC()) {
                    b = 1.3f;
                    if (fn_80139EE8()) {
                        b *= 2.0f;
                    }
                } else {
                    b = 0.8f;
                }
                if (fn_80121F00() && fn_80370174(fn_80121F00(), 3) == 3) {
                    b *= 0.75f;
                }
            }
        }
    } else if (state == 2) {
        if (fn_80139EF4() == 0) {
        } else if (!fn_80139EFC() && IsPartyMember_unk() && !fn_80139F04()) {
            mMotion.fn_80139F24(1);
            if (mMotion.fn_80139F2C(0) != fn_801370B8(0)) {
                mMotion.fn_80097C08(0, fn_801370B8(0), 1, 0, 1, 0.0f, 0.1f);
                mMotion.fn_80139F3C(0, 1.0f);
            }
            fn_80138E1C(0);
            m570 = 0.0f;
            m574.Set_unk(0.0f, 0.0f, 0.0f);
            return;
        } else if (fn_80139F04() && fn_8017B3C0()) {
            fn_8017B434();
        } else if (!mC58.m11C.fn_8011D1FC() || mC58.m11C.fn_8011D21C()) {
            if (mC58.m00 == 0) {
                fn_80017064(fn_80139EF4(), this);
            }
            fn_80017064(fn_80139EF4(), this);
            fn_8013A4E8(&a, &rot, &b);
        }
    } else if (state == 7) {
        if (mStatus.fn_80139F4C()) {
            a = 0.0f;
            rot = mRot;
        } else {
            mRoute.fn_80126214();
            atn::Vector3 dir(*mRoute.fn_80139F58());
            a = dir.Length_unk();
            if (a > 0.0001f) {
                rot = dir.GetNormalized_unk().ToRotation_unk();
            } else {
                rot = mRot;
            }
            if (IsPlayer_unk() && fn_80121F00() && fn_80370174(fn_80121F00(), 3) == 1) {
                b = 0.4f;
            }
            if (mC58.m11C.fn_8013A144()) {
                if ((int)mC58.m11C.m00 == 6) {
                    b *= 0.6f;
                } else {
                    b *= 0.4f;
                }
            }
        }
    } else if (state == 6) {
        if (mControl) {
            if (mControl->vf0C()) {
                boost::function<void()> next = mControl->vf10();
                mControl.reset();
                if (next) {
                    next();
                }
                if (m55C == 6 && !mControl) {
                    fn_8016E970(fn_8013A150());
                }
            } else if (m560 == 22 || m560 == 23) {
                if ((m54C & 0x100) == 0x100) {
                    if (fn_8013A158() && !mStatus.IsState_unk(0x100) && !mStatus.fn_80139F4C() && !fn_800F7F90()) {
                        fn_801698E4(m574.ToRotation_unk());
                    }
                    m54C &= ~0x100;
                } else {
                    fn_80171F3C(&mPos, &m6B8);
                }
            } else if (m560 == 104) {
                fn_80171F3C(&mPos, &m574);
            }
        }
        m580 *= 0.9f;
        m584 *= 0.9f;
        return;
    } else if (state == 10) {
        return;
    }
    if ((m55C == 5 || m55C == 1) && fn_80139E9C()) {
        if (m1208->fn_8013A184() != 2 || !m1208->fn_8013A18C()) {
            m1208 = 0;
        }
    }
    if (!mFlag2) {
        vf34(&rot, 0, a, b);
    } else {
        fn_8013CB68(&rot, 0, a, b);
    }
    if ((m54C & 0x100) == 0x100) {
        if (m55C == 1) {
            fn_8016BBCC(&m574, 15, -1);
        } else if (m55C == 2) {
            fn_8016BBCC(&m574, 1000, -1);
        }
        m54C &= ~0x100;
    }
    if ((m54C & 0x20000) == 0x20000 || (m54C & 0x40000) == 0x40000) {
        if (fn_8013A194()) {
            HitInfo_unk* bullet = fn_80219E6C(0x7DE);
            m13A0.y += 6.0f;
            fn_8013A194()->fn_8013A19C(1);
            fn_800FAB80(fn_8013A194(), this, bullet, fn_800F8548(fn_8013A194()), &m13A0, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            fn_8013A194()->fn_8013A19C(0);
        }
        m54C &= ~0x60000;
    }
    if ((m54C & 0x100000) == 0x100000) {
        if (fn_8013A194()) {
            HitInfo_unk* bullet = fn_80219E6C(0x7E8);
            fn_800FAB80(fn_8013A194(), 0, bullet, fn_800F8548(fn_8013A194()), &m13A0, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        m54C &= ~0x100000;
    }
}

int CharaTask::fn_80139E90() {
    return mFlag27;
}

int CharaTask::fn_80139E9C() {
    return m1208 != 0;
}

int CharaTask::fn_80139EB0() {
    return mFlag3;
}

int CharaTask::fn_80139EBC() {
    return mF54 != 0;
}

int CharaTask::fn_80139ED0() {
    return mFlag5;
}

int CharaTask::fn_80139EDC() {
    return mFlag4;
}

int CharaTask::fn_80139EE8() {
    return (m54C >> 6) & 1;
}

void* fn_80139EF4() {
    return lbl_8087EE68;
}

int CharaTask::fn_80139EFC() {
    return mC54;
}

int CharaTask::fn_80139F04() {
    return m1400.mSize >= 2;
}

void Model_unk::fn_80139F24(int value) {
    m34C = value;
}

int Model_unk::fn_80139F2C(int track) {
    return mTracks[track].m00;
}

void Model_unk::fn_80139F3C(int track, float speed) {
    mTracks[track].m0C = speed;
}

int CharaStatus::fn_80139F4C() {
    return (m0C >> 5) & 1;
}

atn::Vector3* RouteWalker_unk::fn_80139F58() {
    return &m58;
}

// inferred: rotation around y returned by value (callers build it in place); name invented
inline atn::Matrix3x4 RotY_unk(float rad) {
    atn::Matrix3x4 r;
    r.fn_8013A13C(rad);
    return r;
}
// inferred: object at CharaTask+0xD74 (class unknown)
int CharaD74_unk::fn_8013A144() {
    return m20 & 1;
}

int CharaTask::fn_8013A150() {
    return m564;
}

bool CharaTask::fn_8013A158() {
    return mType == 2;
}

int CharaObj1208_unk::fn_8013A184() {
    return m54;
}

int CharaObj1208_unk::fn_8013A18C() {
    return m9C;
}

Effect_unk* fn_8013A194() {
    return lbl_8087F048;
}

void Effect_unk::fn_8013A19C(int value) {
    m3E39C = value;
}

// NONMATCHING: 99.9% - same instructions; only the .data offsets of its two member-pointer
// constants differ until the functions before it are written.
void CharaTask::fn_8013A258() {
    if (mControl) {
        if (mControl->vf0C()) {
            boost::function<void()> next = mControl->vf10();
            if (next) {
                next();
            } else {
                mControl.reset();
            }
        }
    }
}

extern "C" float fn_80011220(float x); // inferred: fabs (weak copy of another helper)
extern "C" float fn_80122560(float x); // inferred: fabs (weak copy of another helper)
extern "C" int fn_8003F440(CharaTask* chara, SkillRef_unk* data);
extern "C" int fn_80018BAC(void* routes, CharaTask* chara);
extern "C" int fn_80018240(void* routes, CharaTask* chara, atn::Vector3* pos, float* radius);
extern "C" int fn_80018078(void* routes, CharaTask* chara);
extern "C" int fn_80017F18(void* routes, CharaTask* chara, atn::Vector3* pos);

// inferred: moves the character for its current mode (mC58.mMode); called by fn_80139560 with the
// speed, direction and rate passed to vf34.
// NONMATCHING: 98.5% - the two node names ("righthand", "head") go to an @STRING@ pool instead of
// @stringBase0 because the function is inline; the Matrix3x4 of mode 10 calls its empty constructor
// (and gets f30 instead of f31); the 0.0f argument of fn_80097C08 is loaded later. The original is
// probably not inline (its literals are in the file pool), but only as an inline candidate does it
// make fn_80139560 reject its helpers as the original does (with a non-inline definition this
// function is at 99.0% and fn_80139560 inlines everything). Tried: non-inline, static, accessors
// as inline functions, small auto-inline stubs of the other callees of fn_80139560.
inline int CharaTask::fn_8013A4E8(float* a, atn::Vector3* rot, float* b) {
    RouteWalker_unk* route = &mC58.m0C;
    if (mC58.m2B4 > 0) {
        mC58.m2B4--;
    }
    if (mC58.mC4 != 0 && mC58.mE0_11 && mC58.mC4->vf78()) {
        atn::Vector3 dir = *mC58.mC4->fn_8013C38C() - mPos;
        dir.y = 0.0f;
        dir = atn::Vector3::fn_8013C394(dir.GetNormalized_unk(),
                                        atn::Vector3(0.0f, mC58.mE0_10 ? 1.0f : -1.0f, 0.0f));
        fn_80155790(dir);
        mC58.mE0_10 = !mC58.mE0_10;
        return 0;
    }
    switch (mC58.mMode) {
    case 0:
        if (mC58.mBC > 0 && mC58.m2B4 <= 0) {
            mC58.mBC--;
        }
        if (mC58.mBC <= 0) {
            mC58.mDone_unk = 1;
        }
        if (fn_8013C3A8()) {
            if (route->m1C == 1 && fn_80153B44() != 1) {
                *a = 1.0f;
                *rot = (-mC14).ToRotation_unk();
            } else if (route->m1C == 2 && fn_80153B44() != 2) {
                *a = 1.0f;
                *rot = mC14.ToRotation_unk();
            }
        } else if (mC58.mC4 != 0) {
            atn::Vector3 dir = *mC58.mC4->fn_8013C38C() - mPos;
            if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                *rot = dir.ToRotation_unk();
            }
        }
        if (IsPartyMember_unk() && mMotion.fn_80139F2C(0) == fn_801370B8(0) &&
            (mMotion.fn_8013C3DC() == 1 || mMotion.fn_8013C3E4(0) > 0.99f)) {
            if (fn_80138E1C(0)) {
                mC58.mBC = MAX(1, mC58.mBC - (int)mMotion.fn_80097D7C(0));
            }
            return 1;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 18:
    case 19: {
        route->fn_80126214();
        if (route->fn_8013C3F4()) {
            mC58.mDone_unk = 1;
        }
        atn::Vector3 dir(*route->fn_80139F58());
        *a = dir.Length_unk();
        if (*a > 0.0001f) {
            atn::Vector3 n = dir.GetNormalized_unk();
            *rot = n.ToRotation_unk();
            m1394->fn_80135338(*route->fn_8013C3FC(), n);
        } else {
            *rot = mRot;
        }
        if (fn_8013C404(0x20) && fn_800F52F8()->fn_8013C41C() && route->fn_8013C424()) {
            m1394->fn_80135380();
        }
        if (IsPartyMember_unk()) {
            fn_80138E1C(1);
        }
        break;
    }
    case 4:
        if (mC58.mC4 != 0) {
            if (mC58.mBC > 0) {
                mC58.mBC--;
                if (mC58.mC4 != 0 && mC58.mC4->fn_8015061C(this)) {
                    if ((mC58.mBC -= 2) < 0) {
                        mC58.mBC = 0;
                    }
                }
            }
            if (mC58.mBC <= 0) {
                if (((unsigned int)mStatus.m14 & 0x2000) == 0x2000 ? true
                    : mC58.mC4->fn_8016F4D8(this)                    ? true
                                                                      : mC58.mC4->fn_8016F530(0) != 0) {
                    mC58.mDone_unk = 1;
                    mC58.mC4->fn_8016F530(this);
                }
            }
            atn::Vector3 dir = *mC58.mC4->fn_8013C38C() - mPos;
            if (fn_80122560(dir.y) > 30.0f) {
                dir.y = 0.0f;
            }
            *a = dir.Length_unk();
            if (mC58.mC4->fn_8013C42C() > 20.0f) {
                *a -= mC58.mC4->fn_8013C42C();
            }
            if (*a < 0.00001f) {
                *a = 0.0f;
                *rot = mRot;
                break;
            }
            dir.Normalize_unk();
            if (*a < 0.8f * m64->m30) {
                if (*a < 20.0f) {
                    *a = 1.0f;
                } else {
                    *a = 0.75f;
                }
                *rot = (-dir).ToRotation_unk();
                break;
            }
            if (*a > 1.2f * m64->m30) {
                *a = 1.0f;
                *rot = dir.ToRotation_unk();
                break;
            }
            if (fn_800F52F8()->fn_8013C434()->m00 != 0) {
                atn::Vector3 pos = fn_8013A194()->fn_80109950(this);
                atn::Vector3 dir2 = pos - mPos;
                *a = dir2.Length_unk();
                if (*a > 0.0f) {
                    dir2.Normalize_unk();
                }
                if (*a < 3.0f) {
                    if (*a > 0.0f) {
                        *rot = dir2.ToRotation_unk();
                    } else {
                        *rot = mRot;
                    }
                    *a = 0.0f;
                } else if (*a > 10.0f) {
                    if (*a > 40.0f && rand() % 50 < 1) {
                        fn_80155790(dir2.GetNormalized_unk());
                    } else if (atn::Vector3::fn_8013C394(dir, dir2).y >= 0.0f) {
                        *rot = atn::Vector3::fn_8013C394(atn::Vector3(0.0f, 1.0f, 0.0f), dir).ToRotation_unk();
                    } else {
                        *rot = atn::Vector3::fn_8013C394(dir, atn::Vector3(0.0f, 1.0f, 0.0f)).ToRotation_unk();
                    }
                } else {
                    *rot = dir2.ToRotation_unk();
                }
                *b = fn_800F52F8()->fn_8013C434()->m18;
            } else {
                *a = 0.0f;
                *rot = dir.ToRotation_unk();
            }
        } else {
            mC58.mDone_unk = 1;
        }
        break;
    case 5:
        if (mC58.mC4 != 0) {
            if (mC58.mStep == 0) {
                route->fn_80126214();
                if (route->mPointA == route->mPointB) {
                    route->fn_80127D8C(0, 0, 0);
                } else {
                    atn::Vector3 dir(*route->fn_80139F58());
                    *a = dir.Length_unk();
                    if (*a > 0.0001f) {
                        *rot = dir.GetNormalized_unk().ToRotation_unk();
                    } else {
                        *rot = mRot;
                    }
                }
                if (route->fn_8013C3F4()) {
                    mC58.mStep = 1;
                }
            } else if (mC58.mStep == 1) {
                mC58.mE8++;
                atn::Vector3 dir = mC58.fn_8011BF3C(0) - mPos;
                if (fn_80122560(dir.y) > 30.0f) {
                    dir.y = 0.0f;
                }
                *a = dir.Length_unk();
                *a -= mC58.mC4->fn_8013C42C();
                if (*a > GetStatus_unk()->mParamB.m28) {
                    *rot = dir.ToRotation_unk();
                } else {
                    *a = 0.0f;
                    *rot = mRot;
                    mC58.mStep = 2;
                }
                if (mC58.mE8 > 60) {
                    mC58.mStep = 0;
                    mC58.mE8 = 0;
                    route->fn_80128930(mC58.mC4, &mPos, 0);
                    route->fn_801255C8();
                }
            } else if (mC58.mStep == 2) {
                mC58.mStep = 3;
                mC58.mDone_unk = 1;
                mC58.mC4->fn_8016F5EC(this);
            }
        } else {
            if (mC58.mStep == 2) {
                mC58.mStep = 3;
            }
            mC58.mDone_unk = 1;
        }
        break;
    case 6:
        if (mC58.mC4 != 0 || mC58.mD0 != 0) {
            if (!fn_8013C3A8()) {
                int kind = 2;
                if (m648->fn_8013C43C() == 1) {
                    kind = 1;
                }
                atn::Vector3 dir = mC58.fn_8011BF3C(kind) - mPos;
                if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                    dir.Normalize_unk();
                    *rot = dir.ToRotation_unk();
                }
            }
            if (mC58.mStep == 0) {
                mC58.mStep = 1;
            } else if (mC58.mStep == 1) {
                if (mC58.mE8 < (int)(m64 != 0 ? m64->m54 : 0.0f)) {
                    mC58.mE8++;
                } else if (fn_8013C444()) {
                    mC58.mStep = 2;
                }
            } else {
                if (mC58.mEC > 0) {
                    mC58.mEC--;
                }
                if (mC58.mEC > 0) {
                    mC58.mStep = 1;
                } else {
                    mC58.mDone_unk = 1;
                }
            }
        } else {
            mC58.mStep = 0;
            mC58.mDone_unk = 1;
        }
        break;
    case 7:
        if (mC58.m2B4 > 0) {
            break;
        }
        if (mC58.mDC != 0 && fn_8003F440(this, mC58.mDC)) {
            HitInfo_unk* data = fn_80219E6C(mC58.mDC->mId);
            if (data != 0 && data->m04 == 0x13B) {
                fn_8016125C(mC58.mDC, mC58.m10C, 0x6A, "righthand", 122.0f);
            } else if (data != 0 && data->m04 == 0x74) {
                fn_8016125C(mC58.mDC, mC58.m10C, 0x13F, "head", 21.0f);
            } else if (data != 0 && ((0x1785 <= data->m04 && data->m04 <= 0x1788) || data->m04 == 0x1791)) {
                fn_8016D74C(data, 0, 0);
            } else {
                if (data != 0 && (int)data->m02 == 4) {
                    mC58.m10C = this;
                }
                atn::Vector3 pos(chr::lbl_807C7030);
                if (mC58.m10C != 0) {
                    pos = *mC58.m10C->fn_8013C38C();
                }
                fn_8015F568(pos, mC58.m10C, data);
            }
            return 1;
        }
        mC58.mDone_unk = 1;
        break;
    case 8:
    case 9:
    case 15: {
        float near = 0.8f * mC58.mRadius;
        float far = 1.2f * mC58.mRadius;
        if (mC58.mStep == 0) {
            if (!fn_80139ED0() && fn_8013C458() && fn_8013C458()->fn_8013C43C() == 0) {
                fn_8013C460(1);
                fn_8013C470(mC58.mC4);
            }
            if (fn_8013C478() == 0) {
                fn_8013C470(mC58.mC4);
            }
            atn::Vector3 v;
            if (mC58.mC4 != 0) {
                v = *mC58.mC4->fn_8013C38C() - mC58.mDest;
            } else {
                v = mPos - mC58.mDest;
            }
            float len = v.Length_unk();
            if (len > 0.0f) {
                v.Normalize_unk();
            }
            float dist = len / 2.0f;
            if (dist > near) {
                dist = near;
            }
            atn::Vector3 dest = mC58.mDest + v * dist;
            dest.y = mPos.y;
            atn::Vector3 dir = dest - mPos;
            *a = dir.Length_unk();
            if (*a > 0.0f) {
                dir.Normalize_unk();
            }
            if (*a < 0.1f) {
                *a = 0.0f;
                *rot = mRot;
            } else if (*a < 10.0f) {
                *a = 0.0f;
                *rot = dir.ToRotation_unk();
            } else if (*a < 40.0f) {
                *rot = dir.ToRotation_unk();
            } else if (*a < 70.0f) {
                fn_80155790(dir);
            } else {
                mC58.mStep = 1;
                if (mC58.mMode == 9) {
                    route->fn_80128930(mC58.mCC, &mPos, 0);
                } else if (mC58.mMode == 8) {
                    route->fn_80128818(mC58.mPlace, &mPos, 0);
                } else {
                    route->fn_80128A30(&mC58.mDest, &mPos, 8, 0.0f);
                    route->mRadius = mC58.mRadius;
                }
                route->fn_801255C8();
            }
        } else if (mC58.mStep == 1) {
            if (fn_80139ED0()) {
                fn_8013C460(0);
                fn_8013C470(0);
            }
            route->fn_80126214();
            atn::Vector3 dir(*route->fn_80139F58());
            if (dir.LengthSq_unk() > 0.0f) {
                *a = dir.Length_unk();
                dir.Normalize_unk();
                *rot = dir.ToRotation_unk();
            }
            if (route->fn_8013C3F4() || (mPos - mC58.mDest).LengthSq_unk() < far * far) {
                mC58.mStep = 0;
            }
        }
        if (mC58.mMode == 9) {
            if (mC58.mCC == 0 || !mC58.mCC->CanAct_unk()) {
                mC58.mStep = -1;
                break;
            }
            mC58.mDest = *mC58.mCC->fn_8013C38C();
        } else if (mC58.mMode == 15) {
            if (fn_80018BAC(fn_80139EF4(), this)) {
                mC58.mStep = -1;
            }
        }
        break;
    }
    case 10:
        if (mC58.mC4 != 0 && mC58.mStep == 0) {
            atn::Vector3 dir = *mC58.mC4->fn_8013C38C() - mPos;
            if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                dir.Normalize_unk();
            }
            atn::Matrix3x4 m;
            float sign = 2.0f * (float)(rand() % 2) - 1.0f;
            m.fn_8013A13C(atn::DegToRad_unk(45.0f) * sign);
            dir.fn_80011410(m);
            fn_80155790(dir);
            mC58.mStep = 1;
        } else {
            mC58.mDone_unk = 1;
        }
        break;
    case 11:
        if (mC58.mStep == 0) {
            RouteTable_unk* table = fn_80121F00() != 0 ? fn_80121F00()->fn_8013C504() : 0;
            RoutePlace_unk* place = table != 0 ? table->FindPlace_unk(mC58.mPlace) : 0;
            if (place != 0) {
                mPos = place->mPos;
                mRot.y = place->m14;
            }
            mC58.mStep = 1;
            mC58.mDone_unk = 1;
        }
        break;
    case 12:
        if (mC58.mStep == 0) {
            RouteTable_unk* table = fn_80121F00() != 0 ? fn_80121F00()->fn_8013C504() : 0;
            RoutePlace_unk* place = table != 0 ? table->FindPlace_unk(mC58.mPlace) : 0;
            if (place != 0) {
                route->fn_80128818(mC58.mPlace, &mPos, 0);
                route->mRadius = mC58.mRadius;
                route->fn_801255C8();
                mC58.mStep = 1;
            } else {
                mC58.mStep = 2;
            }
        } else if (mC58.mStep == 1) {
            route->fn_80126214();
            atn::Vector3 dir(*route->fn_80139F58());
            if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                *a = dir.Length_unk();
                dir.Normalize_unk();
                *rot = dir.ToRotation_unk();
            }
            if (route->fn_8013C3F4()) {
                mC58.mStep = 2;
            }
        } else {
            mC58.mDone_unk = 1;
        }
        break;
    case 13:
        if (mC58.mStep == 0) {
            if (mC58.m10C != 0 && mC58.m10C->CanAct_unk()) {
            float dist = (*mC58.m10C->fn_8013C38C() - mPos).Length_unk();
            route->fn_80126214();
            atn::Vector3 dir(*route->fn_80139F58());
            if (dir.LengthSq_unk() > 0.01f) {
                *a = MIN(dir.Length_unk(), 1.0f);
                if (1.2f * mC58.mRadius < dist) {
                    float rate = 1.0f + 0.5f * ((dist - 1.2f * mC58.mRadius) / (0.8f * mC58.mRadius));
                    *b *= MIN(rate, 2.0f);
                } else if (mC58.mRadius < dist) {
                    *b *= 0.5f + 0.5f * ((dist - mC58.mRadius) / (0.2f * mC58.mRadius));
                }
                dir.Normalize_unk();
                *rot = dir.ToRotation_unk();
            }
            if (route->fn_8013C3F4()) {
                mC58.mStep = 2;
                mC58.mDone_unk = 1;
            }
            if (dist > 5.0f * mC58.mRadius) {
                route->fn_80128930(mC58.m10C, &mPos, 0);
                route->mRadius = mC58.mRadius;
                route->fn_801255C8();
                mC58.mStep = 1;
            }
            } else {
                mC58.mStep = -1;
                mC58.mDone_unk = 1;
            }
        } else if (mC58.mStep == 1) {
            if (mC58.m10C != 0 && mC58.m10C->CanAct_unk()) {
            route->fn_80126214();
            atn::Vector3 dir(*route->fn_80139F58());
            if (dir.LengthSq_unk() > 0.01f) {
                *a = dir.Length_unk();
                dir.Normalize_unk();
                *rot = dir.ToRotation_unk();
            }
            if (route->fn_8013C3F4()) {
                mC58.mStep = 2;
                mC58.mDone_unk = 1;
            }
            } else {
                mC58.mStep = -1;
                mC58.mDone_unk = 1;
            }
        } else if (mC58.mStep == 2) {
            atn::Vector3 dir = *mC58.m10C->fn_8013C38C() - mPos;
            *a = dir.Length_unk();
            if (*a < mC58.mRadius) {
                if (*a > 0.01f) {
                    *rot = dir.GetNormalized_unk().ToRotation_unk();
                }
                *a = 0.0f;
            } else {
                mC58.mStep = 0;
                mC58.mDone_unk = 0;
                route->fn_80128930(mC58.m10C, &mPos, 0);
                route->mRadius = 0.8f * mC58.mRadius;
                route->fn_801255C8();
            }
        }
        break;
    case 14:
        if (mC58.mStep == 0) {
            atn::Vector3 pos;
            float radius;
            if (fn_80018240(fn_80139EF4(), this, &pos, &radius)) {
                mC58.mStep = 1;
                route->fn_80128A30(&pos, &mPos, 8, 0.0f);
                route->mRadius = 0.75f * radius;
                route->fn_801255C8();
            } else {
                mC58.mStep = -1;
                mC58.mDone_unk = 1;
            }
        } else if (mC58.mStep == 1) {
            route->fn_80126214();
            atn::Vector3 dir(*route->fn_80139F58());
            *b *= fn_800F52F8()->fn_8013C50C();
            if (dir.LengthSq_unk() > 0.01f) {
                *a = dir.Length_unk();
                dir.Normalize_unk();
                *rot = dir.ToRotation_unk();
            }
            if (route->fn_8013C3F4()) {
                mC58.mStep = 2;
                mC58.mDone_unk = 1;
            }
        }
        break;
    case 16:
        if (mC58.mStep == 0) {
            atn::Vector3 pos;
            if (fn_80017F18(fn_80139EF4(), this, &pos)) {
                mC58.mStep = 1;
                route->fn_80128A30(&pos, &mPos, 8, 0.0f);
                route->mRadius = 20.0f;
                route->fn_801255C8();
            } else {
                mC58.mStep = -1;
                mC58.mDone_unk = 1;
            }
        } else if (mC58.mStep == 1) {
            route->fn_80126214();
            atn::Vector3 dir(*route->fn_80139F58());
            if (dir.LengthSq_unk() > 0.01f) {
                *a = dir.Length_unk();
                dir.Normalize_unk();
                *rot = dir.ToRotation_unk();
            }
            if (route->fn_8013C3F4() || (mC58.m04 % 15 == 0 && !fn_80018078(fn_80139EF4(), this))) {
                mC58.mStep = 2;
                mC58.mDone_unk = 1;
            }
        }
        break;
    case 17:
        mC58.mDest = mPos;
        mC58.mCC = mC58.m10C = fn_8013A194()->fn_8010C948(this);
        if (mC58.m110 != 0 && mC58.m110->fn_8013C514()) {
            mC58.mDest = *mC58.m110->fn_8013C53C();
            mC58.mRadius = 0.9f * mC58.m110->fn_8013C544();
        } else if (mC58.m10C != 0) {
            mC58.mDest = *mC58.m10C->fn_8013C38C();
        }
        mC58.mDest.y = mPos.y;
        if (mC58.mStep == 0) {
            if (!fn_80139ED0()) {
                fn_8013C460(1);
                fn_8013C470(mC58.mC4);
            }
            atn::Vector3 dir = mC58.mDest - mPos;
            float dist = dir.LengthSq_unk();
            if (dist > 0.0f) {
                dir.Normalize_unk();
            }
            if (dist < 0.8f * (0.8f * (mC58.mRadius * mC58.mRadius))) {
                *rot = dir.ToRotation_unk();
                mC58.mDone_unk = 1;
            } else {
                mC58.mStep = 1;
                mC58.mDone_unk = 0;
            }
        } else if (mC58.mStep == 1) {
            if (!fn_80139ED0()) {
                fn_8013C460(1);
                fn_8013C470(mC58.mC4);
            }
            atn::Vector3 dir = mC58.mDest - mPos;
            float dist = dir.LengthSq_unk();
            if (dist > 0.0f) {
                dir.Normalize_unk();
            }
            float range = mC58.mRadius * mC58.mRadius;
            if (dist < 0.7f * (0.7f * range)) {
                mC58.mStep = 0;
                mC58.mDone_unk = 1;
                *rot = dir.ToRotation_unk();
            } else if (dist < range) {
                *a = 1.0f;
                *rot = dir.ToRotation_unk();
            } else {
                mC58.mStep = 2;
                mC58.mDone_unk = 0;
                if (mC58.m110 != 0 && mC58.m110->fn_8013C514()) {
                    route->fn_80128A30(&mC58.mDest, &mPos, 8, 0.0f);
                } else if (mC58.m10C != 0) {
                    route->fn_80128930(mC58.m10C, &mPos, 0);
                }
                route->mRadius = 0.8f * mC58.mRadius;
                route->fn_801255C8();
            }
        } else if (mC58.mStep == 2) {
            if (fn_80139ED0()) {
                fn_8013C460(0);
                fn_8013C470(0);
            }
            route->fn_80126214();
            atn::Vector3 dir(*route->fn_80139F58());
            if (dir.LengthSq_unk() > 0.01f) {
                *a = 1.0f;
                dir.Normalize_unk();
            }
            *rot = dir.ToRotation_unk();
            if (route->fn_8013C3F4()) {
                float dist = (mC58.mDest - mPos).Length_unk();
                if (mC58.m110 != 0 && mC58.m110->fn_8013C514()) {
                    if (dist > mC58.mRadius) {
                        route->fn_80128A30(&mC58.mDest, &mPos, 8, 0.0f);
                        route->mRadius = 0.8f * mC58.mRadius;
                        route->fn_801255C8();
                    } else {
                        mC58.mStep = 1;
                        mC58.mDone_unk = 0;
                    }
                } else if (dist > 2.0f * mC58.mRadius) {
                    route->fn_80128930(mC58.m10C, &mPos, 0);
                    route->mRadius = 0.8f * mC58.mRadius;
                    route->fn_801255C8();
                } else {
                    mC58.mStep = 1;
                    mC58.mDone_unk = 0;
                }
            }
        }
        break;
    case 20:
    case 21:
    case 22:
        if (mC58.mStep == 0) {
            if (mC58.mMotions[0] != -1) {
                mC58.mEC--;
                mMotion.fn_80139F24(1);
                mMotion.fn_80139F3C(0, 1.0f);
                mMotion.fn_8013C554(0, 1.0f);
                mMotion.fn_80097C08(0, mC58.mMotions[0], mC58.mLoop_unk && mC58.mEC <= 0, 0, 1, 0.0f, 0.1f);
                mC58.mStep++;
            } else {
                mC58.mStep = -1;
                mC58.mDone_unk = 1;
            }
        } else if (mC58.mStep > 0) {
            float length = mMotion.fn_80097D7C(0);
            if (mMotion.fn_80139550(0) >= length) {
                if (mC58.mMotions[mC58.mStep] != -1) {
                    mC58.mEC--;
                    mMotion.fn_80139F24(1);
                    mMotion.fn_80139F3C(0, 1.0f);
                    mMotion.fn_8013C554(0, 1.0f);
                    mMotion.fn_80097C08(0, mC58.mMotions[mC58.mStep], mC58.mLoop_unk && mC58.mEC <= 0, 0, 1, 0.0f,
                                        0.1f);
                    mC58.mStep++;
                } else {
                    mC58.mStep = -1;
                    mC58.mDone_unk = 1;
                }
            }
        }
        break;
    }
    return 1;
}

atn::Vector3* CharaTask::fn_8013C38C() {
    return &mPos;
}

atn::Vector3 atn::Vector3::fn_8013C394(const Vector3& a, const Vector3& b) {
    Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

int CharaTask::fn_8013C3A8() {
    return mFlag31;
}

atn::Vector3 atn::Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

int Model_unk::fn_8013C3DC() {
    return m34C;
}

float Model_unk::fn_8013C3E4(int track) {
    return mTracks[track].m20;
}

int RouteWalker_unk::fn_8013C3F4() {
    return m2C;
}

CharaSubSeg_unk* RouteWalker_unk::fn_8013C3FC() {
    return &m70;
}

int CharaTask::fn_8013C404(unsigned int flag) {
    return (m54C & flag) == flag;
}

int Settings_unk::fn_8013C41C() {
    return m418;
}

int RouteWalker_unk::fn_8013C424() {
    return m94;
}

float CharaTask::fn_8013C42C() {
    return m5B0;
}

Block454_unk* Settings_unk::fn_8013C434() {
    return &m454;
}

int chr::CharaPart_unk::fn_8013C43C() {
    return m04;
}

bool CharaTask::fn_8013C444() {
    return m640 <= 0;
}

chr::CharaPart_unk* CharaTask::fn_8013C458() {
    return m648;
}

void CharaTask::fn_8013C460(int value) {
    mFlag5 = value;
}

void CharaTask::fn_8013C470(CharaTask* chara) {
    mFC0 = chara;
}

CharaTask* CharaTask::fn_8013C478() {
    return mFC0;
}

RouteTable_unk* Field_unk::fn_8013C504() {
    return m10D8;
}

float Settings_unk::fn_8013C50C() {
    return m48;
}

bool CharaD68_unk::fn_8013C514() {
    bool result = false;
    if (m04 != 0 && m00 != 3) {
        result = true;
    }
    return result;
}

atn::Vector3* CharaD68_unk::fn_8013C53C() {
    return &m10;
}

float CharaD68_unk::fn_8013C544() {
    return m5C * m28;
}

void Model_unk::fn_8013C554(int track, float weight) {
    mTracks[track].m20 = weight;
}

// inferred: 16-byte block copied with paired singles; name invented
struct Vec4_unk {
    Vec4_unk(const Vec4_unk& v) {
        __Vec4Copy(&x, &v.x);
    }
    Vec4_unk& operator=(const Vec4_unk& v) {
        __Vec4Copy(&x, &v.x);
        return *this;
    }
    float x, y, z, w;
};

// inferred: camera state at lbl_8087EFA8 + 0x324; names invented
struct CameraState_unk {
    // inferred: m48 is 1 only in CharaTask::fn_80167F1C
    CameraState_unk(int mode, const CameraState_unk& o, int arg = 0)
        : m00(mode), m04(o.m04), m14(o.m14), m24(o.m24), m34(o.m34), m44(o.m44), m48(arg), m4C(0.0f) {}
    int m00;
    Vec4_unk m04;
    Vec4_unk m14;
    Vec4_unk m24;
    Vec4_unk m34;
    int m44;
    int m48;
    float m4C;
};
// inferred: game state (lbl_8087EFA8, see CharaStatus.cpp); names invented
struct GameState_unk {
    unsigned char m000[0x218];
    float m218; // inferred: distance beyond which characters are not updated (fn_80145334)
    float m21C; // inferred: distance beyond which characters are updated every m220 frames; also
                // the distance of the full motion blend for party members (vf24)
    int m220;
    unsigned char m224[0x324 - 0x224];
    CameraState_unk m324;
    unsigned char m374[0x3A4 - 0x374];
    float m3A4; // inferred: time of a frame
    unsigned char m3A8[0x410 - 0x3A8];
    int m410; // inferred: nonzero calls CharaEquip_unk::fn_805663F8 on the equipment (fn_80145334)
};
extern "C" GameState_unk* lbl_8087EFA8;

// inferred: object hit by a sweep test (the character is at 0x0C); name invented
struct CollObj_unk {
    unsigned char m00[0x08];
    unsigned int m08; // inferred: flags (2: the object is a character, CharaTask::fn_8016AFE8)
    CharaTask* m0C;
};

// inferred: collision query result (0x50 bytes; see also chr/ControlTurnQuadruped.cpp)
struct CollResult_unk {
    CollResult_unk(); // 0x80140500
    // inferred: true when the hit object is of kind 0x1A (name invented)
    bool IsKind1A_unk() const {
        bool result = false;
        if (m34 != 0 && *m34 == 0x1A) {
            result = true;
        }
        return result;
    }
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position (fn_801591E8)
    atn::Vector3 m10; // inferred: position after the sweep (fn_8013CB68)
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 m28; // inferred: hit normal (fn_801591E8)
    unsigned int* m34; // inferred: hit object (its first word is a kind)
    CollObj_unk* m38;  // inferred: hit character (fn_8013CB68)
    unsigned int m3C;  // inferred: flags (1 ground, 2 character, 4 ceiling, 0x40/0x80/0x200 bullets)
    int m40;
    atn::Vector3 m44; // inferred: position copied to CharaTask::m13A0 (fn_8013CB68)
};
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to, unsigned int mask,
                           void* ignore, int arg7); // inferred: ray test
extern "C" float fn_8010CA3C(Effect_unk* mgr, CharaTask* chara, float rate);

// inferred: name invented
inline bool CharaTask::IsLanded_unk(const CollResult_unk& result) {
    bool hit = false;
    if (result.IsKind1A_unk() && !fn_8013C404(0x4000000)) {
        hit = true;
    }
    return hit;
}

// inferred: the player rides with parts (name invented)
inline bool CharaTask::HasRideParts_unk() {
    if (mType == 0 && lbl_8087F0A8->m278 != 0 && m5C->m11C == 1) {
        return true;
    }
    if (mType == 0 && mFlagB17) {
        return true;
    }
    return false;
}

// NONMATCHING: 95.6% - the flag word of the mFlag31 test stays in r3 (the original reloads it for
// fn_80139EB0), and from/to of the ray test get r30/r31 the other way round; tried accessors, local
// orders, an inline ray-test helper.
void CharaTask::vf34(void* target, int arg, float a, float b) {
    atn::Vector3 oldPos(mPos);
    atn::Vector3 oldRot(mRot);
    if (lbl_8087F048 != 0 && !mFlag31) {
        b = fn_8010CA3C(lbl_8087F048, this, b);
    }
    b *= m568;
    if (mStatus.IsState_unk(0x40)) {
        b *= MAX(0.3f, 1.0f - GetF94Rate_unk());
    }
    if (mStatus.IsState_unk(0x8000)) {
        b *= 0.5f;
    }
    b *= mStatus.fn_8012DB04();
    if (m55C == 7) {
        float dist = mRoute.fn_801279E8();
        if (dist >= 0.0f && dist < 20.0f) {
            b = MIN(1.2f, b);
        }
    }
    if (fn_8013C3A8()) {
        m13AC = 0;
        if (mC48 != 0) {
            CollResult_unk result;
            atn::Vector3 to;
            atn::Vector3 from;
            from = mPos;
            to = mPos;
            from.y += 5.0f;
            to.y -= 5.0f;
            void* colli = lbl_8087EE98;
            if (fn_8004ED34(colli, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) != 0) {
                mFlag3 = IsLanded_unk(result);
                if (mFlag3) {
                    mFlag31 = 0;
                    mFlag30 = 0;
                    RouteTable_unk* table = lbl_8087F430->m10D8;
                    if (table != 0 && mC38 > 0 && mC3C > 0) {
                        table->GetLink_unk(mC38)->m3C = 0;
                        table->GetLink_unk(mC3C)->m3C = 0;
                    }
                    if (HasRideParts_unk()) {
                        if (fn_80370174(lbl_8087F430, 0x25) >= 1) {
                            fn_8014EEC4(0);
                        } else if (m674 >= 0) {
                            fn_8014DEE4(0, 0, 0);
                        }
                    }
                }
            }
        } else if (m55C == 6) {
            fn_8013CB68(target, 0, a, b);
        } else {
            fn_80141674(target, a, b);
        }
    } else if (fn_80139EB0() || fn_80139E9C()) {
        m13AC = 0;
        float rate = m568 * MIN(0.52f, b);
        fn_8013CB68(target, 0, a, MAX(0.52f, rate));
    } else if (fn_80139EBC()) {
        float rate = 0.6f;
        fn_8013CB68(target, 0, a, MAX(rate, m568 * rate));
    } else if (fn_80139ED0() || fn_80139EDC()) {
        m13AC = 0;
        if (m55C == 6) {
            fn_8013CB68(target, arg, a, b);
        } else {
            b *= 0.675f;
            if (mFC0 != 0) {
                fn_80140588(target, a, b);
            } else {
                fn_801426A4(target, a, b);
            }
        }
    } else {
        fn_8013CB68(target, arg, a, b);
    }
    if (m55C != 6) {
        fn_80171F3C(&mPos, &m574);
    }
    if (!(atn::Abs_unk(oldPos.x - mPos.x) < 1.1920929e-7f && atn::Abs_unk(oldPos.y - mPos.y) < 1.1920929e-7f &&
          atn::Abs_unk(oldPos.z - mPos.z) < 1.1920929e-7f)) {
        mFlag14 = 1;
    }
}

// inferred: battle character with a state at 0x14B0 (checked when the type data kind m24 is 0x35);
// the class is unknown (larger than CharaTask)
struct BattleChara14B0_unk : public CharaTask {
    unsigned char m1424[0x14B0 - 0x1424];
    int m14B0;
};
extern "C" BattleChara14B0_unk* fn_8018059C(); // inferred: returns lbl_8087F090
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: line check
extern "C" int fn_8004D388(void* world, CollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
extern "C" int fn_8004D314(void* world, int arg, const atn::Vector3* pos, const atn::Vector3* dir, unsigned int mask,
                           int arg5, int arg6, float radius); // inferred: sphere test
extern "C" int fn_805B96A8(int town, const atn::Vector3* pos); // inferred: the position is in the town area
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" double sqrt(double x);

// inferred: the field forces the sweep flag 0x2000000 (names invented)
inline bool IsField25_unk() {
    if (lbl_8087F430 != 0 && lbl_8087F430->m48 == 2 && lbl_8087F430->m4C == 0x19) {
        return lbl_8087F430->m50 == 2;
    }
    return false;
}
inline bool UsesSweepFlag_unk(CharaTask* chara) {
    if (chara->mType == 0) {
        return true;
    }
    return IsField25_unk();
}
// inferred: kinds 1 and 3 of BattleChara14B0_unk::m14B0 (name invented)
inline bool IsNotKind13_unk(int kind) {
    bool result = false;
    if (kind != 1 && kind != 3) {
        result = true;
    }
    return result;
}
// inferred: the character is held by a control (ids 0x63, or 0x27 with motion 0x8E); names invented
inline bool IsHeldMotion_unk(CharaTask* chara) {
    bool result = false;
    if (chara->m560 == 0x27 && chara->mMotion.mTracks[0].m00 == 0x8E) {
        result = true;
    }
    return result;
}
inline bool IsHeldControl_unk(CharaTask* chara) {
    bool result = true;
    if (chara->m560 != 0x63 && !IsHeldMotion_unk(chara)) {
        result = false;
    }
    return result;
}
inline bool IsHeld_unk(CharaTask* chara) {
    bool result = false;
    if (chara->m55C == 6 && IsHeldControl_unk(chara)) {
        result = true;
    }
    return result;
}
// inferred: the two characters are on the same side (names invented)
inline bool IsSameSideOf_unk(CharaTask* a, CharaTask* b) {
    if (a != b) {
        if (lbl_8087F610 != 0 && lbl_8087F610->m540 == 0) {
            return false;
        }
        if (a->mStatus.IsState_unk(2) || b->mStatus.IsState_unk(2) || lbl_8087F0A8->mCC != 0) {
            return false;
        }
    }
    if (a->mF14 >= 0 && b->mF14 >= 0) {
        return a->mF14 == b->mF14;
    }
    bool result = false;
    if (a->mType == b->mType || (a->mType == 0 && b->mType == 3) || (a->mType == 3 && b->mType == 0)) {
        result = true;
    }
    return result;
}
// inferred: characters whose model pushes the player away (names invented)
static bool IsPushingChara_unk(CharaTask* chara) {
    std::string name = fn_8006B174(std::string(chara->mMotion.fn_8008B130()));
    if (name == "np131_a00.model" || name == "np206_a00.model" || name == "np038_f00.model" ||
        name == "np131_a01.model" || name == "np206_a01.model" || name == "np038_f01.model") {
        return true;
    }
    return false;
}

// NONMATCHING: ~86% exact-opcode (98.6% ignoring registers and stack offsets; same 3678 instructions and
// frame size) - the saved-register allocation differs (this is r27 in the original), plus small
// scheduling differences (turn update, sin/cos stores, inlined fn_80176548/fn_80139F60 order, the
// "special model" helper's nested bools); tried: fused/unfused float expressions, temporaries vs
// named vectors, top-of-function declarations in all orders, the hit result reusing `arg`, inline vs
// static helpers, wrappers fn_8016B8D4/fn_8016D0A0 for the control changes.
// inferred: moves the character towards the rotation `target` (speed `a` in 0..1, rate `b`), with
// gravity, collisions and the push between characters
void CharaTask::fn_8013CB68(void* target, int arg, float speed, float rate) {
    int keepRot;
    int flag;
    CharaTask* other;
    int push;
    if (m55C == 6 && m560 == 2) {
        m54C &= ~0x180;
    }
    if ((m54C & 0x80) == 0x80) {
        rate *= 0.3f;
    }
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    if (mF54 != 0) {
        rot.y += 3.1415927f;
    }
    float diff = atn::WrapAngle_unk(((atn::Vector3*)target)->y - rot.y);
    float dt = lbl_8087EFA8->m3A4 * mMotion.m350;
    float cur = rot.y;
    float turn = m56C;
    rate *= dt;
    if (mFlag3 && (m54C & 0x200000) == 0x200000) {
        turn *= 0.1f;
    }
    turn *= 1.0f + atn::Abs_unk(diff / 3.1415927f) * 0.75f;
    cur += diff * turn;
    rot.y = atn::WrapAngle_unk(cur);
    m570 *= 0.85f;
    if (m570 < 0.001f) {
        m570 = 0.0f;
    }
    if (speed > 1.0f) {
        speed = 1.0f;
    }
    if (speed > 0.4f) {
        float ofs = 0.25f;
        speed = (speed - 0.4f) / 0.6f;
        speed *= rate;
        if (m570 < 0.5f) {
            ofs = 0.0f;
        }
        float angle = atn::Abs_unk(diff / 3.1415927f) - ofs;
        if (angle < 0.0f) {
            angle = 0.0f;
        }
        speed *= 1.0f - 1.5f * (angle / (1.0f - ofs));
        if (speed < 0.0f) {
            speed = 0.0f;
        }
    } else {
        speed = 0.0f;
    }
    m580 += 0.25f * (0.7f * diff) - 0.4f * m580;
    if (atn::Abs_unk(m580) < 0.001f) {
        m580 = 0.0f;
    }
    m570 += 0.26f * speed - 0.125f * m570;
    vf24();
    m574.x -= m574.x * MIN(1.0f, 0.7f * dt);
    m574.z -= m574.z * MIN(1.0f, 0.7f * dt);
    atn::Vector3 move;
    move.x = m570 * (float)sin(rot.y);
    move.y = 0.0f;
    move.z = m570 * (float)cos(rot.y);
    move.x *= MIN(1.0f, dt);
    move.y *= MIN(1.0f, dt);
    move.z *= MIN(1.0f, dt);
    if (mStatus.mParamB.fn_801404E0(0x40)) {
        int fps = lbl_8087F0A8->m30;
        m574.y -= -98.1f / (float)(fps * fps);
    } else if (!mStatus.mParamB.fn_801404E0(1) && (m54C & 0x200000) != 0x200000 &&
               !(mFlag2 && m55C == 6 &&
                 ((m560 >= 13 && m560 <= 15) || (m560 >= 29 && m560 <= 30) || m560 == 11 || m560 == 50))) {
        int fps = lbl_8087F0A8->m30;
        m574.y += dt * (-98.1f / (float)(fps * fps));
        if (m574.y < 0.0f) {
            m574.y = MIN(m574.y, -0.4f);
        }
    }
    move.x += m6B8.x * MIN(1.0f, dt);
    move.z += m6B8.z * MIN(1.0f, dt);
    move.y += m6B8.y;
    m6B8 -= atn::Vector3(m6B8.x * MIN(1.0f, 0.19999999f * dt), m6B8.y * MIN(1.0f, 0.19999999f * dt),
                         m6B8.z * MIN(1.0f, 0.19999999f * dt));
    m6B8.y = 0.0f;
    if (!(m6B8.Length_unk() >= 0.1f)) {
        m6B8.Set_unk(0.0f, 0.0f, 0.0f);
    }
    move += m574 * dt;
    if (move.y < -6.0f) {
        move.y = -6.0f;
    }
    int check = 1;
    if (IsPartyMember_unk() || lbl_8087FA20 != 0) {
        if (mFlag12) {
            check = 0;
        }
        float dist = LengthSq_unk(lbl_8087EFB4->m104.mEye - mPos);
        if (dist > 202500.0f) {
            check = 0;
        } else if (dist > 40000.0f && (mId + m1390) % 2 != 0) {
            check = 0;
        }
    }
    if (mFlagB9) {
        check = 0;
    }
    if (lbl_8087FA20 != 0 && check && fn_805B96A8(lbl_8087FA20, &mPos) == 0) {
        check = 0;
    }
    keepRot = 1;
    if (check) {
        if (m610 > 3) {
            atn::Vector3 to(pos);
            atn::Vector3 from(pos);
            atn::Vector3 hit;
            from.y += m5B4;
            if (mStatus.mParamB.fn_801404E0(0x40)) {
                to = from;
                from = pos;
            }
            void* world = lbl_8087EE98;
            if (fn_8004ECC0(world, &hit, &from, &to, fn_80179D44() | 0x80000000, &mColli, 0) != 0) {
                pos.y = hit.y;
            } else if (!mStatus.mParamB.fn_801404E0(0x40)) {
                from.y += m5B4;
                void* world2 = lbl_8087EE98;
                if (fn_8004ECC0(world2, &hit, &from, &to, fn_80179D44() | 0x80000000, &mColli, 0) != 0) {
                    pos.y = hit.y;
                }
            }
        }
        CollResult_unk result;
        atn::Vector3 oldPos(pos);
        Sphere_unk sphere = fn_80176548(pos);
        if (mFlagB0) {
            arg = 1;
        }
        unsigned int mask = (arg != 0 ? 0x80000000 : 0) | fn_80179D44();
        if (UsesSweepFlag_unk(this)) {
            mask |= 0x2000000;
        }
        int hitResult = fn_8004D388(lbl_8087EE98, &result, &sphere, &move, mask, &mColli, 0, sphere.mRadius);
        if ((m54C & 0x200) == 0x200) {
            move.y = 0.0f;
            pos += move;
        } else {
            pos = result.m10;
            pos -= m5A4;
            if (mStatus.mParamB.fn_801404E0(0x40)) {
                pos.y += sphere.mRadius;
            } else {
                pos.y -= sphere.mRadius;
            }
        }
        if (hitResult != 0) {
            if ((result.m3C & 1) == 1) {
                move.y = 0.0f;
            }
            if (mStatus.mParamB.fn_801404E0(0x40) && (result.m3C & 0x100) == 0x100) {
                move.y = 0.0f;
            }
            if ((result.m3C & 4) == 4) {
                move.y = MAX(move.y, -3.0f);
                m54C |= 0x80;
                if ((result.m3C & 8) == 8 && (result.m3C & 1) == 1 && LengthSq_unk(move) > 0.1f &&
                    !mStatus.mParamB.fn_801404E0(8)) {
                    m54C |= 0x100;
                }
            }
            if ((result.m3C & 0x40) == 0x40) {
                m54C |= 0x20000;
                m13A0 = result.m44;
            }
            if ((result.m3C & 0x80) == 0x80) {
                m54C |= 0x40000;
                m13A0 = result.m44;
            }
            if ((result.m3C & 0x200) == 0x200) {
                m54C |= 0x100000;
                m13A0 = result.m44;
            }
        }
        mFlag3 = IsLanded_unk(result);
        if ((m54C & 0x200000) == 0x200000) {
            mFlag3 = 1;
        }
        m634 = result.m34 != 0 ? *result.m34 : -1;
        if (hitResult != 0 && (result.m3C & 2) == 2 && m55C == 7) {
            m141C = MIN(1.0f + m141C, 90.0f);
            if (!mFlagB0 && m141C > 45.0f) {
                mFlagB0 = 1;
            }
        } else {
            if (m141C > 0.0f) {
                m141C = MAX(m141C - 0.7f, 0.0f);
            }
            if (mFlagB0 && m141C <= 15.0f) {
                mFlagB0 = 0;
            }
        }
        if (hitResult != 0 && (result.m3C & 2) == 2) {
            other = result.m38->m0C;
            if (IsPlayer_unk() && m648 != 0 && mC58.mC0 == 0 && lbl_8087F430 != 0 && other->mType == 3 &&
                fn_80370174(lbl_8087F430, 1000) >= 0x30) {
                lbl_8087F430->fn_803750E4(0x132);
            }
            if ((mType == 0 || mType == 2) && m648 == 0) {
                push = 1;
                flag = 0;
                if (other->m55C == 6 && other->m560 == 0x44 &&
                    !((chr::ControlPushedDown*)other->mControl.get())->fn_801C59D4()) {
                    push = 0;
                }
                if (m570 < 0.8f) {
                    push = 0;
                }
                int otherType = other->mType;
                if (otherType == 2) {
                    if (other->m60->m24 == 0x35) {
                        if (!IsNotKind13_unk(((BattleChara14B0_unk*)other)->m14B0)) {
                            push = 0;
                        }
                    } else {
                        push = 0;
                    }
                }
                if ((other->m137C & 1) == 1 && (m55C != 7 || ((unsigned int)mRoute.m48 & 0x80) != 0x80)) {
                    push = 0;
                }
                if (m55C == 6) {
                    push = 0;
                }
                if (IsHeld_unk(other)) {
                    push = 0;
                }
                if (lbl_8087F098 != 0 && lbl_8087F098->m48 != 0) {
                    if (otherType == 0 || otherType == 3) {
                        push = 0;
                    }
                    if (IsSameSideOf_unk(other, this)) {
                        push = 0;
                    }
                    if (mType == 0 && otherType == 3) {
                        if (push) {
                            atn::Vector3 dir;
                            dir = other->mPos - oldPos;
                            dir.Normalize_unk();
                            if (other->m55C != 6 || other->m560 == 0x68) {
                                other->m13F8 += 2;
                                if (other->m13F8 > 6) {
                                    other->fn_80178208(new (0, "", "", 0) chr::ControlPushedAvoid(other, dir, this));
                                }
                            }
                        }
                        push = 0;
                    }
                }
                if (push && IsPlayer_unk()) {
                    if (IsPushingChara_unk(other) && !(m55C == 7 && ((unsigned int)mRoute.m48 & 0x80) == 0x80) &&
                        (lbl_8087F430 == 0 || lbl_8087F430->fn_803761DC() < 2)) {
                        if (lbl_8087F430 != 0 && lbl_8087F430->fn_803761DC() == 1) {
                            atn::Vector3 dir;
                            dir = other->mPos - oldPos;
                            dir.Normalize_unk();
                            if (m55C == 1) {
                                atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
                                Mtx m;
                                PSMTXRotRad(m, 'y', mRot.y);
                                PSMTXMultVec(m, &fwd, &fwd);
                                atn::Vector3 back;
                                back = -dir + fwd;
                                back.Normalize_unk();
                                fn_8016B8D4(back, 1, 0, 0);
                            }
                        } else {
                            atn::Vector3 dir;
                            dir = other->mPos - oldPos;
                            dir.Normalize_unk();
                            if (m55C == 1) {
                                atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
                                Mtx m;
                                PSMTXRotRad(m, 'y', mRot.y);
                                PSMTXMultVec(m, &fwd, &fwd);
                                atn::Vector3 back;
                                back = -dir + fwd;
                                back.Normalize_unk();
                                fn_8016B8D4(back, 1, 0, 0);
                            }
                            return;
                        }
                    }
                }
                if (push) {
                    if ((other->IsPartyMember_unk() && other->mC58.mHeld_unk) ||
                        (other->mType == 3 && (other->m137C & 0x8000) == 0x8000)) {
                        flag = 1;
                    }
                    atn::Vector3 dir;
                            dir = other->mPos - oldPos;
                    dir.Normalize_unk();
                    other->fn_8016B8D4(dir, 0, flag, IsPlayer_unk());
                    if (IsPlayer_unk() && fn_8018059C() != 0 && fn_8018059C()->m14B0 != 0) {
                        if (lbl_8087F0A8->m580 != 0) {
                            fn_8016B8D4(-dir, 1, flag, 0);
                        }
                        return;
                    }
                    if (mType == 2 && fn_8018059C() != 0 && fn_8018059C()->m14B0 != 0) {
                        if (lbl_8087F0A8->m57C != 0) {
                            fn_8016B8D4(-dir, 1, flag, 0);
                        }
                        return;
                    }
                    if (mType == 2 && m60->m24 == 0x35) {
                        atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
                        Mtx m;
                        PSMTXRotRad(m, 'y', mRot.y);
                        PSMTXMultVec(m, &fwd, &fwd);
                        atn::Vector3 back;
                                back = -dir + fwd;
                        back.Normalize_unk();
                        fn_8016B8D4(back, 1, 0, 0);
                    }
                    return;
                }
            }
        }
        int falling = 0;
        if ((hitResult == 0 || (result.m3C & 1) != 1) && !mStatus.mParamB.fn_801404E0(0x20) &&
            (m54C & 0x200000) != 0x200000 && !IsPartyMember_unk() && m55C != 6) {
            atn::Vector3 from(pos);
            atn::Vector3 to(pos);
            from.y += 3.0f;
            to.y -= 7.0f;
            if (fn_8004ED34(lbl_8087EE98, 0, &from, &to, 0x80000000, &mColli, 0) == 0) {
                falling = 1;
            }
            if ((m54C & 2) == 2 && to.y < 0.0f) {
                falling = 0;
            }
        }
        if (falling) {
            if (++m630 > 1) {
                atn::Vector3 center(pos);
                atn::Vector3 down(0.0f, -3.0f, 0.0f);
                if (fn_8004D314(lbl_8087EE98, 0, &center, &down, 0x80000000, 0, 0, 3.0f) == 0) {
                    vf8C(move);
                } else {
                    m630 = 0;
                }
            }
        } else {
            m630 = 0;
            if (hitResult != 0 && ((result.m3C & 0x400) == 0x400 || lbl_8087F0A8->m2C0 != 0) &&
                fn_801749EC(&pos, &move) != 0) {
                keepRot = 0;
            }
        }
        float lean = 0.0f;
        atn::Vector3 vel(move + (pos - oldPos));
        if (LengthSq_unk(vel) > 0.001f && speed > 0.001f) {
            lean = atn::WrapAngle_unk(vel.GetNormalized_unk().ToRotation_unk().y - mRot.y);
        }
        lean *= 2.0f;
        m610 = 0;
        m584 = MAX(MIN(m584 - 0.5f * lean, 1.0471976f), -1.0471976f) * 0.9f;
        if (m648 != 0) {
            if (fn_80172EFC(&pos, &move) == 0) {
                if (fn_801720AC(&pos, &move) != 0) {
                    keepRot = 0;
                } else if (mStatus.IsState_unk(0x2000000) && m55C >= 1 && m55C <= 2 &&
                           (m54C & 0x1000000) != 0x1000000 && !mFlag3 &&
                           (LengthSq_unk(m574) > 1.0f || m570 > 0.5f)) {
                    fn_8016D0A0(0);
                    return;
                }
            }
        } else if (m55C != 6 || (m560 != 0x3C && m560 != 0x3D)) {
            if (fn_80174104(&pos, &move) != 0) {
                return;
            }
            if (fn_801720AC(&pos, &move) != 0) {
                keepRot = 0;
            }
        }
    } else {
        move.y = 0.0f;
        if (m55C == 2 && mC58.m0C.fn_80125474()) {
            float len = move.Length_unk();
            float routeLen = sqrt(mC58.m0C.m58.x * mC58.m0C.m58.x + mC58.m0C.m58.z * mC58.m0C.m58.z);
            if (len > 0.0001f && routeLen > 0.0001f) {
                move.y = mC58.m0C.m58.y / (routeLen / len);
            }
        } else if (m55C == 7 && mRoute.fn_80125474()) {
            float len = move.Length_unk();
            float routeLen = sqrt(mRoute.m58.x * mRoute.m58.x + mRoute.m58.z * mRoute.m58.z);
            if (len > 0.0001f && routeLen > 0.0001f) {
                move.y = mRoute.m58.y / (routeLen / len);
            }
        }
        pos += move;
        move.y = 0.0f;
        m610++;
    }
    if ((m54C & 2) == 2 && pos.y < 0.0f) {
        pos.y = 0.0f;
        move.y = 0.0f;
    }
    mPos = pos;
    if (keepRot) {
        mRot = rot;
        if (mF54 != 0) {
            mRot.y -= 3.1415927f;
        }
    }
    m574 = move;
}

bool CharaParam_unk::fn_801404E0(unsigned int flag) {
    return (m9C & flag) == flag;
}

void* fn_801404F8() {
    return lbl_8087EE98;
}

// inferred: out-of-line copy at 0x80140500 (called by fn_8015076C and other files)
CollResult_unk::CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}

bool CharaTask::fn_80140518() {
    return mType == 3;
}

// inferred: forward direction of the character (rotated by mRot.y)
atn::Vector3 CharaTask::fn_8014052C() {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(mRot.y);
    PSMTXMultVec(m.m, &dir, &dir);
    return dir;
}

extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" double sqrt(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* ab);

// inferred: name invented
inline float Dot_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}

// inferred: name invented
inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}


// NONMATCHING: ~83% (181/1083 lines) - same instructions except: the ptmf temporaries of the inlined
// fn_80178208 chain (known issue), the stack slots of the ToRotation temporaries, the m6B8 damping
// (original reloads m6B8.x and subtracts z first), source/destination registers of the vector copies
// (r3/r4 swapped), IsLanded_unk registers, dt load registers; tried operator forms for the damping,
// named/unnamed copies, accessor/if forms for mFlag3, local orders for dt.
// inferred: movement while sliding/pushed (called by vf34 when mFC0 is set); `target` holds the
// wanted rotation
void CharaTask::fn_80140588(void* target, float a, float b) {
    float dt = lbl_8087EFA8->m3A4 * mMotion.m350;
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    atn::Vector3 vel;
    float diff = atn::WrapAngle_unk(((atn::Vector3*)target)->y - rot.y);
    m570 *= 0.85f;
    if (m570 < 0.001f) {
        m570 = 0.0f;
    }
    if (a > 1.0f) {
        a = 1.0f;
    }
    if (a > 0.4f) {
        a = (a - 0.4f) / 0.6f;
        a *= b;
        a *= 1.5f;
    } else {
        a = 0.0f;
    }
    vel.x = a * (float)sin(((atn::Vector3*)target)->y);
    vel.y = 0.0f;
    vel.z = a * (float)cos(((atn::Vector3*)target)->y);
    vel *= dt;
    m570 += 0.26f * a - 0.125f * m570;
    vf24();
    m574.x -= m574.x * MIN(1.0f, 0.7f * dt);
    m574.z -= m574.z * MIN(1.0f, 0.7f * dt);
    if (!mStatus.mParamB.fn_801404E0(1) && !fn_8013C404(0x200000)) {
        m574.y += dt * (-98.1f / (float)(lbl_8087F0A8->m30 * lbl_8087F0A8->m30));
    }
    vel.x += m6B8.x * dt;
    vel.z += m6B8.z * dt;
    vel.y += m6B8.y;
    m6B8 -= atn::Vector3(m6B8.x * MIN(1.0f, 0.19999999f * dt), m6B8.y * MIN(1.0f, 0.19999999f * dt),
                         m6B8.z * MIN(1.0f, 0.19999999f * dt));
    m6B8.y = 0.0f;
    if (!(m6B8.Length_unk() >= 0.1f)) {
        m6B8.x = 0.0f;
        m6B8.y = 0.0f;
        m6B8.z = 0.0f;
    }
    vel += m574;
    if (vel.y < -6.0f) {
        vel.y = -6.0f;
    }
    bool updateRot = true;
    if (LengthSq_unk(lbl_8087EFB4->m104.mEye - mPos) < 202500.0f) {
        CollResult_unk result;
        atn::Vector3 start(pos);
        Sphere_unk sphere = fn_80176548(pos);
        unsigned int mask = fn_80179D44();
        bool town;
        if (mType == 0) {
            town = true;
        } else {
            Field_unk* field = lbl_8087F430;
            if (field != 0 && field->m48 == 2 && field->m4C == 0x19) {
                town = field->m50 == 2;
            } else {
                town = false;
            }
        }
        if (town) {
            mask |= 0x2000000;
        }
        int hit = fn_8004D388(lbl_8087EE98, &result, &sphere, &vel, mask, &mColli, 0, sphere.mRadius);
        if (fn_8013C404(0x200)) {
            vel.y = 0.0f;
            pos += vel;
        } else {
            pos = result.m10;
            pos -= m5A4;
            pos.y -= sphere.mRadius;
        }
        if (hit) {
            unsigned int flags = result.m3C;
            if ((flags & 1) == 1) {
                vel.y = 0.0f;
            }
            if ((flags & 4) == 4 && (flags & 8) == 8 && (flags & 1) == 1 && fn_805F9920(&vel) > 0.1f &&
                !mStatus.mParamB.fn_801404E0(8)) {
                m54C |= 0x100;
            }
            if ((result.m3C & 0x40) == 0x40) {
                m54C |= 0x20000;
                m13A0 = result.m44;
            }
            if ((result.m3C & 0x80) == 0x80) {
                m54C |= 0x40000;
                m13A0 = result.m44;
            }
            if ((result.m3C & 0x200) == 0x200) {
                m54C |= 0x100000;
                m13A0 = result.m44;
            }
        }
        mFlag3 = IsLanded_unk(result);
        if (fn_8013C404(0x200000)) {
            mFlag3 = 1;
        }
        m634 = result.m34 != 0 ? *result.m34 : -1;
        bool fall = false;
        if ((!hit || (result.m3C & 1) != 1) && !mStatus.mParamB.fn_801404E0(0x20) && !fn_8013C404(0x200000) &&
            !IsPartyMember_unk() && m55C != 6) {
            atn::Vector3 from(pos);
            atn::Vector3 to(pos);
            from.y += 3.0f;
            to.y -= 7.0f;
            if (fn_8004ED34(lbl_8087EE98, 0, &from, &to, 0x80000000, &mColli, 0) == 0) {
                fall = true;
            }
            if (fn_8013C404(2) && to.y < 0.0f) {
                fall = false;
            }
        }
        if (fall) {
            if (++m630 > 1) {
                atn::Vector3 start(pos);
                atn::Vector3 move(0.0f, -3.0f, 0.0f);
                if (fn_8004D314(lbl_8087EE98, 0, &start, &move, 0x80000000, 0, 0, 3.0f) == 0) {
                    vf8C(vel);
                } else {
                    m630 = 0;
                }
            }
        } else {
            m630 = 0;
            if (hit && ((result.m3C & 0x400) == 0x400 || lbl_8087F0A8->m2C0 != 0)) {
                fn_801749EC(&pos, &vel);
            }
        }
        if (fn_801720AC(&pos, &vel)) {
            updateRot = false;
        } else if (mStatus.IsState_unk(0x2000000) && (m55C == 1 || m55C == 2) && !fn_8013C404(0x1000000) && !mFlag3 &&
                   (fn_805F9920(&m574) > 1.0f || m570 > 0.5f)) {
            fn_801750FC();
            fn_80178208(new (0, "", "", 0) chr::ControlSlip(this, 0));
            return;
        }
    } else {
        vel.y = 0.0f;
        if (mC58.m0C.fn_80125474()) {
            float len = vel.Length_unk();
            float speed = sqrt(mC58.m0C.m58.x * mC58.m0C.m58.x + mC58.m0C.m58.z * mC58.m0C.m58.z);
            if (len > 0.0001f && speed > 0.0001f) {
                vel.y = mC58.m0C.m58.y / (speed / len);
            }
        }
        pos += vel;
        m610++;
    }
    if (fn_8013C404(2) && pos.y < 0.0f) {
        pos.y = 0.0f;
        vel.y = 0.0f;
    }
    atn::Vector3 dir(mFC0->mPos.x - pos.x, 0.0f, mFC0->mPos.z - pos.z);
    if (atn::Abs_unk(fn_805F9920(&dir)) < 1.1920929e-7f) {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 0.1f;
    }
    dir.Normalize_unk();
    dir.fn_80139F60();
    float turn = atn::WrapAngle_unk(dir.y - rot.y);
    float speed = m56C;
    if (mFlag3 && fn_8013C404(0x200000)) {
        speed *= 0.1f;
    }
    float k = 0.75f * atn::Abs_unk(turn / 3.1415927f);
    speed *= 1.0f + k;
    float step = turn * speed;
    rot.y += MIN(turn, MAX(0.5f, step));
    m580 += 0.25f * (0.7f * turn) - 0.4f * m580;
    if (atn::Abs_unk(m580) < 0.001f) {
        m580 = 0.0f;
    }
    mPos = pos;
    if (updateRot) {
        mRot = rot;
    }
    m574 = vel;
}

// inferred: wall finder for ledges (field lbl_8087F430); fills `ledge` with the next ledge reached
// along `dir` from the links `a` and `b` (name unknown)
extern "C" int fn_8036DB24(Field_unk* field, Ledge_unk* ledge, CharaTask* chara, int a, int b, const atn::Vector3* dir);

// inferred: direction of the yaw `angle` (name invented)
inline atn::Vector3 DirY_unk(float angle) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', angle);
    PSMTXMultVec(m, &dir, &dir);
    return dir;
}

// inferred: name invented
inline float LengthXZ_unk(const atn::Vector3& v) {
    return sqrt(v.x * v.x + v.z * v.z);
}

// NONMATCHING: ~30% by lines (instructions are close, 1029 vs 1036) - mostly stack slots (the
// ToRotation result is a named local here, a temporary in the original: writing it as a temporary
// makes the compiler drop all inlining in this function, the -ipa inline budget is at its edge), the
// motion selection of mFlag5 (original reloads the flag word and selects 0x1B7/0x1B8 with branches),
// linkA/linkB/end register order; tried DirY_unk inline (kept), flag accessors, ternary/if forms.
// inferred: movement while hanging from a ledge (mC14 direction, mC20/mC2C ends; called by vf34)
void CharaTask::fn_80141674(void* target, float a, float b) {
    float dt = lbl_8087EFA8->m3A4 * mMotion.m350;
    atn::Vector3 move = DirY_unk(((atn::Vector3*)target)->y);
    float dot = Dot_unk(move, mC14);
    atn::Vector3 side = Cross_unk(mC14, atn::Vector3(0.0f, 1.0f, 0.0f));
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    const atn::Vector3* end = 0;
    int linkA = 0;
    int linkB = 0;
    if (dot < -0.5f) {
        move = -mC14;
        end = &mC20;
        mFlag29 = 0;
        linkA = mC38 - 1;
        linkB = mC3C - 1;
    } else if (dot > 0.5f) {
        move = mC14;
        end = &mC2C;
        mFlag29 = 1;
        linkA = mC3C - 1;
        linkB = mC38 - 1;
    } else {
        move.x = 0.0f;
        move.y = 0.0f;
        move.z = 0.0f;
    }
    if (end != 0) {
        atn::Vector3 d(pos - *end);
        if (LengthSq_unk(d) < LengthSq_unk(d + move)) {
            Ledge_unk ledge;
            move = *end - pos;
            if (fn_8036DB24(lbl_8087F430, &ledge, this, linkA, linkB, &move)) {
                ReleaseLedge_unk();
                fn_80153698(&ledge, 1);
            }
        }
    }
    m570 = 0.0f;
    m574.x = 0.0f;
    m574.z = 0.0f;
    if (a > 1.0f) {
        a = 1.0f;
    }
    if (a > 0.4f) {
        a = (a - 0.4f) / 0.6f;
        a *= b;
        a *= 0.5f;
    } else {
        a = 0.0f;
    }
    move *= a * dt;
    float len = move.Length_unk();
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    if (len > 0.0f) {
        if (mFlag30) {
            mMotion.fn_80097C08(0, mFlag29 ? m484[9] : m484[10], 1, 0, 1, 0.0f, 0.1f);
        } else if (mFlag5) {
            mMotion.fn_80097C08(0, mFlag29 ? 0x1B7 : 0x1B8, 1, 0, 1, 0.0f, 0.1f);
        } else {
            mMotion.fn_80097C08(0, mFlag29 ? m484[11] : m484[12], 1, 0, 1, 0.0f, 0.1f);
        }
    } else {
        if (mFlag30) {
            mMotion.fn_80097C08(0, mFlag29 ? m484[5] : m484[6], 1, 0, 1, 0.0f, 0.1f);
        } else if (mFlag5) {
            mMotion.fn_80097C08(0, 0x1B9, 1, 0, 1, 0.0f, 0.1f);
        } else {
            mMotion.fn_80097C08(0, mFlag29 ? m484[7] : m484[8], 1, 0, 1, 0.0f, 0.1f);
        }
    }
    m580 = 0.0f;
    m584 = 0.0f;
    atn::Vector3 r = side.ToRotation_unk();
    atn::Vector3 vel(move);
    rot.y = atn::WrapAngle_unk(r.y);
    if (!mStatus.mParamB.fn_801404E0(1) && !fn_8013C404(0x200000)) {
        m574.y += dt * (-98.1f / (float)(lbl_8087F0A8->m30 * lbl_8087F0A8->m30));
    }
    vel += m574;
    if (vel.y < -6.0f) {
        vel.y = -6.0f;
    }
    CollResult_unk result;
    atn::Vector3 start(pos);
    atn::Vector3 fwd = fn_8014052C();
    Sphere_unk sphere = fn_80176548(pos);
    sphere.mCenter -= fwd * 2.5f;
    sphere.mRadius = 3.0f;
    int hit = fn_8004D388(lbl_8087EE98, &result, &sphere, &vel, fn_80179D44() | 0x80000000, &mColli, 0, sphere.mRadius);
    pos = result.m10;
    pos -= sphere.mCenter - start;
    pos.y = start.y;
    vel.y = 0.0f;
    CollResult_unk wall;
    unsigned int mask = fn_80179D44() | 0x40000000;
    sphere = fn_80176548(pos);
    sphere.mCenter += fwd * 2.0f;
    sphere.mRadius = mColli.m54;
    atn::Vector3 zero(0.0f, 0.0f, 0.0f);
    if (fn_8004D388(lbl_8087EE98, &wall, &sphere, &zero, mask, &mColli, 0, sphere.mRadius)) {
        pos = wall.m10 - fwd * 2.0f;
        pos.y = start.y;
    }
    if (hit) {
        if ((result.m3C & 0x40) == 0x40) {
            m54C |= 0x20000;
            m13A0 = result.m44;
        }
        if ((result.m3C & 0x80) == 0x80) {
            m54C |= 0x40000;
            m13A0 = result.m44;
        }
        if ((result.m3C & 0x200) == 0x200) {
            m54C |= 0x100000;
            m13A0 = result.m44;
        }
    }
    mPos = pos;
    mRot = rot;
    m574.y = vel.y;
    Segment_unk seg(mC20, mC2C);
    atn::Vector3 closest;
    fn_80050A1C(&mPos, &seg, &closest);
    float total = LengthXZ_unk(mC20 - mC2C);
    mPos = closest;
    float t = LengthXZ_unk(mC20 - closest) / total;
    mPos.y = mC20.y + t * (mC2C.y - mC20.y);
    atn::Vector3 from(mPos);
    atn::Vector3 to(mPos);
    from.y += 5.0f;
    to.y -= 5.0f;
    void* colli = lbl_8087EE98;
    if (fn_8004ED34(colli, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0) != 0) {
        mPos.y = result.m04.y;
        mFlag3 = IsLanded_unk(result);
        if (mFlag3) {
            ReleaseLedge_unk();
        }
    }
}

// inferred: line check that returns the hit point (as in game/RouteWalker_unk.cpp)

// NONMATCHING: ~87% (145/1099 lines) - same residual differences as fn_80140588 (ptmf temporaries,
// m6B8 damping order, vector copy registers, IsLanded_unk registers, dt load registers).
// inferred: movement with inertia (called by vf34 when mFC0 is not set); `target` holds the wanted
// rotation
void CharaTask::fn_801426A4(void* target, float a, float b) {
    float dt = lbl_8087EFA8->m3A4 * mMotion.m350;
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    float turn = 0.0f;
    rot.y = ((atn::Vector3*)target)->y;
    m570 *= 0.85f;
    if (m570 < 0.001f) {
        m570 = 0.0f;
    }
    if (a > 1.0f) {
        a = 1.0f;
    }
    if (a > 0.4f) {
        a = (a - 0.4f) / 0.6f;
        a *= b;
        a *= 1.5f;
        if (a < 0.0f) {
            a = 0.0f;
        }
    } else {
        a = 0.0f;
        if (!(atn::Abs_unk(fn_805F9920(&m574)) < 1.1920929e-7f)) {
            rot.y = m574.GetNormalized_unk().ToRotation_unk().y;
        }
    }
    m580 += 0.25f * (0.7f * turn) - 0.4f * m580;
    if (atn::Abs_unk(m580) < 0.001f) {
        m580 = 0.0f;
    }
    m570 += 0.26f * a - 0.125f * m570;
    vf24();
    m574.x -= m574.x * MIN(1.0f, 0.7f * dt);
    m574.z -= m574.z * MIN(1.0f, 0.7f * dt);
    atn::Vector3 vel;
    vel.x = m570 * (float)sin(rot.y);
    vel.y = 0.0f;
    vel.z = m570 * (float)cos(rot.y);
    vel *= dt;
    if (mStatus.mParamB.fn_801404E0(0x40)) {
        m574.y -= -98.1f / (float)(lbl_8087F0A8->m30 * lbl_8087F0A8->m30);
    } else if (!mStatus.mParamB.fn_801404E0(1) && !fn_8013C404(0x200000)) {
        m574.y += -98.1f / (float)(lbl_8087F0A8->m30 * lbl_8087F0A8->m30);
    }
    vel += m6B8;
    m6B8 -= atn::Vector3(m6B8.x * MIN(1.0f, 0.19999999f * dt), m6B8.y * MIN(1.0f, 0.19999999f * dt),
                         m6B8.z * MIN(1.0f, 0.19999999f * dt));
    m6B8.y = 0.0f;
    if (!(m6B8.Length_unk() >= 0.1f)) {
        m6B8.x = 0.0f;
        m6B8.y = 0.0f;
        m6B8.z = 0.0f;
    }
    vel += m574;
    if (vel.y < -6.0f) {
        vel.y = -6.0f;
    }
    if (LengthSq_unk(lbl_8087EFB4->m104.mEye - mPos) < 202500.0f) {
        if (m610 > 4) {
            atn::Vector3 to(pos);
            atn::Vector3 from(pos);
            atn::Vector3 hitPos;
            from.y += m5B4;
            if (mStatus.mParamB.fn_801404E0(0x40)) {
                to = from;
                from = pos;
            }
            void* colli = lbl_8087EE98;
            if (fn_8004ECC0(colli, &hitPos, &from, &to, fn_80179D44() | 0x80000000, &mColli, 0)) {
                pos.y = hitPos.y;
            }
        }
        CollResult_unk result;
        atn::Vector3 start(pos);
        Sphere_unk sphere = fn_80176548(pos);
        unsigned int mask = fn_80179D44();
        bool town;
        if (mType == 0) {
            town = true;
        } else {
            Field_unk* field = lbl_8087F430;
            if (field != 0 && field->m48 == 2 && field->m4C == 0x19) {
                town = field->m50 == 2;
            } else {
                town = false;
            }
        }
        if (town) {
            mask |= 0x2000000;
        }
        int hit = fn_8004D388(lbl_8087EE98, &result, &sphere, &vel, mask, &mColli, 0, sphere.mRadius);
        if (fn_8013C404(0x200)) {
            vel.y = 0.0f;
            pos += vel;
        } else {
            pos = result.m10;
            pos -= m5A4;
            if (mStatus.mParamB.fn_801404E0(0x40)) {
                pos.y += sphere.mRadius;
            } else {
                pos.y -= sphere.mRadius;
            }
        }
        if (hit) {
            unsigned int flags = result.m3C;
            if ((flags & 1) == 1) {
                vel.y = 0.0f;
            }
            if (mStatus.mParamB.fn_801404E0(0x40) && (flags & 0x100) == 0x100) {
                vel.y = 0.0f;
            }
            if ((flags & 4) == 4 && (flags & 8) == 8 && (flags & 1) == 1 && fn_805F9920(&vel) > 0.1f &&
                !mStatus.mParamB.fn_801404E0(8)) {
                m54C |= 0x100;
            }
            if ((result.m3C & 0x40) == 0x40) {
                m54C |= 0x20000;
                m13A0 = result.m44;
            }
            if ((result.m3C & 0x80) == 0x80) {
                m54C |= 0x40000;
                m13A0 = result.m44;
            }
            if ((result.m3C & 0x200) == 0x200) {
                m54C |= 0x100000;
                m13A0 = result.m44;
            }
        }
        mFlag3 = IsLanded_unk(result);
        if (fn_8013C404(0x200000)) {
            mFlag3 = 1;
        }
        m634 = result.m34 != 0 ? *result.m34 : -1;
        bool fall = false;
        if ((!hit || (result.m3C & 1) != 1) && !mStatus.mParamB.fn_801404E0(0x20) && !fn_8013C404(0x200000) &&
            !IsPartyMember_unk() && m55C != 6) {
            atn::Vector3 from(pos);
            atn::Vector3 to(pos);
            from.y += 3.0f;
            to.y -= 7.0f;
            if (fn_8004ED34(lbl_8087EE98, 0, &from, &to, 0x80000000, &mColli, 0) == 0) {
                fall = true;
            }
            if (fn_8013C404(2) && to.y < 0.0f) {
                fall = false;
            }
        }
        if (fall) {
            if (++m630 > 1) {
                atn::Vector3 start(pos);
                atn::Vector3 move(0.0f, -3.0f, 0.0f);
                if (fn_8004D314(lbl_8087EE98, 0, &start, &move, 0x80000000, 0, 0, 3.0f) == 0) {
                    vf8C(vel);
                } else {
                    m630 = 0;
                }
            }
        } else {
            m630 = 0;
            if (hit && ((result.m3C & 0x400) == 0x400 || lbl_8087F0A8->m2C0 != 0)) {
                fn_801749EC(&pos, &vel);
            }
        }
        m610 = 0;
        if (!fn_801720AC(&pos, &vel) && mStatus.IsState_unk(0x2000000) && (m55C == 1 || m55C == 2) &&
            !fn_8013C404(0x1000000) && !mFlag3 && (fn_805F9920(&m574) > 1.0f || m570 > 0.5f)) {
            fn_801750FC();
            fn_80178208(new (0, "", "", 0) chr::ControlSlip(this, 0));
            return;
        }
    } else {
        vel.y = 0.0f;
        if (mC58.m0C.fn_80125474()) {
            float len = vel.Length_unk();
            float speed = sqrt(mC58.m0C.m58.x * mC58.m0C.m58.x + mC58.m0C.m58.z * mC58.m0C.m58.z);
            if (len > 0.0001f && speed > 0.0001f) {
                vel.y = mC58.m0C.m58.y / (speed / len);
            }
        }
        pos += vel;
        m610++;
    }
    if (fn_8013C404(2) && pos.y < 0.0f) {
        pos.y = 0.0f;
        vel.y = 0.0f;
    }
    mPos = pos;
    m574 = vel;
}

extern "C" float fn_800A08D4(atn::LoadHandle* motion); // inferred: length of the motion data returned by fn_80097D40

// inferred: length of a motion of the character, 30 frames when it is missing (name invented)
inline float GetMotionLength_unk(Model_unk* model, int id) {
    atn::LoadHandle* motion = model->fn_80097D40(id);
    float length = motion != 0 ? fn_800A08D4(motion) : 30.0f;
    if (length == 0.0f) {
        length = 1.0f;
    }
    return length;
}

// inferred: name invented
inline float Clamp01_unk(float x) {
    if (x < 0.0f) {
        x = 0.0f;
    } else if (x > 1.0f) {
        x = 1.0f;
    }
    return x;
}

// inferred: controls 0x74-0x77 drive the motions themselves (name invented)
inline bool IsSwinging_unk(CharaTask* chara) {
    if (chara->m55C == 6 && chara->m560 >= 0x74 && chara->m560 <= 0x77) {
        return true;
    }
    return false;
}

// NONMATCHING: ~77% (219/966 lines) - saved float registers are one off (dt in f25 instead of f26),
// the original keeps the blend track index in a register (mulli) where ours folds it to 1, and the
// usual ToRotation/WrapAngle register differences; tried MIN with a local, bool inline for the
// control range, !(a > b) forms for the idle tests, an accessor for mFlag11 (kept).
// inferred: chooses the movement motions (idle, walk, run, dash blended by speed)
void CharaTask::vf24() {
    if (mFlag6) {
        return;
    }
    if (m55C == 6 && mControl && mControl->vf3C() == 0) {
        return;
    }
    if (m55C == 9 || mFlag2) {
        return;
    }
    if (m55C == 2 && mC58.mMode >= 20 && mC58.mMode <= 22 && !mC58.mDone_unk) {
        return;
    }
    float dt = lbl_8087EFA8->m3A4 * mMotion.m350;
    if (mFlag5 || mFlag4) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        float speed = m574.Length_unk();
        if (m570 <= 0.01f || !(speed > 0.01f)) {
            mMotion.fn_80097C08(0, m484[25], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
            return;
        }
        atn::Vector3 fwd = fn_8014052C();
        atn::Vector3 dir = m574.GetNormalized_unk();
        float dot = Dot_unk(fwd, dir);
        if (dot > (float)cos(0.7853982f)) {
            mMotion.fn_80097C08(0, m484[27], 1, 0, 1, 0.0f, 0.1f);
        } else if (dot > (float)cos(2.3561945f)) {
            if (atn::WrapAngle_unk(dir.ToRotation_unk().y - mRot.y) > 0.0f) {
                mMotion.fn_80097C08(0, m484[29], 1, 0, 1, 0.0f, 0.1f);
            } else {
                mMotion.fn_80097C08(0, m484[30], 1, 0, 1, 0.0f, 0.1f);
            }
        } else {
            mMotion.fn_80097C08(0, m484[28], 1, 0, 1, 0.0f, 0.1f);
        }
        float limit = m64C != 0 ? 2.0f : 2.5f;
        mMotion.mTracks[0].m0C = MIN(limit, speed / m50C);
        return;
    }
    if (m1208 != 0) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        float speed = m574.Length_unk();
        if (m570 <= 0.01f || !(speed > 0.01f)) {
            mMotion.fn_80097C08(0, 0x6D, 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
        } else {
            mMotion.fn_80097C08(0, 0x6E, 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = speed / 0.25f;
        }
        m54C &= ~0x80;
        return;
    }
    if (mFlag3) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        float speed = m574.Length_unk();
        if (m570 <= 0.01f || speed <= 0.01f) {
            mMotion.fn_80097C08(0, fn_8013C404(0x200000) ? 0x1E8 : 0x5D, 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
        } else {
            mMotion.fn_80097C08(0, fn_8013C404(0x200000) ? 0x1E9 : 0x5E, 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
        }
        m54C &= ~0x80;
        return;
    }
    if (mF54 != 0) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.fn_80097C08(0, 0x219, 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
        m54C &= ~0x80;
        return;
    }
    if (mF50 != 0) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.fn_80097C08(0, 0x21B, 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
        return;
    }
    if (IsFlag11_unk()) {
        return;
    }
    if (m139C != 0) {
        m139C->mMotion.fn_80097C08(0, 0x1F4, 1, 0, 1, 1.0f, 0.1f);
        m139C->mMotion.m34C = 1;
        m139C->mMotion.mTracks[0].m20 = 1.0f;
        m139C->mMotion.mTracks[0].m0C = 0.0f;
        m139C->mMotion.mTracks[0].m08 = mMotion.mTracks[3].m08;
    }
    if (IsSwinging_unk(this)) {
        return;
    }
    if (mFlag27) {
        return;
    }
    if ((m54C & 0x80) == 0x80) {
        m54C &= ~0x80;
        float rate = 0.0f;
        if (dt > 0.0f) {
            rate = m570 / dt;
        }
        float speed = rate / 0.4f;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        if (!(rate > 0.001f)) {
            mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
        } else {
            mMotion.fn_80097C08(0, 0x5B, 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = speed;
        }
        return;
    }
    float walk = GetMotionLength_unk(&mMotion, m484[1]);
    float run = GetMotionLength_unk(&mMotion, m484[2]);
    float dash = GetMotionLength_unk(&mMotion, m484[3]);
    float walkRun = walk / run;
    float runDash = run / dash;
    float walkMax = m500 * walkRun;
    float runMax = m504 * runDash;
    float rate = 0.0f;
    if (dt > 0.0f) {
        rate = m570 / dt;
    }
    if (!(rate < 1000000.0f)) {
        m570 = 0.0f;
        m574.x = 0.0f;
        m574.y = 0.0f;
        m574.z = 0.0f;
        mRot.x = 0.0f;
        mRot.y = 0.0f;
        mRot.z = 0.0f;
    }
    float blend;
    float walkSpeed;
    float runSpeed;
    float dashSpeed;
    if (rate < m500) {
        walkSpeed = 1.0f;
        blend = rate / m500;
        dashSpeed = walkSpeed / walkRun;
        runSpeed = walkSpeed;
    } else if (rate < walkMax) {
        blend = 1.0f;
        dashSpeed = 0.0f;
        walkSpeed = (walkRun - blend) * ((rate - m500) / (walkMax - m500)) + blend;
        runSpeed = walkSpeed / walkRun;
    } else if (rate < m504) {
        runSpeed = 1.0f;
        walkSpeed = walkRun;
        dashSpeed = runSpeed;
        blend = runSpeed + (rate - walkMax) / (m504 - walkMax);
    } else if (rate < runMax) {
        float one = 1.0f;
        blend = 2.0f;
        walkSpeed = 0.0f;
        runSpeed = (runDash - one) * ((rate - m504) / (runMax - m504)) + one;
        dashSpeed = runSpeed / runDash;
    } else if (rate < m508) {
        walkSpeed = 0.0f;
        runSpeed = runDash;
        dashSpeed = 1.0f;
        blend = 2.0f + (rate - runMax) / (m508 - runMax);
    } else {
        float one = 1.0f;
        blend = 3.0f;
        walkSpeed = 0.0f;
        dashSpeed = one + (rate / m508 - one);
        runSpeed = dashSpeed * runDash;
    }
    float dist = 200.0f;
    if (IsPartyMember_unk() || lbl_8087FA20 != 0) {
        dist = lbl_8087EFA8->m21C;
    }
    if (LengthSq_unk(lbl_8087EFB4->m104.mEye - mPos) > dist * dist) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        if (blend < 0.5f) {
            mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
        } else if (blend < 1.5f) {
            mMotion.fn_80097C08(0, m484[1], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = walkSpeed;
        } else if (blend < 2.5f) {
            mMotion.fn_80097C08(0, m484[2], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = runSpeed;
        } else {
            mMotion.fn_80097C08(0, m484[3], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = dashSpeed;
        }
        return;
    }
    mMotion.m34C = 3;
    float weight = Clamp01_unk(1.0f - blend);
    mMotion.fn_80097C08(0, m139C != 0 ? 3 : m484[0], 1, 0, 1, 0.0f, 0.1f);
    int track = 1;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.mTracks[0].m20 = weight;
    if (blend < 2.0f) {
        weight = Clamp01_unk(1.0f - atn::Abs_unk(1.0f - blend));
        bool changed = mMotion.mTracks[track].m00 != m484[1];
        mMotion.fn_80097C08(track, m484[1], 1, 0, 1, 0.0f, 0.1f);
        if (changed) {
            mMotion.mTracks[track].m08 = walkRun * mMotion.mTracks[2].m08;
        }
        mMotion.mTracks[track].m0C = walkSpeed;
        mMotion.mTracks[track].m20 = weight;
        track = 2;
    }
    weight = Clamp01_unk(1.0f - atn::Abs_unk(2.0f - blend));
    bool changed = false;
    if (track == 1 && mMotion.mTracks[track].m00 != m484[2]) {
        changed = true;
    }
    mMotion.fn_80097C08(track, m484[2], 1, 0, 1, 0.0f, 0.1f);
    if (changed) {
        mMotion.mTracks[track].m08 = mMotion.mTracks[track + 1].m08;
    }
    mMotion.mTracks[track].m0C = runSpeed;
    mMotion.mTracks[track].m20 = weight;
    if (track == 2 && !(atn::Abs_unk(walkRun * mMotion.mTracks[2].m08 - mMotion.mTracks[1].m08) < 1.1920929e-7f)) {
        mMotion.mTracks[2].m08 = mMotion.mTracks[1].m08 / walkRun;
    }
    if (blend >= 2.0f) {
        weight = Clamp01_unk(blend - 2.0f);
        mMotion.fn_80097C08(track + 1, m484[3], 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[track + 1].m0C = dashSpeed;
        mMotion.mTracks[track + 1].m20 = weight;
        if (track + 1 == 2 && !(atn::Abs_unk(runDash * mMotion.mTracks[2].m08 - mMotion.mTracks[1].m08) < 1.1920929e-7f)) {
            mMotion.mTracks[2].m08 = mMotion.mTracks[1].m08 / runDash;
        }
    }
    if (blend <= 0.04f) {
        mMotion.mTracks[1].m08 = 16.0f;
        mMotion.mTracks[2].m08 = 16.0f;
    }
}

void atn::Matrix3x4::fn_80144ED4(float rad) {
    Mtx t;
    Mtx r;
    PSMTXRotRad(r, 'z', rad);
    PSMTXConcat(m, r, t);
    __MtxCopy(m, t);
}

void CharaTask::fn_801446F0() {
    m574.x = 0.0f;
    m574.y = 0.0f;
    m574.z = 0.0f;
    m570 = 0.0f;
    m580 = 0.0f;
    m584 = 0.0f;
}



// inferred: camera parameter passed to fn_80095F10 (same block at DialogHead+0x1620); name invented
struct ModelCameraParam_unk {
    ModelCameraParam_unk() : m00(0) {}
    int m00;
    void* m04;
    Camera_unk mCamera;
};
// inferred: applies a camera to a model (also called by DialogHead)
extern "C" void fn_80095F10(void* model, ModelCameraParam_unk* param);

// inferred: the float is an infinity or a NaN (exponent bits all set); name invented
inline bool __IsInfNan_unk(float f) {
    return (*(unsigned int*)&f & 0x7F800000) == 0x7F800000;
}

// inferred: rotation matrix y * x * z from euler angles (name invented)
inline void __MakeRotationYXZ_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
}

// inferred: copy with the arguments in the order of PSMTXCopy (src, dst) (name invented)
inline void __MtxCopySD_unk(const Mtx src, Mtx dst) {
    __MtxCopy(dst, src);
}

// inferred: m = m * r (name invented)
inline void __MtxMul_unk(atn::Matrix3x4& m, const atn::Matrix3x4& r) {
    Mtx t;
    PSMTXConcat(m.m, r.m, t);
    __MtxCopySD_unk(t, m.m);
}

// inferred: m = m * rotation(r) (name invented)
inline void __MtxRotateYXZ_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    atn::Matrix3x4 rot;
    __MakeRotationYXZ_unk(rot, r);
    __MtxMul_unk(m, rot);
}

// inferred: sets a vector (the arguments are evaluated z first, as in GimmickCommon.cpp)
inline void __Vec3Set_unk(atn::Vector3& v, float x, float y, float z) {
    v.x = x;
    v.y = y;
    v.z = z;
}

// NONMATCHING: ~92% (same instruction count; differences ignoring stack offsets: 39/498) - in the
// inlined ToRotation_unk the locals sit lower on the stack and row 0 / translation of the rotation take
// the other float registers; tried __MtxRotate, by-value matrix helpers, operator*=, a reference to
// the rotation, Matrix3x4 temporaries, PSMTXCopy argument order (kept: __MtxCopySD_unk)
// inferred: builds the model matrix (mMotion.mMtx) from the position, rotation and scale, or
// takes it from the matrix the character is attached to (mF1C)
void CharaTask::fn_80144710() {
    atn::Matrix3x4 mtx;
    if (mF1C != 0) {
        mtx = *(atn::Matrix3x4*)mF1C;
        mPos = mtx.GetTrans_unk();
        mtx.m[0][3] = 0.0f;
        mtx.m[1][3] = 0.0f;
        mtx.m[2][3] = 0.0f;
        atn::Vector3 dir;
        dir = atn::Vector3(0.0f, 0.0f, 1.0f);
        PSMTXMultVec(mtx.m, &dir, &dir);
        dir.y = 0.0f;
        mRot = dir.ToRotation_unk();
        mtx = *(atn::Matrix3x4*)mF1C;
    } else {
        if (__IsInfNan_unk(mPos.y) || __IsInfNan_unk(mPos.x) || __IsInfNan_unk(mPos.z)) {
            mPos = m550;
        } else {
            m550 = mPos;
        }
        Mtx trans;
        PSMTXTrans(trans, mPos.x, mPos.y, mPos.z);
        __MtxCopy(mtx.m, trans);
        if (__IsInfNan_unk(mRot.y) || __IsInfNan_unk(mRot.x) || __IsInfNan_unk(mRot.z)) {
            mRot.x = 0.0f;
            mRot.y = 0.0f;
            mRot.z = 0.0f;
        }
        __MtxRotateYXZ_unk(mtx, mRot);
        if (mStatus.mParamB.fn_801404E0(0x40) && m55C != 8) {
            Mtx r;
            PSMTXRotRad(r, 'z', 3.1415927f);
            Mtx t;
            PSMTXConcat(mtx.m, r, t);
            __MtxCopySD_unk(t, mtx.m);
        }
        Mtx scale;
        PSMTXScale(scale, m540.x, m540.y, m540.z);
        Mtx t;
        PSMTXConcat(mtx.m, scale, t);
        __MtxCopySD_unk(t, mtx.m);
    }
    __MtxCopy(mMotion.mMtx, mtx.m);
    atn::Vector3 ax;
    atn::Vector3 ay;
    atn::Vector3 az;
    __Vec3Set_unk(az, mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]);
    float sz = PSVECMag(&az);
    __Vec3Set_unk(ay, mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]);
    float sy = PSVECMag(&ay);
    __Vec3Set_unk(ax, mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]);
    float sx = PSVECMag(&ax);
    atn::Vector3 s(sx, sy, sz);
    mMotion.m54 = MAX(s.x, MAX(s.y, s.z));
}

// NONMATCHING: ~25% (155 vs 250 instructions; the code before and after the copy matches) - the copy
// of the camera (implicit operator=) cannot match: the original copies
// members with paired singles (Vector3, Matrix3x4, a 4x4 matrix at 0x88 and the frustum at 0x100);
// the 4x4 matrix copy needs a paired-single helper that Vec.h does not have, so the layout is kept opaque.
// Layout seen in the copy: 0x00 two ints, 0x08/0x14/0x20/0x2C Vector3, 0x38-0x54 floats, 0x58 Matrix3x4,
// 0x88 4x4 matrix, 0xC8/0xCC floats, 0xD0 Matrix3x4, 0x100 frustum (Matrix3x4, int, 2 floats,
// 4 x {Vector3, float}, 2 Vector3, 6 x {Vector3, float})
// inferred: sets an orthographic camera spanning y0..y1 on the model and the equipment
void CharaTask::fn_80144F4C(int a, void* b, float y0, float y1) {
    Camera_unk camera(0, 1);
    float center = 0.5f * (y1 + y0);
    camera.mUp.Set_unk(0.0f, 1.0f, 0.0f);
    camera.mPos.Set_unk(0.0f, center, -100.0f);
    camera.mTarget.Set_unk(0.0f, center, 0.0f);
    camera.m3C = y0 - y1;
    camera.m40 = 0.0f;
    camera.m44 = -10.0f;
    camera.m48 = 10.0f;
    camera.fn_8004B378();
    ModelCameraParam_unk param;
    param.mCamera = camera;
    param.m00 = a;
    param.m04 = b;
    fn_80095F10(&mMotion, &param);
    chr::CharaEquip_unk** p = m680;
    for (unsigned int i = 0; i < 8; i++, p++) {
        if (*p != 0) {
            fn_80095F10((*p)->m024, &param);
        }
    }
}

extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
// inferred: sphere cast against the collision (hit in `result`, nonzero on a hit)
extern "C" int fn_80418318(); // inferred: id of the water (ground follow) used by fn_804188A0
extern "C" atn::Vector3 fn_804188A0(int id, const Segment_unk* seg);
extern "C" void fn_8009076C(Model_unk* model, const atn::Matrix3x4* mtx); // inferred: updates the nodes
extern "C" void fn_8009757C(Model_unk* model);
extern "C" void fn_80097CCC(Model_unk* model, int track, float weight);
extern "C" void fn_80098698(Model_unk* model);
extern "C" void fn_80093F1C(Model_unk* model, const char* node, const atn::Vector3* a, const atn::Vector3* b,
                            const atn::Vector3* scale, int arg); // inferred: node edit
extern "C" void fn_80094778(Model_unk* model, const char* node); // inferred: clears a node edit
extern "C" int fn_802185FC(const char* name);
extern "C" int fn_80376150(Field_unk* field);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);

// inferred: WrapAngle_unk with the period as a float constant (the pool of this file holds
// (double)6.2831855f, not 6.283185307179586); name invented
inline float __WrapAngleF_unk(float a) {
    a = fmod(a, 6.2831855f);
    if (a > 3.1415927f) {
        a -= 6.2831855f;
    }
    if (a < -3.1415927f) {
        a += 6.2831855f;
    }
    return a;
}

// inferred: helpers of the distance check in fn_80145334 (names invented)
inline bool __IsPartless_unk(CharaTask* chara) {
    return chara->mType != 0 && chara->m648 == 0;
}
inline bool __IsEventView_unk() {
    if (lbl_8087FA20 != 0) {
        return true;
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->m48 == 2 && lbl_8087F430->m4C == 42 && lbl_8087F430->m50 == 2) {
        return true;
    }
    return false;
}

// inferred: squared length of a vector (name invented)
inline float __LengthSq_unk(const atn::Vector3& v) {
    return fn_805F9920(&v);
}

// inferred: member-wise float copy (name invented)
inline void __SetFloat4_unk(Model_unk::Float4_unk& d, const Model_unk::Float4_unk& v) {
    d.m00 = v.m00;
    d.m04 = v.m04;
    d.m08 = v.m08;
    d.m0C = v.m0C;
}

// inferred: sets four floats through a local block (name invented)
inline void __SetFloat4_unk(Model_unk::Float4_unk& d, float a, float b, float c, float e) {
    Model_unk::Float4_unk v;
    v.m00 = a;
    v.m04 = b;
    v.m08 = c;
    v.m0C = e;
    __SetFloat4_unk(d, v);
}

// inferred: rotation matrix z * y * x (same as __MakeRotation_unk in GimmickCommon.cpp)
inline void __MakeRotationZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
}

// NONMATCHING: ~87% (ignoring stack offsets: 336/2500 lines differ; 2471 vs 2473 instructions) -
// stack layout, float/GPR register assignment (scale/extra/maxDist, node/reset), the inlined
// ToRotation_unk (see fn_80144710), `mType != 3` folded away, the sphere radius (target converts the
// constant with frsp), mF50 read once instead of twice, f1 = distSq passed to fn_80148B38 (declared Fv);
// .data/.rodata pool references (ptmf constants, strings, the 2*pi double) shift until the earlier
// functions exist. Tried: decl orders of the floats, Float4 assignment forms (kept the local block),
// helper inlines for the distance checks, WrapAngle with a float period (kept), several `mType` forms.
// inferred: per-frame update of the character (position fixes, model matrix, visibility by distance,
// motion update, effects, timers)
void CharaTask::fn_80145334() {
    float maxDist = 10.0f;
    float extra = 0.0f;
    float scale = 0.0f;
    bool check = false;
    bool forward = false;
    if (mType == 0) {
        if (m55C == 6) {
            switch (m560) {
            case 4:
            case 5:
            case 2:
                check = true;
                break;
            case 14:
                if (m50 == 700022 || m50 == 700023) {
                    extra = 2.0f;
                    check = true;
                    maxDist = 20.0f;
                }
                break;
            case 98:
                if (mFlag6) {
                    scale = 7.0f;
                    check = true;
                    forward = true;
                }
                break;
            }
        } else if (m55C == 1) {
            if (mFlag6) {
                if (m674 >= 0) {
                    scale = 7.0f;
                    check = true;
                    forward = true;
                }
            } else if (!mFlag31) {
                check = m570 > 0.001f;
                maxDist = 3.0f;
            }
        }
    }
    if (check && m520 >= 0) {
        bool keep = true;
        atn::Vector3 dir;
        if (forward) {
            dir = fn_8014052C() * scale;
        } else {
            atn::Vector3 from = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(m524))->GetTrans_unk();
            atn::Vector3 to = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(m520))->GetTrans_unk();
            dir = to - from;
        }
        dir.y = 0.0f;
        if (fn_805F9920(&dir) > 0.01f) {
            atn::Vector3 n = dir.GetNormalized_unk();
            atn::Vector3 front = fn_8014052C();
            if (PSVECDotProduct(&n, &front) > 0.5f) {
                CollResult_unk result;
                unsigned int mask = fn_80179D44() | 0x80000000;
                Sphere_unk sphere;
                sphere.mCenter = m614;
                sphere.mRadius = m620;
                sphere.mCenter = mPos + m5A4;
                if (mStatus.mParamB.fn_801404E0(0x40)) {
                    sphere.mCenter.y -= sphere.mRadius;
                } else {
                    sphere.mCenter.y += sphere.mRadius;
                }
                sphere.mRadius = (mFlag6 ? 4.0f : 3.0f) + extra;
                if (fn_8004D388(lbl_8087EE98, &result, &sphere, &dir, mask, &mColli, 0, sphere.mRadius)) {
                    atn::Vector3 end = sphere.mCenter + dir;
                    atn::Vector3 back(result.m10.x - end.x, 0.0f, result.m10.z - end.z);
                    if (fn_805F9920(&back) > 0.01f) {
                        atn::Vector3 n1 = dir.GetNormalized_unk();
                        atn::Vector3 n2 = back.GetNormalized_unk();
                        if (PSVECDotProduct(&n1, &n2) < -0.5f && ((unsigned int)result.m3C & 4) != 4) {
                            m588 = MIN(maxDist, back.Length_unk());
                            keep = false;
                        }
                    }
                }
            }
        }
        if (keep) {
            if (m588 > 0.01f) {
                m588 -= MIN(m588, mFlag6 ? 0.4f : 0.2f);
            }
            if (mFlag6 && m55C == 1 && mMotion.mTracks[0].m00 == 0x25 && mMotion.mTracks[0].m00 != m484[4]) {
                mMotion.fn_80097C08(0, m484[4], 1, 0, 1, 0.0f, 0.1f);
            }
        } else if (mFlag6 && m55C == 1 && mMotion.mTracks[0].m00 == 0x26) {
            mMotion.fn_80097C08(0, 0x25, 1, 0, 1, 0.0f, 0.1f);
        }
    } else {
        if (m55C == 8) {
            m588 = 0.0f;
        }
        if (m588 > 0.01f) {
            m588 -= MIN(m588, 0.5f);
        }
    }
    mRot.x = __WrapAngleF_unk(mRot.x);
    mRot.y = __WrapAngleF_unk(mRot.y);
    mRot.z = __WrapAngleF_unk(mRot.z);
    if (--m13F8 < 0) {
        m13F8 = 0;
    }
    if (lbl_8087EFA8->m410 != 0) {
        if (mMotion.IsFlag5_unk()) {
            mMotion.mFlag4_unk = 1;
            chr::CharaEquip_unk** equips = m680;
            for (unsigned int i = 0; i < 8; i++, equips++) {
                chr::CharaEquip_unk* equip = *equips;
                if (equip != 0) {
                    equip->fn_805663F8();
                }
            }
        }
    } else if (mMotion.IsFlag5_unk()) {
        mMotion.mFlag4_unk = 0;
        chr::CharaEquip_unk** equips = m680;
        for (unsigned int i = 0; i < 8; i++, equips++) {
            chr::CharaEquip_unk* equip = *equips;
            if (equip != 0) {
                equip->fn_80566408();
            }
        }
    }
    for (CharaPair8_unk* it = m6D0.begin(); it != m6D0.end();) {
        if (--it->m04 <= 0) {
            m6D0.erase(it);
        } else {
            it++;
        }
    }
    if (mFlag5) {
        mF5C = 0;
        mF58++;
    } else {
        mF58 = 0;
        mF5C++;
    }
    if ((mStatus.m0C & 0x40) == 0x40 || mFlag23) {
        float height = m1420;
        if (mF94 > 0) {
            height *= MIN(0.75f, MAX(GetF94Rate_unk(), 0.01f));
        }
        fn_80144F4C(0, 0, mPos.y + height, mPos.y);
    } else if (mFlag8) {
        float height = 50.0f;
        height *= MAX(1.0f - (float)mF9C / (float)mFA0, 0.01f);
        fn_80144F4C(3, 0, mPos.y, mPos.y + height);
    } else if (mFlagB1) {
        fn_80144F4C(1, (char*)lbl_8087EEB0 + 0x70, 20.0f + mPos.y, 20.0f + mPos.y + 60.0f);
    } else {
        mMotion.mFlags &= ~0x80000;
        for (int i = 0; i < 8; i++) {
            if (m680[i] != 0) {
                m680[i]->m028 &= ~0x80000;
            }
        }
    }
    if (mFlag7) {
        mFlag7 = 0;
        mMotion.mTracks[0].m10 = 1.0f;
    }
    if ((mStatus.m0C & 0x100) == 0x100) {
        __SetFloat4_unk(mMotion.m40, 0.1f, 0.1f, 0.1f, 1.0f);
    } else {
        __SetFloat4_unk(mMotion.m40, 1.0f, 1.0f, 1.0f, 1.0f);
    }
    m1154.fn_8007A7BC(&mMotion);
    if (mF50 != 0) {
        CharaTask* other = (CharaTask*)mF50; // inferred: mF50 holds the character dragging this one
        mPos = other->mPos;
        mRot = other->mRot;
        atn::Vector3 offset(0.0f, 0.0f, 12.0f);
        atn::Matrix3x4 rot;
        __MakeRotationZYX_unk(rot, mRot);
        PSMTXMultVec(rot.m, &offset, &offset);
        mPos += offset;
        mRot.y += 3.1415927f;
    }
    if ((m54C & 0x200000) == 0x200000 && fn_80418318() != 0) {
        MtxPtr node = mMotion.GetNodeMtx_unk(m520);
        if (node != 0) {
            atn::Matrix3x4 inv;
            __MtxCopy(inv.m, mMotion.mMtx);
            PSMTXInverse(inv.m, inv.m);
            fn_80144710();
            fn_8009076C(&mMotion, &inv);
            int id = fn_80418318();
            float dy = node[1][3] - mPos.y;
            atn::Vector3 pos = fn_804188A0(id, &Segment_unk(mPos, ((atn::Matrix3x4*)node)->GetTrans_unk()));
            pos.y -= dy;
            pos.y = (pos.y + (m13B8[0].y + m13B8[1].y)) / 3.0f;
            mPos = pos;
        }
    }
    if (mFlag2 && lbl_8087F610 != 0) {
        if (!mFlag1) {
            mPos += m13D8;
            m13D8 *= lbl_8087F610->m5C8;
            mRot.y += MIN(MAX(m13E4, -0.2617994f), 0.2617994f);
            mRot.y = __WrapAngleF_unk(mRot.y);
            m13E4 *= lbl_8087F610->m5C8;
            m13E4 = __WrapAngleF_unk(m13E4);
        } else {
            mFlag1 = 0;
        }
    }
    m13B8[1] = m13B8[0];
    m13B8[0] = mPos;
    m13D0[1] = m13D0[0];
    m13D0[0] = mRot.y;
    atn::Matrix3x4 prev;
    __MtxCopy(prev.m, mMotion.mMtx);
    atn::Matrix3x4 inv(prev);
    PSMTXInverse(inv.m, inv.m);
    fn_80144710();
    int oldNode = mMotion.m50;
    if (m55C == 8) {
        mMotion.m20C_15_8 = mMotion.m50;
        mMotion.m50 = 0;
    } else {
        mMotion.fn_80092A4C();
    }
    bool moved = oldNode != mMotion.m50;
    if (!moved && __LengthSq_unk(prev.GetTrans_unk() - mPos) > 400.0f) {
        moved = true;
    }
    if ((mStatus.m0C & 0x40) == 0x40 && mF94 == 0) {
        atn::Vector3 offset = mF88;
        atn::Matrix3x4* mtx = (atn::Matrix3x4*)mMotion.mMtx;
        mtx->fn_801479E4(mtx->GetTrans_unk() + offset);
    }
    Model_unk::Node_unk* node = &mMotion.m220[m524];
    bool reset = false;
    atn::Vector3 nodePos;
    atn::Vector3 nodeRot;
    if (m1254 != 0) {
        unsigned int motion = mMotion.mTracks[0].m04;
        if (motion != (unsigned int)m518) {
            reset = true;
        }
        m518 = motion;
        float frame = mMotion.mTracks[0].m08;
        if (frame < m51C) {
            reset = true;
        }
        m51C = frame;
        nodePos = node->m04;
        nodeRot.Set_unk(0.0f, 0.0f, 1.0f);
        if (!(atn::Abs_unk(node->m10.x) < 1.1920929e-7f && atn::Abs_unk(node->m10.y) < 1.1920929e-7f &&
              atn::Abs_unk(node->m10.z) < 1.1920929e-7f && atn::Abs_unk(node->m10.w) < 1.1920929e-7f)) {
            Mtx m;
            PSMTXQuat(m, &node->m10);
            PSMTXMultVec(m, &nodeRot, &nodeRot);
        }
        nodeRot.fn_80139F60();
    }
    bool update = true;
    float distSq = __LengthSq_unk(lbl_8087EFB4->m104.mEye - mPos);
    bool far = !mFlagB3 && (IsPartyMember_unk() || __IsPartless_unk(this) || __IsEventView_unk());
    if (lbl_8087F430 != 0 && lbl_8087F430->m56D4 == this) {
        far = false;
    }
    if (far && (mC54 == 0 || distSq > 1600.0f) && !moved && m55C != 8) {
        mMotion.mFlags |= 0x40;
    } else {
        mMotion.mFlags &= ~0x40;
    }
    m1390++;
    if (far && !moved) {
        float dist = lbl_8087EFA8->m218;
        if (IsPartyMember_unk() && mC54 == 0 && (m1400.mSize == 0 || m140C == -1)) {
            dist *= 0.75f;
        }
        if (m55C == 8) {
            dist *= 15.0f;
        }
        if (distSq > dist * dist) {
            update = false;
        } else {
            dist = lbl_8087EFA8->m21C;
            if (IsPartyMember_unk() && mC54 == 0) {
                dist *= 0.75f;
            }
            if (m55C == 8) {
                dist *= 15.0f;
            }
            if (distSq > dist * dist && (m1390 + mId) % lbl_8087EFA8->m220 != 0) {
                update = false;
            }
        }
    }
    bool animate = true;
    if ((m55C != 8 || !mFlag23) && (mStatus.m0C & 0x40) == 0x40 && mF94 == 0) {
        animate = false;
        if (!moved) {
            update = false;
        }
    }
    if (mFlagB27 && mMotion.mTracks[0].m04 != 0) {
        mFlagB26 = fn_802185FC(((atn::LoadHandle*)((atn::LoadHandle*)mMotion.mTracks[0].m04))->fn_80473F18()) == 0;
    }
    bool command = update;
    if (IsPartyMember_unk()) {
        if (distSq > 10000.0f) {
            command = false;
        }
    } else if (mType != 3 && mType == 2 && mFlag22 != 1 && distSq > 90000.0f) {
        command = false;
    }
    if (command) {
        m1220.fn_8012111C();
    }
    if (update) {
        fn_80175B18(distSq);
        mLook.fn_80129A6C();
        chr::CharaEquip_unk* coat = m680[0];
        if (coat != 0 && coat->m004 % 1000 / 10 == 46) {
            if (m648 != 0 || m55C == 8) {
                fn_80093F1C(&mMotion, "leftcoat1", &chr::lbl_807C7030, &chr::lbl_807C7030, &atn::Vector3(1.0f, 1.0f, 1.0f), 8);
                fn_80093F1C(&mMotion, "leftcoat2", &chr::lbl_807C7030, &chr::lbl_807C7030, &atn::Vector3(1.0f, 1.0f, 1.0f), 8);
                fn_80093F1C(&mMotion, "rightcoat1", &chr::lbl_807C7030, &chr::lbl_807C7030, &atn::Vector3(1.0f, 1.0f, 1.0f), 8);
                fn_80093F1C(&mMotion, "rightcoat2", &chr::lbl_807C7030, &chr::lbl_807C7030, &atn::Vector3(1.0f, 1.0f, 1.0f), 8);
            } else {
                fn_80094778(&mMotion, "leftcoat1");
                fn_80094778(&mMotion, "leftcoat2");
                fn_80094778(&mMotion, "rightcoat1");
                fn_80094778(&mMotion, "rightcoat2");
            }
        }
        int updated = mMotion.fn_80097E80(animate);
        if (m55C == 6 && mControl) {
            mControl->vf40();
        }
        if (mFlagB12) {
            vf28();
        }
        if (mFlag2) {
            mMotion.m220[m524].m04 = m13E8;
        }
        if (!reset && m1254 != 0) {
            if ((m1254 & 1) == 1 && (node->m00 & 1) == 1) {
                atn::Vector3 move = node->m04 - nodePos;
                atn::Matrix3x4 rot = ((atn::Matrix3x4*)mMotion.mMtx)->GetRotation_unk();
                PSMTXMultVec(rot.m, &move, &move);
                mPos += move;
                node->m00 &= ~1;
            }
            if ((m1254 & 2) == 2 && (node->m00 & 2) == 2) {
                mRot -= nodeRot;
                atn::Vector3 v(0.0f, 0.0f, 1.0f);
                Mtx m;
                PSMTXQuat(m, &node->m10);
                PSMTXMultVec(m, &v, &v);
                v.fn_80139F60();
                mRot += v;
                node->m00 &= ~2;
            }
            if ((m1254 & 4) == 4 && (node->m00 & 4) == 4) {
                m540 = node->m20;
                node->m00 &= ~4;
            }
            fn_80144710();
        }
        if (updated) {
            if (m588 > 0.01f) {
                mMotion.m220[m524].m04.y -= m588;
            }
            fn_8000D430(&mMotion, boost::function<void()>());
            if (m588 > 0.01f) {
                mMotion.m220[m524].m04.y += m588;
            }
        } else if (!IsPartyMember_unk() || mC54 != 0 || m55C != 2) {
            fn_8009076C(&mMotion, &inv);
        }
        fn_8009757C(&mMotion);
        if (m12BC >= 0) {
            float frame = mMotion.mTracks[3].m08;
            if (frame >= mMotion.fn_80097D7C(3)) {
                m12BC = -1;
                fn_80097CCC(&mMotion, 3, 1.0f);
            }
        }
    } else {
        mLook.fn_80129A48(0.3f);
        mLook.fn_80129A6C();
        if (animate) {
            fn_80098698(&mMotion);
        }
        if (!IsPartyMember_unk() || mC54 != 0 || m55C != 2) {
            fn_8009076C(&mMotion, &inv);
        }
    }
    MapInfo_unk* map = 0;
    if (lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
        map = fn_80373148(lbl_8087F430);
    }
    if (map == 0 && MapManager::sInstance != 0 && MapManager::sInstance->mParts.mSize != 0) {
        map = (MapInfo_unk*)MapManager::sInstance->mParts.mData[0];
    }
    if (mFlagB25 && map != 0) {
        fn_80147A18(map->m104);
    }
    fn_80148B38();
    vf20();
    vf54();
    if (IsPartyMember_unk() || lbl_8087FA20 != 0) {
        if (!(m55C == 6 && m560 == 0x45) && m60 != 0 && m60->m24 != 0x35) {
            if (distSq < 62500.0f && !mFlag12) {
                mColli.mFlags |= 1;
            } else {
                mColli.mFlags &= ~1;
            }
        }
        if (!mC58.mHeld_unk) {
            if (distSq < 40000.0f) {
                m54C |= 0x20;
            } else {
                m54C &= ~0x20;
            }
        }
    }
    if (mType == 0) {
        bool chant = false;
        if (m55C == 6) {
            switch (m560) {
            case 0x1D:
                if (mControl && !mControl->vf58()) {
                    __SetFloat4_unk(lbl_8087F048->m3E304, 1.0f, 1.0f, 1.0f, 1.0f);
                    lbl_8087F048->fn_8010743C(&mF6C, m638 != m63C, mF78);
                    chant = true;
                }
                break;
            case 0xD:
                if (lbl_8087F610 == 0 && lbl_8087F0A8->m314 != 0 && m50 == 1) {
                    break;
                }
            case 0x21:
                __SetFloat4_unk(lbl_8087F048->m3E304, 1.0f, 1.0f, 1.0f, 1.0f);
            case 0xF:
                lbl_8087F048->fn_8010743C(&mF6C, m638 != m63C, mF78);
                chant = true;
                break;
            }
        }
        if (!chant && lbl_8087F048 != 0) {
            lbl_8087F048->fn_80107574();
        }
        mFlagB24 = chant;
        if (lbl_8087F430 != 0 && fn_80219558(m50) == 0) {
            if (fn_80376150(lbl_8087F430)) {
                if (!mFlag17) {
                    vf98();
                    mFlag17 = 1;
                }
            } else if (mFlag17) {
                vf9C(0);
                mFlag17 = 0;
            }
        }
    }
    if (m50 == 700023) {
        if (mFlag5) {
            vf98();
        } else {
            vf9C(0);
        }
    }
    fn_80154C08();
    if (!IsPartyMember_unk()) {
        fn_80164D24();
        fn_80176E30();
        if (m12C4 > 0) {
            m12C4--;
            if (mC58.mC4 != (CharaTask*)mC58.mC8) {
                m12C4 = 0;
            }
        }
        if (m12C8 > 0) {
            m12C8--;
        }
        if (m12CC > 0.0f) {
            m12CC -= 0.011111111f;
            if (m12CC <= 0.0f) {
                m12CC = 0.0f;
            }
        }
    }
    if (m1380 > 0) {
        m1380--;
    }
    if (m1388 > 0) {
        m1388--;
    }
    if (m138A > 0) {
        m138A--;
    }
    if (m678 != 0) {
        if (mFlagB21 && !m678->vf0C() && (m67C == 0 || !m67C->vf0C())) {
            if (m674 == 0) {
                fn_8014DEE4(0, 0, 0);
            }
            mFlagB21 = 0;
        }
        bool broken = false;
        if (m678->m28C != 0 && m678->m288 <= 0) {
            broken = true;
        }
        if (broken) {
            if (m678 != 0) {
                delete m678;
                m678 = 0;
                if (m67C != 0) {
                    delete m67C;
                    m67C = 0;
                }
                if (m674 == 0) {
                    fn_8014DEE4(0, 0, 0);
                }
                fn_8014F8F4()->fn_8014F860(9, this, 0);
            }
            if (lbl_8087F048 != 0) {
                lbl_8087F048->fn_80106728(0, &mMotion);
            }
            fn_800C31F4(sSoundNames_unk[7], 0, -1, 1.0f);
        }
    }
}


int CharaTask::fn_801479D8() {
    return mFlag6;
}

void atn::Matrix3x4::fn_801479E4(const Vector3& v) {
    m[0][3] = v.x;
    m[1][3] = v.y;
    m[2][3] = v.z;
}

Model_unk::Node_unk* Model_unk::fn_80147A00() {
    return m220;
}

atn::Matrix3x4 atn::Matrix3x4::fn_80147A08(const Quaternion_unk& q) {
    Matrix3x4 r;
    PSMTXQuat(r.m, &q);
    return r;
}

void CharaTask::fn_80147A18(int value) {
    m13F4 = value;
    fn_80094E2C(&mMotion, value);
    if (m648 != 0) {
        fn_80094E2C(m648->m010, value);
        if (m64C != 0) {
            fn_80094E2C(m64C->m010, value);
        }
    }
    chr::CharaEquip_unk** p = m680;
    for (unsigned int i = 0; i < 8; i++, p++) {
        if (*p != 0) {
            fn_80094E2C((*p)->m024, value);
        }
    }
    for (unsigned int i = 0; i < m6A4.mCount; i++) {
        if (m6A4.mData[i]->mFlags.mFlag) {
            fn_80094E2C(&m6A4.mData[i]->mModel, value);
        }
    }
}

int CharaTask::fn_80148334() {
    return mFlag2;
}

// inferred: fields where a player character gets a larger collision (name invented)
inline bool IsWideField_unk(Field_unk* field) {
    if (field != 0) {
        if (field->m48 == 1) {
            return true;
        }
        if (field->m48 == 2) {
            switch (field->m4C) {
            case 15:
            case 16:
            case 27:
            case 46:
            case 55:
            case 56:
            case 57:
            case 58:
            case 59:
            case 64:
            case 65:
                return true;
            }
        }
    }
    return false;
}

// inferred: name invented
inline bool IsMode2_unk(int mode) {
    return mode == 2;
}
inline bool IsArenaMode2_unk() {
    bool result = false;
    if (lbl_8087F610 != 0 && IsMode2_unk(lbl_8087F610->m540)) {
        result = true;
    }
    return result;
}

inline atn::Vector3 MultVec_unk(Mtx m, atn::Vector3 v) {
    PSMTXMultVec(m, &v, &v);
    return v;
}
inline const atn::Vector3& MultVec2_unk(Mtx m, const atn::Vector3& v) {
    PSMTXMultVec(m, &v, (atn::Vector3*)&v);
    return v;
}
inline atn::Vector3 GetFront_unk(Mtx m, float ry) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &v, &v);
    return v;
}

// NONMATCHING: 67% (objdiff 94.9%) - instruction scheduling and register allocation; the
// original also reloads the flag word after the store to m620. Tried: accessors for the flags,
// helpers for the rotated offsets, declaration orders.
void CharaTask::vf20() {
    m620 = m5B0;
    if (mFlag13) {
        if (m648 != 0) {
            if (!fn_8013C3A8() && !(m55C == 6 && m560 == 0)) {
                m620 += 2.0f;
            }
        } else if (mType == 0) {
            if (!IsWideField_unk(lbl_8087F430) && !fn_8013C3A8() && !(m55C == 6 && m560 == 0)) {
                m620 += 2.0f;
            }
        } else if (m570 > 2.0f * m500 && m508 > 2.005f * m500) {
            m620 += MIN(2.0f, 2.0f * ((m570 - 2.0f * m500) / (m508 - 2.0f * m500)));
        }
        m620 = MAX(0.001f, m620);
    }
    m614 = mPos + m5A4;
    if (mStatus.mParamB.fn_801404E0(0x40)) {
        m614.y -= m620;
    } else {
        m614.y += m620;
    }
    atn::Vector3 a(mPos.x, mPos.y + m620, mPos.z);
    if (fn_8013C3A8()) {
        switch (mC48) {
        case 1: {
            atn::Vector3 offset(-5.0f, 0.0f, 0.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &offset, &offset);
            a += offset;
            break;
        }
        case 2: {
            atn::Vector3 offset(5.0f, 0.0f, 0.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &offset, &offset);
            a += offset;
            break;
        }
        case 3: {
            atn::Vector3 offset(mFlag29 ? 2.0f : -3.0f, 0.0f, -1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &offset, &offset);
            a += offset;
            break;
        }
        }
    }
    if (m55C == 6 && m560 == 36) {
        MtxPtr left = mMotion.GetNodeMtx_unk("leftleg");
        MtxPtr right = mMotion.GetNodeMtx_unk("rightleg");
        MtxPtr hips = mMotion.GetNodeMtx_unk("hips");
        if (left != 0 && right != 0) {
            a = (GetNodePos_unk(left) + GetNodePos_unk(right)) * 0.5f;
        } else if (hips != 0) {
            a = GetNodePos_unk(hips);
        }
    }
    float radius = m620;
    atn::Vector3 b;
    if (m520 >= 0) {
        b = GetNodePos_unk(mMotion.GetNodeMtx_unk(m520));
        b.y -= 1.0f;
        if (mType == 0 && fn_8013C3A8() && mC48 == 0) {
            radius -= 0.5f;
            b.x = a.x;
            b.z = a.z;
            Mtx m;
            a += GetFront_unk(m, mRot.y) * 2.0f;
            Mtx m2;
            b += GetFront_unk(m2, mRot.y) * 2.0f;

        }
    } else {
        b = a;
        b.y += m5B4 - 2.0f * radius;
    }
    if ((m137C & 0x4000) == 0x4000) {
        atn::Vector3 hips = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
        a.x = hips.x;
        a.z = hips.z;
    }
    if (mType == 2 && IsArenaMode2_unk() && fn_80148334() && mFlag22 != 1) {
        radius += 1.0f;
    }
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = radius;
    if (mFlag12) {
        mColli.mFlags &= ~1;
    }
}

// NONMATCHING: 94% (objdiff 99.9%) - only symbol names differ: the Item14_unk constructor is
// fn_80148990 in the target, and the "" of Vector_unk::reserve are lbl_8087D9E4/lbl_8087D9E0.
void CharaTask::vf50() {
    m624.clear();
    m624.reserve(1, 3);
    Item14_unk item;
    item.m04.mRadius = m5B0;
    item.m04.mCenter = mPos + m5A4;
    if (mStatus.mParamB.fn_801404E0(0x40)) {
        item.m04.mCenter.y -= item.m04.mRadius;
    } else {
        item.m04.mCenter.y += item.m04.mRadius;
    }
    m624.push_back(item);
    mFlagB29 = 1;
}

// NONMATCHING: 96% - the original reloads m624.mData before the vector copy (writing
// m624.mData[0] twice instead swaps r4/r5). Tried: a local item pointer, operand orders.
void CharaTask::vf54() {
    if (m624.mSize != 0) {
        Item14_unk* item = m624.mData;
        item->m04.mRadius = m5B0;
        item->m04.mCenter = mPos + m5A4;
        if (mStatus.mParamB.fn_801404E0(0x40)) {
            m624.mData[0].m04.mCenter.y -= m624.mData[0].m04.mRadius;
        } else {
            m624.mData[0].m04.mCenter.y += m624.mData[0].m04.mRadius;
        }
        if (m55C == 6 && m560 == 36) {
            MtxPtr hips = mMotion.GetNodeMtx_unk("hips");
            if (hips != 0) {
                m624.mData[0].m04.mCenter = GetNodePos_unk(hips);
            }
        }
    }
}

CharaTask::Item14_unk* CharaTask::fn_80148B0C(int index) {
    if (index >= 0 && index < (int)m624.mSize) {
        return &m624.mData[index];
    }
    return 0;
}

extern "C" void fn_80565B98(chr::CharaEquip_unk* equip); // inferred: updates the equipment

// NONMATCHING: 96% - the instructions are the same; only the offsets in @stringBase0 differ
// (the strings of the functions before this one are not all defined yet).
void CharaTask::fn_80148B38() {
    if (m648 != 0) {
        if (m648->m04 == 1) {
            const char* name = "rightwepoint";
            int motion = mMotion.mTracks[0].m00;
            if (motion == 38 || motion == 382) {
                name = "leftwepoint";
            }
            m648->m224 = mMotion.GetNodeMtx_unk(name);
        }
        m648->m050 = mMotion.m40;
        m648->vf10();
        if (m64C != 0) {
            m64C->m050 = mMotion.m40;
            m64C->vf10();
        }
    }
    chr::CharaEquip_unk** p = m680;
    for (unsigned int i = 0; i < 8; i++, p++) {
        if (*p != 0) {
            fn_80565B98(*p);
        }
    }
    for (unsigned int i = 0; i < m6A4.mCount; i++) {
        if (m6A4.mData[i]->mFlags.mFlag) {
            m6A4.mData[i]->vf08();
        }
    }
}

// inferred: node matrix hook (bound as a pointer to member at 0x8077A72C). It calls all its inline
// helpers out of line (too many to inline), so they are emitted after it as weak copies.
// NONMATCHING: 91% - r27/r28 swapped (node/name), string offsets (earlier functions missing) and
// the names of the out-of-line Quaternion_unk constructor/operator= (fn_80149884/fn_80149888).
// inferred: the node chunk is read as a ModelNodeDesc_unk; the context is not used
void CharaTask::fn_80148CC4(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk,
                            const atn::ModelNodeCallbackContext& context) {
    ModelNodeDesc_unk* node = (ModelNodeDesc_unk*)&chunk;
    if (vf1C() != 0) {
        return;
    }
    if (mLook.fn_8012A288(mtx, node) != 0) {
        return;
    }
    const char* name = node->mName;
    if (strcmp(name, "neck") == 0) {
        if (m648 == 0) {
            atn::Vector3 pos = mtx->GetTrans_unk();
            mtx->fn_8010F668();
            mtx->fn_801495E8(RotY_unk(m580 - 0.7f * m584));
            mtx->fn_801479E4(pos);
        } else if (m648 != 0 && m648->fn_8013C43C() == 1) {
            if ((mFlag31 && mC48 != 0) || fn_801479D8()) {
                atn::Vector3 rot = mFA4.ToRotation_unk();
                atn::Vector3 pos = mtx->GetTrans_unk();
                atn::Vector3 side = atn::Vector3::fn_8013C394(fn_8014052C(), atn::Vector3(0.0f, 1.0f, 0.0f));
                float angle = 0.4f * -rot.x;
                if (angle > 0.62831855f) {
                    angle = 0.62831855f;
                } else if (angle < -0.62831855f) {
                    angle = -0.62831855f;
                }
                mtx->fn_8010F668();
                mtx->fn_80149624(angle);
                mtx->fn_801479E4(pos);
            }
        }
    } else if (strcmp(name, "spine") == 0) {
        if (mFlag31) {
            if (mC48 != 0) {
                atn::Vector3 rot = mFA4.ToRotation_unk();
                float angle = atn::WrapAngle_unk(rot.y - mRot.y);
                if (atn::Abs_unk(angle) > 0.9424779f) {
                    angle = (angle > 0.0f) ? 0.9424779f : -0.9424779f;
                }
                atn::Vector3 pos = mtx->GetTrans_unk();
                mtx->fn_8010F668();
                mtx->fn_801495E8(RotY_unk(angle));
                mtx->fn_801479E4(pos);
            }
        } else if (m648 == 0 && !mFlag6) {
            atn::Vector3 pos = mtx->GetTrans_unk();
            mtx->fn_8010F668();
            mtx->fn_801495E8(RotY_unk(0.9f * m580 + m584));
            mtx->fn_801479E4(pos);
        }
    } else if (strcmp(name, "hips") == 0) {
        if (mFlag31) {
            if (mC48 != 0) {
                atn::Vector3 rot = mFA4.ToRotation_unk();
                float angle = atn::WrapAngle_unk(rot.y - mRot.y);
                if (atn::Abs_unk(angle) > 0.9424779f) {
                    if (angle > 0.0f) {
                        angle -= 0.9424779f;
                    } else {
                        angle = 0.9424779f + angle;
                    }
                    atn::Vector3 pos = mtx->GetTrans_unk();
                    mtx->fn_8010F668();
                    mtx->fn_801495E8(RotY_unk(angle));
                    mtx->fn_801479E4(pos);
                }
            }
        } else if (fn_801479D8() && m50 == 700023) {
            atn::Vector3 rot = mFA4.ToRotation_unk();
            atn::Vector3 pos = mtx->GetTrans_unk();
            float pitch = rot.x;
            if (pitch > 0.7853982f) {
                pitch = 0.7853982f;
            } else if (pitch < -0.7853982f) {
                pitch = -0.7853982f;
            }
            mtx->fn_8010F668();
            mtx->fn_80144ED4(0.5f * rot.x);
            mtx->fn_801479E4(pos);
        }
    } else if (strcmp(name, "rightshoulder") == 0 || strcmp(name, "leftshoulder") == 0) {
        if (m648 != 0 && m648->fn_8013C43C() == 1) {
            if (mFlag31 && mC48 != 0) {
                atn::Vector3 rot = mFA4.ToRotation_unk();
                atn::Vector3 pos = mtx->GetTrans_unk();
                float x = rot.x;
                if (x > 0.7853982f) {
                    x = 0.7853982f;
                } else if (x < -0.62831855f) {
                    x = -0.62831855f;
                }
                float y = 0.0f;
                if (strcmp(node->mName, "leftshoulder") == 0 && x < 0.0f && !fn_8014969C()) {
                    if (mC48 == 1) {
                        y = 0.6f * x;
                        x *= 1.1f;
                    } else {
                        y = 0.5f * x;
                    }
                }
                mtx->fn_8010F668();
                mtx->fn_801496A8(x, y, 0.0f);
                mtx->fn_801479E4(pos);
            } else if (fn_801479D8() && m50 != 700022 && m50 != 700023) {
                if (mMotion.fn_80139F2C(0) == 38 || mMotion.fn_80139F2C(0) == 382) {
                    if (strcmp(node->mName, "leftshoulder") == 0) {
                        atn::Vector3 rot = mFA4.ToRotation_unk();
                        atn::Vector3 pos = mtx->GetTrans_unk();
                        float x = rot.x;
                        if (x > 0.7853982f) {
                            x = 0.7853982f;
                        } else if (x < -0.7853982f) {
                            x = -0.7853982f;
                        }
                        mtx->fn_8010F668();
                        mtx->fn_801496A8(0.0f, 0.6f * -x - 0.04712389f, -x - 0.06283186f);
                        mtx->fn_801479E4(pos);
                    }
                } else if (mMotion.fn_80139F2C(0) == 37 || mMotion.fn_80139F2C(0) == 381) {
                    atn::Vector3 rot = mFA4.ToRotation_unk();
                    atn::Vector3 pos = mtx->GetTrans_unk();
                    float x = 0.6f * rot.x;
                    if (x > 0.7853982f) {
                        x = 0.7853982f;
                    } else if (x < -0.62831855f) {
                        x = -0.62831855f;
                    }
                    float y = 0.0f;
                    if (strcmp(node->mName, "leftshoulder") == 0) {
                        y = 0.3f * -x;
                    }
                    mtx->fn_8010F668();
                    mtx->fn_801496A8(x, y, 0.0f);
                    mtx->fn_801479E4(pos);
                }
            }
        }
    } else if (strcmp(name, "hair_b") == 0) {
        if (mFlagB27 && !mFlagB26) {
            int neck = mMotion.fn_80092814("neck", 0);
            int head = mMotion.fn_80092814("head", 0);
            if (neck >= 0 && head >= 0) {
                atn::Vector3 pos = mtx->GetTrans_unk();
                mtx->fn_8010F668();
                atn::Quaternion_unk q1;
                atn::Quaternion_unk q2;
                atn::Vector3 dir1(0.0f, 0.0f, 1.0f);
                if ((mMotion.fn_80147A00()[neck].m00 & 2) == 2) {
                    q1 = mMotion.fn_80147A00()[neck].m10;
                    q1.fn_8014989C(atn::Quaternion_unk::fn_801498F0(mMotion.fn_8000DD04()->m48[neck]->m40).fn_801498D8());
                    dir1.fn_80011410(atn::Matrix3x4::fn_80147A08(q1.fn_801498D8()));
                    dir1.fn_80139F60();
                } else {
                    dir1.Set_unk(0.0f, 0.0f, 0.0f);
                }
                atn::Vector3 dir2(0.0f, 0.0f, 1.0f);
                if ((mMotion.fn_80147A00()[head].m00 & 2) == 2) {
                    q2 = mMotion.fn_80147A00()[head].m10;
                    q2.fn_8014989C(atn::Quaternion_unk::fn_801498F0(mMotion.fn_8000DD04()->m48[head]->m40).fn_801498D8());
                    dir2.fn_80011410(atn::Matrix3x4::fn_80147A08(q2.fn_801498D8()));
                    dir2.fn_80139F60();
                } else {
                    dir2.Set_unk(0.0f, 0.0f, 0.0f);
                }
                mtx->fn_801496A8(0.0f, 0.0f, -(dir1.y + dir2.y));
                mtx->fn_801479E4(pos);
            }
        }
    }
}

atn::Matrix3x4& atn::Matrix3x4::fn_801495E8(const Matrix3x4& a) {
    PSMTXConcat(a.m, m, m);
    return *this;
}

void atn::Matrix3x4::fn_80149624(float rad) {
    Mtx t;
    Mtx r;
    PSMTXRotRad(r, 'y', rad);
    PSMTXConcat(m, r, t);
    __MtxCopy(m, t);
}

int CharaTask::fn_8014969C() {
    return mFlag30;
}

// inferred: *this = *this * rot(axis, rad) (name invented)
inline void Rotate_unk(atn::Matrix3x4& mtx, char axis, float rad) {
    Mtx r;
    Mtx t;
    PSMTXRotRad(r, axis, rad);
    PSMTXConcat(mtx.m, r, t);
    __MtxCopy(mtx.m, t);
}

void atn::Matrix3x4::fn_801496A8(float x, float y, float z) {
    Vector3 rot(x, y, z);
    Matrix3x4 t;
    Matrix3x4 m;
    Matrix3x4* p = &m;
    p->SetIdentity();
    if (0.0f != rot.z) {
        Rotate_unk(*p, 'z', rot.z);
    }
    if (0.0f != rot.y) {
        Rotate_unk(*p, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        Rotate_unk(*p, 'x', rot.x);
    }
    PSMTXConcat(this->m, p->m, t.m);
    *this = t;
}

atn::Quaternion_unk& atn::Quaternion_unk::fn_8014989C(const Quaternion_unk& a) {
    PSQUATMultiply(&a, this, this);
    return *this;
}

atn::Quaternion_unk atn::Quaternion_unk::fn_801498D8() const {
    Quaternion_unk r(*this);
    PSQUATInverse(&r, &r);
    return r;
}

extern "C" void fn_8072D210(float angle, float* s, float* c); // inferred: sine and cosine (256 units a turn)

// inferred: radians to the units of fn_8072D210 (name invented)
inline float ToAngleUnits_unk(float rad) {
    return 40.743664f * rad;
}

atn::Quaternion_unk atn::Quaternion_unk::fn_801498F0(const Vector3& rot) {
    Quaternion_unk q;
    float rx = rot.x;
    float ry = rot.y;
    float rz = rot.z;
    float cx, cy, cz;
    float sx, sy, sz;
    fn_8072D210(ToAngleUnits_unk(rx / 2.0f), &sx, &cx);
    fn_8072D210(ToAngleUnits_unk(ry / 2.0f), &sy, &cy);
    fn_8072D210(ToAngleUnits_unk(rz / 2.0f), &sz, &cz);
    float cycz = cy * cz;
    float sysz = sy * sz;
    q.w = cx * cycz + sx * sysz;
    q.x = sx * cycz - cx * sysz;
    q.y = cz * (cx * sy) + sz * (sx * cy);
    q.z = sz * (cx * cy) - cz * (sx * sy);
    return q;
}

// inferred: squared horizontal length (name invented)
inline float GetLengthXZSq_unk(const atn::Vector3& v) {
    return v.x * v.x + v.z * v.z;
}

// inferred: name invented
inline const wchar_t* SelectText_unk(int kind, const wchar_t* normal, const wchar_t* special) {
    if (kind == 1) {
        return special;
    }
    return normal;
}

// inferred: names invented
inline void DrawSphere_unk(const CharaTask::Sphere_unk sphere) {
    fn_80063D3C(lbl_8087EEB0, &sphere.mCenter, sphere.mRadius, -1, 0.0f);
}

// inferred: fields where the shadow follows the height of the player (name invented)
inline bool IsShadowField_unk(Field_unk* field) {
    if (field != 0) {
        if (field->m48 == 1) {
            return true;
        }
        if (field->m48 == 2) {
            switch (field->m4C) {
            case 2:
            case 3:
            case 10:
            case 15:
            case 16:
            case 17:
            case 27:
            case 38:
            case 46:
            case 55:
            case 56:
            case 57:
            case 58:
            case 59:
            case 64:
            case 65:
                return true;
            case 25:
                return field->m50 == 2;
            case 14:
            case 29:
            case 30:
            case 31:
            case 33:
            case 34:
            case 41:
            case 51:
                switch (field->m50) {
                case 2:
                case 3:
                case 6:
                case 11:
                    return true;
                }
                break;
            }
        }
    }
    return false;
}

inline bool CharaTask::IsShadowField_unk() {
    if ((m137C & 8) == 8) {
        return false;
    }
    return ::IsShadowField_unk(lbl_8087F430);
}

// inferred: name invented
inline bool IsArenaActive_unk() {
    bool result = false;
    if (lbl_8087F610 != 0 && IsArenaMode_unk(lbl_8087F610->m540)) {
        result = true;
    }
    return result;
}


// inferred: name invented
inline bool IsArenaWaiting_unk(Arena_unk* arena) {
    bool result = false;
    if (arena->m4FC == 30 && arena->m50C < 5) {
        result = true;
    }
    return result;
}

// NONMATCHING: 85% (objdiff 99.0%) - register allocation (weapon/loop counters), the
// (m54C & 8) != 8 test (the original computes it as ((8 - x) | (x - 8)) >> 31), the arena test
// (the original reloads lbl_8087F610), the "hdist" branch layout and the scheduling of a few
// swprintf arguments; the string offsets also differ until the earlier functions are added.
// Tried: bool/int/ternary forms of the tests, inline helpers, declaration orders.
void CharaTask::fn_80149A30() {
    if (lbl_8087F0A8->m20 != 0 && (m54C & 8) != 8 && !mFlagB9) {
        float dist = LengthSq_unk(lbl_8087EFB4->m104.mEye - mPos);
        if (m55C != 8) {
            mMotion.mAC = 1.0f;
            if ((IsPartyMember_unk() || lbl_8087FA20 != 0) && dist > 360000.0f) {
                return;
            }
            if (lbl_8087F430 != 0 && lbl_8087F430->m868 != 6) {
                atn::Vector3 pos;
                if (m520 >= 0) {
                    pos = GetNodePos_unk(mMotion.GetNodeMtx_unk(m520));
                } else if (m524 >= 0) {
                    pos = GetNodePos_unk(mMotion.GetNodeMtx_unk(m524));
                } else {
                    pos = mPos;
                }
                atn::Vector3 screen;
                fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
                float hdist = 0.0f;
                if (!(screen.z < 0.0f)) {
                    hdist = GetLengthXZSq_unk(lbl_8087EFB4->m104.mEye - pos);
                }

                if (mType == 0) {
                    bool check = true;
                    int mode = lbl_8087F430->m868;
                    if (mode == 3) {
                        check = false;
                    } else if (mode == 4) {
                        check = false;
                        if (fn_801479D8() && !HasPart674_unk()) {
                            check = true;
                        }
                    }
                    if (check) {
                        atn::Vector3 cam = lbl_8087EFB4->m104.mEye;
                        MtxPtr head = mMotion.GetNodeMtx_unk("head");
                        MtxPtr neck = mMotion.GetNodeMtx_unk("neck");
                        MtxPtr hips = mMotion.GetNodeMtx_unk("hips");
                        MtxPtr rshoulder = mMotion.GetNodeMtx_unk("rightshoulder");
                        MtxPtr rupleg = mMotion.GetNodeMtx_unk("rightupleg");
                        MtxPtr lshoulder = mMotion.GetNodeMtx_unk("leftshoulder");
                        MtxPtr lupleg = mMotion.GetNodeMtx_unk("leftupleg");
                        if (head != 0 && neck != 0 && hips != 0 && rshoulder != 0 && rupleg != 0 &&
                            lshoulder != 0 && lupleg != 0) {
                            Sphere_unk sphere;
                            sphere.mCenter = GetNodePos_unk(head);
                            sphere.mCenter.y += 0.7f;
                            sphere.mRadius = 2.3f;
                            if (LengthSq_unk(cam - sphere.mCenter) < sphere.mRadius * sphere.mRadius) {
                                return;
                            }
                            Capsule_unk capsule;
                            capsule.mStart = GetNodePos_unk(neck);
                            capsule.mStart.y -= 0.2f;
                            capsule.mEnd = GetNodePos_unk(hips);
                            capsule.mRadius = 1.9f;
                            if (fn_80050A1C(&cam, &capsule, 0) < capsule.mRadius * capsule.mRadius) {
                                return;
                            }
                            capsule.mStart = GetNodePos_unk(rshoulder);
                            capsule.mEnd = GetNodePos_unk(rupleg);
                            capsule.mRadius = 1.9f;
                            if (fn_80050A1C(&cam, &capsule, 0) < capsule.mRadius * capsule.mRadius) {
                                return;
                            }
                            capsule.mStart = GetNodePos_unk(lshoulder);
                            capsule.mEnd = GetNodePos_unk(lupleg);
                            capsule.mRadius = 1.9f;
                            if (fn_80050A1C(&cam, &capsule, 0) < capsule.mRadius * capsule.mRadius) {
                                return;
                            }
                        }
                    }
                } else if (mFlag22 != 1 && hdist < 25.0f && lbl_8087EFB4->m104.mEye.y - pos.y < 20.0f) {
                    return;
                }
            }
        } else {
            mMotion.mAC = 1.3f;
        }

        if (lbl_8087F430 != 0 && lbl_8087FA20 == 0) {
            int visible = 1;
            if (lbl_8087F430 != 0) {
                switch (fn_80370174(lbl_8087F430, 217)) {
                case 1: {
                    visible = 1;
                    bool b = true;
                    if (mType != 0 && mFlag22 != 1) {
                        b = false;
                    }
                    if (!b && m55C != 8) {
                        visible = 0;
                    }
                    break;
                }
                case 2:
                    visible = m55C == 8;
                    break;
                case 3:
                    visible = mType == 2;
                    break;
                }
            }
            if (visible) {
                mMotion.mFlags |= 0x400;
                if (fn_8014FB60()) {
                    m648->m014 |= 0x400;
                    if (m64C != 0) {
                        m64C->m014 |= 0x400;
                    }
                }
                chr::CharaEquip_unk** p = m680;
                for (unsigned int i = 0; i < 8; i++, p++) {
                    chr::CharaEquip_unk* equip = *p;
                    if (equip != 0 && ((1u << i) & m6A0) == (1u << i)) {
                        equip->m028 |= 0x400;
                    }
                }
                for (unsigned int i = 0; i < m6A4.mCount; i++) {
                    if (m6A4.mData[i]->mFlags.mFlag) {
                        m6A4.mData[i]->mModel.mFlags |= 0x400;
                    }
                }
            } else {
                mMotion.mFlags &= ~0x400;
                if (fn_8014FB60()) {
                    m648->m014 &= ~0x400;
                    if (m64C != 0) {
                        m64C->m014 &= ~0x400;
                    }
                }
                chr::CharaEquip_unk** p = m680;
                for (unsigned int i = 0; i < 8; i++, p++) {
                    chr::CharaEquip_unk* equip = *p;
                    if (equip != 0 && ((1u << i) & m6A0) == (1u << i)) {
                        equip->m028 &= ~0x400;
                    }
                }
                for (unsigned int i = 0; i < m6A4.mCount; i++) {
                    if (m6A4.mData[i]->mFlags.mFlag) {
                        m6A4.mData[i]->mModel.mFlags &= ~0x400;
                    }
                }
            }
        }

        int shadow = 1;
        if (IsShadowField_unk()) {
            if (lbl_8087F8A0 != 0) {
                CharaTask* player = lbl_8087F8A0->mHead;
                if (player != 0 && mType != 0) {
                    atn::Vector3 v = player->mPos - mPos;
                    shadow = atn::Abs_unk(v.y) < 20.0f;
                }
            }
        }
        if (mFlag3 || mFlagB1 || (m137C & 8) == 8) {
            shadow = 0;
        }
        if (shadow) {
            mMotion.mFlags |= 4;
        } else {
            mMotion.mFlags &= ~4;
        }
        mMotion.fn_8008CD60();

        int parts = fn_8014FB60();
        if (parts) {
            if (lbl_8087F610 != 0 && (m678 != 0 || m67C != 0)) {
                if (m678 != 0 && m678->vf0C() == 0) {
                    m648->vf14();
                }
                if (m67C != 0 && m67C->vf0C() == 0 && m64C != 0) {
                    m64C->vf14();
                }
            } else {
                if (shadow) {
                    m648->m014 |= 4;
                } else {
                    m648->m014 &= ~4;
                }
                m648->vf14();
                if (m64C != 0) {
                    if (shadow) {
                        m64C->m014 |= 4;
                    } else {
                        m64C->m014 &= ~4;
                    }
                    m64C->vf14();
                }
            }
        } else {
            if (m648 != 0) {
                fn_80044E0C(m648, 1);
            }
            if (m64C != 0) {
                fn_80044E0C(m64C, 1);
            }
        }

        int weapon = (m1208 == 0) ? 1 : 0;
        if (weapon) {
            weapon = (unsigned int)(mMotion.fn_80139F2C(0) - 108) > 4;
        }
        if (weapon) {
            int b = (m648 != 0) ? 1 : 0;
            if (b) {
                b = m648->m04 == 1;
            }
            weapon = !b;
        }
        chr::CharaEquip_unk** p = m680;
        for (unsigned int i = 0; i < 8; i++, p++) {
            chr::CharaEquip_unk* equip = *p;
            if (equip != 0 && ((1u << i) & m6A0) == (1u << i)) {
                if (equip->m000 == 4 && mStatus.IsState_unk(0x80000)) {
                    continue;
                }
                if (equip->m000 == 4) {
                    if (shadow) {
                        equip->m028 |= 4;
                    } else {
                        equip->m028 &= ~4;
                    }
                }
                if ((*p)->m000 != 4 || weapon) {
                    fn_80565F34(*p);
                }
            }
        }
        for (unsigned int i = 0; i < m6A4.mCount; i++) {
            if (m6A4.mData[i]->mFlags.mFlag) {
                m6A4.mData[i]->vf0C();
            }
        }
        if (m50 == 700022) {
            mMotion.fn_8009373C("op_03_shield1", weapon);
            mMotion.fn_8009373C("op_03_shield2", weapon);
            mMotion.fn_8009373C("op_04_shield_dam_a", weapon);
            mMotion.fn_8009373C("op_01_arm1", !weapon);
            mMotion.fn_8009373C("op_01_cloth1", !weapon);
            mMotion.fn_8009373C("op_01_cloth2", !weapon);
            mMotion.fn_8009373C("op_02_arm1", weapon);
            mMotion.fn_8009373C("op_02_cloth1", weapon);
            mMotion.fn_8009373C("op_02_cloth2", weapon);
        } else if (m50 == 700023) {
            mMotion.fn_8009373C("archery1", parts);
            mMotion.fn_8009373C("archery1_light", parts);
            mMotion.fn_8009373C("archery2", parts);
            mMotion.fn_8009373C("archery2_light", parts);
        }

        atn::Vector3 screen;
        fn_800BFAC8(&screen, lbl_8087EFB4, &mPos);
        float y = screen.y - 40.0f;
        if (lbl_8087F0A8->m34 != 0 && dist < 62500.0f) {
            fn_80063D3C(lbl_8087EEB0, &m614, m620, 0xFFFFFF00, 0.0f);
            if (screen.z > 0.0f && screen.z < 1.0f) {
                wchar_t text[256];
                sprintf(lbl_807C7B28, "ch%04d_%02d", m50 / 100, m50 % 100);
                fn_8006F2F0(lbl_8087EEC8, text, lbl_807C7B28, 256);
                float w = fn_8006EF48(lbl_8087EEC8, text, 12.0f, 0.0f, 1, 1);
                fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, -1, 1, 1,
                            0, 0, 0xFF000000);
                y += 12.0f;
                swprintf(text, 256, L"ControlMode %d, %d", m55C, m560);
                w = fn_8006EF48(lbl_8087EEC8, text, 12.0f, 0.0f, 1, 1);
                fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, -1, 1, 1,
                            0, 0, 0xFF000000);
                y += 12.0f;
                swprintf(text, 256, L"Motion(blend, speed) wt(%.2f, %.2f) wk(%.2f, %.2f) rn(%.2f, %.2f)",
                         mMotion.fn_8013C3E4(0), mMotion.GetSpeed_unk(0), mMotion.fn_8013C3E4(1),
                         mMotion.GetSpeed_unk(1), mMotion.fn_8013C3E4(2), mMotion.GetSpeed_unk(2));
                w = fn_8006EF48(lbl_8087EEC8, text, 12.0f, 0.0f, 1, 1);
                fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, -1, 1, 1,
                            0, 0, 0xFF000000);
                y += 12.0f;
                swprintf(text, 256, L"inertia %.2f", m570);
                w = fn_8006EF48(lbl_8087EEC8, text, 12.0f, 0.0f, 1, 1);
                fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, -1, 1, 1,
                            0, 0, 0xFF000000);
                y += 12.0f;
            }
        }
        if (lbl_8087F0A8->m58 != 0 && dist < 62500.0f && screen.z > 0.0f && screen.z < 1.0f) {
            wchar_t text[256];
            char buf[256];
            MapManager* map = MapManager::sInstance;
            MapArea_unk* area = (MapArea_unk*)map->fn_804918A4(mMotion.fn_80094F98());
            if (area != 0) {
                sprintf(buf, "Area: %s\0", area->mName);
            } else {
                sprintf(buf, "Area: ---\0", area->mName);
            }
            fn_8006F2F0(lbl_8087EEC8, text, buf, 256);
            fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, 0xAFFFFFFF, 1, 1, 0, 0,
                        0xFF000000);
            y += 12.0f;
        }
        if (lbl_8087F0A8->m54 != 0 && mType != 5 && dist < 62500.0f && screen.z > 0.0f && screen.z < 1.0f) {
            wchar_t text[256];
            char name[512];
            char buf[256];
            sprintf(lbl_807C7B28, "ch%04d_%02d", m50 / 100, m50 % 100);
            sprintf(buf, "%d: %s (%s)\0", m58, m60->m0C, lbl_807C7B28);
            fn_8006F2F0(lbl_8087EEC8, text, buf, 256);
            fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, 0xAFFFFFFF, 1, 1, 0, 0,
                        0xFF000000);
            y += 12.0f;
            if (mType == 0) {
                swprintf(text, 256, L"Lv %d   Life %d   Ten %.3f   Explosion %.3f", mStatus.mParamB.m78,
                         mStatus.m224, mStatus.m228, mStatus.m42C);
            } else if (mType == 3) {
                swprintf(text, 256, L"Lv %d   Life %d   Ten %.3f", mStatus.mParamB.m78, mStatus.m224, mStatus.m228);
            } else {
                swprintf(text, 256, L"Lv %d   Ten %.3f", mStatus.mParamB.m78, mStatus.m228);
            }
            fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, 0xAFFFFFFF, 1, 1, 0, 0,
                        0xFF000000);
            y += 12.0f;
            swprintf(text, 256, L"HP %.2f   MP %.2f   %c", mStatus.m04, mStatus.m08,
                     mStatus.IsState_unk(0x20) ? 'D' : ' ');
            fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, 0xAFFFFFFF, 1, 1, 0, 0,
                        0xFF000000);
            y += 12.0f;
            swprintf(text, 256, L"Atk %.2f   MAtk %.2f", mStatus.mParamB.m00, mStatus.mParamB.m04);
            fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, 0xAFFFFFFF, 1, 1, 0, 0,
                        0xFF000000);
            y += 12.0f;
            swprintf(text, 256, L"Def %.2f   MDef %.2f", mStatus.mParamB.m08, mStatus.mParamB.m0C);
            fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, 0xAFFFFFFF, 1, 1, 0, 0,
                        0xFF000000);
            y += 12.0f;
            for (unsigned int i = 0; i < m12D0.mCount; i++) {
                CharaHit_unk* hit = &m12D0.mData[i];
                if (hit->mHit != 0 && hit->m04 == 0) {
                    fn_800DD3FC(text, L"CIRCLE [%d:%s] count=%d\0", hit->mHit->m04, hit->mHit->m08, hit->m08);
                    fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, 0xAFFFFFFF, 1,
                                1, 0, 0, 0xFF000000);
                    y += 12.0f;
                }
            }
            for (unsigned int i = 0; i < mStatus.mRefs.mCount; i++) {
                CharaStatusRef_unk* ref = &mStatus.mRefs.mData[i];
                if (fn_8021BE98(name, ref->mEntry) != 0) {
                    fn_800DD3FC(text, L"SP [%s] count=%d\0", name, ref->m0C);
                    fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, 0xAFFFFFFF, 1,
                                1, 0, 0, 0xFF000000);
                    y += 12.0f;
                }
            }
            for (unsigned int i = 0; i < 32; i++) {
                if (mStatus.IsState_unk(1 << i)) {
                    sprintf(buf, "COND [%s] count=%d\0", sStatusNames_unk[i], mStatus.fn_801333A4(1 << i));
                    fn_8006F2F0(lbl_8087EEC8, text, buf, 256);
                    fn_80061824(lbl_8087EEB0, screen.x, y, 0.0f, 12.0f, 12.0f, 0.0f, 0.0f, 0.0f, text, 0xAFFFFFFF, 1,
                                1, 0, 0, 0xFF000000);
                    y += 12.0f;
                }
            }
        }
        if (lbl_8087F0A8->m38 != 0) {
            for (unsigned int i = 0; i < m624.mSize; i++) {
                DrawSphere_unk(m624.mData[i].m04);
            }
        }

        int hide = 0;
        if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8 && lbl_8087F540 != 0 && fn_80481654(lbl_8087F540) != 0) {
            hide = 1;
        }
        if (IsFlag_unk(1)) {
            return;
        }
        if (hide) {
            return;
        }
        if (mFlag10 && lbl_8087F048 != 0) {
            fn_8010A4A8(lbl_8087F048, this);
        }
        Arena_unk* arena = lbl_8087F610;
        if (arena == 0 || IsArenaWaiting_unk(arena)) {
            if (((unsigned int)mStatus.m14 & 0x2000) == 0x2000) {
                atn::Vector3 pos = mColli.m48 + atn::Vector3(0.0f, 1.5f * m5B0, 0.0f);
                const wchar_t* text;
                if (mType == 2 && !mStatus.IsState_unk(0x100) && !mStatus.IsState_unk(0x10000) &&
                    !IsArenaActive_unk()) {
                    text = lbl_8087F1E4->Get_unk(127);
                } else {
                    text = lbl_8087F1E4->Get_unk(129);
                }
                float size = 12.0f;
                float w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 1);
                atn::Vector3 screen;
                fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
                if (screen.z > 0.0f && screen.z < 1.0f) {
                    fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, screen.y, 0.0f, size, size, 0.0f, 0.0f, 0.0f, text,
                                0xA000FFFF, 1, 1, 0, 0, 0xFF000000);
                }
            } else if (mStatus.IsState_unk(1)) {
                atn::Vector3 pos = mColli.m48 + atn::Vector3(0.0f, 1.5f * m5B0, 0.0f);
                const wchar_t* text = lbl_8087F1E4->Get_unk(65);
                float size = 12.0f;
                float w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 1);
                atn::Vector3 screen;
                fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
                if (screen.z > 0.0f && screen.z < 1.0f) {
                    fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, screen.y, 0.0f, size, size, 0.0f, 0.0f, 0.0f, text,
                                0xD0AA88AA, 1, 1, 0, 0, 0xFF000000);
                }
            } else if (mStatus.IsState_unk(0x400000)) {
                atn::Vector3 pos = mColli.m48 + atn::Vector3(0.0f, 1.5f * m5B0, 0.0f);
                const wchar_t* text = lbl_8087F1E4->Get_unk(97);
                float size = 12.0f;
                float w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 1);
                atn::Vector3 screen;
                fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
                if (screen.z > 0.0f && screen.z < 1.0f) {
                    fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, screen.y, 0.0f, size, size, 0.0f, 0.0f, 0.0f, text,
                                0xC0AA88FF, 1, 1, 0, 0, 0xFF000000);
                }
            }
        }

        if (mC58.m11C.m01 != 0 && mType == 2) {
            if (mStatus.IsState_unk(0x20)) {
                return;
            }
            if (mC58.m11C.m01 == 2) {
                return;
            }
            atn::Vector3 pos = mColli.m48 + atn::Vector3(0.0f, 1.5f * m5B0, 0.0f);
            float size = 30.0f * (0.4f * m12C8 + 1.0f);
            const wchar_t* text1 = lbl_8087F1E4->Get_unk(121);
            const wchar_t* text2 = lbl_8087F1E4->Get_unk(123);
            const wchar_t* text = lbl_8087F1E4->Get_unk(927);
            unsigned int color = 0xA0FFC040;
            if ((mC58.m11C.fn_8011D21C() == 0 && (mC58.m11C.m20 & 2) == 2) || (mC58.m11C.m20 & 4) == 4) {
                size *= 0.6f;
            } else if (mC58.m11C.m01 == 1) {
                text = text2;
                color = 0xA0FF4040;
            } else {
                text = text1;
            }
            if (mC58.m11C.m00 == 1 && (mC58.m11C.m20 & 4) != 4 && mC58.m11C.m04 >= 90) {
                text = 0;
            }
            if (text == 0) {
                return;
            }
            float w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 1);
            atn::Vector3 screen;
            fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
            if (!(screen.z > 0.0f)) {
                return;
            }
            if (!(screen.z < 1.0f)) {
                return;
            }
            if (lbl_8087F430 != 0 && lbl_8087F430->m868 != 6) {
                screen.y = MIN(MAX(screen.y, 0.1f * lbl_8087EEE0->mHeight), 0.9f * lbl_8087EEE0->mHeight - size);
            }
            fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, screen.y - 12.0f - m12C8, 0.0f, size, size, 0.0f, 0.0f,
                        0.0f, text, color, 1, 1, 0, 0, 0xFF000000);
        } else if (lbl_8087F098 != 0 && lbl_8087F098->m48 != 0) {
            if (mC58.m11C.m01 == 0) {
                return;
            }
            atn::Vector3 pos = mColli.m48 + atn::Vector3(0.0f, 1.5f * m5B0, 0.0f);
            const wchar_t* text = SelectText_unk((unsigned char)mC58.m11C.m01, lbl_8087F1E4->Get_unk(121), lbl_8087F1E4->Get_unk(123));
            unsigned int color = (mC58.m11C.m01 == 1) ? 0xA0FF4040 : 0xA0FFC040;

            float size = 30.0f * (0.4f * m12C8 + 1.0f);
            float w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 1);
            atn::Vector3 screen;
            fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
            if (screen.z > 0.0f && screen.z < 1.0f) {
                fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, screen.y - 12.0f - m12C8, 0.0f, size, size, 0.0f, 0.0f,
                            0.0f, text, color, 1, 1, 0, 0, 0xFF000000);
            }
        } else if (m55C == 6 && m560 == 96 && !mStatus.fn_80134290()) {
            atn::Vector3 pos = mColli.m48 + atn::Vector3(0.0f, 1.5f * m5B0, 0.0f);
            const wchar_t* text = lbl_8087F1E4->Get_unk(121);
            float size = 30.0f * (0.4f * m12C8 + 1.0f);
            float w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 1);
            atn::Vector3 screen;
            fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
            if (screen.z > 0.0f && screen.z < 1.0f) {
                fn_80061824(lbl_8087EEB0, screen.x - 0.5f * w, screen.y - 12.0f - m12C8, 0.0f, size, size, 0.0f, 0.0f,
                            0.0f, text, 0xA0FFC040, 1, 1, 0, 0, 0xFF000000);
            }
        }
    }
}

// inferred: debug menu helpers (signatures as in game/GimmickCommon.cpp)
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087AA0(void* group, const char* name, unsigned int* flags, int bit, int arg4, int arg5); // inferred: bit toggle

// NONMATCHING: pool offsets only - instructions identical; the @stringBase0 offsets of the menu
// labels differ until the strings of the earlier functions (0x80136544-0x80149A30) exist.
// inferred: debug menu of the character (the menu is created once, under `menu`)
void CharaTask::vf30(void* menu) {
    if (menu == 0) {
        return;
    }
    char name[32];
    sprintf(lbl_807C7B28, "ch%04d_%02d", m50 / 100, m50 % 100);
    sprintf(name, "%s [%d]", lbl_807C7B28, m58);
    void* group;
    if (m54.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m54.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_80087E9C(group, "position", &mPos, 0, 0, -9999.0f, 9999.0f, 1.0f);
    fn_80088724(group, "rotation", &mRot, 0, 0, -360.0f, 360.0f, 10.0f);
    fn_80087E9C(group, "scale", &m540, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_8008771C(group, "moveSpeed", &m568, 0, 0, 0.0f, 9999.0f, 0.05f);
    fn_800874C8(group, "ctrlMode", &m55C, 0, 10, 1, 0, 0);
    fn_80087AA0(group, "invincible", &m54C, 4, 0, 0);
    fn_800874C8(group, "attackedMax", &m1028, 0, 16, 1, 0, 0);
    if (mType == 0 || mType == 3) {
        mLook.fn_8012AD20(group);
    }
    if (mType == 2) {
        void* pick = fn_8008937C(group, "Pick");
        fn_8008771C(pick, "DetectRadius", &mC58.m11C.m14, 0, 0, 0.0f, 9999.0f, 1.0f);
        fn_8008771C(pick, "WarnRadius", &mC58.m11C.m18, 0, 0, 0.0f, 9999.0f, 1.0f);
        fn_800874C8(pick, "WarpPointId", &mC58.m11C.m08, 0, 9999999, 1, 0, 0);
        fn_800874C8(pick, "SafeAreaId", &mC58.m11C.m0C, 0, 9999999, 1, 0, 0);
        fn_8008771C(pick, "SightRadius", &mC58.m11C.m1C, 0, 0, 0.0f, 9999.0f, 1.0f);
        fn_800874C8(pick, "UnsafeAreaId", &mC58.m11C.m10, 0, 9999999, 1, 0, 0);
    }
}

// inferred: motion resource info (fn_802096A8 looks it up by the motion name)
struct MotionInfo_unk {
    int m00;
    float m04; // inferred: speed/blend value scaled by CharaTask::m514
};
extern "C" MotionInfo_unk* fn_802096A8(const char* name);

// inferred: name invented
inline MotionInfo_unk* FindMotionInfo_unk(Model_unk& model, int motion) {
    atn::LoadHandle* handle = model.fn_80097D40(motion);
    return handle != 0 ? fn_802096A8(((atn::LoadHandle*)(handle))->fn_80473F18()) : 0;
}

void CharaTask::fn_8014C0B4(int index, int motion) {
    m484[index] = motion;
    switch (index) {
    case 1: {
        MotionInfo_unk* info = FindMotionInfo_unk(mMotion, motion);
        float value;
        if (info != 0) {
            value = info->m04;
        } else {
            value = 0.2f;
        }
        m500 = value * m514;
        break;
    }
    case 2: {
        MotionInfo_unk* info = FindMotionInfo_unk(mMotion, motion);
        float value;
        if (info != 0) {
            value = info->m04;
        } else {
            value = 0.8f;
        }
        m504 = value * m514;
        break;
    }
    case 3: {
        MotionInfo_unk* info = FindMotionInfo_unk(mMotion, motion);
        float value;
        if (info != 0) {
            value = info->m04;
        } else {
            value = 1.0f;
        }
        m508 = value * m514;
        break;
    }
    case 27: {
        MotionInfo_unk* info = FindMotionInfo_unk(mMotion, motion);
        float value;
        if (info != 0) {
            value = info->m04;
        } else {
            value = 0.25f;
        }
        m50C = value * m514;
        break;
    }
    }
}

void CharaTask::fn_8014C228() {
    fn_8014C0B4(0, m510);
    fn_8014C0B4(1, 0x14);
    fn_8014C0B4(2, 0x1A);
    if (fn_80219558(m50) == 0) {
        fn_8014C0B4(3, 0x23);
    } else {
        fn_8014C0B4(3, 0x20);
    }
    fn_8014C0B4(4, 0x28);
    fn_8014C0B4(5, 0x195);
    fn_8014C0B4(6, 0x196);
    fn_8014C0B4(7, 0x1B1);
    fn_8014C0B4(8, 0x1B2);
    fn_8014C0B4(9, 0x197);
    fn_8014C0B4(10, 0x198);
    fn_8014C0B4(11, 0x1B3);
    fn_8014C0B4(12, 0x1B4);
    fn_8014C0B4(13, 0x199);
    fn_8014C0B4(14, 0x19A);
    fn_8014C0B4(15, 0x1B5);
    fn_8014C0B4(16, 0x1B6);
    fn_8014C0B4(17, 0x19B);
    fn_8014C0B4(18, 0x19C);
    fn_8014C0B4(19, 0x156);
    fn_8014C0B4(20, 0x156);
    fn_8014C0B4(22, 0x15C);
    fn_8014C0B4(21, 0x15D);
    fn_8014C0B4(23, 0x15E);
    fn_8014C0B4(24, 0x15F);
    fn_8014C0B4(25, 0x1C0);
    fn_8014C0B4(26, 0x1C1);
    fn_8014C0B4(27, 0x1C6);
    fn_8014C0B4(28, 0x1C7);
    fn_8014C0B4(29, 0x1C8);
    fn_8014C0B4(30, 0x1C9);
    fn_8014FD30();
}


extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_80107850(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107B20(Effect_unk* effect, chr::Motion_unk* motion);
// inferred: effect manager (see gmk/PlayEffect.cpp); mB8 is set to 2 while CharaTask::vf98 starts its effect
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8010C00C(Effect_unk* mgr, CharaTask* chara);
extern "C" void fn_80107CA8(Effect_unk* effect, CharaTask* chara, int arg);
extern "C" void fn_80107BE8(Effect_unk* effect, CharaTask* chara, chr::Motion_unk* motion);
extern "C" void fn_800FC410(Effect_unk* effect, CharaTask* chara, CharaTask* target, int arg, HitInfo_unk* hit, int id,
                            int frames);
extern "C" bool fn_80051A88(const Model_unk::Sphere_unk* sphere, const atn::Vector3* point); // inferred: point in sphere
extern "C" void fn_80239DAC(void* mgr, void* owner, int a, int b); // inferred: stops the effects of `owner`
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the effects (a matrix or an EfpHandle_unk)
extern "C" void fn_8023A680(void* mgr, void* res, int a, int b, atn::Matrix3x4* mtx, int c, int d, const float* color,
                            float scale, int e, int f, int g);
extern "C" int rand();
struct Input_unk {
    int fn_8012476C(int button); // inferred: nonzero while `button` is pressed
    float fn_801248DC(atn::Vector3* dir); // inferred: stick tilt, direction in `dir` (may be 0)
};

// inferred: lens of a camera (Model_unk::m1DC, or the one of the global camera); names invented
struct Lens_unk {
    unsigned char m000[0x158];
    float m158;
    float m15C;
    float m160;
};
// inferred: the global camera seen with its lens pointer at 0x2FC
struct CameraLens_unk {
    unsigned char m000[0x2FC];
    Lens_unk* m2FC;
};
// inferred: result of fn_800C122C (screen data of a sphere); m2C..m38 is a color
struct ScreenInfo_unk {
    unsigned char m00[0x2C];
    float mColor[4];
};
extern "C" ScreenInfo_unk* fn_800C122C(Camera_unk* camera, const Model_unk::Sphere_unk* sphere);

// inferred: link of the character lists of the managers (CharaTask + 0x14AC)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
inline CharaTask* NextChara_unk(CharaTask* chara) {
    return ((CharaListNode_unk*)chara)->mNext;
}

// inferred: names invented
inline Lens_unk* CharaTask::GetLens_unk() {
    return (mMotion.m1DC == 0) ? ((CameraLens_unk*)lbl_8087EFB4)->m2FC : mMotion.m1DC;
}

// inferred: matrix of the emote above the head: translation to the head then the rotation
inline void CharaTask::SetEmoteMtx_unk() {
    atn::Vector3 pos(mPos);
    pos.y = -GetLens_unk()->m160 * GetLens_unk()->m158 + 0.01f;
    Mtx t;
    PSMTXTrans(t, pos.x, pos.y, pos.z);
    atn::Matrix3x4 rot;
    atn::Matrix3x4* p = &rot;
    atn::Matrix3x4* mtx = &mF20;
    __MtxCopy(mtx->m, t);
    p->SetIdentity();
    if (0.0f != mRot.y) {
        Rotate_unk(*p, 'y', mRot.y);
    }
    if (0.0f != mRot.x) {
        Rotate_unk(*p, 'x', mRot.x);
    }
    if (0.0f != mRot.z) {
        Rotate_unk(*p, 'z', mRot.z);
    }
    Mtx r;
    PSMTXConcat(mtx->m, p->m, r);
    __MtxCopy(mtx->m, r);
}

// inferred: the character recovers from a state (0x100 or 0x10000); the shared part (static so
// that the sound name stays in the file pool)
static inline void Recover_unk(CharaTask* chara) {
    if (chara->mType != 2) {
        chara->mF18 = chara->mF14;
        chara->mF14 = 0;
    } else {
        chara->mF18 = chara->mF14;
        chara->mF14 = 1;
    }
    chara->mC58.fn_8011BEB8(0);
    if (chara->IsOnLedge_unk()) {
        chara->ReleaseLedge_unk();
    }
    if (chara->IsFlag5_unk()) {
        chara->mFlag5 = 0;
    }
    chara->Unlink_unk();
    chara->ClearEffect_unk();
    chara->mC58.mC4 = 0;
    if (chara->mType == 0) {
        chara->m55C = 1;
        chara->m564 = 1;
    }
    fn_800C344C("SE_BTCOM_076", &chara->mPos, 0, -1, 1.0f);
}

inline bool CharaTask::IsLedgeLocked_unk() {
    return mFlag31 && mC48 != 0;
}

// inferred: bullet of a hit with flags 0x440000
inline HitInfo_unk* CharaTask::FindHitBullet_unk() {
    CharaHit_unk* hits = m12D0.mData;
    for (unsigned int i = 0; i < m12D0.mCount; i++) {
        HitInfo_unk* hit = hits[i].mHit;
        if ((hit->mAC & 0x440000) == 0x440000 && hit->m04 > 0) {
            return fn_80219E6C(hit->m04);
        }
    }
    return 0;
}

// inferred: `other` is on the same side as this character
inline bool CharaTask::IsSameSide_unk(CharaTask* other) {
    if (other != this && ((lbl_8087F610 != 0 && lbl_8087F610->m540 == 0) || (other->mStatus.m0C & 2) == 2 ||
                          (mStatus.m0C & 2) == 2 || lbl_8087F0A8->mCC != 0)) {
        return false;
    }
    if (other->mF14 >= 0 && mF14 >= 0) {
        return mF14 == other->mF14;
    }
    return other->mType == mType || (other->mType == 0 && mType == 3) || (other->mType == 3 && mType == 0);
}

// NONMATCHING: ~66% by line (554/1641; 1613 vs 1641 instructions) - first pass of a very long
// function: same calls, constants and control flow; the frame is 0x50 bytes smaller (stack slots of
// the locals/temporaries differ), the ptmf .data offsets are noise, fn_8018EEC4 needs a map entry,
// and register allocation differs in the bullet loops; also the matrix of the emote and the
// "hide" condition are scheduled differently. The bullet loop bodies are written in place because
// a helper put the caller over the inline budget (nothing was inlined).
// inferred: per-frame update of the character state (called by the derived classes)
void CharaTask::fn_8014C540() {
    if (lbl_8087F430 != 0 && lbl_8087F430->m56D4 == 0 && lbl_8087F430->m54E4 != 8) {
        mStatus.fn_8012E540();
    }
    mStatus.fn_8012B988();
    fn_80178BFC();
    mFlagB31 = 0;
    mFlagB30 = 0;
    if (mType == 2) {
        for (CharaHit_unk* hit = m12D0.begin(); hit != m12D0.end(); hit++) {
            if ((hit->m0C & 2) == 2) {
                if (lbl_8087F0A8->m294 == 0) {
                    mFlagB31 = 1;
                } else {
                    mFlagB30 = 1;
                }
                break;
            }
        }
    }
    if (m640 > 0) {
        m640--;
    }
    if (--mFB0 < -60) {
        mFB0 = -60;
        mFB4 = 0;
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->m56D4 == 0 && mF94 > 0 && !mFlag23 && --mF94 == 0) {
        chr::ControlCdnStone* control = new (0, "", "", 0) chr::ControlCdnStone(this, 0);
        fn_80178208(control);
    }
    if (mFlag8 && mF9C > 0) {
        mF9C--;
    }
    if (mType == 0) {
        if (mStatus.IsState_unk(1)) {
            CameraState_unk state(lbl_8087EFA8->m324);
            float t = mStatus.fn_801333A4(1) / 900.0f;
            state.m4C = 1.0f - t;
            lbl_8087EFA8->m324 = state;
        }
        if (mF84 > 0) {
            mF84--;
            if (((Input_unk*)((char*)lbl_8087F0A8 + 0x48C))->fn_8012476C(0x22)) {
                mF84 -= 10;
            }
            if (mF84 <= 0) {
                if (mStatus.IsState_unk(0x100)) {
                    mStatus.fn_8013322C(0x100);
                    mStatus.fn_8013322C(0x100);
                    fn_80107850(lbl_8087F048, &mMotion);
                    Recover_unk(this);
                }
                if (mStatus.IsState_unk(0x10000)) {
                    mStatus.fn_8013322C(0x10000);
                    mStatus.fn_8013322C(0x10000);
                    fn_80107B20(lbl_8087F048, &mMotion);
                    Recover_unk(this);
                }
            }
        }
    }
    if (mFlagB7 && (mType == 0 || mType == 3)) {
        if (--m1398 <= 0) {
            m1398 = rand() % 60 + 60;
            if (ColliAttrManager::sInstance != 0) {
                ColliAttrManager::sInstance->fn_801847C0(this);
            }
        }
    }
    int emote = 0;
    if (ColliAttrManager::sInstance != 0 && (unsigned int)(m634 - 28) <= 1 && ((unsigned int)mStatus.mParamB.m9C & 0x200) != 0x200) {
        emote = 1;
    }
    if (mStatus.IsState_unk(0x20)) {
        emote = 0;
    }
    if (mFlagB28) {
        SetEmoteMtx_unk();
        if (!emote) {
            fn_80239DAC(lbl_8087F3C0, &mF20, 0, 0);
            mFlagB28 = 0;
        }
    } else if (emote) {
        SetEmoteMtx_unk();
        Camera_unk* camera = lbl_8087EFB4;
        Model_unk::Sphere_unk sphere = mMotion.fn_80094F98();
        ScreenInfo_unk* info = fn_800C122C(camera, &sphere);
        float color[4];
        color[0] = info->mColor[0];
        color[1] = info->mColor[1];
        color[2] = info->mColor[2];
        color[3] = info->mColor[3];
        void* res = ColliAttrManager::sInstance->fn_801840D8(m634, 0);
        if (res != 0) {
            fn_80232B7C(&mF20, 0);
            fn_8023A680(lbl_8087F3C0, res, -1, 5, &mF20, 0, 0, color, 1.0f, 0, -1, 1);
            mFlagB28 = 1;
        }
    }
    if (mFlag31 && !IsLedgeLocked_unk()) {
        if (++mC4C > 30) {
            mStatus.mParamB.m8C += mStatus.fn_80134168(0x38, -1) / 100.0f;
            if (lbl_8087F610 != 0 && IsArenaMode_unk(lbl_8087F610->m540)) {
                mStatus.mParamB.m8C += (lbl_8087F610 != 0 && lbl_8087F610->m540 == 1) ? 0.1f : 0.15f;
            }
        }
    } else {
        mC4C = 0;
    }
    if (mFE0 > 0) {
        if (!mFlagB31) {
            mFE0--;
        }
    } else {
        mFDC = 0;
        mFE0 = 0;
        mFE4 = 0;
    }
    if (lbl_8087F048 != 0) {
        int hide = 0;
        if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8 && lbl_8087F540 != 0 && fn_80481654(lbl_8087F540) != 0) {
            hide = 1;
        }
        fn_8010C00C(lbl_8087F048, this);
        if (mFlagB18) {
            if (!mStatus.IsState_unk(0x100000) || hide) {
                mFlagB18 = 0;
                fn_80107CA8(lbl_8087F048, this, 1);
            }
        } else if (mStatus.IsState_unk(0x100000) && !hide) {
            mFlagB18 = 1;
            fn_80107BE8(lbl_8087F048, this, &mMotion);
        }
    }
    if (mType == 0 && FindHitBullet_unk() != 0) {
        HitInfo_unk* bullet = FindHitBullet_unk();
        if (bullet != 0 && lbl_8087F048 != 0) {
            Model_unk::Sphere_unk sphere;
            sphere.mCenter = mPos;
            sphere.mRadius = bullet->m58;
            HitInfo_unk* data;
            int id = bullet->m70 + 100000;
            int unused[4] = {0, 0, 0, 0}; // inferred: cleared but not used
            data = fn_80219E6C(bullet->m70);
            CharaTask* chara;
            for (chara = (lbl_8087F8A0 != 0) ? lbl_8087F8A0->mHead : 0; chara != 0; chara = NextChara_unk(chara)) {
                if (chara->CanAct_unk() && chara->fn_8015EB2C() && !chara->IsGrappled_unk() && chara->mFlag22 != 1 &&
                    !IsSameSide_unk(chara) && !chara->fn_801533C8(id, data) && fn_80051A88(&sphere, &chara->m614)) {
                    fn_800FC410(lbl_8087F048, this, chara, 0, data, id, 30);
                }
            }
            for (chara = (EnemyManager::sInstance != 0) ? (CharaTask*)EnemyManager::sInstance->mHead : 0; chara != 0;
                 chara = NextChara_unk(chara)) {
                if (chara->CanAct_unk() && chara->fn_8015EB2C() && !chara->IsGrappled_unk() && chara->mFlag22 != 1 &&
                    !IsSameSide_unk(chara) && !chara->fn_801533C8(id, data) && fn_80051A88(&sphere, &chara->m614)) {
                    fn_800FC410(lbl_8087F048, this, chara, 0, data, id, 30);
                }
            }
        }
    }
    if (mFlag28 && 0.0f != lbl_8087F0A8->m414) {
        mStatus.fn_8012F188(lbl_8087F0A8->m414 / 30.0f);
    }
}

void CharaTask::fn_8014C468() {
    if (m648 == 0) {
        fn_8014C0B4(0, m510);
        return;
    }
    switch (m648->m04) {
    case 0:
        if (m5C->m11C == 2) {
            fn_8014C0B4(0, 4);
        } else if (fn_80219558(m50) == 0) {
            fn_8014C0B4(0, 5);
        } else {
            fn_8014C0B4(0, 3);
        }
        break;
    case 1:
        fn_8014C0B4(0, 0);
        break;
    case 2:
        fn_8014C0B4(0, 1);
        break;
    }
}

extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix

// inferred: *this = *this * trans(v) (name invented)
inline void Translate_unk(atn::Matrix3x4& mtx, const atn::Vector3& v) {
    atn::Matrix3x4 t;
    atn::Matrix3x4 r;
    PSMTXTrans(t.m, v.x, v.y, v.z);
    PSMTXConcat(mtx.m, t.m, r.m);
    mtx = r;
}

// NONMATCHING: ~16% by line in fn_8014EF48 (219 vs 219 instructions, identical except registers;
// the diff tool misaligns the psq lines) - in the three matrix
// copies the original swaps r3/r4 between source and destination (identity copies: source r3,
// destination r4; final copy: source r4, destination r3), plus @stringBase0 offsets until the
// earlier strings exist; tried: operator=, __MtxCopy, named reference/pointer to the destination,
// local CharaPart_unk*, accessor for the identity, inline member setter, copy helper with
// (src, dst) order, Mtx/Matrix3x4 temporaries.
// inferred: attaches the weapons to the hand nodes (body of fn_8014EF48, inlined in fn_8014DEE4;
// static so that its literals stay in the file pool); name invented
static inline void AttachWeapons_unk(CharaTask* chara) {
    if (chara->m648 == 0) {
        return;
    }
    chara->m648->m224 = chara->mMotion.GetNodeMtx_unk("rightwepoint");
    chara->m648->m228 = lbl_807C7060;
    if (chara->m64C == 0) {
        return;
    }
    if (chara->mFlagB5) {
        chara->m64C->m224 = chara->mMotion.GetNodeMtx_unk("leftwepoint");
        chara->m64C->m228 = lbl_807C7060;
    } else {
        chara->m64C->m224 = chara->mMotion.GetNodeMtx_unk("leftwepoint");
        atn::Matrix3x4 mtx;
        atn::Matrix3x4* p = &mtx;
        const atn::Vector3& rot = atn::Vector3(3.1415927f, 3.1415927f, 0.0f);
        p->SetIdentity();
        if (0.0f != rot.z) {
            Rotate_unk(*p, 'z', rot.z);
        }
        if (0.0f != rot.y) {
            Rotate_unk(*p, 'y', rot.y);
        }
        if (0.0f != rot.x) {
            Rotate_unk(*p, 'x', rot.x);
        }
        Translate_unk(mtx, atn::Vector3(0.0f, 0.6f, 0.0f));
        chara->m64C->m228 = mtx;
    }
}

extern "C" atn::Task* lbl_8087F9E8; // inferred: manager notified when the weapon changes (flags read as a task)
extern "C" void fn_8059C084(void* mgr, CharaTask* chara);

// NONMATCHING: ~90% (102/985 lines) - everything matches except the r3/r4 swap in the matrix copies
// of the inlined AttachWeapons_unk (see there) and the @stringBase0 offsets.
// inferred: selects the weapon part `index` and its motion set (keep: only the part; noMotion:
// keep the motion ids)
void CharaTask::fn_8014DEE4(int index, int keep, int noMotion) {
    if (mParts.mCount != 0 && index < (int)mParts.mCount) {
    if (lbl_8087F610 != 0 && m5C->m11C == 2 && mParts.mCount > 2) {
        if ((index == 1 && mParts[1]->m04 == 0) || (index == 0 && mParts[2]->m04 == 0)) {
            chr::CharaPart_unk* part = mParts[1];
            mParts[1] = mParts[2];
            mParts[2] = part;
            m64C = index == 0 ? mParts[1] : 0;
        }
    }
    if (m648 != 0) {
        fn_80044E0C(m648, 1);
    }
    if (m64C != 0) {
        fn_80044E0C(m64C, 1);
    }
    m674 = index;
    m648 = mParts[index];
    if (index == 0 && m678 != 0 && m678->vf0C() == 0) {
        m648 = m678;
    }
    if (keep != 0) {
        return;
    }
    CharaTask* other = mC58.mC4;
    if (other != 0 && other->HasLink_unk(this)) {
        other->RemoveLink_unk(this);
    }
    if (m648->m04 == 0) {
        fn_8014EE48();
    }
    mStatus.fn_8012B988();
    if (lbl_8087F9E8 != 0) {
        fn_8059C084(lbl_8087F9E8, this);
    }
    mStatus.fn_8013407C();
    if (lbl_8087F048 != 0) {
        fn_8010C00C(lbl_8087F048, this);
    }
    if (noMotion == 0) {
        switch (m648->m04) {
        case 0:
            if (m5C->m11C == 2) {
                fn_8014C0B4(0, 0x4);
                fn_8014C0B4(1, 0x16);
                fn_8014C0B4(2, 0x1C);
                fn_8014C0B4(3, 0x22);
                fn_8014C0B4(4, 0x2A);
                fn_8014C0B4(5, 0x19D);
                fn_8014C0B4(6, 0x19E);
                fn_8014C0B4(7, 0x1BA);
                fn_8014C0B4(8, 0x1BB);
                fn_8014C0B4(9, 0x19F);
                fn_8014C0B4(10, 0x1A0);
                fn_8014C0B4(11, 0x1BC);
                fn_8014C0B4(12, 0x1BD);
                fn_8014C0B4(13, 0x1A1);
                fn_8014C0B4(14, 0x1A2);
                fn_8014C0B4(15, 0x1BE);
                fn_8014C0B4(16, 0x1BF);
                fn_8014C0B4(17, 0x1A3);
                fn_8014C0B4(18, 0x1A4);
                fn_8014C0B4(19, 0x16A);
                fn_8014C0B4(20, 0x16A);
                fn_8014C0B4(22, 0x15C);
                fn_8014C0B4(21, 0x15D);
                fn_8014C0B4(23, 0x15E);
                fn_8014C0B4(24, 0x15F);
            } else {
                if (fn_80219558(m50) == 0) {
                    fn_8014C0B4(0, 5);
                } else {
                    fn_8014C0B4(0, 3);
                }
                fn_8014C0B4(1, 0x15);
                fn_8014C0B4(2, 0x1B);
                fn_8014C0B4(3, 0x21);
                fn_8014C0B4(4, 0x29);
                fn_8014C0B4(5, 0x195);
                fn_8014C0B4(6, 0x196);
                fn_8014C0B4(7, 0x1B1);
                fn_8014C0B4(8, 0x1B2);
                fn_8014C0B4(9, 0x197);
                fn_8014C0B4(10, 0x198);
                fn_8014C0B4(11, 0x1B3);
                fn_8014C0B4(12, 0x1B4);
                fn_8014C0B4(13, 0x199);
                fn_8014C0B4(14, 0x19A);
                fn_8014C0B4(15, 0x1B5);
                fn_8014C0B4(16, 0x1B6);
                fn_8014C0B4(17, 0x19B);
                fn_8014C0B4(18, 0x19C);
                if (m648->m274->mParam.m7C == 1) {
                    fn_8014C0B4(19, 0x165);
                    fn_8014C0B4(20, 0x165);
                    fn_8014C0B4(22, 0x166);
                    fn_8014C0B4(21, 0x167);
                    fn_8014C0B4(23, 0x168);
                    fn_8014C0B4(24, 0x169);
                } else {
                    fn_8014C0B4(19, 0x156);
                    if (mMotion.fn_80097D40(0x157)) {
                        fn_8014C0B4(20, 0x157);
                    } else {
                        fn_8014C0B4(20, 0x156);
                    }
                    fn_8014C0B4(22, 0x15C);
                    fn_8014C0B4(21, 0x15D);
                    fn_8014C0B4(23, 0x15E);
                    fn_8014C0B4(24, 0x15F);
                }
            }
            break;
        case 1:
            fn_8014C0B4(0, 0x0);
            fn_8014C0B4(1, 0x12);
            fn_8014C0B4(2, 0x18);
            fn_8014C0B4(3, 0x1E);
            int kind = fn_80219558(m50);
            int motion = 0x25;
            if (kind == 0) {
                motion = 0x26;
            }
            fn_8014C0B4(4, motion);
            fn_8014C0B4(5, 0x185);
            fn_8014C0B4(6, 0x186);
            fn_8014C0B4(7, 0x1A5);
            fn_8014C0B4(8, 0x1A6);
            fn_8014C0B4(9, 0x187);
            fn_8014C0B4(10, 0x188);
            fn_8014C0B4(11, 0x1A7);
            fn_8014C0B4(12, 0x1A8);
            fn_8014C0B4(13, 0x189);
            fn_8014C0B4(14, 0x18A);
            fn_8014C0B4(15, 0x1A9);
            fn_8014C0B4(16, 0x1AA);
            fn_8014C0B4(17, 0x18B);
            fn_8014C0B4(18, 0x18C);
            fn_8014C0B4(19, 0x156);
            fn_8014C0B4(20, 0x156);
            fn_8014C0B4(22, 0x15C);
            fn_8014C0B4(21, 0x15D);
            fn_8014C0B4(23, 0x15E);
            fn_8014C0B4(24, 0x15F);
            break;
        case 2:
            fn_8014C0B4(0, 0x1);
            fn_8014C0B4(1, 0x13);
            fn_8014C0B4(2, 0x19);
            fn_8014C0B4(3, 0x1F);
            fn_8014C0B4(4, 0x27);
            fn_8014C0B4(5, 0x18D);
            fn_8014C0B4(6, 0x18E);
            fn_8014C0B4(7, 0x1AB);
            fn_8014C0B4(8, 0x1AC);
            fn_8014C0B4(9, 0x18F);
            fn_8014C0B4(10, 0x190);
            fn_8014C0B4(11, 0x1AD);
            fn_8014C0B4(12, 0x1AE);
            fn_8014C0B4(13, 0x191);
            fn_8014C0B4(14, 0x192);
            fn_8014C0B4(15, 0x1AF);
            fn_8014C0B4(16, 0x1B0);
            fn_8014C0B4(17, 0x193);
            fn_8014C0B4(18, 0x194);
            fn_8014C0B4(19, 0x156);
            fn_8014C0B4(20, 0x156);
            fn_8014C0B4(22, 0x15C);
            fn_8014C0B4(21, 0x15D);
            fn_8014C0B4(23, 0x15E);
            fn_8014C0B4(24, 0x15F);
            break;
        }
        if (m5C->m11C == 2) {
            fn_8014C0B4(25, 0x1C4);
            fn_8014C0B4(26, 0x1C5);
            fn_8014C0B4(27, 0x1CE);
            fn_8014C0B4(28, 0x1CF);
            fn_8014C0B4(29, 0x1D0);
            fn_8014C0B4(30, 0x1D1);
        } else if (m680[4] != 0) {
            fn_8014C0B4(25, 0x1C2);
            fn_8014C0B4(26, 0x1C3);
            fn_8014C0B4(27, 0x1CA);
            fn_8014C0B4(28, 0x1CB);
            fn_8014C0B4(29, 0x1CC);
            fn_8014C0B4(30, 0x1CD);
        } else if (m648->m274->mParam.m7C == 1) {
            fn_8014C0B4(25, 0x1C4);
            fn_8014C0B4(26, 0x1C5);
            fn_8014C0B4(27, 0x1CE);
            fn_8014C0B4(28, 0x1CB);
            fn_8014C0B4(29, 0x1D0);
            fn_8014C0B4(30, 0x1D1);
        } else if (m50 == 100105) {
            fn_8014C0B4(25, 0x2);
            fn_8014C0B4(26, 0x33);
            fn_8014C0B4(27, 0x14);
            fn_8014C0B4(28, 0x14);
            fn_8014C0B4(29, 0x14);
            fn_8014C0B4(30, 0x14);
        } else {
            fn_8014C0B4(25, 0x1C0);
            fn_8014C0B4(26, 0x1C1);
            fn_8014C0B4(27, 0x1C6);
            fn_8014C0B4(28, 0x1C7);
            fn_8014C0B4(29, 0x1C8);
            fn_8014C0B4(30, 0x1C9);
        }
        fn_8014FD30();
    }
    AttachWeapons_unk(this);
    }
}

void CharaTask::fn_8014EE48() {
    if (m5C->m11C == 2 && mParts.mCount >= 2) {
        m64C = mParts.mData[1];
        if (m67C != 0 && m67C->vf0C() == 0) {
            m64C = m67C;
        }
    }
}

void CharaTask::fn_8014EEC4(int keep) {
    if (m648 != 0) {
        fn_80044E0C(m648, 1);
    }
    if (m64C != 0) {
        fn_80044E0C(m64C, 1);
    }
    m674 = -1;
    m648 = 0;
    m64C = 0;
    if (keep == 0) {
        fn_8014C228();
    }
}

// NONMATCHING: see AttachWeapons_unk (its body is also inlined in fn_8014DEE4)
void CharaTask::fn_8014EF48() {
    AttachWeapons_unk(this);
}

extern "C" int fn_804EB750(Arena_unk* arena, CharaTask* chara, int index); // inferred: part id in the arena
extern "C" void* fn_80206BE4(void* item);
extern "C" void* fn_80211480(void* data);
extern "C" void fn_80211940(void* data, int arg);
extern "C" int fn_8044D3A0(Party_unk* party, int index, int id); // inferred: part id of a party member
extern "C" void* fn_80206B14(int arg, int item);
// inferred: creates a part (returns 0 on failure)
extern "C" chr::CharaPart_unk* fn_800457A4(int kind, int id, int arg, CharaTask* chara, int arg2);

void CharaTask::fn_8014F2B4(int a, int b, int index) {
    if (lbl_8087F610 != 0) {
        int kind = fn_80219558(m50);
        if (kind >= 0 && 7 > kind && mParts.mCount < 2 && a == 0) {
            int id = fn_804EB750(lbl_8087F610, this, index);
            if (id >= 0) {
                if (id >= 1000 && fn_80206B14(0, id) == 0) {
                    fn_80211940(fn_80211480(fn_80206BE4(fn_80206B14(0, id % 1000))), id / 1000);
                }
                b = id;
            }
        }
    } else if (lbl_8087F4F0 != 0 && index >= 0) {
        int id = fn_8044D3A0(lbl_8087F4F0, index, m50);
        if (id >= 0) {
            b = id;
        }
    }
    chr::CharaPart_unk* part = fn_800457A4(a, b, 0, this, 0);
    if (part != 0) {
        if (index >= 0) {
            mParts.insert(&mParts.mData[index], part);
        } else {
            mParts.PushBack_unk(part);
        }
    }
}

// NONMATCHING: 90.1% - the original keeps the index in r31 and recomputes &mData[i] for the
// erase ((i * 4) / 4); ours keeps the element address (lwzu) from the delete.
void CharaTask::fn_8014F4FC(int a, int b) {
    for (unsigned int i = 0; i < mParts.mCount; i++) {
        if (a == mParts.mData[i]->m04 && b == mParts.mData[i]->m08) {
            delete mParts.mData[i];
            mParts.erase(&mParts.mData[i]);
            return;
        }
    }
}

// NONMATCHING: 78.0% - same as fn_8014F4FC: the original does not keep &mData[index] across the
// delete (saves this, index, a, b in r28-r31); tried operator[], begin() + index, a local pointer.
void CharaTask::fn_8014F5D4(int index, int a, int b) {
    delete mParts.mData[index];
    mParts.erase(&mParts.mData[index]);
    fn_8014F2B4(a, b, index);
}

void CharaTask::fn_8014F698(int a, int b, int notify) {
    if (m678 != 0) {
        delete m678;
        m678 = 0;
        if (m67C != 0) {
            delete m67C;
            m67C = 0;
        }
        if (m674 == 0) {
            fn_8014DEE4(0, 0, 0);
        }
    }
    chr::CharaPart_unk* part = fn_800457A4(a, b, 0, this, 0);
    m678 = part;
    part->m288 = 5;
    part->m28C = 1;
    if (m5C->m11C == 2) {
        part = fn_800457A4(a, b, 0, this, 0);
        m67C = part;
        part->m288 = 5;
        part->m28C = 1;
    }
    mFlagB21 = 1;
    if (notify != 0) {
        fn_8014F8F4()->fn_8014F860(9, this, &b);
    }
}

// NONMATCHING: the original loads mEnabled before saving the arguments (4 instructions moved).
void CharaListeners_unk::fn_8014F860(int id, CharaTask* chara, void* arg) {
    int i;
    if (mEnabled != 0) {
        for (i = 0; i < (int)mList.size(); i++) {
            CharaListener_unk& listener = mList[i];
            if (listener.mId == -1 || id == listener.mId) {
                listener.mFunc(id, chara, arg);
            }
        }
    }
}

CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}

void CharaTask::fn_8014F960(int notify) {
    if (m678 != 0) {
        delete m678;
        m678 = 0;
        if (m67C != 0) {
            delete m67C;
            m67C = 0;
        }
        if (m674 == 0) {
            fn_8014DEE4(0, 0, 0);
        }
        if (notify != 0) {
            fn_8014F8F4()->fn_8014F860(9, this, 0);
        }
    }
}

bool CharaTask::fn_8014FAD0() {
    bool result = false;
    for (unsigned int i = 0; i < mParts.mCount; i++) {
        if (mParts.mData[i] != 0 && mParts.mData[i]->vf0C()) {
            result = true;
        }
    }
    return result;
}

// inferred: whether the weapon parts can be shown (0 while a shield replaces them, during grabs...)
int CharaTask::fn_8014FB60() {
    if (lbl_8087F610 != 0 && m674 == 0) {
        if (m678 != 0 && (m678->vf0C() != 0 || m678 != m648)) {
            return 0;
        }
        if (m67C != 0 && (m67C->vf0C() != 0 || m67C != m64C)) {
            return 0;
        }
    }
    if (m648 != 0 && !mFlag3 && m1208 == 0 && mF54 == 0 && m139C == 0 && m13FC == 0 &&
        (m55C != 6 || (m560 != 58 && m560 != 84 && m560 != 121 && m560 != 138))) {
        if (mFlag2) {
            return (unsigned int)(mMotion.mTracks[0].m00 - 108) > 4;
        }
        return 1;
    }
    return 0;
}

bool CharaTask::fn_8014FCC4() {
    bool result = false;
    if (m648 != 0 && m648->m04 == 0) {
        result = true;
    }
    return result;
}

bool CharaTask::fn_8014FCE8() {
    bool result = false;
    if (m648 != 0 && m648->m04 == 1) {
        result = true;
    }
    return result;
}

bool CharaTask::fn_8014FD0C() {
    bool result = false;
    if (m648 != 0 && m648->m04 == 2) {
        result = true;
    }
    return result;
}

void CharaTask::fn_8014FD30() {
    if (mAC == 3) {
        if (m680[0] != 0 && m680[0]->m004 % 1000 / 10 == 46) {
            mMotion.fn_80093F1C("leftcoat1", lbl_807C7030, lbl_807C7030, atn::Vector3(1.0f, 1.0f, 1.0f), 8);
            mMotion.fn_80093F1C("leftcoat2", lbl_807C7030, lbl_807C7030, atn::Vector3(1.0f, 1.0f, 1.0f), 8);
            mMotion.fn_80093F1C("rightcoat1", lbl_807C7030, lbl_807C7030, atn::Vector3(1.0f, 1.0f, 1.0f), 8);
            mMotion.fn_80093F1C("rightcoat2", lbl_807C7030, lbl_807C7030, atn::Vector3(1.0f, 1.0f, 1.0f), 8);
            if (m484[0] == 2) {
                fn_8014C0B4(0, 6);
            }
            if (m484[1] == 20) {
                fn_8014C0B4(1, 23);
            }
            if (m484[2] == 26) {
                fn_8014C0B4(2, 29);
            }
            if (m484[3] == 32) {
                fn_8014C0B4(3, 36);
            }
        } else {
            mMotion.fn_80094778("leftcoat1");
            mMotion.fn_80094778("leftcoat2");
            mMotion.fn_80094778("rightcoat1");
            mMotion.fn_80094778("rightcoat2");
            if (m484[0] == 6) {
                fn_8014C0B4(0, 2);
            }
            if (m484[1] == 23) {
                fn_8014C0B4(1, 20);
            }
            if (m484[2] == 29) {
                fn_8014C0B4(2, 26);
            }
            if (m484[3] == 36) {
                fn_8014C0B4(3, 32);
            }
        }
    }
}

// inferred: equipment of an arena character (returned by fn_804EB650); names invented
struct ArenaEquip_unk {
    int mId;
    unsigned short mHasData : 1; // inferred: m4C is passed to the equipment
    unsigned short mFlag : 1;    // inferred: passed to the equipment instead of the caller's flag
    unsigned short m04_rest : 14;
    unsigned char m06[0x4C - 0x06];
    unsigned char m4C[4]; // inferred: data passed to chr::CharaEquip_unk (size unknown)
};
extern "C" ArenaEquip_unk* fn_804EB650(Arena_unk* arena, CharaTask* chara, int slot);
extern "C" void* fn_8020ED84(int slot, int item);
extern "C" void* fn_8020EF80(void* item);
extern "C" int fn_8044D1A4(Party_unk* party, int slot, int id); // inferred: equipment index of a party member
extern "C" void* fn_8044D034(Party_unk* party, int index);      // inferred: equipment data

// NONMATCHING: 99.2% - the original keeps the dead second null check of the delete (`beq` twice);
// tried a local pointer, a reference, a pointer to the slot, an inline delete helper.
void CharaTask::fn_8014FF9C(int slot, int id, int arg) {
    if (m680[slot] != 0) {
        delete m680[slot];
        m680[slot] = 0;
    }
    void* data = 0;
    int index = 0;
    if (slot < 4) {
        if (lbl_8087F610 != 0) {
            int kind = fn_80219558(m50);
            if (kind >= 0 && 7 > kind && fn_804EB650(lbl_8087F610, this, slot) != 0) {
                ArenaEquip_unk* equip = fn_804EB650(lbl_8087F610, this, slot);
                id = equip->mId;
                if (id >= 1000 && fn_8020ED84(slot, id) == 0) {
                    fn_80211940(fn_80211480(fn_8020EF80(fn_8020ED84(slot, id % 1000))), id / 1000);
                }
                index = 0;
                if (equip->mHasData) {
                    data = equip->m4C;
                    arg = equip->mFlag;
                }
            }
        } else if (lbl_8087F4F0 != 0) {
            index = fn_8044D1A4(lbl_8087F4F0, slot, m50);
            if (index >= 0) {
                data = fn_8044D034(lbl_8087F4F0, index);
            }
        }
    }
    // inferred: the fifth argument of the constructor is a pointer (declared int)
    m680[slot] = new (0, "", "", 0) chr::CharaEquip_unk(slot, id, index, this, (int)data, 0, arg, 1, 1);
    if (slot == 0) {
        fn_8014FD30();
    }
}

void CharaTask::fn_80150178(int slot, chr::CharaEquip_unk* equip) {
    if (m680[slot] != 0) {
        delete m680[slot];
    }
    m680[slot] = equip;
    if (slot == 0) {
        fn_8014FD30();
    }
}

extern "C" int fn_804EB814(Arena_unk* arena, CharaTask* chara);

// inferred: name invented
inline int CharaTask::ControlVf30_unk() {
    return mControl ? mControl->vf30() : 0;
}

// NONMATCHING: ~92% - in the two inlined hit loops the original keeps the array pointer in r4
// (reusing the register of `other`) and the hit in r5, ours the other way round; the rest are the
// .data offsets of the scoped_ptr bool ptmf. tried: index/pointer/iterator loops, a free inline
// function, the ternary written inline. (The same loop inlined in fn_80151448 matches.)
void CharaTask::vf5C(CharaTask* other) {
    if (other->GetFlaggedHitBullet_unk() == 0 && other != 0) {
        if (other->ControlVf30_unk() != 0) {
            return;
        }
    }
    bool combo = false;
    if (mFlagB31) {
        combo = true;
    } else {
        if (other != mFDC) {
            if (lbl_8087F610 == 0) {
                if ((other != 0 && other->mType == 0) || (mFDC != 0 && mFDC->mType == 0)) {
                    combo = true;
                }
            } else if (fn_804EB814(lbl_8087F610, other) == 0 && fn_804EB814(lbl_8087F610, mFDC) == 0 && other != 0 &&
                       other->mF14 > 0 && mFDC != 0 && mFDC->mF14 > 0 && other->mF14 == mFDC->mF14) {
                combo = true;
            }
        }
        if (other->GetFlaggedHitBullet_unk() != 0) {
            combo = true;
        }
    }
    if (combo) {
        fn_80150460(other, mFE4 + 1);
    } else {
        fn_80150460(other, 1);
    }
}

// inferred: parameter of a part (returned by fn_80045694); names invented
struct PartParam_unk {
    int m00;
    int m04; // inferred: value (x30 frames of combo time)
};
extern "C" PartParam_unk* fn_80045694(chr::CharaPart_unk* part, int kind, int arg, int id, int sub);

// inferred: name invented
inline bool HasPartParam_unk(chr::CharaPart_unk* part, int kind, int arg, int id, int sub) {
    return fn_80045694(part, kind, arg, id, sub) != 0;
}

// inferred: starts the combo window against `other` (mFE0: frames, mFDC: last opponent?)
// NONMATCHING: 91% - same r4/r5 swap as vf5C in the two inlined GetFlaggedHitBullet_unk loops;
// tried index, pointer, iterator and break forms of the loop.
void CharaTask::fn_80150460(CharaTask* other, int combo) {
    CharaTask* prev = mFDC;
    mFE4 = combo;
    mFDC = other;
    mFE0 = 60;
    if (other != 0) {
        CharaStatus* status = &other->mStatus;
        if (status->fn_80133EE8(10) != 0) {
            mFE0 += status->fn_80133F18(10, -1) * 30;
        } else if (prev != 0 && prev->m648 != 0 && HasPartParam_unk(prev->m648, 8, 0, 10, -1)) {
            mFE0 += fn_80045694(prev->m648, 8, 0, 10, -1)->m04 * 30;
        }
        if (other->GetFlaggedHitBullet_unk() != 0 || (prev != 0 && prev->GetFlaggedHitBullet_unk() != 0)) {
            mFE0 += 150;
        }
    }
}

// inferred: type pairs that count as the same side (names invented)
inline bool IsSideType_unk(int a, int b) {
    bool result = false;
    if (a == 0 && b == 3) {
        result = true;
    }
    return result;
}
// inferred: same test as IsSideType_unk(b, a), comparing `a` first
inline bool IsSideTypeRev_unk(int a, int b) {
    bool result = false;
    if (a == 3 && b == 0) {
        result = true;
    }
    return result;
}
inline bool IsSameSideType_unk(int a, int b) {
    bool result = true;
    if (a != b && !IsSideType_unk(a, b)) {
        result = false;
    }
    return result;
}
// inferred: whether a and b are on the same side (team ids in mF14, else the types); same test as
// IsSameSideOf_unk and CharaTask::IsSameSide_unk, written differently (each form matches its callers)
inline bool IsSameSidePair_unk(CharaTask* a, CharaTask* b) {
    if (a != b) {
        if (lbl_8087F610 != 0 && lbl_8087F610->m540 == 0) {
            return false;
        }
        if ((a->mStatus.m0C & 2) == 2 || (b->mStatus.m0C & 2) == 2 || lbl_8087F0A8->mCC != 0) {
            return false;
        }
    }
    if (a->mF14 >= 0 && b->mF14 >= 0) {
        return a->mF14 == b->mF14;
    }
    bool result = true;
    if (!IsSameSideType_unk(a->mType, b->mType) && !IsSideType_unk(b->mType, a->mType)) {
        result = false;
    }
    return result;
}
// inferred: same test again, with the types read first (the hit functions 0x80178668 and
// 0x80178BFC); name invented
inline bool IsSameSideHit_unk(CharaTask* a, CharaTask* b) {
    if (a != b) {
        if (lbl_8087F610 != 0 && lbl_8087F610->m540 == 0) {
            return false;
        }
        if ((a->mStatus.m0C & 2) == 2 || (b->mStatus.m0C & 2) == 2 || lbl_8087F0A8->mCC != 0) {
            return false;
        }
    }
    if (a->mF14 >= 0 && b->mF14 >= 0) {
        return a->mF14 == b->mF14;
    }
    int ta = a->mType;
    int tb = b->mType;
    bool result = true;
    if (!IsSameSideType_unk(ta, tb) && !IsSideTypeRev_unk(ta, tb)) {
        result = false;
    }
    return result;
}

// inferred: whether `other` continues the combo of mFDC (same side)
// NONMATCHING: 88% - register allocation of the type checks at the end (the original gives
// b->mType r3, a->mType r6 and the inner bool r5); tried locals, nested ifs, a second pair helper.
int CharaTask::fn_8015061C(CharaTask* other) {
    int result = 0;
    if (mFlagB31) {
        result = 1;
    } else if (lbl_8087F610 == 0) {
        if (mFDC != 0 && mFDC->mType == 0) {
            result = 1;
        }
    } else if (mFDC != 0 && mFDC != other && IsSameSidePair_unk(mFDC, other)) {
        result = 1;
    }
    return result;
}

// inferred: 0x1C-byte record at 0x1470 in the player characters (names invented)
struct PlayerState_unk {
    PlayerState_unk(short id, int value)
        : m00(id), m02(0), m04(0), m0C(0.0f, 0.0f, 0.0f), m18(0) {
        m08 = value;
    }
    // inferred: weak copy 0x80151194
    PlayerState_unk(short id) : m00(id), m02(0), m04(0), m08(150), m0C(0.0f, 0.0f, 0.0f), m18(0) {}
    short m00;
    short m02;
    int m04;
    int m08;
    atn::Vector3 m0C;
    int m18;
};

// inferred: player character as seen from here (PlayerTask: BattleCharaTask: CharaTask)
struct Player_unk : public CharaTask {
    // inferred: weak copies 0x800F7FB4 and 0x801511C4 (names invented)
    int GetM146C_unk() { return m146C; }
    void SetState_unk(const PlayerState_unk& state) { m1470 = state; }

    unsigned char m1424[0x146C - 0x1424];
    int m146C;
    PlayerState_unk m1470;
    unsigned char m148C[0x14AC - 0x148C];
    Player_unk* mNext; // 0x14AC: inferred: next player in the manager list
};


// inferred: data passed to the listeners of event 14 by fn_8015076C (names invented)
struct HitEvent_unk {
    // inferred: implicit constructor (weak copy 0x80151218)
    int mId;
    atn::Vector3 mDir;
    atn::Vector3 m10;
    int m1C;
    unsigned int mFlags;
};

// inferred: (mask & flags) == mask on a flag word (weak copy 0x80151168); name invented
inline bool HasFlags_unk(const int& flags, unsigned int mask) {
    return (mask & flags) == mask;
}

// inferred: object behind lbl_8087F490 (weak copies 0x80151204 and 0x80151210); names invented
struct Unk8087F490_unk;
extern "C" Unk8087F490_unk* lbl_8087F490;
extern "C" void fn_803E2110(Unk8087F490_unk* obj, int arg, CharaTask* chara);
struct Unk8087F490_unk {
    static Unk8087F490_unk* Get_unk() { return lbl_8087F490; }
    void Remove_unk(CharaTask* chara) { fn_803E2110(this, 1, chara); }
};



// inferred: reaction to a hit (knock back, down, guard...) and record of the hit in m6D0
void CharaTask::fn_8015076C(int id, const atn::Vector3& dir, const atn::Vector3& v2, int arg, unsigned int flags,
                            HitMsg_unk* msg) {
    HitEvent_unk ev;
    ev.mId = id;
    ev.mDir = dir;
    ev.m10 = v2;
    ev.m1C = arg;
    ev.mFlags = flags;
    if (msg != 0 && msg->mFlag31) {
        ev.mDir = msg->m28 * 5.0f;
        ev.mFlags |= 0x10000;
    }
    fn_8014F8F4()->fn_8014F860(14, this, &ev);
    if (fn_80139EBC()) {
        fn_80175A38();
    }
    if (m6D0.full()) {
        return;
    }
    if (fn_80148334() && (flags & 0x10000000) != 0x10000000) {
        m6D0.PushBack_unk(CharaPair8_unk(id, arg));
        return;
    }
    float lenSq = dir.LengthSq_unk();
    bool magic = fn_80160324();
    if (m55C == 6) {
        switch (m560) {
        case 35:
        case 36:
        case 46:
        case 52:
        case 53:
        case 121:
        case 132:
        case 138:
        case 142:
            lenSq = 0.0f;
            break;
        }
    }
    if ((flags & 8) != 8 && m55C == 6 && m560 == 30 && m638 != 0 && (m638->m01 == 1 || m638->m01 == 4)) {
        lenSq = 0.0f;
    }
    if (fn_8017B16C() && m55C == 6 && (m560 == 29 || m560 == 31) && m674 >= 0) {
        fn_8014DEE4(0, 0, 0);
    }
    if (fn_80370174(fn_80121F00(), 0x67) && IsPlayer_unk() && !mStatus.fn_80139F4C()) {
        CollResult_unk result;
        atn::Vector3 from = mPos + atn::Vector3(0.0f, 5.0f, 0.0f);
        atn::Vector3 to = (mPos + atn::Vector3(0.0f, 5.0f, 0.0f)) + dir * 8.0f;
        if (fn_8004ED34(fn_801404F8(), &result, &from, &to, 0x80000000, 0, 0) && (result.m3C & 4U) == 4U) {
            result.m04 -= atn::Vector3(0.0f, 5.0f, 0.0f);
            fn_8016DA4C();
            if (m1208 != 0) {
                fn_801750FC();
            }
            fn_8015CC70(atn::Vector3(mPos), atn::Vector3(result.m04), atn::Vector3(result.m28));
            m6D0.PushBack_unk(CharaPair8_unk(id, arg));
            return;
        }
    }
    bool reset = m1208 != 0;
    if (m58C != 3 && m58C != 4) {
        float limit = fn_8013A158() ? 5.0f : 7.0f;
        if ((m137C & 2) == 2) {
            atn::Vector3 rot(mRot);
            CollResult_unk result;
            atn::Vector3 from = mPos + fn_8014052C() * 10.0f;
            atn::Vector3 to = mPos + fn_8014052C() * -10.0f;
            from.y -= 10.0f;
            to.y -= 10.0f;
            if (fn_8004ED34(fn_801404F8(), &result, &from, &to, 0x80000000, 0, 0)) {
                rot = result.m28.ToRotation_unk();
            }
            fn_8016DA4C();
            fn_801698E4(rot);
        } else if (msg != 0 && HasFlags_unk(msg->mData->m90, 0x4000) && mStatus.IsState_unk(0x4000)) {
            fn_801681B0(dir, 0, (msg->mData->mAC >> 9) & 1);
        } else if (msg != 0 && msg->mAttacker != 0 && msg->mAttacker->GetStatus_unk()->fn_80133EE8(24) != 0 &&
                   mStatus.IsState_unk(0x4000)) {
            fn_801681B0(dir, msg->mAttacker->GetStatus_unk()->fn_80133F18(24, -1) * 30, 0);
        } else if ((flags & 0x400) == 0x400 || (flags & 1) == 1) {
        } else if ((flags & 0x20) == 0x20) {
            if (!mFlag31 && !GetStatus_unk()->mParamB.fn_801404E0(0x10) && !(m55C == 6 && m560 == 23)) {
                fn_8015E1B8(dir);
            }
        } else if ((flags & 8) == 8) {
            if (!mFlag31 && !GetStatus_unk()->mParamB.fn_801404E0(0x10) && !(m55C == 6 && m560 == 23)) {
                if ((flags & 0x2000) == 0x2000) {
                    fn_8015DEC0((int)&dir); // inferred: the parameter is declared int (it receives &dir)
                } else {
                    float power = 0.1f;
                    if (msg != 0 && (msg->mData->mAC & 0x20000000) == 0x20000000) {
                        power = 0.6f;
                    }
                    fn_8015D8C0(power, dir, msg != 0 ? (int)msg->mAttacker : 0);
                }
            }
        } else if (msg != 0 && HasFlags_unk(msg->mData->m90, 0x20000) && mStatus.IsState_unk(0x20000)) {
            fn_80168824((msg->mData->mAC >> 9) & 1);
        } else if (lenSq > limit * limit || (flags & 0x10000) == 0x10000) {
            fn_8016DA4C();
            if (!(m55C == 6 && m560 == 23)) {
                fn_8015E7A0(dir, msg != 0 ? msg->m58 : -1, 0);
            }
            if (mC58.mC4 != 0) {
                mC58.mC4->fn_8016F5EC(this);
            }
        } else if (lenSq > 0.010000001f) {
            int knock = 1;
            if (m55C == 6) {
                switch (m560) {
                case 1:
                case 2:
                case 3:
                case 6:
                case 7:
                case 10:
                case 11:
                case 15:
                case 17:
                case 18:
                case 19:
                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 35:
                case 36:
                case 40:
                case 41:
                case 45:
                case 46:
                case 48:
                case 49:
                case 50:
                case 51:
                case 52:
                case 53:
                case 62:
                case 68:
                case 76:
                case 77:
                case 84:
                case 85:
                case 91:
                case 93:
                case 101:
                case 106:
                case 110:
                case 111:
                case 112:
                case 113:
                case 114:
                case 115:
                case 116:
                case 117:
                case 118:
                case 119:
                case 123:
                case 124:
                case 125:
                case 126:
                case 127:
                case 130:
                case 132:
                    knock = 0;
                    break;
                case 33:
                    if (m1208 != 0 && m1208->fn_8013A18C()) {
                        knock = 0;
                    }
                    break;
                case 14:
                    if (!mControl || !mControl->vf54()) {
                        knock = 0;
                    }
                    break;
                }
            }
            if (mFlag31 && IsPlayer_unk() && lenSq < 9.0f) {
                knock = 0;
            }
            if (knock) {
                fn_8016DA4C();
                fn_8015E4B0(dir, msg != 0 ? msg->m54 : -1);
                if (mC58.mC4 != 0) {
                    mC58.mC4->fn_8016F5EC(this);
                }
            } else {
                reset = false;
            }
        } else {
            reset = false;
        }
    }
    if (reset) {
        fn_801750FC();
    }
    if (magic && !fn_80160324()) {
        if (mStatus.fn_80134270() || mStatus.fn_80134290() || mStatus.fn_801342B0()) {
            if (mStatus.fn_80151188()) {
                PlayerState_unk state(2);
                state.m04 = rand() % 5;
                state.m08 = 60;
                if ((fn_80140518() || fn_8013A158()) && ((Player_unk*)this)->GetM146C_unk() <= 0) {
                    ((Player_unk*)this)->SetState_unk(state);
                }
            } else {
                mC58.m2B4 = 30;
            }
        }
        Unk8087F490_unk::Get_unk()->Remove_unk(this);
    }
    m6D0.PushBack_unk(CharaPair8_unk(id, arg));
}

// inferred: whether `chara` can be knocked back by a hit (name unknown)
int fn_80151270(CharaTask* chara) {
    if (chara == 0 || chara->mStatus.IsState_unk(0x20)) {
        return 0;
    }
    if ((chara->m54C & 0x1000000) == 0x1000000 || (chara->m137C & 2) == 2) {
        return 0;
    }
    if ((chara->m137C & 0x10000) == 0x10000) {
        return 0;
    }
    if (chara->mFlag3) {
        return 0;
    }
    switch (chara->m55C) {
    case 5:
        return 0;
    case 6:
        switch (chara->m560) {
        case 2:
            if (chara->mMotion.mTracks[0].m00 == 490) {
                return 0;
            }
            break;
        case 24:
        case 25:
        case 26:
        case 40:
        case 41:
        case 44:
            return 0;
        case 68:
            return 0;
        }
        break;
    case 7:
        if (chara->mType == 0) {
            return 0;
        }
        break;
    case 10:
        return 0;
    }
    if (chara->mStatus.IsState_unk(0x10) || chara->mStatus.IsState_unk(0x40) || chara->mStatus.IsState_unk(0x4000) ||
        chara->mStatus.IsState_unk(0x80)) {
        return 0;
    }
    return 1;
}

extern "C" int fn_8021AA48(const char* name); // inferred

void CharaTask::vf3C(HitInfo_unk* hit) {
    if (!IsFlag_unk(1) || fn_8021AA48(hit->m08) != 0) {
        fn_80151448((HitMsg_unk*)hit); // inferred: the parameter of vf3C is the hit message
        vf40(hit);
    }
}


// inferred: parameters of fn_8003EA3C (names invented)
struct HitParam_unk {
    HitParam_unk() : m00(1), m04(1.0f), m08(1.0f), m0C(0), m10(1.0f), m14(0) {}
    int m00; // inferred: combo count
    float m04;
    float m08;
    int m0C;
    float m10;
    int m14;
};
extern "C" HitParam_unk lbl_807C6B90; // inferred: default parameters
extern "C" void fn_8003EA3C(int* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner, unsigned int flags,
                            const HitParam_unk* param, int arg, int arg2); // inferred: computes the damage
extern "C" int fn_80041A28(int arg, CharaTask* chara, CharaTask* attacker);
extern "C" int fn_8010CD3C(Effect_unk* effect, int kind, CharaTask* chara, void* attacker);
extern "C" void fn_8010CF28(Effect_unk* effect, int kind, CharaTask* chara, void* attacker);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" double cos(double x);

// Same instructions as the target; only the .data offsets of the ptmf constants differ (lbl_8077A720)
// until the earlier .data users exist.
void CharaTask::fn_80151448(HitMsg_unk* msg) {
    unsigned int flags;
    unsigned int kinds = msg->m50;
    flags = msg->m0C;
    mFlag9 = 0;
    if ((kinds & 0x40) != 0x40 && mFlag10) {
        flags |= 0x40;
    }
    if ((kinds & 0x400) != 0x400 && m55C == 6 && (m560 == 1 || m560 == 2)) {
        flags |= 0x400;
    }
    if ((kinds & 0x1000) != 0x1000) {
        if (m55C == 6 && m560 == 96) {
            flags |= 0x1000;
        }
        if ((!mC58.m11C.fn_8011D21C() && (mC58.m11C.m20 & 2) == 2) || (mC58.m11C.m20 & 4) == 4) {
            flags |= 0x1000;
        }
    }
    if ((msg->mData->mAC & 4) == 4) {
        kinds |= 8;
    }
    if (msg->mAttacker != 0 && msg->mAttacker->mStatus.fn_80133EE8(0x48) != 0) {
        kinds |= 8;
    }
    if ((kinds & 8) != 8 && msg->m48 != 0) {
        flags |= 8;
    }
    if ((kinds & 8) == 8) {
        flags |= 0x400000;
    }
    if ((kinds & 0x20) != 0x20 && (msg->mData->mAC & 0x400) == 0x400 && mFlag31) {
        flags |= 0x20;
    }
    if ((kinds & 0x20000) != 0x20000 && mFlagB30) {
        flags |= 0x20000;
    }
    if (msg->m44 == 0) {
        atn::Vector3 dir(0.0f, 0.0f, mFlag31 ? 1.0f : -1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &dir, &dir);
        float dot = PSVECDotProduct(&dir, &msg->m28);
        if ((kinds & 0x10) == 0x10) {
            flags &= ~0x10;
            if (msg->mAttacker != 0) {
                msg->mAttacker->mStatus.fn_80133E24(-1, 0x10);
                if (msg->mAttacker->mStatus.fn_80133F6C(-1, -1) <= 0) {
                    msg->m0C &= ~0x2000000;
                    msg->m94 &= ~0x2000000;
                }
            }
        }
        if ((kinds & 8) != 8 && mFlag5) {
            if (dot > (float)cos(0.5f * lbl_8087F0A8->m334) || mFlagB16 || (lbl_8087F610 != 0 && mType == 0)) {
                if (!IsDown_unk() && !IsStill_unk()) {
                    flags |= 8;
                }
            }
        }
        if (lbl_8087F048 != 0 && fn_8010CD3C(lbl_8087F048, 5, this, this)) {
            fn_8010CF28(lbl_8087F048, 5, this, this);
            flags |= 0x20;
        }
        if ((kinds & 0x80) != 0x80 && msg->m04 != 0) {
            if (msg->m04->GetStatus_unk()->IsState_unk(0x10)) {
                flags |= 0x80;
            }
            if (msg->m04->GetStatus_unk()->IsState_unk(0x4000)) {
                flags |= 0x80;
            }
        }
        bool combo = false;
        if (msg->mAttacker != 0) {
            if (msg->mAttacker->ControlVf30_unk() == 0) {
                combo = true;
            }
            if (msg->mAttacker->ControlVf30_unk() % 2 == 0 &&
                msg->mAttacker->GetFlaggedHitBullet_unk() != 0) {
                combo = true;
            }
            if (msg->mAttacker->m55C == 6 && msg->mAttacker->m560 == 9) {
                combo = true;
            }
        }
        if (combo) {
            vf5C(msg->mAttacker);
            msg->m74 = mFE4;
        } else {
            msg->m74 = 0;
        }
        if (fn_80370174(lbl_8087F430, 0x548) == 1 && mType != 0 && msg->mData->m04 != 0xD0) {
            flags |= 0x2008;
            msg->mAttacker->vf7C(1);
        } else if (fn_80370174(lbl_8087F430, 0x28) == 0 && msg->mAttacker != 0 &&
                   ((unsigned int)msg->mAttacker->mStatus.mParamB.m9C & 4) != 4) {
            bool guard = false;
            if ((flags & 8) == 8 && mFlag5 && !mFlag31 && !msg->mFlag31) {
                guard = true;
            }
            if (lbl_8087F610 == 0 && !mStatus.fn_801346C8(2) && !mStatus.fn_801346C8(3)) {
                guard = false;
            }
            if (guard) {
                if (fn_80041A28(1, this, msg->mAttacker) == 3) {
                    flags |= 0x2008;
                    msg->mAttacker->vf7C(1);
                } else {
                    mFlag9 = (mFlag5 && mF58 >= 15);
                }
            }
        }
        HitParam_unk param;
        param.m00 = msg->m74;
        param.m04 = msg->m5C;
        param.m08 = msg->m60;
        param.m14 = mFlag22;
        fn_8003EA3C(&msg->m64, msg->mData, msg->mAttacker, msg->m04, flags, &param, 0, 0);
        if (msg->mFlag30) {
            flags &= ~8;
            flags |= 0x200000;
        }
        if (msg->mFlag29) {
            flags &= ~0x2000000;
        }
        if (msg->mFlag28) {
            flags &= ~0x8000;
        }
        if (msg->m04 != 0 && msg->m04->mType == 2 && msg->m68 < 0) {
            fn_80150460(msg->mAttacker, 1);
        }
        if (msg->m64 == 1) {
            fn_8015076C(msg->m34, msg->m10, msg->m1C, msg->m38, flags, msg);
        }
        msg->m88 = msg->mData->mC4;
        if ((flags & 0x20) == 0x20) {
            msg->m84 = 2;
        } else if ((flags & 8) == 8) {
            msg->m84 = 1;
        } else {
            msg->m84 = 0;
        }
        msg->m8C = 1;
        msg->m90 = 1;
    } else if (msg->m44 == 1) {
        if ((kinds & 8) != 8) {
            Settings_unk* settings = lbl_8087F0A8;
            if (mFlag5) {
            atn::Vector3 dir(0.0f, 0.0f, mFlag31 ? 1.0f : -1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &dir, &dir);
            float dot = PSVECDotProduct(&dir, &msg->m28);
            if (dot > (float)cos(0.5f * settings->m334) || (lbl_8087F610 != 0 && mType == 0)) {
                if (!IsDown_unk() && !IsStill_unk()) {
                    flags |= 8;
                }
            }
            }
        }
        if ((kinds & 1) != 1 && (mType == 0 || lbl_8087F610 != 0) && mFlag31 && !HasC48_unk()) {
            flags |= 1;
        }
        HitParam_unk param;
        param.m00 = 0;
        param.m04 = 1.0f;
        param.m08 = 1.0f;
        param.m14 = mFlag22;
        if (lbl_8087F430 != 0 && msg->mAttacker != 0 && msg->mAttacker->mType == 0 && msg->mData != 0 &&
            msg->mData->m04 == 0x133) {
            param.m0C = fn_80370174(lbl_8087F430, 0xDA);
        }
        if (msg->mAttacker != 0 && msg->mAttacker->mStatus.fn_80134134(0x43) != 0) {
            param.m10 = 100.0f / (100.0f + msg->mAttacker->mStatus.fn_80134168(0x43, -1));
        }
        fn_8003EA3C(&msg->m64, msg->mData, msg->mAttacker, msg->m04, flags, &param, 0, 0);
        if (msg->mFlag30) {
            flags &= ~8;
            flags |= 0x200000;
        }
        if (msg->mFlag29) {
            flags &= ~0x2000000;
        }
        if (msg->mFlag28) {
            flags &= ~0x8000;
        }
        if (msg->m64 == 1) {
            fn_8015076C(msg->m34, msg->m10, msg->m1C, msg->m38, flags, msg);
        }
        msg->m88 = msg->mData->mC4;
        msg->m84 = (flags >> 3) & 1;
        msg->m8C = 1;
        msg->m90 = 1;
        if ((msg->mData->mAC & 0x8000) == 0x8000) {
            if (fn_80151270(this) && !mFlag2) {
                fn_8016D0A0(1);
            }
            if (msg->mData->m04 == 0x147) {
                msg->m90 = 0;
            }
        }
    } else if (msg->m44 == 2) {
        if ((kinds & 8) != 8) {
            if (msg->mData->m01 == 1) {
                if ((mType == 0 || lbl_8087F610 != 0) && mFlag31 && !HasC48_unk()) {
                    flags |= 0x80009;
                } else if (mFlag5 && !IsDown_unk() && !(atn::Abs_unk(mStatus.mParamB.m1C) < 1.1920929e-7f)) {
                    flags |= 0x80008;
                }
            } else if (mFlag5 && !IsDown_unk() && !IsStill_unk()) {
                flags |= 8;
            }
        }
        HitParam_unk param;
        param.m04 = msg->m5C;
        fn_8003EA3C(&msg->m64, msg->mData, msg->mAttacker, msg->m04, flags, &param, 0, 0);
        if (msg->mFlag30) {
            flags &= ~8;
            flags |= 0x200000;
        }
        if (msg->mFlag29) {
            flags &= ~0x2000000;
        }
        if (msg->mFlag28) {
            flags &= ~0x8000;
        }
        if (msg->m64 == 1) {
            fn_8015076C(msg->m34, msg->m10, msg->m1C, msg->m38, flags, msg);
        }
        msg->m88 = -1;
        if (msg->mData != 0 && msg->mData->m04 == 0xA4) {
            msg->m88 = 2;
        }
        msg->m84 = 0;
        msg->m8C = msg->mAttacker != 0;
        msg->m90 = 1;
        if ((msg->mData->mAC & 0x8000) == 0x8000) {
            if (fn_80151270(this) && !mFlag2) {
                fn_8016D0A0(1);
            }
            if (msg->mData->m04 == 0x147) {
                msg->m90 = 0;
            }
        }
    } else if (msg->m44 == 3) {
        fn_8003EA3C(&msg->m64, msg->mData, msg->mAttacker, msg->m04, flags, &lbl_807C6B90, 0, 0);
        if (msg->mFlag30) {
            flags &= ~8;
            flags |= 0x200000;
        }
        if (msg->mFlag29) {
            flags &= ~0x2000000;
        }
        if (msg->mFlag28) {
            flags &= ~0x8000;
        }
        msg->m88 = -1;
        msg->m84 = 0;
        msg->m8C = msg->mAttacker != 0;
        if (msg->m68 == 0 && msg->m6C == 0 && msg->m70 == 0 && !msg->mFlag31) {
            msg->m90 = 0;
        } else {
            msg->m90 = 1;
        }
    }
    msg->m94 = flags;
    if (msg->m68 > 0) {
        mC4C = 0;
    }
    if (mFlag28 && !CanReact_unk()) {
        fn_80164DCC(1);
    }
}

void CharaTask::vf40(HitInfo_unk* hit) {}

extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* ab);

// inferred: hit reaction check (guard?); returns 0, 1 or 2. The parameters are read by the body,
// so the symbol vf4C__9CharaTaskFv (in symbols.txt) has the wrong signature.
int CharaTask::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    if (mFlag31) {
        return 0;
    }
    if (IsDown_unk()) {
        return 0;
    }
    if (hit->m01 == 1) {
        if (mType == 0) {
            if (mFlag5 && m55C != 6) {
                bool ok = true;
                if (mF58 >= 30 && ((unsigned int)mStatus.m14 & 1) != 1) {
                    ok = false;
                }
                if (Dot_unk(*dir, -fn_8014052C()) > -0.70710677f && ok) {
                    if (mStatus.fn_801346C8(2) || mStatus.fn_801346C8(3) || mStatus.fn_801346C8(0x2E) ||
                        lbl_8087F610 != 0) {
                        return 2;
                    }
                    return 1;
                }
            }
        } else if (mType == 3 && lbl_8087F048 != 0) {
            bool ok = false;
            if (mFlag5 && m55C != 6 && ((unsigned int)mStatus.m14 & 1) == 1) {
                ok = true;
            }
            if (fn_8010CD3C(lbl_8087F048, 8, this, attacker)) {
                ok = true;
            }
            if (Dot_unk(*dir, -fn_8014052C()) > -0.70710677f && ok) {
                if (HasHitBullet_unk(0x1789)) {
                    return 2;
                }
                return 1;
            }
        }
    }
    return 0;
}


// inferred: names invented
inline bool CharaTask::IsPartsMode_unk() {
    if (mType == 0 && lbl_8087F0A8->m278 != 0 && m5C->m11C == 1) {
        return true;
    }
    if (mType == 0 && mFlagB17) {
        return true;
    }
    return false;
}

inline void CharaTask::ResetParts_unk() {
    if (IsPartsMode_unk()) {
        if (fn_80370174(lbl_8087F430, 0x25) >= 1) {
            fn_8014EEC4(0);
        } else if (m674 >= 0) {
            fn_8014DEE4(0, 0, 0);
        }
    }
}

// inferred: body of fn_801539E0 (inlined in vf44 and fn_8015495C, defined below them)
inline void CharaTask::ReleaseLedge_unk() {
    mFlag31 = 0;
    mFlag30 = 0;
    RouteTable_unk* table = lbl_8087F430->fn_8013C504();
    if (table != 0 && mC38 > 0 && mC3C > 0) {
        table->GetLink_unk(mC38)->m3C = 0;
        table->GetLink_unk(mC3C)->m3C = 0;
    }
    ResetParts_unk();
}

inline void CharaTask::Unlink_unk() {
    if (mC58.mC4 != 0 && mC58.mC4->HasLink_unk(this)) {
        mC58.mC4->RemoveLink_unk(this);
    }
}

inline void CharaTask::ClearEffect_unk() {
    if (lbl_8087F048 != 0) {
        lbl_8087F048->fn_801059F8(this);
        lbl_8087F048->fn_80105B3C(this);
    }
    mFlagB8 = 0;
    mFB8 = 0.0f;
    mFBC = 0.0f;
}

// inferred: body of fn_8015495C (inlined in vf44, defined below it)
inline void CharaTask::Reset_unk(int arg, int ledge, int flag, int effect) {
    mC58.fn_8011BEB8(arg);
    if (ledge && IsOnLedge_unk()) {
        ReleaseLedge_unk();
    }
    if (flag && IsFlag5_unk()) {
        mFlag5 = 0;
    }
    Unlink_unk();
    if (effect) {
        ClearEffect_unk();
    }
}

void CharaTask::vf44() {
    vf48();
    fn_80178A6C();
    m58C = 2;
    mStatus.fn_8012DD70();
    Reset_unk(0, 1, 1, 1);
    ClearEffect_unk();
    fn_80164DCC(1);
    mFC0 = 0;
    mFlagB10 = 1;
}

extern "C" void fn_80107798(Effect_unk* effect, chr::Motion_unk* motion, int arg);
extern "C" void fn_80107A68(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107E58(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107FC8(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80108080(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_801079B0(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107F10(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107BD8(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107380(Effect_unk* effect, CharaTask* chara);
extern "C" void fn_80107D94(Effect_unk* effect, chr::Motion_unk* motion);


// NONMATCHING: ~57% - the inlined fn_8016E970 is one inline level short (see fn_80178208), and in
// the camera state copy the original computes the address of the last 16-byte block (addi r9)
// and allocates the registers in the other order; tried member-wise init, a constructor taking
// the blocks, assignments in the body.
void CharaTask::vf48() {
    if (lbl_8087F048 != 0) {
        fn_80107850(lbl_8087F048, &mMotion);
        fn_80107798(lbl_8087F048, &mMotion, 1);
        fn_80107A68(lbl_8087F048, &mMotion);
        fn_80107B20(lbl_8087F048, &mMotion);
        fn_80107E58(lbl_8087F048, &mMotion);
        fn_80107FC8(lbl_8087F048, &mMotion);
        fn_80108080(lbl_8087F048, &mMotion);
    }
    fn_80168048();
    fn_80168190();
    fn_80166544();
    fn_801687D8();
    fn_80168E60();
}


extern "C" int fn_8021A8D0(HitInfo_unk* hit);
extern "C" int fn_8021A918(HitInfo_unk* hit);
extern "C" int fn_8021A888(HitInfo_unk* hit);

// inferred: nonzero if the hit `id` is ignored (name invented)
bool CharaTask::fn_801533C8(int id, HitInfo_unk* hit) {
    for (CharaPair8_unk* it = m6D0.begin(); it != m6D0.end(); it++) {
        if (it->m00 == id) {
            return true;
        }
    }
    if (lbl_8087F430->m54E4 == 9) {
        return true;
    }
    if (!CanAct_unk()) {
        return true;
    }
    if (mFlagB9) {
        return true;
    }
    if (IsPartyMember_unk() && (hit == 0 || (hit->mAC & 0x20000) != 0x20000)) {
        return true;
    }
    if ((m54C & 4) == 4) {
        return true;
    }
    if (m6D0.full()) {
        return true;
    }
    if (hit != 0 && (hit->mAC & 0x4000) == 0x4000) {
        return false;
    }
    if (m55C == 6) {
        switch (m560) {
        case 27:
        case 141:
            if (mType != 2) {
                return true;
            }
            break;
        case 28: case 59: case 64: case 75: case 76: case 77: case 81: case 95: case 100: case 101:
        case 102: case 103: case 105: case 110: case 111: case 113: case 116: case 117: case 118:
        case 119: case 123: case 126: case 127: case 133: case 134: case 135: case 136: case 139:
        case 140: case 142:
            return true;
        case 10:
        case 128:
            if (mType == 0) {
                return true;
            }
            break;
        case 132:
            if (hit == 0 || (!fn_8021A8D0(hit) && !fn_8021A918(hit))) {
                return true;
            }
            break;
        case 30:
            if (hit == 0 || (!fn_8021A8D0(hit) && !fn_8021A918(hit) && !fn_8021A888(hit))) {
                if (!mStatus.mParamB.fn_801404E0(0x100)) {
                    return true;
                }
            }
            break;
        case 5:
            if (SetConfig::sInstance->mAttackType == 0) {
                return mMotion.mTracks[0].m08 < 8.0f;
            }
            break;
        }
    } else if (m55C == 5) {
        return true;
    }
    return false;
}



// NONMATCHING: instructions and stack identical; only the saved registers differ (the original
// keeps the ledge in r30 and this in r29, ours the other way round). Tried a free function, other
// statement orders, the direction computed in place or through helpers.
// inferred: grabs a ledge
void CharaTask::fn_80153698(const Ledge_unk* ledge, int keep) {
    mFlag31 = 1;
    mC48 = 0;
    mC14 = ledge->GetDir_unk();
    mFlag30 = ledge->m34 != 0;
    mC20 = ledge->m18;
    mC2C = ledge->m24;
    mC44 = ledge->m30;
    mC40 = ledge->m34;
    mC38 = ledge->m38;
    mC3C = ledge->m3C;
    mFlag14 = 1;
    Segment_unk seg(mC20, mC2C);
    atn::Vector3 pos;
    fn_80050A1C(&mPos, &seg, &pos);
    mPos = pos;
    atn::Vector3 fwd = fn_8014052C();
    if (Dot_unk(fwd, mC14) >= 0.0f) {
        mFlag29 = 1;
    } else {
        mFlag29 = 0;
    }
    RouteTable_unk* table = lbl_8087F430->fn_8013C504();
    if (table != 0 && mC44 == 0) {
        table->GetLink_unk(mC38)->m3C = 1;
        table->GetLink_unk(mC3C)->m3C = 1;
    }
    if (keep == 0) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        if (mFlag30) {
            mMotion.fn_80097C08(0, mFlag29 ? m484[5] : m484[6], 1, 0, 1, 0.0f, 0.1f);
        } else {
            mMotion.fn_80097C08(0, mFlag29 ? m484[7] : m484[8], 1, 0, 1, 0.0f, 0.1f);
        }
        fn_800C344C(sSoundNames_unk[3], &mPos, 0, -1, 1.0f);
        if (lbl_8087F0A8->m3D8 != 0) {
            fn_80164DCC(1);
        }
    }
    if (mType == 0 && fn_80370174(lbl_8087F430, 0xED) == 0 && fn_80370174(lbl_8087F430, 0xEA) == 1) {
        fn_80370320(lbl_8087F430, 0xED, 1, 0);
    }
}

void CharaTask::fn_801539E0() {
    ReleaseLedge_unk();
}


// inferred: checks where the character can move along the ledge (0 none, 1 back, 2 forward, 3 up)
int CharaTask::fn_80153B44() {
    int result = 0;
    if ((mC44 & 1) == 1) {
        atn::Vector3 d(mPos - mC20);
        if (fn_805F9920(&d) < 2.0f) {
            result = 1;
        }
    }
    if ((mC44 & 2) == 2) {
        atn::Vector3 d(mPos - mC2C);
        if (fn_805F9920(&d) < 2.0f) {
            result = 2;
        }
    }
    if (result == 0 && (mC44 & 4) == 4) {
        result = 3;
    }
    bool loop = true;
    while (loop) {
        loop = false;
        if (result != 0 && mType == 0) {
            atn::Vector3 from(mPos);
            if (result == 1) {
                from -= mC14 * 6.0f;
            } else if (result == 2) {
                from += mC14 * 6.0f;
            }
            from.y += CanClimbUp_unk(result) ? 6.0f : 12.0f;
            atn::Vector3 to(from + fn_8014052C() * 5.0f);
            if (fn_8004ED34(lbl_8087EE98, 0, &from, &to, 0x80000006, 0, 0)) {
                if (mFlag30 && result != 3) {
                    loop = true;
                    result = 3;
                } else {
                    result = 0;
                }
            }
            if (result == 1 || result == 2) {
                from = mPos;
                from.y += 6.0f;
                to = from;
                if (result == 1) {
                    to -= mC14 * 6.0f;
                } else if (result == 2) {
                    to += mC14 * 6.0f;
                }
                if (fn_8004ED34(lbl_8087EE98, 0, &from, &to, 0x80000006, 0, 0)) {
                    if (mFlag30 && result != 3) {
                        loop = true;
                        result = 3;
                    } else {
                        result = 0;
                    }
                }
            }
        }
    }
    return result;
}

int CharaTask::fn_80153FA0() {
    return fn_80153FD8(fn_80153B44());
}

int fn_8013539C();
extern "C" void fn_8023A02C(void* manager, void* owner, int arg2, int arg3);
extern "C" void fn_8023A098(void* manager, void* owner, int arg2, int arg3);

// inferred: starts moving along the ledge (kind from fn_80153B44)
int CharaTask::fn_80153FD8(int kind) {
    mC48 = kind;
    if (kind == 1) {
        mFlag29 = 0;
    } else if (kind == 2) {
        mFlag29 = 1;
    }
    if (mC48 != 0) {
        if (IsPartsMode_unk()) {
            if (m674 >= 0 || fn_80370174(lbl_8087F430, 0x25) == 1 ||
                (fn_80370174(lbl_8087F430, 0x25) == 2 && fn_8013539C())) {
                fn_8014DEE4(1, 0, 0);
            }
        }
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        if (mC48 == 3) {
            if (mType == 0) {
                atn::Vector3 from(mPos);
                from.y += 6.0f;
                atn::Vector3 to(from);
                if (mFlag29) {
                    to += mC14 * 8.0f;
                } else {
                    to -= mC14 * 8.0f;
                }
                void* world = lbl_8087EE98;
                if (fn_8004ED34(world, 0, &from, &to, fn_80179D44() | 0x80000000, 0, 0)) {
                    mFlag29 = !mFlag29;
                }
            }
            mMotion.fn_80097C08(0, mFlag29 ? m484[17] : m484[18], 1, 0, 1, 0.0f, 0.1f);
        } else if (mFlag30) {
            mMotion.fn_80097C08(0, mFlag29 ? m484[13] : m484[14], 1, 0, 1, 0.0f, 0.1f);
        } else {
            mMotion.fn_80097C08(0, mFlag29 ? m484[15] : m484[16], 1, 0, 1, 0.0f, 0.1f);
        }
        fn_800C344C(sSoundNames_unk[4], &mPos, 0, -1, 1.0f);
        fn_8023A02C(lbl_8087F3C0, this, 4, 0x20);
        return 1;
    }
    return 0;
}

void CharaTask::fn_80154344() {
    int mode = lbl_8087F430->m868;
    if (mode == 3 && mode == lbl_8087F430->m86C) {
        mFlag14 = 0;
    }
    mC48 = 0;
    ResetParts_unk();
    fn_8023A098(lbl_8087F3C0, this, 4, 0x20);
}

extern "C" double cos(double x);


// NONMATCHING: 91% - the two copies into `dir` take the source/destination registers in the other
// order (r3/r4) and fabs uses f31 directly (original: fabs f0; frsp f31,f0); tried a ternary,
// Assign_unk, Abs_unk, casts.
// inferred: turns along the ledge towards the target (sets the route walker direction)
void CharaTask::fn_80154488() {
    if (mFlag2) {
        return;
    }
    if (mC58.mC4 == 0 && mC58.mD0 == 0) {
        return;
    }
    if (!mFlag31) {
        return;
    }
    if ((mC44 & 4) == 4) {
        return;
    }
    if ((mC44 & 3) != 3) {
        return;
    }
    atn::Vector3 dir;
    if (mC58.mD0 != 0) {
        dir = mC58.mD0->vf54() - mPos;
    } else {
        dir = mC58.mC4->mPos - mPos;
    }
    PSVECNormalize(&dir, &dir);
    atn::Vector3 axis;
    PSVECCrossProduct(&mC14, &atn::Vector3(0.0f, 1.0f, 0.0f), &axis);
    PSVECNormalize(&axis, &axis);
    float d = atn::Abs_unk(Dot_unk(axis, dir));
    if (d < (float)cos(0.2617994f)) {
        if (Cross_unk(axis, dir).y > 0.0f) {
            mC58.m0C.m1C = 2;
        } else {
            mC58.m0C.m1C = 1;
        }
    }
}

// inferred: sets the resource set of the character (type, id)
void CharaTask::fn_80154654(int type, int arg) {
    mC50 = type;
    mC54 = arg;
    m68[0] = fn_80204E04(type, arg);
    int kind = fn_8015487C();
    if (sTable807374B8_unk[kind] > 0) {
        m68[2] = fn_80204E04(type, sTable807374B8_unk[kind]);
    }
    m68[3] = fn_80204E04(type, 0x251D);
    m68[4] = fn_80204E04(type, 0x2581);
    fn_80154C08();
}

extern "C" int fn_80208748(const char* name);
extern "C" int atoi(const char* s);
// inferred: table returned by fn_80375254 (one value per kind of fn_8015487C)
struct FieldRes_unk {
    unsigned char m00[0x08];
    int m08[7];
};
extern "C" FieldRes_unk* fn_80375254(Field_unk* field);

void CharaTask::fn_801546F4(int arg, atn::Vector3 pos, float rot) {
    int kind = fn_8015487C();
    if (kind != 6 && lbl_8087F430 != 0) {
        int type = m60 != 0 ? fn_80208748(m60->m10) : 0;
        int id = m60 != 0 ? atoi(m60->m10 + 2) * 10000 + atoi(m60->m10 + 8) * 100 + atoi(m60->m10 + 11) : 0;
        if (type == 1 && id / 10000 == 53) {
            return;
        }
        if (type == 0 && id / 10000 == 8) {
            return;
        }
        FieldRes_unk* res = fn_80375254(lbl_8087F430);
        if (res != 0) {
            fn_80154654(mC50, res->m08[kind]);
        }
    }
}

int CharaTask::fn_8015487C() {
    int result = 0;
    switch (m5C->m122) {
    case 4:
        if (m50 != 0x18AED) {
            result = 1;
        }
        break;
    case 2:
        result = 2;
        if (mStatus.m2CC != 0) {
            if (mStatus.m2D0[0].m[0] > 0 && fn_8021A8D0((HitInfo_unk*)fn_80219E6C(mStatus.m2D0[0].m[0]))) {
                result = 5;
            } else if ((unsigned int)mStatus.m2CC >= 2 && mStatus.m2D0[1].m[0] > 0 &&
                       fn_8021A8D0((HitInfo_unk*)fn_80219E6C(mStatus.m2D0[1].m[0]))) {
                result = 5;
            }
        }
        break;
    case 3:
        result = 3;
        break;
    }
    return result;
}

void CharaTask::fn_8015495C(int arg, int ledge, int flag, int effect) {
    Reset_unk(arg, ledge, flag, effect);
}

// inferred: result of fn_800185B4 (lbl_8087EE68 = party manager?): the character a magic is redirected to
struct MagicTarget_unk {
    unsigned char m00[0x08];
    int m08;
    CharaTask* m0C;
};
extern "C" MagicTarget_unk* fn_800185B4(void* manager, CharaTask* chara, HitInfo_unk* hit);

// inferred: name invented
inline bool CharaTask::IsRes1_unk() {
    if (mType != 3 || m68[1] == 0) {
        return false;
    }
    void* manager = fn_80139EF4();
    return (manager != 0 ? fn_800185B4(manager, this, 0) : 0) != 0;
}

// inferred: picks the current resource
void CharaTask::fn_80154C08() {
    CharaRes_unk* prev = m64;
    m64 = m68[0];
    if ((mStatus.m0C & 0x800) == 0x800 && fn_8017A9A8()) {
        m64 = m68[2];
    }
    if (IsRes1_unk()) {
        m64 = m68[1];
    }
    if (!mStatus.fn_80134208() && (mStatus.fn_80134270() || mStatus.fn_80134290() || mStatus.fn_801342B0()) &&
        (mStatus.m0C & 4) == 4) {
        m64 = m68[3];
    }
    if (mC58.m2B8 > 0) {
        m64 = m68[4];
        mC58.m2B8--;
    }
    if (m64 != prev) {
        mC58.fn_8011BE54();
        mC58.fn_8011BEB8(0);
        Unlink_unk();
        if (m64 != 0) {
            mC58.m114 = m64->m88;
            mC58.m118 = m64->m8C;
        } else {
            mC58.m114 = 1;
            mC58.m118 = 0;
        }
        mStatus.fn_8012D180();
    }
}

// inferred: nonzero if `target` can be targeted by this character
bool CharaTask::fn_80154E38(CharaTask* target) {
    if (target != 0 && this == target) {
        return false;
    }
    bool result = false;
    if (lbl_8087F0A8->mCC != 0 || mStatus.fn_80134134(6) != 0 || (mStatus.m0C & 0x200000) == 0x200000) {
        result = true;
    }
    return result;
}

namespace chr {
// inferred: control created by CharaTask; class name unknown (constructor 0x801C041C in the unit
// after chr/ControlDive.cpp). The controls of chr/ControlDive.cpp are in chr/ControlDive.h.
class Control801C041C_unk : public ControlBase {
public:
    Control801C041C_unk(Chara_unk* chara, int a, float b);
    virtual int vf0C();
    unsigned char m08[0x30 - 0x08];
};
} // namespace chr

// fn_80154EC4, fn_801551AC, vf74, fn_80155790: same instructions as the target; only the .data
// offsets of the ptmf constants differ until the earlier .data users exist.
void CharaTask::fn_80154EC4(const atn::Vector3& pos, int link) {
    fn_80178208(new (0, "", "", 0) chr::ControlSwatTurn(this, pos, link));
}

void CharaTask::fn_801551AC() {
    fn_80178208(new (0, "", "", 0) chr::ControlCoverSlip(this));
}

// inferred: the parameter is read by the body, so the symbol vf74__9CharaTaskFv (in symbols.txt)
// has the wrong signature.
void CharaTask::vf74(int arg) {
    fn_80178208(new (0, "", "", 0) chr::Control801C041C_unk(this, arg, 1.0f));
}

// inferred: the argument is a direction (fn_8013A4E8 passes a rotated vector)
void CharaTask::fn_80155790(const atn::Vector3& dir) {
    fn_80178208(new (0, "", "", 0) chr::ControlDive(this, dir));
}
void CharaTask::fn_80155A88(const atn::Vector3& dir, int a) {
    fn_80178208(new (0, "", "", 0) chr::ControlBackStep(this, dir, a));
}

// inferred: nonzero while control 2 is running and its motion is in the first 30 frames
bool CharaTask::fn_80155D70() {
    if (m55C == 6 && m560 == 2 && mMotion.mTracks[0].m08 <= 30.0f) {
        return true;
    }
    return false;
}

// inferred: sets or clears mFlag6 (forced by mFlagB2) and plays the matching motion; characters
// 0xAAE76/0xAAE77 stop motion m484[4] on a fixed frame
void CharaTask::fn_80155DAC(int arg) {
    if (mFlag2 && mFlag6 == arg) {
        return;
    }
    mFlag6 = arg | mFlagB2;
    if (mFlag6) {
        if (m50 == 0xAAE76) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.fn_80097C08(0, m484[4], 0, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 0.0f;
            mMotion.mTracks[0].m08 = 50.0f;
        } else if (m50 == 0xAAE77) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.fn_80097C08(0, m484[4], 0, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 0.0f;
            mMotion.mTracks[0].m08 = 34.0f;
        } else {
            if (IsPartsMode_unk()) {
                if (m674 >= 0 || fn_80370174(lbl_8087F430, 0x25) == 1 ||
                    (fn_80370174(lbl_8087F430, 0x25) == 2 && fn_8013539C())) {
                    fn_8014DEE4(1, 0, 0);
                    fn_800C344C(sSoundNames_unk[4], &mPos, 0, -1, 1.0f);
                }
            }
            if (m674 >= 0) {
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.fn_80097C08(0, m484[4], 1, 0, 1, 0.0f, 0.1f);
                mMotion.mTracks[0].m0C = 1.0f;
            } else {
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 0.0f, 0.1f);
                mMotion.mTracks[0].m0C = 1.0f;
            }
        }
        fn_8023A02C(lbl_8087F3C0, this, 4, 0x20);
    } else {
        ResetParts_unk();
        fn_8023A098(lbl_8087F3C0, this, 4, 0x20);
    }
}

extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

// inferred: nonzero if `target` is within 45 degrees in front of the character
bool CharaTask::fn_80156120(CharaTask* target) {
    bool result = false;
    if (target != 0) {
        Mtx m;
        atn::Vector3 dir = target->mPos - mPos;
        dir.y = 0.0f;
        PSVECNormalize(&dir, &dir);
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &front, &front);
        if (PSVECDotProduct(&dir, &front) > 0.70710677f) {
            result = true;
        }
    }
    return result;
}

// inferred: attacks straight ahead
void CharaTask::fn_801561E4(HitInfo_unk* hit) {
    Mtx m;
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    float ry = mRot.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &front, &front);
    fn_801562A0(hit, mPos + front, 0, 0, 0, 0, 0.0f);
}

// inferred: front direction of a rotation (name invented)
inline atn::Vector3 GetFront2_unk(Mtx m, const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    PSMTXRotRad(m, 'y', rot.y);
    PSMTXMultVec(m, &v, &v);
    return v;
}

// inferred: route test from `pos` along `dir` (game route table at lbl_8087F430->m10D8)
extern "C" int fn_803CC198(RouteTable_unk* table, const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* chara);

// inferred: starts an attack toward `pos`; `dist` is the distance kept from the target, `type` 1/2
// selects a jump attack (2 = second kind), otherwise a dash or jump attack is chosen by ray tests.
// `b` is not read.
void CharaTask::fn_801562A0(HitInfo_unk* hit, const atn::Vector3& pos, int type, int a, int b, CharaTask* target,
                            float dist) {
    m59D = 0;
    m59C = 0;
    m59F = 0;
    if (hit != 0) {
        SetHit_unk(hit);
    } else {
        SetHit_unk(fn_80219E6C(200));
    }
    Mtx m;
    atn::Vector3 dir;
    dir = pos - mPos;
    dir.y = 0.0f;
    dist = PSVECMag(&dir) - 0.9f * dist;
    if (fn_805F9920(&dir) > 0.0001f) {
        PSVECNormalize(&dir, &dir);
    } else {
        dir.Assign_unk(GetFront2_unk(m, *GetRot_unk()));
    }
    if (m648 != 0) {
        m648->vf18(lbl_807C7030, lbl_807C7030, 0, 200);
    }
    if (a == 0 && (mFlag31 || lbl_8087F0A8->m260 == 0 || dist < mStatus.mParamB.m28)) {
        fn_80178208(new (0, "", "", 0) chr::ControlAttack(this, dir));
    } else if (type > 0) {
        atn::Vector3 offset = mPos + atn::Vector3::fn_8013C394(mC14, atn::Vector3(0.0f, 1.0f, 0.0f)) * 5.0f;
        offset.y = mPos.y;
        offset = offset - mPos;
        switch (type) {
        case 1:
            if (target != 0) {
                offset = target->mPos - mPos;
                offset.y = 0.0f;
            }
            fn_80178208(new (0, "", "", 0) chr::ControlJumpAttack(this, pos, offset));
            break;
        case 2:
            fn_80178208(new (0, "", "", 0) chr::ControlJumpAttack2(this, pos, offset));
            break;
        }
    } else {
        atn::Vector3 hitPos;
        atn::Vector3 from(mPos);
        atn::Vector3 to = mPos + dir * 20.0f;
        bool jump = false;
        from.y += 3.0f;
        to.y += 3.0f;
        if (fn_8004ECC0(lbl_8087EE98, &hitPos, &from, &to, 0x80000000, 0, 0)) {
            from.y += 8.0f;
            to.y += 8.0f;
            if (!fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000000, 0, 0) &&
                fn_803CC198(lbl_8087F430->m10D8, &mPos, &dir, this)) {
                jump = true;
            }
        }
        if (jump) {
            atn::Vector3 offset = hitPos;
            offset.y = mPos.y;
            offset = offset - mPos;
            fn_80178208(new (0, "", "", 0) chr::ControlJumpAttack(this, pos, offset));
        } else {
            fn_80178208(new (0, "", "", 0) chr::ControlDashAttack(this, pos));
        }
    }
}

// inferred: next attack of a combo toward `pos`
void CharaTask::fn_80157444(HitInfo_unk* hit, const atn::Vector3& pos) {
    m59D = 0;
    m59C = 1;
    m59F = 0;
    if (hit != 0) {
        SetHit_unk(hit);
    } else {
        SetHit_unk(fn_80219E6C(200));
    }
    if (m648 != 0) {
        m648->vf18(lbl_807C7030, lbl_807C7030, 0, 200);
    }
    atn::Vector3 dir = pos - mPos;
    dir.y = 0.0f;
    if (fn_805F9920(&dir) > 0.0001f) {
        PSVECNormalize(&dir, &dir);
    } else {
        Mtx m;
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        PSMTXRotRad(m, 'y', GetRot_unk()->y);
        PSMTXMultVec(m, &front, &front);
        // inferred: z-first copy called directly (dir.Assign_unk(front) swaps the address registers)
        __VecCopyZFirst(&dir.x, &front.x);
    }
    fn_80178208(new (0, "", "", 0) chr::ControlAttackNext(this, dir));
}

// inferred: multi attack toward `pos`
void CharaTask::fn_8015783C(HitInfo_unk* hit, const atn::Vector3& pos) {
    m59D = 1;
    m59C = 0;
    m59F = 0;
    if (hit != 0) {
        SetHit_unk(hit);
    } else {
        SetHit_unk(fn_80219E6C(200));
    }
    if (m648 != 0) {
        m648->vf18(lbl_807C7030, lbl_807C7030, 0, 200);
    }
    atn::Vector3 dir = pos - mPos;
    dir.y = 0.0f;
    if (fn_805F9920(&dir) > 0.0001f) {
        PSVECNormalize(&dir, &dir);
    } else {
        Mtx m;
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        PSMTXRotRad(m, 'y', GetRot_unk()->y);
        PSMTXMultVec(m, &front, &front);
        // inferred: z-first copy called directly (dir.Assign_unk(front) swaps the address registers)
        __VecCopyZFirst(&dir.x, &front.x);
    }
    fn_80178208(new (0, "", "", 0) chr::ControlMultiAttack(this, dir));
}

// inferred: side wall attack toward `pos`
void CharaTask::fn_80157C34(HitInfo_unk* hit, const atn::Vector3& pos) {
    m59D = 1;
    m59C = 0;
    m59F = 0;
    if (hit != 0) {
        SetHit_unk(hit);
    } else {
        SetHit_unk(fn_80219E6C(200));
    }
    if (m648 != 0) {
        m648->vf18(lbl_807C7030, lbl_807C7030, 0, 200);
    }
    atn::Vector3 dir = pos - mPos;
    dir.y = 0.0f;
    if (fn_805F9920(&dir) > 0.0001f) {
        PSVECNormalize(&dir, &dir);
    } else {
        Mtx m;
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        PSMTXRotRad(m, 'y', GetRot_unk()->y);
        PSMTXMultVec(m, &front, &front);
        // inferred: z-first copy called directly (dir.Assign_unk(front) swaps the address registers)
        __VecCopyZFirst(&dir.x, &front.x);
    }
    fn_80178208(new (0, "", "", 0) chr::ControlSideWallAttack(this, dir));
}

// inferred: swaps places with `target` (target plays motion 0x46 and is reset)
void CharaTask::fn_8015802C(HitInfo_unk* hit, CharaTask* target) {
    m59D = 0;
    m59C = 0;
    m59F = 0;
    if (hit != 0) {
        SetHit_unk(hit);
    } else {
        SetHit_unk(fn_80219E6C(200));
    }
    if (m648 != 0) {
        m648->vf18(lbl_807C7030, lbl_807C7030, 0, 200);
    }
    target->fn_80178208(new (0, "", "", 0) chr::ControlOneMotion(target, 0x46, 0, 0, -1.0f, -1.0f));
    target->Reset_unk(0, 1, 1, 1);
    target->m58C = 0;
    fn_80178208(new (0, "", "", 0) chr::ControlSwapAttack(this, target));
}

// inferred: high jump attack toward `pos`
void CharaTask::fn_801588A0(HitInfo_unk* hit, const atn::Vector3& pos) {
    if (hit != 0) {
        m63C = m638;
        m638 = hit;
    }
    fn_80178208(new (0, "", "", 0) chr::ControlHighJumpAttack(this, pos, 100.0f, 0));
}

// inferred: nonzero when the motion of the running control has reached its end
bool CharaTask::fn_80158BB4() {
    if (m55C == 6) {
        switch (m560) {
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 0x8D:
            if (mMotion.mTracks[0].m00 == 0x162) {
                float frame = mMotion.mTracks[0].m08;
                return frame >= mMotion.fn_80097D7C(0) - 3.0f;
            } else {
                float frame = mMotion.mTracks[0].m08;
                return frame >= mMotion.fn_80097D7C(0);
            }
        case 0xB: {
            float frame = mMotion.mTracks[0].m08;
            return frame >= mMotion.fn_80097D7C(0) - 6.0f;
        }
        }
    }
    return true;
}

// inferred: no caller is left in the game (removed by the linker); it is what emits the weak copy
// of ControlBase::vf20 at this position. Name invented.
float CharaTask::GetControlF20_unk() {
    if (mControl) {
        return mControl->vf20();
    }
    return 0.0f;
}

int CharaTask::fn_80158CA4() {
    if (mControl) {
        return mControl->vf24();
    }
    return 0;
}

int CharaTask::vf78() {
    if (mControl) {
        return mControl->vf28();
    }
    return 0;
}

atn::Vector3 CharaTask::fn_80158E1C() {
    if (mControl) {
        return mControl->vf18();
    }
    return atn::Vector3(0.0f, 0.0f, 1.0f);
}

void CharaTask::fn_80158EF0(int a) {
    fn_80178208(new (0, "", "", 0) chr::ControlAttackSetup(this, a));
}

// inferred: name invented (inlined; static because its "" goes to the file pool)
static void StartWallUp_unk(CharaTask* chara, const atn::Vector3& pos, const atn::Vector3& normal) {
    chara->m13AC = 0;
    chara->fn_80178208(new (0, "", "", 0) chr::ControlWallUp(chara, pos, normal));
}

// inferred: climbs the wall found in direction `dir`
void CharaTask::fn_801591E8(const atn::Vector3& dir) {
    CollResult_unk result;
    atn::Vector3 from(mPos.x, 18.0f + mPos.y, mPos.z);
    atn::Vector3 to = from + dir * m620 * 2.0f;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000020, 0, 0)) {
        StartWallUp_unk(this, result.m04, -result.m28);
    }
}

void CharaTask::fn_801595BC(int a, int count, int motion, int releaseMotion) {
    fn_80178208(new (0, "", "", 0) chr::ControlCaught(this, a, count, motion, releaseMotion));
}

void CharaTask::fn_801598B4(int a, int b, int c) {
    fn_80178208(new (0, "", "", 0) chr::ControlChaseCaught(this, a, b, c));
}

void CharaTask::fn_80159BA4(int a) {
    fn_80178208(new (0, "", "", 0) chr::ControlChaseCatch(this, a));
}

void CharaTask::fn_80159E9C() {
    fn_80178208(new (0, "", "", 0) chr::ControlChaseAssist(this));
}

void CharaTask::fn_8015A184() {
    fn_80178208(new (0, "", "", 0) chr::ControlChaseFollow(this));
}

// inferred: knocked down along `dir` after a reset (returns to the previous state first)
void CharaTask::fn_8015A46C(const atn::Vector3& dir) {
    Reset_unk(0, 1, 1, 1);
    fn_8016E970(m564);
    mF1C = 0;
    if (mType == 0 && ((unsigned int)mStatus.m0C & 0x20) == 0x20) {
        vf44();
    }
    fn_8015E7A0(dir, -1, 0);
}

void CharaTask::fn_8015AC48(int motion) {
    fn_80178208(new (0, "", "", 0) chr::ControlRelease(this, motion));
}

void CharaTask::fn_8015AF40(int a) {
    fn_80178208(new (0, "", "", 0) chr::ControlMatHold(this, a));
}

void CharaTask::fn_8015B238() {
    fn_80178208(new (0, "", "", 0) chr::ControlMatHoldRelease(this));
}

void CharaTask::fn_8015B520() {
    fn_80178208(new (0, "", "", 0) chr::ControlMatHoldDamage(this));
}

void CharaTask::fn_8015B808(const atn::Vector3& dir, int a) {
    fn_80178208(new (0, "", "", 0) chr::ControlThrown(this, dir, a));
}

void CharaTask::fn_8015BAF0(int a, int b) {
    chr::ControlThrownAway* control = new (0, "", "", 0) chr::ControlThrownAway(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_8015BDD8(int a, const atn::Vector3& dir) {
    chr::ControlThrownAvoid* control = new (0, "", "", 0) chr::ControlThrownAvoid(this, a, dir);
    fn_80178208(control);
}

void CharaTask::fn_8015C0C0(int a, int b) {
    chr::ControlThreadCatch* control = new (0, "", "", 0) chr::ControlThreadCatch(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_8015C3A8(int a) {
    chr::ControlThreadDead* control = new (0, "", "", 0) chr::ControlThreadDead(this, a);
    fn_80178208(control);
}

void CharaTask::fn_8015C6A0() {
    chr::ControlThreadRelease* control = new (0, "", "", 0) chr::ControlThreadRelease(this);
    fn_80178208(control);
}

void CharaTask::fn_8015C988(int a, int b) {
    chr::ControlAbsorbedHP* control = new (0, "", "", 0) chr::ControlAbsorbedHP(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_8015CC70(const atn::Vector3& a, const atn::Vector3& b, const atn::Vector3& c) {
    chr::ControlCliffhangZanGurg* control = new (0, "", "", 0) chr::ControlCliffhangZanGurg(this, a, b, c);
    fn_80178208(control);
}

void CharaTask::fn_8015CF98(const atn::Vector3& a) {
    chr::ControlEndCliffhangZanGurg* control = new (0, "", "", 0) chr::ControlEndCliffhangZanGurg(this, a);
    fn_80178208(control);
}

void CharaTask::fn_8015D2A0(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlStickZanGurgStand* control = new (0, "", "", 0) chr::ControlStickZanGurgStand(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_8015D5B0(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlStickZanGurgCliff* control = new (0, "", "", 0) chr::ControlStickZanGurgCliff(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_8015D8C0(float f, const atn::Vector3& dir, int a) {
    if (m50 == 0xAAE77) {
        return;
    }
    chr::ControlGuard* control = new (0, "", "", 0) chr::ControlGuard(this, f, dir, a);
    fn_80178208(control);
}

void CharaTask::vf7C(int arg) {
    chr::ControlGuarded* control = new (0, "", "", 0) chr::ControlGuarded(this, arg);
    fn_80178208(control);
}

void CharaTask::fn_8015DEC0(int a) {
    chr::ControlGuardCounter* control = new (0, "", "", 0) chr::ControlGuardCounter(this, a);
    fn_80178208(control);
}

void CharaTask::fn_8015E1B8(const atn::Vector3& dir) {
    chr::ControlAvoid* control = new (0, "", "", 0) chr::ControlAvoid(this, dir);
    fn_80178208(control);
}

void CharaTask::fn_8015E4B0(const atn::Vector3& dir, int arg) {
    chr::ControlDamage* control = new (0, "", "", 0) chr::ControlDamage(this, dir, arg);
    fn_80178208(control);
    m638 = 0;
}

void CharaTask::fn_8015E7A0(const atn::Vector3& dir, int arg, int arg2) {
    chr::ControlKnockdown* control = new (0, "", "", 0) chr::ControlKnockdown(this, dir, arg, arg2);
    fn_80178208(control);
    m638 = 0;
    if (mFlag28 && lbl_8087F430 != 0) {
        lbl_8087F430->fn_803750E4(0xB5);
    }
    fn_80164DCC(1);
    if (m1208 != 0) {
        fn_801750FC();
    }
}

// NONMATCHING: 96% - the first motion-end check keeps the frame in f30 instead of f31 (the second
// copy matches); tried: one inline used twice, explicit code with locals in each block, at the top
// of the function, swapped declarations, bool result variable, if/else forms.
int CharaTask::fn_8015EB2C() {
    if (m55C != 6) {
        return 1;
    }
    float frame;
    float speed;
    if (mFlag2) {
        if (mFlag25) {
            return 1;
        }
        if (mMotion.mTracks[0].m18 == 0) {
            frame = mMotion.mTracks[0].m08;
            return frame >= mMotion.fn_80097D7C(0);
        }
        speed = mMotion.mTracks[0].m0C;
        frame = mMotion.mTracks[0].m08;
        return frame >= mMotion.fn_80097D7C(0) - speed;
    }
    chr::ControlBase* control = mControl.get();
    if (control != 0) {
        if (m560 == 0x43) {
            return ((chr::ControlStickSwordSp*)control)->fn_80196C68();
        }
        if (m560 == 0x17) {
            return ((chr::ControlKnockdown*)control)->fn_801AE41C();
        }
        if (m560 == 0x3B) {
            return ((chr::ControlFallDead*)control)->fn_801AF81C();
        }
        if (m560 == 0x8E) {
            return ((chr::ControlDamageLastOne*)control)->fn_801B0260();
        }
    }
    if (m574.y > -0.01f) {
        return IsMotionEnd_unk();
    }
    return 0;
}

void CharaTask::fn_8015ECC4() {
    chr::ControlGetUp* control = new (0, "", "", 0) chr::ControlGetUp(this);
    fn_80178208(control);
}

void CharaTask::fn_8015EFAC(int arg) {
    if ((unsigned int)(m60->m24 - 2) <= 1) {
        chr::ControlRevivalSkelton* control = new (0, "", "", 0) chr::ControlRevivalSkelton(this, 60);
        fn_80178208(control);
    } else {
        chr::ControlRevival* control = new (0, "", "", 0) chr::ControlRevival(this, arg);
        fn_80178208(control);
    }
}

extern "C" double cos(double x);
extern "C" double sin(double x);

// inferred: tactics manager (TacView); names invented
struct TacView_unk {
    unsigned char m00[0xA70];
    int mA70; // inferred: nonzero while the view is active (CharaTask::fn_8016DAB0)
};
extern "C" TacView_unk* lbl_8087F9F8;
extern "C" int fn_805ADCC0(TacView_unk* manager, CharaTask* chara, HitInfo_unk* hit, int arg);
extern "C" void fn_805ADD80(TacView_unk* manager, CharaTask* chara, HitInfo_unk* hit);
extern "C" HitInfo_unk* fn_805ADD3C(TacView_unk* manager, CharaTask* chara);

// inferred: helpers inlined in fn_8015F568 (names invented)
inline atn::Vector3 GetDir_unk(const float& yaw) {
    Mtx m;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    PSMTXRotRad(m, 'y', yaw);
    PSMTXMultVec(m, &dir, &dir);
    return dir;
}

inline atn::Vector3 GetSide_unk(const atn::Vector3& front) {
    atn::Vector3 side;
    side.fn_8013C394(atn::Vector3(0.0f, 1.0f, 0.0f), front);
    return side;
}

// inferred: names invented
inline bool IsDebugMode_unk(CharaTask* chara) {
    return chara->mType == 3 && lbl_8087F0A8->m98 != 0;
}

inline bool IsDebugField_unk(CharaTask* chara) {
    return IsDebugMode_unk(chara) && lbl_8087F430 != 0;
}

// NONMATCHING: 94% - register allocation only: f5/f7 swapped in dir * -1.5f, r24/r25 (&pos/&to)
// swapped, and the inlined bool chains (CanAct_unk on the redirected target, IsDebugField_unk) use
// r5/r0 where the original uses r0/r3. The ptmf constant offsets are the known lbl_8077A720 noise.
// tried: named vs helper-returned vectors, from/to order, accessors (fn_8013C458, fn_8013C3A8,
// fn_801404F8, fn_80139EF4, fn_8013C404, fn_80140518), bool helpers with result variables, int
// parameter for the type check.
void CharaTask::fn_8015F568(const atn::Vector3& dest, CharaTask* target, HitInfo_unk* hit) {
    if (m648 == 0) {
        return;
    }
    if (mFlag31) {
        int side = fn_80153B44();
        atn::Vector3 pos = mPos;
        atn::Vector3 from;
        atn::Vector3 to;
        if (side == 1) {
            pos.x += -5.0f * (float)cos(mRot.y);
            pos.z += 5.0f * (float)sin(mRot.y);
        } else if (side == 2) {
            pos.x += 5.0f * (float)cos(mRot.y);
            pos.z += -5.0f * (float)sin(mRot.y);
        } else {
            pos += GetDir_unk(mRot.y) * -1.5f;
        }
        pos += GetSide_unk(mC14) * 2.0f;
        from = mPos;
        to = pos;
        from.y += 5.0f;
        to.y += 5.0f;
        void* colli = lbl_8087EE98;
        if (fn_8004ED34(colli, 0, &from, &to, fn_80179D44() | 0x80000000, 0, 0) == 0) {
            mPos = pos;
        }
        mFlag31 = 0;
        mFlag30 = 0;
        RouteTable_unk* table = lbl_8087F430->fn_8013C504();
        if (table != 0 && mC38 > 0 && mC3C > 0) {
            table->GetLink_unk(mC38)->m3C = 0;
            table->GetLink_unk(mC3C)->m3C = 0;
        }
        if (fn_8017B16C()) {
            if (fn_80370174(lbl_8087F430, 0x25) >= 1) {
                if (m648 != 0) {
                    fn_80044E0C(m648, 1);
                }
                if (m64C != 0) {
                    fn_80044E0C(m64C, 1);
                }
                m674 = -1;
                m648 = 0;
                m64C = 0;
                fn_8014C228();
            } else if (m674 >= 0) {
                fn_8014DEE4(0, 0, 0);
            }
        }
    }
    if (hit == 0) {
        return;
    }
    MagicTarget_unk* redirect = lbl_8087EE68 != 0 ? fn_800185B4(lbl_8087EE68, this, hit) : 0;
    if (redirect != 0 && redirect->m08 == 0 && redirect->m0C != 0) {
        if ((redirect->m0C->m54C & 0x2000) != 0x2000 && redirect->m0C->CanAct_unk() &&
            (mStatus.m0C & 0x100) != 0x100 && (mStatus.m0C & 0x10000) != 0x10000) {
            target = redirect->m0C;
        } else if (redirect->m0C == this) {
            target = redirect->m0C;
        } else if (mC58.mC4 != 0) {
            target = mC58.mC4;
        }
    }
    bool debug = IsDebugField_unk(this) && lbl_8087F430->fn_803761BC() != 0; // inferred: unused
    mFB8 = 0.0f;
    m63C = m638;
    m638 = hit;
    mFBC = hit->mC0;
    if (fn_8013A158() && mFlag22 != 1 && mFlagB10 && hit != 0) {
        if (fn_8021A8D0(hit) != 0 || fn_8021A918(hit) != 0) {
            mFBC = 30.0f;
        }
    }
    if (lbl_8087F9F8 != 0 && fn_805ADCC0(lbl_8087F9F8, this, m638, 0) != 0) {
        if ((m638->mAC & 0x40000) != 0x40000) {
            mFBC = 90.0f;
        }
        fn_805ADD80(lbl_8087F9F8, this, m638);
    }
    mF60 = mPos;
    mF7C = target;
    mF6C = dest;
    mF78 = m638->m58;
    if ((int)m638->m02 == 4) {
        mF7C = this;
        mF6C = mPos;
    }
    if (mFBC <= 0.0f) {
        m63C = m638;
        fn_801603CC(1.0f, false);
    } else {
        chr::ControlChant* control = new (0, "", "", 0) chr::ControlChant(this, mF7C, mF6C, (int)mFBC, mF78);
        fn_80178208(control);
    }
}

extern "C" int fn_80219558(int id);

// NONMATCHING: 92% - the inlined IsDebugField_unk chain uses r0/r5 where the original uses r3/r0
// (same issue as fn_8015F568); tried: int/bool helpers, result variables, int type parameter,
// local copies of the globals.
void CharaTask::fn_80160170(HitInfo_unk* hit, CharaTask* target) {
    if (fn_80160324()) {
        bool debug = false;
        if (IsDebugField_unk(this) && lbl_8087F430->fn_803761BC() != 0) {
            debug = true;
        }
        if (debug) {
            MagicTarget_unk* redirect = lbl_8087EE68 != 0 ? fn_800185B4(lbl_8087EE68, this, hit) : 0;
            bool redirected = fn_80219558(m50) == 4 && redirect != 0; // inferred: unused
        }
        m63C = m638;
        m638 = hit;
        mFBC = hit->mC0;
        mF78 = hit->m58;
        if (fn_8021A888(hit) != 0) {
            mF7C = 0;
        } else {
            mF7C = target;
        }
        if ((int)m638->m02 == 4) {
            mF7C = this;
            mF6C = mPos;
        }
        chr::ControlChant* control = (chr::ControlChant*)mControl.get();
        control->m38 = (int)mFBC;
        control->m34 = mF7C;
    }
}

bool CharaTask::fn_80160324() {
    bool result = false;
    if (m55C == 6 && m560 == 0x1D) {
        result = true;
    }
    return result;
}

void CharaTask::fn_8016034C() {
    Effect_unk* effect = lbl_8087F048;
    if (effect != 0) {
        effect->fn_801059F8(this);
        lbl_8087F048->fn_80105B3C(this);
    }
    mFlagB8 = 0;
    mFB8 = 0.0f;
    mFBC = 0.0f;
    vf8C(chr::lbl_807C7030);
}

void CharaTask::fn_801603CC(float f, bool b) {
    chr::ControlMagic* control = new (0, "", "", 0) chr::ControlMagic(this, mF7C, f, mF6C, b);
    fn_80178208(control);
}

void CharaTask::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    if ((unsigned int)(m60->m24 - 2) <= 1) {
        chr::ControlAppearSkelton* control = new (0, "", "", 0) chr::ControlAppearSkelton(this, pos);
        fn_80178208(control);
    } else if (m60->m24 == 0x2C) {
        chr::ControlAppearCoffin* control = new (0, "", "", 0) chr::ControlAppearCoffin(this, pos);
        fn_80178208(control);
    } else if (mMotion.fn_80097D40(0x2B)) {
        chr::ControlAppear* control = new (0, "", "", 0) chr::ControlAppear(this, pos, offset);
        fn_80178208(control);
    }
}

void CharaTask::vf84(int motion, float blend) {
    chr::ControlAppearEnd* control = new (0, "", "", 0) chr::ControlAppearEnd(this, motion, blend);
    fn_80178208(control);
}


void CharaTask::fn_8016125C(const SkillRef_unk* skill, CharaTask* target, int motion, const char* name, float f) {
    chr::ControlUseSkill* control =
        new (0, "", "", 0) chr::ControlUseSkill(this, fn_80219E6C(skill->mId), target, motion, name, f);
    fn_80178208(control);
}

void CharaTask::fn_80161570(int motion, int a, int b, float c, float d) {
    chr::ControlOneMotion* control = new (0, "", "", 0) chr::ControlOneMotion(this, motion, a, b, c, d);
    fn_80178208(control);
}

void CharaTask::fn_80161880(int motion, int target, int a) {
    chr::ControlOneMotionTarget* control = new (0, "", "", 0) chr::ControlOneMotionTarget(this, motion, target, a);
    fn_80178208(control);
}

void CharaTask::fn_80161B70(int motion, const atn::Vector3& pos) {
    chr::ControlOneMotionTransition* control = new (0, "", "", 0) chr::ControlOneMotionTransition(this, motion, pos);
    fn_80178208(control);
}

void CharaTask::fn_80161E58() {
    chr::ControlDuckUnderA* control = new (0, "", "", 0) chr::ControlDuckUnderA(this);
    fn_80178208(control);
}

void CharaTask::fn_80162140(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlDuckUnderB* control = new (0, "", "", 0) chr::ControlDuckUnderB(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_80162428(const atn::Vector3& rot) {
    chr::ControlDuckJoist* control = new (0, "", "", 0) chr::ControlDuckJoist(this, rot);
    fn_80178208(control);
}

void CharaTask::fn_80162720(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlAlleyWay* control = new (0, "", "", 0) chr::ControlAlleyWay(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_80162A08(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlLadder* control = new (0, "", "", 0) chr::ControlLadder(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_80162CF0(const atn::Vector3& a, const atn::Vector3& b, float f) {
    chr::ControlConstrict* control = new (0, "", "", 0) chr::ControlConstrict(this, a, b, f);
    fn_80178208(control);
}

void CharaTask::fn_80162FE8(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlBumpHead* control = new (0, "", "", 0) chr::ControlBumpHead(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_801632D0(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlStepOn* control = new (0, "", "", 0) chr::ControlStepOn(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_801635B8(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlThroughRope* control = new (0, "", "", 0) chr::ControlThroughRope(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_801638A0(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlCrawling* control = new (0, "", "", 0) chr::ControlCrawling(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_80163B88(const atn::Vector3& a, const atn::Vector3& b) {
    chr::ControlDiveIntoWater* control = new (0, "", "", 0) chr::ControlDiveIntoWater(this, a, b);
    fn_80178208(control);
}

void CharaTask::fn_80163E70(CharaTask* rider) {
    chr::ControlRideBeast* control = new (0, "", "", 0) chr::ControlRideBeast(this, rider);
    fn_80178208(control);
}

void CharaTask::fn_80164168(int a, int b) {
    chr::ControlBoundShade* control = new (0, "", "", 0) chr::ControlBoundShade(this, a, b, 15);
    fn_80178208(control);
}

// inferred: accessor (name invented); keeps the manager reloaded after the first test
inline Effect_unk* GetEffect_unk() { return lbl_8087F048; }

// inferred: starts the bullet `id` (this is unused)
void CharaTask::fn_80164454(int id, int a, int b) {
    Effect_unk* mgr = lbl_8087F048;
    if (mgr != 0) {
        void* bullet;
        if (id > 0) {
            bullet = fn_80219E6C(id);
        } else {
            bullet = 0;
        }
        fn_800FD658(GetEffect_unk(), a, b, bullet, fn_800F8548(mgr), 0, 30);
    }
}

void CharaTask::fn_801644D4(const atn::Vector3& start, CharaTask* target) {
    chr::ControlJumpAt* control = new (0, "", "", 0) chr::ControlJumpAt(this, start, target);
    fn_80178208(control);
}

void CharaTask::fn_801647BC() {
    chr::ControlAttractHateCharge* control = new (0, "", "", 0) chr::ControlAttractHateCharge(this);
    fn_80178208(control);
    if (lbl_8087F430 != 0) {
        if (fn_80370174(lbl_8087F430, 0x135) == 0) {
            fn_80370320(lbl_8087F430, 0x135, 1, 0);
        } else if (fn_80370174(lbl_8087F430, 0x136) == 0) {
            fn_80370320(lbl_8087F430, 0x136, 1, 0);
        }
    }
}

// inferred: argument of fn_80164B00, copied to CharaTask::mFC8-mFD8 (names invented)
struct CharaShade_unk {
    float m00; // inferred: also copied to CharaStatus::m20/m24
    float m04; // inferred: timer counted down by fn_80164D24
    float m08; // inferred: timer counted down by fn_80164D24
    float m0C; // inferred: timer counted down by fn_80164D24
    int m10;   // inferred: overwritten with Field_unk::m5744
};

extern "C" void fn_80105F4C(Effect_unk* effect, CharaTask* chara, chr::Motion_unk* motion);
extern "C" void fn_8010337C(Effect_unk* effect, int arg, CharaTask* chara);

void CharaTask::fn_80164B00(const CharaShade_unk& shade) {
    mFC8.x = shade.m00;
    mFC8.y = shade.m04;
    mFC8.z = shade.m08;
    mFD4 = shade.m0C;
    mFD8 = shade.m10;
    mFD8 = (lbl_8087F430 != 0) ? lbl_8087F430->m5744 : 0;
    mFlag28 = 1;
    mFlag27 = 0;
    fn_80105F4C(lbl_8087F048, this, &mMotion);
    mStatus.m20 = mFC8.x;
    mStatus.m24 = mFC8.x;
    if (lbl_8087F048 != 0) {
        fn_8010337C(lbl_8087F048, 0, this);
    }
    if (lbl_8087F430 != 0 && fn_80375184(lbl_8087F430) == 0 && fn_80370174(lbl_8087F430, 0xD6) == 1) {
        Player_unk* player = (lbl_8087F8A0 != 0) ? (Player_unk*)lbl_8087F8A0->mHead : 0;
        for (; player != 0; player = player->mNext) {
            if (player->CanAct_unk() && (player->m54C & 0x2000) != 0x2000) {
                int kind = fn_80219558(player->m50);
                if (kind == 11 || kind == 14) {
                    player->m1470 = PlayerState_unk(9, 90);
                }
            }
        }
    }
}

void CharaTask::fn_80164D24() {
    if (!mFlag28) {
        return;
    }
    float step = lbl_8087EFA8->m3A4;
    if (mFD4 > 0.0f) {
        mFD4 -= step;
        if (mFD4 <= 0.0f) {
            m1028 = lbl_8087F0A8->m250;
        }
    }
    if (mFC8.y > 0.0f) {
        mFC8.y -= step;
        return;
    }
    if (mFC8.z > 0.0f) {
        mFC8.z -= step;
    }
    if (mFC8.z <= 0.0f && lbl_8087F0A8->m3D0 != 0) {
        mStatus.m20 = 0.0f;
        fn_80164DCC(1);
    }
}

extern "C" void fn_8010607C(Effect_unk* effect, CharaTask* chara);
extern "C" void fn_80105F3C(Effect_unk* effect, CharaTask* chara);
extern "C" void fn_8010608C(Effect_unk* effect, CharaTask* chara, chr::Motion_unk* motion);
extern "C" void fn_8010652C(Effect_unk* effect, CharaTask* chara);

// inferred: ends the shade started by fn_80164B00 (`arg`: with effect and sound)
void CharaTask::fn_80164DCC(int arg) {
    if (!mFlag28 && !(m55C == 6 && m560 == 0x2E)) {
        return;
    }
    if (m55C == 6 && m560 == 0x2E) {
        fn_8016EB48(0, 0, 0);
    }
    mFlag28 = 0;
    mFlag27 = 0;
    mStatus.m20 = 0.0f;
    mStatus.m24 = 0.0f;
    mStatus.m42C = 0.0f;
    mFC8.x = 0.0f;
    mFC8.y = 0.0f;
    mFC8.z = 0.0f;
    mFD4 = 0.0f;
    m1028 = 1;
    fn_8010607C(lbl_8087F048, this);
    fn_80105F3C(lbl_8087F048, this);
    if (arg != 0) {
        fn_8010608C(lbl_8087F048, this, &mMotion);
        fn_800C344C(sSoundNames_unk[2], &mPos, 0, -1, 1.0f);
    }
    fn_8010652C(lbl_8087F048, this);
}

// inferred: returns a record whose m4C is compared with the shade timer (unknown type)
struct Record80210220_unk {
    unsigned char m00[0x4C];
    int m4C;
};
extern "C" Record80210220_unk* fn_80210220(int id);

// inferred: remaining shade time (0 without a shade)
int CharaTask::fn_80164EF4() {
    if (mFlag28) {
        Record80210220_unk* rec = fn_80210220(lbl_8087F0A8->m3CC);
        int elapsed = (int)mFC8.z;
        return rec->m4C - elapsed;
    }
    return 0;
}

// inferred: starts chr::ControlDetect while idle (m55C 2) or detecting (control 0x2F)
void CharaTask::fn_80164F54(int arg) {
    if (!mFlagB4 && ((m55C == 6 && m560 == 0x2F) || m55C == 2) && mMotion.mTracks[0].m00 != 0x1ED) {
        chr::ControlDetect* control = new (0, "", "", 0) chr::ControlDetect(this, arg);
        fn_80178208(control);
    }
}

// inferred: starts chr::ControlDetectHateAtt when `target` has a shade (mFlag28)
void CharaTask::fn_80165284(CharaTask* target) {
    int can = 0;
    int ok = 0;
    if (m55C == 2) {
        can = 1;
    } else if (m55C == 6) {
        switch (m560) {
        case 2:
        case 4:
        case 12:
        case 20:
        case 22:
        case 31:
        case 47:
        case 97:
        case 98:
        case 99:
            can = 1;
            break;
        case 21:
            break;
        }
    }
    if (mFlag22 == 1 || (m54C & 0x2000000) == 0x2000000 || mFlag3) {
        can = 0;
    }
    if (m138A > 0) {
        can = 0;
    }
    if (can && target != 0) {
        ok = target->mFlag28;
        if (mMotion.fn_80097D40(0x1ED) == 0) {
            ok = 0;
        }
        if (mC58.mB4 > 0 && target->mC58.mB4 > 0 && mC58.mB4 != target->mC58.mB4) {
            ok = 0;
        }
    }
    if (can && ok) {
        chr::ControlDetectHateAtt* control = new (0, "", "", 0) chr::ControlDetectHateAtt(this);
        fn_80178208(control);
    }
}

// inferred: looks around for `time` frames (kind 0x18 uses chr::ControlLookAroundEgg for 90 frames)
void CharaTask::fn_801656A4(int time) {
    if (m60->m24 == 0x18) {
        chr::ControlLookAroundEgg* control = new (0, "", "", 0) chr::ControlLookAroundEgg(this, 90);
        fn_80178208(control);
    } else {
        chr::ControlLookAround* control = new (0, "", "", 0) chr::ControlLookAround(this, time);
        fn_80178208(control);
    }
}

// inferred: shoots the bullet `id` (called from BattleCharaTask)
void CharaTask::fn_80165C5C(int id, const atn::Vector3& a, const atn::Vector3& b) {
    if (id >= 0) {
        chr::ControlShoot* control = new (0, "", "", 0) chr::ControlShoot(this, id, a, b);
        fn_80178208(control);
    }
}

void CharaTask::fn_80165F54(const atn::Vector3& pos) {
    chr::ControlClimbDeck* control = new (0, "", "", 0) chr::ControlClimbDeck(this, pos);
    fn_80178208(control);
}

// inferred: turns to stone (status 0x40, see CharaStatus)
void CharaTask::fn_8016624C(int a) {
    chr::ControlCdnStone* control = new (0, "", "", 0) chr::ControlCdnStone(this, a);
    fn_80178208(control);
}

// inferred: recovers from stone (m58C 3 is the stone state)
void CharaTask::fn_80166544() {
    if (m58C == 3) {
        m58C = 0;
        fn_8016E970(m564);
    }
    mStatus.fn_8013322C(0x40);
    mF94 = -1;
}

void CharaTask::fn_80166760() {
    mF94 = mF98;
}

void CharaTask::fn_8016676C() {
    chr::ControlCdnParalyzed* control = new (0, "", "", 0) chr::ControlCdnParalyzed(this);
    fn_80178208(control);
}

void CharaTask::fn_80166A54() {
    chr::ControlCdnRestoreParalyzed* control = new (0, "", "", 0) chr::ControlCdnRestoreParalyzed(this);
    fn_80178208(control);
}

void CharaTask::fn_80166D3C(int a) {
    chr::ControlCdnBetray* control = new (0, "", "", 0) chr::ControlCdnBetray(this, a, 0);
    fn_80178208(control);
}

// inferred: recovers from status 0x100 (the shared part is Recover_unk, also inlined in fn_80175B18)
void CharaTask::fn_80167038() {
    mStatus.fn_8013322C(0x100);
    fn_80107850(lbl_8087F048, &mMotion);
    Recover_unk(this);
}

void CharaTask::fn_80167344(int a) {
    chr::ControlCdnBetray* control = new (0, "", "", 0) chr::ControlCdnBetray(this, a, 1);
    fn_80178208(control);
}

// inferred: recovers from status 0x10000
void CharaTask::fn_80167640() {
    mStatus.fn_8013322C(0x10000);
    fn_80107B20(lbl_8087F048, &mMotion);
    Recover_unk(this);
}

void CharaTask::fn_8016794C() {
    chr::ControlCdnSleep* control = new (0, "", "", 0) chr::ControlCdnSleep(this);
    fn_80178208(control);
}

void CharaTask::fn_80167C34() {
    chr::ControlCdnRestoreSleep* control = new (0, "", "", 0) chr::ControlCdnRestoreSleep(this);
    fn_80178208(control);
}

extern "C" void fn_80107908(Effect_unk* effect, Model_unk* model, chr::Motion_unk* motion);

// inferred: saves the camera mode (mFlag19) and switches the camera to mode 1
void CharaTask::fn_80167F1C() {
    if (lbl_8087F048 != 0) {
        fn_80107908(lbl_8087F048, &mMotion, &mMotion);
    }
    if (mType == 0) {
        mFlag20 = 1;
        GameState_unk* state = lbl_8087EFA8;
        int mode = state->m324.m00;
        CameraState_unk cam(1, state->m324, 1);
        mFlag19 = mode;
        lbl_8087EFA8->m324 = cam;
    }
}

// inferred: restores the camera mode saved by fn_80167F1C (also inlined in vf48)
// NONMATCHING: ~45% by line (same instructions, different registers/schedule) - the original keeps
// &state->m324.m34 in a register (addi r5,r10,0x358) for the read and the write of m34; tried:
// temporary or named CameraState_unk, reloading lbl_8087EFA8, a reference/pointer to m324, a mode
// local, explicit operator= and copy constructor in CameraState_unk.
void CharaTask::fn_80168048() {
    if (lbl_8087F048 != 0) {
        fn_801079B0(lbl_8087F048, &mMotion);
    }
    if (mFlag20 && mType == 0) {
        mFlag20 = 0;
        GameState_unk* state = lbl_8087EFA8;
        state->m324 = CameraState_unk(mFlag19, state->m324);
    }
}

extern "C" void fn_80107E68(Effect_unk* effect, Model_unk* model, chr::Motion_unk* motion);

void CharaTask::fn_80168170() {
    if (lbl_8087F048 != 0) {
        fn_80107E68(lbl_8087F048, &mMotion, &mMotion);
    }
}

void CharaTask::fn_80168190() {
    if (lbl_8087F048 != 0) {
        fn_80107F10(lbl_8087F048, &mMotion);
    }
}

void CharaTask::fn_801681B0(const atn::Vector3& dir, int time, int flag) {
    chr::ControlCdnPiyo* control = new (0, "", "", 0) chr::ControlCdnPiyo(this, dir, time, flag);
    fn_80178208(control);
}

void CharaTask::fn_801684A0() {
    chr::ControlCdnRestorePiyo* control = new (0, "", "", 0) chr::ControlCdnRestorePiyo(this);
    fn_80178208(control);
}

extern "C" void fn_80107B30(Effect_unk* effect, Model_unk* model, chr::Motion_unk* motion);
extern "C" void fn_80107274(Effect_unk* effect, CharaTask* chara);

void CharaTask::fn_80168788() {
    if (lbl_8087F048 != 0) {
        fn_80107B30(lbl_8087F048, &mMotion, &mMotion);
        fn_80107274(lbl_8087F048, this);
    }
}

void CharaTask::fn_801687D8() {
    if (lbl_8087F048 != 0) {
        fn_80107BD8(lbl_8087F048, &mMotion);
        fn_80107380(lbl_8087F048, this);
    }
}

void CharaTask::fn_80168824(int flag) {
    chr::ControlCdnPsycho* control = new (0, "", "", 0) chr::ControlCdnPsycho(this, flag);
    fn_80178208(control);
}

void CharaTask::fn_80168B1C() {
    chr::ControlCdnRestorePsycho* control = new (0, "", "", 0) chr::ControlCdnRestorePsycho(this);
    fn_80178208(control);
}

extern "C" void fn_80107CEC(Effect_unk* effect, Model_unk* model, chr::Motion_unk* motion, float f);

void CharaTask::fn_80168E04() {
    Effect_unk* effect = lbl_8087F048;
    if (effect != 0) {
        fn_80107CEC(effect, &mMotion, &mMotion, vf90());
    }
}

void CharaTask::fn_80168E60() {
    if (lbl_8087F048 != 0) {
        fn_80107D94(lbl_8087F048, &mMotion);
    }
}

extern "C" void fn_801079C0(Effect_unk* effect, Model_unk* model, chr::Motion_unk* motion);

void CharaTask::fn_80168E80() {
    if (lbl_8087F048 != 0) {
        fn_801079C0(lbl_8087F048, &mMotion, &mMotion);
    }
}

void CharaTask::fn_80168EA0() {
    if (lbl_8087F048 != 0) {
        fn_80107A68(lbl_8087F048, &mMotion);
    }
}

extern "C" void fn_80107DA4(Effect_unk* effect, Model_unk* model, chr::Motion_unk* motion, float f,
                            const atn::Vector3& v);

void CharaTask::fn_80168EC0() {
    Effect_unk* effect = lbl_8087F048;
    if (effect != 0) {
        fn_80107DA4(effect, &mMotion, &mMotion, vf90(), vf94());
    }
}

void CharaTask::fn_80168F3C() {
    if (lbl_8087F048 != 0) {
        fn_80107E58(lbl_8087F048, &mMotion);
    }
}

void CharaTask::fn_80168F5C() {
    mStatus.fn_8012B3E8();
    mStatus.fn_8012B988();
}

void CharaTask::fn_80168F94() {
    mStatus.fn_8012B3E8();
    mStatus.fn_8012B988();
}

// inferred: falls down backwards (knock down along the back direction)
void CharaTask::vf88() {
    atn::Vector3 dir(0.0f, 0.0f, -0.001f);
    float rot = mRot.y;
    Mtx m;
    PSMTXRotRad(m, 'y', rot);
    PSMTXMultVec(m, &dir, &dir);
    vf44();
    fn_8015E7A0(dir, -1, 0);
}

namespace chr {
// inferred: controls created by CharaTask 0x8016939C-0x8016AFE8; class names from the vtables stored by
// the constructors, sizes from the operator new calls
} // namespace chr

extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)

// inferred: hangs from a cliff
void CharaTask::fn_8016939C(int a, int b, float f) {
    fn_8016DA4C();
    fn_80178208(new (0, "", "", 0) chr::ControlCliffhang(this, a, b, f));
}

// inferred: leaves the cliffhang control and restores the previous state
void CharaTask::fn_801696E0() {
    if (m55C == 6 && m560 == 0x3A) {
        fn_8016E970(m564);
    }
}

// inferred: falls dead (enemies also notify BattleCharaTask)
void CharaTask::fn_801698E4(const atn::Vector3& rot) {
    fn_80178208(new (0, "", "", 0) chr::ControlFallDead(this, rot));
    mFlag16 = 0;
    if (mType == 2 && (m54C & 0x400) != 0x400) {
        fn_800EE360(this);
    }
}

// inferred: drops down
void CharaTask::vf8C(const atn::Vector3& v) {
    fn_80178208(new (0, "", "", 0) chr::ControlDropDown(this, v));
}

void CharaTask::fn_80169F04(const atn::Vector3& pos, float rotY) {
    fn_80178208(new (0, "", "", 0) chr::ControlGmkCoffinOpening(this, pos, rotY));
}

void CharaTask::fn_8016A20C(const atn::Vector3& pos, int alt, float rotY) {
    fn_80178208(new (0, "", "", 0) chr::ControlGmkCoffinOpen(this, pos, alt, rotY));
}

void CharaTask::fn_8016A504(const atn::Vector3& pos, const atn::Vector3& rot) {
    fn_80178208(new (0, "", "", 0) chr::ControlGmkCoffinGetUp(this, pos, rot));
}

bool CharaTask::fn_8016A7EC() {
    if (m55C == 6 && m560 == 0x5D) {
        return false;
    }
    return true;
}

void CharaTask::fn_8016A814(float damage) {
    fn_80178208(new (0, "", "", 0) chr::ControlGmkTrapBat(this, damage));
}

void CharaTask::fn_8016AB0C(chr::GmkTarget_unk* target, void* arg) {
    fn_80178208(new (0, "", "", 0) chr::ControlGmkBreakArmor(this, target, arg));
}

// inferred: same side by type (players and type 3 together); name invented
inline bool IsSameTypeOf_unk(int a, int b) {
    return a == b || (a == 0 && b == 3) || (a == 3 && b == 0);
}

// inferred: `other` stands in front of this character, on the other side, and this character can
// play the swap motion 0x46
bool CharaTask::fn_8016ADF4(CharaTask* other) {
    if (IsPartyMember_unk() || IsSameTypeOf_unk(mType, other->mType)) {
        return false;
    }
    if (lbl_8087F610 != 0) {
        return false;
    }
    if (!mMotion.fn_80097D40(0x46)) {
        return false;
    }
    if (mFlag31) {
        return false;
    }
    if (mStatus.IsState_unk(0x20)) {
        return false;
    }
    if (mStatus.mParamB.fn_801404E0(0x80)) {
        return false;
    }
    if (mFlag22 == 1) {
        return false;
    }
    if (m55C == 6 && !((unsigned int)(m560 - 0x14) <= 2 || m560 == 4)) {
        return false;
    }
    Mtx m;
    atn::Vector3 diff = other->mPos - mPos;
    if (atn::Abs_unk(diff.y) > 3.0f) {
        return false;
    }
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    PSMTXRotRad(m, 'y', mRot.y);
    PSMTXMultVec(m, &front, &front);
    return !(PSVECDotProduct(&diff, &front) < 0.5f);
}

// inferred: the character found ahead along `dir` if it can swap places with `chara` (name invented)
inline CharaTask* FindSwapTarget_unk(CharaTask* chara, const atn::Vector3& dir) {
    CollResult_unk result;
    atn::Vector3 from(chara->mPos);
    from.y += 5.0f;
    atn::Vector3 to = from + dir * 20.0f;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x20, &chara->mColli, 0) && result.m38 != 0 &&
        (result.m38->m08 & 2) == 2) {
        CharaTask* target = result.m38->m0C;
        if (target->fn_8016ADF4(chara)) {
            return target;
        }
    }
    return 0;
}

// inferred: swaps places with the character found ahead along `dir`
bool CharaTask::fn_8016AFE8(const atn::Vector3& dir) {
    CharaTask* target = FindSwapTarget_unk(this, dir);
    if (target == 0) {
        return false;
    }
    target->fn_80178208(new (0, "", "", 0) chr::ControlOneMotion(target, 0x46, 0, 0, -1.0f, -1.0f));
    target->Reset_unk(0, 1, 1, 1);
    target->m58C = 0;
    mFC0 = target;
    fn_80178208(new (0, "", "", 0) chr::ControlSwapPos(this, dir));
    return true;
}

// inferred: pushed down along `dir` (also inlined in fn_8013CB68)
void CharaTask::fn_8016B8D4(const atn::Vector3& dir, int a, int b, int c) {
    fn_80178208(new (0, "", "", 0) chr::ControlPushedDown(this, dir, a, b, c));
}

// inferred: staggers back (motion 0x52)
void CharaTask::fn_8016BBCC(const atn::Vector3* dir, int a, int b) {
    if (mMotion.fn_80097D40(0x52) && m139C == 0) {
        fn_80178208(new (0, "", "", 0) chr::ControlOttotto(this, dir, a, b));
    }
}

void CharaTask::fn_8016BEDC(CharaTask* other, atn::Vector3 v) {
    fn_80178208(new (0, "", "", 0) chr::ControlThrust(this, other, v, 1));
}

void CharaTask::fn_8016C1E0(CharaTask* other) {
    fn_80178208(new (0, "", "", 0) chr::ControlThrustForCrab(this, other));
}

void CharaTask::fn_8016C4D8(int a, const atn::Vector3& v) {
    fn_80178208(new (0, "", "", 0) chr::ControlThrustForKraken(this, a, v));
}

void CharaTask::fn_8016C7D8(int a) {
    fn_80178208(new (0, "", "", 0) chr::ControlStickByQuark(this, a));
}

void CharaTask::fn_8016CAD0() {
    fn_80178208(new (0, "", "", 0) chr::ControlAbsorbed(this));
}

void CharaTask::fn_8016CDB8(int a, int b) {
    fn_80178208(new (0, "", "", 0) chr::ControlStagger(this, a, b));
}

// inferred: slips (also inlined in fn_8013CB68)
void CharaTask::fn_8016D0A0(int a) {
    fn_801750FC();
    fn_80178208(new (0, "", "", 0) chr::ControlSlip(this, a));
}

// inferred: names invented
inline bool IsMotion8E_unk(CharaTask* chara) {
    bool result = false;
    if (chara->m560 == 0x27 && chara->mMotion.mTracks[0].m00 == 0x8E) {
        result = true;
    }
    return result;
}
inline bool IsControl63_unk(CharaTask* chara) {
    bool result = true;
    if (chara->m560 != 0x63 && !IsMotion8E_unk(chara)) {
        result = false;
    }
    return result;
}
// NONMATCHING: ~48% by line (12/23, same instructions) - the outer and middle bools get r6/r5 instead
// of r5/r6; tried a plain || expression, int-parameter helpers, nested ifs, an || helper, an outer
// inline helper, int result, a ternary, an && helper.
bool CharaTask::fn_8016D3F8() {
    bool result = false;
    if (m55C == 6 && IsControl63_unk(this)) {
        result = true;
    }
    return result;
}

void CharaTask::fn_8016D454(float angle) {
    fn_80178208(new (0, "", "", 0) chr::ControlTurnQuadruped(this, angle));
}

// inferred: charges the attack `hit`
void CharaTask::fn_8016D74C(HitInfo_unk* hit, int a, int b) {
    if (hit != 0) {
        SetHit_unk(hit);
        fn_80178208(new (0, "", "", 0) chr::ControlCharge(this, a, b, 0));
    }
}

void CharaTask::fn_8016DA4C() {
    ClearEffect_unk();
}

// inferred: remaining chant time in frames, rounded for the tactics view
// NONMATCHING: ~93% (6/89, same instructions) - mFBC is loaded before mFB8 and the second fctiwz store
// gets its own stack slot; tried MAX in both orders, a named float/int local, an inline helper, an
// if/else, a negated difference, MIN(result, 89).
int CharaTask::fn_8016DAB0() {
    int result = 0;
    if (fn_80160324() && m638 != 0) {
        result = MAX((int)(mFBC - mFB8), 0);
        if (lbl_8087F9F8 != 0 && lbl_8087F9F8->mA70 != 0) {
            if (fn_805ADCC0(lbl_8087F9F8, this, m638, 1) && (m638->mAC & 0x40000) != 0x40000) {
                result = MIN(89, result);
            }
            result = result / 30 * 30 + 15;
        }
    } else if (lbl_8087F9F8 != 0 && lbl_8087F9F8->mA70 != 0) {
        HitInfo_unk* hit = fn_805ADD3C(lbl_8087F9F8, this);
        if (hit != 0) {
            result = 75;
            if ((hit->mAC & 0x40000) == 0x40000) {
                result = hit->mC0 - 15;
            }
        }
    }
    return result;
}

extern "C" void fn_803E6BB4(Unk8087F490_unk* obj, CharaTask* target);

void CharaTask::fn_8016DC14(CharaTask* target, int quiet) {
    if (mFC4 != target) {
        mFC4 = target;
        if (lbl_8087F490 != 0) {
            fn_803E6BB4(lbl_8087F490, target);
            if (quiet == 0) {
                if (target != 0) {
                    fn_800C31F4(sSoundNames_unk[8], 0, -1, 1.0f);
                } else {
                    fn_800C31F4(sSoundNames_unk[9], 0, -1, 1.0f);
                }
            }
        }
    }
}

bool CharaTask::fn_8016DCD4() {
    return CanReact_unk();
}

bool CharaTask::fn_8016DDB0(int force) {
    if (force == 0 && !fn_8016DCD4()) {
        return false;
    }
    if (mFlag6 || m1208 != 0 || mFlag3 || mF54 != 0 || m139C != 0) {
        return false;
    }
    if (mType == 0) {
        if (m55C != 1) {
            return false;
        }
    } else if (m55C != 2 || mC58.m2B4 > 0) {
        return false;
    }
    if (mFlagB9) {
        return false;
    }
    return true;
}

// inferred: changes the character type; leaving or becoming the player (type 0) swaps the states
// 1 and 2 of m55C/m564 and resets the character
void CharaTask::fn_8016DF3C(int type) {
    if (mType != type) {
        if (mType == 0) {
            if (m55C == 1) {
                m55C = 2;
            }
            if (m564 == 1) {
                m564 = 2;
            }
            Reset_unk(0, 1, 1, 1);
        } else if (type == 0) {
            if (m55C == 2) {
                m55C = 1;
            }
            if (m564 == 2) {
                m564 = 1;
            }
            Reset_unk(0, 1, 1, 1);
        }
        m4C = mType;
        mType = type;
    }
}

// inferred: valid party index of the character id m50
int CharaTask::fn_8016E484() {
    return IsPartyIndex_unk(fn_80219558(m50));
}

extern "C" int fn_80044C50(chr::CharaPart_unk* part, CharaTask* chara);
extern "C" void fn_80044CCC(chr::CharaPart_unk* part);
extern "C" void fn_80566398(chr::CharaEquip_unk* equip, int arg);

// inferred: same as fn_8016E71C(show, 1) with the equipment loop of fn_8016E800 written out
// (inlining fn_8016E71C gives the loop registers in the other order)
inline void CharaTask::SetPartsVisible_unk(int show) {
    if (show) {
        if (m648 != 0 && fn_80044C50(m648, this)) {
            fn_80044CCC(m648);
        }
    } else {
        if (m648 != 0) {
            fn_80044E0C(m648, 1);
        }
        if (m64C != 0) {
            fn_80044E0C(m64C, 1);
        }
        chr::CharaEquip_unk** p = m680;
        for (unsigned int i = 0; i < 8; i++, p++) {
            if (*p != 0) {
                fn_80566398(*p, 0);
            }
        }
        if (mFlagB18) {
            mFlagB18 = 0;
            fn_80107CA8(lbl_8087F048, this, 0);
        }
    }
}

void CharaTask::fn_8016E4C4(int show) {
    SetModelVisible_unk(show);
    SetPartsVisible_unk(show);
}

void CharaTask::fn_8016E5F0(int show) {
    if (show) {
        SetModelVisible_unk(show);
    } else {
        SetModelVisible_unk(show);
        SetPartsVisible_unk(show);
    }
}

void CharaTask::fn_8016E71C(int show, int arg) {
    if (show) {
        if (m648 != 0 && fn_80044C50(m648, this)) {
            fn_80044CCC(m648);
        }
    } else {
        if (m648 != 0) {
            fn_80044E0C(m648, arg);
        }
        if (m64C != 0) {
            fn_80044E0C(m64C, arg);
        }
        fn_8016E800(0);
        if (mFlagB18) {
            mFlagB18 = 0;
            fn_80107CA8(lbl_8087F048, this, 0);
        }
    }
}

void CharaTask::fn_8016E800(int arg) {
    chr::CharaEquip_unk** p = m680;
    for (unsigned int i = 0; i < 8; i++, p++) {
        if (*p != 0) {
            fn_80566398(*p, arg);
        }
    }
}

extern "C" float fn_8010CA34(Effect_unk* effect);

float CharaTask::fn_8016E864() {
    float rate = 1.0f;
    if (mStatus.IsState_unk(0x40)) {
        rate *= MAX(0.3f, 1.0f - GetF94Rate_unk());
    }
    if (mStatus.IsState_unk(0x8000)) {
        rate *= 0.5f;
    }
    rate *= mStatus.fn_8012DB04();
    if (((unsigned int)mStatus.m14 & 0x2000) == 0x2000) {
        rate *= fn_8010CA34(lbl_8087F048);
    }
    return rate;
}

// fn_8016E970, fn_801750FC and fn_80178208 are needed by the functions above (inlined through
// -ipa file).
void CharaTask::fn_8016E970(int state) {
    if (m55C != state) {
        if (m55C != 6 && m55C != 8) {
            m564 = m55C;
        }
        if (m55C == 6) {
            fn_80178208(0);
        }
    }
    m55C = state;
}

// inferred: stops the current control and goes back to the idle state (1 for the player, 2 otherwise);
// the arguments keep the ledge, mFlag5 and the magic effect
void CharaTask::fn_8016EB48(int keepLedge, int keepFlag, int keepEffect) {
    fn_80178208(0);
    if (mType == 0 && !mStatus.IsState_unk(0x100) && !mStatus.IsState_unk(0x10000)) {
        fn_8016E970(1);
    } else {
        fn_8016E970(2);
    }
    if (!keepLedge && IsOnLedge_unk()) {
        ReleaseLedge_unk();
    }
    if (!keepFlag && IsFlag5_unk()) {
        mFlag5 = 0;
    }
    if (!keepEffect) {
        ClearEffect_unk();
    }
}

bool CharaTask::fn_8016F368(int type) {
    bool result = true;
    if (!IsSameSideType_unk(mType, type) && !IsSideTypeRev_unk(mType, type)) {
        result = false;
    }
    return result;
}

bool CharaTask::fn_8016F3D0(CharaTask* other, int check) {
    if (check && this != other) {
        if (lbl_8087F610 != 0 && lbl_8087F610->m540 == 0) {
            return false;
        }
        if ((mStatus.m0C & 2) == 2 || (other->mStatus.m0C & 2) == 2 || lbl_8087F0A8->mCC != 0) {
            return false;
        }
    }
    if (check && mF14 >= 0 && other->mF14 >= 0) {
        return mF14 == other->mF14;
    }
    return fn_8016F368(other->mType);
}

int CharaTask::fn_8016F4D8(CharaTask* chara) {
    for (int i = 0; i < GetLinkCount_unk(); i++) {
        if (mFE8[i] == chara) {
            return 1;
        }
    }
    return 0;
}

int CharaTask::fn_8016F530(CharaTask* chara) {
    if (HasLink_unk(chara)) {
        return 1;
    }
    for (int i = 0; i < GetLinkCount_unk(); i++) {
        if (mFE8[i] == 0) {
            mFE8[i] = chara;
            return 1;
        }
    }
    return 0;
}

int CharaTask::fn_8016F5EC(CharaTask* chara) {
    for (int i = 0; i < 16; i++) {
        if (mFE8[i] == chara) {
            mFE8[i] = 0;
            return 1;
        }
    }
    return 0;
}

void CharaTask::fn_8016F634() {
    for (int i = 0; i < 16; i++) {
        mFE8[i] = 0;
    }
}

// inferred: line checks to 2 units beyond the radius in front: blocked at height 3 but free at
// height 10 (a step that can be climbed?)
bool CharaTask::fn_8016F67C() {
    Mtx m;
    atn::Vector3 hit;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    float rotY = mRot.y;
    PSMTXRotRad(m, 'y', rotY);
    PSMTXMultVec(m, &dir, &dir);
    dir *= 2.0f + m620;
    atn::Vector3 from(mPos);
    atn::Vector3 to = mPos + dir;
    from.y += 3.0f;
    to.y += 3.0f;
    if (fn_8004ECC0(lbl_8087EE98, &hit, &from, &to, 0x80000000, 0, 0) != 0) {
        from = mPos;
        to = mPos + dir;
        from.y += 10.0f;
        to.y += 10.0f;
        if (fn_8004ECC0(lbl_8087EE98, &hit, &from, &to, 0x80000000, 0, 0) == 0) {
            return true;
        }
    }
    return false;
}

extern "C" int fn_803C17FC(RouteTable_unk* table, int point); // inferred: route point -> start index


void CharaTask::fn_8016F824(int point, int arg) {
    RouteTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (table == 0) {
        return;
    }
    if (point > 0) {
        mRoute.fn_80128508(fn_803C17FC(table, point), &mPos, arg);
        if (mRoute.fn_801255C8()) {
            fn_801718F8();
            return;
        }
    }
    if (lbl_8087EEB8 != 0) {
        char message[0x100];
        sprintf(lbl_807C7B28, "ch%04d_%02d", m50 / 100, m50 % 100);
        sprintf(message, "error: invalid pointmove chara[%d:%s] -> movepoint[%d]\n", m58, lbl_807C7B28, point);
        ((DebugMessages_unk*)lbl_8087EEB8)->fn_800697D8(message);
    }
}

extern "C" int fn_803C1840(RouteTable_unk* table, int point); // inferred: route point -> link

void CharaTask::fn_8016FDCC(int point, int arg) {
    RouteTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (table == 0) {
        return;
    }
    if (point > 0) {
        int link = fn_803C1840(table, point);
        if (IsOnLedge_unk() && (mC38 == link || mC3C == link)) {
            return;
        }
        mRoute.fn_801286F0(link, &mPos, arg);
        if (mRoute.fn_801255C8()) {
            fn_801718F8();
            return;
        }
    }
    if (lbl_8087EEB8 != 0) {
        char message[0x100];
        sprintf(lbl_807C7B28, "ch%04d_%02d", m50 / 100, m50 % 100);
        sprintf(message, "error: invalid pointmove chara[%d:%s] -> hidepoint[%d]\n", m58, lbl_807C7B28, point);
        ((DebugMessages_unk*)lbl_8087EEB8)->fn_800697D8(message);
    }
}

void CharaTask::fn_8017039C(int place, int arg) {
    if (m13FC != 0) {
        m13FC->fn_8017039C(place, arg);
        return;
    }
    if ((lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0) == 0) {
        return;
    }
    if (place > 0) {
        mRoute.fn_80128818(place, &mPos, arg);
        if (mRoute.fn_801255C8()) {
            fn_801718F8();
            return;
        }
    }
    if (lbl_8087EEB8 != 0) {
        char message[0x100];
        sprintf(lbl_807C7B28, "ch%04d_%02d", m50 / 100, m50 % 100);
        sprintf(message, "error: invalid pointmove chara[%d:%s] -> warppoint[%d]\n", m58, lbl_807C7B28, place);
        ((DebugMessages_unk*)lbl_8087EEB8)->fn_800697D8(message);
    }
}

// inferred: routes to the character `target` (mRadius is set only when >= 1)
void CharaTask::fn_80170A20(CharaTask* target, int arg, float radius) {
    mRoute.fn_80128930(target, &mPos, arg);
    if (radius >= 1.0f) {
        mRoute.mRadius = radius;
    }
    if (mRoute.fn_801255C8()) {
        fn_801718F8();
    }
}

// inferred: routes to the position `pos`
void CharaTask::fn_80170F20(const atn::Vector3* pos, int arg, float a, float radius) {
    mRoute.fn_80128A30(pos, &mPos, arg, a);
    if (radius >= 1.0f) {
        mRoute.mRadius = radius;
    }
    if (mRoute.fn_801255C8()) {
        fn_801718F8();
    }
}

// inferred: follows the character `target` (route mode 6)
void CharaTask::fn_80171420(CharaTask* target, int arg, float radius) {
    mRoute.fn_80128B20(target, &mPos, arg, radius);
    if (mRoute.fn_801255C8()) {
        fn_801718F8();
    }
}

void CharaTask::fn_801718F8() {
    Reset_unk(0, 1, 1, 1);
    mC58.fn_8011C044(0);
    fn_80017064(lbl_8087EE68, this);
    fn_8016E970(7);
    if (m1208 != 0) {
        fn_801750FC();
    }
}

extern "C" void fn_803AB37C(void* object, CharaTask* chara);

void CharaTask::fn_80171DB0() {
    if (mRoute.m2C != 0) {
        return;
    }
    if (!mRoute.fn_80127EF4()) {
        return;
    }
    Reset_unk(0, 0, 1, 1);
    mC58.fn_8011C044(0);
    fn_80017064(lbl_8087EE68, this);
    RouteTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (table != 0) {
        fn_803AB37C(table->m134, this);
    }
}

// inferred: object at lbl_8087EE68 (class unknown; also passed to fn_80017064)
struct Global8087EE68_unk {
    unsigned char m00[0x90];
    int m90; // inferred: compared with mId % 4 (fn_80171F3C)
};
extern "C" int fn_8036DBC0(Field_unk* field, const atn::Vector3* pos, const atn::Vector3* dest, CharaTask* chara);

int CharaTask::fn_80171F3C(const atn::Vector3* pos, const atn::Vector3* dest) {
    if (lbl_8087F430 == 0) {
        return 0;
    }
    if (mStatus.IsState_unk(0x20)) {
        return 0;
    }
    if (IsPartyMember_unk()) {
        if (mC54 == 0) {
            return 0;
        }
        if ((mColli.mFlags & 1) != 1) {
            return 0;
        }
        if (!mC58.mHeld_unk && mId % 4 == ((Global8087EE68_unk*)lbl_8087EE68)->m90) {
            return 0;
        }
    }
    if (mType == 0) {
        if (fn_805F9920(dest) < 0.002f) {
            return 0;
        }
    } else if (mType == 2) {
        if (fn_805F9920(dest) < 0.02f) {
            return 0;
        }
    } else if (fn_805F9920(dest) < 0.1f) {
        return 0;
    }
    return fn_8036DBC0(lbl_8087F430, pos, dest, this);
}

// inferred: starts chr::ControlGuardMountle (shared by fn_801720AC and fn_801725EC; name invented;
// static so that its "" goes to the file pool)
static void StartMountle_unk(CharaTask* chara, const atn::Vector3& dir) {
    chara->fn_80178208(new (0, "", "", 0) chr::ControlGuardMountle(chara, dir));
}

// inferred: starts chr::ControlStool on `chara` (name invented; see StartMountle_unk)
static void StartStool_unk(CharaTask* chara) {
    chara->fn_80178208(new (0, "", "", 0) chr::ControlStool(chara));
}

// inferred: route test of the mantle (nonzero if the character may climb in `dir`)

// inferred: the player climbs over an obstacle in front (blocked at height 10, free at height 5
// fails over to fn_801725EC, which climbs onto another character)
int CharaTask::fn_801720AC(atn::Vector3* pos, atn::Vector3* vel) {
    if (mType != 0) {
        return 0;
    }
    if (m55C == 6 || m55C == 8) {
        return 0;
    }
    int flag5 = mFlag5;
    if (!flag5 && !mFlag4) {
        return 0;
    }
    if (m1208 != 0 || m139C != 0) {
        return 0;
    }
    atn::Vector3 dir(vel->x, 0.0f, vel->z);
    if (fn_805F9920(&dir) < 0.1f) {
        return 0;
    }
    PSVECNormalize(&dir, &dir);
    atn::Vector3 from(*pos);
    atn::Vector3 to(*pos);
    from.y += 10.0f;
    to.y += 10.0f;
    to += dir * (3.0f + m620);
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) == 0) {
        from = *pos;
        to = *pos;
        from.y += 5.0f;
        to.y += 5.0f;
        to += dir * (3.0f + m620);
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0 &&
            fn_803CC198(lbl_8087F430->m10D8, pos, &dir, this)) {
            StartMountle_unk(this, dir);
            return 1;
        }
    }
    return fn_801725EC(pos, vel, 0);
}

// inferred: climbs onto the character in front (it gets chr::ControlStool); `out` receives it
// NONMATCHING: ~95% - only the scheduling of the line-check setup differs (copies of *pos, the
// CollResult_unk zero stores and the lbl_8087EE98 load); tried the result declared at each point,
// copy constructor / operator= / Assign_unk copies, a world local, a distance local, early return.
int CharaTask::fn_801725EC(atn::Vector3* pos, atn::Vector3* vel, CharaTask** out) {
    atn::Vector3 dir(vel->x, 0.0f, vel->z);
    float len = PSVECMag(&dir);
    if (len < 0.1f) {
        return 0;
    }
    PSVECNormalize(&dir, &dir);
    if (mType != 0 && Dot_unk(dir, fn_8014052C()) < 0.2f) {
        return 0;
    }
    if (mMotion.fn_80097D40(0x41) == 0) {
        return 0;
    }
    if (mFlag3) {
        return 0;
    }
    if (lbl_8087F610 != 0) {
        return 0;
    }
    atn::Vector3 from(*pos);
    atn::Vector3 to(*pos);
    from.y += 5.0f;
    to.y += 5.0f;
    CollResult_unk result;
    to += dir * (3.0f + m620);
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0, &mColli, 0) != 0 && result.m38 != 0 &&
        (result.m38->m08 & 2) == 2) {
        CharaTask* other = result.m38->m0C;
        if (!other->fn_8016F368(mType)) {
            return 0;
        }
        if (other->m55C == 1) {
            if (!other->mFlag6) {
                return 0;
            }
        } else if (other->m55C == 7) {
            if (other->mType == 0) {
                return 0;
            }
        } else if (other->m55C != 2) {
            return 0;
        }
        int ledge = other->mFlag31;
        if (ledge || other->mFlag3) {
            return 0;
        }
        if (other->mMotion.fn_80097D40(m484[4]) == 0) {
            return 0;
        }
        // inferred: the original reads m574.y as the z component
        atn::Vector3 move(other->m574.x, 0.0f, other->m574.y);
        if (PSVECMag(&move) > 0.5f * len && Dot_unk(dir, move.GetNormalized_unk()) > 0.7f) {
            return 0;
        }
        if (atn::Abs_unk(other->mPos.y - mPos.y) > 6.0f) {
            return 0;
        }
        StartMountle_unk(this, dir);
        StartStool_unk(other);
        if (out != 0) {
            *out = other;
        }
        return 1;
    }
    return 0;
}

namespace chr {
// inferred: names from the vtables stored by the constructors (0x801C0D80 is in the unit after
// chr/ControlStickByQuark.cpp, 0x801B192C in chr/ControlDive.cpp)
// inferred: chr::ControlWallMountle and chr::ControlJump are in the unit after
// chr/ControlStickByQuark.cpp
class ControlWallMountle : public ControlBase {
public:
    ControlWallMountle(Chara_unk* chara, const atn::Vector3& normal, const atn::Vector3& pos, int arg); // 0x801C1578
    virtual int vf0C();
    unsigned char m08[0x3C - 0x08];
};
class ControlJump : public ControlBase {
public:
    ControlJump(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& dir); // 0x801C1DCC
    virtual int vf0C();
    unsigned char m08[0x3C - 0x08];
};
} // namespace chr

// inferred: the other side of the pair check of fn_801725EC (name invented)
inline bool IsSideTypeB_unk(int a, int b) {
    bool result = false;
    if (a == 3 && b == 0) {
        result = true;
    }
    return result;
}

// NONMATCHING: 82% - scheduling of `to += dir * (3.0f + m620)`, the register of the loaded flag word
// (other->mFlag31), the ptmf stack slots of the two inlined fn_80178208 chains (known issue), .data
// offsets and two map entries; tried: locals declared up front, named fwd/n, IsOnLedge_unk, the
// pair helpers of fn_8015061C, statement orders of from/to.
inline bool IsSameTypes_unk(int a, int b) {
    bool result = true;
    if (!IsSameSideType_unk(a, b) && !IsSideTypeB_unk(a, b)) {
        result = false;
    }
    return result;
}
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
// inferred: nonzero if a wall can be climbed from `pos` along `dir` (route table query)
extern "C" int fn_803CC3BC(RouteTable_unk* table, const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* chara);

// inferred: same tests as CharaTask::IsActive_unk, IsFree_unk, IsGrappled_unk, CanAct_unk and
// CanReact_unk, written with a result variable (each level is materialized in fn_80172EFC); names
// invented
inline bool IsActiveB_unk(CharaTask* chara) {
    bool result = false;
    if ((chara->mFlags & 4) != 4 && (chara->mFlags & 1) != 1) {
        result = true;
    }
    return result;
}
inline bool IsFreeB_unk(CharaTask* chara) {
    bool result = false;
    if (IsActiveB_unk(chara) && (chara->mStatus.m0C & 0x20) != 0x20) {
        result = true;
    }
    return result;
}
inline bool IsGrappledB_unk(CharaTask* chara) {
    bool result = false;
    if (chara->m55C == 6 && chara->m560 == 28) {
        result = true;
    }
    return result;
}
inline bool CanActB_unk(CharaTask* chara) {
    bool result = false;
    if (IsFreeB_unk(chara) && !IsGrappledB_unk(chara)) {
        result = true;
    }
    return result;
}
inline bool CanReactB_unk(CharaTask* chara) {
    if (!CanActB_unk(chara)) {
        return false;
    }
    if (chara->mStatus.IsState_unk(0x40) && chara->mF94 == 0) {
        return false;
    }
    if (chara->m55C == 6 && chara->m560 >= 17 && chara->m560 <= 19) {
        return false;
    }
    return true;
}

// NONMATCHING: 88% - r0/r5
// swapped in the materialized CanReact checks, the 0x80000020 mask kept whole in r30 by the original
// and the from/to scheduling; tried: a mask local, five IsFree forms, the class accessors.
// inferred: climbs up the wall in front (chr::ControlWallUp) after pushing against it for a while
// (m13AC frames, limit from the event flag 0x120)
int CharaTask::fn_80172EFC(atn::Vector3* pos, atn::Vector3* move) {
    if (fn_8017380C(pos, move)) {
        m13AC = 0;
        return 1;
    }
    bool ok = true;
    if (mType != 0) {
        ok = false;
    }
    if (ok && (m55C == 6 || m55C == 8)) {
        ok = false;
    }
    if (ok && !mStatus.fn_801346C8(0x10) && (lbl_8087F610 == 0 || !mStatus.fn_80134208())) {
        ok = false;
    }
    if (ok && (!CanReactB_unk(this) || m1208 != 0 || m139C != 0)) {
        ok = false;
    }
    if (ok && (lbl_8087F430 == 0 || fn_80375184(lbl_8087F430) == 0)) {
        ok = false;
    }
    atn::Vector3 dir(move->x, 0.0f, move->z);
    if (ok) {
        if (m570 < 1.2f * lbl_8087EFA8->m3A4) {
            ok = false;
        } else {
            PSVECNormalize(&dir, &dir);
            if (PSVECDotProduct(&dir, &fn_8014052C()) < 0.8660254f) {
                ok = false;
            }
        }
    }
    int limit = 12;
    if (lbl_8087F430 != 0) {
        int flag = fn_80370174(lbl_8087F430, 0x120);
        if (flag == 1) {
            limit = 4;
        } else if (flag == 2) {
            limit = 24;
        }
    }
    if (!ok) {
        m13AC = 0;
        return 0;
    }
    atn::Vector3 from(*pos);
    atn::Vector3 to(*pos);
    from.y += 10.0f;
    to.y += 10.0f;
    to += dir * (5.0f + (3.0f + m620));
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000020, 0, 0) != 0) {
        if (++m13AC > limit && fn_803CC3BC(lbl_8087F430->m10D8, pos, &dir, this) != 0 &&
            PSVECDotProduct(&result.m28, &-dir) > 0.8660254f) {
            atn::Vector3 hit(result.m04);
            atn::Vector3 normal(result.m28);
            bool blocked = false;
            for (int i = 0; i < 2; i++) {
                atn::Vector3 side;
                PSVECCrossProduct(&dir, &atn::Vector3(0.0f, 1.0f, 0.0f), &side);
                Mtx m;
                PSMTXRotAxisRad(m, &dir, 3.1415927f * i);
                PSMTXMultVec(m, &side, &side);
                side *= 3.0f;
                from = *pos;
                to = *pos;
                from.y += 10.0f;
                to.y += 10.0f;
                to += dir * (3.0f + m620);
                if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000020, 0, 0) != 0 &&
                    PSVECDotProduct(&result.m28, &normal) < 0.25881904f) {
                    blocked = true;
                    break;
                }
            }
            if (!blocked) {
                hit.y += 8.0f;
                StartWallUp_unk(this, hit, dir);
            }
        }
        return 1;
    }
    m13AC = 0;
    return 0;
}

// NONMATCHING: 85% - only the scheduling of the from/to setup before the first ray (also seen in
// fn_801725EC), .data offsets and the ControlWallMountle map entry; tried: accessors for m620, a
// length local, `to = to + ...`, CollResult declared first, statement orders.
// inferred: climbs up a low wall in front (chr::ControlWallUp), or mounts it
// (chr::ControlWallMountle) when the character is low
int CharaTask::fn_8017380C(atn::Vector3* pos, atn::Vector3* move) {
    if (mType != 0) {
        return 0;
    }
    if (m55C == 6 || m55C == 8) {
        return 0;
    }
    if (m1208 != 0 || m139C != 0) {
        return 0;
    }
    atn::Vector3 dir(move->x, 0.0f, move->z);
    float dt = lbl_8087EFA8->m3A4;
    if (fn_805F9920(&dir) < 0.9f * dt) {
        return 0;
    }
    PSVECNormalize(&dir, &dir);
    float limit = cos(0.5235988f);
    if (PSVECDotProduct(&dir, &fn_8014052C()) < limit) {
        return 0;
    }
    atn::Vector3 from(*pos);
    atn::Vector3 to(*pos);
    from.y += 5.0f;
    to.y += 5.0f;
    to += dir * (3.0f + m620);
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000020, 0, 0) != 0 &&
        ((result.m3C & 0x400) == 0x400 || lbl_8087F0A8->m2C0 != 0)) {
        if (PSVECDotProduct(&result.m28, &-dir) > limit) {
            if (mPos.y >= 90.0f) {
                fn_801591E8(dir);
            } else {
                fn_80178208(new (1, "", "", 0) chr::ControlWallMountle(this, -result.m28, result.m04, 0));
            }
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: 96% - only the r3/r4 roles of the z-first copy of the fallback direction and the
// .data offsets; tried: operator=, Assign_unk, a named copy. The second and third rays clear
// result.m3C (the original stores 0 to the first result there).
// inferred: sits down on the seat in front (chr::ControlSitting)
int CharaTask::fn_80174104(atn::Vector3* pos, atn::Vector3* move) {
    if (mType != 0) {
        return 0;
    }
    if (lbl_8087F098 != 0 && lbl_8087F098->m48 != 0) {
        return 0;
    }
    if (m55C == 6 || m55C == 8) {
        return 0;
    }
    if (lbl_8087F430 != 0 && (fn_80370174(lbl_8087F430, 0x1F) > 0 || fn_80370174(lbl_8087F430, 0xD2) > 0)) {
        return 0;
    }
    atn::Vector3 dir(move->x, 0.0f, move->z);
    if (fn_805F9920(&dir) < 0.01f) {
        dir.Assign_unk(fn_8014052C());
        return 0;
    }
    PSVECNormalize(&dir, &dir);
    atn::Vector3 from(*pos);
    atn::Vector3 to(*pos);
    from.y += 5.0f;
    to.y += 5.0f;
    to += dir * (1.154f * m620);
    CollResult_unk result;
    if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0 && (result.m3C & 4) != 4) {
        if ((result.m3C & 0x10) == 0x10) {
            return 0;
        }
        float dot = PSVECDotProduct(&result.m28, &-dir);
        if (dot > (float)cos(0.34906587f) && (result.m3C & 0x800) == 0x800) {
            CollResult_unk result2;
            atn::Vector3 off = Cross_unk(dir, atn::Vector3(0.0f, 1.0f, 0.0f)) * 3.0f;
            result.m3C = 0;
            if (fn_8004ED34(lbl_8087EE98, &result2, &(from + off), &(to + off), 0x80000000, 0, 0) != 0 &&
                (result2.m3C & 0x800) == 0x800) {
                result.m3C = 0;
                if (fn_8004ED34(lbl_8087EE98, &result2, &(from - off), &(to - off), 0x80000000, 0, 0) != 0 &&
                    (result2.m3C & 0x800) == 0x800) {
                    atn::Vector3 seat = result.m04 + result.m28 * 3.0f;
                    atn::Vector3 rot = result.m28.ToRotation_unk();
                    fn_80178208(new (1, "", "", 0) chr::ControlSitting(this, *pos, seat, rot));
                    return 1;
                }
            }
        }
    }
    return 0;
}

// Same instructions as the target; only the .data offsets of the ptmf constants and the
// chr::ControlJump map entry differ.
// inferred: jumps down when there is no ground ahead (chr::ControlJump)
int CharaTask::fn_801749EC(atn::Vector3* pos, atn::Vector3* vel) {
    if (mType != 0) {
        return 0;
    }
    if (m55C == 6 || m55C == 8) {
        return 0;
    }
    if (m1208 != 0 || m139C != 0) {
        return 0;
    }
    if (PSVECMag(vel) < 0.1f) {
        return 0;
    }
    atn::Vector3 from(*pos);
    from += *vel;
    atn::Vector3 to(from);
    from.y += 3.0f;
    to.y -= 5.0f;
    if (fn_8004ED34(lbl_8087EE98, 0, &from, &to, 0x80000000, 0, 0) == 0) {
        fn_80178208(new (0, "", "", 0) chr::ControlJump(this, *pos, vel->GetNormalized_unk()));
        return 1;
    }
    return 0;
}

namespace chr {
// inferred: controls of chr/ControlCannonLift.cpp (names from the vtables stored by the constructors)
class ControlCannonLift : public ControlBase {
public:
    ControlCannonLift(Chara_unk* chara, GimmickCommon* cannon); // 0x801A0BA8
    virtual int vf0C();
    unsigned char m08[0x10 - 0x08];
};
class ControlCannonThrow : public ControlBase {
public:
    ControlCannonThrow(Chara_unk* chara, const atn::Vector3& dir, float speed); // 0x801A17A4
    virtual int vf0C();
    unsigned char m08[0x18 - 0x08];
};
} // namespace chr

// fn_80174E04, fn_8017518C: same instructions as the target; only the .data offsets of the ptmf
// constants and the constructor map entries differ.
void CharaTask::fn_80174E04(GimmickCommon* cannon) {
    fn_80178208(new (0, "", "", 0) chr::ControlCannonLift(this, cannon));
}

// inferred: CharaTask::m1208 is a gimmick (GimmickCommon: m54 state, m9C shown, vf40 message); the
// byte at 0x520 belongs to the derived class
struct CharaGimmick_unk : public GimmickCommon {
    unsigned char mF4[0x520 - sizeof(GimmickCommon)];
    unsigned char m520;
};

// inferred: releases the held gimmick
void CharaTask::fn_801750FC() {
    if (m1208 != 0) {
        GimmickMsg_unk msg;
        msg.mType = 3;
        msg.m10 = (int)this;
        ((CharaGimmick_unk*)m1208)->vf40(&msg);
        ((CharaGimmick_unk*)m1208)->m520 = 0xFF;
        m1208 = 0;
    }
}

// inferred: mF6C and mF78 are the direction and speed of the shot
void CharaTask::fn_8017518C() {
    fn_80178208(new (0, "", "", 0) chr::ControlCannonThrow(this, mF6C, mF78));
}

// NONMATCHING: 88% - the stack slots of the ptmf temporaries of the two inlined fn_80178208 chains
// are swapped (known issue) plus .data offsets.
void CharaTask::fn_8017547C(CharaTask* other) {
    if (other != 0 && (other->mStatus.m0C & 0x20) == 0x20) {
        fn_80178208(new (0, "", "", 0) chr::ControlDraggingStart(this, (int)other));
        other->fn_80178208(new (0, "", "", 0) chr::ControlDraggedStart(other, (int)this));
    }
}

void CharaTask::fn_80175A38() {
    CharaTask* other = (CharaTask*)mF54;
    other->mMotion.m34C = 1;
    other->mMotion.mTracks[0].m20 = 1.0f;
    other->mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    other->mMotion.mTracks[0].m0C = 1.0f;
    other->mMotion.mTracks[0].m08 = other->mMotion.fn_80097D7C(0);
    other->mControl.reset();
    other->mF50 = 0;
    mF54 = 0;
}

bool CharaTask::fn_80175AEC() {
    if (m55C == 6 && (unsigned int)(m560 - 0x74) <= 3) {
        return true;
    }
    return false;
}

// inferred: gimmick 34100 (0x8534) keeps one 0x28-byte entry per party member at 0x52C; bit 6 of
// the entry makes the player look at the gimmick (fn_80175B18); names invented
struct GimmickLookEntry_unk {
    unsigned int m00_31 : 25;
    unsigned int mLook : 1;
    unsigned int m00_5 : 6;
    unsigned char m04[0x28 - 0x04];
};
struct GimmickLook_unk : public GimmickCommon {
    unsigned char mF4[0x52C - sizeof(GimmickCommon)];
    GimmickLookEntry_unk mEntries[7]; // 0x52C
};

// inferred: finds the look point `id` (name invented)
inline RouteLook_unk* FindLook_unk(RouteTable_unk* table, int id) {
    for (unsigned int i = 0; i < table->mNumLooks; i++) {
        if (id == table->mLooks[i].mId) {
            return &table->mLooks[i];
        }
    }
    return 0;
}

// inferred: position of node `index` of a model (translation of its matrix); name invented
inline atn::Vector3 GetNodePos_unk(Model_unk* model, int index) {
    MtxPtr m = model->GetNodeMtx_unk(index);
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 63% - same control flow, calls and stack layout; register allocation differs (the
// original gives `found` r31, the loop index r30 and this r29), plus small scheduling in the last
// branch; tried: hoisting found/i, reusing one character variable, named diff vector.
// inferred: chooses what the character looks at (mLook): the target's neck, the closest enemy, a
// look point of the route tables, a gimmick or the player
void CharaTask::fn_80175B18(float distSq) {
    if (!mFlag24) {
        return;
    }
    if (m55C == 8) {
        return;
    }
    if (m55C == 6) {
        if (m560 != 0x2F) {
            mLook.fn_80129A48(0.3f);
        }
        return;
    }
    if (m12BC >= 0) {
        m580 *= 0.5f;
        m584 *= 0.5f;
        mLook.fn_80129A48(0.3f);
        return;
    }
    if (IsPartyMember_unk() && mC54 == 0 && (m137C & 4) != 4) {
        return;
    }
    CharaTask* target = mFC4 != 0 ? (CharaTask*)mFC4 : mFC0;
    if (mType == 0 && target != 0 && target->GetModel_unk()->GetNodeMtx_unk("neck") != 0) {
        mLook.fn_80129978(&target->mMotion, "neck", chr::lbl_807C7030, 0.3f);
        return;
    }
    if (m139C != 0) {
        if (mFlag31) {
            float angle = 3.1415927f * (mFlag29 ? -0.1f : 0.1f);
            if (mFlag30) {
                mLook.fn_80129954(atn::Vector3(0.0f, 2.0f * angle, 0.0f), 0.3f);
            } else {
                mLook.fn_80129954(atn::Vector3(0.21991149f, 3.0f * angle, 0.0f), 0.3f);
            }
        } else {
            mLook.fn_80129A48(0.3f);
        }
        return;
    }
    if (mFlag6) {
        mLook.fn_80129A48(0.3f);
        return;
    }
    if (lbl_8087F430 == 0 || lbl_8087F430->m10D8 == 0) {
        return;
    }
    if (mType == 0 && lbl_8087F430->m868 == 9) {
        m580 = 1.0471976f;
        m584 = 0.0f;
        mLook.fn_80129A48(1.0f);
        return;
    }
    if (distSq > lbl_8087EFA8->m21C * lbl_8087EFA8->m21C) {
        mLook.fn_80129A48(0.3f);
        return;
    }
    if (lbl_8087F098 != 0 && lbl_8087F098->m48 != 0 && IsPartyMember_unk()) {
        target = lbl_8087F8A0->mHead;
        float best = fn_805F9920(&(target->mPos - mPos));
        for (CharaTask* enemy = (CharaTask*)EnemyManager::sInstance->mHead; enemy != 0; enemy = NextChara_unk(enemy)) {
            float d = fn_805F9920(&(enemy->mPos - mPos));
            if (best > d) {
                best = d;
                target = enemy;
            }
        }
        if (best < 900.0f) {
            mLook.fn_80129978(&target->mMotion, "neck", chr::lbl_807C7030, 0.3f);
            return;
        }
    }
    int found = 0;
    RouteTable_unk* table = lbl_8087F430->m10D8;
    atn::Vector3 pos;
    for (unsigned int i = 0; i < table->mNumLooks; i++) {
        RouteLook_unk* look = &table->mLooks[i];
        int enabled = look->mEnabled;
        int priority = look->mPriority;
        pos = look->mPos;
        if (look->mLink > 0) {
            RouteLook_unk* link = FindLook_unk(table, look->mLink);
            if (link != 0) {
                enabled = enabled != 0 && link->mEnabled != 0;
                priority = link->mPriority;
                pos = link->mPos;
            }
        }
        if (enabled && priority > found) {
            atn::Vector3 diff = look->mPos - mPos;
            if (PSVECMag(&diff) - look->mRadius < 10.0f) {
                mLook.fn_80129930(pos, 0.3f);
                found = priority;
            }
        }
    }
    if (mType == 0) {
        GimmickCommon* gimmick = lbl_8087F4A0 != 0 ? lbl_8087F4A0->mHead : 0;
        for (; gimmick != 0; gimmick = gimmick->m5C) {
            if (gimmick->IsFlag_unk(4)) {
                continue;
            }
            if (gimmick->m9C == 0) {
                continue;
            }
            if (gimmick->mId != 0x8534) {
                continue;
            }
            int index = gimmick->m54;
            if (!(IsPartyIndex_unk(index) ? ((GimmickLook_unk*)gimmick)->mEntries[index].mLook : 0)) {
                continue;
            }
            if (fn_805F9920(&(mPos - gimmick->m6C)) > 10000.0f) {
                continue;
            }
            mLook.fn_80129930(gimmick->m6C, 0.3f);
            found = 999999;
        }
    }
    if (found == 0 && IsPartyMember_unk()) {
        CharaTask* player = lbl_8087F8A0->mHead;
        atn::Vector3 dir = player->mPos - mPos;
        float d = fn_805F9920(&dir);
        if (d < 4900.0f && d > 0.1f) {
            PSVECNormalize(&dir, &dir);
            if (PSVECDotProduct(&dir, &fn_8014052C()) > -0.5f) {
                int mode = lbl_8087F430->fn_803761DC();
                if (IsPartyIndex_unk(fn_80219558(m50))) {
                    mode = 2;
                }
                if (mode != 1) {
                    if (mode >= 2) {
                        mLook.fn_80129978(&player->mMotion, "neck", chr::lbl_807C7030, 0.3f);
                        found = 1;
                    } else {
                        atn::Vector3 head = GetNodePos_unk(&mMotion, m520);
                        atn::Vector3 other = GetNodePos_unk(&player->mMotion, player->m520);
                        atn::Vector3 away(head.x - other.x, 0.0f, head.z - other.z);
                        PSVECNormalize(&away, &away);
                        atn::Vector3 at = head + away * 2.0f;
                        at.y -= 1.0f;
                        mLook.fn_80129930(at, 0.3f);
                        found = 1;
                    }
                }
            }
        }
    }
    if (found == 0) {
        mLook.fn_80129A48(0.3f);
    }
}

int CharaTask::fn_8017639C(CharaAttach_unk* obj) {
    for (int i = 0; i < 4; i++) {
        if (m120C[i] == 0) {
            m120C[i] = obj;
            return 1;
        }
    }
    return 0;
}

int CharaTask::fn_801763E0(CharaAttach_unk* obj) {
    for (int i = 0; i < 4; i++) {
        if (m120C[i] == obj) {
            m120C[i] = 0;
            return 1;
        }
    }
    return 0;
}

// inferred: with a valid index, nonzero if the skill of mStatus.m2D0[index] is attached; otherwise
// nonzero if anything is attached
int CharaTask::fn_80176428(int index) {
    if (index < 0 || 4 <= index) {

        int count = 0;
        if (m120C[0] != 0) {
            count = 1;
        }
        if (m120C[1] != 0) {
            count++;
        }
        if (m120C[2] != 0) {
            count++;
        }
        if (m120C[3] != 0) {
            count++;
        }
        return count > 0;
    }
    HitInfo_unk* skill = fn_80219E6C(mStatus.m2D0[index].m[0]);
    if (m120C[0] != 0 && m120C[0]->m04 == skill) {
        return 1;
    }
    if (m120C[1] != 0 && m120C[1]->m04 == skill) {
        return 1;
    }
    if (m120C[2] != 0 && m120C[2]->m04 == skill) {
        return 1;
    }
    if (m120C[3] != 0 && m120C[3]->m04 == skill) {
        return 1;
    }
    return 0;
}

// inferred: collision sphere of the character at `pos` (inlined in fn_8013CB68)
CharaTask::Sphere_unk CharaTask::fn_80176548(const atn::Vector3& pos) {
    Sphere_unk sphere;
    sphere.mCenter = m614;
    sphere.mRadius = m620;
    sphere.mCenter = pos + m5A4;
    if (mStatus.mParamB.fn_801404E0(0x40)) {
        sphere.mCenter.y -= sphere.mRadius;
    } else {
        sphere.mCenter.y += sphere.mRadius;
    }
    return sphere;
}

extern "C" void fn_803CC718(RouteTable_unk* table, CharaTask* chara, int arg);

// Same instructions as the target; only the .data offsets of the ptmf constants differ.
void CharaTask::fn_801765D8() {
    mFlags |= 4;
    mColli.mFlags &= ~1;
    vf38(0);
    vf9C(0);
    if (m648 != 0) {
        fn_80044E0C(m648, 0);
    }
    if (m64C != 0) {
        fn_80044E0C(m64C, 0);
    }
    if (mControl && m55C == 6 && m560 == 0x1D) {
        fn_80178208(0);
    }
    ClearEffect_unk();
    fn_80239DAC(lbl_8087F3C0, &mF20, 0, 0);
    mFlagB28 = 0;
    vf48();
    fn_80178A6C();
    Unlink_unk();
    m1220.fn_80121E14(0);
    if (lbl_8087F430 != 0) {
        RouteTable_unk* table = lbl_8087F430->m10D8;
        if (table != 0 && (table->m38 & 2) == 2) {
            fn_803CC718(table, this, 0);
        }
    }
}


// inferred: disables the collision and hides the effect of the character
void CharaTask::fn_80176ACC() {
    mFlags &= ~4;
    mColli.mFlags |= 1;
    vf38(1);
    vf98();
    if (lbl_8087F430 != 0) {
        RouteTable_unk* table = lbl_8087F430->m10D8;
        if (table != 0 && (table->m38 & 2) == 2) {
            fn_803CC718(table, this, 1);
        }
    }
}

// inferred: color as four floats (as in gmk/PlayEffect.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// NONMATCHING: ~75% (28/113 lines, 2 of them the ptmf .data noise) - the three argument temporaries
// (pos, rot, color) get their stack slots in the opposite order (target: pos lowest); tried named locals
// assigned field by field (then the ptmf temporaries move), temporaries, an inline accessor for the
// m121C test.
// inferred: starts the effect m121C on the model (mFlag18 is set while it is shown)
void CharaTask::vf98() {
    bool show = false;
    if (m121C && !mFlag18) {
        show = true;
    }
    if (mType == 0 && lbl_8087F430 != 0 && fn_80219558(m50) == 0) {
        show = show && fn_80376150(lbl_8087F430);
    }
    if (m50 == 700023) {
        show = show && mFlag5;
    }
    if (show) {
        fn_80232B7C(m121C.get(), 0);
        lbl_8087F3C0->mB8 = 2;
        fn_8023A8B4(lbl_8087F3C0, m121C.get(), &mMotion, 0, atn::Vector3(0.0f, 0.0f, 0.0f),
                    atn::Vector3(0.0f, 0.0f, 0.0f), Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
        lbl_8087F3C0->mB8 = 0;
        mFlag18 = 1;
    }
}

// inferred: stops the effect started by vf98
void CharaTask::vf9C(int arg) {
    if (m121C && mFlag18) {
        fn_80239DAC(lbl_8087F3C0, m121C.get(), 0, arg);
        mFlag18 = 0;
    }
}

// NONMATCHING: same instructions, different scheduling/registers (the original converts b first and
// builds the word in r4); tried ternaries and shifts in every order, | and +, bool locals, a bit field
// struct, an inline helper with bool parameters, unsigned parameters.
// inferred: flags of the motion command nodes (tested by fn_80145334)
void CharaTask::fn_80176DFC(int a, int b, int c) {
    m1254 = (a != 0) | (b != 0 ? 2 : 0) | (c != 0 ? 4 : 0);
}

// inferred: quicksort used by fn_80176E30 (the comparison only sees the keys m00). The two
// instantiations of Sort_unk are 0x80177688 (comparison by value) and 0x80177AA0 (by reference,
// recursive part); Median3_unk is 0x80177EB8. Names invented.
typedef bool (*FloatLess_unk)(const float& a, const float& b);
extern "C" void fn_801092C8(Effect_unk* effect, int id, CharaTask* chara, int a, int b);


template <class T>
inline void Swap_unk(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <class T>
struct Ref_unk {
    typedef T& type;
};
template <class T>
struct Ref_unk<T&> {
    typedef T& type;
};

template <class T, class Compare>
void Median3_unk(T* a, T* b, T* c, Compare& comp) {
    bool ca = !comp(c->m00, a->m00);
    bool bc = !comp(b->m00, c->m00);
    if (ca && bc) {
        return;
    }
    if (!ca && !bc) {
        Swap_unk(*a, *b);
    } else {
        if (comp(b->m00, a->m00)) {
            Swap_unk(*a, *b);
        }
        if (ca) {
            Swap_unk(*b, *c);
        } else {
            Swap_unk(*a, *c);
        }
    }
}

template <class T, class Compare>
inline T* MinElement_unk(T* first, T* last, Compare& comp) {
    T* result = first;
    if (first != last) {
        for (T* it = first + 1; it != last; it++) {
            if (comp(it->m00, result->m00)) {
                result = it;
            }
        }
    }
    return result;
}

template <class T, class Compare>
inline void SelectionSort_unk(T* first, T* last, Compare& comp) {
    if (first != last) {
        for (T* end = last - 1; first != end; first++) {
            T* min = MinElement_unk(first, last, comp);
            if (min != first) {
                Swap_unk(*min, *first);
            }
        }
    }
}

// NONMATCHING: ~83% (44/262 lines in both instantiations) - the selection sort swaps the registers of
// `end` and `it` (r30/r26 in the original) and the second pivot offset is scheduled differently;
// tried inline min_element/selection sort helpers, loop forms, declaration orders, `3 * n / 4`.
template <class T, class Compare>
void Sort_unk(T* first, T* last, Compare comp) {
    typedef typename Ref_unk<Compare>::type CompareRef;
    static int sOffset; // inferred: lbl_8087D9D0 and lbl_8087D9D4 (one per instantiation), -4 to 4
    while (true) {
        int n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 20) {
            if (first != last) {
                for (T* end = last - 1; first != end; first++) {
                    T* min = MinElement_unk(first, last, comp);
                    if (min != first) {
                        Swap_unk(*min, *first);
                    }
                }
            }
            return;
        }
        T* a = first + (n / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        T* b = first + (n * 3 / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        T* j;
        T* pivot = last - 1;
        Median3_unk(a, b, pivot, comp);
        T* i = first;
        j = pivot;
        while (comp(i->m00, pivot->m00)) {
            i++;
        }
        while (i != --j && !comp(j->m00, pivot->m00)) {
        }
        if (i < j) {
            Swap_unk(*i, *j);
            i++;
            while (true) {
                while (comp(i->m00, pivot->m00)) {
                    i++;
                }
                while (!comp((--j)->m00, pivot->m00)) {
                }
                if (i >= j) {
                    break;
                }
                Swap_unk(*i, *j);
                i++;
            }
        }
        if (i == first) {
            T* back = last - 1;
            Swap_unk(*i, *pivot);
            i++;
            if (!comp(first->m00, back->m00)) {
                while (i != last && !comp(first->m00, i->m00)) {
                    i++;
                }
                if (i < back) {
                    Swap_unk(*i, *back);
                }
            }
            j = back;
            if (i < j) {
                while (true) {
                    while (!comp(first->m00, i->m00)) {
                        i++;
                    }
                    while (comp(first->m00, (--j)->m00)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    Swap_unk(*i, *j);
                    i++;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            Sort_unk<T, CompareRef>(first, i, comp);
            first = i;
        } else {
            Sort_unk<T, CompareRef>(i, last, comp);
            last = i;
        }
    }
}

// inferred: name invented
inline EffectCharaSlot_unk* GetEffectSlots_unk() {
    return lbl_8087F048 != 0 ? lbl_8087F048->mCBF0 : 0;
}

// NONMATCHING: ~99% - the original computes the slot address in r4 before moving it to r26 (tried
// several forms of the ternary/accessor); the 2*pi double is the known .rodata pool noise.
// inferred: sorts the characters around this one (m125C: angle, character) and sets mFlag10 when
// they surround it
void CharaTask::fn_80176E30() {
    if (m12A0 > 0) {
        m12A0--;
        return;
    }
    EffectCharaSlot_unk* slots = GetEffectSlots_unk();
    if (slots == 0) {
        return;
    }
    m125C.mCount = 0;
    for (int i = 0; i < 0x48; i++) {
        if (m125C.full()) {
            break;
        }
        CharaTask* chara = slots[i].mChara;
        if (chara == 0 || !chara->CanAct_unk() || chara == this) {
            continue;
        }
        bool facing = false;
        if (chara->mType == 0) {
            if (chara->mFC0 == this) {
                facing = true;
            } else {
                atn::Vector3 dir = mPos - chara->mPos;
                if (!(atn::Abs_unk(LengthSq_unk(dir)) < 1.1920929e-7f)) {
                    dir.Normalize_unk();
                }
                if (Dot_unk(dir, chara->fn_8014052C()) < -0.5f) {
                    facing = true;
                }
            }
        } else {
            if (chara->mC58.mMode != 4 && chara->mC58.mMode != 5) {
                continue;
            }
            if (chara->mC58.mC4 == this) {
                facing = true;
            }
        }
        if (!facing) {
            continue;
        }
        atn::Vector3 dir = chara->mPos - mPos;
        if (!(atn::Abs_unk(LengthSq_unk(dir)) < 1.1920929e-7f)) {
            dir.Normalize_unk();
        }
        m125C.PushBack_unk(CharaPair_unk(atn::WrapAngle_unk(dir.ToRotation_unk().y - mRot.y), chara));
    }
    Sort_unk<CharaPair_unk, FloatLess_unk>(m125C.begin(), m125C.end(), fn_80135384);
    if (m125C.mCount <= 1) {
        mFlag10 = 0;
        return;
    }
    float scaleA = 1.0f;
    float scaleB = scaleA;
    if (mFlag10) {
        scaleA = 1.75f;
        scaleB = scaleA;
    }
    if (m50 == 0x189C1 && lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0 &&
        fn_80373148(lbl_8087F430)->m48 == 2 && fn_80373148(lbl_8087F430)->m4C == 13) {
        scaleA *= 2.0f;
        scaleB *= 1.5f;
    }
    CharaPair_unk* it = m125C.begin();
    bool wrapped = false;
    unsigned int count = 0;
    while (!wrapped) {
        float angle = it->m00;
        CharaTask* a = it->m04;
        it++;
        if (it == m125C.end()) {
            angle -= 6.2831855f;
            it = m125C.begin();
            wrapped = true;
        }
        float angle2 = it->m00;
        CharaTask* b = it->m04;
        atn::Vector3 da = a->mPos - mPos;
        atn::Vector3 db = b->mPos - mPos;
        float distA = LengthSq_unk(da);
        float distB = LengthSq_unk(db);
        float radiusA = scaleA * (1.2f * a->m64->m30);
        float sizeA = scaleB * a->vfA0();
        if (a->mType != 2) {
            radiusA *= 1.75f;
            sizeA *= 1.2f;
        }
        float radiusB = scaleA * (1.2f * b->m64->m30);
        float sizeB = scaleB * b->vfA0();
        if (a->mType != 2) {
            radiusB *= 1.75f;
            sizeB *= 1.2f;
        }
        if (distA > radiusA * radiusA || distB > radiusB * radiusB) {
            continue;
        }
        if (distA > 0.0f) {
            da.Normalize_unk();
        }
        if (distB > 0.0f) {
            db.Normalize_unk();
        }
        if (angle2 - angle <= 0.5f * (sizeA + sizeB)) {
            count++;
        }
    }
    if (count >= m125C.mCount) {
        if (!mFlag10) {
            mFlag10 = 1;
            if (mType == 0 && lbl_8087F430 != 0 && fn_80375184(lbl_8087F430)) {
                fn_801092C8(lbl_8087F048, 5, this, 0, 0);
            }
            m12A0 = mType == 2 ? 60 : 30;
        }
    } else {
        mFlag10 = 0;
    }
}

// inferred: input direction of the stick, the rotation when it is not tilted
atn::Vector3 CharaTask::fn_80178018() {
    atn::Vector3 dir;
    if (((Input_unk*)((char*)lbl_8087F0A8 + 0x48C))->fn_801248DC(&dir) < 0.4f) {
        dir = mRot;
    }
    return dir;
}

// inferred: tilt of the stick
float CharaTask::fn_80178078() {
    return ((Input_unk*)((char*)lbl_8087F0A8 + 0x48C))->fn_801248DC(0);
}

// inferred: position given by the running control, else the position
atn::Vector3 CharaTask::fn_80178088() {
    if (m55C == 6 && mControl) {
        return mControl->vf14();
    }
    return mPos;
}

// inferred: top of the collision capsule
atn::Vector3 CharaTask::fn_80178164() {
    return mColli.m48 + atn::Vector3(0.0f, 1.5f * m5B0, 0.0f);
}

// inferred: center of the collision capsule
atn::Vector3 CharaTask::fn_801781B0() {
    return (mColli.m3C + mColli.m48) * 0.5f;
}

void CharaTask::fn_80178208(chr::ControlBase* control) {
    if (m55C == 6) {
        if (mControl) {
            mControl->vf1C();
        }
    }
    fn_8016E970(6);
    mControl.reset(control);
}

// inferred: bullet instance that hits characters (class unknown): m04 is its data, m08 its owner and
// m80 a copy of the data used when fn_8059A264 is set
struct Bullet_unk {
    int fn_8059A264(); // inferred: m80 is in use
    // inferred: name invented
    HitInfo_unk* GetData_unk() {
        HitInfo_unk* data = m04;
        if (fn_8059A264()) {
            data = &m80;
        }
        return data;
    }

    unsigned char m00[0x04];
    HitInfo_unk* m04;
    CharaTask* m08;
    unsigned char m0C[0x80 - 0x0C];
    HitInfo_unk m80;
};

// inferred: arena modes 0 and 1 (name invented)
inline bool IsArena01_unk() {
    bool result = false;
    if (lbl_8087F610 != 0 && IsArenaMode_unk(lbl_8087F610->m540)) {
        result = true;
    }
    return result;
}

// inferred: records the hit of `bullet` (and of its second bullet) on the character
int CharaTask::fn_801784BC(Bullet_unk* bullet, int flag) {
    int result = 0;
    HitInfo_unk* data = bullet->GetData_unk();
    HitInfo_unk* sub = data->m6C > 0 ? fn_80219E6C(data->m6C) : 0;
    if (data != 0) {
        int time = sub != 0 ? 2 : data->m5C;
        unsigned int flags = fn_80179730(data, flag);
        if ((flags & 2) == 2 || (flags & 8) == 8 || (flags & 0x100) == 0x100) {
            time = 2;
        }
        if ((flags & 0x20) == 0x20 || (flags & 0x40) == 0x40 || (flags & 0x200) == 0x200 || (flags & 0x400) == 0x400) {
            time = 60;
        }
        result = fn_80178668(data, 0, time, bullet->m08, flag);
    }
    if (sub != 0) {
        int time = sub->m5C;
        unsigned int flags = fn_80179730(sub, flag);
        if ((flags & 2) == 2 || (flags & 8) == 8 || (flags & 0x100) == 0x100) {
            time = 2;
        }
        if ((flags & 0x20) == 0x20 || (flags & 0x40) == 0x40 || (flags & 0x200) == 0x200 || (flags & 0x400) == 0x400) {
            time = 60;
        }
        result |= fn_80178668(sub, data, time, bullet->m08, flag);
    }
    return result;
}

// NONMATCHING: ~73% (34/127 lines) - register allocation in the inlined side test (the original keeps
// the loop end in r0 and the side result in r25); tried other side-test helpers, no local for the
// attacker, constructing the hit in place or through PushBack_unk.
// inferred: adds a hit (or refreshes the one of the same bullet); returns 1 when recorded
int CharaTask::fn_80178668(HitInfo_unk* hit, HitInfo_unk* parent, int time, CharaTask* attacker, int flag) {
    for (CharaHit_unk* it = m12D0.begin(); it != m12D0.end(); it++) {
        if (it->mHit == hit) {
            CharaTask* other = (CharaTask*)it->m10;
            if (other == 0 || attacker == 0 || IsSameSideHit_unk(other, attacker)) {
                it->m04 = (int)parent;
                it->m08 = time;
                it->m10 = (int)attacker;
                return 1;
            }
        }
    }
    if (!m12D0.full()) {
        m12D0.PushBack_unk(CharaHit_unk(hit, (int)parent, time, fn_80179730(hit, flag), flag != 0, (int)attacker));
        return 1;
    }
    return 0;
}

// inferred: removes the hits of `hit` (or of its second bullet) made by `attacker`
int CharaTask::fn_80178864(HitInfo_unk* hit, CharaTask* attacker) {
    int result = 0;
    HitInfo_unk* sub = hit->m6C > 0 ? fn_80219E6C(hit->m6C) : 0;
    CharaHit_unk* it = m12D0.begin();
    while (it != m12D0.end()) {
        if ((it->mHit == hit || (sub != 0 && it->mHit == sub)) && (CharaTask*)it->m10 == attacker) {
            RemoveHit_unk(it);
            result = 1;
        } else {
            it++;
        }
    }
    return result;
}

// inferred: whether a hit with bullet id `id` (any if <= 0) and all the flags `flags` is recorded
int CharaTask::fn_801789D8(int id, unsigned int flags) {
    return FindHit_unk(id, flags) != 0;
}

extern "C" void fn_8059C8D8(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CA74(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059C98C(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CB2C(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CC9C(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CBE4(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CFB0(atn::Task* mgr, CharaTask* chara, int arg);
extern "C" void fn_8059D004(atn::Task* mgr, CharaTask* chara, int arg);
extern "C" void fn_8059D058(atn::Task* mgr, CharaTask* chara, int arg);
extern "C" void fn_8059D0AC(atn::Task* mgr, CharaTask* chara, int arg);
extern "C" void fn_8059D168(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg);

// inferred: ends the effects of the recorded hits and clears them
void CharaTask::fn_80178A6C() {
    if ((m1374 & 1) == 1) {
        fn_8059C8D8(lbl_8087F9E8, this);
    }
    if ((m1374 & 4) == 4) {
        fn_8059CA74(lbl_8087F9E8, this);
    }
    if ((m1374 & 2) == 2) {
        fn_8059C98C(lbl_8087F9E8, this);
    }
    if ((m1374 & 8) == 8) {
        fn_8059CB2C(lbl_8087F9E8, this);
    }
    if ((m1374 & 0x100) == 0x100) {
        fn_8059CC9C(lbl_8087F9E8, this);
    }
    if ((m1374 & 0x10) == 0x10) {
        fn_8059CBE4(lbl_8087F9E8, this);
    }
    if ((m1374 & 0x20) == 0x20) {
        fn_8059CFB0(lbl_8087F9E8, this, 0);
    }
    if ((m1374 & 0x40) == 0x40) {
        fn_8059D004(lbl_8087F9E8, this, 0);
    }
    if ((m1374 & 0x200) == 0x200) {
        fn_8059D058(lbl_8087F9E8, this, 0);
    }
    if ((m1374 & 0x400) == 0x400) {
        fn_8059D0AC(lbl_8087F9E8, this, 0);
    }
    if ((m1374 & 0x800) == 0x800) {
        fn_8059D168(lbl_8087F9E8, this);
    }
    m1374 = 0;
    m12D0.mCount = 0;
    m1410 = 0.0f;
    fn_800CB5C8(&m1414, 0, 0);
}

extern "C" void fn_8059C800(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059C99C(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059C8E8(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CA84(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CBF4(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CB3C(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CF70(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059CFC4(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059D018(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059D06C(atn::Task* mgr, CharaTask* chara);
extern "C" void fn_8059D0C0(atn::Task* mgr, CharaTask* chara);
extern "C" int fn_8021A960(HitInfo_unk* hit);
extern "C" int fn_8021A984(HitInfo_unk* hit);
extern "C" int fn_8021A9A8(HitInfo_unk* hit);
extern "C" void fn_800CB5B4(atn::SoundRef* sound, int frames);

// NONMATCHING: ~86% (100/717 lines) - register allocation of the summary loop (it/timer/end), of the
// inlined arena test (the original materializes the IsArenaMode_unk bool) and of the side tests;
// tried the side test helpers, the arena helper written with nested ifs and locals.
// inferred: per-frame update of the recorded hits (status effects, timers, the hit sound)
void CharaTask::fn_80178BFC() {
    if ((mStatus.m0C & 0x20) != 0x20 && (m54C & 0x2000) != 0x2000 && lbl_8087F9E8 != 0) {
        fn_8059C084(lbl_8087F9E8, this);
    }
    int removed = 0;
    if (m674 != 0 || m1208 != 0) {
        CharaHit_unk* it = m12D0.begin();
        while (it != m12D0.end()) {
            if ((it->m0C & 0x20) == 0x20 || (it->m0C & 0x40) == 0x40 || (it->m0C & 0x200) == 0x200 ||
                (it->m0C & 0x400) == 0x400) {
                m12D0.erase(it);
                removed = 1;
            } else {
                it++;
            }
        }
    }
    unsigned int effect = 0;
    unsigned int flags = 0;
    bool timer = false;
    bool guard = false;
    CharaHit_unk* end = m12D0.end();
    for (CharaHit_unk* it = m12D0.begin(); it != end; it++) {
        HitInfo_unk* hit = it->mHit;
        flags |= it->m0C;
        if (hit->m6C == 2 || hit->m6C == 5) {
            guard = true;
        }
        if ((hit->mAC & 0x2000) == 0x2000) {
            timer = true;
        }
    }
    if (lbl_8087F9E8 != 0 && (lbl_8087F9E8->mFlags & 1) != 1 && (lbl_8087F9E8->mFlags & 4) != 4 && !timer) {
        mStatus.m2F4 = 0.0f;
    }
    if (((unsigned int)mStatus.mParamB.m9C & 0x800) == 0x800) {
        guard = true;
    }
    for (CharaHit_unk* it = m12D0.begin(); it != m12D0.end(); it++) {
        if (it->m0E == 0 && it->m04 == 0 && !((guard || m55C == 5) && (it->m0C & 2) == 2) &&
            !((it->m0C & 0x100) == 0x100 && (it->m10 == 0 || (CharaTask*)it->m10 == this))) {
            mStatus.fn_8012E244(it->mHit);
            if (it->mHit->m01 != 1 && it->mHit->m01 != 4) {
                effect |= it->mHit->m90;
            }
        }
        if ((it->m0C & 0x100) == 0x100 && (CharaTask*)it->m10 == this) {
            flags |= 0x80;
            flags &= ~0x100;
        }
    }
    if ((m54C & 8) != 8 && !mFlagB1) {
        if ((m1374 & 1) != 1 && (flags & 1) == 1) {
            fn_8059C800(lbl_8087F9E8, this);
        }
        if ((m1374 & 4) != 4 && (flags & 4) == 4) {
            fn_8059C99C(lbl_8087F9E8, this);
        }
        if ((m1374 & 2) != 2 && (flags & 2) == 2) {
            fn_8059C8E8(lbl_8087F9E8, this);
        }
        if ((m1374 & 8) != 8 && (flags & 8) == 8 &&
            (mType != 2 || IsArena01_unk())) {
            fn_8059CA84(lbl_8087F9E8, this);
        }
        if ((m1374 & 0x100) != 0x100 && (flags & 0x100) == 0x100) {
            fn_8059CBF4(lbl_8087F9E8, this);
        }
        if ((m1374 & 0x10) != 0x10 && (flags & 0x10) == 0x10) {
            fn_8059CB3C(lbl_8087F9E8, this);
        }
        if ((m1374 & 0x20) != 0x20 && (flags & 0x20) == 0x20) {
            fn_8059CF70(lbl_8087F9E8, this);
        }
        if ((m1374 & 0x40) != 0x40 && (flags & 0x40) == 0x40) {
            fn_8059CFC4(lbl_8087F9E8, this);
        }
        if ((m1374 & 0x200) != 0x200 && (flags & 0x200) == 0x200) {
            fn_8059D018(lbl_8087F9E8, this);
        }
        if ((m1374 & 0x400) != 0x400 && (flags & 0x400) == 0x400) {
            fn_8059D06C(lbl_8087F9E8, this);
        }
        if ((m1374 & 0x800) != 0x800 && (flags & 0x800) == 0x800) {
            fn_8059D0C0(lbl_8087F9E8, this);
        }
    }
    if ((m1374 & 1) == 1 && (flags & 1) != 1) {
        fn_8059C8D8(lbl_8087F9E8, this);
    }
    if ((m1374 & 4) == 4 && (flags & 4) != 4) {
        fn_8059CA74(lbl_8087F9E8, this);
    }
    if ((m1374 & 2) == 2 && (flags & 2) != 2) {
        fn_8059C98C(lbl_8087F9E8, this);
    }
    if ((m1374 & 8) == 8 && (flags & 8) != 8) {
        fn_8059CB2C(lbl_8087F9E8, this);
    }
    if ((m1374 & 0x100) == 0x100 && (flags & 0x100) != 0x100) {
        fn_8059CC9C(lbl_8087F9E8, this);
    }
    if ((m1374 & 0x10) == 0x10 && (flags & 0x10) != 0x10) {
        fn_8059CBE4(lbl_8087F9E8, this);
    }
    if ((m1374 & 0x20) == 0x20 && (flags & 0x20) != 0x20) {
        fn_8059CFB0(lbl_8087F9E8, this, removed);
    }
    if ((m1374 & 0x40) == 0x40 && (flags & 0x40) != 0x40) {
        fn_8059D004(lbl_8087F9E8, this, removed);
    }
    if ((m1374 & 0x200) == 0x200 && (flags & 0x200) != 0x200) {
        fn_8059D058(lbl_8087F9E8, this, removed);
    }
    if ((m1374 & 0x400) == 0x400 && (flags & 0x400) != 0x400) {
        fn_8059D0AC(lbl_8087F9E8, this, removed);
    }
    if ((m1374 & 0x800) == 0x800 && (flags & 0x800) != 0x800) {
        fn_8059D168(lbl_8087F9E8, this);
    }
    m1374 = flags;
    mStatus.m240 = effect;
    CharaHit_unk* it = m12D0.begin();
    while (it != m12D0.end()) {
        if (--it->m08 <= 0) {
            RemoveHit_unk(it);
        } else {
            it++;
        }
    }
    if (mType == 0) {
        bool enemy = false;
        bool friendly = false;
        for (CharaHit_unk* it = m12D0.begin(); it != m12D0.end(); it++) {
            if (fn_8021A960(it->mHit) && it->mHit->mA0 < 0 && it->m10 != 0 &&
                !IsSameSideHit_unk((CharaTask*)it->m10, this)) {
                enemy = true;
            }
            if (fn_8021A984(it->mHit) && it->m10 != 0 && IsSameSideHit_unk((CharaTask*)it->m10, this)) {
                friendly = true;
            }
        }
        if (enemy && !friendly) {
            m1410 = 1.0f;
            if (m1414.mPtr == 0) {
                m1414 = fn_800C31F4(sSoundNames_unk[6], 0, -1, 1.0f);
            } else {
                fn_800CB5B4(&m1414, 10);
            }
        } else {
            m1410 = MIN(MAX(m1410 - 1.0f / 30.0f, 0.0f), 1.0f);
            if (m1410 > 0.0f) {
                fn_800CB5B4(&m1414, 10);
            } else if (m1414.mPtr != 0) {
                fn_800CB5C8(&m1414, 0, 0);
            }
        }
    }
}

// inferred: flags of a hit (0x1 and 0x4 percentage changes, 0x2/0x8/0x10/0x100 kinds, 0x20-0x400
// timed effects, 0x800 special)
unsigned int CharaTask::fn_80179730(HitInfo_unk* hit, int flag) {
    unsigned int flags = 0;
    if (fn_8021A8D0(hit)) {
        for (int i = 0; i < 4; i++) {
            if (hit->GetRates_unk()[i] != 0) {
                flags |= 1;
                break;
            }
        }
    }
    if (fn_8021A918(hit)) {
        if ((hit->mAC & 0x2000) == 0x2000) {
            flags |= 0x10;
        } else if (fn_8021A9A8(hit)) {
            for (int i = 0; i < 4; i++) {
                if (hit->GetRates_unk()[i] != 0) {
                    flags |= 4;
                    break;
                }
            }
        }
    }
    if (flag) {
        if (hit->m74.m00 > 0.0f) {
            flags |= 0x20;
        }
        if (hit->m74.m04 > 0.0f) {
            flags |= 0x40;
        }
        if (hit->m74.m10 > 0.0f) {
            flags |= 0x200;
        }
        if (hit->m74.m0C > 0.0f) {
            flags |= 0x400;
        }
        if ((hit->mAC & 0x40000000) == 0x40000000) {
            flags |= 0x1000;
        }
    } else if (fn_8021A960(hit)) {
        flags |= 2;
    }
    if (hit->m98 != 0) {
        if ((float)hit->m98 > 0.0f) {
            flags |= 8;
        }
        flags &= ~0x14;
    }
    if ((hit->mAC & 0x100000) == 0x100000) {
        flags |= 0x100;
        flags &= ~0x1C;
        lbl_8087F430->fn_803750E4(0xE9);
    }
    if ((hit->mAC & 0x400000) == 0x400000 && hit->m98 == 0) {
        flags |= 0x800;
        flags &= ~0x1D;
    }
    if ((flags & 0x10) == 0x10 && (mType == 2 || mStatus.HasTimer_unk())) {
        flags &= ~0x10;
    }
    return flags;
}

// inferred: flags of all the recorded hits
unsigned int CharaTask::fn_801799BC() {
    unsigned int flags = 0;
    for (CharaHit_unk* it = m12D0.begin(); it != m12D0.end(); it++) {
        flags |= fn_80179730(it->mHit, it->m0E);
    }
    return flags;
}

// inferred: id of the character from its name ("..XX..YY.ZZ": XX * 10000 + YY * 100 + ZZ)
int CharaTask::fn_80179A34() {
    if (m60 != 0) {
        int a;
        int b;
        b = atoi(m60->m10 + 8) * 100;
        a = atoi(m60->m10 + 2) * 10000;
        return atoi(m60->m10 + 11) + (a + b);
    }
    return 0;
}

// inferred: kind of the character from its name
int CharaTask::fn_80179AB4() {
    return m60 != 0 ? fn_80208748(m60->m10) : 0;
}

// NONMATCHING: ~16% by line (132/157, same instructions) - the original unrolls the equipment loop with
// one pointer to m680[1] (offsets 4..0x18), ours advances a base by 8; tried int/unsigned index, a
// local copy, a pointer loop (not unrolled), peeling element 0.
// inferred: sets or clears model flags on the model, the weapons, the equipment and the items
void CharaTask::fn_80179AD0(unsigned int mask, int clear) {
    if (clear) {
        mMotion.mFlags &= ~mask;
        if (m648 != 0) {
            m648->m014 &= ~mask;
            if (m64C != 0) {
                m64C->m014 &= ~mask;
            }
        }
        for (int i = 0; i < 8; i++) {
            if (m680[i] != 0) {
                m680[i]->m028 &= ~mask;
            }
        }
        for (unsigned int i = 0; i < m6A4.mCount; i++) {
            m6A4.mData[i]->mModel.mFlags &= ~mask;
        }
    } else {
        mMotion.mFlags |= mask;
        if (m648 != 0) {
            m648->m014 |= mask;
            if (m64C != 0) {
                m64C->m014 |= mask;
            }
        }
        for (int i = 0; i < 8; i++) {
            if (m680[i] != 0) {
                m680[i]->m028 |= mask;
            }
        }
        for (unsigned int i = 0; i < m6A4.mCount; i++) {
            m6A4.mData[i]->mModel.mFlags |= mask;
        }
    }
}

// inferred: arena mode 2 (name invented)
inline bool IsArena2_unk() {
    bool result = false;
    if (lbl_8087F610 != 0 && lbl_8087F610->m540 == 2) {
        result = true;
    }
    return result;
}

// NONMATCHING: ~68% by line (30/94, same instructions) - register names only (mType in r6, the arena
// bool in r5 in the original); tried a type local, the arena test written inline, nested and as an
// expression (the nested bool form gets the registers right but keeps 4 extra instructions).
// inferred: collision mask of the character
int CharaTask::fn_80179D44() {
    unsigned int mask = 0;
    if (mType == 0) {
        mask |= 0x20;
    } else if (mType == 3) {
        mask |= 0x40;
    } else if (mType == 2) {
        if (!(lbl_8087F430 != 0 && lbl_8087F430->m48 == 2 && lbl_8087F430->m4C == 25 && lbl_8087F430->m50 == 2)) {
            mask |= 0x80;
        }
    } else if (IsPartyMember_unk()) {
        mask |= 0x800;
    }
    if (mFlag3) {
        mask |= 0x10000;
    }
    if (m60 != 0) {
        int kind = m60->m24;
        if (kind == 0x1A || kind == 0x3B || kind == 0x3C || kind == 0x19 ||
            (kind == 0x17 && atn::Abs_unk(m568) < 1.1920929e-7f)) {
            mask |= 0x20000000;
        }
        if (kind == 0x23) {
            mask |= 0x80000000;
        }
        if (IsArena2_unk() && mType == 2 && lbl_8087F430->m4C == 3 && lbl_8087F430->m50 == 6) {
            mask |= 0x80000000;
        }
    }
    if (mFlag22 == 1) {
        mask |= 0x20000;
    }
    if (mFlagB1) {
        mask |= 0x80000000;
    }
    return mask;
}

// inferred: whether the character can react to an action of kind `kind`
bool CharaTask::fn_80179EBC(int kind) {
    Settings_unk* settings = lbl_8087F0A8;
    if ((mType == 0 && settings->m308 == 0) || (mFlags & 4) == 4 || (m54C & 0x2000) == 0x2000 || mC54 == 0) {
        return false;
    }
    if (m7C == 0) {
        return false;
    }
    if (kind != 10 && (mStatus.m0C & 0x20) == 0x20) {
        return false;
    }
    if (m1380 > 0 && !((unsigned int)(kind - 0x24) <= 3) && !((unsigned int)(kind - 10) <= 1)) {
        if (kind == 2) {
            if (m1384 != 0 && m1384 != 1) {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

// inferred: blocks the reactions for 90 frames (see fn_80179EBC)
void CharaTask::fn_80179F98(int value) {
    m1384 = value;
    m1380 = 90;
}

extern "C" int fn_803761A4(Field_unk* field);

// inferred: whether the character can use its weapon now
bool CharaTask::fn_80179FA8() {
    if (lbl_8087F430 == 0 || !fn_803761A4(lbl_8087F430)) {
        return false;
    }
    if (!CanReact_unk()) {
        return false;
    }
    if (lbl_8087F610 == 0 && !mStatus.fn_801346C8(0xE)) {
        return false;
    }
    bool result = false;
    if (HasWeapon_unk() && IsArenaMode_unk(m5C->m11C)) {
        result = true;
    }
    return result;
}

// inferred: vf34 of the running control
int CharaTask::fn_8017A160() {
    if (m55C == 6 && mControl) {
        return mControl->vf34();
    }
    return 0;
}

// inferred: vf38 of the running control
int CharaTask::fn_8017A228(int arg) {
    if (m55C == 6 && mControl) {
        return mControl->vf38(arg);
    }
    return -1;
}

// inferred: changes the character data and updates the status
void CharaTask::fn_8017A300(chr::CharaData_unk* data) {
    m5C = data;
    mStatus.fn_8012B3E8();
    mStatus.fn_8012B988();
}

// inferred: makes `rider` follow the "spine" node of this character (0 releases the current one)
void CharaTask::fn_8017A33C(CharaTask* rider, int release) {
    if (rider != 0) {
        mMotion.fn_80097C08(3, 0x1F3, 1, 0, 1, 1.0f, 0.1f);
        rider->mF1C = (int)mMotion.GetNodeMtx_unk("spine");
        rider->mColli.mFlags &= ~1;
        rider->mFlag11 = 1;
    } else if (m139C != 0) {
        fn_80097CCC(&mMotion, 3, 0.0f);
        if (release) {
            m139C->mF1C = 0;
            m139C->mColli.mFlags |= 1;
            m139C->mFlag11 = 0;
        }
    }
    m139C = rider;
}

void CharaTask::fn_8017ABD4(int arg) {
    m68[1] = fn_80204E04(mType, arg);
}

// inferred: returns the current control's vf30 value, 0 without a control
int CharaTask::fn_8017A450() {
    return mControl ? mControl->vf30() : 0;
}

// inferred: float color built from an ARGB word (names invented)
struct ColorF_unk {
    ColorF_unk() {}
    ColorF_unk(unsigned int argb) {
        r = (1.0f / 255.0f) * (float)((argb >> 16) & 0xFF);
        g = (1.0f / 255.0f) * (float)((argb >> 8) & 0xFF);
        b = (1.0f / 255.0f) * (float)(argb & 0xFF);
        a = (1.0f / 255.0f) * (float)(argb >> 24);
    }
    ColorF_unk& operator=(const ColorF_unk& o) {
        r = o.r;
        g = o.g;
        b = o.b;
        a = o.a;
        return *this;
    }
    float r, g, b, a;
};

// inferred: sets or clears the status 0x800 of an enemy (mType 2) with its effects
void CharaTask::fn_8017A5D8(int on) {
    if (mType != 2 || IsHidden_unk()) {
        return;
    }

    if (on && (mStatus.m0C & 0x800) != 0x800) {
        mStatus.fn_8013310C(0x800, 0);
        if (mStatus.m430 <= 0) {
            mStatus.fn_80134724(1);
        }
        if (fn_8017A9A8() && fn_8015487C() == 1) {
            fn_8014DEE4(1, 0, 0);
        }
        if (lbl_8087F048 != 0) {
            ColorF_unk color;
            if (mStatus.fn_80134270()) {
                color = ColorF_unk(lbl_8087F0A8->m370);
            } else if (mStatus.fn_80134250()) {
                color = ColorF_unk(lbl_8087F0A8->m36C);
            } else {
                color = ColorF_unk(lbl_8087F0A8->m368);
            }
            lbl_8087F048->fn_801065F4(this, &mMotion, &color);
        }
        if (m1388 <= 0 && mFlag22 != 1) {
            m1388 = 450;
            if (fn_80160324()) {
                fn_8016034C();
            }
        }
        if (m138A <= 0 && mFlag22 != 1) {
            m138A = 240;
        }
    } else if (!on && (mStatus.m0C & 0x800) == 0x800) {
        mStatus.fn_8013322C(0x800);
        mStatus.m430 = 0;
        mStatus.m434 = 0.0f;
        mStatus.m438 = 0.0f;
        if (fn_8017A9A8() && fn_8015487C() == 1) {
            fn_8014DEE4(0, 0, 0);
        }
    }
}


// inferred: puts the stored foot heights (m13B8) on the ground below the neck node
// NONMATCHING: 58% - the segment and the hit point swap their stack slots and the x/z loads swap
// registers; tried: named or temporary segment, constructors, member-wise stores, declaration
// orders, the hit returned by value or through a pointer.
void CharaTask::fn_8017A504() {
    m54C |= 0x200000;
    if (fn_80418318()) {
        MtxPtr node = mMotion.GetNodeMtx_unk(m520);
        int world = fn_80418318();
        float x = node[0][3];
        float y = node[1][3];
        float z = node[2][3];
        float dy = y - mPos.y;
        Segment_unk seg;
        seg.mStart = mPos;
        seg.mEnd.x = x;
        seg.mEnd.y = y;
        seg.mEnd.z = z;
        atn::Vector3 hit = fn_804188A0(world, &seg);
        hit.y -= dy;
        m13B8[0].y = hit.y;
        m13B8[1].y = hit.y;
    }
}


// inferred: whether the character shows its weapon-kind specific effect (kind from fn_8015487C)
int CharaTask::fn_8017A9A8() {
    Settings_unk* settings = lbl_8087F0A8;
    int kind = fn_8015487C();
    if (m68[2] != 0 && sTable807374B8_unk[kind] > 0) {
        int result = 0;
        switch (kind) {
        case 0:
            if ((m137C & 0x200) != 0x200) {
                result = 1;
            }
            break;
        case 1:
            if (settings->m404 >= 1 && (m137C & 0x200) != 0x200) {
                result = 1;
            }
            break;
        case 2:
        case 5:
            if (settings->m404 >= 2) {
                if ((m137C & 0x200) != 0x200) {
                    if (m1388 <= 0 || 300 < m1388) {
                        result = 1;
                    }
                } else if (m1388 >= 0x1BD) {
                    result = 1;
                }
                HitInfo_unk* skill = mStatus.m2D0[0].m[0] > 0 ? fn_80219E6C(mStatus.m2D0[0].m[0]) : 0;
                if (skill != 0 && fn_8021A960(skill) && (skill->mAC & 0x20) == 0x20) {
                    result = 0;
                }
            }
            break;
        case 3:
        case 4:
            if (settings->m404 >= 3 && (m1388 <= 0 || 300 < m1388)) {
                result = 1;
            }
            break;
        }
        CharaTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (result && player != 0) {
            bool same = mC58.mB4 == player->mC58.mB4;
            if (kind == 1) {
                bool ok = false;
                if (mParts.mCount > 1 && mParts.mData[1]->m274->mParam.m78 == 0) {
                    ok = true;
                }
                if (same && ok) {
                    return 1;
                } else {
                    return 0;
                }
            }
            return same;
        }
    }
    return 0;
}

int CharaTask::fn_8017AC08() {
    if (m68[1] != 0) {
        return m68[1]->m04;
    }
    return 0;
}

float CharaTask::fn_8017AC24() {
    return 1.0f;
}

extern "C" void fn_80106680(Effect_unk* effect, CharaTask* chara, Model_unk* model);

void CharaTask::fn_8017AC2C() {
    fn_80106680(lbl_8087F048, this, &mMotion);
}

void CharaTask::fn_8017AC3C(int value) {
    m13FC = (CharaTask*)value;
}

void CharaTask::fn_8017AC44(int clear) {
    if (m13FC != 0) {
        fn_801446F0();
        mF1C = 0;
        atn::Vector3 dir = fn_8014052C();
        Mtx m;
        PSMTXRotRad(m, 'y', -1.5707964f);
        PSMTXMultVec(m, &dir, &dir);
        dir *= 5.0f;
        m574 = dir;
        if (clear != 0) {
            m13FC = 0;
        }
    }
}

// inferred: name table of the field (lbl_8087F430 + 0x54F4) (names invented)
struct NameEntry_unk {
    unsigned char m00[0x08];
    const wchar_t* mName;
};
extern "C" NameEntry_unk* fn_803B3B38(void* table, int id);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

extern "C" void fn_80096218(Model_unk* model, atn::Vector_unk<int>* list, void* arg); // inferred: collects resources
extern "C" void fn_80563A60(chr::CharaEquip_unk* equip, atn::Vector_unk<int>* list, void* arg);
extern "C" int fn_80473F34(atn::LH_MotionSet* set);

// inferred: collects the resources of the character (models, parts, equipment, motion set)
void CharaTask::fn_8017AD44(atn::Vector_unk<int>* list, void* arg) {
    fn_80096218(&mMotion, list, arg);
    for (unsigned int i = 0; i < mParts.mCount; i++) {
        fn_80096218((Model_unk*)mParts.mData[i]->m010, list, arg);
    }
    chr::CharaEquip_unk** equips = m680;
    for (unsigned int i = 0; i < 8; i++) {
        if (equips[i] != 0) {
            fn_80563A60(equips[i], list, arg);
        }
    }
    for (unsigned int i = 0; i < m6A4.mCount; i++) {
        fn_80096218(&m6A4.mData[i]->mModel, list, arg);
    }
    list->push_back(fn_80473F34(&m98));
}

int CharaTask::vfB8(wchar_t* name) {
    if (m5C != 0 && m5C->mC4 > 0 && lbl_8087F430 != 0) {
        NameEntry_unk* entry = fn_803B3B38(lbl_8087F430->m54F4, m5C->mC4);
        if (entry != 0) {
            wcscpy(name, entry->mName);
            return 1;
        }
    }
    return 0;
}

bool CharaTask::fn_8017B16C() {
    if (mType == 0 && lbl_8087F0A8->m278 != 0 && m5C->m11C == 1) {
        return true;
    }
    if (mType == 0 && mFlagB17) {
        return true;
    }
    return false;
}

// inferred: copies the list into m1400 and resets m140C
void CharaTask::fn_8017B1C4(atn::Vector_unk<int>* list) {
    m140C = -1;
    m1400.Assign_unk(*list);
}

bool CharaTask::fn_8017B3C0() {
    bool result = false;
    if (mC58.mC0 == 0 || (mC58.m11C.fn_8011D1FC() && !mC58.m11C.fn_8011D21C())) {
        if (mFlagB14) {
            result = true;
        }
    }
    return result;
}

// inferred: route tables of the field (name invented)
inline RouteTable_unk* GetRouteTable_unk() {
    return lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
}

extern "C" void fn_8011C044(CharaC58_unk* c58, int arg);

// inferred: reports a point move to a missing warp point (name invented)
static void ReportPointMove_unk(CharaTask* chara, int id) {
    if (lbl_8087EEB8 != 0) {
        char message[0x100];
        sprintf(message, "error: invalid pointmove chara[%d:%s] -> warppoint[%d]\n", chara->m58, chara->fn_8017C904(), id);
        ((DebugMessages_unk*)lbl_8087EEB8)->fn_800697D8(message);
    }
}

// NONMATCHING: ~94% (32/518 lines, 8 of them the ptmf/string pool noise) - the route table and the
// loop offset swap r31/r30, and the m5B0 load goes to f31 instead of f2; tried locals in other orders,
// the ternary written inline, the m5B0 accessor.
// inferred: moves the character (or its rider, m13FC) to the next place of the list m1400
void CharaTask::fn_8017B434() {
    if (m1400.mSize < 2) {
        return;
    }
    RouteTable_unk* table = GetRouteTable_unk();
    if (table == 0) {
        return;
    }
    float best = 1e16f;
    RoutePlace_unk* nearest = 0;
    for (unsigned int i = 0; i < m1400.mSize; i++) {
        RoutePlace_unk* place = table->FindPlace_unk(m1400.mData[i]);
        if (place != 0) {
            float dist = LengthSq_unk(place->mPos - mPos);
            if (dist < best) {
                best = dist;
                m140C = i;
                nearest = place;
            }
        }
    }
    if (m140C < 0) {
        return;
    }
    if (LengthSq_unk(nearest->mPos - mPos) < (1.2f * fn_8013C42C()) * (1.2f * fn_8013C42C())) {
        m140C = (m140C + 1) % m1400.mSize;
    }
    int id = m1400.mData[m140C];
    CharaTask* rider = (CharaTask*)m13FC;
    if (rider != 0) {
        if (rider->m13FC != 0) {
            ((CharaTask*)rider->m13FC)->fn_8017039C(id, 8);
            return;
        }
        if (GetRouteTable_unk() == 0) {
            return;
        }
        if (id > 0) {
            rider->mRoute.fn_80128818(id, &rider->mPos, 8);
            if (rider->mRoute.fn_801255C8()) {
                rider->fn_801718F8();
                return;
            }
        }
        ReportPointMove_unk(rider, id);
        return;
    }
    if (GetRouteTable_unk() == 0) {
        return;
    }
    if (id > 0) {
        mRoute.fn_80128818(id, &mPos, 8);
        if (mRoute.fn_801255C8()) {
            Reset_unk(0, 1, 1, 1);
            fn_8011C044(&mC58, 0);
            fn_80017064(lbl_8087EE68, this);
            fn_8016E970(7);
            if (m1208 != 0) {
                fn_801750FC();
            }
            return;
        }
    }
    ReportPointMove_unk(this, id);
}

void CharaTask::fn_8017BC4C() {
    m140C = -1;
}

bool CharaTask::vfC4() {
    if (!CanAct_unk()) {
        return false;
    }
    if (mStatus.mParamB.fn_801404E0(2)) {
        return false;
    }
    if (m55C == 6) {
        switch (m560) {
        case 0x17:
        case 0x1B:
        case 0x1C:
        case 0x1F:
        case 0x23:
        case 0x24:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x36:
        case 0x37:
        case 0x3B:
        case 0x3F:
        case 0x5A:
        case 0x6A:
        case 0x71:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F:
        case 0x8E:
            return false;
        }
    }
    return true;
}

// inferred: random value in [0, 1] (name invented)
inline float RandRate_unk() {
    return (float)(rand() % 1001) / 1000.0f;
}

// inferred: `v` transformed by `m` (name invented)
inline atn::Vector3 TransformVec_unk(const Mtx m, const atn::Vector3& v) {
    atn::Vector3 r(v);
    PSMTXMultVec(m, &r, &r);
    return r;
}

// NONMATCHING: ~90% (43/442 lines) - the original keeps each matrix address in r28 (this moves to r28
// too) and adds the angle constant with the operands the other way round; tried an inline helper for
// the whole point (worse stack layout), matrix pointer locals, other operand orders.
// inferred: four points around the character away from `dir` (out[3] is the position behind it)
void CharaTask::vfCC(atn::Vector3* out, const atn::Vector3& vec) {
    Mtx m0;
    Mtx m1;
    Mtx m2;
    atn::Vector3 dir(vec.x, 0.0f, vec.z);
    if (!(atn::Abs_unk(LengthSq_unk(dir)) < 1.1920929e-7f)) {
        dir.Normalize_unk();
    }
    float angle = atn::WrapAngle_unk(3.1415927f + dir.ToRotation_unk().y);
    out[3] = mPos - dir;
    PSMTXRotRad(m0, 'y', angle + 2.6179938f - 1.3962634f * RandRate_unk());
    out[0] = out[3] - dir * 30.0f + TransformVec_unk(m0, atn::Vector3(0.0f, 0.0f, 10.0f * RandRate_unk() + 20.0f));
    PSMTXRotRad(m1, 'y', angle + 0.6981317f - 1.3962634f * RandRate_unk());
    out[1] = out[3] - dir * 30.0f + TransformVec_unk(m1, atn::Vector3(0.0f, 0.0f, 10.0f * RandRate_unk() + 20.0f));
    PSMTXRotRad(m2, 'y', angle - 1.2217305f - 1.3962634f * RandRate_unk());
    out[2] = out[3] - dir * 30.0f + TransformVec_unk(m2, atn::Vector3(0.0f, 0.0f, 10.0f * RandRate_unk() + 20.0f));
}

// NONMATCHING: the original loads 0.6f into f0 and m5B4 into f2 (ours the other way round);
// tried member and local forms of the offset.
atn::Vector3 CharaTask::vfD0(float angle) {
    atn::Vector3 offset(0.0f, 0.6f * m5B4, 0.0f);
    atn::Vector3 pos(mPos.x + offset.x, mPos.y + offset.y, mPos.z + offset.z);
    Mtx m;
    PSMTXRotRad(m, 'y', angle);
    return pos;
}

extern "C" void fn_80094BD8(void* model, int mode);

// NONMATCHING: 88% - only registers: the original keeps &d in r30 and &dir in r29, &from in r28 and
// &base in r27 (ours swapped); tried: copy/assignment forms of d and from, declarations outside the
// loop.
atn::Vector3 CharaTask::vfD8(CharaTask* target) {
    atn::Vector3 base = vf64(1);
    atn::Vector3 dir = mPos - target->mPos;
    if (fn_805F9920(&dir) > 0.001f) {
        PSVECNormalize(&dir, &dir);
    } else {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 1.0f;
    }
    atn::Vector3 pos = base - dir * 15.0f;
    CollResult_unk coll;
    Mtx rot;
    for (unsigned int i = 0; i < 8; i++) {
        PSMTXRotRad(rot, 'y', 3.1415927f * (float)i / 4.0f);
        atn::Vector3 d = dir;
        PSMTXMultVec(rot, &d, &d);
        atn::Vector3 from = base;
        from.y += 5.0f;
        atn::Vector3 to = from - d * 20.0f;
        if (fn_8004ED34(lbl_8087EE98, &coll, &from, &to, 0x80000000, 0, 0) == 0) {
            pos = base - d * 15.0f;
            break;
        }
    }
    return pos;
}

void CharaTask::fn_8017C7F8(unsigned int on) {
    if (mFlagB1 != on) {
        mFlagB1 = on;
        int mode = 1;
        if (mFlagB1) {
            mode = 4;
        }
        fn_80094BD8(&mMotion, mode);
        for (unsigned int i = 0; i < mParts.mCount; i++) {
            if (mParts.mData[i] != 0) {
                fn_80094BD8(mParts.mData[i]->m010, mode);
            }
        }
        chr::CharaEquip_unk** equips = m680;
        for (unsigned int i = 0; i < 8; i++) {
            if (equips[i] != 0) {
                fn_80094BD8(equips[i]->m024, mode);
            }
        }
        for (unsigned int i = 0; i < m6A4.mCount; i++) {
            fn_80094BD8(&m6A4.mData[i]->mModel, mode);
        }
    }
}

const char* CharaTask::fn_8017C904() {
    sprintf(lbl_807C7B28, "ch%04d_%02d", m50 / 100, m50 % 100);
    return lbl_807C7B28;
}

int CharaTask::fn_8017C974() {
    if (lbl_8087F430 != 0 && lbl_8087F430->m5760 > 0) {
        return 1;
    }
    return (m54C >> 4) & 1;
}

int CharaTask::fn_8017C9A0() {
    return (m54C >> 4) & 1;
}

// inferred: point of the camera ray (eye -> target) closest to the character, moved 5 units further
// NONMATCHING: 81% - only the stack slots of the four Vector3 temporaries are in reverse order
// (target: target-eye lowest, then dir*5, dir*800, eye+dir*800); tried: named far/eye, +=,
// Assign_unk, Segment_unk constructor, declaration orders.
atn::Vector3 fn_8017C9AC(CharaTask* chara) {
    View_unk* view;
    if (lbl_8087F430 != 0) {
        view = &lbl_8087F430->m260;
    } else {
        view = &lbl_8087EFB4->m104;
    }
    Segment_unk seg;
    atn::Vector3 dir;
    dir = view->mTarget - view->mEye;
    PSVECNormalize(&dir, &dir);
    atn::Vector3 eye = view->mEye;
    seg.mStart = eye;
    seg.mEnd = eye + dir * 800.0f;
    atn::Vector3 result;
    fn_80050A1C(&chara->mPos, &seg, &result);
    result += dir * 5.0f;
    return result;
}

int CharaTask::fn_8017CB2C() {
    int result = 0;
    if ((m137C & 0x400) == 0x400 || m50 == 201612 || m50 == 203932 || m50 == 203900) {
        result = 1;
    }
    return result;
}

int CharaTask::vfA4() {
    return 0;
}

int CharaTask::vf0C() {
    return fn_8013655C() == 0;
}
