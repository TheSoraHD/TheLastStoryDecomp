// game/GameEventScript.cpp (0x803935AC-0x803B27F8): event script unit (GameEventScript,
// boost::bad_lexical_cast and the weak copies of std::bad_cast and the MSL string streams).
// The former GameEventScript_cont/_cont2 and std/bad_cast/_what splits are merged here.
#include "atn/SoundManager.h"
#include "atn/Memory.h"
#include "atn/DrawHelper.h"
#include "atn/ColliBase.h"
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
#include "game/EquipItemData.h"
#include "game/FriendManager.h"
#include "db/EventIdTable_unk.h"
#include "db/ItemParam_unk.h"
#include "db/WeaponParam_unk.h"
#include "game/GameVoice.h"
#include "game/EnvSeManager.h"
#include "game/MapManager.h"
#include "game/MapPartsLocatorSet.h"
#include "atn/DebugMessages_unk.h"
#include "game/ChaseManager.h"
#include "game/ChaserNpc.h"
#include "gmk/TreasureBox.h"

#include <exception>
#include "std/string.h"
#include "std/string_members.h"

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
extern "C" int atoi(const char* s);
extern "C" int sprintf(char* buf, const char* format, ...);
extern "C" void fn_801092C8(BattleTask* battle, int kind, int arg, int value, int arg4); // inferred: battle log entry
extern "C" void* lbl_8087EF70;                                     // inferred: pad input
extern "C" int fn_800A555C(void* pad, int port, int button);       // inferred: button triggered
extern "C" void fn_80392D2C(Camera_unk* camera, const Camera_unk* from, float rate); // inferred: blends back
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_8044441C(Inventory* inv, int id, int num); // inferred: removes items
extern "C" float PSVECSquareMag(const atn::Vector3* v);
// inferred: squared length (name invented)
static inline float SqLength_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}
// inferred: value of an argument (type 1: the flag `value` of the field, 2: fn_80370174, 3: a random
// number in [1, value], else the value itself; 0 without a field) (name invented)
static inline int ScriptValue_unk(int type, int value) {
    if (lbl_8087F430 == 0) {
        return 0;
    }
    if (type == 2) {
        return lbl_8087F430->fn_80370174(value);
    }
    if (type == 1) {
        return lbl_8087F430->fn_80370A78(value);
    }
    if (type == 3) {
        value = rand() % value + 1;
    }
    return value;
}
// inferred: command after the end of the block that starts at `cmd` (commands 0x44 open nested
// blocks, 0x45 closes one); the end of the script if missing (name invented)
static inline ScriptCmd_unk* SkipBlock_unk(ScriptData_unk* data, const ScriptCmd_unk* cmd) {
    ScriptCmd_unk* end = &data->mCmds;
    for (unsigned int i = 0; i < data->mCmdCount; i++) {
        end = (ScriptCmd_unk*)((char*)end + lbl_8074ED24[end->mCmd]);
    }
    ScriptCmd_unk* next = (ScriptCmd_unk*)((const char*)cmd + lbl_8074ED24[cmd->mCmd]);
    int depth = 0;
    while (next != end) {
        if ((next->m04 & 1) != 1) {
            if (next->mCmd == 0x44) {
                depth++;
            } else if (next->mCmd == 0x45) {
                if (depth <= 0) {
                    break;
                }
                depth--;
            }
        }
        next = (ScriptCmd_unk*)((char*)next + lbl_8074ED24[next->mCmd]);
    }
    return next;
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
// inferred: view of the area command (fn_803AAC60; names invented)
struct ScriptAreaArgs_unk : public ScriptCmd_unk {
    int m08;
    float m0C; // inferred: blend
    int m10;   // inferred: frames
};
// inferred: view of the follow command (fn_803A5650; names invented)
struct ScriptFollowArgs_unk : public ScriptCmd_unk {
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    float m1C; // inferred: distance
    int m20;   // inferred: 1 stops
};
// inferred: view of the node command (fn_803A4568; names invented)
struct ScriptNodeArgs_unk : public ScriptCmd_unk {
    int m08;
    int m0C;
    char mName[0x20]; // 0x10
    int m30;          // inferred: 0 shows
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

// inferred: creates the event script of a scene (game/GameScene.cpp)
extern "C" GameEventScript* fn_80393610(atn::Task* parent, const char* path, int area, int map) {
    if (parent != 0) {
        return new ((MemoryTag)3, "", "", 0) GameEventScript(parent, path, area, map);
    }
    return 0;
}

extern "C" int fn_8006AA20(const char* path);                     // inferred: the file exists
extern "C" std::string fn_8006B2D8(const std::string& path);      // inferred: name without extension
// inferred: boost::lexical_cast<std::string>(const int&) (weak copy 0x80393BEC; string stream)
extern "C" std::string fn_80393BEC(const int& value);

// NONMATCHING: 99.95% - registers of the string sizes (r23 vs r20)
// inferred: loads the script of the scene (file 0) and its sub scripts (files 1-2): the town sub quest and
// hint scripts when the config asks for them, else <path>_sub1.sc and <path>_sub2.sc
GameEventScript::GameEventScript(atn::Task* parent, const char* path, int area, int map)
    : atn::Task(parent), m60(0), mScene((GameScene*)parent),
      m8C(0), m9C(0), mA0(0), mA4(0), mD0(0), mD4(-1), mD8(0), mDC(0), mE0(0), mE4(0), mE8(0), mEC(0), mF0(0),
      mF4(0), mF8(0), m37C(0), m380(0) {
    memset(m64, 0, sizeof(m64));
    m48[0].vf0C(path);
    if (area == 1 && lbl_8087F0A8->mLoadTownSubScript != 0) {
        std::string file("/game_scene/tw_subquest.sc");
        if (fn_8006AA20(file.c_str()) && map != 3) {
            m48[1].vf0C(file.c_str());
        }
        file = "/game_scene/tw_hintnpc.sc";
        if (fn_8006AA20(file.c_str())) {
            m48[2].vf0C(file.c_str());
        }
    } else {
        for (int i = 1; i < 3; i++) {
            std::string file = fn_8006B2D8(std::string(path));
            file += "_sub";
            file += fn_80393BEC(i);
            file += ".sc";
            if (fn_8006AA20(file.c_str())) {
                m48[i].vf0C(file.c_str());
            }
        }
    }
    memset(m154, 0, sizeof(m154));
}

// inferred: starts or stops the log of the checked events (name invented)
static inline void SetLog_unk(GameEventScript* self, int on) {
    if (self->m37C != on) {
        self->m37C = on;
        if (lbl_8087EEB8 != 0) {
            if (self->m37C != 0) {
                lbl_8087EEB8->fn_80069D48("EventScript Log START");
                if (self->m364.mSize == 0) {
                    self->m364.Resize_unk(self->mScene->m80.size(), 1);
                }
                if (self->m36C.mSize == 0) {
                    self->m36C.Resize_unk(self->mScene->m88.size(), 1);
                }
                if (self->m374.mSize == 0) {
                    self->m374.Resize_unk(self->mScene->m90.size(), 1);
                }
                if (self->m354.mSize == 0) {
                    self->m354.Resize_unk(self->mScene->mE4.size(), 1);
                }
                if (self->m35C.mSize == 0) {
                    self->m35C.Resize_unk(self->mScene->mEC.size(), 1);
                }
            } else {
                lbl_8087EEB8->fn_80069D48("EventScript Log END");
            }
        }
    }
}

// NONMATCHING: 99.45% - registers: the size of the logs (r29 vs r27) and the tree objects and roots
// (r28/r27 swapped)
GameEventScript::~GameEventScript() {
    SetLog_unk(this, 0);
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

// NONMATCHING: 99.77% - registers: the array and the scene of the Resize_unk calls 2-5 (r3/r4) and the
// counters of the sphere loop
// inferred: prepares the flags of the points of the scene (GameScene::fn_803C0F48): one per point, the boxes
// of the event boxes and the groups of boxes and spheres (each one with the others of its group)
void GameEventScript::fn_80394C68() {
    m114.Resize_unk(mScene->m80.size(), 1);
    m11C.Resize_unk(mScene->m88.size(), 1);
    m124.Resize_unk(mScene->m90.size(), 1);
    mFC.Resize_unk(mScene->mE4.size(), 1);
    m10C.Resize_unk(mScene->mEC.size(), 1);
    m104.Resize_unk(mScene->mE4.size(), 1);
    m144.Resize_unk(mScene->mE4.size(), 1);
    m14C.Resize_unk(mScene->mEC.size(), 1);
    GameScene* scene = mScene;
    for (unsigned int i = 0; i < scene->mE4.size(); i++) {
        gs::EventBox& box = scene->mE4[i];
        atn::Vector3 pos = box.mPos;
        pos.y += box.mSize.y;
        atn::Vector3 rot(0.0f, box.m14, 0.0f);
        m104[i].fn_800575BC(&pos, &rot, &box.mSize);
        for (unsigned int j = 0; j < scene->mE4.size(); j++) {
            gs::EventBox& other = scene->mE4[j];
            if (i == j || box.m2C == j || other.m2C == i || (box.m2C >= 0 && box.m2C == other.m2C)) {
                m144[i].push_back(j);
            }
        }
    }
    scene = mScene;
    for (unsigned int i = 0; i < scene->mEC.size(); i++) {
        gs::EventSphere& sphere = scene->mEC[i];
        for (unsigned int j = 0; j < scene->mEC.size(); j++) {
            gs::EventSphere& other = scene->mEC[j];
            if (i == j || sphere.m20 == j || other.m20 == i || (sphere.m20 >= 0 && sphere.m20 == other.m20)) {
                m14C[i].push_back(j);
            }
        }
    }
    fn_80395848(1);
    mB0 = 0;
    mD4 = -1;
    mD8 = 0;
}

// NONMATCHING: 98.4% - registers of the scene and of the entry offset swapped
void GameEventScript::fn_803957F0(int id) {
    GameScene* scene = mScene;
    for (unsigned int i = 0; i < scene->m80.size(); i++) {
        if (id == scene->m80[i].mId) {
            m114[i].mFlags &= ~2;
            return;
        }
    }
}

// inferred: a condition of an event (names invented)
struct ScriptEventCond_unk {
    int m00; // inferred: negative: the event must not be finished, else the kind of fn_8039C7F0
    int m04;
    int m08;
    int m0C;
    int m10;
};
// inferred: an event with its range of the field value and its conditions (names invented)
struct ScriptEventConds_unk : public ScriptEventRange_unk {
    int m0C;
    ScriptEventCond_unk mConds[8]; // 0x10
    int mCount;                    // 0xB0
    int mB4;
    int mB8; // inferred: flags (1: box enabled, 4: box flag 0x20, 8: started from the camera, fn_803AC708)
    int mBC;
    int mC0; // inferred: warp point where the character of the point is placed (fn_80395848)
};

// inferred: fn_80398590 testing every condition (name invented)
inline int GameEventScript::CanStart_unk(int event) {
    const ScriptEventCond_unk* cond;
    int ok = 0;
    if (event != 0) {
        const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)event;
        bool pass = true;
        int value = lbl_8087F430->m10D0;
        if (!e->IsAlways_unk() && !e->Contains_unk(value)) {
            pass = false;
        }
        if (pass) {
            ok = 1;
            cond = e->mConds;
            for (int i = 0; i < e->mCount; cond++, i++) {
                {
                    int r;
                    int id = e->m00;
                    if (cond->m00 < 0) {
                        ScriptDoneNode_unk* found = mDone.End_unk();
                        ScriptDoneNode_unk* node = mDone.mRoot;
                        while (node != 0) {
                            if (!(node->mValue < id)) {
                                found = node;
                                node = node->mLeft;
                            } else {
                                node = node->mRight;
                            }
                        }
                        if (found == mDone.End_unk() || id < found->mValue) {
                            found = mDone.End_unk();
                        }
                        r = found == mDone.End_unk();
                    } else {
                        r = fn_8039C7F0(cond->m00, cond->m04, cond->m10, cond->m08, cond->m0C);
                    }
                    if (r == 0) {
                        ok = 0;
                        break;
                    }
                }
            }
        }
    }
    return ok;
}

extern "C" gmk::TreasureBox::Data_unk* fn_8021ECD0(int id); // inferred: chest data

// inferred: the field value is in the range of the event (as the start of fn_80398590; name invented)
static inline int EventInRange_unk(int event) {
    const ScriptEventRange_unk* e = (const ScriptEventRange_unk*)event;
    bool pass = true;
    int value = lbl_8087F430->m10D0;
    if (!e->IsAlways_unk() && !e->Contains_unk(value)) {
        pass = false;
    }
    return pass;
}

// inferred: one of the scripts has an event in range (name invented)
inline int GameEventScript::HasEventInRange_unk(const atn::SimpleArray<int>& scripts) {
    for (unsigned int j = 0; j < scripts.size(); j++) {
        int event = FindEvent_unk(scripts[j]);
        if (event != 0 && EventInRange_unk(event)) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 93.1% - the loops of HasEventInRange_unk become ctr loops here (the original keeps the
// counter and reloads the size) and registers
// inferred: picks the script of every point of the scene: characters (npcs, enemies, friends), then event
// boxes and spheres; with `check` (towns) the points without an event in range are disabled (flag 0x10)
void GameEventScript::fn_80395848(int check) {
    UpdatePoints_unk(mScene->m80, m114, m364, NpcManager::sInstance, 1, check, (CharaTask*)0);
    UpdatePoints_unk(mScene->m88, m11C, m36C, EnemyManager::sInstance, 2, check, (CharaTask*)0);
    UpdatePoints_unk(mScene->m90, m124, m374, FriendManager::sInstance, 3, check, (CharaTask*)lbl_8087F8A0->mHead);
    GameScene* scene = mScene;
    for (unsigned int i = 0; i < scene->mE4.size(); i++) {
        gs::EventBox& box = scene->mE4[i];
        if (box.m2C < 0) {
            if (check != 0 && lbl_8087FA20 != 0 && !HasEventInRange_unk(box.m24)) {
                mFC[i].mFlags |= 0x10;
            }
            if ((mFC[i].mFlags & 0x10) != 0x10) {
                int index = fn_80396298(&box);
                if (box.m30 != 0) {
                    gmk::TreasureBox::Data_unk* chest = fn_8021ECD0(box.m34);
                    if (chest != 0 && lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(chest->mFlag) > 0) {
                        index = -1;
                    }
                }
                mFC[i].mFlags &= ~4;
                if (index != mFC[i].m00) {
                    mFC[i].mFlags &= ~1;
                    mFC[i].mFlags &= ~8;
                    mFC[i].m00 = index;
                    const ScriptEventConds_unk* e =
                        (const ScriptEventConds_unk*)(index >= 0 ? FindEvent_unk(box.m24[index]) : 0);
                    if (e != 0 && (e->mB8 & 1) == 1) {
                        m104[i].mFlags |= 1;
                        m104[i].m20 = box.m38;
                        m104[i].m1C = box.m3C;
                        if ((e->mB8 & 4) == 4) {
                            m104[i].mFlags |= 0x20;
                        } else {
                            m104[i].mFlags &= ~0x20;
                        }
                    } else {
                        m104[i].mFlags &= ~1;
                    }
                }
                if (m37C != 0) {
                    m354[i].m04 = 5;
                    m354[i].m08 = box.mId;
                    m354[i].m10 = mFC[i].m00;
                    m354[i].m14 = index;
                }
            }
        }
    }
    scene = mScene;
    for (unsigned int i = 0; i < scene->mEC.size(); i++) {
        gs::EventSphere& sphere = scene->mEC[i];
        if (sphere.m20 < 0) {
            if (check != 0 && lbl_8087FA20 != 0 && !HasEventInRange_unk(sphere.m18)) {
                m10C[i].mFlags |= 0x10;
            }
            if ((m10C[i].mFlags & 0x10) != 0x10) {
                int index = fn_80395F34(&sphere);
                m10C[i].mFlags &= ~4;
                if (index != m10C[i].m00) {
                    m10C[i].mFlags &= ~1;
                    m10C[i].mFlags &= ~8;
                    m10C[i].m00 = index;
                }
                if (m37C != 0) {
                    m35C[i].m04 = 4;
                    m35C[i].m08 = sphere.mId;
                    m35C[i].m10 = m10C[i].m00;
                    m35C[i].m14 = index;
                }
            }
        }
    }
}

// inferred: index of the last script of the point whose event can start (bit 0 of the flags enables the
// point, bit 1 when m5694 is set), -1 if none; with the global flag 27, -1 also when that event has no
// condition on the flag 27
int GameEventScript::fn_80395F34(const gs::EventSphere* sphere) {
    if (sphere->m24 != 0 && lbl_8087F430 != 0) {
        int on = lbl_8087F430->m5694 != 0 ? (sphere->m24 & 2) != 0 : (sphere->m24 & 1) != 0;
        if (on == 0) {
            return -1;
        }
    }
    int flag = lbl_8087F430->fn_80370174(27);
    int index = -1;
    for (int i = sphere->m18.size() - 1; i >= 0; i--) {
        if (CanStart_unk(FindEvent_unk(sphere->m18[i]))) {
            index = i;
            break;
        }
    }
    if (flag != 0 && index >= 0) {
        int found = 0;
        const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)FindEvent_unk(sphere->m18[index]);
        const ScriptEventCond_unk* cond = e->mConds;
        for (int i = 0; i < e->mCount; i++, cond++) {
            if (cond->m00 == 1 && cond->m04 == 27) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            index = -1;
        }
    }
    return index;
}

int GameEventScript::fn_80396298(const gs::EventBox* box) {
    if (box->m44 != 0 && lbl_8087F430 != 0) {
        int on = lbl_8087F430->m5694 != 0 ? (box->m44 & 2) != 0 : (box->m44 & 1) != 0;
        if (on == 0) {
            return -1;
        }
    }
    int flag = lbl_8087F430->fn_80370174(27);
    int index = -1;
    for (int i = box->m24.size() - 1; i >= 0; i--) {
        if (CanStart_unk(FindEvent_unk(box->m24[i]))) {
            index = i;
            break;
        }
    }
    if (flag != 0 && index >= 0) {
        int found = 0;
        const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)FindEvent_unk(box->m24[index]);
        const ScriptEventCond_unk* cond = e->mConds;
        for (int i = 0; i < e->mCount; i++, cond++) {
            if (cond->m00 == 1 && cond->m04 == 27) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            index = -1;
        }
    }
    return index;
}

// inferred: the warp point `id` of the scene (name invented)
static inline gs::WarpPoint* FindWarpIn_unk(GameScene* scene, int id) {
    for (unsigned int i = 0; i < scene->m78.size(); i++) {
        if (id == scene->m78[i].mId) {
            return &scene->m78[i];
        }
    }
    return 0;
}

// inferred: bit 25 of the flag word at 0x12A4 through an accessor (reloaded before it is cleared; as in
// gmk/ReinforcePoint.cpp)
inline unsigned int IsFlag25_unk(const CharaTask* chara) {
    return chara->mFlag25;
}

// inferred: the first character of the list of each manager (names invented)
static inline NpcTask* FirstChara_unk(NpcManager* manager) {
    return manager->mHead;
}
static inline EnemyTask* FirstChara_unk(EnemyManager* manager) {
    return manager->mHead;
}
static inline PlayerTask* FirstChara_unk(FriendManager* manager) {
    return (PlayerTask*)manager->fn_8036554C();
}
// inferred: the type of the characters of a manager (name invented)
template <class Manager>
struct PointChara_unk;
template <>
struct PointChara_unk<NpcManager> {
    typedef NpcTask Chara;
};
template <>
struct PointChara_unk<EnemyManager> {
    typedef EnemyTask Chara;
};
template <>
struct PointChara_unk<FriendManager> {
    typedef PlayerTask Chara;
};

// inferred: one of the scripts of the point has an event in range; for a talk point (bit 11 of m12C) one that
// can start by its flags (name invented)
inline int GameEventScript::HasPointEvent_unk(const gs::NpcSetPoint& point) {
    int any = ((point.m12C >> 11) & 1) ^ 1;
    for (unsigned int j = 0; j < point.m140.size(); j++) {
        int event = FindEvent_unk(point.m140[j]);
        if (event != 0) {
            if (any != 0 && fn_80398530(event)) {
                return 1;
            }
            if (any == 0 && CanStartArg_unk(event, 1)) {
                return 1;
            }
        }
    }
    return 0;
}

// NONMATCHING: 98.5% (each copy) - registers (the original keeps the point in r21 and the event in r20;
// a zero is kept on the stack for m58C) and cmpwi of the flags
// inferred: picks the script of every character point (npcs, enemies or friends: `kind` 1-3) and starts,
// moves or stops its character; the friend `keep` is never stopped
template <class Manager>
void GameEventScript::UpdatePoints_unk(atn::SimpleArray<gs::NpcSetPoint>& points,
                                       atn::SimpleArray<ScriptNpcFlag_unk>& flags,
                                       atn::SimpleArray<ScriptLog_unk>& logs, Manager* manager, int kind,
                                       int check, CharaTask* keep) {
    typedef typename PointChara_unk<Manager>::Chara Chara;
    for (unsigned int i = 0; i < points.size(); i++) {
        gs::NpcSetPoint& point = points[i];
        if (check != 0 && lbl_8087FA20 != 0 && !HasPointEvent_unk(point)) {
            flags[i].mFlags |= 0x10;
        }
        if ((flags[i].mFlags & 0x10) == 0x10) {
            continue;
        }
        int index = fn_80396F44(&point);
        Chara* chara;
        for (chara = FirstChara_unk(manager); chara != 0; chara = chara->mNext) {
            if (point.mId == chara->m58) {
                break;
            }
        }
        const ScriptEventConds_unk* e =
            (const ScriptEventConds_unk*)(index >= 0 ? FindEvent_unk(point.m140[index]) : 0);
        if ((flags[i].mFlags & 2) != 2) {
            if (index >= 0) {
                if (chara != 0 && !chara->IsFlag_unk(1)) {
                    gs::WarpPoint* warp = FindWarpIn_unk(mScene, e->mC0);
                    if (warp != 0) {
                        chara->mPos = warp->mPos;
                        chara->mRot = atn::Vector3(0.0f, warp->m14, 0.0f);
                    }
                    chara->fn_80154654(chara->mC50, point.m24);
                    chara->fn_80176ACC();
                    flags[i].mFlags |= 2;
                }
            } else if (chara != 0 && chara->IsFlag_unk(2) && !chara->IsFlag_unk(4)) {
                chara->fn_801765D8();
            }
        } else if (index != flags[i].m00 && (chara != 0 || keep == 0 || point.mId != keep->m58)) {
            if (index >= 0) {
                if ((chara->mC50 != chara->mType || point.m24 != chara->mC54) && point.m24 != 0) {
                    chara->fn_80154654(chara->mType, point.m24);
                }
                if (chara->IsFlag_unk(4)) {
                    gs::WarpPoint* warp = FindWarpIn_unk(mScene, e->mC0);
                    if (warp != 0) {
                        chara->mPos = warp->mPos;
                        chara->mRot = atn::Vector3(0.0f, warp->m14, 0.0f);
                    } else {
                        chara->mPos = point.mPos;
                        chara->mRot = atn::Vector3(0.0f, point.m14, 0.0f);
                    }
                    if (chara->mType == 2 && (chara->mStatus.m0C & 0x20) == 0x20) {
                        bool reset = true;
                        if (flags[i].m00 >= 0 && index >= 0) {
                            // inferred: maps that keep the characters of the points as they are
                            MapTask* map = mScene != 0 ? mScene->m64 : 0;
                            if (map != 0 && map->mType == 2 &&
                                ((map->mId == 1 && map->mSubId == 1) || (map->mId == 3 && map->mSubId == 1) ||
                                 (map->mId == 12 && map->mSubId == 3) || (map->mId == 22 && map->mSubId == 1) ||
                                 (map->mId == 22 && map->mSubId == 2) || (map->mId == 36 && map->mSubId == 1) ||
                                 (map->mId == 36 && map->mSubId == 2) || (map->mId == 36 && map->mSubId == 3) ||
                                 (map->mId == 36 && map->mSubId == 4) || (map->mId == 39 && map->mSubId == 3))) {
                                reset = false;
                            }
                        }
                        if (GimmickManager::instance_ != 0 && GimmickManager::instance_->fn_803EE45C(chara)) {
                            reset = false;
                        }
                        if (reset) {
                            chara->mStatus.fn_8012D8B8();
                            chara->mStatus.m224 = chara->mStatus.mParamB.m98;
                            chara->m58C = 0;
                            chara->fn_800D246C(0);
                            if (IsFlag25_unk(chara)) {
                                chara->mFlag25 = 0;
                            }
                            chara->mFlag15 = 0;
                            chara->fn_80145334();
                        }
                    }
                    chara->fn_80176ACC();
                }
            } else if (!chara->IsFlag_unk(4)) {
                chara->fn_801765D8();
            }
        }
        if (chara != 0 && !chara->IsFlag_unk(4) && e != 0) {
            if ((e->mB8 & 8) == 8) {
                chara->m54C |= 0x400000;
            } else {
                chara->m54C &= ~0x400000;
            }
        }
        if (index != flags[i].m00) {
            flags[i].mFlags &= ~1;
            flags[i].mFlags &= ~8;
        }
        if (m37C != 0) {
            logs[i].m04 = kind;
            logs[i].m08 = point.mId;
            logs[i].m10 = flags[i].m00;
            logs[i].m14 = index;
        }
        flags[i].m00 = index;
    }
}

template void GameEventScript::UpdatePoints_unk<FriendManager>(atn::SimpleArray<gs::NpcSetPoint>&, atn::SimpleArray<ScriptNpcFlag_unk>&, atn::SimpleArray<ScriptLog_unk>&, FriendManager*, int, int, CharaTask*);

int GameEventScript::fn_80396F44(const gs::NpcSetPoint* point) {
    if (point->m13C != 0 && lbl_8087F430 != 0) {
        int on = lbl_8087F430->m5694 != 0 ? (point->m13C & 2) != 0 : (point->m13C & 1) != 0;
        if (on == 0) {
            return -1;
        }
    }
    int flag = lbl_8087F430->fn_80370174(27);
    int index = -1;
    for (int i = point->m140.size() - 1; i >= 0; i--) {
        if (CanStart_unk(FindEvent_unk(point->m140[i]))) {
            index = i;
            break;
        }
    }
    if (flag != 0 && index >= 0) {
        int found = 0;
        const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)FindEvent_unk(point->m140[index]);
        const ScriptEventCond_unk* cond = e->mConds;
        for (int i = 0; i < e->mCount; i++, cond++) {
            if (cond->m00 == 1 && cond->m04 == 27) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            index = -1;
        }
    }
    return index;
}

template void GameEventScript::UpdatePoints_unk<EnemyManager>(atn::SimpleArray<gs::NpcSetPoint>&, atn::SimpleArray<ScriptNpcFlag_unk>&, atn::SimpleArray<ScriptLog_unk>&, EnemyManager*, int, int, CharaTask*);
template void GameEventScript::UpdatePoints_unk<NpcManager>(atn::SimpleArray<gs::NpcSetPoint>&, atn::SimpleArray<ScriptNpcFlag_unk>&, atn::SimpleArray<ScriptLog_unk>&, NpcManager*, int, int, CharaTask*);

// inferred: the field value is in the range of the event (name invented)
static inline bool InEventRange_unk(int event) {
    const ScriptEventRange_unk* e = (const ScriptEventRange_unk*)event;
    int value = lbl_8087F430->m10D0;
    return e->IsAlways_unk() || e->Contains_unk(value);
}

int GameEventScript::fn_80398530(int event) {
    return InEventRange_unk(event);
}


// inferred: the same test as fn_80398590, which the original inlines in UpdatePoints_unk (here a call
// would stay a call; name invented)
inline int GameEventScript::CanStartArg_unk(int event, int arg) {
    int ok = 0;
    if (event != 0) {
        const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)event;
        int value = lbl_8087F430->m10D0;
        bool pass = true;
        if (!e->IsAlways_unk() && !e->Contains_unk(value)) {
            pass = false;
        }
        if (pass) {
            ok = 1;
            const ScriptEventCond_unk* cond = e->mConds;
            for (int i = 0; i < e->mCount; cond++, i++) {
                if (arg == 0 || cond->m00 == 1) {
                    int r;
                    int id = e->m00;
                    if (cond->m00 < 0) {
                        ScriptDoneNode_unk* node = mDone.mRoot;
                        ScriptDoneNode_unk* found = mDone.End_unk();
                        while (node != 0) {
                            if (!(node->mValue < id)) {
                                found = node;
                                node = node->mLeft;
                            } else {
                                node = node->mRight;
                            }
                        }
                        if (found == mDone.End_unk() || id < found->mValue) {
                            found = mDone.End_unk();
                        }
                        r = found == mDone.End_unk();
                    } else {
                        r = fn_8039C7F0(cond->m00, cond->m04, cond->m10, cond->m08, cond->m0C);
                    }
                    if (r == 0) {
                        ok = 0;
                        break;
                    }
                }
            }
        }
    }
    return ok;
}

// NONMATCHING: 99.4% - volatile registers of the range test reversed (r5-r8), as fn_80398530
// inferred: the event can start (field value in range and all its conditions; arg 0 tests only the
// conditions of kind 1)
int GameEventScript::fn_80398590(int event, int arg) {
    int ok = 0;
    if (event != 0) {
        const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)event;
        int value = lbl_8087F430->m10D0;
        bool pass = true;
        if (!e->IsAlways_unk() && !e->Contains_unk(value)) {
            pass = false;
        }
        if (pass) {
            ok = 1;
            const ScriptEventCond_unk* cond = e->mConds;
            for (int i = 0; i < e->mCount; cond++, i++) {
                if (arg == 0 || cond->m00 == 1) {
                    int r;
                    int id = e->m00;
                    if (cond->m00 < 0) {
                        ScriptDoneNode_unk* node = mDone.mRoot;
                        ScriptDoneNode_unk* found = mDone.End_unk();
                        while (node != 0) {
                            if (!(node->mValue < id)) {
                                found = node;
                                node = node->mLeft;
                            } else {
                                node = node->mRight;
                            }
                        }
                        if (found == mDone.End_unk() || id < found->mValue) {
                            found = mDone.End_unk();
                        }
                        r = found == mDone.End_unk();
                    } else {
                        r = fn_8039C7F0(cond->m00, cond->m04, cond->m10, cond->m08, cond->m0C);
                    }
                    if (r == 0) {
                        ok = 0;
                        break;
                    }
                }
            }
        }
    }
    return ok;
}

