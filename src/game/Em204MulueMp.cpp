#include "game/Em204Mulue.h"
#include "atn/Csv.h"
#include "atn/ColliBase.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/Bullet.h"
#include "game/GameBgm.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" double sqrt(double x);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
// inferred: debug menu helper (as in game/CharaTask.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: attack hit
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, int a, int b, int c, float f1, float f2); // inferred: bullet
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet
extern "C" Bullet* fn_801010A0(Effect_unk* mgr); // inferred: a free bullet
// inferred: effect manager (class unknown), global instance at 0x8087F3C0 (two flags at 0xC4/0xC8 are
// set around the spin effect, as in game/Em209SmallGolem.cpp)
struct EffectMgr_unk {
    unsigned char m00[0xC4];
    int mC4;
    int mC8;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" int fn_800CB58C(atn::SoundRef* sound); // inferred: nonzero while playing
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
extern "C" void fn_80097CCC(atn::Model* model, int track, float value); // inferred: stops a track
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" void fn_8004D124(void* world); // inferred: rebuilds the active shapes
extern "C" void fn_805A5078(const atn::Vector3* pos, const atn::Vector3* dir, int id, float radius, float angle);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: field (lbl_8087F430); m10D8: route tables
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: player of the multiplayer session (0xD5C bytes, as in MP/D2AMpMenuCockpit.cpp)
struct MulueMpPlayer_unk {
    CharaTask* m00;
    unsigned char m04[0xD0 - 0x04];
    unsigned int mD0_hi : 1; // inferred: the player is in the game
    unsigned int mD0_lo : 31;
    unsigned char mD4[0xD5C - 0xD4];
};
// inferred: multiplayer session (lbl_8087F610); only the members used here
struct MpSession_unk {
    unsigned char m000[0x5E4];
    MulueMpPlayer_unk* m5E4; // inferred: players
    int m5E8;                // inferred: number of players

    MulueMpPlayer_unk* GetPlayer_unk(int i) {
        if (i >= 0 && i < m5E8) {
            return &m5E4[i];
        }
        return 0;
    }
};
extern "C" MpSession_unk* lbl_8087F610;
extern "C" void fn_804DA48C(MpSession_unk* session, int arg);
// inferred: shot of a sting (BulletShot_unk with defaults; name invented)
struct MulueMpShot_unk : public BulletShot_unk {
    MulueMpShot_unk() {
        m10 = 0;
        m14 = 0.0f;
        m18 = 1.0f;
    }
};

// inferred: helpers of game/Em204Mulue.cpp (angle wrap to [-180, 180] and radians to degrees), called
// out of line from fn_802B6F80
extern "C" inline float fn_802A7910(float deg) {
    float d = fmod(deg, 360.0);
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    return d;
}
extern "C" inline float fn_802A7964(float rad) {
    return 57.295776f * rad;
}

// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: colors of the materials per mode (normal, fire, ice; filled by the static initializer)
MulueMpMaterial_unk lbl_807860A0[6] = {
    {"em204_accessory", {MulueMpColor_unk(1.0f, 1.0f, 1.0f, 1.0f), MulueMpColor_unk(0.78f, 0.24f, 0.24f, 1.0f),
                         MulueMpColor_unk(0.39f, 0.59f, 1.0f, 1.0f)}},
    {"em204_skin", {MulueMpColor_unk(1.0f, 1.0f, 1.0f, 1.0f), MulueMpColor_unk(0.78f, 0.24f, 0.24f, 1.0f),
                    MulueMpColor_unk(0.39f, 0.59f, 1.0f, 1.0f)}},
    {"em204_armor", {MulueMpColor_unk(0.9f, 0.9f, 0.9f, 1.0f), MulueMpColor_unk(0.78f, 0.24f, 0.24f, 1.0f),
                     MulueMpColor_unk(0.39f, 0.59f, 1.0f, 1.0f)}},
    {"em204_hair", {MulueMpColor_unk(1.0f, 1.0f, 1.0f, 1.0f), MulueMpColor_unk(0.78f, 0.24f, 0.24f, 1.0f),
                    MulueMpColor_unk(0.39f, 0.59f, 1.0f, 1.0f)}},
    {"em204_eye", {MulueMpColor_unk(1.0f, 1.0f, 1.0f, 1.0f), MulueMpColor_unk(0.5f, 0.5f, 0.5f, 1.0f),
                   MulueMpColor_unk(0.5f, 0.5f, 0.5f, 1.0f)}},
    {"em204_koura", {MulueMpColor_unk(1.0f, 1.0f, 1.0f, 1.0f), MulueMpColor_unk(0.59f, 0.24f, 0.24f, 1.0f),
                     MulueMpColor_unk(0.39f, 0.59f, 1.0f, 1.0f)}},
};

// inferred: plays an effect on the model (as in game/Em302Kraken.cpp; names invented)
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

// inferred: the motion of track 0 reached its end (frame read before the length; name invented)
inline bool Em204MulueMp::IsMotionEnd_unk() {
    float frame = mMotion.mTracks[0].m08;
    return frame >= mMotion.fn_80097D7C(0);
}
// inferred: starts state `state` (names invented)
inline void Em204MulueMp::SetState_unk(int state) {
    m14E0 = 0;
    m58C = state;
    fn_8016E970(6);
    m560 = 4;
    m590 = fn_800F8548(lbl_8087F048);
}
// inferred: plays a motion on track 0 (as in game/Em204Mulue.cpp)
inline void Em204MulueMp::PlayMotion_unk(int motion, int loop, float speed) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = speed;
    mMotion.fn_80097C08(0, motion, loop, 0, 1, 0.0f, 0.1f);
}
inline void Em204MulueMp::SetPartsFlag_unk() {
    m1698.mFlags |= 1;
    m16F0.mFlags |= 1;
    m1748.mFlags |= 1;
    m17A0.mFlags |= 1;
    m17F8.mFlags |= 1;
}
inline void Em204MulueMp::ClearPartsFlag_unk() {
    m1698.mFlags &= ~1;
    m16F0.mFlags &= ~1;
    m1748.mFlags &= ~1;
    m17A0.mFlags &= ~1;
    m17F8.mFlags &= ~1;
}
inline void Em204MulueMp::StopEffects_unk() {
    fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x67, 0);
}
// inferred: back to state 6 (idle)
inline void Em204MulueMp::Rest_unk() {
    m58C = 6;
    fn_8016E970(3);
    StopEffects_unk();
    m151C = 1;
    mPos.y = 0.0f;
}
// inferred: death (state 17)
inline void Em204MulueMp::Die_unk() {
    m14E0 = 0;
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 17;
    PlayMotion_unk(0x2E, 0, 1.0f);
    StopEffects_unk();
    m151C = 1;
}
// inferred: changes the mode (the session is told about it)
inline void Em204MulueMp::SetMode_unk(int mode) {
    if (m1518 != mode && lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 3);
    }
}
// inferred: spin tackle toward the tackle points (states 13 and 14)
inline void Em204MulueMp::StartSpin_unk(int state) {
    m14E0 = 0;
    m14DC = 0;
    m58C = state;
    fn_8016E970(6);
    m560 = 4;
    PlayMotion_unk(0x140, 1, 1.0f);
    m151C = 0;
}

