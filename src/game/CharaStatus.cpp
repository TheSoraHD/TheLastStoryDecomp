#include "game/CharaStatus.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "game/CharaTask.h"
#include "game/ResHandle.h"
#include "macros.h"

// inferred: game settings (lbl_8087F0A8); per-difficulty table of status factors at 0xD8
struct StatusRate_unk {
    float m00;
    int m04;
    float m08;
    int m0C;
    float m10;
    int m14;
    float m18;
    int m1C;
};
struct Settings_unk {
    unsigned char m00[0xD4];
    int mD4; // inferred: index into mD8 (difficulty?)
    StatusRate_unk mD8[4];
    unsigned char m158[0x194 - 0x158];
    int m194;
    unsigned char m198[0x284 - 0x198];
    int m284;
    unsigned char m288[0x2CC - 0x288];
    int m2CC;
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: field data (lbl_8087F430); names invented
struct FieldInfo_unk {
    unsigned char m00[0x58];
    int m58; // inferred: level of the field
};
struct StatusField_unk {
    unsigned char m00[0x10D4];
    FieldInfo_unk* m10D4;
    unsigned char m10D8[0x5694 - 0x10D8];
    int m5694;
};
extern "C" StatusField_unk* lbl_8087F430;
extern "C" int fn_8020FCE8(int id);
extern "C" int fn_80375B78(StatusField_unk* field, int arg);

inline void CharaStatus::ClearFlags_unk(unsigned int flags) {
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
        for (unsigned int i = 0; i < mChara->m650; i++) {
            if (mChara->m654[i]->m274->mF0 != 0) {
                fn_8013310C(0x100000, 1);
                return;
            }
        }
    }
}

CharaStatusBase::CharaStatusBase() {
    memset(&m1A8, 0, sizeof(m1A8));
    memset(this, 0, sizeof(*this));
}


extern "C" double pow(double x, double y);

int CharaStatus::fn_8012B028() {
    int level = mParamA.m78 > 69 ? 69 : mParamA.m78;
    return (int)(50.0f * (float)pow((float)level, 2.0)) - m1A8.mExp;
}

