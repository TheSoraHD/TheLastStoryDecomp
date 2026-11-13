#include "Event/Behavior.h"
#include "Event/Action.h"
#include "Event/Channel.h"
#include "Event/Section.h"
#include "Event/Cut.h"
#include "atn/Model_unk.h"
#include "game/MslSort_unk.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

extern "C" Event::ParamDef_unk* fn_80008210(int id, int* count, int variant); // inferred: attributes of a command

namespace Event {

Action::Action(Channel* channel, int index)
    : mChannel(channel), m0C(0), m10(0), m14(0), m18(0), m1C(0) {
    fn_805377F0(channel->m0C, index);
}

// inferred: sets the action to the command `id` (its variant `variant`): the attributes take their defaults
// and the curve goes from (0, 1) to (1, 1)
// NONMATCHING: 94.2% - the inlined resize of the attributes: the original fills the new elements with
// {0x28, 0} (a constructor of ActionParam_unk; with it in Event/Action.h: 92.1%, not kept) and lays out its
// stack temporaries in 8 bytes less
void Action::fn_805377F0(int id, int variant) {
    m0C = 0;
    m10 = 0;
    m14 = 0;
    mParams.clear();
    mKeys.clear();
    m38 = 0;
    int count;
    ParamDef_unk* defs = fn_80008210(id, &count, variant);
    if (defs == 0 || count == 0) {
        return;
    }
    m0C = id;
    mParams.resize(count);
    for (int i = 0; i < count; i++) {
        mParams[i].m04.i = defs[i].mDefault.mValue;
        mParams[i].m00 = defs[i].mType;
    }
    ActionKey_unk start;
    start.mTime = 0.0f;
    start.mValue = 1.0f;
    mKeys.push_back(start);
    ActionKey_unk end;
    end.mTime = 1.0f;
    end.mValue = 1.0f;
    mKeys.push_back(end);
}

ActionOwner_unk* Action::fn_805381A0() {
    void* context = reinterpret_cast<void**>(this)[2];
    return reinterpret_cast<ActionOwner_unk**>(context)[2];
}

} // namespace Event

extern "C" void* fn_805381AC(void* self) {
    void* context = reinterpret_cast<void**>(self)[2];
    return reinterpret_cast<void**>(context)[2];
}

extern "C" void* fn_805381B8(void* self) {
    void* context = reinterpret_cast<void**>(self)[2];
    context = reinterpret_cast<void**>(context)[2];
    return reinterpret_cast<void**>(context)[2];
}

namespace Event {

EventManager_unk* Action::fn_805381C8() {
    void* context = reinterpret_cast<void**>(this)[2];
    context = reinterpret_cast<void**>(context)[2];
    context = reinterpret_cast<void**>(context)[2];
    return reinterpret_cast<EventManager_unk**>(context)[2];
}

} // namespace Event

extern "C" void* fn_805381DC(void* self) {
    void* context = reinterpret_cast<void**>(self)[2];
    context = reinterpret_cast<void**>(context)[2];
    context = reinterpret_cast<void**>(context)[2];
    return reinterpret_cast<void**>(context)[2];
}

namespace Event {

atn::Vector3 Action::fn_805381F0(int n) {
    atn::Vector3 v;
    v.x = GetParamAt_unk(n)->m04.f;
    v.y = GetParamAt_unk(n + 1)->m04.f;
    v.z = GetParamAt_unk(n + 2)->m04.f;
    return v;
}

} // namespace Event

