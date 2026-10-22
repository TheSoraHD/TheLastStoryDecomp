// atn/SequenceUnitParam1_const_int.cpp: boot of the game sequences (name of the unit taken from the
// template; the real file is the one that registers BootTask/Title/Multiplayer/GameLoad/Game/Strap in the
// SequenceHolder tables, created from `main`, plus the debug map tables).
#include "atn/SequenceControl.h"
#include "atn/SequenceHolder.h"
#include "game/StrapTask.h"
#include "atn/SequenceBoot_unk.h"

using atn::Task;
using atn::SequenceHolder;
using atn::SequenceControl;

// inferred: the creators of the sequences (their function pointer types are in the typeid names)
// inferred: BootTask creator (C++ linkage: the typeid names of the creators have no extern "C")
Task* fn_8010EB58(Task* const parent);

// inferred: field object passed to the debug map callbacks
extern "C" void fn_80370320(void* field, int id, int value, int arg3); // inferred: sets a field flag

class GameTask {
public:
    // inferred: start options of the game task (0x24 bytes; names invented)
    struct Option {
        Option(int type, int map, int sub, int pos, int m10_, int m14_, void (*callback)(void*)) {
            mType = type;
            mMap = map;
            mSub = sub;
            mPos = pos;
            m10 = m10_;
            m14 = m14_;
            m18 = 0;
            m1C = 0;
            mCallback = callback;
        }
        int mType;
        int mMap;
        int mSub;
        int mPos;
        int m10;
        int m14;
        int m18;
        int m1C;
        void (*mCallback)(void*);
    };
};

Task* fn_80365708(Task* parent, const GameTask::Option& option); // inferred: GameTask creator

class GameLoadTask : public Task {
public:
    // inferred: name of the sequence to start after loading, plus a flag (0x44 bytes). It has a constructor,
    // so MWCC copies it member by member; the copies made by boost::function are never destroyed.
    struct Option {
        Option(const char* next, int flag) {
            if (next != mNext) {
                memcpy(mNext, next, strlen(next) + 1);
            }
            mFlag = flag;
        }
        // inferred: user copy constructor (the copies are unrolled into registers, also for arguments)
        Option(const Option& other) { *this = other; }
        char mNext[0x40];
        int mFlag;
    };
    // inferred: temporary used to build an Option (it has an out-of-line destructor, 0x803B29A8, that
    // the copies of Option do not call). Name invented.
    struct OptionTmp_unk : public Option {
        OptionTmp_unk(const char* next, int flag) : Option(next, flag) {}
        ~OptionTmp_unk(); // 0x803B29A8
    };
};

extern "C" GameLoadTask* fn_803B27F8(Task* parent, const GameLoadTask::Option& option); // inferred: creator

class NewTitleTask : public Task {};
NewTitleTask* fn_80531ED0(Task* parent, int arg); // inferred: NewTitleTask creator

namespace MP {
class Manager2;
}
extern "C" MP::Manager2* lbl_8087F610; // inferred: MP::Manager2 instance

namespace MP {
class Manager2 : public Task {
public:
    Manager2(Task* parent); // 0x804D0328
    // inferred: creates the instance once (inline, its "" literals are @STRING@ pools in .sdata)
    static Manager2* Create_unk(Task* parent) {
        if (lbl_8087F610 == 0) {
            lbl_8087F610 = new (1, "", "", 0) Manager2(parent);
        }
        return lbl_8087F610;
    }
    char m48[0x9A88 - 0x48];
};
} // namespace MP

// inferred: game settings (lbl_8087F0A8), only the boot mode is read here
struct BootSettings_unk {
    char m00[0x1B0];
    int mBootMode; // 0x1B0
};
extern "C" BootSettings_unk* lbl_8087F0A8;

// inferred: GameTask::Option is passed by value to SequenceUnitParam1 (copied by the caller)
template <>
struct atn::SequenceParam_unk<const GameTask::Option> {
    typedef const GameTask::Option type;
};

// inferred: entry of the debug map tables (filled by the __sinit)
struct DebugMap_unk {
    const char* mName;
    GameTask::Option mOption;
};

// inferred: debug start callback of "Noon Town"
static void fn_8057285C(void* field) {
    fn_80370320(field, 0x14, 0x64, 0);
    fn_80370320(field, 0x72B, 0x64, 0);
    fn_80370320(field, 0x71E, 1, 0);
}

