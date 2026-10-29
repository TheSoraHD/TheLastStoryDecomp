#ifndef STD_IOS_INIT_H
#define STD_IOS_INIT_H

// inferred: MSL <iostream> file statics. Every file that includes it gets a __sinit that
// constructs and registers these two objects (MP/NetGlobals_unk 0x804FF960, MP/CryptMP 0x804AE7C4,
// MP/func_enter_setting 0x80501064, Event/ActionActorPosition 0x8055A440).
#include "std/ios_base.h"

namespace std {

// inferred: same for the wide streams (0x8068BD60, separate library file); real name unknown
class wios_base_Init_unk {
public:
    wios_base_Init_unk();  // 0x8068BD60
    ~wios_base_Init_unk(); // 0x8068C240
};

} // namespace std

static std::ios_base::Init __msl_ios_init_unk;          // inferred name
static std::wios_base_Init_unk __msl_wios_init_unk;     // inferred name

#endif
