#include "game/CharaTask.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/SkillEffectManager.h"
#include "std/string.h"
#include "std/new.h"
#include "atn/Memory.h"

struct AIStringWithAllocator_unk;

// inferred: global AI state. The three objects are separate (the code addresses each one with its own
// lis/addi except where MWCC derives lbl_807C6A40 from lbl_807C68C0 + 0x180), so they are defined
// in this file in .bss order. Field names are offsets; types from the code using them.
struct AiInput_unk { // lbl_807C68C0 (0x180): filled before the script runs
    CharaTask* m00; // 0x00: character running the script
    int m04;
    int m08;
    int m0C;
    atn::Vector3 m10; // 0x10: position
    float m1C;
    CharaTask* m20; // 0x20: target
    int m24;
    int m28;
    int m2C;
    int m30;
    int m34;
    int m38;
    int m3C;
    int m40;
    int m44;
    int m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    int m5C;
    int m60;
    int m64;
    int m68;
    int m6C;
    int m70;
    int m74;
    int m78;
    int m7C;
    int m80;
    int m84;
    int m88;
    int m8C;
    int m90;
    int m94;
    int m98;
    int m9C;
    int mA0;
    int mA4;
    int mA8;
    int mAC;
    int mB0;
    int mB4;
    int mB8;
    int mBC;
    int mC0;
    int mC4;
    int mC8;
    int mCC;
    int mD0;
    int mD4;
    int mD8;
    int mDC;
    int mE0;
    int mE4;
    int mE8;
    int mEC;
    int mF0;
    int mF4;
    int mF8;
    int mFC;
    int m100;
    int m104;
    int m108;
    int m10C;
    int m110;
    int m114;
    int m118;
    int m11C;
    int m120;
    int m124;
    int m128;
    int m12C;
    int m130;
    int m134;
    int m138;
    int m13C;
    int m140;
    int m144;
    int m148;
    int m14C;
    int m150;
    int m154;
    int m158;
    int m15C;
    int m160;
    int m164;
    int m168;
    int m16C;
    int m170;
    int m174;
    int m178;
    int m17C;
};
struct AiOutput_unk { // lbl_807C6A40 (0xF0): result of the script step
    int m00; // 0x00: set to 1 by every script step
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28;
    int m2C;
    int m30;
    int m34;
    int m38;
    int m3C;
    int m40;
    int m44;
    int m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    int m5C;
    int m60;
    int m64;
    int m68;
    int m6C;
    int m70;
    int m74;
    int m78;
    int m7C;
    int m80;
    int m84;
    int m88;
    int m8C;
    int m90;
    int m94;
    int m98;
    int m9C;
    int mA0;
    int mA4;
    int mA8;
    int mAC;
    int mB0;
    int mB4;
    int mB8;
    int mBC;
    int mC0;
    int mC4;
    int mC8;
    int mCC;
    int mD0;
    int mD4;
    int mD8;
    int mDC;
    CharaTask* mE0;
    int mE4;
    int mE8;
    int mEC;
};
struct AiTargets_unk { // lbl_807C6B30 (0x48)
    int m00;
    CharaTask* m04; // 0x04: selected target
    CharaTask* m08[16]; // 0x08: candidates (fn_8001A464)
};

extern "C" {

extern void* lbl_8087EE68;
extern CharaManager<PlayerTask>* lbl_8087F8A0;
AiInput_unk lbl_807C68C0;
AiOutput_unk lbl_807C6A40;
AiTargets_unk lbl_807C6B30;
extern unsigned char lbl_807307A0[];
extern float lbl_80880798;
extern float lbl_808807B8;
extern float lbl_808807AC;
extern float lbl_8088079C;
extern float lbl_808807A0;
extern float lbl_808807A4;
extern float lbl_808807A8;
int fn_8001B5B4(atn::Vector3 pos, int arg);
int fn_8001BA88(CharaTask* chara, int point, int a, int b, float c, float d, float e); // inferred: route point search
extern double lbl_8072FF68;
extern unsigned char lbl_807C6B78[];
extern unsigned char lbl_807C6B84[];
extern unsigned char lbl_8087D6F0;
extern unsigned char lbl_8087D6F1;
void __register_global_object(void* object, void (*destructor)(void*, int), void* dso);
int fn_80018078(void* routes, CharaTask* chara);
int fn_80018BAC(void* routes, CharaTask* chara);
int fn_8021A984(HitInfo_unk* hit);
int fn_80599C90(Bullet_unk* bullet, CharaTask* chara);
int fn_8001AEDC(int mode, int* a, int* b, int* c, int* d);
void fn_8001A464(CharaTask** list);
void fn_8001A65C(CharaTask** list);
void fn_8001A788(CharaTask** list);
int fn_8001BE00(CharaTask* chara);
int fn_8001B634(CharaTask* chara); // inferred: item to use (0: none)
int fn_8003CDB0();
int fn_8003D298(int a, int b);
float fn_8001BD14(CharaTask* chara); // inferred: attack range of the character
float fn_8001AD80(CharaTask* a, CharaTask* b);
int fn_8001BEB0(CharaTask* chara);
void fn_8001C068(CharaTask* chara, const char* name); // inferred: sets the debug state text
int fn_8001BF4C(CharaTask* chara);
void fn_8001BADC(CharaTask* chara, int arg);
int fn_8001B5DC(CharaTask* chara); // inferred: item to use (as fn_8001B634)
int fn_8001BC80(CharaTask* chara, int index);
int fn_8001BC38(CharaTask* chara, int slot);
float fn_8001ADE8(int id, CharaTask* chara); // inferred: distance for skill id
int fn_8001BCB4(CharaTask* chara, int slot); // inferred: slot holds a usable skill
int fn_8001B4A4();
int fn_8001AEBC(int size);
int rand();
int fn_8003DBA4();
int fn_800281A0();
int fn_80028D00();
int fn_80029500();
void fn_8001C284();
void fn_8001E730();
void* fn_8001EA18(void*);
AiInput_unk* fn_8001C260();
AiOutput_unk* fn_8001C26C();
AiTargets_unk* fn_8001C278();
int fn_8001ECD0(void*, int);
void fn_8001E1D0(void*, void*, const AIStringWithAllocator_unk*);
}

