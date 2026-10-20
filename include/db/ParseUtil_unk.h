#ifndef DB_PARSEUTIL_UNK_H
#define DB_PARSEUTIL_UNK_H

#include "atn/Csv.h"

// inferred: bullet/skill parameters (0xBC bytes) read from a CSV row by db/ParseUtil_unk.cpp; the
// rows of /boot/skill_param.csv (db/SkillParam_unk.cpp) extend it. Same object as HitInfo_unk
// (game/HitInfo_unk.h), which names more members. Names invented.
struct BulletParam_unk {
    BulletParam_unk(); // 0x8021A4E0

    unsigned char m00;
    signed char m01;
    unsigned char m02;
    unsigned char m03;
    int m04; // inferred: id
    unsigned char m08[0x60 - 0x08];
    int m60;
    int m64;
    unsigned char m68[0x70 - 0x68];
    int m70;
    unsigned char m74[0xBC - 0x74];
};

// inferred: reads the parameters of a row (0x80219F84); version is the "Version" of the file,
// buf/texts are the text buffers and *a/*b their used sizes
extern "C" void fn_80219F84(BulletParam_unk* param, atn::Csv* csv, int version, char* buf, wchar_t* texts, int* a,
                            int* b);
// inferred: reads the two texts of a row of the UTF-16 text file (0x8021A5F8)
extern "C" void fn_8021A5F8(BulletParam_unk* param, atn::CsvU* csv, int version, char* buf, wchar_t* texts, int* a,
                            int* b);

#endif
