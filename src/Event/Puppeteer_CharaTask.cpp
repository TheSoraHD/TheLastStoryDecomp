// Event/Puppeteer_CharaTask.cpp (0x8000CBD0-0x800142C4): Event::Puppeteer<CharaTask>, Event::Puppeteer<atn::Model>
// and Event::Actor, the actors of an event sequence.
#include "Event/Puppeteer.h"
#include "Event/Actor.h"
#include "Event/Sequence.h"
#include "game/CharaTask.h"
#include "game/GimmickCommon.h"
#include "atn/Model_unk.h"
#include "atn/ModelPose_unk.h"
#include "Event/Manager.h"
#include "game/PlayerManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/FriendManager.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/GimmickManager.h"
#include "game/GameScene.h"
#include "game/GameTask.h"

extern "C" GameTask* lbl_8087F430; // inferred: the field

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int atoi(const char* s);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_800D2338(atn::Task* task);       // inferred: removes the task
extern "C" PlayerManager* lbl_8087F8A0;             // inferred: the party manager
// inferred: debug message list (atn/FileUtil_unk.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" DebugMessages_unk* lbl_8087EEB8; // inferred: debug message output
// inferred: motion resource info (as in game/CharaTask.cpp: fn_802096A8 looks it up by the motion name)
struct MotionInfo_unk {
    int m00;
    float m04; // inferred: distance covered by the motion
};
extern "C" MotionInfo_unk* fn_802096A8(const char* name);

// inferred: the party manager and the event manager (weak copies 0x8000D9E8 and 0x8000DB1C; their names are
// unknown, other files call them as fn_8000D9E8 and fn_8000DB1C)
extern "C" inline PlayerManager* fn_8000D9E8() {
    return lbl_8087F8A0;
}
inline PlayerTask* PlayerManager::GetLeader_unk() {
    return m54;
}
inline PlayerTask* PlayerManager::GetHead_unk() {
    return mHead;
}
inline int CharaTask::GetCharaNo_unk() {
    return m50;
}
inline void CharaTask::SetM58_unk(int v) {
    m58 = v;
}
inline Event::Sequence* Event::Actor::GetSequence_unk() const {
    return m04;
}
extern "C" inline Event::Manager* fn_8000DB1C() {
    return lbl_8087F540;
}

namespace Event {
// inferred: as in Event/ActionActorPosition.cpp (inlined in Event::Actor::fn_8000E6B0 and fn_8000E7B8)
template <class T>
T* DataList_unk<T>::Find_unk(int id) {
    for (int i = 0; i < (int)this->size(); i++) {
        T* p = &(*this)[i];
        if (id == p->m14) {
            return p;
        }
    }
    return 0;
}
} // namespace Event

// inferred: loaded data of the first level of the model (weak copy 0x8000DD04)
// The empty asserts put the callers that keep it out of line over the inline budget, as in the
// original (Event::Actor::fn_8000DB24 and fn_8000E474 call all their inlines).
inline atn::Model::Res_unk* atn::Model::fn_8000DD04() {
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    SEQ_ASSERT100_UNK;
    return GetRes_unk();
}

