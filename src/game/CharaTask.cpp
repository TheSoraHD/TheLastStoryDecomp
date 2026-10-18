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
#include "atn/Memory.h"
#include "game/GimmickCommon.h"
#include "game/CharaStatus.h"
#include "game/GimmickCommon.h"
#include <boost/scoped_ptr.hpp>
#include "std/string.h"
#include "std/string_members.h"
#include "atn/Csv.h"
#include "game/EquipItemData.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/tokenizer.hpp>

// inferred: game settings (lbl_8087F0A8, as in CharaStatus.cpp); names invented
struct Settings_unk {
    int fn_8013C41C();
    int* fn_8013C434();
    float fn_8013C50C();

    unsigned char m00[0x48];
    float m48;
    unsigned char m4C[0xCC - 0x4C];
    int mCC; // inferred: nonzero makes every character a valid target (fn_80154E38)
    unsigned char mD0[0x250 - 0xD0];
    int m250; // inferred: copied to CharaTask::m1028 by fn_80164D24
    unsigned char m254[0x278 - 0x254];
    int m278; // inferred: parts mode (fn_801539E0)
    unsigned char m27C[0x3D0 - 0x27C];
    int m3D0; // inferred: tested by CharaTask::fn_80164D24
    unsigned char m3D4[0x3D8 - 0x3D4];
    int m3D8;
    unsigned char m3DC[0x418 - 0x3DC];
    int m418;
    unsigned char m41C[0x454 - 0x41C];
    int m454;
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
// inferred: closest point of a segment to a point (0x80050A1C)
extern "C" void fn_80050A1C(const atn::Vector3* point, const Segment_unk* seg, atn::Vector3* out);

// inferred: field data (lbl_8087F430); m10D8 holds the route tables (see RouteWalker_unk.cpp)
struct Field_unk {
    RouteTable_unk* fn_8013C504();
    void fn_803750E4(int id); // inferred: called with 0xB5 when a knocked-down character had mFlag28

    unsigned char m00[0x260];
    View_unk m260;
    unsigned char m280[0x868 - 0x280];
    int m868;
    int m86C;
    unsigned char m870[0x10D8 - 0x870];
    RouteTable_unk* m10D8;
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4; // inferred: 9 makes every hit valid (fn_801533C8)
    unsigned char m54E8[0x54F4 - 0x54E8];
    unsigned char m54F4[4]; // inferred: name table (fn_803B3B38)
    unsigned char m54F8[0x5744 - 0x54F8];
    int m5744; // inferred: copied to CharaTask::mFD8 by fn_80164B00
    unsigned char m5748[0x5760 - 0x5748];
    int m5760;
};
extern "C" Field_unk* lbl_8087F430;

// inferred: camera (lbl_8087EFB4)
struct CharaCamera_unk {
    unsigned char m00[0x104];
    View_unk m104;
};
extern "C" CharaCamera_unk* lbl_8087EFB4;

// inferred: effect manager (lbl_8087F048, as in CharaStatus.cpp)
struct Effect_unk {
    void fn_8013A19C(int value);

    unsigned char m00[0x3E39C];
    int m3E39C;
};
extern "C" Effect_unk* lbl_8087F048;


extern "C" int fn_80375184(Field_unk* field);
extern "C" int fn_80370174(Field_unk* field, int id);  // inferred: event flag
extern "C" void fn_80370320(Field_unk* field, int id, int value, int arg); // inferred: sets an event flag
extern "C" void* fn_80219E6C(int id); // inferred: bullet data by id (as in gmk/LandMine.cpp)
extern "C" int fn_800F8548(Effect_unk* mgr);
extern "C" void fn_800FD658(Effect_unk* mgr, int a, int b, void* bullet, int arg4, int arg5, int arg6);

// inferred: globals returned by accessors that are called out of line
extern "C" void* lbl_8087EE68;
extern "C" void* lbl_8087EE98;
extern "C" void* lbl_8087F490;

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

extern "C" atn::Task* lbl_8087F128;
extern "C" int lbl_8087FA20;
extern "C" void* lbl_8087EEB8; // inferred: debug message output
extern "C" char lbl_807C7B28[32];
extern "C" int fn_8036823C(Field_unk* field);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int rand();
extern "C" void fn_800697D8(void* output, const char* message);
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
    if (parent == lbl_8087F128) {
        return 0xEF;
    }
    return IsTownField_unk() ? 0x13F : 0x23A;
}