// inferred: a flag of the field (kind 1: global flag, else fn_80370A78); 0 without a field (name invented)
static inline int FlagValue_unk(int kind, int flag) {
    if (lbl_8087F430 == 0) {
        return 0;
    }
    if (kind == 1) {
        return lbl_8087F430->fn_80370174(flag);
    }
    return lbl_8087F430->fn_80370A78(flag);
}

extern "C" void* lbl_8087EE98; // inferred: collision manager
extern "C" int fn_8004ED34(void* world, atn::ColliResult_unk* result, const atn::Vector3* from,
                           const atn::Vector3* to, unsigned int mask, void* ignore, int arg); // inferred: ray test

// inferred: the character can start a talk event (idle or walking without being busy, or the states 60-61
// of the control 6; names invented)
static inline bool CanStartTalk_unk(PlayerTask* chara) {
    switch (chara->m55C) {
    case 0:
    case 1:
    case 9:
    case 10:
        return !chara->mFlag31 && !chara->mFlag6 && chara->m14A0 <= 0 && chara->m1208 == 0;
    case 6:
        return (unsigned int)(chara->m560 - 60) <= 1;
    }
    return false;
}

// inferred: the field flag `flag` is set (0 without a field; name invented)
static inline bool FieldFlagOn_unk(int flag) {
    return lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(flag);
}

extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

// inferred: the talk events may start (an event is running, or the field flag 2590; name invented)
static inline bool TalkAllowed_unk(const GameEventScript* script) {
    bool on = true;
    if (script->mE0 <= 0 && !FieldFlagOn_unk(2590)) {
        on = false;
    }
    return on;
}

// NONMATCHING: 96.5% - as fn_8039AF50
// inferred: same as fn_8039AF50 for the talk event boxes (with m40, the leader must also face the box)
void GameEventScript::fn_8039AB10() {
    PlayerTask* leader = lbl_8087F8A0->mHead;
    atn::Sphere sphere;
    sphere.mCenter = leader->m614;
    sphere.mRadius = leader->m620;
    if (leader->m55C == 6 && leader->m560 == 11) {
        atn::ColliResult_unk result;
        atn::Vector3 to = sphere.mCenter + atn::Vector3(0.0f, -100.0f, 0.0f);
        if (fn_8004ED34(lbl_8087EE98, &result, &sphere.mCenter, &to, 0x80000000, 0, 0)) {
            sphere.mCenter = result.m10 + atn::Vector3(0.0f, sphere.mRadius, 0.0f);
        }
    }
    GameScene* scene = mScene;
    for (unsigned int i = 0; i < scene->mE4.size(); i++) {
        gs::EventBox& box = scene->mE4[i];
        int group = i;
        if (box.m2C >= 0) {
            group = box.m2C;
        }
        ScriptNpcFlag_unk& state = mFC[group];
        if (state.m00 >= 0) {
            state.mFlags &= ~0x20;
            const ScriptEventConds_unk* e =
                (const ScriptEventConds_unk*)FindEvent_unk(scene->mE4[group].m24[state.m00]);
            if (e != 0 && e->m0C == 0) {
                bool facing = true;
                if (box.m40 != 0) {
                    Mtx m;
                    atn::Vector3 front(0.0f, 0.0f, 1.0f);
                    PSMTXRotRad(m, 'y', 3.1415927f + box.m14);
                    PSMTXMultVec(m, &front, &front);
                    Mtx r;
                    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                    PSMTXRotRad(r, 'y', leader->mRot.y);
                    PSMTXMultVec(r, &dir, &dir);
                    facing = PSVECDotProduct(&front, &dir) > 0.70710677f;
                }
                if ((facing & fn_803ACE18(i, &sphere, 0)) && CanStartTalk_unk(leader) && TalkAllowed_unk(this)) {
                    state.mFlags |= 0x20;
                    state.m08 = -1.0f;
                    if (mF4 == 0 || state.m08 < mF4->m08) {
                        mF4 = &state;
                    }
                }
            }
        }
    }
}

