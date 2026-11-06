// game/GameEventScript.cpp (0x803935AC-0x80394068): start of the event script unit (GameEventScript,
// boost::bad_lexical_cast). Only the first function is reconstructed so far.
#include "atn/SoundManager.h"
#include "atn/SoundRef.h"

extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);

// inferred: plays a sound with the sound category of the event scripts (5); called from the
// script commands in GameEventScript_cont
extern "C" atn::SoundRef fn_803935AC(const char* name, float volume) {
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34D0 = 5;
    }
    atn::SoundRef ref = fn_800C31F4(name, 0, -1, volume);
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34D0 = 0;
    }
    return ref;
}
