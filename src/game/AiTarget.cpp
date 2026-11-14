// AI manager (AIManager, lbl_8087EE68) and its targets (AiTarget).
// Every function is written, but several are not at 100% yet (vf14, the AI script steps fn_80017128/fn_8001794C,
// the route search 0x80018D5C-0x8001A228), so the unit is not linked. The unit ends with the weak
// copies of std::runtime_error, boost::bad_function_call and the std::tr1::shared_ptr deleter
// (include/std/stdexcept.h; formerly the split std/runtime_error.cpp).
#include <boost/function.hpp>
#include <typeinfo>
#include "game/AiTarget.h"
#include "atn/DrawHelper.h"
#include "atn/Keyboard.h"
#include "atn/Memory.h"
#include "game/BattleTask.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/FriendManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/GameScene.h"
#include "game/GameTask.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "game/MapTask.h"
#include "game/PlayerTask.h"
#include "game/SkillEffectManager.h"
#include "game/TacView.h"
#include "game/TeamAIManager.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4); // AddBool
extern "C" void fn_800874C8(void* menu, const char* name, int* value, int min, int max, int step, int func,
                            int arg); // AddInt
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet data by id
extern "C" int fn_8021A984(HitInfo_unk* hit); // inferred: the skill is offensive (as in AI/AiCommon_unk.cpp)
extern "C" void fn_8001C1B4(); // inferred: AI/AI_archer_attack01.cpp (empty)
extern "C" void fn_8001C1B8();
extern "C" void fn_8001C1BC();
extern "C" int rand();
extern "C" AiScriptInput_unk* fn_8001C260();  // inferred: input block of the AI scripts (AI/AI_archer_attack01.cpp)
extern "C" AiScriptOutput_unk* fn_8001C26C(); // inferred: output block
extern "C" void fn_8001C1C0(void* script);    // inferred: runs an AI script
extern "C" void fn_800142C4(CharaTask* chara); // inferred: sets the character of the script
extern "C" void fn_800142CC(void* link);       // inferred: sets the link of the script

// inferred: link of the friend list (CharaTask + 0x14AC, as in game/Em301LShade.cpp)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};
inline CharaTask* NextChara_unk(CharaTask* chara) {
    return ((CharaListNode_unk*)chara)->mNext;
}

// inferred: render parameters (as FrameTime_unk in game/GimmickStateCommand.cpp)
struct FrameTime_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: frame speed
    // inferred: inline getter (with the member read directly the operands of the fadds are swapped)
    float GetDelta_unk() const { return m3A4; }
};
extern "C" FrameTime_unk* lbl_8087EFA8;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" BattleTask* lbl_8087F048;
extern "C" GameTask* lbl_8087F430;

// inferred: names shown by AIManager::vf14 (file statics addressed from the start of .rodata): states of
// CharaC58_unk::m00, modes of CharaC58_unk::mMode and kinds of character (AIManager::m78)
static const char* const sStateNames_unk[] = {
    "\x8F\x89\x8A\xFA\x8F\xF3\x91\xD4", // initial state
    "\x91\xD2\x8B\x40",                 // wait
    "\x83\x8F\x81\x5B\x83\x76",         // warp
    "\x83\x7C\x83\x43\x83\x93\x83\x67\x88\xDA\x93\xAE", // point move
    "\x89\x42\x82\xEA\x88\xDA\x93\xAE", // hide move
    "\x83\x6F\x83\x67\x83\x8B\x88\xDA\x93\xAE", // battle move
    "\x92\xC7\x90\xD5",                 // chase
    "\x89\x42\x82\xEA\x82\xE9",         // hide
    "\x8B\xDF\x8B\x97\x97\xA3\x8D\x55\x8C\x82", // short range attack
    "\x89\x93\x8B\x97\x97\xA3\x8D\x55\x8C\x82", // long range attack
    "\x93\xA6\x82\xB0\x82\xE9",         // run away
    "\x83\x41\x83\x43\x83\x68\x83\x8A\x83\x93\x83\x4F", // idling
    "\x89\xF1\x95\x9C",                 // heal
    "\x96\x68\x8C\xE4\x91\xCC\x90\xA8", // guard stance
    0,
};
static const char* const sModeNames_unk[] = {
    "\x91\xD2\x8B\x40",                 // wait
    "\x96\xDA\x93\x49\x92\x6E\x88\xDA\x93\xAE", // move to the destination
    "\x89\x42\x82\xEA\x82\xE9\x88\xDA\x93\xAE", // hide move
    "\x83\x4C\x83\x83\x83\x89\x88\xDA\x93\xAE", // chara move
    "\x83\x6F\x83\x67\x83\x8B\x88\xDA\x93\xAE", // battle move
    "\x8B\xDF\x8B\x97\x97\xA3\x8D\x55\x8C\x82", // short range attack
    "\x89\x93\x8B\x97\x97\xA3\x8D\x55\x8C\x82", // long range attack
    "\x83\x58\x83\x4C\x83\x8B",         // skill
    "\x83\x7C\x83\x43\x83\x93\x83\x67\x83\x4B\x81\x5B\x83\x68", // point guard
    "\x83\x4C\x83\x83\x83\x89\x83\x4B\x81\x5B\x83\x68", // chara guard
    "\x83\x5F\x83\x43\x83\x75",         // dive
    "\x83\x8F\x81\x5B\x83\x76",         // warp
    "\x83\x8F\x81\x5B\x83\x76\x83\x7C\x83\x43\x83\x93\x83\x67\x88\xDA\x93\xAE", // warp point move
    "\x83\x4C\x83\x83\x83\x89\x92\xC7\x90\xD5", // chara chase
    "\x89\xF1\x95\x9C\x83\x54\x81\x5B\x83\x4E\x83\x8B\x88\xDA\x93\xAE", // heal circle move
    "\x89\xF1\x95\x9C\x83\x54\x81\x5B\x83\x4E\x83\x8B\x83\x4B\x81\x5B\x83\x68", // heal circle guard
    "\x8B\x92\x93\x5F\x83\x47\x83\x8A\x83\x41\x88\xDA\x93\xAE", // base area move
    "\x96\x68\x8C\xE4\x91\xCC\x90\xA8", // guard stance
    "\x83\x4C\x83\x83\x83\x89\x88\xDA\x93\xAE+ROT", // chara move + rot
    "\x8C\xB3\x82\xCC\x88\xCA\x92\x75\x82\xC9\x96\xDF\x82\xE9", // back to the original position
    "1\x83\x82\x81\x5B\x83\x56\x83\x87\x83\x93", // 1 motion
    "2\x83\x82\x81\x5B\x83\x56\x83\x87\x83\x93", // 2 motions
    "3\x83\x82\x81\x5B\x83\x56\x83\x87\x83\x93", // 3 motions
    0,
};
static const char* const sKindNames_unk[] = {"[TPC]", "[Enemy]", "[Friend]", "[Player]", "[NPC]", 0};

AIManager* AIManager::fn_800144FC(atn::Task* parent) {
    if (lbl_8087EE68 == 0) {
        lbl_8087EE68 = new ((MemoryTag)3, "", "", 0) AIManager(parent);
    }
    return lbl_8087EE68;
}

AIManager::AIManager(atn::Task* parent)
    : atn::Task(parent), m48(0), m4C(0), m50(0), m54(0), m58(0), m78(0), m7C(0), m80(0), m84(0), m88(0), m8C(0),
      m90(0), m94(0.0f), m98(1), mDC(0), m7D0(-1), m7D4(450), m7D8(1) {
    fn_8001C1B4();
    void* menu = mMenu.Load("AIManager");
    fn_80087994(menu, "DebugMode", &m50, 0, 0);
    fn_80087994(menu, "DrawHate", &m54, 0, 0);
    void* target = fn_8008937C(menu, "Target");
    fn_800874C8(target, "AttackTargetFrame", &m7D0, -1, 9999, 1, 0, 0);
    fn_800874C8(target, "SupportTargetFrame", &m7D4, -1, 9999, 1, 0, 0);
    fn_80087994(target, "OffensiveWithTarget", &m7D8, 0, 0);
    memset(m9C, 0, sizeof(m9C));
    TeamAIManager::Create_unk(this);
}

