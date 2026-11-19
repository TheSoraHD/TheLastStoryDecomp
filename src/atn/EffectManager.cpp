#include "atn/Task.h"
#include "atn/Model_unk.h"
#include "atn/Memory.h"
#include "atn/Array_unk.h"
#include "math/Vec.h"
#include "std/vector.h"
#include "atn/EffectSystem_unk.h"

// The real file (0x80239798-0x8024373C, fused with the former std/char_traits_char_8023C080 and
// atn/EffectManager_cont units) goes on after 0x8023B9D0 with the std:: template code it instantiates
// (iostream facets, up to 0x802411B8) and the EfpInfoList loader, and ends with the this-adjusting
// thunks of EffectManager for its IRender base (0x8024372C/0x80243734, emitted by MWCC at the end).

#include "atn/EffectManager.h"
#include "atn/EffectApi_unk.h"
#include "atn/RenderManager.h"

#include "atn/EffectParams_unk.h"
#include "std/locale_facets_unk.h"
#include "atn/EfpData_unk.h"

namespace atn {

EffectManager* EffectManager::fn_80239798(Task* parent, int a, int b, int c, unsigned int max) {
    if (lbl_8087F3C0 == 0 && parent != 0) {
        lbl_8087F3C0 = new ((MemoryTag)7, "", "", 0) EffectManager(parent, a, b, c, max);
    }
    return lbl_8087F3C0;
}

EffectManager::EffectManager(Task* parent, int a, int b, int c, unsigned int max)
    : Task(parent), mSerial(1), mPaused(0), mFlags(0), mCheckTrace(0), mDrawArg(0) {
    ((void)0); // inferred: keeps the constructor out of fn_80239798, as in the original
    mSystem.fn_8022B850(a, b, c);
    mMax = max;
    mEntries.reserve(max, 7);
}

EffectManager::~EffectManager() {
    mSystem.fn_8022C2A0();
    lbl_8087F3C0 = 0;
}

void EffectManager::vf10() {
    fn_8023AE34(0, 0, -1);
    mSystem.fn_8023686C(1.0f);
}

void EffectManager::fn_80239C14() {
    mSystem.fn_8023686C(1.0f);
}

void EffectManager::fn_80239C20() {
    if (mSystem.mUpdating == 0) {
        mSystem.mUpdating = 1;
        mSystem.fn_8022C3F0();
        mSystem.mUpdating = 0;
    }
}

void EffectManager::vf14() {
    mSystem.fn_8022C3F0();
    atn::RenderManager::instance_->AddRender(static_cast<IRender*>(this), 0xB, 0.0f);
    if ((mSystem.mFlags & 1) == 1) {
        atn::RenderManager::instance_->AddRender(static_cast<IRender*>(this), 6, 0.0f);
    }
    if ((mSystem.mFlags & 8) == 8) {
        atn::RenderManager::instance_->AddRender(static_cast<IRender*>(this), 4, -10000000.0f);
    }
}

void EffectManager::vf10(RenderManager* view) {
    mSystem.fn_802368B0(mDrawArg);
    mDrawArg = 0;
}

} // namespace atn

using atn::EffectManager;

extern "C" int fn_80239D58(EffectManager* mgr, unsigned int owner, int group) {
    for (EffectEntry_unk* it = mgr->mEntries.mData; it != mgr->mEntries.mData + mgr->mEntries.mSize; it++) {
        if (it->mOwner == owner && (it->mGroup == group || group == -2)) {
            return 1;
        }
    }
    return mgr->mSystem.fn_80232B98(owner, group);
}

