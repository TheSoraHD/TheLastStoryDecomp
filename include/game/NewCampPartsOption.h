#ifndef GAME_NEWCAMPPARTSOPTION_H
#define GAME_NEWCAMPPARTSOPTION_H

#include "game/NewCampPartsBase.h"
#include "atn/LH_Binary.h"
#include "atn/FixedArray.h"
#include "atn/static_string.h"

namespace atn {
class D3Anime;
class D3AnimeInstance;
} // namespace atn

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);

namespace atn {
// inferred: wide version of the assignment (wcslen + wmemcpy, inlined in NewCampPartsOption::fn_80522938)
template <>
inline static_string<32, wchar_t, std::char_traits<wchar_t> >&
static_string<32, wchar_t, std::char_traits<wchar_t> >::operator=(const wchar_t* str) {
    if (str != mStr) {
        wmemcpy(mStr, str, wcslen(str) + 1);
    }
    return *this;
}
} // namespace atn

// inferred: text reference {kind, id} of the option tables (as MenuTextRef_unk, fn_80116EB8)
struct NewCampOptionText_unk {
    int mKind;
    int mId;
};

// inferred: option of the tables (0x18 bytes, 0x8075C428...); names invented
struct NewCampOptionDef_unk {
    NewCampOptionText_unk mName;            // 0x00
    NewCampOptionText_unk mHelp;            // 0x08: inferred: help line (none when mId < 0)
    int mCount;                             // 0x10: number of choices
    const NewCampOptionText_unk* mChoices;  // 0x14
};

// inferred: state of an option (0x20 bytes, constructor 0x80526690); names invented
struct NewCampOptionItem_unk {
    NewCampOptionItem_unk() : mKind(0), mIndex(0), mValue(0), mCount(0), mEnabled(1), mShow(1), mAnime(0), mChoices(0) {}
    // inferred: kinds 10 and 11 are bars (option_bar*.d2a)
    bool IsBar_unk() const { return (unsigned int)(mKind - 10) <= 1; }
    void fn_80526358(int cursor, atn::D3Anime* page); // inferred: places and updates the d2anime
    void SetDef_unk(const NewCampOptionDef_unk* def); // inferred: removed by the linker

    int mKind;   // 0x00: inferred: d2anime of NewCampOptionPage_unk::fn_805266BC
    int mIndex;  // 0x04
    int mValue;  // 0x08
    int mCount;  // 0x0C
    int mEnabled; // 0x10
    int mShow;   // 0x14
    atn::D3AnimeInstance* mAnime; // 0x18
    const NewCampOptionText_unk* mChoices; // 0x1C
};

// inferred: page of options (0xC bytes, constructor 0x8052665C, destructor 0x805226A4); names invented
struct NewCampOptionPage_unk {
    NewCampOptionPage_unk() : mAnime(0), mCount(0) {}
    // inferred: creates the d2anime of option `index` (weak copy 0x805266BC)
    void fn_805266BC(int index, int kind, atn::Task* parent);
    // inferred: inline helpers of the constructor and vf1C (names invented)
    void Load_unk();
    void Show_unk(int show);

    atn::D3Anime* mAnime;
    int mCount;
    ArrayPtr_unk<NewCampOptionItem_unk> mItems;
};

// inferred: texts of the controller panes (0x110 bytes, "database/camp/controller_info_xx.u16");
// names invented
struct NewCampOptionInfo_unk {
    NewCampOptionInfo_unk() { mName[0] = 0; }
    void SetName_unk(const char* name) {
        if (name != mName) {
            memcpy(mName, name, strlen(name) + 1);
        }
    }
    char mName[0x10];
    atn::static_string<32, wchar_t> mText[4];
};

// inferred from __vt__18NewCampPartsOption (0x80793610): options of the camp
class NewCampPartsOption : public NewCampPartsBase {
public:
    static NewCampPartsOption* fn_80521E1C(atn::Task* parent); // inferred: creates the parts
    NewCampPartsOption(atn::Task* parent);                       // 0x80521E80
    virtual ~NewCampPartsOption();                               // 0x805227CC
    virtual void vf10();                                         // 0x805231F0
    virtual void vf14();                                         // 0x805233F8
    virtual void vf1C(NewCampScene_unk* scene);                  // 0x8052287C
    virtual void vf20();                                         // 0x80524820
    virtual void vf24();                                         // 0x80524D50
    virtual void vf28();                                         // 0x80524D9C
    virtual void vf2C();                                         // 0x80524E80
    virtual void vf30();                                         // 0x80524E58
    virtual void vf3C(int show);                                 // 0x80524BFC
    virtual void vf40(void* group);                              // 0x80526134
    virtual void vf48();                                         // 0x80526248
    virtual int vf4C();                                          // 0x80522874
    void fn_80522938();             // inferred: reads the controller texts
    void fn_80522DD8();             // inferred: swaps the texts of the swapped buttons
    void fn_805251CC();             // inferred: stores the options into SetConfig
    void fn_80525498();             // inferred: reads the options from SetConfig
    void fn_805256C4(int mode, int last); // inferred: changes the mode
    void fn_80525800();             // inferred: input of the pages
    void fn_80525A08(int count, int ofs); // inferred: input of a list of options
    void fn_80525F08();             // inferred: input of the button swap
    void SetInfo_unk(atn::D3Anime* anime, int index); // inferred: removed by the linker

    float mD4;    // inferred: scroll of the boards
    int mD8;      // inferred: mode
    int mDC;      // inferred: previous mode
    int mE0;
    int mE4;      // inferred: option cursor
    int mE8;      // inferred: page cursor
    int mEC;
    NewCampOptionPage_unk mPages[6]; // 0xF0
    atn::D3Anime* m138;              // inferred: "option_controller_cursor.d2a"
    atn::D3Anime* mController[2];    // 0x13C
    atn::D3Anime* mDive[2];          // 0x144
    float mDiveAlpha[2];             // 0x14C
    int m154;
    int mSwap[2];                    // 0x158
    float mSwapAlpha[2];             // 0x160
    float mSwapStep[2];              // 0x168
    int m170;                        // inferred: brightness
    atn::Task* m174;                 // inferred: "brightness_base.d2a"
    atn::Task* m178;                 // inferred: "brightness_bar.d2a"
    atn::Task* m17C;                 // inferred: "status_select.d2a"
    float m180;                      // inferred: arrows of status_select
    atn::LH_Binary m184;             // inferred: "database/camp/controller_info_xx.u16"
    atn::FixedArray_unk<NewCampOptionInfo_unk, 18> mInfo; // 0x18C
    atn::Vector3 mMove;              // 0x14B0: inferred: "MoveOffset"
};

#endif
