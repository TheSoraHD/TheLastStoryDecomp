// ssl_mutex.c of RevoEX (koopthekoopa/wii-ipl, CC0): its own translation unit, so ssl.c calls
// these helpers out of line.
#include <revolution/OS.h>
#include <revolution/IPC.h>
#include <revolution/SSL.h>
#include <revolution/version.h>
#include <string.h>

void SSL_InitMutex(OSMutex* mutex_p) {
    OSInitMutex(mutex_p);
}

void SSL_LockMutex(OSMutex* mutex_p) {
    OSLockMutex(mutex_p);
}

void SSL_UnlockMutex(OSMutex* mutex_p) {
    OSUnlockMutex(mutex_p);
}
