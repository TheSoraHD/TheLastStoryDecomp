#ifndef GAME_ERRORMANAGER_H
#define GAME_ERRORMANAGER_H

#include <boost/scoped_ptr.hpp>

namespace atn {
class D2A_Task;
class FadeColor;
}

// Error window of the game (0x4C bytes, instance lbl_8087F420, game/SystemErrorMessage_unk.cpp). Real class
// name ErrorManager (boost::function type names in .rodata, see game/main.cpp); member names invented.
class ErrorManager {
public:
    ErrorManager();
    void fn_80363DBC();      // inferred: per-frame update
    void fn_803646C4();      // inferred: draws the message without the layouts
    void fn_80364BE8();      // inferred: loads the OS font and sets up GX
    void fn_80364DB4(int, int, int, int); // inferred: draws a font cell
    void fn_80364E7C(int); // inferred: shows a message with choices
    void fn_8036503C();      // inferred: moves the cursor of the choices
    void fn_80365320();      // inferred: closes the choices
    // inferred: the message is drawn directly (game/SystemErrorMessage_unk.cpp, game/GameSystem_unk.cpp)
    bool IsPadError_unk() const;
    int IsBusy_unk() {
        int busy = 0;
        if (m18 != 0 || m1C != 0 || mReady == 0) {
            busy = 1;
        }
        return busy;
    }
    // inferred: set around the loading of a map (game/GameTask.cpp); name invented
    void SetLoading_unk(int on) {
        m18 = on;
        m1C = on;
    }

    int mError;    // 0x00
    int mPrev;     // 0x04
    int mDelay;    // 0x08
    int m0C;       // inferred: check the Wii Remote
    int m10;       // inferred: frames before the Wii Remote check
    unsigned int mPaused; // 0x14: inferred: 1 sequence, 2 sound, 4 bgm, 8 movie
    int m18;
    int m1C;
    int m20;       // inferred: stop request (game/GameSystem_unk.cpp)
    atn::D2A_Task* mWindow;  // 0x24
    atn::D2A_Task* mChoice;  // 0x28: inferred: window with four lines
    atn::D2A_Task* mChoice2; // 0x2C: inferred: window with two lines
    atn::D2A_Task* mCursor;  // 0x30
    int mReady;    // 0x34
    int m38;       // inferred: closing the window
    int m3C;       // inferred: closing the choices
    int mSelect;   // 0x40
    atn::FadeColor* mFade; // 0x44
    boost::scoped_ptr<unsigned char> mFont; // 0x48
};
extern "C" ErrorManager* lbl_8087F420;

#endif
