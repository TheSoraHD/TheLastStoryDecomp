// game/GameTask.cpp (0x8036555C-0x80376B68): the field task (GameTask). Only accessors are
// reconstructed so far.
#include "game/GameTask.h"
#include "db/StrategyAiTable_unk.h"
#include "game/GameScene.h"
#include "game/CharaTask.h"
#include "game/LibraryTask.h"
#include "game/MapTask.h"
#include "game/SelectMember.h"
#include "game/ShopBuySell.h"
#include "game/ShopColor.h"
#include "game/ShopIdentify.h"
#include "atn/FadeCommon.h"
#include "atn/EffectManager.h"
#include "game/AiTarget.h"
#include "game/EnemyManager.h"
#include "game/GameEventScript.h"
#include "game/GameUI.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "game/HomeButtonMenu.h"
#include "game/NpcManager.h"
#include "game/PlayerManager.h"
#include "game/PlayerTask.h"
#include "game/BattleTask.h"
#include "game/GameSaveData.h"
#include "game/GameSaveBackground.h"
#include "game/SetConfig.h"
#include "game/MapManager.h"
#include "game/FriendManager.h"
#include "game/ChaseManager.h"
#include "Event/Manager.h"
#include "game/BattleResult.h"
#include "game/CrossbowMenuTask.h"
#include "game/ColliAttrManager.h"
#include "game/Inventory.h"
#include "game/EnvSeManager.h"
#include "game/GameBgm.h"
#include "game/GameVoice.h"
#include "game/GameTimer.h"
#include "game/GameSoundControl.h"
#include "game/TacView.h"
#include "game/MarketTask.h"
#include "db/MotionCommandCommon.h"
#include "atn/SoundManager.h"
#include "game/MenuCommonTask.h"
#include "game/ResHandle.h"
#include "game/GameSaveBanner.h"
#include "game/HeapInfo_unk.h"
#include "game/EnemyTask.h"
#include "db/MapIdTable_unk.h"
#include "db/ChapterList_unk.h"
#include "atn/Font_unk.h"
// inferred: post effect manager (atn/PostFx.h, which redefines atn::IRender here)
extern "C" void* lbl_8087EF8C;

extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
extern "C" int fn_80570A4C(GameTaskData_unk* obj);
extern "C" GameTask* lbl_8087F430;
extern "C" atn::Task* lbl_8087F9E0; // inferred: upgrade shop (ShopUpGrade)
extern "C" PlayerManager* lbl_8087F8A0; // inferred: the party

inline MapTask* GameTask::GetCurrentMap() {
    MapTask* map = 0;
    if (m10D8 != 0) {
        map = m10D8->m64;
    }
    return map;
}

// inferred: save menu (game/CampSaveLoad.cpp)
class MenuSaveTask {
public:
    static MenuSaveTask* fn_8011770C(atn::Task* parent);
    static MenuSaveTask* sInstance;
};

// inferred: 0xC-byte entry of sGameTaskStatic_unk: a flag to set when the field starts (m00 1: global,
// else local; -1: unused)
struct GameTaskStaticEntry_unk {
    GameTaskStaticEntry_unk() : m00(-1), m04(0), m08(0) {}
    int m00;
    int m04;
    int m08;
};

// inferred: static object of this file (.bss 0x807C8458, 0x98 bytes). The constructor is __sinit_\GameTask_cpp.
// m04 is a GameTask::Option (used instead of the given one by fn_80365708)
struct GameTaskStatic_unk {
    GameTaskStatic_unk()
        : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1), m18(0), m1C(0), m20(0), m24(0), m28(0), m2C(0),
          m30(0), m34(0) {}
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28; // inferred: added to the level of the party (GameTask::vf0C)
    int m2C;
    int m30;
    int m34;
    GameTaskStaticEntry_unk mEntries[8]; // 0x38
};

GameTaskStatic_unk sGameTaskStatic_unk;

// inferred: object owned by GameTask + 0x558C (0x494 bytes; constructor and destructor in
// game/ReplayBuffer.cpp, 0x8056B388/0x8056B394). Name invented.
struct ReplayRecorder_unk {
    ReplayRecorder_unk();
    ~ReplayRecorder_unk();
    unsigned char m000[0x494];
};

// inferred: object of GameTask + 0x5590 (destructor 0x8056C398 in game/ReplayBuffer.cpp; other members
// in game/GameSystem_unk.cpp)
struct GameTaskData_unk {
    ~GameTaskData_unk();
};

// inferred: pause task of the field (__vt__13GamePauseTask 0x8078A394, size 0x78); the GameTask
// constructor inlines its creation. Member names invented.
class GamePauseTask : public atn::Task {
public:
    GamePauseTask(atn::Task* parent, GameTask* game, GameCamera* camera)
        : atn::Task(parent), mGame(game), mCamera(camera), mLeader(0), m54(0), m58(0), m5C(0) {}
    static GamePauseTask* Create_unk(atn::Task* parent, GameTask* game, GameCamera* camera) {
        GamePauseTask* task;
        if (parent != 0) {
            task = new ((MemoryTag)1, "", "", 0) GamePauseTask(parent, game, camera);
        } else {
            task = 0;
        }
        return task;
    }
    virtual ~GamePauseTask(); // 0x80376A0C
    virtual void vf10();      // 0x80376804

    GameTask* mGame;     // 0x48
    GameCamera* mCamera; // 0x4C
    PlayerTask* mLeader; // 0x50: inferred: leader of the party (GameTask::fn_80371320)
    int m54;
    int m58;
    void* m5C;
    unsigned char m60[0x70 - 0x60];
    ResHandle m70;
    int m74;
};

// inferred: debug message output (lbl_8087EEB8; atn/FileUtil_unk.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;

