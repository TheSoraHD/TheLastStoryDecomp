// game/GameEventScript.cpp (0x803935AC-0x803B27F8): event script unit (GameEventScript,
// boost::bad_lexical_cast and the weak copies of std::bad_cast and the MSL string streams).
// The former GameEventScript_cont/_cont2 and std/bad_cast/_what splits are merged here.
#include "atn/SoundManager.h"
#include "game/GameTask.h"
#include "atn/SoundRef.h"
#include "game/GameEventScript.h"
#include "game/GameField_unk.h"
#include "game/GameChoiceWindow.h"
#include "game/GameUI.h"
#include "Event/Manager.h"
#include "atn/Layer3D.h"
#include "atn/RenderManager.h"
#include "atn/SceneColorMatrix_unk.h"
#include "game/GameScene.h"
#include "game/PlayerManager.h"
#include "game/TeamAIManager.h"
#include "game/SetConfig.h"
#include "game/GameBgm.h"
#include "game/CrowdManager.h"
#include "game/MapTask.h"
#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
#include "game/Inventory.h"
#include "game/GameTask.h"
#include "game/NpcManager.h"
#include "game/EnemyManager.h"
#include "game/NpcTask.h"
#include "game/EnemyTask.h"
#include "game/PlayerTask.h"

#include <exception>

namespace std {

// inferred: MSL <typeinfo>; std::bad_cast derives from std::exception
class bad_cast : public exception {
public:
    virtual ~bad_cast();
    virtual const char* what() const;
};

} // namespace std

namespace boost {

// inferred: Boost 1.35 lexical_cast.hpp (the source/target type_info pointers are not used here)
class bad_lexical_cast : public std::bad_cast {
public:
    virtual ~bad_lexical_cast();
    virtual const char* what() const;
};

} // namespace boost

extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
class TownTask;
extern "C" TownTask* lbl_8087FA20;
extern "C" const char* const lbl_8074EF78[];
extern "C" void fn_8036F268(GameField_unk* field, int type, int id, int subId, int entry, int a, int b, int reason,
                            int arg9); // inferred: changes the map (game/NewCampPartsMapSelect.cpp)
extern "C" void fn_80371164(GameField_unk* field);
extern "C" void fn_803712BC(GameField_unk* field);
extern "C" void fn_80374964(GameField_unk* field, int arg);
extern "C" void fn_80376730(GameField_unk* field); // inferred: autosave
extern "C" void fn_80370094(GameField_unk* field, int state); // inferred: sets m10D0
extern "C" void fn_8037EF30(void* camera, int a, int b, float f);
extern "C" int lbl_8087D990; // inferred: last save slot (-1: none)
extern "C" int fn_804444E8(Inventory* inv, int id); // inferred: number of items

// inferred: save menu (game/CampSaveLoad.cpp)
class MenuSaveTask {
public:
    static MenuSaveTask* sInstance;
};
extern "C" void fn_80375194(GameField_unk* field, int a, int b, float c);
extern "C" PlayerManager* lbl_8087F8A0; // inferred: UI sound names (SE_UICOM_001...)

inline CharaTask* GameEventScript::GetChara_unk(int kind, int id) const {
    CharaTask* chara = 0;
    switch (kind) {
    case 0:
        chara = mDC != 0 ? lbl_8087F8A0->mHead : lbl_8087F8A0->FindByChara_unk(1);
        break;
    case 1:
        chara = NpcManager::sInstance->Find_unk(id);
        if (lbl_8087FA20 != 0 && chara != 0 && chara->IsFlag_unk(1)) {
            chara = 0;
        }
        break;
    case 2:
        chara = EnemyManager::sInstance->Find_unk(id);
        break;
    case 3:
        chara = lbl_8087F8A0->Find_unk(id);
        break;
    }
    return chara;
}

