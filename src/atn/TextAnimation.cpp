#include "atn/Task.h"
#include "atn/RenderSetting_unk.h"
#include "std/string.h"
#include "std/vector.h"
#include "std/new.h"
#include "atn/Memory.h"
#include "std/string_members.h"

// atn::TextAnimation (RTTI name): texts that appear character by character (Event/Dialog uses the
// instance lbl_8087F008). Names of the functions are not known; the ones called from other files are
// `extern "C"` with the object as first argument (Event/Dialog declares them that way).

extern "C" RenderSetting_unk* lbl_8087EFA8;
extern "C" double floor(double x);

// inferred: floorf of MSL (float wrapper of floor; weak copy 0x800DAD48)
inline float floorf(float x) {
    return floor(x);
}

namespace atn {

// inferred: parameters of a text (same layout as DialogText_unk of Event/Dialog, plus m44)
struct TextAnimEntry_unk {
    int mSlot;                          // 0x00
    float m04;                          // 0x04: time of the fade in
    std::basic_string<wchar_t> mText;   // 0x08
    float mPos[2];                      // 0x14
    int mAlign;                         // 0x1C: index of the effect function (m90)
    int mKind;                          // 0x20
    int m24;                            // 0x24: flag 4 = other scale
    float mColor[4];                    // 0x28
    float mSize;                        // 0x38
    float m3C;                          // 0x3C: characters per second
    int m40;                            // 0x40: nonzero: appended to the previous text of the slot
    float m44;                          // 0x44: fade in time
};

// inferred: offset, rotation or scale of a character (12 bytes); the default is (0, 0, 1)
struct TextAnimVec_unk {
    TextAnimVec_unk();
    float x, y, z;
};

typedef void (*TextAnimFunc_unk)(TextAnimVec_unk* out, float t);

class TextAnimation : public Task {
public:
    TextAnimation(Task* parent);
    virtual ~TextAnimation();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    std::vector<TextAnimEntry_unk> m48;        // 0x48: texts
    std::vector<int> m54;                      // 0x54: last visible character of each text
    std::vector<std::vector<float> > m60;      // 0x60: time of each character
    std::vector<float> m6C;                    // 0x6C: delay between characters
    std::vector<float> m78;                    // 0x78: duration of the fade of a character
    std::vector<int> m84;                      // 0x84: frames before the text is restarted
    TextAnimFunc_unk m90[4];                   // 0x90: effects selected by TextAnimEntry_unk::mAlign
    int mA0;                                   // 0xA0
    int mA4;                                   // 0xA4: frames before every text is removed (-1: never)
    float mA8;                                 // 0xA8
    float mAC;                                 // 0xAC
};

} // namespace atn

using namespace atn;

// inferred: the instance (created by fn_800D94F8)
extern "C" TextAnimation* lbl_8087F008;

// inferred: draws the text `entry` with `count` visible characters (not decompiled yet)
extern "C" void fn_800D9960(TextAnimation* self, TextAnimEntry_unk* entry, int count, int index);

static void fn_800D98C8(TextAnimVec_unk* out, float t);
static void fn_800D98F4(TextAnimVec_unk* out, float t);
static void fn_800D9918(TextAnimVec_unk* out, float t);
static void fn_800D993C(TextAnimVec_unk* out, float t);

TextAnimation::TextAnimation(Task* parent) : Task(parent), mA0(-1), mA4(-1), mA8(5.0f), mAC(0.2f) {
    m90[0] = fn_800D98C8;
    m90[1] = fn_800D98F4;
    m90[2] = fn_800D9918;
    m90[3] = fn_800D993C;
}

// inferred: returns the instance, created on first use
extern "C" TextAnimation* fn_800D94F8(Task* parent) {
    if (lbl_8087F008 == 0) {
        lbl_8087F008 = new ((MemoryTag)1, "", "", 0) TextAnimation(parent);
    }
    return lbl_8087F008;
}

// inferred: removes every text
static inline void ClearAll_unk(TextAnimation* self) {
    self->m48.clear();
    self->m54.clear();
    self->m60.clear();
    self->m6C.clear();
    self->m78.clear();
    self->m84.clear();
}

// NONMATCHING: 73% - borrador: orden de los bucles y del vector local sin uso (0x8(r1)) por pulir.
// inferred: advances the texts one frame
extern "C" void fn_800D9560(TextAnimation* self) {
    float step = lbl_8087EFA8->m3A4;
    for (int i = 0; i < (int)self->m48.size(); i++) {
        TextAnimEntry_unk& entry = self->m48[i];
        int len = entry.mText.size();
        if (len <= 0) {
            break;
        }
        for (int j = 0; j < self->m54[i] + 1; j++) {
            float& t = self->m60[i][j];
            if (t < self->m78[i]) {
                t = t + step;
            } else {
                t = self->m78[i];
            }
        }
        if (self->m60[i][self->m54[i]] >= self->m6C[i]) {
            self->m54[i] = self->m54[i] + 1;
            if (self->m54[i] >= len) {
                self->m54[i] = len - 1;
            }
        }
        if (entry.m44 < entry.m04) {
            entry.m44 = entry.m44 + step;
        } else {
            entry.m44 = entry.m04;
        }
        if (self->m84[i] > 0) {
            self->m84[i]--;
            if (self->m84[i] <= 0) {
                self->m48[i].m44 = 0.0f;
                self->m54[i] = 0;
                std::vector<float> unused_unk;
                for (int k = 0; k < (int)self->m60[i].size(); k++) {
                    self->m60[i][k] = 0.0f;
                }
                self->m84[i] = 0;
            }
        }
    }
    if (self->mA4 == 0) {
        ClearAll_unk(self);
    }
    if (self->mA4 >= 0) {
        self->mA4--;
    }
}

