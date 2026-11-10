// game/GameEventScript.cpp (0x803935AC-0x803B27F8): event script unit (GameEventScript,
// boost::bad_lexical_cast and the weak copies of std::bad_cast and the MSL string streams).
// The former GameEventScript_cont/_cont2 and std/bad_cast/_what splits are merged here.
#include "atn/SoundManager.h"
#include "game/TacView.h"
#include "game/GameCamera.h"
#include "game/BattleTask.h"
#include "atn/SoundRef.h"
#include "game/GameEventScript.h"
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
#include "game/TownTask.h"
#include "atn/LoadManager.h"
#include "game/GameConfig_unk.h"
#include "atn/SequenceBoot_unk.h"
#include "game/ArenaTask.h"
#include "Event/Sequence.h"
#include "game/BattleResult.h"
#include "game/GameTimer.h"
#include "game/SongManager.h"
#include "game/MiniMapTask.h"
#include "game/AiTarget.h"

#include <exception>

extern "C" GameTask* lbl_8087F430; // inferred: the field

extern "C" BattleTask* lbl_8087F048;

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
extern "C" TownTask* lbl_8087FA20;
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes a task
extern "C" ArenaTask* lbl_8087EE78;
extern "C" int fn_80444020(Inventory* party, int item, int a, int b, int c, int d, int* outA, int* outB,
                           int* outC); // inferred: adds an item (as in game/BattleResult.cpp)
extern "C" int rand();
extern "C" void* lbl_8087EF70;                                     // inferred: pad input
extern "C" int fn_800A555C(void* pad, int port, int button);       // inferred: button triggered
extern "C" float PSVECSquareMag(const atn::Vector3* v);
// inferred: squared length (name invented)
static inline float SqLength_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}
// inferred: sets the mode of the members of a team, keeping the previous one (name invented)
static inline void SetTeamMode_unk(TeamAI_unk* team, int mode) {
    if (team != 0) {
        for (int i = 0; i < team->mNumMembers; i++) {
            CharaTask* member = (CharaTask*)team->mMembers[i]; // inferred: TeamChara_unk is a view of CharaTask
            member->mF18 = member->mF14;
            member->mF14 = mode;
        }
    }
}
extern "C" void fn_8036E6D4(GameTask* task, int type, int sub, int arg); // inferred: opens a menu of the town
// inferred: view of the commands that name a motion of a character (names invented)
// inferred: view of the commands that name a sound (names invented)
struct ScriptSoundArgs_unk : public ScriptCmd_unk {
    char mName[0x20]; // 0x08
    float m28;        // inferred: volume
    unsigned int m2C; // inferred: flags
};
struct ScriptMotionArgs_unk : public ScriptCmd_unk {
    int m08;
    int m0C;
    int m10;
    char mName[0x20]; // 0x14
    int m34;          // inferred: flags (bit 0: wait)
};
extern "C" const char* const lbl_8074EF78[];
extern "C" void fn_8036F268(GameTask* field, int type, int id, int subId, int entry, int a, int b, int reason,
                            int arg9); // inferred: changes the map (game/NewCampPartsMapSelect.cpp)
extern "C" int lbl_8087D990; // inferred: last save slot (-1: none)
extern "C" int fn_804444E8(Inventory* inv, int id); // inferred: number of items

// inferred: save menu (game/CampSaveLoad.cpp)
class MenuSaveTask {
public:
    static MenuSaveTask* sInstance;
};
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

extern "C" const char lbl_8078B34C[];
extern "C" const char lbl_8078B394[];

// inferred: `event` is the address of the event record (fn_8039CC24); m04/m08: range of the field value m10D0
struct ScriptEventRange_unk {
    bool IsAlways_unk() const { return mMin == 0 && mMax == 0; }
    bool Contains_unk(int value) const { return value >= mMin && value <= mMax; }
    int m00;
    int mMin;
    int mMax;
};

int GameEventScript::vf0C() {
    for (int i = 0; i < 3; i++) {
        if (m48[i].fn_80473F50()) {
            return 0;
        }
    }
    for (int i = 0; i < 3; i++) {
        if (m48[i].fn_80470580()) {
            fn_8039BDB4(i, m48[i].fn_80470580(), m48[i].fn_8047059C());
        }
    }
    return 1;
}

