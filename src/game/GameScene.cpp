#include "game/GameScene.h"
#include "game/GameEventScript.h"
#include "game/CrowdManager.h"
#include "game/ColliAttrManager.h"
#include "game/ArenaTask.h"
#include "game/SongManager.h"
#include "game/EnvSeManager.h"
#include "game/GameBgm.h"
#include "game/TownTask.h"
#include "game/NpcTask.h"
#include "game/PlayerManager.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/Inventory.h"
#include "game/GimmickManager.h"
#include "gmk/TreasureBox.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/Model_unk.h"
#include "atn/SoundCore.h"
#include "std/string.h"
#include "std/vector.h"
#include "game/IntVector_unk.h"
#include "atn/MemoryManager.h"
#include "atn/ColliBase.h"
#include <boost/tokenizer.hpp>

typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;

extern "C" int abs(int x);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" unsigned int fn_80089ACC(const void* array); // inferred: SimpleArray size (weak copy of atn/Model.cpp)
extern "C" int rand();
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ED34(void* world, void* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" void fn_80016888(void* arg);
extern "C" void* lbl_8087EE68;
extern "C" void fn_80394C68(GameEventScript* script); // inferred
struct Field_unk;
extern "C" int fn_80375184(Field_unk* field);
extern "C" int fn_80370174(Field_unk* field, int index); // inferred: reads a value of the field
extern "C" int fn_80370A78(Field_unk* field, int index); // inferred: reads a flag of the field
extern "C" int fn_80435CBC(GimmickCommon* item); // inferred: gmk::RandomItem (game/RandomItem.cpp)
extern "C" int fn_8043A530(GimmickCommon* item); // inferred: gmk::WindyItem (gmk/WindyItem.cpp)
extern "C" void fn_80370320(Field_unk* field, int index, int value, int arg3); // inferred: writes a value of the field
extern "C" gmk::TreasureBox::Data_unk* fn_8021ECD0(int id); // inferred: treasure data
extern "C" int fn_80051CD8(const atn::ColliSphereShape_unk* sphere, const atn::ColliBoxShape_unk* box); // inferred: sphere in box
extern "C" void fn_80376194(Field_unk* field, int on);
extern "C" int fn_80219544(int member); // inferred: character id of a party member
extern "C" unsigned int fn_800E0AA8(const IntVector_unk* v); // inferred: size (weak copy)
extern "C" int* fn_8028B74C(IntVector_unk* v, int i);        // inferred: element (weak copy)
extern "C" void fn_803CCF64(U16Table_unk* table, const unsigned short* values, unsigned int count);
extern "C" void fn_803CD598(U16Table_unk* table, atn::Csv* csv, unsigned int count);

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_8006AA20(const char* path); // inferred: the file exists
extern "C" std::string fn_8006B2D8(const std::string& path); // inferred: name without extension
extern "C" std::string& fn_8006AD24(std::string& path, const std::string& ext); // inferred: sets the extension
extern "C" std::string fn_80393BEC(const int& value); // inferred: lexical_cast<std::string>(int)
extern "C" int fn_8049D52C(char* name, int area, int map, int sub); // inferred: name of a map
extern "C" void* fn_8049D704(atn::Task* parent, int area, int map, int sub); // inferred: map data
extern "C" GameEventScript* fn_80393610(atn::Task* parent, const char* path, int area, int map); // inferred: event script
extern "C" int fn_803CE160(int area, int map, int sub); // inferred: the map uses the town BGM groups
extern "C" void fn_800C3094(const char* group); // inferred: loads a sound group
extern "C" int fn_800C3118(const char* group); // inferred: the sound group is loaded
extern "C" void fn_800C317C();
extern "C" void fn_800C31EC();
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes a task
extern "C" void fn_8036FE74(void* field, atn::Vector3* pos, atn::Vector3* rot); // inferred: start of the player
extern "C" void fn_801027A4(void* bullets, void* chara);
extern "C" void fn_803EC374(atn::Task* parent, gs::Gimmick* gimmick); // inferred: creates a gimmick of the scene
extern "C" GimmickCommon* fn_803EC16C(atn::Task* parent, int item, int index); // inferred: creates a gimmick
extern "C" void fn_8047E5DC(void* mgr, int id);
extern "C" int fn_8047F268(void* mgr, int id);
extern "C" void fn_803B3C10(GameMessageTable_unk* table, const char* path); // inferred: loads the messages
extern "C" int fn_803B3CA8(GameMessageTable_unk* table); // inferred: nonzero while loading
extern "C" void* fn_803B3B38(void* table, int id); // inferred: message `id`
// inferred: entry of the save data (8 bytes, game/GameSaveData_unk.cpp)
struct SaveEntry_unk {
    int m00;
    unsigned char m04;
    unsigned char m05;
};
extern "C" SaveEntry_unk* fn_803BA0CC(void* save, int type, int id); // inferred: entry of type/id

// inferred: game flow manager (lbl_8087F420), only the members used here
struct GameFlow_unk {
    unsigned char m00[0x18];
    int m18;
    int m1C;
};
extern "C" GameFlow_unk* lbl_8087F420;

// inferred: game settings (lbl_8087F0A8), only the members used here
struct Settings_unk {
    unsigned char m000[0x7C];
    int m07C; // inferred: debug menu enabled
    unsigned char m080[0x158 - 0x80];
    int m158;
    unsigned char m15C[0x188 - 0x15C];
    int m188;
    unsigned char m18C[0x190 - 0x18C];
    int m190; // inferred: town side quests
    unsigned char m194[0x5B8 - 0x194];
    int m5B8;
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: area data returned by fn_8021771C (only the members used here)
struct AreaData_unk {
    unsigned char m00[0x44];
    int m44;
    unsigned char m48[0x68 - 0x48];
    const char* m68; // inferred: sound group of the area
};
extern "C" AreaData_unk* fn_8021771C(int area, int map, int sub);

// inferred: field (lbl_8087F430, as in game/CharaTask.cpp); only the members used here
struct Field_unk {
    unsigned char m0000[0x10D0];
    int m10D0;
    unsigned char m10D4[0x54E4 - 0x10D4];
    int m54E4; // inferred: mode (8: no BGM boxes)
    unsigned char m54E8[0x54F4 - 0x54E8];
    unsigned char m54F4[0x20]; // inferred: common message table (fn_803B3B38)
    int m5514;
    int m5518;
    int m551C;
    unsigned char m5520[0x5538 - 0x5520];
    int m5538;
    unsigned char m553C[0x5664 - 0x553C];
    int m5664;
    unsigned char m5668[0x5694 - 0x5668];
    int m5694; // inferred: night

    int fn_803761BC(); // inferred: debug field
};
extern "C" Field_unk* lbl_8087F430;

// inferred: map data (fn_8049D704)
struct MapData_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
    int m50; // inferred: map number
    unsigned char m54[0xE0 - 0x54];
    int mE0; // inferred: current camera
    int mE4;
    int mE8; // inferred: default camera
};
extern "C" void fn_804A04AC(void* map, int camera, int a, int b, gs::CameraBox* box); // inferred: sets the camera

// inferred: voice manager (lbl_8087F498)
struct Voice_unk {
    unsigned char m000[0x10C];
    int m10C;
};
extern "C" Voice_unk* lbl_8087F498;
extern "C" void* lbl_8087F610; // inferred: multiplayer session

extern "C" EnvSeManager* lbl_8087F418;
extern "C" TownTask* lbl_8087FA20;
extern "C" ArenaTask* lbl_8087EE78;
extern "C" void* lbl_8087F540;
extern "C" atn::Task* lbl_8087F048;
extern "C" PlayerManager* lbl_8087F8A0;
struct SoundManager_unk {
    unsigned char m0000[0x2984];
    atn::Sound3dListener m2984;
};
extern "C" SoundManager_unk* lbl_8087EFE8;

// inferred: chara radius (CharaTask::m620)
struct CharaRadius_unk {
    unsigned char m000[0x620];
    float m620;
};

// inferred: path of a scene file of the current map (inline: "li r0, 1" before the test)
inline bool MakeScenePath_unk(char* out, const char* fmt, int area, int map, int sub) {
    char name[0x100];
    if (fn_8049D52C(name, area, map, sub)) {
        sprintf(out, fmt, name);
        return true;
    }
    return false;
}

GameScene* GameScene::fn_803BEBAC(atn::Task* parent, int area, int map, int sub) {
    if (parent != 0) {
        return new ((MemoryTag)3, "", "", 0) GameScene(parent, area, map, sub);
    }
    return 0;
}

GameScene::GameScene(atn::Task* parent, int area, int map, int sub)
    : atn::Task(parent), m48(0), m64(0), m68(0), m6C(0), m70(0), m74(0), mA8(0), m134(0), m138(-1),
      m13C(1.0f), m140(0), m144(0), m9B0(0) {
    char name[0x20];
    char path[0x100];
    if (fn_8049D52C(name, area, map, sub)) {
        if (lbl_8087F420 != 0) {
            lbl_8087F420->m18 = 1;
            lbl_8087F420->m1C = 1;
        }
        sprintf(path, "levels/%s.pk", name);
        if (fn_8006AA20(path)) {
            if (lbl_8087F0A8->m5B8 == 0 || area == 1) {
                m994.fn_80470364(path, area != 1, 1, 1);
            }
        }
        for (unsigned int i = 0; i < 6; i++) {
            if (i == 0) {
                sprintf(path, "levels/%s_tmp.pk", name);
            } else {
                sprintf(path, "levels/%s_tmp%d.pk", name, i);
            }
            if (!fn_8006AA20(path)) {
                break;
            }
            m998[i].fn_80470364(path, 0, 1, 1);
        }
        if (area == 1) {
            if (fn_8006AA20("levels/town_sp.pk")) {
                m998[5].fn_80470364("levels/town_sp.pk", 0, 1, 1);
            }
        }
        if (lbl_8087F420 != 0) {
            lbl_8087F420->m18 = 0;
            lbl_8087F420->m1C = 0;
        }
    }
    char scene[0x100];
    if (MakeScenePath_unk(scene, "/game_scene/%s.gs", area, map, sub)) {
        m4C[0].vf0C(scene);
        if (area == 1 && lbl_8087F0A8->m190 != 0) {
            std::string sub("/game_scene/tw_subquest.gs");
            if (fn_8006AA20(sub.c_str()) && map != 3) {
                m4C[1].vf0C(sub.c_str());
            }
            sub = "/game_scene/tw_hintnpc.gs";
            if (fn_8006AA20(sub.c_str())) {
                m4C[2].vf0C(sub.c_str());
            }
        } else {
            for (unsigned int i = 1; i < 3; i++) {
                std::string file = fn_8006B2D8(std::string(scene));
                file += "_sub";
                file += fn_80393BEC(i + 1);
                file += ".gs";
                if (fn_8006AA20(file.c_str())) {
                    m4C[i].vf0C(file.c_str());
                }
            }
        }
        std::string script(scene);
        fn_8006AD24(script, std::string("sc"));
        m134 = fn_80393610(this, script.c_str(), area, map);
        m134->fn_800D246C(1);
        std::string envse(scene);
        fn_8006AD24(envse, std::string("envse"));
        if (fn_8006AA20(envse.c_str()) && lbl_8087F418 != 0) {
            lbl_8087F418->fn_8035F33C(envse.c_str());
        }
    }
    char message[0x100];
    if (MakeScenePath_unk(message, "/game_message/%s_xx.u16", area, map, sub)) {
        fn_803B3C10(&m11C, message);
    }
    m64 = fn_8049D704(this, area, map, sub);
    if (lbl_8087FA20 != 0) {
        lbl_8087FA20->m128 = 1;
    }
    if (ColliAttrManager::sInstance != 0) {
        ColliAttrManager::sInstance->fn_80183D00(area, map, sub);
    }
    if (fn_803CE160(area, map, sub) && area == 1) {
        int money = lbl_8087F430 != 0 ? lbl_8087F430->m10D0 : 0;
        if (map == 3) {
            fn_800C3094("GROUP_TWBGM004");
        } else if (money >= 0 && money < 400000) {
            fn_800C3094("GROUP_TWBGM001");
        } else if (money >= 400000 && money < 600000) {
            fn_800C3094("GROUP_TWBGM002");
        } else if (money >= 600000) {
            fn_800C3094("GROUP_TWBGM003");
        }
    }
    m144 = fn_8021771C(area, map, sub);
    if (m144 != 0 && !fn_800C3118(((AreaData_unk*)m144)->m68)) {
        fn_800C3094(((AreaData_unk*)m144)->m68);
    }
    if (lbl_8087F498 != 0 && lbl_8087F610 == 0) {
        if (area == 1) {
            lbl_8087F498->m10C = 2;
        } else {
            lbl_8087F498->m10C = 1;
        }
    }
    if (ArenaTask::fn_800431F4(area, map)) {
        ArenaTask::fn_8004318C(this);
    }
    if (lbl_8087F430 != 0 && SongManager::fn_805A2404(area, map, sub)) {
        SongManager::fn_805A2240(this);
    }
    m190.clear();
}

GameScene::~GameScene() {
    m98.Free_unk();
    mA0.Free_unk();
    mAC.Free_unk();
    if (lbl_8087F418 != 0) {
        lbl_8087F418->fn_8035E858();
    }
    int stop = 1;
    if ((mFlags & 2) == 2 && m144 != 0 && lbl_8087F430 != 0 && lbl_8087F430->m5538 != 0) {
        AreaData_unk* next = fn_8021771C(lbl_8087F430->m5514, lbl_8087F430->m5518, lbl_8087F430->m551C);
        if (next != 0 && next->m44 != 1 && strcmp(((AreaData_unk*)m144)->m68, next->m68) == 0) {
            stop = 0;
        }
    }
    if (m144 != 0 && stop) {
        fn_800C317C();
    }
    fn_800C31EC();
    if (SongManager::sInstance != 0) {
        if (lbl_8087F430->m5538 == 0 ||
            !SongManager::fn_805A2404(lbl_8087F430->m5514, lbl_8087F430->m5518, lbl_8087F430->m551C)) {
            fn_800D2338(SongManager::sInstance);
        }
    }
    for (unsigned int i = 0; i < mDC.size(); i++) {
        if (mDC[i].m04 != 0) {
            delete mDC[i].m04;
            mDC[i].m04 = 0;
        }
    }
    if (m140 != 0 && GameBgm::sInstance != 0) {
        GameBgm* bgm = GameBgm::sInstance;
        if (m140->mBgm == (bgm->m84 != 0 ? bgm->mNext.mId : bgm->mReq.mId)) {
            bgm->fn_8037D4C0(0, 60, 1, 0, 1.0f);
            m140 = 0;
        }
    }
}

int GameScene::vf0C() {
    if (m48 == 0) {
        int ready = 1;
        for (unsigned int i = 0; i < 3; i++) {
            if (m4C[i].fn_80473F50()) {
                ready = 0;
            }
        }
        if ((m134->mFlags & 2) != 2) {
            ready = 0;
        }
        if (lbl_8087F418->fn_8035E730()) {
            ready = 0;
        }
        if (ready) {
            m48++;
            for (unsigned int i = 0; i < 3; i++) {
                if (m4C[i].fn_80470580() && m4C[i].fn_8047059C()) {
                    fn_803C3894((const char*)m4C[i].fn_80470580(), m4C[i].fn_8047059C(), i);
                }
            }
            fn_803CAABC();
            for (unsigned int i = 0; i < 3; i++) {
                m4C[i].fn_80473F88();
            }
            atn::Vector3 pos;
            atn::Vector3 rot;
            fn_8036FE74(lbl_8087F430, &pos, &rot);
            if (lbl_8087FA20 != 0) {
                lbl_8087FA20->fn_805B7B04(pos);
            }
            if (lbl_8087F048 != 0) {
                fn_801027A4(lbl_8087F048, lbl_8087F8A0->mHead);
            }
            CrowdManager::fn_801EA4B0(this, (CrowdDesc_unk*)&mD4, m68);
            if (lbl_8087FA20 == 0) {
                for (unsigned int i = 0; i < m80.size(); i++) {
                    fn_803C0634(i);
                }
            }
            for (unsigned int i = 0; i < m88.size(); i++) {
                fn_803C0C2C(i);
            }
            for (unsigned int i = 0; i < m90.size(); i++) {
                fn_803C0938(i);
            }
            fn_803CA0F4();
            fn_803CA530();
            fn_803CA604();
            if (lbl_8087F0A8->m158 != 0) {
                lbl_8087F418->fn_8035E7CC();
            }
            atn::Vector3 listener = pos;
            atn::Vector3 dir = rot;
            listener.y += 1.0f;
            lbl_8087EFE8->m2984.fn_800C7F08(listener, dir);
            if (lbl_8087FA20 != 0) {
                m998[5].fn_80470528();
            }
        }
    } else if (m48 == 1) {
        if (fn_800D3FA4() && !fn_803B3CA8(&m11C) && GameBgm::sInstance->mState != 2) {
            for (unsigned int i = 0; i < m180.size(); i++) {
                if (m180[i] >= 0) {
                    fn_8047E5DC(lbl_8087F540, m180[i]);
                }
            }
            if (lbl_8087EE78 != 0) {
                lbl_8087EE78->fn_80043A24();
            }
            m48++;
        }
    } else if (m48 == 2) {
        int done = 1;
        for (unsigned int i = 0; i < m180.size(); i++) {
            if (m180[i] >= 0 && !fn_8047F268(lbl_8087F540, m180[i])) {
                done = 0;
                break;
            }
        }
        for (unsigned int i = 0; i < mDC.size(); i++) {
            if (((atn::Model*)mDC[i].m04)->fn_8008B140()) {
                done = 0;
                break;
            }
        }
        return done;
    }
    return 0;
}

// inferred: the event of one of the script ids of `point` is running (or done)
inline bool HasEvent_unk(GameEventScript* script, gs::NpcSetPoint& point, int hidden) {
    for (unsigned int i = 0; i < point.m140.size(); i++) {
        int ev = script->fn_8039CC24(point.m140[i]);
        if (ev != 0) {
            if ((hidden && script->fn_80398530(ev)) || (!hidden && script->fn_80398590(ev, 1))) {
                return true;
            }
        }
    }
    return false;
}

// inferred: bit 0 by day, bit 1 by night (0: always)
inline bool IsShown_unk(int when) {
    if (when != 0 && lbl_8087F430 != 0) {
        if (lbl_8087F430->m5694 == 0) {
            return when & 1;
        }
        return (when >> 1) & 1;
    }
    return true;
}

// NONMATCHING: 98.4% - register allocation (hidden flag r29 / point r23)
atn::Task* GameScene::fn_803C0634(unsigned int index) {
    int hidden = ((m80[index].m12C >> 11) & 1) ^ 1;
    if ((lbl_8087FA20 == 0 || lbl_8087F0A8->m188 == 0 || HasEvent_unk(m134, m80[index], hidden)) &&
        IsShown_unk(m80[index].m13C)) {
    NpcTask* npc = NpcTask::fn_80547C28(NpcManager::sInstance, (void*)m80[index].m20);
    npc->vf2C(m80[index].mName, (int)this);
    npc->fn_800D246C(1);
    npc->mPos = m80[index].mPos;
    npc->SetRot_unk(atn::Vector3(0.0f, m80[index].m14, 0.0f));
    npc->m1428 = 1;
    npc->m58 = m80[index].mId;
    npc->mC58.mHeld_unk = 1;
    if (lbl_8087F0A8->m07C != 0) {
        npc->vf30(NpcManager::sInstance->m58);
    }
    if (m80[index].m28 > 0) {
        npc->m510 = m80[index].m28 + 0xD5;
    }
    npc->m137C |= m80[index].m12C;
    if ((npc->m137C & 0x100) == 0x100) {
        npc->fn_80176DFC(1, 1, 1);
    }
    if (m9B0 != 0) {
        SaveEntry_unk* entry = fn_803BA0CC(&m9B0->m04, 1, m80[index].mId);
        if (entry != 0 && entry->m05 == 0) {
            npc->mFlagB20 = 0;
            npc->mMotion.fn_8008B978(1);
        }
    } else if ((npc->m137C & 0x80) == 0x80) {
        npc->mFlagB20 = 0;
        npc->mMotion.fn_8008B978(1);
    }
    return npc;
    }
    return 0;
}

// NONMATCHING: 96.4% - register allocation
atn::Task* GameScene::fn_803C0938(unsigned int index) {
    int hidden = ((m90[index].m12C >> 11) & 1) ^ 1;
    if ((lbl_8087FA20 == 0 || lbl_8087F0A8->m188 == 0 || HasEvent_unk(m134, m90[index], hidden)) &&
        IsShown_unk(m90[index].m13C)) {
    int id = m90[index].m20;
    if ((unsigned int)(id - 199900) <= 99) {
        id = fn_80219544(Inventory::instance_->mOrder[id % 100]);
    }
    PlayerTask* player = PlayerTask::fn_8054A3DC(lbl_8087F8A0, 3, (void*)id);
    player->fn_800D246C(1);
    player->mPos = m90[index].mPos;
    player->SetRot_unk(atn::Vector3(0.0f, m90[index].m14, 0.0f));
    player->m58 = m90[index].mId;
    player->mC58.mHeld_unk = 1;
    if (lbl_8087F048 != 0) {
        fn_801027A4(lbl_8087F048, player);
    }
    player->vf30(lbl_8087F8A0->m5C);
    if (m90[index].m28 > 0) {
        player->m510 = m90[index].m28 + 0xD5;
    }
    player->m137C |= m90[index].m12C;
    if (m9B0 != 0) {
        SaveEntry_unk* entry = fn_803BA0CC(&m9B0->m04, 3, m90[index].mId);
        if (entry != 0 && entry->m05 == 0) {
            player->mFlagB20 = 0;
            player->mMotion.fn_8008B978(1);
        }
    } else if ((player->m137C & 0x80) == 0x80) {
        player->mFlagB20 = 0;
        player->mMotion.fn_8008B978(1);
    }
    }
    return 0;
}

// NONMATCHING: 98.5% - register allocation
atn::Task* GameScene::fn_803C0C2C(unsigned int index) {
    int hidden = ((m88[index].m12C >> 11) & 1) ^ 1;
    if ((lbl_8087FA20 == 0 || lbl_8087F0A8->m188 == 0 || HasEvent_unk(m134, m88[index], hidden)) &&
        IsShown_unk(m88[index].m13C)) {
    EnemyTask* enemy = EnemyTask::fn_8035B534(EnemyManager::sInstance, (EnemyInfo_unk*)&m88[index], 0);
    enemy->vf2C(m88[index].mName, (int)this);
    enemy->fn_800D246C(1);
    enemy->mPos = m88[index].mPos;
    enemy->SetRot_unk(atn::Vector3(0.0f, m88[index].m14, 0.0f));
    enemy->m58 = m88[index].mId;
    if (m88[index].m140.size() == 0) {
        enemy->fn_80154654(enemy->mC50, m88[index].m1C);
    }
    enemy->mC58.mHeld_unk = 1;
    if (lbl_8087F048 != 0) {
        fn_801027A4(lbl_8087F048, enemy);
    }
    if (lbl_8087F0A8->m07C != 0) {
        enemy->vf30(EnemyManager::sInstance->m58);
    }
    if (m88[index].m28 > 0) {
        enemy->m510 = m88[index].m28 + 0xD5;
    }
    enemy->m137C |= m88[index].m12C;
    if ((enemy->m137C & 0x20) == 0x20) {
        enemy->mFlag16 = 1;
    }
    if (m9B0 != 0) {
        SaveEntry_unk* entry = fn_803BA0CC(&m9B0->m04, 2, m88[index].mId);
        if (entry != 0 && entry->m05 == 0) {
            enemy->mFlagB20 = 0;
            enemy->mMotion.fn_8008B978(1);
        }
    } else if ((enemy->m137C & 0x80) == 0x80) {
        enemy->mFlagB20 = 0;
        enemy->mMotion.fn_8008B978(1);
    }
    return enemy;
    }
    return 0;
}

void GameScene::fn_803C0F48(int warp) {
    for (unsigned int i = 0; i < 6; i++) {
        m998[i].fn_80470528();
    }
    for (unsigned int i = 0; i < m78.size(); i++) {
        if (warp == m78[i].mId) {
            lbl_8087F8A0->mHead->mPos = m78[i].mPos;
            lbl_8087F8A0->mHead->SetRot_unk(atn::Vector3(0.0f, m78[i].m14, 0.0f));
            Mtx m;
            PSMTXRotRad(m, 'y', m78[i].m14);
            atn::Vector3 dir = atn::Vector3(0.0f, 0.0f, 1.0f);
            PSMTXMultVec(m, &dir, &dir);
            break;
        }
    }
    lbl_8087F048->fn_800D246C(0);
    EnemyManager::sInstance->vf1C();
    NpcManager::sInstance->vf1C();
    fn_80016888(lbl_8087EE68);
    lbl_8087F8A0->vf1C();
    GimmickManager::instance_->fn_803EE374(0);
    m134->fn_800D246C(0);
    fn_80394C68(m134);
    CrowdManager::sInstance->mCenter = lbl_8087F8A0->mHead->mPos;
    CrowdManager::sInstance->fn_801EB078();
    PlayerTask* leader = lbl_8087F8A0->mHead;
    if (leader != 0) {
        leader->fn_801546F4(0, leader->mPos, 60.0f);
    }
    if (lbl_8087F430 != 0) {
        lbl_8087F430->m5664 = 1;
        int on = 0;
        if (lbl_8087F430->fn_803761BC() && fn_80375184(lbl_8087F430)) {
            on = 1;
        }
        fn_80376194(lbl_8087F430, on);
    }
    fn_803CAABC();
}

// NONMATCHING: 99.4% - f29-f31 allocation (y in f31 in the original)
atn::Vector3 GameScene::fn_803C11A4(int point) {
    if (point <= 0) {
        return atn::Vector3(0.0f, 0.0f, 0.0f);
    }
    gs::MovePoint& p = m98[point - 1];
    float angle = 6.2831855f * ((float)(rand() % 1001) / 1000.0f);
    float r = 0.9f * (p.m14 * (float)sin(0.5f * (3.1415927f * ((float)(rand() % 1001) / 1000.0f))));
    float z = r * (float)sin(angle) + p.mPos.z;
    atn::Vector3 out(r * (float)cos(angle) + p.mPos.x, p.mPos.y, z);
    return out;
}

atn::Vector3 GameScene::fn_803C1338(int point, const atn::Vector3* dir) {
    gs::MovePoint& p = m98[point - 1];
    atn::Vector3 out(p.mPos.x, p.mPos.y, p.mPos.z);
    atn::Vector3 off;
    do {
        float angle = 6.2831855f * ((float)(rand() % 1001) / 1000.0f);
        float r = 0.9f * (p.m14 * (float)sin(0.5f * (3.1415927f * ((float)(rand() % 1001) / 1000.0f))));
        float z = r * (float)sin(angle);
        off = atn::Vector3(r * (float)cos(angle), 0.0f, z);
    } while (PSVECDotProduct(dir, &off) < 0.0f);
    out += off;
    return out;
}

// inferred: candidate of GameScene::fn_803C1560 (name invented)
struct NearPoint_unk {
    NearPoint_unk() : mDist(1e16f), mDy(1e16f), mIndex(0) {}
    float mDist;
    float mDy;
    int mIndex;
    atn::Vector3 mPos;
};

int GameScene::fn_803C1560(const atn::Vector3* pos, int mask, int group, int type, int check, float radius) {
    NearPoint_unk near[4];
    int worst = 0;
    for (int i = 0; i < m74; i++) {
        gs::MovePoint& p = m98[i];
        if (p.m2C == 0) {
            continue;
        }
        if (group > 0 && p.m20 != group) {
            continue;
        }
        if (p.m28 != type) {
            continue;
        }
        if (p.m14 < radius) {
            continue;
        }
        float dy = p.mPos.y - pos->y;
        atn::Vector3 d(p.mPos.x - pos->x, dy, p.mPos.z - pos->z);
        float dist = dy * dy + d.LengthSq_unk();
        if (near[worst].mDist > dist) {
            near[worst].mDist = dist;
            near[worst].mDy = dy;
            near[worst].mIndex = i + 1;
            near[worst].mPos = p.mPos;
            worst = 0;
            float w = near[0].mDist;
            if (near[1].mDist > w) {
                w = near[1].mDist;
                worst = 1;
            }
            if (near[2].mDist > w) {
                w = near[2].mDist;
                worst = 2;
            }
            if (near[3].mDist > w) {
                worst = 3;
            }
        }
    }
    atn::Vector3 up(0.0f, 3.0f, 0.0f);
    int best = 0;
    float bestDist = 1e16f;
    for (int i = 0; i < 4; i++) {
        if (near[i].mIndex != 0 && bestDist > near[i].mDist) {
            if (check != 0) {
                atn::Vector3 from = *pos + up;
                atn::Vector3 to = near[i].mPos + up;
                if (fn_8004ED34(lbl_8087EE98, 0, &from, &to, mask | 0x80000000, 0, 0) != 0) {
                    continue;
                }
            }
            bestDist = near[i].mDist;
            best = near[i].mIndex;
        }
    }
    if (best == 0) {
        best = near[0].mIndex;
    }
    return best;
}

int GameScene::fn_803C17FC(int point) {
    for (int i = 0; i < m74; i++) {
        gs::MovePoint& p = m98[i];
        if (p.mId == point) {
            return i + 1;
        }
    }
    return 0;
}

int GameScene::fn_803C1840(int point) {
    for (int i = 0; i < mA8; i++) {
        gs::HidePoint& p = mAC[i];
        if (p.mId == point) {
            return i + 1;
        }
    }
    return 0;
}

gs::AiArea* GameScene::fn_803C1884(int id) {
    for (unsigned int i = 0; i < mF4.size(); i++) {
        if (mF4[i].m2C == 0 && id == mF4[i].m24) {
            return &mF4[i];
        }
    }
    return 0;
}

void* GameScene::fn_803C18DC(int type, int link) {
    for (unsigned int i = 0; i < mCC.size(); i++) {
        gs::InteractionConnection& c = mCC[i];
        gs::InteractionPoint& a = mC4[c.m00];
        gs::InteractionPoint& b = mC4[c.m04];
        if (a.m14 == type && b.m14 == type && a.mId == link) {
            return &c;
        }
    }
    return 0;
}

void GameScene::vf10() {
    fn_803C1CAC();
    fn_803C1E3C();
    for (unsigned int i = 0; i < mDC.size(); i++) {
        gs::ConstrainModel& c = mDC[i];
        if (c.m0C == 0 || c.m1C == 0) {
            continue;
        }
        atn::Matrix3x4 m;
        m.SetIdentity();
        if (0.0f != c.m10.z) {
            atn::Matrix3x4::RotateAxis_unk(m, 'z', c.m10.z);
        }
        if (0.0f != c.m10.y) {
            atn::Matrix3x4::RotateAxis_unk(m, 'y', c.m10.y);
        }
        if (0.0f != c.m10.x) {
            atn::Matrix3x4::RotateAxis_unk(m, 'x', c.m10.x);
        }
        PSMTXConcat(c.m0C->m, m.m, m.m);
        atn::Model* model = c.m04;
        __MtxCopy(model->mMtx.m, m.m);
        atn::Vector3 scale(atn::Vector3(m.m[0][0], m.m[1][0], m.m[2][0]).Length_unk(),
                           atn::Vector3(m.m[0][1], m.m[1][1], m.m[2][1]).Length_unk(),
                           atn::Vector3(m.m[0][2], m.m[1][2], m.m[2][2]).Length_unk());
        model->m54 = scale.x > (scale.y > scale.z ? scale.y : scale.z)
                         ? scale.x
                         : (scale.y > scale.z ? scale.y : scale.z);
        model->CalcMatrixStack(atn::ModelNodeCallback_unk());
    }
}

// inferred: leader of the party (null without a player manager)
inline CharaTask* GetLeader_unk() {
    return lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
}

// inferred: flag condition of a box (enabled, kind, flag, value)
inline int CheckFlag_unk(const int* cond) {
    int ok = 1;
    if (cond[0] != 0) {
        int v;
        if (cond[1] == 1) {
            v = fn_80370174(lbl_8087F430, cond[2]);
        } else {
            v = fn_80370A78(lbl_8087F430, cond[2]);
        }
        ok = v == cond[3];
    }
    return ok;
}

// inferred: time condition of a box (enabled, start, end)
inline int CheckTime_unk(const int* cond) {
    int ok = 1;
    if (cond[0] != 0 && lbl_8087F430 != 0) {
        int t = lbl_8087F430->m10D0;
        if (t < cond[1] || cond[2] < t) {
            ok = 0;
        }
    }
    return ok;
}

void GameScene::fn_803C1CAC() {
    CharaTask* leader = GetLeader_unk();
    if (leader == 0) {
        return;
    }
    for (unsigned int i = 0; i < m10C.size(); i++) {
        gs::CameraBox& box = m10C[i];
        box.m54 = CheckFlag_unk(box.m38) && CheckTime_unk(box.m48);
    }
    gs::CameraBox* box = fn_803C2090(m10C, leader);
    if (box != 0) {
        if (box->m24 != ((MapData_unk*)m64)->mE0) {
            fn_804A04AC(m64, box->m24, box->m28, box->m2C, box);
        }
    } else if (((MapData_unk*)m64)->mE8 != ((MapData_unk*)m64)->mE0) {
        fn_804A04AC(m64, ((MapData_unk*)m64)->mE8, -1, -1, 0);
    }
}

// inferred: BGM being played (or requested)
inline int GetBgm_unk(GameBgm* bgm) {
    return bgm->m84 != 0 ? bgm->mNext.mId : bgm->mReq.mId;
}

void GameScene::fn_803C1E3C() {
    CharaTask* leader = GetLeader_unk();
    if (leader == 0) {
        return;
    }
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8) {
        return;
    }
    for (unsigned int i = 0; i < m114.size(); i++) {
        gs::BgmBox& box = m114[i];
        box.m4C = CheckFlag_unk(box.m30) && CheckTime_unk(box.m40);
    }
    if (GameBgm::sInstance == 0) {
        return;
    }
    gs::BgmBox* box = fn_803CCDAC(m114, leader);
    if (box != 0) {
        if (box != m140 && box->mBgm != GetBgm_unk(GameBgm::sInstance)) {
            GameBgm::sInstance->fn_8037D4C0(box->mBgm, box->m2C, 1, 0, box->m28);
        }
    } else if (m140 != 0 && m140->mBgm == GetBgm_unk(GameBgm::sInstance)) {
        GameBgm::sInstance->fn_8037D4C0(0, 60, 1, 0, 1.0f);
    }
    m140 = box;
}

