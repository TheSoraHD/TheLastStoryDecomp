#include "game/GameScene.h"
#include "game/GameEventScript.h"
#include "game/CrowdManager.h"
#include "game/ColliAttrManager.h"
#include "game/ArenaTask.h"
#include "game/SongManager.h"
#include "game/EnvSeManager.h"
#include "game/GameBgm.h"
#include "game/TownTask.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/Model_unk.h"
#include "atn/SoundCore.h"
#include "std/string.h"
#include "std/vector.h"
#include "game/IntVector_unk.h"
#include "atn/MemoryManager.h"
#include <boost/tokenizer.hpp>

typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;

extern "C" int abs(int x);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" unsigned int fn_80089ACC(const void* array); // inferred: SimpleArray size (weak copy of atn/Model.cpp)
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
extern "C" void fn_801027A4(void* bullets, int arg);
extern "C" void fn_803EC374(atn::Task* parent, gs::Gimmick* gimmick); // inferred: creates a gimmick of the scene
extern "C" void* fn_803EC16C(atn::Task* parent, int item, int index); // inferred: creates a gimmick
extern "C" void fn_8047E5DC(void* mgr, int id);
extern "C" int fn_8047F268(void* mgr, int id);
extern "C" void fn_803B3C10(GameMessageTable_unk* table, const char* path); // inferred: loads the messages
extern "C" int fn_803B3CA8(GameMessageTable_unk* table); // inferred: nonzero while loading
extern "C" void* fn_803B3B38(void* table, int id); // inferred: message `id`

// inferred: game flow manager (lbl_8087F420), only the members used here
struct GameFlow_unk {
    unsigned char m00[0x18];
    int m18;
    int m1C;
};
extern "C" GameFlow_unk* lbl_8087F420;

// inferred: game settings (lbl_8087F0A8), only the members used here
struct Settings_unk {
    unsigned char m000[0x158];
    int m158;
    unsigned char m15C[0x190 - 0x15C];
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
    unsigned char m10D4[0x54F4 - 0x10D4];
    unsigned char m54F4[0x20]; // inferred: common message table (fn_803B3B38)
    int m5514;
    int m5518;
    int m551C;
    unsigned char m5520[0x5538 - 0x5520];
    int m5538;
};
extern "C" Field_unk* lbl_8087F430;

// inferred: map data (fn_8049D704)
struct MapData_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
};

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
extern "C" void* lbl_8087F048;
struct PlayerManager_unk {
    unsigned char m00[0x48];
    int m48;
};
extern "C" PlayerManager_unk* lbl_8087F8A0;
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
                fn_801027A4(lbl_8087F048, lbl_8087F8A0->m48);
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

// NONMATCHING: stub
atn::Task* GameScene::fn_803C0634(unsigned int index) {
    return 0;
}

// NONMATCHING: stub
void GameScene::fn_803C0938(unsigned int index) {}

// NONMATCHING: stub
void GameScene::fn_803C0C2C(unsigned int index) {}

// NONMATCHING: stub
void GameScene::fn_803C0F48() {}

// NONMATCHING: stub
atn::Vector3 GameScene::fn_803C11A4(int point) {
    return atn::Vector3(0.0f, 0.0f, 0.0f);
}

// NONMATCHING: stub
void GameScene::fn_803C1338() {}

// NONMATCHING: stub
int GameScene::fn_803C1560(const atn::Vector3* pos, int a2, int a3, int a4, int a5, float radius) {
    return 0;
}

// NONMATCHING: stub
int GameScene::fn_803C17FC(int point) {
    return 0;
}

// NONMATCHING: stub
int GameScene::fn_803C1840(int point) {
    return 0;
}

// NONMATCHING: stub
void GameScene::fn_803C1884() {}

// NONMATCHING: stub
void* GameScene::fn_803C18DC(int type, int link) {
    return 0;
}

// NONMATCHING: stub
void GameScene::vf10() {}

// NONMATCHING: stub
void GameScene::fn_803C1CAC() {}

// NONMATCHING: stub
void GameScene::fn_803C1E3C() {}

// NONMATCHING: stub
void GameScene::fn_803C2048() {}

// NONMATCHING: stub
void GameScene::fn_803C2090() {}

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
                    box.m18 = fn_800DC288(csv.fn_8005B9CC());
                    box.m1C = fn_800DC288(csv.fn_8005B9CC());
                    box.m20 = fn_800DC288(csv.fn_8005B9CC());
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
                    m10C[i].m18 = fn_800DC288(csv.fn_8005B9CC());
                    m10C[i].m1C = fn_800DC288(csv.fn_8005B9CC());
                    m10C[i].m20 = fn_800DC288(csv.fn_8005B9CC());
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
                    m114[i].m18 = fn_800DC288(csv.fn_8005B9CC());
                    m114[i].m1C = fn_800DC288(csv.fn_8005B9CC());
                    m114[i].m20 = fn_800DC288(csv.fn_8005B9CC());
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


// NONMATCHING: stub
void GameScene::fn_803CA0F4() {}

// NONMATCHING: stub
void GameScene::fn_803CA530() {}

// NONMATCHING: stub
void GameScene::fn_803CA604() {}

// NONMATCHING: stub
void GameScene::fn_803CAABC() {}

// NONMATCHING: stub
void GameScene::fn_803CAED0() {}

// NONMATCHING: stub
void GameScene::fn_803CB044() {}

// NONMATCHING: stub
void GameScene::fn_803CB474() {}

// NONMATCHING: stub
void GameScene::fn_803CB618() {}

// NONMATCHING: stub
void GameScene::fn_803CB820() {}

int GameScene::fn_803CC198(const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* chara) {
    return fn_803CC1A4(pos, ((CharaRadius_unk*)chara)->m620, 0.5f, dir);
}

// NONMATCHING: stub
int GameScene::fn_803CC1A4(const atn::Vector3* pos, float radius, float cos, const atn::Vector3* dir) {
    return 0;
}

// NONMATCHING: stub
int GameScene::fn_803CC3BC(const atn::Vector3* pos, const atn::Vector3* dir, CharaTask* chara) {
    return 0;
}

// NONMATCHING: stub
void GameScene::fn_803CC5E4() {}

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
    for (unsigned int i = 0; i < (unsigned int)mDC.mSize; i++) {
        if (mDC.mData[i].m08 == chara) {
            mDC.mData[i].m1C = value;
        }
    }
}

atn::Vector3 GameScene::fn_803CC754(unsigned int index) {
    return m80.mData[index].mPos;
}

void GameScene::fn_803CC774(atn::Task* chara) {
    m134->fn_803ABFD4(chara);
}

// NONMATCHING: stub
void GameScene::fn_803CC77C() {}

// NONMATCHING: stub
int GameScene::fn_803CC900() {
    return 0;
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

// NONMATCHING: stub
int GameScene::fn_803CCA84(CharaTask* chara, atn::Vector3* pos, atn::Vector3* rot) {
    return 0;
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

// NONMATCHING: stub
void GameScene::fn_803CCCC0() {}

void GameScene::fn_803CCD98() {
    if (m134 != 0) {
        m134->fn_803ACD08();
    }
}

// NONMATCHING: stub
void GameScene::fn_803CCDAC() {}
