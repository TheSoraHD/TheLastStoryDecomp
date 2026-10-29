#include "game/Em201ZanGurgFinal.h"
#include "game/Inventory.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/GameUI.h"
#include "game/HitInfo_unk.h"
#include "game/HomingBullet.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"
#include "game/Input_unk.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" double cos(double x);
extern "C" double sqrt(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: shield of the arena (fn_800457A4 makes a part, as in game/CharaTask.cpp; fn_80206C50 returns the
// weapon data of an item id, as in gmk/DropItem.cpp)
struct ZanWeapon_unk {
    unsigned char m00[0x78];
    int m78;
    unsigned char m7C[0x80 - 0x7C];
    int m80;
};
extern "C" ZanWeapon_unk* fn_80206C50(int id);
extern "C" chr::CharaPart_unk* fn_800457A4(int kind, int id, int arg, CharaTask* chara, int arg2);
// inferred: field (lbl_8087F430): camera shake at 0x96C and route tables at 0x10D8 (as in
// game/Em101ElderZesyaSecond.cpp)
struct Field_unk {
    unsigned char m000[0x96C];
    int m96C;
    int m970;
    float m974;
    float m978;
    unsigned char m97C[0x10D8 - 0x97C];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" Field_unk* fn_80121F00();
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
extern "C" void fn_80370320(Field_unk* field, int event, int arg2, int arg3); // inferred: starts an event
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, void* homing, int arg6, int arg7, float a, float b);
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)
extern "C" float fn_8010B250(Effect_unk* mgr, int kind, int arg); // inferred: scale of the bullet
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8; // inferred: effect layer (as in game/Arms.cpp)
};
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, void* efp, atn::Model* model, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, const Color4f_unk& color, int arg7, int arg8, int arg9,
                            float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
// inferred: arena UI (lbl_8087F490); fn_803E6850 shows a counter (as in game/Em003LesserDaemon.cpp)
extern "C" GameUI* lbl_8087F490;
extern "C" void fn_803E6850(GameUI* ui, CharaTask* chara, int count, int max, int arg);
extern "C" void fn_803E3384(GameUI* ui); // inferred: closes the button prompt
// inferred: game state (lbl_8087EFA8): time of a frame at 0x3A4 (as in game/CharaStatus.cpp)
struct ZanGame_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" ZanGame_unk* lbl_8087EFA8;
// inferred: system settings (lbl_8087F0A8): frame rate at 0x30 and pad state at 0x48C
struct ZanSystem_unk {
    unsigned char m000[0x30];
    int mFrameRate;
    unsigned char m034[0x48C - 0x34];
    unsigned char mPad[1];
};
extern "C" ZanSystem_unk* lbl_8087F0A8;
// inferred: collision world and its ray test (see game/Em101ElderZesyaSecond.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    atn::ColliBase* m38; // inferred: object hit
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" void fn_8004D388(void* mgr, CollResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                            unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
// inferred: skill effects (lbl_8087F9E8)
extern "C" void* lbl_8087F9E8;
extern "C" void fn_8059C32C(void* mgr, int arg);
// inferred: damage record (as QueenDamage_unk of game/Em208Queen.cpp)
struct ZanDamage_unk {
    ZanDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        m1C = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int m1C : 4;
    unsigned int m1C_rest : 28;
};
// inferred: hit parameters (as in game/CharaTask.cpp); names invented
struct HitParam_unk {
    int m00;
    float m04;
    float m08;
    int m0C;
    float m10;
    int m14;
};
extern "C" HitParam_unk lbl_807C6B90; // inferred: default parameters
extern "C" void fn_8003EA3C(ZanDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const HitParam_unk* param, int arg, int arg2);
extern "C" void fn_80108C10(Effect_unk* effect, ZanDamage_unk* damage, const atn::Vector3& pos, CharaTask* a,
                            CharaTask* chara, int b, int c);
// inferred: item manager (Inventory::instance_)
extern "C" int fn_80206BE4(void* item); // inferred: item id
extern "C" void fn_804438E0(void* mgr, int id);
extern "C" void fn_8044D104(void* mgr, int kind, int id, int arg, int chara);
extern "C" int fn_80219558(int id); // inferred: party member kind of a character id
extern "C" void fn_800FDB20(Effect_unk* mgr, CharaTask* owner, CharaTask* target, int arg, HitInfo_unk* data, int id,
                            const atn::Vector3* pos, const atn::Vector3* dir, float scale);
extern "C" void fn_80101434(Effect_unk* mgr, const atn::Vector3* pos, const atn::Vector3* rot, int a, int b);

// inferred: small inline helpers whose weak copies are emitted in this file (other files call them by these
// automatic names)
extern "C" inline void fn_802657FC(CharaTask* chara, int state) {
    chara->m58C = state;
}
extern "C" inline void fn_80265804(CharaStatus* status, float value) {
    status->m2FC = value;
}
// inferred: plays an effect on the model (name invented)
extern "C" inline void fn_8026607C(EffectMgr_unk* mgr, void* efp, atn::Model* model, int arg) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, arg, 1.0f);
}
extern "C" inline void fn_802660EC(CharaParam_unk* param, unsigned int flags) {
    param->m9C &= ~flags;
}
extern "C" inline void fn_802660FC(CharaParam_unk* param, unsigned int flags) {
    param->m9C |= flags;
}
// inferred: camera target of the field input (as PlayerInput_unk of game/EnemyJill.cpp); names invented
struct PlayerInput_unk {
    unsigned char m000[0x834];
    int m834;
    unsigned char m838[0x910 - 0x838];
    unsigned char m910;
    unsigned char m911;
    unsigned char m912[0x914 - 0x912];
    int m914;
    int m918;
    atn::Vector3 m91C;
    unsigned char m928[0x934 - 0x928];
    float m934;
};
extern "C" inline void fn_8026610C(PlayerInput_unk* input, const atn::Vector3& pos, float f, int keep) {
    input->m911 = input->m910;
    input->m910 = 1;
    input->m91C = pos;
    input->m934 = f;
    input->m914 = keep != 0 ? input->m918 : 0;
}
extern "C" inline atn::Matrix3x4* fn_80266154(atn::Model* model) {
    return (atn::Matrix3x4*)model->mMtx;
}
extern "C" inline atn::Model* fn_8026615C(chr::CharaPart_unk* part) {
    return (atn::Model*)part->m010;
}
extern "C" inline atn::Vector3* fn_80266164(PlayerInput_unk* input) {
    return (atn::Vector3*)&input->m910;
}
extern "C" inline void fn_8026616C(PlayerInput_unk* input, int value) {
    input->m834 = value;
}
extern "C" inline void fn_80266174(PlayerInput_unk* input, int value) {
    input->m910 = 0;
    if (value != 0) {
        input->m914 = 0;
    }
}
extern "C" PlayerInput_unk* fn_80122550(Field_unk* field);
extern "C" CharaManager<PlayerTask>* fn_8000D9E8();
extern "C" CharaTask* fn_8000DCF4(CharaManager<PlayerTask>* mgr);
extern "C" EffectMgr_unk* fn_800F7FA0(); // inferred: returns lbl_8087F3C0
extern "C" void fn_800F7FA8(EffectMgr_unk* mgr, void* owner, int arg);
extern "C" void fn_800F84D0(EffectMgr_unk* mgr, int layer); // inferred: mB8 = layer
// inferred: CharaTask::m6B8 = v (weak copy of chr/ControlStickByQuark.cpp)
extern "C" void fn_80198514(CharaTask* chara, const atn::Vector3& v);
extern "C" float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b); // inferred: dot product
// inferred: weak copies kept in this file (accessors of CharaTask and of the shield)
extern "C" inline FixedArray_unk<chr::CharaPart_unk*, 8>* fn_80266FA4(CharaTask* chara) {
    return &chara->mParts;
}
extern "C" inline void* fn_80266FAC(chr::CharaPart_unk* part) {
    return part->m274;
}
extern "C" inline chr::CharaPart_unk** fn_80266FB4(FixedArray_unk<chr::CharaPart_unk*, 8>* parts, int i) {
    return &parts->mData[i];
}

// inferred: the nodes of the shield (op_09_*) and of the broken shield (op_03_*, op_04_*, op_05_*)
static const char* const sChainNodes_unk[] = {"op_09_chain1", "op_09_chain2", "op_09_spike", "op_09_sword1"};
static const char* const sShieldNodes_unk[] = {"op_03_shield1", "op_03_shield2", "op_04_shield_dam_a"};
static const char* const sDamageNodes_unk[] = {"op_05_shield_dam_b1", "op_05_shield_dam_b2", "op_05_shield_dam_b3",
                                               "op_05_shield_dam_b4", "op_05_shield_dam_b5"};

// inferred: names invented
inline void PlayEffect_unk(atn::EfpFile* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}
inline void PlayMotion_unk(atn::Model* model, int motion, int loop, float speed) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->mTracks[0].m0C = 1.0f;
    model->fn_80097C08(0, motion, loop, 0, 1, 0.0f, speed);
}
inline void PlayMotionB_unk(atn::Model* model, int motion, float blend) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->mTracks[0].m0C = 1.0f;
    model->fn_80097C08(0, motion, 0, 0, 1, blend, 0.1f);
}
// inferred: direction in front of the character (z axis turned by the rotation; name invented)
inline atn::Vector3 Front_unk(float roty) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', roty);
    PSMTXMultVec(m, &v, &v);
    return v;
}
// inferred: same, reading the rotation after the vector is set (name invented)
inline atn::Vector3 FrontR_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rot.y);
    PSMTXMultVec(m, &v, &v);
    return v;
}
// inferred: RouteTable_unk::FindPlace_unk with the array read inside the loop (as in game/Em208Queen.cpp)
inline RoutePlace_unk* FindRoutePlace2_unk(RouteTable_unk* table, int id) {
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        RoutePlace_unk* places = table->mPlaces;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}
inline RoutePlace_unk* FindRoutePlace3_unk(RouteTable_unk* table, int id) {
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        if (table->mPlaces[i].mId == id) {
            return &table->mPlaces[i];
        }
    }
    return 0;
}
inline RoutePlace_unk* FindRoutePlace4_unk(int id) {
    RouteTable_unk* table = lbl_8087F430->m10D8;
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        RoutePlace_unk* places = table->mPlaces;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}
inline bool IsMotionEnd_unk(atn::Model* model) {
    float frame = model->mTracks[0].m08;
    return frame >= model->fn_80097D7C(0);
}

