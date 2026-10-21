// Rest of the LH_Motion.cpp file (0x804766B0-0x804768EC). It cannot be merged into atn/LH_Motion
// (matching): it starts with the weak copy of Loader::Get_unk<LoadMotion> that LH_Motion.cpp
// emits after LH_Motion::vf0C, which is NONMATCHING in every atn::LH_* file (see atn/LoadManager.h).
// The string pool of the file (0x80755F68, std::string error messages + the "" of `new`) stays here.
#include "atn/LH_Motion.h"
#include "atn/LoadManager.h"
#include "atn/LoadMotion.h"

namespace atn {

// NONMATCHING: weak copy at 0x804766B0 (fn_804766B0), same register difference as the other
// Get_unk copies (see atn/LoadManager.h). Instantiated here so that objdiff can show it.
template LoadMotion* Loader::Get_unk<LoadMotion>(const char* path, int cached);

void* LH_Motion::fn_80476884() {
    if (mCore != 0) {
        unsigned char* data = (unsigned char*)((LoadCore*)mCore)->m54;
        if (data != 0) {
            return data + 0x10;
        }
        return 0;
    }
    return 0;
}

int LH_Motion::fn_804768B4() {
    if (mCore != 0) {
        return ((LoadMotion*)mCore)->m5C;
    }
    return 0;
}

int LH_Motion::fn_804768D0() {
    if (mCore != 0) {
        return ((LoadMotion*)mCore)->m60;
    }
    return 0;
}

} // namespace atn
