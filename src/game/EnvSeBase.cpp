#include "game/EnvSeBase.h"
#include "atn/SoundManager.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"
#include "std/vector.h"
#include <boost/tokenizer.hpp>
#include "atn/DrawHelper.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector (.bss, 0x10 bytes)


// inferred: event flags of the field
struct EnvSeField_unk {
    unsigned char m0000[0x54E4];
    int m54E4; // inferred: mode (0 and 8 allow the environment sounds)
};
extern "C" EnvSeField_unk* lbl_8087F430;
extern "C" int fn_80370174(EnvSeField_unk* field, int id);
extern "C" int fn_80370A78(EnvSeField_unk* field, int id);
struct EnvSeMute_unk {
    unsigned char m00[0x70];
    int m70;
};
extern "C" EnvSeMute_unk* lbl_8087F540;
struct EnvSeConfig_unk {
    unsigned char m00[0x7C];
    int m7C; // inferred: debug menus enabled
};
extern "C" EnvSeConfig_unk* lbl_8087F0A8;

// inferred: debug drawing and camera
extern "C" void* lbl_8087EFB4;
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos); // inferred: to screen

// inferred: debug menu entries
typedef void (*EnvSeMenuCallback_unk)(void* arg);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, EnvSeMenuCallback_unk callback,
                            void* arg, float min, float max, float step);
extern "C" void fn_8008771C(void* group, const char* name, float* value, EnvSeMenuCallback_unk callback, void* arg,
                            float min, float max, float step);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step,
                            EnvSeMenuCallback_unk callback, void* arg);

// inferred: shape tests of the collision library
struct EnvSeSphere_unk {
    atn::Vector3 mCenter;
    float mRadius;
};
extern "C" int fn_80051CD8(const EnvSeSphere_unk* sphere, const void* box); // inferred: sphere in box
struct EnvSeSegment_unk {
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};
// inferred: result of the segment test (only the members used here)
struct EnvSeHit_unk {
    EnvSeHit_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 mPos; // 0x10
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
};
extern "C" int fn_8005454C(EnvSeHit_unk* hit, const EnvSeSegment_unk* seg, const void* box); // inferred: segment / box

// inferred: creates the debug menu group `name` under `root` once (names invented)
inline void* LoadMenu_unk(ResHandle& handle, void* root, const char* name) {
    if (handle.mRes == 0 && root != 0 && lbl_8087EF10 != 0) {
        return handle.mRes = fn_8008937C(root, name);
    }
    return 0;
}

// inferred: the name formats end with an extra NUL in the pool ("%s[%d] %s\0"): an empty string
// follows them that the empty literals of the file do not share
static const char* const sTypeNames[] = {"Point", "Line", "Sphere", "Box", "Global", "Area"};

EnvSeBase::~EnvSeBase() {}

void EnvSeBase::fn_80360CDC(atn::Csv* csv) {
    m4C = atoi(csv->fn_8005B9CC());
    csv->fn_8005B9CC();
    mData->fn_80360B40(csv);
    vf1C(csv);
    if (lbl_8087F418 != 0 && lbl_8087F0A8->m7C != 0) {
        void* root = lbl_8087F418->m1AC;
        if (root != 0) {
            char name[0x100];
            sprintf(name, "%s[%d] %s\0", sTypeNames[mData->mType], m4C, mData->mName);
            void* menu = LoadMenu_unk(mMenu, root, name);
            fn_80087E9C(menu, "Position", &mData->mPos, (EnvSeMenuCallback_unk)fn_80361100, this, -9999.0f,
                        9999.0f, 1.0f);
            fn_8008771C(menu, "Distance", &mData->mDistance, (EnvSeMenuCallback_unk)fn_80361100, this, 1.0f,
                        9999.0f, 1.0f);
            fn_8008771C(menu, "Volume", &mData->mVolume, (EnvSeMenuCallback_unk)fn_80361100, this, 0.0f, 2.0f,
                        0.1f);
            fn_8008771C(menu, "LPF", &mData->mLPF, (EnvSeMenuCallback_unk)fn_80361100, this, 0.0f, 1.0f, 0.1f);
        }
    }
}