// NONMATCHING: registers of the std::string temporaries (as Em001Necromancer::Em001Necromancer)
Em201ZanGurgFinal::Em201ZanGurgFinal(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(1.0f), m14CC(0), m14D0(0),
      m14EC(0), m14F0(0), m14F4(0.0f, 0.0f, 0.0f), m1500(45), m1504(0), m1508(0), m1510(0.0f, 0.0f, 0.0f), m152C(6),
      m1530(0.0f, 0.0f, 0.0f), m153C(0.0f), m1540(0.0f), m1544(0), m1558(0.0f, 0.0f, 0.0f), m162C(0), m1630(0),
      m1634(0), m1638(0), m163C(0), m1640(0), m1644(0), m1648(1), m164C(0), m1650(0), m1654(0), m1688(0),
      m1690(0) {
    memset(m14D4, 0, sizeof(m14D4));
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_zangurg_final.csv");
    atn::Csv csv(info->m2C, strlen(info->m2C));
    do {
        const char* s = csv.fn_8005B9CC();
        if (s == 0 || strcmp(s, "") == 0) {
            break;
        }
        if (strncmp(s, "db_name=", 8) == 0) {
            name = s + 8;
        }
    } while (csv.fn_8005BBF8());
    std::string path = fn_8006AFF8(dir, name);
    m14B0.vf0C(path.c_str());
    void* group = m168C.Load("Em201ZanGurgFinal");
    fn_80087994(group, "DebugDisp", &m1690, 0, 0);
    m1564.LoadRequest("/effect/ef_bs007a.efp");
    m1570.LoadRequest("/effect/ef_bs065a.efp");
    m157C.LoadRequest("/effect/ef_bs065c.efp");
    m1588.LoadRequest("/effect/ef_bu031a.efp");
    m1594.LoadRequest("/effect/ef_bs064a.efp");
    m15A0.LoadRequest("/effect/ef_bu032a.efp");
    m15AC.LoadRequest("/effect/ef_bu033a.efp");
    m15B8.LoadRequest("/effect/ef_bs063a.efp");
    m15C4.LoadRequest("/effect/ef_bu040a.efp");
    m15D0.LoadRequest("/effect/ef_bu040d.efp");
    m15DC.LoadRequest("/effect/ef_bu040e.efp");
    m15E8.LoadRequest("/effect/ef_bu040f.efp");
    m15F4.LoadRequest("/effect/ef_bu040h.efp");
    m1600.LoadRequest("/effect/ef_bu040g_01.eff");
    m160C.LoadRequest("ef_we006.efp");
    m1618.LoadRequest("ef_bs073f.efp");
    mFlagB11 = 1;
    ZanWeapon_unk* shield = fn_80206C50(0x18875);
    m1628 = m1624 = fn_800457A4(shield->m78, shield->m80, 0, this, 0);
}

Em201ZanGurgFinal::~Em201ZanGurgFinal() {
    delete m1624;
}

int Em201ZanGurgFinal::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1624->vf0C() == 0 &&
        m1564.IsLoading() == 0 && m1570.IsLoading() == 0 && m157C.IsLoading() == 0 &&
        m1588.IsLoading() == 0 && m1594.IsLoading() == 0 && m15A0.IsLoading() == 0 &&
        m15AC.IsLoading() == 0 && m15B8.IsLoading() == 0 && m15C4.IsLoading() == 0 &&
        m15D0.IsLoading() == 0 && m15DC.IsLoading() == 0 && m15E8.IsLoading() == 0 &&
        m15F4.IsLoading() == 0 && m1600.IsLoading() == 0 && m160C.IsLoading() == 0 &&
        m1618.IsLoading() == 0 && m14B0.fn_80473F50() == 0) {
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        m58C = 0;
        fn_8016E970(2);
        fn_8026580C();
        fn_80265804(&mStatus, 1.0f);
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        m165C = 10000;
        m1658 = 10000;
        if (m14C0 == 1) {
            m14C8 = 2.0f;
            mStatus.m228 = 0.5f;
            mStatus.m2D0[3] = CharaStatusPair_unk(0x616);
        } else if (m14C0 == 0) {
            m1634 = 1;
        }
        m54C &= ~0x1000000;
        return 1;
    }
    return 0;
}

void Em201ZanGurgFinal::fn_8026580C() {
    if (lbl_8087F430 != 0) {
        RoutePlace_unk* place = FindRoutePlace4_unk(0x262);
        if (place != 0) {
            m14F4 = place->mPos;
        }
    }
    atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    do {
        const char* key = csv.fn_8005B9CC();
        if (*key == ';' || *key == '\0') {
            continue;
        }
        if (strcmp(key, "CenterPoint") == 0) {
            if (lbl_8087F430 != 0) {
                RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(atoi(csv.fn_8005B9CC()));
                if (place != 0) {
                    m14F4 = place->mPos;
                }
            }
        } else if (strcmp(key, "Sequence") == 0) {
            m14C0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AtkInterval") == 0) {
            m1500 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "ComboOK") == 0) {
            m14D4[0] = atoi(csv.fn_8005B9CC());
            m14D4[1] = atoi(csv.fn_8005B9CC());
            m14D4[2] = atoi(csv.fn_8005B9CC());
            m14D4[3] = atoi(csv.fn_8005B9CC());
            m14D4[4] = atoi(csv.fn_8005B9CC());
            m14D4[5] = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "ShieldHP") == 0) {
            m1658 = m165C = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
}

