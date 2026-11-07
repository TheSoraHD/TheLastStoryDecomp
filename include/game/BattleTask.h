#ifndef GAME_BATTLETASK_H
#define GAME_BATTLETASK_H

#include "atn/Task.h"
#include "atn/EfpHandle_unk.h"

class CharaTask;
struct EffectColor_unk;
struct ColorF_unk;
namespace atn {
struct Model;
}

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
    void fn_80107CEC(void* owner, atn::Model* model, float scale);
    void fn_80107D94(void* owner);
    void fn_80107E58(void* owner);
    void fn_80107E68(void* owner, atn::Model* model);
    void fn_80107F10(void* owner);
    void fn_80107F20(void* owner, atn::Model* model, float scale);
    void fn_80107FC8(void* owner);
    void fn_80107FD8(void* owner, atn::Model* model);
    void fn_80108080(void* owner);

    unsigned char m48[0x363C0 - 0x48];
    atn::EfpFile mEfp363C0[46];  // 0x363C0: inferred: battle effects (EFP files)
    unsigned char m365E8[0x38BC0 - 0x365E8];
    atn::EfpFile mEfp38BC0[15];  // 0x38BC0: inferred: status effects (EFP files)
    unsigned char m38C74[0x3E300 - 0x38C74];
    int m3E300; // inferred: state of the chant effect (2: hidden)
};

#endif