AIManager::~AIManager() {
    lbl_8087EE68 = 0;
    fn_8001C1B8();
}

int AIManager::vf0C() {
    if (!fn_800D3FA4()) {
        return 0;
    }
    fn_800D246C(1);
    return 1;
}

void AIManager::vf10() {
    m94 += lbl_8087EFA8->GetDelta_unk();
    m98 = 0;
    while (m94 >= 1.0f) {
        m94 -= 1.0f;
        m98++;
    }
    mDC = 300;
    m90 = (m90 + 1) % 4;
    fn_80018834();
}

// NONMATCHING: 93% - register allocation of the pointers (this/chara/scene/res...), argument order of the
// flag words of "Task"/"Action" and the load of the cone angle
// inferred: debug view of the AI (DebugMode): the keys of the USB keyboard pick a character of the kind m78
// and its AI state is drawn in a panel, with its route points and its view cone; DrawHate lists its hate
// toward the characters of the battle. The third flag (m58) shows the values kept by the AI scripts.
// Names of the locals invented.
void AIManager::vf14() {
    if (m50 != 0 && (lbl_8087EF10 == 0 || lbl_8087EF10->m04 == 0)) {
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0xF1B8) ||
            atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0xF1B2)) {
            if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0xF1B8)) {
                if (--m78 < 0) {
                    m78 = 4;
                }
            } else if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0xF1B2)) {
                if (++m78 >= 5) {
                    m78 = 0;
                }
            }
        } else if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT4_unk, 0xF1B4)) {
            switch (m78) {
            case 0:
                if (m88 != 0) {
                    NpcTask* npc = m88->fn_80547E44();
                    while (npc != 0 && npc->m1428 == 0) {
                        npc = npc->fn_80547E44();
                    }
                    if (npc == 0) {
                        npc = NpcManager::sInstance->Last_unk();
                        while (npc != m88 || npc->m1428 == 0) {
                            npc = npc->fn_80547E44();
                        }
                    }
                    m88 = npc;
                }
                break;
            case 1:
                if (m8C != 0) {
                    if (m8C->fn_8035B854() != 0) {
                        m8C = m8C->fn_8035B854();
                    } else {
                        m8C = EnemyManager::sInstance->Last_unk();
                    }
                }
                break;
            case 2:
                if (m80 != 0) {
                    if (((PlayerTask*)m80)->fn_8054A68C() != 0) {
                        m80 = ((PlayerTask*)m80)->fn_8054A68C();
                    } else {
                        m80 = lbl_8087F8A0->Last_unk();
                    }
                }
                break;
            case 4:
                if (m84 != 0) {
                    NpcTask* npc = m84->fn_80547E44();
                    while (npc != 0 && npc->m1428 != 0) {
                        npc = npc->fn_80547E44();
                    }
                    if (npc == 0) {
                        npc = NpcManager::sInstance->Last_unk();
                        while (npc != 0 && npc != m84 || npc->m1428 != 0) {
                            npc = npc->fn_80547E44();
                        }
                    }
                    m84 = npc;
                }
                break;
            }
        } else if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT4_unk, 0xF1B6)) {
            switch (m78) {
            case 0:
                if (m88 != 0) {
                    NpcTask* npc = m88->mNext;
                    while (npc != 0 && npc->m1428 == 0) {
                        npc = npc->mNext;
                    }
                    if (npc == 0) {
                        npc = NpcManager::sInstance->mHead;
                        while (npc->m1428 == 0) {
                            npc = npc->mNext;
                        }
                    }
                    m88 = npc;
                }
                break;
            case 1:
                if (m8C != 0) {
                    if (m8C->mNext != 0) {
                        m8C = m8C->mNext;
                    } else {
                        m8C = EnemyManager::sInstance->mHead;
                    }
                }
                break;
            case 2:
                if (m80 != 0) {
                    if (NextChara_unk(m80) != 0) {
                        m80 = NextChara_unk(m80);
                    } else {
                        m80 = FriendManager::sInstance->fn_8036554C();
                    }
                }
                break;
            case 4:
                if (m84 != 0) {
                    NpcTask* npc = m84->mNext;
                    while (npc != 0 && npc->m1428 != 0) {
                        npc = npc->mNext;
                    }
                    if (npc == 0) {
                        npc = NpcManager::sInstance->mHead;
                        while (npc != 0 && npc->m1428 != 0) {
                            npc = npc->mNext;
                        }
                    }
                    m84 = npc;
                }
                break;
            }
        }
    }

    if (m50 != 0) {
        atn::DrawHelper* dh = lbl_8087EEB0;
        GameScene* scene = lbl_8087F430->m10D8;
        if (dh == 0 || scene == 0) {
            return;
        }
        char buf[256];
        float z = -100.0f;
        float left = 10.0f;
        float width = 250.0f;
        float height = 430.0f;
        float step = 15.0f;
        float size = 12.0f;
        dh->fn_80060D58(left, left, z, width, height, 0x90000000);
        float x0 = 5.0f + left;
        float x1 = 5.0f + (0.5f * width + left);
        float y = step;
        float v0 = 82.0f + left;
        float v1 = 82.0f + (0.5f * width + left);
        unsigned int labelColor = 0xD0FFFFAA;
        CharaTask* chara = 0;
        switch (m78) {
        case 0:
            chara = m88;
            break;
        case 1:
            chara = m8C;
            break;
        case 2:
            chara = m80;
            break;
        case 3:
            chara = m7C;
            break;
        case 4:
            chara = m84;
            break;
        }
        dh->fn_800616C0(x0, y, z - 1.0f, size, size, sKindNames_unk[m78], 0xFFFFFFFF, 0.0f, 1, 1, 0);
        y += step;
        if (chara != 0) {
            chr::CharaData_unk* data = chara->m5C;
            CharaRes_unk* res = chara->m64;
            CharaC58_unk* ai = &chara->mC58;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "CharaID", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%s\0", chara->fn_8017C904());
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "Name", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%s\0", chara->m60->m0C);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "ScriptID", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d\0", chara->m58);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            dh->fn_800616C0(x1, y, z - 1.0f, size, size, "Unique", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d\0", chara->mId);
            dh->fn_800616C0(v1, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "AI", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            if (res != 0) {
                // inferred: the name of the AI script follows the id
                sprintf(buf, "%04d_%02d (%s)\0", res->m04 / 100, res->m04 % 100, (const char*)res->m08_unk);
                dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            }
            y += step;
            y += 5.0f;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "State", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            if (0 <= ai->m00 && ai->m00 < 14) {
                sprintf(buf, "%s\0", sStateNames_unk[ai->m00]);
            } else {
                sprintf(buf, "%d\0", ai->m00);
            }
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "ActMode", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            if (0 <= ai->mMode && ai->mMode < 23 && 0 <= ai->mE4 && ai->mE4 < 23) {
                sprintf(buf, "Now:%s Old:%s Sub:%d\0", sModeNames_unk[ai->mMode], sModeNames_unk[ai->mE4], ai->mStep);
            } else {
                sprintf(buf, "Now:%d Old:%d Sub:%d\0", ai->mMode, ai->mE4, ai->mStep);
            }
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "ActCount", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d\0", ai->mEC);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            dh->fn_800616C0(x1, y, z - 1.0f, size, size, "ActEndFlg", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d\0", ai->mDone_unk);
            dh->fn_800616C0(v1, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "Now", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            if (ai->m0C.mDestPoint > 0) {
                sprintf(buf, "%d\0", scene->m98.mData[ai->m0C.mDestPoint - 1].mId);
            } else {
                sprintf(buf, "%d\0", 0);
            }
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            dh->fn_800616C0(x1, y, z - 1.0f, size, size, "Next", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            if (ai->m0C.mPointA > 0) {
                sprintf(buf, "%d\0", scene->m98.mData[ai->m0C.mPointA - 1].mId);
            } else {
                sprintf(buf, "%d\0", 0);
            }
            dh->fn_800616C0(v1, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "Dest", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            if (ai->m0C.mPointB > 0) {
                sprintf(buf, "%d\0", scene->m98.mData[ai->m0C.mPointB - 1].mId);
            } else {
                sprintf(buf, "%d\0", 0);
            }
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            dh->fn_800616C0(x1, y, z - 1.0f, size, size, "Hide", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            if (ai->m0C.mLink > 0) {
                sprintf(buf, "%d\0", scene->mAC.mData[ai->m0C.mLink - 1].mId);
            } else {
                sprintf(buf, "%d\0", 0);
            }
            dh->fn_800616C0(v1, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "Time", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d\0", ai->mBC);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "StateTimer", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d\0", ai->m04);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "AreaID", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d / %d\0", ai->mB4, ai->mB8);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            dh->fn_800616C0(x1, y, z - 1.0f, size, size, "ShootAreaID", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            if (scene->fn_803C1884(ai->mB4) != 0) {
                sprintf(buf, "%d\0", scene->fn_803C1884(ai->mB4)->m28);
            } else {
                sprintf(buf, "0\0");
            }
            dh->fn_800616C0(v1, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "ArmsNo", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d\0", chara->m674 + 1);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            dh->fn_800616C0(x1, y, z - 1.0f, size, size, "GroupID", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            if (ai->mD4 != 0) {
                int group = ai->mD4->m218 != 0 ? ai->mD4->m218->m04 : 0;
                sprintf(buf, "%d\0", group);
                dh->fn_800616C0(v1, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            }
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "BattleMode", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d\0", ai->mC0);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            dh->fn_800616C0(x1, y, z - 1.0f, size, size, "ActionFlag", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%d\0", ai->mD8);
            dh->fn_800616C0(v1, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            y += 5.0f;
            for (unsigned int i = 0; i < 8; i++) {
                sprintf(buf, "L%02d=%.1f\0", i + 1, ai->m140[i]);
                dh->fn_800616C0(x0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
                sprintf(buf, "L%02d=%.1f\0", i + 9, ai->m140[i + 8]);
                dh->fn_800616C0(75.0f + x0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
                sprintf(buf, "L%02d=%.1f\0", i + 17, ai->m140[i + 16]);
                dh->fn_800616C0(150.0f + x0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
                y += step;
            }
            y += 5.0f;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "Task", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "Q:%d P:%d F:%d\0", chara->IsFlag_unk(2), chara->IsFlag_unk(1), chara->IsFlag_unk(4));
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "CtrlMode", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "Now:%d Old:%d Sub:%d\0", chara->m55C, chara->m560, chara->m564);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "Action", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "Hide:%d Crouch:%d Guard:%d\0", chara->mFlag31, chara->mFlag6, chara->mFlag5);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);
            y += step;
            dh->fn_800616C0(x0, y, z - 1.0f, size, size, "Comment", 0xD0FFFFAA, 0.0f, 1, 1, 0);
            sprintf(buf, "%s\0", (const char*)ai->m1B0);
            dh->fn_800616C0(v0, y, z - 1.0f, size, size, buf, 0xD0FFFFFF, 0.0f, 1, 1, 0);

            if (ai->m0C.mDestPoint > 0 && ai->m0C.mDestPoint <= scene->m74) {
                gs::MovePoint* p = &scene->m98.mData[ai->m0C.mDestPoint - 1];
                dh->fn_80062C8C(12, 0xFF99FF99, p->mPos.x, 4.0f + p->mPos.y, p->mPos.z, 0.0f, p->m14, 1.0f);
            }
            if (ai->m0C.mPointA > 0 && ai->m0C.mPointA <= scene->m74) {
                gs::MovePoint* p = &scene->m98.mData[ai->m0C.mPointA - 1];
                dh->fn_80062C8C(12, 0xFF9999FF, p->mPos.x, 3.0f + p->mPos.y, p->mPos.z, 0.0f, p->m14, 1.0f);
            }
            if (ai->m0C.mPointB > 0 && ai->m0C.mPointB <= scene->m74) {
                gs::MovePoint* p = &scene->m98.mData[ai->m0C.mPointB - 1];
                dh->fn_80062C8C(12, 0xFFFF9999, p->mPos.x, 2.0f + p->mPos.y, p->mPos.z, 0.0f, p->m14, 1.0f);
            }
            if (ai->m0C.mLink > 0 && ai->m0C.mLink <= scene->mA8) {
                gs::HidePoint* p = &scene->mAC.mData[ai->m0C.mLink - 1];
                dh->fn_80062C8C(12, 0xFFFFFF99, p->mPos.x, 2.0f + p->mPos.y, p->mPos.z, 0.0f, 3.0f, 1.0f);
            }
            atn::Vector3 top = chara->mPos;
            top.y += 17.0f;
            dh->fn_800638B0(0.0f, 7.0f, top + atn::Vector3(0.0f, 15.0f, 0.0f), top, 0xFF00FFFF);
            if (data != 0 && res != 0) {
                atn::Vector3 pos = chara->mPos;
                float angle = 0.017453292f * res->m50;
                atn::Vector3 rot = chara->mRot;
                dh->fn_80063484(12, 0xFFFFFF00, pos.x, 3.0f + pos.y, pos.z, rot.y, z, res->m4C, angle);
                dh->fn_80063200(12, 0xFFFF9900, pos.x, 3.0f + pos.y, pos.z, z, res->m58);
            }
            if (ai->m0C.mLink > 0) {
                dh->fn_80062C8C(12, 0xFF88FFFF, ai->m0C.m10.x, ai->m0C.m10.y, ai->m0C.m10.z, 0.0f, 1.0f, 10.0f);
            }

            if (m54 != 0) {
                dh->fn_80060D58(left + width, left, z, 180.0f, height, 0x90000000);
                x0 += width;
                dh->fn_800616C0(x0, step, z, size, size, "Hate", labelColor, 0.0f, 1, 1, 0);
                y = step + step;
                BattleChara_unk* other = lbl_8087F048->mCharas;
                int index = lbl_8087F048->fn_80102890(chara);
                if (index >= 0) {
                    BattleChara_unk* self = &other[index];
                    for (int i = 0; i < lbl_8087F048->mCharaCount; i++, other++) {
                        int hate = self->fn_801083D8(i);
                        sprintf(buf, "%s: %d, %.2f, %.2f\0", other->mChara->fn_8017C904(), hate, self->m128[i],
                                other->m248);
                        CharaTask* c = other->mChara;
                        if (c->CanAct_unk() && (c->mType == 0 || c->mC58.mC0 != 0)) {
                            unsigned int color = 0xFFFFFFFF;
                            if (chara->fn_8016F3D0(c, 1)) {
                                color = 0x80FFFFFF;
                            }
                            if (ai->mC4 == c) {
                                color = 0xFFFFB0B0;
                            }
                            dh->fn_800616C0(x0, y, z, size, size, buf, color, 0.0f, 1, 1, 0);
                            y += step;
                        }
                    }
                }
            }
        }
    }

    if (m58 != 0) {
        atn::DrawHelper* dh = lbl_8087EEB0;
        if (dh != 0) {
            char buf[256];
            float z = -100.0f;
            float x = 10.0f;
            float y = 330.0f;
            float step = 14.0f;
            float size = 13.0f;
            dh->fn_80060D58(x, y, z, 320.0f, 120.0f, 0x44000000);
            x += 5.0f;
            y += 5.0f;
            for (int i = 0; i < 4; i++) {
                sprintf(buf, "G%02d=%.1f\0", i + 1, m9C[i]);
                dh->fn_800616C0(x, y, z - 1.0f, size, size, buf, 0xFFFFFFFF, 0.0f, 1, 1, 0);
                sprintf(buf, "G%02d=%.1f\0", i + 5, m9C[i + 4]);
                dh->fn_800616C0(75.0f + x, y, z - 1.0f, size, size, buf, 0xFFFFFFFF, 0.0f, 1, 1, 0);
                sprintf(buf, "G%02d=%.1f\0", i + 9, m9C[i + 8]);
                dh->fn_800616C0(150.0f + x, y, z - 1.0f, size, size, buf, 0xFFFFFFFF, 0.0f, 1, 1, 0);
                sprintf(buf, "G%02d=%.1f\0", i + 13, m9C[i + 12]);
                dh->fn_800616C0(225.0f + x, y, z - 1.0f, size, size, buf, 0xFFFFFFFF, 0.0f, 1, 1, 0);
                y += step;
            }
        }
    }
}

// inferred: places every character at a random point of its route group, runs their AI once
void AIManager::fn_80016888() {
    lbl_8087FA00->Rebuild_unk();
    m98 = 1;
    fn_80017064(lbl_8087F8A0->mHead);
    for (CharaTask* c = FriendManager::sInstance->fn_8036554C(); c != 0; c = NextChara_unk(c)) {
        if (!c->mC58.mHeld_unk) {
            fn_80016A94(c, boost::function<int(gs::MovePoint*)>());
        }
        fn_80017064(c);
        c->mC58.m08 = c->mC58.m0C.mDestPoint;
    }
    for (NpcTask* n = NpcManager::sInstance->mHead; n != 0; n = n->mNext) {
        fn_80017064(n);
        n->mC58.m08 = n->mC58.m0C.mDestPoint;
    }
    for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
        if (!e->mC58.mHeld_unk) {
            fn_80016A94(e, boost::function<int(gs::MovePoint*)>());
        }
        fn_80017064(e);
        e->mC58.m08 = e->mC58.m0C.mDestPoint;
    }
    fn_8001C1BC();
    fn_800D246C(0);
    m8C = EnemyManager::sInstance->mHead;
    m80 = FriendManager::sInstance->fn_8036554C();
    m7C = lbl_8087F8A0->mHead;
    m84 = NpcManager::sInstance->mHead;
    while (m84 != 0 && m84->m1428 != 0) {
        m84 = m84->mNext;
    }
}

// NONMATCHING: 99.4% - stack slots of the local position and of the returned temporary swapped
// inferred: puts `chara` at a random point of its route group, facing a random direction
void AIManager::fn_80016A94(CharaTask* chara, boost::function<int(gs::MovePoint*)> filter) {
    int point = fn_80016C10(chara->mC58.mB4, filter);
    atn::Vector3 pos;
    pos = lbl_8087F430->m10D8->fn_803C11A4(point);
    atn::Vector3 rot = chara->mRot;
    rot.y = 6.2831855f * ((rand() % 1001) / 1000.0f) - 3.1415927f;
    chara->mPos = pos;
    chara->mRot = rot;
}

// inferred: weight of a move point (m14 plus five times m1C, at most 100); name invented
static inline int MoveWeight_unk(const gs::MovePoint* p) {
    int n = p->m1C < 100 ? p->m1C : 100;
    return (int)p->m14 + n * 5;
}

int AIManager::fn_80016C10(int group, boost::function<int(gs::MovePoint*)> filter) {
    GameScene* scene = lbl_8087F430->m10D8;
    int total = 0;
    atn::SimpleArray<int> weights;
    weights.Resize_unk(scene->m74, 0);
    memset(weights.mData, 0, weights.mSize * sizeof(int));
    if (group > 0) {
        for (int i = 1; i <= scene->m74; i++) {
            if (scene->m98.mData[i - 1].m2C != 0) {
                gs::MovePoint& p = scene->m98[i - 1];
                weights[i - 1] = 0;
                if (filter.empty()) {
                    if (group == p.m20) {
                        weights[i - 1] = MoveWeight_unk(&p);
                    }
                } else if (filter(&p)) {
                    weights[i - 1] = MoveWeight_unk(&p);
                }
            }
        }
    } else {
        for (int i = 1; i <= scene->m74; i++) {
            if (scene->m98.mData[i - 1].m2C != 0) {
                gs::MovePoint& p = scene->m98[i - 1];
                if (filter.empty() || filter(&p)) {
                    weights[i - 1] = MoveWeight_unk(&p);
                }
            }
        }
    }
    for (int i = 1; i <= scene->m74; i++) {
        if (scene->m98[i - 1].m2C != 0) {
            total += weights[i - 1];
        }
    }
    if (total == 0) {
        return -1;
    }
    int r = rand() % total;
    int found;
    for (int i = 1; i <= scene->m74; i++) {
        if (scene->m98[i - 1].m2C != 0) {
            if (r < weights[i - 1]) {
                found = i;
                break;
            }
            r -= weights[i - 1];
        }
    }
    return found;
}

void AIManager::fn_80016FB4() {
    mE0.clear();
    m458.fn_8003E918();
    m48C.clear();
    if (lbl_8087F8A0 != 0) {
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            p->mF18 = p->mF14;
            p->mF14 = 0;
            p->mC58.mB8 = 0;
        }
    }
    if (EnemyManager::sInstance != 0) {
        for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
            e->mF18 = e->mF14;
            e->mF14 = 1;
            e->mC58.mB8 = 0;
        }
    }
}

