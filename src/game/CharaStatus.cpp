#include "game/CharaStatus.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "game/CharaTask.h"
#include "game/ResHandle.h"
#include "macros.h"

CharaStatusBase::CharaStatusBase() {
    memset(&m1A8, 0, sizeof(m1A8));
    memset(this, 0, sizeof(*this));
}


extern "C" double pow(double x, double y);

int CharaStatus::fn_8012B028() {
    int level = mParamA.m78 > 69 ? 69 : mParamA.m78;
    return (int)(50.0f * (float)pow((float)level, 2.0)) - m1A8.mExp;
}

// NONMATCHING: 98.1% - register allocation only: the original keeps zero in r30 and the loop
// pointers of m2D0 in r29/r28; ours uses r29 for zero and r28/r30 for the pointers.
CharaStatus::CharaStatus()
    : m240(0), m248(0), m2CC(0), mChara(0), m2F4(0.0f), m2F8(0.0f), m2FC(1.0f), m300(0),
      m428(0.0f), m42C(0.0f), m430(0), m434(0.0f), m438(0.0f) {
    memset(m24C, 0, sizeof(m24C));
}

CharaStatus::~CharaStatus() {}

void CharaStatus::fn_8012D628(CharaParam_unk* dst, const CharaParam_unk* src) {
    if (src == 0) {
        return;
    }
    dst->m00 += src->m00;
    dst->m04 += src->m04;
    dst->m08 += src->m08;
    dst->m0C += src->m0C;
    dst->m10 *= src->m10;
    dst->m14 *= src->m14;
    dst->m28 += src->m28;
    dst->m2C += src->m2C;
    dst->m20 += src->m20;
    dst->m24 += src->m24;
    dst->m18 += src->m18;
    dst->m1C += src->m1C;
    dst->m30 += src->m30;
    dst->m34 += src->m34;
}

void CharaStatus::fn_8012D714(CharaParam_unk* dst, const CharaParam_unk* a, const CharaParam_unk* b) {
    if (a == 0 || b == 0) {
        return;
    }
    dst->m00 += (a->m00 + b->m00) / 2.0f;
    dst->m04 += (a->m04 + b->m04) / 2.0f;
    dst->m08 += (a->m08 + b->m08) / 2.0f;
    dst->m0C += (a->m0C + b->m0C) / 2.0f;
    dst->m10 *= (a->m10 + b->m10) / 2.0f;
    dst->m14 *= (a->m14 + b->m14) / 2.0f;
    dst->m28 += (a->m28 + b->m28) / 2.0f;
    dst->m2C += (a->m2C + b->m2C) / 2.0f;
    dst->m20 += (a->m20 + b->m20) / 2;
    dst->m24 += (a->m24 + b->m24) / 2;
    dst->m18 += (a->m18 + b->m18) / 2.0f;
    dst->m1C += (a->m1C + b->m1C) / 2.0f;
    dst->m30 += (a->m30 + b->m30) / 2;
    dst->m34 += (a->m34 + b->m34) / 2;
}

int CharaStatus::fn_8012E534() {
    return (m0C >> 12) & 1;
}

// inferred: name invented
inline int SumRefs_unk(FixedArray_unk<CharaStatusRef_unk, 8>& refs, int id) {
    CharaStatusRef_unk* ref = refs.mData;
    int sum = 0;
    for (unsigned int i = 0; i < refs.mCount; i++, ref++) {
        if (ref->mEntry->mId == id) {
            sum += ref->mEntry->mValue;
        }
    }
    return sum;
}

extern "C" int fn_80219344(chr::Chara_unk* chara, int arg);

void CharaStatus::fn_8012F034(float amount) {
    if (mChara->fn_8016E484()) {
        if (!fn_80219344(mChara, 5)) {
            return;
        }
    } else if (m2D0[3].m00 <= 0) {
        return;
    }
    static const float sFactors[5] = {0.6f, 0.8f, 1.0f, 1.2f, 1.4f};
    amount *= m2FC;
    float old = m228;
    amount *= 1.0f + (float)SumRefs_unk(mRefs, 53) / 100.0f;
    if (mChara != 0 && mChara->m50 == 1) {
        amount *= sFactors[m1A8.m54 > 4 ? 4 : m1A8.m54];
    }
    m228 += amount;
    if (old < 1.0f && m228 >= 1.0f) {
        mChara->fn_8017AC2C();
    }
}

int CharaStatus::fn_8012F3D8() {
    if (mChara == 0 || !mChara->mFlag28) {
        return 0;
    }
    if (m42C < 0.0f) {
        return 0;
    }
    if (m42C < 3.0f) {
        return 1;
    }
    if (m42C < 5.0f) {
        return 2;
    }
    return 3;
}

void CharaStatus::fn_8013310C(unsigned int flags, int both) {
    m0C |= flags;
    if (both) {
        m10 |= flags;
    }
}

