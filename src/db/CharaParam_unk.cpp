#include "atn/Csv.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s);        // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: hash of a name

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: current field of the CSV reader (weak copy of an inline accessor of atn::CsvBase in
// another file; its name is unknown)
extern "C" const char* fn_800422CC(atn::Csv* csv);

// inferred: entry of the character id table (db/CharaIdTable_unk.cpp)
struct CharaIdEntry_unk {
    char m00[0x14];
    const char* m14; // inferred: name of the character parameters
};
extern "C" CharaIdEntry_unk* fn_802091E8(void* member);

// inferred: accessor of the resource manager (weak copy fn_8020A3E4)
inline Unk_8087F518* ResMgr_unk() {
    return lbl_8087F518;
}

// inferred: pair of numbers ("a:b"), cleared by its constructor (weak copy fn_8020A59C)
struct CharaParamPair_unk {
    CharaParamPair_unk() { memset(this, 0, sizeof(CharaParamPair_unk)); }
    int m00;
    int m04;
};

// inferred: two lists of up to 7 values (":"-separated columns)
struct CharaParamLists_unk {
    CharaParamLists_unk() {
        for (int i = 0; i < 7; i++) {
            mA[i] = 0.0f;
            mB[i] = 0.0f;
        }
    }
    float mA[7];
    float mB[7];
};

// inferred: one row of /boot/chara_param.csv (0x13C bytes)
struct CharaParamEntry_unk {
    CharaParamEntry_unk()
        : m00(0.0f), m04(0.0f), m08(0.0f), m0C(0.0f), m10(1.0f), m14(1.0f), m18(0.0f), m1C(0.0f), m20(0), m24(0),
          m28(0.0f), m2C(0.0f), m30(0), m34(0), m78(1), m7C(0), m80(0), m84(0), m88(0), m8C(0.0f), m90(0.0f),
          m94(0), m98(0), m9C(0), mHash(0), mC4(0), mC8(6), mCC(0), mD0(0) {
        m120[0] = 0;
        m120[1] = 0;
        m120[2] = 0;
        m120[3] = 0;
        m124 = 100.0f;
        m128 = 100;
        m12C = 0;
        m130 = 0;
        m134 = 1.0f;
        m138 = 1.0f;
        memset(mD4, 0, sizeof(mD4));
        memset(mD8, 0, sizeof(mD8));
        memset(m108, 0, sizeof(m108));
        for (int i = 0; i < 4; i++) {
            memset(&mE8[i], 0, sizeof(mE8[i]));
        }
    }

    float m00;
    float m04;
    float m08;
    float m0C;
    float m10;
    float m14;
    float m18;
    float m1C;
    int m20;
    int m24;
    float m28;
    float m2C;
    int m30;
    int m34;
    CharaParamLists_unk m38;
    int m70;
    int m74;
    int m78;
    int m7C;
    int m80;
    int m84;
    int m88;
    float m8C;
    float m90;
    int m94;
    int m98;
    int m9C;
    char mA0[0x20];
    unsigned int mHash;
    int mC4;
    int mC8;
    int mCC;
    int mD0;
    unsigned char mD4[4]; // inferred: weapon kinds ("ws" 0, "wg" 1, other 2, none 3)
    int mD8[4];           // inferred: weapon numbers
    CharaParamPair_unk mE8[4];
    int m108[5];
    int m11C;
    unsigned char m120[4]; // inferred: indices in the kind tables
    float m124;
    int m128;
    int m12C;
    int m130;
    float m134;
    float m138;
};

// inferred: kind names (four tables; the third one has a free slot)
static const char* const sKindsA[8] = {"aN", "aI", "aF", "aW", "aE", "aH", "aD", "aS"};
static const char* const sKindsB[10] = {"SP", "HM", "GG", "LZ", "UD", "GI", "BS", "RN", "MN", "DM"};
static const char* const sKindsC[6] = {"cX", "cF", "cM", "cH", "cA"};
static const char* const sKindsD[4] = {"sN", "sM", "sW", "sR"};
static const char sFileNameStr[] = "/boot/chara_param.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static CharaParamEntry_unk* sEntries;
static int sCount;

// inferred: index of a name in a table (0 if not found)
extern "C" int fn_802098E0(const char* name, const char* const* table, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(name, table[i]) == 0) {
            return i;
        }
    }
    return 0;
}

// NONMATCHING (weak copy fn_8020A5D4, tokenizer::assign): 95.93% - the original copies the strings of
// the separator word 1 first (as basic_string(const basic_string&)); changing operator= in
// std/string.h fixes it but lowers atn/D2A_Message, so the header is unchanged
typedef boost::tokenizer<boost::char_separator<char> > Tokenizer_unk;

