#include "PowerPC_EABI_Support/MetroTRK/targsupp.h"

/* inferred: C version of targsupp.s (the build has no assembler step).
   One asm block with entry points, like the .s file: TRKOpenFile and
   TRKPositionFile have no callers and, as separate asm functions, the
   linker would strip them (a .s is never deadstripped). Each stub traps
   into the debugger. */

asm ui32 TRKAccessFile(ui32, ui32, ui32*, ui8*){
    nofralloc
    twi 31, r0, 0 /* twui r0, 0 */
    blr
entry TRKOpenFile
    twi 31, r0, 0
    blr
entry TRKCloseFile
    twi 31, r0, 0
    blr
entry TRKPositionFile
    twi 31, r0, 0
    blr
}