// NONMATCHING: 97% - the original reloads the end of the array in the erase (ours reuses the loop test)
extern "C" void fn_80239DAC(EffectManager* mgr, unsigned int owner, int group, int flag) {
    int found = 0;
    EffectEntry_unk* it = mgr->mEntries.mData;
    while (it != mgr->mEntries.mData + mgr->mEntries.mSize) {
        if (it->mOwner == owner && (it->mGroup == group || group == -2)) {
            if (flag == 0) {
                it = mgr->Erase_unk(it);
            } else {
                it->mFrame = -1;
                it++;
            }
            found = 1;
        } else {
            it++;
        }
    }
    if (mgr->mSystem.mInsts != 0) {
        int r = mgr->mSystem.fn_80232D04(owner, group, flag);
        found = (found != 0 || r > 0) ? 1 : 0;
    }
    atn::EffectManagerCallback* listener = mgr->mSystem.mListener;
    if (listener != 0 && mgr->mSystem.mReplaying == 0 && found != 0) {
        EffectEvent_unk event;
        event.mType = 1;
        event.mRes = 0;
        event.mOwner = owner;
        event.mGroup = group;
        event.mFlags = 0;
        event.m5C = flag;
        event.m58 = 0;
        event.mHandle = 0;
        event.mLayer = mgr->mSystem.mLayer;
        listener->vf08(&event);
    }
}

// inferred: int index converted to the unsigned operator[] (the index is incremented before the offset)
extern "C" void fn_8023A02C(EffectManager* mgr, unsigned int owner, int group, unsigned int flags) {
    EffectEntry_unk* e;
    for (int i = 0; i < mgr->mEntries.mSize; i++) {
        e = &mgr->mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->mFlags |= flags;
        }
    }
    if (mgr->mSystem.mInsts != 0) {
        mgr->mSystem.fn_80232FF4(owner, group, flags);
    }
}

// inferred: int index converted to the unsigned operator[] (the index is incremented before the offset)
extern "C" void fn_8023A098(EffectManager* mgr, unsigned int owner, int group, unsigned int flags) {
    EffectEntry_unk* e;
    unsigned int mask = ~flags;
    for (int i = 0; i < mgr->mEntries.mSize; i++) {
        e = &mgr->mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->mFlags &= mask;
        }
    }
    if (mgr->mSystem.mInsts != 0) {
        mgr->mSystem.fn_802330A4(owner, group, flags);
    }
}

extern "C" void fn_8023A108(EffectManager* mgr, unsigned int owner, int group, int count) {
    for (int i = 0; i < count; i++) {
        mgr->fn_8023AE34(owner, group, -1);
    }
    if (mgr->mSystem.mInsts != 0) {
        mgr->mSystem.fn_80232BEC(owner, group, count);
    }
}

namespace atn {

// inferred: int index converted to the unsigned operator[] (the index is incremented before the offset)
void EffectManager::fn_8023A184(unsigned int owner, int group, const EffectColor_unk* color) {
    EffectEntry_unk* e;
    for (int i = 0; i < mEntries.mSize; i++) {
        e = &mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->mColor.r = color->r;
            e->mColor.g = color->g;
            e->mColor.b = color->b;
            e->mColor.a = color->a;
        }
    }
    mSystem.fn_80233158(owner, group, color);
}

void EffectManager::fn_8023A1F4(unsigned int owner, int group, const atn::Vector3* vec) {
    EffectEntry_unk* e;
    for (int i = 0; i < mEntries.mSize; i++) {
        e = &mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->m34 = *vec;
        }
    }
    mSystem.fn_802331B0(owner, group, vec);
}

// inferred: int index converted to the unsigned operator[] (the index is incremented before the offset)
void EffectManager::fn_8023A254(unsigned int owner, int group, float rate) {
    EffectEntry_unk* e;
    for (int i = 0; i < mEntries.mSize; i++) {
        e = &mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->mRate = rate;
        }
    }
    mSystem.fn_80233260(owner, group, rate);
}

void EffectManager::fn_8023A2A8(void* owner) {
    mSystem.fn_80232ACC(owner);
    NotifyStop_unk(owner, 0, 0, 0, 0, 0, 0);
}

void EffectManager::fn_8023A340(EffectRes_unk* res) {
    EffectEntry_unk* it = mEntries.mData;
    while (it != mEntries.mData + mEntries.mSize) {
        if (it->mRes == res) {
            EfpData_unk* emitters = res->mData;
            for (unsigned int i = 0; i < emitters->mCount; i++) {
                if (it->mEmitter < 0 || it->mEmitter == i) {
                    if (emitters->mElems[i].m00 != 0) {
                        int handle = it->mHandle + i;
                        StopEmitter_unk(handle, 0);
                    }
                }
            }
            it = Erase_unk(it);
        } else {
            it++;
        }
    }
}

