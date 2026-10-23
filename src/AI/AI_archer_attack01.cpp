#include "game/CharaTask.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"

extern "C" {
extern void* lbl_8087EE68;
extern CharaManager<PlayerTask>* lbl_8087F8A0;
extern unsigned char lbl_807C68C0[];
extern unsigned char lbl_807C6A40[];
extern unsigned char lbl_807C6B30[];
int fn_80018078(void* routes, CharaTask* chara);
int fn_80018BAC(void* routes, CharaTask* chara);
void fn_8001C284();
void fn_8001E730();
}

extern "C" int fn_8001BF44() {
    return 0;
}

extern "C" int fn_8001BF4C(CharaTask* chara) {
    return fn_80018078(lbl_8087EE68, chara);
}

extern "C" int fn_8001BF58(CharaTask* chara) {
    return fn_80018BAC(lbl_8087EE68, chara);
}

extern "C" int fn_8001C01C(CharaTask* chara) {
    int ret = 0;
    if (chara != 0) {
        ret = chara->mC58.mE0_12;
    }
    return ret;
}

extern "C" void fn_8001C068(CharaTask* chara, const char* name) {
    if (chara != 0 && name != 0) {
        strncpy((char*)chara->mC58.m1B0, name, 0x100);
    }
}

// NONMATCHING: 78.33% - control flow around the null player manager differs.
extern "C" int fn_8001C038(CharaTask*) {
    int ret = 0;
    if (lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead != 0) {
        ret = lbl_8087F8A0->mHead->mFlag28;
    }
    return ret;
}

extern "C" void fn_8001C1B4() {
    fn_8001C284();
}

extern "C" void fn_8001C1B8() {
    fn_8001E730();
}

extern "C" void fn_8001C1BC() {}

extern "C" unsigned char* fn_8001C260() {
    return lbl_807C68C0;
}

extern "C" unsigned char* fn_8001C26C() {
    return lbl_807C6A40;
}

extern "C" unsigned char* fn_8001C278() {
    return lbl_807C6B30;
}

extern "C" int fn_8001EDF0() { return -1; }
extern "C" int fn_8001EDF8() { return -1; }
extern "C" int fn_8001EE30() { return -1; }
extern "C" int fn_8001EE38() { return -1; }
extern "C" int fn_8001EE40() { return -1; }
extern "C" int fn_8001EE48() { return -1; }
extern "C" int fn_8001EDE8() { return -1; }
extern "C" int fn_8001EE00() { return -1; }
extern "C" int fn_8001EE08() { return -1; }
extern "C" int fn_8001EE10() { return -1; }
extern "C" int fn_8001EE18() { return -1; }
extern "C" int fn_8001EE20() { return -1; }
extern "C" int fn_8001EE28() { return -1; }
extern "C" int fn_8001EE50() { return -1; }
extern "C" int fn_8003CDA8() { return -1; }

