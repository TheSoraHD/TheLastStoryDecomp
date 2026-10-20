// atn/LipSync_unk.cpp: lip sync de los personajes (nombre de fichero inventado).
#include "atn/LipSync_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/Model_unk.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_80097CCC(Model_unk* model, int track, float weight); // inferred: stops a track
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)

#define LS_MIN(a, b) ((a) < (b) ? (a) : (b))
#define LS_MAX(a, b) ((a) > (b) ? (a) : (b))

// NONMATCHING: 99.51% - register allocation of the loop (offset of the key and base of the string pool swapped)
void LipSyncData_unk::fn_8007A530(const char* text, int size) {
    atn::CsvTab csv(text, size);
    csv.fn_8005BCE8(lbl_8087D710);
    unsigned int count = fn_800DC12C(csv.fn_8005B9CC());
    Resize_unk(count, 4);
    m08 = fn_800DC12C(csv.fn_8005B9CC());
    unsigned int i;
    for (i = 0; csv.fn_8005BBF8() != 0; i++) {
        if (strcmp(csv.fn_8005B9CC(), "") == 0) {
            break;
        }
        const char* s;
        LipSyncKey_unk* key = &mKeys[i];
        s = csv.fn_8005B9CC();
        key->mVowel = strcmp(s, "A") == 0   ? 0
                      : strcmp(s, "I") == 0 ? 1
                      : strcmp(s, "U") == 0 ? 2
                      : strcmp(s, "E") == 0 ? 3
                      : strcmp(s, "O") != 0 ? 5
                                            : 4;
        key->mTime = fn_800DC288(csv.fn_8005B9CC());
    }
}

LipSyncKey_unk::LipSyncKey_unk() : mVowel(5), mTime(0.0f) {}

Chara1154_unk::Chara1154_unk()
    : m00(0), m04(0.0f), m08(1.0f), m0C(8.0f), m10(6.0f), m2C(0.15f), m30(0) {
    m14[0] = 0.3f;
    m14[1] = 0.2f;
    m14[2] = 0.25f;
    m14[3] = 0.2f;
    m14[4] = 0.25f;
    m14[5] = 0.05f;
}

// NONMATCHING: 99.70% - FPR allocation of the clamp (current value and m2C swapped f2/f3)
void Chara1154_unk::fn_8007A7BC(Model_unk* model) {
    if (m00 == 0 || (m00 != 0 ? m04 >= m00->mCount : true)) {
        fn_80097CCC(model, 5, 0.0f);
        return;
    }
    int i = LS_MIN((int)m00->mCount - 1, (int)(m04 + m10));
    if (0 <= i && i < (int)m00->mCount) {
        LipSyncKey_unk* key = &m00->mKeys[i];
        model->fn_80097C08(5, ((int (*)(int))m30)(key->mVowel), 1, 0, 1, 0.0f, 0.1f);
        Model_unk::Track_unk& track = model->mTracks[5];
        float cur = track.m1C;
        float delta = key->mTime * m0C - cur;
        float v = cur + LS_MAX(-m2C, LS_MIN(m2C, delta));
        track.m1C = LS_MIN(1.0f, LS_MAX(0.0f, v));
        track.m14 = m14[key->mVowel];
    }
    m04 += m08;
}
