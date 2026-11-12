#include "game/LastCocoonQuadruped.h"
#include "atn/RenderManager.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/BattleTask.h"
#include "game/GameTask.h"
#include "game/MapTask.h"
#include "game/EnemyManager.h"
#include "game/GimmickCommon.h"
#include "atn/DrawHelper.h"
#include "game/GameConfig_unk.h"
#include "atn/Csv.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>

struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: field (lbl_8087F430): camera shake and event flags (as in gmk/ShakeCamera.cpp)
struct QuadField_unk {
    void Shake_unk(int frame, float x, float y) {
        m96C = m96C % 2;
        m970 = frame;
        m974 = x;
        m978 = y;
    }
    unsigned char m00[0x96C];
    int m96C;
    int m970;
    float m974;
    float m978;
    unsigned char m97C[0x10D8 - 0x97C];
    RouteTable_unk* m10D8;
};
extern "C" QuadField_unk* lbl_8087F430;
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: nodes of the hit spheres (vf50, vf54)
static const char* const sSphereNodes_unk[3] = {"head", "waist", "spine"};

// inferred: hit capsules of the legs and the neck: two nodes and a radius (vf20; names invented)
struct QuadCapsule_unk {
    const char* mFrom;
    const char* mTo;
    float mRadius;
};
static const QuadCapsule_unk sQuadCapsules_unk[5] = {
    {"neck", "ef_eye", 8.0f},
    {"rightleg", "rightfoot", 8.0f},
    {"leftleg", "leftfoot", 8.0f},
    {"rightmiddle1", "rightforearm", 8.0f},
    {"lefthand", "leftforearm", 8.0f},
};

// inferred: angle used by fn_80460B48 (50 degrees; initialized in the __sinit; name invented)
static float sAngle_unk = atn::DegToRad_unk(50.0f);

// inferred: game state (as in game/CharaTask.cpp); names invented
struct QuadGameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" QuadGameState_unk* lbl_8087EFA8;
extern "C" float fn_800A08D4(atn::LoadHandle* motion); // inferred: length of the motion data
extern "C" float PSVECSquareMag(const atn::Vector3* v);

// inferred: as in game/CharaTask.cpp (names invented)
inline float GetMotionLength_unk(atn::Model* model, int id) {
    atn::LoadHandle* motion = model->fn_80097D40(id);
    float length = motion != 0 ? fn_800A08D4(motion) : 30.0f;
    if (length == 0.0f) {
        length = 1.0f;
    }
    return length;
}
inline float Clamp01_unk(float x) {
    if (x < 0.0f) {
        x = 0.0f;
    } else if (x > 1.0f) {
        x = 1.0f;
    }
    return x;
}
inline float LengthSq_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}

// inferred: damage record shown by fn_80108C10 (as StatusDamage_unk of game/CharaStatus.cpp)
struct QuadDamage_unk {
    QuadDamage_unk(int damage) : m08(0), m0C(0), m10(0), m14(-1) {
        m1C = 0;
        m18 = -1;
        m04 = damage;
        m00 = 1;
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
extern "C" void fn_80108C10(Effect_unk* effect, QuadDamage_unk* damage, const atn::Vector3& pos, CharaTask* from,
                            CharaTask* to, int b, int c);

extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in game/BattleCharaTask.cpp)

struct HitResult_unk;
extern "C" int fn_800F8C6C(Effect_unk* mgr, HitResult_unk* results, int count, CharaTask* chara, HitInfo_unk* data,
                           int id, int time, int hits, float angle); // inferred: deals a hit, returns the hits
// inferred: as in game/Em206EggMonster.cpp (name invented)
inline float WrapDeg_unk(float deg) {
    float d = fmod(deg, 360.0);
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    return d;
}

// inferred: places the quadruped walks around (ids 601-610; names invented)
struct QuadPlace_unk {
    int mId;
    int m04;
    int m08;
};
static const QuadPlace_unk sQuadPlaces[10] = {
    {601, 1, 0}, {606, 0, 1}, {602, 1, 0}, {607, 0, 1}, {603, 1, 0},
    {608, 0, 1}, {604, 1, 0}, {609, 0, 1}, {605, 1, 0}, {610, 0, 1},
};

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, void* model, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, const Color4f_unk& color, int arg7, int arg8, int arg9,
                            float scale);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: plays an effect on the model with the default color and scale (as in game/LastCocoonEgg.cpp)
static inline void PlayEfp_unk(atn::EfpFile* efp, void* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    rot.Set_unk(0.0f, 0.0f, 0.0f);
    pos.Set_unk(0.0f, 0.0f, 0.0f);
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void* lbl_8087EE98; // inferred: collision manager
// inferred: result of the ray test (as in game/Em309Ghoul.cpp)
struct QuadCollResult_unk {
    QuadCollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x34 - 0x1C];
    void* m34;
    void* m38;
    unsigned int m3C;
    int m40;
    atn::Vector3 m44;
};
extern "C" int fn_8004ED34(void* colli, QuadCollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

extern "C" int fn_8004D388(void* world, QuadCollResult_unk* result, const atn::Vector3* from,
                           const atn::Vector3* to, unsigned int mask, void* ignore, int arg,
                           float radius); // inferred: sphere sweep
// inferred: RouteTable_unk::FindPlace_unk on the field table with the array read inside the loop
// (as in game/Em208Queen.cpp)
inline RoutePlace_unk* FindQuadPlace_unk(int id) {
    RouteTable_unk* table = lbl_8087F430->m10D8;
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        RoutePlace_unk* places = table->mPlaces;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}

inline void LastCocoonQuadruped::SetMode_unk(int mode) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    if (lbl_8087F430 != 0) {
        ((GameTask*)lbl_8087F430)->fn_80370AE4(0x8C, 0);
    }
    m58C = mode;
    fn_8016E970(3);
}

extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b); // inferred: a + b
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: node callbacks of the model (as in game/CharaTask.cpp and game/Em403WarLion.cpp)
typedef boost::function<void(atn::Matrix3x4*, const atn::NodeChunk&, const atn::ModelNodeCallbackContext&),
                        std::allocator<boost::function_base> >
    NodeCallback_unk;
extern "C" void fn_800F29D0(atn::Model* model, NodeCallback_unk callback);

