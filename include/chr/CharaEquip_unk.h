#ifndef CHR_CHARAEQUIP_UNK_H
#define CHR_CHARAEQUIP_UNK_H

#include "atn/Model_unk.h"
#include "atn/LH_Binary.h"
#include "atn/Array_unk.h"
#include <boost/scoped_ptr.hpp>

class CharaTask;
struct CharaParam_unk;
struct OptionParts_unk; // db/OptionParts_unk.h

namespace chr {

// inferred: equipment object (CharaTask::m680[]; chr/CharaEquip_unk.cpp, 0x805634D0-0x80566420).
// m414 points to its row of /boot/equip_param.csv (db/EquipParam_unk.h). Names invented.
struct CharaEquip_unk {
    // inferred: CharaTask::fn_801371AC passes the slot and the character
    CharaEquip_unk(int slot, int id, int color, ::CharaTask* chara, unsigned int* options, ::CharaTask* other,
                   int noOptions, int arg7, int arg8); // 0x805634D0
    ~CharaEquip_unk(); // 0x805638BC
    void fn_80563A1C(const char* name); // inferred: loads the item
    // inferred: adds the hashes of its resources to `list` (CharaTask::fn_8017AD44)
    void fn_80563A60(atn::SimpleVector<unsigned int>* list, void* arg);
    int fn_8056405C();  // inferred: nonzero while loading
    void fn_80564190(); // inferred: reads the equipment file
    void fn_80564340(); // inferred: reads the option parts
    void fn_80565064(); // inferred: applies the "MODEL" rows and the option parts
    void fn_80565428();
    void fn_80565568();
    void fn_805657CC(int color, unsigned int* bits); // inferred: applies the option bits
    void fn_805659B4();
    void ClearLevels_unk();
    void ShowParts_unk();
    void fn_80565B98(); // inferred: places the model on the character
    void fn_80565F34(); // inferred: updates the lights of the model
    void fn_80566010(); // inferred: attaches the model
    void fn_805660DC();
    // inferred: stops the effect of the equipment (inlined by the destructor)
    void fn_80566398(int flag);
    void fn_805663F8();
    void fn_80566408(); // inferred: undoes fn_805663F8 (CharaTask::fn_80145334)

    atn::Model* GetModel_unk() { return &m024; }

    int m000; // inferred: kind (4 is handled apart by CharaTask::fn_80149A30)
    int m004; // inferred: item id ((id % 1000) / 10 == 46: coat with "leftcoat"/"rightcoat" nodes)
    int m008; // inferred: color (game/CosEditBase.cpp)
    int m00C; // inferred: nonzero: no option parts (game/CosEditPalette_unk.cpp)
    int m010;
    int m014;
    int m018;
    int m01C; // inferred: an effect is playing (fn_80566398)
    int m020;
    atn::Model m024; // inferred: model of the item (Event::DialogHead calls its virtuals)
    int m3F4; // inferred: node matrix of "leftarmwepoint" (kind 4)
    atn::SimpleArray<int> m3F8; // inferred: node of the character for each node of the item
    float m400;
    float m404;
    float m408;
    ::CharaTask* m40C; // inferred: character wearing it
    ::CharaTask* m410;
    CharaParam_unk* m414; // inferred: row of /boot/equip_param.csv (db/EquipParam_unk.h)
    atn::LH_Binary m418;  // inferred: equipment file
    int m420;             // inferred: loading state (3: done)
    unsigned int* m424;   // inferred: option bits
    atn::LH_Binary m428;  // inferred: option parts file
    boost::scoped_ptr<OptionParts_unk> m430; // inferred: option parts (db/OptionParts_unk.cpp)
    int m434; // inferred: frames before the effect is played again
    unsigned int m438; // inferred: option bits in use
};

} // namespace chr

#endif
