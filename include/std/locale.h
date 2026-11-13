#ifndef STD_LOCALE_H
#define STD_LOCALE_H

// Partial reconstruction of MSL C++ std::locale (no MSL C++ source is available): only what the
// stream code of std/allocator_char uses. The implementation is held in a reference-counted
// pointer (std::tr1, include/std/stdexcept.h): ios_base::getloc (0x800E1D5C) copies two words and
// increments the use count at 0x04 of the control block; the destructor (0x800E0C8C) tests the
// address twice before the release (two levels, as in ~ios_base 0x8068B50C).

#include "std/stdexcept.h"

namespace std {

// inferred: the facet table of a locale (name from __locale_imp::replace in the DOL; defined in
// std/locale_facets_unk.h)
struct __locale_imp;

class locale {
public:
    class facet; // inferred: defined in std/locale_facets_unk.h (atn/EffectManager)
    class id;    // inferred: defined in std/locale_facets_unk.h

    // inferred: a copy of the global locale (inlined in the basic_streambuf constructor, 0x80393BEC;
    // defined in std/istream.h)
    locale();
    locale(const locale& loc) : mImp(loc.mImp) {
        if (mImp.mCntrl != 0) {
            ++mImp.mCntrl->mUseCount;
        }
    }

    static const locale& __global(); // 0x806926D4

    tr1::__shared_ptr_base_unk<__locale_imp> mImp;
};

} // namespace std

#endif
