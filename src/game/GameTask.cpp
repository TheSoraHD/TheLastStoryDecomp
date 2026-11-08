// game/GameTask.cpp (0x8036555C-0x80376B68): the field task (GameTask). The functions that use the string
// pool are written in order up to 0x80370AE4 (fn_80370A78); the rest are accessors and dungeon helpers.
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
#include "atn/SequenceBoot_unk.h"
#include "game/MarketTask.h"
#include "db/MotionCommandCommon.h"
#include "atn/SoundManager.h"
#include "game/MenuCommonTask.h"
#include "game/ResHandle.h"
#include "atn/LoadManager.h"
#include "game/NpcTask.h"
#include "game/SkillEffectManager.h"
#include "game/LogMessageTask.h"
#include "atn/RenderSetting_unk.h"
#include "atn/Keyboard.h"
#include "atn/D2A_Task.h"
#include "atn/DrawHelper.h"
#include "atn/MemoryManager.h"
#include "Event/Sequence.h"
#include "Event/Cut.h"
#include "game/TutorialTask.h"
#include "game/GameChoiceWindow.h"
#include "game/GameSaveBanner.h"
#include "game/HeapInfo_unk.h"
#include "game/EnemyTask.h"
#include "db/MapIdTable_unk.h"
#include "db/ChapterList_unk.h"
#include "atn/Font_unk.h"
#include "atn/FixedArray.h"
#include "game/Input_unk.h"
#include "std/vector.h"
// inferred: post effect manager (atn/PostFx.h, which redefines atn::IRender here)
extern "C" void* lbl_8087EF8C;

extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
extern "C" int fn_80570A4C(GameTaskData_unk* obj);
extern "C" GameTask* lbl_8087F430;
extern "C" atn::Task* lbl_8087F9E0; // inferred: upgrade shop (ShopUpGrade)
extern "C" PlayerManager* lbl_8087F8A0; // inferred: the party

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
    void fn_8056B3D4(); // inferred: 0x8056B3D4
    unsigned char m000[0x494];
};

// inferred: object of GameTask + 0x5590 (destructor 0x8056C398 in game/ReplayBuffer.cpp; other members
// in game/GameSystem_unk.cpp)
struct GameTaskData_unk {
    GameTaskData_unk(); // 0x8056BF8C
    ~GameTaskData_unk();
    unsigned char m00000[0x5BDBC]; // inferred: size from the new of GameTask::fn_8036CA60
};

