#include "game/GameSaveBanner.h"
#include "game/GameSaveBackground.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int memcmp(const void* a, const void* b, unsigned long n);
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);
extern "C" void NANDInitBanner(NANDBanner_unk* banner, unsigned int flags, const wchar_t* title,
                               const wchar_t* comment);
extern "C" void fn_803B84DC(char* name); // inferred: name of the save file
extern "C" void* lbl_8087EEC8;            // inferred: font
// inferred: converts a wide string with the font (as in game/CharaTask.cpp)
extern "C" void fn_8006F420(void* font, char* out, const wchar_t* in, int size);

// inferred: the pool also has "banner.bin" after "" (a literal of code removed by the linker)
static const char sPathStr[] = "/savebanner/savebanner_xx.u16";
static const char* const sPath = sPathStr; // inferred: separate .rodata array + .sdata2 pointer

// inferred: copies a wide string unless it is the destination itself
inline void CopyWString_unk(wchar_t* dst, const wchar_t* src) {
    if (src != dst) {
        wmemcpy(dst, src, wcslen(src) + 1);
    }
}

GameSaveBanner* GameSaveBanner::sInstance;

inline GameSaveBanner::GameSaveBanner(atn::Task* parent, GameSaveBackground* background)
    : atn::Task(parent), m48(background), m4C(0), mC0(0) {
    mC4[0] = 0;
    m104[0] = 0;
    m148 = 0;
    m50.vf0C(sPath);
}

GameSaveBanner* GameSaveBanner::fn_803B9044(atn::Task* parent, GameSaveBackground* background) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) GameSaveBanner(parent, background);
    }
    return sInstance;
}

GameSaveBanner::~GameSaveBanner() {
    sInstance = 0;
}

int GameSaveBanner::vf0C() {
    switch (m4C) {
    case 0: {
        if (m50.fn_80473F50() != 0) {
            return 0;
        }
        fn_803B9324();
        char name[0x100];
        int work;
        fn_803B84DC(name);
        fn_800A49C4(lbl_8087EF68, name, (int)&mF200, mC0 * 0x1200 + 0x60A0, 0, &work);
        m4C = 1;
    }
    case 1: {
        int result;
        if (fn_800A4228(lbl_8087EF68, &result) == 0) {
            if (result == 0) {
                m148 = 1;
            }
            m4C = 2;
        } else {
            return 0;
        }
    }
    case 2:
        if (m58.fn_80473F50() != 0) {
            return 0;
        }
        for (int i = 0; i < mC0; i++) {
            if (m60[i].fn_80473F50() != 0) {
                return 0;
            }
        }
        m4C = 3;
    }
    return 1;
}

void GameSaveBanner::fn_803B9324() {
    char path[0x100];
    atn::CsvTabU csv((const wchar_t*)m50.fn_80470580(), (unsigned int)m50.fn_8047059C() / 2);
    do {
        const wchar_t* field = csv.fn_8005B9CC();
        if (*field == L';' || *field == 0) {
            continue;
        }
        if (wcscmp(field, L"Title") == 0) {
            CopyWString_unk(mC4, csv.fn_8005B9CC());
        } else if (wcscmp(field, L"Comment") == 0) {
            CopyWString_unk(m104, csv.fn_8005B9CC());
        } else if (wcscmp(field, L"Banner") == 0) {
            void* font = lbl_8087EEC8;
            fn_8006F420(font, path, csv.fn_8005B9CC(), 0x100);
            m58.vf0C(path);
        } else if (wcscmp(field, L"Icon") == 0) {
            void* font = lbl_8087EEC8;
            fn_8006F420(font, path, csv.fn_8005B9CC(), 0x100);
            m60[mC0].vf0C(path);
            const wchar_t* speed = csv.fn_8005B9CC();
            if (wcscmp(speed, L"FAST") == 0) {
                mA0[mC0] = 1;
            } else if (wcscmp(speed, L"SLOW") == 0) {
                mA0[mC0] = 3;
            } else {
                mA0[mC0] = 2;
            }
            mC0++;
        } else if (wcscmp(field, L"Animation") == 0) {
            if (wcscmp(csv.fn_8005B9CC(), L"BOUNCE") == 0) {
                m144 = 0x10;
            } else {
                m144 = 0;
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

// NONMATCHING: 80.8% - the icon loop of the original uses offsets added to this (i * 8, i * 4,
// i * 0x1200) instead of pointers; the argument order of NANDInitBanner is scheduled differently
void GameSaveBanner::fn_803B9638() {
    if (m4C >= 3) {
        memset(&m160, 0, sizeof(NANDBanner_unk));
        NANDInitBanner(&m160, 0, mC4, m104);
        memcpy(m160.mBannerTexture, m58.fn_80470580(), sizeof(m160.mBannerTexture));
        for (int i = 0; i < mC0; i++) {
            memcpy(m160.mIconTexture[i], m60[i].fn_80470580(), sizeof(m160.mIconTexture[i]));
            m160.mIconSpeed = (m160.mIconSpeed & ~(3 << (i * 2))) | (mA0[i] << (i * 2));
        }
        if (mC0 < 8) {
            m160.mIconSpeed &= ~(3 << (mC0 * 2));
        }
        int size = mC0 * 0x1200 + 0x60A0;
        m160.mFlags |= m144 | 1;
        int changed = 1;
        if (m148 != 0 && memcmp(&mF200, &m160, size) == 0) {
            changed = 0;
        }
        if (changed != 0) {
            char name[0x40];
            fn_803B84DC(name);
            m48->fn_803B8F50(name, &m160, size, 0, 1);
        }
    }
}

void GameSaveBanner::vf10() {}

int GameSaveBanner::fn_803B97C8() {
    return mC0 * 0x1200 + 0x60A0;
}
