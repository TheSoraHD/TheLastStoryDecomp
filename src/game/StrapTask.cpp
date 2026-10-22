#include "game/StrapTask.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/vector.h"
#include "macros.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned char SCGetLanguage();
extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path); // inferred: loads a texture
extern "C" void fn_800D2338(atn::Task* task);                              // inferred: deletes a task

// inferred: fn_800D59B8 is a member of the texture class, declared in include/game/GimmickCommon.h
// under this name (nonzero while the texture is loading)
struct GimmickUvOwner_unk {
    int fn_800D59B8();
};

// inferred: globals of unknown classes
struct Unk_8087F4E8 {
    unsigned char m00[0x88];
    int m88;
    unsigned char m8C[0x90 - 0x8C];
    int m90;
};
extern "C" Unk_8087F4E8* lbl_8087F4E8;
struct Unk_8087F018 {
    unsigned char m0000[0x40E0];
    int m40E0;
};
extern "C" Unk_8087F018* lbl_8087F018;
// inferred: render manager (see atn/LayerReflect.cpp); vf10 reads the frame buffer height
struct StrapRender_unk {
    unsigned char m00[0x0E];
    unsigned short mFbWidth;  // 0x0E
    unsigned short mFbHeight; // 0x10
    unsigned char m12[0x3C - 0x12];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
    int mWide;   // 0x44: inferred: 16:9
};
extern "C" StrapRender_unk* lbl_8087EEE0;
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A5920(void* obj, int a, int b); // inferred: a button was pressed
#include "atn/SequenceBoot_unk.h"
extern "C" void* lbl_8087F9A0;
extern "C" void fn_8057155C(void* obj, int arg);

StrapTask* StrapTask::fn_805A6208(atn::Task* parent) {
    if (parent != 0) {
        return new (1, "", "", 0) StrapTask(parent);
    }
    return 0;
}

StrapTask::StrapTask(atn::Task* parent) : atn::Task(parent), m48(1), m4C(0), m50(0) {
    unsigned char language = SCGetLanguage();
    for (int i = 0; i < 2; i++) {
        char path[0x100];
        if (language == 6) {
            sprintf(path, "/strap/strap%c_du.texture", 'A' + i);
        } else {
            sprintf(path, "/strap/strap%c_xx.texture", 'A' + i);
        }
        fn_800D5908(&m54[i], path);
    }
    lbl_8087F4E8->m88 = 0;
    lbl_8087F4E8->m90 = 0;
    if (lbl_8087F018 != 0) {
        lbl_8087F018->m40E0 = 0;
    }
}

static void fn_set(int v) {
    if (lbl_8087F018 != 0) {
        lbl_8087F018->m40E0 = v;
    }
}
StrapTask::~StrapTask() {
    fn_set(1);
}

int StrapTask::vf0C() {
    int ready = 1;
    for (int i = 0; i < 2; i++) {
        if (((GimmickUvOwner_unk*)&m54[i])->fn_800D59B8() != 0) {
            ready = 0;
        }
    }
    return ready;
}

void StrapTask::vf10() {
    switch (m48) {
    case 0: {
        int done = 0;
        if (++m50 >= 30 && fn_800A5920(lbl_8087EF70, 0, 0) != 0) {
            done = 1;
        }
        if (m50 >= 599) {
            done = 1;
        }
        if (done != 0) {
            m48 = 2;
            m4C = 0;
        }
        break;
    }
    case 1:
        if ((float)(int)lbl_8087EEE0->mFbHeight > 500.0f) {
            m4C = 0;
            m48 = 3;
        }
        if (++m4C >= 15) {
            m48 = 0;
        }
        break;
    case 2:
        if (++m4C >= 15) {
            fn_800D2338(this);
            lbl_8087F4E8->m88 = 1;
            lbl_8087F4E8->m90 = 1;
            sSequenceBoot_unk->SetNext_unk("Title");
        }
        break;
    case 3:
        if (++m4C > 45 && fn_800A5920(lbl_8087EF70, 0, 0) != 0) {
            m4C = 15;
            m48 = 4;
            fn_8057155C(lbl_8087F9A0, 0);
        }
        break;
    }
}