void CharaStatus::fn_80133130(unsigned int flags, int value, int both) {
    fn_8013310C(flags, both);
    for (int i = 0; i < 32; i++) {
        if ((flags & (1 << i)) == (1 << i)) {
            m24C[i] = value;
        }
    }
}

// NONMATCHING: 91.7% - our compiler turns the parts loop into a ctr loop with the count read once; the
// original reloads mChara->m650 each time and keeps an index and an offset. Tried: int and
// unsigned index, break instead of return, a local chara pointer, a bool helper, a found flag.
void CharaStatus::fn_8013322C(unsigned int flags) {
    m0C &= ~flags;
    m10 &= ~flags;
    for (int i = 0; i < 32; i++) {
        if ((flags & (1 << i)) == (1 << i)) {
            m24C[i] = 0;
        }
    }
    if (mChara != 0) {
        m0C &= ~0x100000;
        m10 &= ~0x100000;
        for (int i = 0; i < mChara->m650; i++) {
            if (mChara->m654[i]->m274->mF0 != 0) {
                fn_8013310C(0x100000, 1);
                return;
            }
        }
    }
}

int CharaStatus::fn_801333A4(unsigned int mask) {
    for (int i = 0; i < 32; i++) {
        if ((mask & (1 << i)) == (1 << i)) {
            return m24C[i];
        }
    }
    return -1;
}

void CharaStatus::fn_801333E4() {
    m04 = MIN(MAX(m04, 0.0f), (float)mParamB.m84);
    m08 = MIN(MAX(m08, 0.0f), (float)mParamB.m88);
}

int CharaStatus::fn_801334CC(int level) {
    if (level >= 99) {
        return 0;
    }
    level = level > 69 ? 69 : level;
    return (int)(50.0f * (float)pow((float)level, 2.0));
}

void CharaStatus::fn_80133B30(int level) {
    bool up = m1C < level;
    m1C = level;
    fn_8012B3E8();
    fn_8012B988();
    if (!up) {
        m04 = MIN(m04, (float)mParamB.m84);
    }
}

void CharaStatus::fn_80133E24(int id, int sub) {
    CharaStatusEntry_unk* it = mEntries.begin();
    while (it != mEntries.end()) {
        if ((sub < 0 || it->mSub == sub) && (id < 0 || it->mId == id)) {
            mEntries.erase(it);
        } else {
            it++;
        }
    }
}

CharaStatusEntry_unk* CharaStatus::fn_80133EE8(int id) {
    CharaStatusEntry_unk* entry = mEntries.mData;
    for (unsigned int i = 0; i < mEntries.mCount; i++, entry++) {
        if (entry->mId == id) {
            return entry;
        }
    }
    return 0;
}

int CharaStatus::fn_80133F18(int id, int sub) {
    int sum = 0;
    CharaStatusEntry_unk* entry = mEntries.mData;
    for (unsigned int i = 0; i < mEntries.mCount; i++, entry++) {
        if ((id < 0 || entry->mId == id) && (sub < 0 || entry->mSub == sub)) {
            sum += entry->mValue;
        }
    }
    return sum;
}

int CharaStatus::fn_80133F6C(int id, int sub) {
    int count = 0;
    CharaStatusEntry_unk* entry = mEntries.mData;
    for (unsigned int i = 0; i < mEntries.mCount; i++, entry++) {
        if ((id < 0 || entry->mId == id) && (sub < 0 || entry->mSub == sub)) {
            count++;
        }
    }
    return count;
}

// inferred: name invented
inline CharaStatusRef_unk* CharaStatus::FindRef_unk(int id, unsigned int key) {
    CharaStatusRef_unk* it = mRefs.mData;
    for (unsigned int i = 0; i < mRefs.mCount; i++, it++) {
        if (it->mEntry->mId == id && it->m04 == key) {
            return it;
        }
    }
    return 0;
}

void CharaStatus::fn_80133FBC(const CharaStatusRef_unk& ref, int replace) {
    if (replace) {
        CharaStatusRef_unk* found = FindRef_unk(ref.mEntry->mId, ref.m04);
        if (found != 0) {
            found->m0C = ref.m0C;
            return;
        }
    }
    mRefs.push_back(ref);
}

void CharaStatus::fn_8013407C() {
    if (mChara == 0) {
        return;
    }
    CharaStatusRef_unk* it = mRefs.begin();
    while (it != mRefs.end()) {
        if (it->m08 == mChara) {
            mRefs.erase(it);
        } else {
            it++;
        }
    }
}

CharaStatusRef_unk* CharaStatus::fn_80134134(int id) {
    CharaStatusRef_unk* ref = mRefs.mData;
    for (unsigned int i = 0; i < mRefs.mCount; i++, ref++) {
        if (ref->mEntry->mId == id) {
            return ref;
        }
    }
    return 0;
}