// inferred: turn toward `target` (frames and direction; as Em204Mulue::SetTurn_unk)
inline void Em204MulueMp::SetTurn_unk(atn::Vector3 target) {
    atn::Vector3 d = target - mPos;
    atn::Vector3 rot = d.GetNormalize().ToRotation_unk();
    float a = fn_802A7910(fn_802A7964(rot.y - mRot.y));
    if (a < 0.0f) {
        m1600 = 0;
        m15FC = -a / 2.0f;
    } else {
        m1600 = 1;
        m15FC = a / 2.0f;
    }
}

// NONMATCHING: 99.81% - registers of the second std::string constructor (as in game/Em204Mulue.cpp)
Em204MulueMp::Em204MulueMp(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14DC(0), m14E0(0), m14E4(1), m14E8(0), m14EC(0), m14F0(0), m14F4(0),
      m14F8(0.5f), m14FC(0), m1500(0), m1504(0), m1508(0), m150C(0), m1510(0), m1518(0), m1520(600), m15E0(1e16f),
      m15F8(0), m15FC(0.0f), m1600(1), m1604(0x4650), m1608(0), m1684(0), m185C(0) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m1524.LoadRequest("ef_sk002b.eff");
    m1530.LoadRequest("ef_sk008d.efp");
    m153C.LoadRequest("ef_uc011.efp");
    for (int i = 0; i < 6; i++) {
        m160C[i] = lbl_807860A0[i].mColor[0];
    }
    std::string dir("database/chara/special/");
    std::string name("db_em204_02_00.csv");
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
    m166C = 0;
    m1670 = 0;
    m1674 = 0;
    m1678 = 0;
    m167C = 0;
    m1680 = 0;
    m1684 = 0;
    m1688 = 0;
    m168C = 0;
    m1850.vf0C(path.c_str());
}