void EffectManager::fn_8023A5A8(int clear) {
    if (clear != 0) {
        mEntries.clear();
    }
    mSystem.fn_80232E48();
}

void EffectManager::fn_8023A60C(int layer) {
    mSystem.fn_80232ED4(layer);
}

void EffectManager::fn_8023A614(int arg) {
    fn_8023AE34(0, 0, arg);
    mSystem.fn_80232F84(arg);
}

void EffectManager::fn_8023A664(void* arg) {
    mDrawArg = arg;
    vf14();
}

void EffectManager::fn_8023A678(unsigned int max) {
    mSystem.fn_8022BEAC(max);
}

} // namespace atn

// NONMATCHING: 99% - the original loads res->mData before storing the stack arguments; its "ef_mr" is at
// offset 0xD of the pool of the whole file (outside this split)
// note: the last integer argument is not used
extern "C" void* fn_8023A680(EffectManager* mgr, EffectRes_unk* res, int a, int b, int c, const atn::Vector3* pos,
                             const atn::Vector3* rot, const EffectColor_unk* color, int d, int e, int f, float scale) {
    if (res->mData == 0) {
        return 0;
    }
    fn_80232AC4(res);
    void* effect = mgr->mSystem.fn_8022FAC4(res->mData, a, b, c, pos, rot, color, d, e, scale);
    if (mgr->mCheckTrace != 0) {
        unsigned int flags = 0;
        if (strstr(((atn::LoadHandle*)res->m04)->fn_80473F18(), "ef_mr") != 0) {
            flags |= 0x18;
        }
        if (flags != 0) {
            mgr->mSystem.fn_802348E4(effect, flags);
        }
    }
    if (mgr->mSystem.mListener != 0 && mgr->mSystem.mReplaying == 0) {
        EffectEvent_unk event;
        event.mType = 0;
        event.mRes = res;
        event.mOwner = fn_80232B88();
        event.mGroup = fn_80232B90();
        event.mFlags = mgr->mFlags;
        event.m58 = fn_802346B4();
        event.mHandle = (int)effect;
        event.mModel = (void*)a;
        event.m0C = b;
        event.m10 = c;
        event.mPos = (pos != 0) ? *pos : atn::Vector3(0.0f, 0.0f, 0.0f);
        event.mRot = (rot != 0) ? *rot : atn::Vector3(0.0f, 0.0f, 0.0f);
        event.mScale = scale;
        event.mColor = (color != 0) ? *color : EffectColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        event.m40 = d;
        event.m44 = e;
        event.mLayer = mgr->mSystem.mLayer;
        mgr->mSystem.mListener->vf08(&event);
    }
    return effect;
}

extern "C" int fn_8023A8B4(EffectManager* mgr, EffectRes_unk* res, void* model, void* node, const atn::Vector3& pos,
                           const atn::Vector3& rot, const EffectColor_unk& color, int emitter, int arg8, int arg9,
                           float scale) {
    if (res->mData == 0) {
        return 0;
    }
    if (mgr->mEntries.mSize >= mgr->mMax) {
        return 0;
    }
    mgr->mSerial++;
    if (mgr->mSerial >= 0x10000) {
        mgr->mSerial = 1;
    }
    EffectEntry_unk entry;
    entry.mRes = res;
    entry.mOwner = fn_80232B88();
    entry.mGroup = fn_80232B90();
    entry.mFlags = mgr->mFlags;
    if (mgr->mSystem.m78 != 0) {
        entry.mFlags |= 0x20;
    }
    if (mgr->mSystem.m7C != 0) {
        entry.mFlags |= 0x100;
    }
    if (node != 0) {
        entry.mTarget = node;
        entry.mFlags |= 0x10000;
    } else {
        entry.mTarget = model;
    }
    entry.mFrame = 0;
    entry.mPos = pos;
    entry.mRot = rot;
    entry.mScale = scale;
    entry.m34.x = 0.0f;
    entry.m34.y = 0.0f;
    entry.m34.z = 0.0f;
    entry.mHandle = mgr->mSerial << 16;
    entry.mEmitter = emitter;
    entry.m46 = arg8;
    entry.mLayer = mgr->mSystem.mLayer;
    entry.m4A = mgr->mSystem.m70;
    entry.mColor = color;
    entry.mRate = 1.0f;
    entry.mTime = 0.0f;
    mgr->mEntries.push_back(entry);
    return mgr->mSerial;
}