// NONMATCHING: 95.9% - stack slots of the ray test vectors and registers of the loop
// inferred: marks the talk event spheres the leader stands in (flag 0x20) and keeps the nearest in mF4;
// while hanging (control 6, state 11) the leader is projected on the ground below
void GameEventScript::fn_8039AF50() {
    PlayerTask* leader = lbl_8087F8A0->mHead;
    atn::Sphere sphere;
    sphere.mCenter = leader->m614;
    sphere.mRadius = leader->m620;
    if (leader->m55C == 6 && leader->m560 == 11) {
        atn::ColliResult_unk result;
        atn::Vector3 to = sphere.mCenter + atn::Vector3(0.0f, -100.0f, 0.0f);
        if (fn_8004ED34(lbl_8087EE98, &result, &sphere.mCenter, &to, 0x80000000, 0, 0)) {
            sphere.mCenter = result.m10 + atn::Vector3(0.0f, sphere.mRadius, 0.0f);
        }
    }
    GameScene* scene = mScene;
    for (unsigned int i = 0; i < scene->mEC.size(); i++) {
        int group = i;
        if (scene->mEC[i].m20 >= 0) {
            group = scene->mEC[i].m20;
        }
        ScriptNpcFlag_unk& state = m10C[group];
        if (state.m00 >= 0) {
            state.mFlags &= ~0x20;
            const ScriptEventConds_unk* e =
                (const ScriptEventConds_unk*)FindEvent_unk(scene->mEC[group].m18[state.m00]);
            if (e != 0 && e->m0C == 0 && fn_803ACF88(i, &sphere, 0) && CanStartTalk_unk(leader) &&
                TalkAllowed_unk(this)) {
                state.mFlags |= 0x20;
                state.m08 = -1.0f;
                if (mF4 == 0 || state.m08 < mF4->m08) {
                    mF4 = &state;
                }
            }
        }
    }
}

extern "C" const char* const lbl_8074EB1C[]; // inferred: names of the script commands
extern "C" const char* const lbl_8074EFC8[]; // inferred: names of the kinds of event

// NONMATCHING: 99.97% - offsets in @stringBase0 (the strings of the unwritten functions before it are missing)
// inferred: debug display of the running event (kind, id, page and command)
void GameEventScript::vf14() {
    if (lbl_8087F0A8->mDebugSceneFlag != 0) {
        lbl_8087EEB0->fn_80060D58(320.0f, 240.0f, 0.0f, 300.0f, 28.0f, 0x80000000);
        if (m8C != 0) {
            char text[256];
            sprintf(text, "%s  ID:%d  PAGE:%d", lbl_8074EFC8[m9C], mA0, mA4 + 1);
            lbl_8087EEB0->fn_800616C0(330.0f, 240.0f, 0.0f, 14.0f, 14.0f, text, 0xFFFFFFFF, 0.0f, 0, 0, 0);
            if (m94 != 0 && m90 != 0) {
                sprintf(text, "COM:%s", lbl_8074EB1C[m90->mCmd]);
                lbl_8087EEB0->fn_800616C0(330.0f, 254.0f, 0.0f, 14.0f, 14.0f, text, 0xFFFFFFFF, 0.0f, 1, 1, 0);
            }
        }
    }
}

// inferred: header of a compiled script file: its events (id, offset) from 0xC (names invented)
struct ScriptFileHeader_unk {
    int m00;
    int mCount; // inferred: number of events
    int m08;
};

// inferred: MSL map::operator[] of the events by id (name invented)
inline int& GameEventScript::EventOffset_unk(const int& key) {
    ScriptEventNode_unk* pred;
    bool left;
    bool leftmost;
    ScriptEventNode_unk* parent = mEvents.FindInsertPos_unk(key, pred, left, leftmost);
    ScriptEventPair_unk* v;
    if (pred == 0 || (v = &pred->mValue, v->first < key)) {
        v = &mEvents.InsertNode_unk(parent, left, leftmost, ScriptEventPair_unk(key, int()))->mValue;
    }
    return v->second;
}

// NONMATCHING: 96.7% - the load of the count is scheduled after the multiply
// inferred: adds the events of the script file `index` (ids of the block index * 10000); noinline: vf0C
// calls it
__declspec(noinline) void GameEventScript::fn_8039BDB4(int index, const void* data, int size) {
    int count = ((const ScriptFileHeader_unk*)data)->mCount;
    m64[index] = (int)data;
    m60 += count;
    m70[index] = (const ScriptEventEntry_unk*)((const char*)data + sizeof(ScriptFileHeader_unk));
    for (int i = 0; i < count; i++) {
        EventOffset_unk(index * 10000 + m70[index][i].mId) = m70[index][i].mOffset;
    }
}

// inferred: compares the flag `flag` (kind 1: fn_80370174, else fn_80370A78) with the script value
// type/value (op 5 ==, 6 !=, 7 <, 8 <=, 9 >, 10 >=)
int GameEventScript::fn_8039C7F0(int kind, int flag, int op, int type, int value) {
    if (lbl_8087F430 == 0) {
        return 0;
    }
    int result = 0;
    switch (op) {
    case 5:
        result = FlagValue_unk(kind, flag) == ScriptValue_unk(type, value);
        break;
    case 6:
        result = FlagValue_unk(kind, flag) != ScriptValue_unk(type, value);
        break;
    case 7:
        result = FlagValue_unk(kind, flag) < ScriptValue_unk(type, value);
        break;
    case 8:
        result = FlagValue_unk(kind, flag) <= ScriptValue_unk(type, value);
        break;
    case 9:
        result = FlagValue_unk(kind, flag) > ScriptValue_unk(type, value);
        break;
    case 10:
        result = FlagValue_unk(kind, flag) >= ScriptValue_unk(type, value);
        break;
    }
    return result;
}

inline int GameEventScript::FindEvent_unk(int id) {
    ScriptEventNode_unk* found = mEvents.End_unk();
    ScriptEventNode_unk* node = mEvents.mRoot;
    while (node != 0) {
        if (!(node->mValue.first < id)) {
            found = node;
            node = node->mLeft;
        } else {
            node = node->mRight;
        }
    }
    if (found == mEvents.End_unk() || id < found->mValue.first) {
        found = mEvents.End_unk();
    }
    if (found != mEvents.End_unk()) {
        int* base = &m64[id / 10000];
        return *base + found->mValue.second;
    }
    return 0;
}

int GameEventScript::fn_8039CC24(int id) {
    return FindEvent_unk(id);
}

// inferred: handlers of the commands, by index (member function pointers in .rodata)
typedef void (GameEventScript::*ScriptCmdFn_unk)(ScriptResult_unk& result, const ScriptCmd_unk* cmd);
extern "C" const ScriptCmdFn_unk lbl_8074F0B0[];

// NONMATCHING: 89.8% - block layout of the two returns 0 and the registers of the inlined recursive call
// (the original keeps &m94->mCmds and reloads the count)
// inferred: the first enabled command from `cmd` (skipping commands 9) is a choice (7, 8, or 15 with
// GameConfig_unk::m0B8)
int GameEventScript::fn_8039CCD0(const ScriptCmd_unk* cmd) {
    if (cmd != 0) {
        ScriptCmd_unk* end = &m94->mCmds;
        for (unsigned int i = 0; i < m94->mCmdCount; i++) {
            end = (ScriptCmd_unk*)((char*)end + lbl_8074ED24[end->mCmd]);
        }
        if (cmd < end) {
            if ((cmd->m04 & 1) == 1 || cmd->mCmd == 9) {
                return fn_8039CCD0((const ScriptCmd_unk*)((const char*)cmd + lbl_8074ED24[cmd->mCmd]));
            }
            return cmd->mCmd - 7 <= 1 || (lbl_8087F0A8->m0B8 != 0 && cmd->mCmd == 15);
        }
    }
    return 0;
}

// NONMATCHING: 99.2% - `end` and `count` take r30/r29 swapped
// inferred: runs the commands of `data` from `cmd`; 0 when a command waits, 1 at the end
int GameEventScript::fn_8039CF8C(ScriptData_unk* data, ScriptCmd_unk* cmd) {
    m94 = data;
    ScriptCmd_unk* end = &data->mCmds;
    unsigned int count = data->mCmdCount;
    for (unsigned int i = 0; i < count; i++) {
        end = (ScriptCmd_unk*)((char*)end + lbl_8074ED24[end->mCmd]);
    }
    for (unsigned int n = 0; n < count; n++) {
        if (cmd == 0 || cmd >= end) {
            break;
        }
        m90 = cmd;
        ScriptResult_unk result;
        if ((cmd->m04 & 1) == 1) {
            result.SetNext_unk(cmd);
        } else {
            switch (cmd->mCmd) {
            case 1: case 3: case 4: case 5: case 7: case 8: case 13: case 15: case 16: case 17: case 20:
            case 25: case 28: case 29: case 30: case 31: case 36: case 39: case 40: case 41: case 43: case 44:
            case 45: case 46: case 56: case 57: case 58: case 60: case 61: case 62: case 79: case 83: case 86:
            case 91: case 92: case 93: case 94: case 95: case 96: case 97: case 101: case 106: case 108:
            case 109:
                if (mE8 > 0) {
                    lbl_8087F540->fn_8047F580();
                    mE8 = 0;
                    mEC = 0;
                }
                break;
            }
            (this->*lbl_8074F0B0[cmd->mCmd])(result, cmd);
        }
        if (result.mWait != 0) {
            return 0;
        }
        cmd = result.mNext;
    }
    return 1;
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

static inline GameScene* FieldScene_unk() {
    return lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
}

// inferred: puts a character on a warp point of the scene, keeping its collision flag (name invented)
static inline void WarpChara_unk(CharaTask* chara, gs::WarpPoint* warp) {
    if (chara->mFlag31) {
        chara->fn_801539E0();
    }
    if (chara->mFlag18) {
        chara->vf9C(0);
        chara->vf98();
    }
    chara->mPos = warp->mPos;
    chara->mRot = atn::Vector3(0.0f, warp->m14, 0.0f);
    int colli = chara->mColli.mFlags & 1;
    chara->fn_80145334();
    if (colli != 0) {
        chara->mColli.mFlags |= 1;
    } else {
        chara->mColli.mFlags &= ~1;
    }
}

// NONMATCHING: 98.7% - the warp point takes r31 and cmd/result r30/r29 (original: cmd r31, result r30,
// warp r29)
// inferred: warps a character (m0C: 0 the leader, 1 the npc, 2 the enemy, 3 the friend with id m10) to
// the warp point m08 of the scene
void GameEventScript::fn_8039D4C4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    gs::WarpPoint* warp = FindWarpIn_unk(mScene, args->m08);
    if (args->m0C == 0) {
        CharaTask* leader = lbl_8087F8A0->mHead;
        WarpChara_unk(leader, warp);
        lbl_8087F430->mCamera.fn_8037EF30(1, 1, -1.0f);
    } else if (args->m0C == 1) {
        for (NpcTask* npc = NpcManager::sInstance->mHead; npc != 0; npc = npc->mNext) {
            if (args->m10 == npc->m58) {
                WarpChara_unk(npc, warp);
                break;
            }
        }
    } else if (args->m0C == 2) {
        for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
            if (args->m10 == enemy->m58) {
                WarpChara_unk(enemy, warp);
                break;
            }
        }
    } else if (args->m0C == 3) {
        for (CharaTask* chara = FriendManager::sInstance->fn_8036554C(); chara != 0;
             chara = ((EnemyTask*)chara)->mNext) {
            if (args->m10 == chara->m58) {
                WarpChara_unk(chara, warp);
                break;
            }
        }
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

// NONMATCHING: 91.3% - registers of the inlined fn_8039CCD0 (as fn_8039E328)
// inferred: waits until the message is read (or m10 frames, if positive), then goes on
void GameEventScript::fn_8039DB2C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    GameMessageWindow* window = lbl_8087F490->m263C;
    if (mA8 != 0) {
        result.SetNext_unk(cmd, 1);
        mB4++;
        if (window->fn_803B5950() || (mB4 >= args->m10 && args->m10 > 0)) {
            fn_803935FC(2, 1.0f);
            mB4 = args->m10;
            mA8 = 0;
            if (!fn_8039CCD0(result.mNext)) {
                lbl_8087F490->m263C->fn_803B57B0(0);
                lbl_8087F8A0->mHead->fn_8016E970(1);
            }
        }
    } else if (mB4 >= args->m10) {
        result.mNext = NextCmd_unk(cmd);
        int state = window->mState;
        result.mWait = !(state == 2 || state == 4 || state == 0);
    } else {
        result.SetNext_unk(cmd, 1);
        if (++mB4 >= args->m10) {
            if (!fn_8039CCD0(result.mNext)) {
                lbl_8087F490->m263C->fn_803B57B0(0);
                lbl_8087F8A0->mHead->fn_8016E970(1);
            }
        }
    }
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

// inferred: arguments of the choice command (names invented)
struct ScriptChoiceArgs_unk : public ScriptCmd_unk {
    int m08;
    int mKind[4];  // 0x0C: 0 goes on, 1 ends, else jumps to the label mLabel
    int mLabel[4]; // 0x1C
    int mTimeout;  // 0x2C: frames before the last choice is taken (0: none)
};

// NONMATCHING: 92% - the inlined fn_8039CCD0 differs as that function does, and the label search
// branches/registers
// inferred: waits for the choice of the message window and picks the next command from it
void GameEventScript::fn_8039E328(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptChoiceArgs_unk* args = static_cast<const ScriptChoiceArgs_unk*>(cmd);
    GameMessageWindow* window = lbl_8087F490->m263C;
    if (mA8 != 0) {
        result.SetNext_unk(cmd, 1);
        int choice = -1;
        mB4++;
        if (window->mChoices->fn_803B4C24()) {
            choice = window->mChoices->mChoice;
            fn_803935FC(0, 1.0f);
        }
        if (mB4 >= args->mTimeout && args->mTimeout > 0) {
            choice = window->mChoices->mCount - 1;
        }
        if (choice >= 0) {
            int kind = args->mKind[choice];
            if (kind == 0) {
                mAC = NextCmd_unk(cmd);
            } else if (kind == 1) {
                mAC = 0;
            } else {
                mAC = FindLabel_unk(args->mLabel[choice]);
            }
            mA8 = 0;
            window->fn_803B57EC();
            if (!fn_8039CCD0(mAC)) {
                lbl_8087F490->m263C->fn_803B57B0(0);
                lbl_8087F8A0->mHead->fn_8016E970(1);
            }
        }
    } else {
        result.mNext = mAC;
        int state = window->mState;
        result.mWait = !(state == 2 || state == 4 || state == 0);
    }
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

// NONMATCHING: 93.4% - registers of the inlined FindLabel_unk and the inlined fn_8039CCD0 (as fn_8039E328)
// inferred: waits for a yes/no choice of the message window (choice 0: m0C/m10, else m14/m18, as
// kind/label of fn_8039E328), after mBC frames
void GameEventScript::fn_8039EBB8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    GameMessageWindow* window = lbl_8087F490->m263C;
    int done = 0;
    if (mB8 == 0) {
        if (--mBC <= 0 && window->mChoices->fn_803B4C24()) {
            if (window->mChoices->mChoice == 0) {
                mB8 = 1;
                mC0 = args->m0C;
                mC4 = args->m10;
                window->fn_803B57EC();
            } else {
                mB8 = 1;
                mC0 = args->m14;
                mC4 = args->m18;
                window->fn_803B57EC();
            }
            fn_803935FC(0, 1.0f);
        }
    } else if (window->mChoices->mState == 0) {
        done = 1;
    }
    if (done != 0) {
        if (mC0 == 0) {
            result.mNext = NextCmd_unk(cmd);
        } else if (mC0 == 1) {
            result.mNext = 0;
        } else {
            result.mNext = FindLabel_unk(mC4);
        }
        if (lbl_8087F0A8->m0B8 != 0) {
            if (!fn_8039CCD0(result.mNext)) {
                lbl_8087F490->m263C->fn_803B57B0(0);
                lbl_8087F8A0->mHead->fn_8016E970(1);
            }
        } else {
            lbl_8087F8A0->mHead->fn_8016E970(1);
        }
        result.mWait = 0;
    } else {
        result.SetNext_unk(cmd, 1);
    }
}

// NONMATCHING: 98.5% - inside the inlined vector::push_back: @stringBase0 offset (earlier strings unwritten)
// and two stack slots of the growth computation swapped
// inferred: makes the character m10/m14 act (m08: 0 fn_8016F824, 1 fn_8016FDCC, 2 fn_8017039C with m0C)
// and, with m18, records the label m1C to run for it
void GameEventScript::fn_8039F08C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m10, args->m14);
    if (chara != 0 && !chara->IsFlag_unk(4) &&
        !(chara->mType == 2 && (chara->mStatus.m0C & 0x20) == 0x20)) {
        switch (args->m08) {
        case 0:
            chara->fn_8016F824(args->m0C, 0);
            break;
        case 1:
            chara->fn_8016FDCC(args->m0C, 0);
            break;
        case 2:
            chara->fn_8017039C(args->m0C, 0);
            break;
        }
        if (args->m18 != 0) {
            fn_803AB37C(chara);
            ScriptData_unk* data = m94;
            ScriptRecord14_unk record;
            record.mChara = chara;
            record.mData = data;
            record.mLabel = FindLabel_unk(args->m1C);
            record.m0C = m9C;
            record.m10 = mA0;
            m12C.push_back(record);
        }
    }
    result.SetNext_unk(cmd);
}