// NONMATCHING: 98.4% - registers of the scene and of the entry offset swapped
void GameEventScript::fn_803957F0(int id) {
    GameScene* scene = mScene;
    for (unsigned int i = 0; i < scene->m80.size(); i++) {
        if (id == scene->m80[i].mId) {
            m118[i].mFlags &= ~2;
            return;
        }
    }
}

// NONMATCHING: 98% - registers of the field value and of the minimum swapped
int GameEventScript::fn_80398530(int event) {
    const ScriptEventRange_unk* e = (const ScriptEventRange_unk*)event;
    int value = lbl_8087F430->m10D0;
    return e->IsAlways_unk() || e->Contains_unk(value);
}

// NONMATCHING: 97.8% - the original loads the value of the node before the block address
int GameEventScript::fn_8039CC24(int id) {
    ScriptEventNode_unk* found = (ScriptEventNode_unk*)&m80;
    ScriptEventNode_unk* node = m80;
    while (node != 0) {
        if (!(node->mKey < id)) {
            found = node;
            node = node->mLeft;
        } else {
            node = node->mRight;
        }
    }
    if (found == (ScriptEventNode_unk*)&m80 || id < found->mKey) {
        found = (ScriptEventNode_unk*)&m80;
    }
    if (found != (ScriptEventNode_unk*)&m80) {
        return m64[id / 10000] + found->mValue;
    }
    return 0;
}

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

// inferred: waits (0: for m0C frames, 1: until the leader stops, 2: until the town is loaded)
void GameEventScript::fn_8039D274(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    mB4++;
    result.mWait = 1;
    CharaTask* leader = lbl_8087F8A0->mHead;
    int type = args->m08;
    if (type == 0) {
        if (mB4 >= args->m0C) {
            result.mWait = 0;
            if (mD0 != 0) {
                leader->fn_8016E970(1);
            }
        }
    } else if (type == 1) {
        if (mD0 != 0 && leader->mRoute.m2C != 0) {
            result.mWait = 0;
        }
    } else if (type == 2) {
        if (lbl_8087FA20 != 0 && GameBgm::sInstance != 0 && GameBgm::sInstance->mState == 2) {
            GameBgm::sInstance->fn_8037D3E8(1, 4, 15);
        }
        if (lbl_8087FA20 == 0 || lbl_8087FA20->fn_800D3FA4() != 0) {
            result.mWait = 0;
            if (lbl_8087FA20 != 0 && GameBgm::sInstance != 0) {
                GameBgm::sInstance->fn_8037D3E8(0, 4, 30);
            }
        } else {
            fn_8046ECDC(lbl_8087F518, 0);
        }
    }
    result.mNext = NextCmd_unk(cmd);
}

// inferred: fades the screen (0, 2: out; 1, 3: in)
void GameEventScript::fn_8039D3E0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    switch (args->m08) {
    case 0:
        lbl_8087F430->fn_80370B78(args->m0C, args->m10, 0, -9.0f);
        break;
    case 2:
        lbl_8087F430->fn_80370B78(args->m0C, args->m10, 0, -1.0f);
        break;
    case 1:
        lbl_8087F430->fn_80370BD0(args->m10, 0, -9.0f);
        break;
    case 3:
        lbl_8087F430->fn_80370BD0(args->m10, 0, -1.0f);
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
    lbl_8087F430->fn_80370C6C();
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

// inferred: conditional jump: fn_8039C7F0 picks one of two (kind, label) pairs (kind 0: next command,
// 1: end, else: jump to the label)
void GameEventScript::fn_8039E8A8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int kind;
    int label;
    if (fn_8039C7F0(args->m08, args->m0C, args->m18, args->m10, args->m14) != 0) {
        kind = args->m1C;
        label = args->m20;
    } else {
        kind = args->m24;
        label = args->m28;
    }
    if (kind == 0) {
        result.mNext = NextCmd_unk(cmd);
    } else if (kind == 1) {
        result.mNext = 0;
    } else {
        result.mNext = FindLabel_unk(label);
    }
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

// inferred: shows the choices of the message m08 and waits for the answer
// NONMATCHING: 84.6% - this and cmd swap r29/r30 (tried an args local)
void GameEventScript::fn_8039EACC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    lbl_8087F490->m263C->fn_803B583C(args->m08);
    fn_803935FC(1, 1.0f);
    if (lbl_8087F0A8->m0B8 == 0 || !lbl_8087F490->m263C->IsShown_unk()) {
        lbl_8087F8A0->mHead->fn_8016E970(5);
    }
    mB8 = 0;
    mBC = 20;
    result = ScriptResult_unk(NextCmd_unk(args), 1);
}