// NONMATCHING: inline budget (as Em001Necromancer::vf0C): the original calls the small helpers
// (fn_8026607C, fn_80265804, fn_802660EC/FC, fn_8026610C-fn_80266174, GetTrans_unk) out of line; with an
// extra inline candidate of about 150 statements the body matches but for names
void Em201ZanGurgFinal::vf10() {
    fn_80266FC4();
    m14BC++;
    if (m14EC > 0) {
        mStatus.m228 = 0.0f;
        if (--m14EC == 0) {
            fn_80239DAC(fn_800F7FA0(), &m15AC, 0, 1);
            fn_800F7FA8(fn_800F7FA0(), &m15A0, 0);
            fn_800F84D0(fn_800F7FA0(), 2);
            fn_8026607C(fn_800F7FA0(), &m15A0, &mMotion, 1);
            fn_800F84D0(fn_800F7FA0(), 0);
        }
    }
    if (m14F0 > 0 && --m14F0 == 0) {
        if (m58C == 6) {
            fn_80239DAC(fn_800F7FA0(), this, 1000, 0);
        } else {
            m14F0 = 1;
        }
    }
    if (m14C0 == 1) {
        if (fn_80370A78(fn_80121F00(), 0xE9) == 2) {
            m1634 = 1;
            for (int i = 0; i < 3; i++) {
                mMotion.fn_8009373C(sShieldNodes_unk[i], 0);
            }
            for (int i = 0; i < 5; i++) {
                mMotion.fn_8009373C(sDamageNodes_unk[i], 1);
            }
            fn_8026AF78();
            fn_80370AE4(fn_80121F00(), 0xE9, 3);
        }
        if (m1634 != 0) {
            fn_80265804(&mStatus, 1.0f);
        } else {
            fn_80265804(&mStatus, 0.0f);
        }
    }
    fn_80370320(fn_80121F00(), 0x396, 0, 0);
    if (m1630 != 0) {
        fn_8026DB8C();
    }
    if (m1634 != 0 && m1630 == 0) {
        fn_8014C0B4(25, 0x1C2);
        fn_8014C0B4(26, 0x1C3);
    } else {
        fn_8014C0B4(25, 0x1C0);
        fn_8014C0B4(26, 0x1C1);
    }
    fn_802660EC(&mStatus.mParamB, 4);
    if (mC58.mC0 == 0 || m14B8 == 0) {
        if (m14B8 == 0) {
            if (m58C == 6) {
                fn_80267F88();
            } else {
                fn_8026AF78();
            }
        } else {
            fn_8016E970(3);
            fn_802657FC(this, 0);
        }
    } else {
        if (m1650 == 0 && m14C0 == 1 && --m1654 < 0) {
            fn_80239DAC(fn_800F7FA0(), this, 200, 1);
            fn_800F7FA8(fn_800F7FA0(), this, 200);
            fn_8026607C(fn_800F7FA0(), &m15C4, &mMotion, 1);
            m1650 = 1;
        }
        if (m55C == 2) {
            fn_8016E970(3);
        }
        switch (m58C) {
        case 8:
            fn_802684C0();
            break;
        case 9:
            fn_8026860C();
            break;
        case 10:
            fn_80268CB4();
            break;
        case 11:
            fn_80269168();
            break;
        case 12:
            fn_802692B4();
            break;
        case 13:
            fn_802696F8();
            break;
        case 14:
            fn_80269CBC();
            break;
        case 15:
            fn_802660FC(&mStatus.mParamB, 4);
            fn_8026A010();
            break;
        case 17:
            fn_8026A4FC();
            break;
        case 18:
            fn_8026A5BC();
            break;
        case 19:
            fn_8026A628();
            break;
        case 16:
            fn_8026A34C();
            break;
        case 7:
            fn_802682C4();
            if (m55C != 6) {
                fn_802676BC();
            }
            break;
        case 20:
            fn_8026A774();
            break;
        case 21:
            fn_8026A7AC();
            break;
        case 22:
            fn_8026A828();
            break;
        case 23:
            fn_8026A8E4();
            break;
        case 24:
            fn_8026A9A0();
            break;
        case 25:
            fn_8026AC8C();
            break;
        default:
            fn_80267F88();
            if (m55C != 6) {
                fn_802676BC();
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    PlayerInput_unk* input = fn_80122550(fn_80121F00());
    fn_8000DCF4(fn_8000D9E8());
    if (m163C == 0 && m1630 != 0 && m1644 != 0 && m1660 == 1 && m1668 < 0.7f) {
        if (m1640 == 0) {
            atn::Vector3 pos = fn_80266154(fn_8026615C(m1624))->GetTrans_unk();
            fn_8026610C(input, pos, 0.0f, 0);
            m1640 = 1;
        } else {
            atn::Vector3 pos = fn_80266154(fn_8026615C(m1624))->GetTrans_unk();
            *(atn::Vector3*)((char*)fn_80266164(input) + 0xC) = pos;
        }
    } else if (m1640 != 0) {
        fn_8026616C(input, 0);
        fn_80266174(input, 0);
        m1640 = 0;
    }
}

void Em201ZanGurgFinal::vf14() {
    BattleCharaTask::vf14();
    if (m1630 != 0 && m1660 != 3) {
        m1624->vf14();
    }
    if (m1634 == 0) {
        atn::Vector3 d = lbl_8087F8A0->mHead->mPos - mPos;
        if (PSVECSquareMag(&d) < 1600.0f) {
            fn_803E6850(lbl_8087F490, this, m1658, m165C, 0);
        }
    }
}

// inferred: starts state `state` (names invented)
inline void Em201ZanGurgFinal::SetState_unk(int state, int control) {
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m14C4 = 0;
    mFlag5 = 0;
    m58C = state;
    fn_8016E970(control);
}
inline void Em201ZanGurgFinal::ToIdle_unk() {
    SetState_unk(6, 3);
    fn_80239DAC(lbl_8087F3C0, this, 100, 0);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
}

// NONMATCHING: r6/r7 of the data of the hit and of the sword flag (rest matches; 6 variants)
void Em201ZanGurgFinal::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m1688++;
    if (m58C == 16) {
        msg->m10 *= 0.0f;
        msg->m64 = 3;
        msg->m90 = 0;
        msg->m84 = 2;
        msg->m88 = msg->mData->mC4;
        msg->m68 = 0;
        return;
    }
    if (msg->m44 == 1 || msg->mData->m01 == 1) {
        msg->m10 *= 0.0f;
    }
    if (m58C == 12) {
        msg->m10 *= 0.0f;
    }
    if ((msg->mData->mAC & 0x8000) == 0x8000) {
        if ((m54C & 0x1000000) != 0x1000000) {
            ToIdle_unk();
            fn_8016D0A0(0);
            m14BC = 0;
            if (m14C0 == 1 && m1630 == 0) {
                fn_8026D0F8(1);
            }
        }
        msg->m90 = 0;
        return;
    }
    if (m14C0 == 1) {
        if (m1634 != 0) {
            msg->m10 *= 0.01f;
            fn_80151448(msg);
            vf40(hit);
            return;
        }
        if (msg->mAttacker != 0 && msg->mAttacker->m648 != 0) {
            bool sword = false;
            if ((unsigned int)(*(int*)((char*)msg->mAttacker->m648 + 8) - 0x1D5) <= 5) {
                sword = true;
            }
            if (sword) {
                msg->m50 |= 8;
                msg->m0C |= 0x40000000;
                atn::Vector3 rot = msg->m28.ToRotation_unk();
                fn_80232B7C(0, 0);
                PlayEffect_unk(&m15F4, &mMotion);
                if (fn_80370A78(lbl_8087F430, 0xEC) == 0) {
                    fn_80370AE4(lbl_8087F430, 0xEC, 1);
                }
                if (m58C - 17U > 2 && m58C != 7 && m58C != 9) {
                    msg->m10 *= 0.0f;
                }
                goto hit_unk;
            }
        }
        if (msg->mData->m04 == 0xD0 && m1630 == 0) {
            SetState_unk(22, 3);
            PlayMotion_unk(&mMotion, 0x33, 0, 0.1f);
            fn_8026D0F8(1);
            m1644 = 1;
            fn_80239DAC(lbl_8087F3C0, this, 100, 0);
            fn_80239DAC(lbl_8087F3C0, this, 101, 1);
        }
        msg->m48 = 1;
        msg->m0C |= 0x8008;
        if (fn_80370A78(lbl_8087F430, 0xEB) == 0) {
            fn_80370AE4(lbl_8087F430, 0xEB, 1);
        }
        if (m58C - 17U <= 2 || m58C == 7) {
            goto hit_unk;
        }
        msg->m64 = 1;
        msg->m90 = 1;
        msg->m84 = 1;
        msg->m88 = msg->mData->mC4;
        msg->m68 = 0;
        msg->m94 = msg->m0C;
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m1588, &mMotion);
        mStatus.fn_8012F034(0.01f);
        fn_800C344C("SE_EM201_008", &msg->m1C, 0, -1, 1.0f);
        m6D0.PushBack_unk(CharaPair8_unk(msg->m34, msg->m38));
        return;
    hit_unk:
        fn_80151448(msg);
        vf40(hit);
    } else if (m1544 == 0 || msg->m44 == 0) {
        fn_80151448(msg);
        vf40(hit);
    }
}

// NONMATCHING: stack slots of the vector temporaries and the order of two loads (inlining matches)
void Em201ZanGurgFinal::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m14C0 == 1) {
        if (m1634 == 0) {
            m1658 -= msg->m68;
            mStatus.m04 = (float)mStatus.mParamB.m84;
            if (mStatus.fn_80139F4C() != 0) {
                mStatus.fn_8013322C(0x20);
            }
            if (m1658 <= 0) {
                fn_80370AE4(fn_80121F00(), 0xE9, 1);
                if (m6B8.LengthSq_unk() > 0.0f) {
                    fn_8015E4B0(m6B8, -1);
                } else {
                    atn::Vector3 front = fn_8014052C();
                    fn_8015E4B0(-front, -1);
                }
                fn_802657FC(this, 0);
                fn_8026AF24();
                m1658 = 0;
            }
            if (m1658 < m165C / 2 && fn_80370A78(fn_80121F00(), 0xEF) == 0) {
                fn_80370AE4(fn_80121F00(), 0xEF, 1);
            }
        } else if (msg->mData->m04 == 0xD0 && m1630 == 0) {
            fn_8026CB50();
        }
    }
    if (m55C == 6) {
        if (m560 - 22U <= 1) {
            fn_800F7FA8(fn_800F7FA0(), this, 101);
            if (!(msg->m0C & 0x40000000) && m1658 > 0) {
                atn::Vector3 dir(msg->m10);
                m1504++;
                if (m1658 > 0 && msg->mAttacker != 0 && msg->m44 == 0 && m1504 >= 4) {
                    msg->mAttacker->vf7C(1);
                    fn_8026C4E8(msg->mAttacker);
                    m1504 = 0;
                } else {
                    fn_8015D8C0(0.1f, dir, (int)msg->mAttacker);
                    fn_80198514(this, dir * 0.9f);
                    fn_802657FC(this, 0);
                    fn_8026AF24();
                }
            } else if (m560 == 23) {
                fn_8015E4B0(m6B8, -1);
                fn_802657FC(this, 0);
                fn_8026AF24();
            } else {
                fn_802657FC(this, 0);
                fn_8026AF24();
            }
            fn_80239DAC(fn_800F7FA0(), this, 100, 0);
            fn_80239DAC(fn_800F7FA0(), this, 101, 1);
        } else if (m560 == 20) {
            atn::Vector3 rot = (-msg->m28).ToRotation_unk();
            mRot.y = rot.y;
            fn_800F7FA8(fn_800F7FA0(), 0, 0);
            fn_8026607C(fn_800F7FA0(), &m1588, &mMotion, 1);
            if (m58C != 7) {
                fn_802657FC(this, 7);
                fn_8026AF24();
                mFlag5 = 1;
                m164C = 10;
                fn_80239DAC(fn_800F7FA0(), this, 100, 0);
                fn_80239DAC(fn_800F7FA0(), this, 101, 1);
            } else if (msg->mAttacker != 0 && ++m1504 >= 2) {
                if (msg->m44 == 0) {
                    msg->mAttacker->vf7C(1);
                    fn_8026C4E8(msg->mAttacker);
                } else {
                    atn::Vector3 d = *msg->mAttacker->fn_8013C38C() - mPos;
                    if (d.Length_unk() < 35.0f) {
                        fn_8026B748();
                    } else {
                        fn_8026B118();
                    }
                }
                m1504 = 0;
            }
            m14BC = 0;
        }
    }
    if (mStatus.fn_80139F4C() != 0) {
        fn_800F7FA8(fn_800F7FA0(), this, 101);
        mStatus.fn_8013322C(0x20);
        mStatus.m04 = 1.0f;
        if (m14C0 == 0) {
            m1544 = 1;
            fn_80370320(fn_80121F00(), 0x395, 1, 0);
        } else {
            fn_800F7FA8(fn_800F7FA0(), this, 101);
            fn_80370320(fn_80121F00(), 0x395, 0, 0);
            fn_80370320(fn_80121F00(), 0x396, 0, 0);
            if (fn_80370A78(fn_80121F00(), 0xEA) == 0) {
                fn_80370AE4(fn_80121F00(), 0xEA, 1);
                if (fn_8000DCF4(fn_8000D9E8()) != 0 && *fn_80266FB4(fn_80266FA4(this), 0) == m1628 &&
                    m1624 != 0) {
                    Inventory::Order()->AddItem(fn_80206BE4(fn_80266FAC(m1624)), 1, 1, 0, 0, 1, 0);
                }
            }
        }
        m163C = 1;
    }
}

// inferred: weak copies of small helpers of this file (called out of line by fn_802676BC; other files call
// them by these names)
extern "C" inline int fn_80267B20(CharaTask* chara) {
    return chara->m55C;
}
extern "C" inline int fn_80267B28(CharaTask* chara) {
    return chara->m560;
}
// inferred: the status has one of the flags 0x80000 or 0x1000000 (name invented)
extern "C" inline int fn_80267B30(CharaStatus* status) {
    return status->IsState_unk(0x80000) || status->IsState_unk(0x1000000);
}

// inferred: a party member that can be targeted (names invented)
inline bool IsTarget_unk(CharaTask* c) {
    bool free;
    bool r;
    bool active;
    r = false;
    active = false;
    free = false;
    if ((c->mFlags & 4) != 4 && (c->mFlags & 1) != 1) {
        free = true;
    }
    if (free && (c->mStatus.m0C & 0x20) != 0x20) {
        active = true;
    }
    if (active) {
        bool grappled = false;
        if (c->m55C == 6 && c->m560 == 28) {
            grappled = true;
        }
        if (!grappled) {
            r = true;
        }
    }
    return r;
}