// inferred: releases a character from the event (removes its record of m12C; the leader sets mD0)
// and sets its state m14 (CharaTask::fn_80154654)
void GameEventScript::fn_8039F600(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara != 0) {
        if (chara->mType == 0) {
            mD0 = 1;
        }
        fn_803AB37C(chara);
        chara->fn_80154654(chara->mC50, args->m14);
    }
    result.SetNext_unk(cmd);
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

// inferred: stops a character's current action (name invented)
static inline void StopAction_unk(CharaTask* chara, int keep) {
    if (keep != 0) {
        chara->fn_8014DEE4(0, 0, 0);
    } else {
        if (chara->IsFlag5_unk()) {
            chara->mFlag5 = 0;
        }
        if (chara->mFlag31) {
            chara->fn_801539E0();
        }
        chara->fn_8014EEC4(0);
    }
}

// NONMATCHING: 99% - operands of the fmuls of case 1 swapped
// inferred: stores a value of `chara` (args->m10) into the script variable m14/m18
void GameEventScript::fn_8039F9A4(CharaTask* chara, const ScriptArgs_unk* args) {
    if (chara == 0) {
        return;
    }
    int value = 0;
    switch (args->m10) {
    case 0: {
        float hp = chara->mStatus.m04;
        value = (int)hp * 100 / chara->mStatus.mParamB.m84;
        if (value == 0 && hp > 0.0f) {
            value = 1;
        }
        break;
    }
    case 1:
        value = 100.0f * chara->m568;
        break;
    case 2:
        value = chara->mStatus.mParamB.m9C & 1;
        break;
    case 3:
        value = chara->mC58.mC0;
        break;
    case 4:
        value = chara->mC58.mB4;
        break;
    case 5:
        value = chara->mC58.mD8 & 1;
        break;
    case 6:
        value = !(chara->m674 < 0);
        break;
    case 7:
        if (chara->mC58.m11C.m00 == 0 || chara->mC58.m11C.m00 == 2) {
            value = 0;
        } else if (chara->mC58.m11C.m00 == 1) {
            value = 2;
        } else {
            value = 1;
        }
        break;
    case 8: {
        CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (leader != 0) {
            atn::Vector3 d = chara->mPos - leader->mPos;
            value = PSVECMag(&d);
        }
        break;
    }
    case 9:
        value = (chara->m54C >> 13) & 1;
        break;
    case 10:
        value = chara->mStatus.m224;
        break;
    case 11:
        value = chara->mStatus.mParamA.m78;
        break;
    case 12:
        value = chara->mStatus.m1C;
        break;
    case 13:
        if (chara->mC58.m11C.m01 == 0 || chara->mC58.m11C.m01 == 2) {
            value = 0;
        } else if (chara->mC58.m11C.m01 == 1) {
            value = 2;
        } else {
            value = 1;
        }
        break;
    case 14:
        value = chara->mF14;
        break;
    case 15:
        value = chara->m5C->m122;
        break;
    case 16:
        if (chara->mC58.m11C.m00 == 0) {
            value = -1;
        } else if (chara->mC58.m11C.m00 == 1) {
            value = ((chara->mC58.m11C.m20 >> 2) & 1) + 1;
        } else {
            value = 0;
        }
        break;
    case 17:
        value = chara->mFlagB14;
        break;
    case 18:
        value = chara->mFlag28;
        break;
    }
    fn_8039BF04(args->m14, args->m18, 0, 0, value);
}

// inferred: sets a value of `chara` (args->m10) from the script value m14/m18 (counterpart of
// fn_8039F9A4)
void GameEventScript::fn_8039FBFC(CharaTask* chara, const ScriptArgs_unk* args) {
    if (chara == 0) {
        return;
    }
    int value = ScriptValue_unk(args->m14, args->m18);
    switch (args->m10) {
    case 0:
        chara->mStatus.m04 = (float)(value * chara->mStatus.mParamB.m84) / 100.0f;
        chara->mStatus.fn_801333E4();
        if ((float)value <= 0.0f && lbl_8087F048 != 0) {
            lbl_8087F048->fn_800FDE60(0, chara, atn::Vector3(0.0f, 1.0f, 0.0f));
        }
        if ((float)value > 99.0f && lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0 &&
            lbl_8087F430->GetCurrentMap()->mType == 2 && lbl_8087F430->GetCurrentMap()->mId == 0x43 &&
            lbl_8087F430->GetCurrentMap()->mSubId == 1) {
            chara->mStatus.fn_8012D8B8();
        }
        break;
    case 1:
        chara->m568 = (float)value / 100.0f;
        break;
    case 2:
        if (value != 0) {
            chara->mStatus.mParamB.m9C |= 1;
        } else {
            chara->mStatus.mParamB.m9C &= ~1;
        }
        break;
    case 3:
        chara->mC58.mC0 = value;
        break;
    case 4:
        chara->mC58.mB4 = value;
        break;
    case 5:
        if (value != 0) {
            chara->mC58.mD8 |= 1;
        } else {
            chara->mC58.mD8 &= ~1;
        }
        break;
    case 6:
        if (value != 0 && chara->mParts.mCount != 0) {
            chara->fn_8014DEE4(0, 0, 0);
        } else {
            chara->fn_8014EEC4(0);
        }
        break;
    case 7:
        if (chara->mType == 2) {
            chara->mC58.m11C.fn_8011CD84(chara, value);
        }
        break;
    case 9:
        if (value != 0) {
            chara->m54C |= 0x2000;
        } else {
            chara->m54C &= ~0x2000;
        }
        break;
    case 10:
        chara->mStatus.m224 = value;
        if (value < 5) {
            chara->mStatus.m300 = 0;
        }
        break;
    case 11:
        chara->mStatus.m1A8.m08 += (float)(value - chara->mStatus.mParamA.m78);
        chara->mStatus.fn_8012B3E8();
        chara->mStatus.fn_8012B988();
        chara->mStatus.fn_8012D8B8();
        break;
    case 12:
        chara->mStatus.fn_80133B30(value);
        chara->mStatus.fn_8012D8B8();
        break;
    case 13:
        if (chara->mType == 2) {
            if (value == 1) {
                chara->mC58.m11C.m01 = 3;
                if (!chara->IsFlag_unk(4)) {
                    fn_800C31F4(lbl_8074EF78[6], 0, -1, 1.0f);
                }
            } else if (value == 2) {
                chara->mC58.m11C.m01 = 1;
                if (!chara->IsFlag_unk(4)) {
                    fn_800C31F4(lbl_8074EF78[7], 0, -1, 1.0f);
                }
            } else {
                chara->mC58.m11C.m01 = 0;
            }
        }
        break;
    case 14:
        chara->mF18 = chara->mF14;
        chara->mF14 = value;
        break;
    case 17:
        chara->mFlagB14 = value != 0;
        break;
    }
}

// inferred: stops the leader and every enemy and friend nearer than m0C to it
void GameEventScript::fn_803A0040(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* leader = lbl_8087F8A0->mHead;
    StopAction_unk(leader, args->m08);
    {
        float range = args->m0C;
        int keep = args->m08;
        range = range * range;
        for (EnemyTask* chara = EnemyManager::sInstance->mHead; chara != 0; chara = chara->mNext) {
            atn::Vector3 d = chara->mPos - leader->mPos;
            if (SqLength_unk(d) < range) {
                StopAction_unk(chara, keep);
            }
        }
    }
    {
        float range = args->m0C;
        int keep = args->m08;
        CharaTask* chara = FriendManager::sInstance->fn_8036554C();
        range = range * range;
        for (; chara != 0; chara = ((EnemyTask*)chara)->mNext) {
            atn::Vector3 d = chara->mPos - leader->mPos;
            if (SqLength_unk(d) < range) {
                StopAction_unk(chara, keep);
            }
        }
    }
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

// inferred: the first non-null of four characters (name invented)
static inline CharaTask* FirstOf_unk(CharaTask* a, CharaTask* b, CharaTask* c, CharaTask* d) {
    if (a != 0) {
        return a;
    }
    if (b != 0) {
        return b;
    }
    if (c != 0) {
        return c;
    }
    return d;
}

// NONMATCHING: 99.2% - only the @stringBase0 offset (0xE2): the earlier strings belong to unwritten functions
// inferred: makes a character use a gimmick (m10, m14); the party member 3 with id <= 0 is the
// first archer, then the first of kind 6, then 1/9/10, then the one with data m122 == 2
void GameEventScript::fn_803A064C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = 0;
    if (args->m08 != 3 || args->m0C > 0) {
        chara = GetChara_unk(args->m08, args->m0C);
    } else if (lbl_8087F8A0 != 0) {
        chara = 0;
        CharaTask* second = 0;
        CharaTask* third = 0;
        CharaTask* fourth = 0;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if (p->mType == 0 || p->IsFlag_unk(4) || (p->m54C & 0x2000) == 0x2000) {
                continue;
            }
            switch (fn_80219558(p->m50)) {
            case 4:
                chara = p;
                break;
            case 6:
                second = p;
                break;
            case 1:
            case 9:
            case 10:
                third = p;
                break;
            default:
                if (p->m5C->m122 == 2) {
                    fourth = p;
                }
                break;
            }
        }
        chara = FirstOf_unk(chara, second, third, fourth);
    }
    if (chara != 0) {
        lbl_8087EE68->fn_80017EB0(chara, args->m10, args->m14);
    } else if (lbl_8087EEB8 != 0) {
        char text[0x100];
        sprintf(text, "ScriptError: ExecComAimGimmick chara not found [gm%03d_%03d_%04d]", args->m10 / 1000,
                args->m10 % 1000, args->m14);
        lbl_8087EEB8->fn_800697D8(text);
    }
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

// inferred: sets a value of the leader (m0C == 0) or of the team AI m08/m0C
void GameEventScript::fn_803A0F4C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    if (args->m0C == 0) {
        CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (leader != 0) {
            switch (args->m10) {
            case 4:
                leader->mC58.mC0 = ScriptValue_unk(args->m14, args->m18);
                break;
            case 5:
                leader->mC58.mB4 = ScriptValue_unk(args->m14, args->m18);
                break;
            }
        }
    } else {
        TeamAI_unk* team = lbl_8087FA00->Find_unk(args->m08, args->m0C);
        if (team != 0) {
            int value = ScriptValue_unk(args->m14, args->m18);
            switch (args->m10) {
            case 0:
                team->fn_805AE864(value);
                break;
            case 3:
                team->SetAIType_unk(value);
                break;
            case 4:
                team->SetBattleMode_unk(value);
                break;
            case 5:
                team->SetAreaID_unk(value);
                break;
            case 6:
                team->SetM242_unk(value);
                break;
            default:
                return;
            }
        }
    }
    result.SetNext_unk(cmd);
}

// inferred: ends the event `id` (hold: the leader drops flag 0x10; successive: argument of
// fn_8047EFD8); always 1
extern "C" int fn_803A19B4(int id, int hold, int successive) {
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (hold != 0 && leader != 0) {
        leader->m54C &= ~0x10;
    }
    leader->fn_80164DCC(0);
    lbl_8087F540->fn_8047EFD8(id, successive);
    if (lbl_8087F540->fn_8047F420(id) != 0) {
        lbl_8087F430->fn_80371674();
        if (lbl_8087F498 != 0) {
            lbl_8087F498->fn_803EBAC8(15, 0);
            if (lbl_8087F430->GetCurrentMap() != 0 && lbl_8087F430->GetCurrentMap()->GetMapType() == 1) {
                lbl_8087F498->mNumChannels = 2;
            } else {
                lbl_8087F498->mNumChannels = 1;
            }
        }
        if (id == 0xEC5) {
            lbl_8087EFA8->m144 = 1;
        }
        if (id == 0x151E) {
            atn::RenderManager::instance_->mLayers[4]->m04 = 1;
        }
        if (id == 0x524) {
            for (PlayerTask* p = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0; p != 0; p = p->mNext) {
                p->fn_8016F634();
            }
        }
    }
    if (lbl_8087F098 != 0) {
        lbl_8087F098->fn_80183948();
    }
    return 1;
}

// inferred: starts an event (id, m18, m0C of the command); 1 if it started
extern "C" int fn_803A11D8(int id, int arg, int arg2, EventIdEntry_unk* entry);
// inferred: keeps the leader's state while an event with flag 0x10 runs (name invented)
static inline void HoldLeader_unk(int& saved, EventIdEntry_unk* entry, CharaTask* leader) {
    if ((entry->mFlags & 0x10) == 0x10 && leader != 0) {
        saved = leader->fn_8017C9A0();
        leader->m54C |= 0x10;
    }
}

// NONMATCHING: 99.9% - only the @stringBase0 offset (the earlier strings belong to unwritten functions)
// inferred: starts the event m08 (flag 2: queued, flag 0x40000000: stops it instead); the result
// is 1 while it runs
void GameEventScript::fn_803A1B20(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    CharaTask* leader;
    int wait;
    EventIdEntry_unk* entry;
    mB4 = 0;
    wait = 0;
    entry = EventIdTable_unk::fn_8020F130()->fn_8020F71C(args->m08);
    leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (entry != 0 && (entry->m44 == 1 || entry->m44 == 3) && ((unsigned int)args->m10 & 2) != 2 &&
        ((unsigned int)args->m10 & 0x40000000) != 0x40000000 && lbl_8087F540->fn_8047F420(args->m08) != 0) {
        mB4 = -1;
        lbl_8087F430->m563C = 0x12;
        lbl_8087F430->fn_80373118();
        wait = 1;
        HoldLeader_unk(mC4, entry, leader);
    }
    if (mB4 != -1) {
        if (((unsigned int)args->m10 & 2) == 2) {
            lbl_8087F540->fn_8047E964(args->m08, 1);
        } else if (((unsigned int)args->m10 & 0x40000000) != 0x40000000) {
            if (entry != 0) {
                wait = fn_803A11D8(args->m08, args->m18, args->m0C, entry);
                HoldLeader_unk(mC4, entry, leader);
                mC0 = 0;
                Event::Sequence* seq = lbl_8087F540->m70;
                if (wait != 0 && seq != 0 && seq->m98_8 == 1 && leader != 0) {
                    leader->m54C |= 0x10;
                    mC0 = 1;
                }
            } else {
                char text[0x100];
                sprintf(text, "Event not found! ev%04d", args->m08);
                if (lbl_8087EEB8 != 0) {
                    lbl_8087EEB8->fn_800697D8(text);
                }
            }
        } else {
            int id = args->m08;
            if (id <= 0 && lbl_8087F540->m70 != 0) {
                id = lbl_8087F540->m70->m190;
            }
            lbl_8087F540->fn_8047EFD8(id, 0);
        }
        if (wait != 0 && lbl_8087F540->m70 != 0 && lbl_8087F540->m70->m194 != 10) {
            lbl_8087F430->fn_80373118();
        }
    }
    result.SetNext_unk(cmd, wait);
}

