#ifndef MP_MPLEVEL_UNK_H
#define MP_MPLEVEL_UNK_H

// inferred: experience table of the session (lbl_8087F610 + 0x610; names invented)
struct MpLevel_unk {
    int m00;
    int m04;
    int m08;
};
struct MpLevelTable_unk;
extern "C" MpLevel_unk* fn_80507610(MpLevelTable_unk* table, int exp);
extern "C" MpLevel_unk* fn_805075C4(MpLevelTable_unk* table, int exp); // inferred: level of an experience
extern "C" int fn_80507764(MpLevelTable_unk* table, int exp);
struct MpLevelTable_unk {
    int GetNext_unk(int exp) { return fn_80507610(this, exp) != 0 ? fn_80507610(this, exp)->m08 : m04->m04; }
    int m00;
    MpLevel_unk* m04;
};

#endif