void Em201ZanGurgFinal::fn_80266FC4() {
    if (m14C0 == 1) {
        mC58.mC8 = (int)mC58.mC4;
        CharaTask* leader = lbl_8087F8A0->mHead;
        CharaTask* yurick = ((PlayerManager*)lbl_8087F8A0)->fn_8054A33C((const char*)3);
        if (leader->mFlag28 || (m1630 != 0 && m1660 == 3)) {
            m1454 = 0;
            m14B8 = leader;
        } else if (m58C == 24 && m1648 != 0 && yurick != 0 && IsTarget_unk(yurick) &&
                   (yurick->m54C & 0x2000) != 0x2000) {
            m1454 = 0;
            m14B8 = yurick;
        } else {
            m1454 = 1;
            m14B8 = mC58.mC4;
        }
        mC58.mC4 = m14B8;
    } else {
        m14B8 = mC58.mC4;
    }
}

// NONMATCHING: r5/r6 of the target and of the difference vector in the direct chase (rest matches)
void Em201ZanGurgFinal::fn_80267134() {
    float speed = 0.0f;
    float scale = 1.0f;
    atn::Vector3 rot = mRot;
    if (m55C == 7) {
        atn::Vector3 dir = lbl_807C7030;
        if (m14C0 == 1) {
            mRoute.fn_80126214();
            dir = mRoute.m58;
            speed = PSVECMag(&dir);
            if (speed > 0.0001f) {
                rot = dir.GetNormalize().ToRotation_unk();
            } else {
                rot = mRot;
            }
        } else {
            atn::Vector3 to = (m14B8->mPos - mPos).GetNormalize();
            if ((m14B8->mPos - mPos).Length_unk() < 25.0f) {
                speed = 0.0f;
            } else {
                speed = to.Length_unk();
            }
            rot = to.ToRotation_unk();
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8013CB68(rot, 0, speed, m568 * scale);
}

void Em201ZanGurgFinal::fn_802676BC() {
    if (m14B8 == 0) {
        return;
    }
    atn::Vector3 target(*m14B8->fn_8013C38C());
    atn::Vector3 d = target - mPos;
    float dist = d.Length_unk();
    if (m14C0 == 0) {
        if (fn_80267B20(m14B8) == 6 && fn_80267B28(m14B8) == 0x85) {
            fn_8026C3E0(5, 14);
            return;
        }
        if (m1544 != 0) {
            fn_8026C7A4();
            return;
        }
    } else {
        if (m1544 != 0) {
            fn_8026C7A4();
            return;
        }
        if (mStatus.m228 >= 1.0f) {
            mStatus.m228 = 0.0f;
            fn_8026C3E0(2, 16);
            return;
        }
    }
    if (m14BC <= m1500) {
        return;
    }
    if (m14C0 == 1) {
        int max = m1630 != 0 ? 6 : 4;
        if (m1630 != 0 && m1660 == 2) {
            fn_8026C3E0(6, 23);
        } else if (m14CC >= max) {
            if (m1630 == 0) {
                fn_8026C3E0(2, 24);
            } else if (rand() % 100 < 50 && m1688 >= 5) {
                fn_8026C3E0(0, 12);
            } else {
                fn_8026C3E0(2, 9);
            }
            m14CC = 0;
        }
    }
    if (dist <= 35.0f) {
        bool front = fn_801A03E8(d, fn_8014052C()) >= 0.0f;
        if (front) {
            switch (rand() % 3) {
            case 0:
                if (m1630 == 0) {
                    fn_8026B008();
                } else {
                    fn_8026D058();
                }
                break;
            case 1:
                fn_8026B748();
                break;
            case 2:
                if (m1630 == 0) {
                    fn_8026B008();
                } else if (fn_80267B30(m14B8->GetStatus_unk()) == 0 && m1688 >= 5) {
                    fn_8026B9F0();
                } else {
                    fn_8026D058();
                }
                break;
            }
        } else {
            fn_8026B748();
        }
        m14CC++;
        return;
    }
    if (m14C0 == 1 && m14BC <= m1500 * 3) {
        return;
    }
    switch (rand() % 5) {
    case 0:
    case 1:
        if (m1630 == 0) {
            fn_8026C3E0(0, 10);
        } else {
            fn_8026D058();
        }
        break;
    case 2:
        if (m1630 == 0) {
            fn_8026C3E0(0, 13);
        } else {
            fn_8026B118();
        }
        break;
    case 3:
        if (m1630 == 0) {
            fn_8026C3E0(4, 15);
        } else {
            fn_8026C3E0(0, 11);
        }
        break;
    default:
        if (m1630 == 0 && m14C0 == 1) {
            fn_8026CDB4();
        } else if (fn_80267B30(m14B8->GetStatus_unk()) == 0 && m1688 >= 5) {
            fn_8026C3E0(0, 12);
        } else {
            fn_8026B118();
        }
        break;
    }
    m14CC++;
}

int Em201ZanGurgFinal::vf68(CharaTask* attacker) {
    if (m14C0 == 0 && m58C == 20) {
        return 8;
    }
    return 0;
}

void Em201ZanGurgFinal::vf6C(int kind, CharaTask* other) {
    if (kind == 8) {
        atn::Vector3 d = mPos - other->mPos;
        d.y = 0.0f;
        d.Normalize_unk();
        fn_8026C844(d);
    }
}

void Em201ZanGurgFinal::vf5C(CharaTask* other) {
    bool combo = false;
    if (mFlagB31) {
        combo = true;
    } else if (other != 0 && other->m55C == 6 && other->m560 == 9) {
        combo = true;
    } else if (other != mFDC) {
        if ((other != 0 && other->mType == 0) || (mFDC != 0 && mFDC->mType == 0)) {
            combo = true;
        }
    }
    mFDC = other;
    mFE0 = 300;
    if (combo) {
        mFE4++;
    } else {
        mFE4 = 1;
    }
}

void Em201ZanGurgFinal::vf98() {
    bool show = false;
    if (m121C && !mFlag18) {
        show = true;
    }
    if (show) {
        fn_80232B7C(m121C.get(), 0);
        lbl_8087F3C0->mB8 = 2;
        PlayEffect_unk((atn::EfpFile*)m121C.get(), &mMotion);
        lbl_8087F3C0->mB8 = 0;
        fn_80232B7C(&m15A0, 0);
        lbl_8087F3C0->mB8 = 2;
        PlayEffect_unk(&m15A0, &mMotion);
        lbl_8087F3C0->mB8 = 0;
        mFlag18 = 1;
    }
}

void Em201ZanGurgFinal::vf9C(int arg) {
    if (m121C && mFlag18) {
        fn_80239DAC(lbl_8087F3C0, m121C.get(), 0, arg);
        fn_80239DAC(lbl_8087F3C0, &m15A0, 0, arg);
        fn_80239DAC(lbl_8087F3C0, &m15AC, 0, arg);
        mFlag18 = 0;
    }
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
}

// inferred: yaw towards a direction on the ground (as the inlined ToRotation_unk; name invented)
inline float YawTo_unk(const atn::Vector3& d) {
    atn::Vector3 rot = d.ToRotation_unk();
    return rot.y;
}

void Em201ZanGurgFinal::fn_80267F88() {
    if (m55C - 6U > 1 && m14B8 != 0) {
        fn_8016E970(3);
        fn_80170A20(m14B8, 0, -1.0f);
    }
    fn_80267134();
    if (m55C != 6 && m14B8 != 0) {
        atn::Vector3 pos = m14B8->mPos;
        atn::Vector3 d = pos - mPos;
        if (PSVECMag(&d) < 30.0f) {
            atn::Vector3 rot = d.ToRotation_unk();
            mRot.y = rot.y;
            SetState_unk(7, 9);
            PlayMotion_unk(&mMotion, m484[25], 1, 0.1f);
            mFlag5 = 1;
            m164C = 300;
        }
    }
}

// NONMATCHING: order of the loads of the motion ids (lwz 0x4EC before 0x2DC; 5 variants)
void Em201ZanGurgFinal::fn_802682C4() {
    fn_8016E970(9);
    fn_80267134();
    if (mMotion.mTracks[0].m00 == m484[26]) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            PlayMotion_unk(&mMotion, m484[25], 1, 0.1f);
        }
    } else if (m14C0 == 0) {
        if (m55C != 6) {
            atn::Vector3 pos = m14B8->mPos;
            atn::Vector3 d = pos - mPos;
            if (PSVECMag(&d) > 35.0f) {
                ToIdle_unk();
            }
        }
    } else if (--m164C <= 0) {
        ToIdle_unk();
    }
}

// inferred: hit of an attack between two frames (names invented)
inline void Em201ZanGurgFinal_Hit_unk(Em201ZanGurgFinal* z, int frameEffect, int skill, const char* node) {
    if (z->m14EC > 0 && z->m14BC == frameEffect) {
        z->fn_8026F9BC(node);
    }
    HitInfo_unk* data = fn_80219E6C(skill);
    z->fn_80144710();
    fn_800F8C6C(lbl_8087F048, 0, 0, z, data, z->m590, 30, -1, 0.0f);
}

// inferred: release assert of the original (empty; without them MWCC inlines the small state functions into
// vf10, which the original calls out of line; as in game/Em101LittleZesya.cpp)
#define ASSERT_UNK(x) ((void)0)

void Em201ZanGurgFinal::fn_802684C0() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    } else if (mMotion.mTracks[0].m08 >= 61.0f && mMotion.mTracks[0].m08 <= 66.0f) {
        if (m14EC > 0 && m14BC == 43) {
            fn_8026F9BC("rightwepoint");
        }
        HitInfo_unk* data = fn_80219E6C(0x60E);
        fn_80144710();
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 30, -1, 0.0f);
    }
}

extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
// inferred: homing parameters with their defaults (as ZesyaHoming_unk of game/Em101LittleZesya.cpp)
struct ZanHoming_unk : public HomingParam_unk {
    ZanHoming_unk() {
        mTarget = 0;
        mAccel = 0.0f;
        m08 = -1.0f;
        mMaxSpeed = 10.0f;
        mTurn = 0.09424778f;
        mTime = 300.0f;
        mDelay = 5.0f;
        mNode = 0;
        mCount = -1;
    }
};

