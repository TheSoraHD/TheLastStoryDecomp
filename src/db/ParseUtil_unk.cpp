#include "atn/Csv.h"
#include "atn/Memory.h"
#include "db/ParseUtil_unk.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>

extern "C" int atoi(const char* s);
extern "C" char* strchr(const char* s, int c);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
// inferred: copies a wide text to buf + pos and returns the next position (db/CharaIdTable_unk)
extern "C" int fn_802089C4(const wchar_t** out, const wchar_t* src, wchar_t* buf, int pos);

namespace atn {
// inferred: degrees to radians (math/Vec.h has it inline; here it is called out of line and its
// constant is not in this file's .sdata2, so it is only declared)
float DegToRad_unk(float deg);
} // namespace atn

typedef boost::tokenizer<boost::char_separator<char> > Tokenizer_unk;

extern "C" void fn_80219F84(BulletParam_unk* param, atn::Csv* csv, int version, char* buf, wchar_t* texts, int* a,
                            int* b) {
    param->mId = atoi(csv->fn_8005B9CC());
    // inferred: columns that are not used
    csv->fn_8005B9CC();
    csv->fn_8005B9CC();
    csv->fn_8005B9CC();
    param->mKind = atoi(csv->fn_8005B9CC());
    param->m10 = atoi(csv->fn_8005B9CC());
    param->m03 = atoi(csv->fn_8005B9CC());
    param->m02 = atoi(csv->fn_8005B9CC());
    param->m14 = atoi(csv->fn_8005B9CC());
    param->m18 = atoi(csv->fn_8005B9CC());
    param->m1C = atoi(csv->fn_8005B9CC());
    param->m20 = atoi(csv->fn_8005B9CC());
    param->m28 = fn_800DC288(csv->fn_8005B9CC());
    param->m2C = fn_800DC288(csv->fn_8005B9CC());
    param->m30 = fn_800DC288(csv->fn_8005B9CC());
    param->m34 = fn_800DC288(csv->fn_8005B9CC());
    param->m38 = fn_800DC288(csv->fn_8005B9CC());
    param->m3C = atoi(csv->fn_8005B9CC());
    param->m40 = fn_800DC288(csv->fn_8005B9CC());
    param->m44 = fn_800DC288(csv->fn_8005B9CC());
    param->m48 = atoi(csv->fn_8005B9CC());
    param->m4C = fn_800DC288(csv->fn_8005B9CC());
    param->m50 = atn::DegToRad_unk(fn_800DC288(csv->fn_8005B9CC()));
    param->m54 = fn_800DC288(csv->fn_8005B9CC());
    param->m58 = fn_800DC288(csv->fn_8005B9CC());
    param->m5C = atoi(csv->fn_8005B9CC());
    param->m64 = atoi(csv->fn_8005B9CC());
    param->m68 = atoi(csv->fn_8005B9CC());
    param->m6C = atoi(csv->fn_8005B9CC());

    std::string str;
    str = csv->fn_8005B9CC();
    Tokenizer_unk tok(str, boost::char_separator<char>(":"));
    int n = 0;
    memset(param->m74, 0, sizeof(param->m74));
    for (Tokenizer_unk::iterator it = tok.begin(); it != tok.end() && n < 7; it++) {
        param->m74[n] = fn_800DC288(it->c_str());
        n++;
    }

    str = csv->fn_8005B9CC();
    param->m90 = atoi(str.c_str());
    const char* second = strchr(str.c_str(), ':');
    if (second != 0) {
        param->m94 = atoi(second + 1);
    }

    memset(param->m98, 0, sizeof(param->m98));
    str = csv->fn_8005B9CC();
    tok.assign(str, boost::char_separator<char>(":"));
    n = 0;
    for (Tokenizer_unk::iterator it = tok.begin(); it != tok.end() && n < 4; it++) {
        param->m98[n] = atoi(it->c_str());
        n++;
    }

    // inferred: a list that is read but not used
    str = csv->fn_8005B9CC();
    tok.assign(str, boost::char_separator<char>(":"));
    for (Tokenizer_unk::iterator it = tok.begin(); it != tok.end(); it++) {
    }

    param->mA8 = atoi(csv->fn_8005B9CC());
    // inferred: columns that are not used
    csv->fn_8005B9CC();
    csv->fn_8005B9CC();
    csv->fn_8005B9CC();
    param->mAC = atoi(csv->fn_8005B9CC());
    param->mB0 = atoi(csv->fn_8005B9CC());
    param->m24 = atoi(csv->fn_8005B9CC());
}

