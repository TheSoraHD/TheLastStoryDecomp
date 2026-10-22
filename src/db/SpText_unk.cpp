#include "atn/Array_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "db/WeaponParam_unk.h"

extern "C" int atoi(const char* s);
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" int fn_800DC1DC(const wchar_t* s);                       // inferred: wide string to int
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" int fn_8006BA30();                                       // inferred: language

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: entry of the character id table (db/CharaIdTable_unk.cpp)
struct SpCharaId_unk {
    int mId;
    const wchar_t* mName;
    const wchar_t* mShortName;
    const char* m0C;
    const char* m10; // inferred: "pcXXXXXX_YY..." model name
};
extern "C" SpCharaId_unk* fn_80209294(int index);
extern "C" unsigned int fn_802092F8();

extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text (shared by the text tables)

namespace atn {

// inferred: pointer to a string owned elsewhere (see atn/ptr_string_base_wchar_t_0.cpp and
// atn/ptr_string_base_char_0.cpp)
template <class T, int N>
class ptr_string_base {
public:
    ptr_string_base() : mPtr(0) {}
    virtual ~ptr_string_base() {}
    const T* c_str() const { return mPtr != 0 ? mPtr : lbl_808813D0; }

    T* mPtr;
};

} // namespace atn

typedef atn::ptr_string_base<wchar_t, 0> SpString_unk;

// inferred: text of an id (0xC bytes; the constructor and destructor are weak copies)
struct SpTextEntry_unk {
    int mId;
    SpString_unk mText;
};

// inferred: special effect of an item: type (sp_types), value, condition (sp_conds) and its value
struct SpEffect_unk {
    int mType;
    int mValue;
    int mCond;
    int mCondValue;
    int m10; // inferred: percentage
};

// inferred: one row of /boot/sp_text_xx.u16 (0x14 bytes)
struct SpText_unk {
    int mType;
    int mCond;
    int mCondValue; // inferred: 0 matches any value
    const wchar_t* mText;
    int mPercent;
};

static const char sTypesFileNameStr[] = "/boot/sp_types_text_xx.u16";
static const char sCondsFileNameStr[] = "/boot/sp_conds_text_xx.u16";
static const char sTextFileNameStr[] = "/boot/sp_text_xx.u16";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sTypesFileName = sTypesFileNameStr;
static const char* const sCondsFileName = sCondsFileNameStr;
static const char* const sTextFileName = sTextFileNameStr;

// inferred: texts of the condition values (sections 1 to 6 of sp_conds_text)
static atn::Buffer_unk<SpTextEntry_unk> sCondValues1;
static atn::Buffer_unk<SpTextEntry_unk> sCondValues2;
static atn::Buffer_unk<SpTextEntry_unk> sCondValues3;
static atn::Buffer_unk<SpTextEntry_unk> sCondValues4;
static atn::Buffer_unk<SpTextEntry_unk> sCondValues5;
static atn::Buffer_unk<SpTextEntry_unk> sCondValues6;

static SpString_unk* sTypes;
static wchar_t* sTypeTexts;
static SpString_unk* sConds;
static wchar_t* sCondTexts;
static SpText_unk* sTexts;
static int sTextCount;
static wchar_t* sTextTexts;

// inferred: loads /boot/sp_types_text_xx.u16
// NONMATCHING: 96.78% - register numbering (the original keeps one more saved register); the unsigned
// subscript of sTypes fixed part of it
extern "C" void fn_8021B060() {
    atn::CsvU csv;
    sTypes = new (3, "", "", 0) SpString_unk[0x49];
    sConds = new (3, "", "", 0) SpString_unk[0x1B];
    if (sTypeTexts == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sTypesFileName, &size, 0x20);
        int length = 0;
        csv.vf08((const wchar_t*)data, (int)size / 2);
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() != 0) {
                continue;
            }
            csv.fn_8005B9CC();
            length += wcslen(csv.fn_8005B9CC()) + 1;
        }
        sTypeTexts = new (12, "", "", 0) wchar_t[length];
        int pos = 0;
        int i = 0;
        csv.vf08((const wchar_t*)data, (int)size / 2);
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() != 0) {
                continue;
            }
            csv.fn_8005B9CC();
            const wchar_t* text = csv.fn_8005B9CC();
            fn_800DD3FC(sTypeTexts + pos, L"%s", text);
            if (i < 0x49) {
                sTypes[(unsigned int)i].mPtr = sTypeTexts + pos;
            }
            pos += wcslen(text) + 1;
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

