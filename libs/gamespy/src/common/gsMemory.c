// GameSpy SDK gsMemory.c, without the managed memory pool (see libs/gamespy/license.txt).
#include "common/gsCommon.h"

void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
void* calloc(size_t num, size_t size);

typedef struct {
    void* (*malloc)(size_t size);
    void (*free)(void* ptr);
    void* (*realloc)(void* ptr, size_t size);
    void* (*memalign)(size_t boundary, size_t size);
} MemManagerCallbacks;

static void* _gsi_malloc(size_t size) {
    return malloc(size);
}

static void _gsi_free(void* ptr) {
    free(ptr);
}

static void* _gsi_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

// no built in system memalign
static void* _gsi_memalign(size_t boundary, size_t size) {
    void* ptr = calloc((size) / (boundary), (boundary));
    return ptr;
}

static MemManagerCallbacks memmanagercallbacks = {&_gsi_malloc, &_gsi_free, &_gsi_realloc,
                                                  &_gsi_memalign};

void gsiMemoryCallbacksSet(gsMallocCB p_malloc, gsFreeCB p_free, gsReallocCB p_realloc,
                           gsMemalignCB p_memalign) {
    memmanagercallbacks.malloc = p_malloc;
    memmanagercallbacks.free = p_free;
    memmanagercallbacks.realloc = p_realloc;
    memmanagercallbacks.memalign = p_memalign;
}

void* gsimalloc(size_t size) {
    return (*memmanagercallbacks.malloc)(size);
}

void* gsirealloc(void* ptr, size_t size) {
    return (*memmanagercallbacks.realloc)(ptr, size);
}

void gsifree(void* ptr) {
    if (ptr == NULL)
        return;
    (*memmanagercallbacks.free)(ptr);
}