void EnvSeBase::vf1C(atn::Csv* csv) {}

void EnvSeBase::fn_80360EA4() {
    if (lbl_8087F418 == 0 || lbl_8087F418->fn_8035E730() != 0 || lbl_8087F418->m78 != 0) {
        return;
    }
    {

        if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 != 0 && lbl_8087F430->m54E4 != 8) {
            return;
        }
        mActive = 1;
        if (lbl_8087F418->mState != 5) {
            mActive = 0;
        } else if (!vf20()) {
            mActive = 0;
        }
        if (mActive != 0 && mData->m44 != 0 && lbl_8087F430 != 0) {
            int value;
            if (mData->m48 == 1) {
                value = fn_80370174(lbl_8087F430, mData->m4C);
            } else {
                value = fn_80370A78(lbl_8087F430, mData->m4C);
            }
            if (value == mData->m50) {
                mActive = 1;
            } else {
                mActive = 0;
            }
        }
        if (mTimer <= 0) {
            if (mActive != 0) {
                if (m58 == 0) {
                    lbl_8087F418->fn_80360230(m48);
                }
            } else if (m58 != 0) {
                lbl_8087F418->fn_80360504(m48, 60);
            }
        } else {
            mTimer--;
        }
    }
}

void EnvSeBase::fn_8036102C(int value) {
    m58 = value;
    mTimer = 30;
}

// NONMATCHING: 86% - the original reloads lbl_8087EFE8 before the distance and keeps `this` (not
// mData) in r31 across PSVECSquareMag; tried locals, inline helpers for the mute and range tests and an
// accessor for the distance.
int EnvSeBase::vf20() {
    if (lbl_8087EFE8 != 0) {
        if (mData->m40 == 1 && lbl_8087F540 != 0 && lbl_8087F540->m70 != 0) {
            return 0;
        }
        atn::Vector3 d = lbl_8087EFE8->mListener.mPos - mData->mPos;
        if (PSVECSquareMag(&d) <= mData->mDistance * mData->mDistance) {
            return 1;
        }
    }
    return 0;
}

void EnvSeBase::fn_80361100(EnvSeBase* self) {
    if (lbl_8087F418 != 0) {
        lbl_8087F418->fn_80360574(self->m48);
    }
}

// inferred: draws the name of the sound at `pos`. Inlined in every vf14 (the out-of-line copy is
// not in the DOL); defined here for the order of the constants and literals.
void EnvSeBase::DrawName_unk(const atn::Vector3& pos) {
    atn::Vector3 screen;
    fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
    if (0.0f < screen.z && screen.z < 1.0f) {
        char text[0x100];
        sprintf(text, "%s[%d] %s\0", sTypeNames[mData->mType], m4C, mData->mName);
        lbl_8087EEB0->fn_800616C0(screen.x, screen.y, 0.0f, 12.0f, 12.0f, text, m58 != 0 ? 0xFFFFFFFF : 0xFF7F7F7F, 0.0f, 1, 1, 0);
    }
}

EnvSePoint* EnvSePoint::fn_8036111C(atn::Task* parent, int id) {
    return new ((MemoryTag)12, "", "", 0) EnvSePoint(parent, id);
}

EnvSePoint::~EnvSePoint() {}

void EnvSePoint::vf1C(atn::Csv* csv) {
    mData->mPos.x = fn_800DC288(csv->fn_8005B9CC());
    mData->mPos.y = fn_800DC288(csv->fn_8005B9CC());
    mData->mPos.z = fn_800DC288(csv->fn_8005B9CC());
}

int EnvSePoint::vf0C() {
    return fn_800D3FA4();
}

void EnvSePoint::vf10() {
    if (m64 == 0) {
        fn_80360EA4();
    }
}

void EnvSePoint::vf14() {
    if (m64 == 0 && lbl_8087F418->m1A4 >= 1) {
        lbl_8087EEB0->fn_80063D3C(mData->mPos, 3.0f, 0.0f, m58 != 0 ? 0xFFFF8080 : 0xFF7F4040);
        DrawName_unk(mData->mPos);
    }
}