namespace atn {
// inferred: motion blur post effect (class in atn/PostFxMotionBlur.cpp)
class PostFxMotionBlur {
public:
    void fn_800AFEBC(int div);
};
} // namespace atn
// inferred: view of the post effect manager lbl_8087EF8C (motion blur at 0x118)
struct PostFxMgrView_unk {
    unsigned char m000[0x118];
    atn::PostFxMotionBlur mMotionBlur;
};

// inferred: views of other singletons used by the constructor and the destructor
struct GameFlow_unk {
    void SetLoading_unk(int on) {
        m18 = on;
        m1C = on;
    }
    unsigned char m00[0x0C];
    int m0C;
    unsigned char m10[0x18 - 0x10];
    int m18;
    int m1C;
};
struct Collision_unk {
    unsigned char m0000[0x9034];
    int m9034; // inferred: single player
};
struct SysInput_unk {
    unsigned char m0000[0x40E0];
    int m40E0;
};
struct MpSettings_unk {
    unsigned char m000[0x7C];
    int m07C;
    unsigned char m080[0xD4 - 0x80];
    int mD4;
    unsigned char m0D8[0x194 - 0xD8];
    int m194;
};
extern "C" GameFlow_unk* lbl_8087F420;
extern "C" Collision_unk* lbl_8087EE98;
extern "C" void* lbl_8087F610; // inferred: multiplayer session (null in single player)
extern "C" atn::EffectManager* lbl_8087F3C0;
extern "C" atn::SoundManager* lbl_8087EFE8;
extern "C" void* lbl_8087EE90; // inferred: sound engine
extern "C" void* lbl_8087F048; // inferred: the battle task
extern "C" void* lbl_8087F8A8;
extern "C" MpSettings_unk* lbl_8087F0A8;
extern "C" SysInput_unk* lbl_8087F018;
extern "C" MP::SaveData_unk* lbl_8087F460; // inferred: pending save data

extern "C" void fn_80212790();
extern "C" int fn_803CE160(int type, int map, int sub);
extern "C" int fn_803CE154();
extern "C" int fn_803CE148();
extern "C" void fn_80047B54(void* sound, int arg);
extern "C" void fn_800F2C9C(void* battle);
extern "C" void fn_8054A9A8(void* obj);
extern "C" void fn_800EF588(int type, int map);
extern "C" void fn_8004829C(void* sound, const char** groups, int count);
extern "C" void fn_803D022C(void* obj);
extern "C" void fn_803D0B20(void* obj);
extern "C" atn::Task* fn_8052A888(atn::Task* parent);
extern "C" int sprintf(char* buf, const char* fmt, ...);

extern "C" int fn_80370174(GameTask* task, unsigned int index);             // inferred: GetGlobalFlag
extern "C" int fn_80370A78(GameTask* task, unsigned int index);             // inferred: GetLocalFlag
extern "C" void fn_80370AE4(GameTask* task, unsigned int index, int value); // inferred: SetLocalFlag
extern "C" int lbl_8087F438; // inferred: the loading time is being measured
extern "C" HeapInfo_unk* lbl_8087F534;
extern "C" void* lbl_8087F518; // inferred: the loader
extern "C" void fn_8046ECDC(void* loader, int arg);
extern "C" atn::Font_unk* lbl_8087EEC8;
extern "C" void fn_8006D0C8(atn::Font_unk* font, int town);
extern "C" int fn_8006D3F8(atn::Font_unk* font);
extern "C" EnvSeManager* lbl_8087F418;
extern "C" int lbl_8087DD64;
extern "C" int fn_80219544(int member); // inferred: character id of a party member
extern "C" CharaInfo_unk* fn_8020924C(int id);
extern "C" chr::CharaData_unk* fn_8020A81C(const char* name); // inferred: character data by name
extern "C" int fn_80214394(int level, int magic); // inferred: weapon of the level
extern "C" int fn_8021446C(int level, int magic); // inferred: second weapon of the level
extern "C" int fn_802143EC(int level, int magic, int seed); // inferred: protector of the level
extern "C" void fn_8044D184(Inventory* inventory, int slot, int item, int upgrade, int index);
extern "C" void fn_80218268(int type, int map, int sub);
extern "C" void fn_803AB96C(void* obj, int arg, unsigned int index, int old, int value);
extern "C" void fn_803D164C(GameUI* ui);
extern "C" void fn_803D2194(GameUI* ui);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: the field of the option has no party (no battle reset; also tested by the constructor)
extern "C" __declspec(noinline) int fn_8036555C(int type, int map, int sub) {
    if (type == 1) {
        return 1;
    }
    if (type == 2) {
        switch (map) {
        case 16:
            return sub != 3;
        case 27:
            return sub == 1 || sub == 2;
        case 15:
        case 55:
            return sub == 1;
        case 14:
        case 31:
        case 33:
        case 34:
        case 41:
        case 51:
            return sub != 5 && sub != 3;
        case 68:
            return sub == 1;
        case 43:
            return sub == 1;
        case 48:
        case 53:
        case 54:
        case 69:
            return sub == 1;
        }
    }
    return 0;
}

extern "C" int fn_8036562C(int type, int map, int sub, int chapter) {
    if (type == 1) {
        return 1;
    }
    if (type == 2) {
        switch (map) {
        case 15:
        case 55:
            return sub == 1;
        case 68:
            return sub == 1;
        case 48:
            return sub == 1;
        case 14:
        case 31:
        case 33:
        case 34:
        case 41:
        case 51:
            return sub == 2 || sub == 4 || sub == 11;
        default:
            if (chapter >= 104000 && map == 8) {
                return sub == 1;
            }
            if (map == 71) {
                return sub == 1;
            }
            break;
        }
    }
    return 0;
}