// AI/AI_archer_attack01 vtable constructors.
// inferred: factories in fn_8001C284 call these after allocation; each installs its own vtable.
// The class declarations below only model these constructors; object layouts remain unknown.
namespace AI {
namespace Script {
struct AI_archer_attack01 { AI_archer_attack01(); };
struct AI_archer_guard01 { AI_archer_guard01(); };
struct AI_arganan01 { AI_arganan01(); };
struct AI_buffer_attackstable01 { AI_buffer_attackstable01(); };
struct AI_buffer_guard01 { AI_buffer_guard01(); };
struct AI_buffer_guard02 { AI_buffer_guard02(); };
struct AI_buffer_pointguard01 { AI_buffer_pointguard01(); };
struct AI_em_archer_attack02 { AI_em_archer_attack02(); };
struct AI_em_bridge_archer { AI_em_bridge_archer(); };
struct AI_em_monster_attack01 { AI_em_monster_attack01(); };
struct AI_em_scout { AI_em_scout(); };
struct AI_em_sword_attack01 { AI_em_sword_attack01(); };
struct AI_em_sword_attack02 { AI_em_sword_attack02(); };
struct AI_em_sword_pointguard01 { AI_em_sword_pointguard01(); };
struct AI_em_type003 { AI_em_type003(); };
struct AI_em_type010 { AI_em_type010(); };
struct AI_em_type102 { AI_em_type102(); };
struct AI_em_type103 { AI_em_type103(); };
struct AI_em_type104 { AI_em_type104(); };
struct AI_em_type105 { AI_em_type105(); };
struct AI_em_type106 { AI_em_type106(); };
struct AI_fr_archer_pick01 { AI_fr_archer_pick01(); };
struct AI_fr_buffer_pick01 { AI_fr_buffer_pick01(); };
struct AI_fr_follow_player { AI_fr_follow_player(); };
struct AI_fr_magic_pick01 { AI_fr_magic_pick01(); };
struct AI_fr_magic_pick02 { AI_fr_magic_pick02(); };
struct AI_fr_runaway { AI_fr_runaway(); };
struct AI_fr_sword_attack01 { AI_fr_sword_attack01(); };
struct AI_fr_sword_pick01 { AI_fr_sword_pick01(); };
struct AI_fr_sword_pointguard { AI_fr_sword_pointguard(); };
struct AI_fr_sword_pointguard01 { AI_fr_sword_pointguard01(); };
struct AI_fr_type005 { AI_fr_type005(); };
struct AI_fr_warppointescape { AI_fr_warppointescape(); };
struct AI_jackal01 { AI_jackal01(); };
struct AI_magic_attack01 { AI_magic_attack01(); };
struct AI_magic_attack01_2 { AI_magic_attack01_2(); };
struct AI_magic_attack02 { AI_magic_attack02(); };
struct AI_magic_attackstable01 { AI_magic_attackstable01(); };
struct AI_magic_cure01 { AI_magic_cure01(); };
struct AI_magic_guard01 { AI_magic_guard01(); };
struct AI_magic_guard01_2 { AI_magic_guard01_2(); };
struct AI_magic_guard02 { AI_magic_guard02(); };
struct AI_np_approach { AI_np_approach(); };
struct AI_np_approach_plus { AI_np_approach_plus(); };
struct AI_np_follow_player { AI_np_follow_player(); };
struct AI_np_tagger_random { AI_np_tagger_random(); };
struct AI_np_type001 { AI_np_type001(); };
struct AI_np_type002 { AI_np_type002(); };
struct AI_np_type003 { AI_np_type003(); };
struct AI_np_type004 { AI_np_type004(); };
struct AI_np_type005 { AI_np_type005(); };
struct AI_np_wait_reaction { AI_np_wait_reaction(); };
struct AI_np_wait_reaction_minus { AI_np_wait_reaction_minus(); };
struct AI_np_wait_reaction_minus2 { AI_np_wait_reaction_minus2(); };
struct AI_np_wait_reaction_plus { AI_np_wait_reaction_plus(); };
struct AI_quark01 { AI_quark01(); };
struct AI_quark02 { AI_quark02(); };
struct AI_silence_guard01 { AI_silence_guard01(); };
struct AI_sword_guard01 { AI_sword_guard01(); };
struct AI_sword_leaderguard01 { AI_sword_leaderguard01(); };
struct AI_sword_leaderguard02 { AI_sword_leaderguard02(); };
struct AI_sword_leaderguard03 { AI_sword_leaderguard03(); };
struct AI_sword_pointguard01 { AI_sword_pointguard01(); };
struct AI_return_guard01 { AI_return_guard01(); };
}
}

