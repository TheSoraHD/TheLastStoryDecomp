// AI manager (AIManager, lbl_8087EE68) and its targets (AiTarget).
// Only part of the unit is written: the large functions (vf14 0x80014A5C, the route search
// 0x80018D5C-0x8001A228) are still missing, so the unit is not linked. The unit ends with the weak
// copies of std::runtime_error, boost::bad_function_call and the std::tr1::shared_ptr deleter
// (include/std/stdexcept.h; formerly the split std/runtime_error.cpp).
#include <boost/function.hpp>
#include <typeinfo>
#include "game/AiTarget.h"
#include "atn/Memory.h"
#include "game/BattleTask.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/GameScene.h"
#include "game/GameTask.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "game/MapTask.h"
#include "game/PlayerTask.h"
#include "game/SkillEffectManager.h"
#include "game/TeamAIManager.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4); // AddBool
extern "C" void fn_800874C8(void* menu, const char* name, int* value, int min, int max, int step, int func,
                            int arg); // AddInt
extern "C" int fn_8021A984(HitInfo_unk* hit); // inferred: the skill is offensive (as in AI/AiCommon_unk.cpp)
extern "C" void fn_8001C1B4(); // inferred: AI/AI_archer_attack01.cpp (empty)
extern "C" void fn_8001C1B8();

// inferred: render parameters (as FrameTime_unk in game/GimmickStateCommand.cpp)
struct FrameTime_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: frame speed
    // inferred: inline getter (with the member read directly the operands of the fadds are swapped)
    float GetDelta_unk() const { return m3A4; }
};
extern "C" FrameTime_unk* lbl_8087EFA8;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void* lbl_8087F048;  // BattleTask
extern "C" GameTask* lbl_8087F430;

AIManager* AIManager::fn_800144FC(atn::Task* parent) {
    if (lbl_8087EE68 == 0) {
        lbl_8087EE68 = new ((MemoryTag)3, "", "", 0) AIManager(parent);
    }
    return lbl_8087EE68;
}

AIManager::AIManager(atn::Task* parent)
    : atn::Task(parent), m48(0), m4C(0), m50(0), m54(0), m58(0), m78(0), m7C(0), m80(0), m84(0), m88(0), m8C(0),
      m90(0), m94(0.0f), m98(1), mDC(0), m7D0(-1), m7D4(450), m7D8(1) {
    fn_8001C1B4();
    void* menu = mMenu.Load("AIManager");
    fn_80087994(menu, "DebugMode", &m50, 0, 0);
    fn_80087994(menu, "DrawHate", &m54, 0, 0);
    void* target = fn_8008937C(menu, "Target");
    fn_800874C8(target, "AttackTargetFrame", &m7D0, -1, 9999, 1, 0, 0);
    fn_800874C8(target, "SupportTargetFrame", &m7D4, -1, 9999, 1, 0, 0);
    fn_80087994(target, "OffensiveWithTarget", &m7D8, 0, 0);
    memset(m9C, 0, sizeof(m9C));
    TeamAIManager::Create_unk(this);
}

AIManager::~AIManager() {
    lbl_8087EE68 = 0;
    fn_8001C1B8();
}

int AIManager::vf0C() {
    if (!fn_800D3FA4()) {
        return 0;
    }
    fn_800D246C(1);
    return 1;
}

void AIManager::vf10() {
    m94 += lbl_8087EFA8->GetDelta_unk();
    m98 = 0;
    while (m94 >= 1.0f) {
        m94 -= 1.0f;
        m98++;
    }
    mDC = 300;
    m90 = (m90 + 1) % 4;
    fn_80018834();
}

void AIManager::fn_80016FB4() {
    mE0.clear();
    m458.fn_8003E918();
    m48C.clear();
    if (lbl_8087F8A0 != 0) {
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            p->mF18 = p->mF14;
            p->mF14 = 0;
            p->mC58.mB8 = 0;
        }
    }
    if (EnemyManager::sInstance != 0) {
        for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
            e->mF18 = e->mF14;
            e->mF14 = 1;
            e->mC58.mB8 = 0;
        }
    }
}

void AIManager::fn_80017060() {
    fn_80016FB4();
}

int AIManager::fn_80017064(CharaTask* chara) {
    if (chara->m55C != 2) {
        return 0;
    }
    if ((chara->m54C & 0x2000) == 0x2000 || chara->mC54 == 0) {
        return 0;
    }
    int ret = 0;
    for (int i = 0; i < m98; i++) {
        if (chara->mType == 1 && chara->mC50 == 1) {
            ret = fn_8001794C(chara);
        } else {
            ret = fn_80017128(chara);
        }
    }
    return ret;
}

void AIManager::fn_80017E90(CharaTask* chara, CharaTask* target) {
    if (lbl_8087F048 != 0) {
        ((BattleTask*)lbl_8087F048)->fn_80102A40(chara, target, 100000, -1);
    }
}

// inferred: gimmick `gimmick` of the current map (same loop as GimmickManager::fn_803EEE10); name invented
static inline GimmickCommon* FindGimmick_unk(int gimmick, int arg) {
    GameScene* scene = lbl_8087F430->m10D8;
    if (scene != 0 && scene->m64 != 0) {
        if (gimmick < 1000) {
            gimmick *= 1000;
        }
        for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
            if (gimmick == g->mId && arg == g->m4C) {
                return g;
            }
        }
    }
    return 0;
}

void AIManager::fn_80017EB0(CharaTask* chara, int gimmick, int arg) {
    chara->mC58.mD0 = FindGimmick_unk(gimmick, arg);
}