// inferred: test of GameTask::fn_8036823C on the values of an option (inlined in the constructor)
static inline int IsSpecialField_unk(int type, int map, int sub) {
    if (type == 1) {
        return 1;
    }
    if (type == 2 && map == 0x44) {
        return sub == 1;
    }
    return 0;
}

atn::Task* fn_80365708(atn::Task* parent, const GameTask::Option& option) {
    if (parent != 0 && lbl_8087F430 == 0) {
        if (sGameTaskStatic_unk.m00 != 0) {
            lbl_8087F430 = new ((MemoryTag)1, "", "", 0)
                GameTask(parent, *(const GameTask::Option*)&sGameTaskStatic_unk.m04);
        } else if (lbl_8087F460 != 0) {
            GameSaveFlags_unk& flags = ((GameSaveData_unk*)lbl_8087F460)->mFlags;
            GameTask::Option opt(flags.mMap, flags.mPoint, flags.mArea, 0, -1, 0, 0);
            opt.m1C = (int)lbl_8087F460;
            lbl_8087F430 = new ((MemoryTag)1, "", "", 0) GameTask(parent, opt);
            if (lbl_8087F460 != 0) {
                delete lbl_8087F460;
                lbl_8087F460 = 0;
            }
        } else {
            lbl_8087F430 = new ((MemoryTag)1, "", "", 0) GameTask(parent, option);
        }
    }
    return lbl_8087F430;
}