gs::CameraBox* GameScene::fn_803C2048(CharaTask* chara) {
    gs::CameraBox* box = fn_803C2090(m10C, chara);
    if (box != 0 && (unsigned int)(box->m2C - 1) <= 1) {
        return box;
    }
    return 0;
}

gs::CameraBox* GameScene::fn_803C2090(atn::SimpleArray<gs::CameraBox>& boxes, CharaTask* chara) {
    if (chara == 0) {
        return 0;
    }
    gs::CameraBox* best = 0;
    float bestSize = 1e16f;
    for (unsigned int i = 0; i < boxes.size(); i++) {
        gs::CameraBox& box = boxes[i];
        if (box.m54 == 0) {
            continue;
        }
        atn::ColliBoxShape_unk shape;
        shape.mMtx = atn::Matrix3x4::RotY_unk(box.m14);
        atn::Vector3 center = box.mPos;
        center.y += box.mSize.y;
        shape.mMtx.fn_801479E4(center);
        shape.mSize = box.mSize;
        atn::ColliSphereShape_unk sphere;
        sphere.mCenter = chara->mPos;
        sphere.mCenter.y += chara->m5B0;
        sphere.mRadius = 0.0f;
        if (fn_80051CD8(&sphere, &shape) != 0) {
            float size = box.mSize.LengthSq_unk();
            if (size < bestSize) {
                bestSize = size;
                best = &box;
            }
        }
    }
    return best;
}

