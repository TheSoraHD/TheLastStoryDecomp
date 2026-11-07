// game/GameEventScript.cpp (0x803935AC-0x803B27F8): event script unit (GameEventScript,
// boost::bad_lexical_cast and the weak copies of std::bad_cast and the MSL string streams).
// The former GameEventScript_cont/_cont2 and std/bad_cast/_what splits are merged here.
#include "atn/SoundManager.h"
#include "atn/SoundRef.h"
#include "game/GameEventScript.h"
#include "game/GameField_unk.h"
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

void GameEventScript::fn_8039E9C4(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 0);
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

// NONMATCHING: 98% - registers r0/r6 swapped (type/zero)
void GameEventScript::fn_803A61CC(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A6F68(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A8464(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    mB8 = 0;
    fn_803A8470(result, cmd);
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

void GameEventScript::fn_803AB368(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 1);
}

extern "C" const char lbl_8078B34C[];
extern "C" const char lbl_8078B394[];

const char* boost::bad_lexical_cast::what() const { return lbl_8078B34C; }

const char* std::bad_cast::what() const { return lbl_8078B394; }