EnvSeLine* EnvSeLine::fn_80361414(atn::Task* parent, int id) {
    return new ((MemoryTag)12, "", "", 0) EnvSeLine(parent, id);
}

// NONMATCHING: 14.7% - the original inlines the std::string and boost::char_separator constructors,
// the iterator comparison, the token copy and std::vector::push_back (calling only begin, end and
// operator++ out of line); here the inline budget is exceeded and almost everything is called out
// of line. Its literals then also come later in the pool (EnvSeArea_unk::fn_80363108 99.85%).
void EnvSeLine::vf1C(atn::Csv* csv) {
    std::string str(csv->fn_8005B9CC());
    std::vector<int> ids;
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(str, boost::char_separator<char>(":"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        ids.push_back(atoi(it->c_str()));
    }
    m64.Resize_unk(ids.size(), 0);
    for (unsigned int i = 0; i < ids.size(); i++) {
        m64.mData[i] = ids[i];
    }
}

// inferred: the original inlines push_back in vf1C, so its "vector length error" comes right after the
// ":" of vf1C in the string pool; ours calls it out of line, instantiated here to keep that order
template void std::vector<int>::push_back(const int& x);

EnvSeLine::~EnvSeLine() {}

int EnvSeLine::vf0C() {
    return fn_800D3FA4();
}

// inferred: clamp to [0, 1] (name invented)
inline float Clamp01_unk(float t) {
    return (t > 0.0f ? t : 0.0f) < 1.0f ? (t > 0.0f ? t : 0.0f) : 1.0f;
}

// NONMATCHING: 95.2% - stack layout of the vectors and one more saved register (the original keeps
// the listener and the best position below the segment ends).
void EnvSeLine::vf10() {
    if (lbl_8087EFE8 == 0) {
        return;
    }
    atn::Vector3 listener = lbl_8087EFE8->mListener.mPos;
    float best = 1e16f;
    atn::Vector3 bestPos(0.0f, 0.0f, 0.0f);
    EnvSeManager* mgr = lbl_8087F418;
    for (unsigned int i = 0; i < m64.mSize; i++) {
        EnvSeLink_unk* pair = &mgr->mLinks.mData[m64.mData[i]];
        EnvSeBase* a = lbl_8087F418->fn_8035FF44(pair->mA);
        EnvSeBase* b = lbl_8087F418->fn_8035FF44(pair->mB);
        if (a != 0 && b != 0) {
            atn::Vector3 pa = a->mData->mPos;
            atn::Vector3 pb = b->mData->mPos;
            atn::Vector3 d = pb - pa;
            float len = PSVECSquareMag(&d);
            atn::Vector3 p;
            if (atn::Abs_unk(len) < 1.1920929e-07f) {
                p = pa;
            } else {
                atn::Vector3 e = pa - listener;
                float t = Clamp01_unk(-PSVECDotProduct(&d, &e) / len);
                p = pa + d * t;
            }
            atn::Vector3 q = p - listener;
            float dist = PSVECSquareMag(&q);
            if (best > dist) {
                best = dist;
                bestPos = p;
            }
        }
    }
    mData->mPos = bestPos;
    if (m58 != 0) {
        lbl_8087F418->fn_8036055C(m48);
    }
    fn_80360EA4();
}

// NONMATCHING: 99.93% - the segment ends are 4 bytes lower on the stack than in the original.
void EnvSeLine::vf14() {
    EnvSeManager* mgr = lbl_8087F418;
    if (mgr->m1A4 < 1) {
        return;
    }
    for (unsigned int i = 0; i < m64.mSize; i++) {
        EnvSeLink_unk* pair = &mgr->mLinks.mData[m64.mData[i]];
        EnvSeBase* a = lbl_8087F418->fn_8035FF44(pair->mA);
        EnvSeBase* b = lbl_8087F418->fn_8035FF44(pair->mB);
        if (a != 0 && b != 0) {
            atn::Vector3 pb;
            atn::Vector3 pa;
            pa = a->mData->mPos;
            pb = b->mData->mPos;
            lbl_8087EEB0->fn_80063764(pa, pb, 0.0f, m58 != 0 ? 0xFFFFFF80 : 0xFF7F7F40);
            DrawName_unk((pa + pb) * 0.5f);
        }
    }
}

EnvSeSphere* EnvSeSphere::fn_80362278(atn::Task* parent, int id) {
    return new ((MemoryTag)12, "", "", 0) EnvSeSphere(parent, id);
}

EnvSeSphere::~EnvSeSphere() {}

void EnvSeSphere::vf1C(atn::Csv* csv) {
    mCenter.x = fn_800DC288(csv->fn_8005B9CC());
    mCenter.y = fn_800DC288(csv->fn_8005B9CC());
    mCenter.z = fn_800DC288(csv->fn_8005B9CC());
    mRadius = fn_800DC288(csv->fn_8005B9CC());
}

int EnvSeSphere::vf0C() {
    return fn_800D3FA4();
}

void EnvSeSphere::vf10() {
    if (lbl_8087EFE8 == 0) {
        return;
    }
    atn::Vector3 listener = lbl_8087EFE8->mListener.mPos;
    atn::Vector3 d = listener - mCenter;
    if (PSVECSquareMag(&d) > mRadius * mRadius) {
        if (!(atn::Abs_unk(PSVECSquareMag(&d)) < 1.1920929e-07f)) {
            PSVECNormalize(&d, &d);
        }
        d *= mRadius;
        mData->mPos = mCenter + d;
    } else {
        mData->mPos = listener;
    }
    if (m58 != 0) {
        lbl_8087F418->fn_8036055C(m48);
    }
    fn_80360EA4();
}

void EnvSeSphere::vf14() {
    if (lbl_8087F418->m1A4 >= 1) {
        lbl_8087EEB0->fn_80063D3C(mCenter, mRadius, 0.0f, m58 != 0 ? 0xFF80FF80 : 0xFF407F40);
        DrawName_unk(mCenter);
    }
}

EnvSeBox* EnvSeBox::fn_803626A8(atn::Task* parent, int id) {
    return new ((MemoryTag)12, "", "", 0) EnvSeBox(parent, id);
}

EnvSeBox::~EnvSeBox() {}

void EnvSeBox::vf1C(atn::Csv* csv) {
    atn::Vector3 pos;
    pos.x = fn_800DC288(csv->fn_8005B9CC());
    pos.y = fn_800DC288(csv->fn_8005B9CC());
    pos.z = fn_800DC288(csv->fn_8005B9CC());
    float rot = fn_800DC288(csv->fn_8005B9CC());
    mSize.x = fn_800DC288(csv->fn_8005B9CC());
    mSize.y = fn_800DC288(csv->fn_8005B9CC());
    mSize.z = fn_800DC288(csv->fn_8005B9CC());
    pos.y += mSize.y;
    mMtx = atn::Matrix3x4::RotY_unk(rot);
    mMtx.m[0][3] = pos.x;
    mMtx.m[1][3] = pos.y;
    mMtx.m[2][3] = pos.z;
    mRadius = mSize.Length_unk();
}

int EnvSeBox::vf0C() {
    return fn_800D3FA4();
}

// NONMATCHING: 77.8% - stack layout of the temporaries and scheduling of the copies of the centre.
void EnvSeBox::vf10() {
    if (lbl_8087EFE8 == 0) {
        return;
    }
    atn::Vector3 listener = lbl_8087EFE8->mListener.mPos;
    atn::Vector3 center = mMtx.GetTrans_unk();
    atn::Vector3 d = listener - center;
    if (PSVECSquareMag(&d) < (mData->mDistance + mRadius) * (mData->mDistance + mRadius)) {
        EnvSeSphere_unk sphere;
        sphere.mCenter = listener;
        sphere.mRadius = 0.0f;
        if (fn_80051CD8(&sphere, &mSize)) {
            mData->mPos = listener;
        } else {
            if (!(atn::Abs_unk(PSVECSquareMag(&d)) < 1.1920929e-07f)) {
                PSVECNormalize(&d, &d);
            }
            d *= mRadius;
            EnvSeSegment_unk seg;
            seg.mStart = center + d;
            seg.mEnd = mMtx.GetTrans_unk();
            EnvSeHit_unk hit;
            if (fn_8005454C(&hit, &seg, &mSize)) {
                mData->mPos = hit.mPos;
            } else {
                mData->mPos = mMtx.GetTrans_unk();
            }
        }
    } else {
        mData->mPos = mMtx.GetTrans_unk();
    }
    if (m58 != 0) {
        lbl_8087F418->fn_8036055C(m48);
    }
    fn_80360EA4();
}

void EnvSeBox::vf14() {
    if (lbl_8087F418->m1A4 >= 1) {
        lbl_8087EEB0->fn_800629F0(lbl_807C7030, mSize, 0.0f, m58 != 0 ? 0xFF80FFFF : 0xFF407F7F, &mMtx);
        atn::Vector3 pos = mMtx.GetTrans_unk();
        DrawName_unk(pos);
    }
}

EnvSeGlobal* EnvSeGlobal::fn_80362CC4(atn::Task* parent, int id) {
    return new ((MemoryTag)12, "", "", 0) EnvSeGlobal(parent, id);
}

EnvSeGlobal::~EnvSeGlobal() {}

void EnvSeGlobal::vf1C(atn::Csv* csv) {
    mData->mPos.x = fn_800DC288(csv->fn_8005B9CC());
    mData->mPos.y = fn_800DC288(csv->fn_8005B9CC());
    mData->mPos.z = fn_800DC288(csv->fn_8005B9CC());
}

int EnvSeGlobal::vf0C() {
    return fn_800D3FA4();
}

void EnvSeGlobal::vf10() {
    fn_80360EA4();
}

void EnvSeGlobal::vf14() {
    if (lbl_8087F418->m1A4 >= 1) {
        lbl_8087EEB0->fn_80063D3C(mData->mPos, 3.0f, 0.0f, m58 != 0 ? 0xFF8080FF : 0xFF40407F);
        DrawName_unk(mData->mPos);
    }
}

void EnvSeArea_unk::fn_80362FA4(atn::Csv* csv) {
    mId = atoi(csv->fn_8005B9CC());
    csv->fn_8005B9CC();
    mData.fn_80360B40(csv);
    atn::Vector3 pos;
    pos.x = fn_800DC288(csv->fn_8005B9CC());
    pos.y = fn_800DC288(csv->fn_8005B9CC());
    pos.z = fn_800DC288(csv->fn_8005B9CC());
    float rot = fn_800DC288(csv->fn_8005B9CC());
    mSize.x = fn_800DC288(csv->fn_8005B9CC());
    mSize.y = fn_800DC288(csv->fn_8005B9CC());
    mSize.z = fn_800DC288(csv->fn_8005B9CC());
    pos.y += mSize.y;
    mMtx = atn::Matrix3x4::RotY_unk(rot);
    mMtx.m[0][3] = pos.x;
    mMtx.m[1][3] = pos.y;
    mMtx.m[2][3] = pos.z;
    mReverb = atoi(csv->fn_8005B9CC());
    mA0 = fn_800DC288(csv->fn_8005B9CC());
}

void EnvSeArea_unk::fn_80363108(void* root) {
    if (root != 0) {
        char name[0x100];
        sprintf(name, "Area[%d] %s\0", mId, mData.mName);
        void* menu = LoadMenu_unk(mMenu, root, name);
        fn_80087E9C(menu, "Position", &mData.mPos, (EnvSeMenuCallback_unk)fn_8036324C, this, -9999.0f, 9999.0f,
                    1.0f);
        fn_8008771C(menu, "Volume", &mData.mVolume, (EnvSeMenuCallback_unk)fn_8036324C, this, 0.0f, 2.0f, 0.1f);
        fn_8008771C(menu, "LPF", &mData.mLPF, (EnvSeMenuCallback_unk)fn_8036324C, this, 0.0f, 1.0f, 0.1f);
        fn_800874C8(menu, "ReverbParamId", &mReverb, 0, 20, 1, (EnvSeMenuCallback_unk)fn_8036324C, this);
    }
}

void EnvSeArea_unk::fn_8036324C(EnvSeArea_unk* self) {
    if (lbl_8087F418 != 0 && self->mId == lbl_8087F418->m84) {
        lbl_8087F418->fn_8035FF94(self->mId, 0, 1);
    }
}