// NONMATCHING: stub
void GameScene::vf14() {}

void GameScene::fn_803C3724(gs::PointBase_unk* point, atn::Csv& csv) {
    point->mId = atoi(csv.fn_8005B9CC());
    point->mPos.x = fn_800DC288(csv.fn_8005B9CC());
    point->mPos.y = fn_800DC288(csv.fn_8005B9CC());
    point->mPos.z = fn_800DC288(csv.fn_8005B9CC());
}

void GameScene::fn_803C3798(int* values, atn::Csv& csv) {
    values[0] = atoi(csv.fn_8005B9CC());
    values[1] = atoi(csv.fn_8005B9CC());
    values[2] = atoi(csv.fn_8005B9CC());
    values[3] = atoi(csv.fn_8005B9CC());
}

void GameScene::fn_803C380C(int* values, atn::Csv& csv) {
    values[0] = atoi(csv.fn_8005B9CC());
    values[1] = atoi(csv.fn_8005B9CC());
    values[2] = atoi(csv.fn_8005B9CC());
    if (values[1] < 0 || values[2] <= 0 || values[1] > values[2]) {
        values[0] = 0;
    }
}

void GameScene::fn_803C3894(const char* text, int length, int file) {
    atn::Csv csv(text, length);
    std::string s;
    int offset = file * 10000;
    int version = 0;
    unsigned int boxBase = 0;
    unsigned int sphereBase = 0;
    do {
        s = csv.fn_8005B9CC();
        if (s == "begin") {
            s = csv.fn_8005B9CC();
            if (s == "Header" && file <= 0) {
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s == "Version") {
                        version = atoi(csv.fn_8005B9CC());
                    }
                }
            } else if (s == "MainSetting") {
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s == "NpcNum" && file <= 0) {
                        m68 = atoi(csv.fn_8005B9CC());
                    } else if (s == "EnemyNum" && file <= 0) {
                        m6C = atoi(csv.fn_8005B9CC());
                    } else if (s == "PlayerStartPoint" && file <= 0) {
                        m70 = atoi(csv.fn_8005B9CC());
                    } else if (s == "NpcAiNum" && file <= 0) {
                        mD4.Resize_unk(atoi(csv.fn_8005B9CC()), 3, "NpcAi");
                        for (unsigned int i = 0; i < fn_80089ACC(&mD4); i++) {
                            if (!csv.fn_8005BBF8()) {
                                break;
                            }
                            s = csv.fn_8005B9CC();
                            if (s[0] == ';') {
                                i--;
                                continue;
                            }
                            mD4[i].m04 = atoi(csv.fn_8005B9CC());
                            mD4[i].m08 = atoi(csv.fn_8005B9CC());
                            mD4[i].m10 = fn_800DC288(csv.fn_8005B9CC());
                            mD4[i].m00 = atoi(csv.fn_8005B9CC());
                            mD4[i].m14 = atoi(csv.fn_8005B9CC());
                            mD4[i].m18 = atoi(csv.fn_8005B9CC());
                            mD4[i].m1C = atoi(csv.fn_8005B9CC());
                        }
                    } else if (s == "ConstrainModelNum") {
                        unsigned int i = 0;
                        if (mDC.GetData_unk() == 0) {
                            mDC.Resize_unk(atoi(csv.fn_8005B9CC()), 3, "SceneCosntrainModel");
                        } else {
                            i = fn_80089ACC(&mDC);
                            mDC.Grow_unk(i + atoi(csv.fn_8005B9CC()), 3, "SceneCosntrainModel");
                        }
                        for (; i < fn_80089ACC(&mDC); i++) {
                            if (!csv.fn_8005BBF8()) {
                                break;
                            }
                            s = csv.fn_8005B9CC();
                            if (s[0] == ';') {
                                i--;
                                continue;
                            }
                            mDC[i].m00 = atoi(s.c_str());
                            mDC[i].m04 = new ((MemoryTag)12, "", "", 0) atn::Model(0x100, 0x20);
                            const char* name = csv.fn_8005B9CC();
                            mDC[i].m04->fn_8008AD4C(name, 0);
                        }
                    }
                }
            } else if (s == "WarpPoint") {
                s = csv.fn_8005B9CC();
                unsigned int base = 0;
                if (m78.GetData_unk() == 0) {
                    m78.Resize_unk(atoi(s.c_str()), 3, "SceneWarpPoint");
                } else {
                    base = fn_80089ACC(&m78);
                    m78.Grow_unk(base + atoi(s.c_str()), 3, "SceenWarpPoint");
                }
                unsigned int i = base;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    m78[i].mIndex = base + atoi(s.c_str());
                    fn_803C3724(&m78[i], csv);
                    m78[i].m14 = fn_800DC288(csv.fn_8005B9CC());
                    fn_803C380C(m78[i].m18, csv);
                    m78[i].m24 = 1;
                    i++;
                }
            } else if (s == "NpcSetPoint") {
                fn_803C74E0(m80, csv, file);
            } else if (s == "EnemySetPoint") {
                fn_803C74E0(m88, csv, file);
            } else if (s == "FriendSetPoint") {
                fn_803C74E0(m90, csv, file);
            } else if (s == "MovePoint" && file <= 0) {
                s = csv.fn_8005B9CC();
                m74 = atoi(s.c_str());
                m98.Resize_unk(m74, 3, "SceneMovePoint");
                unsigned int i = 0;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    m98[i].mIndex = atoi(s.c_str());
                    fn_803C3724(&m98[i], csv);
                    m98[i].m14 = fn_800DC288(csv.fn_8005B9CC());
                    m98[i].m18 = atoi(csv.fn_8005B9CC());
                    m98[i].m1C = atoi(csv.fn_8005B9CC());
                    m98[i].m20 = atoi(csv.fn_8005B9CC());
                    int unused[4];
                    fn_803C3798(unused, csv);
                    m98[i].m2C = 1;
                    m98[i].m24 = atoi(csv.fn_8005B9CC());
                    m98[i].m28 = atoi(csv.fn_8005B9CC());
                    i++;
                }
            } else if (s == "HidePoint" && file <= 0) {
                s = csv.fn_8005B9CC();
                mA8 = atoi(s.c_str());
                mAC.Resize_unk(mA8, 3, "SceneHidePoint");
                unsigned int i = 0;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    mAC[i].mIndex = atoi(s.c_str());
                    fn_803C3724(&mAC[i], csv);
                    mAC[i].m14 = atoi(csv.fn_8005B9CC());
                    mAC[i].m18 = atoi(csv.fn_8005B9CC());
                    mAC[i].m1C = atoi(csv.fn_8005B9CC());
                    fn_803C3798(mAC[i].m28, csv);
                    mAC[i].m38 = 1;
                    mAC[i].m20 = atoi(csv.fn_8005B9CC());
                    mAC[i].m24 = atoi(csv.fn_8005B9CC());
                    mAC[i].m3C = 0;
                    i++;
                }
            } else if (s == "InteractionPoint" && file <= 0) {
                mC4.Resize_unk(atoi(csv.fn_8005B9CC()), 3, "SceneInteractPoint");
                unsigned int i = 0;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    mC4[i].mIndex = atoi(s.c_str());
                    fn_803C3724(&mC4[i], csv);
                    mC4[i].m14 = atoi(csv.fn_8005B9CC());
                    fn_803C3798(mC4[i].m1C, csv);
                    mC4[i].m18 = atoi(csv.fn_8005B9CC());
                    mC4[i].m2C = 1;
                    i++;
                }
            } else if (s == "AttentionPoint") {
                unsigned int base = 0;
                if (mFC.GetData_unk() == 0) {
                    mFC.Resize_unk(atoi(csv.fn_8005B9CC()), 3, "SceneAttentionPoint");
                } else {
                    base = fn_80089ACC(&mFC);
                    mFC.Grow_unk(base + atoi(csv.fn_8005B9CC()), 3, "SceneAttentionPoint");
                }
                unsigned int i = base;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    mFC[i].mIndex = base + atoi(s.c_str());
                    fn_803C3724(&mFC[i], csv);
                    mFC[i].m14 = fn_800DC288(csv.fn_8005B9CC());
                    mFC[i].m18 = atoi(csv.fn_8005B9CC());
                    fn_803C3798(mFC[i].m1C, csv);
                    mFC[i].m3C = atoi(csv.fn_8005B9CC());
                    fn_803C380C(mFC[i].m2C, csv);
                    mFC[i].m38 = 1;
                    i++;
                }
            } else if (s == "EventBox") {
                boxBase = 0;
                if (mE4.GetData_unk() == 0) {
                    mE4.Resize_unk(atoi(csv.fn_8005B9CC()), 3, "SceneEventBox");
                } else {
                    boxBase = fn_80089ACC(&mE4);
                    mE4.Grow_unk(boxBase + atoi(csv.fn_8005B9CC()), 3, "SceneEventBox");
                }
                unsigned int i = boxBase;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    gs::EventBox& box = mE4[i];
                    box.mIndex = boxBase + atoi(s.c_str());
                    fn_803C3724(&box, csv);
                    box.m14 = fn_800DC288(csv.fn_8005B9CC());
                    csv.fn_8005B9CC();
                    box.mSize.x = fn_800DC288(csv.fn_8005B9CC());
                    box.mSize.y = fn_800DC288(csv.fn_8005B9CC());
                    box.mSize.z = fn_800DC288(csv.fn_8005B9CC());
                    std::string scripts(csv.fn_8005B9CC());
                    IntVector_unk ids;
                    Tokenizer tok(scripts, boost::char_separator<char>(":"));
                    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
                        ids.push_back(atoi(it->c_str()));
                    }
                    box.m24.Resize_unk(fn_800E0AA8(&ids), 3, "SceneScript");
                    for (unsigned int j = 0; j < fn_800E0AA8(&ids); j++) {
                        if (file > 0) {
                            *fn_8028B74C(&ids, j) += offset;
                        }
                        box.m24[j] = *fn_8028B74C(&ids, j);
                    }
                    box.m2C = atoi(csv.fn_8005B9CC());
                    box.m30 = atoi(csv.fn_8005B9CC()) == 1;
                    box.m38 = atoi(csv.fn_8005B9CC());
                    box.m3C = atoi(csv.fn_8005B9CC());
                    box.m34 = atoi(csv.fn_8005B9CC());
                    box.m40 = atoi(csv.fn_8005B9CC());
                    box.m44 = atoi(csv.fn_8005B9CC());
                    i++;
                }
            } else if (s == "EventSphere") {
                sphereBase = 0;
                if (mEC.GetData_unk() == 0) {
                    mEC.Resize_unk(atoi(csv.fn_8005B9CC()), 3, "SceneEventSphere");
                } else {
                    sphereBase = fn_80089ACC(&mEC);
                    mEC.Grow_unk(sphereBase + atoi(csv.fn_8005B9CC()), 3, "SceneEventSphere");
                }
                unsigned int i = sphereBase;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    gs::EventSphere& sphere = mEC[i];
                    sphere.mIndex = sphereBase + atoi(s.c_str());
                    fn_803C3724(&sphere, csv);
                    sphere.m14 = fn_800DC288(csv.fn_8005B9CC());
                    csv.fn_8005B9CC();
                    std::string scripts(csv.fn_8005B9CC());
                    IntVector_unk ids;
                    Tokenizer tok(scripts, boost::char_separator<char>(":"));
                    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
                        ids.push_back(atoi(it->c_str()));
                    }
                    sphere.m18.Resize_unk(fn_800E0AA8(&ids), 3, "SceneScript");
                    for (unsigned int j = 0; j < fn_800E0AA8(&ids); j++) {
                        if (file > 0) {
                            *fn_8028B74C(&ids, j) += offset;
                        }
                        sphere.m18[j] = *fn_8028B74C(&ids, j);
                    }
                    sphere.m20 = atoi(csv.fn_8005B9CC());
                    csv.fn_8005B9CC();
                    csv.fn_8005B9CC();
                    sphere.m24 = atoi(csv.fn_8005B9CC());
                    i++;
                }
            } else if (s == "AiArea" && file <= 0) {
                mF4.Resize_unk(atoi(csv.fn_8005B9CC()), 3, "SceneAiArea");
                unsigned int i = 0;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    mF4[i].mIndex = atoi(s.c_str());
                    fn_803C3724(&mF4[i], csv);
                    mF4[i].m14 = fn_800DC288(csv.fn_8005B9CC());
                    csv.fn_8005B9CC();
                    mF4[i].m18 = fn_800DC288(csv.fn_8005B9CC());
                    mF4[i].m1C = fn_800DC288(csv.fn_8005B9CC());
                    mF4[i].m20 = fn_800DC288(csv.fn_8005B9CC());
                    mF4[i].m24 = atoi(csv.fn_8005B9CC());
                    mF4[i].m28 = atoi(csv.fn_8005B9CC());
                    mF4[i].m2C = atoi(csv.fn_8005B9CC()) == 1;
                    i++;
                }
            } else if (s == "MovePointGroup") {
                fn_803C6FE4(m158, m98, csv);
            } else if (s == "HidePointGroup") {
                fn_803C6FE4(m160, mAC, csv);
            } else if (s == "WarpPointGroup") {
                fn_803C6FE4(m168, m78, csv);
            } else if (s == "NpcGroup") {
            } else if (s == "EnemyGroup") {
                fn_803C606C(m178, m88, csv);
            } else if (s == "FriendGroup") {
                fn_803C606C(m170, m90, csv);
            } else if (s == "Gimmick") {
                s = csv.fn_8005B9CC();
                unsigned int base = 0;
                if (m104.GetData_unk() == 0) {
                    m104.Resize_unk(atoi(s.c_str()), 3, "SceneGimmick");
                } else {
                    base = fn_80089ACC(&m104);
                    m104.Grow_unk(base + atoi(s.c_str()), 3, "SceneGimmick");
                }
                unsigned int i = base;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    m104[i].mIndex = base + atoi(s.c_str());
                    fn_803C3724(&m104[i], csv);
                    m104[i].m14 = fn_800DC288(csv.fn_8005B9CC());
                    csv.fn_8005B9CC();
                    m104[i].m18 = atoi(csv.fn_8005B9CC());
                    m104[i].m1C = atoi(csv.fn_8005B9CC());
                    for (int k = 0; k < 8; k++) {
                        m104[i].m20[k] = atoi(csv.fn_8005B9CC());
                    }
                    for (int k = 0; k < 8; k++) {
                        m104[i].m40[k] = fn_800DC288(csv.fn_8005B9CC());
                    }
                    fn_803C3798(m104[i].m60, csv);
                    fn_803C380C(m104[i].m70, csv);
                    m104[i].m7C = 1;
                    i++;
                }
            } else if (s == "CameraBox") {
                s = csv.fn_8005B9CC();
                unsigned int base = 0;
                if (m10C.GetData_unk() == 0) {
                    m10C.Resize_unk(atoi(s.c_str()), 3, "SceneCameraBox");
                } else {
                    base = fn_80089ACC(&m10C);
                    m10C.Grow_unk(base + atoi(s.c_str()), 3, "SceneCameraBox");
                }
                unsigned int i = base;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    m10C[i].mIndex = base + atoi(s.c_str());
                    fn_803C3724(&m10C[i], csv);
                    m10C[i].m14 = fn_800DC288(csv.fn_8005B9CC());
                    csv.fn_8005B9CC();
                    m10C[i].mSize.x = fn_800DC288(csv.fn_8005B9CC());
                    m10C[i].mSize.y = fn_800DC288(csv.fn_8005B9CC());
                    m10C[i].mSize.z = fn_800DC288(csv.fn_8005B9CC());
                    fn_803C3798(m10C[i].m38, csv);
                    m10C[i].m24 = atoi(csv.fn_8005B9CC());
                    m10C[i].m28 = atoi(csv.fn_8005B9CC());
                    m10C[i].m2C = atoi(csv.fn_8005B9CC());
                    m10C[i].m30 = atoi(csv.fn_8005B9CC());
                    m10C[i].m34 = atoi(csv.fn_8005B9CC());
                    fn_803C380C(m10C[i].m48, csv);
                    m10C[i].m54 = 1;
                    i++;
                }
            } else if (s == "BgmBox") {
                s = csv.fn_8005B9CC();
                unsigned int base = 0;
                if (m114.GetData_unk() == 0) {
                    m114.Resize_unk(atoi(s.c_str()), 3, "SceneBgmBox");
                } else {
                    base = fn_80089ACC(&m114);
                    m114.Grow_unk(base + atoi(s.c_str()), 3, "SceneBgmBox");
                }
                unsigned int i = base;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    m114[i].mIndex = base + atoi(s.c_str());
                    fn_803C3724(&m114[i], csv);
                    m114[i].m14 = fn_800DC288(csv.fn_8005B9CC());
                    csv.fn_8005B9CC();
                    m114[i].mSize.x = fn_800DC288(csv.fn_8005B9CC());
                    m114[i].mSize.y = fn_800DC288(csv.fn_8005B9CC());
                    m114[i].mSize.z = fn_800DC288(csv.fn_8005B9CC());
                    fn_803C3798(m114[i].m30, csv);
                    m114[i].mBgm = atoi(csv.fn_8005B9CC());
                    m114[i].m28 = fn_800DC288(csv.fn_8005B9CC());
                    m114[i].m2C = atoi(csv.fn_8005B9CC());
                    fn_803C380C(m114[i].m40, csv);
                    m114[i].m4C = 1;
                    i++;
                }
            } else if (s == "MovePointConnection" && file <= 0) {
                gs::Table2D_unk<unsigned short> table;
                table.Alloc(m74, m74);
                unsigned int row = 0;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    for (int k = 0; k < m74; k++) {
                        table[row][k] = atoi(csv.fn_8005B9CC());
                    }
                    row++;
                }
                atn::MemoryManager::Instance()->SetMode_unk(1, 0x20);
                mA0.Resize_unk(m74, 3, "ScenePointConnect");
                for (unsigned int j = 0; j < fn_80089ACC(&mA0); j++) {
                    fn_803CCF64(&mA0[j], table[j], fn_80089ACC(&mA0));
                }
                atn::MemoryManager::Instance()->ResetMode_unk();
            } else if (s == "MovePointConnectionPack" && file <= 0) {
                mA0.Resize_unk(m74, 3, "ScenePointConnect");
                unsigned int j = 0;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    fn_803CD598(&mA0[j], &csv, m74);
                    j++;
                }
            } else if (s == "HideVsHideConnection" && file <= 0) {
                if (version >= 7) {
                    std::vector<gs::HideLink_unk> links;
                    std::vector<gs::HideLink2_unk> links2;
                    while (csv.fn_8005BBF8()) {
                        s = csv.fn_8005B9CC();
                        if (s == "end") {
                            break;
                        }
                        if (s[0] == ';') {
                            continue;
                        }
                        atoi(csv.GetField_unk());
                        int from = atoi(csv.fn_8005B9CC());
                        int to = atoi(csv.fn_8005B9CC());
                        int flags = atoi(csv.fn_8005B9CC());
                        if ((flags & 1) == 1) {
                            gs::HideLink_unk link;
                            link.mFrom = from;
                            link.mTo = to;
                            link.m08 = 1;
                            link.mMid = (mAC[from].mPos + mAC[to].mPos) * 0.5f;
                            links.push_back(link);
                        }
                        if ((flags & 4) == 4) {
                            gs::HideLink2_unk link2;
                            link2.mFrom = from;
                            link2.mTo = to;
                            link2.m08 = 1;
                            links2.push_back(link2);
                        }
                    }
                    mB4.Resize_unk(links.size(), 3, "ScenePointConnect");
                    for (unsigned int j = 0; j < links.size(); j++) {
                        mB4[j] = links[j];
                    }
                    mBC.Resize_unk(links2.size(), 3, "ScenePointConnect");
                    for (unsigned int j = 0; j < links2.size(); j++) {
                        mBC[j] = links2[j];
                    }
                } else {
                    s = csv.fn_8005B9CC();
                    gs::Table2D_unk<unsigned char> table;
                    table.Alloc(mA8, mA8);
                    unsigned int row = 0;
                    while (csv.fn_8005BBF8()) {
                        s = csv.fn_8005B9CC();
                        if (s == "end") {
                            break;
                        }
                        if (s[0] == ';') {
                            continue;
                        }
                        for (int k = 0; k < mA8; k++) {
                            table[row][k] = atoi(csv.fn_8005B9CC());
                        }
                        row++;
                    }
                    std::vector<gs::HideLink_unk> links;
                    std::vector<gs::HideLink2_unk> links2;
                    for (unsigned int a = 0; a < fn_80089ACC(&mAC); a++) {
                        for (unsigned int b = 0; b < fn_80089ACC(&mAC); b++) {
                            if ((table[a][b] & 1) == 1) {
                                gs::HideLink_unk link;
                                link.mFrom = a;
                                link.mTo = b;
                                link.m08 = 1;
                                link.mMid = (mAC[a].mPos + mAC[b].mPos) * 0.5f;
                                links.push_back(link);
                            }
                            if ((table[a][b] & 4) == 4) {
                                gs::HideLink2_unk link2;
                                link2.mFrom = a;
                                link2.mTo = b;
                                link2.m08 = 1;
                                links2.push_back(link2);
                            }
                        }
                    }
                    mB4.Resize_unk(links.size(), 3, "ScenePointConnect");
                    for (unsigned int j = 0; j < links.size(); j++) {
                        mB4[j] = links[j];
                    }
                    mBC.Resize_unk(links2.size(), 3, "ScenePointConnect");
                    for (unsigned int j = 0; j < links2.size(); j++) {
                        mBC[j] = links2[j];
                    }
                }
            } else if (s == "InteractionConnection" && file <= 0) {
                mCC.Resize_unk(atoi(csv.fn_8005B9CC()), 3, "ScenePointConnect");
                unsigned int j = 0;
                while (csv.fn_8005BBF8()) {
                    s = csv.fn_8005B9CC();
                    if (s == "end") {
                        break;
                    }
                    if (s[0] == ';') {
                        continue;
                    }
                    mCC[j].m00 = atoi(s.c_str());
                    mCC[j].m04 = atoi(csv.fn_8005B9CC());
                    j++;
                }
            } else if (s == "EventIdList") {
                int count = atoi(csv.fn_8005B9CC());
                int i = 0;
                if (m180.GetData_unk() == 0) {
                    m180.Resize_unk(count, 3, "SceneEventIdList");
                } else {
                    i = fn_80089ACC(&m180);
                    m180.Grow_unk(i + count, 3, "SceneEventIdList");
                }
                if (csv.fn_8005BBF8()) {
                    for (; i < (int)fn_80089ACC(&m180); i++) {
                        m180[i] = abs(atoi(csv.fn_8005B9CC()));
                    }
                }
            }
        }
    } while (csv.fn_8005BBF8());
    for (unsigned int i = boxBase; i < fn_80089ACC(&mE4); i++) {
        if (mE4[i].m2C > 0) {
            mE4[i].m2C = gs::FindIndex_unk(mE4, mE4[i].m2C);
        } else {
            mE4[i].m2C = -1;
        }
    }
    for (unsigned int i = sphereBase; i < fn_80089ACC(&mEC); i++) {
        if (mEC[i].m20 > 0) {
            mEC[i].m20 = gs::FindIndex_unk(mEC, mEC[i].m20);
        } else {
            mEC[i].m20 = -1;
        }
    }
}