namespace Event {


template <>
int Puppeteer<CharaTask>::vf0C() {
    return 1;
}

// NONMATCHING: partial - places the character on the matrix m54 (rotations of the euler angles); written up to
// the identity matrix so that its 0.0f and 1.0f open .sdata2 as in the original
template <>
void Puppeteer<CharaTask>::vf10() {
    if (m50 == 0) {
        return;
    }
    if (m50->m194 != 10) {
        bool ending = m50->m194 == 11 || m50->m194 == 12;
        if (!ending) {
            return;
        }
    }
    CharaTask* chara = m48;
    if (chara == 0 || chara->m55C != 8 || !chara->IsFlag_unk(4)) {
        return;
    }
    if (m54 == 0) {
        return;
    }
    atn::Matrix3x4 mtx(*m54);
    if (mB8 == 0) {
        mtx.fn_801479E4(chara->mPos);
        m58.m[0][3] = m58.m[1][3] = m58.m[2][3] = 0.0f;
    }
    if (mB9 == 0) {
        atn::Matrix3x4 rot(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        m88 = rot;
    }
}

template <>
void Puppeteer<CharaTask>::vf14() {
    if (m50 == 0) {
        return;
    }
    if (m50->m194 != 10) {
        bool ending = m50->m194 == 11 || m50->m194 == 12;
        if (!ending) {
            return;
        }
    }
    CharaTask* chara = m48;
    if (chara != 0 && chara->m55C == 8 && chara->IsFlag_unk(4)) {
        chara->fn_80149A30();
    }
}

template <>
int Puppeteer<atn::Model>::vf0C() {
    return 1;
}

// DRAFT: stub; the matrix stack of the model is not computed
// NONMATCHING: stub - computes the matrix stack of the model
template <>
void Puppeteer<atn::Model>::vf10() {
}

template <>
void Puppeteer<atn::Model>::vf14() {
    if (m4C_0 != 1) {
        return;
    }
    if (m50 == 0) {
        return;
    }
    if (m50->m194 != 10) {
        bool ending = m50->m194 == 11 || m50->m194 == 12;
        if (!ending) {
            return;
        }
    }
    m48->fn_8008CD60();
}

// NONMATCHING: registers of the vtable and of the memset arguments (r3/r4 swapped)
Actor::Actor(Sequence* owner, const char* name, int id)
    : CommonData(owner, name, id), m20(0), m58(-1) {
    m1C = 0;
    m18 = 0;
    mB8[0] = 0;
    mB8[1] = 0;
    mB8[2] = 0;
    mC4 = lbl_807C7030;
    mD0 = lbl_807C7030;
    mDC = lbl_807C7030;
    mE8 = lbl_807C7030;
    mF4 = lbl_807C7030;
    m100 = lbl_807C7030;
    m10C = lbl_807C7030;
    m130 = -1;
    m134 = -1;
    m144 = 1;
    m145 = 1;
    m146 = 1;
    m188 = 1;
    m154 = 0;
}

// inferred: character of kind `type` with character id `id` (leader, NPCs, friends and enemies)
extern "C" CharaTask* fn_8000D8C0(int type, int id) {
    if (fn_8000D9E8() != 0) {
        CharaTask* leader = fn_8000D9E8()->GetLeader_unk();
        if (leader != 0 && type == leader->mType && id == leader->GetCharaNo_unk()) {
            return leader;
        }
    }
    if (NpcManager::sInstance != 0) {
        for (NpcTask* c = NpcManager::sInstance->mHead; c != 0; c = c->mNext) {
            if (type == c->mType && id == c->m50) {
                return c;
            }
        }
    }
    if (FriendManager::sInstance != 0) {
        for (PlayerTask* c = (PlayerTask*)FriendManager::sInstance->fn_8036554C(); c != 0; c = c->mNext) {
            if (type == c->mType && id == c->m50) {
                return c;
            }
        }
    }
    if (EnemyManager::sInstance != 0) {
        for (EnemyTask* c = EnemyManager::sInstance->mHead; c != 0; c = c->mNext) {
            if (type == c->mType && id == c->m50) {
                return c;
            }
        }
    }
    return 0;
}

// inferred: character of kind `type` placed on the set point `point` (NPCs, friends and enemies)
extern "C" CharaTask* fn_8000DA00(int type, int point) {
    if (NpcManager::sInstance != 0) {
        for (NpcTask* c = NpcManager::sInstance->mHead; c != 0; c = c->mNext) {
            if (type == c->mType && point == c->m58) {
                return c;
            }
        }
    }
    if (FriendManager::sInstance != 0) {
        for (PlayerTask* c = (PlayerTask*)FriendManager::sInstance->fn_8036554C(); c != 0; c = c->mNext) {
            if (type == c->mType && point == c->m58) {
                return c;
            }
        }
    }
    if (EnemyManager::sInstance != 0) {
        for (EnemyTask* c = EnemyManager::sInstance->mHead; c != 0; c = c->mNext) {
            if (type == c->mType && point == c->m58) {
                return c;
            }
        }
    }
    return 0;
}

// inferred: character id of the set point `point` of the scene (-1 if missing)
extern "C" int fn_8000DAF0(int type, int point) {
    GameScene* scene = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : fn_8000DB1C()->m1A6C;
    return scene->fn_803CC5E4(type, point);
}

void Actor::fn_8000DB24(Sequence* seq, int kind, int id) {
    int chara = fn_8000DAF0(kind, id);
    if (chara == -1) {
        m20 = 0;
        return;
    }
    m20 = 1;
    m18_6 = 1;
    CharaTask* c = fn_8000DA00(kind, id);
    if (c == 0) {
        CharaTask* head = fn_8000D9E8() != 0 ? fn_8000D9E8()->GetHead_unk() : 0;
        if (head != 0 && chara == head->GetCharaNo_unk()) {
            c = head;
        } else {
            c = fn_8000DB1C()->fn_8047FF70(kind, chara, 0.0f, lbl_807C7030);
            if (c != 0) {
                c->SetM58_unk(id);
            }
        }
    }
    if (c == 0) {
        m20 = 0;
        return;
    }
    m1C = Puppeteer<CharaTask>::Create_unk(seq, c);
    atn::Model::Res_unk* res = ((Puppeteer<CharaTask>*)m1C)->GetTarget_unk()->GetModel_unk()->fn_8000DD04();
    if (res != 0) {
        for (int i = 0; i < res->m30; i++) {
            unsigned int hash = fn_800DC6B4(res->m50[i]);
            ColorF_unk color(fn_8000DD98(((Puppeteer<CharaTask>*)m1C)->GetTarget_unk()->GetModel_unk()->fn_800929C0(hash)));
            m158.push_back(color);
            m170.push_back(color);
            int shown = ((Puppeteer<CharaTask>*)m1C)->GetTarget_unk()->GetModel_unk()->fn_80093E90(hash);
            m164.push_back(shown);
            m17C.push_back(shown);
        }
    }
}

// inferred: instantiated behind their first caller, as in the original (their "vector length error" opens the
// string pool)
template void std::vector<ColorF_unk>::push_back(const ColorF_unk& x);
template void std::vector<int>::push_back(const int& x);

void Actor::fn_8000E474(Sequence* seq, int kind, int id) {
    m18_6 = 0;
    m20 = 1;
    CharaTask* c = fn_8000D8C0(kind, id);
    if (c == 0) {
        c = fn_8000DB1C()->fn_8047FF70(kind, id, 0.0f, lbl_807C7030);
    }
    if (c == 0) {
        m20 = 0;
        return;
    }
    m1C = Puppeteer<CharaTask>::Create_unk(seq, c);
    atn::Model::Res_unk* res = ((Puppeteer<CharaTask>*)m1C)->GetTarget_unk()->GetModel_unk()->fn_8000DD04();
    if (res != 0) {
        for (int i = 0; i < res->m30; i++) {
            unsigned int hash = fn_800DC6B4(res->m50[i]);
            ColorF_unk color(fn_8000DD98(((Puppeteer<CharaTask>*)m1C)->GetTarget_unk()->GetModel_unk()->fn_800929C0(hash)));
            m158.push_back(color);
            m170.push_back(color);
            int shown = ((Puppeteer<CharaTask>*)m1C)->GetTarget_unk()->GetModel_unk()->fn_80093E90(hash);
            m164.push_back(shown);
            m17C.push_back(shown);
        }
    }
}

void Actor::fn_8000E5D0(int id, int usbId) {
    if (GimmickManager::instance_ == 0) {
        m20 = 0;
        return;
    }
    m18_6 = 1;
    m20 = 2;
    m1C = 0;
    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        if (id == g->mId && usbId == g->m4C) {
            m1C = g;
            break;
        }
    }
    if (m1C == 0) {
        if (lbl_8087EEB8 != 0) {
            char buf[256];
            sprintf(buf, "Gimmick not found! (id=%d, usbId=%d)", id, usbId);
            lbl_8087EEB8->fn_800697D8(buf);
        }
        m20 = 0;
    }
}

void Actor::fn_8000E6B0(Sequence* seq, int id) {
    ModelData* data = seq->mF4.Find_unk(id);
    if (data == 0) {
        m20 = 0;
        return;
    }
    m18_6 = 0;
    m20 = 3;
    atn::Model* model = data->m24;
    m1C = Puppeteer<atn::Model>::Create_unk(seq, model);
}

void Actor::fn_8000E7B8(Sequence* seq, int id) {
    Data48_unk* data = seq->m104.Find_unk(id);
    if (data == 0) {
        m20 = 0;
        return;
    }
    m18_6 = 0;
    m20 = 4;
    m1C = (atn::Task*)data;
}

void Actor::vf08() {
    Sequence* seq = GetSequence_unk();
    if (m08.size() == 0 || m08 == "!world") {
        m20 = 6;
        return;
    }
    if (m08 == "!player") {
        m20 = 7;
        m1C = Puppeteer<CharaTask>::Create_unk(seq, fn_8000D9E8()->GetLeader_unk());
        atn::Model::Res_unk* res = ((Puppeteer<CharaTask>*)m1C)->GetTarget_unk()->GetModel_unk()->fn_8000DD04();
        if (res != 0) {
            for (int i = 0; i < res->m30; i++) {
                unsigned int hash = fn_800DC6B4(res->m50[i]);
                ColorF_unk color(
                    fn_8000DD98(((Puppeteer<CharaTask>*)m1C)->GetTarget_unk()->GetModel_unk()->fn_800929C0(hash)));
                m158.push_back(color);
                m170.push_back(color);
                int shown = ((Puppeteer<CharaTask>*)m1C)->GetTarget_unk()->GetModel_unk()->fn_80093E90(hash);
                m164.push_back(shown);
                m17C.push_back(shown);
            }
        }
        return;
    }
    // inferred: "<kind><number>[.<number>]", the kind in two letters
    unsigned short kind = (m08[0] << 8) | m08[1];
    char first[32];
    char second[32];
    memset(first, 0, sizeof(first));
    memset(second, 0, sizeof(second));
    int n = 0;
    char* p = first;
    for (int i = 2; i < (int)m08.size(); i++) {
        if (m08[i] >= '0' && m08[i] <= '9') {
            p[n] = m08[i];
            n++;
        } else if (m08[i] == '.') {
            p[n] = 0;
            p = second;
            n = 0;
        } else if (m08[i] != '_') {
            return;
        }
    }
    p[n] = 0;
    int id = atoi(first);
    int sub = atoi(second);
    switch (kind) {
    case 'em':
        fn_8000DB24(seq, 2, id);
        break;
    case 'fr':
        fn_8000DB24(seq, 3, id);
        break;
    case 'np':
        fn_8000DB24(seq, 1, id);
        break;
    case 'ev':
        fn_8000E474(seq, 4, id);
        break;
    case 'gm':
        fn_8000E5D0(id, sub);
        break;
    case 'md':
        fn_8000E6B0(seq, id);
        break;
    case 'dm':
        fn_8000E7B8(seq, id);
        break;
    }
    mB8[0] = 0;
    mB8[1] = 0;
    mB8[2] = 0;
}

void Actor::vf0C() {
    unsigned int kill = GetSequence_unk()->mNoKill_unk;
    switch (m20) {
    case 7:
    case 1:
        if (kill) {
            for (int i = 0; i < (int)mAC.size(); i++) {
                GetChara_unk()->fn_8014F4FC(mAC[i].m00, mAC[i].m04);
            }
            fn_800D2338(m1C);
            m1C = 0;
        }
        break;
    case 3:
        if (kill) {
            fn_800D2338(m1C);
            m1C = 0;
        }
        break;
    }
    m1C = 0;
    m20 = 0;
}

int Actor::vf10() const {
    if (m20 == 6) {
        return 1;
    }
    if (m1C == 0) {
        return 1;
    }
    switch (m20) {
    case 7:
    case 1:
        if (!((Puppeteer<CharaTask>*)m1C)->m48->IsFlag_unk(2)) {
            return 0;
        }
        if (((Puppeteer<CharaTask>*)m1C)->m48->fn_8014FAD0()) {
            return 0;
        }
        if (((Puppeteer<CharaTask>*)m1C)->m48->mAC != 3) {
            return 0;
        }
        break;
    case 3:
        if (((Puppeteer<atn::Model>*)m1C)->m48->fn_8008B140()) {
            return 0;
        }
        break;
    case 4:
        return 1;
    }
    return 1;
}

// inferred: position of the actor
atn::Vector3 Actor::fn_80010374() {
    switch (m20) {
    case 6:
        return lbl_807C7030;
    case 7:
    case 1:
        if (m1C != 0) {
            return ((Puppeteer<CharaTask>*)m1C)->m48->mPos;
        }
    case 2:
        if (m1C != 0) {
            return ((GimmickCommon*)m1C)->m6C;
        }
    case 3:
        if (m1C != 0) {
            return ((Puppeteer<atn::Model>*)m1C)->m48->mMtx.GetTrans_unk();
        }
    case 4:
        if (m1C != 0) {
            atn::Matrix3x4* mtx = (atn::Matrix3x4*)((Data48_unk*)m1C)->m18;
            return atn::Vector3(mtx->m[0][3], mtx->m[1][3], mtx->m[2][3]);
        }
    }
    return lbl_807C7030;
}

// inferred: places the actor on the matrix `mtx`, optionally relative to the node matrix `node`
// NONMATCHING: the original keeps the compares of the empty cases 2 and 4 and copies the matrix through a
// stack copy interleaved with the stores to m58 (probably inlines that are not known)
void Actor::fn_8001122C(MtxPtr node, int arg2, int arg3, const atn::Matrix3x4& mtx) {
    switch (m20) {
    case 7:
    case 1: {
        atn::Matrix3x4 m(mtx);
        ((Puppeteer<CharaTask>*)m1C)->m58 = m;
        ((Puppeteer<CharaTask>*)m1C)->m54 = (atn::Matrix3x4*)node;
        ((Puppeteer<CharaTask>*)m1C)->mB8 = arg2;
        ((Puppeteer<CharaTask>*)m1C)->mB9 = arg3;
        break;
    }
    case 2:
        break;
    case 3:
        ((Puppeteer<atn::Model>*)m1C)->m54 = (atn::Matrix3x4*)node;
        ((Puppeteer<atn::Model>*)m1C)->mB8 = arg2;
        ((Puppeteer<atn::Model>*)m1C)->mB9 = arg3;
        break;
    case 4:
        break;
    }
}

// inferred: rotation of the actor around y
float Actor::fn_80011964() {
    switch (m20) {
    case 6:
        return 0.0f;
    case 7:
    case 1:
        return GetChara_unk()->mRot.y;
    case 2:
        return ((GimmickCommon*)m1C)->m78.y;
    }
    return 0.0f;
}

// inferred: sets the speed of the motion `motion` from the distance moved
void Actor::fn_800121F0(int motion, float distance) {
    if (m20 == 7 || m20 == 1) {
        atn::Model* model = m1C != 0 ? GetChara_unk()->GetModel_unk() : 0;
        atn::LoadHandle* handle = model->fn_80097D40(motion);
        MotionInfo_unk* info = handle != 0 ? fn_802096A8(handle->fn_80473F18()) : 0;
        model->mTracks[0].m0C = info != 0 ? distance / info->m04 : 0.0f;
    }
}

// inferred: the character driven by a character actor (kinds 1 and 7)
inline CharaTask* Actor::GetChara_unk() {
    return ((Puppeteer<CharaTask>*)m1C)->m48;
}

void Actor::fn_80012540(int arg) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->fn_80176DFC(arg, arg, 0);
    }
}

