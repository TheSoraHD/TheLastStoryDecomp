#ifndef DB_PARSEUTIL_UNK_H
#define DB_PARSEUTIL_UNK_H

#include "atn/Csv.h"

// inferred: bullet/skill parameters (0xBC bytes) read from a CSV row by db/ParseUtil_unk.cpp; the
// rows of /boot/skill_param.csv (db/SkillParam_unk.cpp) extend it. Same object as HitInfo_unk
// (game/HitInfo_unk.h), which names more members. Names invented.
struct BulletParam_unk {
    BulletParam_unk(); // 0x8021A4E0

    unsigned char m00;
    signed char mKind; // inferred: 0-6 (tested by fn_8021A684..fn_8021AA48)
    unsigned char m02;
    unsigned char m03;
    int mId;
    const wchar_t* mName; // inferred: texts of the UTF-16 text file (fn_8021A5F8)
    const wchar_t* mText;
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    float m28;
    float m2C;
    float m30;
    float m34;
    float m38;
    int m3C;
    float m40;
    float m44;
    int m48;
    float m4C;
    float m50; // inferred: angle (read in degrees)
    float m54;
    float m58;
    int m5C;
    int m60;
    int m64;
    int m68;
    int m6C;
    int m70;
    float m74[7]; // inferred: ":"-separated list
    int m90;      // inferred: "a:b" pair
    int m94;
    int m98[4]; // inferred: ":"-separated list
    int mA8;
    int mAC; // inferred: flags
    int mB0;
    int mB4;
    float mB8;
};

// inferred: reads the parameters of a row (0x80219F84); version is the "Version" of the file,
// buf/texts are the text buffers and *a/*b their used sizes (only used by fn_8021A5F8)
extern "C" void fn_80219F84(BulletParam_unk* param, atn::Csv* csv, int version, char* buf, wchar_t* texts, int* a,
                            int* b);
// inferred: reads the two texts of a row of the UTF-16 text file (0x8021A5F8)
extern "C" void fn_8021A5F8(BulletParam_unk* param, atn::CsvU* csv, int version, char* buf, wchar_t* texts, int* a,
                            int* b);

#endif
