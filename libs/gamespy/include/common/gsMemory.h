// GameSpy SDK memory hooks (see libs/gamespy/license.txt).
#ifndef __GSMEMORY_H__
#define __GSMEMORY_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*gsMallocCB)(size_t size);
typedef void (*gsFreeCB)(void* ptr);
typedef void* (*gsReallocCB)(void* ptr, size_t size);
typedef void* (*gsMemalignCB)(size_t boundary, size_t size);

void gsiMemoryCallbacksSet(gsMallocCB p_malloc, gsFreeCB p_free, gsReallocCB p_realloc,
                           gsMemalignCB p_memalign);
void* gsimalloc(size_t size);
void* gsirealloc(void* ptr, size_t size);
void gsifree(void* ptr);
void* gsimemalign(size_t boundary, size_t size);

#ifdef __cplusplus
}
#endif

#endif
