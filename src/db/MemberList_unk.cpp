#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int atoi(const char* s);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of /boot/member_list.csv (0x14 bytes): from a level on, which of the 7
// members are in the party (and in the camp, /boot/member_camp_list.csv); the constructor is
// emitted out of line (fn_80217D64) for __construct_new_array
struct MemberList_unk {
    MemberList_unk() { memset(this, 0, sizeof(MemberList_unk)); }

    int mLevel;
    bool mMembers[7];
    unsigned char mMemberCount;
    bool mCampMembers[7];
    unsigned char mCampMemberCount;
};

static const char sFileNameStr[] = "/boot/member_list.csv";
static const char sCampFileNameStr[] = "/boot/member_camp_list.csv";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sFileName = sFileNameStr;
static const char* const sCampFileName = sCampFileNameStr;

static MemberList_unk* sEntries;
static int sCount;

extern "C" MemberList_unk* fn_80217D9C(int level);

// inferred: loads /boot/member_list.csv and /boot/member_camp_list.csv (rows whose first column is
// empty)
extern "C" void fn_802179FC() {
    atn::Csv csv;
    if (sEntries == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() == '\0') {
                sCount++;
            }
        }
        sEntries = new ((MemoryTag)12, "", "", 0) MemberList_unk[sCount];
        csv.SetData((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() != '\0') {
                continue;
            }
            MemberList_unk* entry = &sEntries[i];
            entry->mLevel = atoi(csv.fn_8005B9CC());
            csv.fn_8005B9CC(); // inferred: columns that are not used
            csv.fn_8005B9CC();
            entry->mMembers[0] = true;
            entry->mMemberCount++;
            for (int j = 1; j < 7; j++) {
                const char* field = csv.fn_8005B9CC();
                entry->mMembers[j] = *field != '\0';
                if (entry->mMembers[j]) {
                    entry->mMemberCount++;
                }
            }
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);

        data = fn_8046DC5C(lbl_8087F518, sCampFileName, &size, 0x20);
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() != '\0') {
                continue;
            }
            MemberList_unk* entry = fn_80217D9C(atoi(csv.fn_8005B9CC()));
            if (entry == 0) {
                continue;
            }
            csv.fn_8005B9CC(); // inferred: columns that are not used
            csv.fn_8005B9CC();
            entry->mCampMembers[0] = true;
            entry->mCampMemberCount++;
            for (int j = 1; j < 7; j++) {
                const char* field = csv.fn_8005B9CC();
                entry->mCampMembers[j] = *field != '\0';
                if (entry->mCampMembers[j]) {
                    entry->mCampMemberCount++;
                }
            }
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: row of a level (the last row whose level is not above it)
// NONMATCHING: 98.39% - registers only: count - 1 and its offset (count - 1) * 0x14 are swapped
// between r4 and r5; tried locals for the count, the table and the first level, a row pointer
// and other comparison orders.
extern "C" MemberList_unk* fn_80217D9C(int level) {
    int count = sCount;
    int last = sEntries[count - 1].mLevel;
    if (level <= sEntries[0].mLevel) {
        return sEntries;
    }
    if (last <= level) {
        return &sEntries[count - 1];
    }
    for (int i = 1; i < count; i++) {
        if (sEntries[i].mLevel > level) {
            return &sEntries[i - 1];
        }
    }
    return sEntries;
}