// NONMATCHING: registers of the case 3 (r3/r4/r5)
void Actor::fn_8001256C(int show) {
    switch (m20) {
    case 7:
    case 1:
        GetChara_unk()->fn_8016E5F0(show);
        break;
    case 2:
        ((GimmickCommon*)m1C)->m9C = show;
        break;
    case 3:
        ((Puppeteer<atn::Model>*)m1C)->SetFlag0_unk(show);
        break;
    }
}

void Actor::fn_800125C8(int on) {
    if (m20 == 7 || m20 == 1) {
        CharaTask* chara = GetChara_unk();
        if (on != 0) {
            chara->mColli.mFlags |= 1;
        } else {
            chara->mColli.mFlags &= ~1;
        }
    }
}

void* Actor::fn_8001260C() {
    if (m20 == 7 || m20 == 1) {
        return &GetChara_unk()->mColli;
    }
    return 0;
}

void Actor::fn_80012638(int on) {
    if (m20 == 7 || m20 == 1) {
        if (on != 0) {
            GetChara_unk()->vf98();
        } else {
            GetChara_unk()->vf9C(0);
        }
    }
}

// inferred: sends the message `msg` to a gimmick actor (or sets its state)
void Actor::fn_8001268C(int msg, int state) {
    if (m20 == 2) {
        if (state != 0) {
            ((GimmickCommon*)m1C)->vf70(msg);
        } else {
            GimmickMsg_unk m(msg);
            ((GimmickCommon*)m1C)->vf40(&m);
        }
    }
}