// NONMATCHING: 0% - the original is over the inlining budget (docs/inlining.md R3) and calls every inline
// (LH_Binary, Vector3, Matrix3x4, QuadRoutes_unk and ResHandle constructors, the strings, the tokenizer);
// this one stays under it and inlines them (same problem as the constructor of LastCocoonChrysalis)
LastCocoonQuadruped::LastCocoonQuadruped(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m14CC(0),
      m14D4(0.0f, 0.0f, 0.0f), m14E0(0.0f, 0.0f, 0.0f), m14EC(0), m1554(0), m1558(1), m1574(0), m15A8(0),
      m15B0(0.0f, 0.0f, 0.0f), m15BC(0.0f, 0.0f, 0.0f), m15C8(0), m15CC(0.0f, 0.0f, 0.0f), m1800(0), m18EC(0),
      m18F4(0) {
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_last_cocoon.csv");
    std::string params(info->m2C);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(params, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp(it->c_str(), "db_name=", 8) == 0) {
            name = it->c_str() + 8;
        }
    }
    std::string path = fn_8006AFF8(dir, name);
    m14B0.vf0C(path.c_str());
    m1790.LoadRequest("ef_bs042a.efp");
    m179C.LoadRequest("ef_bs043a.efp");
    m1824.LoadRequest("ef_sk043c_01.eff");
    m1830.LoadRequest("ef_sk043a.efp");
    m183C.LoadRequest("ef_sk043g_01.eff");
    m1848.LoadRequest("ef_sk043g_02.eff");
    mFlagC31 = 1;
}

int LastCocoonQuadruped::vf0C() {
    if (fn_8013655C() == 0 && m14B0.fn_80473F50() == 0 && m1790.IsLoading() == 0 && m179C.IsLoading() == 0 &&
        m1824.IsLoading() == 0 && m1830.IsLoading() == 0 && m183C.IsLoading() == 0 && m1848.IsLoading() == 0) {
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC219;
        mStatus.m18 |= 0x3800000;
        fn_80087994(m18F0.Load("LastCocoonQuadruped"), "DebugDisp", &m18F4, 0, 0);
        if (m14B0.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
            do {
                csv.fn_8005B9CC();
            } while (csv.fn_8005BBF8());
        }
        m56C = 0.125f;
        mColli.mFlags &= ~0x80000000;
        for (int i = 0; i < 5; i++) {
            mCapsules[i].m0C = (int)this;
            mCapsules[i].mFlags |= 3;
            mCapsules[i].mFlags &= ~0x80000000;
        }
        m54C |= 0x200;
        m17A8.m20 = 0x98;
        m17A8.mFlags &= ~1;
        m17A8.mFlags |= 2;
        m17A8.m0C = (int)this;
        fn_800F29D0(&mMotion, boost::bind(&LastCocoonQuadruped::fn_80462CCC, this, _1, _2));
        mMotion.fn_80091CFC("neck");
        mMotion.fn_80091CFC("waist");
        mMotion.fn_80091CFC("spine");
        atn::Vector3 limit(0.7853982f, 0.62831855f, 0.0f);
        mLook.mLimit = limit;
        return 1;
    }
    return 0;
}

// inferred: field object (as in gmk/FlamingArrow.cpp)
struct Field_unk {
    RouteTable_unk* fn_8013C504();
};
extern "C" Field_unk* fn_80121F00();
// inferred: player input of the field (as in game/Em201ZanGurgFinal.cpp; weak copies with C names)
struct QuadInput_unk;
extern "C" QuadInput_unk* fn_80122550(Field_unk* field);
extern "C" void fn_8026616C(QuadInput_unk* input, CharaTask* chara);
extern "C" void fn_8026610C(QuadInput_unk* input, const atn::Vector3& pos, float f, int keep);
extern "C" atn::Vector3* fn_80266164(QuadInput_unk* input);
extern "C" atn::Matrix3x4* fn_80266154(atn::Model* model);
extern "C" float fn_80011220(float x); // inferred: fabs

// NONMATCHING: 0% - the original is over the inlining budget (docs/inlining.md R3: Vector3 copies, operators,
// ToRotation_unk, GetTrans_unk and the matrix helpers are all called); this one inlines them
void LastCocoonQuadruped::vf10() {
    fn_804617A4();
    atn::Vector3 pos(mPos);
    atn::Vector3 rot(mRot);
    m14C0++;
    if (mC58.mC0 == 0 || m14B8 == 0) {
        fn_8016E970(3);
    } else {
        if (m14C4 == 0 && m58C != 6 && m58C != 8) {
            int kind = 0;
            atn::Vector3 target;
            float angle;
            if (fn_80468038(&kind, &target, &angle) != 0) {
                if (++m14C8 > 5) {
                    m14D4 = fn_804687B0(&m14F4, 0, 1);
                    atn::Vector3 next = fn_80468A08(m14F4, 0);
                    m14E0 = (next - m14D4).ToRotation_unk();
                    m14C4 = 1;
                    m14EC = 0;
                    m14C8 = 0;
                    m14F0 = (rand() % 3) * 2 + 4;
                } else {
                    m14D4 = target;
                    m14E0 = mRot;
                    m14E0.y = angle;
                }
                fn_80466B8C();
            }
        }
        if (m14B8 != 0) {
            int look = 0;
            switch (m58C) {
            case 6:
                fn_80463610();
                look = 1;
                break;
            case 7:
                fn_804638A4();
                break;
            case 8:
                fn_80463C58();
                break;
            case 9:
                fn_80463DCC();
                break;
            case 10:
                fn_80463F20();
                break;
            case 11:
                fn_80464608();
                break;
            case 12:
                fn_80464CF0();
                break;
            case 13:
                fn_80465294();
                break;
            case 14:
                fn_804654F0();
                break;
            case 15:
                fn_804655AC();
                break;
            case 16:
                fn_80465B88();
                break;
            case 17:
                fn_80466370();
                break;
            case 18:
                fn_804666D8();
                look = 1;
                break;
            case 19:
                fn_80466868();
                break;
            default:
                if (m55C != 6) {
                    fn_80463174();
                }
                if (m58C == 0) {
                    fn_804635B4();
                }
                break;
            }
            mFlag24 = 0;
            if (look != 0) {
                mLook.fn_80129A48(0.3f);
            } else {
                mLook.fn_80129978(m14B8->GetModel_unk(), "neck", lbl_807C7030, 0.3f);
            }
            MapTask* map = ((GameTask*)fn_80121F00())->GetCurrentMap();
            QuadInput_unk* input = fn_80122550(fn_80121F00());
            if (m58C == 15 || m58C == 16) {
                if (map->GetCamera_unk() != 2) {
                    map->fn_804A04AC(2, 30, -1, 0);
                    map->SetDefaultCamera_unk(2);
                }
            } else if (m58C == 6) {
                if (mMotion.fn_80139F2C(0) == 0x14A) {
                    if (map->GetCamera_unk() != 0) {
                        map->fn_804A04AC(0, 15, -1, 0);
                        map->SetDefaultCamera_unk(0);
                    }
                } else {
                    atn::Vector3 spine = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine1"))->GetTrans_unk();
                    if (m18EC == 0) {
                        fn_8026616C(input, this);
                        fn_8026610C(input, spine, 0.0f, 0);
                        m18EC = 1;
                    } else {
                        fn_8026616C(input, this);
                        fn_80266164(input)[1] = spine;
                    }
                    if (map->GetCamera_unk() != 2) {
                        map->fn_804A04AC(2, 150, -1, 0);
                        map->SetDefaultCamera_unk(2);
                    }
                }
            } else if (m58C == 17) {
                if (map->GetCamera_unk() != 0) {
                    map->fn_804A04AC(0, 30, -1, 0);
                    map->SetDefaultCamera_unk(0);
                }
            } else if (m58C == 18) {
                if (map->GetCamera_unk() != 0) {
                    map->fn_804A04AC(0, 60, -1, 0);
                    map->SetDefaultCamera_unk(0);
                }
            } else {
                if (map->GetCamera_unk() != 0) {
                    map->fn_804A04AC(0, 15, -1, 0);
                    map->SetDefaultCamera_unk(0);
                }
            }
        }
    }
    m15CC.y *= 0.8f;
    float d = rot.y + m15CC.y - mRot.y;
    if (fn_80011220(d) > sAngle_unk) {
        d = sAngle_unk * (d / fn_80011220(d));
    }
    m15CC.y = d;
    fn_8014C540();
    fn_80145334();
    atn::Vector3 offset(0.0f, 0.0f, 0.0f);
    m1578 = *fn_80266154(&mMotion);
    offset += m1578.GetTrans_unk();
    m1578.fn_8010F668();
    atn::Matrix3x4 turn;
    turn.fn_8013A13C(3.1415927f);
    m1578.fn_801495E8(turn);
    m1578.fn_801479E4(offset);
    m148C = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("neck"))->GetTrans_unk();
    m1498 = 5.0f;
}

