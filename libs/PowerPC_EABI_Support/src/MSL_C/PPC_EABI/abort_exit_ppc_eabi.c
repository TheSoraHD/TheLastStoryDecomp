#include <signal.h>

void (*__stdio_exit)(void);
BOOL __aborting;

void abort(void) {
    raise(1);
    __aborting = TRUE;
    exit(1);
}

int __register_atexit(void (*func)(void));

int atexit(void (*func)(void)) {
    return __register_atexit(func);
}

//unused
void __atexit(){
}

//unused
void __exit(){
}