namespace Event {

// NONMATCHING: 98.3% - scheduling of the first mChannel load (as fn_805389B8)
atn::Vector3 Action::fn_805382BC(int n) {
    EventManager_unk* manager = (EventManager_unk*)mChannel->mSection->mCut->mSequence;
    Actor* actor = manager->FindActor_unk(GetParamAt_unk(n)->m04.i);
    atn::Vector3 v;
    v.x = GetParamAt_unk(n + 1)->m04.f;
    v.y = GetParamAt_unk(n + 2)->m04.f;
    v.z = GetParamAt_unk(n + 3)->m04.f;
    if (actor->m20 == 6) {
        return manager->fn_80541BD8(v);
    }
    Mtx m;
    PSMTXRotRad(m, 'y', actor->fn_80011964());
    PSMTXMultVec(m, &v, &v);
    v += actor->fn_80010374();
    return v;
}

// inferred: position of the parameters n+2..n+4 relative to the node n+1 of the actor n (turned by the yaw
// of the node), or to the actor itself when it has no model or no node
// NONMATCHING: 99.37% - order of the store of r.z in the inlined Vector3::RotZ_unk (math/Vec.h)
atn::Vector3 Action::fn_805384CC(int n) {
    EventManager_unk* manager = (EventManager_unk*)mChannel->mSection->mCut->mSequence;
    Actor* actor = manager->FindActor_unk(GetParamAt_unk(n)->m04.i);
    atn::Vector3 v;
    v.x = GetParamAt_unk(n + 2)->m04.f;
    v.y = GetParamAt_unk(n + 3)->m04.f;
    v.z = GetParamAt_unk(n + 4)->m04.f;
    if (actor->m20 == 6) {
        return manager->fn_80541BD8(v);
    }
    atn::Model* model = actor->fn_80012A1C();
    unsigned int node = GetParamAt_unk(n + 1)->m04.i;
    if (model != 0 && node != 0) {
        MtxPtr mtx = model->GetNodeMtxByHash_unk(node);
        if (mtx != 0) {
            const atn::Matrix3x4* nm = (const atn::Matrix3x4*)mtx;
            atn::Vector3 rot = nm->GetRow_unk(2).ToRotationZ_unk();
            Mtx m;
            PSMTXRotRad(m, 'y', rot.y);
            PSMTXMultVec(m, &v, &v);
            v += nm->GetTrans_unk();
        }
    } else {
        Mtx m;
        PSMTXRotRad(m, 'y', actor->fn_80011964());
        PSMTXMultVec(m, &v, &v);
        v += actor->fn_80010374();
    }
    return v;
}

// NONMATCHING: 96.9% - scheduling: the original loads mChannel before the first range check (same in
// fn_805382BC); tried the fn_805381C8 accessor, a typed chain and the id in a local first
MtxPtr Action::fn_805389B8(int n) {
    // inferred: the event manager is the Sequence of the cut (fn_80541C94 is in Event/Sequence)
    EventManager_unk* manager = (EventManager_unk*)mChannel->mSection->mCut->mSequence;
    Actor* actor = manager->FindActor_unk(GetParamAt_unk(n)->m04.i);
    atn::Vector3 pos = lbl_807C7030; // inferred: unused (only the copy is left)
    MtxPtr mtx = 0;
    atn::Model* model = actor->fn_80012A1C();
    if (model != 0) {
        mtx = model->GetNodeMtx_unk(model->fn_800928B0(GetParamAt_unk(n + 1)->m04.i, 0));
    }
    return mtx;
}

atn::Vector3 Action::fn_80538B18(int n) {
    atn::Vector3 v;
    v.x = GetParamAt_unk(n)->m04.f;
    v.y = GetParamAt_unk(n + 1)->m04.f;
    v.z = GetParamAt_unk(n + 2)->m04.f;
    void* context = reinterpret_cast<void**>(this)[2];
    context = reinterpret_cast<void**>(context)[2];
    context = reinterpret_cast<void**>(context)[2];
    return reinterpret_cast<EventManager_unk**>(context)[2]->fn_80541C94(v);
}

atn::Vector3 Action::fn_80538C14(int n) {
    atn::Vector3 v;
    v.x = GetParamAt_unk(n)->m04.f;
    v.y = GetParamAt_unk(n + 1)->m04.f;
    v.z = GetParamAt_unk(n + 2)->m04.f;
    return v;
}

void Action::fn_80538CE0(int n, const atn::Vector3& v) {
    SetFloat_unk(n, v.x);
    SetFloat_unk(n + 1, v.y);
    SetFloat_unk(n + 2, v.z);
}

void Action::fn_80538DC4(int n) {
    atn::Vector3 v;
    v.x = GetParamAt_unk(n)->m04.f;
    v.y = GetParamAt_unk(n + 1)->m04.f;
    v.z = GetParamAt_unk(n + 2)->m04.f;
    v = fn_805381C8()->fn_80541BD8(v);
    SetTyped_unk(n, 4, v.x);
    SetTyped_unk(n + 1, 5, v.y);
    SetTyped_unk(n + 2, 6, v.z);
}

void Action::fn_80538FD4(int n) {
    atn::Vector3 v;
    v.x = GetParamAt_unk(n)->m04.f;
    v.y = GetParamAt_unk(n + 1)->m04.f;
    v.z = GetParamAt_unk(n + 2)->m04.f;
    v = fn_805381C8()->fn_80541C94(v);
    SetTyped_unk(n, 1, v.x);
    SetTyped_unk(n + 1, 1, v.y);
    SetTyped_unk(n + 2, 1, v.z);
}

int Action::fn_805391E4(unsigned int hash) {
    const CommandDef_unk* def = fn_800081C0(m0C);
    // inferred: empty asserts of the release build (with them it is not inlined into its callers,
    // as in the original)
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ParamDef_unk* params = def->mParams;
    for (int i = 0; i < def->mParamCount; i++) {
        if (def->mVariantParam >= 0 && i >= def->mVariantParam) {
            params = def->mVariants[GetParamAt_unk(def->mVariantParam)->m04.i];
        }
        if (hash == params[i].mHash) {
            return i;
        }
    }
    return -1;
}

ActionParam_unk* Action::fn_805392BC(unsigned int hash) {
    int i = fn_805391E4(hash);
    if (i != -1) {
        return GetParamAt_unk(i);
    }
    return 0;
}

ActionParam_unk* Action::fn_80539330(unsigned int hash) {
    int i = fn_805391E4(hash);
    if (i != -1) {
        return GetParamAt_unk(i);
    }
    return 0;
}

// inferred: const (m10 is loaded before the stack frame is made)
float Action::fn_805393A4(int frame) const {
    return (float)(frame - m10) / (float)(m14 - m10);
}

bool KeyLess_unk(const ActionKey_unk& a, const ActionKey_unk& b);

// inferred: clamp of a key component to [lo, hi]
inline float KeyClamp_unk(float v, float lo, float hi) {
    v = (hi <= v) ? hi : v;
    return (lo >= v) ? lo : v;
}

// inferred: index of the key at `time` (-1: none)
inline int KeyFind_unk(std::vector<ActionKey_unk>& keys, float time) {
    for (int i = 0; i < (int)keys.size(); i++) {
        if (time == keys[i].mTime) {
            return i;
        }
    }
    return -1;
}

// NONMATCHING: 96.7% - the original copies the keys member by member (floats) in the reallocation of
// push_back and in the sorts (72-56%): ActionKey_unk is probably not a POD there (a constructor in
// Event/Action.h renumbers the @N labels of every Event file, so it is not added; with it: median 100%,
// sorts 87%, this function 90.8%)
// inferred: sets the key of the curve at its time (time and value clamped to [0, 1]), keeps the curve
// sorted and returns the index of the key
int Action::fn_805393F8(const ActionKey_unk& key) {
    ActionKey_unk k;
    k.mTime = key.mTime;
    k.mValue = key.mValue;
    k.mTime = KeyClamp_unk(k.mTime, 0.0f, 1.0f);
    k.mValue = KeyClamp_unk(k.mValue, 0.0f, 1.0f);
    int index = KeyFind_unk(mKeys, k.mTime);
    if (index == -1) {
        mKeys.push_back(k);
    } else {
        mKeys[index] = k;
    }
    MslSort_unk(MslIter_unk<ActionKey_unk>(mKeys.begin()), MslIter_unk<ActionKey_unk>(mKeys.end()), KeyLess_unk);
    for (int i = 0; i < (int)mKeys.size(); i++) {
        if (k.mTime == mKeys[i].mTime && k.mValue == mKeys[i].mValue) {
            return i;
        }
    }
    return -1;
}

// inferred: comparator of the curve keys (its address is passed to std::sort by fn_805393F8)
bool KeyLess_unk(const ActionKey_unk& a, const ActionKey_unk& b) {
    return a.mTime < b.mTime;
}

// NONMATCHING: 89% - the original reloads the key buffer (m20) for the comparison in the loop but
// uses the one loaded at the start for the interpolation (tried &mKeys[0] locals, begin(), at())
float Action::fn_8053A2E8(float t) {
    if (mKeys.size() == 0) {
        return 0.0f;
    }
    if (t <= mKeys[0].mTime) {
        return mKeys[0].mValue;
    }
    int last = mKeys.size() - 1;
    if (t >= mKeys[last].mTime) {
        return mKeys[last].mValue;
    }
    for (int i = 0; i < last; i++) {
        if (t <= mKeys[i + 1].mTime) {
            ActionKey_unk& a = mKeys[i];
            ActionKey_unk& b = mKeys[i + 1];
            if (a.mTime == b.mTime) {
                return 0.5f * (a.mValue + b.mValue);
            }
            return (t - a.mTime) / (b.mTime - a.mTime) * (b.mValue - a.mValue) + a.mValue;
        }
    }
    return 0.0f;
}

void Action::vf0C(int n, int value) {
    GetParamAt_unk(n)->m04.i = value;
}

Action::~Action() {}

} // namespace Event
