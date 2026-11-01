// inferred: weapon factory (0x800457A4); a file of its own: its "" has its own string pool
// (0x80730F08) after the one of game/Arms.cpp
#include "game/Arms.h"
#include "game/Gun.h"
#include "game/Sword.h"
#include "atn/Memory.h"

extern "C" void* fn_80206B14(int arg, int item); // inferred: equipment data

// inferred: creates the weapon of `kind` (0 sword, 1 and 2 guns); null when the equipment data of the
// first kinds is missing
extern "C" Arms* fn_800457A4(int kind, int id, int arg0C, CharaTask* chara, int arg5) {
    if (kind < 3 && fn_80206B14(kind, id) == 0) {
        return 0;
    }
    Arms* arms = 0;
    switch (kind) {
    case 0:
        arms = new ((MemoryTag)3, "", "", 0) Sword(kind, id, arg0C, chara, arg5);
        break;
    case 1:
        arms = new ((MemoryTag)3, "", "", 0) Gun(kind, id, arg0C, chara, arg5);
        break;
    case 2:
        arms = new ((MemoryTag)3, "", "", 0) Gun(kind, id, arg0C, chara, arg5);
        break;
    }
    return arms;
}