namespace atn {

// NONMATCHING: 74% - advances the entries and emits their particles; written from the original, the
// register allocation and the matrix copies differ
void EffectManager::fn_8023AE34(unsigned int owner, int group, int arg) {
    if (mPaused != 0) {
        return;
    }
    EffectEntry_unk* it = mEntries.mData;
    while (it != mEntries.mData + mEntries.mSize) {
        if ((it->mFlags & 2) == 2) {
            it++;
            continue;
        }
        if (it->mFrame < 0) {
            if (mSystem.fn_802346BC(it->mHandle) == 0) {
                it = Erase_unk(it);
            } else {
                it++;
            }
            continue;
        }
        if (owner != 0 && !(it->mOwner == owner && (it->mGroup == group || group == -2))) {
            it++;
            continue;
        }
        if (arg >= 0 && it->mLayer != arg) {
            it++;
            continue;
        }
        it->mTime = it->mRate * lbl_8087EFA8->mTimeScale + it->mTime;
        while (it->mTime >= 1.0f) {
            EfpData_unk* emitters = it->mRes->mData;
            for (unsigned int i = 0; i < emitters->mCount; i++) {
                if (it->mEmitter >= 0 && it->mEmitter != i) {
                    continue;
                }
                EfpElem_unk* em = &emitters->mElems[i];
                if (em->m00 == 0) {
                    continue;
                }
                int handle = it->mHandle + i;
                if (em->mStart == it->mFrame) {
                    void* target = it->mTarget;
                    int mode = em->mParent;
                    if (target != 0) {
                        if ((it->mFlags & 0x10000) == 0x10000) {
                            if (mode == 3) {
                                target = (unsigned char*)target + 0x30;
                                mode = 5;
                            } else if (mode == 4) {
                                int index = ((atn::Model*)(*(unsigned char**)((unsigned char*)target + 4) + 0x10))->fn_800928B0(em->mNode, 0);
                                void* nodes;
                                if (index >= 0 && (nodes = *(void**)((unsigned char*)it->mTarget + 0x80)) != 0) {
                                    target = (unsigned char*)nodes + index * 0x30;
                                    mode = 5;
                                } else {
                                    mode = 0;
                                }
                            }
                        } else if (mode == 4) {
                            int index = ((atn::Model*)target)->fn_800928B0(em->mNode, 0);
                            void* node = (index >= 0) ? *(unsigned char**)((unsigned char*)target + 0x3C) + index * 0x30 : 0;
                            if (node != 0) {
                                target = node;
                                mode = 5;
                            } else {
                                mode = 0;
                            }
                        }
                    } else if (mode > 1) {
                        mode = 0;
                    }
                    atn::Vector3 offset = em->mPos;
                    atn::Vector3 rot = em->mRot;
                    EffectColor_unk color;
                    color.r = em->mColor1 * it->mColor.r;
                    color.g = em->mColor2 * it->mColor.g;
                    color.b = em->mColor3 * it->mColor.b;
                    color.a = em->mColor0 * it->mColor.a;
                    float scale = em->mScale;
                    float m[3][4];
                    float tmp[3][4];
                    float mtx[3][4];
                    m[0][0] = 1.0f;
                    m[0][1] = 0.0f;
                    m[0][2] = 0.0f;
                    m[0][3] = 0.0f;
                    m[1][0] = 0.0f;
                    m[1][1] = 1.0f;
                    m[1][2] = 0.0f;
                    m[1][3] = 0.0f;
                    m[2][0] = 0.0f;
                    m[2][1] = 0.0f;
                    m[2][2] = 1.0f;
                    m[2][3] = 0.0f;
                    if (it->mRot.z != 0.0f) {
                        PSMTXRotRad(tmp, 'z', it->mRot.z);
                        PSMTXConcat(m, tmp, mtx);
                        for (int k = 0; k < 3; k++) {
                            for (int l = 0; l < 4; l++) {
                                m[k][l] = mtx[k][l];
                            }
                        }
                    }
                    if (it->mRot.y != 0.0f) {
                        PSMTXRotRad(tmp, 'y', it->mRot.y);
                        PSMTXConcat(m, tmp, mtx);
                        for (int k = 0; k < 3; k++) {
                            for (int l = 0; l < 4; l++) {
                                m[k][l] = mtx[k][l];
                            }
                        }
                    }
                    if (it->mRot.x != 0.0f) {
                        PSMTXRotRad(tmp, 'x', it->mRot.x);
                        PSMTXConcat(m, tmp, mtx);
                        for (int k = 0; k < 3; k++) {
                            for (int l = 0; l < 4; l++) {
                                m[k][l] = mtx[k][l];
                            }
                        }
                    }
                    PSMTXMultVec(m, &offset, &offset);
                    offset.x *= it->mScale;
                    offset.y *= it->mScale;
                    offset.z *= it->mScale;
                    offset.x += it->mPos.x;
                    offset.y += it->mPos.y;
                    offset.z += it->mPos.z;
                    rot.x += it->mRot.x;
                    rot.y += it->mRot.y;
                    rot.z += it->mRot.z;
                    scale *= it->mScale;
                    mSystem.mLayer = it->mLayer;
                    mSystem.m70 = it->m4A;
                    fn_80232B7C(it->mOwner, it->mGroup);
                    fn_802346AC(handle);
                    fn_8023A680(this, (EffectRes_unk*)&em->mLoader, it->m46, mode, (int)target, &offset, &rot, &color,
                                em->m40, em->m44, 1, scale);
                    mSystem.fn_80234764(handle, &it->m34);
                    mSystem.fn_802346F0(handle, it->mFlags);
                    fn_80232B7C(0, 0);
                    fn_802346AC(0);
                    mSystem.m70 = 0;
                    mSystem.mLayer = 0;
                } else if (em->mEnd != 0 && em->mEnd == it->mFrame) {
                    StopEmitter_unk(handle, 1);
                }
            }
            it->mFrame++;
            it->mTime -= 1.0f;
        }
        int length = it->mRes->mData->mLength;
        if (it->mFrame >= length && length != 0 && mSystem.fn_802346BC(it->mHandle) == 0) {
            it = Erase_unk(it);
        } else {
            it++;
        }
    }
}

void EffectManager::fn_8023B71C(EffectManager* mgr, int handle) {
    fn_802346AC(handle);
}

// NONMATCHING: 99.7% - registers of the inlined fn_8023A02C loop
// Replays a recorded event.
int EffectManager::fn_8023B724(EffectEvent_unk* event) {
    int result = 0;
    switch (event->mType) {
    case 0:
        fn_80232B7C(event->mOwner, event->mGroup);
        fn_802346AC(event->m58);
        result = (int)fn_8023A680(this, (EffectRes_unk*)event->mRes, (int)event->mModel, event->m0C, event->m10,
                                  &event->mPos, &event->mRot, &event->mColor, event->m40, event->m44, 1, event->mScale);
        fn_8023A02C(this, event->mOwner, event->mGroup, event->mFlags);
        break;
    case 1:
        if (event->mRes != 0) {
            fn_8023A2A8(event->mRes);
        } else if (event->m58 != 0) {
            StopEmitter_unk(event->m58, event->m5C);
        } else if (event->mHandle != 0) {
            StopEffect_unk((void*)event->mHandle, event->m5C);
        } else {
            fn_80239DAC(this, event->mOwner, event->mGroup, event->m5C);
        }
        break;
    }
    return result;
}

} // namespace atn