extern "C" {
extern char __vt__Q32AI6Script18AI_archer_attack01[];
extern char __vt__Q32AI6Script17AI_archer_guard01[];
extern char __vt__Q32AI6Script12AI_arganan01[];
extern char __vt__Q32AI6Script24AI_buffer_attackstable01[];
extern char __vt__Q32AI6Script17AI_buffer_guard01[];
extern char __vt__Q32AI6Script17AI_buffer_guard02[];
extern char __vt__Q32AI6Script22AI_buffer_pointguard01[];
extern char __vt__Q32AI6Script21AI_em_archer_attack02[];
extern char __vt__Q32AI6Script19AI_em_bridge_archer[];
extern char __vt__Q32AI6Script22AI_em_monster_attack01[];
extern char __vt__Q32AI6Script11AI_em_scout[];
extern char __vt__Q32AI6Script20AI_em_sword_attack01[];
extern char __vt__Q32AI6Script20AI_em_sword_attack02[];
extern char __vt__Q32AI6Script24AI_em_sword_pointguard01[];
extern char __vt__Q32AI6Script13AI_em_type003[];
extern char __vt__Q32AI6Script13AI_em_type010[];
extern char __vt__Q32AI6Script13AI_em_type102[];
extern char __vt__Q32AI6Script13AI_em_type103[];
extern char __vt__Q32AI6Script13AI_em_type104[];
extern char __vt__Q32AI6Script13AI_em_type105[];
extern char __vt__Q32AI6Script13AI_em_type106[];
extern char __vt__Q32AI6Script19AI_fr_archer_pick01[];
extern char __vt__Q32AI6Script19AI_fr_buffer_pick01[];
extern char __vt__Q32AI6Script19AI_fr_follow_player[];
extern char __vt__Q32AI6Script18AI_fr_magic_pick01[];
extern char __vt__Q32AI6Script18AI_fr_magic_pick02[];
extern char __vt__Q32AI6Script13AI_fr_runaway[];
extern char __vt__Q32AI6Script20AI_fr_sword_attack01[];
extern char __vt__Q32AI6Script18AI_fr_sword_pick01[];
extern char __vt__Q32AI6Script22AI_fr_sword_pointguard[];
extern char __vt__Q32AI6Script24AI_fr_sword_pointguard01[];
extern char __vt__Q32AI6Script13AI_fr_type005[];
extern char __vt__Q32AI6Script21AI_fr_warppointescape[];
extern char __vt__Q32AI6Script11AI_jackal01[];
extern char __vt__Q32AI6Script17AI_magic_attack01[];
extern char __vt__Q32AI6Script19AI_magic_attack01_2[];
extern char __vt__Q32AI6Script17AI_magic_attack02[];
extern char __vt__Q32AI6Script23AI_magic_attackstable01[];
extern char __vt__Q32AI6Script15AI_magic_cure01[];
extern char __vt__Q32AI6Script16AI_magic_guard01[];
extern char __vt__Q32AI6Script18AI_magic_guard01_2[];
extern char __vt__Q32AI6Script16AI_magic_guard02[];
extern char __vt__Q32AI6Script14AI_np_approach[];
extern char __vt__Q32AI6Script19AI_np_approach_plus[];
extern char __vt__Q32AI6Script19AI_np_follow_player[];
extern char __vt__Q32AI6Script19AI_np_tagger_random[];
extern char __vt__Q32AI6Script13AI_np_type001[];
extern char __vt__Q32AI6Script13AI_np_type002[];
extern char __vt__Q32AI6Script13AI_np_type003[];
extern char __vt__Q32AI6Script13AI_np_type004[];
extern char __vt__Q32AI6Script13AI_np_type005[];
extern char __vt__Q32AI6Script19AI_np_wait_reaction[];
extern char __vt__Q32AI6Script25AI_np_wait_reaction_minus[];
extern char __vt__Q32AI6Script26AI_np_wait_reaction_minus2[];
extern char __vt__Q32AI6Script24AI_np_wait_reaction_plus[];
extern char __vt__Q32AI6Script10AI_quark01[];
extern char __vt__Q32AI6Script10AI_quark02[];
extern char __vt__Q32AI6Script18AI_silence_guard01[];
extern char __vt__Q32AI6Script16AI_sword_guard01[];
extern char __vt__Q32AI6Script22AI_sword_leaderguard01[];
extern char __vt__Q32AI6Script22AI_sword_leaderguard02[];
extern char __vt__Q32AI6Script22AI_sword_leaderguard03[];
extern char __vt__Q32AI6Script21AI_sword_pointguard01[];
extern char __vt__Q32AI6Script17AI_return_guard01[];
}