// inferred: message table (lbl_8087F1E4, see game/CharaTask.cpp)
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct StrapMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" StrapMsgTable_unk* lbl_8087F1E4;
extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" void fn_800E0000(std::vector<std::basic_string<wchar_t> >* lines, const wchar_t* text, int arg2, int font,
                            void* callback, float maxWidth, float size, float spacing); // inferred: splits the lines
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width
extern "C" void fn_80061824(void* draw, float x, float y, float z, float w, float h, float f6, float f7, float f8,
                            const wchar_t* text, unsigned int color, int a, int b, int c, int d,
                            unsigned int color2); // inferred: text
extern "C" void fn_8005DFC8(void* renderer, unsigned int color, atn::Texture_unk* tex, int arg, float x, float y,
                            float z, float w, float h); // inferred: draw texture

// inferred: draws a texture of texW x texH over the screen (4:3 or 16:9); name invented
inline float Ratio_unk(int a, int b) {
    return (float)a / (float)b;
}
inline void DrawScreen_unk(atn::Texture_unk* tex, unsigned int color, float texW, float texH) {
    float w = texW;
    float h = texH;
    StrapRender_unk* render = lbl_8087EEE0;
    float scrW = render->mWidth;
    float sx = Ratio_unk(render->mWidth, render->mFbWidth);
    float sy = Ratio_unk(render->mHeight, render->mFbHeight);
    float kx = 1.0f;
    float ky = 1.0f;
    float x0;
    if (render->mWide != 0) {
        sx = 1.0f;
        kx = 1.3333334f;
        x0 = 30.0f;
    } else {
        ky = 0.88f;
        x0 = 3.0f;
    }
    float fx = kx * sx;
    float fy = sy * ky;
    h *= fy;
    float s = (sy * fx) / ky;
    w /= s;
    float x = 0.5f * (scrW * s - texW) + x0;
    fn_8005DFC8(lbl_8087EEB0, color, tex, 1, x, 0.0f, 0.0f, w, h);
}

// NONMATCHING: 85.9% - FPR allocation (alpha in f31 vs f29, constants of the line loop) and the
// scheduling of the screen ratio computations. The colour of a line as a named local fixes the reload of
// the line address after the width call.
// inferred: draws the strap texture (two pages alternating every 150 frames) or the message of states 3-4
void StrapTask::vf14() {
    float alpha = 1.0f;
    int page = (m50 / 150) % 2;
    switch (m48) {
    case 1:
        alpha = m4C / 15.0f;
        break;
    case 2:
        alpha = (15 - m4C) / 15.0f;
        break;
    case 3:
    case 4: {
        alpha = MIN(alpha, m4C / 15.0f);
        std::vector<std::basic_string<wchar_t> > lines;
        fn_800E0000(&lines, lbl_8087F1E4->Get_unk(0x38F), 1, 1, 0, 560.0f, 20.0f, 0.0f);
        for (unsigned int i = 0; i < lines.size(); i++) {
            float w = fn_8006EF48(lbl_8087EEC8, lines[i].c_str(), 20.0f, 0.0f, 1, 1);
            float x = 0.5f * (640.0f - w);
            float y = 30.0f * i + 50.0f;
            unsigned int color = ((unsigned int)(255.0f * alpha) << 24) | 0xFFFFFF;
            fn_80061824(lbl_8087EEB0, x, y, 0.0f, 20.0f, 20.0f, 0.0f, 0.0f, 0.0f,
                        lines[i].c_str(), color, 1, 1, 0, 0, 0xFF000000);
        }
        return;
    }
    }
    DrawScreen_unk(&m54[page], ((unsigned int)(255.0f * alpha) << 24) | 0xFFFFFF, 1024.0f, 512.0f);
}