void AIManager::fn_80017060() {
    fn_80016FB4();
}

int AIManager::fn_80017064(CharaTask* chara) {
    if (chara->m55C != 2) {
        return 0;
    }
    if ((chara->m54C & 0x2000) == 0x2000 || chara->mC54 == 0) {
        return 0;
    }
    int ret = 0;
    for (int i = 0; i < m98; i++) {
        if (chara->mType == 1 && chara->mC50 == 1) {
            ret = fn_8001794C(chara);
        } else {
            ret = fn_80017128(chara);
        }
    }
    return ret;
}

// inferred: one step of the AI script of a character: fills the input block (state, targets, parameters of the
// character data, life and mana in percent, the values kept in CharaC58_unk::m140/m1A0 and AIManager::m9C), runs
// the script (fn_8001C1C0 on the script of the character data) and reads back the new state, the part to hold
// and the kept values
// inferred: AI block of a character (inlined accessor, as fn_800F530C of game/BattleTask.cpp)
inline CharaC58_unk* GetAi_unk(CharaTask* chara) {
    return &chara->mC58;
}

int AIManager::fn_80017128(CharaTask* chara) {
    GameScene* scene = lbl_8087F430->m10D8;
    CharaC58_unk* ai = GetAi_unk(chara);
    CharaRes_unk* res = chara->m64;
    if (scene == 0 || res == 0) {
        return 0;
    }
    int state = ai->m00;
    float targetDist = 9999.0f;
    if (ai->mC4 != 0) {
        atn::Vector3 d = chara->mPos - ai->mC4->mPos;
        targetDist = PSVECMag(&d) - ai->mC4->m5B0;
    }
    float followDist = 9999.0f;
    if (ai->mCC != 0) {
        atn::Vector3 d = chara->mPos - ai->mCC->mPos;
        followDist = PSVECMag(&d);
    }
    float life = 0.0f;
    float mana = 0.0f;
    if ((chara->mStatus.m0C & 0x20) != 0x20) {
        if (chara->mStatus.mParamB.m84 > 0) {
            life = 100.0f * chara->mStatus.m04 / (float)chara->mStatus.mParamB.m84;
        }
        if (chara->mStatus.mParamB.m88 > 0) {
            mana = 100.0f * chara->mStatus.m08 / (float)chara->mStatus.mParamB.m88;
        }
    }
    if (ai->m00 == 0) {
        int type = ai->m0C.mA0;
        ai->m0C.mDestPoint = scene->fn_803C1560(&chara->mPos, chara->fn_80179D44(), ai->mB4, type, 1, -1.0f);
    }
    AiScriptInput_unk* in = fn_8001C260();
    in->mChara = chara;
    in->mState = ai->m00;
    in->mDone = ai->mDone_unk;
    in->mFrame = ai->m04;
    in->mPos = chara->mPos;
    in->mTargetDist = targetDist;
    in->mTarget = ai->mC4;
    in->mFollowDist = followDist;
    in->mFollow = ai->mCC;
    in->mHasGimmick = ai->mD0 != 0;
    in->m30[0] = res->m58;
    in->m30[1] = res->m5C;
    in->m30[2] = res->m60;
    in->m3C[0] = res->m30;
    in->m3C[1] = res->m34;
    in->m3C[2] = res->m38;
    if (chara->mType == 3 && lbl_8087F430 != 0 && lbl_8087F430->fn_80370174(10) > 0) {
        in->m3C[1] *= 3;
        in->m3C[2] *= 3;
    }
    in->m48[0] = res->m44;
    in->m48[1] = res->m48;
    in->m48[2] = res->m3C;
    in->m48[3] = res->m40;
    const float* f = (const float*)res->m64_unk; // inferred: m64-m84 are read as floats here
    for (int i = 0; i < 9; i++) {
        in->m7C[i] = f[i];
    }
    in->mLife = life;
    in->mMana = mana;
    in->m60[0] = ai->m08;
    in->m60[1] = ai->m0C.mLink;
    in->m60[2] = chara->m674;
    in->m60[3] = ai->mC0;
    const float* kept = (const float*)ai->m140; // inferred: floats
    for (int i = 0; i < 24; i++) {
        in->mA8[i] = kept[i];
    }
    for (int i = 0; i < 16; i++) {
        in->m10C[i] = m9C[i];
    }
    const int* words = (const int*)ai->m1A0;
    for (int i = 0; i < 4; i++) {
        in->m170[i] = words[i];
    }
    fn_800142C4(chara);
    fn_800142CC(ai->mD4);
    fn_8001C1C0(&res->m08_unk);
    fn_800142C4(0);
    fn_800142CC(0);
    AiScriptOutput_unk* out = fn_8001C26C();
    if (out->mValid > 0) {
        ai->m00 = out->mState;
        if (out->mPart != chara->m674) {
            if (out->mPart >= 0) {
                if (out->mPart < (int)chara->mParts.mCount) {
                    chara->fn_8014DEE4(out->mPart, 0, 0);
                }
            } else {
                chara->fn_8014EEC4(0);
            }
        }
        ai->mC0 = out->mC0;
        float* keptOut = (float*)ai->m140;
        for (int i = 0; i < 24; i++) {
            keptOut[i] = out->m18[i];
        }
        for (int i = 0; i < 16; i++) {
            m9C[i] = out->m7C[i];
        }
        int* wordsOut = (int*)ai->m1A0;
        for (int i = 0; i < 4; i++) {
            wordsOut[i] = out->mE0[i];
        }
    }
    if (state != ai->m00) {
        ai->m04 = 0;
        ai->mDone_unk = 0;
    } else {
        ai->m04++;
    }
    if (ai->mD0 != 0 && ai->mD0->vf5C() == 0) {
        ai->mD0 = 0;
    }
    return 1;
}

