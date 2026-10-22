#include "game/Em314Ivy.h"
#include "atn/Memory.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_80062C8C(void* draw, int segments, unsigned int color, float x, float y, float z, float a,
                            float radius, float b); // inferred: circle
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: sound effects of the ivy
static const char* const sSounds_unk[] = {"SE_EM314_004"};

// inferred: plays sound `id` at `pos`
extern "C" atn::SoundRef fn_80310314(int id, const void* pos) {
    return fn_800C344C(sSounds_unk[id], pos, 0, -1, 1.0f);
}

Em314Ivy::Em314Ivy(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(1.0f), m14D8(info->m14), m14DC(1.0f), m14E0(1e16f), m14E4(0), m14EC(0),
      m14F0(0), m14F4(0), m14F8(0), m14FC(1.0f), m1500(1.0f), m1504(1.0f), m1508(1.0f), m150C(0), m151C(0), m15A0(1), m15A4(0), m15A8(0), m15AC(0), m15B0(-1), m15B4(0), m15B8(0) {
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em314_00_00.csv\0");
    }
    m14B4.vf0C(path);
}

Em314Ivy::~Em314Ivy() {}

void Em314Ivy::vf14() {
    fn_80149A30();
    if (m15B8 != 0) {
        for (unsigned int i = 0; i < m151C; i++) {
            Point_unk* p = m1520[i];
            unsigned int color = 0xFFFFFF00;
            if (i == 0) {
                color = 0xFFFFAA00;
            }
            fn_80062C8C(lbl_8087EEB0, 16, color, p->m04.x, p->m04.y, p->m04.z, 1.0f, p->m14, 5.0f);
        }
    }
}

int Em314Ivy::vf10C(HitMsg_unk* msg) {
    if (m58C == 7 || m58C == 8 || m58C == 10) {
        return 0;
    }
    if ((msg->mData->mAC & 4) != 4 && msg->m44 == 1) {
        return 0;
    }
    msg->m10 *= 1.0f;
    return 1;
}

void Em314Ivy::vfFC() {
    fn_8016E970(2);
    vf100(10);
}

void Em314Ivy::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em314 Ivy[%d]", m58);
    void* group;
    if (m54.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m54.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
    fn_80087994(group, "DebugDisp", &m15B8, 0, 0);
}
