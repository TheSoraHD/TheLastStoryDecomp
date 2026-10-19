// GameSpy SDK revolution/gsThreadRevolution.c, adapted to the version linked in this game
// (see libs/gamespy/license.txt). Included through gsUtilRevolution.c.

int gsiStartThread(GSThreadFunc aThreadFunc, gsi_u32 theStackSize, void* arg,
                   GSIThreadID* theThreadIdOut) {
    char* aStackBase;

    theThreadIdOut->mStack = gsimalloc(theStackSize);
    aStackBase = (char*)theThreadIdOut->mStack;
    aStackBase += theStackSize;
    OSCreateThread(&theThreadIdOut->mThread, (OSThreadFunc)aThreadFunc, arg, (void*)aStackBase,
                   theStackSize, 16, OS_THREAD_DETACHED);
    OSResumeThread(&theThreadIdOut->mThread);
    return 0;
}

void gsiCancelThread(GSIThreadID theThreadID) {
    OSCancelThread(&theThreadID.mThread);
    if (theThreadID.mStack) {
        gsifree(theThreadID.mStack);
        theThreadID.mStack = NULL;
    }
}

void gsiCleanupThread(GSIThreadID theThreadID) {
    if (!OSIsThreadTerminated(&theThreadID.mThread))
        OSCancelThread(&theThreadID.mThread);
    if (theThreadID.mStack) {
        gsifree(theThreadID.mStack);
        theThreadID.mStack = NULL;
    }
}

void gsiInitializeCriticalSection(GSICriticalSection* theCrit) {
    OSInitMutex(theCrit);
}

void gsiEnterCriticalSection(GSICriticalSection* theCrit) {
    OSLockMutex(theCrit);
}

void gsiLeaveCriticalSection(GSICriticalSection* theCrit) {
    OSUnlockMutex(theCrit);
}