// inferred: pause task of the field (__vt__13GamePauseTask 0x8078A394, size 0x78); the GameTask
// constructor inlines its creation. Member names invented.
class GamePauseTask : public atn::Task {
public:
    GamePauseTask(atn::Task* parent, GameTask* game, GameCamera* camera)
        : atn::Task(parent), mGame(game), mCamera(camera), mLeader(0), m54(0), m58(0) {}
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
    boost::function<void(int)> m5C; // inferred: called with the frames left (vf10)
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
namespace atn {
// inferred: cached composed texture (atn/CompositeIntensityTintMulti.cpp)
struct CompositeCacheEntry_unk {
    void fn_800D8448(); // inferred: add a reference
    void fn_800D8458(); // inferred: release
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

// inferred: per-difficulty factors (as in game/CharaStatus.cpp)
struct StatusRate_unk {
    float m00;
    int m04;
    float m08;
    int m0C;
    float m10;
    int m14;
    float m18;
    int m1C;
};
struct MpSettings_unk {
    unsigned char m000[0x28];
    int m028; // inferred: map file lists are used (GameTask::vf10)
    unsigned char m02C[0x7C - 0x2C];
    int m07C;
    unsigned char m080[0x90 - 0x80];
    int m090; // inferred: battle flag checked when the replay ends (GameTask::fn_803737C0)
    unsigned char m094[0xD0 - 0x94];
    int mD0; // inferred: 1 while the battle is replayed (GameTask::fn_803727DC)
    int mD4; // inferred: index into mD8 (difficulty?)
    StatusRate_unk mD8[4];
    unsigned char m158[0x184 - 0x158];
    int m184; // inferred: debug text of the field (GameTask::vf14)
    unsigned char m188[0x194 - 0x188];
    int m194;
    unsigned char m198[0x320 - 0x198];
    int m320; // inferred: halves the talk range (GameTask fn_8036DDD4)
    unsigned char m324[0x48C - 0x324];
    Input_unk mInput; // 0x48C
    unsigned char m4DC[0x5A0 - 0x4DC];
    int m5A0;
};
extern "C" GameFlow_unk* lbl_8087F420;
extern "C" Collision_unk* lbl_8087EE98;
extern "C" void* lbl_8087F610; // inferred: multiplayer session (null in single player)
extern "C" atn::EffectManager* lbl_8087F3C0;
extern "C" atn::SoundManager* lbl_8087EFE8;
extern "C" void* lbl_8087EE90; // inferred: sound engine
extern "C" BattleTask* lbl_8087F048; // inferred: the battle task
extern "C" int fn_80219558(int id);
// inferred: popup messages of the field (lbl_8087F8A8; code in game/PopupTask.cpp)
class PopupTask : public atn::Task {
public:
    void fn_8054E51C();
    void fn_8054AF08();
};
extern "C" PopupTask* lbl_8087F8A8;
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

extern "C" void fn_80370094(GameTask* task, int chapter);
extern "C" int fn_8037587C(GameTask* task, int type, int id, int sub);
extern "C" int fn_803754F0(GameTask* task);
extern "C" void fn_80376324(GameTask* task, int id);
extern "C" void fn_801092C8(void* battle, int kind, CharaTask* chara, int bit, int arg);
extern "C" TutorialData_unk* fn_8021F09C(int id); // inferred: row of the tutorial table
extern "C" void fn_80570A14(GameTaskData_unk* obj, int value);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound
extern "C" void fn_803D83A8(GameUI* ui);
extern "C" void fn_803DAB4C(GameUI* ui, int icons, int tacView);
extern "C" void fn_8056F2F8(GameTaskData_unk* obj, int value);
extern "C" TacView* lbl_8087F9F8; // inferred: the tactical view
extern "C" atn::Task* lbl_8087FA20;
extern "C" int fn_8006AA20(const char* path); // inferred: the file exists
extern "C" void fn_805AA734(TacView* tac);
extern "C" void fn_803743AC(GameTask* task);
extern "C" void fn_800F3D28(BattleTask* battle);
extern "C" void fn_800F3170(BattleTask* battle);
extern "C" int fn_800F3490(BattleTask* battle);
extern "C" void fn_800EFA18(int type, int map);
extern "C" void fn_8056C3D8(GameTaskData_unk* obj);
extern "C" void fn_80390A88(GameCamera* camera);
extern "C" void fn_8038F52C(GameCamera* camera);
extern "C" void fn_8037F744(GameCamera* camera);
extern "C" int fn_800A555C(void* obj, int a, int b);
extern "C" void* lbl_8087EF70;
extern "C" void fn_804A5E40(MenuCommonTask* menu, int kind, atn::Vector3* pos, int arg);
extern "C" atn::Vector3 lbl_807C7030;
extern "C" void fn_800D2494(atn::Task* task); // inferred: runs one update of the task
extern "C" atn::Task* lbl_8087F440;
extern "C" RenderSetting_unk* lbl_8087EFA8; // render settings (atn::RenderConfig)
extern "C" double floor(double x);
extern "C" double fmod(double x, double y);
extern "C" float PSVECMag(const atn::Vector3* v);
// inferred: object at 0x258 of lbl_8087FA20 (switch at 0x160C toggled with a button)
struct TacSwitch_unk {
    unsigned char m0000[0x160C];
    int m160C;
};
struct TaskWithTac_unk {
    unsigned char m000[0x258];
    TacSwitch_unk* m258;
};
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FECE0(atn::D2A_Layout_unk* layout, unsigned int hash, float value);
extern "C" float fn_8006EF48(atn::Font_unk* font, const wchar_t* text, float size, float f2, int a, int b);
extern "C" void fn_8006F2F0(atn::Font_unk* font, wchar_t* dst, const char* src, int size); // inferred: to wide text
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" int fn_80370174(GameTask* task, unsigned int index);             // inferred: GetGlobalFlag
extern "C" int fn_80370A78(GameTask* task, unsigned int index);             // inferred: GetLocalFlag
extern "C" void fn_80370AE4(GameTask* task, unsigned int index, int value); // inferred: SetLocalFlag
extern "C" int lbl_8087F438; // inferred: the loading time is being measured
extern "C" HeapInfo_unk* lbl_8087F534;
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

extern "C" __declspec(noinline) int fn_8036562C(int type, int map, int sub, int chapter) {
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
      m56A4(0), m56D0(0), m56D4(0), m56D8(0), m56E4(0), m56E8(-1), m56EC(0), m56F0(0),
      m5744(0), m5748(0), m574C(0), m5750(0), m5754(0), m5758(0), m575C(0), m5760(0),
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
    fn_80370094(this, mOption.mPos);
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

GameMessage::~GameMessage() {}

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
extern const int sDungeonIds_unk[];

static inline int IsDungeonListed_unk(const char* id) {
    int dungeon = GetDungeonId_unk(id);
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
extern const DungeonFlag_unk sDungeonFlags_unk[];

static inline const DungeonFlag_unk* FindDungeonFlag_unk(const char* id) {
    int dungeon = GetDungeonId_unk(id);
    for (int i = 0; sDungeonFlags_unk[i].mDungeon > 0; i++) {
        if (dungeon == sDungeonFlags_unk[i].mDungeon) {
            return &sDungeonFlags_unk[i];
        }
    }
    return 0;
}

// inferred: the dungeon of a map is already cleared (inlined twice in vf0C and in fn_803754F0)
inline int GameTask::IsDungeonCleared_unk(int type, int id, int sub) {
    int cleared = 0;
    MapId_unk* map = fn_8021771C(type, id, sub);
    if (map != 0 && map->mType == 2 && IsDungeonListed_unk((map->mId))) {
        const DungeonFlag_unk* flag = FindDungeonFlag_unk((map->mId));
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
        if (fn_800D3FA4() && !Inventory::instance_->fn_8044386C() && GameSaveBanner::sInstance == 0) {
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
            int value = sGameTaskStatic_unk.m30;
            m5694 = value;
            m5698 = value;
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
            int cleared = IsDungeonCleared_unk(mOption.mType, mOption.mMap, mOption.mSub);
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
        }
    } else if (m10DC == 1) {
        if (lbl_8087F418 == 0 || !lbl_8087F418->fn_80360A2C()) {
            m10DC++;
        }
    } else if (m10DC == 2) {
        if (lbl_8087F8A0->mHead->IsFlag_unk(2) && !NpcManager::sInstance->fn_80547B54()) {
            m5690.fn_80470528();
            m10DC++;
            m10D8 = GameScene::fn_803BEBAC(this, mOption.mType, mOption.mMap, mOption.mSub);
            if (mOption.m1C != 0) {
                m10D8->m9B0 = (GameSceneSave_unk*)mOption.m1C;
            }
            m10D8->fn_800D246C(1);
        }
    } else {
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
    if (ready) {
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
    if (mOption.m1C == 0 && m10D0 >= 100000 && 403000 > m10D0) {
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
    if (IsDungeonCleared_unk(mOption.mType, mOption.mMap, mOption.mSub)) {
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
    }
    return 0;
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

// inferred: comparison of the cache entries (the instantiation 0x8036B44C is emitted here)
struct LessCache_unk {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};
// inferred: comparison of the sorted resource hashes (atn/Model.cpp instantiates Sort_unk<unsigned int,
// LessU_unk> at 0x8009246C)
struct LessU_unk {
    bool operator()(unsigned int a, unsigned int b) const { return a < b; }
};
template <class T, class Compare>
void Sort_unk(T* first, T* last, Compare comp);

// inferred: removes the repeated values of a sorted vector (std::unique and erase, inlined)
static inline void Unique_unk(atn::SimpleVector<unsigned int>& v) {
    unsigned int* first = v.mData;
    unsigned int* last = v.mData + v.mSize;
    unsigned int* it = first;
    if (it != last) {
        unsigned int* next = it + 1;
        for (; next != last; it = next, next++) {
            if (*it == *next) {
                goto found;
            }
        }
    }
    it = last;
found:
    if (it != last) {
        for (unsigned int* p = it + 2; p != last; p++) {
            if (*it != *p) {
                *++it = *p;
            }
        }
        it++;
    }
    if (it != v.mData + v.mSize) {
        int index = it - v.mData;
        int count = (v.mData + v.mSize) - it;
        for (unsigned int i = index; i < v.mSize - count; i++) {
            v.mData[i] = v.mData[i + count];
        }
        v.mSize = v.mSize - count;
    }
}

// inferred: the map change keeps the characters of the field (same map kind)
static inline int IsSameField_unk(const GameTaskTable_unk& table) {
    if (table.mId == 0x44 && table.mSub == 1) {
        return 1;
    }
    if (table.mId == 7 && table.mSub == 1) {
        return 1;
    }
    if (table.mType != table.mPrevType) {
        return 0;
    }
    if (table.mId == 15 || table.mId == 55) {
        return 0;
    }
    return 1;
}

// inferred: sets the global flag 281 to the level of the dungeon of the map (from the leader)
inline void GameTask::SetDungeonLevel_unk(int type, int id, int sub) {
    PlayerTask* leader = lbl_8087F8A0->mHead;
    if (leader != 0) {
        MapId_unk* map = fn_8021771C(type, id, sub);
        int level = leader->mStatus.mParamB.m78;
        ChapterEntry_unk* chapter = fn_802085E0(type, id);
        if (chapter != 0 && map != 0) {
            mGlobalFlags[281] = map->mC0 + (int)(level * map->mC4) - chapter->m54;
        }
    }
}

// inferred: places the second party member next to `pos` (on the nearest move point)
static inline void PlaceSecondMember_unk(GameScene* scene, PlayerTask* second, const atn::Vector3& pos, float rot) {
    atn::Vector3 offset(-5.0f, 0.0f, -5.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rot);
    PSMTXMultVec(m, &offset, &offset);
    atn::Vector3 target(pos.x + offset.x, pos.y + offset.y, pos.z + offset.z);
    int index = scene->fn_803C1560(&target, 0x80000000, 0, 0, 1, -1.0f);
    if (index != 0) {
        gs::MovePoint* point = &scene->m98.mData[index - 1];
        second->mPos = point->mPos;
        second->mRot = atn::Vector3(0.0f, rot, 0.0f);
        second->fn_8016EB48(0, 0, 0);
    }
}

// inferred: warp point `id` of the scene
static inline gs::WarpPoint* FindWarpPoint_unk(GameScene* scene, int id) {
    for (unsigned int i = 0; i < scene->m78.size(); i++) {
        if (id == scene->m78.mData[i].mId) {
            return &scene->m78.mData[i];
        }
    }
    return 0;
}

// inferred: milliseconds since the start (OSTicksToMilliseconds(OSGetTime()))
static inline long long GetTimeMs_unk() {
    return OSGetTime() / ((*(unsigned int*)0x800000F8 / 4) / 1000);
}

// NONMATCHING: written for the order of the string pool (the map change states come first); not compared yet
void GameTask::vf10() {
    m5744++;
    m5744 = m5744 < 107999970 ? m5744 : 107999970;
    if (m5748 != 0) {
        m5748--;
    }
    mGlobalFlags[3280] = m5744 / 30 / 60;
    if (m550C.mState != 0) {
        switch (m550C.mState) {
        case 1: {
            if (m5624->mActive == 0) {
                m5624->mActive = 1;
            }
            lbl_8087F534->mTimer4.Start_unk();
            char name[0x100];
            char path[0x100];
            MapTask::GetMapNameFromId(name, m550C.mType, m550C.mId, m550C.mSub);
            sprintf(path, "flist/%s.flist", name);
            if (IsSameField_unk(m550C) && m550C.m30 == 0 && m5764 < 20 && lbl_8087F0A8->m028 != 0 &&
                fn_8006AA20(path)) {
                m5764++;
                m550C.mFile.vf0C(path);
                m550C.mState = 2;
                MapId_unk* map = fn_8021771C(m550C.mType, m550C.mId, m550C.mSub);
                if (map != 0 && map->mMembers != 0) {
                    for (int i = 0; map->mMembers[i] >= 0; i++) {
                        int chara = map->mMembers[i];
                        if (chara >= 199900 && chara <= 199999) {
                            chara = fn_80219544(Inventory::instance_->mOrder[chara % 100]);
                        }
                        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                            if (chara == p->m50) {
                                p->fn_8017AD44(&m550C.m38, &m550C.m44);
                            }
                        }
                        for (NpcTask* p = NpcManager::sInstance->mHead; p != 0; p = p->mNext) {
                            if (chara == p->m50) {
                                p->fn_8017AD44(&m550C.m38, &m550C.m44);
                            }
                        }
                        for (EnemyTask* p = EnemyManager::sInstance->mHead; p != 0; p = p->mNext) {
                            if (chara == p->m50) {
                                p->fn_8017AD44(&m550C.m38, &m550C.m44);
                            }
                        }
                    }
                    Sort_unk(m550C.m38.mData, m550C.m38.mData + m550C.m38.mSize, LessU_unk());
                    Unique_unk(m550C.m38);
                    LessCache_unk less;
                    Sort_unk<unsigned int, LessCache_unk&>(m550C.m44.mData, m550C.m44.mData + m550C.m44.mSize, less);
                    Unique_unk(m550C.m44);
                    if (m550C.m38.mSize != 0) {
                        lbl_8087F518->fn_8046F8B8(m550C.m38.mData, m550C.m38.mSize);
                    }
                    for (unsigned int i = 0; i < m550C.m44.mSize; i++) {
                        ((atn::CompositeCacheEntry_unk*)m550C.m44.mData[i])->fn_800D8448();
                    }
                }
            } else {
                m5764 = 0;
                m550C.mState = 3;
            }
            if (GameBgm::sInstance != 0 && m10D8->m64 != 0) {
                GameBgm::sInstance->fn_8037D684(m550C.mType, m550C.mId);
            }
            if (lbl_8087F418 != 0) {
                lbl_8087F418->fn_8035E858();
            }
            break;
        }
        case 2:
            if (!m550C.mFile.fn_80473F50()) {
                void* data = m550C.mFile.fn_80470580();
                unsigned int size = m550C.mFile.fn_8047059C();
                if (data != 0) {
                    lbl_8087F518->fn_8046F8B8((unsigned int*)data, size / 4);
                }
                m550C.mState = 3;
            }
            break;
        case 3: {
            if (GameBgm::sInstance != 0) {
                int state = GameBgm::sInstance->mState;
                int playing = 0;
                if (state >= 3 && state <= 5) {
                    playing = 1;
                }
                if (playing && !GameBgm::sInstance->fn_8037D34C()) {
                    break;
                }
            }
            if (m54E4 == 14) {
                fn_805AA734(lbl_8087F9F8);
                fn_803743AC(lbl_8087F430);
                mCamera.m8E4 = 0;
                mCamera.fn_80389838(0);
            }
            ((atn::Task*)lbl_8087F048)->fn_800D246C(1);
            lbl_8087EE68->FreezeTask();
            lbl_8087EE68->fn_80017060();
            MapManager::instance_->FreezeTask();
            lbl_8087F8A0->fn_8054A09C(lbl_8087F8A0->m54, 0);
            lbl_8087F8A0->mHead->fn_800EBA5C();
            fn_80218268(m550C.mType, m550C.mId, m550C.mSub);
            if (m550C.mType != m550C.mPrevType) {
                fn_8006D0C8(lbl_8087EEC8, m550C.mType == 1);
            }
            lbl_8087F3C0->fn_8023A5A8(1);
            lbl_8087F3C0->fn_800D246C(1);
            lbl_8087F8A0->fn_800D246C(1);
            EnemyManager::sInstance->fn_800D246C(1);
            NpcManager::sInstance->fn_800D246C(1);
            lbl_8087F9E8->fn_8059C32C(1);
            lbl_8087F9E8->FreezeTask();
            fn_800F3D28(lbl_8087F048);
            BattleResult::sInstance->fn_800F13B0();
            lbl_8087F8A8->fn_8054E51C();
            LogMessageTask::sInstance->mEntries.mSize = 0;
            if (m550C.m30 != 0) {
                lbl_8087F8A0->DeleteAll_unk();
                NpcManager::sInstance->fn_80547A20();
            } else {
                lbl_8087F8A0->fn_8054A398();
                if (!fn_8036562C(m550C.mType, m550C.mId, m550C.mSub, m10D0)) {
                    NpcManager::sInstance->fn_80547A20();
                }
            }
            EnemyManager::sInstance->DeleteAll_unk();
            NpcManager::sInstance->fn_80547A84();
            m10D8->fn_803CCD98();
            if (lbl_8087F540 != 0) {
                lbl_8087F540->fn_8047E528();
                lbl_8087F540->fn_80483714(0, 0);
                lbl_8087F540->fn_804805B4();
            }
            BattleResult::sInstance->fn_800F1378();
            mCamera.fn_803933A4();
            if (lbl_8087F098 != 0) {
                lbl_8087F098->fn_80183774(0);
                lbl_8087F098->fn_80182890();
            }
            m5660 = 0;
            fn_800D2338(m10D8);
            m10D8 = 0;
            m5594 = 0;
            m55A4 = 0;
            if (m5590 != 0) {
                delete m5590;
                m5590 = 0;
                lbl_8087F3C0->mSystem.mListener = 0;
            }
            m56D0 = 0;
            m56D4 = 0;
            m56D8 = 0;
            if (fn_8036555C(m550C.mType, m550C.mId, m550C.mSub)) {
                fn_800F3170(lbl_8087F048);
                lbl_8087F8A8->fn_8054AF08();
                lbl_8087F490->fn_803D11B4();
            }
            lbl_8087F490->fn_803E454C();
            lbl_8087F490->fn_800D246C(1);
            if (CrossbowMenuTask::sInstance != 0) {
                CrossbowMenuTask::sInstance->fn_801E97DC();
            }
            lbl_8087EFA8->m3A4 = 1.0f;
            lbl_8087EFA8->m240.mEnabled = 0;
            if (m550C.mType == 2 && m550C.mId == 9) {
                m10D4 = fn_80208634(m550C.mType, m550C.mId, m550C.mSub);
            } else {
                m10D4 = fn_802085E0(m550C.mType, m550C.mId);
            }
            m550C.mState = 4;
            if (m54E4 == 6) {
                m54E4 = 0;
            }
            break;
        }
        case 4:
            if (lbl_8087EE90 != 0) {
                int before = fn_803CE160(m550C.mPrevType, m550C.mPrevId, m550C.mPrevSub);
                int after = fn_803CE160(m550C.mType, m550C.mId, m550C.mSub);
                if (before != after) {
                    if (after != 0) {
                        void* sound = lbl_8087EE90;
                        fn_80047B54(sound, fn_803CE154());
                    } else {
                        void* sound = lbl_8087EE90;
                        fn_80047B54(sound, fn_803CE148());
                    }
                }
            }
            if (m550C.m30 != 0) {
                ((GameSaveData_unk*)m550C.m30)->fn_803BE590();
            }
            lbl_8087F3C0->fn_8023A678(m550C.mType == 1 ? 0x80 : 0x120);
            if (fn_8036562C(m550C.mType, m550C.mId, m550C.mSub, m10D0) &&
                (m550C.m30 != 0 || !fn_8036562C(m550C.mPrevType, m550C.mPrevId, m550C.mPrevSub, m10D0))) {
                m5690.fn_80470364("preload/change_dg_cache.pk", 0, 0, 1);
            } else if (m550C.mType != 1 && m550C.mPrevType == 1) {
                m5690.fn_80470364("preload/change_dg.pk", 0, 0, 1);
            } else if (!fn_8036555C(m550C.mType, m550C.mId, m550C.mSub) &&
                       fn_8036555C(m550C.mPrevType, m550C.mPrevId, m550C.mPrevSub)) {
                m5690.fn_80470364("preload/change_dg.pk", 0, 0, 1);
            }
            if (lbl_8087F8A0->mHead != 0) {
                int type = m550C.mType;
                if (m550C.mId == 0x44 && m550C.mSub == 1) {
                    type = 1;
                }
                lbl_8087F8A0->mHead->fn_800EBD00(type, m550C.mPrevType);
            } else {
                lbl_8087F8A0->fn_80549F14(m550C.mType);
            }
            if (fn_8036562C(m550C.mType, m550C.mId, m550C.mSub, m10D0)) {
                NpcManager::sInstance->fn_80547980();
            }
            if (lbl_8087F418 != 0) {
                lbl_8087F418->fn_8036097C(m550C.mType, m550C.mId, m550C.mSub, m550C.m14, m550C.m30);
            }
            m550C.mState = 5;
            break;
        case 5:
            if (m5624->mFrame >= m5624->mDelay + m5624->mDuration ||
                (m54F0 == 1 && m5620->mFrame >= m5620->mDelay + m5620->mDuration)) {
                if (lbl_8087F418 == 0 || !lbl_8087F418->fn_80360A2C()) {
                    if (lbl_8087F8A0->mHead->IsFlag_unk(2) && !lbl_8087F8A0->mHead->fn_800EBE84() &&
                        !NpcManager::sInstance->fn_80547B54()) {
                        if (IsDungeonCleared_unk(m550C.mType, m550C.mId, m550C.mSub)) {
                            SetDungeonLevel_unk(m550C.mType, m550C.mId, m550C.mSub);
                        }
                        m550C.mState = 6;
                    }
                }
                fn_8046ECDC(lbl_8087F518, 1);
            }
            break;
        case 6:
            ((PostFxMgrView_unk*)lbl_8087EF8C)
                ->mMotionBlur.fn_800AFEBC(IsSpecialField_unk(m550C.mType, m550C.mId, m550C.mSub) ? 4 : 2);
            fn_800EFA18(m550C.mType, m550C.mId);
            if (!fn_8036555C(m550C.mType, m550C.mId, m550C.mSub)) {
                fn_800F2C9C(lbl_8087F048);
                fn_8054A9A8(lbl_8087F8A8);
                fn_803D0B20(lbl_8087F490);
            }
            m550C.mState = 7;
            fn_8046ECDC(lbl_8087F518, 1);
            break;
        case 7:
            fn_8046ECDC(lbl_8087F518, 1);
            if (!fn_800F3490(lbl_8087F048) && !lbl_8087F490->fn_803D1574() && lbl_8087F8A8->fn_800D3FA4()) {
                m5690.fn_80470528();
                m550C.mState = 8;
            }
            break;
        case 8: {
            fn_8046ECDC(lbl_8087F518, 1);
            if (m550C.m30 != 0) {
                int cleared = IsDungeonCleared_unk(mOption.mType, mOption.mMap, mOption.mSub);
                m5694 = cleared;
                m5698 = cleared;
            }
            m550C.mScene = GameScene::fn_803BEBAC(this, m550C.mType, m550C.mId, m550C.mSub);
            if (m550C.m30 != 0) {
                m550C.mScene->m9B0 = (GameSceneSave_unk*)m550C.m30;
            }
            m550C.mScene->fn_800D246C(1);
            m550C.mState = 9;
            m10D8 = m550C.mScene;
            break;
        }
        case 9: {
            if (fn_8036FD4C()) {
                break;
            }
            if (lbl_8087F8A0->mHead->fn_800EBE84()) {
                break;
            }
            ((atn::Task*)lbl_8087F048)->fn_800D246C(0);
            lbl_8087EE68->MeltTask();
            MapManager::instance_->MeltTask();
            lbl_8087F9E8->MeltTask();
            lbl_8087F3C0->fn_800D246C(0);
            lbl_8087F8A0->fn_800D246C(0);
            EnemyManager::sInstance->fn_800D246C(0);
            NpcManager::sInstance->fn_800D246C(0);
            m10D8 = m550C.mScene;
            if (m550C.m30 != 0) {
                ((GameSaveData_unk*)m550C.m30)->fn_803BE670();
            } else {
                memset(mLocalFlags, 0, sizeof(mLocalFlags));
                memset(&mGlobalFlags[2200], 0, 0x100 * sizeof(int));
                memset(&mGlobalFlags[2500], 0, 0x100 * sizeof(int));
            }
            fn_8036CA60(m550C.m14);
            if (m550C.m30 != 0) {
                ((GameSaveData_unk*)m550C.m30)->fn_803BE6B8();
                mCamera.fn_8037EF30(1, 1, -1.0f);
                m5748 = 5;
            }
            fn_80367A40(0);
            fn_803D164C(lbl_8087F490);
            fn_803D2194(lbl_8087F490);
            lbl_8087F490->fn_803E32EC();
            lbl_8087F490->fn_800D246C(0);
            if (m574C > 0) {
                mGlobalFlags[212] = 1;
                mGlobalFlags[282] = m574C;
            }
            m574C = 0;
            Inventory::instance_->fn_8044D028();
            if (m550C.m30 == 0 && lbl_8087FA20 != 0 && m10D0 >= 600000) {
                PlayerTask* second = lbl_8087F8A0->fn_8054A33C(1);
                PlayerTask* leader = lbl_8087F8A0->mHead;
                atn::Vector3 rot = leader->mRot;
                atn::Vector3 pos = leader->mPos;
                if (second != 0) {
                    atn::Vector3 d(second->mPos.x - pos.x, second->mPos.y - pos.y, second->mPos.z - pos.z);
                    if (PSVECMag(&d) > 20.0f) {
                        PlaceSecondMember_unk(m10D8, second, pos, rot.y);
                    }
                }
            }
            if (IsDungeonCleared_unk(m550C.mType, m550C.mId, m550C.mSub)) {
                SetDungeonLevel_unk(m550C.mType, m550C.mId, m550C.mSub);
            }
            mCamera.fn_80389838(mCamera.m804);
            mCamera.m8E8 = 0;
            lbl_8087F8A0->mHead->m54C &= ~0x10;
            lbl_8087F048->m367B0 = m5618;
            m550C.mScene = 0;
            m550C.mState = 0;
            m550C.m30 = 0;
            m550C.m14 = 0;
            m550C.m34 = 0;
            if (lbl_8087F460 != 0) {
                delete lbl_8087F460;
                lbl_8087F460 = 0;
            }
            m5624->mFrame = 0;
            m5624->mWait = 0;
            m5624->mDelay = 2;
            m5624->mColor0 = 0xFF000000;
            m5624->mColor1 = 0;
            if (m54F0 == 1) {
                fn_80370BD0(30, 2, 0.00001f);
            }
            m5760 = 4;
            m5638 = 1;
            fn_80370320(12, m5670, 0);
            m5630 = 1;
            if (m5668 != 0 && lbl_8087FA20 != 0 && m10D8 != 0 && m10D0 >= 600000) {
                PlayerTask* second = lbl_8087F8A0->fn_8054A33C(1);
                if (second != 0) {
                    gs::WarpPoint* warp = FindWarpPoint_unk(m10D8, 9020);
                    if (warp != 0) {
                        second->mPos = warp->mPos;
                        second->mRot = atn::Vector3(0.0f, warp->m14, 0.0f);
                        second->fn_8016EB48(0, 0, 0);
                    }
                }
            }
            m5668 = 0;
            if (fn_803754F0(this) && fn_80370174(this, 0x1C0) == 0) {
                fn_80370320(0x1C0, 1, 0);
            }
            m54EC = 0;
            if (m550C.m38.mSize != 0) {
                lbl_8087F518->fn_8046F96C(m550C.m38.mData, m550C.m38.mSize);
            }
            for (unsigned int i = 0; i < m550C.m44.mSize; i++) {
                ((atn::CompositeCacheEntry_unk*)m550C.m44.mData[i])->fn_800D8458();
            }
            m550C.m38.clear();
            m550C.m44.clear();
            void* data = m550C.mFile.fn_80470580();
            unsigned int size = m550C.mFile.fn_8047059C();
            if (data != 0) {
                lbl_8087F518->fn_8046F96C((unsigned int*)data, size / 4);
                m550C.mFile.fn_80473F88();
            }
            if (GameBgm::sInstance != 0) {
                int left = GameBgm::sInstance->m84 != 0 ? GameBgm::sInstance->mNext.mId : GameBgm::sInstance->mReq.mId;
                if (left > 0 && GameBgm::sInstance->fn_8037D34C()) {
                    GameBgm::sInstance->fn_8037D3E8(0, 0x10, 0x2D);
                }
            }
            HomeButtonMenu::sInstance->m88 = 1;
            lbl_8087F534->mTimer4.Stop_unk();
            break;
        }
        }
        return;
    }
    PlayerTask* leader = lbl_8087F8A0->mHead;
    if (m54E4 == 0 && (leader->HasC48_unk() || leader->mFlag6)) {
        atn::RenderManager::instance_->m54964 = 1;
    } else {
        atn::RenderManager::instance_->m54964 = 0;
    }
    if (m5674 == 0 && !lbl_8087F0A8->mInput.fn_801240B4(9)) {
        m5674 = 1;
    }
    if (m566C != 0) {
        m566C = 0;
        fn_80370CD0();
        return;
    }
    if (lbl_8087F0A8->m194 != 0) {
        if (m54E4 == 8) {
            m5744--;
        }
        if (m5744 >= 21600) {
            if (m54E4 == 6) {
                if (m575C != 0) {
                    fn_800D2338(m575C);
                    m575C = 0;
                }
            } else if (m575C == 0) {
                if (m54E4 != 8 || lbl_8087F540->m70 == 0) {
                    m575C = atn::FadeColor::Create(this, 60, 0, 0xFF000000);
                    m575C->mZ = -10000000.0f;
                    m575C->mWait = 100000;
                    m575C->mActive = 1;
                    if (lbl_8087EFE8 != 0) {
                        lbl_8087EFE8->fn_800D0240(0, 0.0f, 60);
                        lbl_8087EFE8->fn_800D0240(1, 0.0f, 60);
                        lbl_8087EFE8->fn_800D0240(2, 0.0f, 60);
                    }
                }
            } else if (m575C->mFrame >= m575C->mDuration) {
                int type = m10D8->m64->mType;
                if (type != 3 && type != 4) {
                    m5624->m78 = 0x12;
                    m5624->Capture();
                    m566C = 1;
                }
                if (m56EC != 0) {
                    m56EC->FreezeTask();
                }
                if (m56F0 != 0) {
                    m56F0->FreezeTask();
                }
                if (MenuCommonTask::sInstance != 0) {
                    MenuCommonTask::sInstance->fn_804A2FBC();
                }
                lbl_8087F3C0->fn_800D246C(0);
                lbl_8087F540->FreezeTask();
                if (m567C != 0) {
                    m567C->FreezeTask();
                }
                if (lbl_8087EFE8 != 0) {
                    lbl_8087EFE8->fn_800CF45C(0, 0);
                    lbl_8087EFE8->fn_800D0240(0, 1.0f, 0);
                    lbl_8087EFE8->fn_800D0240(1, 1.0f, 0);
                    lbl_8087EFE8->fn_800D0240(2, 1.0f, 0);
                }
                if (lbl_8087F418 != 0) {
                    lbl_8087F418->fn_803606CC(0, 1, 0);
                    lbl_8087F418->fn_80360780(0, 1, 0);
                }
            }
        }
    }
    int capture = m5684;
    m5760--;
    if (capture != 0) {
        if (fn_8036E170()) {
            fn_8036E494();
        }
        m5684 = 0;
    }
    switch (m54E4) {
    case 0: {
        if (lbl_8087F490->mD8C == 1 && lbl_8087F0A8->mInput.fn_801231D0(0)) {
            if (m56F4.mCount != 0) {
                fn_80376324(this, m56F4.Front_unk());
                m56F4.Pop_unk();
            }
            lbl_8087F490->fn_803E4478();
            break;
        }
        if (lbl_8087F490->mD8C != 1 && m56F4.mCount != 0) {
            m56F4.mCount = 0;
        }
        lbl_8087F8A0->mHead->fn_800E3F5C();
        if (m56A4 > 0) {
            if (m569C > 0) {
                if (--m569C <= 0) {
                    MotionBlurParam_unk blur = lbl_8087EFA8->m240;
                    m56A8 = blur;
                    blur.mEnabled = 1;
                    lbl_8087EFA8->m240 = blur;
                    m56CC = lbl_8087EFA8->m3A4;
                    lbl_8087EFA8->m3A4 = 0.2f;
                }
            } else if (--m56A4 <= 0) {
                lbl_8087EFA8->m240 = m56A8;
                lbl_8087EFA8->m3A4 = m56CC;
            }
        }
        int value = mGlobalFlags[1];
        if (value >= 10) {
            mGlobalFlags[2] = value / 10;
        } else if (value <= -10) {
            mGlobalFlags[2] = -1;
        } else {
            mGlobalFlags[2] = 0;
        }
        if (m5590 != 0) {
            fn_8056C3D8(m5590);
        }
        if (m5620->mActive == 0) {
            m54F0 = 0;
        }
        if (mCamera.m7FC == 1) {
            fn_8036CFA4();
        } else {
            lbl_8087F490->fn_803E32EC();
            PlayerTask* player = lbl_8087F8A0->mHead;
            if (player->m648 != 0) {
                if (lbl_8087F0A8->mInput.fn_801231D0(0xB) && m5590 != 0 && fn_80570A4C(m5590) && m5594 != 0 &&
                    m55A4 > 0) {
                    fn_803731E8();
                } else {
                    if (mCamera.m7FC == 6 && mCamera.m7FC == mCamera.m800 && lbl_8087F9F8->mA70 == 0) {
                        fn_80390A88(&mCamera);
                    }
                    fn_8036CC90();
                }
            } else {
                if (player->m55C == 5) {
                    fn_8038F52C(&mCamera);
                    fn_8037F744(&mCamera);
                } else {
                    fn_8036CC90();
                }
                if (lbl_8087FA20 != 0 && lbl_8087F0A8->mInput.fn_801231D0(0x2D) && m56F0 == 0) {
                    TacSwitch_unk* tac = ((TaskWithTac_unk*)lbl_8087FA20)->m258;
                    tac->m160C = tac->m160C == 0;
                }
            }
        }
        mFinish.fn_803792F0();
        break;
    }
    case 1:
        if (m5620->mActive == 0) {
            m54F0 = 0;
        }
        break;
    case 5:
        if (m5750->mState == 1) {
            int value = m5750->mDecided;
            fn_8036EE0C();
            if (m5754 == 1) {
                fn_80370320(m5758, value, 0);
            } else {
                fn_80370AE4(this, m5758, value);
            }
            if (m5750 != 0) {
                fn_800D2338(m5750);
                m5750 = 0;
            }
        }
        break;
    case 6:
        if (lbl_8087F440 != 0 && !lbl_8087F440->IsFlag_unk(2) && m5624 != 0 && m5624->mActive != 0 &&
            m5624->mFrame <= 1) {
            m5624->mFrame = 0;
            m5624->mDelay = 2;
        }
        break;
    case 7:
        fn_80373AE8();
        if (mFinish.m394 != 0) {
            m10D8->m64->fn_804A0580(0, &lbl_8087F8A0->mHead->mPos);
        } else {
            m10D8->m64->fn_804A0580(1, &lbl_8087F8A0->mHead->mPos);
        }
        if (m5598 == 0 && m55AC == 1) {
            fn_8037F744(&mCamera);
            int replay = 0;
            if (lbl_8087F490 != 0) {
                GameUIElem1C_unk mode;
                mode.m00 = 6;
                mode.m04 = 13;
                lbl_8087F490->m764 = mode;
            }
            if (fn_800A555C(lbl_8087EF70, 0, 4)) {
                replay = 1;
            }
            if (replay) {
                fn_803737C0();
            }
            m55A4--;
        }
        break;
    case 14:
        if (mCamera.m7FC == mCamera.m800) {
            lbl_8087EFA8->m240.mEnabled = 0;
        }
        lbl_8087F3C0->fn_8023A614(5);
        lbl_8087F3C0->fn_8023A614(7);
        mFinish.fn_803792F0();
        m10D8->m64->fn_804A0580(0, &lbl_8087F8A0->mHead->mPos);
        if (lbl_8087F9F8->mA70 == 0) {
            fn_803743AC(this);
        }
        break;
    case 8: {
        if (!lbl_8087F540->GetCamera_unk()->fn_80116E7C()) {
            fn_8036CFA4();
        }
        SetConfig* config = SetConfig::sInstance;
        if (lbl_8087F540->m70 != 0 && lbl_8087F540->m70->m194 == 10) {
            m5650++;
        }
        if ((m5650 > 90 || config->mEventSkipType == 2) && lbl_8087F540->m70 != 0 &&
            !(lbl_8087F0A8->m194 != 0 && lbl_8087F540->m70->m190 == 0x19D)) {
            int kind = lbl_8087F540->m70->fn_80541210()->mSkip;
            if (kind == 1 || kind == 2) {
                if (m5654 > 0) {
                    m5654--;
                    atn::Vector3 pos = lbl_807C7030;
                    fn_804A5E40(MenuCommonTask::sInstance, 7, &pos, 0);
                    if (lbl_8087F0A8->mInput.fn_801231D0(0x28)) {
                        lbl_8087F540->fn_8047F994(0, 0);
                    }
                } else if (lbl_8087F0A8->mInput.fn_801231D0(0x27)) {
                    m5654 = 120;
                }
            } else if (kind == 3) {
                atn::Keyboard* keyboard = atn::Keyboard::instance_;
                if (m565C != 0 || lbl_8087F0A8->mInput.fn_801231D0(0x27)) {
                    lbl_8087F540->fn_8047F994(0, 0);
                    m565C = 0;
                } else if (lbl_8087F0A8->mInput.fn_80123C7C(0x27)) {
                    lbl_8087F540->fn_8047F994(1, 0);
                } else if (keyboard != 0 && keyboard->Press((atn::Keyboard::Port)0, 0xF1B1)) {
                    lbl_8087F540->fn_8047F91C();
                }
            } else {
                m565C = 0;
            }
        } else if (lbl_8087F0A8->mInput.fn_801231D0(0x27)) {
            m565C = 1;
        } else if (lbl_8087F0A8->mInput.fn_80123C7C(0x27)) {
            m565C = 0;
        }
        if (lbl_8087F540 == 0) {
            break;
        }
        if (lbl_8087F540->m2388 != 0 && lbl_8087EFA8->m414 == 0 && lbl_8087F540->m70 != 0 &&
            lbl_8087F540->m70->m194 == 10) {
            if (m5768 == 0) {
                m5768 = 1;
                long long start = GetTimeMs_unk();
                m5774 = (int)start;
                m5770 = (int)(start >> 32);
            }
            long long now = GetTimeMs_unk();
            int elapsed = (int)now - m5774;
            m5774 = (int)now;
            m5770 = (int)(now >> 32);
            m5658 += elapsed / 1000.0f - 1.0f / 30.0f;
            if (lbl_8087F540->m1E78 != 0) {
                lbl_8087F540->m1E88 += 1.0f;
            }
            if (m5658 >= 1.0f / 30.0f) {
                int frames = (int)floor(30.0f * m5658);
                mFlags |= 0x10;
                int flag = mFlags & 1;
                fn_800D246C(0);
                for (int i = frames; i > 0; i--) {
                    fn_800D2494(this);
                    lbl_8087F3C0->fn_8023AE34(0, 0, -1);
                    lbl_8087F3C0->fn_80239C14();
                }
                mFlags &= ~0x10;
                fn_800D246C(flag);
                m5658 -= 1.0f / 30.0f * frames;
                if (lbl_8087F540->m1E78 != 0) {
                    for (int i = 0; i < frames; i++) {
                        lbl_8087F540->m1E88 += 1.0f;
                    }
                }
            }
        }
        if (lbl_8087F540->m1E78 != 0) {
            lbl_8087F540->m1E8C += 1.0f;
        }
        break;
    }
    case 9:
        m10D8->m64->fn_804A0580(1, &lbl_8087F8A0->mHead->mPos);
        mFinish.fn_803792F0();
        if (mFinish.m00 == 0) {
            fn_803727DC();
        }
        break;
    case 10:
    case 11:
        m10D8->m64->fn_804A0580(1, &lbl_8087F8A0->mHead->mPos);
        mFinish.fn_803792F0();
        if (mFinish.m358 > 150 && m54E4 == 10) {
            m54E4 = 11;
            MotionBlurParam_unk blur = lbl_8087EFA8->m240;
            blur.mEnabled = 0;
            lbl_8087EFA8->m240 = blur;
        }
        break;
    case 12:
        fn_8037F744(&mCamera);
        if (m5620->mActive == 0) {
            m54F0 = 0;
        }
        break;
    case 13: {
        TutorialTask* tutorial = m56EC;
        if (tutorial->IsFlag_unk(2) && tutorial->mState == 0) {
            tutorial->fn_805BA354(&lbl_8087F430->mCamera.m1F4);
            if (MenuCommonTask::sInstance != 0) {
                MenuCommonTask::sInstance->fn_804A2F98();
            }
        } else if (tutorial->mState == 4) {
            if (MenuCommonTask::sInstance != 0) {
                MenuCommonTask::sInstance->fn_804A2FBC();
            }
            fn_80376568();
        }
        break;
    }
    }
    if (lbl_8087F0A8->mInput.fn_801231D0(0x10) && fn_8036E170()) {
        m5624->m78 = 0x12;
        m5624->Capture();
        m5684 = 1;
        m5688 = -1;
        m568C = -1;
    }
    if (lbl_8087EFE8 != 0 && m54E4 == 0) {
        atn::Vector3 pos = lbl_8087F8A0->mHead->mPos;
        pos.y += 1.0f;
        atn::Vector3 rot = mCamera.mRot;
        float angle = fmod(3.1415927f + rot.y, 6.2831854820251465);
        if (angle > 3.1415927f) {
            angle -= 6.2831855f;
        }
        if (angle < -3.1415927f) {
            angle += 6.2831855f;
        }
        rot.y = angle;
        lbl_8087EFE8->mListener.fn_800C7F08(pos, rot);
    }
    if (m564C > 0 && --m564C <= 0 && lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead != 0) {
        lbl_8087F8A0->mHead->m54C &= ~0x10;
    }
    if (lbl_8087F8A0 != 0) {
        int count = 0;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if (!p->IsFlag_unk(4) && (p->m54C & 0x2000) != 0x2000 && p->m5C->m122 == 2) {
                count++;
            }
        }
        if (mGlobalFlags[134] != count) {
            fn_80370320(0x86, count, 0);
        }
    }
    if (m54E8 != m54E4) {
        m54E8 = m54E4;
        m54EC = 0;
    } else {
        m54EC++;
    }
    if (m56F0 != 0) {
        if (m56F0->GetFlag1_unk() == 1) {
            if (!lbl_8087F540->fn_80481668()) {
                m56F0->fn_800D246C(0);
            }
        } else if (m56F0->mState == 4) {
            fn_800D2338(m56F0);
            m56F0 = 0;
        }
    } else if (m571C.mCount != 0 && m54E4 != 8 && m54E4 != 13) {
        int id = m571C.Front_unk();
        m571C.Pop_unk();
        fn_80376324(this, id);
    }
}

// inferred: sound names of the field (lbl_8074DA10; replay icons use 5 and 6)
static const char* const sFieldSounds_unk[] = {
    "SE_UI001_002", "SE_UI001_007", "SE_UI001_016", "SE_UICOM_003", "SE_UICOM_001", "SE_UI001_012", "SE_UI001_013",
};

// inferred: one line of the debug text, right aligned at x 590
static inline void DrawDebugLine_unk(const wchar_t* text, float y, float size, float z) {
    float width = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 1);
    lbl_8087EEB0->fn_80061824(590.0f - width, y, z, size, size, 0.0f, 0.0f, 0.0f, text, 0xFFFFFFFF, 1, 1, 0, 0,
                              0xFF000000);
}

// inferred: hides the replay icons of the HUD and rewinds the first four
static inline void HideReplayIcons_unk(GameUI* ui) {
    ui->mB08[0]->FreezeTask();
    ui->mB08[1]->FreezeTask();
    ui->mB08[2]->FreezeTask();
    ui->mB08[3]->FreezeTask();
    ui->mB08[4]->FreezeTask();
    ui->mB08[5]->FreezeTask();
    ui->mB20[0]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[0])->Rewind_unk();
    ((atn::D2A_Task*)ui->mB08[1])->Rewind_unk();
    ((atn::D2A_Task*)ui->mB08[2])->Rewind_unk();
    ((atn::D2A_Task*)ui->mB08[3])->Rewind_unk();
}

static inline bool IsIconHidden_unk(atn::Task* task) {
    return task->IsFlag_unk(4);
}

static inline bool IsIconEnd_unk(atn::Task* task) {
    return ((atn::D2A_Task*)task)->IsEnd_unk();
}

inline void atn::D2A_Task::SetLayoutValue_unk(const char* name, float value) {
    atn::D2A_Layout_unk* layout = &mData.m10;
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

// inferred: used memory of a heap in MB
static inline float GetHeapUsed_unk(atn::DLAllocator* heap) {
    return (unsigned int)((int)heap->vf14() - (int)heap->vf18() + 4) / 1048576.0f;
}

static inline float GetHeapFree_unk(atn::DLAllocator* heap) {
    return (unsigned int)heap->fn_80084F84() / 1048576.0f;
}

// NONMATCHING: written for the order of the string pool; not compared yet
void GameTask::vf14() {
    if (lbl_8087F0A8->m184 != 0) {
        lbl_8087EEB0->mA0 = 0x13;
        float y = 45.0f;
        wchar_t name[0x100];
        wchar_t text[0x100];
        if (GetCurrentMap() != 0 && GetCurrentMap()->m54 != 0) {
            MapId_unk* id = GetCurrentMap()->m54;
            fn_8006F2F0(lbl_8087EEC8, name, id->mId, 0x100);
            fn_800DD3FC(text, L"%s %s", name, id->mName);
            DrawDebugLine_unk(text, y, 14.0f, 0.0f);
            y += 15.0f;
            fn_800DD3FC(text, L"Chapter: %d", m10D0);
            DrawDebugLine_unk(text, y, 14.0f, 0.0f);
            y += 15.0f;
            atn::Vector3 pos = lbl_8087F8A0->mHead->mPos;
            fn_800DD3FC(text, L"Pos: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
            DrawDebugLine_unk(text, y, 14.0f, 0.0f);
            y += 15.0f;
            if (m54E4 == 8 && lbl_8087F540->m70 != 0) {
                Event::Sequence* sequence = lbl_8087F540->m70;
                fn_8006F2F0(lbl_8087EEC8, name, sequence->mName.c_str(), 0x100);
                if (sequence->fn_80541210() != 0) {
                    fn_800DD3FC(text, L"Event: %s  cut %d", name, sequence->fn_80541210()->mIndex);
                } else {
                    fn_800DD3FC(text, L"Event: %s", name);
                }
                DrawDebugLine_unk(text, y, 14.0f, 0.0f);
                y += 15.0f;
            }
            if (lbl_8087F0A8->m194 != 0) {
                unsigned int seconds = m5744 / 30;
                fn_800DD3FC(text, L"PlayTime: %02d:%02d", seconds / 60, seconds % 60);
                DrawDebugLine_unk(text, y, 14.0f, 0.0f);
                y += 15.0f;
            }
        }
        float used1 = GetHeapUsed_unk(&atn::MemoryManager::Instance()->mMem1);
        float used2 = GetHeapUsed_unk(&atn::MemoryManager::Instance()->mMem2);
        float usedDbg = GetHeapUsed_unk(&atn::MemoryManager::Instance()->mOverflow);
        float free1 = GetHeapFree_unk(&atn::MemoryManager::Instance()->mMem1);
        float free2 = GetHeapFree_unk(&atn::MemoryManager::Instance()->mMem2);
        float freeDbg = GetHeapFree_unk(&atn::MemoryManager::Instance()->mOverflow);
        char buf[0x100];
        sprintf(buf, "MEM1+2: %8.4f / %8.4f", free1 + free2, used1 + used2);
        fn_8006F2F0(lbl_8087EEC8, text, buf, 0x100);
        DrawDebugLine_unk(text, y, 12.0f, -1000000.0f);
        y += 13.0f;
        sprintf(buf, "MEMDBG: %8.4f / %8.4f", freeDbg, usedDbg);
        fn_8006F2F0(lbl_8087EEC8, text, buf, 0x100);
        DrawDebugLine_unk(text, y, 12.0f, -1000000.0f);
        lbl_8087EEB0->mA0 = 0x12;
    }
    m5620->FreezeTask();
    if (m550C.mState != 0) {
        if (m550C.mState == 1) {
            m5624->Capture();
        }
        m5620->MeltTask();
        return;
    }
    if (m563C >= 0) {
        m5624->m78 = m563C;
        m5624->Capture();
        m563C = -1;
    }
    if (CrossbowMenuTask::sInstance->IsFlag_unk(4)) {
        CrossbowMenuTask::sInstance->fn_801E9EA4();
    }
    CrossbowMenuTask::sInstance->FreezeTask();
    PlayerTask* leader = lbl_8087F8A0->mHead;
    if (m54E4 == 0 && mCamera.m7FC != 6 && mCamera.m7FC != 5) {
        if ((leader->m648 != 0 && leader->m648->m04 == 1) ||
            (leader->mFlag6 && (leader->m50 == 700022 || leader->m50 == 700023))) {
            CrossbowMenuTask::sInstance->MeltTask();
        }
    }
    lbl_8087F490->fn_803D2A6C();
    fn_803D83A8(lbl_8087F490);
    int showIcons = 0;
    int tacView = 0;
    int swing = SetConfig::sInstance->mSwingReplay;
    switch (m54E4) {
    case 0: {
        if (mCamera.m7FC == 3 || mCamera.m7FC == 4) {
            lbl_8087F8A0->mHead->fn_800E426C();
        }
        if (m56D4 != 0) {
            m56D4->fn_80149A30();
        }
        m5620->MeltTask();
        int mode = mCamera.m7FC;
        if (mode == 6 && mode == mCamera.m800) {
            showIcons = 1;
        } else if (mode == 7) {
            showIcons = 1;
            tacView = 1;
        } else if (mode == 5) {
            PlayerTask* player = lbl_8087F8A0->mHead;
            if (lbl_8087F610 != 0 && player != 0 && player->m55C == 6 && player->m560 == 13) {
                showIcons = 1;
                tacView = 1;
            }
        }
        if (m5590 == 0) {
            break;
        }
        GameUI* ui = lbl_8087F490;
        if ((m5594 != 0 && m55A4 > 0) || m55B8 > 0) {
            if ((!swing && IsIconHidden_unk(ui->mB08[0]) && IsIconHidden_unk(ui->mB08[1])) ||
                (swing && IsIconHidden_unk(ui->mB08[2]) && IsIconHidden_unk(ui->mB08[3]))) {
                fn_800C31F4(sFieldSounds_unk[5], 0, -1, 1.0f);
                if (swing) {
                    ui->mB08[2]->MeltTask();
                } else {
                    ui->mB08[0]->MeltTask();
                }
            } else if ((!swing && !IsIconHidden_unk(ui->mB08[0]) && IsIconHidden_unk(ui->mB08[1])) ||
                       (swing && !IsIconHidden_unk(ui->mB08[2]) && IsIconHidden_unk(ui->mB08[3]))) {
                if (swing) {
                    ui->mB08[3]->FreezeTask();
                    if (IsIconEnd_unk(ui->mB08[2])) {
                        ui->mB08[3]->MeltTask();
                    }
                } else {
                    ui->mB08[1]->FreezeTask();
                    if (IsIconEnd_unk(ui->mB08[0])) {
                        ui->mB08[1]->MeltTask();
                    }
                }
            } else if ((!swing && !IsIconHidden_unk(ui->mB08[1])) || (swing && !IsIconHidden_unk(ui->mB08[3]))) {
                if (swing) {
                    ui->mB08[2]->FreezeTask();
                } else {
                    ui->mB08[0]->FreezeTask();
                }
            }
        } else {
            if (!((!swing && IsIconHidden_unk(ui->mB08[0]) && IsIconHidden_unk(ui->mB08[1])) ||
                  (swing && IsIconHidden_unk(ui->mB08[2]) && IsIconHidden_unk(ui->mB08[3])))) {
                fn_800C31F4(sFieldSounds_unk[6], 0, -1, 1.0f);
            }
            HideReplayIcons_unk(ui);
        }
        break;
    }
    case 1:
        m5620->MeltTask();
        break;
    case 3:
        m5620->MeltTask();
        break;
    case 7: {
        GameUI* ui = lbl_8087F490;
        if (!IsIconHidden_unk(ui->mB20[0]) && IsIconHidden_unk(ui->mB08[5]) && IsIconEnd_unk(ui->mB20[0])) {
            ui->mB20[0]->FreezeTask();
            ui->mB08[5]->MeltTask();
        }
        if (!IsIconHidden_unk(ui->mB08[4]) && IsIconEnd_unk(ui->mB08[4])) {
            ui->mB08[4]->FreezeTask();
        }
        fn_8056F2F8(m5590, -m5598);
        if (mFinish.m394 != 0 && m5598 == 0) {
            ((atn::D2A_Task*)ui->mB08[5])->SetLayoutValue_unk("iconOn", 0.0f);
            showIcons = 1;
        }
        break;
    }
    case 14: {
        GameUI* ui = lbl_8087F490;
        if (!IsIconHidden_unk(ui->mB20[0]) && IsIconHidden_unk(ui->mB08[5]) && IsIconEnd_unk(ui->mB20[0])) {
            ui->mB20[0]->FreezeTask();
            ui->mB08[5]->MeltTask();
        }
        if (!IsIconHidden_unk(ui->mB08[4]) && IsIconEnd_unk(ui->mB08[4])) {
            ui->mB08[4]->FreezeTask();
        }
        if (mFinish.m394 != 0) {
            ((atn::D2A_Task*)ui->mB08[5])->SetLayoutValue_unk("iconOn", 0.0f);
            showIcons = 1;
            if (lbl_8087F9F8 != 0 && lbl_8087F9F8->mA70 == 0) {
                tacView = 1;
            }
        }
        break;
    }
    case 8:
        m5620->MeltTask();
        if (m5590 != 0) {
            HideReplayIcons_unk(lbl_8087F490);
        }
        break;
    case 9:
    case 10:
    case 11:
        mFinish.fn_8037C69C();
        if (m5590 != 0) {
            HideReplayIcons_unk(lbl_8087F490);
        }
        break;
    case 12:
        fn_8046ECDC(lbl_8087F518, 1);
        m5620->MeltTask();
        if (m56E4 == 0) {
            GameScene* scene = m10D8;
            PlayerTask* player = lbl_8087F8A0->mHead;
            gs::WarpPoint* warp = 0;
            for (unsigned int i = 0; i < scene->m78.size(); i++) {
                if (m56E8 == scene->m78.mData[i].mId) {
                    warp = &scene->m78.mData[i];
                    break;
                }
            }
            if (warp != 0) {
                player->mPos = warp->mPos;
                player->mRot = atn::Vector3(0.0f, warp->m14, 0.0f);
                player->fn_8016EB48(0, 0, 0);
                mCamera.fn_8037EF30(1, 1, -1.0f);
                if (m10D0 >= 600000) {
                    PlayerTask* second = lbl_8087F8A0->fn_8054A33C(1);
                    if (second != 0) {
                        atn::Vector3 offset(-5.0f, 0.0f, -5.0f);
                        Mtx m;
                        PSMTXRotRad(m, 'y', warp->m14);
                        PSMTXMultVec(m, &offset, &offset);
                        atn::Vector3 target(warp->mPos.x + offset.x, warp->mPos.y + offset.y, warp->mPos.z + offset.z);
                        int index = m10D8->fn_803C1560(&target, 0x80000000, 0, 0, 1, -1.0f);
                        if (index != 0) {
                            gs::MovePoint* point = &m10D8->m98.mData[index - 1];
                            second->mPos = point->mPos;
                            second->mRot = atn::Vector3(0.0f, warp->m14, 0.0f);
                            second->fn_8016EB48(0, 0, 0);
                        }
                    }
                }
            }
        } else if (m56E4 > 5) {
            if (lbl_8087FA20 == 0 || lbl_8087FA20->fn_800D3FA4()) {
                fn_8036FC58();
            }
        }
        m56E4++;
        break;
    }
    fn_803DAB4C(lbl_8087F490, showIcons, tacView);
    mCamera.fn_8039328C();
}

// inferred: places the party at the warp point `warp` (or the start point) once the map is loaded
// NONMATCHING: 99.6% - r3/r4 swapped for lbl_8087F418 and the map in the sound check
void GameTask::fn_8036CA60(int warp) {
    m10D8->fn_800D246C(0);
    if (warp > 0) {
        m10D8->fn_803C0F48(warp);
    } else {
        m10D8->fn_803C0F48(m10D8->m70);
    }
    mCamera.fn_80389838(mCamera.m804);
    mCamera.fn_8037EF30(1, 1, -1.0f);
    MapTask* map = m10D8->m64;
    if ((unsigned int)(map->mType - 2) <= 2) {
        for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
            if ((enemy->m137C & 0x40) != 0x40) {
                enemy->fn_8014DEE4(0, 0, 0);
            }
        }
        for (PlayerTask* player = lbl_8087F8A0->mHead; player != 0; player = player->mNext) {
            if ((player->m137C & 0x40) != 0x40) {
                player->fn_8014DEE4(0, 0, 0);
            }
        }
        if (map->mType == 2 && m5590 == 0 && map->mId == 9) {
            m5590 = new ((MemoryTag)1, "", "", 0) GameTaskData_unk;
            lbl_8087F3C0->mSystem.mListener = (EffectListener_unk*)m5590;
        }
    }
    fn_80371320(lbl_8087F8A0->mHead);
    if (lbl_8087F418 != 0) {
        if (lbl_8087F418->mState != 5) {
            MapTask* current = m10D8->m64;
            if (current != 0 && current->mType == 2 && current->mId == 0x29 && current->mSubId == 0xD && warp == 100) {
                lbl_8087F418->m74 = 3;
            }
            lbl_8087F418->fn_8035E7CC();
        }
        lbl_8087F418->fn_80360B00();
    }
    if (map->mType == 2 && map->mId == 0x28 && map->mSubId == 4) {
        mGlobalFlags[921] = 1;
    }
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

// inferred: a character moving fast enough slips on a nearby gimmick of kind 30/31 (called by CharaTask)
extern "C" int fn_8036DBC0(GameTask* task, const atn::Vector3* pos, const atn::Vector3* move, CharaTask* chara) {
    if (task->m10D8 == 0) {
        return 0;
    }
    if (task->m10D8->fn_803CB820()) {
        return 1;
    }
    if (chara->m55C == 6) {
        return 0;
    }
    float speed = PSVECSquareMag(move);
    int slow = 0;
    if (chara->IsPartyMember_unk() && ((task->m5744 + chara->mId) & 3) == 0) {
        slow = 1;
    }
    if (speed > (slow ? 0.1f : 0.7f) && !chara->mFlag3) {
        for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
            if ((unsigned int)(g->m50 - 30) <= 1 && !g->IsFlag_unk(4) && g->vf60()) {
                atn::Vector3 d(g->m6C.x - chara->mPos.x, g->m6C.y - chara->mPos.y, g->m6C.z - chara->mPos.z);
                float dist = PSVECSquareMag(&d);
                PSVECNormalize(&d, &d);
                if (dist < 64.0f) {
                    GimmickMsg_unk msg;
                    msg.mType = 3;
                    msg.m10 = (int)chara;
                    g->vf40(&msg);
                    chara->fn_8016D0A0(0);
                    return 1;
                }
            }
        }
    }
    return 0;
}

extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

// inferred: friend in front of the leader and close enough to talk (called by BattleCharaTask)
// NONMATCHING: 99.7% - r28/r29 swapped (the found flag and the int-to-float constant)
extern "C" EnemyTask* fn_8036DDD4(GameTask* task) {
    PlayerTask* leader = lbl_8087F8A0->mHead;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', leader->mRot.y);
    PSMTXMultVec(m, &dir, &dir);
    for (EnemyTask* chara = (EnemyTask*)FriendManager::sInstance->fn_8036554C(); chara != 0; chara = chara->mNext) {
        if (chara->IsFlag_unk(4) || chara->mF50 != 0 || chara->fn_80175AEC()) {
            continue;
        }
        if ((chara->mStatus.m0C & 0x20) == 0x20 && !chara->fn_8015EB2C()) {
            continue;
        }
        if (chara->mStatus.m0C == 0 && !(chara->mStatus.m04 / (float)chara->mStatus.mParamB.m84 < 1.0f) &&
            chara->mStatus.m1C >= 0) {
            if (!chara->vfF4()) {
                continue;
            }
            if (!chara->CanAct_unk()) {
                continue;
            }
        }
        atn::Vector3 d(chara->mPos.x - leader->mPos.x, chara->mPos.y - leader->mPos.y, chara->mPos.z - leader->mPos.z);
        float dist = PSVECSquareMag(&d);
        PSVECNormalize(&d, &d);
        float range = 30.0f;
        if (lbl_8087F0A8->m320 != 0) {
            range *= 0.667f;
        }
        int found = 0;
        if (dist < range * range && PSVECDotProduct(&d, &dir) > 0.1f) {
            found = 1;
        }
        if (found) {
            return chara;
        }
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


// NONMATCHING: 99.8% - registers of the inlined fn_80370C2C (same swap as that function)
// inferred: nonzero when the field menu can be opened (field idle or a closing shop, no fade, no event)
int GameTask::fn_8036E170() {
    if (m54E4 != 0 && m54E4 != 14) {
        if (m54E4 != 3) {
            return 0;
        }
        if (ShopBuySell::sInstance != 0 && ShopBuySell::sInstance->m25B40 == 0) {
            return 0;
        }
        if (lbl_8087F9E0 != 0 && ((ShopBase*)lbl_8087F9E0)->m25B40 == 0) {
            return 0;
        }
        if (ShopColor::sInstance != 0 && ShopColor::sInstance->m25B40 == 0) {
            return 0;
        }
        if (ShopIdentify::sInstance != 0 && ShopIdentify::sInstance->m25B40 == 0) {
            return 0;
        }
        if (LibraryTask::sInstance != 0) {
            return 0;
        }
    }
    if (!fn_80370C2C()) {
        return 0;
    }
    if (m5620 != 0 && m5620->mActive != 0 && !m5620->IsEnd()) {
        return 0;
    }
    if (lbl_8087F540->fn_80481668()) {
        return 0;
    }
    if (mGlobalFlags[108] != 0 || lbl_8087F610 != 0) {
        return 0;
    }
    GameMessageWindow* window = lbl_8087F490->m263C;
    if (window != 0 && window->mState != 0) {
        return 0;
    }
    if (m56D4 != 0 || m56D8 != 0) {
        return 0;
    }
    if (m54E4 != 3) {
        PlayerTask* leader = lbl_8087F8A0->mHead;
        if (leader->fn_8017C974() || m5760 > 0) {
            return 0;
        }
        if (leader->m568 < 0.001f) {
            return 0;
        }
        if (leader->m55C != 1) {
            if (leader->m55C == 6) {
                if (leader->m560 != 4) {
                    return 0;
                }
            } else {
                return 0;
            }
        }
    }
    if (m10D8 != 0 && m10D8->m134 != 0 && m10D8->m134->mE8 > 0) {
        return 0;
    }
    int state;
    if (lbl_8087EF68 != 0 && fn_800A4228(lbl_8087EF68, &state)) {
        return 0;
    }
    return 1;
}

void GameTask::fn_8036E430(int a, int b) {
    m5624->m78 = 0x12;
    m5624->Capture();
    m5684 = 1;
    m5688 = a;
    m568C = b;
}

extern "C" void fn_80570A40(GameTaskData_unk* obj, int value);
extern "C" void fn_8056BD34(ReplayRecorder_unk* recorder); // inferred: stops the recording
// inferred: sound engine of lbl_8087EE90 (as EnvSeEngine_unk of game/EnvSeManager.cpp)
struct SoundEngineReverb_unk {
    unsigned char m000[0x8C0];
    int m8C0; // inferred: reverb of the field
};
extern "C" void fn_8004A1D4(void* engine, int reverb); // inferred: sets the reverb
extern "C" void fn_803CE070(GameSoundControl* control);

// inferred: leaves the field for the camp (called by game/NewCampTask)
extern "C" void fn_8036E624(GameTask* task) {
    if (task->m5590 != 0) {
        fn_80570A14(task->m5590, 0);
        fn_80570A40(task->m5590, 0);
    }
    fn_8056BD34(task->m558C.mPtr);
    task->fn_8036EE0C();
    if (task->m567C != 0) {
        task->m567C->FreezeTask();
    }
    lbl_8087EFE8->mFx2dEnabled = 0;
    fn_8004A1D4(lbl_8087EE90, ((SoundEngineReverb_unk*)lbl_8087EE90)->m8C0);
    EnvSeManager* envSe = lbl_8087F418;
    envSe->m70 = envSe->m6C;
    envSe->m6C = 1;
    fn_803CE070(GameSoundControl::sInstance);
    lbl_8087F3C0->mSystem.mReplaying = 0;
}

extern "C" void fn_8058ABF4(atn::Task* parent, int a, int b, int kind); // inferred: creates the upgrade shop

// inferred: opens a shop of the field (called by game/GameEventScript)
extern "C" void fn_8036E6D4(GameTask* task, int type, int sub, int arg) {
    if (lbl_8087F430 != 0) {
        lbl_8087F430->fn_80370320(0x394, 1, 0);
    }
    task->fn_8036EA6C(3);
    switch (type) {
    case 0:
        ShopBuySell::fn_80584FE4(task, type, sub, arg);
        break;
    case 1:
        ShopBuySell::fn_80584FE4(task, type, sub, arg);
        break;
    case 4:
        ShopBuySell::fn_80584FE4(task, 4, sub, arg);
        break;
    case 2:
        if (sub == 0) {
            ShopIdentify::fn_80588684(task, sub, arg);
        } else if (sub == 1) {
            fn_8058ABF4(task, sub, arg, 0);
        } else if (sub == 2) {
            ShopColor::fn_80586DB0(task, sub, arg);
        } else if (sub == 3) {
            fn_8058ABF4(task, sub, arg, 1);
        } else if (sub == 4) {
            fn_8058ABF4(task, sub, arg, 2);
        } else if (sub == 5) {
            fn_8058ABF4(task, sub, arg, 3);
        } else {
            ShopIdentify::fn_80588684(task, sub, arg);
        }
        break;
    }
    if (lbl_8087F540->fn_80481654()) {
        lbl_8087F540->fn_8048169C(1);
    }
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

// inferred: enters a menu state of the field (1: everything frozen, 2: leader frozen, 3-5: shops and menus)
void GameTask::fn_8036EA6C(int state) {
    m5680 = m54E4;
    m54E4 = state;
    switch (state) {
    case 1:
        m10D8->FreezeTask();
        GetCurrentMap()->FreezeTask();
        ((atn::Task*)lbl_8087F048)->FreezeTask();
        lbl_8087F8A0->FreezeTask();
        EnemyManager::sInstance->FreezeTask();
        NpcManager::sInstance->FreezeTask();
        lbl_8087F098->FreezeTask();
        lbl_8087EE68->FreezeTask();
        GimmickManager::instance_->FreezeTask();
        lbl_8087F3C0->FreezeTask();
        MapManager::instance_->mType = 6;
        GameSoundControl::sInstance->FreezeTask();
        lbl_8087F490->FreezeTask();
        if (lbl_8087F9F8 != 0) {
            lbl_8087F9F8->FreezeTask();
        }
        if (ShopBuySell::sInstance != 0) {
            ShopBuySell::sInstance->FreezeTask();
        }
        if (lbl_8087F9E0 != 0) {
            lbl_8087F9E0->FreezeTask();
        }
        if (ShopColor::sInstance != 0) {
            ShopColor::sInstance->FreezeTask();
        }
        if (ShopIdentify::sInstance != 0) {
            ShopIdentify::sInstance->FreezeTask();
        }
        if (LibraryTask::sInstance != 0) {
            LibraryTask::sInstance->FreezeTask();
        }
        lbl_8087F9E8->FreezeTask();
        if (lbl_8087F540 != 0) {
            lbl_8087F540->FreezeTask();
        }
        break;
    case 3:
    case 4:
    case 5:
        MapManager::instance_->fn_800D246C(1);
        ((atn::Task*)lbl_8087F048)->fn_800D246C(1);
        lbl_8087F8A0->fn_800D246C(1);
        EnemyManager::sInstance->fn_800D246C(1);
        NpcManager::sInstance->fn_800D246C(1);
        lbl_8087EE68->fn_800D246C(1);
        lbl_8087F3C0->fn_800D246C(1);
        lbl_8087F418->fn_800D246C(1);
        m10D8->fn_800D246C(1);
        if (lbl_8087F540 != 0) {
            lbl_8087F540->fn_800D246C(1);
        }
        break;
    case 2:
        lbl_8087F8A0->mHead->m54C |= 0x10;
        break;
    }
    if (lbl_8087FA20 != 0 && ((TaskWithTac_unk*)lbl_8087FA20)->m258 != 0) {
        ((atn::Task*)((TaskWithTac_unk*)lbl_8087FA20)->m258)->FreezeTask();
    }
    MenuCommonTask::sInstance->fn_804A2F98();
    MenuCommonTask::sInstance->fn_804A313C();
    lbl_8087F490->m263C->FreezeTask();
    lbl_8087F490->m2640->FreezeTask();
    MarketTask::sInstance->FreezeTask();
    GameTimer::sInstance->FreezeTask();
    lbl_8087EFE8->fn_800D0518(1, 0.2f, 30);
    lbl_8087EFE8->m34D4 = 2;
    lbl_8087EFE8->fn_800CFBA0(8, 1, 4, 15);
    if (lbl_8087EFE8->fn_800D089C(1, 1) > 0 && GameBgm::sInstance != 0) {
        int id = GameBgm::sInstance->m84 != 0 ? GameBgm::sInstance->mNext.mId : GameBgm::sInstance->mReq.mId;
        if (id != 0x67) {
            lbl_8087EFE8->fn_800CFBA0(2, 1, 4, 15);
        }
    }
}

// inferred: leaves the menu state entered by fn_8036EA6C and goes back to the previous state
// NONMATCHING: 98.5% - the original tests m5680 against {1, 3, 4, 5, 14} with a bit mask and the 1..5 range with
// two compares; ours merges 3..5 into a range test (tried switch, if chains in several orders, a local)
void GameTask::fn_8036EE0C() {
    switch (m54E4) {
    case 1:
        m10D8->MeltTask();
        GetCurrentMap()->MeltTask();
        ((atn::Task*)lbl_8087F048)->MeltTask();
        lbl_8087F8A0->MeltTask();
        EnemyManager::sInstance->MeltTask();
        NpcManager::sInstance->MeltTask();
        lbl_8087F098->MeltTask();
        lbl_8087EE68->MeltTask();
        GimmickManager::instance_->MeltTask();
        lbl_8087F3C0->MeltTask();
        MapManager::instance_->mType = 0;
        GameSoundControl::sInstance->MeltTask();
        lbl_8087F490->MeltTask();
        if (lbl_8087F9F8 != 0) {
            lbl_8087F9F8->MeltTask();
        }
        if (ShopBuySell::sInstance != 0) {
            ShopBuySell::sInstance->MeltTask();
        }
        if (lbl_8087F9E0 != 0) {
            lbl_8087F9E0->MeltTask();
        }
        if (ShopColor::sInstance != 0) {
            ShopColor::sInstance->MeltTask();
        }
        if (ShopIdentify::sInstance != 0) {
            ShopIdentify::sInstance->MeltTask();
        }
        if (LibraryTask::sInstance != 0) {
            LibraryTask::sInstance->MeltTask();
        }
        lbl_8087F9E8->MeltTask();
        if (lbl_8087F540 != 0) {
            lbl_8087F540->MeltTask();
        }
        break;
    case 3:
    case 4:
    case 5:
        MapManager::instance_->fn_800D246C(0);
        ((atn::Task*)lbl_8087F048)->fn_800D246C(0);
        lbl_8087F8A0->fn_800D246C(0);
        EnemyManager::sInstance->fn_800D246C(0);
        NpcManager::sInstance->fn_800D246C(0);
        lbl_8087EE68->fn_800D246C(0);
        lbl_8087F3C0->fn_800D246C(0);
        lbl_8087F418->fn_800D246C(0);
        m10D8->fn_800D246C(0);
        lbl_8087F490->fn_803E4100();
        if (lbl_8087F540 != 0) {
            lbl_8087F540->fn_800D246C(0);
        }
        break;
    case 2:
        lbl_8087F8A0->mHead->m54C &= ~0x10;
        break;
    }
    int keep = 0;
    int prev = m5680;
    if (prev == 1 || prev == 3 || prev == 4 || prev == 5 || prev == 14) {
        keep = 1;
    }
    if (!keep) {
        MenuCommonTask::sInstance->fn_804A2FBC();
        GameTimer::sInstance->MeltTask();
    }
    if (!keep) {
        if (lbl_8087FA20 != 0 && ((TaskWithTac_unk*)lbl_8087FA20)->m258 != 0) {
            ((atn::Task*)((TaskWithTac_unk*)lbl_8087FA20)->m258)->MeltTask();
        }
        lbl_8087F490->m263C->MeltTask();
        lbl_8087F490->m2640->MeltTask();
        MarketTask::sInstance->MeltTask();
    }
    MenuCommonTask::sInstance->fn_804A313C();
    if (m5680 < 1 || m5680 > 5) {
        lbl_8087EFE8->fn_800D0518(1, 1.0f, 30);
        lbl_8087EFE8->m34D4 = 1;
        lbl_8087EFE8->fn_800CFBA0(8, 0, 4, 15);
        lbl_8087EFE8->fn_800CFBA0(2, 0, 4, 15);
        lbl_8087F0A8->mInput.m14[0x24] = 1; // inferred: locks actions 0x24 and 7
        lbl_8087F0A8->mInput.m14[7] = 1;
    }
    if (m54E4 == 1 && m5680 >= 3 && m5680 <= 5) {
        m54E4 = m5680;
        m5680 = 0;
        if (ShopBuySell::sInstance != 0) {
            ShopBuySell::sInstance->vf40();
        }
        if (lbl_8087F9E0 != 0) {
            ((ShopBase*)lbl_8087F9E0)->vf40();
        }
        if (ShopColor::sInstance != 0) {
            ShopColor::sInstance->vf40();
        }
        if (ShopIdentify::sInstance != 0) {
            ShopIdentify::sInstance->vf40();
        }
    } else {
        m54E4 = m5680;
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

// inferred: sets the chapter (m10D0), warning on a rollback or a chapter out of range
// inferred: nonzero while the field is not ready to change the map
int GameTask::fn_8036FD4C() {
    int busy = 0;
    if (!m550C.mScene->IsFlag_unk(2)) {
        busy = 1;
    }
    if (!lbl_8087F8A0->fn_800D3FA4()) {
        busy = 1;
    }
    if (!EnemyManager::sInstance->fn_800D3FA4()) {
        busy = 1;
    }
    if (!NpcManager::sInstance->fn_800D3FA4()) {
        busy = 1;
    }
    if (fn_800F3490(lbl_8087F048)) {
        busy = 1;
    }
    if (lbl_8087F490->fn_803D1574()) {
        busy = 1;
    }
    if (!lbl_8087F8A8->fn_800D3FA4()) {
        busy = 1;
    }
    if (!BattleResult::sInstance->fn_800D3FA4()) {
        busy = 1;
    }
    if (fn_8006D3F8(lbl_8087EEC8)) {
        busy = 1;
    }
    atn::FadeCapture* fade = m5624;
    if (fade->mFrame < fade->mDelay + fade->mDuration) {
        busy = 1;
    } else if (busy) {
        fn_8046ECDC(lbl_8087F518, 1);
    }
    return busy;
}

// inferred: position of the party in the save data (names invented)
struct SavePosition_unk {
    unsigned char m0000[0x4F64];
    atn::Vector3 mPos; // 0x4F64
    atn::Vector3 mRot; // 0x4F70
};

// inferred: start position of the party: from the save data, else from the route place of the entry
// (called by game/GameScene)
// NONMATCHING: 98.2% - registers of the inlined FindPlace_unk loops (as fn_80374710)
extern "C" void fn_8036FE74(GameTask* task, atn::Vector3* pos, atn::Vector3* rot) {
    *pos = lbl_807C7030;
    *rot = lbl_807C7030;
    if (task->m550C.m30 != 0) {
        SavePosition_unk* save = (SavePosition_unk*)task->m550C.m30;
        *pos = save->mPos;
        *rot = save->mRot;
        return;
    }
    if (task->m550C.mScene != 0) {
        RouteTable_unk* table = (RouteTable_unk*)task->m550C.mScene;
        RoutePlace_unk* place = table->FindPlace_unk(task->m550C.m14);
        if (place == 0) {
            place = table->FindPlace_unk(table->m70);
        }
        if (place != 0) {
            *pos = place->mPos;
            rot->y = place->m14;
        }
        return;
    }
    if (task->mOption.m1C != 0) {
        SavePosition_unk* save = (SavePosition_unk*)task->mOption.m1C;
        *pos = save->mPos;
        *rot = save->mRot;
        return;
    }
    if (task->m10D8 != 0) {
        RouteTable_unk* table = (RouteTable_unk*)task->m10D8;
        RoutePlace_unk* place = table->FindPlace_unk(task->mOption.m10);
        if (place == 0) {
            place = table->FindPlace_unk(table->m70);
        }
        if (place != 0) {
            *pos = place->mPos;
            rot->y = place->m14;
        }
    }
}

extern "C" void fn_80370094(GameTask* task, int chapter) {
    if (lbl_8087EEB8 != 0) {
        char buf[0x100];
        if (task->m10D0 > chapter) {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "[WARNING] rollback chapter %d -> %d\n", task->m10D0, chapter);
            lbl_8087EEB8->fn_800697D8(buf);
        }
        if (task->m10D0 < 0 || 999999 < task->m10D0) {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "[WARNING] chapter is out of range %d\n", chapter);
            lbl_8087EEB8->fn_800697D8(buf);
        }
    }
    task->m10D0 = chapter;
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

// inferred: sets or clears status bit `bit` of the party member `chara` in the inventory and on the character
// NONMATCHING: 75% - the range test of the member and the register order of the bit math
extern "C" void fn_803701E0(CharaTask* chara, int bit, int on) {
    if (chara != 0) {
        int member = fn_80219558(chara->m50);
        if (member >= 0 && member < 7) {
            CharaStatusData_unk* data = &Inventory::instance_->mPlayers[member].m1A8;
            if (on) {
                data->mBits[bit / 32] |= 1 << (bit % 32);
                if (lbl_8087F048 != 0) {
                    fn_801092C8(lbl_8087F048, 0x14, chara, bit, 0);
                }
            } else {
                data->mBits[bit / 32] &= ~(1 << (bit % 32));
            }
            if (chara != 0) {
                unsigned int mask = 1 << (bit % 32);
                if ((data->mBits[bit / 32] & mask) == mask) {
                    chara->mStatus.m1A8.mBits[bit / 32] |= mask;
                } else {
                    chara->mStatus.m1A8.mBits[bit / 32] &= ~mask;
                }
            }
        }
    }
}

// inferred: saves the status of `chara` and starts the finishing skill `id` (inlined three times)
inline void GameTask::StartFinish_unk(CharaTask* chara, int id) {
    Inventory* inventory = Inventory::instance_;
    inventory->mDF30.mChara = chara;
    inventory->mDF30.mData = chara->mStatus.m1A8;
    inventory->mDF30.mData.m08 = 0.0f;
    inventory->mDF30.m8C = 0;
    inventory->mDF30.m80 = 0;
    inventory->mDF30.m88 = 0;
    Inventory::instance_->mDF30.m90 = id;
    fn_80371790();
    fn_80374D64();
}

// inferred: SetGlobalFlag (arg: do not run the side effects of the flag)
// NONMATCHING: the copy of CharaStatusData_unk and the GameUI calls (the original passes the id too)
void GameTask::fn_80370320(int index, int value, int arg) {
    if ((unsigned int)index > 0xFFF) {
        if (lbl_8087EEB8 != 0) {
            char buf[0x100];
            sprintf(buf, "SetGlobalFlag:Index Out Of Bounds! %d", index);
            lbl_8087EEB8->fn_800697D8(buf);
        }
        return;
    }
    if (index == 0) {
        return;
    }
    if (mGlobalFlags[283] > 0 &&
        (index == 0x88 || index == 0x11E || index == 0x11F || index == 6 || index == 0xD0 || index == 5)) {
        return;
    }
    if (lbl_8087F0A8->m194 != 0 && (index == 0x11F || index == 6)) {
        return;
    }
    int old = mGlobalFlags[index];
    mGlobalFlags[index] = value;
    if (m10D8 != 0 && m10D8->m134 != 0) {
        fn_803AB96C(m10D8->m134, 1, index, old, value);
    }
    if (arg != 0) {
        return;
    }
    if (old != value && Inventory::instance_ != 0 && lbl_8087F8A0 != 0) {
        CharaTask* chara = lbl_8087F8A0->m54;
        if (index == 5) {
            fn_803701E0(chara, 10, value == 0);
            fn_803701E0(chara, 13, value == 0);
        }
        if (index == 0x88) {
            fn_803701E0(chara, 9, value == 0);
            if (chara != 0 && (chara->mStatus.m1A8.mBits[0] & 0x200) == 0x200) {
                StartFinish_unk(chara, 9);
            }
        }
        if (index == 0x11E) {
            fn_803701E0(chara, 0x3D, value == 0);
            if (chara != 0 && (chara->mStatus.m1A8.mBits[1] & 0x20000000) == 0x20000000) {
                StartFinish_unk(chara, 0x3D);
            }
        }
        if (index == 0x11F) {
            fn_803701E0(chara, 2, value == 0);
            if (chara != 0 && (chara->mStatus.m1A8.mBits[0] & 4) == 4) {
                StartFinish_unk(chara, 2);
            }
        }
    }
    if ((index >= 0x8C && index <= 0x15E) || (index >= 0x1AE && index <= 0x1E0)) {
        if (old != value && value != 0) {
            TutorialData_unk* data = fn_8021F09C(index);
            if (data != 0 && (SetConfig::sInstance->mShowTutorial != 0 || index == 0x1B3) &&
                (mGlobalFlags[283] == 0 || (unsigned int)(index - 0x1B8) <= 1)) {
                if (data->m04 != 0) {
                    fn_80376324(this, index);
                } else {
                    if ((unsigned int)m56F4.mCount < 8) {
                        m56F4.Push_unk(index);
                    }
                    lbl_8087F490->fn_803E43B8(1);
                }
            }
        }
    }
    if (index == 0xAC && value != 0) {
        lbl_8087F490->fn_803E4434(1);
    }
    if (index == 0x6D && value != 0) {
        CrossbowMenuTask::sInstance->fn_801E97DC();
    }
    if (index == 0x25 && value == 0 && old != 0) {
        PlayerTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (leader != 0) {
            mGlobalFlags[index] = old;
            if (leader->HasC48_unk()) {
                leader->fn_80154344();
            }
            if (leader->mFlag6) {
                leader->fn_80155DAC(0);
            }
            mGlobalFlags[index] = value;
        }
    }
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

// NONMATCHING: 99% - r4/r5 swapped in the compare (also tried mDelay + mDuration, locals, no local fade)
int GameTask::fn_80370C2C() {
    atn::FadeCommon* fade = m5620;
    if (fade->mActive != 0) {
        return fade->mFrame < fade->mDelay + fade->mDuration;
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

extern "C" void fn_80376B68(GameTask* task, int arg);

// inferred: starts the game over (field state 6): freezes the field and fades to white
void GameTask::fn_80370CD0() {
    if (GameBgm::sInstance != 0) {
        GameBgm::sInstance->fn_8037D4C0(0, 30, 1, 0, 0.0f);
    }
    if (lbl_8087FA20 != 0 && ((TaskWithTac_unk*)lbl_8087FA20)->m258 != 0) {
        ((atn::Task*)((TaskWithTac_unk*)lbl_8087FA20)->m258)->FreezeTask();
    }
    if (lbl_8087F498 != 0 && m5638 == 0 && lbl_8087F8A0 != 0) {
        PlayerTask* leader = lbl_8087F8A0->mHead;
        if (leader != 0 && !fn_80219558(leader->m50) && (leader->mStatus.m0C & 0x20) == 0x20) {
            lbl_8087F498->fn_803EA77C(0, 0x26, 0, 1.0f, 10.0f);
        }
    }
    for (PlayerTask* player = lbl_8087F8A0->mHead; player != 0; player = player->mNext) {
        player->mC58.mC0 = 0;
    }
    for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
        enemy->mC58.mC0 = 0;
    }
    if (m54E4 == 8) {
        lbl_8087F540->FreezeTask();
        lbl_8087F540->fn_8048169C(1);
        lbl_8087F540->fn_8047E528();
    }
    lbl_8087F490->FreezeTask();
    lbl_8087F490->m263C->fn_803B57B0(1);
    lbl_8087F490->m263C->fn_803B57EC();
    lbl_8087F490->m2640->fn_803B6C88(1);
    ((atn::Task*)lbl_8087F048)->FreezeTask();
    lbl_8087EE68->FreezeTask();
    lbl_8087F8A0->fn_800D246C(1);
    EnemyManager::sInstance->fn_800D246C(1);
    NpcManager::sInstance->fn_800D246C(1);
    lbl_8087F9E8->FreezeTask();
    if (m10D8 != 0) {
        m10D8->FreezeTask();
    }
    if (m56F0 != 0) {
        fn_800D2338(m56F0);
        m56F0 = 0;
    }
    m56F4.mCount = 0;
    m571C.mCount = 0;
    int on = 0;
    if (mGlobalFlags[211] != 0) {
        on = 1;
    }
    fn_80376B68(this, on);
    fn_8037529C(0xFFFFFFFF, 0xFFFFFF, 2, 0, 30);
    m5624->mZ = -5.0f;
    m54E4 = 6;
}

extern "C" void fn_8036F268(GameTask* task, int type, int id, int subId, int entry, int a, void* save,
                            int reason, int arg9); // inferred: changes the map (save: data to restore)

// inferred: leaves the game over screen: continues from `save` (or from the copy the field started from), or
// stays on the map when `stay` is set (called by game/GameOverTask and game/NewCampPartsTop)
extern "C" void fn_80370F8C(GameTask* task, int count, void* save, int stay) {
    if (count != 0) {
        task->m574C = task->mGlobalFlags[282] + 1;
    }
    if (lbl_8087FA20 != 0 && ((TaskWithTac_unk*)lbl_8087FA20)->m258 != 0) {
        ((atn::Task*)((TaskWithTac_unk*)lbl_8087FA20)->m258)->MeltTask();
    }
    lbl_8087F490->MeltTask();
    ((atn::Task*)lbl_8087F048)->MeltTask();
    lbl_8087EE68->MeltTask();
    lbl_8087F8A0->fn_800D246C(1);
    EnemyManager::sInstance->fn_800D246C(1);
    NpcManager::sInstance->fn_800D246C(1);
    lbl_8087F9E8->MeltTask();
    lbl_8087F490->m263C->fn_803B57B0(1);
    lbl_8087F490->m2640->fn_803B6C88(1);
    if (lbl_8087F9F8 != 0 && lbl_8087F9F8->mA70 != 0) {
        fn_805AA734(lbl_8087F9F8);
        fn_803743AC(lbl_8087F430);
    }
    lbl_8087F540->MeltTask();
    GameBgm::sInstance->fn_8037D4C0(0, 60, 1, 0, 1.0f);
    if (stay == 0) {
        if (save != 0) {
            fn_8036F268(lbl_8087F430, 0, 0, 0, 0, 1, save, 0x12, 0);
        } else {
            fn_8036F268(lbl_8087F430, 0, 0, 0, 0, 1, task->m562C.mPtr->mData, 0x12, 0);
        }
    } else if (task->m10D8 != 0) {
        task->m10D8->MeltTask();
    }
}

// inferred: route data of the map table (as in game/NewCampPartsMapSelect.cpp)
struct MapRouteData_unk {
    unsigned char mMapType;
    unsigned char mMapId;
    unsigned char mMapSubId;
    int mEntry;
};
extern "C" MapRouteData_unk* fn_80216544(int id, int chapter, int flag);

// inferred: goes back to the map of the chapter (called by game/GameEventScript)
extern "C" void fn_80371164(GameTask* task) {
    int route;
    if (201000 <= task->m10D0 && task->m10D0 < 205000) {
        route = 0x193;
    } else if (205000 <= task->m10D0 && task->m10D0 < 301000) {
        route = 0x198;
    } else {
        route = 0xC9;
    }
    int special = 0;
    if (task->GetCurrentMap() != 0) {
        int type = task->GetCurrentMap()->mType;
        int id = task->GetCurrentMap()->mId;
        if (type == 2 && (id == 0x16 || id == 0x14)) {
            route = 0x138;
            special = 1;
        }
    }
    MapRouteData_unk* data = fn_80216544(route, task->m10D0, lbl_8087F430->mGlobalFlags[99]);
    if (data != 0) {
        task->m5668 = 1;
        fn_8036F268(task, data->mMapType, data->mMapId, data->mMapSubId, special ? 200 : data->mEntry, 1, 0, 0x12, 0);
    }
}

extern "C" void fn_803712BC(GameTask* task) {
    atn::FadeColor* fade = atn::FadeColor::Create((atn::Task*)task->m20, 5, 0xFF000000, 0xFF000000);
    fade->mActive = 1;
    fade->mDeleteAtEnd = 1;
    sSequenceBoot_unk->SetNext_unk("Title");
    fn_800D2338(task);
}

void GameTask::fn_80371320(PlayerTask* player) {
    if (m10E0 != 0) {
        m10E0->mLeader = player;
    }
}

void BattleTask::fn_80371664() {
    mCBAC = 0;
}

extern "C" void fn_80450B44(); // inferred: resumes the item slot animations
extern "C" void fn_800A5B30(void* pad, int arg); // inferred: pad reset (atn/Pad_unk.cpp)
extern "C" int VISetTimeToDimming(int type);

// inferred: ends the event of the field (field state 0; called by game/GameEventScript)
extern "C" void fn_80371674(GameTask* task) {
    task->m54E4 = 0;
    if (lbl_8087FA20 != 0 && ((TaskWithTac_unk*)lbl_8087FA20)->m258 != 0) {
        ((atn::Task*)((TaskWithTac_unk*)lbl_8087FA20)->m258)->MeltTask();
    }
    if (task->m5590 != 0) {
        fn_80570A14(task->m5590, 0);
        fn_80570A40(task->m5590, 0);
    }
    task->m5650 = 0;
    task->m5654 = 0;
    MenuCommonTask::sInstance->fn_804A2FBC();
    task->mCamera.m900 = task->mCamera.m900 % 2;
    task->mCamera.m904 = 0;
    task->mCamera.m908 = 0.0f;
    task->mCamera.m90C = 0.0f;
    task->m5674 = 0;
    lbl_8087F9E8->MeltTask();
    fn_80450B44();
    if (lbl_8087F498 != 0) {
        lbl_8087F498->fn_803EB4A8(0, 0);
    }
    HomeButtonMenu::sInstance->m88 = 1;
    fn_800A5B30(lbl_8087EF70, 5);
    VISetTimeToDimming(0);
}

void GameTask::fn_8037177C() {
    m5768 = 0;
    m5774 = 0;
    m5770 = 0;
}

void GameTask::fn_803727DC() {
    m54E4 = 0;
    if (m5590 != 0) {
        fn_80570A14(m5590, 0);
        fn_80570A40(m5590, 0);
    }
    if (lbl_8087F0A8->mD0 != 1) {
        fn_8056BD34(m558C.mPtr);
    }
    MotionBlurParam_unk blur = lbl_8087EFA8->m240;
    blur.mEnabled = 0;
    lbl_8087EFA8->m240 = blur;
    if (lbl_8087F0A8->mD0 == 1) {
        lbl_8087F8A0->fn_800D246C(0);
    } else {
        lbl_8087F8A0->MeltTask();
    }
    FriendManager::sInstance->fn_800D246C(0);
    EnemyManager::sInstance->MeltTask();
    m10D8->fn_800D246C(0);
    MenuCommonTask::sInstance->fn_804A2FBC();
    Inventory::instance_->fn_8044D028();
    if (Inventory::instance_->mDF30.mChara != 0) {
        Inventory::instance_->mDF30.mChara = 0;
        Inventory::instance_->mDF30.m90 = 0;
    }
    if (lbl_8087F490->mD8C == 1 && mGlobalFlags[207] != 0) {
        fn_8037F744(&mCamera);
        if (m56F4.mCount != 0) {
            fn_80376324(this, m56F4.Front_unk());
            m56F4.Pop_unk();
        }
        lbl_8087F490->fn_803E4478();
    }
}

// inferred: empty release asserts (rule of the autoinline threshold, as GHOUL_ASSERT_UNK in game/Em309Ghoul.cpp)
#define GT_ASSERT_UNK(x) ((void)0)

// inferred: multiplayer session (lbl_8087F610; layout from MP/D2AMpMenuResult.cpp); names invented
struct MpMember_unk {
    EnemyTask* mChara; // inferred: character of the member
    unsigned char m04[0xD5C - 4];
};
struct MpRoom_unk {
    unsigned char m00[8];
    atn::FixedArray_unk<MpMember_unk, 4> mMembers; // 0x08
};
struct MpArena_unk {
    unsigned char m0000[0x2B80];
    MpRoom_unk mRoom; // 0x2B80
    MpRoom_unk* GetRoom_unk() {
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0); GT_ASSERT_UNK(this != 0);
        return &mRoom;
    }
};
extern "C" void fn_804DA3F4(MpArena_unk* arena); // inferred: ends the session battle
extern "C" SkillEffectManager* lbl_8087F9E8;
// inferred: global accessors (weak copies of other files, called out of line by fn_803729B4)
extern "C" inline atn::EffectManager* fn_800F7FA0() {
    return lbl_8087F3C0;
}
extern "C" inline SkillEffectManager* fn_80356FFC() {
    return lbl_8087F9E8;
}
extern "C" inline RenderSetting_unk* fn_802F0990() {
    return lbl_8087EFA8;
}
extern "C" inline int fn_80267B20(CharaTask* chara) {
    return chara->m55C;
}
struct AimControl_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual void vf58();
    virtual void vf5C(); // inferred: stops aiming
};
extern "C" inline AimControl_unk* fn_80318F90(CharaTask* chara) {
    return (AimControl_unk*)chara->mControl.get();
}
extern "C" int fn_8004ED34(void* colli, void* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg);
// inferred: rotation matrix z, y, x (weak copy, as in game/Em203GreaterDaemon.cpp)
extern "C" inline atn::Matrix3x4 fn_800109E0(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.z) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.y) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (0.0f != rot.x) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    return m;
}
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
// inferred: translation matrix (weak copy 0x800F80A8 of game/BattleTask.cpp, as in game/TutorialTask.cpp)
extern "C" inline atn::Matrix3x4 fn_800F80A8(const atn::Vector3* pos) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, pos->x, pos->y, pos->z);
    return m;
}
inline float MtxColumn_unk(const atn::Matrix3x4& m, int i) {
    atn::Vector3 v(m.m[0][i], m.m[1][i], m.m[2][i]);
    return PSVECMag(&v);
}
inline float Max3_unk(const atn::Vector3& v) {
    float yz = v.y > v.z ? v.y : v.z;
    return v.x > yz ? v.x : (v.y > v.z ? v.y : v.z);
}
// inferred: world matrix of a model and its largest scale axis (weak copy 0x80316E38, as in
// game/Em316LaserSphere.cpp)
extern "C" inline void fn_80316E38(atn::Model* model, const atn::Matrix3x4* mtx) {
    __MtxCopy(model->mMtx.m, mtx->m);
    float sz = MtxColumn_unk(*mtx, 2);
    float sy = MtxColumn_unk(*mtx, 1);
    float sx = MtxColumn_unk(*mtx, 0);
    atn::Vector3 s(sx, sy, sz);
    model->m54 = Max3_unk(s);
}
inline BattleTask* fn_8013A194() {
    return lbl_8087F048;
}
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
extern "C" inline MpArena_unk* fn_800F7F90() {
    return (MpArena_unk*)lbl_8087F610;
}

extern "C" void PSMTXScale(Mtx m, float x, float y, float z);

// inferred: *this = *this * rot_y * rot_x * rot_z (weak copy; name invented)
inline void RotateYXZ_unk(atn::Matrix3x4& mtx, const atn::Vector3& rot) {
    atn::Matrix3x4 t;
    atn::Matrix3x4 m;
    Mtx rz;
    Mtx tz;
    Mtx rx;
    Mtx tx;
    Mtx ry;
    Mtx ty;
    atn::Matrix3x4* p = &m;
    p->SetIdentity();
    if (0.0f != rot.y) {
        PSMTXRotRad(ry, 'y', rot.y);
        PSMTXConcat(p->m, ry, ty);
        __MtxCopy(p->m, ty);
    }
    if (0.0f != rot.x) {
        PSMTXRotRad(rx, 'x', rot.x);
        PSMTXConcat(p->m, rx, tx);
        __MtxCopy(p->m, tx);
    }
    if (0.0f != rot.z) {
        PSMTXRotRad(rz, 'z', rot.z);
        PSMTXConcat(p->m, rz, tz);
        __MtxCopy(p->m, tz);
    }
    PSMTXConcat(mtx.m, p->m, t.m);
    mtx = t;
}

// inferred: *this = *this * scale(s) (weak copy, called by game/TutorialTask; name invented)
inline void Scale_unk(atn::Matrix3x4& mtx, const atn::Vector3& s) {
    atn::Matrix3x4 t;
    atn::Matrix3x4 sm;
    PSMTXScale(sm.m, s.x, s.y, s.z);
    PSMTXConcat(mtx.m, sm.m, t.m);
    mtx = t;
}

inline void atn::EffectManager::SetPaused_unk(int paused) {
    mPaused = paused;
}
inline CharaLook_unk* CharaTask::GetLook_unk() {
    return &mLook;
}
inline atn::Vector3* CharaTask::GetScale_unk() {
    return &m540;
}
inline void CharaTask::SetPosition_unk(const atn::Vector3& pos) {
    mPos = pos;
}
inline GameMessageWindow* GameUI::GetMessageWindow_unk() {
    return m263C;
}
inline GameTextMessageWindow* GameUI::GetTextWindow_unk() {
    return m2640;
}
inline void RenderSetting_unk::SetColorMatrix(const ColorMatrixParam_unk& p) {
    m324 = p;
}
inline DamageEffectParam_unk* RenderSetting_unk::GetDamage_unk() {
    return &m374;
}
inline void RenderSetting_unk::SetDamage_unk(DamageEffectParam_unk p) {
    m374 = p;
}
inline void RenderSetting_unk::SetMotionBlur_unk(int enabled) {
    m240.mEnabled = enabled;
}
inline void RenderSetting_unk::SetFrameSpeed_unk(float speed) {
    m3A4 = speed;
}
inline ColorMatrixParam_unk* RenderSetting_unk::GetColorMatrix_unk() {
    return &m324;
}

// NONMATCHING: draft - victory pose of the multiplayer battle (places the members around the leader and
// starts the finish camera); every inline is called out of line in the original (R3)
extern "C" void fn_803729B4(GameTask* task, CharaTask* chara) {
    if (chara == 0) {
        return;
    }
    fn_8013A194()->fn_801011B8();
    fn_80356FFC()->fn_8059C32C(0);
    fn_800F7FA0()->fn_800D246C(1);
    fn_800F7FA0()->SetPaused_unk(1);
    fn_800F7FA0()->fn_8023A60C(0);
    fn_800F7FA0()->fn_8023A60C(2);
    fn_800F7FA0()->fn_8023A60C(3);
    fn_800F7FA0()->fn_8023A60C(5);
    // inferred: m10D8 is read as the route table here (as in gmk/BulletPoint.cpp)
    RoutePlace_unk* place = ((RouteTable_unk*)task->m10D8)->FindPlace_unk(99991);
    atn::Vector3 base(*(place != 0 ? &place->mPos : chara->fn_8013C38C()));
    atn::Vector3 rot;
    if (place != 0) {
        rot.Set_unk(0.0f, place->m14, 0.0f);
    } else {
        rot = *chara->GetRot_unk();
        for (int i = 0; i < 4; i++) {
            atn::Vector3 from(base);
            from.y += 5.0f;
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            atn::Matrix3x4 m;
            m.fn_8013A13C(rot.y);
            dir.fn_80011410(m);
            atn::Vector3 to = from + dir * 20.0f;
            if (fn_8004ED34(fn_801404F8(), 0, &from, &to, 0x80000008, 0, 0) == 0) {
                break;
            }
            rot.y = atn::WrapAngle_unk(1.5707964f + rot.y);
        }
    }
    std::vector<EnemyTask*> list;
    MpRoom_unk* room = fn_800F7F90()->GetRoom_unk();
    atn::FixedArray_unk<MpMember_unk, 4>& members = room->mMembers;
    for (unsigned int i = 0; i < members.size(); i++) {
        if (members[i].mChara != 0) {
            list.push_back(members[i].mChara);
        }
    }
    task->mFinish.fn_803786F0();
    for (unsigned int i = 0; i < list.size(); i++) {
        CharaTask* c = (CharaTask*)list[i];
        c->GetStatus_unk()->fn_8012D8B8();
        c->fn_80178A6C();
        if (c->fn_8013C3A8()) {
            c->fn_801539E0();
        }
        if (c->fn_801479D8()) {
            c->fn_80155DAC(0);
        }
        if (fn_80267B20(c) == 6 && fn_80318F90(c) != 0) {
            fn_80318F90(c)->vf5C();
        }
        c->GetLook_unk()->fn_80129A48(1.0f);
        c->GetLook_unk()->fn_80129A6C();
        c->fn_801750FC();
        c->fn_8014DEE4(0, 0, 0);
        if (i == 0) {
            c->GetModel_unk()->fn_80097C08(0, 0x177, 0, 1, 1, 1.0f, 0.1f);
        } else {
            int motion = c->fn_801370B8(0);
            c->GetModel_unk()->fn_80097C08(0, motion, 0, 1, 1, 1.0f, 0.1f);
        }
        c->GetModel_unk()->fn_80139F24(1);
        c->GetModel_unk()->fn_8013C554(0, 1.0f);
        c->GetModel_unk()->SetMotionSpeed(0, 1.0f);
        atn::Vector3 off;
        if (i == 0) {
            off.Set_unk(0.0f, 0.0f, 0.0f);
        } else if ((i & 1) == 1) {
            off.Set_unk(8.0f * (float)((i - 1) >> 1) + 10.0f, 0.0f, i > 2 ? -14.0f : -8.0f);
        } else if ((i & 1) == 0) {
            off.Set_unk(-10.0f - 8.0f * (float)((i - 1) >> 1), 0.0f, i > 2 ? -14.0f : -8.0f);
        }
        atn::Vector3 pos = base + off.Transform_unk(fn_800109E0(rot));
        atn::Matrix3x4 mtx = fn_800F80A8(&pos);
        RotateYXZ_unk(mtx, rot);
        Scale_unk(mtx, *c->GetScale_unk());
        fn_80316E38(c->GetModel_unk(), &mtx);
        c->GetModel_unk()->fn_80097E80(1);
        c->GetModel_unk()->CalcMatrixStack(atn::ModelNodeCallback_unk(0));
        c->fn_80148B38(10.0f);
        c->GetModel_unk()->SetMotionFrame(0, 0.0f);
        c->FreezeTask();
        c->SetPosition_unk(pos);
        c->SetRot_unk(rot);
        task->mFinish.fn_8037865C(c, base, rot, 0);
    }
    task->mFinish.fn_8037C690((Xsca_unk*)task->m56DC.fn_804786F8(), 0);
    task->mFinish.fn_80378610(6, chara, atn::Vector3(0.0f, 0.0f, 0.0f));
    task->mFinish.fn_803786FC(0);
    task->mFinish.fn_803792F0();
    task->m54E4 = 10;
    GameUI::Get_unk()->GetMessageWindow_unk()->fn_803B57B0(1);
    GameUI::Get_unk()->GetTextWindow_unk()->fn_803B6C88(1);
    fn_8013A194()->fn_80371664();
    ((PopupTask*)fn_8036635C())->fn_8054E51C();
    fn_802F0990()->SetMotionBlur_unk(0);
    ColorMatrixParam_unk cm(*fn_802F0990()->GetColorMatrix_unk());
    cm.mMono = 0;
    fn_802F0990()->SetColorMatrix(cm);
    fn_802F0990()->SetFrameSpeed_unk(1.0f);
    DamageEffectParam_unk damage(*fn_802F0990()->GetDamage_unk());
    damage.mEnabled = 0;
    fn_802F0990()->SetDamage_unk(damage);
    fn_804DA3F4(fn_800F7F90());
}

// inferred: the original emits this copy (and its "vector length error") right after fn_803729B4
template void std::vector<EnemyTask*>::push_back(EnemyTask* const& x);

void GameTask::fn_80373118() {
    m5624->mDelay++;
    m5620->mDelay++;
    m5760++;
}

MapTask* GameTask::GetCurrentMap() {
    if (m10D8 != 0) {
        return m10D8->m64;
    }
    return 0;
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

// inferred: ends the replay (field state 0): resumes the field and restores the render settings
void GameTask::fn_803737C0() {
    m54E4 = 0;
    m5598 = 60;
    m5594 = 0;
    m55B0 = 0;
    m55B4 = 0;
    mFinish.fn_80378CCC();
    ((atn::Task*)lbl_8087F048)->MeltTask();
    lbl_8087F8A0->MeltTask();
    EnemyManager::sInstance->MeltTask();
    NpcManager::sInstance->MeltTask();
    FriendManager::sInstance->MeltTask();
    lbl_8087EE68->MeltTask();
    m10D8->fn_800D246C(0);
    GimmickManager::instance_->fn_803EE560();
    GameTimer::sInstance->MeltTask();
    GameUI* ui = lbl_8087F490;
    ui->mB08[0]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[0])->mFrame = 0.0f;
    ui->mB08[1]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[1])->mFrame = 0.0f;
    ui->mB08[2]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[2])->mFrame = 0.0f;
    ui->mB08[3]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[3])->mFrame = 0.0f;
    ui->mB08[4]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[4])->mFrame = 0.0f;
    ui->mB08[5]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[5])->mFrame = 0.0f;
    ((atn::D2A_Task*)ui->mB08[5])->SetLayoutValue_unk("iconOn", 1.0f);
    ui->mB20[0]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[0])->mFrame = 0.0f;
    ui->mB20[1]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[1])->mFrame = 0.0f;
    ui->mB20[2]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[2])->mFrame = 0.0f;
    ui->mB20[3]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[3])->mFrame = 0.0f;
    ui->mB20[4]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[4])->mFrame = 0.0f;
    ui->mB20[5]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[5])->mFrame = 0.0f;
    lbl_8087F490->fn_803E4950();
    lbl_8087F3C0->fn_800D246C(0);
    lbl_8087EFE8->fn_800CFA28(1, 0, 2, 30);
    if (GameBgm::sInstance != 0) {
        GameBgm::sInstance->fn_8037D49C(GameBgm::sInstance->mE0, 30);
    }
    fn_80570A14(m5590, 0);
    lbl_8087F9E8->fn_8059D268(1);
    if (lbl_8087F0A8->m090 != 0) {
        lbl_8087F048->fn_8010AB34();
    }
    lbl_8087EFA8->SetColorMatrix(m55C4);
    lbl_8087EFA8->m3A4 = m55C0;
}

