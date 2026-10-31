#include "game/Em002_01BlackSkelton.h"
#include "chr/ControlBase.h"
#include "game/FriendManager.h"
#include "game/GimmickCommon.h"

extern "C" void* lbl_8087F048; // inferred: effect/bullet manager of the characters
extern "C" int fn_800F8548(void* mgr); // inferred: new bullet id
extern "C" void fn_80105BD8(void* mgr, atn::Model* model);
extern "C" void fn_801092C8(void* mgr, int arg, CharaTask* chara, int a, int b);
// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78 and written with fn_80370AE4
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
extern "C" int fn_80375184(Field_unk* field);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: length
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: plays an effect on the model (name invented)
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

// inferred: link of the character lists of the managers (CharaTask + 0x14AC, as in game/CharaTask.cpp)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
inline CharaTask* NextChara_unk(CharaTask* chara) {
    return ((CharaListNode_unk*)chara)->mNext;
}

// inferred: event flags set for each character carried to a mirror ({flag, character id}, one table
// per value of the flag 206)
static const int sCharaFlags[3][6][2] = {
    {{200, 100401}, {201, 100201}, {202, 0}, {203, 101101}, {204, 100601}, {205, 0}},
    {{200, 0}, {201, 100401}, {202, 100201}, {203, 0}, {204, 101101}, {205, 100601}},
    {{200, 100601}, {201, 0}, {202, 100401}, {203, 100201}, {204, 0}, {205, 101101}},
};

inline void CharaList_unk::Erase_unk(CharaTask** it) {
    memmove(it, it + 1, (&this->mData[this->mSize] - it - 1) * sizeof(CharaTask*));
    this->mSize--;
}

Em002_01BlackSkelton::Em002_01BlackSkelton(atn::Task* parent, void* data)
    : EnemyTask(parent, data), m14B8(0), m14CC(0) {
    m14BC.__shrink_unk(0);
    m14D0.LoadRequest("/effect/ef_uc023.efp");
}

Em002_01BlackSkelton::~Em002_01BlackSkelton() {}

// NONMATCHING: 94.4% - same instructions; the flag word 0x12A4 and the size of m14BC get other
// registers (the original ors the loaded word into a new register)
int Em002_01BlackSkelton::vf0C() {
    if (fn_8013655C() == 0 && m14D0.IsLoading() == 0) {
        m14B0 = 0;
        m14B4 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 0;
        mFlag25 = 1;
        mFlag15 = 1;
        m14CC = 60;
        m14A8 &= ~0x04000000;
        m14BC.__shrink_unk(0);
        mStatus.m18 |= 0x40;
        mStatus.m18 |= 0x80;
        mStatus.m18 |= 0x100;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x8000;
        mStatus.m18 |= 0x4000;
        mStatus.m18 |= 0x1;
        mStatus.m18 |= 0x1000000;
        mStatus.m18 |= 0x2000000;
        m54C |= 0x01000000;
        return 1;
    }
    return 0;
}

