#ifndef GAME_BATTLETASK_H
#define GAME_BATTLETASK_H

#include "atn/Task.h"
#include "atn/EfpHandle_unk.h"

class CharaTask;
struct EffectColor_unk;
struct ColorF_unk;
struct HitInfo_unk;
struct RayObj_unk;
class PlayerTask;
namespace atn {
struct Vector3;
}
namespace atn {
struct Model;
}

// inferred: object hit by the cursor ray (as game/BattleCharaTask.cpp)
struct RayObj_unk {
    unsigned char m00[0x08];
    unsigned int m08; // inferred: flags (2: the object is a character, 8: another object)
    CharaTask* m0C;
};

// inferred: 0x21C-byte entry of BattleTask::mSlots (only the flag and the character are known)
struct BattleSlot_unk {
    int mUsed;
    unsigned char m04[0x218 - 0x4];
    CharaTask* mChara;
};

// inferred: 0x934-byte entry of BattleTask::mCharas (constructor 0x80108090, built by __construct_array)
struct BattleChara_unk {
    BattleChara_unk();  // 0x80108090
    ~BattleChara_unk(); // 0x80108150
    void fn_801088B8(int id, int value); // inferred: adds a hate entry (at most 10 per id; drops the oldest)
    void fn_8010895C(int id, int all);   // inferred: removes the entries of an id and compacts the list
    void fn_801081D8();                  // inferred: resets the entry

    CharaTask* mChara;
    int m04;
    int m08[72];        // inferred: one value per entry of BattleTask::mCharas
    float m128[72];
    float m248;
    float m24C[72];
    int m36C[72];
    int m48C[72];
    int m5AC[72];
    int m6CC;           // inferred: random value 0..70
    short* mHateValues; // 0x6D0: inferred
    short* mHateIds;    // 0x6D4: inferred
    int mHateCount;     // 0x6D8: inferred: size of both arrays (10, or 5 in multiplayer)
    int m6DC;
    int m6E0;
    float m6E4[72];
    int m804;
    int m808;
    int m80C[72];
    unsigned char m92C;
    unsigned char m92D;
    unsigned char m92E;
    unsigned char m92F;
    int m930;
};