void Actor::fn_8001271C(int on) {
    switch (m20) {
    case 7:
    case 1:
        GetChara_unk()->m1220.m10 = on != 0;
        break;
    case 2: {
        GimmickCommon* gimmick = (GimmickCommon*)m1C;
        gimmick->mB0.mEnabled = on;
        if (gimmick->mE4 != 0) {
            gimmick->mE4->m50 = on;
        }
        break;
    }
    }
}

void Actor::fn_80012774(int on) {
    switch (m20) {
    case 7:
    case 1:
        GetChara_unk()->m1220.m11 = on != 0;
        break;
    case 2: {
        GimmickCommon* gimmick = (GimmickCommon*)m1C;
        gimmick->mB0.mSoundOn = on;
        if (gimmick->mE4 != 0) {
            gimmick->mE4->m54 = on;
        }
        break;
    }
    }
}

int Actor::fn_800127CC(int id, int b) {
    if (id == -1 || b == -1) {
        return 0;
    }
    FixedArray_unk<chr::CharaPart_unk*, 8>& parts = GetChara_unk()->mParts;
    for (int i = 0; i < (int)parts.mCount; i++) {
        if (id == parts.mData[(unsigned int)i]->m04) {
            return 1;
        }
    }
    return 0;
}

void Actor::fn_8001282C(int id, int b) {
    if (m20 == 7 || m20 == 1) {
        if (id == -1) {
            CharaTask* chara = GetChara_unk();
            if (chara->m648 != 0) {
                chara->fn_8014EEC4(0);
            }
        } else {
            fn_800127CC(id, b);
            CharaTask* chara = GetChara_unk();
            int i;
            FixedArray_unk<chr::CharaPart_unk*, 8>& parts = chara->mParts;
            for (i = 0; i < (int)parts.mCount; i++) {
                if (id == parts.mData[(unsigned int)i]->m04) {
                    chara->fn_8014DEE4(i, 0, 0);
                    return;
                }
            }
        }
    }
}