template <class G, class T>
void GameScene::fn_803C606C(atn::SimpleArray<G>& groups, atn::SimpleArray<T>& points, atn::Csv& csv) {
    unsigned int i = 0;
    if (groups.GetData_unk() == 0) {
        groups.Resize_unk(atoi(csv.fn_8005B9CC()), 1, "SceneCharaGroup");
    } else {
        i = fn_80089ACC(&groups);
        groups.Grow_unk(i + atoi(csv.fn_8005B9CC()), 1, "SceneCharaGroup");
    }
    while (csv.fn_8005BBF8()) {
        std::string s(csv.fn_8005B9CC());
        if (s == "end") {
            break;
        }
        if (s[0] == ';') {
            continue;
        }
        fn_803C3724(&groups[i], csv);
        int n = 0;
        memset(groups[i].mPoints, 0, sizeof(groups[i].mPoints));
        std::string list(csv.fn_8005B9CC());
        Tokenizer tok(list, boost::char_separator<char>(":"));
        for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
            T* point = FindPoint_unk(points, atoi(it->c_str()));
            if (point != 0) {
                groups[i].mPoints[n++] = point;
                if (n >= 32) {
                    break;
                }
            }
        }
        groups[i].m90 = atoi(csv.fn_8005B9CC());
        groups[i].m94 = FindPoint_unk(points, groups[i].m90);
        groups[i].m98 = atoi(csv.fn_8005B9CC());
        csv.fn_8005B9CC();
        groups[i].m9C = atoi(csv.fn_8005B9CC());
        i++;
    }
}