void GameEventScript::fn_8039F7D8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087F430->fn_80370094(static_cast<const ScriptArgs_unk*>(cmd)->m08);
    mF0 = 1;
    lbl_8087F430->fn_80370320(0x63, 0, 0);
    lbl_8087F430->fn_80370320(0x11D, 0, 0);
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_8039F878(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    fn_8039F9A4(GetChara_unk(args->m08, args->m0C), args);
    result.SetNext_unk(cmd);
}

// inferred: sends the message m10 to the gimmick (m08, m0C) (bit 0 of m14: through vf70)
void GameEventScript::fn_803A02C8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    GimmickCommon* gimmick = GimmickManager::instance_->fn_803EEE10(args->m08, args->m0C);
    if (gimmick != 0) {
        if ((args->m14 & 1) != 1) {
            GimmickMsg_unk msg(args->m10);
            gimmick->vf40(&msg);
        } else {
            gimmick->vf70(args->m10);
        }
    }
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

// inferred: the character (m10, m14) gets hate from the character (m08, m0C) (AIManager::fn_80017E90)
void GameEventScript::fn_803A08D4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    lbl_8087EE68->fn_80017E90(chara, GetChara_unk(args->m10, args->m14));
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

// inferred: stores a value of the leader (m0C 0: m10 4 mC58.mC0, 5 mC58.mB4) or of the team (m08, m0C) in
// (m14, m18): 0 HP ratio (at least 1 when above 0), 1 member ratio, 2 leader HP ratio, 3 to 6 AI type,
// battle mode, area and m242; other values do nothing (the command stays)
void GameEventScript::fn_803A0D74(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    if (args->m0C == 0) {
        CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (leader != 0) {
            switch (args->m10) {
            case 4:
                fn_8039BF04(args->m14, args->m18, 0, 0, leader->mC58.mC0);
                break;
            case 5:
                fn_8039BF04(args->m14, args->m18, 0, 0, leader->mC58.mB4);
                break;
            }
        }
    } else {
        TeamAI_unk* team = lbl_8087FA00->Find_unk(args->m08, args->m0C);
        if (team != 0) {
            int value;
            switch (args->m10) {
            case 0:
                value = team->fn_805AE454();
                if (value == 0 && team->fn_805AE454() > 0.0f) {
                    value = 1;
                }
                break;
            case 1:
                value = team->fn_805AE5E8();
                break;
            case 2:
                value = team->fn_805AE74C();
                break;
            case 3:
                value = team->mAIType;
                break;
            case 4:
                value = team->mBattleMode;
                break;
            case 5:
                value = team->mAreaID;
                break;
            case 6:
                value = team->m242;
                break;
            default:
                return;
            }
            fn_8039BF04(args->m14, args->m18, 0, 0, value);
        }
    }
    result.SetNext_unk(cmd);
}

// inferred: requests a BGM (unless the same one is already requested)
// NONMATCHING: 96% - load order of m10
void GameEventScript::fn_803A2154(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgsF_unk* args = static_cast<const ScriptArgsF_unk*>(cmd);
    GameBgm* bgm = GameBgm::sInstance;
    if (bgm != 0) {
        if (args->m08 > 0 || (bgm->m84 != 0 ? bgm->mNext.mId : bgm->mReq.mId) != 0x67) {
            if (lbl_8087F430->fn_803761FC() == 0) {
                GameBgm::sInstance->fn_8037D4C0(args->m08, *(int*)&args->m10, 1, 0, args->m0C);
            }
        }
    }
    result.SetNext_unk(cmd);
}

