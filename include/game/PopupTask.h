#ifndef GAME_POPUPTASK_H
#define GAME_POPUPTASK_H

#include "atn/FixedArray.h"
#include "atn/Task.h"
#include "atn/static_string.h"
#include "math/Vec.h"

class CharaTask;

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);

namespace atn {
// inferred: the wide string of the popups (as in game/NewCampPartsOption.h; the copy keeps the source
// pointer in a register: a local in the copy assignment)
template <>
inline static_string<32, wchar_t, std::char_traits<wchar_t> >::static_string() {
    mStr[0] = 0;
}
template <>
inline static_string<32, wchar_t, std::char_traits<wchar_t> >::~static_string() {}
template <>
inline static_string<32, wchar_t, std::char_traits<wchar_t> >&
static_string<32, wchar_t, std::char_traits<wchar_t> >::operator=(const wchar_t* str) {
    if (str != mStr) {
        wmemcpy(mStr, str, wcslen(str) + 1);
    }
    return *this;
}
template <>
inline static_string<32, wchar_t, std::char_traits<wchar_t> >&
static_string<32, wchar_t, std::char_traits<wchar_t> >::operator=(const static_string& o) {
    const wchar_t* str = o.mStr;
    if (str != mStr) {
        wmemcpy(mStr, str, wcslen(str) + 1);
    }
    return *this;
}
} // namespace atn

struct PopupD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0xA0 - 0x58]; // 0x58: inferred: layout (as MP::Popup's PopupTask_unk)
    float mLastFrame;                   // 0xA0 (as MP::MpD2A_unk)
    unsigned char mA4[0xFC - 0xA4];
    unsigned int mFlagFC_hi : 1;
    unsigned int mFlagFC : 1;
    unsigned int mFlagFC_29 : 1;
    unsigned int mFlagFC2 : 1;
    unsigned int mFlagFC_lo : 28;
};

// inferred: a d2anime of the popups and its state (0xC bytes; PopupEntry_unk::m9C points to one)
struct PopupAnime_unk {
    int m00;
    PopupD2A_unk* mTask;
    int m08;
};

// inferred: one popup (0xB4 bytes; copied member by member when a popup is removed, PopupTask::vf10)
struct PopupEntry_unk {
    PopupEntry_unk()
        : m00(0), m04(0.0f), m08(0.0f), m0C(0), m10(0), m14(0), m18(0), m1C(1), m60(0.0f, 0.0f, 0.0f),
          m6C(0.0f, 0.0f, 0.0f), m78(0), m7C(-1), m80(-1), m84(0), m88(0), m8C(0), m90(0), m94(0), m98(0), m9C(0),
          mA0(0), mA4(0), mA8(0), mAC(0), mB0(0) {}
    ~PopupEntry_unk() {}
    PopupEntry_unk& operator=(const PopupEntry_unk& o) {
        m00 = o.m00;
        m04 = o.m04;
        m08 = o.m08;
        m0C = o.m0C;
        m10 = o.m10;
        m14 = o.m14;
        m18 = o.m18;
        m1C = o.m1C;
        m20 = o.m20;
        m60 = o.m60;
        m6C = o.m6C;
        m78 = o.m78;
        m7C = o.m7C;
        m80 = o.m80;
        m84 = o.m84;
        m88 = o.m88;
        m8C = o.m8C;
        m90 = o.m90;
        m94 = o.m94;
        m98 = o.m98;
        m9C = o.m9C;
        mA0 = o.mA0;
        mA4 = o.mA4;
        mA8 = o.mA8;
        mAC = o.mAC;
        mB0 = o.mB0;
        return *this;
    }

    int m00;   // inferred: frame
    float m04; // inferred: duration
    float m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    atn::static_string<32, wchar_t, std::char_traits<wchar_t> > m20; // inferred: text
    atn::Vector3 m60;
    atn::Vector3 m6C;
    int m78;
    int m7C;
    int m80;
    int m84;
    int m88;
    unsigned int m8C;
    PopupAnime_unk* m90; // inferred: animes shown with the popup (0 when its d2anime is missing)
    PopupAnime_unk* m94;
    PopupAnime_unk* m98;
    PopupAnime_unk* m9C;
    PopupAnime_unk* mA0;
    PopupAnime_unk* mA4;
    PopupAnime_unk* mA8;
    PopupAnime_unk* mAC;
    int mB0;
};