int Em204MulueMp::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1524.IsLoading() == 0 &&
        m1530.IsLoading() == 0 && m153C.IsLoading() == 0 && m1850.fn_80473F50() == 0) {
        mColli.mFlags &= ~0x80000000;
        m1698.mFlags |= 3;
        m1698.m0C = (int)this;
        m16F0.mFlags |= 3;
        m16F0.m0C = (int)this;
        m1748.mFlags |= 3;
        m1748.m0C = (int)this;
        m17A0.mFlags |= 3;
        m17A0.m0C = (int)this;
        m17F8.mFlags |= 3;
        m17F8.m0C = (int)this;
        fn_802B4B08();
        m154C.mSize = 0;
        if (m1850.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            atn::Csv csv((const char*)m1850.fn_80470580(), m1850.fn_8047059C());
            do {
                const char* key = csv.fn_8005B9CC();
                if (strcmp(key, "MoveTime") == 0) {
                    m1520 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "TacklePoint") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m154C.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "SpinPoint") == 0) {
                    int id;
                    do {
                        id = fn_800DC12C(csv.fn_8005B9CC());
                        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(id);
                        if (place != 0) {
                            m1590.push_back(place);
                        }
                    } while (id != 0);
                } else if (strcmp(key, "TackleID") == 0) {
                    m166C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BackID") == 0) {
                    m1670 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "BiteID") == 0) {
                    m1674 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "PressID") == 0) {
                    m1678 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "WaveID") == 0) {
                    m167C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "StingID") == 0) {
                    m1680 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SpinTackleID") == 0) {
                    m1684 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SpinIceID") == 0) {
                    m1688 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SpinIceWeakID") == 0) {
                    m168C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SpinFrareID") == 0) {
                    m1690 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "SpinFrareWeakID") == 0) {
                    m1694 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                } else if (strcmp(key, "MoveDistMax") == 0) {
                    m15E0 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "SpinTackleHpRate") == 0) {
                    m14F8 = fn_800DC288(csv.fn_8005B9CC());
                } else if (strcmp(key, "SpinTackleMax") == 0) {
                    m1500 = fn_800DC12C(csv.fn_8005B9CC());
                } else if (strcmp(key, "SpinTackleInterval") == 0) {
                    m1508 = fn_800DC12C(csv.fn_8005B9CC());
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC0;
        mStatus.m18 |= 0x1000000;
        mStatus.m18 |= 0x208;
        mStatus.m18 |= 0x2880000;
        mStatus.m18 |= 0x8414;
        m54C |= 0x200;
        if (m1590.size() != 0) {
            m15D4 = m1590.mData[0]->mPos;
        }
        void* group = m1858.Load("em204");
        fn_80087994(group, "DebugDisp", &m185C, 0, 0);
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

void Em204MulueMp::vf10() {
    int on = m151C;
    if (on != 0) {
        m1698.mFlags |= 1;
    } else {
        m1698.mFlags &= ~1;
    }
    if (on != 0) {
        m16F0.mFlags |= 1;
    } else {
        m16F0.mFlags &= ~1;
    }
    if (on != 0) {
        m1748.mFlags |= 1;
    } else {
        m1748.mFlags &= ~1;
    }
    if (on != 0) {
        m17A0.mFlags |= 1;
    } else {
        m17A0.mFlags &= ~1;
    }
    if (on != 0) {
        m17F8.mFlags |= 1;
    } else {
        m17F8.mFlags &= ~1;
    }
    if (!CharaTask::mFlag2) {
        if (CanAct_unk() && m58C != 6 && !vfF8()) {
            vf100(6);
        }
        vf104();
    } else if (m58C == 17) {
        vfEC();
    }
    fn_8014C540();
    fn_80145334();
    fn_802B4B08();
    mStatus.mParamB.m54[0] = 0.0f;
    mStatus.mParamB.m54[1] = 0.0f;
    mStatus.mParamB.m54[2] = 0.0f;
    mStatus.mParamB.m54[3] = 0.0f;
    mStatus.mParamB.m54[4] = 0.0f;
    mStatus.mParamB.m54[5] = 0.0f;
    mStatus.mParamB.m54[6] = 0.0f;
    if (m1518 == 1) {
        mStatus.mParamB.m54[0] = 4.0f;
        mStatus.mParamB.m54[1] = -4.0f;
    } else if (m1518 == 2) {
        mStatus.mParamB.m54[0] = -4.0f;
        mStatus.mParamB.m54[1] = 4.0f;
    }
    if (mMotion.mTracks[0].m00 == 0x140) {
        if (fn_800CB58C(&m1548) == 0) {
            m1548 = fn_800C344C("SE_EM204_003", &mPos, 0, -1, 1.0f);
        } else {
            fn_800CB6E4(&m1548, &mPos);
        }
    } else if (fn_800CB58C(&m1548) != 0) {
        fn_800CB5C8(&m1548, 0, 0);
    }
}

inline MulueMpColor_unk Lerp_unk(const MulueMpColor_unk& a, const MulueMpColor_unk& b, float t) {
    return (b - a) * t + a;
}

void Em204MulueMp::vf14() {
    for (int i = 0; i < 6; i++) {
        if (mMotion.fn_80092954(lbl_807860A0[i].mName) != 0) {
            MulueMpColor_unk target = lbl_807860A0[i].mColor[0];
            if (m1518 == 1) {
                target = lbl_807860A0[i].mColor[1];
            } else if (m1518 == 2) {
                target = lbl_807860A0[i].mColor[2];
            }
            m160C[i] = Lerp_unk(m160C[i], target, 0.1f);
            mMotion.fn_80092954(lbl_807860A0[i].mName)->mColor = m160C[i].ToGX();
        }
    }
    fn_80149A30();
}

void Em204MulueMp::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m10 = lbl_807C7030;
    msg->m50 |= 0x10;
    fn_80151448(msg);
    if (!CharaTask::mFlag2) {
        vf40(hit);
    } else {
        vf118(hit);
    }
}