template <class T>
T* GameScene::FindPoint_unk(atn::SimpleArray<T>& points, int id) {
    for (unsigned int i = 0; i < (unsigned int)points.mSize; i++) {
        if (id == points.mData[i].mId) {
            return &points.mData[i];
        }
    }
    return 0;
}

template <class G, class T>
void GameScene::fn_803C6FE4(atn::SimpleArray<G>& groups, atn::SimpleArray<T>& points, atn::Csv& csv) {
    unsigned int i = 0;
    if (groups.GetData_unk() == 0) {
        groups.Resize_unk(atoi(csv.fn_8005B9CC()), 1, "ScenePointGroup");
    } else {
        i = fn_80089ACC(&groups);
        groups.Grow_unk(i + atoi(csv.fn_8005B9CC()), 1, "ScenePointGroup");
    }
    while (csv.fn_8005BBF8()) {
        std::string s(csv.fn_8005B9CC());
        if (s == "end") {
            break;
        }
        if (s[0] == ';') {
            continue;
        }
        fn_803C3724(&groups[i], csv);
        int n = 0;
        memset(groups[i].mPoints, 0, sizeof(groups[i].mPoints));
        std::string list(csv.fn_8005B9CC());
        Tokenizer tok(list, boost::char_separator<char>(":"));
        for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
            T* point = FindPoint_unk(points, atoi(it->c_str()));
            if (point != 0) {
                groups[i].mPoints[n++] = point;
                if (n >= 32) {
                    break;
                }
            }
        }
        groups[i].m90 = FindPoint_unk(points, atoi(csv.fn_8005B9CC()));
        i++;
    }
}