GameTask::GameTask(atn::Task* parent, const Option& option)
    : atn::Task(parent), mOption(option), m10D0(0), m10D4(0), m10D8(0), m10DC(0), m10E0(0), m54E4(0), m54E8(15),
      m54EC(0), m54F0(0), m555C(0), m5560(0), m5564(0), m5568(0), m556C(0),
      m558C(new ((MemoryTag)1, "", "", 0) ReplayRecorder_unk), m5590(0), m5594(0), m5598(0), m55B0(0), m55B4(0),
      m55B8(0), m55BC(0), m5618(2), m561C(2), m5620(0), m5624(0),
      m5628(new ((MemoryTag)1, "", "", 0) GameSaveParty_unk), m5638(1), m563C(-1), m5640(0), m5644(0), m5648(0),
      m564C(0), m5650(0), m5654(0), m5658(0.0f), m565C(0), m5660(0), m5664(1), m5668(0), m566C(0), m5670(0),
      m5674(1), m567C(0), m5680(0), m5684(0), m5688(-1), m568C(-1), m5694(0), m5698(0), m569C(0), m56A0(0),
      m56A4(0), m56A8(0), m56AC(0), m56B0(0), m56B4(1.0f), m56B8(0.0f), m56BC(0), m56C0(0), m56C4(0.02f),
      m56C8(0.02f), m56D0(0), m56D4(0), m56D8(0), m56E4(0), m56E8(-1), m56EC(0), m56F0(0), m56F4(0), m5718(0),
      m571C(0), m5740(0), m5744(0), m5748(0), m574C(0), m5750(0), m5754(0), m5758(0), m575C(0), m5760(0),
      m5764(0) {
    if (lbl_8087F420 != 0) {
        lbl_8087F420->SetLoading_unk(1);
    }
    m5690.fn_8047043C("preload/game_start_dg.pk", 0, 0);
    if (lbl_8087F420 != 0) {
        lbl_8087F420->SetLoading_unk(0);
    }
    if (lbl_8087EE98 != 0) {
        lbl_8087EE98->m9034 = lbl_8087F610 == 0;
    }
    m562C.reset(new ((MemoryTag)1, "", "", 0) MP::SaveWork_unk);
    if (mOption.m1C != 0) {
        memcpy(m562C.mPtr->mData, (void*)mOption.m1C, sizeof(m562C.mPtr->mData));
        GameSaveData_unk* save = (GameSaveData_unk*)m562C.mPtr->mData;
        mOption.m1C = (int)save;
        mOption.mPos = save->mFlags.mState;
        mOption.mType = save->mFlags.mMap;
        mOption.mMap = save->mFlags.mPoint;
        mOption.mSub = save->mFlags.mArea;
        mOption.m14 = 1;
        m5630 = 0;
        m5748 = 5;
    } else {
        m5630 = 1;
    }
    m5634 = GameSaveBackground::fn_803B8C6C(this);
    if (lbl_8087F610 == 0) {
        if (mOption.mType == 2 && mOption.mMap == 9) {
            m10D4 = fn_80208634(mOption.mType, mOption.mMap, mOption.mSub);
        } else {
            m10D4 = fn_802085E0(mOption.mType, mOption.mMap);
        }
    }
    if (lbl_8087F610 == 0) {
        SetConfig::sInstance->fn_8057EF40();
    }
    fn_80212790();
    atn::EffectManager::fn_80239798(parent, 0x240, 0x480, 8, 0x180);
    lbl_8087F3C0->fn_8023A678(option.mType == 1 ? 0x80 : 0x120);
    lbl_8087F3C0->mCheckTrace = 1;
    if (option.mType != 3 && option.mType != 4) {
        GameCamera* camera = &mCamera;
        m10E0 = GamePauseTask::Create_unk(parent, this, camera);
    }
    MapManager::SetTask(this);
    PlayerManager::fn_80549D8C(this);
    EnemyManager::fn_80334B38(this);
    NpcManager::fn_805477C8(this);
    FriendManager::fn_80365464(this);
    BattleTask::fn_800F13BC(this);
    AIManager::fn_800144FC(this);
    ChaseManager::fn_801823A8(this);
    Event::Manager::fn_8047E4BC(this);
    GameUI::fn_803CE910(this);
    BattleResult::fn_800EFF68(this);
    CrossbowMenuTask::fn_801E9398(this);
    GimmickManager::SetTask(this);
    ColliAttrManager::fn_801839FC(this);
    Inventory::fn_80442F10();
    EnvSeManager::fn_8035DF48(this);
    GameBgm::fn_8037C8C4(this);
    GameVoice::fn_803E836C(this);
    GameTimer::fn_803CE4B0(this);
    GameSoundControl::fn_803CD9A8(this);
    TacView::fn_805A70F0(this);
    MarketTask::fn_804A1DDC(this);
    db::MotionCommandCommon::fn_80218720(this);
    lbl_8087EFE8->fn_800CDF84();
    lbl_8087EFE8->m34D4 = 1;
    memset(mGlobalFlags, 0, sizeof(mGlobalFlags));
    memset(mLocalFlags, 0, sizeof(mLocalFlags));
    mMessages.fn_803B3C10("/game_message/system_xx.u16");
    MenuCommonTask::fn_804A29C4(this);
    if (lbl_8087F610 == 0) {
        m567C = fn_8052A888(this);
    }
    if (lbl_8087EE90 != 0) {
        if (fn_803CE160(option.mType, option.mMap, option.mSub)) {
            void* sound = lbl_8087EE90;
            fn_80047B54(sound, fn_803CE154());
        } else {
            void* sound = lbl_8087EE90;
            fn_80047B54(sound, fn_803CE148());
        }
    }
    if (!fn_8036555C(option.mType, option.mMap, option.mSub)) {
        fn_800F2C9C(lbl_8087F048);
        fn_8054A9A8(lbl_8087F8A8);
    }
    ((PostFxMgrView_unk*)lbl_8087EF8C)->mMotionBlur.fn_800AFEBC(IsSpecialField_unk(option.mType, option.mMap, option.mSub) ? 4 : 2);
    fn_800EF588(option.mType, option.mMap);
    if (lbl_8087EE90 != 0 && lbl_8087F610 == 0) {
        const char* groups[2] = {"GROUP_SYSTEM", "GROUP_PC001"};
        fn_8004829C(lbl_8087EE90, groups, 2);
    }
    fn_803D022C(lbl_8087F490);
    if (!fn_8036555C(option.mType, option.mMap, option.mSub)) {
        fn_803D0B20(lbl_8087F490);
    }
    m5620 = atn::FadeColor::Create(this, 10, 0xFF000000, 0);
    m5624 = atn::FadeCapture::Create(this, 30, 0xFFFFFFFF, 0xFF000000, 0);
    mFinish.m368 = &mCamera;
    m5620->mFrame = 0;
    m5620->mDelay = 0;
    m5620->mDuration = 1;
    m5620->mWait = 999999999;
    m5620->mColor0 = 0xFF000000;
    m5620->mColor1 = 0xFF000000;
    m5620->mZ = 0.0f;
    m5620->mActive = 1;
    int chapter = mOption.mPos;
    if (lbl_8087EEB8 != 0) {
        char buf[0x100];
        if (m10D0 > chapter) {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "[WARNING] rollback chapter %d -> %d\n", m10D0, chapter);
            lbl_8087EEB8->fn_800697D8(buf);
        }
        if (m10D0 < 0 || 999999 < m10D0) {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "[WARNING] chapter is out of range %d\n", chapter);
            lbl_8087EEB8->fn_800697D8(buf);
        }
    }
    m10D0 = chapter;
    mGlobalFlags[0] = 1;
    mGlobalFlags[44] = 1;
    lbl_8087F0A8->mD4 = 1;
    SetConfig::sInstance->mShowTutorial = 1;
    if (lbl_8087F0A8->m194 != 0) {
        mGlobalFlags[290] = 1;
    }
    mGlobalFlags[30] = 1;
    if (mOption.mCallback != 0) {
        mOption.mCallback(this);
    }
    ((atn::Task*)lbl_8087F8A0)->fn_800D246C(1);
    EnemyManager::sInstance->fn_800D246C(1);
    NpcManager::sInstance->fn_800D246C(1);
    FriendManager::sInstance->fn_800D246C(1);
    MenuCommonTask::sInstance->fn_800D246C(1);
    if (m567C != 0) {
        m567C->fn_800D246C(1);
    }
    CrossbowMenuTask::sInstance->fn_800D246C(1);
    m56DC.vf0C("/camera/battle/cm000_lv000_00.xsca");
    HomeButtonMenu::sInstance->m88 = 0;
    if (lbl_8087F610 == 0) {
        lbl_8087F420->m0C = 1;
        if (lbl_8087F018 != 0) {
            lbl_8087F018->m40E0 = 1;
        }
    }
    fn_8037177C();
}

// inferred: returns lbl_8087F8A8 (called by atn/PacketPack)
extern "C" void* fn_8036635C() {
    return lbl_8087F8A8;
}

// inferred: dungeon number of a map id ("dg01_00" -> 100; "dg09_xx01" adds the sub dungeon)
static inline int GetDungeonId_unk(const char* id) {
    int dungeon = -1;
    if (strncmp(id, "dg", 2) == 0) {
        int n = atoi(id + 2);
        dungeon = n * 100;
        if (n == 9 && strlen(id) >= 8) {
            dungeon += atoi(id + 6);
        }
    }
    return dungeon;
}

// inferred: dungeons with a clear condition (lbl_8074DAF8)
static const int sDungeonIds_unk[] = {
    400, 800, 902, 912, 1000, 7100, 4800, 600, 1700, 1800, 1900, 1100, 700, 300, 2000,
    2200, 2300, 905, 915, 3200, 6300, 2500, 2600, 906, 916, 4200, 6700, 100, -1, 0,
};

static inline int IsDungeonListed_unk(int dungeon) {
    for (const int* p = sDungeonIds_unk; *p > 0; p++) {
        if (dungeon == *p) {
            return 1;
        }
    }
    return 0;
}