void GameTask::fn_80373F78() {
    if (m5590 != 0 && fn_80570A4C(m5590) != 0) {
        fn_803731E8();
    }
}

extern "C" void fn_80450B28();         // inferred: freezes the item slot animations
extern "C" void fn_802328DC(int frame); // inferred: effect system frame

// inferred: opens the tactical view (field state 14): freezes the field and keeps the render settings
extern "C" void fn_80373FC4(GameTask* task) {
    task->m54E4 = 14;
    task->m55B8 = 2;
    task->mFinish.m394 = 1;
    task->mFinish.fn_80378CAC();
    ((atn::Task*)lbl_8087F048)->fn_800D246C(1);
    lbl_8087F8A8->FreezeTask();
    lbl_8087F8A0->fn_800D246C(1);
    EnemyManager::sInstance->fn_800D246C(1);
    NpcManager::sInstance->fn_800D246C(1);
    lbl_8087EE68->FreezeTask();
    task->m10D8->fn_800D246C(1);
    GimmickManager::instance_->fn_800D246C(1);
    fn_80450B28();
    GameTimer::sInstance->FreezeTask();
    GameUI* ui = lbl_8087F490;
    ui->mB08[0]->FreezeTask();
    ui->mB08[1]->FreezeTask();
    ui->mB08[2]->FreezeTask();
    ui->mB08[3]->FreezeTask();
    ui->mB20[1]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[1])->mFrame = 0.0f;
    ui->mB20[2]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[2])->mFrame = 0.0f;
    ui->mB20[3]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[3])->mFrame = 0.0f;
    ui->mB20[4]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[4])->mFrame = 0.0f;
    ui->mB20[5]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[5])->mFrame = 0.0f;
    lbl_8087F3C0->fn_800D246C(1);
    lbl_8087F3C0->fn_8023A60C(5);
    MenuCommonTask::sInstance->fn_804A2F98();
    lbl_8087EFE8->fn_800CFA28(1, 1, 2, 10);
    if (GameBgm::sInstance != 0) {
        float volume = GameBgm::sInstance->mReq.mVolume;
        GameBgm::sInstance->fn_8037D49C(0.5f * volume, 10);
    }
    lbl_8087EFA8->m240.mEnabled = 1;
    ColorMatrixParam_unk cm(*lbl_8087EFA8->GetColorMatrix_unk());
    task->m55C4 = cm;
    float speed = lbl_8087EFA8->m3A4;
    task->m55C0 = speed;
    lbl_8087EFA8->m3A4 = 1.0f;
    PlayerTask* leader = lbl_8087F8A0->mHead;
    atn::Vector3 pos = leader->mPos;
    atn::Vector3 rot = lbl_8087F8A0->mHead->mRot;
    task->mCamera.m8E4 = 1;
    task->mCamera.fn_80389838(6);
    task->mCamera.fn_803903E0(&pos, -rot.y, 1);
    fn_802328DC(60);
    lbl_8087F9E8->fn_8059D178();
    fn_802328DC(0);
    lbl_8087F3C0->mPaused = 0;
    lbl_8087F3C0->fn_800D246C(1);
    if (task->mFinish.m394 != 0) {
        lbl_8087F9F8->fn_805AA4E4();
    }
    fn_800C31F4("SE_UICOM_022", 0, -1, 1.0f);
    lbl_8087F490->m263C->FreezeTask();
    lbl_8087F490->m2640->FreezeTask();
    lbl_8087EFE8->fn_800CFBA0(8, 1, 2, 15);
}


