#include "atn/D2A_Message.h"
#include "std/string_members.h"
#include "std/vector.h"
#include "atn/DrawHelper.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: functions of the string tables (automatic names)
extern "C" void fn_801F0670(atn::D2A_StringTable_unk* table, int key, std::basic_string<wchar_t>& value, int arg);
extern "C" std::basic_string<wchar_t> fn_801F0758(atn::D2A_StringTable_unk* table, int key);
extern "C" void fn_801FEA9C(atn::D2A_Layout_unk* layout, atn::D2A_TextParam_unk* text); // inferred: unregisters

// inferred: piece of a line of text: a string or an icon (mIcon >= 0)
struct D2A_MsgPiece_unk {
    std::basic_string<wchar_t> mText;
    int mIcon; // 0x0C
};

// inferred: text helpers of other files (automatic names)
extern "C" void fn_800E0000(std::vector<std::basic_string<wchar_t> >* lines, const wchar_t* text, int arg2, int font,
                            void* callback, float maxWidth, float size, float spacing); // inferred: splits the lines
extern "C" void fn_801F1418();
extern "C" float fn_801F14FC(const wchar_t* text, int arg1, int font, float size, float spacing); // inferred: width
extern "C" void fn_801F1658(std::vector<D2A_MsgPiece_unk>* pieces, std::basic_string<wchar_t>* line);

struct D2A_IconRect_unk {
    float x, y, w, h, z;
};
struct D2A_Icons_unk;
extern "C" D2A_Icons_unk* lbl_8087F138;
extern "C" void fn_801F10E0(D2A_Icons_unk* icons, int icon, D2A_IconRect_unk* rect, float alpha);
extern "C" float fn_801F1E7C(int icon, float size); // inferred: width of an icon

struct TextMgr_unk;
extern "C" TextMgr_unk* lbl_8087EEC8;
extern "C" float fn_8006EF48(TextMgr_unk* mgr, const wchar_t* text, int arg2, int font, float size, float spacing);

// inferred: lines of the message being drawn (shared by all messages)
static std::vector<std::basic_string<wchar_t> > sLines;
static bool sLinesReady;