SequenceBoot_unk* sSequenceBoot_unk;

// inferred: not inline (its literals are in @stringBase0); always inlined, the copy is stripped
SequenceBoot_unk::SequenceBoot_unk(Task* parent) : mParent(parent) {
    mHolders[0] = new (1, "", "", 0) SequenceHolder;
    mHolders[1] = new (1, "", "", 0) SequenceHolder;
    mHolders[2] = new (1, "", "", 0) SequenceHolder;
    mHolders[3] = new (1, "", "", 0) SequenceHolder;
    SequenceControl::fn_800C3610(mParent, mHolders[0]);
    if (lbl_8087F0A8->mBootMode == 2) {
        mHolders[0]->Regist_unk(SequenceHolder::SequenceUnit("BootTask", fn_8010EB58));
    } else {
        fn_80572BA0();
    }
    fn_80574918();
    SetNext_unk("BootTask");
}

void SequenceBoot_unk::Create_unk(Task* parent) {
    if (sSequenceBoot_unk == 0) {
        sSequenceBoot_unk = new (1, "", "", 0) SequenceBoot_unk(parent);
    }
}

void SequenceBoot_unk::SetNext_unk(const char* name) {
    if (SequenceControl::sInstance != 0) {
        SequenceControl::sInstance->mNextId = SequenceControl::sInstance->fn_800C37FC(name);
    }
}

static DebugMap_unk sDebugMaps1_unk[] = {
    {"Lizard's Cave 1", GameTask::Option(2, 4, 1, 0x18a88, -1, 1, 0)},
    {"Road To Castle (save point)", GameTask::Option(2, 8, 1, 0x18e70, -1, 1, 0)},
    {"Cryptoporticus", GameTask::Option(2, 0x13, 1, 0x49bb0, -1, 1, 0)},
    {"Mansion", GameTask::Option(2, 7, 1, 0x49f98, -1, 1, 0)},
    {"Ruri Castle (kanan)", GameTask::Option(2, 0x1d, 6, 0x62e08, -1, 1, 0)},
    {"Noon Town", GameTask::Option(1, 2, 1, 0x63da8, -1, 1, fn_8057285C)},
    {0, GameTask::Option(1, 1, 1, 0, -1, 0, 0)},
};