// NONMATCHING: 99.8% - r0/r5 swap in SetNext_unk (docs/mwcc-blockers.md, GameEventScript family)
// inferred: waits for the event started by fn_803A1B20: the first frame starts it, then waits until
// it ends and restores the camera and the leader
void GameEventScript::fn_803A1DB0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    EventIdEntry_unk* entry = EventIdTable_unk::fn_8020F130()->fn_8020F71C(args->m08);
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    result.SetNext_unk(cmd, 1);
    if (mB4 == -1) {
        fn_803A11D8(args->m08, args->m18, args->m0C, entry);
        if (entry->m44 == 1) {
            lbl_8087F430->fn_8037529C(0xFFFFFFFF, 0xFFFFFF, 0, 0, 15);
        }
        mC0 = 0;
        Event::Sequence* seq = lbl_8087F540->m70;
        if (seq != 0 && seq->m98_8 == 1 && leader != 0) {
            leader->m54C |= 0x10;
            mC0 = 1;
        }
        lbl_8087F430->fn_80373118();
        mB4++;
        return;
    }
    mB4++;
    if (entry != 0 && mB4 > 15 && entry->m44 != 3) {
        int fade = lbl_8087F540->m1AA0 == 0;
        if (lbl_8087F540->m1AA0 == 2 && (args->m08 == 0x232F || args->m08 == 0x12C3 || args->m08 == 0x5E4)) {
            fade = 1;
        }
        if (fade) {
            lbl_8087F430->m563C = 0x12;
        }
    }
    if (lbl_8087F540->fn_8047F364(args->m08) != 0) {
        entry = EventIdTable_unk::fn_8020F130()->fn_8020F71C(args->m08);
        if (entry != 0) {
            switch (entry->m48) {
            case 1:
                lbl_8087F430->mCamera.fn_8037EF30(1, 1, -1.0f);
                lbl_8087F430->fn_8037529C(0xFFFFFFFF, 0xFFFFFF, 0, 0, 30);
                break;
            case 2:
                lbl_8087F430->fn_8037529C(0xFFFFFFFF, 0xFFFFFF, 5, 0, 10);
                fn_80392D2C(&lbl_8087F430->mCamera, lbl_8087F540->GetCamera_unk(), 0.05f);
                lbl_8087F430->mCamera.fn_8037EF30(1, 1, -1.0f);
                break;
            default:
                lbl_8087F430->mCamera.fn_8037EF30(1, 1, -1.0f);
                break;
            }
            if ((entry->mFlags & 0x10) == 0x10 && leader != 0) {
                if (mC4 != 0) {
                    leader->m54C |= 0x10;
                } else {
                    leader->m54C &= ~0x10;
                }
            }
        }
        fn_803A19B4(args->m08, mC0 == 1, lbl_8087F0A8->mProcessSuccessiveEvent);
        if (lbl_8087F0A8->mProcessSuccessiveEvent != 0) {
            mE8 = args->m08;
        }
        mE0 = 0x24;
        result.mWait = 0;
    } else {
        Event::Sequence* seq = lbl_8087F540->m70;
        if (seq != 0) {
            if (seq->m194 != 10) {
                lbl_8087F430->fn_80373118();
            } else if (args->m08 == 0x324 || args->m08 == 0x4E0) {
                for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                    if (p->fn_8016E484() != 0 && (p->mStatus.m0C & 0x20) == 0x20) {
                        p->mStatus.fn_8012D8B8();
                        p->m58C = 0;
                    }
                }
            }
        }
    }
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
void GameEventScript::fn_803A2600(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
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

// inferred: arguments of the turn/place command (names invented)
struct ScriptPlaceArgs_unk : public ScriptCmd_unk {
    int m08;
    int m0C;
    unsigned int m10; // inferred: kind (0..7)
    float m14;
    int m18;
    int m1C;
    float m20;
    unsigned int m24; // inferred: flags (bit 0: no wait, bit 1: instant, bit 2: keeps flag 0x200)
    int m28;
};

extern "C" GameTask* fn_80121F00(); // inferred: field task (lbl_8087F430), weak copy
extern "C" int fn_80267B20(CharaTask* chara); // inferred: returns m55C (game/GameTask.cpp)
extern "C" float fn_80011220(float x); // inferred: fabs (weak copy of another helper)
gs::AttentionPoint* FindAttention_unk(GameScene* scene, int id);

// NONMATCHING: 98.5% - the original calls the weak accessor fn_8013C504 (named after the Field_unk view
// in symbols.txt) where this loads GameTask::m10D8, and the weak lookup FindPlace_unk__14RouteTable_unk
// (RouteTable_unk is a view of the warp points of GameScene) where this calls FindWarpIn_unk; the
// store of mBC is scheduled after the first flag extraction
// inferred: turns or places the character m08/m0C (m10: 0 turns by m14, 1 turns to m14, 2 next to the
// character m18/m1C, 3 faces it, 4 scene position, 5 route place m28, 6 faces it, 7 faces the attention
// point m28); instant with bit 1 of m24, otherwise walking. Every inline is called (budget, R3)
void GameEventScript::fn_803A26F4(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptPlaceArgs_unk* args = static_cast<ScriptPlaceArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    int wait = 0;
    if (chara != 0) {
        unsigned int flags = args->m24;
        mBC = 0;
        wait = 1;
        int nowait = flags & 1;
        int instant = (flags >> 1) & 1;
        int keep = (flags >> 2) & 1;
        if (instant && chara->GetCharaNo_unk() == 0x92AE2 && fn_80267B20(chara) == 6) {
            chara->fn_8016EB48(0, 0, 0);
        }
        switch (args->m10) {
        case 0:
            if (!instant) {
                chara->fn_80170F20(chara->fn_8013C38C(), 0, args->m14 + chara->GetRot_unk()->y, -1.0f);
            } else {
                chara->SetRot_unk(*chara->GetRot_unk() + atn::Vector3(0.0f, args->m14, 0.0f));
            }
            break;
        case 1:
            if (!instant) {
                chara->fn_80170F20(chara->fn_8013C38C(), 0, args->m14, -1.0f);
            } else {
                atn::Vector3 rot(*chara->GetRot_unk());
                rot.y = args->m14;
                chara->SetRot_unk(rot);
            }
            break;
        case 2: {
            CharaTask* target = GetChara_unk(args->m18, args->m1C);
            float dist = MAX(args->m20, 5.0f + (chara->fn_8013C42C() + target->fn_8013C42C()));
            if (!instant) {
                chara->fn_80170A20(target, dist, 0);
            } else {
                atn::Vector3 dir = *chara->fn_8013C38C() - *target->fn_8013C38C();
                if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                    dir.Normalize_unk();
                }
                dir *= dist;
                chara->SetPosition_unk(*target->fn_8013C38C() + dir);
            }
            break;
        }
        case 3: {
            CharaTask* target = GetChara_unk(args->m18, args->m1C);
            if (target != 0) {
                atn::Vector3 dir = *target->fn_8013C38C() - *chara->fn_8013C38C();
                if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                    dir.Normalize_unk();
                }
                if (!instant) {
                    chara->fn_80170F20(chara->fn_8013C38C(), 0, dir.ToRotation_unk().y, -1.0f);
                } else {
                    atn::Vector3 rot(*chara->GetRot_unk());
                    rot.y = dir.ToRotation_unk().y;
                    chara->SetRot_unk(rot);
                }
            }
            break;
        }
        case 4: {
            GameScene* scene = fn_80121F00() != 0 ? fn_80121F00()->m10D8 : 0;
            if (scene != 0) {
                atn::Vector3 pos;
                atn::Vector3 rot;
                if (scene->fn_803CCA84(chara, &pos, &rot) != 0) {
                    if (!instant) {
                        chara->fn_80170F20(&pos, 0, rot.y, -1.0f);
                    } else {
                        chara->SetPosition_unk(pos);
                        chara->SetRot_unk(rot);
                    }
                }
            }
            break;
        }
        case 5: {
            float dist = MAX(args->m20, 5.0f + chara->fn_8013C42C()); // inferred: unused
            if (!instant) {
                chara->fn_8017039C(args->m28, 0);
            } else {
                GameScene* scene = fn_80121F00() != 0 ? fn_80121F00()->m10D8 : 0;
                gs::WarpPoint* warp = scene != 0 ? FindWarpIn_unk(scene, args->m28) : 0;
                if (warp != 0) {
                    chara->SetPosition_unk(warp->mPos);
                    chara->SetRot_unk(atn::Vector3(0.0f, warp->m14, 0.0f));
                }
            }
            break;
        }
        case 6: {
            GameScene* scene = fn_80121F00() != 0 ? fn_80121F00()->m10D8 : 0;
            gs::WarpPoint* warp = scene != 0 ? FindWarpIn_unk(scene, args->m28) : 0;
            if (warp != 0) {
                atn::Vector3 dir = warp->mPos - *chara->fn_8013C38C();
                if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                    dir.Normalize_unk();
                }
                if (!instant) {
                    chara->fn_80170F20(chara->fn_8013C38C(), 0, dir.ToRotation_unk().y, -1.0f);
                } else {
                    atn::Vector3 rot(*chara->GetRot_unk());
                    rot.y = dir.ToRotation_unk().y;
                    chara->SetRot_unk(rot);
                }
            }
            break;
        }
        case 7: {
            GameScene* scene = fn_80121F00() != 0 ? fn_80121F00()->m10D8 : 0;
            gs::AttentionPoint* point = scene != 0 ? FindAttention_unk(scene, args->m28) : 0;
            if (point != 0) {
                atn::Vector3 dir = point->mPos - *chara->fn_8013C38C();
                if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                    dir.Normalize_unk();
                }
                if (!instant) {
                    chara->fn_80170F20(chara->fn_8013C38C(), 0, dir.ToRotation_unk().y, -1.0f);
                } else {
                    atn::Vector3 rot(*chara->GetRot_unk());
                    rot.y = dir.ToRotation_unk().y;
                    chara->SetRot_unk(rot);
                }
            }
            break;
        }
        }
        if (nowait || instant) {
            wait = 0;
        }
        if (wait && keep) {
            mB8 = chara->fn_8013C404(0x200);
            chara->SetFlag(0x200);
        }
    }
    result.mNext = NextCmd_unk(cmd);
    result.mWait = wait;
}

// inferred: the point `id` of an array, null if missing (name invented)
static inline gs::AttentionPoint* FindPoint_unk(atn::SimpleArray<gs::AttentionPoint>& array, int id) {
    for (unsigned int i = 0; i < array.size(); i++) {
        if (id == array[i].mId) {
            return &array[i];
        }
    }
    return 0;
}

// inferred: the attention point `id` of the scene, null if missing (name invented)
gs::AttentionPoint* FindAttention_unk(GameScene* scene, int id) {
    return FindPoint_unk(scene->mFC, id);
}

// inferred: arguments of the wait-and-warp command (name invented)
struct ScriptWarpArgs_unk : public ScriptCmd_unk {
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    float m20;
    int m24;
    int m28;
};


