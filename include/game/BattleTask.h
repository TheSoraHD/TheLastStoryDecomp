#ifndef GAME_BATTLETASK_H
#define GAME_BATTLETASK_H

#include "atn/Task.h"
#include "atn/EfpHandle_unk.h"

class CharaTask;
struct EffectColor_unk;
struct ColorF_unk;
struct HitInfo_unk;
namespace atn {
struct Model;
}

// inferred: 0x934-byte entry of BattleTask::mCharas (only the character is known)
struct BattleChara_unk {
    CharaTask* mChara;
    unsigned char m04[0x934 - 0x4];
};

// inferred from __vt__10BattleTask (0x80779DA0): battle manager (lbl_8087F048). Other units still use
// partial views of it named Effect_unk (CharaTask, CharaStatus, AI...); the methods those call keep the
// Effect_unk name until the views are unified. Method names invented.
class BattleTask : public atn::Task {
public:
    // inferred: start (model, group) and stop (character, group) of the battle effects; the stop
    // functions call fn_80239DAC with the effect group of the start
    void fn_80105950(void* owner, atn::Model* model);
    void fn_801059F8(CharaTask* chara); // inferred: called when a magic ends
    void fn_80105A08(void* owner, atn::Model* model);
    void fn_80105AB0(void* owner, atn::Model* model, const EffectColor_unk& color);
    void fn_80105B3C(CharaTask* chara);
    void fn_80105B4C(void* owner, atn::Model* model, const EffectColor_unk* color);
    void fn_80105F3C(void* owner);
    void fn_8010607C(void* owner);
    void fn_8010652C(void* owner);
    void fn_8010653C(void* owner, atn::Model* model);
    void fn_801065E4(void* owner);
    void fn_801065F4(CharaTask* chara, atn::Model* model, const ColorF_unk* color); // inferred: tints the model
    void fn_80106680(void* owner, atn::Model* model);
    void fn_80106728(int arg, atn::Model* model); // inferred: called when a part is destroyed
    void fn_801067D0(CharaTask* chara, atn::Model* model);
    void fn_80107574(); // inferred: hides the chant effect
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
    float fn_8010CA24();
    float fn_8010CA2C();
    float fn_8010CA34();
    float fn_8010CCD0();
    int fn_8010D770(HitInfo_unk* hit);
    int fn_8010D7B4(HitInfo_unk* hit);

    unsigned char m48[0xCBF0 - 0x48];
    BattleChara_unk mCharas[0x48]; // 0xCBF0: inferred: characters known to the battle (CharaTask::fn_80176E30)
    unsigned char m36290[0x363B0 - 0x36290];
    int mCharaCount;               // 0x363B0
    unsigned char m363B4[0x363C0 - 0x363B4];
    atn::EfpFile mEfp363C0[46];  // 0x363C0: inferred: battle effects (EFP files)
    unsigned char m365E8[0x38A04 - 0x365E8];
    float m38A04; // inferred: gauge value, clamped to [0, m38A0C]
    float m38A08;
    float m38A0C; // inferred: gauge maximum
    unsigned char m38A10[0x38A28 - 0x38A10];
    float m38A28; // inferred: gauge increments
    float m38A2C;
    unsigned char m38A30[0x38BC0 - 0x38A30];
    atn::EfpFile mEfp38BC0[15];  // 0x38BC0: inferred: status effects (EFP files)
    unsigned char m38C74[0x3E300 - 0x38C74];
    int m3E300; // inferred: state of the chant effect (2: hidden)
};

#endif