// MSL_C math (sign and class of a double)
extern "C" int __signbitd(double);
extern "C" int __fpclassifyd(double);

namespace std {

// inferred: upper case of a range (MSL inline)
inline const char* ctype<char>::toupper(char* low, const char* high) const {
    return do_toupper(low, high);
}

// inferred: the decimal digits of a double (MSL locale.cpp, 0x80693884; see libs/.../MSL_C++/locale.cpp)
struct __decimal_unk {
    __decimal_unk(double);
    int __round_dir_unk(size_t) const;
    string __round_unk(int, long&) const;

    string digits;
    short exp;
    short cls;
};

// inferred: MSL inlines
template <class C>
inline typename numpunct<C>::char_type numpunct<C>::decimal_point() const {
    return do_decimal_point();
}
template <class C>
inline typename numpunct<C>::char_type numpunct<C>::thousands_sep() const {
    return do_thousands_sep();
}
// inferred: weak copy 0x80241C20 (a copy of the string)
inline string ctype<char>::widen(const string& s) const {
    return s;
}

// inferred: one character in place of the contents (MSL inline, declared in std/string.h)
template <class T, class Tr, class A>
inline basic_string<T, Tr, A>& basic_string<T, Tr, A>::operator=(T c) {
    T* p;
    if (is_long()) {
        mLong.mSize = 1;
        p = mLong.mData;
    } else {
        mShortFlags.mSize = 1;
        p = mShort.mData;
    }
    p[0] = c;
    p[1] = T();
    return *this;
}

// NONMATCHING: 81.5% - written from the original, not tuned (registers, stack slots of the iterator tags)
// inferred: scientific format of a finite value (0x802411B8): first digit, the decimal point (with showpoint or a
// precision), precision digits, e or E, the sign and at least two exponent digits; returns the exponent (name
// invented)
template <class C>
int __put_float_scientific_unk(ios_base& str, basic_string<C>& s, long double v) {
    locale loc = str.getloc();
    const ctype<C>& ct = use_facet<ctype<C> >(loc);
    const numpunct<C>& np = use_facet<numpunct<C> >(loc);
    C zero = ct.widen('0');
    C dp = np.decimal_point();
    C e;
    if ((str.flags() & __uppercase_unk) != 0) {
        e = ct.widen('E');
    } else {
        e = ct.widen('e');
    }
    int prec = str.mPrecision;
    if (v == 0.0) {
        s = zero;
        if ((str.flags() & __showpoint_unk) != 0 || prec > 0) {
            s += dp;
        }
        if (prec > 0) {
            s.replace(s.size(), 0, prec, zero);
        }
        s += e;
        s += ct.widen('+');
        s.replace(s.size(), 0, 2, zero);
        return 0;
    }
    __decimal_unk d(v);
    long exp;
    s = ct.widen(d.__round_unk(prec + 1, exp));
    int result = exp;
    int point = 0;
    if ((str.flags() & __showpoint_unk) != 0 || prec > 0) {
        s.replace(1, 0, 1, dp);
        point = 1;
    }
    if (prec + point + 1 > (int)s.size()) {
        s.replace(s.size(), 0, prec + point + 1 - s.size(), zero);
    }
    s += e;
    if (exp < 0) {
        s += ct.widen('-');
        exp = -exp;
    } else {
        s += ct.widen('+');
    }
    if (exp < 10) {
        s += ct.widen('0');
        s += ct.widen((char)('0' + exp));
    } else {
        typename basic_string<C>::size_type start = s.size();
        for (; exp > 0; exp /= 10) {
            s += ct.widen((char)('0' + exp % 10));
        }
        __reverse_unk(&s[0] + start, &s[0] + s.size());
    }
    return result;
}

// NONMATCHING: 85.8% - written from the original, not tuned (registers, stack slots of the iterator tags)
// inferred: fixed format of a finite value (0x80241CB8): the digits rounded to precision places after the
// decimal point, zeros before them for a small value, the decimal point (with showpoint or a precision) and
// the thousands separators of the numpunct (name invented)
template <class C>
void __put_float_fixed_unk(ios_base& str, basic_string<C>& s, long double v) {
    locale loc = str.getloc();
    const ctype<C>& ct = use_facet<ctype<C> >(loc);
    const numpunct<C>& np = use_facet<numpunct<C> >(loc);
    C zero = ct.widen('0');
    C one = ct.widen('1');
    C dp = np.decimal_point();
    int prec = str.mPrecision;
    if (v == 0.0) {
        s = zero;
        if ((str.flags() & __showpoint_unk) != 0 || prec > 0) {
            s += dp;
        }
        s.replace(s.size(), 0, prec, zero);
        return;
    }
    __decimal_unk d(v);
    long exp = d.exp;
    int n = prec + exp + 1;
    if (n < 0) {
        int p = str.mPrecision;
        s = zero;
        if ((str.flags() & __showpoint_unk) != 0 || p > 0) {
            s += dp;
        }
        s.replace(s.size(), 0, p, zero);
        return;
    }
    if (n == 0) {
        if (exp == -1) {
            if (d.__round_dir_unk(0) == 1) {
                s = one;
            } else {
                s = zero;
            }
            if ((str.flags() & __showpoint_unk) != 0) {
                s += dp;
            }
        } else {
            s = zero;
            s += dp;
            if (exp < -2) {
                s.replace(s.size(), 0, -2 - exp, zero);
            }
            if (d.__round_dir_unk(0) == 1) {
                s += one;
            } else {
                s += zero;
            }
        }
        return;
    }
    s = ct.widen(d.__round_unk(n, exp));
    if (exp < 0) {
        basic_string<C> t;
        t = zero;
        t += dp;
        if (exp < -1) {
            t.replace(t.size(), 0, -1 - exp, zero);
        }
        t += s;
        s = t;
        if (prec + 2 - t.size() == 1) {
            s += zero;
        }
    } else {
        s.replace(s.size(), 0, prec + exp + 1 - s.size(), zero);
        if ((str.flags() & __showpoint_unk) != 0 || prec > 0) {
            s.replace(exp + 1, 0, 1, dp);
        }
        __put_grouping_float(dp, np.thousands_sep(), np.grouping(), s);
    }
}

// NONMATCHING: 81.1% - written from the original, not tuned (the last character is dropped in place there)
// inferred: general format (0x802407CC): scientific with precision - 1 digits after the point, turned into the
// fixed form when the exponent is in [-4, precision); without showpoint the trailing zeros and a trailing
// decimal point are removed; the format of the stream is saved and restored (name invented)
template <class C>
void __put_float_general_unk(ios_base& str, basic_string<C>& s, long double v) {
    __save_format_unk save(str);
    int prec = str.mPrecision;
    if (prec == 0) {
        prec = 1;
    }
    str.mPrecision = prec - 1;
    int exp = __put_float_scientific_unk(str, s, v);
    locale loc = str.getloc();
    const ctype<C>& ct = use_facet<ctype<C> >(loc);
    const numpunct<C>& np = use_facet<numpunct<C> >(loc);
    C zero = ct.widen('0');
    C dp = np.decimal_point();
    if (exp >= -4 && exp < prec) {
        if (s.data()[1] == dp) {
            s.erase(prec + 1);
        } else {
            s.erase(1);
        }
        if (exp < 0) {
            if (s.data()[1] == dp) {
                s.erase(1, 1);
            }
            s.replace(0, 0, -exp, zero);
            s.replace(1, 0, 1, dp);
        } else if (exp > 0) {
            s.erase(1, 1);
            if (exp < prec - 1 || (str.flags() & __showpoint_unk) != 0) {
                s.replace(exp + 1, 0, 1, dp);
            }
        }
        if ((str.flags() & __showpoint_unk) == 0 && s.find(dp) != basic_string<C>::npos) {
            const C* end = s.data() + s.size();
            const C* q = end - 1;
            while (*q == zero) {
                --q;
            }
            ++q;
            if (q != end) {
                s.erase(q - s.data(), end - q);
            }
            if (s.data()[s.size() - 1] == dp) {
                s.erase(s.size() - 1);
            }
        }
        __put_grouping_float(dp, np.thousands_sep(), np.grouping(), s);
    } else {
        if ((str.flags() & __showpoint_unk) == 0 && s.data()[1] == dp) {
            C e;
            if ((str.flags() & __uppercase_unk) != 0) {
                e = ct.widen('E');
            } else {
                e = ct.widen('e');
            }
            const C* p = s.data() + s.find(e);
            const C* q = p - 1;
            while (*q == zero) {
                --q;
            }
            ++q;
            if (q != p) {
                s.erase(q - s.data(), p - q);
            }
            if (s.data()[2] == e) {
                s.erase(1, 1);
            }
        }
    }
}

// inferred: a finite value formatted fixed, scientific or general after the flags, else nan or inf (inlined by
// do_put(long double); name invented)
template <class C>
inline void __put_float_unk(ios_base& str, basic_string<C>& s, long double v) {
    if (__fpclassifyd(v) > 2) {
        switch (str.flags() & (__fixed_unk | __scientific_unk)) {
        case __fixed_unk:
            __put_float_fixed_unk(str, s, v);
            break;
        case __scientific_unk:
            __put_float_scientific_unk(str, s, v);
            break;
        default:
            __put_float_general_unk(str, s, v);
            break;
        }
    } else {
        if (__fpclassifyd(v) == 1) {
            s = __literal("nan", C());
        } else {
            s = __literal("inf", C());
        }
        if ((str.flags() & __uppercase_unk) != 0) {
            const ctype<C>& ct = use_facet<ctype<C> >(str.getloc());
            ct.toupper((C*)s.data(), s.data() + s.size());
        }
    }
}

// NONMATCHING: 85.8% - the string is passed through a saved register in the original, registers of the facet
// (the replace of use_facet), stack slots of the iterator tags of the string assignments (90.2% with the
// ostreambuf_iterator::operator= of docs/paralelo-20261113-qD2.patch, which renumbers GameEventScript)
// inferred: float output (0x8023F5E0): the sign (or + with showpos) as a prefix; a finite value formatted fixed,
// scientific or general after the flags; else nan or inf (upper case with uppercase); then padded like the
// integers
template <class C, class O>
typename num_put<C, O>::iter_type num_put<C, O>::do_put(iter_type out, ios_base& str, char_type fill,
                                                        long double v) const {
    const ctype<C>& ct = use_facet<ctype<C> >(str.getloc());
    C pre[1];
    int npre = 0;
    bool neg = __signbitd(v);
    if (!neg && (str.flags() & __showpos_unk) != 0) {
        npre = 1;
        pre[0] = ct.widen('+');
    } else if (neg) {
        npre = 1;
        pre[0] = ct.widen('-');
        v = -v;
    }
    basic_string<C> s;
    __put_float_unk(str, s, v);
    return __put_padded_unk(out, str, fill, pre, npre, s.data(), (int)s.size());
}

} // namespace std