namespace atn {

// inferred: outline parameters filled by D2A_Common::fn_801ED928 (see D2A_Window)
struct D2A_MsgOutline_unk {
    float x;
    float y;
    float r, g, b, a;
};

inline unsigned int MsgToU8_unk(float f) {
    return (f >= 1.0f) ? 255 : (f <= 0.0f) ? 0 : (unsigned int)(f * 255.0f + 0.5f);
}

// NONMATCHING: 66.61% - MWCC inlines fn_801F25E0 here (the original calls it); ((void)0), locals
// and by-value helpers did not change that
D2A_Message::D2A_Message(D2A_Data* owner, unsigned short id)
    : D2A_Common(owner, id), mFont(1), mAlign(0), mLineSpace(2.0f) {
    mType = 1;
    char name[0x20];
    sprintf(name, "string%04d", id);
    mText.mId = fn_800DC6B4(name);
    mText.fn_801F25E0(0, std::basic_string<wchar_t>(L"MESSAGE"), 0, 1);
    mText.mValue[0] = fn_801F0758(&mText.mTables[0], 0);
}

// NONMATCHING: 70.76% - the original copies a short string word 1, word 0, word 2 and keeps the
// copy address in r31 (one more saved register); same copy constructor as std/string.h otherwise
void D2A_TextParam_unk::fn_801F25E0(int key, const std::basic_string<wchar_t>& value, int index, int arg) {
    ((void)0);
    std::basic_string<wchar_t> copy(value);
    fn_801F0670(&mTables[index], key, copy, arg);
}

D2A_Message::~D2A_Message() {
    if (mFC != 0) {
        fn_801FEA9C(&((D2A_Data*)mFC)->m10, &mText);
    }
}

// inferred: registers the text parameter of a message in the layout (name invented; the extra
// inline level gives the register order of the original)
inline void AddMessage_unk(D2A_Layout_unk* layout, D2A_Message* message) {
    layout->mTexts.PushBackRef_unk(message->mText);
}

void D2A_Message::vf0C() {
    fn_801EC784();
    D2A_Data* owner = (D2A_Data*)mFC;
    if (owner != 0) {
        AddMessage_unk(&owner->m10, this);
    }
}

void D2A_Message::vf14() {}

// NONMATCHING: 91.29% - same code shape; register allocation differs (this in r21 instead of r31)
// and the reserve keeps the new storage in a register instead of the buffer
void D2A_Message::vf18() {
    D2A_DrawRect rect;
    D2A_DrawColor color;
    D2A_DrawExtra extra;
    D2A_MsgOutline_unk outline;
    fn_801ED928(&rect, &color, &extra, &outline);
    if (rect.y >= 460.0f) {
        return;
    }
    if (!fn_801EDC78()) {
        return;
    }
    D2A_DrawColor outlineColor;
    outlineColor.r = outline.r;
    outlineColor.g = outline.g;
    outlineColor.b = outline.b;
    outlineColor.a = outline.a;
    D2A_Data* owner = (D2A_Data*)mFC;
    if (owner->m9C != 0) {
        if (m0E == 0) {
            owner->m9C(this, &color, 0);
        } else {
            owner->m9C(this, &color, &outlineColor);
        }
    }
    if (color.a <= 0.0f) {
        return;
    }
    outlineColor.a *= color.a;
    if (!sLinesReady) {
        sLines.reserve(30);
        sLinesReady = true;
    } else {
        sLines.clear();
    }
    fn_800E0000(&sLines, mText.mValue[0].data(), 1, mFont, (void*)fn_801F1418, 100000.0f, rect.w, 0.0f);
    for (unsigned int i = 0; i < sLines.size(); i++) {
        std::basic_string<wchar_t>::size_type pos = sLines[i].rfind(L'\r');
        if (pos != std::basic_string<wchar_t>::npos) {
            sLines[i] = sLines[i].substr(0, pos);
        }
    }
    for (unsigned int i = 0; i < sLines.size(); i++) {
        float x = 0.0f;
        float width = fn_801F14FC(sLines[i].data(), 1, mFont, rect.w, 0.0f);
        if (mAlign == 1) {
            x = 0.5f * -width;
        }
        if (mAlign == 2) {
            x = -width;
        }
        x = rect.x + x;
        std::vector<D2A_MsgPiece_unk> pieces;
        pieces.clear();
        fn_801F1658(&pieces, &sLines[i]);
        for (unsigned int j = 0; j < pieces.size(); j++) {
            D2A_MsgPiece_unk* piece = &pieces[j];
            if (piece->mIcon < 0) {
                const wchar_t* text = piece->mText.data();
                unsigned int oc = (MsgToU8_unk(outlineColor.a) << 24) | (MsgToU8_unk(outlineColor.r) << 16) |
                                  (MsgToU8_unk(outlineColor.g) << 8) | MsgToU8_unk(outlineColor.b);
                int outlined = mOutline;
                unsigned int c = (MsgToU8_unk(color.a) << 24) | (MsgToU8_unk(color.r) << 16) |
                                 (MsgToU8_unk(color.g) << 8) | MsgToU8_unk(color.b);
                lbl_8087EEB0->fn_80061824(x, rect.y, rect.z, rect.w, rect.h, 0.0f, outline.x, outline.y, text, c, 1, mFont, outlined, m0E, oc);
                x += fn_8006EF48(lbl_8087EEC8, text, 1, mFont, rect.w, 0.0f);
            } else {
                D2A_IconRect_unk iconRect;
                iconRect.y = rect.y;
                iconRect.w = rect.w;
                iconRect.h = rect.h;
                iconRect.z = rect.z;
                iconRect.x = x;
                fn_801F10E0(lbl_8087F138, piece->mIcon, &iconRect, 255.0f * color.a);
                x += fn_801F1E7C(piece->mIcon, rect.w);
            }
        }
        rect.y += rect.h + mLineSpace;
    }
}

} // namespace atn