void Em204MulueMp::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->m68 != 0) {
        m14EC += msg->m68;
        if (m14EC >= mStatus.mParamB.m84 / 2 && m14E4 == 1) {
            m14E4 = 3;
            m14E8 = 0;
        }
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        Die_unk();
    }
}

void Em204MulueMp::vfFC() {
    m58C = 6;
    fn_8016E970(3);
    StopEffects_unk();
    m151C = 1;
    mPos.y = 0.0f;
    atn::Vector3 d = mPos - m15D4;
    if ((float)sqrt(d.x * d.x + d.z * d.z) > 130.0f) {
        mPos = m15D4;
    }
}

// NONMATCHING: 99.90% - stack slots of the locals of case 10 and of the default case
void Em204MulueMp::vf104() {
    if (mC58.mC4 == 0) {
        mC58.mC4 = lbl_8087F8A0->mHead;
    }
    if (m58C != 16 && m58C != 7) {
        m14D4 = mC58.mC4;
    }
    float rate;
    if (mStatus.mParamB.m84 > 0) {
        rate = mStatus.m04 / mStatus.mParamB.m84;
    } else {
        rate = 0.0f;
    }
    if (rate < 0.4f && m1608 == 0) {
        m1608 = 1;
        if (GameBgm::sInstance != 0) {
            GameBgm::sInstance->fn_8037D4C0(0x8B8, 15, 1, 0, 1.0f);
        }
    }
    m14E0++;
    if (mC58.mC0 == 0 || m14D4 == 0) {
        fn_8016E970(3);
        fn_80097CCC(&mMotion, 0, 0.0f);
        PlayMotion_unk(2, 1, 1.0f);
        Rest_unk();
    } else if (m14D4 != 0) {
        switch (m58C) {
        case 7:
            if (fn_80158BB4()) {
                m151C = 1;
                fn_8004D124(lbl_8087EE98);
                Rest_unk();
            }
            if (mMotion.mTracks[0].m08 >= 23.0f && mMotion.mTracks[0].m08 <= 27.0f) {
                fn_800F8C6C(lbl_8087F048, 0, 0, this, m1674, m590, 30, -1, 0.0f);
            }
            break;
        case 8:
            fn_802B52D8();
            break;
        case 9: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                switch (m1514) {
                case 0:
                    fn_802B6AF8(2);
                    break;
                case 2:
                    StartSpin_unk(13);
                    SetMode_unk(1);
                    m1518 = 1;
                    m150C = 0;
                    m14F4 = -1;
                    m15E4 = m15D4;
                    break;
                case 3:
                    StartSpin_unk(12);
                    SetMode_unk(2);
                    m1518 = 2;
                    break;
                case 4:
                    StartSpin_unk(14);
                    SetMode_unk(1);
                    m1518 = 1;
                    m150C = 0;
                    m14F4 = -1;
                    m15E4 = m15D4;
                    break;
                case 5:
                    StartSpin_unk(14);
                    SetMode_unk(2);
                    m1518 = 2;
                    m150C = 0;
                    m14F4 = -1;
                    m15E4 = m15D4;
                    break;
                default: {
                    m14E0 = 0;
                    m58C = 11;
                    fn_8016E970(6);
                    m560 = 4;
                    PlayMotion_unk(0x140, 1, 1.0f);
                    lbl_8087F3C0->mC4 = 1;
                    lbl_8087F3C0->mC8 = 1;
                    fn_80232B7C(this, 0x66);
                    {
                        Color4f_unk color;
                        atn::Vector3 rot;
                        atn::Vector3 pos;
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
                        fn_8023A8B4(lbl_8087F3C0, &m1530, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
                    }
                    lbl_8087F3C0->mC4 = 0;
                    lbl_8087F3C0->mC8 = 0;
                    m151C = 0;
                    m1518 = 0;
                    break;
                }
                }
            }
            break;
        }
        case 11:
            if ((float)m14E0 > 35.0f) {
                fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
                SetState_unk(15);
                PlayMotion_unk(0x141, 0, 1.0f);
            } else if ((float)m14E0 <= 35.0f) {
                fn_802B55B0(m14E0 % 10 == 0, 0);
            }
            break;
        case 12:
            fn_802B596C();
            break;
        case 13:
            fn_802B5DA4();
            break;
        case 14:
            fn_802B6104();
            break;
        case 15: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                Rest_unk();
            }
            break;
        }
        case 16:
            if (m15FC < 0.0f) {
                Rest_unk();
            } else {
                mRot.y += m1600 != 0 ? 0.034906585f : -0.034906585f;
                m15FC -= 1.0f;
            }
            break;
        case 10: {
            atn::Vector3 d = m15D4 - mPos;
            d.Length_unk();
            if (PSVECSquareMag(&d) < 100.0f) {
                if ((float)m14E0 > 35.0f) {
                    fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
                    SetState_unk(15);
                    PlayMotion_unk(0x141, 0, 1.0f);
                    break;
                }
            } else {
                atn::Vector3 n = d.GetNormalize();
                n.y = 0.0f;
                mPos += n * 10.0f;
                atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
                Effect_unk* mgr = lbl_8087F048;
                fn_800FAB80(mgr, this, fn_80219E6C(0x4B5), 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            }
            if ((float)m14E0 <= 35.0f) {
                fn_802B55B0(m14E0 % 10 == 0, 0);
            }
            break;
        }
        case 17:
            vfEC();
            break;
        default: {
            if (m55C < 6 || m55C > 7) {
                fn_8016E970(3);
                fn_80170A20(m14D4, 0, -1.0f);
            }
            fn_802B43C8();
            atn::Vector3 pos;
            pos = fn_801781B0();
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &dir, &dir);
            fn_805A5078(&pos, &dir, 100, 1.2f * m5B0, 3.1415927f);
            fn_802B475C();
            break;
        }
        }
    }
    m15F8++;
}

