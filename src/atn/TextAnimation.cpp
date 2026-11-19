#include "atn/Task.h"
#include "atn/RenderSetting_unk.h"
#include "std/string.h"
#include "std/vector.h"
#include "std/new.h"
#include "atn/Memory.h"
#include "std/string_members.h"
#include "atn/DrawHelper.h"

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
    unsigned int mSlot;                 // 0x00
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
    // inferred: weak copy 0x800DAD3C (called by fn_800DAB68 on the last text)
    void ResetTime_unk() { m44 = 0.0f; }
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

static void fn_800D98C8(TextAnimVec_unk* out, float t) {
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = t;
}

// inferred: default of the per-character vectors (array constructor of fn_800D9960)
TextAnimVec_unk::TextAnimVec_unk() : x(0.0f), y(0.0f), z(1.0f) {}

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

namespace atn {
class Font_unk;
}
extern "C" atn::Font_unk* lbl_8087EEC8; // inferred: font of the game texts
extern "C" float fn_8006EF48(void* font, float size, const wchar_t* text, float f2, int a, int b);
extern "C" wchar_t* fn_800DC3C8(wchar_t* s, const wchar_t* delim, wchar_t** context); // inferred: wcstok
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

namespace std {
// inferred: one character in place of the contents (MSL inline, declared in std/string.h)
template <class T, class Tr, class A>
inline basic_string<T, Tr, A>& basic_string<T, Tr, A>::operator=(T c) {
    T* p;
    if (is_long()) {
        mLong.mSize = 1;
        p = mLong.mData;
    } else {
        mShortFlags.mSize = 1;
        p = mShort.mData;
    }
    p[0] = c;
    p[1] = T();
    return *this;
}

} // namespace std

// inferred: array of `mCount` elements allocated with new[] (two locals of fn_800D9960; name invented)
template <class T>
struct TextAnimArray_unk {
    TextAnimArray_unk() : mCount(0), mData(0) {}
    ~TextAnimArray_unk() {
        delete[] mData;
        mData = 0;
        mCount = 0;
    }
    void Resize_unk(int n) {
        delete[] mData;
        mCount = n;
        if (n != 0) {
            mData = new ((MemoryTag)0, "", "", 0) T[n];
        } else {
            mData = 0;
        }
    }
    int mCount;
    T* mData;
};

// inferred: one channel of a color as a byte (rounded)
static inline unsigned int TextAnimByte_unk(float c) {
    if (c >= 1.0f) {
        return 0xFF;
    }
    if (c <= 0.0f) {
        return 0;
    }
    return 255.0f * c + 0.5f;
}

// inferred: RGBA floats as 0xAARRGGBB
static inline unsigned int TextAnimColor_unk(const float* c) {
    unsigned int r = TextAnimByte_unk(c[0]);
    unsigned int g = TextAnimByte_unk(c[1]);
    unsigned int b = TextAnimByte_unk(c[2]);
    unsigned int a = TextAnimByte_unk(c[3]);
    return (a << 24) | (r << 16) | (g << 8) | b;
}