// inferred: plays the motion named in the command on a character (bit 0 of m34: wait)
// NONMATCHING: 97.8% - registers of the character, cmd and result, and the store of mWait
void GameEventScript::fn_803A2210(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptMotionArgs_unk* args = static_cast<const ScriptMotionArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    int wait = 0;
    if (chara != 0) {
        char name[0x20];
        memcpy(name, args->mName, sizeof(name));
        chara->fn_80161570(fn_802180A8(name), 0, 0, -1.0f, -1.0f);
        if ((args->m34 & 1) == 1) {
            wait = 1;
        }
    }
    result.SetNext_unk(cmd, wait);
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

// inferred: opens the gauge of the UI (the leader keeps flag 0x10 and its value in mC0); without the
// gauge, in the map (2, 0x36, 1) it goes back to the title
// NONMATCHING: 98.9% - "Title" is in the other string pool of the file (lbl_8074F8CC + 0x13E)
void GameEventScript::fn_803A24A4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0 && lbl_8087F430->m5694 != 0) {
        if (lbl_8087F490 != 0) {
            lbl_8087F490->fn_803E58E4();
            CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
            if (leader != 0) {
                mC0 = leader->fn_8017C9A0();
                leader->m54C |= 0x10;
            }
        }
        result.SetNext_unk(cmd, 1);
    } else {
        result.SetNext_unk(cmd);
        if (lbl_8087F430->GetCurrentMap() != 0 && lbl_8087F430->GetCurrentMap()->mType == 2 &&
            lbl_8087F430->GetCurrentMap()->mId == 0x36 && lbl_8087F430->GetCurrentMap()->mSubId == 1) {
            sSequenceBoot_unk->SetNext_unk("Title");
            fn_800D2338(lbl_8087F430);
        }
    }
}

// inferred: waits for the gauge of the UI; meanwhile sets flag 0x10 of the leader from mC0
// NONMATCHING: 96.7% - registers of this, result and cmd (r29-r31)
void GameEventScript::fn_803A2600(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0 && lbl_8087F430->m5694 != 0) {
        int wait;
        if (lbl_8087F490 != 0 && lbl_8087F490->fn_803E5940()) {
            wait = 1;
        } else {
            wait = 0;
            CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
            if (leader != 0) {
                if (mC0 != 0) {
                    leader->m54C |= 0x10;
                } else {
                    leader->m54C &= ~0x10;
                }
            }
        }
        result.SetNext_unk(cmd, wait);
    } else {
        result = ScriptResult_unk(0, 0);
    }
}

// NONMATCHING: 97.8% - registers of the index and of the entry offset swapped
ScriptEntry40_unk* GameEventScript::fn_803A2DD8(int id) {
    for (unsigned int i = 0; i < mFC.size(); i++) {
        if (id == mFC[i].mId) {
            return &mFC[i];
        }
    }
    return 0;
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
        lbl_8087F430->fn_80374964(args->m08);
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

// inferred: battle camera on (m08 0: mode m0C) or off (1, 2: also opens the tactical view)
void GameEventScript::fn_803A39F8(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    int wait = 0;
    if (lbl_8087F430 != 0) {
        if (args->m08 == 0) {
            if (lbl_8087F430->mCamera.m7FC == 6) {
                lbl_8087F430->fn_803748E0(1);
            }
            lbl_8087F430->fn_80374710(args->m0C);
        } else if (lbl_8087F430->mCamera.m7FC == 6) {
            if (args->m08 == 2) {
                lbl_8087F9F8->fn_805AA4E4();
            }
            wait = 1;
        }
    }
    result.SetNext_unk(cmd, wait);
}

// inferred: waits for the tactical view (m08 2) or for a button after showing a guide (3: guide 13,
// else 12) in the UI; then ends the grapple camera
// NONMATCHING: 93.7% - registers of the guide fields and the store of mWait
void GameEventScript::fn_803A3AB4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int wait = 0;
    if (lbl_8087F430 != 0) {
        if (args->m08 == 2) {
            if (lbl_8087F9F8->mA70 == 0) {
                lbl_8087F9F8->fn_805AA734();
                lbl_8087F430->fn_803748E0(1);
                wait = 0;
            } else {
                wait = 1;
            }
        } else {
            if (lbl_8087F490 != 0) {
                GameUIElem1C_unk& guide = lbl_8087F490->m764;
                guide.m00 = 6;
                guide.m04 = (args->m08 == 3) + 12;
                guide.m08 = -1;
                guide.m0C = 0;
                guide.m10 = 0;
                guide.m14 = 0;
                guide.m18 = 0;
            }
            if (fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
                fn_800C31F4(lbl_8074EF78[8], 0, -1, 1.0f);
                lbl_8087F430->fn_803748E0(1);
                wait = 0;
            } else {
                wait = 1;
            }
        }
    }
    result.SetNext_unk(cmd, wait);
}

