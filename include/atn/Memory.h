#ifndef ATN_MEMORY_H
#define ATN_MEMORY_H

// RSO: global enum MemoryTag (heap of an allocation); enumerator names unknown, so the callers
// cast the heap number: new ((MemoryTag)n, "", "", 0) T(...)
enum MemoryTag {};

// engine allocation operator (0x80084320; RSO __nw__FUl9MemoryTagPCcPCci). Called as
// new ((MemoryTag)heap, name, file, line) T(...); release builds pass "" for both strings and 0 as line.
void* operator new(unsigned long size, MemoryTag heap, const char* name, const char* file, int line);
// array version (0x800846FC; RSO __nwa__FUl9MemoryTagPCcPCci)
void* operator new[](unsigned long size, MemoryTag heap, const char* name, const char* file, int line);

#endif
