#include "game/CharaStatus.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "game/CharaTask.h"
#include "game/ResHandle.h"

CharaStatusBase::CharaStatusBase() {
    memset(m1A8, 0, sizeof(m1A8));
    memset(this, 0, sizeof(*this));
}


extern "C" double pow(double x, double y);

int CharaStatus::fn_8012B028() {
    int level = mParamA.m78 > 69 ? 69 : mParamA.m78;
    return (int)(50.0f * (float)pow((float)level, 2.0)) - *(int*)&m1A8[4];
}

// NONMATCHING: 98.1% - register allocation only: the original keeps zero in r30 and the loop
// pointers of m2D0 in r29/r28; ours uses r29 for zero and r28/r30 for the pointers.
CharaStatus::CharaStatus()
    : m240(0), m248(0), m2CC(0), mChara(0), m2F4(0.0f), m2F8(0.0f), m2FC(1.0f), m300(0), mNumEntries(0),
      mNumRefs(0), m428(0.0f), m42C(0.0f), m430(0), m434(0.0f), m438(0.0f) {
    memset(m24C, 0, sizeof(m24C));
}

CharaStatus::~CharaStatus() {}

int CharaStatus::fn_8012E534() {
    return (m0C >> 12) & 1;
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

int CharaStatus::fn_801333A4(unsigned int mask) {
    for (int i = 0; i < 32; i++) {
        if ((mask & (1 << i)) == (1 << i)) {
            return m24C[i];
        }
    }
    return -1;
}

CharaStatusEntry_unk* CharaStatus::fn_80133EE8(int id) {
    CharaStatusEntry_unk* entry = mEntries;
    for (unsigned int i = 0; i < mNumEntries; i++, entry++) {
        if (entry->mId == id) {
            return entry;
        }
    }
    return 0;
}

int CharaStatus::fn_80133F18(int id, int sub) {
    int sum = 0;
    CharaStatusEntry_unk* entry = mEntries;
    for (unsigned int i = 0; i < mNumEntries; i++, entry++) {
        if ((id < 0 || entry->mId == id) && (sub < 0 || entry->mSub == sub)) {
            sum += entry->mValue;
        }
    }
    return sum;
}

int CharaStatus::fn_80133F6C(int id, int sub) {
    int count = 0;
    CharaStatusEntry_unk* entry = mEntries;
    for (unsigned int i = 0; i < mNumEntries; i++, entry++) {
        if ((id < 0 || entry->mId == id) && (sub < 0 || entry->mSub == sub)) {
            count++;
        }
    }
    return count;
}

CharaStatusRef_unk* CharaStatus::fn_80134134(int id) {
    CharaStatusRef_unk* ref = mRefs;
    for (unsigned int i = 0; i < mNumRefs; i++, ref++) {
        if (ref->mEntry->mId == id) {
            return ref;
        }
    }
    return 0;
}

int CharaStatus::fn_80134168(int id, int sub) {
    int sum = 0;
    CharaStatusRef_unk* ref = mRefs;
    for (unsigned int i = 0; i < mNumRefs; i++, ref++) {
        CharaStatusEntry_unk* entry = ref->mEntry;
        if (entry->mId == id && (sub < 0 || entry->mSub == sub)) {
            sum += entry->mValue;
        }
    }
    return sum;
}

int CharaStatus::fn_801341B8(int id, int sub) {
    int sum = 0;
    CharaStatusRef_unk* ref = mRefs;
    for (unsigned int i = 0; i < mNumRefs; i++, ref++) {
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
