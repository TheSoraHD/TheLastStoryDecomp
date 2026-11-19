#ifndef DB_CHARAIDTABLE_UNK_H
#define DB_CHARAIDTABLE_UNK_H

// inferred: one row of /boot/chara_id_table_xx.u16 (0x30 bytes); the texts point into the two
// buffers sWideTexts and sTexts of db/CharaIdTable_unk.cpp
struct CharaIdEntry_unk {
    int mId;
    const wchar_t* mName;      // inferred
    const wchar_t* mShortName; // inferred: the name again when fn_8006BA30 is set (shown by the menus)
    const char* m0C;
    const char* m10;
    const char* m14; // inferred: name of the character parameters (fn_8020A81C)
    const char* m18;
    const char* m1C;
    const char* m20;
    int m24;
    const char* m28;
    int mIcon; // 0x2C: inferred: face icon (1-based, 4 per row)
};

extern "C" CharaIdEntry_unk* fn_8020924C(int id); // inferred: entry of a character id

#endif