// inferred: global flag that marks a dungeon as cleared (lbl_8074DA2C)
struct DungeonFlag_unk {
    int mDungeon;
    int mFlag;
    int mValue;
};
static const DungeonFlag_unk sDungeonFlags_unk[] = {
    {902, 15, 100}, {912, 15, 100}, {1000, 16, 100}, {7100, 16, 100}, {1700, 17, 100}, {1800, 18, 100},
    {1100, 19, 100}, {700, 20, 100}, {300, 21, 100}, {2300, 22, 100}, {905, 23, 100}, {915, 23, 100},
    {906, 24, 100}, {916, 24, 100}, {4200, 25, 100}, {100, 26, 100}, {-1, -1, -1},
};

static inline const DungeonFlag_unk* FindDungeonFlag_unk(int dungeon) {
    for (int i = 0; sDungeonFlags_unk[i].mDungeon > 0; i++) {
        if (dungeon == sDungeonFlags_unk[i].mDungeon) {
            return &sDungeonFlags_unk[i];
        }
    }
    return 0;
}

// inferred: the dungeon of the field is already cleared (inlined twice in vf0C)
inline int GameTask::IsDungeonCleared_unk() {
    int cleared = 0;
    MapId_unk* map = fn_8021771C(mOption.mType, mOption.mMap, mOption.mSub);
    if (map != 0 && map->mType == 2 && IsDungeonListed_unk(GetDungeonId_unk(map->mId))) {
        const DungeonFlag_unk* flag = FindDungeonFlag_unk(GetDungeonId_unk(map->mId));
        if (flag == 0) {
            ChapterEntry_unk* chapter = fn_802085E0(2, map->mMap);
            if (chapter != 0 && chapter->mNext != 0) {
                cleared = m10D0 >= chapter->mNext->m04;
            }
        } else {
            cleared = fn_80370174(this, flag->mFlag) >= flag->mValue;
        }
    }
    return cleared;
}