CharaTask* Actor::fn_800128FC() {
    if (m20 == 7 || m20 == 1) {
        if (m1C != 0) {
            return GetChara_unk();
        }
        return 0;
    }
    return 0;
}

CharaTask* Actor::fn_80012934() {
    if (m20 == 7 || m20 == 1) {
        if (m1C != 0) {
            return GetChara_unk();
        }
        return 0;
    }
    return 0;
}

// inferred: model of the actor (same code as fn_80012A1C)
atn::Model* Actor::fn_8001296C() {
    switch (m20) {
    case 7:
    case 1:
        if (m1C != 0) {
            return GetChara_unk()->GetModel_unk();
        }
        return 0;
    case 2:
        if (m1C != 0) {
            return ((GimmickCommon*)m1C)->vf38();
        }
        return 0;
    case 3:
        if (m1C != 0) {
            return ((Puppeteer<atn::Model>*)m1C)->m48;
        }
        return 0;
    case 4:
    case 6:
        return 0;
    }
    return 0;
}

atn::Model* Actor::fn_80012A1C() {
    switch (m20) {
    case 7:
    case 1:
        if (m1C != 0) {
            return GetChara_unk()->GetModel_unk();
        }
        return 0;
    case 2:
        if (m1C != 0) {
            return ((GimmickCommon*)m1C)->vf38();
        }
        return 0;
    case 3:
        if (m1C != 0) {
            return ((Puppeteer<atn::Model>*)m1C)->m48;
        }
        return 0;
    case 4:
    case 6:
        return 0;
    }
    return 0;
}