// inferred: fires bullet `skill` (name invented)
inline void Shoot_unk(CharaTask* owner, int skill, const atn::Vector3* pos, const atn::Vector3* dir, HomingParam_unk* param) {
    Effect_unk* mgr = lbl_8087F048;
    fn_800F8574(mgr, owner, fn_80219E6C(skill), pos, dir, param, 0, 0, 0.0f, 1.0f);
}

void Em201ZanGurgFinal::fn_8026860C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    } else if (mMotion.mTracks[0].m08 >= 5.0f && mMotion.mTracks[0].m08 <= 20.0f) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftwepoint"));
        if (m14B8 != 0) {
            atn::Vector3 hips = GetNodePos_unk(m14B8->mMotion.GetNodeMtx_unk("hips"));
            m1510 = (hips - pos).GetNormalize();
            mRot.y = m1510.ToRotationZ_unk().y;
        }
    } else if (atn::Abs_unk(mMotion.mTracks[0].m08 - 50.0f) < 1.1920929e-7f) {
        m12CC = 1.0f;
        atn::Vector3 pos1 = GetNodePos_unk(mMotion.GetNodeMtx_unk("ef_locater01"));
        atn::Vector3 pos2 = GetNodePos_unk(mMotion.GetNodeMtx_unk("ef_locater02"));
        if (m14C0 == 1) {
            ZanHoming_unk param;
            param.mTarget = m14B8;
            param.mTurn = 0.94247788f;
            param.mNode = fn_800DC6B4("hips");
            Shoot_unk(this, 0x617, &pos1, &m1510, &param);
            CharaTask* target = 0;
            for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = (PlayerTask*)p->mNext) {
                if (IsTarget_unk(p) && p->m5C->m11C - 3U <= 1) {
                    target = p;
                    break;
                }
            }
            param.mTarget = target != 0 ? target : m14B8;
            param.mTurn = 0.94247788f;
            param.mNode = fn_800DC6B4("hips");
            Shoot_unk(this, 0x617, &pos2, &m1510, &param);
        } else {
            Shoot_unk(this, 0x60F, &pos1, &m1510, 0);
            Shoot_unk(this, 0x60F, &pos2, &m1510, 0);
        }
    }
}


// inferred: puts the character on the result of the sweep test (name invented)
inline void SetPos_unk(CharaTask* c, const CollResult_unk& result, const CharaTask::Sphere_unk& sphere) {
    atn::Vector3 pos = result.m10;
    pos -= c->m5A4;
    pos.y -= sphere.mRadius;
    c->mPos = pos;
}

// inferred: __MtxRotate with the angle by reference and the zero first (as ElderRotZYX_unk of
// game/Em101ElderZesya.cpp); names invented
inline void ZanMtxRotate_unk(Mtx m, char axis, const float& rad) {
    if (0.0f != rad) {
        Mtx r;
        PSMTXRotRad(r, axis, rad);
        Mtx t;
        PSMTXConcat(m, r, t);
        __MtxCopy(m, t);
    }
}
inline void ZanRotZYX4_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    ZanMtxRotate_unk(m.m, 'z', rot.z);
    ZanMtxRotate_unk(m.m, 'y', rot.y);
    ZanMtxRotate_unk(m.m, 'x', rot.x);
}
void Em201ZanGurgFinal::fn_80268CB4() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    } else if (mMotion.mTracks[0].m08 >= 28.0f && mMotion.mTracks[0].m08 <= 30.0f) {
        if (m14EC > 0 && m14BC == 29) {
            fn_8026F9BC("rightwepoint");
        }
        HitInfo_unk* data = fn_80219E6C(0x610);
        fn_80144710();
        int id = m590;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 30, -1, 0.0f);
    } else if (mMotion.mTracks[0].m08 >= 44.0f && mMotion.mTracks[0].m08 <= 46.0f) {
        if (m14EC > 0 && m14BC == 45) {
            fn_8026F9BC("rightwepoint");
        }
        HitInfo_unk* data = fn_80219E6C(0x611);
        fn_80144710();
        int id = m590;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 30, -1, 0.0f);
    } else if (mMotion.mTracks[0].m08 >= 70.0f && mMotion.mTracks[0].m08 <= 73.0f) {
        if (m14EC > 0 && m14BC == 72) {
            fn_8026F9BC("rightwepoint");
        }
        HitInfo_unk* data = fn_80219E6C(0x612);
        fn_80144710();
        int id = m590;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 30, -1, 0.0f);
    }
    int fps = lbl_8087F0A8->mFrameRate;
    float g = -98.1f / (float)(fps * fps);
    atn::Vector3 move(0.0f, g, 0.29999998f);
    atn::Matrix3x4 m;
    ZanRotZYX4_unk(m, mRot);
    PSMTXMultVec(m.m, &move, &move);
    CollResult_unk result;
    CharaTask::Sphere_unk sphere = fn_80176548(mPos);
    fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x80000000, &mColli, 0, sphere.mRadius);
    SetPos_unk(this, result, sphere);
}

void Em201ZanGurgFinal::fn_80269168() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    } else if (mMotion.mTracks[0].m08 >= 14.0f && mMotion.mTracks[0].m08 <= 16.0f) {
        if (m14EC > 0 && m14BC == 15) {
            fn_8026F9BC("leftwepoint");
        }
        HitInfo_unk* data = fn_80219E6C(0x613);
        fn_80144710();
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 30, -1, 0.0f);
    }
}

inline bool IsTarget2_unk(CharaTask* c) {
    bool active;
    bool free;
    bool r;
    r = false;
    active = false;
    free = false;
    if ((c->mFlags & 4) != 4 && (c->mFlags & 1) != 1) {
        free = true;
    }
    if (free && (c->mStatus.m0C & 0x20) != 0x20) {
        active = true;
    }
    if (active) {
        bool grappled = false;
        if (c->m55C == 6 && c->m560 == 28) {
            grappled = true;
        }
        if (!grappled) {
            r = true;
        }
    }
    return r;
}

// NONMATCHING: register allocation and stack layout of the bullet loop (the original keeps the vectors and the
// target array in r19/r25-r27 and uses a 0x220 frame; 10 variants)
void Em201ZanGurgFinal::fn_802692B4() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    } else if (m14C4 == 0 && mMotion.mTracks[0].m08 >= 34.0f) {
        m14C4++;
        HitInfo_unk* data = fn_80219E6C(0x61D);
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, data, fn_800F8548(mgr), &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        FixedArray_unk<CharaTask*, 32> targets;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = (PlayerTask*)p->mNext) {
            if (IsTarget2_unk(p) && (p->m54C & 0x2000) != 0x2000 && !fn_8016F3D0(p, 1) && p->mC58.mC0 != 0) {
                targets.PushBack_unk(p);
            }
        }
        unsigned int n = targets.mCount;
        if (n < 7) {
            n = 7;
        }
        int id = fn_800F8548(lbl_8087F048);
        for (int i = 0; i < (int)n; i++) {
            Mtx m;
            atn::Vector3 dir;
            PSMTXRotRad(m, 'y', (float)i * (6.2831855f / (float)(int)n));
            atn::Vector3 front = FrontR_unk(mRot);
            dir = front;
            PSMTXMultVec(m, &dir, &dir);
            dir.y += 0.4f;
            if (targets.mCount != 0) {
                ZanHoming_unk param;
                param.mTarget = targets[i % targets.mCount];
                param.mTurn = 1.2566371f;
                param.mDelay = 0.0f;
                param.mCount = id;
                Effect_unk* mgr = lbl_8087F048;
                atn::Vector3 pos = fn_801781B0();
                fn_800F8574(mgr, this, fn_80219E6C(data->m6C), &pos, &dir, &param, 0x2006, 0, 0.0f, 1.0f);
            }
        }
    }
}

// NONMATCHING: stack slots of the temporaries (the original keeps the loop vectors above the grab ones) and
// the scheduling of the grab position; 9 variants
// inferred: grab (state 13): picks the nearest party member in front, holds it and throws it
void Em201ZanGurgFinal::fn_802696F8() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    } else if (atn::Abs_unk(mMotion.mTracks[0].m08 - 24.0f) < 1.1920929e-7f) {
        m150C = 0;
        float best = 1e16f;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = (PlayerTask*)p->mNext) {
            if (IsTarget_unk(p) && !p->fn_80155D70()) {
                atn::Vector3 d = p->mPos - mPos;
                atn::Vector3 fwd = FrontR_unk(mRot);
                float dot = PSVECDotProduct(&d, &fwd);
                if (dot >= (float)cos(0.7853982f)) {
                    float dist = PSVECSquareMag(&d);
                    if (dist < 1600.0f) {
                        if (p == m14B8) {
                            m150C = p;
                            break;
                        }
                        if (dist < best) {
                            m150C = p;
                            best = dist;
                        }
                    }
                }
            }
        }
        if (m150C != 0) {
            atn::Vector3 pos = mPos + FrontR_unk(mRot) * 20.0f;
            atn::Vector3 dir = FrontR_unk(mRot);
            atn::Vector3 back(-dir.x, -dir.y, -dir.z);
            atn::Vector3 a(pos);
            atn::Vector3 b(back);
            m150C->fn_8015D2A0(a, b);
        } else {
            m1508 = 0;
        }
    } else if (mMotion.mTracks[0].m08 >= 31.0f && mMotion.mTracks[0].m08 <= 32.0f) {
        if (m1508 == 0) {
            ToIdle_unk();
        }
    } else if (mMotion.mTracks[0].m08 >= 70.0f && mMotion.mTracks[0].m08 < 95.0f) {
        mRot.y += 0.12566371f;
    }
    if (atn::Abs_unk(mMotion.mTracks[0].m08 - 103.0f) < 1.1920929e-7f) {
        if (m14EC > 0) {
            fn_8026F9BC("leftwepoint");
        }
        ZanDamage_unk damage;
        fn_8003EA3C(&damage, fn_80219E6C(0x61E), this, m150C, 0, &lbl_807C6B90, 0, 0);
        if ((m150C->mStatus.m0C & 0x20) == 0x20) {
            m150C->vf44();
        }
        atn::Vector3 pos = m150C->fn_80148B0C(0)->m04.mCenter;
        pos += atn::Vector3(0.0f, 15.0f, 0.0f);
        fn_80108C10(lbl_8087F048, &damage, pos, this, m150C, 0, 0);
    }
}