int GameTask::vf0C() {
    if (lbl_8087F438 == 0) {
        lbl_8087F438 = 1;
        lbl_8087F534->mTimer4.Start_unk();
    }
    fn_8046ECDC(lbl_8087F518, 1);
    if (m10DC == 0) {
        if (!fn_800D3FA4()) {
            return 0;
        }
        if (Inventory::instance_->fn_8044386C()) {
            return 0;
        }
        if (GameSaveBanner::sInstance != 0) {
            return 0;
        }
        m10DC++;
        if (mOption.m1C != 0) {
            ((GameSaveData_unk*)mOption.m1C)->fn_803BE590();
        } else if (mOption.m18 == 0) {
            int level = 4;
            if (m10D4 != 0) {
                level = m10D4->m54;
            }
            if (sGameTaskStatic_unk.m00 != 0) {
                level += sGameTaskStatic_unk.m28;
            }
            for (int i = 0; i < 7; i++) {
                CharaInfo_unk* info = fn_8020924C(fn_80219544(i));
                chr::CharaData_unk* data;
                if (info != 0) {
                    data = fn_8020A81C((const char*)info->m14);
                } else {
                    data = 0;
                }
                if (data != 0) {
                    int kind = data->m11C;
                    int magic = 0;
                    if (kind == 3 || kind == 4) {
                        magic = 1;
                    }
                    int weapon = fn_80214394(level + data->mParam.m78 - 4, magic);
                    fn_8044D184(Inventory::instance_, 0, weapon, 0, i);
                    if (kind == 2) {
                        fn_8044D184(Inventory::instance_, 1, weapon, 0, i);
                    }
                    if (kind == 1) {
                        fn_8044D184(Inventory::instance_, 1, fn_8021446C(level + data->mParam.m78 - 4, magic), 0, i);
                    }
                    int protector = fn_802143EC(level + data->mParam.m78 - 4, magic, mOption.mMap + i);
                    for (int j = 0; j < 4; j++) {
                        Inventory::instance_->fn_8044D0DC(j, protector, -1, i);
                    }
                }
            }
        }
        if (sGameTaskStatic_unk.m00 != 0) {
            m5694 = sGameTaskStatic_unk.m30;
            m5698 = sGameTaskStatic_unk.m30;
            GameTaskStaticEntry_unk* entry = sGameTaskStatic_unk.mEntries;
            for (int i = 0; i < 8; i++, entry++) {
                if (entry->m00 != -1) {
                    if (entry->m00 == 1) {
                        fn_80370320(entry->m04, entry->m08, 1);
                    } else {
                        fn_80370AE4(this, entry->m04, entry->m08);
                    }
                }
            }
        } else if (m5694 != 0) {
            m5698 = m5694;
        } else {
            int cleared = IsDungeonCleared_unk();
            m5694 = cleared;
            m5698 = cleared;
        }
        fn_80218268(mOption.mType, mOption.mMap, mOption.mSub);
        lbl_8087F8A0->fn_80549F14(mOption.mType);
        if (fn_8036562C(mOption.mType, mOption.mMap, mOption.mSub, m10D0)) {
            NpcManager::sInstance->fn_80547980();
        }
        fn_8006D0C8(lbl_8087EEC8, mOption.mType == 1);
        if (lbl_8087F418 != 0) {
            lbl_8087F418->fn_8036097C(mOption.mType, mOption.mMap, mOption.mSub, mOption.m10, mOption.m1C);
        }
        return 0;
    }
    if (m10DC == 1) {
        if (lbl_8087F418 == 0 || !lbl_8087F418->fn_80360A2C()) {
            m10DC++;
        }
        return 0;
    }
    if (m10DC == 2) {
        if (lbl_8087F8A0->mHead->IsFlag_unk(2) && !NpcManager::sInstance->fn_80547B54()) {
            m5690.fn_80470528();
            m10DC++;
            m10D8 = GameScene::fn_803BEBAC(this, mOption.mType, mOption.mMap, mOption.mSub);
            if (mOption.m1C != 0) {
                m10D8->m9B0 = (GameSceneSave_unk*)mOption.m1C;
            }
            m10D8->fn_800D246C(1);
        }
        return 0;
    }
    int ready = 1;
    if (m56DC.fn_80473F50()) {
        ready = 0;
    }
    if (mMessages.fn_803B3CA8()) {
        ready = 0;
    }
    if (!fn_800D3FA4()) {
        ready = 0;
    }
    if (!lbl_8087F8A0->fn_800D3FA4()) {
        ready = 0;
    }
    if (!EnemyManager::sInstance->fn_800D3FA4()) {
        ready = 0;
    }
    if (!NpcManager::sInstance->fn_800D3FA4()) {
        ready = 0;
    }
    if (lbl_8087F490->fn_803D1574()) {
        ready = 0;
    }
    if (fn_8006D3F8(lbl_8087EEC8)) {
        ready = 0;
    }
    if (!ready) {
        return 0;
    }
    lbl_8087F8A0->fn_800D246C(0);
    EnemyManager::sInstance->fn_800D246C(0);
    NpcManager::sInstance->fn_800D246C(0);
    FriendManager::sInstance->fn_800D246C(0);
    MenuCommonTask::sInstance->fn_800D246C(0);
    MenuCommonTask::sInstance->FreezeTask();
    if (m567C != 0) {
        m567C->fn_800D246C(0);
        m567C->FreezeTask();
    }
    CrossbowMenuTask::sInstance->fn_800D246C(0);
    if (mOption.m1C != 0) {
        ((GameSaveData_unk*)mOption.m1C)->fn_803BE670();
    }
    fn_8036CA60(mOption.m10);
    if (mOption.m1C != 0) {
        ((GameSaveData_unk*)mOption.m1C)->fn_803BE6B8();
        mCamera.fn_8037EF30(1, 1, -1.0f);
    }
    fn_80367A40(1);
    if (lbl_8087F0A8->m07C != 0 && lbl_8087F8A0->FindByChara_unk(1) != 0) {
        PlayerManager* players = lbl_8087F8A0;
        players->FindByChara_unk(1)->vf30(players->m5C);
    }
    if (mOption.m1C == 0 && m10D0 >= 100000 && m10D0 < 403000) {
        fn_80370320(0x11F, 1, 0);
    }
    fn_803D164C(lbl_8087F490);
    fn_803D2194(lbl_8087F490);
    m5620->mFrame = 0;
    m5620->mDelay = 3;
    m5620->mDuration = 10;
    m5620->mWait = 0;
    m5620->mColor0 = 0xFF000000;
    m5620->mColor1 = 0;
    m5620->mZ = 0.0f;
    m5620->mActive = 1;
    m54F0 = 2;
    m5760 = 4;
    lbl_8087F534->mTimer4.Stop_unk();
    lbl_8087F438 = 0;
    if (sGameTaskStatic_unk.m00 != 0) {
        sGameTaskStatic_unk.m00 = 0;
    }
    if (lbl_8087F610 == 0) {
        HomeButtonMenu::sInstance->m88 = 1;
    }
    if (IsDungeonCleared_unk()) {
        PlayerTask* leader = lbl_8087F8A0->mHead;
        if (leader != 0) {
            MapId_unk* map = fn_8021771C(mOption.mType, mOption.mMap, mOption.mSub);
            int level = leader->mStatus.mParamB.m78;
            ChapterEntry_unk* chapter = fn_802085E0(mOption.mType, mOption.mMap);
            if (chapter != 0 && map != 0) {
                mGlobalFlags[281] = map->mC0 + (int)(level * map->mC4) - chapter->m54;
            }
        }
        for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
            if ((enemy->m137C & 0x40) != 0x40) {
                enemy->mStatus.fn_8012B3E8();
                enemy->mStatus.fn_8012B988();
            }
        }
    }
    lbl_8087DD64 = 15;
    return 1;
}

int GameTask::fn_8036823C() {
    if (mOption.mType == 1) {
        return 1;
    }
    if (mOption.mType == 2 && mOption.mMap == 0x44 && mOption.mSub == 1) {
        return 1;
    }
    return 0;
}

int GameTask::fn_8036DAA8(CharaTask* chara) {
    if (m56D8 != 0 || m56D4 != 0 || mFinish.m00 != 0) {
        return 0;
    }
    if (m54E4 != 0) {
        return 0;
    }
    m56D8 = chara;
    return 1;
}

// inferred: Ledge_unk (CharaTask.h) and GameSceneRoute_unk (GameScene.h) have the same 0x40-byte layout;
// unifying them renumbers the @N labels of ~50 units (pending)
int GameTask::fn_8036DAF4(Ledge_unk* route, CharaTask* chara, int check, int point) {
    GameScene* scene = m10D8;
    if (scene == 0) {
        return 0;
    }
    if (chara->IsOnLedge_unk()) {
        return 0;
    }
    return scene->fn_803CB044((GameSceneRoute_unk*)route, chara, check, point);
}

// inferred: `dir` is passed on in r8 but not used
int GameTask::fn_8036DB24(Ledge_unk* route, CharaTask* chara, int point, int exclude, const atn::Vector3* dir) {
    GameScene* scene = m10D8;
    if (scene != 0) {
        return scene->fn_803CB474((GameSceneRoute_unk*)route, chara, point, exclude);
    }
    return 0;
}