// inferred: nearest AI area of the group of `chara` (its position raised by the half height)
int AIManager::fn_80017F18(CharaTask* chara, atn::Vector3* pos) {
    GameScene* scene = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (scene != 0) {
        float best = 1.0e16f;
        int found = -1;
        for (unsigned int i = 0; i < scene->mF4.size(); i++) {
            gs::AiArea& area = scene->mF4[i];
            if (area.m2C != 0 && area.m24 == chara->mC58.mB8) {
                atn::Vector3 d = area.mPos - chara->mPos;
                float dist = d.LengthSq_unk();
                if (best > dist) {
                    best = dist;
                    found = i;
                }
            }
        }
        if (found >= 0) {
            gs::AiArea& area = scene->mF4[found];
            *pos = area.mPos + atn::Vector3(0.0f, area.mSize.y, 0.0f);
            return 1;
        }
    }
    return 0;
}

// inferred: segment and oriented box tested by fn_80051E90 (atn/ColliTest_unk; box as in gmk/RandomItem.cpp)
struct ColliSegment_unk {
    atn::Vector3 mFrom;
    atn::Vector3 mTo;
};
struct ColliBox_unk {
    atn::Vector3 mSize; // inferred: half size
    atn::Matrix3x4 mMtx;
};
extern "C" int fn_80051E90(const ColliSegment_unk* seg, const ColliBox_unk* box); // inferred: segment hits box

// NONMATCHING: 99.7% - r3/r5 swapped for the addresses of the first sum and seg.mFrom
// inferred: zero when a vertical segment through `chara` crosses an AI area of its group
int AIManager::fn_80018078(CharaTask* chara) {
    GameScene* scene = lbl_8087F430->m10D8;
    int ret = 0;
    if (scene != 0 && chara->mC58.mB8 > 0) {
        ColliSegment_unk seg;
        seg.mFrom = chara->mPos + atn::Vector3(0.0f, 20.0f, 0.0f);
        seg.mTo = chara->mPos + atn::Vector3(0.0f, -20.0f, 0.0f);
        for (unsigned int i = 0; i < scene->mF4.size(); i++) {
            gs::AiArea& area = scene->mF4[i];
            if (area.m2C != 0 && area.m24 == chara->mC58.mB8) {
                ColliBox_unk box;
                box.mSize = area.mSize;
                Mtx r;
                PSMTXRotRad(r, 'y', area.m14);
                box.mMtx = *(atn::Matrix3x4*)r;
                ret = 1;
                atn::Vector3 pos = area.mPos;
                pos.y += area.mSize.y;
                box.mMtx.m[0][3] = pos.x;
                box.mMtx.m[1][3] = pos.y;
                box.mMtx.m[2][3] = pos.z;
                if (fn_80051E90(&seg, &box) != 0) {
                    ret = 0;
                    break;
                }
            }
        }
    }
    return ret;
}

// inferred: nearest effect that can reach `chara` (position and radius)
int AIManager::fn_80018240(CharaTask* chara, atn::Vector3* pos, float* radius) {
    float best = 1.0e16f;
    SkillEffect* found = 0;
    for (unsigned int i = 0; i < 32; i++) {
        SkillEffect* e = lbl_8087F9E8->GetBullet_unk(i);
        if (e != 0 && e->IsActive_unk() && e->fn_80599C90(chara) && fn_8021A984(e->m04)) {
            atn::Vector3 d = chara->mPos - e->m10;
            float dist = d.LengthSq_unk();
            if (dist < best) {
                best = dist;
                found = e;
            }
        }
    }
    if (found != 0) {
        *pos = found->m10;
        *radius = found->m5C * found->m28;
        return 1;
    }
    return 0;
}

AiTarget* AIManager::fn_80018394(CharaTask* chara) {
    for (AiTarget* p = mE0.begin(); p != mE0.end(); p++) {
        if (p->m08 == 0 && p->m0C == chara) {
            return p;
        }
    }
    return 0;
}

// inferred: redirects `chara` to `target` for `frames` frames (AttackTargetFrame without them)
void AIManager::fn_800183E0(CharaTask* chara, CharaTask* target, HitInfo_unk* hit, int frames) {
    AiTarget t;
    t.m08 = 0;
    t.m0C = target;
    t.m04 = chara;
    t.m28 = hit;
    t.m30 = frames > 0 ? frames : m7D0;
    fn_800184F4(&t);
}

void AIManager::fn_800184F4(const AiTarget* target) {
    fn_80018608(target->m04, 0);
    m48C.push_back(*target);
}

AiTarget* AIManager::fn_800185B4(CharaTask* chara, HitInfo_unk* hit) {
    for (AiTarget* p = m48C.begin(); p != m48C.end(); p++) {
        if (p->m04 == chara && (hit == 0 || p->m28 == hit)) {
            return p;
        }
    }
    return 0;
}

// inferred: no effect of an enemy touches `chara`
int AIManager::fn_80018BAC(CharaTask* chara) {
    for (unsigned int i = 0; i < 32; i++) {
        SkillEffect* e = lbl_8087F9E8->GetBullet_unk(i);
        if (e != 0 && e->IsActive_unk() && fn_8021A984(e->m04) && e->fn_80599D60(chara, 1)) {
            return 0;
        }
    }
    return 1;
}

namespace std {
namespace tr1 {
namespace detail {

// 0x8001A34C
template <class T, class D>
void* shared_ptr_deleter<T, D>::get_deleter(const type_info& ti) const {
    return ti == typeid(D) ? (void*)&mPair : 0;
}

} // namespace detail
} // namespace tr1
} // namespace std

// inferred: stands in for the functions of the unit not written yet that call an empty
// boost::function0 (they instantiate the throw of bad_function_call); not in the original
void fn_runtime_error_unk(boost::function0<void>& f) {
    f();
}