// NONMATCHING: 97.6% - only the scheduling of the fn_8012C6D4 arguments differs (the original starts the
// mChara->m5C->mC8 chain before reading m0C). Tried: argument locals, an inline for the third
// argument, other sums for the second.
void CharaStatus::fn_8012B3E8() {
    if (mChara == 0 || mChara->m5C == 0) {
        return;
    }
    chr::CharaData_unk* data = mChara->m5C;
    mParamA = data->mParam;
    mParamA.m9C |= mChara->mC10;
    FieldInfo_unk* info = lbl_8087F430 != 0 ? lbl_8087F430->m10D4 : 0;
    int bonus = 0;
    int extra = 0;
    if (lbl_8087F0A8->m284 != 0 && mChara->mType == 2 && info != 0 && fn_8020FCE8(mChara->m5C->mC8)) {
        if (mChara->mFlag22 == 1 || (mChara->m5C->mParam.m9C & 0x20000) == 0x20000) {
            bonus = (int)((float)info->m58 * lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m08 + 0.2f);
            bonus += lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m0C;
            extra = MAX(0, (bonus - 70) * 20 + 1000);
        } else {
            bonus = (int)((float)info->m58 * lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m00 + 0.2f);
            bonus += lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m04;
            bonus = MIN(bonus, MAX(info->m58, 60));
        }
    }
    if (lbl_8087F430 != 0 && mChara->mType == 2) {
        bonus += fn_80375B78(lbl_8087F430, 1);
    }
    fn_8012C6D4(&mParamA, (int)m1A8.m08 + (bonus + mParamA.m78), m1C - ((m0C & 0x400) ? 3 : 0),
                mChara->m5C->mC8);
    mParamA.m84 += (int)m1A8.m0C;
    mParamA.m00 += m1A8.m10 + extra;
    mParamA.m04 += m1A8.m14 + extra;
    mParamA.m08 += m1A8.m18;
    mParamA.m0C += m1A8.m1C;
    mParamA.m94 += m1A8.m20;
    if (lbl_8087F0A8->m284 != 0 && mChara->mType == 2 && info != 0 && !fn_8020FCE8(mChara->m5C->mC8)) {
        mParamA.m84 = (int)((float)mParamA.m84 * lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m10 + 0.2f);
        mParamA.m84 += lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m14;
        mParamA.m00 *= lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m18;
        mParamA.m00 += lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m1C;
        mParamA.m04 *= lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m18;
        mParamA.m04 += lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m1C;
    }
    fn_8012D180();
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

// inferred: per-kind equipment table (fn_80219160); names invented
struct EquipSet_unk {
    unsigned char m00[2];
    unsigned char m02[128]; // inferred: one entry per bit of CharaStatusData_unk::mBits
    signed char m82[6];
};
extern "C" int fn_80219558(int kind);
extern "C" EquipSet_unk* fn_80219160(int kind, int level);
extern "C" int fn_8021AF98(int slot);
extern "C" int fn_8021AFF4(int slot);
struct Inventory2_unk;
extern "C" Inventory2_unk* lbl_8087EE68;
extern "C" int fn_800185B4(Inventory2_unk* inventory, CharaTask* chara, int item);
extern "C" int lbl_8087F610;
extern "C" int fn_80370174(StatusField_unk* field, int id);

// inferred: counterpart of fn_80134674 (never emitted on its own); name invented
inline void CharaStatus::ClearBit_unk(int bit) {
    if ((unsigned int)bit > 127) {
        return;
    }
    unsigned int* word = &m1A8.mBits[bit / 32];
    unsigned int mask = 1 << (bit % 32);
    if ((*word & mask) == mask) {
        *word &= ~mask;
    }
}

// NONMATCHING: 96.3% - only the registers of the inlined set/clear of a bit differ (same problem as
// fn_80134674).
void CharaStatus::fn_8012CE4C(int kind) {
    EquipSet_unk* set = fn_80219160(kind, mParamA.m78);
    if (set == 0) {
        return;
    }
    for (int i = 0; i < 128; i++) {
        bool allowed = true;
        if (lbl_8087F430 != 0) {
            switch (i) {
            case 10:
            case 13:
                if (kind == 2) {
                    if (fn_80370174(lbl_8087F430, 283) == 0) {
                        allowed = false;
                    }
                } else if (fn_80370174(lbl_8087F430, 5) > 0) {
                    allowed = false;
                }
                break;
            case 9:
                if (fn_80370174(lbl_8087F430, 136) > 0) {
                    allowed = false;
                }
                break;
            case 61:
                if (kind == 2) {
                    if (fn_80370174(lbl_8087F430, 283) == 0) {
                        allowed = false;
                    }
                } else if (fn_80370174(lbl_8087F430, 286) > 0) {
                    allowed = false;
                }
                break;
            case 62:
            case 63:
                if (kind == 2 && fn_80370174(lbl_8087F430, 283) == 0) {
                    allowed = false;
                }
                break;
            case 2:
                if (fn_80370174(lbl_8087F430, 287) > 0) {
                    allowed = false;
                }
                break;
            case 14:
            case 16:
                if ((kind == 2 || kind == 3) && fn_80370174(lbl_8087F430, 283) == 0 &&
                    lbl_8087F0A8->m194 == 0) {
                    allowed = false;
                }
                break;
            }
        }
        if (allowed && set->m02[i] != 0 && !fn_801346C8(i)) {
            fn_80134674(i);
        }
        if (!allowed || set->m02[i] == 0) {
            if (fn_801346C8(i)) {
                ClearBit_unk(i);
            }
        }
    }
}

// NONMATCHING: 97.5% - the loop that resets m2D0 walks a pointer (the original uses base + offset, with the
// zero in the register of `set`). Tried: unsigned and shared counters, a helper, sizeof bound.
void CharaStatus::fn_8012D180() {
    int kind = fn_80219558(mChara->m50);
    if (kind < 0 || 7 <= kind) {
        return;
    }
    EquipSet_unk* set = fn_80219160(kind, mParamA.m78);
    if (set == 0) {
        return;
    }
    if (lbl_8087F610 != 0) {
        return;
    }
    fn_8012CE4C(kind);
    int ids[6];
    int items[6];
    int owned[6];
    for (int i = 0; i < 6; i++) {
        ids[i] = fn_8021AF98(set->m82[i]);
        items[i] = fn_8021AFF4(set->m82[i]);
        bool ok = false;
        if (lbl_8087EE68 != 0 && items[i] != 0) {
            ok = true;
        }
        owned[i] = ok ? fn_800185B4(lbl_8087EE68, mChara, items[i]) : 0;
    }
    for (int i = 0; i < 4; i++) {
        m2D0[i] = CharaStatusPair_unk(0);
    }
    switch (kind) {
    case 0:
        m2D0[0] = CharaStatusPair_unk(ids[1]);
        m2D0[3] = CharaStatusPair_unk(ids[5]);
        break;
    case 1:
        if (owned[3] == 0) {
            m2D0[0] = CharaStatusPair_unk(ids[1]);
        } else {
            m2D0[0] = CharaStatusPair_unk(ids[3]);
        }
        if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 909) == 1) {
            m2D0[3] = CharaStatusPair_unk(1730);
        } else {
            m2D0[3] = CharaStatusPair_unk(ids[5]);
        }
        break;
    case 2:
    case 3:
        if (owned[4] != 0) {
            m2D0[0] = CharaStatusPair_unk(ids[4]);
        }
        m2D0[3] = CharaStatusPair_unk(ids[5]);
        break;
    case 4:
        if (owned[4] == 0) {
            m2D0[0] = CharaStatusPair_unk(ids[1]);
        } else {
            m2D0[0] = CharaStatusPair_unk(ids[4]);
        }
        if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 908) == 1) {
            m2D0[3] = CharaStatusPair_unk(6042);
        } else {
            m2D0[3] = CharaStatusPair_unk(ids[5]);
        }
        break;
    case 6:
        if (owned[4] == 0) {
            m2D0[0] = CharaStatusPair_unk(ids[1]);
        } else {
            m2D0[0] = CharaStatusPair_unk(ids[4]);
        }
        m2D0[3] = CharaStatusPair_unk(ids[5]);
        break;
    case 5:
        if (owned[1] == 0) {
            m2D0[0] = CharaStatusPair_unk(ids[3]);
        } else {
            m2D0[0] = CharaStatusPair_unk(ids[1]);
        }
        m2D0[3] = CharaStatusPair_unk(ids[5]);
        break;
    }
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