int GameTask::fn_8036DB3C(atn::Vector3* pos, int* index, CharaTask* chara) {
    GameScene* scene = m10D8;
    if (scene == 0) {
        return 0;
    }
    GameSceneRoute_unk route;
    if (scene->fn_803CB618(&route, chara)) {
        *index = route.mFromIndex;
        *pos = route.mFrom;
        return 1;
    }
    return 0;
}

// NONMATCHING: same instructions; the original loads mGlobalFlags[31] (m1160) before the two floats of `dir` (tried
// the declaration orders, assignments, bool, ?:, the dir built by members)
GimmickCommon* GameTask::fn_8036E098() {
    PlayerTask* leader;
    int any;
    any = mGlobalFlags[31] == 1;
    leader = lbl_8087F8A0->mHead;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', leader->mRot.y);
    PSMTXMultVec(m, &dir, &dir);
    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        if (g->mE8 == 6 && g->vf48(&leader->mPos, &dir, any)) {
            return g;
        }
    }
    return 0;
}

void GameTask::fn_8036E430(int a, int b) {
    m5624->m78 = 0x12;
    m5624->Capture();
    m5684 = 1;
    m5688 = a;
    m568C = b;
}

void GameTask::fn_8036E89C() {
    if (lbl_8087F430 != 0) {
        lbl_8087F430->fn_80370320(0x394, 0, 0);
    }
    fn_8036EE0C();
    if (ShopBuySell::sInstance != 0) {
        fn_800D2338(ShopBuySell::sInstance);
    }
    if (lbl_8087F9E0 != 0) {
        fn_800D2338(lbl_8087F9E0);
    }
    if (ShopColor::sInstance != 0) {
        fn_800D2338(ShopColor::sInstance);
    }
    if (ShopIdentify::sInstance != 0) {
        fn_800D2338(ShopIdentify::sInstance);
    }
}

void GameTask::fn_8036E92C(int arg) {
    fn_8036EA6C(4);
    MenuSaveTask::fn_8011770C(this);
}

void GameTask::fn_8036E964() {
    fn_8036EE0C();
    if (MenuSaveTask::sInstance != 0) {
        fn_800D2338((atn::Task*)MenuSaveTask::sInstance);
    }
}

void GameTask::fn_8036E994(int type, int a, int b) {
    fn_8036EA6C(5);
    m5750 = SelectMember::fn_8057162C(this, type, 1);
    m5754 = a;
    m5758 = b;
}

void GameTask::fn_8036EA04() {
    fn_8036EA6C(3);
    LibraryTask::fn_80468F54(this);
}

void GameTask::fn_8036EA3C() {
    fn_8036EE0C();
    if (LibraryTask::sInstance != 0) {
        fn_800D2338(LibraryTask::sInstance);
    }
}

void GameTask::fn_8036FB4C(int entry) {
    m54E4 = 12;
    m10D8->m134->fn_800D246C(1);
    lbl_8087F8A0->fn_800D246C(1);
    EnemyManager::sInstance->fn_800D246C(1);
    NpcManager::sInstance->fn_800D246C(1);
    lbl_8087EE68->fn_800D246C(1);
    lbl_8087F3C0->fn_800D246C(1);
    m56E4 = 0;
    m5624->mFrame = 0;
    m5624->mWait = 9999999;
    m5624->mColor0 = 0xFFFFFFFF;
    m5624->mColor1 = 0xFF000000;
    m5624->m78 = 0x12;
    m5624->mDuration = 30;
    if (m563C >= 0) {
        m5624->mActive = 1;
        m563C = -1;
    }
    lbl_8087F490->fn_803E4510();
    m56E8 = entry;
    HomeButtonMenu::sInstance->m88 = 0;
}

void GameTask::fn_8036FC58() {
    m54E4 = 0;
    m10D8->m134->fn_800D246C(0);
    lbl_8087F8A0->fn_800D246C(0);
    EnemyManager::sInstance->fn_800D246C(0);
    NpcManager::sInstance->fn_800D246C(0);
    lbl_8087EE68->fn_800D246C(0);
    lbl_8087F3C0->fn_800D246C(0);
    m5624->mFrame = 0;
    m5624->mWait = 0;
    m5624->mColor0 = 0xFF000000;
    m5624->mColor1 = 0;
    m5624->m78 = 0x12;
    m5624->mDuration = 30;
    if (m563C >= 0) {
        m5624->mActive = 1;
        m563C = -1;
    }
    mCamera.fn_803933A4();
    HomeButtonMenu::sInstance->m88 = 1;
}

extern "C" int fn_80370174(GameTask* task, unsigned int index) {
    if (index > 0xFFF) {
        if (lbl_8087EEB8 != 0) {
            char buf[0x100];
            sprintf(buf, "GetGlobalFlag:Index Out Of Bounds! %d", index);
            lbl_8087EEB8->fn_800697D8(buf);
        }
        return 0;
    }
    return task->mGlobalFlags[index];
}

extern "C" int fn_80370A78(GameTask* task, unsigned int index) {
    if (index > 0xFF) {
        if (lbl_8087EEB8 != 0) {
            char buf[0x100];
            sprintf(buf, "GetLocalFlag:Index Out Of Bounds! %d", index);
            lbl_8087EEB8->fn_800697D8(buf);
        }
        return 0;
    }
    return task->mLocalFlags[index];
}

extern "C" void fn_80370AE4(GameTask* task, unsigned int index, int value) {
    if (index > 0xFF) {
        if (lbl_8087EEB8 != 0) {
            char buf[0x100];
            sprintf(buf, "SetLocalFlag:Index Out Of Bounds! %d", index);
            lbl_8087EEB8->fn_800697D8(buf);
        }
        return;
    }
    int old = task->mLocalFlags[index];
    task->mLocalFlags[index] = value;
    if (task->m10D8 != 0 && task->m10D8->m134 != 0) {
        fn_803AB96C(task->m10D8->m134, 0, index, old, value);
    }
}