void LastCocoonQuadruped::vf14() {
    fn_80149A30();
}

void LastCocoonQuadruped::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m14C4 != 7 && m58C != 8) {
        if (m14C4 == 6 || m14C4 == 1 || (m58C >= 17 && m58C <= 19)) {
            msg->m10 *= 0.0f;
        }
        fn_80151448(msg);
        vf40(hit);
    }
}

// NONMATCHING: 99.9% - r5/r6 of the rotation temporary and of m14E0 swapped
void LastCocoonQuadruped::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    int down = 0;
    if (m58C == 13 && ++m15A8 > 3) {
        down = 1;
    }
    if (down) {
        SetMode_unk(14);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x148, 0, 0, 1, 0.0f, 0.1f);
        if (m1574 != 0) {
            m1574->fn_8015B238();
            m1574 = 0;
        }
        fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
    }
    if (msg->mData->m04 == 0x5BA) {
        m18E8 = 1;
    }
    float rate = mStatus.mParamB.m84 > 0 ? mStatus.m04 / (float)mStatus.mParamB.m84 : 0.0f;
    if (rate < 0.5f) {
        m14C4 = 7;
        RoutePlace_unk* place = FindQuadPlace_unk(600);
        if (place != 0) {
            m14D4 = place->mPos;
            m14E0 = atn::Vector3(0.0f, place->m14, 0.0f);
            fn_80466B8C();
        } else {
            fn_80466A04();
        }
    }
}

atn::Vector3 LastCocoonQuadruped::vf64(int center) {
    atn::Vector3 result = mPos;
    result = m624.mData[0].m04.mCenter;
    return result;
}

// inferred: sets the segment and the radius of a capsule (name invented)
inline void SetCapsule_unk(atn::ColliCapsule& c, const atn::Vector3& a, const atn::Vector3& b, float r) {
    c.m3C = a;
    c.m48 = b;
    c.m54 = r;
}

// inferred: hit spheres on the head, waist and spine
void LastCocoonQuadruped::vf50() {
    Item14_unk item;
    item.m04.mRadius = 1.5f * m5B0;
    for (int i = 0; i < 3; i++) {
        item.m04.mCenter = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sSphereNodes_unk[i]))->GetTrans_unk();
        m624.push_back(item);
    }
    mFlagB29 = 0;
}

void LastCocoonQuadruped::vf54() {
    if (m624.mSize != 0) {
        for (int i = 0; i < 3; i++) {
            MtxPtr mtx = mMotion.GetNodeMtx_unk(sSphereNodes_unk[i]);
            m624.mData[i].m04.mCenter = ((atn::Matrix3x4*)mtx)->GetTrans_unk();
            if (i == 1) {
                m624.mData[i].m04.mRadius = 10.0f;
            } else if (i == 2) {
                m624.mData[i].m04.mCenter = (m624.mData[0].m04.mCenter + m624.mData[1].m04.mCenter) * 0.5f;
                m624.mData[i].m04.mRadius = 17.0f;
            }
        }
    }
}

void LastCocoonQuadruped::vf20() {
    m620 = 1.5f * m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    float radius = m5B0;
    atn::Vector3 a = m614;
    atn::Vector3 b = a;
    b.y += MAX(m5B4 - 2.0f * radius, 1.0f);
    SetCapsule_unk(mColli, a, b, radius);
    for (int i = 0; i < 5; i++) {
        float r = sQuadCapsules_unk[i].mRadius;
        a = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sQuadCapsules_unk[i].mFrom))->GetTrans_unk();
        b = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sQuadCapsules_unk[i].mTo))->GetTrans_unk();
        SetCapsule_unk(mCapsules[i], a, b, r);
    }
    a = mPos;
    b = a;
    b.y += 100.0f;
    SetCapsule_unk(m17A8, a, b, 50.0f);
}

void LastCocoonQuadruped::fn_804617A4() {
    if (m58C >= 11 && m58C <= 12) {
        mC58.mC8 = (int)mC58.mC4;
        m1454 = 0;
        m14B8 = m1554;
        mC58.mC4 = m1554;
    } else if (m58C == 13) {
        mC58.mC8 = (int)mC58.mC4;
        m1454 = 0;
        m14B8 = m1574;
        mC58.mC4 = m1574;
    } else if (m58C == 17) {
        mC58.mC8 = (int)mC58.mC4;
        m1454 = 0;
        m14B8 = m15C8;
        mC58.mC4 = m15C8;
    } else {
        m1454 = 1;
        m14B8 = mC58.mC4;
    }
}