// inferred: plays `motion` from frame `frame` (name invented)
inline void PlayMotionAt_unk(atn::Model* model, int motion, float frame) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->fn_80097C08(0, motion, 0, 0, 1, 1.0f, 0.1f);
    model->mTracks[0].m0C = 1.0f;
    model->mTracks[0].m08 = frame;
}

// NONMATCHING: stack slots of the grab position temporaries (same case as fn_802696F8)
// inferred: grab of the leader (state 14)
void Em201ZanGurgFinal::fn_80269CBC() {
    if (atn::Abs_unk(mMotion.mTracks[0].m08 - 70.0f) < 1.1920929e-7f) {
        PlayMotionAt_unk(&mMotion, 0x145, 70.0f);
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    } else if (atn::Abs_unk(mMotion.mTracks[0].m08 - 24.0f) < 1.1920929e-7f) {
        PlayerTask* leader = lbl_8087F8A0->mHead;
        if (leader->fn_8017A450() < 5) {
            atn::Vector3 pos = mPos + FrontR_unk(mRot) * 20.0f;
            atn::Vector3 dir = FrontR_unk(mRot);
            atn::Vector3 back(-dir.x, -dir.y, -dir.z);
            atn::Vector3 a(pos);
            atn::Vector3 b(back);
            leader->fn_8015D5B0(a, b);
        } else {
            atn::Vector3 pos(mPos);
            leader->fn_8015CF98(pos);
            m1508 = 0;
        }
    } else if (mMotion.mTracks[0].m08 >= 31.0f && mMotion.mTracks[0].m08 <= 32.0f) {
        if (m1508 == 0) {
            ToIdle_unk();
        }
    }
    if (mMotion.mTracks[0].m08 >= 70.0f && mMotion.mTracks[0].m08 <= 95.0f) {
        mRot.y += 0.12566371f;
    }
}

// inferred: charge (state 15) in three steps
void Em201ZanGurgFinal::fn_8026A010() {
    float frame;
    float speed = 0.0f;
    if (m14C4 == 0) {
        frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14C4 = 1;
            PlayMotionB_unk(&mMotion, 0x14B, 0.0f);
        }
    } else if (m14C4 == 1) {
        speed = 1.6315789f;
        frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14C4 = 2;
            PlayMotionB_unk(&mMotion, 0x14C, 0.0f);
        }
        HitInfo_unk* data = fn_80219E6C(0x615);
        fn_80144710();
        int id = m590;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 30, -1, 0.0f);
    } else if (m14C4 == 2) {
        speed = 0.96666664f;
        frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            ToIdle_unk();
        } else if (mMotion.mTracks[0].m08 <= 15.0f) {
            HitInfo_unk* data = fn_80219E6C(0x615);
            fn_80144710();
            int id = m590;
            fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 30, -1, 0.0f);
        }
    }
    atn::Vector3 move = m1530 * speed;
    int fps = lbl_8087F0A8->mFrameRate;
    move.y += -98.1f / (float)(fps * fps);
    CollResult_unk result;
    CharaTask::Sphere_unk sphere = fn_80176548(mPos);
    fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x80000000, &mColli, 0, sphere.mRadius);
    SetPos_unk(this, result, sphere);
}

// inferred: roar (state 16): the weapon effect lasts 900 frames
void Em201ZanGurgFinal::fn_8026A34C() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
        m14EC = 900;
    } else if (atn::Abs_unk(mMotion.mTracks[0].m08 - 80.0f) < 1.1920929e-7f) {
        fn_80232B7C(&m15AC, 0);
        PlayEffect_unk(&m15AC, &mMotion);
    } else if (atn::Abs_unk(mMotion.mTracks[0].m08 - 60.0f) < 1.1920929e-7f) {
        Field_unk* field = lbl_8087F430;
        field->m96C %= 2;
        field->m970 = 35;
        field->m974 = 0.3f;
        field->m978 = 0.3f;
        fn_8059C32C(lbl_8087F9E8, 0);
    }
}

// inferred: warp (state 17): appears at m151C, then starts m152C
void Em201ZanGurgFinal::fn_8026A4FC() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    if (m14BC > 10) {
        SetState_unk(18, 3);
        mPos = m151C;
        mRot.y = m1528;
        fn_800C344C("SE_SP014_001", &mPos, 0, -1, 1.0f);
    }
}

void Em201ZanGurgFinal::fn_8026A5BC() {
    if (m14BC <= 10) {
        return;
    }
    switch (m152C) {
    case 8:
        fn_8026B008();
        break;
    case 9:
        fn_8026B118();
        break;
    case 10:
        fn_8026B430();
        break;
    case 11:
        fn_8026B748();
        break;
    case 12:
        fn_8026B9F0();
        break;
    case 13:
        fn_8026BB08();
        break;
    case 14:
        fn_8026BE2C();
        break;
    case 15:
        fn_8026C150();
        break;
    case 16:
        fn_8026C2C0();
        break;
    case 23:
        fn_8026CC28();
        break;
    case 24:
        fn_8026CDB4();
        break;
    case 25:
        fn_8026D058();
        break;
    default:
        fn_8026AF78();
        break;
    }
}

void Em201ZanGurgFinal::fn_8026A628() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    } else if (mMotion.mTracks[0].m08 >= 19.0f && mMotion.mTracks[0].m08 <= 21.0f) {
        if (m14EC > 0 && m14BC == 20) {
            fn_8026F9BC("rightwepoint");
        }
        HitInfo_unk* data = fn_80219E6C(0x61C);
        fn_80144710();
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 30, -1, 0.0f);
    }
}

void Em201ZanGurgFinal::fn_8026A774() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    fn_8016E970(9);
    fn_80267134();
}

// inferred: defeated (state 21)
void Em201ZanGurgFinal::fn_8026A7AC() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        fn_80370AE4(lbl_8087F430, 0xE6, 1);
    }
}

void Em201ZanGurgFinal::fn_8026A828() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    }
}

void Em201ZanGurgFinal::fn_8026A8E4() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    }
}

// NONMATCHING: f3/f4 of the frame and of the new yaw (6 variants)
// inferred: jump (state 24): turns to the target and throws the shield in the middle of the jump
void Em201ZanGurgFinal::fn_8026A9A0() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    if (m1630 == 0) {
        CharaTask* t = m14B8;
        mRot.y = (t->mPos - mPos).ToRotationZ_unk().y;
        if (mMotion.mTracks[0].m08 > 67.0f) {
            fn_8026D0F8(0);
        }
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        m1648 = m1648 == 0;
        ToIdle_unk();
    }
}

// inferred: warp attack (state 25)
void Em201ZanGurgFinal::fn_8026AC8C() {
    if (m14C4 == 0 && mMotion.mTracks[0].m08 > 35.0f) {
        m14C4 = 1;
        atn::Vector3 pos;
        float rot;
        fn_8026F38C(0, &pos, &rot);
        mPos = pos;
        mRot.y = rot;
    }
    if (m14C4 == 1 && mMotion.mTracks[0].m08 > 54.0f) {
        m14C4 = 2;
        atn::Vector3 pos = mPos + FrontR_unk(mRot) * 20.0f;
        fn_80232B7C(0, 0);
        fn_8023A8B4(lbl_8087F3C0, &m160C, 0, 0, pos, lbl_807C7030, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1,
                    3.0f);
        HitInfo_unk* data = fn_80219E6C(0x619);
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, data, fn_800F8548(mgr), &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        fn_800C344C("SE_MB099_003", &pos, 0, -1, 1.0f);
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        ToIdle_unk();
    }
}

void Em201ZanGurgFinal::fn_8026AF24() {
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m14C4 = 0;
    mFlag5 = 0;
}

void Em201ZanGurgFinal::fn_8026AF78() {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ToIdle_unk();
}

// inferred: start of the attack states (8-16): state, motion and effect (names invented)
void Em201ZanGurgFinal::fn_8026B008() {
    SetState_unk(8, 3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.5f;
    mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m1594, &mMotion);
}

void Em201ZanGurgFinal::fn_8026B118() {
    SetState_unk(9, 3);
    PlayMotionB_unk(&mMotion, 0x13F, 0.0f);
    CharaTask* t = m14B8;
    mRot.y = (t->mPos - mPos).ToRotationZ_unk().y;
    fn_80232B7C(this, 100);
    PlayEffect_unk(&m15B8, &mMotion);
}

void Em201ZanGurgFinal::fn_8026B430() {
    SetState_unk(10, 3);
    PlayMotionB_unk(&mMotion, 0x140, 0.0f);
    CharaTask* t = m14B8;
    mRot.y = (t->mPos - mPos).ToRotationZ_unk().y;
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m1594, &mMotion);
}

void Em201ZanGurgFinal::fn_8026B748() {
    SetState_unk(11, 3);
    PlayMotionB_unk(&mMotion, 0x141, 0.0f);
    CharaTask* t = m14B8;
    mRot.y = (t->mPos - mPos).ToRotationZ_unk().y;
}

void Em201ZanGurgFinal::fn_8026B9F0() {
    SetState_unk(12, 3);
    PlayMotionB_unk(&mMotion, 0x142, 0.0f);
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m1618, &mMotion);
    m1688 = 0;
}

void Em201ZanGurgFinal::fn_8026BB08() {
    SetState_unk(13, 3);
    PlayMotionB_unk(&mMotion, 0x145, 0.0f);
    CharaTask* t = m14B8;
    mRot.y = (t->mPos - mPos).ToRotationZ_unk().y;
    m1508 = 1;
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m1594, &mMotion);
}

void Em201ZanGurgFinal::fn_8026BE2C() {
    SetState_unk(14, 3);
    PlayMotionB_unk(&mMotion, 0x146, 0.0f);
    CharaTask* t = m14B8;
    mRot.y = (t->mPos - mPos).ToRotationZ_unk().y;
    m1508 = 1;
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m1594, &mMotion);
}

void Em201ZanGurgFinal::fn_8026C150() {
    SetState_unk(15, 3);
    PlayMotionB_unk(&mMotion, 0x14A, 0.0f);
    CharaTask* t = m14B8;
    m1530 = t->mPos - mPos;
    m1540 = PSVECMag(&m1530);
    PSVECNormalize(&m1530, &m1530);
    m153C = 0.0f;
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m1594, &mMotion);
}

