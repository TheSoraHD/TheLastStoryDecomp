#include "game/CharaTask.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "game/CharaStatus.h"
#include <boost/scoped_ptr.hpp>

// inferred: game settings (lbl_8087F0A8, as in CharaStatus.cpp); names invented
struct Settings_unk {
    int fn_8013C41C();
    int* fn_8013C434();
    float fn_8013C50C();

    unsigned char m00[0x48];
    float m48;
    unsigned char m4C[0x418 - 0x4C];
    int m418;
    unsigned char m41C[0x454 - 0x41C];
    int m454;
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: field data (lbl_8087F430); m10D8 holds the route tables (see RouteWalker_unk.cpp)
struct Field_unk {
    RouteTable_unk* fn_8013C504();

    unsigned char m00[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;

// inferred: effect manager (lbl_8087F048, as in CharaStatus.cpp)
struct Effect_unk {
    void fn_8013A19C(int value);

    unsigned char m00[0x3E39C];
    int m3E39C;
};
extern "C" Effect_unk* lbl_8087F048;

// inferred: globals returned by accessors that are called out of line
extern "C" void* lbl_8087EE68;
extern "C" void* lbl_8087EE98;
extern "C" void* lbl_8087F490;

void* fn_80139EF4() {
    return lbl_8087EE68;
}

int CharaTask::fn_80139E90() {
    return mFlag27;
}

int CharaTask::fn_80139E9C() {
    return m1208 != 0;
}

int CharaTask::fn_80139EB0() {
    return mFlag3;
}

int CharaTask::fn_80139EBC() {
    return mF54 != 0;
}

int CharaTask::fn_80139ED0() {
    return mFlag5;
}

int CharaTask::fn_80139EDC() {
    return mFlag4;
}

int CharaTask::fn_80139EE8() {
    return (m54C >> 6) & 1;
}

int CharaTask::fn_80139EFC() {
    return mC54;
}

int CharaTask::fn_80139F04() {
    return m1400 >= 2;
}

void Model_unk::fn_80139F24(int value) {
    m34C = value;
}

int Model_unk::fn_80139F2C(int track) {
    return mTracks[track].m00;
}

void Model_unk::fn_80139F3C(int track, float speed) {
    mTracks[track].m0C = speed;
}

int CharaStatus::fn_80139F4C() {
    return (m0C >> 5) & 1;
}

atn::Vector3* RouteWalker_unk::fn_80139F58() {
    return &m58;
}

void atn::Matrix3x4::fn_8013A13C(float rad) {
    PSMTXRotRad(m, 'y', rad);
}

// inferred: object at CharaTask+0xD74 (class unknown)
int CharaD74_unk::fn_8013A144() {
    return m20 & 1;
}

int CharaTask::fn_8013A150() {
    return m564;
}

bool CharaTask::fn_8013A158() {
    return mType == 2;
}

int CharaObj1208_unk::fn_8013A184() {
    return m54;
}

int CharaObj1208_unk::fn_8013A18C() {
    return m9C;
}

Effect_unk* fn_8013A194() {
    return lbl_8087F048;
}

void Effect_unk::fn_8013A19C(int value) {
    m3E39C = value;
}

// NONMATCHING: 71.9% - the bad_function_call throw of boost::function::operator() needs MSL's
// std::runtime_error (reference-counted message, see docs/msl_cpp.md); the shim one differs.
void CharaTask::fn_8013A258() {
    if (mControl) {
        if (mControl->vf0C()) {
            boost::function<void()> next = mControl->vf10();
            if (next) {
                next();
            } else {
                mControl.reset();
            }
        }
    }
}

atn::Vector3* CharaTask::fn_8013C38C() {
    return &mPos;
}

void atn::Vector3::fn_8013C394(const Vector3& a, const Vector3& b) {
    PSVECCrossProduct(&a, &b, this);
}

int CharaTask::fn_8013C3A8() {
    return mFlag31;
}

atn::Vector3 atn::Vector3::operator-() const {
    return Vector3(-x, -y, -z);
}

int Model_unk::fn_8013C3DC() {
    return m34C;
}

float Model_unk::fn_8013C3E4(int track) {
    return mTracks[track].m20;
}

int RouteWalker_unk::fn_8013C3F4() {
    return m2C;
}

CharaSubSeg_unk* RouteWalker_unk::fn_8013C3FC() {
    return &m70;
}

int CharaTask::fn_8013C404(unsigned int flag) {
    return (m54C & flag) == flag;
}

int Settings_unk::fn_8013C41C() {
    return m418;
}

int RouteWalker_unk::fn_8013C424() {
    return m94;
}

float CharaTask::fn_8013C42C() {
    return m5B0;
}

int* Settings_unk::fn_8013C434() {
    return &m454;
}

int chr::CharaPart_unk::fn_8013C43C() {
    return m04;
}

bool CharaTask::fn_8013C444() {
    return m640 <= 0;
}

chr::CharaPart_unk* CharaTask::fn_8013C458() {
    return m648;
}

void CharaTask::fn_8013C460(int value) {
    mFlag5 = value;
}

void CharaTask::fn_8013C470(CharaTask* chara) {
    mFC0 = chara;
}

CharaTask* CharaTask::fn_8013C478() {
    return mFC0;
}

RouteTable_unk* Field_unk::fn_8013C504() {
    return m10D8;
}

float Settings_unk::fn_8013C50C() {
    return m48;
}

bool CharaD68_unk::fn_8013C514() {
    bool result = false;
    if (m04 != 0 && m00 != 3) {
        result = true;
    }
    return result;
}

atn::Vector3* CharaD68_unk::fn_8013C53C() {
    return &m10;
}

float CharaD68_unk::fn_8013C544() {
    return m5C * m28;
}

void Model_unk::fn_8013C554(int track, float weight) {
    mTracks[track].m20 = weight;
}

bool CharaParam_unk::fn_801404E0(unsigned int flag) {
    return (m9C & flag) == flag;
}

void* fn_801404F8() {
    return lbl_8087EE98;
}

bool CharaTask::fn_80140518() {
    return mType == 3;
}

// inferred: forward direction of the character (rotated by mRot.y)
// NONMATCHING: 99.8% - only the .sdata2 offset of 0.0f differs (the pool order needs the
// functions before this one).
atn::Vector3 CharaTask::fn_8014052C() {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(mRot.y);
    PSMTXMultVec(m.m, &dir, &dir);
    return dir;
}

void atn::Matrix3x4::fn_80144ED4(float rad) {
    Mtx t;
    Mtx r;
    PSMTXRotRad(r, 'z', rad);
    PSMTXConcat(m, r, t);
    __MtxCopy(m, t);
}

// NONMATCHING: 99.4% - only the .sdata2 offset of 0.0f differs (pool order).
void CharaTask::fn_801446F0() {
    m574.x = 0.0f;
    m574.y = 0.0f;
    m574.z = 0.0f;
    m570 = 0.0f;
    m580 = 0.0f;
    m584 = 0.0f;
}

int CharaTask::fn_801479D8() {
    return mFlag6;
}

void atn::Matrix3x4::fn_801479E4(const Vector3& v) {
    m[0][3] = v.x;
    m[1][3] = v.y;
    m[2][3] = v.z;
}

Model_unk::Node_unk* Model_unk::fn_80147A00() {
    return m220;
}

void atn::Matrix3x4::fn_80147A08(const Quaternion_unk& q) {
    PSMTXQuat(m, &q);
}

void CharaTask::fn_80147A18(int value) {
    m13F4 = value;
    fn_80094E2C(&mMotion, value);
    if (m648 != 0) {
        fn_80094E2C(m648->m010, value);
        if (m64C != 0) {
            fn_80094E2C(m64C->m010, value);
        }
    }
    chr::CharaEquip_unk** p = m680;
    for (unsigned int i = 0; i < 8; i++, p++) {
        if (*p != 0) {
            fn_80094E2C((*p)->m024, value);
        }
    }
    for (unsigned int i = 0; i < m6A4; i++) {
        if (m6A8[i]->m3DC) {
            fn_80094E2C(m6A8[i]->m004, value);
        }
    }
}

int CharaTask::fn_80148334() {
    return mFlag2;
}

CharaTask::Item14_unk* CharaTask::fn_80148B0C(int index) {
    if (index >= 0 && index < m624) {
        return &m62C[index];
    }
    return 0;
}

atn::Matrix3x4& atn::Matrix3x4::fn_801495E8(const Matrix3x4& a) {
    PSMTXConcat(a.m, m, m);
    return *this;
}

void atn::Matrix3x4::fn_80149624(float rad) {
    Mtx t;
    Mtx r;
    PSMTXRotRad(r, 'y', rad);
    PSMTXConcat(m, r, t);
    __MtxCopy(m, t);
}

int CharaTask::fn_8014969C() {
    return mFlag30;
}

atn::Quaternion_unk& atn::Quaternion_unk::fn_8014989C(const Quaternion_unk& a) {
    PSQUATMultiply(&a, this, this);
    return *this;
}

atn::Quaternion_unk atn::Quaternion_unk::fn_801498D8() const {
    Quaternion_unk r(*this);
    PSQUATInverse(&r, &r);
    return r;
}

void CharaTask::fn_8014C468() {
    if (m648 == 0) {
        fn_8014C0B4(0, m510);
        return;
    }
    switch (m648->m04) {
    case 0:
        if (m5C->m11C == 2) {
            fn_8014C0B4(0, 4);
        } else if (fn_80219558(m50) == 0) {
            fn_8014C0B4(0, 5);
        } else {
            fn_8014C0B4(0, 3);
        }
        break;
    case 1:
        fn_8014C0B4(0, 0);
        break;
    case 2:
        fn_8014C0B4(0, 1);
        break;
    }
}

void CharaTask::fn_8014EE48() {
    if (m5C->m11C == 2 && mParts.mCount >= 2) {
        m64C = mParts.mData[1];
        if (m67C != 0 && m67C->vf0C() == 0) {
            m64C = m67C;
        }
    }
}

void CharaTask::fn_8014EEC4(int keep) {
    if (m648 != 0) {
        fn_80044E0C(m648, 1);
    }
    if (m64C != 0) {
        fn_80044E0C(m64C, 1);
    }
    m674 = -1;
    m648 = 0;
    m64C = 0;
    if (keep == 0) {
        fn_8014C228();
    }
}

// NONMATCHING: 90.1% - the original keeps the index in r31 and recomputes &mData[i] for the
// erase ((i * 4) / 4); ours keeps the element address (lwzu) from the delete.
void CharaTask::fn_8014F4FC(int a, int b) {
    for (unsigned int i = 0; i < mParts.mCount; i++) {
        if (a == mParts.mData[i]->m04 && b == mParts.mData[i]->m08) {
            delete mParts.mData[i];
            mParts.erase(&mParts.mData[i]);
            return;
        }
    }
}

// NONMATCHING: 78.0% - same as fn_8014F4FC: the original does not keep &mData[index] across the
// delete (saves this, index, a, b in r28-r31); tried operator[], begin() + index, a local pointer.
void CharaTask::fn_8014F5D4(int index, int a, int b) {
    delete mParts.mData[index];
    mParts.erase(&mParts.mData[index]);
    fn_8014F2B4(a, b, index);
}

bool CharaTask::fn_8014FAD0() {
    bool result = false;
    for (unsigned int i = 0; i < mParts.mCount; i++) {
        if (mParts.mData[i] != 0 && mParts.mData[i]->vf0C()) {
            result = true;
        }
    }
    return result;
}

bool CharaTask::fn_8014FCC4() {
    bool result = false;
    if (m648 != 0 && m648->m04 == 0) {
        result = true;
    }
    return result;
}

bool CharaTask::fn_8014FCE8() {
    bool result = false;
    if (m648 != 0 && m648->m04 == 1) {
        result = true;
    }
    return result;
}

bool CharaTask::fn_8014FD0C() {
    bool result = false;
    if (m648 != 0 && m648->m04 == 2) {
        result = true;
    }
    return result;
}

void CharaTask::fn_80150178(int slot, chr::CharaEquip_unk* equip) {
    if (m680[slot] != 0) {
        delete m680[slot];
    }
    m680[slot] = equip;
    if (slot == 0) {
        fn_8014FD30();
    }
}
