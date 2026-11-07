#ifndef DB_CHAPTERLIST_UNK_H
#define DB_CHAPTERLIST_UNK_H

// inferred: one row of /boot/chapter_list.csv (0x64 bytes); the rows are linked in order
struct ChapterEntry_unk {
    ChapterEntry_unk() : m04(0), mType(-1), mMap(0), mPoint(0), m54(0), m58(0), mPrev(0), mNext(0) {}

    int m00;
    int m04;
    int mType;  // inferred: from the map id prefix ("tw" 1, "dg" 2, otherwise 7)
    int mMap;   // inferred: number after the prefix
    int mPoint; // inferred: second number of a "dg" id
    char mName[0x40];
    int m54;
    int m58;
    ChapterEntry_unk* mPrev;
    ChapterEntry_unk* mNext;
};

// inferred: chapter of a map (db/ChapterList_unk.cpp)
extern "C" ChapterEntry_unk* fn_802085E0(int type, int map);
extern "C" ChapterEntry_unk* fn_80208634(int type, int map, int point);

#endif
