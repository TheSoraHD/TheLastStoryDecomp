// game/GameEventScript.cpp (0x803935AC-0x803B27F8): event script unit (GameEventScript,
// boost::bad_lexical_cast and the weak copies of std::bad_cast and the MSL string streams).
// The former GameEventScript_cont/_cont2 and std/bad_cast/_what splits are merged here.
#include "atn/SoundManager.h"
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
extern "C" const char* const lbl_8074EF78[];
extern "C" void fn_8036F268(GameField_unk* field, int type, int id, int subId, int entry, int a, int b, int reason,
                            int arg9); // inferred: changes the map (game/NewCampPartsMapSelect.cpp)
extern "C" void fn_80370C6C(GameField_unk* field);
extern "C" void fn_80371164(GameField_unk* field);
extern "C" void fn_803712BC(GameField_unk* field);
extern "C" void fn_80374964(GameField_unk* field, int arg);
extern "C" void fn_80375194(GameField_unk* field, int a, int b, float c);
extern "C" void fn_8036E430(GameField_unk* field, int a, int b);
extern "C" void fn_8036E994(GameField_unk* field, int a, int b, int c);
extern "C" void fn_80373F78(GameField_unk* field);
extern "C" PlayerManager* lbl_8087F8A0; // inferred: UI sound names (SE_UICOM_001...)

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

// NONMATCHING: 92% - the original loads m18 before the other arguments
void GameEventScript::fn_8039D178(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    fn_8039BF04(args->m08, args->m0C, args->m18, args->m10, args->m14);
    result.SetNext_unk(cmd);
}

// NONMATCHING: 75% - argument load order
void GameEventScript::fn_8039D930(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    fn_8036F268(lbl_8087F430, args->m08, args->m0C, args->m10, args->m14, args->m18, 0, 0x12, 0);
    result = ScriptResult_unk(0, 1);
}

void GameEventScript::fn_8039D99C(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 1);
}

void GameEventScript::fn_8039D9B0(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    fn_80370C6C(lbl_8087F430);
    lbl_8087F8A0->mHead->fn_8016E970(0);
    result = ScriptResult_unk(0, 1);
}

void GameEventScript::fn_8039DA00(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 0);
}

void GameEventScript::fn_8039DA10(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039E80C(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039E834(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result.mNext = FindLabel_unk(static_cast<ScriptArgs_unk*>(cmd)->m08);
    result.mWait = 0;
}

void GameEventScript::fn_8039E9C4(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 0);
}

void GameEventScript::fn_8039E9D4(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    lbl_8087F8A0->mHead->mFlagB2 = 1;
    lbl_8087F8A0->mHead->fn_80155DAC(1);
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039EA50(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    lbl_8087F8A0->mHead->mFlagB2 = 0;
    lbl_8087F8A0->mHead->fn_80155DAC(0);
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A03A0(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A04C0(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    mScene->fn_803CAABC();
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0AB8(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnTarget_unk(static_cast<TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0B1C(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnTarget2_unk(static_cast<TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0B80(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnParent_unk(static_cast<TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0BE4(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnMembers_unk(static_cast<TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0C48(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnMsg20_unk(static_cast<TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0CAC(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnMsg21_unk(static_cast<TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A0D10(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    lbl_8087FA00->OnChara_unk(static_cast<TeamMsgIn_unk*>(cmd));
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A362C(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
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

void GameEventScript::fn_803A4400(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    if (lbl_8087F430 != 0) {
        fn_80375194(lbl_8087F430, args->m1C, args->m14, *(float*)&args->m18);
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
void GameEventScript::fn_803A59D4(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    GameTextMessageWindow* window = lbl_8087F490->m2640;
    result.mNext = NextCmd_unk(cmd);
    result.mWait = window->mState != 0;
}

// NONMATCHING: 98% - registers r0/r6 swapped (type/zero), as fn_803A61CC
void GameEventScript::fn_803A5F14(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
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
void GameEventScript::fn_803A5F88(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (leader != 0) {
        if (static_cast<ScriptArgs_unk*>(cmd)->m08 == 0) {
            leader->m54C |= 0x10;
        } else {
            leader->m54C &= ~0x10;
        }
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 98% - registers r0/r6 swapped (type/zero)
void GameEventScript::fn_803A61CC(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

// NONMATCHING: 98% - registers r0/r6 swapped (type/zero), as fn_803A61CC
void GameEventScript::fn_803A61F4(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0) {
        lbl_8087F430->m5664 = static_cast<ScriptArgs_unk*>(cmd)->m08;
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A6F68(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

// NONMATCHING: 98% - registers r0/r4 swapped (type/zero), as fn_803A61CC
void GameEventScript::fn_803A83FC(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0) {
        fn_80373F78(lbl_8087F430);
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A8464(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    mB8 = 0;
    fn_803A8470(result, cmd);
}

void GameEventScript::fn_803A90E8(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    fn_803712BC(lbl_8087F430);
    lbl_8087F8A0->mHead->fn_8016E970(0);
    result = ScriptResult_unk(0, 1);
}

void GameEventScript::fn_803A9138(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    mB8 = 0;
    mC0 = 60;
    fn_803A914C(result, cmd);
}

void GameEventScript::fn_803A9558(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    mB8 = 0;
    mC0 = 60;
    fn_803A956C(result, cmd);
}

// NONMATCHING: 80% - the original stores mB8 before the flag test (rlwinm after the store)
void GameEventScript::fn_803A98C4(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    unsigned int flags = static_cast<ScriptArgs_unk*>(cmd)->m14;
    mB8 = 0;
    int n = 25;
    if ((flags & 4) == 4) n = 45;
    mC0 = n;
    fn_803A98EC(result, cmd);
}

// inferred: waits while the field is in state 5
void GameEventScript::fn_803AAA74(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0) {
        fn_8036E430(lbl_8087F430, 2, static_cast<ScriptArgs_unk*>(cmd)->m08);
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 72% - the original loads the arguments before saving r30/r31
void GameEventScript::fn_803AAD58(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    lbl_8087F490->fn_803E44C4((args->m0C & 1) ^ 1, args->m08, 45);
    result.SetNext_unk(cmd);
}

// NONMATCHING: 92% - the original loads m10 through the saved copy of cmd first (as fn_8039D178)
void GameEventScript::fn_803AB144(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    fn_8036E994(lbl_8087F430, args->m08, args->m0C, args->m10);
    result.SetNext_unk(cmd, 1);
}

void GameEventScript::fn_803AB1B0(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    int wait = 1;
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 != 5) {
        wait = 0;
    }
    result.SetNext_unk(cmd, wait);
}

void GameEventScript::fn_803AB328(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    fn_80371164(lbl_8087F430);
    result.mWait = 1;
    result.mNext = 0;
}

void GameEventScript::fn_803AB368(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
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