// inferred: autosave point (when autosave is enabled)
// inferred: sets the camera of the current map
void GameEventScript::fn_803A3BF4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    MapTask* map = lbl_8087F430 != 0 ? lbl_8087F430->GetCurrentMap() : 0;
    if (map != 0) {
        map->fn_804A04AC(args->m08, args->m0C != 0 ? 10 : 0, -1, 0);
        map->mDefaultCamera = args->m08;
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A3CA8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0 && SetConfig::sInstance->mAutoSave != 0) {
        lbl_8087F430->fn_80376730();
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A4400(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    if (lbl_8087F430 != 0) {
        lbl_8087F430->fn_80375194(args->m1C, args->m14, *(float*)&args->m18);
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

// inferred: plays the sound named in the command for the field (bit 0 of m2C: stops the sounds of
// the field instead; "SE_VOTWN_041" stops them first)
// NONMATCHING: 99.9% - only the offset of "SE_VOTWN_041" in the string pool (other strings of the file not written yet)
void GameEventScript::fn_803A47A4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptSoundArgs_unk* args = static_cast<const ScriptSoundArgs_unk*>(cmd);
    char name[0x20];
    memcpy(name, args->mName, sizeof(name));
    atn::SoundManager* sound = lbl_8087EFE8;
    if (sound != 0) {
        if ((args->m2C & 1) != 1) {
            if (strncmp(args->mName, "SE_VOTWN_041", 12) == 0) {
                sound->fn_800CFD18(lbl_8087F430, 15);
            }
            lbl_8087EFE8->m34D8 = lbl_8087F430;
            fn_803935AC(name, args->m28);
            lbl_8087EFE8->m34D8 = 0;
        } else {
            sound->fn_800CFD18(lbl_8087F430, 15);
        }
    }
    result.SetNext_unk(cmd);
}

// inferred: sends the members of the team (m08, m0C) away (state 1 of the group: m08...)
// NONMATCHING: 96.1% - registers (r26-r29) of the leader, the team and the member pointer
void GameEventScript::fn_803A48A8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int wait = 0;
    if (args->m08 == 2) {
        CharaTask* leader = lbl_8087F8A0->mHead;
        TeamAI_unk* team = lbl_8087FA00->Find_unk(args->m08, args->m0C);
        if (team != 0) {
            float best = 0.0f;
            CharaTask* far = 0;
            for (int i = 0; i < team->mNumMembers; i++) {
                CharaTask* member = (CharaTask*)team->mMembers[i]; // inferred: TeamChara_unk is a view of CharaTask
                if (member->mC58.mC0 > 0) {
                    float dist = SqLength_unk(leader->mPos - member->mPos);
                    if (dist > best) {
                        best = dist;
                        far = member;
                    }
                }
            }
            if (far != 0) {
                far->fn_80161570(0x6A, 0, 0, -1.0f, -1.0f);
                mB4 = 0;
                wait = 1;
            }
        }
    }
    result.SetNext_unk(cmd, wait);
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

// inferred: sets the mode of the members of three teams (pairs m08/m0C, m10/m14, m18/m1C) to 0, 1 and 2
// NONMATCHING: 98.5% - the member pointer and the member swap r5/r7
void GameEventScript::fn_803A5384(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    SetTeamMode_unk(lbl_8087FA00->Find_unk(args->m08, args->m0C), 0);
    SetTeamMode_unk(lbl_8087FA00->Find_unk(args->m10, args->m14), 1);
    SetTeamMode_unk(lbl_8087FA00->Find_unk(args->m18, args->m1C), 2);
    result.SetNext_unk(cmd);
}

// NONMATCHING: 95.5% - IsFree_unk is computed as a bool (cror) in the original and mState is reused
// inferred: shows the text messages m10 to m1C of the scene in the text window (m14 bit 0: do not wait,
// bit 1: closes the window instead, bit 2: sets the event counter (m08, m0C))
void GameEventScript::fn_803A5804(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    GameMessage* msg = (GameMessage*)mScene->fn_803CC6B4(args->m10);
    GameTextMessageWindow* window = lbl_8087F490->m2640;
    if ((args->m14 & 2) != 2) {
        if (window != 0 && window->IsFree_unk(5.0f) && msg != 0) {
            if (window->mState == 2) {
                window->fn_803B6E04(msg, args->m18, 5.0f);
            } else {
                window->fn_803B6970(msg, args->m18, 5.0f);
            }
            window->mCount = 0;
            if (args->m1C > args->m10) {
                for (int i = args->m10 + 1; i <= args->m1C; i++) {
                    GameMessage* next = (GameMessage*)mScene->fn_803CC6B4(i);
                    if (next != 0) {
                        window->fn_803B78B4(next);
                    }
                }
            }
            window->fn_803B78E8();
            if ((args->m14 & 4) == 4) {
                window->m71 = 1;
                window->m74 = args->m08;
                window->m78 = args->m0C;
            }
            mB4 = 0;
            if ((args->m14 & 1) != 1) {
                result.mWait = 1;
            } else {
                result.mWait = 0;
            }
        } else {
            result.mWait = 0;
        }
    } else {
        if (window->mState != 0) {
            window->mCount = 0;
            window->fn_803B6C88(0);
        }
        result.mWait = 0;
    }
    result.mNext = NextCmd_unk(cmd);
}

void GameEventScript::fn_803A59D4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    GameTextMessageWindow* window = lbl_8087F490->m2640;
    result.mNext = NextCmd_unk(cmd);
    result.mWait = window->mState != 0;
}

// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
// inferred: writes a message of the scene to the battle log
void GameEventScript::fn_803A5A10(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    GameMessage* msg = (GameMessage*)mScene->fn_803CC6B4(static_cast<const ScriptArgs_unk*>(cmd)->m0C);
    if (msg != 0) {
        lbl_8087F048->fn_80109828(msg->mText);
    }
    result.SetNext_unk(cmd);
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

// inferred: stores in (m10, m14) whether the event m08 (<= 0: the current one) is done (m0C 0) or
// playing (m0C 1)
// NONMATCHING: 99%% - registers r0/r4 swapped in the next command (as fn_803A61CC)
void GameEventScript::fn_803A6230(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int id = args->m08;
    int value = 0;
    if (id <= 0 && lbl_8087F540->m70 != 0) {
        id = lbl_8087F540->m70->m190;
    }
    if (id > 0) {
        switch (args->m0C) {
        case 0:
            value = lbl_8087F540->fn_8047F268(id) != 0;
            break;
        case 1:
            value = lbl_8087F540->fn_8047F2EC(id) != 0;
            break;
        }
    }
    fn_8039BF04(args->m10, args->m14, 0, 0, value);
    result.SetNext_unk(cmd);
}

// inferred: stores a value of the arena (m08: 0 running, 1 mode, 2 m5C, 3 and 4 scores, 5 result) in
// (m0C, m10)
// NONMATCHING: 99%% - registers r0/r4 swapped in the next command (as fn_803A61CC)
void GameEventScript::fn_803A6324(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int value = 0;
    ArenaTask* arena = lbl_8087EE78;
    if (arena != 0) {
        switch (args->m08) {
        case 0:
            value = arena->mState == 1;
            break;
        case 1:
            value = arena->mMode;
            break;
        case 2:
            value = arena->m5C;
            break;
        case 3:
            value = arena->mScore0;
            break;
        case 4:
            value = arena->mScore1;
            break;
        case 5:
            value = arena->mResult;
            break;
        }
    }
    fn_8039BF04(args->m0C, args->m10, 0, 0, value);
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

// NONMATCHING: 92.3% - the original reloads mItemCount for the slot of AddItem_unk (as BattleCharaTask::fn_800EE360) and stores mDB0C later
// inferred: gives the item m08 (bit 0 of m0C: added to the result of the battle instead) and shows it
void GameEventScript::fn_803A6B9C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    Inventory::instance_->mDB0C = 0;
    if (((unsigned int)args->m0C & 1) != 1) {
        int a;
        int b;
        int c;
        if (fn_80444020(Inventory::instance_, args->m08, 1, -1, 0, 0, &a, &b, &c) != 0) {
            lbl_8087F490->fn_803E5E64(a, b, c);
            fn_800C31F4(lbl_8074EF78[4], 0, -1, 1.0f);
        }
    } else if (BattleResult::sInstance != 0) {
        BattleResult::sInstance->AddItem_unk(args->m08);
    }
    Inventory::instance_->mDB0C = 1;
    result.SetNext_unk(cmd);
}

// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
// inferred: controls the game timer (m08 0/1: starts it counting down/up to the value, 2: pauses,
// 3: stops, 4: adds, 5: subtracts); the value is m10, or the flag m10 (m0C 1, 2) or a random number
// in [1, m10] (m0C 3)
void GameEventScript::fn_803A6CDC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int type = args->m0C;
    int value = args->m10;
    if (lbl_8087F430 == 0) {
        value = 0;
    } else if (type == 2) {
        value = lbl_8087F430->fn_80370174(value);
    } else if (type == 1) {
        value = lbl_8087F430->fn_80370A78(value);
    } else if (type == 3) {
        value = rand() % value + 1;
    }
    GameTimer* timer = GameTimer::sInstance;
    if (timer != 0) {
        switch (args->m08) {
        case 0:
        case 1:
            timer->fn_803CE688(value, args->m08 == 0);
            break;
        case 2:
            timer->fn_803CE6BC(value != 0);
            break;
        case 3:
            timer->fn_803CE6FC();
            break;
        case 4:
            timer->fn_803CE708(value);
            break;
        case 5:
            timer->fn_803CE738(value);
            break;
        }
    }
    result.SetNext_unk(cmd);
}

// inferred: passes a character to the gimmick (m10, m14) (vf4C)
void GameEventScript::fn_803A6E24(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara != 0) {
        GimmickCommon* gimmick = GimmickManager::instance_->fn_803EEE10(args->m10, args->m14);
        if (gimmick != 0) {
            gimmick->vf4C(chara);
        }
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A6F68(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A7BB0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0) {
        lbl_8087F430->mCamera.fn_8037EF30(static_cast<const ScriptArgs_unk*>(cmd)->m08, 1, -1.0f);
    }
    result.SetNext_unk(cmd);
}

// inferred: m08 != 0: fn_8017A504 on the leader, 0: clears its flag 0x200000
// NONMATCHING: 99%% - registers r0/r4 swapped in the next command (as fn_803A61CC)
void GameEventScript::fn_803A7C28(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (leader != 0) {
        if (static_cast<const ScriptArgs_unk*>(cmd)->m08 != 0) {
            leader->fn_8017A504();
        } else {
            leader->m54C &= ~0x200000;
        }
    }
    result.SetNext_unk(cmd);
}

// inferred: opens a menu of the town (m10 bit 1: shop animations of town m14, bit 2: fn_803E65F8,
// else m08 3: library, 5: nothing, others through fn_8036E6D4)
// NONMATCHING: 99%% - registers r0/r4 swapped in the next command (as fn_803A61CC)
void GameEventScript::fn_803A7CC0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int flags = args->m10;
    if ((flags & 2) == 2) {
        if (lbl_8087F490 != 0) {
            lbl_8087F490->fn_803E627C(args->m14);
        }
    } else if ((flags & 4) == 4) {
        if (lbl_8087F490 != 0) {
            lbl_8087F490->fn_803E65F8();
        }
    } else if (lbl_8087F430 != 0) {
        if (args->m08 == 3) {
            lbl_8087F430->fn_8036EA04();
        } else if (args->m08 != 5) {
            fn_8036E6D4(lbl_8087F430, args->m08, args->m0C, args->m14);
        }
    }
    result.SetNext_unk(cmd, 1);
}