// inferred: loads /boot/chara_param.csv
// NONMATCHING: 99.43% - register numbering of the token counters and their offsets (the original
// gives the counter the higher register); tried moving the declaration of the counter
extern "C" void fn_80209960() {
    unsigned long size = 0;
    atn::Csv csv;
    if (sEntries == 0) {
        void* data = fn_8046DC5C(ResMgr_unk(), sFileName, &size, 0x20);
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            if (strlen(csv.fn_8005B9CC()) != 0) {
                sCount++;
            }
        }
        sEntries = new ((MemoryTag)3, "", "", 0) CharaParamEntry_unk[sCount];
        csv.SetData((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            const char* name = csv.fn_8005B9CC();
            if (strlen(name) == 0) {
                continue;
            }
            memset(&sEntries[i], 0, sizeof(CharaParamEntry_unk));
            sEntries[i].mHash = fn_800DC6B4(name);
            csv.fn_8005B9CC(); // inferred: a column that is not used
            sEntries[i].m78 = atoi(csv.fn_8005B9CC());
            sEntries[i].m7C = atoi(csv.fn_8005B9CC());
            sEntries[i].mC8 = atoi(csv.fn_8005B9CC());
            sEntries[i].mCC = atoi(csv.fn_8005B9CC());
            sEntries[i].m80 = atoi(csv.fn_8005B9CC());
            sEntries[i].m98 = atoi(csv.fn_8005B9CC());
            sEntries[i].m84 = atoi(csv.fn_8005B9CC());
            sEntries[i].m88 = atoi(csv.fn_8005B9CC());
            if (sEntries[i].m88 <= 0) {
                sEntries[i].m88 = 100;
            }
            sEntries[i].m00 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m04 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m08 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m0C = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m10 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m14 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m94 = atoi(csv.fn_8005B9CC());
            sEntries[i].m28 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m2C = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m20 = atoi(csv.fn_8005B9CC());
            sEntries[i].m24 = atoi(csv.fn_8005B9CC());
            sEntries[i].m18 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m1C = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m30 = atoi(csv.fn_8005B9CC());
            sEntries[i].m34 = atoi(csv.fn_8005B9CC());
            sEntries[i].m134 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m8C = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m90 = fn_800DC288(csv.fn_8005B9CC());
            if (sEntries[i].m90 <= 0.0f) {
                sEntries[i].m90 = 5.0f;
            }
            std::string str(csv.fn_8005B9CC());
            Tokenizer_unk tok(str, boost::char_separator<char>(":"));
            int n = 0;
            for (Tokenizer_unk::iterator it = tok.begin(); it != tok.end() && n < 7; it++) {
                sEntries[i].m38.mA[n] = fn_800DC288(it->c_str());
                n++;
            }
            str = csv.fn_8005B9CC();
            tok.assign(str, boost::char_separator<char>(":"));
            n = 0;
            for (Tokenizer_unk::iterator it = tok.begin(); it != tok.end() && n < 7; it++) {
                sEntries[i].m38.mB[n] = fn_800DC288(it->c_str());
                n++;
            }
            sEntries[i].m124 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m128 = atoi(csv.fn_8005B9CC());
            sEntries[i].m12C = atoi(csv.fn_8005B9CC());
            sEntries[i].m130 = atoi(csv.fn_8005B9CC());
            sEntries[i].mD0 = atoi(csv.fn_8005B9CC());
            sEntries[i].m11C = atoi(csv.fn_8005B9CC());
            for (int j = 0; j < 4; j++) {
                if (strcmp(csv.fn_8005B9CC(), "") == 0) {
                    sEntries[i].mD4[j] = 3;
                    sEntries[i].mD8[j] = 0;
                } else {
                    if (strncmp(fn_800422CC(&csv), "ws", 2) == 0) {
                        sEntries[i].mD4[j] = 0;
                    } else if (strncmp(fn_800422CC(&csv), "wg", 2) == 0) {
                        sEntries[i].mD4[j] = 1;
                    } else {
                        sEntries[i].mD4[j] = 2;
                    }
                    sEntries[i].mD8[j] = atoi(fn_800422CC(&csv) + 2);
                }
            }
            for (int j = 0; j < 4; j++) {
                CharaParamPair_unk pair;
                int count = 0;
                std::string text(csv.fn_8005B9CC());
                Tokenizer_unk pairTok(text, boost::char_separator<char>(":"));
                int* value = &pair.m00;
                for (Tokenizer_unk::iterator it = pairTok.begin(); it != pairTok.end() && count < 2; ++it) {
                    *value++ = atoi(it->c_str());
                    count++;
                }
                sEntries[i].mE8[j] = pair;
            }
            for (int j = 0; j < 5; j++) {
                sEntries[i].m108[j] = atoi(csv.fn_8005B9CC() + 2);
            }
            sEntries[i].m9C = atoi(csv.fn_8005B9CC());
            sEntries[i].m138 = fn_800DC288(csv.fn_8005B9CC());
            sEntries[i].m120[0] = fn_802098E0(csv.fn_8005B9CC(), sKindsA, 8);
            sEntries[i].m120[1] = fn_802098E0(csv.fn_8005B9CC(), sKindsB, 10);
            sEntries[i].m120[2] = fn_802098E0(csv.fn_8005B9CC(), sKindsC, 5);
            sEntries[i].m120[3] = fn_802098E0(csv.fn_8005B9CC(), sKindsD, 4);
            sEntries[i].mC4 = atoi(csv.fn_8005B9CC());
            i++;
        }
        fn_8046DD20(ResMgr_unk(), data);
    }
}

// inferred: parameters of a character by name (the first entry if unknown)
extern "C" CharaParamEntry_unk* fn_8020A81C(const char* name) {
    if (name != 0) {
        unsigned int hash = fn_800DC6B4(name);
        for (int i = 0; i < sCount; i++) {
            if (hash == sEntries[i].mHash) {
                return &sEntries[i];
            }
        }
    }
    return sEntries;
}

// inferred: parameters of a party member
extern "C" CharaParamEntry_unk* fn_8020A888(void* member) {
    CharaIdEntry_unk* entry = fn_802091E8(member);
    if (entry != 0) {
        return fn_8020A81C(entry->m14);
    }
    return 0;
}