// NONMATCHING: 99.5% - the frame time (dt) is in f28 here, f29 in the original. Tried: local
// declaration orders, a copy of dt after the motion lengths, separate weights.
// inferred: walk motions blended by speed (simplified CharaTask::vf24)
void LastCocoonQuadruped::vf24() {
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
    float dt = lbl_8087EFA8->m3A4;
    float walk = GetMotionLength_unk(&mMotion, m484[1]);
    float run = GetMotionLength_unk(&mMotion, m484[3]);
    float hi;
    float rate = m570 / dt;
    float lo = m500;
    hi = m508;
    float blend;
    float walkSpeed;
    float runSpeed;
    if (rate < lo) {
        blend = rate / lo;
        walkSpeed = 1.0f;
        runSpeed = 0.0f;
    } else if (rate < hi) {
        blend = 1.0f;
        runSpeed = 0.0f;
        walkSpeed = blend + (rate - lo) / (hi - lo);
    } else {
        blend = 2.0f;
        walkSpeed = 0.0f;
        runSpeed = 0.1f * (rate / hi - 1.0f) + 1.0f;
    }
    if (LengthSq_unk(atn::RenderManager::instance_->mCamera.mPos - mPos) > 62500.0f) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        if (blend < 0.5f) {
            mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = 1.0f;
        } else if (blend < 1.5f) {
            mMotion.fn_80097C08(0, m484[1], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = walkSpeed;
        } else {
            mMotion.fn_80097C08(0, m484[3], 1, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m0C = runSpeed;
        }
    } else {
        mMotion.m34C = 3;
        float w = Clamp01_unk(1.0f - blend);
        mMotion.fn_80097C08(0, m484[0], 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m20 = w;
        mMotion.mTracks[0].m0C = 1.0f;
        w = Clamp01_unk(1.0f - (float)__fabs(1.0f - blend));
        mMotion.fn_80097C08(1, m484[1], 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[1].m0C = walkSpeed;
        mMotion.mTracks[1].m20 = w;
        w = Clamp01_unk(blend - 1.0f);
        mMotion.fn_80097C08(2, m484[3], 1, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[2].m0C = runSpeed;
        mMotion.mTracks[2].m20 = w;
    }
}

// inferred: m = m * rotation(r) z, y, x (as in game/QuadrupedTask.cpp and game/Em403WarLion.cpp; name
// invented)
inline atn::Matrix3x4& QuadRotationZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
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
    return m;
}

// inferred: turns the neck with the lean, the waist and the spine with m15CC.y (node matrix callback)
// NONMATCHING: 99.2% - as QuadrupedTask::fn_805688CC (register of the node, stack of the matrices)
void LastCocoonQuadruped::fn_80462CCC(atn::Matrix3x4* mtx, const atn::NodeChunk& chunk) {
    ModelNodeDesc_unk* node = (ModelNodeDesc_unk*)&chunk;
    if (mLook.fn_8012A288(mtx, node) != 0) {
        return;
    }
    const char* name = node->mName;
    if (strcmp(name, "neck") == 0) {
        if (m648 == 0) {
            atn::Vector3 pos = mtx->GetTrans_unk();
            mtx->m[0][3] = 0.0f;
            mtx->m[1][3] = 0.0f;
            mtx->m[2][3] = 0.0f;
            Mtx r;
            PSMTXRotRad(r, 'z', m580 - 0.7f * m584);
            PSMTXConcat(r, mtx->m, mtx->m);
            mtx->m[0][3] = pos.x;
            mtx->m[1][3] = pos.y;
            mtx->m[2][3] = pos.z;
        }
    } else if (strcmp(name, "waist") == 0) {
        if (m55C != 8) {
            atn::Vector3 pos = mtx->GetTrans_unk();
            mtx->m[0][3] = 0.0f;
            mtx->m[1][3] = 0.0f;
            mtx->m[2][3] = 0.0f;
            atn::Vector3 rot(m15CC.y, 0.0f, 0.1f * m15CC.y);
            atn::Matrix3x4 r;
            QuadRotationZYX_unk(r, rot);
            PSMTXConcat(r.m, mtx->m, mtx->m);
            mtx->m[0][3] = pos.x;
            mtx->m[1][3] = pos.y;
            mtx->m[2][3] = pos.z;
        }
    } else if (strcmp(name, "spine") == 0) {
        if (m55C != 8) {
            atn::Vector3 pos = mtx->GetTrans_unk();
            mtx->m[0][3] = 0.0f;
            mtx->m[1][3] = 0.0f;
            mtx->m[2][3] = 0.0f;
            atn::Vector3 rot(0.0f, 0.0f, 0.3f * m15CC.y);
            atn::Matrix3x4 r;
            QuadRotationZYX_unk(r, rot);
            PSMTXConcat(r.m, mtx->m, mtx->m);
            mtx->m[0][3] = pos.x;
            mtx->m[1][3] = pos.y;
            mtx->m[2][3] = pos.z;
        }
    }
}

class GimmickManager;
class GimmickCommon;
extern "C" GimmickManager* fn_80279790();                // inferred: GimmickManager::instance_
extern "C" GimmickCommon* fn_80279788(GimmickManager* m); // inferred: head of its list
extern "C" atn::Vector3* fn_802797A0(GimmickCommon* g);   // inferred: position of the gimmick
extern "C" GimmickCommon* fn_802797D4(GimmickCommon* g);  // inferred: next gimmick of the list
extern "C" int fn_804635A4(GimmickCommon* g);
extern "C" CharaTask* fn_804635AC(GimmickCommon* g);
extern "C" float fn_802A7964(float rad); // inferred: radians to degrees
extern "C" float fn_802A7910(float deg); // inferred: wraps an angle in degrees
void* fn_801404F8();                     // inferred: collision world
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: line check

// NONMATCHING: the original is over the inlining budget (docs/inlining.md R3) and calls the Vector3 copies,
// operators and the angle helpers; this one inlines them
void LastCocoonQuadruped::fn_80463174() {
    if (m14B8 == 0) {
        return;
    }
    atn::Vector3 tpos(*m14B8->fn_8013C38C());
    atn::Vector3 diff = tpos - mPos;
    float dist = diff.Length_unk();
    if (m14C4 == 0) {
        if (m14C0 > 60 && dist < 35.0f) {
            fn_80466AE4();
        }
    } else if (m14C4 == 1) {
        if (m14EC == 0) {
            fn_80467F50();
            m14EC = 1;
            return;
        }
        int walk = 1;
        if (m14F0 <= 0) {
            int found = 0;
            atn::Vector3 pos;
            CharaTask* chara = 0;
            if (m1558 >= 0) {
                for (GimmickCommon* g = fn_80279788(fn_80279790()); g != 0; g = fn_802797D4(g)) {
                    if (fn_804635A4(g) == 0x455 && g->vf5C() != 0) {
                        pos = *fn_802797A0(g);
                        chara = fn_804635AC(g);
                        atn::Vector3 d = pos - mPos;
                        float len = d.Length_unk();
                        float angle = fn_80011220(
                            fn_802A7910(fn_802A7964(d.GetNormalize().ToRotation_unk().y - GetRot_unk()->y)));
                        if (angle <= 360.0f && len < 100.0f) {
                            found = 1;
                            break;
                        }
                    }
                }
            }
            if (found != 0) {
                m1558 = 0;
                m14C4 = 6;
                fn_80467C18(chara, atn::Vector3(pos));
                walk = 0;
            } else if (m14F0 < -2) {
                m1558++;
                m14C4 = 4;
                m1554 = m14B8;
                fn_80468B68(&m1504, m14F4, &m14F4, 1);
                fn_804672CC();
                walk = 0;
            }
        }
        if (walk != 0) {
            m14F0--;
            fn_80468B68(&m1504, m14F4, &m14F4, 0);
            fn_80466CA4();
        }
    } else if (m14C4 == 4) {
        if (m1554 == 0 || !m1554->CanAct_unk()) {
            m14C4 = 0;
            fn_80466974();
        }
        atn::Vector3 d = *m1554->fn_8013C38C() - mPos;
        float len = diff.Length_unk();
        float angle =
            fn_80011220(fn_802A7910(fn_802A7964(d.GetNormalize().ToRotation_unk().y - GetRot_unk()->y)));
        if (angle <= 45.0f && len < 100.0f) {
            atn::Vector3 from(mPos);
            from.y += 10.0f;
            atn::Vector3 to(tpos);
            to.y += 10.0f;
            if (fn_8004ECC0(fn_801404F8(), 0, &from, &to, 0x80000080, &mColli, 0) == 0) {
                m14C4 = 5;
                fn_804678F4();
            }
        }
    } else if (m14C4 == 5) {
        m14C4 = 0;
        m1574 = 0;
    } else if (m14C4 == 6) {
        m14C4 = 0;
    } else if (m14C4 == 7) {
        fn_80466A04();
    }
}

// inferred: id and owner of a gimmick (weak copies; C names)
extern "C" int fn_804635A4(GimmickCommon* g) {
    return g->mId;
}

extern "C" CharaTask* fn_804635AC(GimmickCommon* g) {
    return (CharaTask*)g->m4C;
}

void LastCocoonQuadruped::fn_804635B4() {
    if (m14C4 == 4 && (m55C < 6 || m55C > 7)) {
        fn_80170A20(m1554, -1.0f, 0);
    }
    fn_80461838();
}

void LastCocoonQuadruped::fn_80463610() {
    if (mMotion.mTracks[0].m00 == 0x14A) {
        if ((float)__fabs(mMotion.mTracks[0].m08 - 3.0f) < 1.1920929e-7f) {
            lbl_8087F430->Shake_unk(60, 0.3f, 0.3f);
        }
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
            lbl_8087F430->Shake_unk(180, 0.1f, 0.1f);
            fn_800C344C("SE_EM206_027", &mPos, 0, -1, 1.0f);
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if ((float)__fabs(frame - 50.0f) < 1.1920929e-7f) {
            ((GameTask*)lbl_8087F430)->fn_80370AE4(0x5B, 1);
        } else if (frame >= mMotion.fn_80097D7C(0)) {
            mFlag25 = 1;
            mColli.mFlags &= ~1;
            for (int i = 0; i < 5; i++) {
                mCapsules[i].mFlags &= ~1;
            }
            m17A8.mFlags &= ~1;
            mFlag15 = 1;
            fn_801765D8();
            for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
                if (enemy->m60->m24 == 0x3B) {
                    atn::Vector3 dir;
                    dir.x = 0.0f;
                    dir.y = 0.0f;
                    dir.z = 1.0f;
                    atn::Matrix3x4 rot;
                    rot.fn_8013A13C(mRot.y);
                    PSMTXMultVec(rot.m, &dir, &dir);
                    enemy->vf80(mPos, dir);
                }
            }
        }
    }
}

void LastCocoonQuadruped::fn_804638A4() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80466974();
    } else {
        if (mMotion.mTracks[0].m08 < 20.0f) {
            CharaTask* target = m14B8;
            atn::Vector3 d0 = target->mPos - mPos;
            atn::Vector3 rot = d0.GetNormalize().ToRotation_unk();
            float d = WrapDeg_unk(57.295776f * (rot.y - mRot.y));
            if ((float)__fabs(d) > atn::DegToRad_unk(2.0f)) {
                d = d / (float)__fabs(d) * atn::DegToRad_unk(2.0f);
            }
            mRot.y += d;
        }
        float f = mMotion.mTracks[0].m08;
        if (f >= 19.0f && f <= 26.0f) {
            fn_80144710();
            fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x5B4), m590, 30, -1, 0.0f);
        }
    }
}