// NONMATCHING: 97.3% - the store of mWait is scheduled after the table address in the original
// inferred: closes a gauge of the UI (m10 bit 1) or opens it (bit 2), the minimap of the town follows;
// otherwise waits while the field is in state 3
void GameEventScript::fn_803A7D94(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int flags = args->m10;
    int wait = 1;
    if ((flags & 2) == 2) {
        if (lbl_8087F490 != 0 && !lbl_8087F490->fn_803E644C()) {
            lbl_8087F490->fn_803E64F0();
            lbl_8087F490->fn_803E656C();
            wait = 0;
            if (lbl_8087FA20 != 0 && lbl_8087FA20->m258 != 0) {
                ((MiniMapTask*)lbl_8087FA20->m258)->m4C = 0; // inferred: m258 is the minimap (fn_804A7EA4)
            }
        }
    } else if ((flags & 4) == 4) {
        if (lbl_8087F490 != 0 && lbl_8087F490->m262C.m0C == 0) {
            lbl_8087F490->fn_803E63D0();
            wait = 0;
            if (lbl_8087FA20 != 0 && lbl_8087FA20->m258 != 0) {
                ((MiniMapTask*)lbl_8087FA20->m258)->m4C = 1;
            }
        }
    } else if (lbl_8087F430->m54E4 != 3) {
        wait = 0;
    }
    result.SetNext_unk(cmd, wait);
}

