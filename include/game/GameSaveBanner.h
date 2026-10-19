#ifndef GAME_GAMESAVEBANNER_H
#define GAME_GAMESAVEBANNER_H

#include "atn/Task.h"
#include "atn/LH_Binary.h"

class GameSaveBackground;

// inferred: NANDBanner of the RVL SDK (0xF0A0 bytes)
struct NANDBanner_unk {
    unsigned int mMagic;
    unsigned int mFlags;     // 0x04
    unsigned short mIconSpeed; // 0x08
    unsigned char m0A[0x20 - 0x0A];
    wchar_t mTitle[0x20];    // 0x20
    wchar_t mSubtitle[0x20]; // 0x60
    unsigned char mBannerTexture[0x6000];  // 0xA0
    unsigned char mIconTexture[8][0x1200]; // 0x60A0
};

// inferred from __vt__14GameSaveBanner (0x8078B5E8): builds the banner of the save file from
// "/savebanner/savebanner_xx.u16" (singleton 0x8087F470). Names invented.
class GameSaveBanner : public atn::Task {
public:
    static GameSaveBanner* fn_803B9044(atn::Task* parent, GameSaveBackground* background);

    GameSaveBanner(atn::Task* parent, GameSaveBackground* background);
    virtual ~GameSaveBanner();
    virtual int vf0C();
    virtual void vf10();
    void fn_803B9324(); // inferred: reads the banner description
    void fn_803B9638(); // inferred: writes the banner when it changed
    int fn_803B97C8();  // inferred: size of the banner

    GameSaveBackground* m48;
    int m4C; // inferred: state
    atn::LH_Binary m50;      // inferred: description (UTF-16 tab separated text)
    atn::LH_Binary m58;      // inferred: banner texture
    atn::LH_Binary m60[8];   // inferred: icon textures
    int mA0[8];              // inferred: icon speeds
    int mC0;                 // inferred: number of icons
    wchar_t mC4[0x20];       // inferred: title
    wchar_t m104[0x20];      // inferred: comment
    int m144;                // inferred: animation flags (0x10: bounce)
    int m148;                // inferred: nonzero if the old banner was read
    unsigned char m14C[0x160 - 0x14C];
    NANDBanner_unk m160;
    NANDBanner_unk mF200;    // inferred: banner read from the save file

    static GameSaveBanner* sInstance;
};

#endif
