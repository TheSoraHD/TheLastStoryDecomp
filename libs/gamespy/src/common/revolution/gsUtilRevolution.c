// GameSpy SDK revolution/gsUtilRevolution.c, adapted to the version linked in this game
// (see libs/gamespy/license.txt). Included by gsPlatformUtil.c.

#ifndef GSI_THREADS_IN_OWN_TU
#include "common/revolution/gsThreadRevolution.c"
#endif

long gsiTimeInSec(long* timer) {
    long t = 0;
    t = (gsi_i32)OS_TICKS_TO_SEC(OSGetTime());
    if (timer)
        *timer = t;
    return t;
}