// NONMATCHING: 97.8% - the int -> float conversions of the output loops store the 0x4330 word once in the
// prologue (the original stores it before each element in a slot shared with the float -> int ones, so the loops
// unroll by 6/4 instead of 8; same blocker as fn_80017128)
// inferred: AI step of the NPCs (characters of type 1 with mC50 == 1): runs every fourth frame (by the unique id),
// with a reduced input block (state, position, the floats of the character data and the kept values) and reads
// back the state and the kept values. Same script calls as fn_80017128.
int AIManager::fn_8001794C(CharaTask* chara) {
    if (m90 != chara->mId % 4) {
        chara->mC58.m04++;
        return 0;
    }
    GameScene* scene = lbl_8087F430->m10D8;
    CharaC58_unk* ai = GetAi_unk(chara);
    CharaRes_unk* res = chara->m64;
    if (scene == 0 || res == 0) {
        return 0;
    }
    int state = ai->m00;
    if (state == 0) {
        int type = ai->m0C.mA0;
        ai->m0C.mDestPoint = scene->fn_803C1560(&chara->mPos, chara->fn_80179D44(), ai->mB4, type, 1, -1.0f);
    }
    AiScriptInput_unk* in = fn_8001C260();
    in->mChara = chara;
    in->mState = ai->m00;
    in->mDone = ai->mDone_unk;
    in->mFrame = ai->m04;
    in->mPos = chara->mPos;
    in->m30[0] = res->m58;
    in->m30[1] = res->m5C;
    in->m30[2] = res->m60;
    const float* f = (const float*)res->m64_unk; // inferred: m64-m84 are read as floats here
    for (int i = 0; i < 9; i++) {
        in->m7C[i] = f[i];
    }
    in->m60[0] = ai->m08;
    for (int i = 0; i < 24; i++) {
        in->mA8[i] = ai->m140[i];
    }
    for (int i = 0; i < 16; i++) {
        in->m10C[i] = m9C[i];
    }
    fn_800142C4(chara);
    fn_800142CC(ai->mD4);
    fn_8001C1C0(&res->m08_unk);
    fn_800142C4(0);
    fn_800142CC(0);
    AiScriptOutput_unk* out = fn_8001C26C();
    if (out->mValid > 0) {
        ai->m00 = out->mState;
        for (int i = 0; i < 24; i++) {
            ai->m140[i] = out->m18[i];
        }
        for (int i = 0; i < 16; i++) {
            m9C[i] = out->m7C[i];
        }
    }
    if (state != ai->m00) {
        ai->m04 = 0;
        ai->mDone_unk = 0;
    } else {
        ai->m04++;
    }
    return 1;
}

