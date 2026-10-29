#ifndef STD_LOCALE_H
#define STD_LOCALE_H

// Partial reconstruction of MSL C++ std::locale (no MSL C++ source is available): only what the
// stream code of std/allocator_char uses. The implementation is held in a reference-counted
// pointer (std::tr1, include/std/stdexcept.h): ios_base::getloc (0x800E1D5C) copies two words and
// increments the use count at 0x04 of the control block; the destructor (0x800E0C8C) tests the
// address twice before the release (two levels, as in ~ios_base 0x8068B50C).

#include "std/stdexcept.h"

namespace std {

class locale {
public:
    struct imp; // inferred: name of the implementation type unknown

    locale(const locale& loc) : mImp(loc.mImp) {
        if (mImp.mCntrl != 0) {
            ++mImp.mCntrl->mUseCount;
        }
    }

    tr1::__shared_ptr_base_unk<imp> mImp;
};

} // namespace std

#endif