// NONMATCHING: only the parts loop of the inlined ClearFlags_unk differs (see the header).
void CharaStatus::fn_8012D8B8() {
    m04 = (float)mParamB.m84;
    if (mChara != 0 && mChara->mType == 0) {
        m08 = 0.0f;
    } else {
        m08 = (float)mParamB.m88;
    }
    ClearFlags_unk(0xFFFFFFFF);
    if (mChara != 0) {
        mChara->vf48();
    }
    m300 = SumRefs_unk(mRefs, 57);
    m428 = 0.0f;
    m244 = m04;
    m14 = 0;
}

// NONMATCHING: only the parts loop of the inlined ClearFlags_unk differs (see the header).
void CharaStatus::fn_8012DD70() {
    if ((m0C & 0x20) == 0x20) {
        return;
    }
    m04 = 0.0f;
    if ((m0C & 0x100) == 0x100) {
        mChara->fn_80167038();
    }
    if ((m0C & 0x10000) == 0x10000) {
        mChara->fn_80167640();
    }
    if ((m0C & 0x40) == 0x40) {
        mChara->fn_80166544();
    }
    ClearFlags_unk(0xFFFFFFFF);
    fn_8013310C(0x20, 0);
    if (m300 <= 0 && m224 > 0) {
        m224--;
    }
    m428 = 0.0f;
    m430 = 0;
    m434 = 0.0f;
    m438 = 0.0f;
}

