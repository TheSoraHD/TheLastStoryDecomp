#ifndef GAME_RESETSHUTDOWN_H
#define GAME_RESETSHUTDOWN_H

namespace atn {
class FadeColor;
}

// Power/reset handling of the game (0x30 bytes, singleton lbl_8087F9A0, game/GameSystem_unk.cpp). Class
// name from the type name of the functor bound by main (boost::bind(&ResetShutdown::fn_80570CF8, ...),
// 0x807561F4); member names invented.
class ResetShutdown {
public:
    void fn_80570CF8();          // inferred: per-frame update (also called directly by main)
    void fn_805714DC(int wait);  // inferred: restart request (power)
    void fn_8057155C(int wait);  // inferred: return to the Wii menu
    void fn_805715C4(int wait);  // inferred: restart to the disc channel

    int mPower;      // 0x00: inferred: power button pressed (restart)
    int mReset;      // 0x04: inferred: reset button pressed
    int mWait;       // 0x08: inferred: wait for the save to end
    int mToMenu;     // 0x0C
    int mToDisc;     // 0x10
    int mShutdown;   // 0x14
    atn::FadeColor* mFade; // 0x18
    int mIdle;       // 0x1C: inferred: frames without input
    int mNoFade;     // 0x20
    int mLock;       // 0x24: inferred: requests are delayed while set
    int mPowerReq;   // 0x28
    int mResetReq;   // 0x2C
};

extern "C" ResetShutdown* lbl_8087F9A0; // inferred: the instance (created by fn_80570C50)
extern "C" void fn_80570C50();          // inferred: creates the instance and installs the callbacks

#endif