void Em204MulueMp::vf100(int state) {}

void Em204MulueMp::vf108() {}

void Em204MulueMp::fn_802B43C8() {
    float dist = 0.0f;
    float scale = 1.0f;
    atn::Vector3 rot(mRot);
    if (m55C == 7) {
        atn::Vector3 d = m14D4->mPos - mPos;
        dist = d.Length_unk();
        if (dist < 0.0001f) {
            rot = mRot;
        } else {
            rot = d.GetNormalize().ToRotation_unk();
        }
        scale = 0.5f;
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    ClearPartsFlag_unk();
    fn_8004D124(lbl_8087EE98);
    mFlag30 = 1;
    fn_8013CB68(rot, 1, dist, m568 * scale);
    SetPartsFlag_unk();
    fn_8004D124(lbl_8087EE98);
}

void Em204MulueMp::fn_802B475C() {
    if (m14D4 == 0) {
        return;
    }
    atn::Vector3 home = mPos - m15D4;
    if (home.Length_unk() >= m15E0) {
        m1510 = 3;
        m15F8 = 0;
        fn_802B6F80();
        return;
    }
    atn::Vector3 target(m14D4->mPos);
    atn::Vector3 d = target - mPos;
    d.Length_unk();
    atn::Vector3 rot = d.GetNormalize().ToRotation_unk();
    fn_802A7910(fn_802A7964(rot.y - mRot.y));
    if (m15F8 < m1520) {
        switch (m14E8 % 5) {
        case 0:
        case 3:
            m1510 = 2;
            break;
        case 1:
        case 2:
        case 4:
            m1510 = 1;
            break;
        }
    } else {
        m1510 = 3;
        m15F8 = 0;
    }
    fn_802B6F80();
}

// NONMATCHING: 98.41% - float registers of the first capsule and r6/r7 of the node copies (__VecCopy family,
// docs/mwcc-blockers.md)
void Em204MulueMp::fn_802B4B08() {
    atn::Vector3 a;
    atn::Vector3 b;
    float r0 = m5B0;
    m614 = mPos + m5A4;
    m620 = r0;
    m614.y += r0;
    a = m614;
    b = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    b.y -= 1.0f;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = m620;
    {
        float r = 7.0f;
        a = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftforearm"));
        b = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftmiddle1"));
        m1698.m3C = a;
        m1698.m48 = b;
        m1698.m54 = r;
    }
    {
        float r = 7.0f;
        a = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightforearm"));
        b = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightmiddle1"));
        m16F0.m3C = a;
        m16F0.m48 = b;
        m16F0.m54 = r;
    }
    {
        float r = 5.0f;
        a = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftleg"));
        b = GetNodePos_unk(mMotion.GetNodeMtx_unk("leftfoot"));
        m1748.m3C = a;
        m1748.m48 = b;
        m1748.m54 = r;
    }
    {
        float r = 5.0f;
        a = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightleg"));
        b = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightfoot"));
        m17A0.m3C = a;
        m17A0.m48 = b;
        m17A0.m54 = r;
    }
    {
        float r = 9.0f;
        a = GetNodePos_unk(mMotion.GetNodeMtx_unk("joint2"));
        b = GetNodePos_unk(mMotion.GetNodeMtx_unk("joint4"));
        m17F8.m3C = a;
        m17F8.m48 = b;
        m17F8.m54 = r;
    }
}

// NONMATCHING: 98.93% - r3/r4 of the copy of the center (__VecCopy of a temporary, docs/mwcc-blockers.md)
void Em204MulueMp::vf54() {
    if (m624.mSize != 0) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
        m624.mData[0].m04.mRadius = 1.5f * m5B0;
        m624.mData[0].m04.mCenter = pos + m5A4;
        m624.mData[0].m04.mCenter.y = mPos.y + m624.mData[0].m04.mRadius;
    }
}

atn::Vector3 Em204MulueMp::vf64(int center) {
    atn::Vector3 r(mPos);
    if (center == 0) {
        r = GetNodePos_unk(mMotion.GetNodeMtx_unk("spine1"));
        r.y = mPos.y;
    } else {
        r = mPos;
    }
    return r;
}

