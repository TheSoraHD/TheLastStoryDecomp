// inferred: second source file of what was one split with game/SpecialEnemyBase.cpp: the enemy name
// table and the enemy factory have their own .rodata block (table strings, table, and a string pool with
// only the "" of new at 0x807637DC), .data jump table and .sdata2 "" (I2, docs/i2-poolorder.md)
#include "game/SpecialEnemyBase.h"
#include "atn/ColliBase.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/ChaserGuard.h"
#include "game/Em314Ivy.h"
#include "game/Em316LaserSphere.h"
#include "game/Em316LastLaser.h"
#include "game/ChaserNpc.h"
#include "game/CopyRobotTask.h"
#include "game/Em002Skelton.h"
#include "game/Em005DoppelGanger.h"
#include "game/Em101ElderZesyaSecond.h"
#include "game/Em101LittleZesyaSecond.h"
#include "game/Em002_01BlackSkelton.h"
#include "game/Em002_02SkeltonKing.h"
#include "game/Em006DarkMaster.h"
#include "game/Em101ElderZesya.h"
#include "game/Em001Necromancer.h"
#include "game/Em201ZanGurg.h"
#include "game/Em201ZanGurgFinal.h"
#include "game/Em204Mulue.h"
#include "game/Em205Ogre.h"
#include "game/Em301LShade.h"
#include "game/Em310Gargoyle.h"
#include "game/Em003LesserDaemon.h"
#include "game/Em101LittleZesya.h"
#include "game/Em302Kraken.h"
#include "game/Em302KrakenMp.h"
#include "game/Em303KrakenFoot.h"
#include "game/Em303KrakenFootMp.h"
#include "game/Em306GurgBeast.h"
#include "game/Em308InvertCrab.h"
#include "game/Em308ParmCrab.h"
#include "game/Em309Ghoul.h"
#include "game/Em309GhoulSimple.h"
#include "game/Em312DarkServant.h"
#include "game/Em315Coffin.h"
#include "game/Em401_01_WhiteLeoPard.h"
#include "game/Em403WarLion.h"
#include "game/Em202Spider.h"
#include "game/Em202_01_BSpider.h"
#include "game/Em202SpiderBase.h"
#include "game/Em203GreaterDaemon.h"
#include "game/Em203GreaterDaemonRed.h"
#include "game/Em208Queen.h"
#include "game/Em209BigGolem.h"
#include "game/Em209SmallGolem.h"
#include "game/Em305VampireMp.h"
#include "game/EnemyFieldMagician.h"
#include "game/EnemyJill.h"
#include "game/EnemyLizerdBlade.h"
#include "game/EnemyQuarkFirst.h"
#include "game/EnemyQuarkSecond.h"
#include "game/EnemyQuarkStand.h"
#include "game/EnemySeaLizerd.h"
#include "game/EnemyTasha.h"
#include "game/EnemyTrista.h"
#include "game/LastCocoonChrysalis.h"
#include "game/Np131LivingArmor.h"
#include "game/Em202ForestSpider.h"
#include "game/Em202MothersSpider.h"
#include "game/Em203GreaterDaemonRedMp.h"
#include "game/Em206CocoonMp.h"
#include "game/Em206EggMonster.h"
#include "game/LastCocoonEgg.h"
#include "game/LastCocoonImago.h"
#include "game/LastCocoonQuadruped.h"
#include "game/QuadrupedTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" CharaInfo_unk* fn_8020924C(int id);

static const char* const sEnemyNames[] = {
    "",
    "NECROMANCER",
    "EM002",
    "EM002_01",
    "EM003",
    "EM005",
    "EM006",
    "EM101_ELDER",
    "EM101_ELDER_SECOND",
    "EM101_LITTLE",
    "EM101_LITTLE_SECOND",
    "EM201",
    "EM202",
    "EM202_01",
    "EM202_BASE",
    "EM202FOREST",
    "EM202MOTHERS",
    "EM203",
    "EM203_RED",
    "EM203_RED_MP",
    "EM204",
    "EM204_BIG",
    "EM204_MP",
    "EM205",
    "EM206",
    "EM206_MP",
    "EM208",
    "EM209_SMALL",
    "EM209_BIG",
    "EM301",
    "EM302",
    "EM303",
    "EM302_MP",
    "EM303_MP",
    "EM305",
    "EM305_MP",
    "EM306",
    "EM308",
    "EM308_INVERT",
    "EM309",
    "EM309_SIMPLE",
    "EM310",
    "EM312",
    "EM314",
    "EM315",
    "EM316",
    "EM316_LAST",
    "EM401_01",
    "EM403",
    "EM601_BIT",
    "QUADRUPED",
    "COPYROBOT",
    "LIVING_ARMOR",
    "CHASER",
    "CHASER_NPC",
    "EM_TRISTA",
    "LAST_COCOON_EGG",
    "LAST_COCOON_CHRYSALIS",
    "LAST_COCOON_QUADRUPED",
    "LAST_COCOON_IMAGO",
    "LAST_COCOON_IMAGO_FINAL",
    "EM_QUARK",
    "EM_QUARK_STAND",
    "EM_JILL",
    "EM_TASHA",
    "ZANGURG_FINAL",
    "FR_ARGANAN",
    "EM_FIELD_MAGICIAN",
    "EM002_02_SKELTON_KING",
    "EM_SEA_LIZERD",
    "EM_QUARK_1ST",
    "EM_QUARK_2ND",
    "EM_LIZERD_BLADE",
};

