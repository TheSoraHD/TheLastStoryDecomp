#include "PowerPC_EABI_Support/MetroTRK/targsupp.h"

/* inferred: C version of targsupp.s (the build has no assembler step): four
   asm stubs that trap into the debugger. TRKOpenFile and TRKPositionFile have
   no callers; config.yml keeps them with force_active (a .s is never
   deadstripped). MWCC does not know twui: twi 31, r0, 0 gives the same bytes. */

asm ui32 TRKAccessFile(ui32, ui32, ui32*, ui8*) {
    nofralloc
    twi 31, r0, 0
    blr
}

asm ui32 TRKOpenFile(ui32, ui32, ui32*, ui8*) {
    nofralloc
    twi 31, r0, 0
    blr
}

asm ui32 TRKCloseFile(ui32, ui32) {
    nofralloc
    twi 31, r0, 0
    blr
}

asm ui32 TRKPositionFile(ui32, ui32, ui32*, ui8) {
    nofralloc
    twi 31, r0, 0
    blr
}