namespace AI {

namespace Script {

// inferred: base of the AI scripts (only its RTTI is left in .data). The virtuals take no
// arguments and do not use the object: the scripts work on the global AI state
// (lbl_807C68C0, lbl_807C6A40). vf08 is overridden by every script; the defaults return -1.
class CharaAi {
public:
    virtual int vf08() = 0;
    virtual int vf0C(); // 0x8001EDE8
    virtual int vf10(); // 0x8001EDF0
    virtual int vf14(); // 0x8001EDF8
    virtual int vf18(); // 0x8001EE00
    virtual int vf1C(); // 0x8001EE08
    virtual int vf20(); // 0x8001EE10
    virtual int vf24(); // 0x8001EE18
    virtual int vf28(); // 0x8001EE20
    virtual int vf2C(); // 0x8001EE28
    virtual int vf30(); // 0x8001EE30
    virtual int vf34(); // 0x8001EE38
    virtual int vf38(); // 0x8001EE40
    virtual int vf3C(); // 0x8001EE48
    virtual int vf40(); // 0x8001EE50
};

// inferred: one class per script name registered by fn_8001C284 (vtable slots from .data)
class AI_archer_attack01 : public CharaAi {
public:
    AI_archer_attack01();
    static int fn_8001FB08(); // 0x8001FB08
    virtual int vf08(); // 0x8001EE58
    virtual int vf0C(); // 0x8001F6F8
    virtual int vf18(); // 0x8001F248
    virtual int vf1C(); // 0x8001F814
    virtual int vf20(); // 0x8001EEBC
    virtual int vf24(); // 0x8001F478
    virtual int vf28(); // 0x8001F944
    virtual int vf2C(); // 0x8001FA2C
    virtual int vf40(); // 0x8001F674
};
class AI_archer_guard01 : public CharaAi {
public:
    AI_archer_guard01();
    static int fn_800209F0(); // 0x800209F0
    virtual int vf08(); // 0x8001FC40
    virtual int vf0C(); // 0x8002055C
    virtual int vf18(); // 0x8002008C
    virtual int vf1C(); // 0x800206B8
    virtual int vf20(); // 0x8001FCB4
    virtual int vf24(); // 0x800202BC
    virtual int vf28(); // 0x8002082C
    virtual int vf2C(); // 0x80020914
    virtual int vf40(); // 0x800204D8
};
class AI_arganan01 : public CharaAi {
public:
    AI_arganan01();
    virtual int vf08(); // 0x80020B28
    virtual int vf0C(); // 0x80020B80
    virtual int vf18(); // 0x80020E64
    virtual int vf20(); // 0x8002113C
    virtual int vf24(); // 0x80020F40
    virtual int vf2C(); // 0x80021038
    virtual int vf40(); // 0x800210AC
};
class AI_buffer_attackstable01 : public CharaAi {
public:
    AI_buffer_attackstable01();
    virtual int vf08(); // 0x800211B0
    virtual int vf0C(); // 0x8002121C
    virtual int vf2C(); // 0x80021404
    virtual int vf40(); // 0x80021478
};
class AI_buffer_guard01 : public CharaAi {
public:
    AI_buffer_guard01();
    virtual int vf08(); // 0x80021508
    virtual int vf0C(); // 0x80021594
    virtual int vf18(); // 0x80021A20
    virtual int vf20(); // 0x80021D40
    virtual int vf24(); // 0x80021AFC
    virtual int vf2C(); // 0x80021C3C
    virtual int vf40(); // 0x80021CB0
};
class AI_buffer_guard02 : public CharaAi {
public:
    AI_buffer_guard02();
    virtual int vf08(); // 0x80021DB4
    virtual int vf0C(); // 0x80021E40
    virtual int vf18(); // 0x80022308
    virtual int vf20(); // 0x80022628
    virtual int vf24(); // 0x800223E4
    virtual int vf2C(); // 0x80022524
    virtual int vf40(); // 0x80022598
};
class AI_buffer_pointguard01 : public CharaAi {
public:
    AI_buffer_pointguard01();
    virtual int vf08(); // 0x8002269C
    virtual int vf0C(); // 0x800226E4
    virtual int vf1C(); // 0x80022C04
    virtual int vf28(); // 0x80022F44
    virtual int vf2C(); // 0x80022BD4
    virtual int vf40(); // 0x80022E20
};
class AI_em_archer_attack02 : public CharaAi {
public:
    AI_em_archer_attack02();
    static int fn_80023C84(); // 0x80023C84
    virtual int vf08(); // 0x80022F74
    virtual int vf0C(); // 0x800237E8
    virtual int vf14(); // 0x80023C54
    virtual int vf18(); // 0x80023338
    virtual int vf1C(); // 0x80023960
    virtual int vf20(); // 0x80022FAC
    virtual int vf24(); // 0x80023568
    virtual int vf28(); // 0x80023A90
    virtual int vf2C(); // 0x80023B78
    virtual int vf40(); // 0x80023764
};
class AI_em_bridge_archer : public CharaAi {
public:
    AI_em_bridge_archer();
    virtual int vf08(); // 0x80023DBC
    virtual int vf0C(); // 0x80023E90
    virtual int vf18(); // 0x8002403C
    virtual int vf24(); // 0x800240C0
    virtual int vf2C(); // 0x80023DEC
    virtual int vf40(); // 0x80023FAC
};
class AI_em_monster_attack01 : public CharaAi {
public:
    AI_em_monster_attack01();
    virtual int vf08(); // 0x800240F0
    virtual int vf0C(); // 0x80024540
    virtual int vf1C(); // 0x80024110
    virtual int vf20(); // 0x8002433C
    virtual int vf28(); // 0x80024254
    virtual int vf40(); // 0x800244B0
};
class AI_em_scout : public CharaAi {
public:
    AI_em_scout();
    static int fn_80024BC0(); // 0x80024BC0
    virtual int vf08(); // 0x80024718
    virtual int vf0C(); // 0x80024608
    virtual int vf14(); // 0x80024A5C
    virtual int vf1C(); // 0x80024B50
    virtual int vf24(); // 0x800248CC
    virtual int vf28(); // 0x800249EC
    virtual int vf2C(); // 0x80024824
    virtual int vf40(); // 0x80024760
};
class AI_em_sword_attack01 : public CharaAi {
public:
    AI_em_sword_attack01();
    virtual int vf08(); // 0x80024D6C
    virtual int vf0C(); // 0x80025478
    virtual int vf18(); // 0x80024E64
    virtual int vf1C(); // 0x80024F9C
    virtual int vf20(); // 0x800251B4
    virtual int vf24(); // 0x80024DCC
    virtual int vf28(); // 0x800250CC
    virtual int vf40(); // 0x800253E8
};
class AI_em_sword_attack02 : public CharaAi {
public:
    AI_em_sword_attack02();
    virtual int vf08(); // 0x8002552C
    virtual int vf0C(); // 0x80025D2C
    virtual int vf18(); // 0x80025624
    virtual int vf1C(); // 0x8002575C
    virtual int vf20(); // 0x80025A68
    virtual int vf24(); // 0x8002558C
    virtual int vf28(); // 0x80025980
    virtual int vf40(); // 0x80025C9C
};
class AI_em_sword_pointguard01 : public CharaAi {
public:
    AI_em_sword_pointguard01();
    virtual int vf08(); // 0x80025DE0
    virtual int vf0C(); // 0x80026570
    virtual int vf1C(); // 0x80025E30
    virtual int vf20(); // 0x80026310
    virtual int vf28(); // 0x800261C0
    virtual int vf40(); // 0x800264E0
};
class AI_em_type003 : public CharaAi {
public:
    AI_em_type003();
    virtual int vf08(); // 0x80026A40
    virtual int vf0C(); // 0x80026D78
    virtual int vf1C(); // 0x80026AB8
    virtual int vf28(); // 0x80026BEC
    virtual int vf40(); // 0x80026CD4
};
class AI_em_type010 : public CharaAi {
public:
    AI_em_type010();
    virtual int vf08(); // 0x80026E94
    virtual int vf0C(); // 0x80026F30
    virtual int vf1C(); // 0x800270B8
    virtual int vf28(); // 0x800271EC
    virtual int vf40(); // 0x800272D4
};
class AI_em_type102 : public CharaAi {
public:
    AI_em_type102();
    virtual int vf08(); // 0x80027444
    virtual int vf0C(); // 0x800274F0
    virtual int vf18(); // 0x8002765C
    virtual int vf24(); // 0x800276D4
    virtual int vf2C(); // 0x80027458
    virtual int vf40(); // 0x800275CC
};
class AI_em_type103 : public CharaAi {
public:
    AI_em_type103();
    virtual int vf08(); // 0x80028178
    virtual int vf0C(); // 0x8002779C
    virtual int vf18(); // 0x80027954
    virtual int vf1C(); // 0x80027E54
    virtual int vf20(); // 0x80027BB0
    virtual int vf24(); // 0x8002806C
    virtual int vf28(); // 0x80027F84
    virtual int vf2C(); // 0x80027704
    virtual int vf40(); // 0x80027B2C
};
class AI_em_type104 : public CharaAi {
public:
    AI_em_type104();
    virtual int vf08(); // 0x80028CD8
    virtual int vf0C(); // 0x8002836C
    virtual int vf18(); // 0x800284B4
    virtual int vf1C(); // 0x800289B4
    virtual int vf20(); // 0x80028710
    virtual int vf24(); // 0x80028BCC
    virtual int vf28(); // 0x80028AE4
    virtual int vf2C(); // 0x800282D4
    virtual int vf40(); // 0x8002868C
};
class AI_em_type105 : public CharaAi {
public:
    AI_em_type105();
    virtual int vf08(); // 0x80028E34
    virtual int vf0C(); // 0x800292B0
    virtual int vf18(); // 0x80028F3C
    virtual int vf20(); // 0x80028E64
    virtual int vf24(); // 0x80029018
    virtual int vf2C(); // 0x80029424
    virtual int vf40(); // 0x8002922C
};
class AI_em_type106 : public CharaAi {
public:
    AI_em_type106();
    virtual int vf08(); // 0x8002973C
    virtual int vf0C(); // 0x8002962C
    virtual int vf14(); // 0x80029A84
    virtual int vf1C(); // 0x80029AFC
    virtual int vf24(); // 0x800298F0
    virtual int vf28(); // 0x80029A14
    virtual int vf2C(); // 0x80029848
    virtual int vf40(); // 0x80029784
};
class AI_fr_archer_pick01 : public CharaAi {
public:
    AI_fr_archer_pick01();
    static int fn_8002AD40(); // 0x8002AD40
    virtual int vf08(); // 0x80029D18
    virtual int vf0C(); // 0x8002A774
    virtual int vf18(); // 0x8002A1FC
    virtual int vf1C(); // 0x8002A968
    virtual int vf20(); // 0x80029D7C
    virtual int vf24(); // 0x8002A42C
    virtual int vf28(); // 0x8002AB7C
    virtual int vf2C(); // 0x8002AC64
    virtual int vf40(); // 0x8002A6F0
};
class AI_fr_buffer_pick01 : public CharaAi {
public:
    AI_fr_buffer_pick01();
    virtual int vf08(); // 0x8002AE78
    virtual int vf0C(); // 0x8002AE98
    virtual int vf18(); // 0x8002B358
    virtual int vf20(); // 0x8002B53C
    virtual int vf24(); // 0x8002B434
    virtual int vf2C(); // 0x8002B4C8
    virtual int vf40(); // 0x8002B788
};
class AI_fr_follow_player : public CharaAi {
public:
    AI_fr_follow_player();
    virtual int vf08(); // 0x8002B818
    virtual int vf14(); // 0x8002B88C
    virtual int vf20(); // 0x8002B8FC
};
class AI_fr_magic_pick01 : public CharaAi {
public:
    AI_fr_magic_pick01();
    virtual int vf08(); // 0x8002B9D0
    virtual int vf0C(); // 0x8002BA24
    virtual int vf18(); // 0x8002BBEC
    virtual int vf20(); // 0x8002C00C
    virtual int vf24(); // 0x8002BCC8
    virtual int vf2C(); // 0x8002BF30
    virtual int vf40(); // 0x8002C260
};
class AI_fr_magic_pick02 : public CharaAi {
public:
    AI_fr_magic_pick02();
    virtual int vf08(); // 0x8002C2F0
    virtual int vf0C(); // 0x8002C354
    virtual int vf18(); // 0x8002CC58
    virtual int vf20(); // 0x8002C7D8
    virtual int vf24(); // 0x8002CD34
    virtual int vf2C(); // 0x8002C680
    virtual int vf40(); // 0x8002D108
};
class AI_fr_runaway : public CharaAi {
public:
    AI_fr_runaway();
    virtual int vf08(); // 0x8002D36C
    virtual int vf0C(); // 0x8002D294
    virtual int vf14(); // 0x8002D554
    virtual int vf18(); // 0x8002D648
    virtual int vf24(); // 0x8002D474
    virtual int vf40(); // 0x8002D3B0
};
class AI_fr_sword_attack01 : public CharaAi {
public:
    AI_fr_sword_attack01();
    virtual int vf08(); // 0x8002D704
    virtual int vf0C(); // 0x8002DE6C
    virtual int vf18(); // 0x8002D7F4
    virtual int vf1C(); // 0x8002D960
    virtual int vf20(); // 0x8002DB78
    virtual int vf24(); // 0x8002D75C
    virtual int vf28(); // 0x8002DA90
    virtual int vf40(); // 0x8002DDDC
};
class AI_fr_sword_pick01 : public CharaAi {
public:
    AI_fr_sword_pick01();
    virtual int vf08(); // 0x8002DF20
    virtual int vf0C(); // 0x8002E93C
    virtual int vf18(); // 0x8002E0FC
    virtual int vf1C(); // 0x8002E268
    virtual int vf20(); // 0x8002E564
    virtual int vf24(); // 0x8002DF78
    virtual int vf28(); // 0x8002E47C
    virtual int vf40(); // 0x8002E8AC
};
class AI_fr_sword_pointguard : public CharaAi {
public:
    AI_fr_sword_pointguard();
    virtual int vf08(); // 0x8002EB20
    virtual int vf0C(); // 0x8002EE58
    virtual int vf1C(); // 0x8002EB98
    virtual int vf28(); // 0x8002ECCC
    virtual int vf40(); // 0x8002EDB4
};
class AI_fr_sword_pointguard01 : public CharaAi {
public:
    AI_fr_sword_pointguard01();
    virtual int vf08(); // 0x8002EF74
    virtual int vf0C(); // 0x8002F704
    virtual int vf1C(); // 0x8002EFC4
    virtual int vf20(); // 0x8002F4A4
    virtual int vf28(); // 0x8002F354
    virtual int vf40(); // 0x8002F674
};
class AI_fr_type005 : public CharaAi {
public:
    AI_fr_type005();
    virtual int vf08(); // 0x8002FBD4
    virtual int vf0C(); // 0x8002FC40
    virtual int vf1C(); // 0x8002FECC
    virtual int vf28(); // 0x8002FD54
    virtual int vf40(); // 0x8002FFE0
};
class AI_fr_warppointescape : public CharaAi {
public:
    AI_fr_warppointescape();
    virtual int vf08(); // 0x800300CC
    virtual int vf0C(); // 0x8003002C
    virtual int vf14(); // 0x80030220
    virtual int vf18(); // 0x800303C0
    virtual int vf20(); // 0x800302A0
    virtual int vf24(); // 0x800302F0
    virtual int vf40(); // 0x8003015C
};
class AI_jackal01 : public CharaAi {
public:
    AI_jackal01();
    virtual int vf08(); // 0x8003068C
    virtual int vf0C(); // 0x80031004
    virtual int vf18(); // 0x80030AFC
    virtual int vf1C(); // 0x80031178
    virtual int vf20(); // 0x80030700
    virtual int vf24(); // 0x80030D2C
    virtual int vf28(); // 0x800312EC
    virtual int vf2C(); // 0x800313D4
    virtual int vf40(); // 0x80030F80
};
class AI_magic_attack01 : public CharaAi {
public:
    AI_magic_attack01();
    virtual int vf08(); // 0x80031758
    virtual int vf0C(); // 0x80031688
    virtual int vf18(); // 0x80031C04
    virtual int vf20(); // 0x800317AC
    virtual int vf24(); // 0x800319AC
    virtual int vf2C(); // 0x80031B28
    virtual int vf40(); // 0x8003191C
};
class AI_magic_attack01_2 : public CharaAi {
public:
    AI_magic_attack01_2();
    virtual int vf08(); // 0x80031DE4
    virtual int vf0C(); // 0x80031CE0
    virtual int vf18(); // 0x80032348
    virtual int vf20(); // 0x80031E38
    virtual int vf24(); // 0x80032090
    virtual int vf2C(); // 0x8003226C
    virtual int vf40(); // 0x80032000
};
class AI_magic_attack02 : public CharaAi {
public:
    AI_magic_attack02();
    virtual int vf08(); // 0x80032424
    virtual int vf0C(); // 0x80032470
    virtual int vf18(); // 0x800328D8
    virtual int vf20(); // 0x80032768
    virtual int vf24(); // 0x800329B4
    virtual int vf2C(); // 0x8003268C
    virtual int vf40(); // 0x80032D60
};
class AI_magic_attackstable01 : public CharaAi {
public:
    AI_magic_attackstable01();
    virtual int vf08(); // 0x80032F04
    virtual int vf0C(); // 0x80032DF0
    virtual int vf2C(); // 0x80032FA8
    virtual int vf40(); // 0x80032F18
};
class AI_magic_cure01 : public CharaAi {
public:
    AI_magic_cure01();
    virtual int vf08(); // 0x80033020
    virtual int vf0C(); // 0x80033040
    virtual int vf18(); // 0x80033418
    virtual int vf20(); // 0x800335FC
    virtual int vf24(); // 0x800334F4
    virtual int vf2C(); // 0x80033588
    virtual int vf40(); // 0x80033764
};
class AI_magic_guard01 : public CharaAi {
public:
    AI_magic_guard01();
    virtual int vf08(); // 0x800337F4
    virtual int vf0C(); // 0x80033854
    virtual int vf18(); // 0x80033978
    virtual int vf20(); // 0x80033CF4
    virtual int vf24(); // 0x80033A54
    virtual int vf2C(); // 0x80033C18
    virtual int vf40(); // 0x80033EA8
};
class AI_magic_guard01_2 : public CharaAi {
public:
    AI_magic_guard01_2();
    virtual int vf08(); // 0x80033F38
    virtual int vf0C(); // 0x80033F98
    virtual int vf18(); // 0x800340F0
    virtual int vf20(); // 0x800344C4
    virtual int vf24(); // 0x800341CC
    virtual int vf2C(); // 0x800343E8
    virtual int vf40(); // 0x800346D0
};
class AI_magic_guard02 : public CharaAi {
public:
    AI_magic_guard02();
    virtual int vf08(); // 0x80034760
    virtual int vf0C(); // 0x800347C0
    virtual int vf18(); // 0x80034A44
    virtual int vf20(); // 0x80035030
    virtual int vf24(); // 0x80034B20
    virtual int vf2C(); // 0x80034F54
    virtual int vf40(); // 0x800351E4
};
class AI_np_approach : public CharaAi {
public:
    AI_np_approach();
    virtual int vf08(); // 0x80035274
    virtual int vf0C(); // 0x800352E8
    virtual int vf14(); // 0x80035440
    virtual int vf20(); // 0x800354C4
    virtual int vf34(); // 0x800355FC
};
class AI_np_approach_plus : public CharaAi {
public:
    AI_np_approach_plus();
    virtual int vf08(); // 0x8003574C
    virtual int vf0C(); // 0x800357BC
    virtual int vf14(); // 0x80035914
    virtual int vf20(); // 0x80035998
    virtual int vf34(); // 0x80035AD0
};
class AI_np_follow_player : public CharaAi {
public:
    AI_np_follow_player();
    virtual int vf08(); // 0x80035C20
    virtual int vf14(); // 0x80035C94
    virtual int vf20(); // 0x80035D04
};
class AI_np_tagger_random : public CharaAi {
public:
    AI_np_tagger_random();
    virtual int vf08(); // 0x80035D18
    virtual int vf0C(); // 0x80035D2C
    virtual int vf14(); // 0x80035EB4
};
class AI_np_type001 : public CharaAi {
public:
    AI_np_type001();
    virtual int vf08(); // 0x80035EE4
    virtual int vf0C(); // 0x80036048
    virtual int vf14(); // 0x80035FD0
};
class AI_np_type002 : public CharaAi {
public:
    AI_np_type002();
    virtual int vf08(); // 0x80036148
    virtual int vf0C(); // 0x800361C0
    virtual int vf14(); // 0x800362C0
    virtual int vf20(); // 0x80036580
};
class AI_np_type003 : public CharaAi {
public:
    AI_np_type003();
    virtual int vf08(); // 0x80036600
    virtual int vf0C(); // 0x80036688
    virtual int vf14(); // 0x80036788
    virtual int vf20(); // 0x80036A08
};
class AI_np_type004 : public CharaAi {
public:
    AI_np_type004();
    virtual int vf08(); // 0x80036A88
    virtual int vf14(); // 0x80036AA8
    virtual int vf20(); // 0x80036B20
};
class AI_np_type005 : public CharaAi {
public:
    AI_np_type005();
    virtual int vf08(); // 0x80036C94
    virtual int vf14(); // 0x80036B34
    virtual int vf20(); // 0x80036BE8
};
class AI_np_wait_reaction : public CharaAi {
public:
    AI_np_wait_reaction();
    virtual int vf08(); // 0x80036CC4
    virtual int vf0C(); // 0x80036CF8
    virtual int vf34(); // 0x80036E3C
};
class AI_np_wait_reaction_minus : public CharaAi {
public:
    AI_np_wait_reaction_minus();
    virtual int vf08(); // 0x80036F8C
    virtual int vf0C(); // 0x80036FCC
    virtual int vf34(); // 0x800370D4
};
class AI_np_wait_reaction_minus2 : public CharaAi {
public:
    AI_np_wait_reaction_minus2();
    virtual int vf08(); // 0x800371E0
    virtual int vf0C(); // 0x80037220
    virtual int vf34(); // 0x80037364
};
class AI_np_wait_reaction_plus : public CharaAi {
public:
    AI_np_wait_reaction_plus();
    virtual int vf08(); // 0x800374E0
    virtual int vf0C(); // 0x80037520
    virtual int vf34(); // 0x80037664
};
class AI_quark01 : public CharaAi {
public:
    AI_quark01();
    virtual int vf08(); // 0x800377AC
    virtual int vf0C(); // 0x8003806C
    virtual int vf18(); // 0x80037944
    virtual int vf1C(); // 0x80037AB0
    virtual int vf20(); // 0x80037D20
    virtual int vf24(); // 0x80037854
    virtual int vf28(); // 0x80037C38
    virtual int vf38(); // 0x8003818C
    virtual int vf3C(); // 0x80038338
    virtual int vf40(); // 0x80037FDC
};
class AI_quark02 : public CharaAi {
public:
    AI_quark02();
    virtual int vf08(); // 0x80038674
    virtual int vf0C(); // 0x80038750
    virtual int vf40(); // 0x800386C0
};
class AI_silence_guard01 : public CharaAi {
public:
    AI_silence_guard01();
    virtual int vf08(); // 0x800388C4
    virtual int vf0C(); // 0x80038924
    virtual int vf18(); // 0x80038A30
    virtual int vf1C(); // 0x80038CA8
    virtual int vf24(); // 0x80038B0C
    virtual int vf40(); // 0x80038C18
};
class AI_sword_guard01 : public CharaAi {
public:
    AI_sword_guard01();
    virtual int vf08(); // 0x80038D90
    virtual int vf0C(); // 0x80039624
    virtual int vf18(); // 0x80038ED4
    virtual int vf1C(); // 0x80039040
    virtual int vf20(); // 0x800392C8
    virtual int vf24(); // 0x80038DF8
    virtual int vf28(); // 0x800391E0
    virtual int vf38(); // 0x8003975C
    virtual int vf40(); // 0x80039594
};
class AI_sword_leaderguard01 : public CharaAi {
public:
    AI_sword_leaderguard01();
    virtual int vf08(); // 0x80039908
    virtual int vf0C(); // 0x8003A0D4
    virtual int vf1C(); // 0x80039950
    virtual int vf20(); // 0x80039E74
    virtual int vf28(); // 0x80039D24
    virtual int vf40(); // 0x8003A044
};
class AI_sword_leaderguard02 : public CharaAi {
public:
    AI_sword_leaderguard02();
    virtual int vf08(); // 0x8003A580
    virtual int vf0C(); // 0x8003AD38
    virtual int vf1C(); // 0x8003A5C8
    virtual int vf20(); // 0x8003AAD8
    virtual int vf28(); // 0x8003A988
    virtual int vf40(); // 0x8003ACA8
};
class AI_sword_leaderguard03 : public CharaAi {
public:
    AI_sword_leaderguard03();
    virtual int vf08(); // 0x8003B2A4
    virtual int vf0C(); // 0x8003BA30
    virtual int vf1C(); // 0x8003B2F4
    virtual int vf20(); // 0x8003B7D0
    virtual int vf28(); // 0x8003B680
    virtual int vf40(); // 0x8003B9A0
};
class AI_sword_pointguard01 : public CharaAi {
public:
    AI_sword_pointguard01();
    virtual int vf08(); // 0x8003C074
    virtual int vf0C(); // 0x8003C804
    virtual int vf1C(); // 0x8003C0C4
    virtual int vf20(); // 0x8003C5A4
    virtual int vf28(); // 0x8003C454
    virtual int vf40(); // 0x8003C774
};
class AI_return_guard01 : public CharaAi {
public:
    AI_return_guard01();
    virtual int vf08(); // 0x8003CBE0
    virtual int vf0C(); // 0x8003CCD4
    virtual int vf3C(); // 0x8003CDA8
    virtual int vf40(); // 0x8003CC44
};
} // namespace Script
} // namespace AI