// NONMATCHING: 99.8% - the instructions are the same; only the .sdata2 offsets of the float
// constants differ until "" (sStatusNames_unk) is the first .sdata2 object, which needs its user
// fn_80149A30 (checked: with the table referenced this is 100%).
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
      mFlag2(0), mFlag1_0(0), mFlagB31_28(0), mFlagB27(0), mFlagB26(0), mFlagB25(0), mFlagB24_21(0), mFlagB20(1), mFlagB18(0), mFlagB17(0), mFlagB16(0), mFlagB15(0), mFlagB14(1), mFlagB13_11(0),
      mFlagB10(1), mFlagB9(0), mFlagB8(0), mFlagB7(1), mFlagB6_2(0), mFlagB1(0), mFlagB0(0), m12AC(2), m12B0(0), m12B4(100), m12B8(100),
      m12BC(-1), m12C0(0), m12C4(0), m12C8(0), m12CC(0.0f), m1374(0), m137C(0), m1380(0), m1384(0),
      m1388(0), m138A(0), m138C(0), m1390(rand() % 4), m1398(0), m139C(0), m13AC(0), m13B0(0),
      m13B4(1.0f), m13D8(0.0f), m13DC(0.0f), m13E0(0.0f), m13E4(0.0f), m13E8(0.0f), m13EC(0.0f),
      m13F0(0.0f), m13F4(4), m13F8(0), m13FC(0), m1418(0), m141C(0.0f),
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
                fn_800697D8(lbl_8087EEB8, message);
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