// inferred: d2anime task (bits 30 and 28 of the word at 0xFC are cleared after the creation)
class PopupTask : public atn::Task {
public:
    PopupTask(atn::Task* parent);
    virtual ~PopupTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    void fn_8054A9A8();
    void fn_8054AF08();
    void fn_8054D794(int kind, const wchar_t* text, const atn::Vector3* pos, int value, unsigned int flags,
                     int combo, int arg7, CharaTask* src, CharaTask* owner, int m1C, int elem, int m80);
    void fn_8054E0FC(PopupEntry_unk* entry);

    // inferred: the anime, or 0 when its d2anime is missing
    PopupAnime_unk* Anime_unk(PopupAnime_unk* anime) { return anime->mTask == 0 ? 0 : anime; }
    // inferred: the level up anime, taken from the other popups
    PopupAnime_unk* TakeLevelUp_unk() {
        if (m17B4.mTask == 0) {
            return 0;
        }
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            if (mEntries.mData[i].mA4 == &m17B4) {
                mEntries.mData[i].mA4 = 0;
            }
        }
        return &m17B4;
    }
    // inferred: the combo anime, taken from the other popups of the owner
    PopupAnime_unk* TakeCombo_unk(CharaTask* owner) {
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            if (mEntries.mData[i].m94 == &m16D0 && (CharaTask*)mEntries.mData[i].m8C == owner) {
                mEntries.mData[i].m94 = 0;
            }
        }
        return &m16D0;
    }
    // inferred: the critical anime, taken from the popups of the same source and owner
    PopupAnime_unk* TakeCritical_unk(CharaTask* src, CharaTask* owner) {
        if (m16DC.mTask == 0) {
            return 0;
        }
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            if (mEntries.mData[i].m90 == &m16DC && (CharaTask*)mEntries.mData[i].m88 == src && (CharaTask*)mEntries.mData[i].m8C == owner) {
                mEntries.mData[i].m90 = 0;
            }
        }
        return &m16DC;
    }
    // inferred: the anime, taken from the popups of the same source and owner
    PopupAnime_unk* TakeAnime_unk(PopupAnime_unk* anime, CharaTask* src, CharaTask* owner) {
        PopupAnime_unk* result;
        if (anime->mTask == 0) {
            result = 0;
        } else {
            result = anime;
            for (unsigned int i = 0; i < mEntries.mSize; i++) {
                if (mEntries.mData[i].m98 == result && (CharaTask*)mEntries.mData[i].m88 == src &&
                    (CharaTask*)mEntries.mData[i].m8C == owner) {
                    mEntries.mData[i].m98 = 0;
                }
                if (mEntries.mData[i].mAC == result && (CharaTask*)mEntries.mData[i].m88 == src &&
                    (CharaTask*)mEntries.mData[i].m8C == owner) {
                    mEntries.mData[i].mAC = 0;
                }
            }
        }
        return result;
    }
    void fn_8054E51C();
    void* fn_8054E528(unsigned int id);

    int m48;
    // inferred: counted array of popups (count at 0x4C, entries at 0x50)
    // (a class of its own over the fixed array: the destructor tests the address twice, 0x8054A950)
    struct Entries_unk : public atn::FixedArray_unk<PopupEntry_unk, 0x20> {} mEntries; // 0x4C
    struct Base_unk {
        unsigned char m00[0x04];
        unsigned int mFlags; // 0x16D4->m00 is a task, see the ctor
    };
    PopupAnime_unk m16D0;     // inferred: combo (its address is the owner tag of the popups, stored in m94)
    PopupAnime_unk m16DC;     // inferred: critical
    PopupAnime_unk m16E8[3];
    PopupAnime_unk m170C[11];
    PopupAnime_unk m1790[3];
    PopupAnime_unk m17B4;
    PopupAnime_unk m17C0;
    PopupAnime_unk m17CC;     // inferred: element hit
};

extern "C" PopupTask* lbl_8087F8A8;

#endif