void AIManager::fn_80017E90(CharaTask* chara, CharaTask* target) {
    if (lbl_8087F048 != 0) {
        lbl_8087F048->fn_80102A40(chara, target, 100000, -1);
    }
}

// inferred: gimmick `gimmick` of the current map (same loop as GimmickManager::fn_803EEE10); name invented
static inline GimmickCommon* FindGimmick_unk(int gimmick, int arg) {
    GameScene* scene = lbl_8087F430->m10D8;
    if (scene != 0 && scene->m64 != 0) {
        if (gimmick < 1000) {
            gimmick *= 1000;
        }
        for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
            if (gimmick == g->mId && arg == g->m4C) {
                return g;
            }
        }
    }
    return 0;
}

void AIManager::fn_80017EB0(CharaTask* chara, int gimmick, int arg) {
    chara->mC58.mD0 = FindGimmick_unk(gimmick, arg);
}

// inferred: nearest AI area of the group of `chara` (its position raised by the half height)
int AIManager::fn_80017F18(CharaTask* chara, atn::Vector3* pos) {
    GameScene* scene = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (scene != 0) {
        float best = 1.0e16f;
        int found = -1;
        for (unsigned int i = 0; i < scene->mF4.size(); i++) {
            gs::AiArea& area = scene->mF4[i];
            if (area.m2C != 0 && area.m24 == chara->mC58.mB8) {
                atn::Vector3 d = area.mPos - chara->mPos;
                float dist = d.LengthSq_unk();
                if (best > dist) {
                    best = dist;
                    found = i;
                }
            }
        }
        if (found >= 0) {
            gs::AiArea& area = scene->mF4[found];
            *pos = area.mPos + atn::Vector3(0.0f, area.mSize.y, 0.0f);
            return 1;
        }
    }
    return 0;
}