#define SP_COUNT_SECTION(values)                                                                                  \
    if (*csv.fn_8005B9CC() != 0) {                                                                                \
        continue;                                                                                                 \
    }                                                                                                             \
    if (*csv.fn_8005B9CC() != 0) {                                                                                \
        length += wcslen(csv.fn_8005B9CC()) + 1;                                                                  \
        count++;                                                                                                  \
    } else {                                                                                                      \
        values.Resize_unk(count, 0);                                                                              \
        count = 0;                                                                                                \
        section++;                                                                                                \
    }

#define SP_READ_SECTION(values)                                                                                   \
    if (*csv.fn_8005B9CC() != 0) {                                                                                \
        continue;                                                                                                 \
    }                                                                                                             \
    const wchar_t* field = csv.fn_8005B9CC();                                                                     \
    if (*field != 0) {                                                                                            \
        int id = fn_800DC1DC(field);                                                                              \
        const wchar_t* text = csv.fn_8005B9CC();                                                                  \
        fn_800DD3FC(sCondTexts + pos, L"%s", text);                                                               \
        values.mData[i].mId = id;                                                                                 \
        values.mData[i].mText.mPtr = sCondTexts + pos;                                                            \
        i++;                                                                                                      \
        pos += wcslen(text) + 1;                                                                                  \
    } else {                                                                                                      \
        i = 0;                                                                                                    \
        part++;                                                                                                     \
    }