void LastCocoonQuadruped::fn_80463C58() {
    if (mMotion.mTracks[0].m08 >= 5.0f) {
        SetMode_unk(9);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x140, 0, 0, 1, 1.0f, 0.1f);
        QuadCollResult_unk res;
        mPos = m14D4;
        atn::Vector3 offset(0.0f, -1000.0f, 0.0f);
        atn::Vector3 to = offset + m14D4;
        if (fn_8004ED34(lbl_8087EE98, &res, &m14D4, &to, 0x20000000, &mColli, 0) != 0) {
            mPos = res.m10;
        }
        mRot = m14E0;
        fn_80239DAC(lbl_8087F3C0, this, 0x65, 1);
    }
}

void LastCocoonQuadruped::fn_80463DCC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m14C4 == 1 || m14C4 == 7) {
            fn_80466974();
        } else {
            fn_80466AE4();
        }
    }
}

// NONMATCHING: 99.9% - stack slots of the two path positions and of the direction rotated by one
// inferred: walks along the route for 45 frames, hitting with the bullet 0x5B4
void LastCocoonQuadruped::fn_80463F20() {
    if (m14C0 >= 45) {
        SetMode_unk(0);
        if (m1800 != 0) {
            ((BattleTask*)lbl_8087F048)->fn_80107A68(&mMotion);
            m1800 = 0;
        }
        return;
    }
    mPos = m1504.GetPos_unk(1.0f / 45.0f * m14C0);
    mRot = (mPos - m1504.GetPos_unk(1.0f / 45.0f * (m14C0 - 1))).ToRotationZ_unk();
    fn_80144710();
    HitInfo_unk* bullet = fn_80219E6C(0x5B4);
    fn_800F8C6C(lbl_8087F048, 0, 0, this, bullet, m590, 30, -1, 0.0f);
    if (m18F4 != 0) {
        lbl_8087EEB0->fn_80063D3C(mPos, mStatus.mParamB.m28 + bullet->m40, 0.0f, 0xFFFF0000);
    }
}