// NONMATCHING: 99.5% - registers r0/r4 swapped in the next command (as fn_803A61CC)
// inferred: stores a value of the song (m08: 0 state, 1 index, 3 distance, 4 and 5 volumes in percent)
// in (m0C, m10)
void GameEventScript::fn_803A7EB8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int value = 0;
    SongManager* song = SongManager::sInstance;
    if (song != 0) {
        switch (args->m08) {
        case 0:
            value = song->m48;
            break;
        case 1:
            value = song->m4C;
            break;
        case 3:
            value = song->mDistance.mTo;
            break;
        case 4: {
            float volume = song->mMinVolume.mTo;
            value = 100.0f * volume;
            break;
        }
        case 5: {
            float volume = song->mMaxVolume.mTo;
            value = 100.0f * volume;
            break;
        }
        }
    }
    fn_8039BF04(args->m0C, args->m10, 0, 0, value);
    result.SetNext_unk(cmd);
}

// inferred: stores the money of the inventory (m08 0) in (m0C, m10)
// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
void GameEventScript::fn_803A81F8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int value = 0;
    if (Inventory::instance_ != 0 && args->m08 == 0) {
        value = Inventory::instance_->mMoney;
    }
    fn_8039BF04(args->m0C, args->m10, 0, 0, value);
    result.SetNext_unk(cmd);
}