void Em201ZanGurgFinal::fn_8026C2C0() {
    SetState_unk(16, 3);
    PlayMotionB_unk(&mMotion, 0x143, 0.0f);
    fn_80239DAC(lbl_8087F3C0, &m15A0, 0, 0);
    fn_80232B7C(this, 100);
    PlayEffect_unk(&m1570, &mMotion);
}

void Em201ZanGurgFinal::fn_8026C3E0(int point, int next) {
    SetState_unk(17, 3);
    m152C = next;
    fn_8026F38C(point, &m151C, &m1528);
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m1564, &mMotion);
}

// inferred: counterattack (state 19) against `attacker`
void Em201ZanGurgFinal::fn_8026C4E8(CharaTask* attacker) {
    SetState_unk(19, 6);
    m560 = 10;
    mRot.y = (attacker->mPos - mPos).ToRotationZ_unk().y;
    PlayMotionB_unk(&mMotion, 0x1D2, 0.0f);
}

void Em201ZanGurgFinal::fn_8026C7A4() {
    SetState_unk(20, 9);
    PlayMotion_unk(&mMotion, 1, 1, 0.1f);
}

void Em201ZanGurgFinal::fn_8026C844(atn::Vector3 dir) {
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    ASSERT_UNK(this != 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 21;
    fn_8016E970(6);
    m560 = 0x8E;
    PlayMotion_unk(&mMotion, 0x171, 0, 0.05f);
    atn::Vector3 n = dir;
    PSVECNormalize(&n, &n);
    atn::Vector3 v = dir;
    v.y *= 0.3f;
    m6B8 = v;
    m1558 = atn::Vector3(-n.x, -n.y, -n.z).ToRotation_unk();
}

void Em201ZanGurgFinal::fn_8026CB50() {
    SetState_unk(22, 3);
    PlayMotionB_unk(&mMotion, 0x33, 0.0f);
    fn_8026D0F8(1);
    m1644 = 1;
    fn_80239DAC(lbl_8087F3C0, this, 100, 0);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
}

// inferred: picks up the shield again (state 23)
void Em201ZanGurgFinal::fn_8026CC28() {
    if (m1660 != 2) {
        ToIdle_unk();
        return;
    }
    SetState_unk(23, 3);
    PlayMotionB_unk(&mMotion, 0x143, 1.0f);
    mMotion.mTracks[0].m08 = mMotion.fn_80097D7C(0) - 30.0f;
    m1630 = 0;
    for (int i = 0; i < 4; i++) {
        mMotion.fn_8009373C(sChainNodes_unk[i], 1);
    }
    if (m162C != 0 && fn_80370A78(lbl_8087F430, 0xEE) == 0) {
        fn_80370AE4(lbl_8087F430, 0xEE, 1);
    }
}

void Em201ZanGurgFinal::fn_8026CDB4() {
    SetState_unk(24, 3);
    PlayMotionB_unk(&mMotion, 0x166, 1.0f);
    CharaTask* t = m14B8;
    mRot.y = (t->mPos - mPos).ToRotationZ_unk().y;
}

void Em201ZanGurgFinal::fn_8026D058() {
    SetState_unk(25, 3);
    PlayMotionB_unk(&mMotion, 0x165, 0.0f);
}

// inferred: largest scale axis of a matrix (as MaxScale_unk of gmk/Chain.cpp; names invented)
#define ZAN_MAX_unk(a, b) ((a) > (b) ? (a) : (b))
inline atn::Vector3 ZanCol_unk(const atn::Matrix3x4& m, int i) {
    return atn::Vector3(m.m[0][i], m.m[1][i], m.m[2][i]);
}
inline float ZanMaxScale_unk(const atn::Matrix3x4& mtx) {
    atn::Vector3 az = ZanCol_unk(mtx, 2);
    float sz = PSVECMag(&az);
    atn::Vector3 ay = ZanCol_unk(mtx, 1);
    float sy = PSVECMag(&ay);
    atn::Vector3 ax = ZanCol_unk(mtx, 0);
    float sx = PSVECMag(&ax);
    atn::Vector3 s(sx, sy, sz);
    return ZAN_MAX_unk(s.x, ZAN_MAX_unk(s.y, s.z));
}
// inferred: puts the shield model on a node matrix (name invented)
inline void SetShieldMtx_unk(chr::CharaPart_unk* part, const atn::Matrix3x4& m) {
    atn::Model* model = (atn::Model*)part->m010;
    __MtxCopy(model->mMtx, m.m);
    model->m54 = ZanMaxScale_unk(m);
}
inline atn::Matrix3x4& ShieldMtx_unk(chr::CharaPart_unk* part) {
    return *(atn::Matrix3x4*)((atn::Model*)part->m010)->mMtx;
}

// NONMATCHING: stack layout of the temporaries and the scale helper (f0/f8 of the column loads); logic follows
// the original
// inferred: the shield leaves the hand: 0 = thrown at the target, 1 = dropped (taken by the leader when
// it held it, mode 3)
void Em201ZanGurgFinal::fn_8026D0F8(int mode) {
    m1630 = 1;
    for (int i = 0; i < 4; i++) {
        mMotion.fn_8009373C(sChainNodes_unk[i], 0);
    }
    int prev = m1660;
    m1660 = mode;
    switch (mode) {
    case 0: {
        m1624->m224 = 0;
        SetShieldMtx_unk(m1624, *(atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightwepoint"));
        m1668 = 0.0f;
        m166C = ShieldMtx_unk(m1624).GetTrans_unk();
        atn::Vector3 d(m14B8->mPos.x - m166C.x, 0.0f, m14B8->mPos.z - m166C.z);
        PSVECNormalize(&d, &d);
        m1678 = m14B8->mPos + d * 3.0f;
        atn::Vector3 from = m1678;
        from.y += 10.0f;
        atn::Vector3 to = from;
        to.y -= 30.0f;
        CollResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
            m1678 = result.m04;
        }
        m1678.y = mPos.y + 3.0f;
        m1684 = fn_800F8548(lbl_8087F048);
        break;
    }
    case 1: {
        PlayerTask* leader = lbl_8087F8A0->mHead;
        m1624->m224 = 0;
        if (prev == 3) {
            SetShieldMtx_unk(m1624, *(atn::Matrix3x4*)leader->mMotion.GetNodeMtx_unk("rightwepoint"));
        } else {
            SetShieldMtx_unk(m1624, *(atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightwepoint"));
        }
        m1668 = 0.0f;
        m166C = ShieldMtx_unk(m1624).GetTrans_unk();
        atn::Vector3 v(0.0f, 0.0f, 1.0f);
        atn::Matrix3x4 rot = ShieldMtx_unk(m1624).GetRotation_unk();
        PSMTXMultVec(rot.m, &v, &v);
        m1664 = v.ToRotation_unk().x - 2.984513f;
        CollResult_unk result;
        atn::Vector3 from = prev == 3 ? leader->mPos : mPos;
        from.y += 10.0f;
        atn::Vector3 to = from;
        atn::Vector3 dir = prev == 3 ? FrontR_unk(leader->mRot) : FrontR_unk(mRot);
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        atn::Vector3 side;
        PSVECCrossProduct(&dir, &up, &side);
        atn::Vector3 n = side - dir;
        PSVECNormalize(&n, &n);
        to += n * 40.0f;
        m1678 = to;
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0)) {
            m1678 = result.m04 - n * 5.0f;
        }
        m1678.y = mPos.y + 11.0f;
        break;
    }
    }
}

inline atn::Vector3 Neg2_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}
// inferred: smaller of two values, as a macro (name invented)
#define MIN_UNK(a, b) ((a) < (b) ? (a) : (b))

// inferred: start and top height of the shield falling (.sdata); names invented
float lbl_8087DC30 = 0.0f;
float lbl_8087DC34 = 50.0f;

// inferred: zone hint shown while the leader can take the shield (as GolemUIHint_unk of game/Em209SmallGolem.cpp)
struct ZanUIHint_unk : public GameUIElem1C_unk {
    ZanUIHint_unk(int a, int b, int c) {
        m00 = a;
        m04 = b;
        m08 = c;
        m0C = 0;
        m10 = 0;
        m14 = 0;
        m18 = 1;
    }
};

// inferred: the leader takes the shield (names invented)
inline void TakeShield_unk(Em201ZanGurgFinal* z, CharaTask* leader) {
    ZanWeapon_unk* data = (ZanWeapon_unk*)z->m1624->m274;
    fn_8044D104(Inventory::instance_, data->m78, data->m80, 0, leader->m50);
    chr::CharaPart_unk* old = leader->mParts.mData[1];
    fn_80044E0C(old, 1);
    leader->mParts.mData[1] = z->m1624;
    leader->fn_8014EF48();
    if (leader->m648 == old) {
        leader->fn_8014DEE4(0, 0, 0);
    }
    if (z->m1624 == z->m1628) {
        if (z->m162C == 0) {
            fn_804438E0(Inventory::instance_, 0x18875);
        }
        z->m162C = 1;
    }
    z->m1624 = old;
}

// NONMATCHING: written from the original's logic; the original inlines the drop of fn_8026D0F8(1) next to the
// hit character (inlining it here exceeds the inline budget) and the register/stack allocation differs
// inferred: update of the shield that left the hand: 0 = flying to the target (and back), 1 = falling, 2 = on
// the ground (the leader can take it), 3 = taken by the leader
void Em201ZanGurgFinal::fn_8026DB8C() {
    switch (m1660) {
    case 0: {
        atn::Vector3 d0 = m1678 - m166C;
        float speed = 1.0f;
        if (1.0f >= 15.0f / PSVECMag(&d0)) {
            speed = 15.0f / PSVECMag(&d0);
        }
        bool hit = false;
        if (m1668 >= 1.0f) {
            speed = 0.1f;
        }
        if (!(m1668 >= 1.0f)) {
            hit = true;
        }
        float prev = m1668;
        atn::Vector3 from = atn::Lerp_unk(m166C, m1678, MIN_UNK(1.0f, m1668));
        m1668 += speed * lbl_8087EFA8->m3A4;
        atn::Vector3 to = atn::Lerp_unk(m166C, m1678, MIN_UNK(1.0f, m1668));
        if (m1668 >= 1.0f) {
            if (prev < 1.0f) {
                fn_800C344C("SE_SP015_001", &to, 0, -1, 1.0f);
            }
            if (m1668 >= 2.0f) {
                m1660 = 2;
                m1668 = 2.0f;
                int id = fn_800F8548(lbl_8087F048);
                HitInfo_unk* data = fn_80219E6C(0x61A);
                fn_80232B7C(this, 1000);
                Color4f_unk color(1.0f, 1.0f, 1.0f, 1.0f);
                fn_8023A8B4(lbl_8087F3C0, &m157C, 0, 0, m1678, mRot, color, -1, -1, 1, 1.0f);
                fn_800FAB80(lbl_8087F048, this, data, id, &m1678, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
                fn_800C344C("SE_SP016_002", &to, 0, -1, 1.0f);
            }
        }
        atn::Vector3 pos = atn::Lerp_unk(m166C, m1678, MIN_UNK(1.0f, m1668));
        atn::Vector3 rot = (m1678 - m166C).ToRotation_unk();
        rot.x = atn::WrapAngle_unk(-rot.x - 1.5707964f);
        rot.y = atn::WrapAngle_unk(rot.y + 3.1415927f);
        rot.z = atn::WrapAngle_unk(rot.z);
        if (m1668 >= 0.8f) {
            rot.x += atn::WrapAngle_unk(-2.984513f - rot.x) * MIN_UNK(1.0f, 3.5f * (m1668 - 0.8f));
            pos.y += MIN_UNK(1.0f, 3.5f * (m1668 - 0.8f)) * (8.0f + m1678.y - pos.y);
        }
        atn::Matrix3x4 m;
        PSMTXTrans(m.m, pos.x, pos.y, pos.z);
        atn::Matrix3x4 r;
        ZanRotZYX4_unk(r, atn::Vector3(rot.x, rot.y, rot.z));
        PSMTXConcat(m.m, r.m, m.m);
        SetShieldMtx_unk(m1624, m);
        if (hit) {
            CollResult_unk result;
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 2, &mColli, 0) && result.m38 != 0 &&
                (result.m38->mFlags & 2) == 2) {
                atn::Vector3 at = result.m04;
                CharaTask* chara = (CharaTask*)result.m38->m0C;
                HitInfo_unk* data = fn_80219E6C(0x61B);
                atn::Vector3 dir = d0;
                dir.y = 0.0f;
                PSVECNormalize(&dir, &dir);
                int r2 = chara->vf4C(this, data, (int)&at, &dir);
                if (fn_80219558(chara->m50) == 3 && IsTarget_unk(chara) && chara->mC58.mC0 != 0 && rand() % 3 == 0) {
                    r2 = 2;
                }
                if (r2 != 0) {
                    float scale = fn_8010B250(lbl_8087F048, data->m3C - chara->mStatus.mParamB.m34, 0);
                    atn::Vector3 v = dir * scale;
                    chara->fn_8015D8C0(0.1f, v * 4.0f, (int)this);
                    fn_800C344C("SE_BTCOM_015", &at, 0, -1, 2.0f);
                    fn_8026D0F8(1);
                    m1644 = 1;
                } else {
                    float scale = fn_8010B250(lbl_8087F048, data->m3C, 0);
                    atn::Vector3 n = d0;
                    PSVECNormalize(&n, &n);
                    fn_800FDB20(lbl_8087F048, this, chara, 0, data, m1684, &at, &n, scale);
                    fn_80101434(lbl_8087F048, &at, &rot, 0, 0);
                    fn_800C344C("SE_BTCOM_006", &at, 0, -1, 1.0f);
                }
            }
        }
        break;
    }
    case 1: {
        m1668 += 0.03f * lbl_8087EFA8->m3A4;
        if (m1668 >= 1.0f) {
            fn_800C344C("SE_SP015_001", &m1678, 0, -1, 1.0f);
            m1660 = 2;
            m1668 = 1.0f;
        }
        float t = MIN_UNK(1.0f, m1668);
        float h = 1.0f - 2.0f * t;
        h = 1.0f - h * h;
        atn::Vector3 d = m1678 - m166C;
        atn::Vector3 pos(m166C.x + d.x * t, m166C.y + d.y * t + (lbl_8087DC30 + h * (lbl_8087DC34 - lbl_8087DC30)),
                         m166C.z + d.z * t);
        float spin = m1668 > 0.95f ? 0.12566371f : 1.8849558f;
        if (m1668 > 0.9f) {
            if (atn::Abs_unk(m1664) > 0.01f) {
                if (m1664 < 0.0f && m1664 > -spin) {
                    m1664 = 0.0f;
                } else {
                    m1664 += spin;
                }
            }
        } else {
            m1664 += spin;
        }
        m1664 = atn::WrapAngle_unk(m1664);
        atn::Matrix3x4 m;
        PSMTXTrans(m.m, pos.x, pos.y, pos.z);
        Mtx r;
        PSMTXRotRad(r, 'x', -2.984513f + m1664);
        PSMTXConcat(m.m, r, m.m);
        SetShieldMtx_unk(m1624, m);
        break;
    }
    case 2: {
        PlayerTask* leader = lbl_8087F8A0->mHead;
        atn::Vector3 pos = ShieldMtx_unk(m1624).GetTrans_unk();
        atn::Vector3 d = pos - leader->mPos;
        if ((float)sqrt(d.x * d.x + d.z * d.z) < 20.0f && leader->fn_8016DDB0(0)) {
            if (lbl_8087F490 != 0) {
                lbl_8087F490->m764 = ZanUIHint_unk(6, 14, -1);
            }
            if (((Input_unk*)lbl_8087F0A8->mPad)->fn_801231D0(2)) {
                if (fn_80370A78(lbl_8087F430, 0xF1) == 0) {
                    fn_80370AE4(lbl_8087F430, 0xF1, 1);
                }
                m1660 = 3;
                TakeShield_unk(this, lbl_8087F8A0->mHead);
                leader->fn_80161570(0x1EC, 0, 0, -1.0f, -1.0f);
                fn_800C344C("SE_BTCOM_102", &leader->mPos, 0, -1, 1.0f);
                if (lbl_8087F490 != 0) {
                    fn_803E3384(lbl_8087F490);
                }
                if (m163C == 0) {
                    fn_80370320(lbl_8087F430, 0x396, 1, 0);
                }
            }
        }
        if (fn_80370A78(lbl_8087F430, 0xED) == 0) {
            fn_80370AE4(lbl_8087F430, 0xED, 1);
        }
        break;
    }
    case 3: {
        if (m163C == 0) {
            fn_80370320(lbl_8087F430, 0x396, 1, 0);
        }
        PlayerTask* leader = lbl_8087F8A0->mHead;
        if (leader->m55C == 6 && (leader->m560 == 0x17 || leader->m560 == 0x63)) {
            TakeShield_unk(this, leader);
            fn_8026D0F8(1);
            fn_80370320(lbl_8087F430, 0x396, 0, 0);
            m1644 = 0;
        }
        break;
    }
    }
    ((atn::Model*)m1624->m010)->CalcMatrixStack(0, atn::ModelNodeCallback_unk());
}

