#include "PowerPC_EABI_Support/MetroTRK/targsupp.h"

/* inferred: C version of targsupp.s (the build has no assembler step);
   each stub traps into the debugger. TRKOpenFile and TRKPositionFile have
   no callers: they are force_active in symbols.txt, as a .s file is never
   deadstripped. */

asm ui32 TRKAccessFile(ui32, ui32, ui32*, ui8*){
    nofralloc
    twi 31, r0, 0 /* twui r0, 0 */
    blr
}

asm ui32 TRKOpenFile(ui32, ui32, ui32*, ui8*){
    nofralloc
    twi 31, r0, 0
    blr
}

asm ui32 TRKCloseFile(ui32, ui32){
    nofralloc
    twi 31, r0, 0
    blr
}

asm ui32 TRKPositionFile(ui32, ui32, ui32*, ui8){
    nofralloc
    twi 31, r0, 0
    blr
}