// inferred: matrix of the node `node` (name hash) of the actor, its world matrix for 0
// NONMATCHING: 99.6% - the original loads m20 before saving r31 and r30 (prologue scheduling); tried a local
// for the kind, a cast, a result variable, a default case, if/else and an int argument
MtxPtr Actor::fn_80012AC4(unsigned int node) {
    switch (m20) {
    case 7:
    case 1:
        if (node != 0) {
            return GetChara_unk()->GetModel_unk()->GetNodeMtxByHash_unk(node);
        }
        return GetChara_unk()->GetModel_unk()->mMtx.m;
    case 2:
        if (node != 0) {
            return ((GimmickCommon*)m1C)->vf38()->GetNodeMtxByHash_unk(node);
        }
        return ((GimmickCommon*)m1C)->vf38()->mMtx.m;
    case 3:
        if (node != 0) {
            return ((Puppeteer<atn::Model>*)m1C)->m48->GetNodeMtxByHash_unk(node);
        }
        return ((Puppeteer<atn::Model>*)m1C)->m48->mMtx.m;
    case 4:
        return (MtxPtr)((Data48_unk*)m1C)->m18; // inferred: m18 holds a 3x4 matrix (0x30 bytes)
    }
    return 0;
}

void Actor::fn_80012CBC(const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_80129954(pos, speed);
    }
}