// inferred: camera object owned by a model (see Model_unk::m208); 0x1FC bytes
struct Camera_unk {
    Camera_unk(int a = 0, int b = 0);
    ~Camera_unk();
    unsigned char m00[0x1F4];
};
struct ModelCamera_unk {
    ModelCamera_unk() : m00(0) {}
    int m00;
    int m04;
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
extern "C" int lbl_8087F610; // inferred: nonzero disables "PBONE"
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

void* fn_80139EF4() {
    return lbl_8087EE68;
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

void atn::Matrix3x4::fn_8013A13C(float rad) {
    PSMTXRotRad(m, 'y', rad);
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

atn::Vector3* CharaTask::fn_8013C38C() {
    return &mPos;
}

void atn::Vector3::fn_8013C394(const Vector3& a, const Vector3& b) {
    PSVECCrossProduct(&a, &b, this);
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

int* Settings_unk::fn_8013C434() {
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

bool CharaParam_unk::fn_801404E0(unsigned int flag) {
    return (m9C & flag) == flag;
}

void* fn_801404F8() {
    return lbl_8087EE98;
}

bool CharaTask::fn_80140518() {
    return mType == 3;
}

// inferred: forward direction of the character (rotated by mRot.y)
// NONMATCHING: 99.8% - only the .sdata2 offset of 0.0f differs (the pool order needs the
// functions before this one).
atn::Vector3 CharaTask::fn_8014052C() {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(mRot.y);
    PSMTXMultVec(m.m, &dir, &dir);
    return dir;
}

void atn::Matrix3x4::fn_80144ED4(float rad) {
    Mtx t;
    Mtx r;
    PSMTXRotRad(r, 'z', rad);
    PSMTXConcat(m, r, t);
    __MtxCopy(m, t);
}

// NONMATCHING: 99.4% - only the .sdata2 offset of 0.0f differs (pool order).
void CharaTask::fn_801446F0() {
    m574.x = 0.0f;
    m574.y = 0.0f;
    m574.z = 0.0f;
    m570 = 0.0f;
    m580 = 0.0f;
    m584 = 0.0f;
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

void atn::Matrix3x4::fn_80147A08(const Quaternion_unk& q) {
    PSMTXQuat(m, &q);
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

CharaTask::Item14_unk* CharaTask::fn_80148B0C(int index) {
    if (index >= 0 && index < (int)m624.mSize) {
        return &m624.mData[index];
    }
    return 0;
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

atn::Quaternion_unk& atn::Quaternion_unk::fn_8014989C(const Quaternion_unk& a) {
    PSQUATMultiply(&a, this, this);
    return *this;
}

atn::Quaternion_unk atn::Quaternion_unk::fn_801498D8() const {
    Quaternion_unk r(*this);
    PSQUATInverse(&r, &r);
    return r;
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

// NONMATCHING: the original loads mEnabled before saving the arguments (4 instructions moved).
void CharaListeners_unk::fn_8014F860(int id, CharaTask* chara, int arg) {
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

bool CharaTask::fn_8014FAD0() {
    bool result = false;
    for (unsigned int i = 0; i < mParts.mCount; i++) {
        if (mParts.mData[i] != 0 && mParts.mData[i]->vf0C()) {
            result = true;
        }
    }
    return result;
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

void CharaTask::fn_80150178(int slot, chr::CharaEquip_unk* equip) {
    if (m680[slot] != 0) {
        delete m680[slot];
    }
    m680[slot] = equip;
    if (slot == 0) {
        fn_8014FD30();
    }
}

void CharaTask::vf40(HitInfo_unk* hit) {}

extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* ab);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length (next to PSVECMag)
extern "C" int fn_8010CD3C(Effect_unk* effect, int kind, CharaTask* chara, void* attacker);

// inferred: name invented
inline float Dot_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}

// NONMATCHING: 98.4% - CharaStatus::fn_801346C8 is called with a second argument 0 (li r5,0) in
// every caller of the game, but it is declared with one (its symbol is fixed as Fi).
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

extern "C" int fn_80370174(Field_unk* field, int id);
extern "C" void fn_80370320(Field_unk* field, int id, int value, int arg);
extern "C" void fn_801059F8(Effect_unk* effect, CharaTask* chara);
extern "C" void fn_80105B3C(Effect_unk* effect, CharaTask* chara);

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
        fn_801059F8(lbl_8087F048, this);
        fn_80105B3C(lbl_8087F048, this);
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

extern "C" void fn_80107850(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107798(Effect_unk* effect, chr::Motion_unk* motion, int arg);
extern "C" void fn_80107A68(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107B20(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107E58(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107FC8(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80108080(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_801079B0(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107F10(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107BD8(Effect_unk* effect, chr::Motion_unk* motion);
extern "C" void fn_80107380(Effect_unk* effect, CharaTask* chara);
extern "C" void fn_80107D94(Effect_unk* effect, chr::Motion_unk* motion);

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
    CameraState_unk(int mode, const CameraState_unk& o)
        : m00(mode), m04(o.m04), m14(o.m14), m24(o.m24), m34(o.m34), m44(o.m44), m48(0), m4C(0.0f) {}
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
    unsigned char m000[0x324];
    CameraState_unk m324;
    unsigned char m374[0x3A4 - 0x374];
    float m3A4; // inferred: time of a frame
};
extern "C" GameState_unk* lbl_8087EFA8;

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
    if (lbl_8087F048 != 0) {
        fn_801079B0(lbl_8087F048, &mMotion);
    }
    if (mFlag20 && mType == 0) {
        mFlag20 = 0;
        GameState_unk* state = lbl_8087EFA8;
        state->m324 = CameraState_unk(mFlag19, state->m324);
    }
    if (lbl_8087F048 != 0) {
        fn_80107F10(lbl_8087F048, &mMotion);
    }
    if (m58C == 3) {
        m58C = 0;
        fn_8016E970(m564);
    }
    mStatus.fn_8013322C(0x40);
    mF94 = -1;
    if (lbl_8087F048 != 0) {
        fn_80107BD8(lbl_8087F048, &mMotion);
        fn_80107380(lbl_8087F048, this);
    }
    if (lbl_8087F048 != 0) {
        fn_80107D94(lbl_8087F048, &mMotion);
    }
}

// inferred: lbl_8087F9C0 (class unknown)
struct Global9C0_unk {
    unsigned char m00[0x28];
    int m28;
};
extern "C" Global9C0_unk* lbl_8087F9C0;
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
            if (lbl_8087F9C0->m28 == 0) {
                return mMotion.mTracks[0].m08 < 8.0f;
            }
            break;
        }
    } else if (m55C == 5) {
        return true;
    }
    return false;
}

extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);


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

extern "C" int fn_8004ED34(void* world, int arg, const atn::Vector3* from, const atn::Vector3* to, unsigned int mask,
                           int arg5, int arg6); // inferred: ray test

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
extern "C" void* lbl_8087F3C0; // inferred: effect manager (see gmk/PlayEffect.cpp)
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

// inferred: name invented
inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

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

void CharaTask::fn_801546F4() {
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

extern "C" int fn_800185B4(void* manager, CharaTask* chara, int arg);

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
// inferred: controls created by CharaTask; class names unknown (constructors 0x801B11BC,
// 0x801B151C and 0x801B0D88 are in chr/ControlDive.cpp, 0x801C041C in the next unit)
class Control801B11BC_unk : public ControlBase {
public:
    Control801B11BC_unk(Chara_unk* chara, int a, int b);
    virtual int vf0C();
    unsigned char m08[0x1C - 0x08];
};
class Control801B151C_unk : public ControlBase {
public:
    Control801B151C_unk(Chara_unk* chara);
    virtual int vf0C();
    unsigned char m08[0x18 - 0x08];
};
class Control801C041C_unk : public ControlBase {
public:
    Control801C041C_unk(Chara_unk* chara, int a, float b);
    virtual int vf0C();
    unsigned char m08[0x30 - 0x08];
};
class Control801B0D88_unk : public ControlBase {
public:
    Control801B0D88_unk(Chara_unk* chara, int a);
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};
} // namespace chr

// fn_80154EC4, fn_801551AC, vf74, fn_80155790: same instructions as the target; only the .data
// offsets of the ptmf constants differ until the earlier .data users exist.
void CharaTask::fn_80154EC4(int a, int b) {
    fn_80178208(new (0, "", "", 0) chr::Control801B11BC_unk(this, a, b));
}

void CharaTask::fn_801551AC() {
    fn_80178208(new (0, "", "", 0) chr::Control801B151C_unk(this));
}

// inferred: the parameter is read by the body, so the symbol vf74__9CharaTaskFv (in symbols.txt)
// has the wrong signature.
void CharaTask::vf74(int arg) {
    fn_80178208(new (0, "", "", 0) chr::Control801C041C_unk(this, arg, 1.0f));
}

void CharaTask::fn_80155790(int arg) {
    fn_80178208(new (0, "", "", 0) chr::Control801B0D88_unk(this, arg));
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

// inferred: 0x1C-byte record at 0x1470 in the player characters (names invented)
struct PlayerState_unk {
    PlayerState_unk(short id, int value)
        : m00(id), m02(0), m04(0), m0C(0.0f, 0.0f, 0.0f), m18(0) {
        m08 = value;
    }
    short m00;
    short m02;
    int m04;
    int m08;
    atn::Vector3 m0C;
    int m18;
};

// inferred: player character as seen from here (PlayerTask: BattleCharaTask: CharaTask)
struct Player_unk : public CharaTask {
    unsigned char m1424[0x1470 - 0x1424];
    PlayerState_unk m1470;
    unsigned char m148C[0x14AC - 0x148C];
    Player_unk* mNext; // 0x14AC: inferred: next player in the manager list
};

// inferred: player manager (lbl_8087F8A0, CharaManager<PlayerTask>)
struct PlayerList_unk {
    unsigned char m00[0x48];
    Player_unk* mHead;
};
extern "C" PlayerList_unk* lbl_8087F8A0;

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
        Player_unk* player = (lbl_8087F8A0 != 0) ? lbl_8087F8A0->mHead : 0;
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

// fn_8016E970, fn_801750FC and fn_80178208 are needed by the functions above (inlined through
// -ipa file); the functions between them are not written yet.
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

void CharaTask::fn_80178208(chr::ControlBase* control) {
    if (m55C == 6) {
        if (mControl) {
            mControl->vf1C();
        }
    }
    fn_8016E970(6);
    mControl.reset(control);
}

void CharaTask::fn_8017ABD4(int arg) {
    m68[1] = fn_80204E04(mType, arg);
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
    m13FC = value;
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

bool CharaTask::fn_8017B3C0() {
    bool result = false;
    if (mC58.mC0 == 0 || (mC58.m11C.fn_8011D1FC() && !mC58.m11C.fn_8011D21C())) {
        if (mFlagB14) {
            result = true;
        }
    }
    return result;
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