// NONMATCHING: 96.1% - the erase in the final loop reuses the end pointer computed by the loop test;
// the original recomputes it (4 instructions more). Calling Erase_unk through a local
// `CharaList_unk* l = &m14BC` inside the if gives the recomputation but swaps this/busy (r30/r31).
void Em002_01BlackSkelton::vf10() {
    fn_8014C540();
    int busy = 0;
    if (mC58.mC0 == 0) {
        mPos = atn::Vector3(0.0f, 1000.0f, 0.0f);
        fn_80144710();
        m58C = 0;
        fn_8016E970(0);
        m14BC.__shrink_unk(0);
    } else if (mC58.mC0 == 1) {
        switch (m58C) {
        case 0:
            fn_80247DA4();
            break;
        case 1: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0) && m14B8 != 0) {
                m14B0 = 0;
                m14B4 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 2;
                fn_8017547C(m14B8);
                switch (fn_80219558(m14B8->m50)) {
                case 2:
                    fn_80370AE4(lbl_8087F430, 240, 1);
                    break;
                case 6:
                    fn_80370AE4(lbl_8087F430, 241, 1);
                    break;
                case 4:
                    fn_80370AE4(lbl_8087F430, 242, 1);
                    break;
                case 11:
                    fn_80370AE4(lbl_8087F430, 243, 1);
                    break;
                }
            }
            busy = 1;
            break;
        }
        case 2:
            fn_80247FB0();
            busy = 1;
            break;
        case 3:
            fn_8024845C();
            busy = 1;
            break;
        case 4: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mColli.mFlags &= ~1;
                if (m1434 > 0) {
                    if (--m1434 == 15) {
                        fn_80105BD8(lbl_8087F048, &mMotion);
                        vf9C(1);
                    }
                } else {
                    m14B0 = 0;
                    m14B4 = 0;
                    m590 = fn_800F8548(lbl_8087F048);
                    m58C = 0;
                    mFlag15 = 1;
                    mFlag25 = 1;
                    m14CC = 60;
                }
            }
            busy = 1;
            break;
        }
        }
    }
    mStatus.fn_8013322C(0x20);
    fn_80370AE4(lbl_8087F430, 7, busy);
    if (busy) {
        m54C &= ~0x2000;
    } else {
        m54C |= 0x2000;
    }
    CharaTask** it = m14BC.begin();
    while (it != m14BC.end()) {
        if (((*it)->mStatus.m0C & 0x20) != 0x20) {
            m14BC.Erase_unk(it);
        } else {
            it++;
        }
    }
    fn_80145334();
}

void Em002_01BlackSkelton::vf14() {
    if (m58C != 0) {
        fn_80149A30();
    }
}

void Em002_01BlackSkelton::vf3C(HitInfo_unk* hit) {
    if (m58C == 0 || m58C == 3 || m58C == 4) {
        return;
    }
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m90 = 0;
    msg->m88 = -1;
    msg->m84 = 0;
    vf40(hit);
}

void Em002_01BlackSkelton::vf40(HitInfo_unk* hit) {
    fn_80248A4C(1);
}

void Em002_01BlackSkelton::fn_80247C68(CharaTask* chara) {
    int level = fn_80370A78(lbl_8087F430, 206);
    if (level < 0) {
        level = 0;
    }
    if (level > 2) {
        level = 2;
    }
    int id = chara->m50;
    const int(*table)[2] = sCharaFlags[level];
    int found = 0;
    for (int i = 0; i < 6; i++) {
        if (id == table[i][1]) {
            fn_80370AE4(lbl_8087F430, table[i][0], id);
            found = 1;
            break;
        }
    }
    if (found) {
        int count = fn_80370A78(lbl_8087F430, 35);
        if (count == 0) {
            for (int i = 0; i < 6; i++) {
                if (id != table[i][1]) {
                    fn_80370AE4(lbl_8087F430, table[i][0], 0);
                }
                if (table[i][1] < 0) {
                    fn_80370AE4(lbl_8087F430, table[i][0], 25);
                }
            }
        }
        fn_80370AE4(lbl_8087F430, 35, count + 1);
    }
}

void Em002_01BlackSkelton::fn_80247DA4() {
    mPos = atn::Vector3(0.0f, 1000.0f, 0.0f);
    fn_80144710();
    if (m14CC > 0) {
        m14CC--;
        return;
    }
    CharaTask* target = 0;
    for (CharaTask* c = FriendManager::sInstance->fn_8036554C(); c != 0; c = NextChara_unk(c)) {
        int carried = 0;
        for (unsigned int i = 0; i < m14BC.size(); i++) {
            if (m14BC[i] == c) {
                carried = 1;
            }
        }
        if (!carried && c->IsActive_unk() && (c->m54C & 0x2000) != 0x2000 && c->mC54 != 0 &&
            (c->mStatus.m0C & 0x20) == 0x20 && c->fn_8015EB2C()) {
            target = c;
            break;
        }
    }
    if (target != 0) {
        GimmickCommon* best = 0;
        float bestDist = 1e16f;
        for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
            if (g->vf60() && g->m50 == 29) {
                float dist = PSVECSquareMag(&(g->m6C - target->mPos));
                if (dist < bestDist) {
                    best = g;
                    bestDist = dist;
                }
            }
        }
        if (best != 0) {
            fn_80248854(target, best);
        }
    }
}

