#include "atn/Bgm.h"

extern "C" void fn_80709AD0(void* sound, int frames);

namespace atn {

Bgm::Bgm() : m04(0), m0C(0), m10(0) {}

Bgm::~Bgm() {
    // inferred: inlined handle stop (cf. nw4r::snd::SoundHandle::Stop)
    if (mSound.mPtr != 0 && mSound.mPtr->mSound != 0) {
        fn_80709AD0(mSound.mPtr->mSound, 0);
    }
    mSound.Reset();
}

} // namespace atn