// inferred: index of an enemy name (0 if missing)
extern "C" int fn_805A52F4(const char* s) {
    int result = 0;
    for (int i = 0; i < 73; i++) {
        if (strcmp(s, sEnemyNames[i]) == 0) {
            result = i;
            break;
        }
    }
    return result;
}


// inferred: creates the enemy of the kind given by its character table entry (EnemyTask::fn_8035B534)
extern "C" EnemyTask* fn_805A5374(atn::Task* parent, EnemyInfo_unk* info) {
    void* data = info->m20;
    CharaInfo_unk* chara = fn_8020924C((int)data);
    if (chara != 0) {
        int kind = chara->m24;
        EnemyTask* enemy = 0;
        switch (kind) {
        case 1:
            enemy = new ((MemoryTag)3, "", "", 0) Em001Necromancer(parent, info);
            break;
        case 50:
            enemy = new ((MemoryTag)3, "", "", 0) QuadrupedTask(parent, data);
            break;
        case 36:
            enemy = new ((MemoryTag)3, "", "", 0) Em306GurgBeast(parent, info);
            break;
        case 12:
            enemy = new ((MemoryTag)3, "", "", 0) Em202Spider(parent, info);
            break;
        case 14:
            enemy = new ((MemoryTag)3, "", "", 0) Em202SpiderBase(parent, info);
            break;
        case 29:
            enemy = new ((MemoryTag)3, "", "", 0) Em301LShade(parent, info);
            break;
        case 20:
            enemy = new ((MemoryTag)3, "", "", 0) Em204Mulue(parent, info);
            break;
        case 21:
            enemy = new ((MemoryTag)3, "", "", 0) Em204BigMulue(parent, info);
            break;
        case 22:
            enemy = new ((MemoryTag)3, "", "", 0) Em204MulueMp(parent, info);
            break;
        case 34:
            enemy = new ((MemoryTag)3, "", "", 0) Em305Vampire(parent, info);
            break;
        case 13:
            enemy = new ((MemoryTag)3, "", "", 0) Em202_01_BSpider(parent, data);
            break;
        case 11:
            enemy = new ((MemoryTag)3, "", "", 0) Em201ZanGurg(parent, info);
            break;
        case 24:
            enemy = new ((MemoryTag)3, "", "", 0) Em206EggMonster(parent, data);
            break;
        case 2:
            enemy = new ((MemoryTag)3, "", "", 0) Em002Skelton(parent, data);
            break;
        case 3:
            enemy = new ((MemoryTag)3, "", "", 0) Em002_01BlackSkelton(parent, data);
            break;
        case 39:
            enemy = new ((MemoryTag)3, "", "", 0) Em309Ghoul(parent, info);
            break;
        case 40:
            enemy = new ((MemoryTag)3, "", "", 0) Em309GhoulSimple(parent, info);
            break;
        case 41:
            enemy = new ((MemoryTag)3, "", "", 0) Em310Gargoyle(parent, info);
            break;
        case 47:
            enemy = new ((MemoryTag)3, "", "", 0) Em401_01_WhiteLeoPard(parent, info);
            break;
        case 48:
            enemy = new ((MemoryTag)3, "", "", 0) Em403WarLion(parent, data);
            break;
        case 23:
            enemy = new ((MemoryTag)3, "", "", 0) Em205Ogre(parent, info);
            break;
        case 26:
            enemy = new ((MemoryTag)3, "", "", 0) Em208Queen(parent, data);
            break;
        case 52:
            enemy = new ((MemoryTag)3, "", "", 0) Np131LivingArmor(parent, info);
            break;
        case 43:
            enemy = new ((MemoryTag)3, "", "", 0) Em314Ivy(parent, info);
            break;
        case 37:
            enemy = new ((MemoryTag)3, "", "", 0) Em308ParmCrab(parent, info);
            break;
        case 38:
            enemy = new ((MemoryTag)3, "", "", 0) Em308InvertCrab(parent, info);
            break;
        case 15:
            enemy = new ((MemoryTag)3, "", "", 0) Em202ForestSpider(parent, info);
            break;
        case 4:
            enemy = new ((MemoryTag)3, "", "", 0) Em003LesserDaemon(parent, info);
            break;
        case 17:
            enemy = new ((MemoryTag)3, "", "", 0) Em203GreaterDaemon(parent, info);
            break;
        case 18:
            enemy = new ((MemoryTag)3, "", "", 0) Em203GreaterDaemonRed(parent, info);
            break;
        case 19:
            enemy = new ((MemoryTag)3, "", "", 0) Em203GreaterDaemonRedMp(parent, info);
            break;
        case 5:
            enemy = new ((MemoryTag)3, "", "", 0) Em005DoppelGanger(parent, info);
            break;
        case 30:
            enemy = new ((MemoryTag)3, "", "", 0) Em302Kraken(parent, info);
            break;
        case 31:
            enemy = new ((MemoryTag)3, "", "", 0) Em303KrakenFoot(parent, info);
            break;
        case 32:
            enemy = new ((MemoryTag)3, "", "", 0) Em302KrakenMp(parent, info);
            break;
        case 33:
            enemy = new ((MemoryTag)3, "", "", 0) Em303KrakenFootMp(parent, info);
            break;
        case 51:
            enemy = new ((MemoryTag)3, "", "", 0) CopyRobotTask(parent, info);
            break;
        case 44:
            enemy = new ((MemoryTag)3, "", "", 0) Em315Coffin(parent, info);
            break;
        case 6:
            enemy = new ((MemoryTag)3, "", "", 0) Em006DarkMaster(parent, info);
            break;
        case 42:
            enemy = new ((MemoryTag)3, "", "", 0) Em312DarkServant(parent, info);
            break;
        case 53:
            enemy = new ((MemoryTag)3, "", "", 0) ChaserGuard(parent, info);
            break;
        case 54:
            enemy = new ((MemoryTag)3, "", "", 0) ChaserNpc(parent, info);
            break;
        case 55:
            enemy = new ((MemoryTag)3, "", "", 0) EnemyTrista(parent, info);
            break;
        case 49:
            enemy = new ((MemoryTag)3, "", "", 0) Em316LaserSphere(parent, info);
            break;
        case 56:
            enemy = new ((MemoryTag)3, "", "", 0) LastCocoonEgg(parent, info);
            break;
        case 57:
            enemy = new ((MemoryTag)3, "", "", 0) LastCocoonChrysalis(parent, info);
            break;
        case 58:
            enemy = new ((MemoryTag)3, "", "", 0) LastCocoonQuadruped(parent, info);
            break;
        case 59:
            enemy = new ((MemoryTag)3, "", "", 0) LastCocoonImago(parent, info);
            break;
        case 7:
            enemy = new ((MemoryTag)3, "", "", 0) Em101ElderZesya(parent, info);
            break;
        case 9:
            enemy = new ((MemoryTag)3, "", "", 0) Em101LittleZesya(parent, info);
            break;
        case 63:
            enemy = new ((MemoryTag)3, "", "", 0) EnemyJill(parent, info);
            break;
        case 45:
            enemy = new ((MemoryTag)3, "", "", 0) Em316LaserSphere(parent, info);
            break;
        case 46:
            enemy = new ((MemoryTag)3, "", "", 0) Em316LastLaser(parent, info);
            break;
        case 27:
            enemy = new ((MemoryTag)3, "", "", 0) Em209SmallGolem(parent, info);
            break;
        case 28:
            enemy = new ((MemoryTag)3, "", "", 0) Em209BigGolem(parent, info);
            break;
        case 62:
            enemy = new ((MemoryTag)3, "", "", 0) EnemyQuarkStand(parent, info);
            break;
        case 70:
            enemy = new ((MemoryTag)3, "", "", 0) EnemyQuarkFirst(parent, info);
            break;
        case 71:
            enemy = new ((MemoryTag)3, "", "", 0) EnemyQuarkSecond(parent, info);
            break;
        case 64:
            enemy = new ((MemoryTag)3, "", "", 0) EnemyTasha(parent, info);
            break;
        case 8:
            enemy = new ((MemoryTag)3, "", "", 0) Em101ElderZesyaSecond(parent, info);
            break;
        case 10:
            enemy = new ((MemoryTag)3, "", "", 0) Em101LittleZesyaSecond(parent, info);
            break;
        case 65:
            enemy = new ((MemoryTag)3, "", "", 0) Em201ZanGurgFinal(parent, info);
            break;
        case 25:
            enemy = new ((MemoryTag)3, "", "", 0) Em206CocoonMp(parent, info);
            break;
        case 67:
            enemy = new ((MemoryTag)3, "", "", 0) EnemyFieldMagician(parent, info);
            break;
        case 68:
            enemy = new ((MemoryTag)3, "", "", 0) Em002_02SkeltonKing(parent, info);
            break;
        case 69:
            enemy = new ((MemoryTag)3, "", "", 0) EnemySeaLizerd(parent, info);
            break;
        case 16:
            enemy = new ((MemoryTag)3, "", "", 0) Em202MothersSpider(parent, info);
            break;
        case 35:
            enemy = new ((MemoryTag)3, "", "", 0) Em305VampireMp(parent, info);
            break;
        case 72:
            enemy = new ((MemoryTag)3, "", "", 0) EnemyLizerdBlade(parent, info);
            break;
        }
    if (enemy != 0) {
            enemy->m146C = kind;
            return enemy;
        }
    }
    return 0;
}