int CharaStatus::fn_80134168(int id, int sub) {
    int sum = 0;
    CharaStatusRef_unk* ref = mRefs.mData;
    for (unsigned int i = 0; i < mRefs.mCount; i++, ref++) {
        CharaStatusEntry_unk* entry = ref->mEntry;
        if (entry->mId == id && (sub < 0 || entry->mSub == sub)) {
            sum += entry->mValue;
        }
    }
    return sum;
}

int CharaStatus::fn_801341B8(int id, int sub) {
    int sum = 0;
    CharaStatusRef_unk* ref = mRefs.mData;
    for (unsigned int i = 0; i < mRefs.mCount; i++, ref++) {
        CharaStatusEntry_unk* entry = ref->mEntry;
        if (entry->mId == id && (sub < 0 || entry->mSub == sub)) {
            sum += entry->m10;
        }
    }
    return sum;
}

bool CharaStatus::fn_80134208() {
    int type = mChara->m5C->m122;
    bool result = true;
    if (type != 1 && type != 0) {
        result = false;
    }
    return result;
}

bool CharaStatus::fn_80134234() {
    return mChara->m5C->m11C == 2;
}

bool CharaStatus::fn_80134250() {
    return mChara->m5C->m122 == 4;
}

bool CharaStatus::fn_80134270() {
    return mChara->m5C->m122 == 2;
}

bool CharaStatus::fn_80134290() {
    return mChara->m5C->m122 == 3;
}

bool CharaStatus::fn_801342B0() {
    return mChara->m5C->m122 == 3;
}

inline bool TestBit_unk(const unsigned int* bits, int bit) {
    unsigned int mask = 1 << (bit % 32);
    return (bits[bit / 32] & mask) == mask;
}
inline void SetBit_unk(unsigned int* bits, int bit) {
    bits[bit / 32] |= 1 << (bit % 32);
}

struct Settings_unk {
    unsigned char m00[0x2CC];
    int m2CC;
};
extern "C" Settings_unk* lbl_8087F0A8;
struct StatusField_unk {
    unsigned char m00[0x5694];
    int m5694;
};
extern "C" StatusField_unk* lbl_8087F430;
extern "C" int rand();

struct RandomPick_unk {
    int mThreshold;
    int mValue;
};

void CharaStatus::fn_8013459C() {
    static const RandomPick_unk sTable[6] = {{30, 0}, {50, 1}, {70, 2}, {80, 3}, {90, 4}, {100, 5}};
    if (lbl_8087F0A8->m2CC != 0) {
        bool active = false;
        if (lbl_8087F430 != 0 && lbl_8087F430->m5694 != 0) {
            active = true;
        }
        if (active) {
            int r = rand() % 100;
            for (int i = 0; i < 6; i++) {
                if (r < sTable[i].mThreshold) {
                    m424 = sTable[i].mValue;
                    break;
                }
            }
        } else {
            m424 = 0;
        }
    }
}

// NONMATCHING: 98.1% - registers only (the original keeps the constant 1 in r4 and the word
// index in r7). Tried: mask/word locals, test and set written in place, helpers with a mask local.
void CharaStatus::fn_80134674(int bit) {
    if ((unsigned int)bit > 127) {
        return;
    }
    if (!TestBit_unk(m1A8.mBits, bit)) {
        SetBit_unk(m1A8.mBits, bit);
    }
}

// NONMATCHING: 90.9% - our compiler merges the two range checks into one unsigned compare
// (cmplwi 127); the original keeps cmpwi 0 / cmpwi 128 with a shared false block. Same problem as
// fn_801347C8; separate ifs, nested ifs, goto, ternary and an inline range helper were tried.
bool CharaStatus::fn_801346C8(int bit) {
    if (bit < 0) {
        return false;
    }
    if (bit >= 128) {
        return false;
    }
    unsigned int mask = 1 << (bit % 32);
    return (m1A8.mBits[bit / 32] & mask) == mask;
}

void CharaStatus::fn_80134724(int add) {
    int value = m430 + add;
    m430 = MIN(MAX(value, 0), 4);
    m434 = 0.0f;
    if (m430 > 0) {
        m438 = 150.0f;
    }
}

void CharaStatus::fn_80134774(int index, int on) {
    if (index < 0) {
        return;
    }
    if (index >= 4) {
        return;
    }
    if (on) {
        m230 |= (unsigned char)(1 << index);
    } else {
        m230 &= (unsigned char)~(1 << index);
    }
}

// NONMATCHING: 81.1% - range checks merged into cmplwi 3 (see fn_801346C8).
bool CharaStatus::fn_801347C8(int index) {
    if (index >= 0) {
        if (index < 4) {
            return (m230 & (1 << index)) == (1 << index);
        }
    }
    return false;
}