// inferred: closes the tactical view: resumes the field and restores the render settings
extern "C" void fn_803743AC(GameTask* task) {
    task->m54E4 = 0;
    task->mFinish.fn_80378CCC();
    task->m55B8 = 0;
    task->m55BC = 0;
    ((atn::Task*)lbl_8087F048)->fn_800D246C(0);
    lbl_8087F8A8->MeltTask();
    lbl_8087F8A0->fn_800D246C(0);
    EnemyManager::sInstance->fn_800D246C(0);
    NpcManager::sInstance->fn_800D246C(0);
    lbl_8087EE68->MeltTask();
    task->m10D8->fn_800D246C(0);
    GimmickManager::instance_->fn_800D246C(0);
    fn_80450B44();
    GameTimer::sInstance->MeltTask();
    GameUI* ui = lbl_8087F490;
    ui->mB08[0]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[0])->mFrame = 0.0f;
    ui->mB08[1]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[1])->mFrame = 0.0f;
    ui->mB08[2]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[2])->mFrame = 0.0f;
    ui->mB08[3]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[3])->mFrame = 0.0f;
    ui->mB08[4]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[4])->mFrame = 0.0f;
    ui->mB08[5]->FreezeTask();
    ((atn::D2A_Task*)ui->mB08[5])->mFrame = 0.0f;
    ((atn::D2A_Task*)ui->mB08[5])->SetLayoutValue_unk("iconOn", 1.0f);
    ui->mB20[0]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[0])->mFrame = 0.0f;
    ui->mB20[1]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[1])->mFrame = 0.0f;
    ui->mB20[2]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[2])->mFrame = 0.0f;
    ui->mB20[3]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[3])->mFrame = 0.0f;
    ui->mB20[4]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[4])->mFrame = 0.0f;
    ui->mB20[5]->FreezeTask();
    ((atn::D2A_Task*)ui->mB20[5])->mFrame = 0.0f;
    lbl_8087F490->fn_803E4950();
    lbl_8087F3C0->fn_800D246C(0);
    lbl_8087EFE8->fn_800CFA28(1, 0, 2, 30);
    if (GameBgm::sInstance != 0) {
        GameBgm::sInstance->fn_8037D49C(GameBgm::sInstance->mE0, 30);
    }
    lbl_8087F9E8->fn_8059D268(1);
    MenuCommonTask::sInstance->fn_804A2FBC();
    lbl_8087F048->fn_801058F8();
    lbl_8087EFA8->SetMotionBlur_unk(0);
    lbl_8087EFA8->SetColorMatrix(task->m55C4);
    lbl_8087EFA8->m3A4 = task->m55C0;
    lbl_8087F490->m263C->MeltTask();
    lbl_8087F490->m2640->MeltTask();
    lbl_8087EFE8->fn_800CFBA0(8, 0, 2, 15);
    lbl_8087F0A8->mInput.fn_80124BE4();
    lbl_8087F0A8->mInput.m14[7] = 1; // inferred: locks action 7
}