// inferred: segment and oriented box tested by fn_80051E90 (atn/ColliTest_unk; box as in gmk/RandomItem.cpp)
struct ColliSegment_unk {
    atn::Vector3 mFrom;
    atn::Vector3 mTo;
};
struct ColliBox_unk {
    atn::Vector3 mSize; // inferred: half size
    atn::Matrix3x4 mMtx;
};
extern "C" int fn_80051E90(const ColliSegment_unk* seg, const ColliBox_unk* box); // inferred: segment hits box

// NONMATCHING: 99.7% - r3/r5 swapped for the addresses of the first sum and seg.mFrom
// inferred: zero when a vertical segment through `chara` crosses an AI area of its group
int AIManager::fn_80018078(CharaTask* chara) {
    GameScene* scene = lbl_8087F430->m10D8;
    int ret = 0;
    if (scene != 0 && chara->mC58.mB8 > 0) {
        ColliSegment_unk seg;
        seg.mFrom = chara->mPos + atn::Vector3(0.0f, 20.0f, 0.0f);
        seg.mTo = chara->mPos + atn::Vector3(0.0f, -20.0f, 0.0f);
        for (unsigned int i = 0; i < scene->mF4.size(); i++) {
            gs::AiArea& area = scene->mF4[i];
            if (area.m2C != 0 && area.m24 == chara->mC58.mB8) {
                ColliBox_unk box;
                box.mSize = area.mSize;
                Mtx r;
                PSMTXRotRad(r, 'y', area.m14);
                box.mMtx = *(atn::Matrix3x4*)r;
                ret = 1;
                atn::Vector3 pos = area.mPos;
                pos.y += area.mSize.y;
                box.mMtx.m[0][3] = pos.x;
                box.mMtx.m[1][3] = pos.y;
                box.mMtx.m[2][3] = pos.z;
                if (fn_80051E90(&seg, &box) != 0) {
                    ret = 0;
                    break;
                }
            }
        }
    }
    return ret;
}

// inferred: nearest effect that can reach `chara` (position and radius)
int AIManager::fn_80018240(CharaTask* chara, atn::Vector3* pos, float* radius) {
    float best = 1.0e16f;
    SkillEffect* found = 0;
    for (unsigned int i = 0; i < 32; i++) {
        SkillEffect* e = lbl_8087F9E8->GetBullet_unk(i);
        if (e != 0 && e->IsActive_unk() && e->fn_80599C90(chara) && fn_8021A984(e->m04)) {
            atn::Vector3 d = chara->mPos - e->m10;
            float dist = d.LengthSq_unk();
            if (dist < best) {
                best = dist;
                found = e;
            }
        }
    }
    if (found != 0) {
        *pos = found->m10;
        *radius = found->m5C * found->m28;
        return 1;
    }
    return 0;
}

AiTarget* AIManager::fn_80018394(CharaTask* chara) {
    for (AiTarget* p = mE0.begin(); p != mE0.end(); p++) {
        if (p->m08 == 0 && p->m0C == chara) {
            return p;
        }
    }
    return 0;
}

// inferred: redirects `chara` to `target` for `frames` frames (AttackTargetFrame without them)
void AIManager::fn_800183E0(CharaTask* chara, CharaTask* target, HitInfo_unk* hit, int frames) {
    AiTarget t;
    t.m08 = 0;
    t.m0C = target;
    t.m04 = chara;
    t.m28 = hit;
    t.m30 = frames > 0 ? frames : m7D0;
    fn_800184F4(&t);
}

void AIManager::fn_800184F4(const AiTarget* target) {
    fn_80018608(target->m04, 0);
    m48C.push_back(*target);
}

AiTarget* AIManager::fn_800185B4(CharaTask* chara, HitInfo_unk* hit) {
    for (AiTarget* p = m48C.begin(); p != m48C.end(); p++) {
        if (p->m04 == chara && (hit == 0 || p->m28 == hit)) {
            return p;
        }
    }
    return 0;
}

// inferred: ends the redirections of `chara` (all of them, or those of `hit` or of its charged bullet)
void AIManager::fn_80018608(CharaTask* chara, HitInfo_unk* hit) {
    for (AiTarget* p = m48C.begin(); p != m48C.end();) {
        if (p->m04 == chara) {
            if (hit == 0) {
                if (lbl_8087F9F8 != 0) {
                    lbl_8087F9F8->fn_805AA014(p->m04, 6);
                }
                m48C.erase(p);
            } else {
                HitInfo_unk* charged = hit->mC8 > 0 ? fn_80219E6C(hit->mC8) : 0;
                if (p->m28 == hit || (charged != 0 && p->m28 == charged)) {
                    if (lbl_8087F9F8 != 0) {
                        lbl_8087F9F8->fn_805AA014(p->m04, 6);
                    }
                    m48C.erase(p);
                } else {
                    p++;
                }
            }
        } else {
            p++;
        }
    }
}

// inferred: per-frame update of the targets (vf10)
void AIManager::fn_80018834() {
    for (AiTarget* p = mE0.begin(); p != mE0.end();) {
        bool remove = false;
        if (p->m08 == 0 && p->m0C != 0 && !p->m0C->CanAct_unk()) {
            remove = true;
        }
        if (p->m30 >= 0 && --p->m30 == 0) {
            remove = true;
        }
        if (remove) {
            mE0.erase(p);
        } else {
            p++;
        }
    }
    if (m424.m08 != 4 && m424.m30 >= 0 && --m424.m30 < 0) {
        m424.fn_8003E918();
    }
    if (m458.m08 != 4) {
        SkillEffect* effect = (SkillEffect*)m458.m0C; // inferred: a healing magic
        for (PlayerTask* player = lbl_8087F8A0->mHead; player != 0; player = player->mNext) {
            player->fn_800EE794(effect);
        }
        m458.fn_8003E918();
    }
    for (AiTarget* p = m48C.begin(); p != m48C.end();) {
        bool remove = false;
        if (p->m04 != 0 && (p->m04->mFlags & 4) == 4) {
            remove = true;
        }
        if (p->m30 >= 0 && --p->m30 == 0) {
            remove = true;
        }
        if (remove) {
            if (p->m04 != 0) {
                if (lbl_8087F9F8 != 0) {
                    lbl_8087F9F8->fn_805AA014(p->m04, 6);
                }
                m48C.erase(p);
            } else {
                p++;
            }
        } else {
            p++;
        }
    }
}

// inferred: no effect of an enemy touches `chara`
int AIManager::fn_80018BAC(CharaTask* chara) {
    for (unsigned int i = 0; i < 32; i++) {
        SkillEffect* e = lbl_8087F9E8->GetBullet_unk(i);
        if (e != 0 && e->IsActive_unk() && fn_8021A984(e->m04) && e->fn_80599D60(chara, 1)) {
            return 0;
        }
    }
    return 1;
}