// inferred: moves back along the yaw (name invented; its locals come first as those of an inline)
inline void Em204MulueMp::MoveBack_unk() {
    Mtx m;
    float y = mRot.y;
    PSMTXRotRad(m, 'y', y);
    atn::Vector3 back(0.0f, 0.0f, -0.25f);
    atn::Vector3 v(back);
    PSMTXMultVec(m, &v, &v);
    mPos += v;
}

void Em204MulueMp::fn_802B52D8() {
    if (m14DC == 0) {
        if (m14E0 > 60) {
            PlayMotion_unk(3, 1, 1.0f);
            m14DC = 1;
        } else {
            MoveBack_unk();
        }
    } else if (m14DC == 1) {
        if (m14E0 > 10) {
            PlayMotion_unk(0x145, 0, 0.5f);
            m14DC = 2;
        }
    } else if (m14DC == 2) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
        if (fn_80158BB4()) {
            Rest_unk();
        }
        if (mMotion.mTracks[0].m08 >= 17.0f && mMotion.mTracks[0].m08 <= 19.0f) {
            fn_800FAB80(lbl_8087F048, this, m1678, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        if (mMotion.mTracks[0].m08 >= 20.0f && mMotion.mTracks[0].m08 <= 22.0f) {
            fn_800FAB80(lbl_8087F048, this, m167C, m590, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
    }
}

// inferred: identity rotated by z, y and x (as QueenRotZYX_unk of game/Em208Queen.cpp; name invented)
inline void MulueMpRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    if (rot.z != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.y != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.x != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
}

// NONMATCHING: 98.72% - allocation of the saved registers (r23-r31); 7 variants tried
void Em204MulueMp::fn_802B55B0(int aimed, int arg) {
    if (m1680 == 0) {
        return;
    }
    atn::Matrix3x4 m;
    MulueMpRotZYX_unk(m, mRot);
    atn::Model* model = &mMotion;
    atn::Vector3 hips = GetNodePos_unk(model->GetNodeMtx_unk("hips"));
    if (aimed && lbl_8087F610 != 0) {
        fn_800C344C("SE_EM204_014", &mPos, 0, -1, 1.0f);
        int n = lbl_8087F610->m5E8;
        for (int i = 0; i < n; i++) {
            MulueMpPlayer_unk* player = lbl_8087F610->GetPlayer_unk(i);
            if (player != 0 && player->mD0_hi == 1 && player->m00 != 0) {
                atn::Vector3 pos = GetNodePos_unk(player->m00->mMotion.GetNodeMtx_unk("hips"));
                atn::Vector3 to = (pos - hips).GetNormalize();
                Bullet* bullet = fn_801010A0(lbl_8087F048);
                if (bullet == 0) {
                    return;
                }
                MulueMpShot_unk shot;
                shot.m00 = 2.0f;
                shot.m04 = (int)&m1524;
                shot.m08 = 0;
                shot.m0C = m1680;
                bullet->fn_8010EE78(&shot, &hips, &to, this, -1, 0, -1.0f);
            }
        }
    }
}

// NONMATCHING: 99.93% - r4/r5 of the copy of the home (__VecCopy family)
void Em204MulueMp::fn_802B596C() {
    atn::Vector3 home(m15D4);
    home.y += 40.0f;
    atn::Vector3 d = home - mPos;
    d.Length_unk();
    if (m14DC != 0 || PSVECSquareMag(&d) < 100.0f) {
        if (m14DC == 0) {
            if (m14E0 > 60) {
                m14E0 = 0;
                m14DC = 1;
            }
        } else if ((float)m14E0 > 35.0f) {
            fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
            SetState_unk(15);
            PlayMotion_unk(0x141, 0, 1.0f);
        } else if (m14E0 > 10) {
            mPos.y -= 2.0f;
            if (mPos.y < 0.0f) {
                mPos.y = 0.0f;
            }
        } else {
            atn::Model* model = &mMotion;
            atn::Vector3 pos = GetNodePos_unk(model->GetNodeMtx_unk("hips"));
            if (m14E0 == 10) {
                atn::Vector3 dir(0.0f, -1.0f, 0.000001f);
                fn_800F8574(lbl_8087F048, this, m1688, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
            } else {
                atn::Vector3 dir(0.0f, -1.0f, 2.0f);
                Mtx m1;
                PSMTXRotRad(m1, 'y', 0.62831855f * m14E0);
                PSMTXMultVec(m1, &dir, &dir);
                Mtx m2;
                PSMTXRotRad(m2, 'x', 0.09424778f * (rand() / 32767.0f) + -0.04712389f);
                PSMTXMultVec(m2, &dir, &dir);
                PSVECNormalize(&dir, &dir);
                fn_800F8574(lbl_8087F048, this, m168C, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
            }
        }
    } else {
        atn::Vector3 n = d.GetNormalize();
        mPos += n * 10.0f;
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, fn_80219E6C(0x4B5), 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
}

// NONMATCHING: 99.91% - r3/r4 of the copy of the home (__VecCopy family)
void Em204MulueMp::fn_802B5DA4() {
    atn::Vector3 d = m15E4 - mPos;
    d.Length_unk();
    if (PSVECSquareMag(&d) < 25.0f) {
        int done = 0;
        if (m14DC == 0) {
            if (m14E0 > 60) {
                int n = m154C.mSize;
                m14E0 = 0;
                m14DC = 1;
                if (n >= 1) {
                    int i = rand() % (unsigned int)n;
                    m14F0 = i;
                    m14F4 = i;
                    m15E4 = m154C.mData[i]->mPos;
                } else {
                    done = 1;
                }
            }
        } else if (m14F4 == -1) {
            if (++m150C >= 2) {
                done = 1;
            } else {
                int i = rand() % m154C.size();
                m14F0 = i;
                m14F4 = i;
                m15E4 = m154C.mData[i]->mPos;
            }
        } else {
            m14F4++;
            if ((int)m154C.size() <= m14F4) {
                m14F4 = 0;
            }
            if (m14F4 == m14F0) {
                m14F4 = -1;
            }
            if (m14F4 == -1) {
                m15E4 = m15D4;
            } else {
                m15E4 = m154C.mData[m14F4]->mPos;
            }
        }
        if (done) {
            fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
            SetState_unk(15);
            PlayMotion_unk(0x141, 0, 1.0f);
        }
    } else {
        atn::Vector3 n = d.GetNormalize();
        n.y = 0.0f;
        mPos += n * 10.0f;
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
        fn_800FAB80(lbl_8087F048, this, m1684, 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
}

// NONMATCHING: 99.83% - r4/r5 of the copies of the distance (__VecCopy family)
void Em204MulueMp::fn_802B6104() {
    atn::Vector3 d(0.0f, 0.0f, 0.0f);
    if (m14DC < 2) {
        d = m15E4 - mPos;
    } else {
        atn::Vector3 home(m15D4);
        home.y += 40.0f;
        d = home - mPos;
    }
    d.Length_unk();
    if (m14DC == 3 || PSVECSquareMag(&d) < 25.0f) {
        int done = 0;
        if (m14DC == 0) {
            if (m14E0 > 60) {
                int n = m154C.mSize;
                m14E0 = 0;
                m14DC = 1;
                if (n >= 1) {
                    int i = rand() % (unsigned int)n;
                    m14F0 = i;
                    m14F4 = i;
                    m15E4 = m154C.mData[i]->mPos;
                }
            }
        } else if (m14DC == 1) {
            if (m14F4 == -1) {
                if (++m150C >= 2) {
                    done = 1;
                } else {
                    int i = rand() % m154C.size();
                    m14F0 = i;
                    m14F4 = i;
                    m15E4 = m154C.mData[i]->mPos;
                }
            } else {
                m14F4++;
                if ((int)m154C.size() <= m14F4) {
                    m14F4 = 0;
                }
                if (m14F4 == m14F0) {
                    m14F4 = -1;
                }
                if (m14F4 == -1) {
                    m15E4 = m15D4;
                } else {
                    m15E4 = m154C.mData[m14F4]->mPos;
                }
            }
            if (done) {
                m14E0 = 0;
                m14DC = 2;
            }
        } else if (m14DC == 2) {
            if (m14E0 > 30) {
                m14E0 = 0;
                m14DC = 3;
            }
        } else if (m14DC == 3) {
            if ((float)m14E0 > 35.0f) {
                fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
                SetState_unk(15);
                PlayMotion_unk(0x141, 0, 1.0f);
            } else if (m14E0 > 10) {
                mPos.y -= 2.0f;
                if (mPos.y < 0.0f) {
                    mPos.y = 0.0f;
                }
            } else {
                atn::Model* model = &mMotion;
                atn::Vector3 pos = GetNodePos_unk(model->GetNodeMtx_unk("hips"));
                if (m14E0 == 10) {
                    atn::Vector3 dir(0.0f, -1.0f, 0.000001f);
                    fn_800F8574(lbl_8087F048, this, m1518 == 1 ? m1690 : m1688, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
                } else {
                    atn::Vector3 dir(0.0f, -1.0f, 2.0f);
                    Mtx m1;
                    PSMTXRotRad(m1, 'y', 0.62831855f * m14E0);
                    PSMTXMultVec(m1, &dir, &dir);
                    Mtx m2;
                    PSMTXRotRad(m2, 'x', 0.09424778f * (rand() / 32767.0f) + -0.04712389f);
                    PSMTXMultVec(m2, &dir, &dir);
                    PSVECNormalize(&dir, &dir);
                    fn_800F8574(lbl_8087F048, this, m1518 == 1 ? m1694 : m168C, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
                }
            }
        }
    } else {
        atn::Vector3 n = d.GetNormalize();
        mPos += n * 10.0f;
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("hips"));
        fn_800FAB80(lbl_8087F048, this, m1684, 1000, &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    }
}

void Em204MulueMp::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        m151C = 0;
        if (m1434 > 0) {
            if (--m1434 == 30) {
                fn_80232B7C(0, 0);
                PlayEffect_unk(&m153C, &mMotion);
                vf9C(1);
            }
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        m1434 = 55;
        if (mMotion.mTracks[0].m00 == 46 && mMotion.mTracks[0].m08 < 70.0f) {
            atn::Vector3 v(0.0f, 0.0f, -35.0f / mMotion.fn_80097D7C(0));
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &v, &v);
            mPos += v;
        }
    }
}

void Em204MulueMp::fn_802B6928() {
    SetState_unk(7);
    PlayMotion_unk(0x156, 0, 0.75f);
    fn_8004D124(lbl_8087EE98);
}

void Em204MulueMp::fn_802B69C0() {
    SetState_unk(8);
    PlayMotion_unk(0x1C7, 1, 0.5f);
    m14DC = 0;
}

void Em204MulueMp::fn_802B6A5C(int kind) {
    SetState_unk(9);
    PlayMotion_unk(0x13F, 0, 1.0f);
    m1514 = kind;
}

void Em204MulueMp::fn_802B6AF8(int arg) {
    m14E0 = 0;
    m58C = 10;
    fn_8016E970(6);
    m560 = 4;
    PlayMotion_unk(0x140, 1, 1.0f);
    lbl_8087F3C0->mC4 = 1;
    lbl_8087F3C0->mC8 = 1;
    fn_80232B7C(this, 0x66);
    {
        Color4f_unk color;
        atn::Vector3 rot;
        atn::Vector3 pos;
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
        EffectMgr_unk* mgr = lbl_8087F3C0;
        fn_8023A8B4(mgr, &m1530, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
    }
    lbl_8087F3C0->mC4 = 0;
    lbl_8087F3C0->mC8 = 0;
    m151C = 0;
    int i = rand() % m1590.size();
    m15E4 = m1590.mData[i]->mPos;
    m15F4 = 0;
    m15F0 = arg;
    m1518 = 0;
}

void Em204MulueMp::fn_802B6C64() {
    SetState_unk(16);
    SetTurn_unk(m14D4->mPos);
}

void Em204MulueMp::fn_802B6F80() {
    atn::Vector3 target(*m14D4->fn_8013C38C());
    atn::Vector3 d = target - mPos;
    float dist = d.Length_unk();
    float angle = fn_802A7910(fn_802A7964(d.GetNormalize().ToRotation_unk().y - GetRot_unk()->y));
    int front = 0;
    if (angle <= 22.5f && angle >= -22.5f) {
        front = 1;
    }
    switch (m1510) {
    case 1:
        if (dist < 40.0f) {
            if (front) {
                fn_802B6928();
                m14E8++;
            } else {
                fn_802B6C64();
            }
        }
        break;
    case 2:
        if (dist < 70.0f) {
            if (front) {
                fn_802B69C0();
                m14E8++;
            } else {
                fn_802B6C64();
            }
        }
        break;
    case 4:
        fn_802B6A5C(1);
        break;
    case 3:
        if (++m1504 < 3) {
            fn_802B6A5C(0);
        } else {
            m14FC++;
            if (mStatus.m04 / (float)mStatus.mParamB.m84 < 0.4f) {
                if (rand() % 2 == 0) {
                    fn_802B6A5C(4);
                } else {
                    fn_802B6A5C(5);
                }
            } else {
                switch (m1518) {
                case 0:
                    fn_802B6A5C(2);
                    m14FC = 0;
                    break;
                case 1:
                    if (m14FC < 3) {
                        fn_802B6A5C(2);
                    } else {
                        fn_802B6A5C(3);
                        m14FC = 0;
                    }
                    break;
                case 2:
                    if (m14FC < 3) {
                        fn_802B6A5C(3);
                    } else {
                        fn_802B6A5C(2);
                        m14FC = 0;
                    }
                    break;
                }
            }
        }
        break;
    }
}

void Em204MulueMp::vf110(SpecialState_unk* state) {
    MulueMpState_unk* s = (MulueMpState_unk*)state;
    s->m00 = m58C;
    s->m04 = m14BC;
    s->m08 = m1518;
    s->m0A = m151C;
    s->m0C = m1608;
}

void Em204MulueMp::vf114(const SpecialState_unk* state) {
    const MulueMpState_unk* s = (const MulueMpState_unk*)state;
    int mode = m1518;
    if (s->m00 == 17 && m58C != 17) {
        Die_unk();
    }
    if (m1608 == 0 && s->m0C != 0 && GameBgm::sInstance != 0) {
        GameBgm::sInstance->fn_8037D4C0(0x8B8, 15, 1, 0, 1.0f);
    }
    m58C = s->m00;
    m14BC = s->m04;
    m1518 = s->m08;
    m151C = s->m0A;
    m1608 = s->m0C;
    if (mode != s->m08 && lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 3);
    }
}

Em204MulueMp::~Em204MulueMp() {}