// inferred: default of the per-character vectors (array constructor of fn_800D9960)
TextAnimVec_unk::TextAnimVec_unk() : x(0.0f), y(0.0f), z(1.0f) {}

static void fn_800D98C8(TextAnimVec_unk* out, float t) {
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = t;
}

static void fn_800D98F4(TextAnimVec_unk* out, float t) {
    out->y = 0.0f;
    out->z = t;
    out->x = -10.0f * (1.0f - t);
}

static void fn_800D9918(TextAnimVec_unk* out, float t) {
    out->y = 0.0f;
    out->z = t;
    out->x = 10.0f * (1.0f - t);
}

static void fn_800D993C(TextAnimVec_unk* out, float t) {
    out->x = 0.0f;
    out->y = 10.0f * (1.0f - t);
    out->z = t;
}

void TextAnimation::vf14() {
    for (int i = 0; i < (int)m48.size(); i++) {
        if (m84[i] == 0) {
            fn_800D9960(this, &m48[i], m54[i] + 1, i);
        }
    }
}

// NONMATCHING: 48.96% - el vaciado de los vectores: el original vacia m48 y m60 (con destructores) y luego
// pone a cero los tamanos de m54, m6C, m78 y m84; aqui clear() sale de otra forma.
// inferred: removes every text now (arg 0) or after `arg` frames
extern "C" void fn_800DAA3C(TextAnimation* self, int arg) {
    if (arg == 0) {
        ClearAll_unk(self);
        self->mA4 = -1;
    } else {
        self->mA4 = arg;
        self->mA0 = arg;
    }
}

// NONMATCHING: 94.66% - borrador: el original usa constantes de .sdata para los push_back (lbl_8087D940/44)
// y fn_800DAD3C para el ultimo elemento; registros.
// inferred: adds a text
extern "C" void fn_800DAB68(TextAnimation* self, TextAnimEntry_unk* text) {
    int restart = 0;
    if (text->m40 != 0) {
        for (int i = 0; i < (int)self->m48.size(); i++) {
            if (self->m48[i].mSlot == text->mSlot) {
                int delay = self->m6C[i];
                restart += (int)self->m78[i] + (self->m48[i].mText.size() - 1) * delay;
            }
        }
    }
    self->m84.push_back(restart);
    self->m48.push_back(*text);
    self->m48[self->m48.size() - 1].m44 = 0.0f;
    self->m54.push_back(0);
    std::vector<float> times;
    for (int i = 0; i < (int)text->mText.size(); i++) {
        times.push_back(0.0f);
    }
    self->m60.push_back(times);
    float speed = text->m3C;
    if (speed <= 0.0f) {
        speed = 1.0f;
    }
    float delay = self->mA8 / speed;
    float fade = floorf((self->mA8 * self->mAC) / speed);
    self->m6C.push_back(fade);
    float d = floorf(delay);
    self->m78.push_back(d);
}

// NONMATCHING: 98.14% - registros (constante cero) y orden de operandos de lfsx.
// inferred: shows every character of the texts at once
extern "C" void fn_800DBC1C(TextAnimation* self) {
    for (int i = 0; i < (int)self->m48.size(); i++) {
        self->m54[i] = self->m48[i].mText.size() - 1;
        self->m84[i] = 0;
        for (int j = 0; j < self->m54[i] + 1; j++) {
            self->m60[i][j] = self->m78[i];
        }
    }
}

// NONMATCHING: 87.33% - el original usa un bucle con ctr (bdnz) y otro orden de cargas.
// inferred: nonzero when the last character of every text has finished its fade
extern "C" int fn_800DBCC8(TextAnimation* self) {
    if (self->m48.size() == 0) {
        return 0;
    }
    for (int i = 0; i < (int)self->m48.size(); i++) {
        if (self->m60[i][self->m48[i].mText.size() - 1] != self->m78[i]) {
            return 0;
        }
    }
    return 1;
}

// NONMATCHING: 69.81% - destructores de los vectores: el original limpia el singleton antes y destruye los
// vectores con el patron de 3 niveles de vector.h; falta comprobar el orden.
TextAnimation::~TextAnimation() {
    lbl_8087F008 = 0;
}

int TextAnimation::vf0C() {
    return 1;
}

void TextAnimation::vf10() {}