void GameTask::fn_80370B78(unsigned int color, int frames, int arg, float speed) {
    m54F0 = 1;
    m5620->mFrame = 1;
    m5620->mColor0 = color & 0xFFFFFF;
    m5620->mColor1 = color;
    m5620->mDelay = arg;
    m5620->mWait = 999999999;
    m5620->mDuration = frames;
    m5620->mZ = speed;
    m5620->mActive = 1;
}

void GameTask::fn_80370BD0(int frames, int arg, float speed) {
    unsigned int color = m5620->mColor1;
    m54F0 = 2;
    m5620->mFrame = 1;
    m5620->mColor0 = color;
    m5620->mColor1 = color & 0xFFFFFF;
    m5620->mDelay = arg;
    m5620->mWait = 0;
    m5620->mDuration = frames;
    m5620->mZ = speed;
    m5620->mActive = 1;
}

// NONMATCHING: 99% - r4/r5 swapped in the compare
int GameTask::fn_80370C2C() {
    atn::FadeCommon* fade = m5620;
    if (fade->mActive != 0) {
        return fade->mFrame < fade->mDuration + fade->mDelay;
    }
    return 1;
}

// NONMATCHING: 96% - the original branches bne/b around the type 4 test
void GameTask::fn_80370C6C() {
    int type = m10D8->m64->mType;
    if (type != 3) {
        if (type == 4) {
            return;
        }
        m5624->m78 = 0x12;
        m5624->Capture();
        m566C = 1;
    }
}

void GameTask::fn_80371320(PlayerTask* player) {
    if (m10E0 != 0) {
        m10E0->mLeader = player;
    }
}

void BattleTask::fn_80371664() {
    mCBAC = 0;
}

void GameTask::fn_8037177C() {
    m5768 = 0;
    m5774 = 0;
    m5770 = 0;
}

void GameTask::fn_80373118() {
    m5624->mDelay++;
    m5620->mDelay++;
    m5760++;
}

void GameTask::fn_80373164(CharaTask* owner, const atn::Vector3* pos, int id) {
    if (m56D4 != 0 && m56D4 == owner) {
        if (id != 0) {
            mFinish.m358 = (int)mFinish.m3B0 - 30;
        }
    } else if (m5594 != 0 && mFinish.m04 == 0 && mFinish.mChara == owner) {
        mFinish.mTarget = *pos;
        m5614 = m5598;
    }
}

void GameTask::fn_80373F78() {
    if (m5590 != 0 && fn_80570A4C(m5590) != 0) {
        fn_803731E8();
    }
}

int GameTask::fn_80375184() {
    return m5638 == 0;
}

void GameTask::fn_80375194(int id, int kind, float rot) {
    m5644 = id;
    m5640 = kind;
    PlayerTask* leader = lbl_8087F8A0->mHead;
    for (PlayerTask* player = leader; player != 0; player = player->mNext) {
        if (!player->IsFlag_unk(4) && !player->IsFlag54C_unk(0x2000) && player->mC54 != 0) {
            player->fn_801546F4(m5640, leader->mPos, rot);
        }
    }
}

StrategyAiEntry_unk* GameTask::fn_80375254() {
    StrategyAiEntry_unk* entry = fn_8021CF08(m5644, m5640);
    if (entry == 0) {
        entry = fn_8021CF08(0, m5640);
    }
    return entry;
}

void GameTask::fn_8037529C(unsigned int color0, unsigned int color1, int delay, int wait, int duration) {
    m5624->mFrame = 0;
    m5624->mColor0 = color0;
    m5624->mColor1 = color1;
    m5624->mDelay = delay;
    m5624->mWait = wait;
    m5624->mDuration = duration;
    m5624->mStep = 1;
    m5624->mZ = -100.0f;
    m5624->mActive = 1;
}

int GameTask::fn_803754C4() {
    atn::Task* task = m567C;
    if (task != 0 && !task->IsFlag_unk(4)) {
        return 1;
    }
    return 0;
}

// NONMATCHING: 93% - the null scene path joins the null map return directly
int GameTask::fn_80376150() {
    MapTask* map = GetCurrentMap();
    if (map == 0) {
        return 0;
    }
    int ret = 0;
    if (mGlobalFlags[4] == 0 && map->mType != 1) {
        ret = 1;
    }
    return ret;
}

void GameTask::fn_80376194(int on) {
    mGlobalFlags[4] = on == 0;
}

int GameTask::fn_803761A4() {
    return 1;
}

int GameTask::fn_803761AC() {
    return mGlobalFlags[6] == 0;
}

int GameTask::fn_803761BC() {
    return mGlobalFlags[5] == 0;
}

int GameTask::fn_803761CC() {
    return mGlobalFlags[11] == 0;
}

int GameTask::fn_803761DC() {
    return mGlobalFlags[2];
}

void GameTask::fn_803761E4(int visible) {
    mGlobalFlags[13] = visible;
}

int GameTask::fn_803761EC() {
    return mGlobalFlags[14];
}

void GameTask::fn_803761F4(int value) {
    mGlobalFlags[14] = value;
}

int GameTask::fn_803761FC() {
    return mGlobalFlags[32] != 0;
}

int GameTask::fn_80376210() {
    return mGlobalFlags[38] != 0;
}

int GameTask::fn_80376224() {
    return mGlobalFlags[39] != 0;
}

int GameTask::fn_80376238() {
    return mGlobalFlags[44] != 0;
}

void GameTask::fn_8037624C(int value) {
    mGlobalFlags[44] = value;
}

void GameTask::fn_803766E4() {
    if (m56F0 != 0) {
        fn_800D2338(m56F0);
        m56F0 = 0;
    }
    m571C = 0;
}