BulletParam_unk::BulletParam_unk() {
    m00 = 2;
    mKind = 12;
    m02 = 10;
    m03 = 100;
    mId = 0;
    m10 = 0;
    mA8 = 0;
    mAC = 0;
    mB0 = 0;
    mB4 = 0;
    mB8 = 0.0f;
    m14 = 0;
    m18 = 0;
    m1C = 0;
    m20 = 0;
    m24 = 0;
    m28 = 1.0f;
    m2C = 1.0f;
    m30 = 1.0f;
    m34 = 1.0f;
    m38 = 1.0f;
    m3C = 0;
    m40 = 1.0f;
    m44 = 0.0f;
    m48 = 0;
    m4C = 1.0f;
    m50 = 1.5707964f;
    m54 = 0.0f;
    m58 = 30.0f;
    m5C = 100;
    m60 = 0;
    m64 = 6;
    m68 = 0;
    m6C = 0;
    m70 = 0;
    memset(m74, 0, sizeof(m74));
    m90 = 0;
    m94 = 0;
    memset(m98, 0, sizeof(m98));
    mName = 0;
    mText = 0;
}

extern "C" void fn_8021A5F8(BulletParam_unk* param, atn::CsvU* csv, int version, char* buf, wchar_t* texts, int* a,
                            int* b) {
    *b = fn_802089C4(&param->mName, csv->fn_8005B9CC(), texts, *b);
    *b = fn_802089C4(&param->mText, csv->fn_8005B9CC(), texts, *b);
}

extern "C" bool fn_8021A77C(BulletParam_unk* param);
extern "C" bool fn_8021A888(BulletParam_unk* param);
extern "C" bool fn_8021A8D0(BulletParam_unk* param);
extern "C" bool fn_8021A918(BulletParam_unk* param);
extern "C" bool fn_8021A960(BulletParam_unk* param);
extern "C" bool fn_8021A984(BulletParam_unk* param);
extern "C" bool fn_8021A9A8(BulletParam_unk* param);
extern "C" bool fn_8021A9CC(BulletParam_unk* param);

// NONMATCHING: 98.06% - one instruction: the original copies the 0 of the first inlined bool from the
// result register (mr r4, r6) where here it is loaded again (li r4, 0); same in fn_8021A7A0
// (98.97%), fn_8021A9CC (98.06%) and fn_8021AA48 (97.86%). Tried if/else chains, a ternary, a local
// for the first test and other forms of the inlined predicates.
extern "C" bool fn_8021A684(BulletParam_unk* param) {
    bool result = false;
    if (fn_8021A77C(param) || (fn_8021A888(param) && !fn_8021A9CC(param))) {
        result = true;
    }
    return result;
}

extern "C" bool fn_8021A77C(BulletParam_unk* param) {
    return param != 0 && param->mKind == 0;
}

extern "C" bool fn_8021A7A0(BulletParam_unk* param) {
    bool result = false;
    if (fn_8021A888(param) || fn_8021A8D0(param) || fn_8021A918(param)) {
        result = true;
    }
    return result;
}

extern "C" bool fn_8021A888(BulletParam_unk* param) {
    bool result = false;
    if ((param != 0 && param->mKind == 1) || fn_8021A960(param)) {
        result = true;
    }
    return result;
}

extern "C" bool fn_8021A8D0(BulletParam_unk* param) {
    bool result = false;
    if ((param != 0 && param->mKind == 2) || fn_8021A984(param)) {
        result = true;
    }
    return result;
}

extern "C" bool fn_8021A918(BulletParam_unk* param) {
    bool result = false;
    if ((param != 0 && param->mKind == 3) || fn_8021A9A8(param)) {
        result = true;
    }
    return result;
}

extern "C" bool fn_8021A960(BulletParam_unk* param) {
    return param != 0 && param->mKind == 4;
}

extern "C" bool fn_8021A984(BulletParam_unk* param) {
    return param != 0 && param->mKind == 5;
}

extern "C" bool fn_8021A9A8(BulletParam_unk* param) {
    return param != 0 && param->mKind == 6;
}

extern "C" bool fn_8021A9CC(BulletParam_unk* param) {
    bool result = false;
    if (fn_8021A960(param) || fn_8021A984(param) || fn_8021A9A8(param)) {
        result = true;
    }
    return result;
}

extern "C" bool fn_8021AA48(BulletParam_unk* param) {
    bool result = false;
    if (fn_8021A8D0(param) && param != 0 && (param->mAC & 0x20) == 0x20) {
        result = true;
    }
    return result;
}