// inferred: starts the grapple camera, toward the route place `id` when there is one
// NONMATCHING: 99.5% - registers of the inlined FindPlace_unk loop (index and table swapped; tried if/else, a helper)
void GameTask::fn_80374710(int id) {
    PlayerTask* leader = lbl_8087F8A0->mHead;
    mCamera.fn_80389838(6);
    atn::Vector3 pos = leader->mPos;
    float angle = -leader->mRot.y;
    if (id > 0) {
        // inferred: m10D8 is read as the route table here (as in gmk/BulletPoint.cpp)
        RouteTable_unk* table = (RouteTable_unk*)m10D8;
        RoutePlace_unk* place = table != 0 ? table->FindPlace_unk(id) : 0;
        if (place != 0) {
            pos = place->mPos;
            angle = -place->m14;
        }
        mCamera.m8E4 = 0;
    } else {
        mCamera.m8E4 = 1;
    }
    mCamera.fn_803903E0(&pos, angle, 1);
    mCamera.m8E8 = 0;
    if (GetCurrentMap() != 0 && GetCurrentMap()->mType == 2 && GetCurrentMap()->mId == 0xB &&
        GetCurrentMap()->mSubId == 2 && id == 6003) {
        mCamera.m8E8 = 1;
    }
    leader->m54C |= 0x10;
    lbl_8087F048->m367B0 = m561C;
    if (GameSoundControl::sInstance != 0) {
        GameSoundControl::sInstance->fn_803CDAE4();
    }
}