// inferred: plays a sound with the sound category of the event scripts (5); called from the
// script commands
extern "C" atn::SoundRef fn_803935AC(const char* name, float volume) {
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34D0 = 5;
    }
    atn::SoundRef ref = fn_800C31F4(name, 0, -1, volume);
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34D0 = 0;
    }
    return ref;
}

// inferred: plays one of the UI sounds of the table
extern "C" atn::SoundRef fn_803935FC(int index, float volume) {
    return fn_803935AC(lbl_8074EF78[index], volume);
}

std::bad_cast::~bad_cast() {}

boost::bad_lexical_cast::~bad_lexical_cast() {}

void GameEventScript::fn_8039D178(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    fn_8039BF04(args->m08, args->m0C, args->m18, args->m10, args->m14);
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039D1E8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    mB4 = 0;
    CharaTask* leader = lbl_8087F8A0->mHead;
    if (static_cast<const ScriptArgs_unk*>(cmd)->m08 == 0 && mD0 != 0) {
        leader->fn_8016E970(0);
    }
    result.SetNext_unk(cmd, 1);
}

// inferred: fades the screen (0, 2: out; 1, 3: in)
void GameEventScript::fn_8039D3E0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    switch (args->m08) {
    case 0:
        ((GameTask*)lbl_8087F430)->fn_80370B78(args->m0C, args->m10, 0, -9.0f);
        break;
    case 2:
        ((GameTask*)lbl_8087F430)->fn_80370B78(args->m0C, args->m10, 0, -1.0f);
        break;
    case 1:
        ((GameTask*)lbl_8087F430)->fn_80370BD0(args->m10, 0, -9.0f);
        break;
    case 3:
        ((GameTask*)lbl_8087F430)->fn_80370BD0(args->m10, 0, -1.0f);
        break;
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039D930(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    fn_8036F268(lbl_8087F430, args->m08, args->m0C, args->m10, args->m14, args->m18, 0, 0x12, 0);
    result = ScriptResult_unk(0, 1);
}

void GameEventScript::fn_8039D99C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 1);
}

void GameEventScript::fn_8039D9B0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    ((GameTask*)lbl_8087F430)->fn_80370C6C();
    lbl_8087F8A0->mHead->fn_8016E970(0);
    result = ScriptResult_unk(0, 1);
}

void GameEventScript::fn_8039DA00(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 0);
}

void GameEventScript::fn_8039DA10(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

// inferred: shows a message of the scene in the message window and waits
// inferred: shows a message of the scene in the message window (bit 0 of m0C: do not wait)
void GameEventScript::fn_8039DA38(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* leader = lbl_8087F8A0->mHead;
    GameMessage* msg = (GameMessage*)mScene->fn_803CC6B4(args->m08);
    GameMessageWindow* window = lbl_8087F490->m263C;
    if (msg != 0) {
        if (window->IsShown_unk()) {
            window->fn_803B5830(msg, 0);
        } else {
            window->fn_803B5774(msg, 0);
        }
        mB4 = 0;
        if (((unsigned int)args->m0C & 1) != 1) {
            mA8 = 1;
            leader->fn_8016E970(5);
            result.mWait = 1;
        } else {
            result.mWait = 1;
        }
    } else {
        result.mWait = 0;
    }
    result.mNext = NextCmd_unk(cmd);
}

void GameEventScript::fn_8039E280(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    GameMessageWindow* window = lbl_8087F490->m263C;
    GameMessage* msg = (GameMessage*)mScene->fn_803CC6B4(static_cast<const ScriptArgs_unk*>(cmd)->m08);
    CharaTask* leader = lbl_8087F8A0->mHead;
    if (msg != 0) {
        window->fn_803B58D8(msg);
        leader->fn_8016E970(5);
        mB4 = 0;
        mA8 = 1;
        result.mWait = 1;
    } else {
        result.mWait = 0;
    }
    result.mNext = NextCmd_unk(cmd);
}

void GameEventScript::fn_8039E80C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039E834(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result.mNext = FindLabel_unk(static_cast<ScriptArgs_unk*>(cmd)->m08);
    result.mWait = 0;
}

void GameEventScript::fn_8039E9C4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 0);
}