static DebugMap_unk sDebugMaps2_unk[] = {
    {"dg042_01 Lizard's Cave 5 01 -beta+-", GameTask::Option(2, 0x2a, 1, 0x92ba8, -1, 0, 0)},
    {"dg001_01 Mother's Chapel 01 -beta+-", GameTask::Option(2, 1, 1, 0x92ba8, -1, 0, 0)},
    {"dg041_02 Ruri Castle(Postwar) -beta+-", GameTask::Option(2, 0x29, 2, 0x92ba8, -1, 0, 0)},
    {"dg056_01 Ariel's 3 -beta+-", GameTask::Option(2, 0x38, 1, 0x1a1f8, -1, 0, 0)},
    {"dg060_02 Ruri Castle(Before the guard) -beta+-", GameTask::Option(2, 0x3c, 2, 0x1a5e0, -1, 0, 0)},
    {"dg027_01 Trista's ship 01 -beta+-", GameTask::Option(2, 0x1b, 1, 0x320c8, -1, 0, 0)},
    {"dg040_01 LastWorld 01 -beta+-", GameTask::Option(2, 0x28, 1, 0x7c060, -1, 0, 0)},
    {"dg067_01 Tasha battle -beta+-", GameTask::Option(2, 0x43, 1, 0x92ba8, -1, 0, 0)},
    {"tw04_01  Noon Town(Postwar) -beta+-", GameTask::Option(1, 4, 1, 0x92ba8, -1, 0, 0)},
    {"dg009_06 Arena 3 -beta+-", GameTask::Option(2, 9, 6, 0x92ba8, -1, 0, 0)},
    {"dg043_01 Knight's Oath -beta+-", GameTask::Option(2, 0x2b, 1, 0x92ba8, -1, 0, 0)},
    {"dg066_01 Hill Outside Of Town -beta+-", GameTask::Option(2, 0x42, 1, 0x92ba8, -1, 0, 0)},
    {"dg048_01 Observatory 1 -beta+-", GameTask::Option(2, 0x30, 1, 0x19e10, -1, 0, 0)},
    {"dg053_01 Observatory 2 -beta+-", GameTask::Option(2, 0x35, 1, 0x92ba8, -1, 0, 0)},
    {"dg054_01 Observatory 3 -beta+-", GameTask::Option(2, 0x36, 1, 0x92ba8, -1, 0, 0)},
    {"dg009_05 arena 2 -beta-", GameTask::Option(2, 9, 5, 0x635d8, -1, 0, 0)},
    {"dg032_01 gurg castle 01 -beta-", GameTask::Option(2, 0x20, 1, 0x639c0, -1, 0, 0)},
    {"dg033_01 ruri castle(false victory) 01 -beta-", GameTask::Option(2, 0x21, 1, 0x63da8, -1, 0, 0)},
    {"dg063_01 lizard's cave 3 01 -beta-", GameTask::Option(2, 0x3f, 1, 0x63e0c, -1, 0, 0)},
    {"dg034_01 ruri castle(conferment ceremony) 01 -beta-", GameTask::Option(2, 0x22, 1, 0x64190, -1, 0, 0)},
    {"dg035_02 ruri castle(shell a castle) 02 -beta-", GameTask::Option(2, 0x23, 2, 0x7a508, -1, 0, 0)},
    {"dg036_01 fortress island 01 -beta-", GameTask::Option(2, 0x24, 1, 0x7b4a8, -1, 0, 0)},
    {"dg036_07 ruri castle (jill 01) -beta-", GameTask::Option(2, 0x24, 7, 0x7b4a8, -1, 0, 0)},
    {"dg037_01 gurg ship 3 01 -beta-", GameTask::Option(2, 0x25, 1, 0x7b890, -1, 0, 0)},
    {"dg039_01 lizard's cave 4 01 -beta-", GameTask::Option(2, 0x27, 1, 0x7bc78, -1, 0, 0)},
    {"tw06_01  night town -5th-", GameTask::Option(1, 6, 1, 0, -1, 0, 0)},
    {"dg058_01 chase a knucker -5th-", GameTask::Option(2, 0x3a, 1, 0x61e68, -1, 0, 0)},
    {"dg014_08 military facilities -5th-", GameTask::Option(2, 0xe, 8, 0, -1, 0, 0)},
    {"dg014_09 viewing deck -5th-", GameTask::Option(2, 0xe, 9, 0, -1, 0, 0)},
    {"dg014_11 library room -5th-", GameTask::Option(2, 0xe, 0xb, 0, -1, 0, 0)},
    {"dg016_01 gurg ship (town) 01 -5th-", GameTask::Option(2, 0x10, 1, 0x318f8, -1, 0, 0)},
    {"dg017_01 wrecked ship 01 -5th-", GameTask::Option(2, 0x11, 1, 0x318f8, -1, 0, 0)},
    {"dg020_01 darkness corridor 01 -5th-", GameTask::Option(2, 0x14, 1, 0x4a768, -1, 0, 0)},
    {"dg021_01 ruri castle (move) 01 -5th-", GameTask::Option(2, 0x15, 1, 0x61e68, -1, 0, 0)},
    {"dg022_01 trial ground 01 -5th-", GameTask::Option(2, 0x16, 1, 0x62250, -1, 0, 0)},
    {"dg023_01 lizard's cave 2 01 -5th-", GameTask::Option(2, 0x17, 1, 0x62638, -1, 0, 0)},
    {"dg024_02 ruri castle (raided) 02 -5th-", GameTask::Option(2, 0x18, 2, 0x62a20, -1, 0, 0)},
    {"dg025_01 turret underground 01 -5th-", GameTask::Option(2, 0x19, 1, 0x7a8f0, -1, 0, 0)},
    {"dg025_04 ayatsuri no ryoiki -5th-", GameTask::Option(2, 0x19, 4, 0x7a8f0, -1, 0, 0)},
    {"dg029_02 ruri castle (escape) 02 -5th-", GameTask::Option(2, 0x1d, 2, 0x62e08, -1, 0, 0)},
    {"dg030_01 ruri castle (sneak) 01 -5th-", GameTask::Option(2, 0x1e, 1, 0x631f0, -1, 0, 0)},
    {"dg031_01 ruri castle (arrival) 01 -5th-", GameTask::Option(2, 0x1f, 1, 0x635d8, -1, 0, 0)},
    {"dg018_01 mysterious forest 01 -alpha-", GameTask::Option(2, 0x12, 1, 0x318f8, -1, 0, 0)},
    {"dg019_01 cryptoporticus 01 -alpha-", GameTask::Option(2, 0x13, 1, 0x49bb0, -1, 0, 0)},
    {"dg026_01 underpass 01 -alpha-", GameTask::Option(2, 0x1a, 1, 0x7b0c0, -1, 0, 0)},
    {"tw01_01  sunset town -2nd-", GameTask::Option(1, 1, 1, 0, -1, 0, 0)},
    {"tw02_01  noon town -3rd-", GameTask::Option(1, 2, 1, 0, -1, 0, 0)},
    {"tw03_01  night town (fireworks) -4th-", GameTask::Option(1, 3, 1, 0, -1, 0, 0)},
    {"dg001_01 abbey -1st-", GameTask::Option(2, 1, 1, 0x92f90, -1, 0, 0)},
    {"dg002_01 ship01 -2nd-", GameTask::Option(2, 2, 1, 0x31128, -1, 0, 0)},
    {"dg003_01 tavern -2nd-", GameTask::Option(2, 3, 1, 0x49f98, -1, 0, 0)},
    {"dg004_01 lizard01 -3rd-", GameTask::Option(2, 4, 1, 0x18a88, -1, 0, 0)},
    {"dg005_01 castle01 -3rd-", GameTask::Option(2, 5, 1, 0x1a5e0, -1, 0, 0)},
    {"dg006_01 cliff01 -3rd-", GameTask::Option(2, 6, 1, 0x1adb0, -1, 0, 0)},
    {"dg007_01 mansion -3rd-", GameTask::Option(2, 7, 1, 0x49f98, -1, 0, 0)},
    {"dg008_01 road for castle -4th-", GameTask::Option(2, 8, 1, 0x18e70, -1, 0, 0)},
    {"dg009_01 arena01 -4th-", GameTask::Option(2, 9, 1, 0x19258, -1, 0, 0)},
    {"dg010_01 storehouse01 -4th-", GameTask::Option(2, 0xa, 1, 0x19a28, -1, 0, 0)},
    {"dg011_01 lizards' gallery01  -4th-", GameTask::Option(2, 0xb, 1, 0x49f98, -1, 0, 0)},
    {"dg012_01 gurgbase01 -4th-", GameTask::Option(2, 0xc, 1, 0x31ce0, -1, 0, 0)},
    {"dg013_01 harbor -4th-", GameTask::Option(2, 0xd, 1, 0x7acd8, -1, 0, 0)},
    {"dg014_01 ball court -4th-", GameTask::Option(2, 0xe, 1, 0x497c8, -1, 0, 0)},
    {"dg014_02 large hall -4th-", GameTask::Option(2, 0xe, 2, 0x497c8, -1, 0, 0)},
    {"dg014_03 patio -4th-", GameTask::Option(2, 0xe, 3, 0x497c8, -1, 0, 0)},
    {"dg014_04 arganan's room -4th-", GameTask::Option(2, 0xe, 4, 0x497c8, -1, 0, 0)},
    {"dg014_05 interrogation center -4th-", GameTask::Option(2, 0xe, 5, 0x497c8, -1, 0, 0)},
    {"dg014_06 cage -4th-", GameTask::Option(2, 0xe, 6, 0x497c8, -1, 0, 0)},
    {"dg014_07 deep underground -4th-", GameTask::Option(2, 0xe, 7, 0x497c8, -1, 0, 0)},
    {"dg015_01 ariel's -4th-", GameTask::Option(2, 0xf, 1, 0, -1, 0, 0)},
    {"Game", GameTask::Option(2, 1, 1, 0, -1, 0, 0)},
    {0, GameTask::Option(1, 1, 1, 0, -1, 0, 0)},
};