// NONMATCHING: r3/r4 of the target and of the temporary in every case (6 variants)
// inferred: warp point `point` around the target (0/4/5 behind it at 25/50/20, 1 in front, 2 the center of the
// arena, 6 the shield) put on the ground, and the rotation facing the target
void Em201ZanGurgFinal::fn_8026F38C(int point, atn::Vector3* pos, float* rot) {
    if (m14B8 == 0) {
        return;
    }
    atn::Vector3 d(m14B8->mPos.x - m14F4.x, 0.0f, m14B8->mPos.z - m14F4.z);
    if (PSVECMag(&d) > 0.0001f) {
        PSVECNormalize(&d, &d);
    } else {
        d = atn::Vector3(0.0f, 0.0f, 1.0f);
    }
    switch (point) {
    case 0:
        *pos = m14B8->mPos + Neg2_unk(d) * 25.0f;
        pos->y += 10.0f;
        break;
    case 1:
        *pos = m14B8->mPos + d * 25.0f;
        pos->y += 10.0f;
        break;
    case 2:
        *pos = m14F4;
        pos->y += 10.0f;
        break;
    case 4:
        *pos = m14B8->mPos + Neg2_unk(d) * 50.0f;
        pos->y += 10.0f;
        break;
    case 5:
        *pos = m14B8->mPos + Neg2_unk(d) * 20.0f;
        pos->y += 10.0f;
        break;
    case 6:
        *pos = ShieldMtx_unk(m1624).GetTrans_unk();
        pos->y += 10.0f;
        break;
    }
    CollResult_unk result;
    atn::Vector3 to = atn::Vector3(0.0f, -100.0f, 0.0f) + *pos;
    if (fn_8004ED34(lbl_8087EE98, &result, pos, &to, 0x80000000, &mColli, 0)) {
        *pos = result.m10;
    }
    *rot = (m14B8->mPos - *pos).ToRotationZ_unk().y;
}

// inferred: shock wave in front (the node is not used)
void Em201ZanGurgFinal::fn_8026F9BC(const char* node) {
    atn::Vector3 v(0.0f, 0.0f, 20.0f);
    Mtx m;
    float y = mRot.y;
    PSMTXRotRad(m, 'y', y);
    PSMTXMultVec(m, &v, &v);
    atn::Vector3 pos = mPos + v;
    CollResult_unk result;
    atn::Vector3 to = atn::Vector3(0.0f, -100.0f, 0.0f) + pos;
    if (fn_8004ED34(lbl_8087EE98, &result, &pos, &to, 0x80000000, &mColli, 0)) {
        atn::Vector3* hit = &result.m10;
        pos = *hit;
    }
    int id = fn_800F8548(lbl_8087F048);
    HitInfo_unk* data = fn_80219E6C(0x616);
    fn_80232B7C(this, 1000);
    fn_8023A8B4(lbl_8087F3C0, &m157C, 0, 0, pos, mRot, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
    fn_800FAB80(lbl_8087F048, this, data, id, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    fn_800C344C("SE_SP016_002", &pos, 0, -1, 1.0f);
    m14F0 = 10;
}