namespace AI {
namespace Script {
AI_archer_attack01::AI_archer_attack01() { *(void**)this = ::__vt__Q32AI6Script18AI_archer_attack01; } // 8001E0A8
AI_archer_guard01::AI_archer_guard01() { *(void**)this = ::__vt__Q32AI6Script17AI_archer_guard01; } // 8001E340
AI_arganan01::AI_arganan01() { *(void**)this = ::__vt__Q32AI6Script12AI_arganan01; } // 8001E350
AI_buffer_attackstable01::AI_buffer_attackstable01() { *(void**)this = ::__vt__Q32AI6Script24AI_buffer_attackstable01; } // 8001E360
AI_buffer_guard01::AI_buffer_guard01() { *(void**)this = ::__vt__Q32AI6Script17AI_buffer_guard01; } // 8001E370
AI_buffer_guard02::AI_buffer_guard02() { *(void**)this = ::__vt__Q32AI6Script17AI_buffer_guard02; } // 8001E380
AI_buffer_pointguard01::AI_buffer_pointguard01() { *(void**)this = ::__vt__Q32AI6Script22AI_buffer_pointguard01; } // 8001E390
AI_em_archer_attack02::AI_em_archer_attack02() { *(void**)this = ::__vt__Q32AI6Script21AI_em_archer_attack02; } // 8001E3A0
AI_em_bridge_archer::AI_em_bridge_archer() { *(void**)this = ::__vt__Q32AI6Script19AI_em_bridge_archer; } // 8001E3B0
AI_em_monster_attack01::AI_em_monster_attack01() { *(void**)this = ::__vt__Q32AI6Script22AI_em_monster_attack01; } // 8001E3C0
AI_em_scout::AI_em_scout() { *(void**)this = ::__vt__Q32AI6Script11AI_em_scout; } // 8001E3D0
AI_em_sword_attack01::AI_em_sword_attack01() { *(void**)this = ::__vt__Q32AI6Script20AI_em_sword_attack01; } // 8001E3E0
AI_em_sword_attack02::AI_em_sword_attack02() { *(void**)this = ::__vt__Q32AI6Script20AI_em_sword_attack02; } // 8001E3F0
AI_em_sword_pointguard01::AI_em_sword_pointguard01() { *(void**)this = ::__vt__Q32AI6Script24AI_em_sword_pointguard01; } // 8001E400
AI_em_type003::AI_em_type003() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type003; } // 8001E410
AI_em_type010::AI_em_type010() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type010; } // 8001E420
AI_em_type102::AI_em_type102() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type102; } // 8001E430
AI_em_type103::AI_em_type103() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type103; } // 8001E440
AI_em_type104::AI_em_type104() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type104; } // 8001E450
AI_em_type105::AI_em_type105() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type105; } // 8001E460
AI_em_type106::AI_em_type106() { *(void**)this = ::__vt__Q32AI6Script13AI_em_type106; } // 8001E470
AI_fr_archer_pick01::AI_fr_archer_pick01() { *(void**)this = ::__vt__Q32AI6Script19AI_fr_archer_pick01; } // 8001E480
AI_fr_buffer_pick01::AI_fr_buffer_pick01() { *(void**)this = ::__vt__Q32AI6Script19AI_fr_buffer_pick01; } // 8001E490
AI_fr_follow_player::AI_fr_follow_player() { *(void**)this = ::__vt__Q32AI6Script19AI_fr_follow_player; } // 8001E4A0
AI_fr_magic_pick01::AI_fr_magic_pick01() { *(void**)this = ::__vt__Q32AI6Script18AI_fr_magic_pick01; } // 8001E4B0
AI_fr_magic_pick02::AI_fr_magic_pick02() { *(void**)this = ::__vt__Q32AI6Script18AI_fr_magic_pick02; } // 8001E4C0
AI_fr_runaway::AI_fr_runaway() { *(void**)this = ::__vt__Q32AI6Script13AI_fr_runaway; } // 8001E4D0
AI_fr_sword_attack01::AI_fr_sword_attack01() { *(void**)this = ::__vt__Q32AI6Script20AI_fr_sword_attack01; } // 8001E4E0
AI_fr_sword_pick01::AI_fr_sword_pick01() { *(void**)this = ::__vt__Q32AI6Script18AI_fr_sword_pick01; } // 8001E4F0
AI_fr_sword_pointguard::AI_fr_sword_pointguard() { *(void**)this = ::__vt__Q32AI6Script22AI_fr_sword_pointguard; } // 8001E500
AI_fr_sword_pointguard01::AI_fr_sword_pointguard01() { *(void**)this = ::__vt__Q32AI6Script24AI_fr_sword_pointguard01; } // 8001E510
AI_fr_type005::AI_fr_type005() { *(void**)this = ::__vt__Q32AI6Script13AI_fr_type005; } // 8001E520
AI_fr_warppointescape::AI_fr_warppointescape() { *(void**)this = ::__vt__Q32AI6Script21AI_fr_warppointescape; } // 8001E530
AI_jackal01::AI_jackal01() { *(void**)this = ::__vt__Q32AI6Script11AI_jackal01; } // 8001E540
AI_magic_attack01::AI_magic_attack01() { *(void**)this = ::__vt__Q32AI6Script17AI_magic_attack01; } // 8001E550
AI_magic_attack01_2::AI_magic_attack01_2() { *(void**)this = ::__vt__Q32AI6Script19AI_magic_attack01_2; } // 8001E560
AI_magic_attack02::AI_magic_attack02() { *(void**)this = ::__vt__Q32AI6Script17AI_magic_attack02; } // 8001E570
AI_magic_attackstable01::AI_magic_attackstable01() { *(void**)this = ::__vt__Q32AI6Script23AI_magic_attackstable01; } // 8001E580
AI_magic_cure01::AI_magic_cure01() { *(void**)this = ::__vt__Q32AI6Script15AI_magic_cure01; } // 8001E590
AI_magic_guard01::AI_magic_guard01() { *(void**)this = ::__vt__Q32AI6Script16AI_magic_guard01; } // 8001E5A0
AI_magic_guard01_2::AI_magic_guard01_2() { *(void**)this = ::__vt__Q32AI6Script18AI_magic_guard01_2; } // 8001E5B0
AI_magic_guard02::AI_magic_guard02() { *(void**)this = ::__vt__Q32AI6Script16AI_magic_guard02; } // 8001E5C0
AI_np_approach::AI_np_approach() { *(void**)this = ::__vt__Q32AI6Script14AI_np_approach; } // 8001E5D0
AI_np_approach_plus::AI_np_approach_plus() { *(void**)this = ::__vt__Q32AI6Script19AI_np_approach_plus; } // 8001E5E0
AI_np_follow_player::AI_np_follow_player() { *(void**)this = ::__vt__Q32AI6Script19AI_np_follow_player; } // 8001E5F0
AI_np_tagger_random::AI_np_tagger_random() { *(void**)this = ::__vt__Q32AI6Script19AI_np_tagger_random; } // 8001E600
AI_np_type001::AI_np_type001() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type001; } // 8001E610
AI_np_type002::AI_np_type002() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type002; } // 8001E620
AI_np_type003::AI_np_type003() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type003; } // 8001E630
AI_np_type004::AI_np_type004() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type004; } // 8001E640
AI_np_type005::AI_np_type005() { *(void**)this = ::__vt__Q32AI6Script13AI_np_type005; } // 8001E650
AI_np_wait_reaction::AI_np_wait_reaction() { *(void**)this = ::__vt__Q32AI6Script19AI_np_wait_reaction; } // 8001E660
AI_np_wait_reaction_minus::AI_np_wait_reaction_minus() { *(void**)this = ::__vt__Q32AI6Script25AI_np_wait_reaction_minus; } // 8001E670
AI_np_wait_reaction_minus2::AI_np_wait_reaction_minus2() { *(void**)this = ::__vt__Q32AI6Script26AI_np_wait_reaction_minus2; } // 8001E680
AI_np_wait_reaction_plus::AI_np_wait_reaction_plus() { *(void**)this = ::__vt__Q32AI6Script24AI_np_wait_reaction_plus; } // 8001E690
AI_quark01::AI_quark01() { *(void**)this = ::__vt__Q32AI6Script10AI_quark01; } // 8001E6A0
AI_quark02::AI_quark02() { *(void**)this = ::__vt__Q32AI6Script10AI_quark02; } // 8001E6B0
AI_silence_guard01::AI_silence_guard01() { *(void**)this = ::__vt__Q32AI6Script18AI_silence_guard01; } // 8001E6C0
AI_sword_guard01::AI_sword_guard01() { *(void**)this = ::__vt__Q32AI6Script16AI_sword_guard01; } // 8001E6D0
AI_sword_leaderguard01::AI_sword_leaderguard01() { *(void**)this = ::__vt__Q32AI6Script22AI_sword_leaderguard01; } // 8001E6E0
AI_sword_leaderguard02::AI_sword_leaderguard02() { *(void**)this = ::__vt__Q32AI6Script22AI_sword_leaderguard02; } // 8001E6F0
AI_sword_leaderguard03::AI_sword_leaderguard03() { *(void**)this = ::__vt__Q32AI6Script22AI_sword_leaderguard03; } // 8001E700
AI_sword_pointguard01::AI_sword_pointguard01() { *(void**)this = ::__vt__Q32AI6Script21AI_sword_pointguard01; } // 8001E710
AI_return_guard01::AI_return_guard01() { *(void**)this = ::__vt__Q32AI6Script17AI_return_guard01; } // 8001E720
}
}
