#include "Event/Behavior.h"
#include "Event/Action.h"
#include "Event/Channel.h"

namespace Event {

Action::Action(Channel* channel, int index)
    : mChannel(channel), m0C(0), m10(0), m14(0), m18(0), m1C(0) {
    fn_805377F0(channel->m0C, index);
}

ActionOwner_unk* ActionArg_unk::fn_805381A0() {
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

EventManager_unk* ActionArg_unk::fn_805381C8() {
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
    v.x = GetParam_unk(n)->m04.f;
    v.y = GetParam_unk(n + 1)->m04.f;
    v.z = GetParam_unk(n + 2)->m04.f;
    return v;
}

} // namespace Event

namespace Event {

atn::Vector3 Action::fn_80538B18(int n) {
    atn::Vector3 v;
    v.x = GetParam_unk(n)->m04.f;
    v.y = GetParam_unk(n + 1)->m04.f;
    v.z = GetParam_unk(n + 2)->m04.f;
    void* context = reinterpret_cast<void**>(this)[2];
    context = reinterpret_cast<void**>(context)[2];
    context = reinterpret_cast<void**>(context)[2];
    return reinterpret_cast<EventManager_unk**>(context)[2]->fn_80541C94(v);
}

atn::Vector3 Action::fn_80538C14(int n) {
    atn::Vector3 v;
    v.x = GetParam_unk(n)->m04.f;
    v.y = GetParam_unk(n + 1)->m04.f;
    v.z = GetParam_unk(n + 2)->m04.f;
    return v;
}

void Action::fn_80538CE0(int n, const atn::Vector3& v) {
    SetFloat_unk(n, v.x);
    SetFloat_unk(n + 1, v.y);
    SetFloat_unk(n + 2, v.z);
}

int Action::fn_805391E4(unsigned int hash) {
    CommandDef_unk* def = fn_800081C0(m0C);
    // inferred: empty asserts of the release build (with them it is not inlined into its callers,
    // as in the original)
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ParamDef_unk* params = def->mParams;
    for (int i = 0; i < def->mParamCount; i++) {
        if (def->mVariantParam >= 0 && i >= def->mVariantParam) {
            params = def->mVariants[GetParam_unk(def->mVariantParam)->m04.i];
        }
        if (hash == params[i].mHash) {
            return i;
        }
    }
    return -1;
}

ActionValue_unk* Action::fn_805392BC(unsigned int hash) {
    int i = fn_805391E4(hash);
    if (i != -1) {
        return GetParam_unk(i);
    }
    return 0;
}

ActionValue_unk* Action::fn_80539330(unsigned int hash) {
    int i = fn_805391E4(hash);
    if (i != -1) {
        return GetParam_unk(i);
    }
    return 0;
}

// NONMATCHING: 61.9% - scheduling: the original loads m10 and the 0x4330 high word before the stack
// frame is made (tried locals for start/end/length, float and int temporaries)
float Action::fn_805393A4(int frame) {
    return (float)(frame - m10) / (float)(m14 - m10);
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
    GetParam_unk(n)->m04.i = value;
}

Action::~Action() {}

} // namespace Event
