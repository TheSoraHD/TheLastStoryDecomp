#ifndef DB_MAPIDTABLE_UNK_H
#define DB_MAPIDTABLE_UNK_H

// inferred: one map of /boot/map_id_table_xx.u16 (0xCC bytes); the inline constructor is emitted
// out of line (fn_802174E8) for __construct_new_array
struct MapId_unk {
    MapId_unk() : mCharas(0) {}

    const char* mId; // inferred: "wd000_00", "tw00_00", "dg000_00", "mp000_00"
    wchar_t mName[0x20];
    int mType; // inferred: 0x44: from the id prefix (fn_80216AFC)
    int mMap;
    int mPoint;
    int m50;
    int m54;
    int m58;
    const char* m5C;
    const char* m60;
    float m64;
    const char* m68;
    const char* m6C;
    const char* m70;
    int m74[16];
    unsigned int mCharas; // inferred: 0xB4: bit per chara group (/boot/map_motion_table.csv)
    int* mMembers;        // inferred: 0xB8: -1 terminated list (/boot/map_chara_list.csv)
    int mBC;
    int mC0;
    float mC4;
    int mC8;
};

// inferred: map by kind and numbers (kind 4 is searched as 3; db/MapIdTable_unk.cpp)
extern "C" MapId_unk* fn_8021771C(int type, int map, int point);

#endif