template <class T>
void GameScene::fn_803C74E0(atn::SimpleArray<T>& points, atn::Csv& csv, int file) {
    unsigned int base = 0;
    if (points.GetData_unk() == 0) {
        points.Resize_unk(atoi(csv.fn_8005B9CC()), 1, "SceneCharaPoint");
    } else {
        base = fn_80089ACC(&points);
        points.Grow_unk(base + atoi(csv.fn_8005B9CC()), 1, "SceneCharaPoint");
    }
    int offset = file * 10000;
    unsigned int i = base;
    while (csv.fn_8005BBF8()) {
        std::string s(csv.fn_8005B9CC());
        if (s == "end") {
            break;
        }
        if (s[0] == ';') {
            continue;
        }
        points[i].mIndex = base + atoi(s.c_str());
        fn_803C3724(&points[i], csv);
        points[i].m14 = fn_800DC288(csv.fn_8005B9CC());
        points[i].m18 = atoi(csv.fn_8005B9CC());
        points[i].m1C = atoi(csv.fn_8005B9CC());
        points[i].m20 = atoi(csv.fn_8005B9CC());
        std::string scripts(csv.fn_8005B9CC());
        IntVector_unk ids;
        Tokenizer tok(scripts, boost::char_separator<char>(":"));
        for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
            ids.push_back(atoi(it->c_str()));
        }
        points[i].m140.Resize_unk(fn_800E0AA8(&ids), 3, "SceneScript");
        for (unsigned int j = 0; j < fn_800E0AA8(&ids); j++) {
            if (file > 0) {
                *fn_8028B74C(&ids, j) += offset;
            }
            points[i].m140[j] = *fn_8028B74C(&ids, j);
        }
        csv.fn_8005B9CC();
        points[i].m24 = atoi(csv.fn_8005B9CC());
        points[i].m28 = atoi(csv.fn_8005B9CC());
        strncpy(points[i].mName, csv.fn_8005B9CC(), 0xFF);
        points[i].mName[0xFF] = 0;
        points[i].m12C = atoi(csv.fn_8005B9CC());
        for (int k = 0; k < 3; k++) {
            points[i].m130[k] = atoi(csv.fn_8005B9CC());
        }
        points[i].m13C = atoi(csv.fn_8005B9CC());
        i++;
    }
}


void GameScene::fn_803CA0F4() {
    int n = 0;
    for (unsigned int i = 0; i < m170.size(); i++) {
        if (IsShown_unk(m170[i].m9C)) {
            n++;
        }
    }
    m148.Resize_unk(n, 3);
    int k = 0;
    for (unsigned int i = 0; i < m170.size(); i++) {
        gs::CharaGroup<gs::NpcSetPoint>& g = m170[i];
        if (!IsShown_unk(g.m9C)) {
            continue;
        }
        gs::SceneGroup_unk& e = m148.mData[k];
        e.mType = 3;
        e.mId = g.mId;
        memset(e.mCharas, 0, sizeof(e.mCharas));
        unsigned int j = 0;
        for (; g.mPoints[j] != 0; j++) {
            e.mCharas[j] = lbl_8087F8A0->Find_unk(g.mPoints[j]->mId);
        }
        if (g.m94 != 0) {
            e.mLeader = lbl_8087F8A0->Find_unk(g.m94->mId);
        } else if (g.m90 == -1) {
            CharaTask* leader = lbl_8087F8A0->mHead;
            if (j < 32) {
                e.mCharas[j] = leader;
                e.mLeader = leader;
            }
        } else {
            e.mLeader = 0;
        }
        e.m08 = g.m98;
        k++;
    }
    if (m148.mSize == 0) {
        m148.Resize_unk(1, 3);
        m148.mData[0].mType = 3;
        m148.mData[0].mId = 1;
        memset(m148.mData[0].mCharas, 0, sizeof(m148.mData[0].mCharas));
        CharaTask* leader = lbl_8087F8A0->mHead;
        m148.mData[0].mCharas[0] = leader;
        m148.mData[0].mLeader = leader;
    }
    n = 0;
    for (unsigned int i = 0; i < m178.size(); i++) {
        if (IsShown_unk(m178[i].m9C)) {
            n++;
        }
    }
    m150.Resize_unk(n, 3);
    k = 0;
    for (unsigned int i = 0; i < m178.size(); i++) {
        gs::CharaGroup<gs::NpcSetPoint>& g = m178[i];
        if (!IsShown_unk(g.m9C)) {
            continue;
        }
        gs::SceneGroup_unk& e = m150.mData[k];
        e.mType = 2;
        e.mId = g.mId;
        memset(e.mCharas, 0, sizeof(e.mCharas));
        for (unsigned int j = 0; g.mPoints[j] != 0; j++) {
            e.mCharas[j] = EnemyManager::sInstance->Find_unk(g.mPoints[j]->mId);
        }
        if (g.m94 != 0) {
            e.mLeader = EnemyManager::sInstance->Find_unk(g.m94->mId);
        } else {
            e.mLeader = 0;
        }
        e.m08 = g.m98;
        k++;
    }
}

