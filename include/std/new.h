#ifndef MSL_CPP_NEW_H
#define MSL_CPP_NEW_H

#include "std/msl_size.h"

// placement new, as in MSL <new>
inline void* operator new(__msl_size_t, void* ptr) { return ptr; }
inline void* operator new[](__msl_size_t, void* ptr) { return ptr; }

#endif