void GameTask::fn_803748E0(int arg) {
    mCamera.fn_80389838(mCamera.m804);
    mCamera.m8E8 = 0;
    lbl_8087F8A0->mHead->m54C &= ~0x10;
    lbl_8087F048->m367B0 = m5618;
    if (arg != 0 && GameSoundControl::sInstance != 0) {
        GameSoundControl::sInstance->fn_803CDB64();
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

// inferred: the dungeon of a map id is cleared (1 for other maps)
extern "C" int fn_803752F4(GameTask* task, const char* id) {
    if (id == 0 || strlen(id) < 5) {
        return 1;
    }
    if (strncmp(id, "dg", 2) == 0) {
        int map = atoi(id + 2);
        const DungeonFlag_unk* flag = FindDungeonFlag_unk((id));
        if (flag == 0) {
            ChapterEntry_unk* chapter = fn_802085E0(2, map);
            if (chapter != 0 && chapter->mNext != 0) {
                return task->m10D0 >= chapter->mNext->m04;
            }
        } else {
            return fn_80370174(task, flag->mFlag) >= flag->mValue;
        }
    }
    return 1;
}

int GameTask::fn_803754C4() {
    atn::Task* task = m567C;
    if (task != 0 && !task->IsFlag_unk(4)) {
        return 1;
    }
    return 0;
}

// inferred: dungeons that do not count (lbl_8074DB70)
extern const int sDungeonExcluded_unk[];

static inline int IsDungeonExcluded_unk(const char* id) {
    int dungeon = GetDungeonId_unk(id);
    for (const int* p = sDungeonExcluded_unk; *p > 0; p++) {
        if (dungeon == *p) {
            return 1;
        }
    }
    return 0;
}

// inferred: the current map is a cleared dungeon (not the one of type 2, map 9)
// NONMATCHING: 95% - the address of the dungeon tables is kept in r30 (the original reloads it at each use)
extern "C" int fn_803754F0(GameTask* task) {
    if (task->m5694 != 0 && task->GetCurrentMap() != 0) {
        int type = task->GetCurrentMap()->mType;
        int id = task->GetCurrentMap()->mId;
        int sub = task->GetCurrentMap()->mSubId;
        if (fn_8037587C(task, type, id, sub)) {
            if (type == 2 && id == 9) {
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

// inferred: the map is a cleared dungeon that counts (the map was visited)
extern "C" int fn_8037587C(GameTask* task, int type, int id, int sub) {
    int cleared = 0;
    MapId_unk* map = fn_8021771C(type, id, sub);
    if (map != 0 && map->mType == 2) {
        cleared = task->IsDungeonCleared_unk(type, id, sub);
        if (cleared) {
            cleared = !IsDungeonExcluded_unk((map->mId));
        }
    }
    return cleared;
}

// inferred: global flag holding the level of some dungeons (lbl_8074DB88)
struct DungeonLevel_unk {
    int mDungeon;
    int mFlag;
};
extern const DungeonLevel_unk sDungeonLevels_unk[];

static inline int GetDungeonLevelFlag_unk(int dungeon) {
    for (int i = 0; sDungeonLevels_unk[i].mDungeon > 0; i++) {
        if (dungeon == sDungeonLevels_unk[i].mDungeon) {
            return sDungeonLevels_unk[i].mFlag;
        }
    }
    return -1;
}

// inferred: level of the dungeon of the field (0 outside dungeons)
extern "C" int fn_80375B78(GameTask* task, int arg) {
    int level = 0;
    MapId_unk* map = fn_8021771C(task->mOption.mType, task->mOption.mMap, task->mOption.mSub);
    if (map != 0 && map->mType == 2) {
        int flag = GetDungeonLevelFlag_unk(GetDungeonId_unk(map->mId));
        if (flag > 0) {
            level = fn_80370174(task, flag);
        } else if (task->mGlobalFlags[281] > 0) {
            level = task->mGlobalFlags[281];
        } else if (arg != 0) {
            level = map->mBC;
        }
        if (lbl_8087F0A8->m5A0 == 1 && arg != 0) {
            level -= task->mGlobalFlags[282];
        }
    }
    return level;
}

// inferred: level cap of the field (chapter level scaled by the difficulty, plus the dungeon bonus)
// NONMATCHING: 99.7% - the map type and id of the inlined fn_803754F0 get r28/r29 swapped
extern "C" int fn_80375D0C(GameTask* task) {
    int cap = 0;
    ChapterEntry_unk* chapter = lbl_8087F430->m10D4;
    if (fn_803754F0(task)) {
        chapter = fn_80208694(lbl_8087F430->m10D0);
    }
    if (chapter != 0) {
        cap = (int)((float)chapter->m54 * lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m00 + 0.2f);
        cap += lbl_8087F0A8->mD8[lbl_8087F0A8->mD4].m04;
    }
    MapId_unk* map = fn_8021771C(task->mOption.mType, task->mOption.mMap, task->mOption.mSub);
    if (map != 0 && map->mType == 2) {
        cap += map->mBC;
    }
    return cap;
}

int GameTask::fn_80376150() {
    MapTask* map;
    if (m10D8 == 0 || (map = m10D8->m64) == 0) {
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

// inferred: starts the blur effect of the field (vf10 counts it down); called by game/Em309Ghoul
// NONMATCHING: 97.4% - r4/r8 swapped (render settings pointer and the copied blur)
extern "C" void fn_80376254(GameTask* task, int delay, int frames) {
    task->m569C = delay;
    task->m56A4 = frames;
    if (delay <= 0) {
        MotionBlurParam_unk blur = lbl_8087EFA8->m240;
        task->m56A8 = blur;
        blur.mEnabled = 1;
        lbl_8087EFA8->m240 = blur;
        float speed = lbl_8087EFA8->m3A4;
        task->m56CC = speed;
        lbl_8087EFA8->m3A4 = 0.2f;
    }
}

// inferred: shows the help `id` (queued while another help is shown)
// NONMATCHING: 99% - the original reloads the queue count after loading its head
extern "C" void fn_80376324(GameTask* task, int id) {
    TutorialData_unk* data = fn_8021F09C(id);
    if (data == 0) {
        return;
    }
    if (data->m04 != 0) {
        if (task->m56F0 != 0 || task->m56EC != 0) {
            if ((unsigned int)task->m571C.mCount < 8) {
                task->m571C.Push_unk(id);
            }
        } else {
            task->m56F0 = TutorialPopup::fn_805BC6C8(task, id, lbl_8087F8A0->mHead);
            if (id == 0x8E && fn_80370174(task, id) > 1) {
                task->m56F0->mSpeed = 1.5f;
            }
            if (lbl_8087F540->fn_80481668()) {
                task->m56F0->fn_800D246C(1);
            }
        }
    } else {
        if (task->m56F0 != 0) {
            fn_800D2338(task->m56F0);
            task->m56F0 = 0;
            task->m571C.mCount = 0;
        }
        if (task->m5590 != 0) {
            fn_80570A14(task->m5590, 1);
        }
        task->m558C.mPtr->fn_8056B3D4();
        lbl_8087F8A0->fn_800D246C(1);
        FriendManager::sInstance->fn_800D246C(1);
        EnemyManager::sInstance->fn_800D246C(1);
        task->m10D8->fn_800D246C(1);
        lbl_8087F3C0->fn_800D246C(1);
        lbl_8087F490->m263C->FreezeTask();
        lbl_8087F490->m2640->FreezeTask();
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->fn_800CFBA0(8, 1, 2, 15);
        }
        task->m54E4 = 13;
        task->m56EC = TutorialTask::fn_805B9F50(task, id, lbl_8087F8A0->mHead);
        if (id == 0xA7 && fn_80370174(task, 0x13F) == 0) {
            task->fn_80370320(0x13F, 1, 0);
        }
    }
}

// inferred: ends the tutorial of the field and shows the next waiting one
void GameTask::fn_80376568() {
    if (m5590 != 0) {
        fn_80570A14(m5590, 0);
        fn_80570A40(m5590, 0);
    }
    fn_8056BD34(m558C.mPtr);
    lbl_8087F8A0->fn_800D246C(0);
    FriendManager::sInstance->fn_800D246C(0);
    EnemyManager::sInstance->fn_800D246C(0);
    m10D8->fn_800D246C(0);
    lbl_8087F3C0->fn_800D246C(0);
    lbl_8087F490->m263C->MeltTask();
    lbl_8087F490->m2640->MeltTask();
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->fn_800CFBA0(8, 0, 2, 15);
    }
    m54E4 = 0;
    fn_800D2338(m56EC);
    m56EC = 0;
    atn::Keyboard* keyboard = atn::Keyboard::instance_;
    if (keyboard != 0) {
        int pressed = 0;
        // inferred: the original passes the keyboard itself as the key state (mKeyState[-2])
        if (keyboard->mD90 != 0 && keyboard->CheckKeyState(keyboard->mKeyState[-2], 0xF1B1) != 0) {
            pressed = 1;
        }
        if (pressed) {
            m56F4.mCount = 0;
        }
    }
    if (m56F4.mCount != 0) {
        fn_80376324(this, m56F4.Front_unk());
        m56F4.Pop_unk();
    }
}

void GameTask::fn_803766E4() {
    if (m56F0 != 0) {
        fn_800D2338(m56F0);
        m56F0 = 0;
    }
    m571C.mCount = 0;
}

// inferred: auto save of the field into the copy it started from (called by GameEventScript and
// gmk::AttractHatePoint); counts the saves in global flag 0x8E
extern "C" void fn_80376730(GameTask* task) {
    if (lbl_8087F0A8->m194 == 0 && task->m5694 == 0 && task->m5748 == 0) {
        MP::SaveWork_unk* work = task->m562C.mPtr;
        if (work != 0 && task->m5634->m154 == 0) {
            ((GameSaveData_unk*)work->mData)->fn_803BDEE4();
            SaveTime_unk time;
            fn_8004B20C(&time);
            ((GameSaveSlot_unk*)work)->fn_803BE8C0((GameSaveData_unk*)work->mData, &time);
            if (task->m5630 != 0) {
                task->m5630 = 0;
            }
            task->m5634->fn_803B8E60(task->m562C.mPtr, 0, 1);
            task->fn_80370320(0x8E, task->mGlobalFlags[0x8E] + 1, 0);
        }
    }
}

// inferred: dungeon tables (.rodata 0x8074DA2C-0x8074DBC0), defined after their users
const DungeonFlag_unk sDungeonFlags_unk[] = {
    {902, 15, 100}, {912, 15, 100}, {1000, 16, 100}, {7100, 16, 100}, {1700, 17, 100}, {1800, 18, 100},
    {1100, 19, 100}, {700, 20, 100}, {300, 21, 100}, {2300, 22, 100}, {905, 23, 100}, {915, 23, 100},
    {906, 24, 100}, {916, 24, 100}, {4200, 25, 100}, {100, 26, 100}, {-1, -1, -1},
};

const int sDungeonIds_unk[] = {
    400, 800, 902, 912, 1000, 7100, 4800, 600, 1700, 1800, 1900, 1100, 700, 300, 2000,
    2200, 2300, 905, 915, 3200, 6300, 2500, 2600, 906, 916, 4200, 6700, 100, -1, 0,
};

const int sDungeonExcluded_unk[] = {800, 4800, 600, 300, 3200, -1};

const DungeonLevel_unk sDungeonLevels_unk[] = {
    {902, 203}, {912, 203}, {905, 204}, {915, 204}, {906, 205}, {916, 205}, {-1, -1},
};

// inferred: counts down the pause of the field (m58 frames before it stops, m54 frames of pause)
void GamePauseTask::vf10() {
    if (m58 > 0) {
        if (--m58 == 0) {
            mGame->fn_800D246C(1);
            lbl_8087F3C0->fn_800D246C(1);
        }
    }
    if (m54 > 0) {
        m54--;
        if (m5C) {
            m5C(m54);
        }
        if (m54 == 0) {
            mGame->fn_800D246C(0);
            lbl_8087F3C0->fn_800D246C(0);
        }
    }
}

GamePauseTask::~GamePauseTask() {}