void GameScene::fn_803CA530() {
    if (m64 != 0) {
        for (unsigned int i = 0; i < m104.size(); i++) {
            fn_803EC374(this, &m104[i]);
        }
        if (((MapData_unk*)m64)->m48 == 1 && lbl_8087F430 != 0) {
            if (((MapData_unk*)m64)->m4C != 3) {
                fn_803EC16C(this, 60000, 1);
            }
            fn_803EC16C(this, 61000, 1);
            fn_803EC16C(this, 64000, 1);
        }
    }
}

void GameScene::fn_803CA604() {
    int reset = 0;
    if (lbl_8087F430 != 0) {
        if (lbl_8087F430->m5694 != 0) {
            reset = 1;
            int maps = fn_80370174(lbl_8087F430, 0x392);
            int map = ((MapData_unk*)m64)->m50;
            for (int i = 0; i < 8; i++) {
                if (map == (maps & 0xF)) {
                    reset = 0;
                    break;
                }
                maps >>= 4;
            }
            if (reset) {
                fn_80370320(lbl_8087F430, 0x392, (fn_80370174(lbl_8087F430, 0x392) << 4) | (map & 0xF), 1);
            }
        } else {
            fn_80370320(lbl_8087F430, 0x392, 0, 1);
        }
    }
    int n = 0;
    for (unsigned int i = 0; i < mE4.size(); i++) {
        if (mE4[i].m2C <= 0 && mE4[i].m30 != 0) {
            n++;
        }
    }
    m188.Resize_unk(n, 3);
    int k = 0;
    for (unsigned int i = 0; i < mE4.size(); i++) {
        gs::EventBox& box = mE4[i];
        if (box.m2C > 0 || box.m30 == 0) {
            continue;
        }
        int id = box.mId;
        int type = 21000;
        gmk::TreasureBox::Data_unk* data = fn_8021ECD0(box.m34);
        if (data != 0) {
            type = data->mType + 21000;
        }
        GimmickCommon* gimmick = fn_803EC16C(this, type, id);
        if (gimmick == 0) {
            continue;
        }
        atn::Vector3 pos(0.0f, 0.0f, 2.8f + -box.mSize.z);
        Mtx m;
        PSMTXRotRad(m, 'y', box.m14);
        PSMTXMultVec(m, &pos, &pos);
        pos += box.mPos;
        atn::Vector3 rot(0.0f, box.m14, 0.0f);
        atn::Vector3 scale(1.0f, 1.0f, 1.0f);
        gimmick->m6C = pos;
        gimmick->m78 = rot;
        gimmick->m90 = scale;
        gimmick->vf68(0, box.m34);
        m188.mData[k++] = gimmick;
        atn::Vector3 size(4.5f, 6.0f, 8.0f);
        atn::Vector3 center(0.0f, 0.0f, 1.0f + (size.z - 2.8f));
        Mtx m2;
        PSMTXRotRad(m2, 'y', box.m14);
        PSMTXMultVec(m2, &center, &center);
        center += pos;
        box.mPos = center;
        box.mSize = size;
        box.m40 = 0;
        if (type % 1000 == 1 && reset && data != 0 && lbl_8087F430 != 0) {
            fn_80370320(lbl_8087F430, data->mFlag, 0, 1);
        }
    }
}

void GameScene::fn_803CAABC() {
    if (lbl_8087F430 == 0) {
        return;
    }
    for (unsigned int i = 0; i < mAC.size(); i++) {
        gs::HidePoint& p = mAC[i];
        p.m38 = CheckFlag_unk(p.m28);
    }
    for (unsigned int i = 0; i < mB4.size(); i++) {
        gs::HideLink_unk& link = mB4[i];
        if (mAC[link.mFrom].m38 != 0 && mAC[link.mTo].m38 != 0) {
            link.m08 = 1;
        } else {
            link.m08 = 0;
        }
    }
    for (unsigned int i = 0; i < mBC.size(); i++) {
        gs::HideLink2_unk& link = mBC[i];
        if (mAC[link.mFrom].m38 != 0 && mAC[link.mTo].m38 != 0) {
            link.m08 = 1;
        } else {
            link.m08 = 0;
        }
    }
    for (unsigned int i = 0; i < m78.size(); i++) {
        gs::WarpPoint& p = m78[i];
        p.m24 = CheckTime_unk(p.m18);
    }
    for (unsigned int i = 0; i < mC4.size(); i++) {
        gs::InteractionPoint& p = mC4[i];
        p.m2C = CheckFlag_unk(p.m1C);
    }
    for (unsigned int i = 0; i < mFC.size(); i++) {
        gs::AttentionPoint& p = mFC[i];
        p.m38 = CheckFlag_unk(p.m1C) && CheckTime_unk(p.m2C);
    }
    for (unsigned int i = 0; i < m104.size(); i++) {
        gs::Gimmick& p = m104[i];
        p.m7C = CheckFlag_unk(p.m60) && CheckTime_unk(p.m70);
    }
}

void GameScene::fn_803CAED0(GameSceneRoute_unk* out, gs::HideLink_unk* link) {
    gs::HidePoint& a = mAC[link->mFrom];
    gs::HidePoint& b = mAC[link->mTo];
    out->mDir = (b.mPos - a.mPos).GetNormalize();
    atn::Vector3 side;
    atn::Vector3 up(0.0f, 1.0f, 0.0f);
    PSVECCrossProduct(&out->mDir, &up, &side);
    out->mSide = side;
    out->mFrom = a.mPos;
    out->mTo = b.mPos;
    int type = a.m14;
    out->mType = type;
    out->mFlags = 0;
    out->mFlags |= (a.m18 != 0);
    out->mFlags |= (b.m18 != 0) ? 2 : 0;
    if ((unsigned int)(type - 1) <= 1) {
        out->mFlags |= 4;
    }
    out->mFromIndex = link->mFrom + 1;
    out->mToIndex = link->mTo + 1;
}

// inferred: segment passed to fn_80050A1C
struct Segment_unk {
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};
// inferred: squared distance from a point to a segment; the closest point goes to `out`
extern "C" float fn_80050A1C(const atn::Vector3* point, const Segment_unk* seg, atn::Vector3* out);

int GameScene::fn_803CB044(GameSceneRoute_unk* out, CharaTask* chara, int check, int point) {
    atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
    atn::ColliResult_unk unused;
    Mtx m;
    PSMTXRotRad(m, 'y', chara->mRot.y);
    PSMTXMultVec(m, &fwd, &fwd);
    float reach = 4.0f * chara->m570;
    float best = 1e16f;
    gs::HideLink_unk* found = 0;
    point -= 1;
    for (unsigned int i = 0; i < mB4.size(); i++) {
        gs::HideLink_unk& link = mB4[i];
        gs::HidePoint& a = mAC[link.mFrom];
        gs::HidePoint& b = mAC[link.mTo];
        if (link.m08 == 0) {
            continue;
        }
        if (point >= 0 && link.mFrom != point && link.mTo != point) {
            continue;
        }
        atn::Vector3 d(link.mMid.x - chara->m614.x, link.mMid.y - chara->m614.y, link.mMid.z - chara->m614.z);
        if (d.LengthSq_unk() > 10000.0f) {
            continue;
        }
        Segment_unk seg;
        seg.mStart = a.mPos;
        seg.mEnd = b.mPos;
        float h = chara->m5A4.y + chara->m620;
        seg.mStart.y += h;
        seg.mEnd.y += h;
        atn::Vector3 dir(seg.mEnd.x - seg.mStart.x, seg.mEnd.y - seg.mStart.y, seg.mEnd.z - seg.mStart.z);
        dir.Normalize_unk();
        atn::Vector3 side;
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        PSVECCrossProduct(&dir, &up, &side);
        float dot = PSVECDotProduct(&side, &fwd);
        if (check != 0 && dot < 0.5f) {
            continue;
        }
        atn::ColliSphereShape_unk sphere;
        sphere.mCenter = chara->m614;
        sphere.mRadius = chara->m620;
        sphere.mRadius = dot * reach + (sphere.mRadius > 16.0f ? sphere.mRadius : 16.0f);
        atn::Vector3 closest;
        float dist = fn_80050A1C(&sphere.mCenter, &seg, &closest);
        if (dist < sphere.mRadius * sphere.mRadius && dist < best) {
            atn::Vector3 off(closest.x - chara->m614.x, 0.0f, closest.z - chara->m614.z);
            atn::Vector3 n;
            if (off.LengthSq_unk() < 4.0f || (n = off, n.Normalize_unk(), PSVECDotProduct(&n, &fwd) > 0.5f)) {
                int blocked = 0;
                if (point < 0) {
                    atn::ColliResult_unk result;
                    if (fn_8004ED34(lbl_8087EE98, &result, &sphere.mCenter, &closest,
                                    chara->fn_80179D44() | 0x80000000, 0, 0) != 0) {
                        blocked = 1;
                    }
                    if ((float)__fabs(closest.y - chara->mPos.y) > 8.0f) {
                        blocked = 1;
                    }
                }
                if (!blocked) {
                    best = dist;
                    found = &link;
                }
            }
        }
    }
    if (found != 0) {
        fn_803CAED0(out, found);
        return 1;
    }
    return 0;
}

int GameScene::fn_803CB474(GameSceneRoute_unk* out, CharaTask* chara, int point, int exclude) {
    for (unsigned int i = 0; i < mB4.size(); i++) {
        gs::HideLink_unk& link = mB4[i];
        if (link.m08 == 0) {
            continue;
        }
        if (link.mFrom != point && link.mTo != point) {
            continue;
        }
        if (link.mFrom == exclude || link.mTo == exclude) {
            continue;
        }
        gs::HidePoint& a = mAC[link.mFrom];
        gs::HidePoint& b = mAC[link.mTo];
        atn::Vector3 dir(b.mPos.x - a.mPos.x, b.mPos.y - a.mPos.y, b.mPos.z - a.mPos.z);
        dir.Normalize_unk();
        atn::Vector3 side;
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        PSVECCrossProduct(&dir, &up, &side);
        atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', chara->mRot.y);
        PSMTXMultVec(m, &fwd, &fwd);
        if (PSVECDotProduct(&side, &fwd) < 0.70710677f) {
            continue;
        }
        fn_803CAED0(out, &link);
        return 1;
    }
    return 0;
}