// inferred: loads /boot/sp_conds_text_xx.u16 (the names of the conditions, then six sections with
// the texts of their values, separated by empty rows)
// NONMATCHING: 99.49% - register numbering of the counters of the first pass
extern "C" void fn_8021B2F8() {
    atn::CsvU csv;
    if (sCondTexts == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sCondsFileName, &size, 0x20);
        int length = 0;
        int section = 0;
        int count = 0;
        csv.vf08((const wchar_t*)data, (int)size / 2);
        while (csv.fn_8005BBF8() != 0) {
            if (section == 0) {
                if (*csv.fn_8005B9CC() != 0) {
                    continue;
                }
                if (*csv.fn_8005B9CC() != 0) {
                    length += wcslen(csv.fn_8005B9CC()) + 1;
                    count++;
                } else {
                    count = 0;
                    section++;
                }
            } else if (section == 1) {
                SP_COUNT_SECTION(sCondValues1)
            } else if (section == 2) {
                SP_COUNT_SECTION(sCondValues2)
            } else if (section == 3) {
                SP_COUNT_SECTION(sCondValues3)
            } else if (section == 4) {
                SP_COUNT_SECTION(sCondValues4)
            } else if (section == 5) {
                SP_COUNT_SECTION(sCondValues5)
            } else if (section == 6) {
                SP_COUNT_SECTION(sCondValues6)
            } else {
                break;
            }
        }
        if (count > 0) {
            sCondValues6.Resize_unk(count, 0);
        }
        sCondTexts = new (12, "", "", 0) wchar_t[length];
        int pos = 0;
        int part = 0;
        int i = 0;
        csv.vf08((const wchar_t*)data, (int)size / 2);
        while (csv.fn_8005BBF8() != 0) {
            if (part == 0) {
                if (*csv.fn_8005B9CC() != 0) {
                    continue;
                }
                if (*csv.fn_8005B9CC() != 0) {
                    const wchar_t* text = csv.fn_8005B9CC();
                    fn_800DD3FC(sCondTexts + pos, L"%s", text);
                    if (i < 0x1B) {
                        sConds[i].mPtr = sCondTexts + pos;
                    }
                    pos += wcslen(text) + 1;
                    i++;
                } else {
                    i = 0;
                    part++;
                }
            } else if (part == 1) {
                SP_READ_SECTION(sCondValues1)
            } else if (part == 2) {
                SP_READ_SECTION(sCondValues2)
            } else if (part == 3) {
                SP_READ_SECTION(sCondValues3)
            } else if (part == 4) {
                SP_READ_SECTION(sCondValues4)
            } else if (part == 5) {
                SP_READ_SECTION(sCondValues5)
            } else if (part == 6) {
                SP_READ_SECTION(sCondValues6)
            } else {
                break;
            }
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: text of the type of an effect
extern "C" bool fn_8021BE98(wchar_t* buf, SpEffect_unk* effect) {
    if (effect != 0) {
    int type = effect->mType;
    int value = effect->mValue;
    const wchar_t* format = sTypes[type].c_str();
    switch (type) {
    case 3:
    case 67:
        fn_800DD3FC(buf, format, value / 10.0f);
        return true;
    case 4:
    case 5:
    case 7:
    case 8:
    case 10:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 36:
    case 43:
    case 44:
    case 45:
    case 51:
    case 52:
    case 53:
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 66:
    case 69:
    case 70:
    case 71:
        fn_800DD3FC(buf, format, value);
        return true;
    case 63:
        fn_800DD3FC(buf, format, sCondValues1.mData[value].mText.c_str());
        return true;
    default:
        fn_800DD3FC(buf, format, value);
        return true;
    case 0:
        return false;
    }
    }
    return false;
}

// inferred: text of the value of a condition
static inline const wchar_t* FindValue_unk(atn::Buffer_unk<SpTextEntry_unk>& values, int id) {
    for (unsigned int i = 0; i < (unsigned int)values.mSize; i++) {
        if (id == values.mData[i].mId) {
            return values[i].mText.c_str();
        }
    }
    return 0;
}

// inferred: text of a condition and its value
// NONMATCHING: 86.23% - the original reloads the data of the buffer when the value is found and
// numbers the registers of the chara search differently
extern "C" bool fn_8021BFA0(wchar_t* buf, int cond, int value) {
    const wchar_t* format = sConds[cond].c_str();
    switch (cond) {
    case 1:
        buf[0] = 0;
        return true;
    case 4:
        fn_800DD3FC(buf, format, FindValue_unk(sCondValues1, value));
        return true;
    case 5:
        fn_800DD3FC(buf, format, FindValue_unk(sCondValues2, value));
        return true;
    case 6:
        fn_800DD3FC(buf, format, FindValue_unk(sCondValues3, value));
        return true;
    case 7:
        fn_800DD3FC(buf, format, FindValue_unk(sCondValues4, value));
        return true;
    case 8: {
        const wchar_t* text = FindValue_unk(sCondValues5, value);
        if (value == 0) {
            fn_800DD3FC(buf, format, text);
        } else {
            WeaponParamEntry_unk* weapon = fn_80206B14(0, value + 300);
            if (weapon == 0) {
                return false;
            }
            wchar_t name[0x100];
            fn_800DD3FC(name, text, weapon->mName);
            fn_800DD3FC(buf, format, name);
        }
        return true;
    }
    case 15:
        fn_800DD3FC(buf, format, FindValue_unk(sCondValues6, value));
        return true;
    case 20: {
        SpCharaId_unk* chara = 0;
        for (unsigned int i = 0; i < fn_802092F8(); i++) {
            chara = fn_80209294(i);
            const char* model = chara->m10;
            if (strlen(model) >= 13) {
                int b = atoi(model + 8) * 100;
                int a = atoi(chara->m10 + 2) * 10000;
                if (value == atoi(chara->m10 + 11) + (a + b)) {
                    break;
                }
            }
        }
        if (chara != 0) {
            fn_800DD3FC(buf, format, chara->mName);
            return true;
        }
        return false;
    }
    default:
        fn_800DD3FC(buf, format);
        return true;
    case 0:
        return false;
    }
}

// inferred: loads /boot/sp_text_xx.u16 (texts of whole effects)
// NONMATCHING: 99.62% - register numbering (data/position r30/r31 and the length counter); the unsigned
// subscript of sTexts fixed the row offset. Tried: declaration orders of length/size, a length local
extern "C" void fn_8021C3F4() {
    atn::CsvU csv;
    unsigned long size;
    void* data = fn_8046DC5C(lbl_8087F518, sTextFileName, &size, 0x20);
    csv.vf08((const wchar_t*)data, (int)size / 2);
    int length = 0;
    sTextCount = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() != 0) {
            continue;
        }
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        csv.fn_8005B9CC();
        length += wcslen(csv.fn_8005B9CC()) + 1;
        sTextCount++;
    }
    sTextTexts = new (12, "", "", 0) wchar_t[length];
    sTexts = new (3, "", "", 0) SpText_unk[sTextCount];
    int i = 0;
    int pos = 0;
    csv.vf08((const wchar_t*)data, (int)size / 2);
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() != 0) {
            continue;
        }
        int type = fn_800DC1DC(csv.fn_8005B9CC());
        int cond = fn_800DC1DC(csv.fn_8005B9CC());
        int condValue = fn_800DC1DC(csv.fn_8005B9CC());
        const wchar_t* text = csv.fn_8005B9CC();
        fn_800DD3FC(sTextTexts + pos, L"%s", text);
        sTexts[(unsigned int)i].mType = type;
        sTexts[(unsigned int)i].mCond = cond;
        sTexts[(unsigned int)i].mCondValue = condValue;
        sTexts[(unsigned int)i].mText = sTextTexts + pos;
        pos += wcslen(text) + 1;
        sTexts[(unsigned int)i].mPercent = fn_800DC1DC(csv.fn_8005B9CC());
        i++;
    }
    fn_8046DD20(lbl_8087F518, data);
}