// inferred: weak copies of the MSL numeric facets used by the stream output of this file (its code that
// used them is not in the DOL), in the order of the DOL
namespace std {
template basic_ostream<char>& basic_ostream<char>::operator<<(int n);
template num_put<char>::~num_put();
template num_put<char>::iter_type num_put<char>::do_put(iter_type out, ios_base& str, char fill, bool v) const;
template num_put<char>::iter_type num_put<char>::do_put(iter_type out, ios_base& str, char fill, long v) const;
template num_put<char>::iter_type num_put<char>::do_put(iter_type out, ios_base& str, char fill,
                                                         unsigned long v) const;
template num_put<char>::iter_type num_put<char>::do_put(iter_type out, ios_base& str, char fill, long long v) const;
template num_put<char>::iter_type num_put<char>::do_put(iter_type out, ios_base& str, char fill,
                                                         unsigned long long v) const;
template num_put<char>::iter_type num_put<char>::do_put(iter_type out, ios_base& str, char fill, double v) const;
template num_put<char>::iter_type num_put<char>::do_put(iter_type out, ios_base& str, char fill,
                                                         long double v) const;
template num_put<char>::iter_type num_put<char>::do_put(iter_type out, ios_base& str, char fill,
                                                         const void* v) const;
template numpunct<char>::string_type numpunct<char>::truename() const;
template numpunct<char>::string_type numpunct<char>::falsename() const;
template numpunct<char>::string_type numpunct<char>::do_truename() const;
template numpunct<char>::string_type numpunct<char>::do_falsename() const;
template void __put_grouping_float<char, string>(char dp, char sep, const string& grouping, string& s);
template int __put_digits_unk<char, unsigned long long>(ios_base& str, unsigned long long v, char* buf,
                                                      const ctype<char>& ct, bool noGroup);
template int __put_digits_unk<char, unsigned long>(ios_base& str, unsigned long v, char* buf, const ctype<char>& ct,
                                                 bool noGroup);
template char numpunct<char>::do_decimal_point() const;
template numpunct<char>::string_type numpunct<char>::grouping() const;
template numpunct<char>::string_type numpunct<char>::do_grouping() const;
template char numpunct<char>::do_thousands_sep() const;
template numpunct<char>::~numpunct();
template locale::id num_put<char>::id;
template locale::id numpunct<char>::id;
} // namespace std