extern "C" double cos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void PSVECCrossProduct(const atn::Vector3* a, const atn::Vector3* b, atn::Vector3* axb);
extern "C" int fn_80019E88(AIManager* ai, atn::Vector3* out, int* kind, int point, gs::HideLink_unk* link);
extern "C" unsigned short fn_803CD958(U16Table_unk* links, int point); // inferred: the two points are linked

// NONMATCHING: 92.7% - scheduling of the loads of mB4 and of the last float argument
// inferred: the original module calls it with a C name (AI/AiCommon_unk), so it stays extern "C"
// inferred: route point for `chara` (its group, and the sub group while flag 2 of mD8 is set) in the
// direction it faces (AI/AiCommon_unk fn_8001BA88)
extern "C" int fn_80018C80(AIManager* ai, CharaTask* chara, int arg4, int arg5, float range, float min, float max) {
    int ret;
    if (chara != 0) {
        int sub = 0;
        if ((chara->mC58.mD8 & 2) == 2) {
            sub = chara->mC58.mB8;
        }
        ret = ai->fn_80018D5C(&chara->mPos, chara->mC58.mB4, sub, arg4, arg5, chara->fn_80179D44(), 1,
                              chara->mRot.y, range, min, max);
    } else {
        ret = 0;
    }
    return ret;
}

// NONMATCHING: 99.5% - registers of the scene (r28/r27) and stack slots of the front vector
// inferred: random move point seen from `pos` facing `dir` (cone of `range` degrees) between the distances
// min and max, linked to the nearest point (weighted by m1C when arg4 >= 0)
int AIManager::fn_80018D5C(const atn::Vector3* pos, int group, int arg3, int arg4, int arg5, int mask, int arg7,
                           float dir, float range, float min, float max) {
    GameScene* scene = lbl_8087F430->m10D8;
    int result = 0;
    if (scene != 0) {
        int start = scene->fn_803C1560(pos, mask, 0, 0, arg7, -1.0f);
        atn::Vector3 origin = scene->m98[start - 1].mPos;
        Mtx rot;
        PSMTXRotRad(rot, 'y', dir);
        atn::Vector3 axis(0.0f, 0.0f, 1.0f);
        atn::Vector3 front = axis;
        PSMTXMultVec(rot, &front, &front);
        float limit = cos(0.017453292f * (0.5f * range));
        int sum = 0;
        for (int i = 1; i <= scene->m74; i++) {
            gs::MovePoint& p = scene->m98[i - 1];
            if (p.m2C != 0 && (group <= 0 || p.m20 == group)) {
                sum += p.m1C;
                if (sum > 0) {
                    break;
                }
            }
        }
        if (sum <= 0) {
            arg4 = -1;
        }
        int found[256];
        int count = 0;
        for (int i = 1; i <= scene->m74; i++) {
            gs::MovePoint& p = scene->m98[i - 1];
            if (p.m2C == 0 || start == i || start == 0) {
                continue;
            }
            if ((int)fn_803CD958(&scene->mA0.mData[start - 1], i - 1) <= 0) {
                continue;
            }
            if (p.m1C < arg4) {
                continue;
            }
            if (arg5 >= 0 && p.m18 != arg5) {
                continue;
            }
            if (group > 0 && p.m20 != group) {
                continue;
            }
            if (arg3 > 0 && p.m24 != arg3) {
                continue;
            }
            atn::Vector3 diff = p.mPos - origin;
            atn::Vector3 d = diff;
            float dist = PSVECSquareMag(&d);
            PSVECNormalize(&d, &d);
            if (dist < min * min || max * max < dist) {
                continue;
            }
            if (PSVECDotProduct(&d, &front) >= limit && count < 255) {
                found[count++] = i;
            }
        }
        if (count > 0) {
            if (arg4 >= 0) {
                int total = 0;
                for (int k = 0; k < count; k++) {
                    total += scene->m98[found[k] - 1].m1C;
                }
                int r = rand() % total;
                for (int k = 0; k < count; k++) {
                    int w = scene->m98[found[k] - 1].m1C;
                    if (r < w) {
                        result = found[k];
                        break;
                    }
                    r -= w;
                }
            } else {
                result = found[rand() % count];
            }
        }
    }
    return result;
}

// NONMATCHING: 99.2% - register allocation (as fn_80018D5C)
// inferred: as fn_80018D5C from the move point `point` (the nearest one to `pos` when 0); when no point is
// in the cone, any linked point closer than `max` (game/CrowdSimulation)
extern "C" int fn_80019254(AIManager* ai, int point, const atn::Vector3* pos, int group, int arg3, int arg4,
                           float dir, float range, float min, float max) {
    GameScene* scene = lbl_8087F430->m10D8;
    int result = 0;
    if (scene != 0) {
        if (point == 0) {
            point = scene->fn_803C1560(pos, 0, 0, 0, 0, -1.0f);
        }
        atn::Vector3 origin = scene->m98[point - 1].mPos;
        Mtx rot;
        PSMTXRotRad(rot, 'y', dir);
        atn::Vector3 axis(0.0f, 0.0f, 1.0f);
        atn::Vector3 front = axis;
        PSMTXMultVec(rot, &front, &front);
        float limit = cos(0.017453292f * (0.5f * range));
        int sum = 0;
        for (int i = 1; i <= scene->m74; i++) {
            gs::MovePoint& p = scene->m98[i - 1];
            if (p.m2C != 0 && (group <= 0 || p.m20 == group)) {
                sum += p.m1C;
                if (sum > 0) {
                    break;
                }
            }
        }
        if (sum <= 0) {
            arg4 = -1;
        }
        int nears[256];
        int found[256];
        int nearCount = 0;
        int count = 0;
        for (int i = 1; i <= scene->m74; i++) {
            gs::MovePoint& p = scene->m98[i - 1];
            if (p.m2C == 0 || point == i || point == 0) {
                continue;
            }
            if ((int)fn_803CD958(&scene->mA0.mData[point - 1], i - 1) <= 0) {
                continue;
            }
            if (p.m1C < arg4) {
                continue;
            }
            if (group > 0 && p.m20 != group) {
                continue;
            }
            if (arg3 > 0 && p.m24 != arg3) {
                continue;
            }
            atn::Vector3 diff = p.mPos - origin;
            atn::Vector3 d = diff;
            float dist = PSVECSquareMag(&d);
            PSVECNormalize(&d, &d);
            if (max * max < dist) {
                continue;
            }
            if (nearCount < 255) {
                nears[nearCount++] = i;
            }
            if (dist < min * min) {
                continue;
            }
            if (PSVECDotProduct(&d, &front) >= limit && count < 255) {
                found[count++] = i;
            }
        }
        if (count == 0) {
            memcpy(found, nears, sizeof(found));
            count = nearCount;
        }
        if (count > 0) {
            if (arg4 >= 0) {
                result = found[0];
                int total = 0;
                for (int k = 0; k < count; k++) {
                    total += scene->m98[found[k] - 1].m1C;
                }
                int r = rand() % total;
                for (int k = 0; k < count; k++) {
                    int w = scene->m98[found[k] - 1].m1C;
                    if (r < w) {
                        result = found[k];
                        break;
                    }
                    r -= w;
                }
            } else {
                result = found[rand() % count];
            }
        }
    }
    return result;
}

extern "C" float PSVECMag(const atn::Vector3* v);

// inferred: visited hide points of fn_800197A4 (one bit per point)
static unsigned int sHideVisited_unk[0x80];

// inferred: normalizes unless the vector is (almost) zero; name invented (as in game/Em006DarkMaster.cpp)
static inline void SafeNormalize_unk(atn::Vector3* v) {
    if (!((float)__fabs(PSVECSquareMag(v)) < 1.1920929e-7f)) {
        PSVECNormalize(v, v);
    }
}