// NONMATCHING: 99.9% - stack slots of the two path positions and of the direction rotated by one
// inferred: walks along the route for 33 frames, hitting with the bullet 0x5B4
void LastCocoonQuadruped::fn_80464608() {
    if (m14C0 >= 33) {
        SetMode_unk(0);
        if (m1800 != 0) {
            ((BattleTask*)lbl_8087F048)->fn_80107A68(&mMotion);
            m1800 = 0;
        }
        return;
    }
    mPos = m1504.GetPos_unk(1.0f / 45.0f * m14C0);
    mRot = (mPos - m1504.GetPos_unk(1.0f / 45.0f * (m14C0 - 1))).ToRotationZ_unk();
    fn_80144710();
    HitInfo_unk* bullet = fn_80219E6C(0x5B4);
    fn_800F8C6C(lbl_8087F048, 0, 0, this, bullet, m590, 30, -1, 0.0f);
    if (m18F4 != 0) {
        lbl_8087EEB0->fn_80063D3C(mPos, mStatus.mParamB.m28 + bullet->m40, 0.0f, 0xFFFF0000);
    }
}

extern "C" GameConfig_unk* lbl_8087F0A8; // inferred: game settings (m030: frames per second)

// NONMATCHING: 99.9% - stack slots of the position, rotation and color of PlayEfp_unk (reversed)
// inferred: mode 12: jumps towards m155C (m1568 per frame, with gravity), the bullet 0x5B5 at frame 24
void LastCocoonQuadruped::fn_80464CF0() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        if (m1574 != 0) {
            SetMode_unk(13);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
            m15A8 = 0;
            fn_80232B7C(this, 1000);
            PlayEfp_unk(&m179C, &mMotion);
        } else {
            SetMode_unk(0);
            if (m1800 != 0) {
                ((BattleTask*)lbl_8087F048)->fn_80107A68(&mMotion);
                m1800 = 0;
            }
        }
        return;
    }
    if (m14C0 < 24) {
        QuadCollResult_unk result;
        QuadCollResult_unk unused;
        atn::Vector3 move = m1568;
        move.y += -98.1f / (float)(lbl_8087F0A8->m030 * lbl_8087F0A8->m030);
        CharaTask::Sphere_unk sphere = fn_80176548(mPos);
        sphere.mRadius += 30.0f;
        sphere.mCenter.y += 30.0f;
        fn_8004D388(lbl_8087EE98, &result, &sphere.mCenter, &move, 0x80000080, &mColli, 0, sphere.mRadius);
        mPos = result.m10;
        mPos.x -= m5A4.x;
        mPos.y -= m5A4.y;
        mPos.z -= m5A4.z;
        mPos.y -= sphere.mRadius;
    } else if (m14C0 == 24) {
        fn_80144710();
        m1574 = 0;
        float rotY = mRot.y;
        int id = fn_800F8548(lbl_8087F048);
        fn_800F8C6C(lbl_8087F048, 0, 0, this, fn_80219E6C(0x5B5), id, 30, -1, rotY);
    } else {
        QuadCollResult_unk result;
        atn::Vector3 move(0.0f, -98.1f / (float)(lbl_8087F0A8->m030 * lbl_8087F0A8->m030), 1.0f);
        atn::Matrix3x4 m;
        QuadRotationZYX_unk(m, mRot);
        PSMTXMultVec(m.m, &move, &move);
        QuadCollResult_unk unused;
        CharaTask::Sphere_unk sphere = fn_80176548(mPos);
        sphere.mRadius += 30.0f;
        sphere.mCenter.y += 30.0f;
        fn_8004D388(lbl_8087EE98, &result, &sphere.mCenter, &move, 0x80000080, &mColli, 0, sphere.mRadius);
        mPos = result.m10;
        mPos.x -= m5A4.x;
        mPos.y -= m5A4.y;
        mPos.z -= m5A4.z;
        mPos.y -= sphere.mRadius;
    }
}

// NONMATCHING: 99.8% - the bitfield word of the damage record is loaded into r7 (r3 in the original),
// as in Em204Mulue::fn_802A91D0
void LastCocoonQuadruped::fn_80465294() {
    if (m1574 == 0 || m1574->m560 != 0x6E) {
        SetMode_unk(14);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x148, 0, 0, 1, 0.0f, 0.1f);
        if (m1574 != 0) {
            m1574->fn_8015B238();
            m1574 = 0;
        }
        fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
    } else if (m14C0 % 30 == 0) {
        float drain = 50.0f;
        QuadDamage_unk damage((int)drain);
        m1574->mStatus.fn_8012DF7C(damage.m04, 0, 0);
        fn_80108C10(lbl_8087F048, &damage, m1574->mPos + atn::Vector3(0.0f, 15.0f, 0.0f), this, m1574, 0, 0);
        damage.m04 /= -1;
        atn::Vector3 pos = fn_80148B0C(0)->m04.mCenter;
        pos += atn::Vector3(0.0f, 15.0f, 0.0f);
        mStatus.fn_8012DF7C(damage.m04, 0, 0);
        fn_80108C10(lbl_8087F048, &damage, pos, this, this, 0, 0);
    }
}

void LastCocoonQuadruped::fn_804654F0() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80466974();
    }
}

void LastCocoonQuadruped::fn_80466370() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80466974();
    } else {
        float f = mMotion.mTracks[0].m08;
        if (f <= 20.0f) {
            mPos += m15BC;
        } else if (20.0f < f && f < 24.0f && m14BC == 0) {
            HitInfo_unk* skill = fn_80219E6C(0x5B5);
            Effect_unk* mgr = lbl_8087F048;
            fn_800FAB80(mgr, this, skill, m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            m14BC = 1;
        }
        atn::Vector3 dir = m15BC;
        dir.y = 0.0f;
        dir.Normalize_unk();
        mRot = dir.ToRotation_unk();
    }
}