void Actor::fn_80012CE4(const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_80129930(pos, speed);
    }
}

void Actor::fn_80012D0C(float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_80129A48(speed);
    }
}

// ---- fn_80012D34 and its helpers (over the inlining budget: weak copies at 0x800132EC-0x80013484) ----
} // namespace Event
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
// inferred: rotation matrix from euler angles, z then y then x (weak copy 0x800109E0)
extern "C" inline atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}
// inferred: *this = m * *this (declared in math/Vec.h; weak copy 0x80011410)
inline void atn::Vector3::fn_80011410(const atn::Matrix3x4& m) {
    PSMTXMultVec(m.m, this, this);
}
// inferred: fabs (weak copy 0x80011220)
extern "C" inline float fn_80011220(float x) {
    double r = __fabs(x);
    return r;
}
// inferred: the vectors differ (weak copy 0x80013444)
extern "C" inline int fn_80013444(const atn::Vector3* a, const atn::Vector3* b) {
    return !(a->x == b->x && a->y == b->y && a->z == b->z);
}
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
namespace Event {

// DRAFT: not checked against the original (draft of the eye/head turn)
// NONMATCHING: draft - turns the eyes of the character towards a point (the camera, limited to `angle`
// around the eye direction), or the head of another actor towards `target`
void Actor::fn_80012D34(Actor* target, int node, const atn::Vector3& cam, int arg4, const atn::Vector3& jitter,
                        int arg6, float weight, float angle) {
    if (m20 != 7 && m20 != 1) {
        return;
    }
    if (arg6 != 0) {
        atn::Vector3 pos(cam);
        atn::Matrix3x4* eye = (atn::Matrix3x4*)fn_800128FC()->GetModel_unk()->GetNodeMtx_unk("eye_R");
        atn::Vector3 dir(0.0f, 0.0f, (pos - eye->GetTrans_unk()).Length_unk());
        dir.fn_80011410(eye->GetRotation_unk());
        atn::Vector3 aim = (cam - eye->GetTrans_unk()).ToRotation_unk();
        float dx = atn::WrapAngle_unk(aim.x - dir.ToRotation_unk().x);
        float dy = atn::WrapAngle_unk(aim.y - dir.ToRotation_unk().y);
        if ((atn::Abs_unk(dx) <= angle && atn::Abs_unk(dy) <= angle) || fn_80011220(angle) < 1.1920929e-07f) {
            pos = cam;
        } else {
            atn::Vector3 rot(angle, angle, 0.0f);
            if (atn::Abs_unk(dx) <= angle) {
                rot.x = dx;
            } else if (dx <= 0.0f) {
                rot.x *= -1.0f;
            }
            if (atn::Abs_unk(dy) <= angle) {
                rot.y = dy;
            } else if (dy <= 0.0f) {
                rot.y *= -1.0f;
            }
            dir.fn_80011410(fn_800109E0(rot));
            pos = eye->GetTrans_unk() + dir;
        }
        if (fn_80013444(&jitter, &lbl_807C7030)) {
            atn::Vector3 base = fn_80010374();
            atn::Vector3 d = pos - base;
            d.fn_80011410(fn_800109E0(jitter));
            pos = base + d;
        }
        fn_800134E0(pos, weight);
    } else if (target->fn_80012A1C() != 0) {
        atn::Matrix3x4* mtx = (atn::Matrix3x4*)target->fn_80012A1C()->GetNodeMtxByHash_unk(node);
        if (mtx == 0) {
            return;
        }
        atn::Vector3 aim(0.0f, 0.0f, 0.0f);
        if (arg4 != 0) {
            atn::Vector3 d(0.0f, 0.0f, 0.0f);
            if (fn_80013444(&cam, &lbl_807C7030)) {
                atn::Vector3 base = fn_80010374();
                d = mtx->GetTrans_unk() - base;
                d.fn_80011410(fn_800109E0(cam));
                d += base;
                d -= mtx->GetTrans_unk();
                aim = d;
            }
        } else {
            aim += cam;
        }
        if (fn_80013444(&jitter, &lbl_807C7030)) {
            atn::Vector3 d(0.0f, 0.0f, 0.0f);
            atn::Vector3 base = fn_80010374();
            d = mtx->GetTrans_unk() + aim - base;
            d.fn_80011410(fn_800109E0(jitter));
            d += base;
            d -= mtx->GetTrans_unk();
            aim = d;
        }
        fn_80013508((float*)mtx, aim, weight);
    } else {
        atn::Vector3 base = fn_80010374();
        atn::Vector3 pos = target->fn_80010374();
        if (arg4 != 0) {
            pos = cam + jitter;
            fn_800134B8(pos, weight);
        } else {
            pos += cam;
            if (fn_80013444(&jitter, &lbl_807C7030)) {
                atn::Vector3 d = pos - base;
                d.fn_80011410(fn_800109E0(jitter));
                pos = base + d;
            }
            fn_800134E0(pos, weight);
        }
    }
}

void Actor::fn_800134B8(const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_8012ABD0(pos, speed);
    }
}

void Actor::fn_800134E0(const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_8012ABF4(pos, speed);
    }
}

void Actor::fn_80013508(float* target, const atn::Vector3& pos, float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_8012AC18(target, pos, speed);
    }
}

void Actor::fn_80013530(float speed) {
    if (m20 == 7 || m20 == 1) {
        GetChara_unk()->mLook.fn_8012AC40(speed);
    }
}

} // namespace Event