// NONMATCHING: 99% - register allocation and the order of the bit index computation
// inferred: nearest hide point for `chara` in the cone `dir`/`angle` that hides it from its target (mC58.mC4;
// the target is ignored when `arg` is set, without a target or in another group); writes the point beside it
// with fn_80019E88 (AI/AiCommon_unk)
extern "C" int fn_800197A4(AIManager* ai, atn::Vector3* out, int* kind, CharaTask* chara, const atn::Vector3* dir,
                           int arg, float angle) {
    atn::Vector3 self = chara->mPos;
    atn::Vector3 enemy = chara->mPos;
    if (arg == 0) {
        CharaTask* target = chara->mC58.mC4;
        if (target == 0) {
            arg = 1;
        } else {
            int group = chara->mC58.mB4;
            enemy = target->mPos;
            if (group > 0 && target->mC58.mB4 > 0 && group != target->mC58.mB4) {
                arg = 1;
            }
        }
    }
    int best = 0;
    int bestLink = 0;
    int group = chara->mC58.mB4;
    int sub = chara->mC58.mB8;
    GameScene* scene = lbl_8087F430->m10D8;
    float bestDist = 1.0e16f;
    float limit = cos(0.017453292f * angle);
    atn::Vector3 away = enemy - self;
    float enemyDist = PSVECSquareMag(&away);
    if (!((float)__fabs(enemyDist) < 1.1920929e-7f)) {
        PSVECNormalize(&away, &away);
    }
    memset(sHideVisited_unk, 0, sizeof(sHideVisited_unk));
    for (unsigned int i = 0; i < scene->mB4.size(); i++) {
        gs::HideLink_unk* link = &scene->mB4.mData[i];
        if (link->m08 == 0) {
            continue;
        }
        if (chara->mFlag31 && link->mFrom + 1 == chara->mC38 && link->mTo + 1 == chara->mC3C) {
            continue;
        }
        if ((float)__fabs(link->mMid.y - self.y) > 15.0f) {
            continue;
        }
        gs::HidePoint* points[2];
        points[0] = &scene->mAC.mData[link->mFrom];
        points[1] = &scene->mAC.mData[link->mTo];
        atn::Vector3 side = points[1]->mPos - points[0]->mPos;
        float linkLen = PSVECSquareMag(&side);
        SafeNormalize_unk(&side);
        if (arg == 0) {
            atn::Vector3 toEnemy = enemy - link->mMid;
            SafeNormalize_unk(&toEnemy);
            atn::Vector3 up(0.0f, 1.0f, 0.0f);
            atn::Vector3 normal;
            PSVECCrossProduct(&up, &toEnemy, &normal);
            SafeNormalize_unk(&normal);
            if (PSVECDotProduct(&normal, &side) < 0.70710677f) {
                continue;
            }
        }
        if (linkLen < chara->m5B0 * chara->m5B0) {
            continue;
        }
        if (points[0]->m3C != 0 && points[1]->m3C != 0) {
            continue;
        }
        for (unsigned int k = 0; k < 2; k++) {
            gs::HidePoint* p = points[k];
            unsigned int word = sHideVisited_unk[p->mIndex / 32];
            unsigned int mask = 1 << (p->mIndex % 32);
            if ((mask & word) == mask) {
                continue;
            }
            sHideVisited_unk[p->mIndex / 32] = word | mask;
            if (points[k]->m38 == 0) {
                continue;
            }
            if (fn_80019E88(ai, 0, 0, points[k]->mIndex, link) == 0) {
                continue;
            }
            if (group > 0 && group != points[k]->m1C) {
                continue;
            }
            if (sub > 0 && sub != points[k]->m20) {
                continue;
            }
            p = points[k];
            if (p->m24 != 0 && chara->mType != 0) {
                continue;
            }
            atn::Vector3 to = p->mPos - self;
            float dist = PSVECSquareMag(&to);
            if (dist > 0.0f) {
                PSVECNormalize(&to, &to);
            }
            if ((chara->mC58.mD8 & 4) != 4 && dist < 100.0f) {
                continue;
            }
            if (dist > 400.0f && PSVECDotProduct(&to, dir) < limit) {
                continue;
            }
            if (arg == 0) {
                atn::Vector3 a = enemy - self;
                if (PSVECSquareMag(&a) > 0.0f) {
                    PSVECNormalize(&a, &a);
                }
                atn::Vector3 b = enemy - points[k]->mPos;
                float e = PSVECSquareMag(&b);
                if (e > 0.0f) {
                    PSVECNormalize(&b, &b);
                }
                if (e < 900.0f && enemyDist > e) {
                    continue;
                }
                if (enemyDist > 6400.0f && 2.0f * (2.0f * enemyDist) < e) {
                    continue;
                }
                if (PSVECDotProduct(&away, &b) < 0.0f) {
                    continue;
                }
            }
            if (bestDist > dist) {
                bestDist = dist;
                bestLink = i;
                best = points[k]->mIndex;
            }
        }
    }
    if (best > 0) {
        fn_80019E88(ai, out, kind, best, &scene->mB4.mData[bestLink]);
    }
    return best;
}

// inferred: point beside the hide point `point` along its link (`link`, else the first link of the
// point); `kind` gets 3 for the points of kind 1-2, else 1/2 for the start/end of the link (game/
// RouteWalker_unk). The manager is not used
extern "C" int fn_80019E88(AIManager* ai, atn::Vector3* out, int* kind, int point, gs::HideLink_unk* link) {
    GameScene* scene = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
    if (scene == 0) {
        return 0;
    }
    if (link == 0) {
        for (unsigned int i = 0; i < scene->mB4.size(); i++) {
            gs::HideLink_unk* l = &scene->mB4.mData[i];
            if (l->mFrom + 1 == point || l->mTo + 1 == point) {
                link = l;
                break;
            }
        }
    }
    if (link == 0) {
        return 0;
    }
    gs::HidePoint& from = scene->mAC[link->mFrom];
    gs::HidePoint& to = scene->mAC[link->mTo];
    if (from.m3C != 0 || to.m3C != 0) {
        return 0;
    }
    atn::Vector3 dir = to.mPos - from.mPos;
    float len = PSVECMag(&dir);
    if (len > 0.0f) {
        PSVECNormalize(&dir, &dir);
    }
    gs::HidePoint& p = scene->mAC.mData[point - 1];
    if (p.m14 == 1 || p.m14 == 2) {
        if (out != 0) {
            float d = 0.2f * len;
            if (link->mFrom + 1 == point) {
                *out = p.mPos + dir * d;
            } else {
                *out = p.mPos - dir * d;
            }
        }
        if (kind != 0) {
            *kind = 3;
        }
        return 1;
    }
    if (p.m18 != 0) {
        if (out != 0) {
            float d = len / 2.0f;
            d = d < 4.0f ? d : 4.0f;
            if (link->mFrom + 1 == point) {
                *out = p.mPos + dir * d;
            } else {
                *out = p.mPos - dir * d;
            }
        }
        if (kind != 0) {
            if (link->mFrom + 1 == point) {
                *kind = 1;
            } else {
                *kind = 2;
            }
        }
        return 1;
    }
    return 0;
}

namespace std {
namespace tr1 {
namespace detail {

// 0x8001A34C
template <class T, class D>
void* shared_ptr_deleter<T, D>::get_deleter(const type_info& ti) const {
    return ti == typeid(D) ? (void*)&mPair : 0;
}

} // namespace detail
} // namespace tr1
} // namespace std

// inferred: stands in for the functions of the unit not written yet that call an empty
// boost::function0 (they instantiate the throw of bad_function_call); not in the original
void fn_runtime_error_unk(boost::function0<void>& f) {
    f();
}