// inferred from __vt__10BattleTask (0x80779DA0): battle manager (lbl_8087F048). Other units still use
// partial views of it named Effect_unk (CharaTask, CharaStatus, AI...); the methods those call keep the
// Effect_unk name until the views are unified. Method names invented.
class BattleTask : public atn::Task {
public:
    // inferred: index of the character in mCharas, -1 if missing (inlined)
    int FindChara_unk(CharaTask* chara) {
        if (chara == 0) {
            return -1;
        }
        for (int i = 0; i < mCharaCount; i++) {
            if (mCharas[i].mChara == chara) {
                return i;
            }
        }
        return -1;
    }
    // inferred: start (model, group) and stop (character, group) of the battle effects; the stop
    // functions call fn_80239DAC with the effect group of the start
    int fn_80100BB8(CharaTask** out, CharaTask* chara, const atn::Vector3* dir, float range, float theta);
    void fn_801021B4(RayObj_unk* obj, int a, int b); // inferred: cursor target
    void fn_80102824(CharaTask* chara, PlayerTask* leader);
    void fn_80102A40(CharaTask* chara, CharaTask* target, int a, int b); // inferred: attack order (hate)
    void fn_80103600(CharaTask* chara);
    BattleChara_unk* fn_80103F60(CharaTask* chara); // inferred: entry of the character, null if missing
    void fn_80103FD4();
    void fn_80105510(int value); // inferred: sets m363BC (clamped; 9999 sets the field flag 10)
    void fn_801058F8(); // inferred: updates every player
    void fn_80105950(void* owner, atn::Model* model);
    void fn_801059F8(CharaTask* chara); // inferred: called when a magic ends
    void fn_80105A08(void* owner, atn::Model* model);
    void fn_80105AB0(void* owner, atn::Model* model, const EffectColor_unk& color);
    void fn_80105B3C(CharaTask* chara);
    void fn_80105B4C(void* owner, atn::Model* model, const EffectColor_unk* color);
    void fn_80105BD8(atn::Model* model);
    void fn_80105C9C(CharaTask* chara, atn::Model* model);
    void fn_80105F3C(void* owner);
    void fn_80105F4C(CharaTask* chara, atn::Model* model);
    void fn_8010607C(void* owner);
    void fn_8010608C(CharaTask* chara, atn::Model* model);
    void fn_8010652C(void* owner);
    void fn_8010653C(void* owner, atn::Model* model);
    void fn_801065E4(void* owner);
    void fn_801065F4(CharaTask* chara, atn::Model* model, const ColorF_unk* color); // inferred: tints the model
    void fn_80106680(void* owner, atn::Model* model);
    void fn_80106728(int arg, atn::Model* model); // inferred: called when a part is destroyed
    void fn_801067D0(CharaTask* chara, atn::Model* model);
    void fn_80107208(int id, const atn::Vector3* pos, float scale); // inferred: sound of the battle effect table
    int fn_80107274(CharaTask* chara);  // inferred: adds the character to mSlots (0: already there or full)
    void fn_80107380(CharaTask* chara); // inferred: removes the character from mSlots
    void fn_80107574(); // inferred: hides the chant effect
    void fn_80107584(); // inferred: updates the chant effect (fades it in or out)
    void fn_801076D0(void* owner, atn::Model* model); // inferred: paralysis effect
    void fn_80107798(void* owner, int flag);
    void fn_801077A8(void* owner, atn::Model* model);
    void fn_80107850(void* owner);
    void fn_80107860(void* owner, atn::Model* model);
    void fn_80107908(void* owner, atn::Model* model);
    void fn_801079B0(void* owner);
    void fn_801079C0(void* owner, atn::Model* model);
    void fn_80107A68(void* owner);
    void fn_80107A78(void* owner, atn::Model* model);
    void fn_80107B20(void* owner);
    void fn_80107B30(void* owner, atn::Model* model);
    void fn_80107BD8(void* owner);
    void fn_80107BE8(void* owner, atn::Model* model);
    void fn_80107CA8(void* owner, int flag);
    void fn_80107CEC(void* owner, atn::Model* model, float scale);
    void fn_80107D94(void* owner);
    void fn_80107DA4(void* owner, atn::Model* model, float scale, const atn::Vector3& pos);
    void fn_80107E58(void* owner);
    void fn_80107E68(void* owner, atn::Model* model);
    void fn_80107F10(void* owner);
    void fn_80107F20(void* owner, atn::Model* model, float scale);
    void fn_80107FC8(void* owner);
    void fn_80107FD8(void* owner, atn::Model* model);
    void fn_80108080(void* owner);
    int fn_80108378(CharaTask* chara); // inferred: index of the character in mCharas of the instance, -1 if missing
    void fn_80108A54(CharaTask* chara);
    void fn_80108A5C(CharaTask* chara, void* arg);
    void fn_80109828(const wchar_t* text); // inferred: debug log message
    void fn_8010AB34();
    void fn_8010AB94();
    int fn_8010B180(CharaTask* a, CharaTask* b); // inferred: the two characters can reach each other (AI areas)
    float fn_8010CA24();
    float fn_8010CA2C();
    float fn_8010CA34();
    float fn_8010CA3C(CharaTask* chara, float rate);
    float fn_8010CCD0();
    float fn_8010CCD8(CharaTask* chara); // inferred: gauge gained by the magic
    int fn_8010D770(HitInfo_unk* hit);
    int fn_8010D7B4(HitInfo_unk* hit);

    unsigned char m48[0xBA50 - 0x48];
    BattleSlot_unk mSlots[8];      // 0xBA50: inferred: characters with an active battle effect
    unsigned char mCB30[0xCBF0 - 0xCB30];
    BattleChara_unk mCharas[0x48]; // 0xCBF0: inferred: characters known to the battle (CharaTask::fn_80176E30)
    unsigned char m36290[0x363B0 - 0x36290];
    int mCharaCount;               // 0x363B0
    unsigned char m363B4[0x363BC - 0x363B4];
    int m363BC;
    atn::EfpFile mEfp363C0[46];  // 0x363C0: inferred: battle effects (EFP files)
    unsigned char m365E8[0x389F4 - 0x365E8];
    int m389F4; // inferred: owner tag of the chant effect (its address is the effect owner)
    unsigned char m389F8[0x38A04 - 0x389F8];
    float m38A04; // inferred: gauge value, clamped to [0, m38A0C]
    float m38A08;
    float m38A0C; // inferred: gauge maximum
    unsigned char m38A10[0x38A28 - 0x38A10];
    float m38A28; // inferred: gauge increments
    float m38A2C;
    unsigned char m38A30[0x38A48 - 0x38A30];
    float m38A48; // inferred: gauge gained by a magic
    unsigned char m38A4C[0x38AF4 - 0x38A4C];
    atn::EfpFile mEfp38AF4[32];  // 0x38AF4: inferred: status and magic effects (EFP files)
    unsigned char m38C74[0x3E2E0 - 0x38C74];
    int m3E2E0; // inferred: character under the cursor
    CharaTask* m3E2E4; // inferred: object under the cursor
    RayObj_unk* m3E2E8; // inferred: cursor target
    int m3E2EC;
    int m3E2F0;
    unsigned char m3E2F4[0x3E2FC - 0x3E2F4];
    float m3E2FC; // inferred: alpha of the chant effect
    int m3E300;   // inferred: state of the chant effect (1: fading in, 2: fading out)
    float m3E304; // inferred: color of the chant effect (set to 1, 1, 1, 1)
    float m3E308;
    float m3E30C;
    float m3E310;
};

#endif