static DebugMap_unk sDebugMaps3_unk[] = {
    {"dg999_01 throw test", GameTask::Option(2, 0x3e7, 1, 0, -1, 0, 0)},
    {"dg999_02 simple test", GameTask::Option(2, 0x3e7, 2, 0, -1, 0, 0)},
    {"dg999_04 crowd test", GameTask::Option(2, 0x3e7, 4, 0, -1, 0, 0)},
    {"dg999_05 battle test", GameTask::Option(2, 0x3e7, 5, 0, -1, 0, 0)},
    {"dg999_96 shop test", GameTask::Option(2, 0x3e7, 0x60, 0, -1, 0, 0)},
    {"dg999_97 npc test", GameTask::Option(2, 0x3e7, 0x61, 0, -1, 0, 0)},
    {"dg999_98 simple map", GameTask::Option(2, 0x3e7, 0x62, 0, -1, 0, 0)},
    {"dg999_20 test dungeon A1", GameTask::Option(2, 0x3e7, 0x14, 0, -1, 0, 0)},
    {"dg999_20 test dungeon A2", GameTask::Option(2, 0x3e7, 0x18, 0, -1, 0, 0)},
    {"dg999_21 test dungeon B", GameTask::Option(2, 0x3e7, 0x15, 0, -1, 0, 0)},
    {"dg999_26 damage test", GameTask::Option(2, 0x3e7, 0x1a, 0, -1, 0, 0)},
    {"dg999_27 chara test", GameTask::Option(2, 0x3e7, 0x1b, 0, -1, 0, 0)},
    {"dg999_10 damage test 2", GameTask::Option(2, 0x3e7, 0xa, 0, -1, 0, 0)},
    {0, GameTask::Option(1, 1, 1, 0, -1, 0, 0)},
};
void SequenceBoot_unk::fn_80572BA0() {
    mHolders[0]->Regist_unk(SequenceHolder::SequenceUnitParam1<const int>("Title", fn_80531ED0, -1));
    for (DebugMap_unk* map = sDebugMaps1_unk; map->mName != 0; map++) {
        mHolders[0]->Regist_unk(
            SequenceHolder::SequenceUnitParam1<const GameTask::Option>(map->mName, fn_80365708, map->mOption));
    }
    mHolders[0]->Regist_unk(SequenceHolder::SequenceUnitParam1<const GameLoadTask::Option>(
        "GameLoadMultiplayer", fn_803B27F8, GameLoadTask::OptionTmp_unk("Multiplayer", 1)));
    mHolders[0]->Regist_unk(SequenceHolder::SequenceUnit("Multiplayer", MP::Manager2::Create_unk));
    mHolders[0]->Regist_unk(SequenceHolder::SequenceUnitParam1<const GameLoadTask::Option>(
        "GameLoad", fn_803B27F8, GameLoadTask::OptionTmp_unk("Game", 1)));
    mHolders[0]->Regist_unk(SequenceHolder::SequenceUnit("Strap", StrapTask::fn_805A6208));
    mHolders[0]->Regist_unk(SequenceHolder::SequenceUnitParam1<const GameTask::Option>(
        "Game", fn_80365708, GameTask::Option(2, 4, 1, 0x18A88, -1, 1, 0)));
    mHolders[0]->Regist_unk(SequenceHolder::SequenceUnit("BootTask", fn_8010EB58));
}

void SequenceBoot_unk::fn_80574918() {
    mHolders[1]->Regist_unk(SequenceHolder::SequenceUnitParam1<const int>("Title", fn_80531ED0, -1));
    for (DebugMap_unk* map = sDebugMaps2_unk; map->mName != 0; map++) {
        mHolders[1]->Regist_unk(
            SequenceHolder::SequenceUnitParam1<const GameTask::Option>(map->mName, fn_80365708, map->mOption));
    }
    mHolders[1]->Regist_unk(SequenceHolder::SequenceUnitParam1<const GameLoadTask::Option>(
        "GameLoadMultiplayer", fn_803B27F8, GameLoadTask::OptionTmp_unk("Multiplayer", 1)));
    mHolders[1]->Regist_unk(SequenceHolder::SequenceUnit("Multiplayer", MP::Manager2::Create_unk));
    mHolders[1]->Regist_unk(SequenceHolder::SequenceUnitParam1<const GameLoadTask::Option>(
        "GameLoad", fn_803B27F8, GameLoadTask::OptionTmp_unk("Game", 1)));
}