// inferred: whole text of a type and condition (0 if none)
static inline SpText_unk* FindText_unk(int type, int cond, int condValue) {
    for (unsigned int i = 0; i < (unsigned int)sTextCount; i++) {
        if (type == sTexts[i].mType && cond == sTexts[i].mCond &&
            (condValue == 0 || condValue == sTexts[i].mCondValue)) {
            return &sTexts[i];
        }
    }
    return 0;
}

// inferred: text of the condition of an effect
static inline bool GetCondText_unk(wchar_t* buf, SpEffect_unk* effect) {
    if (effect != 0) {
        return fn_8021BFA0(buf, effect->mCond, effect->mCondValue);
    }
    return false;
}

// inferred: text of an effect (a whole text of sp_text if there is one, else the texts of its
// condition and type)
// NONMATCHING: 99.94% - order of the wide literals (@wstringBase0: the original has L"" and L"%s %s"
// before the percentage formats, from code that is not reconstructed)
extern "C" bool fn_8021C6A4(wchar_t* buf, SpEffect_unk* effect) {
    if (effect != 0) {
    SpText_unk* text = FindText_unk(effect->mType, effect->mCond, effect->mCondValue);
    if (text != 0 && text->mText[0] != 0) {
        int kind;
        switch (effect->mType) {
        case 3:
        case 67:
            kind = 6;
            break;
        case 5:
        case 32:
        case 33:
        case 66:
        case 68:
            kind = 7;
            break;
        case 4:
        case 7:
        case 8:
        case 10:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 35:
        case 36:
        case 43:
        case 44:
        case 45:
        case 51:
        case 52:
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
        case 58:
        case 59:
        case 69:
        case 71:
            kind = 3;
            break;
        case 63:
            kind = 1;
            break;
        default:
            kind = 2;
            break;
        case 0:
            kind = 0;
            break;
        }
        switch (kind) {
        case 0:
            fn_800DD3FC(buf, text->mText);
            break;
        case 1:
            fn_800DD3FC(buf, text->mText, sCondValues1.mData[effect->mValue].mText.c_str());
            break;
        case 2:
            fn_800DD3FC(buf, text->mText, sTypes[effect->mType].c_str());
            break;
        case 3:
            fn_800DD3FC(buf, text->mText, effect->mValue);
            break;
        case 4:
            fn_800DD3FC(buf, text->mText, effect->mValue / 50 + 1);
            break;
        case 5:
            fn_800DD3FC(buf, text->mText, effect->mValue / 2 + 1);
            break;
        case 6:
            fn_800DD3FC(buf, text->mText, effect->mValue / 10.0f);
            break;
        case 7:
            fn_800DD3FC(buf, text->mText, 1.0f + effect->mValue / 100.0f);
            break;
        }
        if (text->mPercent != 0) {
            if (fn_8006BA30() == 2 || fn_8006BA30() == 3) {
                fn_800DD3FC(buf, L"%s(%d %%)", buf, effect->m10);
            } else {
                fn_800DD3FC(buf, L"%s(%d%%)", buf, effect->m10);
            }
        }
        return true;
    }
    if (effect != 0) {
        wchar_t typeText[0x100];
        wchar_t cond[0x100];
        memset(cond, 0, sizeof(cond));
        memset(typeText, 0, sizeof(typeText));
        if (GetCondText_unk(cond, effect) && fn_8021BE98(typeText, effect)) {
            if (cond[0] != 0) {
                fn_800DD3FC(buf, L"%s %s", cond, typeText);
            } else {
                fn_800DD3FC(buf, L"%s", typeText);
            }
            return true;
        }
    }
    return false;
    }
    return false;
}