void GameEventScript::fn_8039E9D4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087F8A0->mHead->mFlagB2 = 1;
    lbl_8087F8A0->mHead->fn_80155DAC(1);
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039EA50(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087F8A0->mHead->mFlagB2 = 0;
    lbl_8087F8A0->mHead->fn_80155DAC(0);
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039F7D8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    fn_80370094(lbl_8087F430, static_cast<const ScriptArgs_unk*>(cmd)->m08);
    mF0 = 1;
    ((GameTask*)lbl_8087F430)->fn_80370320(0x63, 0, 0);
    ((GameTask*)lbl_8087F430)->fn_80370320(0x11D, 0, 0);
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039F878(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    fn_8039F9A4(GetChara_unk(args->m08, args->m0C), args);
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A03A0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

// inferred: runs an event with a value of a gimmick (state or vf64)
void GameEventScript::fn_803A03C8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    for (GimmickCommon* gimmick = GimmickManager::instance_->mHead; gimmick != 0; gimmick = gimmick->m5C) {
        if (args->m08 == gimmick->mId && args->m0C == gimmick->m4C) {
            int mode = args->m10;
            int value = 0;
            switch (mode) {
            case 0:
                value = gimmick->m54;
                break;
            case 1:
            case 2:
            case 3:
                value = gimmick->vf64(mode - 1);
                break;
            }
            fn_8039BF04(args->m14, args->m18, 0, 0, value);
            break;
        }
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A04C0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    mScene->fn_803CAABC();
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0520(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    fn_8039FBFC(GetChara_unk(args->m08, args->m0C), args);
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0AB8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnTarget_unk(static_cast<const TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0B1C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnTarget2_unk(static_cast<const TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0B80(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnParent_unk(static_cast<const TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0BE4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnMembers_unk(static_cast<const TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0C48(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnMsg20_unk(static_cast<const TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0CAC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnMsg21_unk(static_cast<const TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0D10(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnChara_unk(static_cast<const TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

// inferred: requests a BGM (unless the same one is already requested)
// NONMATCHING: 96% - load order of m10
void GameEventScript::fn_803A2154(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgsF_unk* args = static_cast<const ScriptArgsF_unk*>(cmd);
    GameBgm* bgm = GameBgm::sInstance;
    if (bgm != 0) {
        if (args->m08 > 0 || (bgm->m84 != 0 ? bgm->mNext.mId : bgm->mReq.mId) != 0x67) {
            if (((GameTask*)lbl_8087F430)->fn_803761FC() == 0) {
                GameBgm::sInstance->fn_8037D4C0(args->m08, *(int*)&args->m10, 1, 0, args->m0C);
            }
        }
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 97% - the wait flag is stored before the table lookup
void GameEventScript::fn_803A2378(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    int wait = 0;
    if (chara != 0 && chara->m55C == 6 && chara->m560 == 0x27) {
        wait = 1;
    }
    result.SetNext_unk(cmd, wait);
}

// NONMATCHING: 87% - the original keeps the character and the value in r5/r4 across fn_8014C468
void GameEventScript::fn_803A330C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara != 0) {
        int value = args->m10;
        if (value == 0) {
            chara->fn_8014C468();
        }
        chara->fn_8014C0B4(0, value + 0xD5);
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A362C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A3654(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    int wait = 0;
    if (lbl_8087F430 != 0) {
        fn_80374964(lbl_8087F430, args->m08);
        if (args->m08 == 3) {
            wait = 1;
        }
    }
    result.SetNext_unk(cmd, wait);
}

// inferred: waits while the field is in an event state (9, 10, 13)
void GameEventScript::fn_803A36D8(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    int wait = 0;
    if (lbl_8087F430 != 0 &&
        (lbl_8087F430->m54E4 == 9 || lbl_8087F430->m54E4 == 10 || lbl_8087F430->m54E4 == 13)) {
        wait = 1;
    }
    result.SetNext_unk(cmd, wait);
}

// inferred: autosave point (when autosave is enabled)
// inferred: sets the camera of the current map
void GameEventScript::fn_803A3BF4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    MapTask* map = lbl_8087F430 != 0 ? ((GameTask*)lbl_8087F430)->GetCurrentMap() : 0;
    if (map != 0) {
        map->fn_804A04AC(args->m08, args->m0C != 0 ? 10 : 0, -1, 0);
        map->mDefaultCamera = args->m08;
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A3CA8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0 && SetConfig::sInstance->mAutoSave != 0) {
        fn_80376730(lbl_8087F430);
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A4400(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    if (lbl_8087F430 != 0) {
        fn_80375194(lbl_8087F430, args->m1C, args->m14, *(float*)&args->m18);
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A4474(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    if (args->m0C == 0) {
        int count = fn_804444E8(Inventory::instance_, args->m08);
        fn_8039BF04(args->m10, args->m14, 0, 0, count);
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 77% - register allocation; the original loads m08 before the first store
void GameEventScript::fn_803A450C(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgsF_unk* args = static_cast<ScriptArgsF_unk*>(cmd);
    ScriptCmd_unk* next = NextCmd_unk(cmd);
    GameCamera& camera = lbl_8087F430->mCamera;
    camera.m900 = camera.m900 % 2;
    camera.m904 = args->m08;
    camera.m908 = args->m0C;
    camera.m90C = args->m10;
    result.mNext = next;
    result.mWait = 0;
}

// inferred: waits while the message window is open
// inferred: waits 30 frames, then sets the battle mode of a team
void GameEventScript::fn_803A49F4(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    int wait = 1;
    if (++mB4 >= 30) {
        ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
        lbl_8087FA00->Find_unk(args->m08, args->m0C)->SetBattleMode_unk(1);
        wait = 0;
    }
    result.SetNext_unk(cmd, wait);
}

void GameEventScript::fn_803A59D4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    GameTextMessageWindow* window = lbl_8087F490->m2640;
    result.mNext = NextCmd_unk(cmd);
    result.mWait = window->mState != 0;
}

// NONMATCHING: 98% - registers r0/r6 swapped (type/zero), as fn_803A61CC
void GameEventScript::fn_803A5F14(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    atn::IRenderLayer* layer = 0;
    switch (args->m08) {
    case 0:
        layer = atn::RenderManager::instance_->mLayers[4];
        break;
    case 1:
        layer = atn::RenderManager::instance_->mLayers[3];
        lbl_8087EFA8->m104.mEnabled = args->m0C;
        break;
    }
    if (layer != 0) {
        layer->m04 = args->m0C;
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 98% - registers r0/r6 swapped (type/zero), as fn_803A61CC
void GameEventScript::fn_803A5F88(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (leader != 0) {
        if (static_cast<const ScriptArgs_unk*>(cmd)->m08 == 0) {
            leader->m54C |= 0x10;
        } else {
            leader->m54C &= ~0x10;
        }
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 98% - registers r0/r6 swapped (type/zero)
void GameEventScript::fn_803A61CC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

// NONMATCHING: 98% - registers r0/r6 swapped (type/zero), as fn_803A61CC
void GameEventScript::fn_803A61F4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0) {
        lbl_8087F430->m5664 = static_cast<const ScriptArgs_unk*>(cmd)->m08;
    }
    result.SetNext_unk(cmd);
}

// inferred: enables or disables the crowd of NPCs
// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
void GameEventScript::fn_803A66B0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    int enable = static_cast<const ScriptArgs_unk*>(cmd)->m10;
    if (CrowdManager::sInstance != 0) {
        if (enable != 0) {
            CrowdManager::sInstance->MeltTask();
        } else {
            CrowdManager::sInstance->FreezeTask();
        }
        CrowdManager::sInstance->fn_801EB8C0(enable);
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A6F68(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A7BB0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0) {
        fn_8037EF30(&lbl_8087F430->mCamera, static_cast<const ScriptArgs_unk*>(cmd)->m08, 1, -1.0f);
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 98% - registers r0/r4 swapped (type/zero), as fn_803A61CC
void GameEventScript::fn_803A83FC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0) {
        ((GameTask*)lbl_8087F430)->fn_80373F78();
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A8464(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    mB8 = 0;
    fn_803A8470(result, cmd);
}

void GameEventScript::fn_803A90E8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    fn_803712BC(lbl_8087F430);
    lbl_8087F8A0->mHead->fn_8016E970(0);
    result = ScriptResult_unk(0, 1);
}

void GameEventScript::fn_803A9138(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    mB8 = 0;
    mC0 = 60;
    fn_803A914C(result, cmd);
}

void GameEventScript::fn_803A9558(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    mB8 = 0;
    mC0 = 60;
    fn_803A956C(result, cmd);
}

// NONMATCHING: 98% - registers r0/r6 swapped in the next command (as fn_803A61CC)
void GameEventScript::fn_803A98C4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    unsigned int flags = static_cast<const ScriptArgs_unk*>(cmd)->m14;
    mB8 = 0;
    int n = 25;
    if ((flags & 4) == 4) n = 45;
    mC0 = n;
    fn_803A98EC(result, cmd);
}

// inferred: waits while the field is in state 5
void GameEventScript::fn_803AA82C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara != 0) {
        chara->mFlagB20 = 1;
        chara->mMotion.fn_8008B978(0);
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803AA954(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara != 0) {
        chara->mFlagB22 = 1;
        chara->mFlagB20 = 0;
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803AAA74(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0) {
        ((GameTask*)lbl_8087F430)->fn_8036E430(2, static_cast<const ScriptArgs_unk*>(cmd)->m08);
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803AAD58(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    lbl_8087F490->fn_803E44C4((args->m0C & 1) ^ 1, args->m08, 45);
    result.SetNext_unk(cmd);
}

// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
void GameEventScript::fn_803AB144(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    ((GameTask*)lbl_8087F430)->fn_8036E994(args->m08, args->m0C, args->m10);
    result.SetNext_unk(cmd, 1);
}

void GameEventScript::fn_803AB1B0(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    int wait = 1;
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 != 5) {
        wait = 0;
    }
    result.SetNext_unk(cmd, wait);
}

// inferred: opens the save menu
// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
void GameEventScript::fn_803AB1F4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    ((GameTask*)lbl_8087F430)->fn_80370320(0x391, static_cast<const ScriptArgs_unk*>(cmd)->m14, 0);
    ((GameTask*)lbl_8087F430)->fn_8036E92C(1);
    result.SetNext_unk(cmd, 1);
}

// inferred: runs the save event (unless the save menu is open)
void GameEventScript::fn_803AB26C(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    int wait = 1;
    if (MenuSaveTask::sInstance == 0) {
        if (lbl_8087D990 >= 0) {
            fn_8039BF04(args->m0C, args->m10, 0, 0, 1);
        } else {
            fn_8039BF04(args->m0C, args->m10, 0, 0, 0);
        }
        ((GameTask*)lbl_8087F430)->fn_80370320(0x391, 0, 0);
        wait = 0;
    }
    result.SetNext_unk(cmd, wait);
}

void GameEventScript::fn_803AB328(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    fn_80371164(lbl_8087F430);
    result.mWait = 1;
    result.mNext = 0;
}

void GameEventScript::fn_803AB368(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 1);
}

// inferred: ends the event started by the script on the event manager
void GameEventScript::fn_803ACD08() {
    if (mE8 > 0) {
        lbl_8087F540->fn_8047F580();
        mE8 = 0;
        mEC = 0;
    }
}

extern "C" const char lbl_8078B34C[];
extern "C" const char lbl_8078B394[];

const char* boost::bad_lexical_cast::what() const { return lbl_8078B34C; }

const char* std::bad_cast::what() const { return lbl_8078B394; }