// inferred: waits while the character m08/m0C moves (m10 <= 7); when it is stuck for 90 frames it is
// warped (2: next to the character m18/m1C at least m20 away, 4: to the scene position, 5: to the
// warp point m28)
void GameEventScript::fn_803A2E20(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptWarpArgs_unk* args = static_cast<ScriptWarpArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    int wait = 0;
    if (chara != 0) {
        wait = 1;
        if ((unsigned int)args->m10 <= 7) {
            if (chara->m570 > 0.1f && SqLength_unk(chara->m13B8[1] - chara->m13B8[0]) < 1.0f) {
                if (++mBC > 45 && (chara->m54C & 0x200) != 0x200) {
                    mB8 = (chara->m54C & 0x200) == 0x200;
                    chara->m54C |= 0x200;
                }
                if (mBC > 90) {
                    switch (args->m10) {
                    case 2: {
                        CharaTask* target = GetChara_unk(args->m18, args->m1C);
                        float r;
                        float c;
                        r = target->m5B0;
                        c = chara->m5B0;
                        float min = 5.0f + (c + r);
                        float dist = args->m20 > min ? args->m20 : min;
                        atn::Vector3 dir = chara->mPos - target->mPos;
                        if (!((float)__fabs(SqLength_unk(dir)) < 1.1920929e-07f)) {
                            PSVECNormalize(&dir, &dir);
                        }
                        dir *= dist;
                        chara->mPos = target->mPos + dir;
                        chara->fn_80171DB0();
                        break;
                    }
                    case 4: {
                        GameScene* scene = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
                        if (scene != 0) {
                            atn::Vector3 pos;
                            atn::Vector3 rot;
                            if (scene->fn_803CCA84(chara, &pos, &rot) != 0) {
                                chara->mPos = pos;
                                chara->mRot = rot;
                            }
                        }
                        chara->fn_80171DB0();
                        break;
                    }
                    case 5: {
                        GameScene* scene = FieldScene_unk();
                        gs::WarpPoint* warp = scene != 0 ? FindWarpIn_unk(scene, args->m28) : 0;
                        if (warp != 0) {
                            chara->mPos = warp->mPos;
                            chara->mRot = atn::Vector3(0.0f, warp->m14, 0.0f);
                        }
                        chara->fn_80171DB0();
                        break;
                    }
                    }
                }
            } else {
                mBC = 0;
            }
            if (chara->mRoute.m2C != 0) {
                wait = 0;
                if (mB8 == 0) {
                    chara->m54C &= ~0x200;
                }
            }
        }
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

// inferred: shakes the camera (90 frames) and every character that can act (fn_8016CDB8 with m08)
void GameEventScript::fn_803A3444(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    GameCamera& camera = lbl_8087F430->mCamera;
    camera.m900 %= 2;
    camera.m904 = 90;
    camera.m908 = 1.0f;
    camera.m90C = 1.0f;
    for (PlayerTask* player = lbl_8087F8A0->mHead; player != 0; player = player->mNext) {
        if (player->CanAct_unk()) {
            player->fn_8016CDB8(90, args->m08);
        }
    }
    for (EnemyTask* enemy = EnemyManager::sInstance->mHead; enemy != 0; enemy = enemy->mNext) {
        if (enemy->CanAct_unk()) {
            enemy->fn_8016CDB8(90, args->m08);
        }
    }
    result.SetNext_unk(cmd, 1);
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

// inferred: arguments of the cliff command (name invented)
struct ScriptCliffArgs_unk : public ScriptCmd_unk {
    int m08;
    int m0C;
    unsigned int m10;
    float m14;
};

// inferred: the interaction point `id` of the scene (name invented)
static inline gs::InteractionPoint* FindInteraction_unk(GameScene* scene, int id) {
    for (unsigned int i = 0; i < scene->mC4.size(); i++) {
        if (id == scene->mC4[i].mId) {
            return &scene->mC4[i];
        }
    }
    return 0;
}

// inferred: the point linked after `point` (name invented)
static inline gs::InteractionPoint* NextInteraction_unk(GameScene* scene, gs::InteractionPoint* point) {
    for (unsigned int i = 0; i < scene->mCC.size(); i++) {
        gs::InteractionConnection& c = scene->mCC[i];
        if (point->mIndex - 1 == c.m00) {
            return &scene->mC4[c.m04];
        }
    }
    return 0;
}

// inferred: the point linked before `point` (name invented)
static inline gs::InteractionPoint* PrevInteraction_unk(GameScene* scene, gs::InteractionPoint* point) {
    for (unsigned int i = 0; i < scene->mCC.size(); i++) {
        gs::InteractionConnection& c = scene->mCC[i];
        if (point->mIndex - 1 == c.m04) {
            return &scene->mC4[c.m00];
        }
    }
    return 0;
}

// NONMATCHING: 97.2% - case 5 reads m14 through the incoming cmd register, case 13 schedules the
// vector arithmetic with other float registers
// inferred: cliff hanging of the leader (m08: 5 grabs/leaves, 10 moves between the two points of a
// connection, 13 climbs along the link of the point); m10 bit 0 leaves, bit 5 takes the reverse link
void GameEventScript::fn_803A372C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptCliffArgs_unk* args = static_cast<const ScriptCliffArgs_unk*>(cmd);
    CharaTask* leader = lbl_8087F8A0->mHead;
    if (leader != 0) {
        unsigned int flags = args->m10;
        if ((flags & 1) != 1) {
            switch (args->m08) {
            case 5:
                leader->fn_8016939C(args->m0C, flags, args->m14);
                break;
            case 10: {
                gs::InteractionPoint* point = FindInteraction_unk(mScene, args->m0C);
                if ((flags & 0x20) != 0x20) {
                    leader->fn_80162A08(point->mPos, NextInteraction_unk(mScene, point)->mPos);
                } else {
                    leader->fn_80162A08(point->mPos, PrevInteraction_unk(mScene, point)->mPos);
                }
                break;
            }
            case 13: {
                gs::InteractionPoint* point = FindInteraction_unk(mScene, args->m0C);
                if (point != 0) {
                    gs::InteractionConnection* c = (gs::InteractionConnection*)mScene->fn_803C18DC(13, point->mId);
                    if (c != 0) {
                        gs::InteractionPoint& other = mScene->mC4[c->m04];
                        leader->fn_80163B88(leader->mPos, leader->mPos + (other.mPos - point->mPos));
                    }
                }
                break;
            }
            }
        } else if (args->m08 == 5) {
            leader->fn_801696E0();
        }
    }
    result.SetNext_unk(cmd);
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

// inferred: control in [4, 12) of kind 6 (name invented)
static inline bool InControl6_unk(int ctl, int id) {
    return ctl == 6 && id >= 4 && id < 12;
}

// inferred: stores a state of the character m08/m0C (m10) into the script variable m14/m18
void GameEventScript::fn_803A3D20(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara != 0) {
        int value = 0;
        int ctl = chara->m55C;
        int id = chara->m560;
        switch (args->m10) {
        case 0:
            value = chara->mFlag31;
            break;
        case 1:
            value = chara->mFlag31 && chara->mC48 != 0;
            break;
        case 2:
            value = ctl == 6 && id == 0x17;
            break;
        case 3:
            value = ctl == 6 && id == 0x3B;
            break;
        case 4:
            if (InControl6_unk(ctl, id)) {
                value = 1;
            }
            break;
        case 5:
            value = ctl == 6 && id == 0x16;
            break;
        case 6:
            if (ctl == 6 && (id == 0x1D || id == 0x1E)) {
                value = 1;
            }
            break;
        case 7:
            value = chara->mFlag6;
            break;
        case 8:
            value = ctl == 6 && id == 0x62;
            break;
        case 9:
            value = ctl == 6 && id == 0x44;
            break;
        case 10:
            if (chara->fn_8016D3F8()) {
                value = 1;
            }
            break;
        case 11:
            value = ctl == 6 && id == 0x2A;
            break;
        case 12:
            value = ctl == 6 && id == 0x3D;
            break;
        }
        fn_8039BF04(args->m14, args->m18, 0, 0, value);
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 97.9% - the original stores mDB0C = 1 in the middle of SetNext_unk (as fn_803A6B9C)
// inferred: gives (or removes, m14 bit 0) the item m08 (unknown codes are item 1) in the amount
// m0C/m10, logs it and shows it
void GameEventScript::fn_803A3FFC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int item = args->m08;
    if (fn_80211480(item) == 0) {
        item = 1;
    }
    Inventory::instance_->mDB0C = 0;
    if ((args->m14 & 1) == 1) {
        fn_8044441C(Inventory::instance_, item, ScriptValue_unk(args->m0C, args->m10));
    } else {
        int full = 1;
        Inventory::instance_->AddItem(item, ScriptValue_unk(args->m0C, args->m10), 0, 1, &full, 1, 1);
        if (fn_80206B9C(args->m08) && full == 0) {
            fn_801092C8(lbl_8087F048, 2, 0, args->m08, ScriptValue_unk(args->m0C, args->m10));
            lbl_8087F490->fn_803E5E64(item, ScriptValue_unk(args->m0C, args->m10), 0);
        } else {
            fn_801092C8(lbl_8087F048, 1, 0, item, ScriptValue_unk(args->m0C, args->m10));
            lbl_8087F490->fn_803E5E64(item, ScriptValue_unk(args->m0C, args->m10), 1);
        }
        fn_800C31F4(lbl_8074EF78[4], 0, -1, 1.0f);
    }
    Inventory::instance_->mDB0C = 1;
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

// NONMATCHING: 99.9% - only the offsets of the two strings in the pool (other strings of the file not written yet)
// inferred: shows (m30 0) or hides a node of the model of a character; "@eqN" names the equipped
// item N (its flag is set instead)
void GameEventScript::fn_803A4568(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptNodeArgs_unk* args = static_cast<const ScriptNodeArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara != 0) {
        atn::Model* model = &chara->mMotion;
        char name[0x20];
        memcpy(name, args->mName, sizeof(name));
        if (name[0] == '@') {
            if (strncmp(name, "@eq", 3) == 0) {
                int i = atoi(name + 3);
                EquipItemData* item = i < (int)chara->m6A4.mCount ? chara->m6A4.mData[i] : 0;
                if (item != 0) {
                    item->mFlags.mFlag = args->m30 == 0;
                } else {
                    char text[0x100];
                    sprintf(text, "OptionParts not found! <%s>", name);
                    lbl_8087EEB8->fn_800697D8(text);
                }
            }
        } else if (model->GetNodeNoByName(name, 0) >= 0) {
            model->fn_80093B88(name);
            model->fn_8009373C(name, args->m30 == 0);
        } else {
            char text[0x100];
            sprintf(text, "OptionParts not found! <%s>", name);
            lbl_8087EEB8->fn_800697D8(text);
        }
    }
    result.SetNext_unk(cmd);
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
void GameEventScript::fn_803A48A8(ScriptResult_unk& result, ScriptCmd_unk* cmd) {
    ScriptArgs_unk* args = static_cast<ScriptArgs_unk*>(cmd);
    TeamAI_unk* team;
    TeamChara_unk** members;
    int wait;
    CharaTask* leader;
    wait = 0;
    if (args->m08 == 2) {
        leader = lbl_8087F8A0->mHead;
        team = lbl_8087FA00->Find_unk(args->m08, args->m0C);
        if (team != 0) {
            float best = 0.0f;
            CharaTask* far = 0;
            members = team->mMembers;
            for (int i = 0; i < team->mNumMembers; members++, i++) {
                CharaTask* member = (CharaTask*)*members; // inferred: TeamChara_unk is a view of CharaTask
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

// inferred: moves a character back along its facing direction (name invented)
static inline void StepBack_unk(CharaTask* chara) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &front, &front);
    chara->fn_8015E7A0(atn::Vector3(-front.x, -front.y, -front.z), -1, 0);
}

// inferred: character m08/m0C: m10 0 disables it, 1 moves it back along its facing direction and
// jumps to the end of its motion, 2 only moves it back
void GameEventScript::fn_803A5118(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara != 0) {
        chara->vf44();
        switch (args->m10) {
        case 0:
            chara->fn_801765D8();
            break;
        case 1:
            chara->FreezeTask();
            StepBack_unk(chara);
            chara->mMotion.mTracks[0].m10 = 1.0f;
            chara->mMotion.mTracks[0].m08 = chara->mMotion.fn_80097D7C(0);
            chara->fn_80145334();
            chara->MeltTask();
            break;
        case 2:
            StepBack_unk(chara);
            break;
        }
    }
    result.SetNext_unk(cmd);
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

// inferred: makes the party member (m08, m0C) the leader (m10: swap flag kept in mDC)
void GameEventScript::fn_803A54C0(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara->mType == 0 || chara->mType == 3) {
        if (lbl_8087F430->mCamera.m7FC == 6) {
            lbl_8087F430->fn_803748E0(0);
        }
        mDC = args->m10;
        lbl_8087F8A0->fn_8054A09C((PlayerTask*)chara, args->m10);
        lbl_8087F430->mCamera.fn_8037EF30(1, 1, -1.0f);
        if (lbl_8087F490 != 0) {
            lbl_8087F490->fn_803E0D4C();
        }
    }
    result.SetNext_unk(cmd);
}

// inferred: the members of the team (m08, m0C) that can act follow the character (m10, m14) (m20 0,
// within m1C) or stop
void GameEventScript::fn_803A5650(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptFollowArgs_unk* args = static_cast<const ScriptFollowArgs_unk*>(cmd);
    TeamAI_unk* team = lbl_8087FA00->Find_unk(args->m08, args->m0C);
    CharaTask* target = GetChara_unk(args->m10, args->m14);
    if (team != 0 && target != 0) {
        for (int i = 0; i < team->mNumMembers; i++) {
            CharaTask* member = (CharaTask*)team->mMembers[i];
            if (member->mType != 0 && member != target && (member->mFlags & 4) != 4 &&
                !(member->mType == 2 && (member->mStatus.m0C & 0x20) == 0x20)) {
                if (args->m20 == 0) {
                    member->fn_80171420(target, 0, args->m1C);
                } else {
                    member->fn_80171DB0();
                }
            }
        }
    }
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

// inferred: value of the chase or of the config `kind` (name invented)
static inline int ChaseValue_unk(int kind) {
    int value = 0;
    switch (kind) {
    case 0:
        if (lbl_8087F098 != 0 && lbl_8087F098->mState != 0) {
            value = 1;
        } else if (ChaserNpc::fn_8018059C() != 0 && ChaserNpc::fn_8018059C()->m14B0 != 5) {
            value = 1;
        }
        break;
    case 1:
        if (lbl_8087F098 != 0 && lbl_8087F098->mState != 0) {
            if (lbl_8087F098->mState == 2) {
                value = 4;
            } else if (lbl_8087F098->m50 > 0) {
                value = 3;
            } else if (lbl_8087F098->m4C > 0) {
                value = 2;
            } else {
                value = 1;
            }
        } else if (ChaserNpc::fn_8018059C() != 0) {
            value = ChaserNpc::fn_8018059C()->m14B0;
        }
        break;
    case 2:
        if (lbl_8087F098 != 0 && lbl_8087F098->mState != 0) {
            value = lbl_8087F098->mGoalId;
        } else if (ChaserNpc::fn_8018059C() != 0) {
            ChaserNpc* npc = ChaserNpc::fn_8018059C();
            value = npc->m14C0 >= 0 ? npc->m14B4[npc->m14C0] : 0;
        }
        break;
    case 5:
        value = lbl_8087F0A8->m4DC.m08;
        break;
    case 6:
        value = lbl_8087F0A8->m4DC.m04;
        break;
    case 7:
        value = lbl_8087F0A8->m4DC.m0C * 57.29578f;
        break;
    case 9:
        value = lbl_8087F0A8->m53C.m44;
        break;
    case 10:
        value = lbl_8087F0A8->m53C.m40;
        break;
    case 11:
        if (lbl_8087F098 != 0) {
            value = lbl_8087F098->mShowGoal;
        }
        break;
    }
    return value;
}

// inferred: writes a field flag without recording it (m37C cleared meanwhile) (name invented)
static inline void StoreValue_unk(GameEventScript* self, int value, int flag, int mode) {
    GameTask* field = lbl_8087F430;
    if (field != 0) {
        int saved = self->m37C;
        self->m37C = 0;
        if (mode == 1) {
            field->fn_80370320(flag, value, 0);
        } else {
            field->fn_80370AE4(flag, value);
        }
        self->mF0 = 1;
        self->m37C = saved;
    }
}

// NONMATCHING: 98.4% - the value takes r28 instead of r31 (this/result/cmd shifted) and the operands
// of the fmuls of case 7 are swapped
// inferred: stores a value of the chase or of the config (m08) into the field flag m10 (m0C: 1 with
// fn_80370320, else fn_80370AE4); counterpart of the setter above
void GameEventScript::fn_803A5A8C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int value = ChaseValue_unk(args->m08);
    StoreValue_unk(this, value, args->m10, args->m0C);
    result.SetNext_unk(cmd);
}

// NONMATCHING: 99.8% - the jump table label and registers r0/r4 swapped in the next command (as fn_803A61CC)
// inferred: settings of the chase and of the game config, with the value (m10, m0C, as fn_803A6CDC)
void GameEventScript::fn_803A5CDC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int value = ScriptValue_unk(args->m10, args->m0C);
    switch ((unsigned int)args->m08) {
    case 0:
        if (lbl_8087F098 != 0 && value != 0) {
            lbl_8087F098->fn_801826E0();
        } else {
            lbl_8087F098->fn_80182890();
        }
        break;
    case 1:
        break;
    case 2:
        if (lbl_8087F098 != 0) {
            lbl_8087F098->fn_80183618(value);
        }
        break;
    case 3:
        if (lbl_8087F098 != 0) {
            lbl_8087F098->fn_80183588(value);
        }
        break;
    case 4:
        lbl_8087F0A8->m4DC.m38 = value;
        break;
    case 5:
        lbl_8087F0A8->m4DC.m08 = value;
        break;
    case 6:
        lbl_8087F0A8->m4DC.m04 = value;
        break;
    case 7:
        lbl_8087F0A8->m4DC.m0C = 0.017453292f * value;
        break;
    case 8:
        if (ChaserNpc::fn_8018059C() != 0) {
            if (value == 1) {
                ChaserNpc::fn_8018059C()->fn_80181284();
            } else if (value == 0) {
                ChaserNpc::fn_8018059C()->fn_8018120C();
            }
        }
        break;
    case 9:
        lbl_8087F0A8->m53C.m44 = value;
        break;
    case 10:
        lbl_8087F0A8->m53C.m40 = value;
        break;
    case 11:
        if (lbl_8087F098 != 0) {
            lbl_8087F098->fn_80183774(value);
        }
        break;
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

// NONMATCHING: 99.5% - the counter and the count of the end loop of SkipBlock_unk swap r4/r5
// inferred: conditional block: runs it when fn_8039C7F0 holds, else goes on after its end (as
// fn_803AADCC)
void GameEventScript::fn_803A5FF8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    result.mWait = 0;
    if (fn_8039C7F0(args->m08, args->m0C, args->m18, args->m10, args->m14) != 0) {
        result.mNext = NextCmd_unk(cmd);
    } else {
        result.mNext = SkipBlock_unk(m94, cmd);
    }
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

// NONMATCHING: 99.8% - r0/r4 swap in SetNext_unk (docs/mwcc-blockers.md, GameEventScript family)
// inferred: arena settings (m08) from the values m0C/m10 and m14/m18
void GameEventScript::fn_803A6410(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int value = ScriptValue_unk(args->m0C, args->m10);
    int value2 = ScriptValue_unk(args->m14, args->m18);
    ArenaTask* arena = lbl_8087EE78;
    if (arena != 0) {
        switch (args->m08) {
        case 0:
            if (value != 0) {
                arena->fn_80043B34();
            } else {
                arena->fn_80043BDC();
            }
            break;
        case 1:
            arena->mMode = value;
            break;
        case 2:
            arena->m5C = value;
            break;
        case 3:
            arena->mScore0 = value;
            break;
        case 4:
            arena->mScore1 = value;
            break;
        case 5:
            arena->mResult = value;
            arena->mFirstResult = 1;
            break;
        case 6:
            arena->mSe = value;
            break;
        case 7:
            arena->mVoice = value;
            break;
        case 8:
            arena->mVoices[0].mCount = 0;
            break;
        case 9: {
            ArenaTask::Voice_unk voice;
            voice.mFirst = value;
            voice.mLast = value2;
            arena->mVoices[0].PushBack_unk(voice);
            break;
        }
        case 10:
            arena->mVoices[1].mCount = 0;
            break;
        case 11: {
            ArenaTask::Voice_unk voice;
            voice.mFirst = value;
            voice.mLast = value2;
            arena->mVoices[1].PushBack_unk(voice);
            break;
        }
        case 12:
            arena->mVoices[2].mCount = 0;
            break;
        case 13: {
            ArenaTask::Voice_unk voice;
            voice.mFirst = value;
            voice.mLast = value2;
            arena->mVoices[2].PushBack_unk(voice);
            break;
        }
        case 14:
            arena->mVoices[3].mCount = 0;
            break;
        case 15: {
            ArenaTask::Voice_unk voice;
            voice.mFirst = value;
            voice.mLast = value2;
            arena->mVoices[3].PushBack_unk(voice);
            break;
        }
        }
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

// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
// inferred: marks (m08 0) or unmarks (1) a character on the minimap of the town
void GameEventScript::fn_803A6744(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    if (lbl_8087FA20 != 0) {
        MiniMapTask* minimap = (MiniMapTask*)lbl_8087FA20->m258; // inferred: m258 is the minimap (fn_804A7EA4)
        CharaTask* chara = GetChara_unk(args->m0C, args->m10);
        switch (args->m08) {
        case 0: {
            atn::Color color(0.8156863f, 0.0f, 0.0f, 1.0f);
            minimap->fn_804AAD68(chara, &color);
            break;
        }
        case 1:
            minimap->fn_804AAFCC(chara);
            break;
        }
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 93.9% - register allocation (free/entry/this and the loop variables), r0/r4 in SetNext_unk
// inferred: shows or hides (m10) the map parts named mName in every map (m08: 0 the parts, 1 the
// locator sets) and remembers it in m154
void GameEventScript::fn_803A68B8(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptMotionArgs_unk* args = static_cast<const ScriptMotionArgs_unk*>(cmd);
    int value = ScriptValue_unk(args->m0C, args->m10);
    if (MapManager::instance_ != 0) {
        atn::FixedArray_unk<MapTask*, 64>* maps;
        int free;
        ScriptPartsFlag_unk* entry;
        char name[0x20];
        memcpy(name, args->mName, sizeof(name));
        free = -1;
        entry = 0;
        unsigned int hash = fn_800DC6B4(name);
        for (int i = 0; i < 64; i++) {
            if (hash == m154[i].mHash) {
                entry = &m154[i];
                break;
            }
            if (m154[i].mHash == 0 && free == -1) {
                free = i;
            }
        }
        if (entry == 0 && free >= 0) {
            entry = &m154[free];
            entry->mHash = hash;
        }
        maps = &MapManager::instance_->mMaps;
        for (unsigned int i = 0; i < maps->size(); i++) {
            MapTask* map = (*maps)[i];
            switch (args->m08) {
            case 0:
                map->fn_804A0E18(name, value);
                if (entry != 0) {
                    if (value != 0) {
                        entry->mFlags |= 1;
                    } else {
                        entry->mFlags &= ~1;
                    }
                    entry->mFlags |= 2;
                }
                break;
            case 1: {
                MapPartsLocatorSet* set = (MapPartsLocatorSet*)map->fn_804A06B4(4, name);
                if (set != 0) {
                    set->fn_8049B72C(value != 0 ? set->mFlags | 0x400 : set->mFlags & ~0x400);
                }
                if (entry != 0) {
                    if (value != 0) {
                        entry->mFlags |= 4;
                    } else {
                        entry->mFlags &= ~4;
                    }
                    entry->mFlags |= 8;
                }
                break;
            }
            }
        }
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
    int value = ScriptValue_unk(args->m0C, args->m10);
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

// inferred: stores the status flag m10 of a character in the flag m18 of the field (m14 1: through
// fn_80370320)
void GameEventScript::fn_803A6F90(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    if (chara != 0) {
        unsigned int bit = 1 << args->m10;
        GameTask* field = lbl_8087F430;
        int value = (chara->mStatus.m0C & bit) == bit;
        int flag = args->m18;
        int mode = args->m14;
        if (field != 0) {
            int saved = m37C;
            m37C = 0;
            if (mode == 1) {
                field->fn_80370320(flag, value, 0);
            } else {
                field->fn_80370AE4(flag, value);
            }
            mF0 = 1;
            m37C = saved;
        }
    }
    result.SetNext_unk(cmd);
}

// inferred: links (value 1) or unlinks the characters m0C/m10 and m14/m18 (m08 == 0 only)
void GameEventScript::fn_803A7954(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m0C, args->m10);
    CharaTask* other = GetChara_unk(args->m14, args->m18);
    int value = ScriptValue_unk(args->m1C, args->m20);
    if (args->m08 == 0) {
        if (value == 1) {
            chara->fn_8017A33C(other, 1);
        } else {
            chara->fn_8017A33C(0, 1);
        }
    }
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

// NONMATCHING: 99.6% - r0/r4 swap in SetNext_unk (docs/mwcc-blockers.md, GameEventScript family)
// inferred: song control (m08: 0 state, 1 song, 2 position of a warp point, 3 distance, 4/5 volumes
// in percent, 6 environment sounds)
void GameEventScript::fn_803A7FBC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int value = ScriptValue_unk(args->m0C, args->m10);
    if (SongManager::sInstance != 0) {
        switch (args->m08) {
        case 0:
            SongManager::sInstance->fn_805A2FD4(value);
            break;
        case 1:
            SongManager::sInstance->fn_805A32C8(value);
            break;
        case 2: {
            GameScene* scene = FieldScene_unk();
            gs::WarpPoint* warp = scene != 0 ? FindWarpIn_unk(scene, value) : 0;
            if (warp != 0) {
                SongManager::sInstance->fn_805A3448(warp->mPos);
            }
            break;
        }
        case 3:
            SongManager::sInstance->fn_805A358C(value);
            break;
        case 4:
            SongManager::sInstance->fn_805A3660(value / 100.0f);
            break;
        case 5:
            SongManager::sInstance->fn_805A3734(value / 100.0f);
            break;
        case 6:
            SongManager::sInstance->fn_805A3808(value);
            break;
        }
    }
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

// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
// inferred: sets the money (m08 0) to the value (m0C, m10, as fn_803A6CDC), with the notice of the
// change in the battle log and the UI
void GameEventScript::fn_803A8284(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int value = ScriptValue_unk(args->m0C, args->m10);
    if (Inventory::instance_ != 0 && args->m08 == 0) {
        if (lbl_8087F048 != 0) {
            int diff = value - Inventory::instance_->mMoney;
            if (diff >= 0) {
                fn_801092C8(lbl_8087F048, 3, 0, diff, 0);
            } else {
                fn_801092C8(lbl_8087F048, 4, 0, -diff, 0);
            }
            lbl_8087F490->fn_803E5E64(10004, diff, 1);
            fn_800C31F4(lbl_8074EF78[4], 0, -1, 1.0f);
        }
        Inventory::instance_->fn_8044D6E0(value);
    }
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

// inferred: offsets of the render camera written by the shake command (names invented)
struct ScriptShake_unk {
    atn::Vector3 mPos;
    atn::Vector3 mTarget;
    float mFovy;
};
// inferred: sets or clears flag 0x10 of the leader (name invented)
static inline void HoldLeaderFlag_unk(int hold) {
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (leader != 0) {
        if (hold != 0) {
            leader->m54C |= 0x10;
        } else {
            leader->m54C &= ~0x10;
        }
    }
}
extern "C" const char* const lbl_8074F020[][2]; // inferred: sounds of the start of the swing

// NONMATCHING: 94.3% - the leader takes r31 instead of r27 (shifting this/result/cmd), the leader lookups
// of flag 0x80 are not reloaded, float registers as fn_803A956C
// inferred: makes the leader walk to the place m14 (m10: 0x01 no walk), waits for it and for the
// camera, fades out and swings the camera for mC0 frames (as fn_803A956C, with a cosine)
void GameEventScript::fn_803A914C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int wait;
    CharaTask* leader;
    leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    wait = 1;
    switch (mB8) {
    case 0:
        if (((unsigned int)args->m10 & 0x80) == 0x80) {
            HoldLeaderFlag_unk(1);
        }
        if (((unsigned int)args->m10 & 1) != 1 && leader != 0) {
            leader->fn_8017039C(args->m14, 0);
        }
        mB8++;
    case 1:
        if (((unsigned int)args->m10 & 1) != 1 && leader != 0 && leader->mRoute.m2C == 0) {
            break;
        }
        if (((unsigned int)args->m10 & 0x40) != 0x40 && lbl_8087F430 != 0 &&
            lbl_8087F430->mCamera.m800 != lbl_8087F430->mCamera.m7FC) {
            break;
        }
        mB8++;
    case 2:
        if (lbl_8087F430 != 0 && ((unsigned int)args->m10 & 2) != 2) {
            lbl_8087F430->fn_80370B78(0xFF000000, mC0 - 5, 5, -9.0f);
        }
        if (((unsigned int)args->m10 & 0x10) != 0x10) {
            fn_803935AC(lbl_8074F020[args->m08][args->m0C], 1.0f);
        }
        mBC = 0;
        mB8++;
    case 3:
        if (lbl_8087F430 != 0) {
            if (((unsigned int)args->m10 & 2) != 2 || ((unsigned int)args->m10 & 4) != 4) {
                if (((unsigned int)args->m10 & 4) != 4 && ((unsigned int)args->m10 & 0x20) != 0x20) {
                    float t;
                    float amp;
                    float scale;
                    t = (float)mBC / (float)mC0;
                    ScriptShake_unk shake;
                    shake.mPos.x = 0.0f;
                    shake.mPos.y = 0.0f;
                    shake.mPos.z = 0.0f;
                    shake.mTarget.x = 0.0f;
                    shake.mTarget.y = 0.0f;
                    shake.mTarget.z = 0.0f;
                    shake.mFovy = 0.0f;
                    GameTask* field = lbl_8087F430;
                    amp = (1.0f - (float)cos(3.1415927f * t * 0.5f)) * 0.5f;
                    scale = field->mCamera.mDist / 3.0f;
                    if (((unsigned int)args->m10 & 0x20) != 0x20) {
                        if (args->m0C == 1) {
                            scale *= -1.0f;
                        }
                        shake.mTarget.x = 0.0f;
                        shake.mTarget.y = amp * scale;
                        shake.mTarget.z = 0.0f;
                    } else {
                        if (args->m0C == 0) {
                            scale *= -1.0f;
                        }
                        amp *= scale;
                        shake.mPos.x = 0.0f;
                        shake.mPos.y = 0.7f * amp;
                        shake.mPos.z = 0.0f;
                        shake.mTarget.x = 0.0f;
                        shake.mTarget.y = amp;
                        shake.mTarget.z = 0.0f;
                    }
                    field->mCamera.m93C = shake.mPos;
                    field->mCamera.m948 = shake.mTarget;
                    field->mCamera.m954 = shake.mFovy;
                }
                if (++mBC < mC0) {
                    break;
                }
            }
        }
        mB8++;
    case 4: {
        GameTask* field = lbl_8087F430;
        if (field != 0) {
            field->mCamera.m93C.x = 0.0f;
            field->mCamera.m93C.y = 0.0f;
            field->mCamera.m93C.z = 0.0f;
            field->mCamera.m948.x = 0.0f;
            field->mCamera.m948.y = 0.0f;
            field->mCamera.m948.z = 0.0f;
            field->mCamera.m954 = 0.0f;
        }
        if (((unsigned int)args->m10 & 0x80) == 0x80) {
            HoldLeaderFlag_unk(0);
        }
        wait = 0;
        break;
    }
    }
    result.SetNext_unk(cmd, wait);
}

void GameEventScript::fn_803A9558(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    mB8 = 0;
    mC0 = 60;
    fn_803A956C(result, cmd);
}

extern "C" const char* const lbl_8074F088[][2]; // inferred: sounds of the end of the shake

// NONMATCHING: 96.6% - float registers of the shake computation (f3/f4 swapped, zero in f7)
// inferred: shakes the camera for mC0 frames (m0C: direction, m10 flags: 0x04 no shake, 0x10 no sound,
// 0x20 vertical, 0x80 holds the leader), then plays the sound m08/m0C
void GameEventScript::fn_803A956C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int wait = 1;
    switch (mB8) {
    case 0:
        if (((unsigned int)args->m10 & 0x80) == 0x80) {
            CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
            if (leader != 0) {
                leader->m54C |= 0x10;
            }
        }
        if (lbl_8087F430 != 0 && lbl_8087F430->m54F0 == 1) {
            lbl_8087F430->fn_80370BD0(mC0, 0, 1e-05f);
        }
        mBC = 0;
        mB8++;
    case 1: {
        GameTask* field = lbl_8087F430;
        if (field != 0 && ((unsigned int)args->m10 & 4) != 4 && ((unsigned int)args->m10 & 0x20) != 0x20) {
            float t; float amp; float scale;
            t = (float)mBC / (float)mC0;
            ScriptShake_unk shake;
            shake.mPos.x = 0.0f;
            shake.mPos.y = 0.0f;
            shake.mPos.z = 0.0f;
            shake.mTarget.x = 0.0f;
            shake.mTarget.y = 0.0f;
            shake.mTarget.z = 0.0f;
            shake.mFovy = 0.0f;
            amp = 1.0f - (float)sin(0.5f * (3.1415927f * t));
            scale = field->mCamera.mDist / 3.0f;
            if (((unsigned int)args->m10 & 0x20) != 0x20) {
                if (args->m0C == 1) {
                    scale *= -1.0f;
                }
                shake.mTarget.x = 0.0f;
                shake.mTarget.y = amp * scale;
                shake.mTarget.z = 0.0f;
            } else {
                if (args->m0C == 0) {
                    scale *= -1.0f;
                }
                amp *= scale;
                shake.mPos.x = 0.0f;
                shake.mPos.y = amp;
                shake.mPos.z = 0.0f;
                shake.mTarget.x = 0.0f;
                shake.mTarget.y = 0.7f * amp;
                shake.mTarget.z = 0.0f;
            }
            field->mCamera.m93C = shake.mPos;
            field->mCamera.m948 = shake.mTarget;
            field->mCamera.m954 = shake.mFovy;
            if (++mBC < mC0) {
                break;
            }
        }
        mB8++;
    }
    case 2: {
        GameTask* field = lbl_8087F430;
        if (field != 0) {
            field->mCamera.m93C.x = 0.0f;
            field->mCamera.m93C.y = 0.0f;
            field->mCamera.m93C.z = 0.0f;
            field->mCamera.m948.x = 0.0f;
            field->mCamera.m948.y = 0.0f;
            field->mCamera.m948.z = 0.0f;
            field->mCamera.m954 = 0.0f;
            field->mCamera.m900 %= 2;
            field->mCamera.m904 = 30;
            field->mCamera.m908 = 0.1f;
            field->mCamera.m90C = 0.2f;
        }
        if (((unsigned int)args->m10 & 0x10) != 0x10) {
            fn_803935AC(lbl_8074F088[args->m08][args->m0C], 1.0f);
        }
        if (((unsigned int)args->m10 & 0x80) == 0x80) {
            CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
            if (leader != 0) {
                leader->m54C &= ~0x10;
            }
        }
        wait = 0;
        break;
    }
    }
    result.SetNext_unk(cmd, wait);
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

// inferred: accessor of CharaTask::mFlag0 (reloads the flag word; name invented)
static inline bool IsFlag0_unk(const CharaTask* chara) {
    return chara->mFlag0;
}

// NONMATCHING: 99% - the store of `wait` in SetNext_unk is scheduled one instruction later in the
// original (the non-const cmd that fixes it elsewhere does not fit: fn_803A98C4 passes its const cmd)
// inferred: makes the leader walk to the place m18 (m14 flags: 0x1 no walk, 0x2 wait for the route
// instead of mC0 frames, 0x4 keeps flag 3, 0x8 keeps the input) with a fade out
void GameEventScript::fn_803A98EC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* leader = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    int wait = 1;
    switch (mB8) {
    case 0:
        if ((args->m14 & 1) != 1 && leader != 0) {
            leader->fn_8017039C(args->m18, 0);
        }
        if ((args->m14 & 2) != 2 && lbl_8087F430 != 0) {
            lbl_8087F430->fn_80370B78(0xFF000000, mC0 - 5, 5, -9.0f);
        }
        if ((args->m14 & 4) == 4) {
            mC4 = lbl_8087F430->fn_80370174(3);
            lbl_8087F430->fn_80370320(3, 1, 0);
        }
        if ((args->m14 & 8) != 8 && lbl_8087F430 != 0) {
            lbl_8087F430->m5664 = 0;
        }
        if (leader != 0 && leader->m55C == 6 && (leader->m560 == 0x54 || leader->m560 == 0x28)) {
            leader->mFlag0 = 1;
        }
        mBC = 0;
        mB8++;
        if (lbl_8087F430 != 0 && lbl_8087F430->GetCurrentMap() != 0 && (args->m14 & 1) != 1) {
            int stop = 0;
            MapTask* map = lbl_8087F430->GetCurrentMap();
            if (map->mType == 1) {
                int id = map->mId;
                if ((id == 1 && args->m18 == 0x69) || (id == 2 && args->m18 == 0x19) ||
                    (id == 3 && args->m18 == 0x11) || (id == 6 && args->m18 == 0x48)) {
                    stop = 1;
                }
            }
            if (stop != 0) {
                if (GameBgm::sInstance != 0) {
                    GameBgm::sInstance->fn_8037D4C0(0, 0x2D, 1, 0, 1.0f);
                }
                if (lbl_8087F418 != 0) {
                    lbl_8087F418->fn_8035E858();
                }
            }
        }
    case 1:
        if ((args->m14 & 1) != 1 || (args->m14 & 2) != 2) {
            if ((args->m14 & 2) != 2) {
                if (++mBC < mC0) {
                    break;
                }
            } else if (leader->mRoute.m2C == 0) {
                break;
            }
        }
        mB8++;
    case 2:
        if ((args->m14 & 4) == 4 && lbl_8087F430 != 0) {
            lbl_8087F430->fn_80370320(3, mC4, 0);
        }
        if ((args->m14 & 8) != 8 && lbl_8087F430 != 0) {
            lbl_8087F430->m5664 = 1;
        }
        if ((args->m14 & 1) != 1 && leader != 0) {
            leader->fn_80171DB0();
        }
        if (leader != 0 && IsFlag0_unk(leader)) {
            leader->mFlag0 = 0;
        }
        wait = 0;
        break;
    }
    result.SetNext_unk(cmd, wait);
}

// inferred: takes the control of a character for the event (saves its flags in mC0/mC4) and runs the
// next step (fn_803A9DBC)
void GameEventScript::fn_803A9C40(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m10, args->m14);
    mB8 = 0;
    mC0 = 0;
    if (chara != 0) {
        mC0 = (chara->m54C >> 2) & 1;
        mC4 = (chara->mStatus.m0C >> 12) & 1;
        chara->mStatus.fn_8013322C(-1);
        chara->vf48();
        chara->m54C |= 4;
        chara->mStatus.fn_8013310C(0x1000, 0);
    }
    fn_803A9DBC(result, cmd);
}

// inferred: arguments of the constrain command (names invented)
struct ScriptConstrainArgs_unk : public ScriptCmd_unk {
    int m08;
    int m0C;
    char mNode[0x20]; // 0x10
    int mModel;       // 0x30
    int mRelease;     // 0x34
    atn::Vector3 mOffset; // 0x38
};

// NONMATCHING: 99.9% - only the @stringBase0 offsets (the earlier strings belong to unwritten functions)
// inferred: attaches the constrained model mModel of the field scene to the node mNode of the
// character m08/m0C (mRelease: detaches it)
void GameEventScript::fn_803AA5E4(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptConstrainArgs_unk* args = static_cast<const ScriptConstrainArgs_unk*>(cmd);
    CharaTask* chara = GetChara_unk(args->m08, args->m0C);
    GameScene* scene = FieldScene_unk();
    if (chara != 0 && scene != 0) {
        char name[0x100];
        gs::ConstrainModel* model = scene->fn_803CC6D0(args->mModel);
        if (model != 0) {
            if (args->mRelease == 0) {
                atn::Model* motion = &chara->mMotion;
                memset(name, 0, sizeof(name));
                memcpy(name, args->mNode, 0x20);
                int node = motion->GetNodeNoByName(name, 0);
                if (node < 0) {
                    char text[0x100];
                    sprintf(text, "ConstrainModel node not found! <%s>", name);
                    lbl_8087EEB8->fn_800697D8(text);
                }
                model->m1C = !chara->IsFlag_unk(4);
                model->m08 = chara;
                model->m0C = (const atn::Matrix3x4*)motion->GetNodeMtx_unk(node);
                model->m10 = args->mOffset;
            } else {
                model->m1C = 0;
                model->m08 = 0;
                model->m0C = 0;
            }
        } else {
            char text[0x100];
            sprintf(text, "ConstrainModel model not found! <%s>", name);
            lbl_8087EEB8->fn_800697D8(text);
        }
    }
    result.SetNext_unk(cmd);
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

// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
// inferred: switches the current map to the area list m08 (modulo the count) with the blend m0C over
// m10 frames
void GameEventScript::fn_803AAC60(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptAreaArgs_unk* args = static_cast<const ScriptAreaArgs_unk*>(cmd);
    MapTask* map = lbl_8087F430 != 0 ? lbl_8087F430->GetCurrentMap() : 0;
    if (map != 0 && (int)map->mAreas.mSize > 0) {
        int index = args->m08 % (int)map->mAreas.mSize;
        float speed = 1.0f;
        if (args->m10 > 0) {
            speed = 1.0f / args->m10;
        }
        lbl_8087F430->GetCurrentMap()->fn_804A0F58(index, args->m0C, speed);
    }
    result.SetNext_unk(cmd);
}

void GameEventScript::fn_803AAD58(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    lbl_8087F490->fn_803E44C4((args->m0C & 1) ^ 1, args->m08, 45);
    result.SetNext_unk(cmd);
}

// NONMATCHING: 99.5% - the counter and the count of the end loop of SkipBlock_unk swap r4/r5
// inferred: block (closed by command 0x45, nested by 0x44): runs it when the counter m10D0 of the field
// lies in [m08, m0C], else goes on after its end
void GameEventScript::fn_803AADCC(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    result.mWait = 0;
    int count = lbl_8087F430 != 0 ? lbl_8087F430->m10D0 : 0;
    if (args->m08 <= count && count <= args->m0C) {
        result.mNext = NextCmd_unk(cmd);
        return;
    }
    result.mNext = SkipBlock_unk(m94, cmd);
}

// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
// inferred: m08 1: shows the notice 0 of the UI when the value (m0C, m10, as fn_803A6CDC) is 0
void GameEventScript::fn_803AAF74(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    const ScriptArgs_unk* args = static_cast<const ScriptArgs_unk*>(cmd);
    int value = ScriptValue_unk(args->m0C, args->m10);
    if (args->m08 == 1 && lbl_8087F490 != 0 && value == 0) {
        lbl_8087F490->fn_803E598C(0, 30, 0);
    }
    result.SetNext_unk(cmd);
}

// NONMATCHING: 99% - registers r0/r4 swapped in the next command (as fn_803A61CC)
// inferred: notices and gauges of the UI (m08: 0 notice 1, 1 fn_803E58CC, 2 notice 0, 3 fn_803E58E4,
// 4 fn_803DF5B4)
void GameEventScript::fn_803AB06C(ScriptResult_unk& result, const ScriptCmd_unk* cmd) {
    switch (static_cast<const ScriptArgs_unk*>(cmd)->m08) {
    case 0:
        lbl_8087F490->fn_803E598C(1, 0, 0);
        break;
    case 1:
        lbl_8087F490->fn_803E58CC();
        break;
    case 2:
        lbl_8087F490->fn_803E598C(0, 0, 0);
        break;
    case 3:
        lbl_8087F490->fn_803E58E4();
        break;
    case 4:
        lbl_8087F490->fn_803DF5B4();
        break;
    }
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

extern "C" void* lbl_8087EE98; // inferred: collision manager
extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test

// NONMATCHING: 99.7% - stack slots of the two copies swapped (start at 0x18 and end at 0x24 in the original)
// inferred: nothing blocks the line from `from` to `to` (stopped `radius` before it; only farther than
// 1.5 times the radius); used by Em206EggMonster and SpecialEnemyBase
extern "C" int fn_803ABE54(const atn::Vector3* from, const atn::Vector3* to, float radius) {
    atn::Vector3 start = *from;
    atn::Vector3 end = *to;
    float dist = (end - start).Length_unk();
    if (!((float)__fabs(dist) < 1.1920929e-7f)) {
        end -= start;
        end.Normalize_unk();
        end *= dist - radius;
        end += start;
    }
    if (dist > 1.5f * radius && fn_8004ECC0(lbl_8087EE98, 0, &start, &end, 0x80000010, 0, 0) == 0) {
        return 1;
    }
    return 0;
}

// inferred: removes the record of `chara` from m12C; 1 if there was one
int GameEventScript::fn_803ABFD4(atn::Task* chara) {
    for (unsigned int i = 0; i < m12C.size(); i++) {
        if (m12C[i].mChara == chara) {
            m12C.erase(&m12C[i]);
            return 1;
        }
    }
    return 0;
}

// Both helpers are called out of line in the original (__declspec(noinline), weak copies at the end of the unit)
// inferred: characters of the set points with a picked event of kind 4 (without the flags `mask`) that the
// leader can reach: in range and in sight (or the field flag 206)
__declspec(noinline) inline int GameEventScript::CountCharas_unk(atn::SimpleArray<gs::NpcSetPoint>& points,
                                            atn::SimpleArray<ScriptNpcFlag_unk>& flags, int kind, CharaTask* leader,
                                            const atn::Vector3& pos, unsigned int mask, float range) {
    int count = 0;
    for (unsigned int i = 0; i < points.size(); i++) {
        int script = flags[i].m00;
        gs::NpcSetPoint& point = points[i];
        if (script >= 0) {
            const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)FindEvent_unk(point.m140[script]);
            if (e != 0 && e->m0C == 4 && (e->mB8 & mask) == 0) {
                CharaTask* chara = GetChara_unk(kind, point.mId);
                if (chara != 0 && chara->CanAct_unk()) {
                    atn::Vector3 target = chara->mColli.m48;
                    atn::Vector3 d = target - pos;
                    if (PSVECSquareMag(&d) < range * range) {
                        if (fn_8004ECC0(lbl_8087EE98, 0, &pos, &target, 0x80000010, 0, 0) != 0 &&
                            (lbl_8087F430 == 0 || lbl_8087F430->fn_80370174(206) == 0)) {
                            continue;
                        }
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

// NONMATCHING: 98.6% - registers of the loop and stack slots of the two ends of the ray
// inferred: same for the event spheres (the ray is shortened at both ends)
__declspec(noinline) inline int GameEventScript::CountSpheres_unk(atn::SimpleArray<gs::EventSphere>& spheres,
                                             atn::SimpleArray<ScriptNpcFlag_unk>& states, CharaTask* leader,
                                             const atn::Vector3& pos, unsigned int mask, float range) {
    int count = 0;
    for (unsigned int i = 0; i < spheres.size(); i++) {
        gs::EventSphere& sphere = spheres[i];
        int group = i;
        if (sphere.m20 >= 0) {
            group = sphere.m20;
        }
        int script = states[group].m00;
        if (script >= 0) {
            const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)FindEvent_unk(spheres[group].m18[script]);
            if (e != 0 && e->m0C == 4 && (e->mB8 & mask) == 0) {
                atn::Vector3 d = sphere.mPos - pos;
                float sq = PSVECSquareMag(&d);
                if (sq > 0.0f) {
                    PSVECNormalize(&d, &d);
                }
                if (sq < range * range) {
                    float margin = 5.0f;
                    if (leader != 0 && !leader->mFlag31) {
                        margin = 2.0f;
                    }
                    atn::Vector3 to = sphere.mPos - d * sphere.m14 * 1.2f;
                    atn::Vector3 from = pos + d * margin;
                    if (fn_8004ECC0(lbl_8087EE98, 0, &from, &to, 0x80000010, 0, 0) != 0 &&
                        (lbl_8087F430 == 0 || lbl_8087F430->fn_80370174(206) == 0)) {
                        continue;
                    }
                    count++;
                }
            }
        }
    }
    return count;
}

int GameEventScript::fn_803AC088() {
    PlayerTask* leader;
    if (lbl_8087F8A0 == 0 || (leader = lbl_8087F8A0->mHead) == 0 || lbl_8087F430 == 0 ||
        !lbl_8087F430->IsFlag_unk(2)) {
        return 0;
    }
    atn::Vector3 pos = leader->mColli.m48;
    if (leader->m55C == 1 && lbl_8087F430->mCamera.m7FC == 0) {
        pos = leader->mColli.m3C + atn::Vector3(0.0f, leader->m5B4, 0.0f);
    }
    float range = 500.0f;
    int count = CountCharas_unk(mScene->m80, m114, 1, leader, pos, 16, range);
    count += CountCharas_unk(mScene->m88, m11C, 2, leader, pos, 16, range);
    count += CountCharas_unk(mScene->m90, m124, 3, leader, pos, 16, range);
    return count + CountSpheres_unk(mScene->mEC, m10C, leader, pos, 16, range);
}

int GameEventScript::fn_803AC230() {
    PlayerTask* leader;
    if (lbl_8087F8A0 == 0 || (leader = lbl_8087F8A0->mHead) == 0 || lbl_8087F430 == 0 ||
        !lbl_8087F430->IsFlag_unk(2)) {
        return 0;
    }
    atn::Vector3 pos = leader->mColli.m48;
    if (leader->m55C == 1 && lbl_8087F430->mCamera.m7FC == 0) {
        pos = leader->mColli.m3C + atn::Vector3(0.0f, leader->m5B4, 0.0f);
    }
    float range = 500.0f;
    int count = CountCharas_unk(mScene->m80, m114, 1, leader, pos, 48, range);
    count += CountCharas_unk(mScene->m88, m11C, 2, leader, pos, 48, range);
    count += CountCharas_unk(mScene->m90, m124, 3, leader, pos, 48, range);
    return count + CountSpheres_unk(mScene->mEC, m10C, leader, pos, 48, range);
}

extern "C" int fn_80051BC4(const atn::ColliCapsuleShape_unk* a, const atn::ColliCapsuleShape_unk* b);

// NONMATCHING: 95.3% - stack slots of the locals (the segment, and those of the inlined fn_803ABE54, as there)
// inferred: the camera segment (`range` long) touches the capsule of `chara`, nothing blocks the view and
// the action 36 is held (debug input) or the camera mode is stable (with `wait`, for 15 frames)
int GameEventScript::fn_803AC3D8(CharaTask* chara, atn::Vector3* line, int wait, float range) {
    atn::ColliCapsuleShape_unk segment;
    segment.mStart = lbl_8087F430->mCamera.mPos;
    segment.mEnd = lbl_8087F430->mCamera.mTarget;
    segment.mEnd -= segment.mStart;
    if (!((float)__fabs(segment.mEnd.LengthSq_unk()) < 1.1920929e-7f)) {
        segment.mEnd.Normalize_unk();
    }
    segment.mEnd *= range;
    segment.mEnd += segment.mStart;
    int result = 0;
    atn::ColliCapsuleShape_unk capsule(chara->mColli.m3C, chara->mColli.m48, chara->mColli.m54);
    if (fn_80051BC4(&segment, &capsule) && fn_803ABE54(&segment.mStart, &capsule.mEnd, capsule.mRadius)) {
        int ok;
        if (lbl_8087F0A8->m0B0 != 0) {
            ok = lbl_8087F0A8->mInput.fn_801240B4(36);
        } else {
            GameCamera& camera = lbl_8087F430->mCamera;
            ok = camera.m7FC == camera.m800 && (wait == 0 || camera.m808 >= 15);
        }
        if (ok) {
            result = 1;
            if (line != 0) {
                line[0] = segment.mStart;
                line[1] = segment.mEnd;
            }
        }
    }
    return result;
}

extern "C" int fn_80051B70(const atn::ColliCapsuleShape_unk* capsule, const atn::ColliSphereShape_unk* sphere);

// NONMATCHING: 93.4% - temporaries of the segment (the original frame is 0x10 bytes smaller and keeps a
// second copy of the camera position at 0x3C) and registers of the loop
// inferred: the camera looks (80 units ahead) at an event sphere whose picked event is of kind 7 with the
// flag 8
int GameEventScript::fn_803AC708(CharaTask* chara) {
    atn::Camera& camera = atn::RenderManager::instance_->mCamera;
    atn::Vector3 pos = camera.mPos;
    atn::Vector3 dir = (camera.mTarget - camera.mPos).GetNormalize();
    atn::Vector3 end = pos + dir * 80.0f;
    atn::Vector3 start = pos;
    atn::ColliCapsuleShape_unk capsule;
    capsule.mStart = start;
    capsule.mEnd = end;
    GameScene* scene = mScene;
    for (unsigned int i = 0; i < scene->mEC.size(); i++) {
        gs::EventSphere& sphere = scene->mEC[i];
        int group = i;
        if (sphere.m20 >= 0) {
            group = sphere.m20;
        }
        int script = m10C[group].m00;
        if (script >= 0) {
            const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)FindEvent_unk(sphere.m18[script]);
            if (e != 0 && e->m0C == 7 && (e->mB8 & 8) == 8) {
                atn::ColliSphereShape_unk bounds(sphere.mPos, sphere.m14);
                if (fn_80051B70(&capsule, &bounds)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// inferred: the set point of `chara` has a picked script whose event is of kind 7 (name invented)
inline int GameEventScript::HasTalkEvent_unk(atn::SimpleArray<gs::NpcSetPoint>& points,
                                             atn::SimpleArray<ScriptNpcFlag_unk>& flags, CharaTask* chara) {
    for (unsigned int i = 0; i < points.size(); i++) {
        gs::NpcSetPoint& point = points[i];
        if (point.mId == chara->m58) {
            int script = flags[i].m00;
            if (script >= 0 && script < point.m140.mSize) {
                const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)FindEvent_unk(point.m140[script]);
                if (e != 0 && e->m0C == 7) {
                    return 1;
                }
            }
            return 0;
        }
    }
    return 0;
}

// NONMATCHING: 85.3% - registers (the original keeps the type in r5 and the party test in r0)
// inferred: the character has an event of kind 7 at its set point (NPC, enemy or friend)
int GameEventScript::fn_803AC978(CharaTask* chara) {
    if (chara == 0) {
        return 0;
    }
    if (chara->IsPartyMember_unk() && HasTalkEvent_unk(mScene->m80, m114, chara)) {
        return 1;
    }
    if (chara->mType == 2 && HasTalkEvent_unk(mScene->m88, m11C, chara)) {
        return 1;
    }
    if (chara->mType == 3 && HasTalkEvent_unk(mScene->m90, m124, chara)) {
        return 1;
    }
    return 0;
}

// inferred: ends the event started by the script on the event manager
void GameEventScript::fn_803ACD08() {
    if (mE8 > 0) {
        lbl_8087F540->fn_8047F580();
        mE8 = 0;
        mEC = 0;
    }
}

// inferred: the event `id` has a condition of kind -2
int GameEventScript::fn_803ACD50(int id) {
    const ScriptEventConds_unk* e = (const ScriptEventConds_unk*)FindEvent_unk(id);
    if (e != 0) {
        for (int i = 0; i < e->mCount; i++) {
            if (e->mConds[i].m00 == -2) {
                return 1;
            }
        }
    }
    return 0;
}

extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" int fn_80051CD8(const atn::Sphere* sphere, const atn::ColliBoxShape_unk* box); // inferred: sphere in box

// The original calls both tests recursively out of line (not inlined into themselves)
// NONMATCHING: 99.96% - stack slots of the two matrices of RotateAxis_unk swapped
__declspec(noinline) int GameEventScript::fn_803ACE18(int index, const atn::Sphere* sphere, int group) {
    gs::EventBox& event = mScene->mE4[index];
    int hit = 0;
    if (group != 0) {
        if (event.m2C >= 0) {
            index = event.m2C;
        }
        for (unsigned int i = 0; i < m144[index].size() && hit == 0; i++) {
            hit = fn_803ACE18(m144[index][i], sphere, 0);
        }
    } else {
        atn::ColliBoxShape_unk box;
        box.mSize = event.mSize;
        atn::Matrix3x4 trans;
        PSMTXTrans(trans.m, event.mPos.x, event.mPos.y, event.mPos.z);
        box.mMtx = trans;
        atn::Matrix3x4::RotateAxis_unk(box.mMtx, 'y', event.m14);
        hit = fn_80051CD8(sphere, &box);
    }
    return hit;
}

extern "C" int fn_80051A88(const atn::Sphere* a, const atn::Sphere* b); // inferred: the spheres touch

__declspec(noinline) int GameEventScript::fn_803ACF88(int index, const atn::Sphere* sphere, int group) {
    gs::EventSphere& event = mScene->mEC[index];
    int hit = 0;
    if (group != 0) {
        if (event.m20 >= 0) {
            index = event.m20;
        }
        for (unsigned int i = 0; i < m14C[index].size() && hit == 0; i++) {
            hit = fn_803ACF88(m14C[index][i], sphere, 0);
        }
    } else {
        atn::Sphere bounds;
        bounds.mCenter = event.mPos;
        bounds.mRadius = event.m14;
        hit = fn_80051A88(sphere, &bounds);
    }
    return hit;
}

const char* boost::bad_lexical_cast::what() const { return lbl_8078B34C; }

const char* std::bad_cast::what() const { return lbl_8078B394; }