// Script primitives. The script steps are built from these inline helpers (inferred from the code:
// every step repeats the same instruction sequences, so they were inlined from common functions).

// inferred: random value in [center - width / 2, center + width / 2)
static inline int RandRange_unk(int center, int width) {
    int lo = center - width / 2;
    int range = center + width / 2 - lo;
    if (range > 0) {
        lo += rand() % range;
    }
    return lo;
}

// inferred: issues a command to the character (fn_8001AEDC takes its four arguments by address)
static inline void Command_unk(int mode, int a, int b, int c, int d) {
    int a_;
    int b_;
    int c_;
    int d_;
    d_ = d;
    c_ = c;
    b_ = b;
    a_ = a;
    fn_8001AEDC(mode, &a_, &b_, &c_, &d_);
}

// inferred: 1 (and out.m00 set) while out.m10 is still 0
static inline int CheckCount_unk() {
    if (lbl_807C6A40.m10 >= 1) {
        return -1;
    }
    lbl_807C6A40.m00 = 1;
    return 1;
}

// inferred: attacks the target (in.m20) after a random delay around in.m50 (in.m54 wide)
static inline int AttackRandom_unk() {
    if (lbl_807C68C0.m08 != 0) {
        Command_unk(4, (int)lbl_807C68C0.m20, RandRange_unk(lbl_807C68C0.m50, lbl_807C68C0.m54), 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

// inferred: random attack delay around in.m50 (in.m54 wide)
static inline int AttackDelay_unk() {
    return RandRange_unk(lbl_807C68C0.m50, lbl_807C68C0.m54);
}

// inferred: random wait time around in.m48 (in.m4C wide)
static inline int WaitDelay_unk() {
    return RandRange_unk(lbl_807C68C0.m48, lbl_807C68C0.m4C);
}

// inferred: attacks the target if it is still valid (fn_8001BE00), otherwise stops
static inline int AttackValid_unk() {
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m08 != 0) {
            Command_unk(4, (int)lbl_807C68C0.m20, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 5;
        }
        return -1;
    }
    Command_unk(0, 0, 0, 0, 0);
    lbl_807C6A40.m00 = 1;
    return 1;
}

// inferred: command 6 on a valid target (returns 9), otherwise stops
static inline int Special_unk() {
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m08 != 0) {
            Command_unk(6, 1, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 9;
        }
        return -1;
    }
    Command_unk(0, 0, 0, 0, 0);
    lbl_807C6A40.m00 = 1;
    return 1;
}

// inferred: in.m1C (distance to the target) below range
static inline bool InRange_unk(float range) {
    return lbl_807C68C0.m1C < range;
}
// inferred: picks an item to use (stored in in.m64) when the target is in range
static inline bool CanUseItem_unk() {
    if (InRange_unk(9999.0f)) {
        int item = fn_8001B634(lbl_807C68C0.m00);
        if (item > 0) {
            lbl_807C68C0.m64 = item;
            return true;
        }
    }
    return false;
}

// inferred: when the target is out of range, approaches it (command 3, returns 6), otherwise
// command 6 (returns 9)
static inline int ApproachOrSpecial_unk() {
    if (lbl_807C68C0.m08 != 0) {
        if (lbl_807C68C0.m1C > fn_8001BD14(lbl_807C68C0.m00)) {
            if (lbl_807C68C0.m20 != 0) {
                Command_unk(3, (int)lbl_807C68C0.m20, 30, 0, 0);
            }
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        Command_unk(6, 1, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 9;
    }
    return -1;
}

// inferred: 6 (and out.m00 set) when in.m08 is set
static inline int Ready_unk() {
    if (lbl_807C68C0.m08 != 0) {
        lbl_807C6A40.m00 = 1;
        return 6;
    }
    return -1;
}

// inferred: 6 when the target is farther than twice in.m30, otherwise command 5 (returns 8)
static inline int FarOrSpecial_unk() {
    if (lbl_807C68C0.m1C > lbl_807C68C0.m30 * 2) {
        lbl_807C6A40.m00 = 1;
        return 6;
    }
    if (lbl_807C68C0.m08 != 0) {
        Command_unk(5, 1, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    return -1;
}

// inferred: stops (command 0) when in.m08 is set
static inline int Stop_unk() {
    if (lbl_807C68C0.m08 != 0) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// inferred: as CanUseItem_unk with the other item selection (fn_8001B5DC)
static inline bool CanUseItem2_unk() {
    if (InRange_unk(9999.0f)) {
        int item = fn_8001B5DC(lbl_807C68C0.m00);
        if (item > 0) {
            lbl_807C68C0.m64 = item;
            return true;
        }
    }
    return false;
}

// inferred: turns towards out.m24 (command 12) once, then returns 3
static inline int Turn_unk() {
    Command_unk(12, lbl_807C6A40.m24, 20, 0, 0);
    lbl_807C6A40.m28 = 1;
    lbl_807C6A40.m00 = 1;
    return 3;
}

// inferred: command 6 on out.m18 when the target is in range (returns 4)
static inline int ShootInRange_unk() {
    if (lbl_807C68C0.m08 != 0) {
        if (InRange_unk(9999.0f)) {
            Command_unk(6, lbl_807C6A40.m18, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}

// inferred: distance between two characters (0 when one is missing)
static inline int Distance_unk(CharaTask* a, CharaTask* b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    return fn_8001AD80(a, b);
}

// inferred: route point near base (within 120, then anywhere), base itself when none is found
static inline int PickPoint_unk(int base) {
    int point;
    if ((point = fn_8001BA88(lbl_807C68C0.m00, base, 1, -1, 120.0f, 100.0f, 9999.0f)) <= 0 &&
        (point = fn_8001BA88(lbl_807C68C0.m00, base, 1, -1, -1.0f, 0.0f, 9999.0f)) <= 0) {
        point = base;
    }
    return point;
}

// inferred: waits a random time around in.m48 (in.m4C wide) when in.m08 is set
static inline int WaitRandom_unk() {
    if (lbl_807C68C0.m08 != 0) {
        Command_unk(0, RandRange_unk(lbl_807C68C0.m48, lbl_807C68C0.m4C), 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

// inferred: attacks the target when it is closer than in.m30 (in.m1C: distance), otherwise waits
// inferred: Command_unk(0) through one more inline level: used where the random wait is nested in
// another step (the inline depth decides the stack order of the command arguments)
static inline void Wait_unk(int frames) {
    Command_unk(0, frames, 0, 0, 0);
}

// inferred: same as WaitRandom_unk with the command through Wait_unk
static inline int WaitRandom2_unk() {
    if (lbl_807C68C0.m08 != 0) {
        Wait_unk(RandRange_unk(lbl_807C68C0.m48, lbl_807C68C0.m4C));
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

static inline int AttackNear_unk() {
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return WaitRandom2_unk();
}

extern "C" unsigned char* fn_8001C088();
extern "C" unsigned char* fn_8001C1C0(unsigned char* script);

// inferred: this global sentinel is registered with the destructor for its three linked buckets.
extern "C" void fn_8003E6B8(void* object, int freeObject);
extern "C" void fn_8003E688();

// inferred: three-word MSL string header; the high bit selects the heap-buffer representation.
struct AIString_unk {
    unsigned int mFlags;
    unsigned int mSize;
    char* mData;
};

// inferred: the registry uses an rb-tree node header followed by a string key at +0xC.
struct AIScriptMapNode_unk {
    AIScriptMapNode_unk* mLeft;
    AIScriptMapNode_unk* mRight;
    AIScriptMapNode_unk* mParent;
    AIString_unk mKey;
};

extern "C" int memcmp(const void*, const void*, unsigned long);
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);

void operator delete(void* ptr);
extern "C" void* fn_8001E168(AIString_unk* str, int freeObject);

// inferred: 0x8001E0B8 copies the 12-byte string followed by the allocator word.
struct AIStringWithAllocator_unk {
    AIString_unk mString;
    void* mAllocator;
};
extern "C" AIStringWithAllocator_unk* fn_8001E0B8(AIStringWithAllocator_unk* dst, const AIStringWithAllocator_unk* src, const unsigned int* allocator);

extern "C" AIScriptMapNode_unk* fn_8003DD50(void* tree,
                                             AIScriptMapNode_unk* candidate,
                                             unsigned char insertLeft,
                                             unsigned char insertFirst,
                                             const AIStringWithAllocator_unk* key);
extern "C" void fn_8001E1D0(void* result, void* tree, const AIStringWithAllocator_unk* value);
extern "C" void fn_8001C068(CharaTask* chara, const char* name);

// NONMATCHING: 0.00% - the inferred copy loops compile to a different routine shape.
// inferred: copies the script's fixed parameter blocks into the global AI state.
extern "C" unsigned char* fn_8001C088() {
    unsigned char* src = (unsigned char*)fn_8001C260();
    unsigned char* dst = (unsigned char*)fn_8001C26C();

    *(int*)dst = 0;
    *(int*)(dst + 4) = *(int*)(src + 4);
    *(int*)(dst + 8) = *(int*)(src + 0x64);
    *(int*)(dst + 0xC) = *(int*)(src + 0x68);
    *(int*)(dst + 0x10) = *(int*)(src + 0x6C);
    for (int i = 0; i < 5; i++) {
        *(int*)(dst + 0x14 + i * 0x14) = *(int*)(src + 0xA4 + i * 0x14);
        *(int*)(dst + 0x18 + i * 0x14) = *(int*)(src + 0xA8 + i * 0x14);
        *(int*)(dst + 0x1C + i * 0x14) = *(int*)(src + 0xAC + i * 0x14);
        *(int*)(dst + 0x20 + i * 0x14) = *(int*)(src + 0xB0 + i * 0x14);
        *(int*)(dst + 0x24 + i * 0x14) = *(int*)(src + 0xB4 + i * 0x14);
    }
    for (int i = 0; i < 5; i++) {
        *(int*)(dst + 0x78 + i * 0x14) = *(int*)(src + 0x108 + i * 0x14);
        *(int*)(dst + 0x7C + i * 0x14) = *(int*)(src + 0x10C + i * 0x14);
        *(int*)(dst + 0x80 + i * 0x14) = *(int*)(src + 0x110 + i * 0x14);
        *(int*)(dst + 0x84 + i * 0x14) = *(int*)(src + 0x114 + i * 0x14);
        *(int*)(dst + 0x88 + i * 0x14) = *(int*)(src + 0x118 + i * 0x14);
    }
    for (int i = 0; i < 5; i++) {
        *(int*)(dst + 0xDC + i * 4) = *(int*)(src + 0x16C + i * 4);
    }

    unsigned char* tail = (unsigned char*)fn_8001C278();
    CharaManager<PlayerTask>* manager = lbl_8087F8A0;
    *(int*)tail = manager != 0 ? *(int*)((unsigned char*)manager + 0x48) : 0;
    *(int*)(tail + 4) = 0;
    return tail;
}

extern "C" void fn_8001C1B4() {
    fn_8001C284();
}

extern "C" void fn_8001C1B8() {
    fn_8001E730();
}

extern "C" void fn_8001C1BC() {}

// NONMATCHING: 80.53% - MWCC lowers the global state pointers through different code than the target.
// inferred: applies the selected script's two indexed parameter values to global state.
extern "C" unsigned char* fn_8001C1C0(unsigned char* script) {
    fn_8001C088();
    unsigned char* result = (unsigned char*)fn_8001EA18(script);
    if (result != 0) {
        AiInput_unk* src = &lbl_807C68C0;
        AiOutput_unk* dst = &lbl_807C6A40;
        if (src->m04 != 0) {
            int value = fn_8001ECD0(result, 0xE);
            if (value >= 0) {
                src->m04 = value;
                dst->m04 = value;
            }
        }
        int value = fn_8001ECD0(result, src->m04);
        if (value >= 0) {
            dst->m04 = value;
        }
    }
    return result;
}
extern "C" void fn_8001C284();

extern "C" AiInput_unk* fn_8001C260() {
    return &lbl_807C68C0;
}

extern "C" AiOutput_unk* fn_8001C26C() {
    return &lbl_807C6A40;
}

extern "C" AiTargets_unk* fn_8001C278() {
    return &lbl_807C6B30;
}

// NONMATCHING: 92.27% - registry population is reconstructed; local layout and call scheduling differ.
// inferred: registers each script name with a heap-3 instance in the global AI script tree.
extern "C" void fn_8001C284() {
    unsigned char pairs[64][8];
    AIStringWithAllocator_unk values[64];
    void* objects[64];
#define REGISTER_AI_SCRIPT(I, NAME, CLASS) do { \
        std::string key(NAME); \
        objects[I] = new (3, "", "", 0) AI::Script::CLASS(); \
        fn_8001E0B8(&values[I], (const AIStringWithAllocator_unk*)&key, (const unsigned int*)&objects[I]); \
        fn_8001E1D0(pairs[I], lbl_807C6B84, &values[I]); \
        fn_8001E168(&values[I].mString, -1); \
    } while (0)
    REGISTER_AI_SCRIPT(0, "archer_attack01", AI_archer_attack01);
    REGISTER_AI_SCRIPT(1, "archer_guard01", AI_archer_guard01);
    REGISTER_AI_SCRIPT(2, "arganan01", AI_arganan01);
    REGISTER_AI_SCRIPT(3, "buffer_attackstable01", AI_buffer_attackstable01);
    REGISTER_AI_SCRIPT(4, "buffer_guard01", AI_buffer_guard01);
    REGISTER_AI_SCRIPT(5, "buffer_guard02", AI_buffer_guard02);
    REGISTER_AI_SCRIPT(6, "buffer_pointguard01", AI_buffer_pointguard01);
    REGISTER_AI_SCRIPT(7, "em_archer_attack02", AI_em_archer_attack02);
    REGISTER_AI_SCRIPT(8, "em_bridge_archer", AI_em_bridge_archer);
    REGISTER_AI_SCRIPT(9, "em_monster_attack01", AI_em_monster_attack01);
    REGISTER_AI_SCRIPT(10, "em_scout", AI_em_scout);
    REGISTER_AI_SCRIPT(11, "em_sword_attack01", AI_em_sword_attack01);
    REGISTER_AI_SCRIPT(12, "em_sword_attack02", AI_em_sword_attack02);
    REGISTER_AI_SCRIPT(13, "em_sword_pointguard01", AI_em_sword_pointguard01);
    REGISTER_AI_SCRIPT(14, "em_type003", AI_em_type003);
    REGISTER_AI_SCRIPT(15, "em_type010", AI_em_type010);
    REGISTER_AI_SCRIPT(16, "em_type102", AI_em_type102);
    REGISTER_AI_SCRIPT(17, "em_type103", AI_em_type103);
    REGISTER_AI_SCRIPT(18, "em_type104", AI_em_type104);
    REGISTER_AI_SCRIPT(19, "em_type105", AI_em_type105);
    REGISTER_AI_SCRIPT(20, "em_type106", AI_em_type106);
    REGISTER_AI_SCRIPT(21, "fr_archer_pick01", AI_fr_archer_pick01);
    REGISTER_AI_SCRIPT(22, "fr_buffer_pick01", AI_fr_buffer_pick01);
    REGISTER_AI_SCRIPT(23, "fr_follow_player", AI_fr_follow_player);
    REGISTER_AI_SCRIPT(24, "fr_magic_pick01", AI_fr_magic_pick01);
    REGISTER_AI_SCRIPT(25, "fr_magic_pick02", AI_fr_magic_pick02);
    REGISTER_AI_SCRIPT(26, "fr_runaway", AI_fr_runaway);
    REGISTER_AI_SCRIPT(27, "fr_sword_attack01", AI_fr_sword_attack01);
    REGISTER_AI_SCRIPT(28, "fr_sword_pick01", AI_fr_sword_pick01);
    REGISTER_AI_SCRIPT(29, "fr_sword_pointguard", AI_fr_sword_pointguard);
    REGISTER_AI_SCRIPT(30, "fr_sword_pointguard01", AI_fr_sword_pointguard01);
    REGISTER_AI_SCRIPT(31, "fr_type005", AI_fr_type005);
    REGISTER_AI_SCRIPT(32, "fr_warppointescape", AI_fr_warppointescape);
    REGISTER_AI_SCRIPT(33, "jackal01", AI_jackal01);
    REGISTER_AI_SCRIPT(34, "magic_attack01", AI_magic_attack01);
    REGISTER_AI_SCRIPT(35, "magic_attack01_2", AI_magic_attack01_2);
    REGISTER_AI_SCRIPT(36, "magic_attack02", AI_magic_attack02);
    REGISTER_AI_SCRIPT(37, "magic_attackstable01", AI_magic_attackstable01);
    REGISTER_AI_SCRIPT(38, "magic_cure01", AI_magic_cure01);
    REGISTER_AI_SCRIPT(39, "magic_guard01", AI_magic_guard01);
    REGISTER_AI_SCRIPT(40, "magic_guard01_2", AI_magic_guard01_2);
    REGISTER_AI_SCRIPT(41, "magic_guard02", AI_magic_guard02);
    REGISTER_AI_SCRIPT(42, "np_approach", AI_np_approach);
    REGISTER_AI_SCRIPT(43, "np_approach_plus", AI_np_approach_plus);
    REGISTER_AI_SCRIPT(44, "np_follow_player", AI_np_follow_player);
    REGISTER_AI_SCRIPT(45, "np_tagger_random", AI_np_tagger_random);
    REGISTER_AI_SCRIPT(46, "np_type001", AI_np_type001);
    REGISTER_AI_SCRIPT(47, "np_type002", AI_np_type002);
    REGISTER_AI_SCRIPT(48, "np_type003", AI_np_type003);
    REGISTER_AI_SCRIPT(49, "np_type004", AI_np_type004);
    REGISTER_AI_SCRIPT(50, "np_type005", AI_np_type005);
    REGISTER_AI_SCRIPT(51, "np_wait_reaction", AI_np_wait_reaction);
    REGISTER_AI_SCRIPT(52, "np_wait_reaction_minus", AI_np_wait_reaction_minus);
    REGISTER_AI_SCRIPT(53, "np_wait_reaction_minus2", AI_np_wait_reaction_minus2);
    REGISTER_AI_SCRIPT(54, "np_wait_reaction_plus", AI_np_wait_reaction_plus);
    REGISTER_AI_SCRIPT(55, "quark01", AI_quark01);
    REGISTER_AI_SCRIPT(56, "quark02", AI_quark02);
    REGISTER_AI_SCRIPT(57, "silence_guard01", AI_silence_guard01);
    REGISTER_AI_SCRIPT(58, "sword_guard01", AI_sword_guard01);
    REGISTER_AI_SCRIPT(59, "sword_leaderguard01", AI_sword_leaderguard01);
    REGISTER_AI_SCRIPT(60, "sword_leaderguard02", AI_sword_leaderguard02);
    REGISTER_AI_SCRIPT(61, "sword_leaderguard03", AI_sword_leaderguard03);
    REGISTER_AI_SCRIPT(62, "sword_pointguard01", AI_sword_pointguard01);
    REGISTER_AI_SCRIPT(63, "return_guard01", AI_return_guard01);
#undef REGISTER_AI_SCRIPT
}

namespace AI {

namespace Script {

AI_archer_attack01::AI_archer_attack01() {} // 0x8001E0A8
} // namespace Script
} // namespace AI

extern "C" AIStringWithAllocator_unk* fn_8001E0B8(AIStringWithAllocator_unk* dst,
                                                   const AIStringWithAllocator_unk* src,
                                                   const unsigned int* allocator) {
    if ((src->mString.mFlags >> 31) == 0) {
        dst->mString = src->mString;
    } else {
        dst->mString.mFlags = 0;
        dst->mString.mSize = 0;
        dst->mString.mData = 0;
        std::string* str = (std::string*)&dst->mString;
        str->reserve(src->mString.mSize);
        std::random_access_iterator_tag tag;
        str->__replace_unk(0, 0, src->mString.mData,
                           src->mString.mData + src->mString.mSize,
                           tag);
    }
    dst->mAllocator = (void*)*allocator;
    return dst;
}

extern "C" void* fn_8001E168(AIString_unk* str, int freeObject) {
    if (str != 0) {
        if (str != 0) {
            if ((str->mFlags >> 31) != 0) {
                operator delete(str->mData);
            }
        }
        if (freeObject > 0) {
            operator delete(str);
        }
    }
    return str;
}

// NONMATCHING: 47.60% - the tree insertion helper still has a different stack and register layout.
// inferred: returns the inserted/existing node and a flag in the caller-provided 8-byte pair.
extern "C" void fn_8001E1D0(void* result, void* tree,
                             const AIStringWithAllocator_unk* value) {
    AIScriptMapNode_unk* parent = 0;
    unsigned char insertLeft = 1;
    unsigned char insertFirst = 1;
    AIScriptMapNode_unk* candidate = fn_8003E530(
        tree, &value->mString, &parent, &insertLeft, &insertFirst);

    bool insert = parent == 0;
    if (parent != 0) {
        const AIString_unk* key = &value->mString;
        const AIString_unk* existing = &parent->mKey;
        unsigned int keySize = (key->mFlags >> 31) != 0
                                   ? key->mSize
                                   : (*(const unsigned char*)key & 0x7F);
        const char* keyData = (key->mFlags >> 31) != 0
                                  ? key->mData
                                  : (const char*)key + 1;
        unsigned int existingSize = (existing->mFlags >> 31) != 0
                                        ? existing->mSize
                                        : (*(const unsigned char*)existing & 0x7F);
        const char* existingData = (existing->mFlags >> 31) != 0
                                       ? existing->mData
                                       : (const char*)existing + 1;
        unsigned int commonSize = keySize < existingSize ? keySize : existingSize;
        int cmp = memcmp(keyData, existingData, commonSize);
        if (cmp == 0) {
            cmp = keySize < existingSize ? -1 : (keySize == existingSize ? 0 : 1);
        }
        insert = cmp < 0;
    }

    AIScriptMapNode_unk* node;
    unsigned char flag;
    if (insert) {
        node = fn_8003DD50(tree, candidate, insertLeft, insertFirst, value);
        flag = lbl_8087D6F0;
    } else {
        node = parent;
        flag = lbl_8087D6F1;
    }
    *(AIScriptMapNode_unk**)result = node;
    *((unsigned char*)result + 4) = flag;
}

namespace AI {

namespace Script {

AI_archer_guard01::AI_archer_guard01() {} // 0x8001E340

AI_arganan01::AI_arganan01() {} // 0x8001E350

AI_buffer_attackstable01::AI_buffer_attackstable01() {} // 0x8001E360

AI_buffer_guard01::AI_buffer_guard01() {} // 0x8001E370

AI_buffer_guard02::AI_buffer_guard02() {} // 0x8001E380

AI_buffer_pointguard01::AI_buffer_pointguard01() {} // 0x8001E390

AI_em_archer_attack02::AI_em_archer_attack02() {} // 0x8001E3A0

AI_em_bridge_archer::AI_em_bridge_archer() {} // 0x8001E3B0

AI_em_monster_attack01::AI_em_monster_attack01() {} // 0x8001E3C0

AI_em_scout::AI_em_scout() {} // 0x8001E3D0

AI_em_sword_attack01::AI_em_sword_attack01() {} // 0x8001E3E0

AI_em_sword_attack02::AI_em_sword_attack02() {} // 0x8001E3F0

AI_em_sword_pointguard01::AI_em_sword_pointguard01() {} // 0x8001E400

AI_em_type003::AI_em_type003() {} // 0x8001E410

AI_em_type010::AI_em_type010() {} // 0x8001E420

AI_em_type102::AI_em_type102() {} // 0x8001E430

AI_em_type103::AI_em_type103() {} // 0x8001E440

AI_em_type104::AI_em_type104() {} // 0x8001E450

AI_em_type105::AI_em_type105() {} // 0x8001E460

AI_em_type106::AI_em_type106() {} // 0x8001E470

AI_fr_archer_pick01::AI_fr_archer_pick01() {} // 0x8001E480

AI_fr_buffer_pick01::AI_fr_buffer_pick01() {} // 0x8001E490

AI_fr_follow_player::AI_fr_follow_player() {} // 0x8001E4A0

AI_fr_magic_pick01::AI_fr_magic_pick01() {} // 0x8001E4B0

AI_fr_magic_pick02::AI_fr_magic_pick02() {} // 0x8001E4C0

AI_fr_runaway::AI_fr_runaway() {} // 0x8001E4D0

AI_fr_sword_attack01::AI_fr_sword_attack01() {} // 0x8001E4E0

AI_fr_sword_pick01::AI_fr_sword_pick01() {} // 0x8001E4F0

AI_fr_sword_pointguard::AI_fr_sword_pointguard() {} // 0x8001E500

AI_fr_sword_pointguard01::AI_fr_sword_pointguard01() {} // 0x8001E510

AI_fr_type005::AI_fr_type005() {} // 0x8001E520

AI_fr_warppointescape::AI_fr_warppointescape() {} // 0x8001E530

AI_jackal01::AI_jackal01() {} // 0x8001E540

AI_magic_attack01::AI_magic_attack01() {} // 0x8001E550

AI_magic_attack01_2::AI_magic_attack01_2() {} // 0x8001E560

AI_magic_attack02::AI_magic_attack02() {} // 0x8001E570

AI_magic_attackstable01::AI_magic_attackstable01() {} // 0x8001E580

AI_magic_cure01::AI_magic_cure01() {} // 0x8001E590

AI_magic_guard01::AI_magic_guard01() {} // 0x8001E5A0

AI_magic_guard01_2::AI_magic_guard01_2() {} // 0x8001E5B0

AI_magic_guard02::AI_magic_guard02() {} // 0x8001E5C0

AI_np_approach::AI_np_approach() {} // 0x8001E5D0

AI_np_approach_plus::AI_np_approach_plus() {} // 0x8001E5E0

AI_np_follow_player::AI_np_follow_player() {} // 0x8001E5F0

AI_np_tagger_random::AI_np_tagger_random() {} // 0x8001E600

AI_np_type001::AI_np_type001() {} // 0x8001E610

AI_np_type002::AI_np_type002() {} // 0x8001E620

AI_np_type003::AI_np_type003() {} // 0x8001E630

AI_np_type004::AI_np_type004() {} // 0x8001E640

AI_np_type005::AI_np_type005() {} // 0x8001E650

AI_np_wait_reaction::AI_np_wait_reaction() {} // 0x8001E660

AI_np_wait_reaction_minus::AI_np_wait_reaction_minus() {} // 0x8001E670

AI_np_wait_reaction_minus2::AI_np_wait_reaction_minus2() {} // 0x8001E680

AI_np_wait_reaction_plus::AI_np_wait_reaction_plus() {} // 0x8001E690

AI_quark01::AI_quark01() {} // 0x8001E6A0

AI_quark02::AI_quark02() {} // 0x8001E6B0

AI_silence_guard01::AI_silence_guard01() {} // 0x8001E6C0

AI_sword_guard01::AI_sword_guard01() {} // 0x8001E6D0

AI_sword_leaderguard01::AI_sword_leaderguard01() {} // 0x8001E6E0

AI_sword_leaderguard02::AI_sword_leaderguard02() {} // 0x8001E6F0

AI_sword_leaderguard03::AI_sword_leaderguard03() {} // 0x8001E700

AI_sword_pointguard01::AI_sword_pointguard01() {} // 0x8001E710

AI_return_guard01::AI_return_guard01() {} // 0x8001E720
} // namespace Script
} // namespace AI

int AI::Script::CharaAi::vf0C() { return -1; }

int AI::Script::CharaAi::vf10() { return -1; }

int AI::Script::CharaAi::vf14() { return -1; }

int AI::Script::CharaAi::vf18() { return -1; }

int AI::Script::CharaAi::vf1C() { return -1; }

int AI::Script::CharaAi::vf20() { return -1; }

int AI::Script::CharaAi::vf24() { return -1; }

int AI::Script::CharaAi::vf28() { return -1; }

int AI::Script::CharaAi::vf2C() { return -1; }

int AI::Script::CharaAi::vf30() { return -1; }

int AI::Script::CharaAi::vf34() { return -1; }

int AI::Script::CharaAi::vf38() { return -1; }

int AI::Script::CharaAi::vf3C() { return -1; }

int AI::Script::CharaAi::vf40() { return -1; }

int AI::Script::AI_archer_attack01::vf08() {
    int result = 1;
    lbl_807C6A40.m1C = lbl_807C68C0.m30;
    lbl_807C6A40.m20 = 0x5A;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_archer_attack01::vf20() {
    if (lbl_807C6A40.m18 == 0) {
        int result = Special_unk();
        if (result == 6) {
            int next = fn_8003CDB0();
            if (next == 9) {
                lbl_807C6A40.m00 = 1;
                return 9;
            }
            if (next == 4) {
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            return -1;
        }
        if (result == 1) {
            lbl_807C6A40.m00 = 1;
            return 1;
        }
        if (result == 9) {
            lbl_807C6A40.m00 = 1;
            return 9;
        }
        return -1;
    }
    int result = AttackValid_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (result == 6) {
        if (lbl_807C68C0.m0C < lbl_807C6A40.m20) {
            return -1;
        }
        if (CanUseItem_unk()) {
            Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_archer_attack01::vf18() {
    if (lbl_807C6A40.m18 == 0) {
        int result = WaitRandom_unk();
        if (result == 7) {
            lbl_807C6A40.m00 = 1;
            return 7;
        }
        return -1;
    }
    int result = AttackNear_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_archer_attack01::vf24() {
    if (lbl_807C6A40.m18 == 0) {
        int result = ApproachOrSpecial_unk();
        if (result == 9) {
            lbl_807C6A40.m00 = 1;
            return 9;
        }
        if (result == 6) {
            int next = fn_8003CDB0();
            if (next == 6) {
                lbl_807C6A40.m00 = 1;
                return 6;
            }
            if (next == 4) {
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            if (next == 9) {
                lbl_807C6A40.m00 = 1;
                return 9;
            }
            return -1;
        }
        return -1;
    }
    int result = Ready_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_archer_attack01::vf40() {
    if (fn_8001FB08() == 0) {
        lbl_807C6A40.m00 = 1;
        return 0;
    }
    int result;
    if (lbl_807C6A40.m10 >= 1) {
        result = -1;
    } else {
        lbl_807C6A40.m00 = 1;
        result = 1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_archer_attack01::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C6A40.m18 == 0) {
            int next = fn_8003CDB0();
            if (next == 6) {
                lbl_807C6A40.m00 = 1;
                return 6;
            }
            if (next == 9) {
                lbl_807C6A40.m00 = 1;
                return 9;
            }
            if (next == 4) {
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            return -1;
        }
        int next = fn_8003DBA4();
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_archer_attack01::vf1C() {
    int result = FarOrSpecial_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    if (result == 8) {
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    return -1;
}

int AI::Script::AI_archer_attack01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_archer_attack01::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_archer_guard01::vf08() {
    int result = 1;
    lbl_807C6A40.m1C = lbl_807C68C0.m30;
    lbl_807C6A40.m20 = 0x5A;
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x64;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_archer_guard01::vf20() {
    int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
    if (check == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (check == 10) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (lbl_807C6A40.m18 == 0) {
        int result = Special_unk();
        if (result == 9) {
            lbl_807C6A40.m00 = 1;
            return 9;
        }
        if (result == 6) {
            int next = fn_8003CDB0();
            if (next == 9) {
                lbl_807C6A40.m00 = 1;
                return 9;
            }
            if (next == 6) {
                return -1;
            }
            if (next == 4) {
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            return -1;
        }
        if (result == 1) {
            lbl_807C6A40.m00 = 1;
            return 1;
        }
        return -1;
    }
    int result = AttackValid_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (result == 6) {
        if (lbl_807C68C0.m0C < lbl_807C6A40.m20) {
            return -1;
        }
        if (CanUseItem_unk()) {
            Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_archer_guard01::vf18() {
    if (lbl_807C6A40.m18 == 0) {
        int result = WaitRandom_unk();
        if (result == 7) {
            lbl_807C6A40.m00 = 1;
            return 7;
        }
        return -1;
    }
    int result = AttackNear_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_archer_guard01::vf24() {
    int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
    if (check == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (check == 10) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (lbl_807C6A40.m18 == 0) {
        int result = ApproachOrSpecial_unk();
        if (result == 9) {
            lbl_807C6A40.m00 = 1;
            return 9;
        }
        if (result == 6) {
            int next = fn_8003CDB0();
            if (next == 6) {
                lbl_807C6A40.m00 = 1;
                return 6;
            }
            if (next == 4) {
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            return -1;
        }
        return -1;
    }
    int result = Ready_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_archer_guard01::vf40() {
    if (fn_800209F0() == 0) {
        lbl_807C6A40.m00 = 1;
        return 0;
    }
    int result;
    if (lbl_807C6A40.m10 >= 1) {
        result = -1;
    } else {
        lbl_807C6A40.m00 = 1;
        result = 1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_archer_guard01::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (lbl_807C68C0.m08 != 0) {
        if (fn_8001BE00(lbl_807C68C0.m20)) {
            int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
            if (check == 1) {
                    lbl_807C6A40.m00 = 1;
                    return 1;
            }
            if (check == 10) {
                    lbl_807C6A40.m00 = 1;
                    return 1;
            }
            if (lbl_807C6A40.m18 == 0) {
                int next = fn_8003CDB0();
                if (next == 6) {
                    lbl_807C6A40.m00 = 1;
                    return 6;
                }
                if (next == 4) {
                    lbl_807C6A40.m00 = 1;
                    return 4;
                }
                return -1;
            }
            int next = fn_8003DBA4();
            if (next == 6) {
                lbl_807C6A40.m00 = 1;
                return 6;
            }
            if (next == 4) {
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_archer_guard01::vf1C() {
    int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
    if (check == 10) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (check == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    int result = FarOrSpecial_unk();
    if (result == 8) {
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_archer_guard01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_archer_guard01::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

// inferred: never inlined into vf40, its only caller (noinline as in MP/CryptMP)
__declspec(noinline) int AI::Script::AI_archer_guard01::fn_800209F0() {
    if (lbl_807C6A40.m18 == 0) {
        if (lbl_807C68C0.m1C < lbl_807C6A40.m1C) {
            lbl_807C6A40.m18 = 1;
            lbl_807C6A40.m0C = 1;
            lbl_807C6A40.m00 = 1;
            Command_unk(0, 0, 0, 0, 0);
            return 0;
        }
        return -1;
    }
    if (lbl_807C68C0.m1C >= lbl_807C6A40.m1C) {
        lbl_807C6A40.m18 = 0;
        lbl_807C6A40.m0C = 0;
        lbl_807C6A40.m00 = 1;
        Command_unk(0, 0, 0, 0, 0);
        return 0;
    }
    return -1;
}

int AI::Script::AI_arganan01::vf08() {
    int result = 1;
    lbl_807C6A40.m24 = 0x50;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

static inline int IsBusy_unk(CharaTask* chara, int next) {
    if (next == 0) {
        return fn_8001BC80(chara, 0);
    }
    return fn_8001BC80(chara, next - 1);
}
static inline void Combo7_unk(int step) {
    Command_unk(7, step, (int)lbl_807C68C0.m00, 0, 0);
}
static inline int Combo_unk() {
    if (IsBusy_unk(lbl_807C68C0.m00, lbl_807C6A40.m18 + 1)) {
        return -1;
    }
    Combo7_unk(lbl_807C6A40.m18);
    lbl_807C6A40.m00 = 1;
    lbl_807C6A40.m18 += 1;
    if (fn_8001BCB4(lbl_807C68C0.m00, lbl_807C6A40.m18)) {
        return 9;
    }
    lbl_807C6A40.m18 = 0;
    return 9;
}

int AI::Script::AI_arganan01::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m08 != 0) {
            if (fn_8001BF4C(lbl_807C68C0.m00)) {
                Command_unk(16, 0, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                fn_8001BADC(lbl_807C68C0.m00, 1);
                return 6;
            }
            if (lbl_807C68C0.m58 > lbl_807C6A40.m24) {
                if (fn_8001BEB0(lbl_807C68C0.m00)) {
                    Command_unk(0, 30, 0, 0, 0);
                    lbl_807C6A40.m00 = 1;
                    return 1;
                }
                if (CanUseItem2_unk()) {
                    Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
                    lbl_807C6A40.m00 = 1;
                    return 4;
                }
                Command_unk(0, 30, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            return Combo_unk();
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_arganan01::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_arganan01::vf24() {
    if (fn_8001BF4C(lbl_807C68C0.m00)) {
        Command_unk(16, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        fn_8001BADC(lbl_807C68C0.m00, 1);
        return 6;
    }
    int result = Stop_unk();
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_arganan01::vf2C() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_arganan01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_arganan01::vf20() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_attackstable01::vf08() {
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x50;
    lbl_807C6A40.m00 = 1;
    int d = 0;
    int c = 0;
    int b = 0;
    int a = 0;
    fn_8001AEDC(0, &a, &b, &c, &d);
    return 1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_buffer_attackstable01::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m08 != 0) {
            if (IsBusy_unk(lbl_807C68C0.m00, lbl_807C6A40.m18 + 1)) {
                Command_unk(0, 30, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            if (fn_8001BC38(lbl_807C68C0.m00, lbl_807C6A40.m18)) {
                Command_unk(7, lbl_807C6A40.m18, (int)lbl_807C68C0.m00, 0, 0);
                lbl_807C6A40.m00 = 1;
                lbl_807C6A40.m18 += 1;
                if (fn_8001BCB4(lbl_807C68C0.m00, lbl_807C6A40.m18)) {
                    return 9;
                }
                lbl_807C6A40.m18 = 0;
                return 9;
            }
            Command_unk(0, 30, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 1;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_buffer_attackstable01::vf2C() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_attackstable01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard01::vf08() {
    int result = 1;
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x50;
    lbl_807C6A40.m00 = 1;
    int d = 0;
    int c = 0;
    int b = 0;
    int a = 0;
    fn_8001AEDC(0, &a, &b, &c, &d);
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_buffer_guard01::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m08 != 0) {
            if (fn_8001BF4C(lbl_807C68C0.m00)) {
                Command_unk(16, 0, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                fn_8001BADC(lbl_807C68C0.m00, 1);
                return 6;
            }
            if (IsBusy_unk(lbl_807C68C0.m00, lbl_807C6A40.m18 + 1)) {
                int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
                if (check == 1) {
                    lbl_807C6A40.m00 = 1;
                    return 1;
                }
                if (check == 10) {
                    lbl_807C6A40.m00 = 1;
                    return 1;
                }
                if (fn_8001BEB0(lbl_807C68C0.m00)) {
                    Command_unk(0, 30, 0, 0, 0);
                    lbl_807C6A40.m00 = 1;
                    return 1;
                }
                if (CanUseItem2_unk()) {
                    Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
                    lbl_807C6A40.m00 = 1;
                    return 4;
                }
                Command_unk(0, 30, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            if (fn_8001BC38(lbl_807C68C0.m00, lbl_807C6A40.m18)) {
                Command_unk(7, lbl_807C6A40.m18, (int)lbl_807C68C0.m00, 0, 0);
                lbl_807C6A40.m00 = 1;
                lbl_807C6A40.m18 += 1;
                if (fn_8001BCB4(lbl_807C68C0.m00, lbl_807C6A40.m18)) {
                    return 9;
                }
                lbl_807C6A40.m18 = 0;
                return 9;
            }
            int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
            if (check == 1) {
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            if (check == 10) {
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            if (fn_8001BEB0(lbl_807C68C0.m00)) {
                Command_unk(0, 30, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            if (CanUseItem2_unk()) {
                Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            Command_unk(0, 30, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 1;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard01::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_buffer_guard01::vf24() {
    if (fn_8001BF4C(lbl_807C68C0.m00)) {
        Command_unk(16, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        fn_8001BADC(lbl_807C68C0.m00, 1);
        return 6;
    }
    int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
    if (check == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (check == 10) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    int result = Stop_unk();
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard01::vf2C() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard01::vf20() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard02::vf08() {
    int result = 1;
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x50;
    lbl_807C6A40.m00 = 1;
    int d = 0;
    int c = 0;
    int b = 0;
    int a = 0;
    fn_8001AEDC(0, &a, &b, &c, &d);
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_buffer_guard02::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m08 != 0) {
            if (fn_8001BF4C(lbl_807C68C0.m00)) {
                Command_unk(16, 0, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                fn_8001BADC(lbl_807C68C0.m00, 1);
                return 6;
            }
            if (lbl_807C6A40.m18 > 0 && !fn_8001BC80(lbl_807C68C0.m00, 0)) {
                lbl_807C6A40.m18 = 0;
                lbl_807C6A40.m00 = 1;
            }
            if (IsBusy_unk(lbl_807C68C0.m00, lbl_807C6A40.m18 + 1)) {
                int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
                if (check == 1) {
                    lbl_807C6A40.m00 = 1;
                    return 1;
                }
                if (check == 10) {
                    lbl_807C6A40.m00 = 1;
                    return 1;
                }
                if (fn_8001BEB0(lbl_807C68C0.m00)) {
                    Command_unk(0, 30, 0, 0, 0);
                    lbl_807C6A40.m00 = 1;
                    return 1;
                }
                if (CanUseItem2_unk()) {
                    Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
                    lbl_807C6A40.m00 = 1;
                    return 4;
                }
                Command_unk(0, 30, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            if (fn_8001BC38(lbl_807C68C0.m00, lbl_807C6A40.m18)) {
                Command_unk(7, lbl_807C6A40.m18, (int)lbl_807C68C0.m00, 0, 0);
                lbl_807C6A40.m00 = 1;
                lbl_807C6A40.m18 += 1;
                if (fn_8001BCB4(lbl_807C68C0.m00, lbl_807C6A40.m18)) {
                    return 9;
                }
                lbl_807C6A40.m18 = 0;
                return 9;
            }
            int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
            if (check == 1) {
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            if (check == 10) {
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            if (fn_8001BEB0(lbl_807C68C0.m00)) {
                Command_unk(0, 30, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            if (CanUseItem2_unk()) {
                Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            Command_unk(0, 30, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 1;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard02::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_buffer_guard02::vf24() {
    if (fn_8001BF4C(lbl_807C68C0.m00)) {
        Command_unk(16, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        fn_8001BADC(lbl_807C68C0.m00, 1);
        return 6;
    }
    int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
    if (check == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (check == 10) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    int result = Stop_unk();
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard02::vf2C() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard02::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_guard02::vf20() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_pointguard01::vf08() {
    lbl_807C6A40.m18 = lbl_807C68C0.m7C;
    lbl_807C6A40.m1C = lbl_807C68C0.m80;
    lbl_807C6A40.m20 = lbl_807C68C0.m84;
    lbl_807C6A40.m24 = lbl_807C68C0.m88;
    lbl_807C6A40.m50 = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

static inline int SkillDist_unk(int id, CharaTask* chara) {
    if (id <= 0 || chara == 0) {
        return 0;
    }
    return fn_8001ADE8(id, chara);
}

int AI::Script::AI_buffer_pointguard01::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        if (lbl_807C6A40.m50 == 0) {
            return -1;
        }
        lbl_807C6A40.m50 = 0;
        lbl_807C6A40.m00 = 1;
        Command_unk(0, 0, 0, 0, 0);
        return 1;
    }
    if (fn_8001BC38(lbl_807C68C0.m00, 0)) {
        if (fn_8001BC80(lbl_807C68C0.m00, 0)) {
            int dist = SkillDist_unk(lbl_807C6A40.m18, lbl_807C68C0.m20);
            if (dist < lbl_807C6A40.m1C) {
                lbl_807C6A40.m0C = 1;
                lbl_807C6A40.m00 = 1;
                lbl_807C6A40.m50 = 0;
                Command_unk(4, (int)lbl_807C68C0.m20, AttackDelay_unk(), 0, 0);
                return 5;
            }
            if (lbl_807C6A40.m50 == 0) {
                lbl_807C6A40.m50 = 1;
                lbl_807C6A40.m00 = 1;
                Command_unk(8, lbl_807C6A40.m18, lbl_807C6A40.m1C, 0, 0);
                return 1;
            }
            return -1;
        }
        if (lbl_807C6A40.m3C <= 0) {
            Command_unk(7, 0, (int)lbl_807C68C0.m00, 0, 0);
            lbl_807C6A40.m00 = 1;
            lbl_807C6A40.m3C = lbl_807C6A40.m20;
            return 9;
        }
        int dist = SkillDist_unk(lbl_807C6A40.m18, lbl_807C68C0.m20);
        if (dist < lbl_807C6A40.m1C) {
            lbl_807C6A40.m0C = 1;
            lbl_807C6A40.m00 = 1;
            lbl_807C6A40.m50 = 0;
            Command_unk(4, (int)lbl_807C68C0.m20, AttackDelay_unk(), 0, 0);
            return 5;
        }
        if (lbl_807C6A40.m50 == 0) {
            lbl_807C6A40.m50 = 1;
            lbl_807C6A40.m00 = 1;
            Command_unk(8, lbl_807C6A40.m18, lbl_807C6A40.m1C, 0, 0);
            return 1;
        }
        return -1;
    }
    int dist = SkillDist_unk(lbl_807C6A40.m18, lbl_807C68C0.m20);
    if (dist < lbl_807C6A40.m1C) {
        lbl_807C6A40.m0C = 1;
        lbl_807C6A40.m00 = 1;
        lbl_807C6A40.m50 = 0;
        Command_unk(4, (int)lbl_807C68C0.m20, AttackDelay_unk(), 0, 0);
        return 5;
    }
    if (lbl_807C6A40.m50 == 0) {
        lbl_807C6A40.m50 = 1;
        lbl_807C6A40.m00 = 1;
        Command_unk(8, lbl_807C6A40.m18, lbl_807C6A40.m1C, 0, 0);
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_pointguard01::vf2C() {
    if (lbl_807C68C0.m08 != 0) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_buffer_pointguard01::vf1C() {
    int dist = SkillDist_unk(lbl_807C6A40.m18, lbl_807C68C0.m20);
    if (dist < lbl_807C6A40.m1C) {
        if (fn_8001BC80(lbl_807C68C0.m00, 0)) {
            if (lbl_807C68C0.m08 != 0 && lbl_807C6A40.m24 == 1) {
                if (fn_8001BE00(lbl_807C68C0.m20)) {
                    Command_unk(5, 1, 0, 0, 0);
                    lbl_807C6A40.m00 = 1;
                    return 8;
                }
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            return -1;
        }
        if (lbl_807C6A40.m3C <= 0) {
            lbl_807C6A40.m0C = 0;
            lbl_807C6A40.m00 = 1;
            return 1;
        }
        if (lbl_807C68C0.m08 != 0 && lbl_807C6A40.m24 == 1) {
            if (fn_8001BE00(lbl_807C68C0.m20)) {
                Command_unk(5, 1, 0, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 8;
            }
            lbl_807C6A40.m00 = 1;
            return 1;
        }
        return -1;
    }
    lbl_807C6A40.m0C = 0;
    lbl_807C6A40.m00 = 1;
    if (lbl_807C6A40.m50 == 0) {
        lbl_807C6A40.m50 = 1;
        Command_unk(8, lbl_807C6A40.m18, lbl_807C6A40.m1C, 0, 0);
        return 1;
    }
    return 1;
}

int AI::Script::AI_buffer_pointguard01::vf40() {
    if (lbl_807C6A40.m3C > 0) {
        if (fn_8001BC80(lbl_807C68C0.m00, 0)) {
            int result = CheckCount_unk();
            if (result == 1) {
                lbl_807C6A40.m00 = 1;
                return 1;
            }
            return -1;
        }
        lbl_807C6A40.m3C -= 1;
        lbl_807C6A40.m00 = 1;
        int result = CheckCount_unk();
        if (result == 1) {
            return 1;
        }
        return -1;
    }
    int result = CheckCount_unk();
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_buffer_pointguard01::vf28() {
    if (lbl_807C68C0.m08 != 0) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_archer_attack02::vf08() {
    lbl_807C6A40.m1C = 0x1E;
    lbl_807C6A40.m20 = 0x5A;
    lbl_807C6A40.m24 = lbl_807C68C0.m7C;
    lbl_807C6A40.m00 = 1;
    return 1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_em_archer_attack02::vf20() {
    if (lbl_807C6A40.m18 == 0) {
        int result = Special_unk();
        if (result == 1) {
            lbl_807C6A40.m00 = 1;
            return 1;
        }
        if (result == 9) {
            lbl_807C6A40.m00 = 1;
            return 9;
        }
        if (result == 6) {
            int next = fn_8003CDB0();
            if (next == 4) {
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            if (next == 9) {
                lbl_807C6A40.m00 = 1;
                return 9;
            }
            return -1;
        }
        return -1;
    }
    int result = AttackValid_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (result == 6) {
        if (lbl_807C68C0.m0C < lbl_807C6A40.m20) {
            return -1;
        }
        if (CanUseItem_unk()) {
            Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_em_archer_attack02::vf18() {
    if (lbl_807C6A40.m18 == 0) {
        int result = WaitRandom_unk();
        if (result == 7) {
            lbl_807C6A40.m00 = 1;
            return 7;
        }
        return -1;
    }
    int result = AttackNear_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_em_archer_attack02::vf24() {
    if (lbl_807C6A40.m18 == 0) {
        int result = ApproachOrSpecial_unk();
        if (result == 6) {
            int next = fn_8003CDB0();
            if (next == 6) {
                lbl_807C6A40.m00 = 1;
                return 6;
            }
            if (next == 9) {
                lbl_807C6A40.m00 = 1;
                return 9;
            }
            if (next == 4) {
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            return -1;
        }
        if (result == 9) {
            lbl_807C6A40.m00 = 1;
            return 9;
        }
        return -1;
    }
    int result = Ready_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_em_archer_attack02::vf40() {
    if (fn_80023C84() == 0) {
        lbl_807C6A40.m00 = 1;
        return 0;
    }
    int result;
    if (lbl_807C6A40.m10 >= 1) {
        result = -1;
    } else {
        lbl_807C6A40.m00 = 1;
        result = 1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_archer_attack02::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C6A40.m28 == 0) {
            return Turn_unk();
        }
        if (lbl_807C6A40.m18 == 0) {
            int next = fn_8003CDB0();
            if (next == 4) {
                lbl_807C6A40.m00 = 1;
                return 4;
            }
            if (next == 9) {
                lbl_807C6A40.m00 = 1;
                return 9;
            }
            if (next == 6) {
                lbl_807C6A40.m00 = 1;
                return 6;
            }
            return -1;
        }
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_em_archer_attack02::vf1C() {
    int result = FarOrSpecial_unk();
    if (result == 8) {
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_em_archer_attack02::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_em_archer_attack02::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_em_archer_attack02::vf14() {
    if (lbl_807C68C0.m08 != 0) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

// inferred: never inlined into vf40, its only caller (noinline as in MP/CryptMP)
__declspec(noinline) int AI::Script::AI_em_archer_attack02::fn_80023C84() {
    if (lbl_807C6A40.m18 == 0) {
        if (lbl_807C68C0.m1C < lbl_807C6A40.m1C) {
            lbl_807C6A40.m18 = 1;
            lbl_807C6A40.m0C = 1;
            lbl_807C6A40.m00 = 1;
            Command_unk(0, 0, 0, 0, 0);
            return 0;
        }
        return -1;
    }
    if (lbl_807C68C0.m1C >= lbl_807C6A40.m1C) {
        lbl_807C6A40.m18 = 0;
        lbl_807C6A40.m0C = 0;
        lbl_807C6A40.m00 = 1;
        Command_unk(0, 0, 0, 0, 0);
        return 0;
    }
    return -1;
}

int AI::Script::AI_em_bridge_archer::vf08() {
    lbl_807C6A40.m18 = lbl_807C68C0.m7C;
    lbl_807C6A40.m1C = lbl_807C68C0.m80;
    lbl_807C6A40.m00 = 1;
    return 1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_em_bridge_archer::vf2C() {
    return ShootInRange_unk();
}

int AI::Script::AI_em_bridge_archer::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (CanUseItem2_unk()) {
        vf18();
        lbl_807C6A40.m00 = 1;
        return 9;
    }
    Command_unk(0, lbl_807C6A40.m1C, 0, 0, 0);
    lbl_807C6A40.m00 = 1;
    return 9;
}

int AI::Script::AI_em_bridge_archer::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_bridge_archer::vf18() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = lbl_807C6A40.m1C;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 9;
    }
    return -1;
}

int AI::Script::AI_em_bridge_archer::vf24() {
    if (lbl_807C68C0.m08 != 0) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_monster_attack01::vf08() {
    lbl_807C6A40.m1C = 0xF;
    lbl_807C6A40.m00 = 1;
    return 1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_em_monster_attack01::vf1C() {
    int result = FarOrSpecial_unk();
    if (result == 6) {
        if (lbl_807C68C0.m20 != 0) {
            Command_unk(3, (int)lbl_807C68C0.m20, 30, 0, 0);
        }
        lbl_807C6A40.m00 = 1;
        return 6;
    }
    if (result == 8) {
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    return -1;
}

int AI::Script::AI_em_monster_attack01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_em_monster_attack01::vf20() {
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m08 != 0) {
            Command_unk(4, (int)lbl_807C68C0.m20, lbl_807C6A40.m1C, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 5;
        }
        if (lbl_807C68C0.m0C < lbl_807C6A40.m18) {
            if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
                Command_unk(4, (int)lbl_807C68C0.m20, lbl_807C6A40.m1C, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 5;
            }
            return -1;
        }
        return -1;
    }
    Command_unk(0, 0, 0, 0, 0);
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_em_monster_attack01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_monster_attack01::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m20 != 0) {
            Command_unk(3, (int)lbl_807C68C0.m20, 30, 0, 0);
        }
        lbl_807C6A40.m00 = 1;
        return 6;
    }
    return -1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_em_scout::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (InRange_unk(9999.0f)) {
        int next = fn_80024BC0();
        if (next == 3) {
            lbl_807C6A40.m00 = 1;
            return 3;
        }
        if (next == 7) {
            lbl_807C6A40.m00 = 1;
            return 7;
        }
        if (next == 5) {
            lbl_807C6A40.m00 = 1;
            return 5;
        }
        if (next == 8) {
            lbl_807C6A40.m00 = 1;
            return 8;
        }
        if (next == 1) {
            lbl_807C6A40.m00 = 1;
            return 1;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_em_scout::vf08() {
    lbl_807C6A40.m18 = lbl_807C68C0.m7C;
    lbl_807C6A40.m1C = lbl_807C68C0.m80;
    lbl_807C6A40.m20 = lbl_807C68C0.m84;
    lbl_807C6A40.m24 = 0;
    lbl_807C6A40.m28 = 0;
    lbl_807C6A40.m2C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_em_scout::vf40() {
    lbl_807C6A40.m3C = Distance_unk(lbl_807C68C0.m00, lbl_807C68C0.m20);
    lbl_807C6A40.m00 = 1;
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_scout::vf2C() {
    if (lbl_807C68C0.m08 != 0) {
        Command_unk(6, 1, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        lbl_807C6A40.m24 = 2;
        return 1;
    }
    fn_8001C068(lbl_807C68C0.m00, (const char*)(lbl_807307A0 + 0x3F5)); // "まだ隠れられていない"
    lbl_807C6A40.m00 = 1;
    return -1;
}

// NONMATCHING: 99.72% - pending the string pool (the literal is loaded before in.m00).
int AI::Script::AI_em_scout::vf24() {
    if (CanUseItem2_unk()) {
        Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        lbl_807C6A40.m24 = 1;
        return 9;
    }
    fn_8001C068(lbl_807C68C0.m00, (const char*)(lbl_807307A0 + 0x3E0)); // "隠れるポイント未発見"
    lbl_807C6A40.m00 = 1;
    Command_unk(6, 1, 0, 0, 0);
    lbl_807C6A40.m24 = 3;
    return 1;
}

// NONMATCHING: 99.32% - MWCC schedules local stores and address setup differently.
int AI::Script::AI_em_scout::vf28() {
    int args[4];
    args[0] = (int)(lbl_807307A0 + 0x40A);
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    fn_8001AEDC(0x14, &args[0], &args[1], &args[2], &args[3]);
    lbl_807C6A40.m00 = 1;
    lbl_807C6A40.m24 = 5;
    return 1;
}

int AI::Script::AI_em_scout::vf14() {
    int base = fn_8001B5B4(lbl_807C68C0.m10, 0);
    Command_unk(1, PickPoint_unk(base), 0, 0, 0);
    lbl_807C6A40.m00 = 1;
    lbl_807C6A40.m28 = 1;
    return 1;
}

int AI::Script::AI_em_scout::vf1C() {
    int d = 0;
    int c = 0;
    int b = (int)lbl_807C68C0.m20;
    int a = 0;
    fn_8001AEDC(7, &a, &b, &c, &d);
    lbl_807C6A40.m00 = 1;
    lbl_807C6A40.m24 = 6;
    return 1;
}

// inferred: never inlined into vf0C, its only caller (noinline as in MP/CryptMP)
__declspec(noinline) int AI::Script::AI_em_scout::fn_80024BC0() {
    lbl_807C6A40.m00 = 1;
    lbl_807C6A40.m40 += 1;
    if (lbl_807C6A40.m28 >= 1) {
        lbl_807C6A40.m28 += 1;
        if (lbl_807C6A40.m28 >= 90) {
            lbl_807C6A40.m28 = 0;
            return 1;
        }
        return 1;
    }
    lbl_807C6A40.m44 += 1;
    if (lbl_807C6A40.m24 >= 1) {
        if (lbl_807C68C0.m08 != 0) {
            lbl_807C6A40.m48 += 1;
            if (lbl_807C6A40.m24 < 5) {
                lbl_807C6A40.m24 = 0;
                int roll = rand() % 8 + 1;
                lbl_807C6A40.m2C = roll;
                int result = 5;
                if (roll <= 4) {
                    result = 3;
                }
                return result;
            }
            lbl_807C6A40.m24 = 0;
            if (Distance_unk(lbl_807C68C0.m00, lbl_807C68C0.m20) <= lbl_807C6A40.m1C) {
                return 7;
            }
            return 3;
        }
        return 1;
    }
    if (Distance_unk(lbl_807C68C0.m00, lbl_807C68C0.m20) <= lbl_807C6A40.m1C) {
        return 7;
    }
    return 3;
}

int AI::Script::AI_em_sword_attack01::vf08() {
    int result = 1;
    lbl_807C6A40.m18 = 0x5A;
    lbl_807C6A40.m1C = 0xF;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_em_sword_attack01::vf24() {
    int result = Ready_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_em_sword_attack01::vf18() {
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, lbl_807C6A40.m1C, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (lbl_807C68C0.m08 != 0) {
        Command_unk(0, WaitDelay_unk(), 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_em_sword_attack01::vf1C() {
    int result = FarOrSpecial_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    if (result == 8) {
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    return -1;
}

int AI::Script::AI_em_sword_attack01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_em_sword_attack01::vf20() {
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m08 != 0) {
            Command_unk(4, (int)lbl_807C68C0.m20, lbl_807C6A40.m1C, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 5;
        }
        if (lbl_807C68C0.m0C < lbl_807C6A40.m18) {
            if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
                Command_unk(4, (int)lbl_807C68C0.m20, lbl_807C6A40.m1C, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 5;
            }
            return -1;
        }
        if (lbl_807C68C0.m1C > lbl_807C68C0.m34 && CanUseItem_unk()) {
            Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    Command_unk(0, 0, 0, 0, 0);
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_em_sword_attack01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_sword_attack01::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        int next = fn_8003DBA4();
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_em_sword_attack02::vf08() {
    int result = 1;
    lbl_807C6A40.m18 = 0x5A;
    lbl_807C6A40.m1C = 0xF;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_em_sword_attack02::vf24() {
    int result = Ready_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_em_sword_attack02::vf18() {
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, lbl_807C6A40.m1C, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (lbl_807C68C0.m08 != 0) {
        Command_unk(0, WaitDelay_unk(), 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key, AIScriptMapNode_unk** parent, unsigned char* insertLeft, unsigned char* insertFirst);
extern "C" void fn_8003E688();

int AI::Script::AI_em_sword_attack02::vf1C() {
    int result = FarOrSpecial_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    if (result == 8) {
        if (rand() % 2 == 0) {
            if (fn_8001BCB4(lbl_807C68C0.m00, lbl_807C6A40.m3C)) {
                if (fn_8001BC38(lbl_807C68C0.m00, lbl_807C6A40.m3C)) {
                    Command_unk(7, lbl_807C6A40.m3C, (int)lbl_807C68C0.m20, 0, 0);
                    lbl_807C6A40.m00 = 1;
                    lbl_807C6A40.m3C += 1;
                    if (fn_8001BCB4(lbl_807C68C0.m00, lbl_807C6A40.m3C)) {
                        return 8;
                    }
                    lbl_807C6A40.m3C = 0;
                    return 8;
                }
                lbl_807C6A40.m00 = 1;
                return 8;
            }
            lbl_807C6A40.m00 = 1;
            return 8;
        }
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    return -1;
}

int AI::Script::AI_em_sword_attack02::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_em_sword_attack02::vf20() {
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        if (lbl_807C68C0.m08 != 0) {
            Command_unk(4, (int)lbl_807C68C0.m20, lbl_807C6A40.m1C, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 5;
        }
        if (lbl_807C68C0.m0C < lbl_807C6A40.m18) {
            if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
                Command_unk(4, (int)lbl_807C68C0.m20, lbl_807C6A40.m1C, 0, 0);
                lbl_807C6A40.m00 = 1;
                return 5;
            }
            return -1;
        }
        if (lbl_807C68C0.m1C > lbl_807C68C0.m34 && CanUseItem_unk()) {
            Command_unk(2, lbl_807C68C0.m64, 0, 0, 0);
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    Command_unk(0, 0, 0, 0, 0);
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_em_sword_attack02::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_sword_attack02::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        int next = fn_8003DBA4();
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_em_sword_pointguard01::vf08() {
    lbl_807C6A40.m1C = lbl_807C68C0.m7C;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = lbl_807C68C0.m84;
    lbl_807C6A40.m28 = lbl_807C68C0.m88;
    lbl_807C6A40.m8C = lbl_807C68C0.m8C;
    lbl_807C6A40.m3C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_em_sword_pointguard01::vf20() {
    int result = AttackValid_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, AttackDelay_unk(), 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_em_sword_pointguard01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 89.70% - the local-slot allocation and scratch-store scheduling differ.
int AI::Script::AI_em_type003::vf08() {
    int d = 0;
    int c = 0xA;
    int b = 0;
    int a = 0;
    lbl_807C6A40.m18 = 0x28;
    d = lbl_807C68C0.m7C;
    lbl_807C6A40.m1C = d;
    lbl_807C6A40.m00 = 1;
    fn_8001AEDC(8, &d, &c, &b, &a);
    return 1;
}

int AI::Script::AI_em_type003::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

// NONMATCHING: 76.72% - MWCC's parameter stack slots and scratch-store order differ.
int AI::Script::AI_em_type010::vf08() {
    int a = lbl_807C68C0.m7C;
    lbl_807C6A40.m18 = 0x28;
    lbl_807C6A40.m1C = a;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = 0;
    lbl_807C6A40.m28 = lbl_807C68C0.m84;
    lbl_807C6A40.m00 = 1;
    int b = 0xA;
    int c = 0;
    int d = 0;
    fn_8001AEDC(8, &a, &b, &c, &d);
    return 1;
}

int AI::Script::AI_em_type010::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_em_type102::vf08() {
    lbl_807C6A40.m00 = 1;
    return 1;
}

// NONMATCHING: 88.97% - float comparison and parameter stack-slot allocation differ.
int AI::Script::AI_em_type102::vf2C() {
    if (lbl_807C68C0.m08 != 0 && lbl_807C68C0.m1C < lbl_80880798) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0xA;
        fn_8001AEDC(6, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 4;
    }
    return -1;
}

int AI::Script::AI_em_type102::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_type102::vf18() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0xD2;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 9;
    }
    return -1;
}

int AI::Script::AI_em_type102::vf24() {
    if (lbl_807C68C0.m08 != 0) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 88.97% - float comparison and parameter stack-slot allocation differ.
int AI::Script::AI_em_type103::vf2C() {
    if (lbl_807C68C0.m08 != 0 && lbl_807C68C0.m1C < lbl_80880798) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 3;
        fn_8001AEDC(6, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 4;
    }
    return -1;
}

int AI::Script::AI_em_type103::vf40() {
    if (fn_800281A0() == 0) {
        lbl_807C6A40.m00 = 1;
        return 0;
    }
    int result;
    if (lbl_807C6A40.m10 >= 1) {
        result = -1;
    } else {
        lbl_807C6A40.m00 = 1;
        result = 1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_type103::vf1C() {
    int result = FarOrSpecial_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    if (result == 8) {
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    return -1;
}

int AI::Script::AI_em_type103::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_em_type103::vf08() {
    lbl_807C6A40.m1C = 0x1E;
    lbl_807C6A40.m20 = 0x5A;
    lbl_807C6A40.m00 = 1;
    return 1;
}

// NONMATCHING: 88.97% - float comparison and parameter stack-slot allocation differ.
int AI::Script::AI_em_type104::vf2C() {
    if (lbl_807C68C0.m08 != 0 && lbl_807C68C0.m1C < lbl_80880798) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 3;
        fn_8001AEDC(6, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 4;
    }
    return -1;
}

int AI::Script::AI_em_type104::vf40() {
    if (fn_80028D00() == 0) {
        lbl_807C6A40.m00 = 1;
        return 0;
    }
    int result;
    if (lbl_807C6A40.m10 >= 1) {
        result = -1;
    } else {
        lbl_807C6A40.m00 = 1;
        result = 1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_type104::vf1C() {
    int result = FarOrSpecial_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    if (result == 8) {
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    return -1;
}

int AI::Script::AI_em_type104::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_em_type104::vf08() {
    lbl_807C6A40.m1C = 0x1E;
    lbl_807C6A40.m20 = 0x5A;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_em_type105::vf08() {
    lbl_807C6A40.m1C = lbl_807C68C0.m30;
    lbl_807C6A40.m20 = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_em_type105::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_em_type105::vf40() {
    if (fn_80029500() == 0) {
        lbl_807C6A40.m00 = 1;
        return 0;
    }
    int result;
    if (lbl_807C6A40.m10 >= 1) {
        result = -1;
    } else {
        lbl_807C6A40.m00 = 1;
        result = 1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_em_type105::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_em_type106::vf08() {
    lbl_807C6A40.m18 = lbl_807C68C0.m7C;
    lbl_807C6A40.m1C = lbl_807C68C0.m80;
    lbl_807C6A40.m20 = lbl_807C68C0.m84;
    lbl_807C6A40.m24 = 0;
    lbl_807C6A40.m28 = 0;
    lbl_807C6A40.m2C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_em_type106::vf40() {
    lbl_807C6A40.m3C = Distance_unk(lbl_807C68C0.m00, lbl_807C68C0.m20);
    lbl_807C6A40.m00 = 1;
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        return 1;
    }
    return -1;
}

// NONMATCHING: 99.32% - MWCC schedules local stores and address setup differently.
int AI::Script::AI_em_type106::vf28() {
    int args[4];
    args[0] = (int)(lbl_807307A0 + 0x40A);
    args[1] = 0;
    args[2] = 0;
    args[3] = 0;
    fn_8001AEDC(0x14, &args[0], &args[1], &args[2], &args[3]);
    lbl_807C6A40.m00 = 1;
    lbl_807C6A40.m24 = 5;
    return 1;
}

int AI::Script::AI_em_type106::vf14() {
    int d = 0;
    int c = 0;
    int b = 5;
    int a = lbl_807C6A40.m18;
    fn_8001AEDC(0xC, &a, &b, &c, &d);
    lbl_807C6A40.m00 = 1;
    lbl_807C6A40.m28 = 1;
    return 1;
}

int AI::Script::AI_em_type106::vf1C() {
    int d = 0;
    int c = 0;
    int b = (*(int*)&lbl_807C68C0.m00);
    int a = 0;
    fn_8001AEDC(7, &a, &b, &c, &d);
    lbl_807C6A40.m00 = 1;
    lbl_807C6A40.m24 = 6;
    return 1;
}

int AI::Script::AI_fr_archer_pick01::vf08() {
    int result = 1;
    lbl_807C6A40.m1C = lbl_807C68C0.m30;
    lbl_807C6A40.m20 = 0x5A;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_fr_archer_pick01::vf18() {
    if (lbl_807C6A40.m18 == 0) {
        int result = WaitRandom_unk();
        if (result == 7) {
            lbl_807C6A40.m00 = 1;
            return 7;
        }
        return -1;
    }
    int result = AttackNear_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_fr_archer_pick01::vf40() {
    if (fn_8002AD40() == 0) {
        lbl_807C6A40.m00 = 1;
        return 0;
    }
    int result;
    if (lbl_807C6A40.m10 >= 1) {
        result = -1;
    } else {
        lbl_807C6A40.m00 = 1;
        result = 1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_fr_archer_pick01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_fr_archer_pick01::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

// inferred: never inlined into vf40, its only caller (noinline as in MP/CryptMP)
__declspec(noinline) int AI::Script::AI_fr_archer_pick01::fn_8002AD40() {
    if (lbl_807C6A40.m18 == 0) {
        if (lbl_807C68C0.m1C < lbl_807C6A40.m1C) {
            lbl_807C6A40.m18 = 1;
            lbl_807C6A40.m0C = 1;
            lbl_807C6A40.m00 = 1;
            Command_unk(0, 0, 0, 0, 0);
            return 0;
        }
        return -1;
    }
    if (lbl_807C68C0.m1C >= lbl_807C6A40.m1C) {
        lbl_807C6A40.m18 = 0;
        lbl_807C6A40.m0C = 0;
        lbl_807C6A40.m00 = 1;
        Command_unk(0, 0, 0, 0, 0);
        return 0;
    }
    return -1;
}

int AI::Script::AI_fr_buffer_pick01::vf08() {
    lbl_807C6A40.mE0 = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_fr_buffer_pick01::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_fr_buffer_pick01::vf24() {
    int result;
    if (lbl_807C68C0.m08 != 0) {
        // inferred: these four zeroed outputs occupy one consecutive stack aggregate.
        struct Results_unk {
            int d;
            int c;
            int b;
            int a;
        } results = {0, 0, 0, 0};
        fn_8001AEDC(0, &results.a, &results.b, &results.c, &results.d);
        lbl_807C6A40.m00 = 1;
        result = 1;
    } else {
        result = -1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_fr_buffer_pick01::vf2C() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_fr_buffer_pick01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 88.31% - local output aggregate and register allocation differ.
// inferred: the helper receives four consecutive output words; only the first is initialized from m80.
int AI::Script::AI_fr_follow_player::vf08() {
    int results[4] = {0, 0, 0, 0};
    int value = lbl_807C68C0.m7C;
    results[0] = lbl_807C68C0.m80;
    lbl_807C6A40.m18 = value;
    lbl_807C6A40.m1C = 1;
    lbl_807C6A40.m00 = 1;
    fn_8001AEDC(0, &results[0], &results[1], &results[2], &results[3]);
    return 3;
}

int AI::Script::AI_fr_follow_player::vf14() {
    int d = 0;
    int c = 0;
    int b = lbl_807C6A40.m18;
    int a = lbl_807C6B30.m00;
    fn_8001AEDC(0xD, &a, &b, &c, &d);
    lbl_807C6A40.m00 = 1;
    return 6;
}

int AI::Script::AI_fr_magic_pick01::vf08() {
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_fr_magic_pick01::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_fr_magic_pick01::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_fr_magic_pick01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_fr_magic_pick02::vf08() {
    int result = 1;
    lbl_807C6A40.m18 = 0;
    lbl_807C6A40.m1C = 0x46;
    lbl_807C6A40.mE0 = 0;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_fr_magic_pick02::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_fr_runaway::vf08() {
    lbl_807C6A40.m18 = lbl_807C68C0.m7C;
    lbl_807C6A40.m1C = lbl_807C68C0.m80;
    lbl_807C6A40.m20 = lbl_807C68C0.m84;
    lbl_807C6A40.m24 = 0;
    lbl_807C6A40.m28 = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_fr_sword_attack01::vf08() {
    int result = 1;
    lbl_807C6A40.m18 = 0x5A;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_fr_sword_attack01::vf1C() {
    int result = FarOrSpecial_unk();
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    if (result == 8) {
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    return -1;
}

int AI::Script::AI_fr_sword_attack01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_fr_sword_attack01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_fr_sword_attack01::vf0C() {
    int result = CheckCount_unk();
    if (result == 1) {
        return -1;
    }
    if (fn_8001BE00(lbl_807C68C0.m20)) {
        int next = fn_8003DBA4();
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_fr_sword_pick01::vf08() {
    int result = 1;
    lbl_807C6A40.m18 = 0x5A;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_fr_sword_pick01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_fr_sword_pick01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 89.57% - stack output order and register allocation differ.
int AI::Script::AI_fr_sword_pointguard::vf08() {
    int d = lbl_807C68C0.m7C;
    int c = 0x1E;
    int b = 0;
    int a = 0;
    lbl_807C6A40.m18 = 0x3C;
    lbl_807C6A40.m1C = d;
    lbl_807C6A40.m00 = 1;
    fn_8001AEDC(8, &d, &c, &b, &a);
    return 1;
}

int AI::Script::AI_fr_sword_pointguard::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_fr_sword_pointguard01::vf08() {
    lbl_807C6A40.m1C = lbl_807C68C0.m7C;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = lbl_807C68C0.m84;
    lbl_807C6A40.m28 = lbl_807C68C0.m88;
    lbl_807C6A40.m8C = lbl_807C68C0.m8C;
    lbl_807C6A40.m3C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_fr_sword_pointguard01::vf20() {
    int result = AttackValid_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, AttackDelay_unk(), 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_fr_sword_pointguard01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 88.52% - argument-register allocation differs around the AI call.
int AI::Script::AI_fr_type005::vf08() {
    lbl_807C6A40.m18 = 0x2711;
    lbl_807C6A40.m1C = 0x32;
    lbl_807C6A40.m00 = 1;
    int d = 0;
    int c = 0;
    int b = 0x32;
    int a = 0x2711;
    fn_8001AEDC(8, &a, &b, &c, &d);
    return 1;
}

int AI::Script::AI_fr_type005::vf40() {
    int result;
    if (lbl_807C6A40.m10 >= 1) {
        result = -1;
    } else {
        lbl_807C6A40.m00 = 1;
        result = 1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_fr_warppointescape::vf08() {
    lbl_807C6A40.m18 = lbl_807C68C0.m7C;
    lbl_807C6A40.m1C = lbl_807C68C0.m80;
    lbl_807C6A40.m20 = lbl_807C68C0.m84;
    lbl_807C6A40.m24 = lbl_807C68C0.m88;
    lbl_807C6A40.m28 = lbl_807C68C0.m8C;
    lbl_807C6A40.m2C = lbl_807C68C0.m90;
    lbl_807C6A40.m30 = lbl_807C68C0.m94;
    lbl_807C6A40.m34 = lbl_807C68C0.m98;
    lbl_807C6A40.m38 = lbl_807C68C0.m9C;
    lbl_807C6A40.m3C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_fr_warppointescape::vf14() {
    int d = 0;
    int c = 0;
    int b = 0xA;
    int a = lbl_807C6A40.m3C;
    fn_8001AEDC(0xC, &a, &b, &c, &d);
    lbl_807C6A40.m00 = 1;
    ++lbl_807C6A40.m40;
    return 6;
}

int AI::Script::AI_fr_warppointescape::vf20() {
    if (lbl_807C68C0.m08 != 0) {
        if (lbl_807C6A40.m50 >= 0xC8) {
            lbl_807C6A40.m00 = 1;
            return 7;
        } else {
            lbl_807C6A40.m00 = 1;
            return 1;
        }
    }
    return -1;
}

int AI::Script::AI_jackal01::vf08() {
    int result = 1;
    lbl_807C6A40.m1C = lbl_807C68C0.m30;
    lbl_807C6A40.m20 = 0x5A;
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x64;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_jackal01::vf18() {
    if (lbl_807C6A40.m18 == 0) {
        int result = WaitRandom_unk();
        if (result == 7) {
            lbl_807C6A40.m00 = 1;
            return 7;
        }
        return -1;
    }
    int result = AttackNear_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_jackal01::vf1C() {
    int check = fn_8003D298(lbl_807C6A40.m24, lbl_807C6A40.m28);
    if (check == 10) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (check == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    int result = FarOrSpecial_unk();
    if (result == 8) {
        lbl_807C6A40.m00 = 1;
        return 8;
    }
    if (result == 6) {
        int next = fn_8003DBA4();
        if (next == 4) {
            lbl_807C6A40.m00 = 1;
            return 4;
        }
        if (next == 6) {
            lbl_807C6A40.m00 = 1;
            return 6;
        }
        return -1;
    }
    return -1;
}

int AI::Script::AI_jackal01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_jackal01::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_attack01::vf08() {
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_magic_attack01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_attack01::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_attack01::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_attack01_2::vf08() {
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_magic_attack01_2::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_attack01_2::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_attack01_2::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_attack02::vf08() {
    int result = 1;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_magic_attack02::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_attack02::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_attack02::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_attackstable01::vf08() {
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_magic_attackstable01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_attackstable01::vf2C() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0xF;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_cure01::vf08() {
    lbl_807C6A40.mE0 = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_magic_cure01::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_cure01::vf24() {
    int result;
    if (lbl_807C68C0.m08 != 0) {
        // inferred: these four zeroed outputs occupy one consecutive stack aggregate.
        struct Results_unk {
            int d;
            int c;
            int b;
            int a;
        } results = {0, 0, 0, 0};
        fn_8001AEDC(0, &results.a, &results.b, &results.c, &results.d);
        lbl_807C6A40.m00 = 1;
        result = 1;
    } else {
        result = -1;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_cure01::vf2C() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_cure01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_guard01::vf08() {
    int result = 1;
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x64;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_magic_guard01::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_guard01::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_guard01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_guard01_2::vf08() {
    int result = 1;
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x64;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_magic_guard01_2::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_guard01_2::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_guard01_2::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_magic_guard02::vf08() {
    int result = 1;
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x64;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_magic_guard02::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_guard02::vf2C() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_magic_guard02::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_np_approach::vf08() {
    lbl_807C6A40.m18 = 0x4B;
    lbl_807C6A40.m1C = 0x4B;
    lbl_807C6A40.m20 = 0x64;
    lbl_807C6A40.m24 = 0xA;
    lbl_807C6A40.m28 = 0;
    lbl_807C6A40.m2C = fn_8001B4A4();
    lbl_807C6A40.m30 = 0x12C;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_np_approach_plus::vf08() {
    lbl_807C6A40.m18 = 0x4B;
    lbl_807C6A40.m1C = 0;
    lbl_807C6A40.m20 = 0x4B;
    lbl_807C6A40.m24 = 0xA;
    lbl_807C6A40.m28 = 0;
    lbl_807C6A40.m2C = fn_8001B4A4();
    lbl_807C6A40.m30 = 0x12C;
    lbl_807C6A40.m00 = 1;
    return 1;
}

// NONMATCHING: 88.31% - local output aggregate and register allocation differ.
// inferred: duplicate script stub of fn_8002B818 at a second address.
int AI::Script::AI_np_follow_player::vf08() {
    int results[4] = {0, 0, 0, 0};
    int value = lbl_807C68C0.m7C;
    results[0] = lbl_807C68C0.m80;
    lbl_807C6A40.m18 = value;
    lbl_807C6A40.m1C = 1;
    lbl_807C6A40.m00 = 1;
    fn_8001AEDC(0, &results[0], &results[1], &results[2], &results[3]);
    return 3;
}

int AI::Script::AI_np_follow_player::vf14() {
    int d = 0;
    int c = 0;
    int b = lbl_807C6A40.m18;
    int a = lbl_807C6B30.m00;
    fn_8001AEDC(0xD, &a, &b, &c, &d);
    lbl_807C6A40.m00 = 1;
    return 6;
}

int AI::Script::AI_np_follow_player::vf20() {
    lbl_807C6A40.m00 = 1;
    return 3;
}

int AI::Script::AI_np_tagger_random::vf08() {
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_np_tagger_random::vf14() {
    if (lbl_807C68C0.m08 != 0) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// inferred: picks a target near the current position (first within 120, then anywhere)
// NONMATCHING: 95.15% - the order of the zero stores to the output arguments differs.
int AI::Script::AI_np_type001::vf08() {
    int base = fn_8001B5B4(lbl_807C68C0.m10, 0);
    int a;
    int b;
    int c;
    int d;
    if ((a = fn_8001BA88(lbl_807C68C0.m00, base, 1, -1, lbl_8088079C, lbl_808807A0, lbl_80880798)) <= 0 &&
        (a = fn_8001BA88(lbl_807C68C0.m00, base, 1, -1, lbl_808807A4, lbl_808807A8, lbl_80880798)) <= 0) {
        a = base;
    }
    d = 0;
    c = 0;
    b = 0;
    fn_8001AEDC(1, &a, &b, &c, &d);
    lbl_807C6A40.m00 = 1;
    return 3;
}

int AI::Script::AI_np_type001::vf14() {
    if (lbl_807C68C0.m08 != 0) {
        int d = 0;
        int c = 0;
        int b = 0;
        int a = 0x5A;
        fn_8001AEDC(0, &a, &b, &c, &d);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// inferred: waits 30-599 frames before the first action
int AI::Script::AI_np_type002::vf08() {
    lbl_807C6A40.m18 = rand() % 570 + 30;
    lbl_807C6A40.m1C = 200;
    lbl_807C6A40.m20 = 100;
    lbl_807C6A40.m24 = 15;
    lbl_807C6A40.m00 = 1;
    return 1;
}

// inferred: same as AI_np_type002::vf08, with two more parameters
int AI::Script::AI_np_type003::vf08() {
    lbl_807C6A40.m18 = rand() % 570 + 30;
    lbl_807C6A40.m1C = 200;
    lbl_807C6A40.m20 = 100;
    lbl_807C6A40.m24 = 15;
    lbl_807C6A40.m28 = 0;
    lbl_807C6A40.m2C = 300;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_np_type004::vf08() {
    lbl_807C6A40.m18 = 0;
    lbl_807C6A40.m00 = 1;
    return 3;
}

// NONMATCHING: 70.77% - argument setup and local register allocation differ.
// inferred: a 0x12-byte helper allocation is passed as the first output word.
int AI::Script::AI_np_type004::vf14() {
    int a = fn_8001AEBC(0x12);
    int b = lbl_807C6A40.m18;
    int c = 0;
    int d = 0;
    fn_8001AEDC(0xD, &a, &b, &c, &d);
    lbl_807C6A40.m00 = 1;
    return 6;
}

int AI::Script::AI_np_type004::vf20() {
    lbl_807C6A40.m00 = 1;
    return 3;
}

int AI::Script::AI_np_type005::vf08() {
    lbl_807C6A40.m18 = 0x1E;
    lbl_807C6A40.m1C = 0x64;
    lbl_807C6A40.m20 = 0x190;
    lbl_807C6A40.m00 = 1;
    return 3;
}

int AI::Script::AI_np_wait_reaction::vf08() {
    lbl_807C6A40.m18 = 0;
    lbl_807C6A40.m1C = 0;
    lbl_807C6A40.m20 = 0x64;
    lbl_807C6A40.m24 = 0;
    lbl_807C6A40.m28 = 1;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_np_wait_reaction_minus::vf08() {
    lbl_807C6A40.m18 = 0;
    lbl_807C6A40.m1C = 0;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = 0;
    lbl_807C6A40.m28 = lbl_807C68C0.m7C;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_np_wait_reaction_minus2::vf08() {
    lbl_807C6A40.m18 = 0;
    lbl_807C6A40.m1C = 0;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = 0;
    lbl_807C6A40.m28 = lbl_807C68C0.m7C;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_np_wait_reaction_plus::vf08() {
    lbl_807C6A40.m18 = 0;
    lbl_807C6A40.m1C = 0;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = 0;
    lbl_807C6A40.m28 = lbl_807C68C0.m7C;
    lbl_807C6A40.m00 = 1;
    return 1;
}

// NONMATCHING: 61.90% - the original loads the 1.5f factor before the input count.
int AI::Script::AI_quark01::vf08() {
    int result = 1;
    lbl_807C6A40.m18 = 240;
    lbl_807C6A40.m1C = (int)(lbl_808807B8 * (float)lbl_807C68C0.m30);
    lbl_807C6A40.m24 = lbl_807C68C0.m7C;
    lbl_807C6A40.m28 = 100;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_quark01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_quark01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_quark02::vf08() {
    lbl_807C6A40.m18 = 0;
    lbl_807C6A40.m1C = lbl_807C68C0.m7C;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = lbl_807C68C0.m84;
    lbl_807C6A40.m28 = lbl_807C68C0.m88;
    lbl_807C6A40.m3C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_quark02::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_silence_guard01::vf08() {
    int result = 1;
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x64;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_silence_guard01::vf18() {
    int result = WaitRandom_unk();
    if (result == 7) {
        lbl_807C6A40.m00 = 1;
        return 7;
    }
    return -1;
}

int AI::Script::AI_silence_guard01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_sword_guard01::vf08() {
    int result;
    lbl_807C6A40.m18 = 0x5A;
    lbl_807C6A40.m24 = 0x3C;
    lbl_807C6A40.m28 = 0x64;
    result = 1;
    lbl_807C6A40.m00 = 1;
    if (fn_8001BEB0(lbl_807C68C0.m00)) {
        result = 7;
    }
    return result;
}

int AI::Script::AI_sword_guard01::vf28() {
    int result = AttackRandom_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_sword_guard01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_sword_leaderguard01::vf08() {
    lbl_807C6A40.m1C = lbl_807C68C0.m7C;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = lbl_807C68C0.m84;
    lbl_807C6A40.m28 = lbl_807C68C0.m88;
    lbl_807C6A40.m3C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_sword_leaderguard01::vf20() {
    int result = AttackValid_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, AttackDelay_unk(), 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_sword_leaderguard01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_sword_leaderguard02::vf08() {
    lbl_807C6A40.m1C = lbl_807C68C0.m7C;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = lbl_807C68C0.m84;
    lbl_807C6A40.m28 = lbl_807C68C0.m88;
    lbl_807C6A40.m3C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_sword_leaderguard02::vf20() {
    int result = AttackValid_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, AttackDelay_unk(), 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_sword_leaderguard02::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 70.00% - state-machine branches and temporary scheduling still differ from MWCC.
// inferred: advances an archer attack action using the current AI script parameters.
int AI::Script::AI_sword_leaderguard02::vf0C() {
    int* script = (int*)&lbl_807C68C0;
    int* action = (int*)&lbl_807C6A40;
    int result;
    if (action[4] >= 1) {
        result = -1;
    } else {
        action[0] = 1;
        result = 1;
    }
    if (result != 1) {
        return -1;
    }
    if (action[0x64 / 4] == 0) {
        action[0x64 / 4] = script[0x58 / 4];
    } else if (action[0x64 / 4] - script[0x58 / 4] > 0) {
        action[0x64 / 4] = script[0x58 / 4];
        action[0] = 1;
        action[0x18 / 4] = 3;
    }

    int state = action[0x18 / 4];
    if (state == 0) {
        CharaTask** candidates = lbl_807C6B30.m08;
        fn_8001A464(candidates);
        fn_8001A65C(candidates);
        fn_8001A788(candidates);
        CharaTask* target = candidates[0];
        lbl_807C6B30.m04 = target;
        if (target != 0) {
            action[0xE0 / 4] = (int)target;
            action[0x18 / 4] = 1;
        } else {
            action[0x18 / 4] = 4;
        }
        action[0] = 1;
        return -1;
    }
    if (state == 1) {
        CharaTask* target = (CharaTask*)action[0xE0 / 4];
        if (fn_8001BE00(target)) {
            int args[4];
            args[3] = 0;
            args[2] = 0;
            args[1] = action[0x1C / 4];
            args[0] = (int)target;
            fn_8001AEDC(9, &args[0], &args[1], &args[2], &args[3]);
            script[0x28 / 4] = (int)target;
            action[0] = 1;
            action[0x18 / 4] = 2;
        } else {
            action[0x18 / 4] = 4;
            action[0] = 1;
        }
        return -1;
    }
    if (state == 2) {
        CharaTask* target = (CharaTask*)script[0x20 / 4];
        CharaTask* current = (CharaTask*)action[0xE0 / 4];
        if (!fn_8001BE00(current)) {
            action[0] = 1;
            action[0x18 / 4] = 0;
            return -1;
        }
        if (fn_8001BE00(target)) {
            int distance = 0;
            if (target != 0 && current != 0) {
                distance = (int)fn_8001AD80(target, current);
            }
            if (distance < action[0x20 / 4]) {
                int args[4];
                args[3] = 0;
                args[2] = 0;
                args[1] = 0;
                args[0] = script[0x20 / 4];
                action[0x18 / 4] = 3;
                action[0] = 1;
                fn_8001AEDC(4, &args[0], &args[1], &args[2], &args[3]);
                return 5;
            }
            int originDistance = 0;
            if (script[0x20 / 4] != 0 && script[0] != 0) {
                originDistance = (int)fn_8001AD80((CharaTask*)script[0], target);
            }
            if ((double)originDistance < lbl_8072FF68 * (double)script[0x30 / 4]) {
                int args[4];
                args[3] = 0;
                args[2] = 0;
                args[1] = 0;
                args[0] = 1;
                fn_8001AEDC(5, &args[0], &args[1], &args[2], &args[3]);
                action[0] = 1;
                action[0x18 / 4] = 3;
                action[0x3C / 4] = action[0x28 / 4];
                return 8;
            }
            return -1;
        }
        return -1;
    }
    if (state == 3) {
        CharaTask* target = (CharaTask*)script[0x20 / 4];
        if (fn_8001BE00(target)) {
            int delay = (int)(lbl_808807AC * (float)script[0x30 / 4]);
            if (target != 0) {
                int fallbackArgs[4];
                int delayArgs[4];
                if (delay != 0) {
                    delayArgs[3] = 0;
                    delayArgs[2] = 0;
                    delayArgs[1] = delay;
                    delayArgs[0] = (int)target;
                    fn_8001AEDC(3, &delayArgs[0], &delayArgs[1], &delayArgs[2], &delayArgs[3]);
                } else {
                    fallbackArgs[3] = 0;
                    fallbackArgs[2] = 0;
                    fallbackArgs[1] = 0x1E;
                    fallbackArgs[0] = (int)target;
                    fn_8001AEDC(3, &fallbackArgs[0], &fallbackArgs[1], &fallbackArgs[2], &fallbackArgs[3]);
                }
            }
            action[0] = 1;
            action[0x18 / 4] = 1;
            return 6;
        }
        action[0x18 / 4] = 0;
        action[0] = 1;
        return -1;
    }
    CharaTask* target = (CharaTask*)script[0x20 / 4];
    if (fn_8001BE00(target) && script[2] != 0) {
        int delay = (int)(lbl_808807AC * (float)script[0x30 / 4]);
        if (target != 0) {
            int fallbackArgs[4];
            int delayArgs[4];
            if (delay != 0) {
                delayArgs[3] = 0;
                delayArgs[2] = 0;
                delayArgs[1] = delay;
                delayArgs[0] = (int)target;
                fn_8001AEDC(3, &delayArgs[0], &delayArgs[1], &delayArgs[2], &delayArgs[3]);
            } else {
                fallbackArgs[3] = 0;
                fallbackArgs[2] = 0;
                fallbackArgs[1] = 0x1E;
                fallbackArgs[0] = (int)target;
                fn_8001AEDC(3, &fallbackArgs[0], &fallbackArgs[1], &fallbackArgs[2], &fallbackArgs[3]);
            }
        }
        action[0] = 1;
        return 6;
    }
    return -1;
}

int AI::Script::AI_sword_leaderguard03::vf08() {
    lbl_807C6A40.m1C = lbl_807C68C0.m7C;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = lbl_807C68C0.m84;
    lbl_807C6A40.m28 = lbl_807C68C0.m88;
    lbl_807C6A40.m30 = lbl_807C68C0.m90;
    lbl_807C6A40.m3C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_sword_leaderguard03::vf20() {
    int result = AttackValid_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, AttackDelay_unk(), 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_sword_leaderguard03::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_sword_pointguard01::vf08() {
    lbl_807C6A40.m1C = lbl_807C68C0.m7C;
    lbl_807C6A40.m20 = lbl_807C68C0.m80;
    lbl_807C6A40.m24 = lbl_807C68C0.m84;
    lbl_807C6A40.m28 = lbl_807C68C0.m88;
    lbl_807C6A40.m2C = lbl_807C68C0.m8C;
    lbl_807C6A40.m3C = 0;
    lbl_807C6A40.m00 = 1;
    return 1;
}

int AI::Script::AI_sword_pointguard01::vf20() {
    int result = AttackValid_unk();
    if (result == 5) {
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    if (result == 1) {
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    if (lbl_807C68C0.m1C < lbl_807C68C0.m30) {
        Command_unk(4, (int)lbl_807C68C0.m20, AttackDelay_unk(), 0, 0);
        lbl_807C6A40.m00 = 1;
        return 5;
    }
    return -1;
}

int AI::Script::AI_sword_pointguard01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

// NONMATCHING: 89.40% - conversion to float and generated constant do not match MWCC's sequence.
int AI::Script::AI_return_guard01::vf08() {
    int count = lbl_807C68C0.m30;
    lbl_807C6A40.m00 = 1;
    lbl_807C6A40.m1C = (int)(lbl_808807B8 * (float)count);
    return 1;
}

int AI::Script::AI_return_guard01::vf40() {
    int result = CheckCount_unk();
    if (result == 1) {
        Command_unk(0, 0, 0, 0, 0);
        lbl_807C6A40.m00 = 1;
        return 1;
    }
    return -1;
}

int AI::Script::AI_return_guard01::vf3C() { return -1; }

// NONMATCHING: 88.17% - the loop setup and register allocation differ from the tree-search body.
// inferred: finds the insertion position for a string key and returns the last visited node.
extern "C" AIScriptMapNode_unk* fn_8003E530(void* tree, const AIString_unk* key,
                                            AIScriptMapNode_unk** parent,
                                            unsigned char* insertLeft,
                                            unsigned char* insertFirst) {
    AIScriptMapNode_unk* node = *(AIScriptMapNode_unk**)((unsigned char*)tree + 4);
    AIScriptMapNode_unk* candidate = (AIScriptMapNode_unk*)((unsigned char*)tree + 4);
    *parent = 0;
    *insertLeft = 1;
    *insertFirst = 1;
    while (node != 0) {
        candidate = node;
        AIString_unk* nodeKey = &node->mKey;
        unsigned int nodeSize;
        const char* nodeData;
        if ((nodeKey->mFlags >> 31) == 0) {
            nodeSize = *(unsigned char*)nodeKey & 0x7F;
            nodeData = (const char*)nodeKey + 1;
        } else {
            nodeSize = nodeKey->mSize;
            nodeData = nodeKey->mData;
        }
        std::string* keyString = (std::string*)key;
        int cmp = keyString->compare(0, keyString->size(), nodeData, nodeSize);
        if (cmp < 0) {
            node = node->mLeft;
            *insertLeft = 1;
        } else {
            *parent = node;
            node = node->mRight;
            *insertLeft = 0;
            *insertFirst = 0;
        }
    }
    return candidate;
}

extern "C" void fn_8003E688() {
    struct ListHead_unk {
        void* first;
        void* last;
        void* end;
    };
    ListHead_unk* head = (ListHead_unk*)lbl_807C6B84;
    head->first = 0;
    head->last = 0;
    head->end = &head->last;
    __register_global_object(head, fn_8003E6B8, lbl_807C6B78);
}
