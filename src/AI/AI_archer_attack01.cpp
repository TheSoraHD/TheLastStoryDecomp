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