int GameScene::fn_803CB618(GameSceneRoute_unk* out, CharaTask* chara) {
    int point = chara->mC38 - 1;
    if (chara->fn_80153B44() == 2) {
        point = chara->mC3C - 1;
    }
    for (unsigned int i = 0; i < mBC.size(); i++) {
        gs::HideLink2_unk& link = mBC[i];
        if (link.m08 == 0 || point != link.mFrom) {
            continue;
        }
        int to = mBC[i].mTo;
        gs::HidePoint& a = mAC[point];
        gs::HidePoint& b = mAC[to];
        atn::Vector3 dir(b.mPos.x - chara->mPos.x, b.mPos.y - chara->mPos.y, b.mPos.z - chara->mPos.z);
        atn::Vector3 link2(b.mPos.x - a.mPos.x, b.mPos.y - a.mPos.y, b.mPos.z - a.mPos.z);
        out->mDir = dir;
        atn::Vector3 side;
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        PSVECCrossProduct(&link2, &up, &side);
        out->mSide = side;
        out->mFrom = mAC[to].mPos;
        out->mTo = mAC[to].mPos;
        out->mType = mAC[to].m14;
        out->mFlags = 0;
        out->mFlags |= (mAC[to].m18 != 0);
        out->mFlags |= (mAC[to].m18 != 0) ? 2 : 0;
        out->mFromIndex = to + 1;
        return 1;
    }
    return 0;
}

// NONMATCHING: stub
void GameScene::fn_803CB820() {}

int GameScene::fn_803CC198(const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* chara) {
    return fn_803CC1A4(pos, ((CharaRadius_unk*)chara)->m620, 0.5f, dir);
}

// inferred: a x b
inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

int GameScene::fn_803CC1A4(const atn::Vector3* pos, float radius, float cosMin, const atn::Vector3* dir) {
    for (unsigned int i = 0; i < mB4.size(); i++) {
        gs::HideLink_unk& link = mB4[i];
        if (link.m08 == 0) {
            continue;
        }
        if (mAC[link.mFrom].m14 != 1) {
            continue;
        }
        if (atn::Vector3(link.mMid.x - pos->x, link.mMid.y - pos->y, link.mMid.z - pos->z).LengthSq_unk() > 10000.0f) {
            continue;
        }
        Segment_unk seg;
        seg.mStart = mAC[link.mFrom].mPos;
        seg.mEnd = mAC[link.mTo].mPos;
        atn::Vector3 side(seg.mEnd.x - seg.mStart.x, seg.mEnd.y - seg.mStart.y, seg.mEnd.z - seg.mStart.z);
        side.Normalize_unk();
        side = Cross_unk(side, atn::Vector3(0.0f, 1.0f, 0.0f));
        if (PSVECDotProduct(&side, dir) < cosMin) {
            continue;
        }
        if (fn_80050A1C(pos, &seg, 0) > radius * radius) {
            continue;
        }
        return 1;
    }
    return 0;
}

int GameScene::fn_803CC3BC(const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* chara) {
    float cosMin = cos(1.0471975803375244);
    float radius = chara->m620;
    for (unsigned int i = 0; i < mB4.size(); i++) {
        gs::HideLink_unk& link = mB4[i];
        if (link.m08 == 0) {
            continue;
        }
        if (mAC[link.mFrom].m14 != 0) {
            continue;
        }
        if (atn::Vector3(link.mMid.x - pos->x, link.mMid.y - pos->y, link.mMid.z - pos->z).LengthSq_unk() > 10000.0f) {
            continue;
        }
        Segment_unk seg;
        seg.mStart = mAC[link.mFrom].mPos;
        seg.mEnd = mAC[link.mTo].mPos;
        atn::Vector3 side(seg.mEnd.x - seg.mStart.x, seg.mEnd.y - seg.mStart.y, seg.mEnd.z - seg.mStart.z);
        side.Normalize_unk();
        side = Cross_unk(side, atn::Vector3(0.0f, 1.0f, 0.0f));
        if (PSVECDotProduct(&side, dir) < cosMin) {
            continue;
        }
        if (fn_80050A1C(pos, &seg, 0) > radius * radius) {
            continue;
        }
        return 1;
    }
    return 0;
}

int GameScene::fn_803CC5E4(int type, int id) {
    if (type == 1) {
        for (int i = 0; i < m80.mSize; i++) {
            gs::NpcSetPoint& p = m80.mData[i];
            if (id == p.mId) {
                return p.m20;
            }
        }
    } else if (type == 2) {
        for (int i = 0; i < m88.mSize; i++) {
            gs::NpcSetPoint& p = m88.mData[i];
            if (id == p.mId) {
                return p.m20;
            }
        }
    } else if (type == 3) {
        for (int i = 0; i < m90.mSize; i++) {
            gs::NpcSetPoint& p = m90.mData[i];
            if (id == p.mId) {
                return p.m20;
            }
        }
    }
    return -1;
}

void* GameScene::fn_803CC6B4(int id) {
    if (id < 10000) {
        return fn_803B3B38(&m11C, id);
    }
    return fn_803B3B38(lbl_8087F430->m54F4, id);
}

gs::ConstrainModel* GameScene::fn_803CC6D0(int id) {
    for (unsigned int i = 0; i < (unsigned int)mDC.mSize; i++) {
        if (id == mDC.mData[i].m00) {
            return &mDC.mData[i];
        }
    }
    return 0;
}

void GameScene::fn_803CC718(atn::Task* chara, int value) {
    for (unsigned int i = 0; i < mDC.size(); i++) {
        gs::ConstrainModel& c = mDC[i];
        if (c.m08 == chara) {
            c.m1C = value;
        }
    }
}

atn::Vector3 GameScene::fn_803CC754(unsigned int index) {
    return m80.mData[index].mPos;
}

void GameScene::fn_803CC774(atn::Task* chara) {
    m134->fn_803ABFD4(chara);
}

int GameScene::fn_803CC77C() {
    GameEventScript* script = m134;
    int n = 0;
    if (script != 0) {
        n = script->fn_803AC088();
    }
    if (EnemyManager::sInstance != 0) {
        for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
            if (e->CanAct_unk() && (e->m54C & 0x2000) != 0x2000) {
                n += e->vfF4();
            }
        }
    }
    if (GimmickManager::instance_ != 0) {
        GimmickCommon* g = GimmickManager::instance_->fn_803EEE10(61000, 1);
        if (g != 0) {
            n += fn_80435CBC(g);
        }
        g = GimmickManager::instance_->fn_803EEE10(64000, 1);
        if (g != 0) {
            n += fn_8043A530(g);
        }
    }
    if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0xA1E) > 0) {
        n = 0;
    }
    return n;
}

int GameScene::fn_803CC900() {
    GameEventScript* script = m134;
    int n = 0;
    if (script != 0) {
        n = script->fn_803AC230();
    }
    if (EnemyManager::sInstance != 0) {
        for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
            if (e->CanAct_unk() && (e->m54C & 0x2000) != 0x2000) {
                n += e->vfF4();
            }
        }
    }
    if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0xA1E) > 0) {
        n = 0;
    }
    return n;
}

int GameScene::fn_803CCA34(CharaTask* chara) {
    if (m134 != 0) {
        return m134->fn_803AC708(chara);
    }
    return 0;
}

int GameScene::fn_803CCA4C(CharaTask* chara) {
    if (m134 != 0) {
        return m134->fn_803AC978(chara);
    }
    return 0;
}

int GameScene::fn_803CCA64(CharaTask* chara, int arg, float range) {
    if (m134 != 0) {
        return m134->fn_803AC3D8(chara, 0, arg, range);
    }
    return 0;
}

// inferred: position and rotation of the set point of `chara` in `points`
inline int GetSetPoint_unk(atn::SimpleArray<gs::NpcSetPoint>& points, CharaTask* chara, atn::Vector3* pos,
                           atn::Vector3* rot) {
    for (unsigned int i = 0; i < points.size(); i++) {
        gs::NpcSetPoint& p = points[i];
        if (p.mId == chara->m58) {
            if (pos != 0) {
                *pos = p.mPos;
            }
            if (rot != 0) {
                *rot = atn::Vector3(0.0f, p.m14, 0.0f);
            }
            return 1;
        }
    }
    return 0;
}

int GameScene::fn_803CCA84(CharaTask* chara, atn::Vector3* pos, atn::Vector3* rot) {
    if (chara == 0) {
        return 0;
    }
    int found = 0;
    switch (chara->mType) {
    case 1:
        found = GetSetPoint_unk(m80, chara, pos, rot);
        break;
    case 2:
        found = GetSetPoint_unk(m88, chara, pos, rot);
        break;
    case 3:
        found = GetSetPoint_unk(m90, chara, pos, rot);
        break;
    }
    return found;
}

int GameScene::fn_803CCC4C() {
    return m134 != 0 && m134->m8C != 0;
}

void GameScene::fn_803CCC70(int value) {
    if (m134 != 0) {
        m134->mB4 = value;
    }
}

void GameScene::fn_803CCC84(int a, int b) {
    GameScenePair_unk pair;
    pair.m00 = a;
    pair.m04 = b;
    m190.push_back(pair);
}

void GameScene::fn_803CCCC0() {
    for (unsigned int i = 0; i < m190.size(); i++) {
        GameScenePair_unk& pair = m190[i];
        CharaTask* chara = 0;
        switch (pair.m00) {
        case 0:
            chara = lbl_8087F8A0->Find_unk(pair.m04);
            break;
        case 1:
            chara = NpcManager::sInstance->Find_unk(pair.m04);
            break;
        case 2:
            chara = EnemyManager::sInstance->Find_unk(pair.m04);
            break;
        case 3:
            chara = lbl_8087F8A0->Find_unk(pair.m04);
            break;
        }
        if (chara != 0) {
            chara->fn_8016E4C4(1);
        }
    }
    m190.clear();
}

void GameScene::fn_803CCD98() {
    if (m134 != 0) {
        m134->fn_803ACD08();
    }
}

gs::BgmBox* GameScene::fn_803CCDAC(atn::SimpleArray<gs::BgmBox>& boxes, CharaTask* chara) {
    if (chara == 0) {
        return 0;
    }
    gs::BgmBox* best = 0;
    float bestSize = 1e16f;
    for (unsigned int i = 0; i < boxes.size(); i++) {
        gs::BgmBox& box = boxes[i];
        if (box.m4C == 0) {
            continue;
        }
        atn::ColliBoxShape_unk shape;
        shape.mMtx = atn::Matrix3x4::RotY_unk(box.m14);
        atn::Vector3 center = box.mPos;
        center.y += box.mSize.y;
        shape.mMtx.fn_801479E4(center);
        shape.mSize = box.mSize;
        atn::ColliSphereShape_unk sphere;
        sphere.mCenter = chara->mPos;
        sphere.mCenter.y += chara->m5B0;
        sphere.mRadius = 0.0f;
        if (fn_80051CD8(&sphere, &shape) != 0) {
            float size = box.mSize.LengthSq_unk();
            if (size < bestSize) {
                bestSize = size;
                best = &box;
            }
        }
    }
    return best;
}