void Em002_01BlackSkelton::fn_80247FB0() {
    if (m14B0 == 0) {
        if (mControl) {
            if (mControl->vf0C()) {
                mControl.reset();
            }
        }
        if (mControl.get() == 0) {
            if (m14C8 != 0 && mF54 != 0) {
                m14B0 = 1;
                atn::Vector3 pos(0.0f, 0.0f, 5.0f);
                Mtx m;
                PSMTXRotRad(m, 'y', m14C8->m78.y);
                PSMTXMultVec(m, &pos, &pos);
                pos += m14C8->m6C;
                fn_80170F20(&pos, 0, -1.0f * m14C8->m78.y, -1.0f);
            } else {
                fn_80248A4C(0);
            }
        }
    } else if (m14B0 == 1) {
        atn::Vector3 rot = mRot;
        mRoute.fn_80126214();
        atn::Vector3 dir = mRoute.m58;
        float len = dir.Length_unk();
        if (len > 1.0f) {
            rot = dir.GetNormalize().ToRotation_unk();
            vf34(rot, 0, len, 1.0f);
        } else {
            m14B0 = 0;
            mColli.mFlags &= ~1;
            m14B4 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m58C = 3;
            GimmickMsg_unk msg(4);
            m14C8->vf40(&msg);
        }
    }
}

void Em002_01BlackSkelton::fn_8024845C() {
    atn::Vector3 pos(0.0f, 0.0f, -50.0f);
    Mtx m;
    float ry = m14C8->m78.y;
    PSMTXRotRad(m, 'y', ry);
    PSMTXMultVec(m, &pos, &pos);
    pos += m14C8->m6C;
    atn::Vector3 d(pos.x - mPos.x, 0.0f, pos.z - mPos.z);
    float len = d.Length_unk();
    if (len > 0.0001f) {
        if (len > 0.4f) {
            d.Normalize_unk();
            d *= 0.4f;
        }
        mPos += d;
        mRot = d.GetNormalize().ToRotation_unk();
        mRot.y += 3.1415927f;
    } else {
        CharaTask* chara = (CharaTask*)mF54;
        chara->mStatus.fn_8012D8B8();
        switch (fn_80219558(((CharaTask*)mF54)->m50)) {
        case 2:
            fn_80370AE4(lbl_8087F430, 30, 1);
            break;
        case 6:
            fn_80370AE4(lbl_8087F430, 31, 1);
            break;
        case 4:
            fn_80370AE4(lbl_8087F430, 32, 1);
            break;
        case 11:
            fn_80370AE4(lbl_8087F430, 33, 1);
            break;
        }
        fn_80247C68((CharaTask*)mF54);
        fn_80248A4C(0);
    }
}

void Em002_01BlackSkelton::fn_80248854(CharaTask* target, GimmickCommon* mirror) {
    if (target != 0) {
        m14B0 = 0;
        m14B4 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 1;
        m14B8 = target;
        m14C8 = mirror;
        mColli.mFlags |= 1;
        mFlag15 = 0;
        mFlag25 = 0;
        mStatus.fn_8012D8B8();
        Mtx m;
        atn::Vector3 ofs(0.0f, 0.0f, 12.0f);
        PSMTXRotRad(m, 'y', target->mRot.y);
        PSMTXMultVec(m, &ofs, &ofs);
        mRot = target->mRot;
        mRot.y += 3.1415927f;
        mPos = target->mPos + ofs;
        fn_80145334();
        GimmickMsg_unk msg(3);
        m14C8->vf40(&msg);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.fn_80097C08(0, 0x2B, 0, 0, 1, 1.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
        PlayEffect_unk(&m14D0, &mMotion);
    }
}

void Em002_01BlackSkelton::fn_80248A4C(int arg) {
    m14B0 = 0;
    m14B4 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 4;
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    mFC0 = 0;
    m1434 = 30;
    if (arg && lbl_8087F430 != 0 && fn_80375184(lbl_8087F430)) {
        fn_801092C8(lbl_8087F048, 6, this, 0, 0);
    }
    if (mF54 != 0) {
        fn_80175A38();
        if (m14B8 != 0) {
            m14BC.push_back(m14B8);
        }
    }
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = 1.0f;
    GimmickMsg_unk msg(2);
    m14C8->vf40(&msg);
    m14B8 = 0;
    m14C8 = 0;
}
