#ifndef MSL_CPP_NEW_H
#define MSL_CPP_NEW_H

// placement new, as in MSL <new>
inline void* operator new(unsigned long, void* ptr) { return ptr; }
inline void* operator new[](unsigned long, void* ptr) { return ptr; }

#endif