void LastCocoonQuadruped::fn_804666D8() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80466974();
    } else {
        float f = mMotion.mTracks[0].m08;
        if (f > 80.0f) {
            if (m1800 != 0) {
                ((BattleTask*)lbl_8087F048)->fn_80107A68(&mMotion);
                m1800 = 0;
            }
            mMotion.mTracks[0].m0C = 1.0f;
        } else if (f > 60.0f) {
            if (m1800 == 0) {
                ((BattleTask*)lbl_8087F048)->fn_801079C0(&mMotion, &mMotion);
                m1800 = 1;
            }
            mMotion.mTracks[0].m0C = 0.075f;
        } else if (!(f <= 10.0f) && 10.0f < f && f <= 55.0f) {
            mPos += m15BC;
        }
    }
}

void LastCocoonQuadruped::fn_80466868() {
    if ((float)__fabs(mMotion.mTracks[0].m08 - 3.0f) < 1.1920929e-7f) {
        lbl_8087F430->Shake_unk(60, 0.3f, 0.3f);
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetMode_unk(0);
        if (m1800 != 0) {
            ((BattleTask*)lbl_8087F048)->fn_80107A68(&mMotion);
            m1800 = 0;
        }
    }
}

void LastCocoonQuadruped::fn_80466974() {
    SetMode_unk(0);
    if (m1800 != 0) {
        ((BattleTask*)lbl_8087F048)->fn_80107A68(&mMotion);
        m1800 = 0;
    }
}

// NONMATCHING: 91.7% - "SE_EM206_001" is at @stringBase0+0xC9 (needs the earlier strings of the ctor)
void LastCocoonQuadruped::fn_80466A04() {
    SetMode_unk(6);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
    m17A8.mFlags |= 1;
    fn_800C344C("SE_EM206_001", &mPos, 0, -1, 1.0f);
}

void LastCocoonQuadruped::fn_80466AE4() {
    SetMode_unk(7);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.1f);
}

void LastCocoonQuadruped::fn_80466B8C() {
    SetMode_unk(8);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 0x65);
    PlayEfp_unk(&m1790, &mMotion);
}


// inferred: modes 10 and 11: walk along the route, facing its start
void LastCocoonQuadruped::fn_80466CA4() {
    SetMode_unk(10);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x21, 1, 0, 1, 0.0f, 0.1f);
    mRot = (m1504.GetPos_unk(1.0f / 45.0f) - m1504.GetPos_unk(0.0f)).ToRotationZ_unk();
}

void LastCocoonQuadruped::fn_804672CC() {
    SetMode_unk(11);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x21, 1, 0, 1, 0.0f, 0.1f);
    mRot = (m1504.GetPos_unk(1.0f / 45.0f) - m1504.GetPos_unk(0.0f)).ToRotationZ_unk();
}

void LastCocoonQuadruped::fn_804678F4() {
    SetMode_unk(12);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.1f);
    m155C = m1554->mPos;
    m1568 = m155C - mPos;
    m1568.y = 0.0f;
    m1568 *= 0.033333335f;
    mRot = m1568.GetNormalize().ToRotation_unk();
}

// NONMATCHING: 86% - the store of m15AC is scheduled after the step (one more saved register) and the
// subtractions are in another order. Tried: m15B0/m15AC order, pos instead of m15B0, a named
// difference, the scalar on the left.
void LastCocoonQuadruped::fn_80467C18(CharaTask* target, const atn::Vector3& pos) {
    SetMode_unk(15);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
    m15AC = target;
    m15B0 = pos;
    m15BC = (m15B0 - mPos) * 0.2f;
    m15BC.y = 4.6666665f;
    atn::Vector3 dir = m15BC;
    dir.y = 0.0f;
    dir.Normalize_unk();
    mRot = dir.ToRotation_unk();
}

void LastCocoonQuadruped::fn_80467F50() {
    SetMode_unk(19);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x14A, 0, 0, 1, 0.0f, 0.1f);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
    fn_800C344C("SE_EM206_001", &mPos, 0, -1, 1.0f);
}

extern "C" int fn_800FBA9C(Effect_unk* mgr, CharaTask* chara, CharaTask* target, HitInfo_unk* data, int arg,
                           float f); // inferred: the attack reaches the target
extern "C" int fn_800FBE70(Effect_unk* mgr, CharaTask* chara, CharaTask* target, HitInfo_unk* data, int arg,
                           atn::Vector3* dir, int a, int b); // inferred: the attack in `dir` reaches the target

// NONMATCHING: 97.7% - `m560 == 4 || m560 == 5` becomes a range test (two compares in the original), which
// shifts the registers r23-r31
// inferred: looks for a player attacking it; returns the place to move to (fn_80468534) and its angle
int LastCocoonQuadruped::fn_80468038(int* count, atn::Vector3* pos, float* angle) {
    if (++*count > 5) {
        return 1;
    }
    atn::Vector3 at;
    at.x = 0.0f;
    at.y = 0.0f;
    at.z = 0.0f;
    int found = 0;
    float yaw = 0.0f;
    float dist = 40.0f;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        if (!p->IsActive_unk() || p->vf78() == 0) {
            continue;
        }
        if (p->m560 == 4 || p->m560 == 5) {
            if (fn_800FBA9C(lbl_8087F048, p, this, p->m638, -1, 0.0f) >= 0) {
                at = p->mPos;
                found = 1;
                dist = 30.0f;
                yaw = p->mRot.y;
                break;
            }
        } else if (p->m560 == 30) {
            HitInfo_unk* data = p->m638;
            if (data != 0 && data->m01 == 1) {
                atn::Vector3 dir;
                dir = p->fn_80158E1C();
                PSVECNormalize(&dir, &dir);
                atn::Vector3 from = p->mPos;
                from.y += 20.0f;
                atn::Vector3 to = from + dir * 1000.0f;
                QuadCollResult_unk result;
                if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0, &p->mColli, 0) != 0 &&
                    (((atn::ColliBase*)result.m38)->mFlags & 2) == 2 &&
                    ((atn::ColliBase*)result.m38)->m0C == (int)this) {
                    at = mPos;
                    yaw = dir.ToRotationZ_unk().y;
                    found = 1;
                    dist = 40.0f;
                    break;
                }
            }
        } else if (p->m560 == 14) {
            atn::Vector3 dir = p->fn_80158E1C();
            if (fn_800FBE70(lbl_8087F048, p, this, p->m638, -1, &dir, 1, -1) >= 0) {
                at = dir;
                found = 1;
                yaw = p->mRot.y;
                break;
            }
        } else {
            at = p->mPos;
            found = 1;
            yaw = p->mRot.y;
            break;
        }
    }
    if (found != 0) {
        fn_80468534(pos, angle, at, yaw, dist);
        return 1;
    }
    return 0;
}


