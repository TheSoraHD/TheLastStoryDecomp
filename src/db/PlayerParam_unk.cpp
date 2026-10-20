#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int fn_80219558(int id); // inferred: player index (0-6) of a chara id (db/ReverbPresetParam_unk)
extern "C" int fn_8021AF98(int id); // inferred: skill id of a skill growth entry (db/SkillGrowth_unk)
struct HitInfo_unk;
extern "C" HitInfo_unk* fn_8021AFF4(int id); // inferred: skill data of a skill growth entry

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: game settings (lbl_8087F0A8); only the member used here
struct PlayerParamSettings_unk {
    char m000[0x194];
    int mDemo; // inferred: 0x194, demo version (selects the table)
};
extern "C" PlayerParamSettings_unk* lbl_8087F0A8;

// inferred: a player character; only the members used here
struct PlayerChara_unk {
    unsigned char m000[0x50];
    int mId; // inferred: 0x50, chara id
    unsigned char m054[0x874 - 0x54];
    int mLevel; // inferred: 0x874
};

// inferred: skills of a player from a level on (0x88 bytes, 32 per player, 7 players); the inline
// constructor is emitted out of line (fn_80219018) for __construct_new_array
struct PlayerParam_unk {
    PlayerParam_unk() : mPlayer(-1), mLevel(0) {
        memset(mSkills, 0, sizeof(mSkills));
        memset(mSlots, 0, sizeof(mSlots));
    }

    signed char mPlayer;
    signed char mLevel;
    bool mSkills[0x80];      // inferred: skill growth ids learnt
    signed char mSlots[6];   // inferred: skill growth id of each slot
};

static const char sFileNameStr[] = "/boot/player_param.csv";
static const char sDemoFileNameStr[] = "/boot/player_param_demo.csv";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sFileName = sFileNameStr;
static const char* const sDemoFileName = sDemoFileNameStr;

static PlayerParam_unk* sEntries;

// NONMATCHING: 88.49% - the original computes player * 32 inside the inner loop each time (here
// it is hoisted before the loop and the slot index is strength reduced), which shifts the
// registers; tried other index orders and an entry pointer/inline accessor.
// inferred: loads /boot/player_param.csv: for each player a "begin,skill,<player>" block of rows
// (level, skill, slot) up to "end"
extern "C" void fn_80218BD4() {
    unsigned long size;
    void* data = fn_8046DC5C(lbl_8087F518, lbl_8087F0A8->mDemo != 0 ? sDemoFileName : sFileName, &size, 0x20);
    sEntries = new (3, "", "", 0) PlayerParam_unk[7 * 32];
    atn::Csv csv((const char*)data, size);
    while (csv.fn_8005BBF8() != 0) {
        if (strcmp(csv.fn_8005B9CC(), "begin") != 0) {
            continue;
        }
        if (strcmp(csv.fn_8005B9CC(), "skill") != 0) {
            continue;
        }
        int player = atoi(csv.fn_8005B9CC());
        int slot = 0;
        while (csv.fn_8005BBF8() != 0 && slot < 32) {
            const char* field = csv.fn_8005B9CC();
            if (strcmp(field, "end") == 0) {
                break;
            }
            if (*field != '\0') {
                continue;
            }
            int level = atoi(csv.fn_8005B9CC());
            int skill = atoi(csv.fn_8005B9CC());
            int index = atoi(csv.fn_8005B9CC());
            if (skill > 0) {
                if (sEntries[slot + player * 32].mLevel < level) {
                    slot++;
                }
                for (int i = slot; i < 32; i++) {
                    PlayerParam_unk* entry = &sEntries[i + player * 32];
                    entry->mPlayer = player;
                    entry->mLevel = level;
                    entry->mSkills[skill] = true;
                    entry->mSlots[index] = skill;
                }
            }
        }
    }
    fn_8046DD20(lbl_8087F518, data);
}

extern "C" PlayerParam_unk* fn_80219160(int player, int level);

// inferred: skills of a chara at its level
extern "C" PlayerParam_unk* fn_80219074(PlayerChara_unk* chara) {
    int player = fn_80219558(chara->mId);
    int level = chara->mLevel;
    return fn_80219160(player, level);
}

// inferred: skills of a player at a level (0 if none)
extern "C" PlayerParam_unk* fn_80219160(int player, int level) {
    if (player < 0 || player > 6) {
        return 0;
    }
    PlayerParam_unk* found = 0;
    for (int i = 0; i < 32; i++) {
        PlayerParam_unk* entry = &sEntries[player * 32 + i];
        if (entry->mLevel < 0) {
            continue;
        }
        if (level < entry->mLevel) {
            break;
        }
        found = entry;
    }
    return found;
}

// NONMATCHING: 99.05% - registers only inside the inlined search (the level read after the call is
// in r9 in the original, r7 here); same for fn_80219344
extern "C" int fn_8021921C(PlayerChara_unk* chara, int slot) {
    if (chara == 0) {
        return 0;
    }
    PlayerParam_unk* entry = fn_80219074(chara);
    if (entry != 0) {
        return fn_8021AF98(entry->mSlots[slot]);
    }
    return 0;
}

extern "C" HitInfo_unk* fn_80219344(PlayerChara_unk* chara, int slot) {
    if (chara == 0) {
        return 0;
    }
    PlayerParam_unk* entry = fn_80219074(chara);
    if (entry != 0) {
        return fn_8021AFF4(entry->mSlots[slot]);
    }
    return 0;
}

extern "C" HitInfo_unk* fn_8021946C(int player, int level, int slot) {
    PlayerParam_unk* entry = fn_80219160(player, level);
    if (entry != 0) {
        return fn_8021AFF4(entry->mSlots[slot]);
    }
    return 0;
}