// NONMATCHING: 94.9% - the original loads m0C before the int-to-float constant and reloads the
// count of the other character's refs before the sum loop (ours reuses it after the find).
float CharaStatus::fn_8012DB04() {
    int n = m430;
    if ((m0C & 0x8) == 0x8) {
        n += 3;
    } else if ((m0C & 0x200) == 0x200) {
        n += 2;
    }
    if ((m0C & 0x800000) == 0x800000) {
        n += 4;
    }
    float rate = 1.0f - 0.1f * n;
    if ((m18 & 0x8) == 0 && (m18 & 0x800000) == 0) {
        if ((m0C & 0x800) == 0x800 && m434 < 180.0f) {
            rate = MIN(MAX(rate - 0.6f * (180.0f - m434) / 180.0f, 0.05f), 1.0f);
        }
        if ((m0C & 0x800) == 0x800 && mChara != 0 && mChara->mD1C != 0) {
            CharaStatus* status = &mChara->mD1C->mStatus;
            if (status->fn_80134134(68) != 0) {
                int sum = status->fn_80134168(68, -1);
                rate = MIN(MAX(rate - 0.1f * sum / 100.0f, 0.05f), 1.0f);
            }
        }
    }
    if ((m0C & 0x1000000) == 0x1000000) {
        rate = 0.05f;
    }
    return rate;
}

int CharaStatus::fn_8012E534() {
    return (m0C >> 12) & 1;
}

extern "C" int fn_80219344(chr::Chara_unk* chara, int arg);

void CharaStatus::fn_8012F034(float amount) {
    if (mChara->fn_8016E484()) {
        if (!fn_80219344(mChara, 5)) {
            return;
        }
    } else if (m2D0[3].m[0] <= 0) {
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

extern "C" int fn_80375184(StatusField_unk* field);
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_8010652C(Effect_unk* effect, chr::Chara_unk* chara);
extern "C" void fn_801061BC(Effect_unk* effect, chr::Chara_unk* chara, void* model, int level);

// NONMATCHING: 95.3% - the original repeats the chara/flag test of the inlined fn_8012F3D8
// (ours reuses the first test) and passes the new gauge value to the second one from a register
// (frsp) instead of reloading m42C. Tried: a HasGauge_unk helper, the body in an inline helper.
void CharaStatus::fn_8012F188(float amount) {
    if (lbl_8087F430 != 0 && !fn_80375184(lbl_8087F430)) {
        return;
    }
    if (mChara == 0 || !mChara->mFlag28) {
        return;
    }
    if ((m1A8.mBits[1] & 0x20000000) != 0x20000000) {
        return;
    }
    float max = 10.0f;
    if ((m1A8.mBits[1] & 0x80000000) != 0x80000000) {
        max = 5.0f;
        if ((m1A8.mBits[1] & 0x40000000) != 0x40000000) {
            max = 3.0f;
        }
    }
    int before = fn_8012F3D8();
    float rate = 1.0f;
    if (mChara->fn_801789D8(0, 0x2000)) {
        rate *= 1.5f;
    }
    m42C = MIN(MAX(m42C + amount * rate, 0.0f), max - 0.1f);
    int after = fn_8012F3D8();
    if (before < after && after > 1 && lbl_8087F048 != 0) {
        fn_8010652C(lbl_8087F048, mChara);
        fn_801061BC(lbl_8087F048, mChara, &mChara->mMotion, after - 1);
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

// NONMATCHING: 89.8% - the parts loop of ClearFlags_unk (see the header).
void CharaStatus::fn_8013322C(unsigned int flags) {
    ClearFlags_unk(flags);
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

bool CharaStatus::fn_801346C8(int bit) {
    if (bit < 0 || 128 <= bit) {
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

bool CharaStatus::fn_801347C8(int index) {
    if (0 <= index && index < 4) {
        return (m230 & (1 << index)) == (1 << index);
    }
    return false;
}