// NONMATCHING: 98% - registers r0/r4 swapped (type/zero), as fn_803A61CC
void GameEventScript::fn_803A83FC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    if (lbl_8087F430 != 0) {
        lbl_8087F430->fn_80373F78();
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803A8464(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    mB8 = 0;
    fn_803A8470(result, cmd);
}

void GameEventScript::fn_803A90E8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087F430->fn_803712BC();
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
        lbl_8087F430->fn_8036E430(2, static_cast<const ScriptArgs_unk*>(cmd)->m08);
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
    lbl_8087F430->fn_8036E994(args->m08, args->m0C, args->m10);
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
    lbl_8087F430->fn_80370320(0x391, static_cast<const ScriptArgs_unk*>(cmd)->m14, 0);
    lbl_8087F430->fn_8036E92C(1);
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
        lbl_8087F430->fn_80370320(0x391, 0, 0);
        wait = 0;
    }
    result.SetNext_unk(cmd, wait);
}

void GameEventScript::fn_803AB328(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    lbl_8087F430->fn_80371164();
    result.mWait = 1;
    result.mNext = 0;
}

void GameEventScript::fn_803AB368(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    result = ScriptResult_unk(0, 1);
}

// inferred: removes the record of `chara` from m12C (CharaTask::fn_80171DB0, through the scene)
void GameEventScript::fn_803AB37C(CharaTask* chara) {
    for (unsigned int i = 0; i < m12C.size(); i++) {
        CharaTask* v = m12C[i].mChara;
        if (v == chara) {
            m12C.erase(&m12C[i]);
            return;
        }
    }
}

// inferred: ends the event started by the script on the event manager
void GameEventScript::fn_803ACD08() {
    if (mE8 > 0) {
        lbl_8087F540->fn_8047F580();
        mE8 = 0;
        mEC = 0;
    }
}

const char* boost::bad_lexical_cast::what() const { return lbl_8078B34C; }

const char* std::bad_cast::what() const { return lbl_8078B394; }