// NONMATCHING: 82.1% - written from the original, not tuned (the effect output is a local without constructor in the
// original)
// inferred: draws the text `entry` with `count` visible characters: each character gets the offset and the
// alpha of the effect m90[mAlign] at its time, an outline (m24 & 1) or a shadow (m24 & 2), and is drawn four
// times when bold (m24 & 4); the lines are separated by "\n" (also written as the two characters "\\n")
extern "C" void fn_800D9960(TextAnimation* self, TextAnimEntry_unk* entry, int count, int index) {
    float shadow;
    float bold = 0.0f;
    if (entry->m24 & 4) {
        shadow = 1.2f;
        bold = 0.1f;
    } else {
        shadow = 1.0f;
    }
    int len = entry->mText.size();
    if (len != 0) {
        TextAnimArray_unk<TextAnimVec_unk> offsets;
        TextAnimArray_unk<float> alphas;
        offsets.Resize_unk(count);
        alphas.Resize_unk(count);
        if (entry->mAlign < 4 && self->m90[entry->mAlign] != 0) {
            for (int i = 0; i < count; i++) {
                float t = self->m60[index][i];
                float f;
                if (t <= 0.0f) {
                    f = 0.0f;
                } else {
                    f = t / self->m78[index];
                    if (!(f <= 1.0f)) {
                        f = 1.0f;
                    }
                }
                TextAnimVec_unk v;
                self->m90[entry->mAlign](&v, f);
                offsets.mData[i] = v;
                alphas.mData[i] = v.z;
                if (self->mA4 >= 0) {
                    alphas.mData[i] *= (float)self->mA4 / (float)self->mA0;
                }
            }
        }
        wchar_t buf[0x100];
        wcscpy(buf, entry->mText.data());
        for (int j = 0; j < len - 1; j++) {
            if (buf[j] == L'\\' && buf[j + 1] == L'n') {
                buf[j + 1] = '\n';
                buf[j] = '\n';
            }
        }
        int line = 0;
        wchar_t* context;
        wchar_t* tok = fn_800DC3C8(buf, L"\n", &context);
        float x = entry->mPos[0];
        while (tok != 0) {
            std::basic_string<wchar_t> text(tok);
            float width = fn_8006EF48(lbl_8087EEC8, entry->mSize, text.data(), 0.0f, 1, 1);
            switch (entry->mKind) {
            case 0:
                x = entry->mPos[0] - 0.5f * width;
                break;
            case 2:
                x = entry->mPos[0] - width;
                break;
            }
            float spacing = 0.0f * (count - 1);
            std::basic_string<wchar_t> done(L"");
            float px;
            for (int j = 0; j < count; j++) {
                float alpha = alphas.mData[j];
                float ox = offsets.mData[j].x;
                float oy = offsets.mData[j].y;
                int a = 255.0f * alpha;
                float color[4];
                color[0] = entry->mColor[0];
                color[1] = entry->mColor[1];
                color[2] = entry->mColor[2];
                color[3] = alpha;
                std::basic_string<wchar_t> ch(L"");
                ch = tok[j];
                switch (entry->mKind) {
                case 0:
                    px = ox + (x - 0.5f * (spacing + fn_8006EF48(lbl_8087EEC8, entry->mSize, ch.data(), 0.0f, 1, 1)));
                    break;
                case 1:
                    px = x + ox;
                    break;
                case 2:
                    px = ox + (x - (entry->mSize + spacing));
                    break;
                }
                px += fn_8006EF48(lbl_8087EEC8, entry->mSize, done.data(), 0.0f, 1, 1);
                float py = oy + (entry->mSize * line + entry->mPos[1]);
                px = 0.0f * j + px;
                done += ch;
                if (entry->m24 & 1) {
                    lbl_8087EEB0->fn_80061824(px - shadow, py - shadow, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f, 0.0f, ch.data(),
                                              a << 24, 1, 1, 0, 0, 0xFF000000);
                    lbl_8087EEB0->fn_80061824(px - shadow, py + shadow, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f,
                                              0.0f, ch.data(), a << 24, 1, 1, 0, 0, 0xFF000000);
                    lbl_8087EEB0->fn_80061824(px + shadow, py + shadow, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f,
                                              0.0f, ch.data(), a << 24, 1, 1, 0, 0, 0xFF000000);
                    lbl_8087EEB0->fn_80061824(px + shadow, py - shadow, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f,
                                              0.0f, ch.data(), a << 24, 1, 1, 0, 0, 0xFF000000);
                } else if (entry->m24 & 2) {
                    lbl_8087EEB0->fn_80061824(px + shadow, py + shadow, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f, 0.0f, ch.data(),
                                              a << 24, 1, 1, 0, 0, 0xFF000000);
                }
                if (entry->m24 & 4) {
                    lbl_8087EEB0->fn_80061824(px - bold, py - bold, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f, 0.0f,
                                              ch.data(), TextAnimColor_unk(color), 1, 1, 0, 0, 0xFF000000);
                    lbl_8087EEB0->fn_80061824(px - bold, py + bold, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f, 0.0f,
                                              ch.data(), TextAnimColor_unk(color), 1, 1, 0, 0, 0xFF000000);
                    lbl_8087EEB0->fn_80061824(px + bold, py + bold, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f, 0.0f,
                                              ch.data(), TextAnimColor_unk(color), 1, 1, 0, 0, 0xFF000000);
                    lbl_8087EEB0->fn_80061824(px + bold, py - bold, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f, 0.0f,
                                              ch.data(), TextAnimColor_unk(color), 1, 1, 0, 0, 0xFF000000);
                } else {
                    lbl_8087EEB0->fn_80061824(px, py, -1.0f, entry->mSize, entry->mSize, 0.0f, 0.0f, 0.0f, ch.data(),
                                              TextAnimColor_unk(color), 1, 1, 0, 0, 0xFF000000);
                }
            }
            line++;
            tok = fn_800DC3C8(0, L"\n", &context);
        }
    }
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

// NONMATCHING: 99.4% - operandos del mullw; el original llama al destructor de vector<float> por la copia
// 0x8000D844, que symbols.txt llama Event::PathLengths_unk::~PathLengths_unk (Event/Puppeteer_CharaTask)
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
    self->m48.back().ResetTime_unk();
    self->m54.push_back(0);
    std::vector<float> times;
    for (int i = 0; i < (int)text->mText.size(); i++) {
        times.push_back(0.0f);
    }
    self->m60.push_back(times);
    float speed = text->m3C;
    if (speed <= 0.0f) {
        speed = 0.1f;
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