// NONMATCHING: 99.6% - r28/r29 of the rotated vector and of the direction swapped
void LastCocoonQuadruped::fn_80468534(atn::Vector3* pos, float* angle, const atn::Vector3& at, float base,
                                      float dist) {
    atn::Vector3 dir;
    dir.z = dist;
    QuadCollResult_unk result;
    atn::Vector3 from = at;
    dir.x = 0.0f;
    from.y += m620;
    dir.y = 0.0f;
    float best = 0.0f;
    float yaw = mRot.y;
    atn::Vector3 bestPos;
    for (int i = 0; i < 2; i++) {
        float a = base + 3.1415927f * i - 1.5707964f;
        atn::Matrix3x4 m;
        m.fn_8013A13C(a);
        atn::Vector3 v = dir;
        PSMTXMultVec(m.m, &v, &v);
        fn_8004D388(lbl_8087EE98, &result, &from, &v, 0x80000000, &mColli, 0, m620);
        v = result.m10;
        float sq = LengthSq_unk(v - from);
        if (sq > best) {
            yaw = atn::WrapAngle_unk(3.1415927f + a);
            best = sq;
            bestPos = v;
        }
    }
    *pos = bestPos;
    *angle = yaw;
}

// NONMATCHING: 99.8% - the two compares with the table are emitted as cmpw arg, entry (entry, arg in
// the original). Tried: both operand orders, != with continue, a reference to the entry, locals.
atn::Vector3 LastCocoonQuadruped::fn_804687B0(int* id, int kind8, int kind4) {
    atn::Vector3 result = lbl_807C7030;
    atn::Vector3 center = lbl_807C7030;
    center.x = 0.0f;
    center.y = 0.0f;
    center.z = 0.0f;
    int count = 0;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        if (p->IsActive_unk() || p->mType == 0) {
            count++;
            center += p->mPos;
        }
    }
    float inv = 1.0f / count;
    center *= inv;
    float maxDist = 0.0f;
    int found = 0;
    for (int i = 0; i < 10; i++) {
        const QuadPlace_unk& e = sQuadPlaces[i];
        if (e.m08 == kind8 && e.m04 == kind4) {
            RoutePlace_unk* place = FindQuadPlace_unk(e.mId);
            if (place != 0) {
                float dist = LengthSq_unk(place->mPos - center);
                if (dist > maxDist) {
                    maxDist = dist;
                    result = place->mPos;
                    found = place->mId;
                }
            }
        }
    }
    if (id != 0) {
        *id = found;
    }
    return result;
}

atn::Vector3 LastCocoonQuadruped::fn_80468A08(int id, int* next) {
    atn::Vector3 pos = lbl_807C7030;
    int nextId = 0;
    for (int i = 0; i < 10; i++) {
        if (id == sQuadPlaces[i].mId) {
            if (i == 9) {
                nextId = 601;
            } else {
                nextId = sQuadPlaces[i + 1].mId;
            }
        }
    }
    RoutePlace_unk* place = FindQuadPlace_unk(nextId);
    if (place != 0) {
        pos = place->mPos;
    }
    if (next != 0) {
        *next = nextId;
    }
    return pos;
}

// inferred: points of the path, a std::vector<atn::Vector3> handled through the weak copies of
// game/Em403WarLion.cpp (C names)
struct QuadPoints_unk {
    atn::Vector3* mData;
    unsigned int mSize;
    unsigned int mCap;
};
extern "C" void fn_8032AB90(QuadPoints_unk* v);                         // inferred: constructor
extern "C" QuadPoints_unk* fn_8032ABA4(QuadPoints_unk* v, int flag);    // inferred: destructor
extern "C" void fn_8032AC1C(QuadPoints_unk* v, const atn::Vector3* p);  // inferred: push_back
extern "C" void fn_8032AF90(QuadRoutes_unk* path, QuadPoints_unk* pts); // inferred: builds the path

// NONMATCHING: the original is over the inlining budget (docs/inlining.md R3) and calls the Vector3
// copies and RouteTable_unk::FindPlace_unk; the points are a std::vector<atn::Vector3> whose weak copies
// (C names) live in game/Em403WarLion.cpp, so here they are plain calls and the budget is not reached
int LastCocoonQuadruped::fn_80468B68(QuadRoutes_unk* path, int id, int* next, int last) {
    atn::Vector3 pos1 = lbl_807C7030;
    atn::Vector3 pos2 = lbl_807C7030;
    atn::Vector3 pos0 = lbl_807C7030;
    int id2 = 0;
    int id1 = 0;
    for (int i = 0; i < 10; i++) {
        if (id == sQuadPlaces[i].mId) {
            int j = i + 1;
            if (j == 10) {
                j = 0;
            }
            id1 = sQuadPlaces[j].mId;
            if (last != 0) {
                id2 = 600;
            } else {
                int k = j + 1;
                if (k == 10) {
                    k = 0;
                }
                id2 = sQuadPlaces[k].mId;
            }
            break;
        }
    }
    RoutePlace_unk* place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id);
    if (place != 0) {
        pos0 = place->mPos;
    }
    place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id1);
    if (place != 0) {
        pos1 = place->mPos;
    }
    place = fn_80121F00()->fn_8013C504()->FindPlace_unk(id2);
    if (place != 0) {
        pos2 = place->mPos;
    }
    QuadPoints_unk points;
    fn_8032AB90(&points);
    fn_8032AC1C(&points, &pos0);
    fn_8032AC1C(&points, &pos1);
    fn_8032AC1C(&points, &pos2);
    fn_8032AF90(path, &points);
    if (next != 0) {
        *next = id2;
    }
    fn_8032ABA4(&points, -1);
    return 1;
}

int LastCocoonQuadruped::vfE4() {
    int ret = 0;
    if (m58C == 16 && m14BC == 1) {
        ret = 1;
    }
    return ret;
}

float LastCocoonQuadruped::vf90() {
    return 3.0f;
}

LastCocoonQuadruped::~LastCocoonQuadruped() {}
